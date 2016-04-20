/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativeorganizermodel_p.h"

#include <QtCore/qfile.h>
#include <QtCore/qmath.h>
#include <QtCore/qurl.h>
#include <QtCore/qpointer.h>
#include <QtCore/qtimer.h>

#include <QtQml/qqmlinfo.h>

#include <QtOrganizer/qorganizeritemdetails.h>
#include <QtOrganizer/qorganizeritemrequests.h>
#include <QtOrganizer/qorganizermanager.h>

#include <QtVersitOrganizer/qversitorganizerimporter.h>
#include <QtVersitOrganizer/qversitorganizerexporter.h>

#include "qdeclarativeorganizercollection_p.h"

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

// TODO:
// - Improve handling of itemsModified signal. Instead of fetching all items each time the
//   signal is received, only modified items should be fetched. Item based fetching allows easier
//   use of any item id based caches backends might have.
// - Full update is not needed every time some model property changes. Collections should
//   be updated only if collections have been changed while autoUpdate is off.
// - Changing the time period is by far the most common use case and should be optimized.
//   If new time period overlaps with the old, no need to fetch all the items in new time period.
// - All changes happening during autoUpdate is off should be monitored. If only timePeriod changes
//   there is no need for full update after switching autoUpdate on.


static QString urlToLocalFileName(const QUrl& url)
{
   if (!url.isValid()) {
      return url.toString();
   } else if (url.scheme() == "qrc") {
      return url.toString().remove(0, 5).prepend(':');
   } else {
      return url.toLocalFile();
   }

}

static const char ITEM_TO_SAVE_PROPERTY[] = {"ITEM_TO_SAVE_PROPERTY"};
static const char MANUALLY_TRIGGERED_PROPERTY[] = {"MANUALLY_TRIGGERED"};

class QDeclarativeOrganizerModelPrivate
{
public:
    QDeclarativeOrganizerModelPrivate()
        :m_manager(0),
        m_fetchHint(0),
        m_filter(0),
        m_fetchRequest(0),
        m_occurrenceFetchRequest(0),
        m_reader(0),
        m_writer(0),
        m_startPeriod(QDateTime::currentDateTime()),
        m_endPeriod(QDateTime::currentDateTime()),
        m_error(QOrganizerManager::NoError),
        m_autoUpdate(true),
        m_updatePendingFlag(QDeclarativeOrganizerModelPrivate::NonePending),
        m_componentCompleted(false),
        m_initialUpdate(false),
        m_lastRequestId(0)
    {
    }
    ~QDeclarativeOrganizerModelPrivate()
    {
        if (m_manager)
            delete m_manager;
        delete m_reader;
        delete m_writer;
}

    QList<QDeclarativeOrganizerItem*> m_items;
    QHash<QString, QDeclarativeOrganizerItem *> m_itemIdHash;
    QOrganizerManager* m_manager;
    QDeclarativeOrganizerItemFetchHint* m_fetchHint;
    QList<QOrganizerItemSortOrder> m_sortOrders;
    QList<QDeclarativeOrganizerItemSortOrder*> m_declarativeSortOrders;
    QDeclarativeOrganizerItemFilter* m_filter;
    QOrganizerItemFetchRequest* m_fetchRequest;
    QSet<QOrganizerItemId> m_addedItemIds;
    QMap<QOrganizerAbstractRequest*, QSet<QOrganizerItemId> > m_notifiedItems;
    QOrganizerItemOccurrenceFetchRequest* m_occurrenceFetchRequest;
    QStringList m_importProfiles;
    QVersitReader *m_reader;
    QVersitWriter *m_writer;
    QDateTime m_startPeriod;
    QDateTime m_endPeriod;
    QList<QDeclarativeOrganizerCollection*> m_collections;

    QTimer m_updateTimer;
    QTimer m_updateItemsTimer;
    QTimer m_fetchCollectionsTimer;
    QTimer m_modelChangedTimer;

    QOrganizerManager::Error m_error;

    bool m_autoUpdate;
    enum UpdateTypePending {
        NonePending = 0x0,
        UpdatingItemsPending = 0x1,
        UpdatingCollectionsPending = 0x2
    };
    int m_updatePendingFlag;
    bool m_componentCompleted;
    bool m_initialUpdate;

    QAtomicInt m_lastRequestId;
    QHash<QOrganizerAbstractRequest *, int> m_requestIdHash;
    QUrl m_lastExportUrl;
    QUrl m_lastImportUrl;
};

/*!
    \qmltype OrganizerModel
    \instantiates QDeclarativeOrganizerModel
    \brief The OrganizerModel element provides access to organizer items from the organizer store.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main

    OrganizerModel provides a model of organizer items from the organizer store.
    The contents of the model can be specified with \l filter, \l sortOrders and \l fetchHint properties.
    Whether the model is automatically updated when the store or
    \l{Qt Organizer Overview} {C++ organizer} item changes, can be
    controlled with \l OrganizerModel::autoUpdate property.

    There are two ways of accessing the organizer item data: via the model by using views and delegates,
    or alternatively via \l items list property. Of the two, the model access is preferred.
    Direct list access (i.e. non-model) is not guaranteed to be in order set by \l sortOrder.

    At the moment the model roles provided by OrganizerModel are \c display and \c item.
    Through the \c item role can access any data provided by the OrganizerItem element.


    \note Both the \c startPeriod and \c endPeriod are set by default to the current time (when the OrganizerModel was created).
     In most cases, both (or at least one) of the startPeriod and endPeriod should be set; otherwise, the OrganizerModel will contain
     zero items because the \c startPeriod and \c endPeriod are the same value. For example, if only \c endPeriod is provided,
     the OrganizerModel will contain all items from now (the time of the OrganizerModel's creation) to the \c endPeriod time.

    \sa OrganizerItem, {QOrganizerManager}
*/

/*!
  \qmlsignal OrganizerModel::onModelChanged()

  This signal is emitted, when there are changes in items contained by \l OrganizerModel's data model. Items have either
  been added, removed or modified. This signal is also always emitted during OrganizerModel construction when data model is
  ready for use, even in cases when data model is not having any items in it.
 */
