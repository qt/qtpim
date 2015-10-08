/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#include "qdeclarativecontactmodel_p.h"

#include <QtCore/qfile.h>
#include <QtCore/qhash.h>
#include <QtCore/qmap.h>
#include <QtCore/qpointer.h>
#include <QtCore/qurl.h>
#include <QtCore/qmimedatabase.h>
#include <QtCore/qmimetype.h>
#include <QtCore/qtemporaryfile.h>
#include <QtCore/qdir.h>

#include <QtGui/qcolor.h>
#include <QtGui/qpixmap.h>

#include <QtQml/qqmlinfo.h>
#include <QtQml/qqmlengine.h>

#include <QtContacts/qcontactdetails.h>
#include <QtContacts/qcontactmanager.h>
#include <QtContacts/qcontactmanagerengine.h>
#include <QtContacts/qcontactdetailfilter.h>
#include <QtContacts/qcontactidfilter.h>
#include <QtContacts/qcontactintersectionfilter.h>
#include <QtContacts/qcontactrequests.h>

#include <QtVersit/qversitreader.h>
#include <QtVersit/qversitwriter.h>
#include <QtVersit/qversitcontactimporter.h>
#include <QtVersit/qversitcontactexporter.h>

QTCONTACTS_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype ContactModel
    \instantiates QDeclarativeContactModel
    \brief The ContactModel element provides access to contacts from the contacts store.
    \ingroup qml-contacts-main
    \inqmlmodule QtContacts

    This element is part of the \b{QtContacts} module.

    ContactModel provides a model of contacts from the contacts store.
    The contents of the model can be specified with \l filter, \l sortOrders and \l fetchHint properties.
    Whether the model is automatically updated when the store or \l contacts changes, can be
    controlled with \l ContactModel::autoUpdate property.

    There are two ways of accessing the contact data: via model by using views and delegates,
    or alternatively via \l contacts list property. Of the two, the model access is preferred.
    Direct list access (i.e. non-model) is not guaranteed to be in order set by \l sortOrder.

    At the moment the model roles provided by ContactModel are display, decoration and \c contact.
    Through the \c contact role can access any data provided by the Contact element.

    \sa RelationshipModel, Contact, {QContactManager}
*/

// Helper class to store contact binary data into a temporary file
//
// QContactVcard only supports URL values for images. During a vcard import if the contact cotains
// an avatar, ringtone or any property formated in a binary data, QVersit will use the
// ContactExporterResourceHandler to store the binary data.
// The default implementation of QVersitResourceHandler does not store any data and that
// causes avatar data loss during the import process.
// This class will store the data into a temporary file and removes the file when the model gets destroyed.
class ContactExporterResourceHandler : public QVersitResourceHandler
{
public:
    ContactExporterResourceHandler()
    {
    }

    ~ContactExporterResourceHandler()
    {
        foreach (const QString& fileName, m_files)
            QFile::remove(fileName);

        m_files.clear();
    }

    bool saveResource(const QByteArray& contents,
                      const QVersitProperty& property,
                      QString* location)
    {
        const QMimeType mt = QMimeDatabase().mimeTypeForData(contents);
        QString extension(QStringLiteral("data"));
        if (mt.isValid())
            extension = mt.suffixes()[0];

        // use property.name() to create a new file for each binary property (avatar, ringtone, etc...)
        QTemporaryFile tmpFile(QString::fromLatin1("%1/%2_XXXXXX.%3")
                               .arg(QDir::tempPath())
                               .arg(property.name().toLower())
                               .arg(extension));
        tmpFile.setAutoRemove(false);
        if (tmpFile.open()) {
            // the location expect a string in url format ex: file:///tmp/filename.png
            *location = QUrl::fromLocalFile(tmpFile.fileName()).toString();
            m_files << *location;
            tmpFile.write(contents);
            tmpFile.close();
            return true;
        }
        return false;
    }

    bool loadResource(const QString &location, QByteArray *contents, QString *mimeType)
    {
        if (location.isEmpty())
            return false;

        QFile file(location);
        if (!file.open(QIODevice::ReadOnly))
            return false;

        *contents = file.readAll();
        const QMimeType mt = QMimeDatabase().mimeTypeForData(*contents);
        if (mt.isValid())
            *mimeType = mt.suffixes()[0];

        return !contents->isEmpty();
    }

    QStringList m_files;
};


class QDeclarativeContactModelPrivate
{
public:
    QDeclarativeContactModelPrivate()
        :m_manager(0),
        m_fetchHint(0),
        m_filter(0),
        m_error(QContactManager::NoError),
        m_autoUpdate(true),
        m_componentCompleted(false),
        m_progressiveLoading(true),
        m_updatePendingFlag(QDeclarativeContactModelPrivate::NonePending)
    {
    }
    ~QDeclarativeContactModelPrivate()
    {
        if (m_manager)
            delete m_manager;
    }

    enum UpdateTypePending {
        NonePending = 0x0,
        UpdatingContactsPending = 0x1,
        UpdatingCollectionsPending = 0x2
    };

    QList<QDeclarativeContact*> m_contacts;
    QMap<QContactId, QDeclarativeContact*> m_contactMap;
    QMap<QContactId, QDeclarativeContact*> m_contactFetchedMap;
    QContactManager* m_manager;
    QDeclarativeContactFetchHint* m_fetchHint;
    QList<QDeclarativeContactSortOrder*> m_sortOrders;
    QDeclarativeContactFilter* m_filter;

    QVersitReader m_reader;
    QVersitWriter m_writer;
    QStringList m_importProfiles;
    ContactExporterResourceHandler m_resourceHandler;

    QContactManager::Error m_error;

    bool m_autoUpdate;
    bool m_componentCompleted;
    QUrl m_lastExportUrl;
    QUrl m_lastImportUrl;
    QAtomicInt m_lastRequestId;
    QHash<QContactAbstractRequest *, int> m_requestIdHash;
    QList<QContactFetchRequest*> m_pendingRequests;
    QList<QContact> m_pendingContacts;
    QList<QDeclarativeContactCollection*> m_collections;
    bool m_progressiveLoading;
    int m_updatePendingFlag;
};

QDeclarativeContactModel::QDeclarativeContactModel(QObject *parent) :
    QAbstractListModel(parent),
    d(new QDeclarativeContactModelPrivate)
{
    QHash<int, QByteArray> roleNames;
    roleNames = QAbstractItemModel::roleNames();
    roleNames.insert(ContactRole, "contact");
    setRoleNames(roleNames);

    connect(this, SIGNAL(managerChanged()), SLOT(doUpdate()));
    connect(this, SIGNAL(filterChanged()), SLOT(doContactUpdate()));
    connect(this, SIGNAL(fetchHintChanged()), SLOT(doContactUpdate()));
    connect(this, SIGNAL(sortOrdersChanged()), SLOT(doContactUpdate()));

    //import vcard
    connect(&d->m_reader, SIGNAL(stateChanged(QVersitReader::State)), this, SLOT(startImport(QVersitReader::State)));
    connect(&d->m_writer, SIGNAL(stateChanged(QVersitWriter::State)), this, SLOT(contactsExported(QVersitWriter::State)));
}

