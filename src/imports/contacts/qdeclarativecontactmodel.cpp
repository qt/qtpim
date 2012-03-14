/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <qcontactdetails.h>
#include <QtQml/qqmlinfo.h>

#include "qdeclarativecontactmodel_p.h"
#include "qcontactmanager.h"
#include "qcontactmanagerengine.h"
#include "qcontactdetailfilter.h"
#include "qcontactidfilter.h"
#include "qcontactintersectionfilter.h"
#include "qversitreader.h"
#include "qversitwriter.h"
#include "qversitcontactimporter.h"
#include "qversitcontactexporter.h"
#include <QColor>
#include <QHash>
#include <QPixmap>
#include <QFile>
#include <QMap>
#include <QUrl>

#include "qcontactrequests.h"

/*!
    \qmlclass ContactModel QDeclarativeContactModel
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

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactModelPrivate
{
public:
    QDeclarativeContactModelPrivate()
        :m_manager(0),
        m_fetchHint(0),
        m_filter(0),
        m_error(QContactManager::NoError),
        m_autoUpdate(true),
        m_componentCompleted(false)
    {
    }
    ~QDeclarativeContactModelPrivate()
    {
        if (m_manager)
            delete m_manager;
    }

    QList<QDeclarativeContact*> m_contacts;
    QMap<QContactId, QDeclarativeContact*> m_contactMap;
    QContactManager* m_manager;
    QDeclarativeContactFetchHint* m_fetchHint;
    QList<QDeclarativeContactSortOrder*> m_sortOrders;
    QDeclarativeContactFilter* m_filter;

    QVersitReader m_reader;
    QVersitWriter m_writer;
    QStringList m_importProfiles;

    QContactManager::Error m_error;

    bool m_autoUpdate;
    bool m_componentCompleted;
    QUrl m_lastExportUrl;
    QUrl m_lastImportUrl;
    QAtomicInt m_lastRequestId;
    QHash<QContactAbstractRequest *, int> m_requestIdHash;
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
    connect(this, SIGNAL(filterChanged()), SLOT(doUpdate()));
    connect(this, SIGNAL(fetchHintChanged()), SLOT(doUpdate()));
    connect(this, SIGNAL(sortOrdersChanged()), SLOT(doUpdate()));
    
    //import vcard
    connect(&d->m_reader, SIGNAL(stateChanged(QVersitReader::State)), this, SLOT(startImport(QVersitReader::State)));
    connect(&d->m_writer, SIGNAL(stateChanged(QVersitWriter::State)), this, SLOT(contactsExported(QVersitWriter::State)));
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
    if (d->m_manager)
        delete d->m_manager;


    d->m_manager = new QContactManager(managerName);

    connect(d->m_manager, SIGNAL(dataChanged()), this, SLOT(update()));
    connect(d->m_manager, SIGNAL(contactsAdded(QList<QContactId>)), this, SLOT(onContactsAdded(QList<QContactId>)));
    connect(d->m_manager, SIGNAL(contactsRemoved(QList<QContactId>)), this, SLOT(onContactsRemoved(QList<QContactId>)));
    connect(d->m_manager, SIGNAL(contactsChanged(QList<QContactId>)), this, SLOT(onContactsChanged(QList<QContactId>)));

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
    if (!d->m_componentCompleted)
        return;
    QMetaObject::invokeMethod(this, "fetchAgain", Qt::QueuedConnection);
}

void QDeclarativeContactModel::doUpdate()
{
    if (d->m_autoUpdate)
        update();
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
            return QLatin1String("DoesNotExist");
        case QContactManager::AlreadyExistsError:
            return QLatin1String("AlreadyExists");
        case QContactManager::InvalidDetailError:
            return QLatin1String("InvalidDetail");
        case QContactManager::InvalidRelationshipError:
            return QLatin1String("InvalidRelationship");
        case QContactManager::LockedError:
            return QLatin1String("LockedError");
        case QContactManager::DetailAccessError:
            return QLatin1String("DetailAccessError");
        case QContactManager::PermissionsError:
            return QLatin1String("PermissionsError");
        case QContactManager::OutOfMemoryError:
            return QLatin1String("OutOfMemory");
        case QContactManager::NotSupportedError:
            return QLatin1String("NotSupported");
        case QContactManager::BadArgumentError:
            return QLatin1String("BadArgument");
        case QContactManager::UnspecifiedError:
            return QLatin1String("UnspecifiedError");
        case QContactManager::VersionMismatchError:
            return QLatin1String("VersionMismatch");
        case QContactManager::LimitReachedError:
            return QLatin1String("LimitReached");
        case QContactManager::InvalidContactTypeError:
            return QLatin1String("InvalidContactType");
        default:
            break;
        }
    }
    return QLatin1String("NoError");
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
  \qmlmethod void ContactModel::importContacts(url url, list<string> profiles)

  Import contacts from a vcard by the given \a url and optional \a profiles.
  Only one import operation can be active at a time.
  Supported profiles are:
  \list
  \li "Sync"  Imports contacts in sync mode, currently, this is the same as passing in an empty list, and is generally what you want.
  \li "Backup" imports contacts in backup mode, use this mode if the vCard was generated by exporting in backup mode.

  \endlist

  \sa QVersitContactHandlerFactory
  \sa QVersitContactHandlerFactory::ProfileSync
  \sa QVersitContactHandlerFactory::ProfileBackup

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
    emit importCompleted(importError, url);
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
  \sa QVersitContactHandlerFactory::ProfileSync
  \sa QVersitContactHandlerFactory::ProfileBackup
  */