QDeclarativeOrganizerModel::QDeclarativeOrganizerModel(QObject *parent) :
    QAbstractListModel(parent),
    d_ptr(new QDeclarativeOrganizerModelPrivate)
{
    QHash<int, QByteArray> roleNames;
    roleNames = QAbstractItemModel::roleNames();
    roleNames.insert(OrganizerItemRole, "item");
    setRoleNames(roleNames);

    d_ptr->m_updateTimer.setSingleShot(true);
    d_ptr->m_updateItemsTimer.setSingleShot(true);
    d_ptr->m_fetchCollectionsTimer.setSingleShot(true);
    d_ptr->m_modelChangedTimer.setSingleShot(true);
    d_ptr->m_updateTimer.setInterval(1);
    d_ptr->m_updateItemsTimer.setInterval(1);
    d_ptr->m_fetchCollectionsTimer.setInterval(1);
    d_ptr->m_modelChangedTimer.setInterval(1);
    connect(&d_ptr->m_updateTimer, &QTimer::timeout, this, &QDeclarativeOrganizerModel::doUpdate);
    connect(&d_ptr->m_updateItemsTimer, &QTimer::timeout, this, &QDeclarativeOrganizerModel::doUpdateItems);
    connect(&d_ptr->m_fetchCollectionsTimer, &QTimer::timeout, this, &QDeclarativeOrganizerModel::fetchCollections);
    connect(&d_ptr->m_modelChangedTimer, &QTimer::timeout, this, &QDeclarativeOrganizerModel::modelChanged);

    connect(this, &QDeclarativeOrganizerModel::filterChanged, &d_ptr->m_updateItemsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(this, &QDeclarativeOrganizerModel::fetchHintChanged, &d_ptr->m_updateItemsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(this, &QDeclarativeOrganizerModel::sortOrdersChanged, &d_ptr->m_updateItemsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(this, &QDeclarativeOrganizerModel::startPeriodChanged, &d_ptr->m_updateItemsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(this, &QDeclarativeOrganizerModel::endPeriodChanged, &d_ptr->m_updateItemsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
}

QDeclarativeOrganizerModel::~QDeclarativeOrganizerModel()
{
}

/*!
  \qmlproperty string OrganizerModel::manager

  This property holds the manager name or manager uri of the organizer backend engine.
  The manager uri format: qtorganizer:<managerid>:<key>=<value>&<key>=<value>.

  For example, memory organizer engine has an optional id parameter, if user want to
  share the same memory engine with multiple OrganizerModel instances, the manager property
  should declared like this:
  \code
    model : OrganizerModel {
       manager:"qtorganizer:memory:id=organizer1
    }
  \endcode

  instead of just the manager name:
  \code
    model : OrganizerModel {
       manager:"memory"
    }
  \endcode

  \sa QOrganizerManager::fromUri()
  */
QString QDeclarativeOrganizerModel::manager() const
{
    Q_D(const QDeclarativeOrganizerModel);
    if (d->m_manager)
        return d->m_manager->managerUri();
    return QString();
}

/*!
  \qmlproperty string OrganizerModel::managerName

  This property holds the manager name of the organizer backend engine.
  This property is read only.
  \sa QOrganizerManager::fromUri()
  */
QString QDeclarativeOrganizerModel::managerName() const
{
    Q_D(const QDeclarativeOrganizerModel);
    if (d->m_manager)
        return d->m_manager->managerName();
    return QString();
}

/*!
  \qmlproperty list<string> OrganizerModel::availableManagers

  This property holds the list of available manager names.
  This property is read only.
  */
QStringList QDeclarativeOrganizerModel::availableManagers() const
{
    return QOrganizerManager::availableManagers();
}

/*!
  \qmlproperty bool OrganizerModel::autoUpdate

  This property indicates whether or not the organizer model should be updated automatically, default value is true.

  \sa OrganizerModel::update()
  */
void QDeclarativeOrganizerModel::setAutoUpdate(bool autoUpdate)
{
    Q_D(QDeclarativeOrganizerModel);
    if (autoUpdate == d->m_autoUpdate)
        return;
    d->m_autoUpdate = autoUpdate;
    emit autoUpdateChanged();
}

bool QDeclarativeOrganizerModel::autoUpdate() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_autoUpdate;
}

/*!
  \qmlmethod OrganizerModel::update()

  Manually update the organizer model content including both
  items and collections.

  \sa OrganizerModel::updateItems
  \sa OrganizerModel::updateCollections
  \sa OrganizerModel::autoUpdate
  */
void QDeclarativeOrganizerModel::update()
{
    Q_D(QDeclarativeOrganizerModel);
    if (!d->m_componentCompleted || d->m_updatePendingFlag)
        return;

    // Disallow possible duplicate request triggering
    d->m_updatePendingFlag = (QDeclarativeOrganizerModelPrivate::UpdatingItemsPending | QDeclarativeOrganizerModelPrivate::UpdatingCollectionsPending);
    d->m_fetchCollectionsTimer.setProperty(MANUALLY_TRIGGERED_PROPERTY, QVariant::fromValue<bool>(true));
    d->m_fetchCollectionsTimer.start();
}

void QDeclarativeOrganizerModel::doUpdate()
{
    Q_D(QDeclarativeOrganizerModel);
    if (d->m_autoUpdate)
        update();
}

void QDeclarativeOrganizerModel::doUpdateItems()
{
    Q_D(QDeclarativeOrganizerModel);
    if (d->m_autoUpdate)
        updateItems();
}

/*!
  \qmlmethod OrganizerModel::updateItems()

  Manually update the organizer model items.

  \sa OrganizerModel::update
  \sa OrganizerModel::updateCollections
  \sa OrganizerModel::autoUpdate
  */
void QDeclarativeOrganizerModel::updateItems()
{
    Q_D(QDeclarativeOrganizerModel);
    if (!d->m_componentCompleted || d->m_updatePendingFlag)
        return;
    d->m_updatePendingFlag = QDeclarativeOrganizerModelPrivate::UpdatingItemsPending;// Disallow possible duplicate request triggering
    QMetaObject::invokeMethod(this, "fetchAgain", Qt::QueuedConnection);
}

/*!
  \qmlmethod OrganizerModel::updateCollections()

  Manually update the organizer model collections.

  \sa OrganizerModel::update
  \sa OrganizerModel::updateItems
  \sa OrganizerModel::autoUpdate
  */
void QDeclarativeOrganizerModel::updateCollections()
{
    Q_D(QDeclarativeOrganizerModel);
    if (!d->m_componentCompleted || d->m_updatePendingFlag)
        return;
    d->m_updatePendingFlag = QDeclarativeOrganizerModelPrivate::UpdatingCollectionsPending;// Disallow possible duplicate request triggering
    d->m_fetchCollectionsTimer.setProperty(MANUALLY_TRIGGERED_PROPERTY, QVariant::fromValue<bool>(true));
    d->m_fetchCollectionsTimer.start();
}

/*!
  \qmlmethod OrganizerModel::cancelUpdate()

  Cancel the running organizer model content update request.

  \sa OrganizerModel::autoUpdate, OrganizerModel::update
  */
void QDeclarativeOrganizerModel::cancelUpdate()
{
    Q_D(QDeclarativeOrganizerModel);
    if (d->m_fetchRequest) {
        d->m_fetchRequest->cancel();
        d->m_fetchRequest->deleteLater();
        d->m_fetchRequest = 0;
        d->m_updatePendingFlag = QDeclarativeOrganizerModelPrivate::NonePending;
    }
}
/*!
  \qmlproperty date OrganizerModel::startPeriod

  This property holds the start date and time period used by the organizer model to fetch organizer items.
  The default value is the datetime of OrganizerModel creation.
  */
QDateTime QDeclarativeOrganizerModel::startPeriod() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_startPeriod;
}
void QDeclarativeOrganizerModel::setStartPeriod(const QDateTime& start)
{
    Q_D(QDeclarativeOrganizerModel);
    if (start != d->m_startPeriod) {
        d->m_startPeriod = start;
        emit startPeriodChanged();
    }
}

/*!
  \qmlproperty date OrganizerModel::endPeriod

  This property holds the end date and time period used by the organizer model to fetch organizer items.
  The default value is the datetime of OrganizerModel creation.
  */
QDateTime QDeclarativeOrganizerModel::endPeriod() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_endPeriod;
}
void QDeclarativeOrganizerModel::setEndPeriod(const QDateTime& end)
{
    Q_D(QDeclarativeOrganizerModel);
    if (end != d->m_endPeriod) {
        d->m_endPeriod = end;
        emit endPeriodChanged();
    }
}

/*!
  \qmlproperty enumeration OrganizerModel::ImportError

  Defines the errors cases for \l OrganizerModel::importItems() -function.

  \list
  \li OrganizerModel::ImportNoError             Completed successfully, no error.
  \li OrganizerModel::ImportUnspecifiedError    Unspecified error.
  \li OrganizerModel::ImportIOError             Input/output error.
  \li OrganizerModel::ImportOutOfMemoryError    Out of memory error.
  \li OrganizerModel::ImportNotReadyError       Not ready for importing. Only one import operation can be active at a time.
  \li OrganizerModel::ImportParseError          Error during parsing.
  \endlist
*/

/*!
  \qmlsignal OrganizerModel::onImportCompleted(ImportError error, URL url, list<string> ids)

  This signal is emitted, when \l OrganizerModel::importItems() completes. The success of operation
  can be seen on \a error which is defined in \l OrganizerModel::ImportError. \a url indicates the
  file, which was imported. \a ids contains the imported items ids.

  If the operation was successful, items are now imported to backend. If \l OrganizerModel::autoUpdate
  is enabled, \l OrganizerModel::modelChanged will be emitted when imported items are also visible on
  \l OrganizerModel's data model.

  \sa OrganizerModel::importItems
 */

/*!
  \qmlmethod OrganizerModel::importItems(url url, list<string> profiles)

  Import organizer items from a vcalendar by the given \a url and optional \a profiles.
  Only one import operation can be active at a time.
 */
void QDeclarativeOrganizerModel::importItems(const QUrl& url, const QStringList &profiles)
{
    Q_D(QDeclarativeOrganizerModel);

    ImportError importError = ImportNotReadyError;

    // Reader is capable of handling only one request at the time.
  if (!d->m_reader || (d->m_reader->state() != QVersitReader::ActiveState)) {

        d->m_importProfiles = profiles;

        //TODO: need to allow download vcard from network
        QFile *file = new QFile(urlToLocalFileName(url));
        if (file->open(QIODevice::ReadOnly)) {
            if (!d->m_reader) {
                d->m_reader = new QVersitReader;
                connect(d->m_reader, SIGNAL(stateChanged(QVersitReader::State)), this, SLOT(startImport(QVersitReader::State)));
            }

            d->m_reader->setDevice(file);
            if (d->m_reader->startReading()) {
                d->m_lastImportUrl = url;
                return;
            }
            importError = QDeclarativeOrganizerModel::ImportError(d->m_reader->error());
        } else {
            importError = ImportIOError;
        }
  }

    // If cannot startReading because already running then report the import error now
    emit importCompleted(importError, url, QStringList());
}

/*!
  \qmlsignal OrganizerModel::onExportCompleted()

  This signal is emitted, when \l OrganizerModel::exportItems() completes. The success of operation
  can be seen on \a error which is defined in \l OrganizerModel::ExportError. \a url indicates the
  file, which was exported.
 */
/*!
  \qmlmethod OrganizerModel::exportItems(url url, list<string> profiles)
  Export organizer items into a vcalendar file to the given \a url by optional \a profiles.
  At the moment only the local file url is supported in export method.
  */
void QDeclarativeOrganizerModel::exportItems(const QUrl &url, const QStringList &profiles)
{
    Q_D(QDeclarativeOrganizerModel);
    ExportError exportError = ExportNotReadyError;

    // Writer is capable of handling only one request at the time.
    if (!d->m_writer || (d->m_writer->state() != QVersitWriter::ActiveState)) {

        QString profile = profiles.isEmpty() ? QString() : profiles.at(0);

        QVersitOrganizerExporter exporter(profile);
        QList<QOrganizerItem> items;
        foreach (QDeclarativeOrganizerItem *di, d->m_items)
            items.append(di->item());

        exporter.exportItems(items, QVersitDocument::ICalendar20Type);
        QVersitDocument document = exporter.document();
        QFile *file = new QFile(urlToLocalFileName(url));
        if (file->open(QIODevice::ReadWrite)) {
            if (!d->m_writer) {
                d->m_writer = new QVersitWriter;
                connect(d->m_writer, SIGNAL(stateChanged(QVersitWriter::State)), this, SLOT(itemsExported(QVersitWriter::State)));
            }
            d->m_writer->setDevice(file);
            if (d->m_writer->startWriting(document)) {
                d->m_lastExportUrl = url;
                return;
            }
            exportError = QDeclarativeOrganizerModel::ExportError(d->m_writer->error());
        } else {
            exportError = ExportIOError;
        }
    }
    emit exportCompleted(exportError, url);
}

void QDeclarativeOrganizerModel::itemsExported(QVersitWriter::State state)
{
    Q_D(QDeclarativeOrganizerModel);
    if (state == QVersitWriter::FinishedState || state == QVersitWriter::CanceledState) {
         emit exportCompleted(QDeclarativeOrganizerModel::ExportError(d->m_writer->error()), d->m_lastExportUrl);
         delete d->m_writer->device();
         d->m_writer->setDevice(0);
    }
}

int QDeclarativeOrganizerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_items.count();
}

void QDeclarativeOrganizerModel::setManager(const QString& managerName)
{
    Q_D(QDeclarativeOrganizerModel);
    if (d->m_manager && (managerName == d->m_manager->managerName() || managerName == d->m_manager->managerUri()))
        return;

    if (d->m_manager) {
        cancelUpdate();
        d->m_updatePendingFlag = QDeclarativeOrganizerModelPrivate::NonePending;
        delete d->m_manager;
    }

    if (managerName.startsWith("qtorganizer:")) {
        d->m_manager = QOrganizerManager::fromUri(managerName, this);
    } else {
        d->m_manager = new QOrganizerManager(managerName, QMap<QString, QString>(), this);
    }

    connect(d->m_manager, &QOrganizerManager::collectionsAdded, &d->m_fetchCollectionsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(d->m_manager, &QOrganizerManager::collectionsChanged, &d->m_fetchCollectionsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(d->m_manager, &QOrganizerManager::collectionsRemoved, &d->m_fetchCollectionsTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(d->m_manager, &QOrganizerManager::dataChanged, &d->m_updateTimer, static_cast<void (QTimer::*)(void)>(&QTimer::start));
    connect(d->m_manager, SIGNAL(itemsModified(QList<QPair<QOrganizerItemId,QOrganizerManager::Operation> >)), this, SLOT(onItemsModified(QList<QPair<QOrganizerItemId,QOrganizerManager::Operation> >)));

    const QOrganizerManager::Error managerError = d->m_manager->error();
    if (QOrganizerManager::NoError != managerError && d->m_error != managerError) {
        d->m_error = managerError;
        emit errorChanged();
    } else if (QOrganizerManager::NoError != d->m_error) {
        d->m_error = QOrganizerManager::NoError;
        emit errorChanged();
    }

    emit managerChanged();
}

void QDeclarativeOrganizerModel::componentComplete()
{
    Q_D(QDeclarativeOrganizerModel);
    d->m_componentCompleted = true;
    if (!d->m_manager)
        setManager(QString());

    if (d->m_autoUpdate) {
        d->m_initialUpdate = true;
        update();
    } else {
        emit modelChanged();
    }
}
/*!
  \qmlproperty Filter OrganizerModel::filter

  This property holds the filter instance used by the organizer model.

  Set filter property to 'null', when you want to reset it to default value.

  \sa Filter
  */
QDeclarativeOrganizerItemFilter* QDeclarativeOrganizerModel::filter() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_filter;
}

void QDeclarativeOrganizerModel::setFilter(QDeclarativeOrganizerItemFilter* filter)
{
    Q_D(QDeclarativeOrganizerModel);
    if (filter != d->m_filter) {
        if (d->m_filter)
            disconnect(d->m_filter, SIGNAL(filterChanged()), this, SIGNAL(filterChanged()));
        d->m_filter = filter;
        if (d->m_filter)
            connect(d->m_filter, SIGNAL(filterChanged()), this, SIGNAL(filterChanged()), Qt::UniqueConnection);
        emit filterChanged();
    }
}

/*!
  \qmlproperty FetchHint OrganizerModel::fetchHint

  This property holds the fetch hint instance used by the organizer model.

  \sa FetchHint
  */
QDeclarativeOrganizerItemFetchHint* QDeclarativeOrganizerModel::fetchHint() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_fetchHint;
}

void QDeclarativeOrganizerModel::setFetchHint(QDeclarativeOrganizerItemFetchHint* fetchHint)
{
    Q_D(QDeclarativeOrganizerModel);
    if (fetchHint != d->m_fetchHint) {
        if (d->m_fetchHint)
            disconnect(d->m_fetchHint, SIGNAL(fetchHintChanged()), this, SIGNAL(fetchHintChanged()));
        d->m_fetchHint = fetchHint;
        if (d->m_fetchHint)
            connect(d->m_fetchHint, SIGNAL(fetchHintChanged()), this, SIGNAL(fetchHintChanged()), Qt::UniqueConnection);
        emit fetchHintChanged();
    }
}
/*!
  \qmlproperty int OrganizerModel::itemCount

  This property holds the size of organizer items the OrganizerModel currently holds.

  This property is read only.
  */
int QDeclarativeOrganizerModel::itemCount() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_items.size();
}
/*!
  \qmlproperty string OrganizerModel::error

  This property holds the latest error code returned by the organizer manager.

  This property is read only.
  */
QString QDeclarativeOrganizerModel::error() const
{
    Q_D(const QDeclarativeOrganizerModel);
    if (d->m_manager) {
        switch (d->m_error) {
        case QOrganizerManager::DoesNotExistError:
            return QStringLiteral("DoesNotExist");
        case QOrganizerManager::AlreadyExistsError:
            return QStringLiteral("AlreadyExists");
        case QOrganizerManager::InvalidDetailError:
            return QStringLiteral("InvalidDetail");
        case QOrganizerManager::InvalidCollectionError:
            return QStringLiteral("InvalidCollection");
        case QOrganizerManager::LockedError:
            return QStringLiteral("LockedError");
        case QOrganizerManager::DetailAccessError:
            return QStringLiteral("DetailAccessError");
        case QOrganizerManager::PermissionsError:
            return QStringLiteral("PermissionsError");
        case QOrganizerManager::OutOfMemoryError:
            return QStringLiteral("OutOfMemory");
        case QOrganizerManager::NotSupportedError:
            return QStringLiteral("NotSupported");
        case QOrganizerManager::BadArgumentError:
            return QStringLiteral("BadArgument");
        case QOrganizerManager::UnspecifiedError:
            return QStringLiteral("UnspecifiedError");
        case QOrganizerManager::LimitReachedError:
            return QStringLiteral("LimitReached");
        case QOrganizerManager::InvalidItemTypeError:
            return QStringLiteral("InvalidItemType");
        case QOrganizerManager::InvalidOccurrenceError:
            return QStringLiteral("InvalidOccurrence");
        default:
            break;
        }
    }
    return QStringLiteral("NoError");
}

/*!
  \qmlproperty list<SortOrder> OrganizerModel::sortOrders

  This property holds a list of sort orders used by the organizer model.

  \sa SortOrder
  */
QQmlListProperty<QDeclarativeOrganizerItemSortOrder> QDeclarativeOrganizerModel::sortOrders()
{
    return QQmlListProperty<QDeclarativeOrganizerItemSortOrder>(this,
                                                                        0,
                                                                        sortOrder_append,
                                                                        sortOrder_count,
                                                                        sortOrder_at,
                                                                        sortOrder_clear);
}

void QDeclarativeOrganizerModel::startImport(QVersitReader::State state)
{
    Q_D(QDeclarativeOrganizerModel);
    if (state == QVersitReader::FinishedState || state == QVersitReader::CanceledState) {
        QStringList ids;

        if (!d->m_reader->results().isEmpty()) {
            QVersitOrganizerImporter importer;
            importer.importDocument(d->m_reader->results().at(0));
            QList<QOrganizerItem> items = importer.items();
            delete d->m_reader->device();
            d->m_reader->setDevice(0);

            if (d->m_manager && !d->m_manager->saveItems(&items)) {
                if (d->m_error != d->m_manager->error()) {
                    d->m_error = d->m_manager->error();
                    emit errorChanged();
                }
            } else {
                foreach (const QOrganizerItem i, items) {
                    ids << i.id().toString();
                }
            }
        }
        emit importCompleted(QDeclarativeOrganizerModel::ImportError(d->m_reader->error()), d->m_lastImportUrl, ids);
    }
}

bool QDeclarativeOrganizerModel::itemHasRecurrence(const QOrganizerItem& oi) const
{
    if (oi.type() == QOrganizerItemType::TypeEvent || oi.type() == QOrganizerItemType::TypeTodo) {
        QOrganizerItemRecurrence recur = oi.detail(QOrganizerItemDetail::TypeRecurrence);
        return !recur.recurrenceDates().isEmpty() || !recur.recurrenceRules().isEmpty();
    }

    return false;
}

QDeclarativeOrganizerItem* QDeclarativeOrganizerModel::createItem(const QOrganizerItem& item)
{
    QDeclarativeOrganizerItem* di;
    if (item.type() == QOrganizerItemType::TypeEvent)
        di = new QDeclarativeOrganizerEvent(this);
    else if (item.type() == QOrganizerItemType::TypeEventOccurrence)
        di = new QDeclarativeOrganizerEventOccurrence(this);
    else if (item.type() == QOrganizerItemType::TypeTodo)
        di = new QDeclarativeOrganizerTodo(this);
    else if (item.type() == QOrganizerItemType::TypeTodoOccurrence)
        di = new QDeclarativeOrganizerTodoOccurrence(this);
    else if (item.type() == QOrganizerItemType::TypeJournal)
        di = new QDeclarativeOrganizerJournal(this);
    else if (item.type() == QOrganizerItemType::TypeNote)
        di = new QDeclarativeOrganizerNote(this);
    else
        di = new QDeclarativeOrganizerItem(this);
    di->setItem(item);
    return di;
}

void QDeclarativeOrganizerModel::checkError(const QOrganizerAbstractRequest *request)
{
    Q_D(QDeclarativeOrganizerModel);
    if (request && d->m_error != request->error()) {
        d->m_error = request->error();
        emit errorChanged();
    }
}

/*!
    \qmlsignal OrganizerModel::onItemsFetched(int requestId, list<OrganizerItem> fetchedItems)

    This handler is called when request of the given \a requestId is finished with the \a fetchedItems.

    \sa fetchItems
 */

/*!
    \qmlmethod int OrganizerModel::fetchItems(date start, date end,
                               Filter filter,
                               int maxCount,
                               list<SortOrder> sortOrders,
                               FetchHint fetchHint)

    This method will start a request to fetch items between the given \a start and \a end dates.
    Optionally a \a sort order, \a filter, \a fetchHint and \a maxCount can
    be specified to narrow the search. If nothing is set for these optional paramenters then
    defaults are applied, essentially any sort order, default filter, default storage location and all items.

    The unique ID of this request will be returned.  If the request can't be started -1 is returned.
    The end date must be greater than the start date for this method to start a fetch request.

    Note that the items fetched won't be added to the model, but can be accessed through the onItemsFetched
    handler. No properties in the model are updated at all.

    \sa onItemsFetched
  */
int QDeclarativeOrganizerModel::fetchItems(const QDateTime &start, const QDateTime &end,
                                           QDeclarativeOrganizerItemFilter *filter,
                                           int maxCount,
                                           const QVariantList &sortOrders,
                                           QDeclarativeOrganizerItemFetchHint *fetchHint)
{
    Q_D(QDeclarativeOrganizerModel);
    if (!start.isValid() || !end.isValid() || !(end > start))
    {
        return -1;
    }

    // Parameter validation left to fetch request method.
    QOrganizerItemFetchRequest *fetchRequest = new QOrganizerItemFetchRequest(this);
    connect(fetchRequest, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
            this, SLOT(onFetchItemsRequestStateChanged(QOrganizerAbstractRequest::State)));

    QList<QOrganizerItemSortOrder> sList;
    QVariantList::const_iterator it = sortOrders.begin();
    while (it != sortOrders.end()) {
        if ((*it).canConvert<QObject *>()) {
            QDeclarativeOrganizerItemSortOrder *sortOrderItem = (*it).value<QDeclarativeOrganizerItemSortOrder *>();
            sList << sortOrderItem->sortOrder();
        }
        ++it;
    }

    const QOrganizerItemFilter &fetchFilter = filter->filter();
    fetchRequest->setFilter( fetchFilter );
    const QOrganizerItemFetchHint &hint = fetchHint->fetchHint();

    fetchRequest->setManager(d->m_manager);
    fetchRequest->setStartDate(start);
    fetchRequest->setEndDate(end);
    fetchRequest->setSorting(sList);
    fetchRequest->setMaxCount(maxCount);
    fetchRequest->setFetchHint(hint);

    int requestId = d->m_lastRequestId.fetchAndAddOrdered(1);
    d->m_requestIdHash.insert(fetchRequest, requestId);
    if (fetchRequest->start()) {
        return requestId;
    } else {
        d->m_requestIdHash.remove(fetchRequest);
        return -1;
    }
}

/*!
    \qmlmethod int OrganizerModel::fetchItems(stringlist itemIds)

    Starts a request to fetch items by the given \a itemIds, and returns the unique ID of this request.
    -1 is returned if the request can't be started.

    Note that the items fetched won't be added to the model, but can be accessed through the onItemsFetched
    handler.

    \sa onItemsFetched
  */
int QDeclarativeOrganizerModel::fetchItems(const QStringList &itemIds)
{
    Q_D(QDeclarativeOrganizerModel);
    if (itemIds.isEmpty())
        return -1;

    QOrganizerItemFetchByIdRequest *fetchRequest = new QOrganizerItemFetchByIdRequest(this);
    connect(fetchRequest, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
            this, SLOT(onFetchItemsRequestStateChanged(QOrganizerAbstractRequest::State)));
    fetchRequest->setManager(d->m_manager);

    QList<QOrganizerItemId> ids;
    foreach (const QString &itemId, itemIds)
        ids.append(QOrganizerItemId::fromString(itemId));
    fetchRequest->setIds(ids);
    int requestId = d->m_lastRequestId.fetchAndAddOrdered(1);
    d->m_requestIdHash.insert(fetchRequest, requestId);
    if (fetchRequest->start()) {
        return requestId;
    } else {
        d->m_requestIdHash.remove(fetchRequest);
        return -1;
    }
}

/*!
    \internal
 */
void QDeclarativeOrganizerModel::onFetchItemsRequestStateChanged(QOrganizerAbstractRequest::State state)
{
    Q_D(QDeclarativeOrganizerModel);

    QOrganizerAbstractRequest *request=qobject_cast<QOrganizerAbstractRequest *>(sender());

    if (state != QOrganizerAbstractRequest::FinishedState || !request)
        return;

    QOrganizerItemFetchRequest *itemFetchRequest = qobject_cast<QOrganizerItemFetchRequest *>(request);
    QOrganizerItemFetchByIdRequest *itemByIdFetchRequest = qobject_cast<QOrganizerItemFetchByIdRequest *>(request);

    if (!itemFetchRequest && !itemByIdFetchRequest)
        return;

    checkError(request);
    const int requestId = d->m_requestIdHash.value(request, -1);
    if (requestId == -1)
        qWarning() << Q_FUNC_INFO << "transaction not found from the request hash";
    else
        d->m_requestIdHash.remove(request);

    QVariantList list;
    if (request->error() == QOrganizerManager::NoError) {
            const QList<QOrganizerItem> &items((!itemFetchRequest) ? itemByIdFetchRequest->items():itemFetchRequest->items());
        QDeclarativeOrganizerItem *declarativeItem(0);
        foreach (const QOrganizerItem &item, items) {
            switch (item.type()) {
            case QOrganizerItemType::TypeEvent:
                declarativeItem = new QDeclarativeOrganizerEvent(this);
                break;
            case QOrganizerItemType::TypeEventOccurrence:
                declarativeItem = new QDeclarativeOrganizerEventOccurrence(this);
                break;
            case QOrganizerItemType::TypeTodo:
                declarativeItem = new QDeclarativeOrganizerTodo(this);
                break;
            case QOrganizerItemType::TypeTodoOccurrence:
                declarativeItem = new QDeclarativeOrganizerTodoOccurrence(this);
                break;
            default:
                declarativeItem = new QDeclarativeOrganizerItem(this);
                break;
            }
            declarativeItem->setItem(item);
            list.append(QVariant::fromValue((QObject *)declarativeItem));
        }
    }
    emit itemsFetched(requestId, list);

    request->deleteLater();
}

/*!
    \qmlmethod list<bool> OrganizerModel::containsItems(date start, date end, int interval)

    Returns a list of booleans telling if there is any item falling in the given time range.

    For example, if the \a start time is 2011-12-08 14:00:00, the \a end time is 2011-12-08 20:00:00,
    and the \a interval is 3600 (seconds), a list of size 6 is returned, telling if there is any item
    falling in the range of 14:00:00 to 15:00:00, 15:00:00 to 16:00:00, ..., 19:00:00 to 20:00:00.
 */
QList<bool> QDeclarativeOrganizerModel::containsItems(const QDateTime &start, const QDateTime &end, int interval)
{
    Q_D(QDeclarativeOrganizerModel);

    if (!(start.isValid() && end.isValid() && start < end && interval > 0))
        return QList<bool>();

    int i(0);
    int count = qCeil(start.secsTo(end) / static_cast<double>(interval));
    QVector<bool> occupiedTimeSlots(count, false);

    QVector<QDateTime> dateTime(count + 1);
    dateTime[0] = start;
    for (i = 1; i < count; ++i)
        dateTime[i] = dateTime.at(i - 1).addSecs(interval);
    dateTime[count] = end;

    QDateTime startTime;
    QDateTime endTime;
    bool itemStartFound;

    foreach (QDeclarativeOrganizerItem *item, d->m_items) {
        startTime = item->itemStartTime();
        endTime = item->itemEndTime();

        // check if item is occurring between start and end
        if (!((!startTime.isNull() && startTime >= start && startTime < end)
              || (!endTime.isNull() && endTime > start && endTime <= end)
              || (!startTime.isNull() && !endTime.isNull() && startTime <= start && endTime >= end)))
            continue;

        itemStartFound = (!startTime.isNull() &&  startTime <= start);
        for (i = 0; i < count; ++i) {

            if (!endTime.isNull() && endTime > dateTime.at(i) && endTime <= dateTime.at(i + 1)) {
                // item end time found, no need to check more time slots
                occupiedTimeSlots[i] = true;
                break;
            }
            if (occupiedTimeSlots.at(i))
                continue;

            if (itemStartFound) {
                occupiedTimeSlots[i] = true;
            } else if (!startTime.isNull() && startTime < dateTime.at(i + 1)) {
                if (startTime >= dateTime.at(i))
                    occupiedTimeSlots[i] = true;
                if (endTime.isNull())
                    break;
                itemStartFound = true;
            }
        }
    }
    return occupiedTimeSlots.toList();
}

/*!
  \qmlmethod bool OrganizerModel::containsItems(date start, date end)

  Returns true if there is at least one OrganizerItem between the given date range.
  Both the \a start and  \a end parameters are optional, if no \a end parameter, returns true
  if there are item(s) after \a start, if neither start nor end date time provided, returns true if
  items in the current model is not empty, otherwise return false.

  \sa itemIds()
  */
bool QDeclarativeOrganizerModel::containsItems(const QDateTime &start, const QDateTime &end)
{
    return !itemIds(start, end).isEmpty();
}

/*!
    \qmlmethod list<OrganizerItem> OrganizerModel::itemsByTimePeriod(date start, date end)

    Returns the list of organizer items between the given \a start and \a end period.
 */
QVariantList QDeclarativeOrganizerModel::itemsByTimePeriod(const QDateTime &start, const QDateTime &end)
{
    Q_D(QDeclarativeOrganizerModel);
    QVariantList list;

    if (start.isValid() && end.isValid()) {
        QDateTime startTime;
        QDateTime endTime;
        foreach (QDeclarativeOrganizerItem *item, d->m_items) {
            startTime = item->itemStartTime();
            endTime = item->itemEndTime();
            if ((startTime.isValid() && startTime <= start && endTime >= end)
                || (startTime >= start && startTime <= end)
                || (endTime >= start && endTime <= end)) {
                list.append(QVariant::fromValue((QObject *)item));
            }
        }
    } else if (start.isValid()) {
        foreach (QDeclarativeOrganizerItem *item, d->m_items) {
            if (item->itemEndTime() >= start)
                list.append(QVariant::fromValue((QObject *)item));
        }
    } else if (end.isValid()) {
        foreach (QDeclarativeOrganizerItem *item, d->m_items) {
            if (item->itemStartTime() <= end)
                list.append(QVariant::fromValue((QObject *)item));
        }
    } else {
        foreach (QDeclarativeOrganizerItem *item, d->m_items)
            list.append(QVariant::fromValue((QObject *)item));
    }

    return list;
}

/*!
    \qmlmethod OrganizerItem OrganizerModel::item(string itemId)

    Returns the OrganizerItem object with the given \a itemId.
 */
QDeclarativeOrganizerItem *QDeclarativeOrganizerModel::item(const QString &itemId)
{
    Q_D(QDeclarativeOrganizerModel);
    if (itemId.isEmpty())
        return 0;

    return d->m_itemIdHash.value(itemId, 0);
}

/*!
  \qmlmethod list<string> OrganizerModel::itemIds(date start, date end)

  Returns the list of organizer item ids between the given date range \a start and \a end,
  excluding generated occurrences. Both the \a start and \a end parameters are optional,
  if no \a end parameter, returns all item ids from \a start, if neither start nor end date
  time provided, returns all item ids in the current model.

  \sa containsItems()
  */
QStringList QDeclarativeOrganizerModel::itemIds(const QDateTime &start, const QDateTime &end)
{
    Q_D(QDeclarativeOrganizerModel);
    //TODO: quick search this
    QStringList ids;
    if (!end.isNull()) {
        // both start date and end date are valid
        foreach (QDeclarativeOrganizerItem* item, d->m_items) {
            if (item->generatedOccurrence())
                continue;
            if ( (item->itemStartTime() >= start && item->itemStartTime() <= end)
                 || (item->itemEndTime() >= start && item->itemEndTime() <= end)
                 || (item->itemEndTime() > end && item->itemStartTime() < start))
                ids << item->itemId();
        }
    } else if (!start.isNull()) {
        // only a valid start date is valid
        foreach (QDeclarativeOrganizerItem* item, d->m_items) {
            if (!item->generatedOccurrence() && item->itemStartTime() >= start)
                ids << item->itemId();
        }
    } else {
        // neither start nor end date is valid
        foreach (QDeclarativeOrganizerItem* item, d->m_items) {
            if (!item->generatedOccurrence())
                ids << item->itemId();
        }
    }
    return ids;
}

void QDeclarativeOrganizerModel::fetchAgain()
{
    Q_D(QDeclarativeOrganizerModel);
    cancelUpdate();

    d->m_fetchRequest  = new QOrganizerItemFetchRequest(this);
    d->m_fetchRequest->setManager(d->m_manager);
    d->m_fetchRequest->setSorting(d->m_sortOrders);
    d->m_fetchRequest->setStartDate(d->m_startPeriod);
    d->m_fetchRequest->setEndDate(d->m_endPeriod);

    if (d->m_filter){
        d->m_fetchRequest->setFilter(d->m_filter->filter());
    } else {
        d->m_fetchRequest->setFilter(QOrganizerItemFilter());
    }
    d->m_fetchRequest->setFetchHint(d->m_fetchHint ? d->m_fetchHint->fetchHint() : QOrganizerItemFetchHint());

    connect(d->m_fetchRequest, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(requestUpdated()));
    d->m_fetchRequest->start();
}

/*
  This slot function is connected with item fetch requests and item occurrence fetch requests,
  so the QObject::sender() must be checked for the right sender type.
  During update() function, the fetchAgain() will be invoked, inside fetchAgain(), a QOrganizerItemFetchRequest object
  is created and started, when this fetch request finished, this requestUpdate() slot will be invoked for the first time.
  Then check each of the organizer items returned by the item fetch request, if the item is a recurrence item,
  a QOrganizerItemOccurrenceFetchRequest object will be created and started. When each of these occurrence fetch requests
  finishes, this requestUpdated() slot will be invoked again and insert the returned occurrence items into the d->m_items
  list.
  */
void QDeclarativeOrganizerModel::requestUpdated()
{
    Q_D(QDeclarativeOrganizerModel);
    QList<QOrganizerItem> items;
    QOrganizerItemFetchRequest* ifr = qobject_cast<QOrganizerItemFetchRequest*>(QObject::sender());
    Q_ASSERT(ifr);
    if (ifr->isFinished()) {
        items = ifr->items();
        checkError(ifr);
        ifr->deleteLater();
        d->m_fetchRequest = 0;
        d->m_updatePendingFlag &= ~QDeclarativeOrganizerModelPrivate::UpdatingItemsPending;
    } else {
        return;
    }

    if (!items.isEmpty() || !d->m_items.isEmpty() || d->m_initialUpdate) {
        // full update: first go through new items and check if they
        // existed earlier. if they did, use the existing declarative wrapper.
        // otherwise create new declarative item.
        // for occurrences new declarative item is always created.
        QList<QDeclarativeOrganizerItem *> newList;
        QHash<QString, QDeclarativeOrganizerItem *> newItemIdHash;
        QHash<QString, QDeclarativeOrganizerItem *>::iterator iterator;
        QOrganizerItem item;
        QString idString;
        QDeclarativeOrganizerItem *declarativeItem;
        d->m_initialUpdate = false;

        int i;
        for (i = 0; i < items.size(); i++) {
            item = items[i];
            idString = item.id().toString();
            if (item.id().isNull()) {
                // this is occurrence
                declarativeItem = createItem(item);
            } else {
                iterator = d->m_itemIdHash.find(idString);
                if (iterator != d->m_itemIdHash.end()) {
                    declarativeItem = iterator.value();
                    declarativeItem->setItem(item);
                } else {
                    declarativeItem = createItem(item);
                }
                newItemIdHash.insert(idString, declarativeItem);
            }
            newList.append(declarativeItem);
        }

        // go through old items and delete items, which are not part of the
        // new item set. delete also all old occurrences.
        for (i = 0; i < d->m_items.size(); i++) {
            // FIXME: avoid unnecessary usage of item getter which copies all details...
            if (d->m_items[i]->item().id().isNull()) {
                d->m_items[i]->deleteLater();
            } else {
                iterator = newItemIdHash.find(d->m_items[i]->itemId());
                if (iterator == newItemIdHash.end())
                    d->m_items[i]->deleteLater();
            }
        }
        beginResetModel();
        d->m_items = newList;
        endResetModel();

        d->m_itemIdHash = newItemIdHash;
        d->m_modelChangedTimer.start();
    }
}

/*!
  \qmlmethod OrganizerModel::saveItem(OrganizerItem item)

  Saves asynchronously the given \a item into the organizer backend.
 */
void QDeclarativeOrganizerModel::saveItem(QDeclarativeOrganizerItem* di)
{
    Q_D(QDeclarativeOrganizerModel);
    if (di) {
        QOrganizerItem item = di->item();
        QOrganizerItemSaveRequest* req = new QOrganizerItemSaveRequest(this);
        req->setManager(d->m_manager);
        req->setItem(item);

        if (di->itemId().isEmpty()) {
            // if the item id is empty this means that this item is a new event
            // we need to keep trace of this declarative item to update with the
            // new Id as soon as this request finish
            QPointer<QDeclarativeOrganizerItem> pItem = di;
            req->setProperty(ITEM_TO_SAVE_PROPERTY, QVariant::fromValue(pItem));
        }

        connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

        req->start();
    }
}

/*!
  \qmlmethod OrganizerModel::removeItem(string itemId)
  Removes the organizer item with the given \a itemId from the backend.

  */
void QDeclarativeOrganizerModel::removeItem(const QString& id)
{
    QList<QString> ids;
    ids << id;
    removeItems(ids);
}

/*!
  \qmlmethod OrganizerModel::removeItem(OrganizerItem item)
  Removes the given organizer \a item from the backend.
  */
void QDeclarativeOrganizerModel::removeItem(QDeclarativeOrganizerItem *item)
{
    Q_D(QDeclarativeOrganizerModel);
    QOrganizerItemRemoveRequest* req = new QOrganizerItemRemoveRequest(this);
    req->setManager(d->m_manager);
    req->setItem(item->item());
    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));
    req->start();
}