QDeclarativeContactModel::~QDeclarativeContactModel()
{
}

/*!
  \qmlproperty string ContactModel::manager

  This property holds the manager uri of the contact backend engine.
  */
QString QDeclarativeContactModel::manager() const
{
    if (d->m_manager)
        return d->m_manager->managerName();
    return QString();
}
void QDeclarativeContactModel::setManager(const QString& managerName)
{
    if (d->m_manager && (managerName == d->m_manager->managerName() || managerName == d->m_manager->managerUri()))
        return;

    if (d->m_manager) {
        cancelUpdate();
        delete d->m_manager;
    }

    d->m_manager = new QContactManager(managerName);

    connect(d->m_manager, SIGNAL(dataChanged()), this, SLOT(doUpdate()));
    connect(d->m_manager, SIGNAL(contactsAdded(QList<QContactId>)), this, SLOT(onContactsAdded(QList<QContactId>)));
    connect(d->m_manager, SIGNAL(contactsRemoved(QList<QContactId>)), this, SLOT(onContactsRemoved(QList<QContactId>)));
    connect(d->m_manager, SIGNAL(contactsChanged(QList<QContactId>,QList<QContactDetail::DetailType>)), this, SLOT(onContactsChanged(QList<QContactId>)));
    connect(d->m_manager, SIGNAL(collectionsAdded(QList<QContactCollectionId>)), this, SLOT(fetchCollections()));
    connect(d->m_manager, SIGNAL(collectionsChanged(QList<QContactCollectionId>)), this, SLOT(fetchCollections()));
    connect(d->m_manager, SIGNAL(collectionsRemoved(QList<QContactCollectionId>)), this, SLOT(fetchCollections()));


    if (d->m_error != QContactManager::NoError) {
        d->m_error = QContactManager::NoError;
        emit errorChanged();
    }

    emit managerChanged();
}

void QDeclarativeContactModel::componentComplete()
{
    if (!d->m_manager)
        setManager(QString());

    d->m_componentCompleted = true;

    if (d->m_autoUpdate)
        update();
}
/*!
  \qmlproperty bool ContactModel::autoUpdate

  This property indicates whether or not the contact model should be updated automatically, default value is true.
  */
void QDeclarativeContactModel::setAutoUpdate(bool autoUpdate)
{
    if (autoUpdate == d->m_autoUpdate)
        return;
    d->m_autoUpdate = autoUpdate;
    emit autoUpdateChanged();
}

bool QDeclarativeContactModel::autoUpdate() const
{
    return d->m_autoUpdate;
}

void QDeclarativeContactModel::update()
{
    if (!d->m_componentCompleted || d->m_updatePendingFlag)
        return;
    // Disallow possible duplicate request triggering
    d->m_updatePendingFlag = (QDeclarativeContactModelPrivate::UpdatingContactsPending | QDeclarativeContactModelPrivate::UpdatingCollectionsPending);
    QMetaObject::invokeMethod(this, "fetchCollections", Qt::QueuedConnection);
}

/*!
  \qmlmethod ContactModel::updateContacts()

  Manually update the contact model contacts.

  \sa ContactModel::update
  \sa ContactModel::updateCollections
  \sa ContactModel::autoUpdate
  */
void QDeclarativeContactModel::updateContacts()
{
    if (!d->m_componentCompleted || d->m_updatePendingFlag)
        return;
    // Disallow possible duplicate request triggering
    d->m_updatePendingFlag = QDeclarativeContactModelPrivate::UpdatingContactsPending;
    QMetaObject::invokeMethod(this, "fetchAgain", Qt::QueuedConnection);
}

/*!
  \qmlmethod ContactModel::updateCollections()

  Manually update the contact model collections.

  \sa ContactModel::update
  \sa ContactModel::updateContacts
  \sa ContactModel::autoUpdate
  */
void QDeclarativeContactModel::updateCollections()
{
    if (!d->m_componentCompleted || d->m_updatePendingFlag)
        return;
    // Disallow possible duplicate request triggering
    d->m_updatePendingFlag = QDeclarativeContactModelPrivate::UpdatingCollectionsPending;
    QMetaObject::invokeMethod(this, "fetchCollections", Qt::QueuedConnection);
}

/*!
  \qmlmethod ContactModel::cancelUpdate()

  Cancel the running contact model content update request.

  \sa ContactModel::autoUpdate
  \sa ContactModel::update
  */
void QDeclarativeContactModel::cancelUpdate()
{
    foreach (QContactFetchRequest *req, d->m_pendingRequests) {
        req->cancel();
        req->deleteLater();
    }
    d->m_pendingRequests.clear();;
    d->m_updatePendingFlag = QDeclarativeContactModelPrivate::NonePending;
}

void QDeclarativeContactModel::doContactUpdate()
{
    if (d->m_autoUpdate)
        updateContacts();
}

/*!
  \qmlproperty string ContactModel::error

  This property holds the latest error code returned by the contact manager.

  This property is read only.
  */
QString QDeclarativeContactModel::error() const
{
    if (d->m_manager) {
        switch (d->m_error) {
        case QContactManager::DoesNotExistError:
            return QStringLiteral("DoesNotExist");
        case QContactManager::AlreadyExistsError:
            return QStringLiteral("AlreadyExists");
        case QContactManager::InvalidDetailError:
            return QStringLiteral("InvalidDetail");
        case QContactManager::InvalidRelationshipError:
            return QStringLiteral("InvalidRelationship");
        case QContactManager::LockedError:
            return QStringLiteral("LockedError");
        case QContactManager::DetailAccessError:
            return QStringLiteral("DetailAccessError");
        case QContactManager::PermissionsError:
            return QStringLiteral("PermissionsError");
        case QContactManager::OutOfMemoryError:
            return QStringLiteral("OutOfMemory");
        case QContactManager::NotSupportedError:
            return QStringLiteral("NotSupported");
        case QContactManager::BadArgumentError:
            return QStringLiteral("BadArgument");
        case QContactManager::UnspecifiedError:
            return QStringLiteral("UnspecifiedError");
        case QContactManager::VersionMismatchError:
            return QStringLiteral("VersionMismatch");
        case QContactManager::LimitReachedError:
            return QStringLiteral("LimitReached");
        case QContactManager::InvalidContactTypeError:
            return QStringLiteral("InvalidContactType");
        default:
            break;
        }
    }
    return QStringLiteral("NoError");
}