void QDeclarativeContactModel::exportContacts(const QUrl& url, const QStringList& profiles, const QVariantList &declarativeContacts)
{
    // Writer is capable of handling only one request at the time.
    ExportError exportError = ExportNotReadyError;
    if (d->m_writer.state() != QVersitWriter::ActiveState) {
        QString profile = profiles.isEmpty()? QString() : profiles.at(0);
        //only one profile string supported now.
        QVersitContactExporter exporter(profile);

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
        if (d->m_filter) {
            disconnect(d->m_filter, SIGNAL(filterChanged()), this, SLOT(update()));
        }
        d->m_filter = filter;
        if (d->m_filter) {
            connect(d->m_filter, SIGNAL(filterChanged()), SLOT(update()), Qt::UniqueConnection);
        }
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
    if (fetchHint && fetchHint != d->m_fetchHint) {
        if (d->m_fetchHint)
            delete d->m_fetchHint;
        d->m_fetchHint = fetchHint;
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
    qmlInfo(0) << tr("ContactModel: appending contacts is not currently supported");
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
        importer.importDocuments(d->m_reader.results());
        QList<QContact> contacts = importer.contacts();

        delete d->m_reader.device();
        d->m_reader.setDevice(0);

        if (d->m_manager) {
            if (d->m_manager->saveContacts(&contacts)) {
                qmlInfo(this) << tr("contacts imported.");
            } else {
                if (d->m_error != d->m_manager->error()) {
                    d->m_error = d->m_manager->error();
                    emit errorChanged();
                }
            }
        }
        emit importCompleted(QDeclarativeContactModel::ImportError(d->m_reader.error()), d->m_lastImportUrl);
    }
}

/*!
    \qmlmethod int ContactModel::fetchContacts(list<string> contactIds)

    Starts a request to fetch contacts by the given \a contactIds, and returns the unique ID of this request.
    -1 is returned if the request can't be started.

    Note that the contacts fetched won't be added to the model, but can be accessed through the onContactsFetched
    handler.

    \sa onContactsFetched()
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
    if (fetchRequest->start()) {
        int requestId(d->m_lastRequestId.fetchAndAddOrdered(1));
        d->m_requestIdHash.insert(fetchRequest, requestId);
        return requestId;
    } else
        return -1;
}

/*!
    \internal
 */
void QDeclarativeContactModel::onFetchContactsRequestStateChanged(QContactAbstractRequest::State state)
{
    if (state != QContactAbstractRequest::FinishedState || !sender())
        return;

    QContactFetchByIdRequest *request = qobject_cast<QContactFetchByIdRequest *>(sender());
    if (!request)
        return;

    checkError(request);

    int requestId(d->m_requestIdHash.value(request, -1));
    if (requestId == -1)
        qWarning()<<Q_FUNC_INFO<<"transaction not found from the request hash";
    QVariantList list;
    if (request->error() == QContactManager::NoError) {
        QList<QContact> contacts(request->contacts());
        foreach (const QContact &contact, contacts) {
            QDeclarativeContact *declarativeContact(0);
            declarativeContact = new QDeclarativeContact(this);
            declarativeContact->setContact(contact);
            list.append(QVariant::fromValue(declarativeContact));
        }
    }
    emit contactsFetched(requestId, list);
    request->deleteLater();
}

void QDeclarativeContactModel::clearContacts()
{
    qDeleteAll(d->m_contacts);
    d->m_contacts.clear();
    d->m_contactMap.clear();
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

    connect(fetchRequest,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(requestUpdated()));

    fetchRequest->start();
}

void QDeclarativeContactModel::requestUpdated()
{
    QContactFetchRequest* req = qobject_cast<QContactFetchRequest*>(QObject::sender());
    if (req && req->isFinished()) {
        QList<QContact> contacts = req->contacts();
        QList<QDeclarativeContact*> dcs;
        foreach (const QContact &c, contacts) {
            if (d->m_contactMap.contains(c.id())) {
                QDeclarativeContact* dc = d->m_contactMap.value(c.id());
                dc->setContact(c);
                dcs.append(dc);
            } else {
                QDeclarativeContact* dc = new QDeclarativeContact(this);
                if (dc) {
                    d->m_contactMap.insert(c.id(), dc);
                    dc->setContact(c);
                    dcs.append(dc);
                }
            }
        }

        if (d->m_contacts.isEmpty()) {
            reset();
            if (contacts.count()>0) {
                beginInsertRows(QModelIndex(), 0, contacts.count() - 1);
                d->m_contacts = dcs;
                endInsertRows();
            }
        } else {
            //Partial updating, insert the fetched contacts into the the exist contact list.
            beginResetModel();
            d->m_contacts.clear();
            if (dcs.count() > 0) {
                d->m_contacts = dcs;
            }
            endResetModel();
        }
        emit contactsChanged();
        checkError(req);
        req->deleteLater();
    }
}

/*!
  \qmlmethod ContactModel::saveContact(Contact contact)
  Save the given \a contact into the contacts store. Once saved successfully, the dirty flags of this contact will be reset.

  \sa Contact::modified
  */
void QDeclarativeContactModel::saveContact(QDeclarativeContact* dc)
{
    if (dc) {
        QContactSaveRequest* req = new QContactSaveRequest(this);
        req->setManager(d->m_manager);
        req->setContact(dc->contact());
        connect(req,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(onRequestStateChanged(QContactAbstractRequest::State)));
        req->start();
    }
}

void QDeclarativeContactModel::onRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState != QContactAbstractRequest::FinishedState) {
        return;
    }
    if (!sender()) {
        qWarning() << Q_FUNC_INFO << "Called without a sender.";
        return;
    }

    QContactAbstractRequest *request = qobject_cast<QContactAbstractRequest *>(sender());
    if (!request)
        return;
    checkError(request);
    request->deleteLater();
}