/*!
  \qmlmethod OrganizerModel::removeItems(list<string> itemId)
  Removes asynchronously the organizer items with the given \a ids from the backend.

  */
void QDeclarativeOrganizerModel::removeItems(const QStringList& ids)
{
    Q_D(QDeclarativeOrganizerModel);
    QOrganizerItemRemoveByIdRequest* req = new QOrganizerItemRemoveByIdRequest(this);
    req->setManager(d->m_manager);
    QList<QOrganizerItemId> oids;

    // FIXME: no special format for occurrence ids
    foreach (const QString& id, ids) {
        if (id.startsWith(QString("qtorganizer:occurrence"))) {
            qmlInfo(this) << tr("Can't remove an occurrence item, please modify the parent item's recurrence rule instead!");
            continue;
        }
        QOrganizerItemId itemId = QOrganizerItemId::fromString(id);
        if (!itemId.isNull()) {
             oids.append(itemId);
        }
    }

    req->setItemIds(oids);

    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

    req->start();
}

/*!
  \qmlmethod OrganizerModel::removeItems(list<OrganizerItem> items)
  Removes asynchronously the organizer items in the given \a items list from the backend.
  */
void QDeclarativeOrganizerModel::removeItems(const QList<QDeclarativeOrganizerItem> &items)
{
    Q_D(QDeclarativeOrganizerModel);
    QOrganizerItemRemoveRequest* req = new QOrganizerItemRemoveRequest(this);
    req->setManager(d->m_manager);
    QList<QOrganizerItem> ois;

    for (int i = 0; i < items.size(); i++) {
        ois.append(items[i].item());
    }

    req->setItems(ois);
    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));
    req->start();
}