/*!
  \qmlproperty list<string> ContactModel::availableManagers

  This property holds the list of available manager names.
  This property is read only.
  */
QStringList QDeclarativeContactModel::availableManagers() const
{
    return QContactManager::availableManagers();
}
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

/*!
  \qmlproperty enumeration ContactModel::ImportError

  Defines the errors cases for \l ContactModel::importContacts() -function.

  \list
  \li ContactModel::ImportNoError             Completed successfully, no error.
  \li ContactModel::ImportUnspecifiedError    Unspecified error.
  \li ContactModel::ImportIOError             Input/output error.
  \li ContactModel::ImportOutOfMemoryError    Out of memory error.
  \li ContactModel::ImportNotReadyError       Not ready for importing. Only one import operation can be active at a time.
  \li ContactModel::ImportParseError          Error during parsing.
  \endlist
*/

/*!
  \qmlsignal ContactModel::onImportCompleted(ImportError error, URL url, list<string> ids)

  This signal is emitted, when \l ContactModel::importContacts() completes. The success of operation
  can be seen on \a error which is defined in \l ContactModel::ImportError. \a url indicates the
  file, which was imported. \a ids contains the imported contacts ids.

  If the operation was successful, contacts are now imported to backend. If \l ContactModel::autoUpdate
  is enabled, \l ContactModel::modelChanged will be emitted when imported contacts are also visible on
  \l ContactModel's data model.

  \sa ContactModel::importContacts
 */

/*!
  \qmlmethod void ContactModel::importContacts(url url, list<string> profiles)

  Import contacts from a vcard by the given \a url and optional \a profiles.
  Only one import operation can be active at a time.
  Supported profiles are:
  \list
  \li "Sync"  Imports contacts in sync mode, currently, this is the same as passing in an empty list, and is generally what you want.
  \li "Backup" imports contacts in backup mode, use this mode if the vCard was generated by exporting in backup mode.

  \endlist

  \sa QVersitContactHandlerFactory
  \sa QVersitContactHandlerFactory::ProfileSync()
  \sa QVersitContactHandlerFactory::ProfileBackup()

  */
void QDeclarativeContactModel::importContacts(const QUrl& url, const QStringList& profiles)
{
    // Reader is capable of handling only one request at the time.
    ImportError importError = ImportNotReadyError;
    if (d->m_reader.state() != QVersitReader::ActiveState) {

        d->m_importProfiles = profiles;

        //TODO: need to allow download vcard from network
        QFile*  file = new QFile(urlToLocalFileName(url));
        bool ok = file->open(QIODevice::ReadOnly);
        if (ok) {
            d->m_reader.setDevice(file);
            if (d->m_reader.startReading()) {
                d->m_lastImportUrl = url;
                return;
            }
            importError = QDeclarativeContactModel::ImportError(d->m_reader.error());
        } else {
            importError = ImportIOError;
        }
    }
    emit importCompleted(importError, url, QStringList());
}

/*!
  \qmlmethod void ContactModel::exportContacts(url url, list<string> profiles, list<variant> declarativeContacts)

  Export all contacts of this model into a vcard file to the given \a url by optional \a profiles.
  The optional \a declarativeContacts list can be used to export an arbitrary list of QDeclarativeContact objects
  not necessarily belonging to the data set of this model.
  At the moment only the local file url is supported in export method.
  Also, only one export operation can be active at a time.
  Supported profiles are:
  \list
  \li "Sync"  exports contacts in sync mode, currently, this is the same as passing in an empty list, and is generally what you want.
  \li "Backup" exports contacts in backup mode, this will add non-standard properties to the generated vCard
              to try to save every detail of the contacts. Only use this if the vCard is going to be imported using the backup profile.
#include "moc_qdeclarativecontactmodel_p.cpp"
  \endlist

  \sa QVersitContactHandlerFactory
  \sa QVersitContactHandlerFactory::ProfileSync()
  \sa QVersitContactHandlerFactory::ProfileBackup()
  */
void QDeclarativeContactModel::exportContacts(const QUrl& url, const QStringList& profiles, const QVariantList &declarativeContacts)
{
    // Writer is capable of handling only one request at the time.
    ExportError exportError = ExportNotReadyError;
    if (d->m_writer.state() != QVersitWriter::ActiveState) {
        QString profile = profiles.isEmpty()? QString() : profiles.at(0);
        //only one profile string supported now.
        QVersitContactExporter exporter(profile);
        exporter.setResourceHandler(&d->m_resourceHandler);

        QList<QContact> contacts;
        if (declarativeContacts.isEmpty()) {
            foreach (QDeclarativeContact* dc, d->m_contacts) {
                contacts.append(dc->contact());
            }

        } else {
            foreach (const QVariant &contactVariant, declarativeContacts) {
                QObject *rawObject = contactVariant.value<QObject*>();
                QDeclarativeContact *dc = qobject_cast<QDeclarativeContact*>(rawObject);
                if (dc) {
                    contacts.append(dc->contact());
                }
            }
        }

        exporter.exportContacts(contacts, QVersitDocument::VCard30Type);
        QList<QVersitDocument> documents = exporter.documents();
        QFile* file = new QFile(urlToLocalFileName(url));
        bool ok = file->open(QIODevice::WriteOnly);
        if (ok) {
            d->m_writer.setDevice(file);
            if (d->m_writer.startWriting(documents)) {
                d->m_lastExportUrl = url;
                return;
            }
            exportError = QDeclarativeContactModel::ExportError(d->m_writer.error());
        } else {
            exportError = ExportIOError;
        }
    }
    emit exportCompleted(exportError, url);
}

void QDeclarativeContactModel::contactsExported(QVersitWriter::State state)
{
    if (state == QVersitWriter::FinishedState || state == QVersitWriter::CanceledState) {
         delete d->m_writer.device();
         d->m_writer.setDevice(0);
         emit exportCompleted(QDeclarativeContactModel::ExportError(d->m_writer.error()), d->m_lastExportUrl);
    }
}

void QDeclarativeContactModel::onFetchedContactDestroyed(QObject *obj)
{
    QContactId id = d->m_contactFetchedMap.key(static_cast<QDeclarativeContact*>(obj));
    if (!id.isNull())
        d->m_contactFetchedMap.remove(id);
}

int QDeclarativeContactModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->m_contacts.count();
}



/*!
  \qmlproperty Filter ContactModel::filter

  This property holds the filter instance used by the contact model.

  \sa Filter
  */