void QDeclarativeContactModel::checkError(const QContactAbstractRequest *request)
{
    if (request && d->m_error != request->error()) {
        d->m_error = request->error();
        emit errorChanged();
    }
}

void QDeclarativeContactModel::onContactsAdded(const QList<QContactId>& ids)
{
    if (d->m_autoUpdate && !ids.isEmpty()) {
        QContactFetchRequest *fetchRequest = new QContactFetchRequest(this);
        connect(fetchRequest,SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(onContactsAddedFetchRequestStateChanged(QContactAbstractRequest::State)));
        fetchRequest->setManager(d->m_manager);
        fetchRequest->setFetchHint(d->m_fetchHint ? d->m_fetchHint->fetchHint() : QContactFetchHint());

        QContactIdFilter idFilter;
        idFilter.setIds(ids);
        if (d->m_filter) {
            QContactIntersectionFilter filter;
            filter.append(d->m_filter->filter());
            filter.append(idFilter);
            fetchRequest->setFilter(filter);
        } else {
            fetchRequest->setFilter(idFilter);
        }
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
        if (d->m_contactMap.contains(id)) {
            int row = 0;
            //TODO:need a fast lookup
            for (; row < d->m_contacts.count(); row++) {
                if (d->m_contacts.at(row)->contactId() == id.toString())
                    break;
            }

            if (row < d->m_contacts.count()) {
                beginRemoveRows(QModelIndex(), row, row);
                d->m_contacts.removeAt(row);
                d->m_contactMap.remove(id);
                endRemoveRows();
                emitSignal = true;
            }
        }
    }
    if (emitSignal)
        emit contactsChanged();
}