/*!
    \internal
 */
void QDeclarativeOrganizerModel::onRequestStateChanged(QOrganizerAbstractRequest::State newState)
{
    if (newState != QOrganizerAbstractRequest::FinishedState)
        return;

    QOrganizerAbstractRequest *request = qobject_cast<QOrganizerAbstractRequest *>(sender());
    Q_ASSERT(request);

    if (request->error() == QOrganizerManager::NoError &&
        request->type() == QOrganizerAbstractRequest::ItemSaveRequest) {
        QVariant vItem = request->property(ITEM_TO_SAVE_PROPERTY);
        if (vItem.isValid()) {
            QPointer<QDeclarativeOrganizerItem> pItem = vItem.value<QPointer<QDeclarativeOrganizerItem> >();
            // Fill declarative item id
            QOrganizerItemSaveRequest *sr = static_cast<QOrganizerItemSaveRequest *>(request);
            if (pItem && sr->items().length() == 1)
                pItem->setItem(sr->items()[0]);
        }
    }

    checkError(request);
    request->deleteLater();
}

void QDeclarativeOrganizerModel::removeItemsFromModel(const QList<QString> &itemIds)
{
    Q_D(QDeclarativeOrganizerModel);
    bool emitSignal = false;
    bool itemIdFound = false;

    foreach (const QString &itemId, itemIds) {
        itemIdFound = false;
        // generated occurrences are not in m_itemIdHash
        if (d->m_itemIdHash.remove(itemId) > 0)
            itemIdFound = true;
        for (int i = d->m_items.count() - 1; i >= 0; i--) {
            if (itemIdFound) {
                if (d->m_items.at(i)->itemId() == itemId) {
                    beginRemoveRows(QModelIndex(), i, i);
                    d->m_items.removeAt(i);
                    endRemoveRows();
                    emitSignal = true;
                    break;
                }
            } else if (d->m_items.at(i)->generatedOccurrence()) {
                QDeclarativeOrganizerItemDetail *parentDetail = d->m_items.at(i)->detail(QDeclarativeOrganizerItemDetail::Parent);
                if (parentDetail->value(QDeclarativeOrganizerItemParent::FieldParentId).toString() == itemId) {
                    beginRemoveRows(QModelIndex(), i, i);
                    d->m_items.removeAt(i);
                    endRemoveRows();
                    emitSignal = true;
                }
            }
        }
    }
    if (emitSignal)
        d->m_modelChangedTimer.start();
}