QDeclarativeContactFilter* QDeclarativeContactModel::filter() const
{
    return d->m_filter;
}

void QDeclarativeContactModel::setFilter(QDeclarativeContactFilter* filter)
{
    if (d->m_filter != filter) {
        if (d->m_filter)
            disconnect(d->m_filter, SIGNAL(filterChanged()), this, SIGNAL(filterChanged()));
        d->m_filter = filter;
        if (d->m_filter)
            connect(d->m_filter, SIGNAL(filterChanged()), SIGNAL(filterChanged()), Qt::UniqueConnection);
        emit filterChanged();
    }
}

/*!
  \qmlproperty FetchHint ContactModel::fetchHint

  This property holds the fetch hint instance used by the contact model.

  \sa FetchHint
  */
QDeclarativeContactFetchHint* QDeclarativeContactModel::fetchHint() const
{
    return d->m_fetchHint;
}
void QDeclarativeContactModel::setFetchHint(QDeclarativeContactFetchHint* fetchHint)
{
    if (d->m_fetchHint != fetchHint) {
        if (d->m_fetchHint)
            disconnect(d->m_fetchHint, SIGNAL(fetchHintChanged()), this, SIGNAL(fetchHintChanged()));
        d->m_fetchHint = fetchHint;
        if (d->m_fetchHint)
            connect(d->m_fetchHint, SIGNAL(fetchHintChanged()), SIGNAL(fetchHintChanged()), Qt::UniqueConnection);
        emit fetchHintChanged();
    }
}

/*!
  \qmlproperty list<Contact> ContactModel::contacts

  This property holds the list of contacts.

  \sa Contact
  */
QQmlListProperty<QDeclarativeContact> QDeclarativeContactModel::contacts()
{
    return QQmlListProperty<QDeclarativeContact>(this,
                                                         0,
                                                         contacts_append,
                                                         contacts_count,
                                                         contacts_at,
                                                         contacts_clear);
}



void QDeclarativeContactModel::contacts_append(QQmlListProperty<QDeclarativeContact>* prop, QDeclarativeContact* contact)
{
    Q_UNUSED(prop);
    Q_UNUSED(contact);
    qWarning() << Q_FUNC_INFO << "appending contacts is not currently supported";
}

int QDeclarativeContactModel::contacts_count(QQmlListProperty<QDeclarativeContact>* prop)
{
    return static_cast<QDeclarativeContactModel*>(prop->object)->d->m_contacts.count();
}

QDeclarativeContact* QDeclarativeContactModel::contacts_at(QQmlListProperty<QDeclarativeContact>* prop, int index)
{
    return static_cast<QDeclarativeContactModel*>(prop->object)->d->m_contacts.at(index);
}

void QDeclarativeContactModel::contacts_clear(QQmlListProperty<QDeclarativeContact>* prop)
{
    QDeclarativeContactModel* model = static_cast<QDeclarativeContactModel*>(prop->object);
    model->clearContacts();
    emit model->contactsChanged();
}


/*!
  \qmlproperty list<SortOrder> ContactModel::sortOrders

  This property holds a list of sort orders used by the contacts model.
  \sa SortOrder
  */
QQmlListProperty<QDeclarativeContactSortOrder> QDeclarativeContactModel::sortOrders()
{
    return QQmlListProperty<QDeclarativeContactSortOrder>(this,
                                                                  0,
                                                                  sortOrder_append,
                                                                  sortOrder_count,
                                                                  sortOrder_at,
                                                                  sortOrder_clear);
}

void QDeclarativeContactModel::startImport(QVersitReader::State state)
{
    if (state == QVersitReader::FinishedState || state == QVersitReader::CanceledState) {
        QVersitContactImporter importer(d->m_importProfiles);
        importer.setResourceHandler(&d->m_resourceHandler);
        importer.importDocuments(d->m_reader.results());
        QList<QContact> contacts = importer.contacts();

        delete d->m_reader.device();
        d->m_reader.setDevice(0);

        QStringList ids;

        if (d->m_manager) {
            if (!d->m_manager->saveContacts(&contacts)) {
                if (d->m_error != d->m_manager->error()) {
                    d->m_error = d->m_manager->error();
                    emit errorChanged();
                }
            } else {
                foreach (const QContact &c, contacts) {
                    ids << c.id().toString();
                }
            }
        }

        emit importCompleted(QDeclarativeContactModel::ImportError(d->m_reader.error()), d->m_lastImportUrl, ids);
    }
}

/*!
  \qmlsignal ContactModel::contactsFetched(int requestId, list<Contact> fetchedContacts)

  This signal is emitted, when a contact fetch request is finished.

  \sa ContactModel::fetchContacts
 */

/*!
    \qmlmethod int ContactModel::fetchContacts(list<string> contactIds)

    Starts a request to fetch contacts by the given \a contactIds, and returns the unique ID of this request.
    -1 is returned if the request can't be started.

    Note that the contacts fetched won't be added to the model, but can be accessed through the contactsFetched
    signal handler.

    \sa ContactModel::contactsFetched
  */
int QDeclarativeContactModel::fetchContacts(const QStringList &contactIds)
{
    if (contactIds.isEmpty())
        return -1;

    QContactFetchByIdRequest *fetchRequest = new QContactFetchByIdRequest(this);
    connect(fetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)),
            this, SLOT(onFetchContactsRequestStateChanged(QContactAbstractRequest::State)));
    fetchRequest->setManager(d->m_manager);

    QList<QContactId> ids;
    foreach (const QString &contactId, contactIds)
        ids.append(QContactId::fromString(contactId));
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
  \qmlmethod ContactModel::removeCollection(string collectionId)
  Removes asynchronously the contact collection with the given \a collectionId from the backend.
  */
void QDeclarativeContactModel::removeCollection(const QString &collectionId)
{
    QContactCollectionRemoveRequest* req = new QContactCollectionRemoveRequest(this);
    req->setManager(d->m_manager);
    req->setCollectionId(QContactCollectionId::fromString(collectionId));

    connect(req, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(onRequestStateChanged(QContactAbstractRequest::State)));

    req->start();
}

/*!
  \qmlmethod OContactModel::saveCollection(Collection collection)

  Saves asynchronously the given \a collection into the contact backend.
 */
void QDeclarativeContactModel::saveCollection(QDeclarativeContactCollection *declColl)
{
    if (declColl) {
        QContactCollection collection = declColl->collection();
        QContactCollectionSaveRequest* req = new QContactCollectionSaveRequest(this);
        req->setManager(d->m_manager);
        req->setCollection(collection);

        if (declColl->collection().id().isNull()) {
            // if the collection id is empty this means that this is a new collection
            // we need to keep trace of this declarative collection to update with the
            // new Id as soon as this request finish
            QPointer<QDeclarativeContactCollection> pCollection = declColl;
            req->setProperty("DeclarativeCollection", QVariant::fromValue(pCollection));
        }

        connect(req, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(onRequestStateChanged(QContactAbstractRequest::State)));
        req->start();
    }
}