void QDeclarativeContactModel::onContactsChanged(const QList<QContactId> &contactIdList)
{
    if (d->m_autoUpdate && !contactIdList.isEmpty()) {
        QContactFetchRequest *fetchRequest = new QContactFetchRequest(this);
        connect(fetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(onContactsChangedFetchRequestStateChanged(QContactAbstractRequest::State)));
        fetchRequest->setManager(d->m_manager);
        fetchRequest->setFetchHint(d->m_fetchHint ? d->m_fetchHint->fetchHint() : QContactFetchHint());
        QContactIdFilter idFilter;
        idFilter.setIds(contactIdList);
        if (d->m_filter) {
            QContactIntersectionFilter filter;
            filter.append(idFilter); // result handling assumes that id filter is the first filter
            filter.append(d->m_filter->filter());
            fetchRequest->setFilter(filter);
        } else
            fetchRequest->setFilter(idFilter);
        fetchRequest->start();
    }
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
            return c.displayLabel();
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
    \internal

    It's invoked by the fetch request from onContactsAdded().
 */
void QDeclarativeContactModel::onContactsAddedFetchRequestStateChanged(QContactAbstractRequest::State state)
{

    if (state != QContactAbstractRequest::FinishedState || !sender())
        return;
    QContactFetchRequest *request = qobject_cast<QContactFetchRequest *>(sender());
    if (!request)
        return;

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
    if (state != QContactAbstractRequest::FinishedState || !sender())
        return;

    QContactFetchRequest *request = qobject_cast<QContactFetchRequest *>(sender());
    if (!request)
        return;

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
        foreach (QContactId id, requestedContactIds) {
            if (contactListDoesNotContainContactWithId(fetchedContacts, id)) {
                for (int i=0;i<d->m_contacts.size();++i) {
                    if (d->m_contacts.at(i)->contactId() == id.toString()) {
                        beginRemoveRows(QModelIndex(), i, i);
                        d->m_contacts.removeAt(i);
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
                    int index = contactIndex(dc);
                    beginInsertRows(QModelIndex(), index, index);
                    d->m_contactMap.remove(fetchedContact.id());
                    d->m_contacts.removeAt(i);
                    d->m_contacts.insert(index, dc);
                    d->m_contactMap.insert(fetchedContact.id(),dc);
                    if (!contactsUpdated)
                        contactsUpdated = true;
                    endInsertRows();
                    fetchedContactFound = true;
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

QTCONTACTS_END_NAMESPACE