/*!
    \internal

    It's invoked upon the QOrganizerManager::itemsModified() signal.
 */
void QDeclarativeOrganizerModel::onItemsModified(const QList<QPair<QOrganizerItemId, QOrganizerManager::Operation> > &itemIds)
{
    Q_D(QDeclarativeOrganizerModel);
    if (!d->m_autoUpdate)
        return;

    QSet<QOrganizerItemId> addedAndChangedItems;
    QList<QString> removedItems;
    for (int i = itemIds.size() - 1; i >= 0; i--) {
        if (itemIds[i].second == QOrganizerManager::Remove) {
            // check that item has not been added after removing it
            if (!addedAndChangedItems.contains(itemIds[i].first))
                removedItems.append(itemIds[i].first.toString());
        } else {
            addedAndChangedItems.insert(itemIds[i].first);
        }
    }
    if (!removedItems.isEmpty())
        removeItemsFromModel(removedItems);

    if (!addedAndChangedItems.isEmpty()) {
        // FIXME; to be optimized with fetching only the modified items
        // from the storage locations modified items are on
        QOrganizerItemFetchRequest *fetchRequest = new QOrganizerItemFetchRequest(this);
        connect(fetchRequest, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)),
                this, SLOT(onItemsModifiedFetchRequestStateChanged(QOrganizerAbstractRequest::State)));
        fetchRequest->setManager(d->m_manager);
        fetchRequest->setStartDate(d->m_startPeriod);
        fetchRequest->setEndDate(d->m_endPeriod);
        fetchRequest->setFilter(d->m_filter ? d->m_filter->filter() : QOrganizerItemFilter());
        fetchRequest->setSorting(d->m_sortOrders);
        fetchRequest->setFetchHint(d->m_fetchHint ? d->m_fetchHint->fetchHint() : QOrganizerItemFetchHint());
        d->m_notifiedItems.insert(fetchRequest, addedAndChangedItems);

        fetchRequest->start();
    }
}