/*!
  \qmlmethod OContactModel::fetchCollections()
  Fetch asynchronously a list of contact collections from the contact backend.
*/
void QDeclarativeContactModel::fetchCollections()
{
    // fetchCollections() is used for both direct calls and
    // signals from model. For signal from model, check also the
    // autoupdate-flag.
    if (sender() == d->m_manager && !d->m_autoUpdate) {
        return;
    }

    QContactCollectionFetchRequest* req = new QContactCollectionFetchRequest(this);
    connect(req,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(collectionsFetched()));
    req->setManager(d->m_manager);
    req->start();
}

/*!
    \internal
 */
void QDeclarativeContactModel::onFetchContactsRequestStateChanged(QContactAbstractRequest::State state)
{
    if (state != QContactAbstractRequest::FinishedState)
        return;

    QContactFetchByIdRequest *request = qobject_cast<QContactFetchByIdRequest *>(sender());
    Q_ASSERT(request);

    checkError(request);

    const int requestId = d->m_requestIdHash.value(request, -1);
    if (requestId == -1)
        qWarning() << Q_FUNC_INFO << "transaction not found from the request hash";
    else
        d->m_requestIdHash.remove(request);
    QVariantList list;
    if (request->error() == QContactManager::NoError) {
        QList<QContact> contacts(request->contacts());
        foreach (const QContact &contact, contacts) {
            // if the contact was already fetched update the contact
            QDeclarativeContact *declarativeContact = d->m_contactFetchedMap.value(contact.id(), 0);
            if (!declarativeContact) {
                declarativeContact = new QDeclarativeContact(this);
                // Transfer the ownership to QML
                // The model will destroy the contact if it get removed from the backend, otherwise the QML side need to destroy it.
                QQmlEngine::setObjectOwnership(declarativeContact, QQmlEngine::JavaScriptOwnership);

                // keep track of contact destruction to remove it from the list if QML destroys it
                connect(declarativeContact, SIGNAL(destroyed(QObject*)), SLOT(onFetchedContactDestroyed(QObject*)));

                // we need keep track of the contact to update it if the contact get update on the backend. or destroy it
                // if the contact get removed from the backend
                d->m_contactFetchedMap[contact.id()] = declarativeContact;
            }
            declarativeContact->setContact(contact);
            list.append(QVariant::fromValue(declarativeContact));
        }
    }
    emit contactsFetched(requestId, list);
    request->deleteLater();
}

/*!
    \internal
 */
void QDeclarativeContactModel::collectionsFetched()
{
    QContactCollectionFetchRequest* req = qobject_cast<QContactCollectionFetchRequest*>(QObject::sender());
    Q_ASSERT(req);
    if (req->isFinished() && QContactManager::NoError == req->error()) {
        d->m_updatePendingFlag &= ~QDeclarativeContactModelPrivate::UpdatingCollectionsPending;
        // prepare tables
        QHash<QString, const QContactCollection*> collections;
        foreach (const QContactCollection& collection, req->collections()) {
            collections.insert(collection.id().toString(), &collection);
        }
        QHash<QString, QDeclarativeContactCollection*> declCollections;
        foreach (QDeclarativeContactCollection* declCollection, d->m_collections) {
            declCollections.insert(declCollection->collection().id().toString(), declCollection);
        }
        // go tables through
        QHashIterator<QString, const QContactCollection*> collIterator(collections);
        while (collIterator.hasNext()) {
            collIterator.next();
            if (declCollections.contains(collIterator.key())) {
                // collection on both sides, update the declarative collection
                declCollections.value(collIterator.key())->setCollection(*collections.value(collIterator.key()));
            } else {
                // new collection, add it to declarative collection list
                QDeclarativeContactCollection* declCollection = new QDeclarativeContactCollection(this);
                declCollection->setCollection(*collections.value(collIterator.key()));
                d->m_collections.append(declCollection);
            }
        }
        QHashIterator<QString, QDeclarativeContactCollection*> declCollIterator(declCollections);
        while (declCollIterator.hasNext()) {
            declCollIterator.next();
            if (!collections.contains(declCollIterator.key())) {
                // collection deleted on the backend side, delete from declarative collection list
                QDeclarativeContactCollection* toBeDeletedColl = declCollections.value(declCollIterator.key());
                d->m_collections.removeOne(toBeDeletedColl);
                toBeDeletedColl->deleteLater();
            }
        }
        emit collectionsChanged();
        if (d->m_updatePendingFlag & QDeclarativeContactModelPrivate::UpdatingContactsPending)
            QMetaObject::invokeMethod(this, "fetchAgain", Qt::QueuedConnection);
        req->deleteLater();
    }
    checkError(req);
}

void QDeclarativeContactModel::clearContacts()
{
    qDeleteAll(d->m_contacts);
    d->m_contacts.clear();
    d->m_contactMap.clear();
    qDeleteAll(d->m_contactFetchedMap.values());
    d->m_contactFetchedMap.clear();
}

void QDeclarativeContactModel::fetchAgain()
{
    QList<QContactSortOrder> sortOrders;
    foreach (QDeclarativeContactSortOrder* so, d->m_sortOrders) {
        sortOrders.append(so->sortOrder());
    }
    QContactFetchRequest* fetchRequest = new QContactFetchRequest(this);

    fetchRequest->setManager(d->m_manager);
    fetchRequest->setSorting(sortOrders);

    if (d->m_filter){
        fetchRequest->setFilter(d->m_filter->filter());
    } else {
        fetchRequest->setFilter(QContactFilter());
    }

    fetchRequest->setFetchHint(d->m_fetchHint ? d->m_fetchHint->fetchHint() : QContactFetchHint());

    connect(fetchRequest, SIGNAL(resultsAvailable()), this, SLOT(requestUpdated()));
    connect(fetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)),
            this, SLOT(fetchRequestStateChanged(QContactAbstractRequest::State)));

    // cancel all previous requests
    foreach (QContactFetchRequest *req, d->m_pendingRequests) {
        req->cancel();
        req->deleteLater();
    }

    d->m_pendingContacts.clear();
    d->m_pendingRequests.clear();
    d->m_pendingRequests.append(fetchRequest);

    // if we have no contacts yet, we can display results as soon as they arrive
    // but if we are updating the model after a sort or filter change, we have to
    // wait for all contacts before processing the update
    d->m_progressiveLoading = d->m_contacts.isEmpty();

    fetchRequest->start();
}