/*!
    \internal

    It's invoked by the fetch request from onItemsModified().
 */
void QDeclarativeOrganizerModel::onItemsModifiedFetchRequestStateChanged(QOrganizerAbstractRequest::State state)
{
    // NOTE: this function assumes the sorting algorithm gives always the same result with
    // same data. E.g. items which have the identical sorting key must be sorted too.

    Q_D(QDeclarativeOrganizerModel);
    if (state != QOrganizerAbstractRequest::FinishedState)
        return;

    QOrganizerItemFetchRequest *request = qobject_cast<QOrganizerItemFetchRequest *>(sender());
    Q_ASSERT(request);

    checkError(request);

    QSet<QOrganizerItemId> notifiedItems = d->m_notifiedItems.value(request);
    if (notifiedItems.isEmpty())
        return;

    if (request->error() == QOrganizerManager::NoError) {
        bool emitSignal = false;
        QList<QOrganizerItem> fetchedItems = request->items();
        QOrganizerItem oldItem;
        QOrganizerItem newItem;
        QOrganizerItemParent oldParentDetail;
        QOrganizerItemParent newParentDetail;
        QDeclarativeOrganizerItem *declarativeItem;
        QSet<QOrganizerItemId> removedIds;
        QSet<QOrganizerItemId> addedIds;
        int oldInd = 0;
        int newInd = 0;
        while (newInd < fetchedItems.size()) {
            bool addNewItem = false;
            bool oldItemExists = false;

            newItem = fetchedItems[newInd];
            if (oldInd < d->m_items.size()) {
                // quick check if items are same in old and new event lists
                // FIXME: avoid unnecessary usage of item getter which copies all details
                oldItem = d->m_items[oldInd]->item();
                oldItemExists = true;
                if (!newItem.id().isNull() && !oldItem.id().isNull() && newItem.id() == oldItem.id()) {
                    if (notifiedItems.contains(newItem.id())) {
                        d->m_items[oldInd]->setItem(newItem);
                        const QModelIndex idx = index(oldInd, 0);
                        emit dataChanged(idx, idx);
                        emitSignal = true;
                    }
                    newInd++;
                    oldInd++;
                    continue;
                }
            }

            // check should we remove old item
            if (oldItemExists) {
                if (oldItem.id().isNull()) {
                    // this is generated occurrence
                    oldParentDetail = oldItem.detail(QOrganizerItemDetail::TypeParent);
                    if (notifiedItems.contains(oldParentDetail.parentId())) {
                        beginRemoveRows(QModelIndex(), oldInd, oldInd);
                        d->m_items.takeAt(oldInd)->deleteLater();
                        endRemoveRows();
                        emitSignal = true;
                        continue;
                    }
                } else if (notifiedItems.contains(oldItem.id())) {
                    // if notifiedItems contains the oldItem id, it means the item has been
                    // changed and we should reuse the declarative part and only remove
                    // rows from abstract list model
                    // it might also mean that oldItem has been changed so that it does not belong to
                    // the model anymore (e.g. changing fron normal item to recurring item)
                    beginRemoveRows(QModelIndex(), oldInd, oldInd);
                    d->m_items.removeAt(oldInd);
                    endRemoveRows();
                    removedIds.insert(oldItem.id());
                    emitSignal = true;
                    continue;
                } else if (notifiedItems.contains(newItem.id())) {
                    // if newItem is a notified item and does not correspond to an oldItem
                    // then find a correspondent item by id in the old items list
                    // and remove it from the hash and list
                    for (int removeInd = oldInd + 1; removeInd < d->m_items.size(); ++removeInd) {
                        if (newItem.id() == d->m_items[removeInd]->item().id()) {
                            beginRemoveRows(QModelIndex(), removeInd, removeInd);
                            d->m_itemIdHash.remove(d->m_items[removeInd]->itemId());
                            d->m_items.takeAt(removeInd)->deleteLater();
                            endRemoveRows();
                            emitSignal = true;
                            break;
                        }
                    }
                }
            }
            // check should we add the new item
            if (newItem.id().isNull() && (newItem.type() == QOrganizerItemType::TypeEventOccurrence || newItem.type() == QOrganizerItemType::TypeTodoOccurrence)) {
                // this is occurrence (generated or exception)
                newParentDetail = newItem.detail(QOrganizerItemDetail::TypeParent);
                if (notifiedItems.contains(newParentDetail.parentId())) {
                    declarativeItem = createItem(newItem);
                    addNewItem = true;
                }
            } else if (notifiedItems.contains(newItem.id())) {
                QHash<QString, QDeclarativeOrganizerItem *>::const_iterator iterator = d->m_itemIdHash.find(newItem.id().toString());
                if (iterator == d->m_itemIdHash.end()) {
                    declarativeItem = createItem(newItem);
                    d->m_itemIdHash.insert(declarativeItem->itemId(), declarativeItem);
                } else {
                    declarativeItem = d->m_itemIdHash.value(newItem.id().toString());
                    addedIds.insert(newItem.id());
                }
                addNewItem = true;
            }

            if (addNewItem) {
                beginInsertRows(QModelIndex(), oldInd, oldInd);
                d->m_items.insert(oldInd, declarativeItem);
                endInsertRows();
                emitSignal = true;
            }
            oldInd++;
            newInd++;
        }
        // remove the rest of the old items
        if (oldInd <= d->m_items.size() - 1) {
            beginRemoveRows(QModelIndex(), oldInd, d->m_items.size() - 1);
            while (oldInd < d->m_items.size()) {
                d->m_itemIdHash.remove(d->m_items[oldInd]->itemId());
                d->m_items.takeAt(oldInd)->deleteLater();
                emitSignal = true;
                oldInd++;
            }
            endRemoveRows();
        }
        // remove items which were changed so that they are no longer part of the model
        // they have been removed from the model earlier, but need to still be removed from the hash
        // and deleted

        removedIds.subtract(addedIds);
        foreach (const QOrganizerItemId &id, removedIds) {
            QDeclarativeOrganizerItem *changedItem = d->m_itemIdHash.take(id.toString());
            if (changedItem) {
                changedItem->deleteLater();
                emitSignal = true;
            }
        }

        if (emitSignal)
            d->m_modelChangedTimer.start();
    }
    d->m_notifiedItems.remove(request);
    request->deleteLater();
}