void QDeclarativeContactModel::requestUpdated()
{

    QContactFetchRequest* req = qobject_cast<QContactFetchRequest*>(QObject::sender());
    Q_ASSERT(req);
    if (req) {
        QList<QContact> contacts = req->contacts();

        // if we are starting from scratch, we can show contact results as they arrive
        if (d->m_progressiveLoading) {
            QList<QDeclarativeContact*> dcs;
            foreach (const QContact &c, contacts) {
                if (d->m_contactMap.contains(c.id())) {
                    QDeclarativeContact* dc = d->m_contactMap.value(c.id());
                    dc->setContact(c);
                } else {
                    QDeclarativeContact* dc = new QDeclarativeContact(this);
                    if (dc) {
                        d->m_contactMap.insert(c.id(), dc);
                        dc->setContact(c);
                        dcs.append(dc);
                    }
                }
            }

            if (dcs.count() > 0) {
                beginInsertRows(QModelIndex(), d->m_contacts.count(), d->m_contacts.count() + dcs.count() - 1);
                // At this point we need to relay on the backend and assume that the partial results are following the fetch sorting property
                d->m_contacts += dcs;
                endInsertRows();

                emit contactsChanged();
            }
        } else {
            d->m_pendingContacts << contacts;
        }

        checkError(req);
    }
}

void QDeclarativeContactModel::fetchRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState != QContactAbstractRequest::FinishedState)
        return;

    d->m_updatePendingFlag &= ~QDeclarativeContactModelPrivate::UpdatingContactsPending;
    QContactFetchRequest* req = qobject_cast<QContactFetchRequest*>(QObject::sender());
    Q_ASSERT(req);
    if (req) {
        // if we were not processing contacts as soon as they arrive, we need to process them here.
        if (!d->m_progressiveLoading) {
            // start by removing the contacts that don't belong to this result set anymore
            for (int i = d->m_contacts.count()-1; i >= 0; --i) {
                QDeclarativeContact *contact = d->m_contacts[i];
                if (!d->m_pendingContacts.contains(contact->contact())) {
                    beginRemoveRows(QModelIndex(), i, i);
                    d->m_contacts.removeAt(i);
                    d->m_contactMap.remove(contact->contact().id());
                    endRemoveRows();
                }
            }

            // now insert new contacts and move existing ones to their final positions
            int count = d->m_pendingContacts.count();
            for (int i = 0; i < count; ++i) {
                QContact c = d->m_pendingContacts[i];
                if (!d->m_contactMap.contains(c.id())) {
                    QDeclarativeContact* dc = new QDeclarativeContact(this);
                    dc->setContact(c);
                    beginInsertRows(QModelIndex(), i, i);
                    d->m_contacts.insert(i, dc);
                    d->m_contactMap.insert(c.id(),dc);
                    endInsertRows();
                } else {
                    QDeclarativeContact *contact = d->m_contactMap[c.id()];

                    // If there are duplicates in the pending contacts list, then the current index
                    // can be outside this contact lists range and we need to adjust it to avoid crashing.
                    const int oldIdx = d->m_contacts.indexOf(contact);
                    const int newIdx = i < d->m_contacts.size() ? i : d->m_contacts.size() - 1;
                    if (oldIdx != newIdx) {
                        beginMoveRows(QModelIndex(), oldIdx, oldIdx, QModelIndex(), newIdx);
                        d->m_contacts.move(oldIdx, newIdx);
                        endMoveRows();
                    }
                }
            }
            emit contactsChanged();
        }

        // and now clear the pending contact list as the model is up-to-date
        d->m_pendingContacts.clear();
        d->m_pendingRequests.removeOne(req);
        req->deleteLater();
    }
}

/*!
    \internal
 */
void QDeclarativeContactModel::doUpdate()
{
    if (d->m_autoUpdate)
        update();
}

/*!
  \qmlmethod ContactModel::saveContact(Contact contact)

  Save the given \a contact into the contacts backend.
  Once saved successfully, the dirty flags of this contact will be reset.

  \sa Contact::modified
  */
void QDeclarativeContactModel::saveContact(QDeclarativeContact* dc)
{
    if (dc) {
        QContactSaveRequest* req = new QContactSaveRequest(this);
        req->setManager(d->m_manager);
        req->setContact(dc->contact());
        if (dc->contact().id().isNull()) {
            // if the contact id is empty this means that this contact is a new contact
            // we need to keep trace of this declarative contact to update with the
            // new Id as soon as this request finish
            QPointer<QDeclarativeContact> pContact = dc;
            req->setProperty("DeclarativeContact", QVariant::fromValue(pContact));
        }

        connect(req,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(onRequestStateChanged(QContactAbstractRequest::State)));
        req->start();
    }
}

void QDeclarativeContactModel::onRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState != QContactAbstractRequest::FinishedState) {
        return;
    }

    QContactAbstractRequest *request = qobject_cast<QContactAbstractRequest *>(sender());
    Q_ASSERT(request);

    if (request->error() == QContactManager::NoError) {
        switch (request->type()) {
        case QContactAbstractRequest::ContactSaveRequest:
        {
            QVariant vContact = request->property("DeclarativeContact");
            if (vContact.isValid()) {
                QPointer<QDeclarativeContact> pContact = vContact.value<QPointer<QDeclarativeContact> >();
                // Update contact info.
                // this is necessary to make sure that the declarative contact get the new contact ID otherwise
                // the contact Id will be empty
                QList<QContact> contacts = qobject_cast<QContactSaveRequest*>(request)->contacts();
                if (pContact && contacts.length() == 1) {
                    pContact->setContact(contacts[0]);
                }
            }
            break;
        }
        case QContactAbstractRequest::CollectionSaveRequest:
        {
            QVariant vCollection = request->property("DeclarativeCollection");
            if (vCollection.isValid()) {
                QPointer<QDeclarativeContactCollection> pCollection = vCollection.value<QPointer<QDeclarativeContactCollection> >();
                // Update collection info.
                // this is necessary to make sure that the declarative collection get the new collection ID otherwise
                // the collection Id will be empty
                QList<QContactCollection> collections = qobject_cast<QContactCollectionSaveRequest*>(request)->collections();
                if (pCollection && collections.length() == 1) {
                    pCollection->setCollection(collections[0]);
                }
            }
            break;
        }
        default:
            break;
        }
    }
    checkError(request);
    request->deleteLater();
}

void QDeclarativeContactModel::checkError(const QContactAbstractRequest *request)
{
    if (request) {
        updateError(request->error());
    }
}