/*!
  \qmlmethod OrganizerModel::fetchCollections()
  Fetch asynchronously a list of organizer collections from the organizer backend.
  */
void QDeclarativeOrganizerModel::fetchCollections()
{
    Q_D(QDeclarativeOrganizerModel);
    // fetchCollections() is used for both direct calls and
    // signals from model. For signal from model, check also the
    // autoupdate-flag.
    if (qobject_cast<QTimer*>(sender()) == &d->m_fetchCollectionsTimer) {
        if (!d->m_fetchCollectionsTimer.property(MANUALLY_TRIGGERED_PROPERTY).toBool() && !d->m_autoUpdate)
            return; // it was automatically triggered, but autoUpdate is false, so don't update the model data.
        // reset the state of the manually triggered properly, for next time.
        d->m_fetchCollectionsTimer.setProperty(MANUALLY_TRIGGERED_PROPERTY, QVariant::fromValue<bool>(false));
    }

    QOrganizerCollectionFetchRequest* req = new QOrganizerCollectionFetchRequest(this);
    req->setManager(d->m_manager);

    connect(req,SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(collectionsFetched()));

    req->start();
}

void QDeclarativeOrganizerModel::collectionsFetched()
{
    Q_D(QDeclarativeOrganizerModel);
    QOrganizerCollectionFetchRequest* req = qobject_cast<QOrganizerCollectionFetchRequest*>(QObject::sender());
    Q_ASSERT(req);
    if (req->isFinished() && QOrganizerManager::NoError == req->error()) {
        d->m_updatePendingFlag &= ~QDeclarativeOrganizerModelPrivate::UpdatingCollectionsPending;
        // prepare tables
        QHash<QString, const QOrganizerCollection*> collections;
        foreach (const QOrganizerCollection& collection, req->collections()) {
            collections.insert(collection.id().toString(), &collection);
        }
        QHash<QString, QDeclarativeOrganizerCollection*> declCollections;
        foreach(QDeclarativeOrganizerCollection* declCollection, d->m_collections) {
            declCollections.insert(declCollection->collection().id().toString(), declCollection);
        }
        // go tables through
        QHashIterator<QString, const QOrganizerCollection*> collIterator(collections);
        while (collIterator.hasNext()) {
            collIterator.next();
            if (declCollections.contains(collIterator.key())) {
                // collection on both sides, update the declarative collection
                declCollections.value(collIterator.key())->setCollection(*collections.value(collIterator.key()));
            } else {
                // new collection, add it to declarative collection list
                QDeclarativeOrganizerCollection* declCollection = new QDeclarativeOrganizerCollection(this);
                declCollection->setCollection(*collections.value(collIterator.key()));
                d->m_collections.append(declCollection);
            }
        }
        QHashIterator<QString, QDeclarativeOrganizerCollection*> declCollIterator(declCollections);
        while (declCollIterator.hasNext()) {
            declCollIterator.next();
            if (!collections.contains(declCollIterator.key())) {
                // collection deleted on the backend side, delete from declarative collection list
                QDeclarativeOrganizerCollection* toBeDeletedColl = declCollections.value(declCollIterator.key());
                d->m_collections.removeOne(toBeDeletedColl);
                toBeDeletedColl->deleteLater();
            }
        }
        emit collectionsChanged();
        if (d->m_updatePendingFlag & QDeclarativeOrganizerModelPrivate::UpdatingItemsPending)
            QMetaObject::invokeMethod(this, "fetchAgain", Qt::QueuedConnection);
        req->deleteLater();
    }
    checkError(req);
}

/*!
  \qmlmethod OrganizerModel::saveCollection(Collection collection)

  Saves asynchronously the given \a collection into the organizer backend.
 */
void QDeclarativeOrganizerModel::saveCollection(QDeclarativeOrganizerCollection* declColl)
{
    Q_D(QDeclarativeOrganizerModel);
    if (declColl) {
        QOrganizerCollection collection = declColl->collection();
        QOrganizerCollectionSaveRequest* req = new QOrganizerCollectionSaveRequest(this);
        req->setManager(d->m_manager);
        req->setCollection(collection);

        connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

        req->start();
    }
}

/*!
  \qmlmethod OrganizerModel::removeCollection(string collectionId)
  Removes asynchronously the organizer collection with the given \a collectionId from the backend.
  */
void QDeclarativeOrganizerModel::removeCollection(const QString &collectionId)
{
    Q_D(QDeclarativeOrganizerModel);
    QOrganizerCollectionRemoveRequest* req = new QOrganizerCollectionRemoveRequest(this);
    req->setManager(d->m_manager);
    req->setCollectionId(QOrganizerCollectionId::fromString(collectionId));

    connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), this, SLOT(onRequestStateChanged(QOrganizerAbstractRequest::State)));

    req->start();
}

/*!
  \qmlmethod string OrganizerModel::defaultCollectionId()
  Returns the id of a default Collection object.
  */
QString QDeclarativeOrganizerModel::defaultCollectionId() const
{
    Q_D(const QDeclarativeOrganizerModel);
    return d->m_manager->defaultCollectionId().toString();
}

/*!
  \qmlmethod Collection OrganizerModel::collection(string collectionId)
  Returns the Collection object which collection id is the given \a collectionId and
  null if collection id is not found.
  */
QDeclarativeOrganizerCollection* QDeclarativeOrganizerModel::collection(const QString &collectionId)
{
    Q_D(QDeclarativeOrganizerModel);
    foreach (QDeclarativeOrganizerCollection* collection, d->m_collections) {
        if (collection->id() == collectionId)
            return collection;
    }
    return 0;
}

QVariant QDeclarativeOrganizerModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QDeclarativeOrganizerModel);
    //Check if QList itme's index is valid before access it, index should be between 0 and count - 1
    if (index.row() < 0 || index.row() >= d->m_items.count()) {
        return QVariant();
    }

    QDeclarativeOrganizerItem* di = d->m_items.at(index.row());
    Q_ASSERT(di);
    QOrganizerItem item = di->item();
    switch(role) {
        case Qt::DisplayRole:
            return item.displayLabel();
        case Qt::DecorationRole:
            //return pixmap for this item type
        case OrganizerItemRole:
            return QVariant::fromValue(di);
    }
    return QVariant();
}

/*!
  \qmlproperty list<OrganizerItem> OrganizerModel::items

  This property holds a list of organizer items in the organizer model.

  \sa OrganizerItem
  */
QQmlListProperty<QDeclarativeOrganizerItem> QDeclarativeOrganizerModel::items()
{
    return QQmlListProperty<QDeclarativeOrganizerItem>(this, 0, item_count, item_at);
}

/*!
  \qmlproperty list<Collection> OrganizerModel::collections

  This property holds a list of collections in the organizer model.

  \sa Collection
  */
QQmlListProperty<QDeclarativeOrganizerCollection> QDeclarativeOrganizerModel::collections()
{
    return QQmlListProperty<QDeclarativeOrganizerCollection>(this, 0, collection_count, collection_at);
}

int  QDeclarativeOrganizerModel::item_count(QQmlListProperty<QDeclarativeOrganizerItem> *p)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);
    if (model)
        return model->d_ptr->m_items.count();
    return 0;
}

QDeclarativeOrganizerItem * QDeclarativeOrganizerModel::item_at(QQmlListProperty<QDeclarativeOrganizerItem> *p, int idx)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);
    if (model && idx >= 0 && idx < model->d_ptr->m_items.size())
        return model->d_ptr->m_items.at(idx);
    return 0;
}

void QDeclarativeOrganizerModel::sortOrder_append(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p, QDeclarativeOrganizerItemSortOrder *sortOrder)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);
    if (model && sortOrder) {
        QObject::connect(sortOrder, SIGNAL(sortOrderChanged()), model, SIGNAL(sortOrdersChanged()));
        model->d_ptr->m_declarativeSortOrders.append(sortOrder);
        model->d_ptr->m_sortOrders.append(sortOrder->sortOrder());
        emit model->sortOrdersChanged();
    }
}

int  QDeclarativeOrganizerModel::sortOrder_count(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);
    if (model)
        return model->d_ptr->m_declarativeSortOrders.size();
    return 0;
}
QDeclarativeOrganizerItemSortOrder * QDeclarativeOrganizerModel::sortOrder_at(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p, int idx)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);

    QDeclarativeOrganizerItemSortOrder* sortOrder = 0;
    if (model) {
        int i = 0;
        foreach (QDeclarativeOrganizerItemSortOrder* s, model->d_ptr->m_declarativeSortOrders) {
            if (i == idx) {
                sortOrder = s;
                break;
            } else {
                i++;
            }
        }
    }
    return sortOrder;
}
void  QDeclarativeOrganizerModel::sortOrder_clear(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);

    if (model) {
        model->d_ptr->m_sortOrders.clear();
        model->d_ptr->m_declarativeSortOrders.clear();
        emit model->sortOrdersChanged();
    }
}

int  QDeclarativeOrganizerModel::collection_count(QQmlListProperty<QDeclarativeOrganizerCollection> *p)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);
    return model ? model->d_ptr->m_collections.count() : 0;
}

QDeclarativeOrganizerCollection* QDeclarativeOrganizerModel::collection_at(QQmlListProperty<QDeclarativeOrganizerCollection> *p, int idx)
{
    QDeclarativeOrganizerModel* model = qobject_cast<QDeclarativeOrganizerModel*>(p->object);
    QDeclarativeOrganizerCollection* collection = 0;
    if (model) {
        if (!model->d_ptr->m_collections.isEmpty() && idx >= 0 && idx < model->d_ptr->m_collections.count())
            collection = model->d_ptr->m_collections.at(idx);
    }
    return collection;
}

#include "moc_qdeclarativeorganizermodel_p.cpp"

QT_END_NAMESPACE