void QDeclarativeContactModel::updateError(QContactManager::Error error)
{
    if (d->m_error != error) {
        d->m_error = error;
        emit errorChanged();
    }
}

void QDeclarativeContactModel::onContactsAdded(const QList<QContactId>& ids)
{
    if (d->m_autoUpdate && !ids.isEmpty()) {
        QContactFetchRequest *fetchRequest = createContactFetchRequest(ids);
        connect(fetchRequest,SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(onContactsAddedFetchRequestStateChanged(QContactAbstractRequest::State)));
        fetchRequest->start();
    }
}

/*!
  \qmlmethod ContactModel::removeContact(string contactId)
  Remove the contact from the contacts store by given \a contactId.
  After removing a contact it is not possible to save it again.
  \sa Contact::contactId
  */
void QDeclarativeContactModel::removeContact(QString id)
{
    QList<QString> ids;
    ids << id;
    removeContacts(ids);
}

/*!
  \qmlmethod ContactModel::removeContacts(list<string> contactIds)
  Remove the list of contacts from the contacts store by given \a contactIds.
  \sa Contact::contactId
  */

void QDeclarativeContactModel::removeContacts(const QStringList &ids)
{
    QContactRemoveRequest* req = new QContactRemoveRequest(this);
    QList<QContactId> contactIdsAsList;
    req->setManager(d->m_manager);

    foreach (const QString& id, ids) {
        QContactId contactId = QContactId::fromString(id);
        if (!contactId.isNull())
            contactIdsAsList.append(contactId);
    }
    req->setContactIds(contactIdsAsList);

    connect(req,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(onRequestStateChanged(QContactAbstractRequest::State)));

    req->start();
}


void QDeclarativeContactModel::onContactsRemoved(const QList<QContactId> &ids)
{
    if (!d->m_autoUpdate)
        return;

    bool emitSignal = false;
    foreach (const QContactId &id, ids) {
        // delete the contact from fetched map if necessary
        QDeclarativeContact* contact = d->m_contactFetchedMap.take(id);
        if (contact)
            contact->deleteLater();

        if (d->m_contactMap.contains(id)) {
            int row = 0;
            //TODO:need a fast lookup
            for (; row < d->m_contacts.count(); row++) {
                if (d->m_contacts.at(row)->contactId() == id.toString())
                    break;
            }

            if (row < d->m_contacts.count()) {
                beginRemoveRows(QModelIndex(), row, row);
                contact = d->m_contacts.takeAt(row);
                contact->deleteLater();
                d->m_contactMap.remove(id);
                endRemoveRows();
                emitSignal = true;
            }
        }
    }
    if (emitSignal)
        emit contactsChanged();
}

void QDeclarativeContactModel::onContactsChanged(const QList<QContactId> &ids)
{
    if (d->m_autoUpdate && !ids.isEmpty()) {
        QContactFetchRequest *fetchRequest = createContactFetchRequest(ids);
        connect(fetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(onContactsChangedFetchRequestStateChanged(QContactAbstractRequest::State)));
        fetchRequest->start();
    }

    // If any contact in the fetchedList has changed we need to update it.
    // We need a different query because feched contacts could not be part of the model.
    //
    // For example: if the model contains a filter
    if (!ids.isEmpty()) {
        QStringList pendingFetch;
        foreach (const QContactId &id, ids) {
            QDeclarativeContact* dc = d->m_contactFetchedMap.value(id);
            if (dc)
                pendingFetch << dc->contactId();
        }
        if (!pendingFetch.isEmpty())
            fetchContacts(pendingFetch);
    }
}

QContactFetchRequest *QDeclarativeContactModel::createContactFetchRequest(const QList<QContactId> &ids)
{
    QContactFetchRequest *fetchRequest = new QContactFetchRequest(this);
    fetchRequest->setManager(d->m_manager);
    fetchRequest->setFetchHint(d->m_fetchHint ? d->m_fetchHint->fetchHint() : QContactFetchHint());

    QContactIdFilter idFilter;
    idFilter.setIds(ids);
    if (d->m_filter) {
        QContactIntersectionFilter filter;
        filter.append(idFilter); // result handling assumes that id filter is the first filter
        filter.append(d->m_filter->filter());
        fetchRequest->setFilter(filter);
    } else
        fetchRequest->setFilter(idFilter);
    return fetchRequest;
}

QVariant QDeclarativeContactModel::data(const QModelIndex &index, int role) const
{
    //Check if QList itme's index is valid before access it, index should be between 0 and count - 1
    if (index.row() < 0 || index.row() >= d->m_contacts.count()) {
        return QVariant();
    }

    QDeclarativeContact* dc = d->m_contacts.value(index.row());
    Q_ASSERT(dc);
    QContact c = dc->contact();

    switch(role) {
        case Qt::DisplayRole:
             return c.detail(QContactDetail::TypeDisplayLabel).value(QContactDisplayLabel::FieldLabel);
        case Qt::DecorationRole:
            return QPixmap();
        case ContactRole:
            return QVariant::fromValue(dc);
    }
    return QVariant();
}


void QDeclarativeContactModel::sortOrder_append(QQmlListProperty<QDeclarativeContactSortOrder> *p, QDeclarativeContactSortOrder *sortOrder)
{
    QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(p->object);
    if (model && sortOrder) {
        QObject::connect(sortOrder, SIGNAL(sortOrderChanged()), model, SIGNAL(sortOrdersChanged()));
        model->d->m_sortOrders.append(sortOrder);
        emit model->sortOrdersChanged();
    }
}

int  QDeclarativeContactModel::sortOrder_count(QQmlListProperty<QDeclarativeContactSortOrder> *p)
{
    QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(p->object);
    if (model)
        return model->d->m_sortOrders.size();
    return 0;
}
QDeclarativeContactSortOrder * QDeclarativeContactModel::sortOrder_at(QQmlListProperty<QDeclarativeContactSortOrder> *p, int idx)
{
    QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(p->object);

    QDeclarativeContactSortOrder* sortOrder = 0;
    if (model) {
        int i = 0;
        foreach (QDeclarativeContactSortOrder* s, model->d->m_sortOrders) {
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
void  QDeclarativeContactModel::sortOrder_clear(QQmlListProperty<QDeclarativeContactSortOrder> *p)
{
    QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(p->object);

    if (model) {
        model->d->m_sortOrders.clear();
        emit model->sortOrdersChanged();
    }
}

/*!
  \qmlproperty list<Collection> OContactModel::collections

  This property holds a list of collections in the contact model.

  \sa Collection
  */
QQmlListProperty<QDeclarativeContactCollection> QDeclarativeContactModel::collections()
{
    return QQmlListProperty<QDeclarativeContactCollection>(this, 0, collection_count, collection_at);
}

int QDeclarativeContactModel::collection_count(QQmlListProperty<QDeclarativeContactCollection> *p)
{
    QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(p->object);
    return model ? model->d->m_collections.count() : 0;
}

QDeclarativeContactCollection *QDeclarativeContactModel::collection_at(QQmlListProperty<QDeclarativeContactCollection> *p, int idx)
{
    QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(p->object);
    QDeclarativeContactCollection* collection = 0;
    if (model) {
        if (!model->d->m_collections.isEmpty() && idx >= 0 && idx < model->d->m_collections.count())
            collection = model->d->m_collections.at(idx);
    }
    return collection;
}
/*!
    \internal

    It's invoked by the fetch request from onContactsAdded().
 */
void QDeclarativeContactModel::onContactsAddedFetchRequestStateChanged(QContactAbstractRequest::State state)
{

    if (state != QContactAbstractRequest::FinishedState)
        return;
    QContactFetchRequest *request = qobject_cast<QContactFetchRequest *>(sender());
    Q_ASSERT(request);

    checkError(request);

    if (request->error() == QContactManager::NoError) {
        QList<QContact> fetchedContacts(request->contacts());
        bool contactsAdded = false;
        foreach (const QContact &c,fetchedContacts) {
            if (d->m_contactMap.contains(c.id())) {
                qWarning() <<Q_FUNC_INFO <<"contact to be added already exists in the model";
                continue;
            }
            QDeclarativeContact* dc = new QDeclarativeContact(this);
            dc->setContact(c);
            int index = contactIndex(dc);
            beginInsertRows(QModelIndex(), index, index);
            d->m_contacts.insert(index, dc);
            d->m_contactMap.insert(c.id(), dc);
            if (!contactsAdded)
                contactsAdded = true;
            endInsertRows();
        }
        if (contactsAdded)
            emit contactsChanged();
    }
    request->deleteLater();
}


static bool contactListDoesNotContainContactWithId(const QList<QContact> &contactList, const QContactId &contactId) {
    foreach (const QContact &contact, contactList) {
        if (contact.id() == contactId)
            return false;
    }
    return true;
}

/*!
    \internal

    It's invoked by the fetch request from onContactsChanged().
 */
void QDeclarativeContactModel::onContactsChangedFetchRequestStateChanged(QContactAbstractRequest::State state)
{
    if (state != QContactAbstractRequest::FinishedState)
        return;

    QContactFetchRequest *request = qobject_cast<QContactFetchRequest *>(sender());
    Q_ASSERT(request);

    checkError(request);
    bool contactsUpdated = false;
    if (request->error() == QContactManager::NoError || request->error() == QContactManager::DoesNotExistError) {
        QList<QContact> fetchedContacts(request->contacts());
        QList<QContactId> requestedContactIds;
        //read requested contacts ids from the filter
        if (request->filter().type() == QContactFilter::IdFilter) {
            QContactIdFilter idFilter(request->filter());
            requestedContactIds = idFilter.ids();
        } else {
            QContactIntersectionFilter intersectionFilter(request->filter());
            QContactIdFilter idFilter(intersectionFilter.filters().at(0)); // assuming that id filter is the first filter
            requestedContactIds = idFilter.ids();
        }
        //handle updated contacts which needs removal from model
        //all contacts requested but not received are removed
        foreach (const QContactId &id, requestedContactIds) {
            if (contactListDoesNotContainContactWithId(fetchedContacts, id)) {
                for (int i=0;i<d->m_contacts.size();++i) {
                    if (d->m_contacts.at(i)->contactId() == id.toString()) {
                        beginRemoveRows(QModelIndex(), i, i);
                        // Remove and delete contact object
                        QDeclarativeContact* dc = d->m_contacts.takeAt(i);
                        dc->deleteLater();
                        d->m_contactMap.remove(id);
                        endRemoveRows();
                        contactsUpdated = true;
                    }
                }
            }
        }
        foreach (const QContact &fetchedContact, fetchedContacts) {
            QString contactIdString(fetchedContact.id().toString());
            bool fetchedContactFound = false;
            for (int i = 0; i < d->m_contacts.size(); ++i) {
                //handle updated contacts which should be updated in the model
                if (d->m_contacts.at(i)->contactId() == contactIdString) {
                    QDeclarativeContact* dc = d->m_contacts.at(i);
                    dc->setContact(fetchedContact);

                    // Since the contact can change the position due the sort order we need take care of it
                    // First we need to remove it from previous position and notify the model about that
                    beginRemoveRows(QModelIndex(), i, i);
                    d->m_contactMap.remove(fetchedContact.id());
                    d->m_contacts.removeAt(i);
                    endRemoveRows();

                    // Calculate the new position
                    int index = contactIndex(dc);
                    // Notify the model about the new item position
                    beginInsertRows(QModelIndex(), index, index);
                    d->m_contacts.insert(index, dc);
                    d->m_contactMap.insert(fetchedContact.id(),dc);
                    if (!contactsUpdated)
                        contactsUpdated = true;
                    endInsertRows();

                    fetchedContactFound = true;
                    break;
                }
            }
            //handle updated contacts which needs to be added in the model
            if (!fetchedContactFound) {
                QDeclarativeContact* dc = new QDeclarativeContact(this);
                dc->setContact(fetchedContact);
                int index = contactIndex(dc);
                beginInsertRows(QModelIndex(), index, index);
                d->m_contacts.insert(index, dc);
                d->m_contactMap.insert(fetchedContact.id(),dc);
                contactsUpdated = true;
                endInsertRows();
            }
        }
    }

    if (contactsUpdated)
        emit contactsChanged();

    request->deleteLater();
}

int QDeclarativeContactModel::contactIndex(const QDeclarativeContact* contact)
{
    if (d->m_sortOrders.count() > 0) {
        QList<QContactSortOrder> mSortOrders;
        foreach (QDeclarativeContactSortOrder *sortOrder, d->m_sortOrders)
            mSortOrders.append(sortOrder->sortOrder());
        for (int i = 0; i < d->m_contacts.size(); i++) {
            // check to see if the new contact should be inserted here
            int comparison = QContactManagerEngine::compareContact(d->m_contacts.at(i)->contact(),
                                                                   contact->contact(),
                                                                   mSortOrders);
            //if the contacts are equal or cannot be compared
            //we return the current position.The default case is if the new contact
            //should appear before the compared contact in m_contacts
            if (comparison >= 0)
                return i;
        }
    }
    return d->m_contacts.size();
}

#include "moc_qdeclarativecontactmodel_p.cpp"

QT_END_NAMESPACE
