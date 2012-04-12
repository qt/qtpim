/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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
#include <QCoreApplication>
#include "qcontact.h"

#include "qcontactjsondbrequesthandler.h"
#include "qcontactjsondbengine.h"
#include "qcontactjsondbrequestmanager.h"
#include "qcontactjsondbconverter.h"
#include "qcontactjsondbid.h"
#include "qcontactjsondbglobal.h"
#include "qcontactjsondbstring.h"

#include <QtJsonDb/qjsondbreadrequest.h>
#include <QtJsonDb/qjsondbwriterequest.h>

#include <QDebug>
#include <QString>
#include <QMap>
#include <QTimer>

QTCONTACTS_BEGIN_NAMESPACE

const int QContactJsonDbRequestHandler::TIMEOUT_INTERVAL(100);

QContactJsonDbRequestHandler::QContactJsonDbRequestHandler()
    : m_engine(0), m_jsonDbConnection(0), m_timer(0)
{
}

QContactJsonDbRequestHandler::~QContactJsonDbRequestHandler()
{
    delete m_timer;
    delete m_reqStateMutex;
    delete m_converter;
    delete m_requestMgr;
    m_jsonDbConnection->deleteLater();
}

void QContactJsonDbRequestHandler::init()
{
    m_reqStateMutex = new QMutex();
    m_requestMgr = new QContactJsonDbRequestManager();
    m_converter = new QContactJsonDbConverter();
    m_jsonDbConnection = new QJsonDbConnection(this);
    connect(m_jsonDbConnection, SIGNAL(error(QtJsonDb::QJsonDbConnection::ErrorCode,QString)),
            this, SLOT(onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode,QString)));
    m_jsonDbConnection->connectToServer();
    Q_ASSERT(storageLocationToPartition(QContactAbstractRequest::UserDataStorage) !=
            storageLocationToPartition(QContactAbstractRequest::SystemStorage));
    createWatcherForStorageLocation(QContactAbstractRequest::UserDataStorage);
    createWatcherForStorageLocation(QContactAbstractRequest::SystemStorage);
}

void QContactJsonDbRequestHandler::createWatcherForStorageLocation(QContactAbstractRequest::StorageLocation storageLocation)
{
    QJsonDbWatcher* jsonDbWatcher = new QJsonDbWatcher(this);
    jsonDbWatcher->setWatchedActions(QJsonDbWatcher::All);
    jsonDbWatcher->setQuery(QContactJsonDbStr::contactsJsonDbNotificationQuery());
    jsonDbWatcher->setPartition(storageLocationToPartition(storageLocation));
    new QContactJsonDbPartitionWatcher(this, jsonDbWatcher, storageLocation);
    m_jsonDbConnection->addWatcher(jsonDbWatcher);
}

void QContactJsonDbRequestHandler::setEngine(QContactJsonDbEngine *engine)
{
    m_engine = engine;
}

bool QContactJsonDbRequestHandler::waitForRequestFinished(QContactAbstractRequest *req, int msecs)
{
    // This function is called by the QContactJsonDbEngine thread (main thread)

    // TODO: timeout handling
    Q_UNUSED(msecs);

    QMutexLocker locker(m_reqStateMutex);
    QWaitCondition waitCondition;

    if (req->state() == QContactAbstractRequest::FinishedState)
        return true;
    else if (req->state() == QContactAbstractRequest::CanceledState)
        return false;

    // Request might still be inactive if this function is called immediatelly after sending a signal to
    // handleRequest slot. The signal goes to event loop and might be handled later than this function is executed.
    bool requestInactive = !m_requestMgr->setWaitCondition(req,&waitCondition);
    bool requestFinished;

    if (msecs <= 0)
        requestFinished = waitCondition.wait(m_reqStateMutex);
    else
        requestFinished = waitCondition.wait(m_reqStateMutex, msecs);

    if (requestInactive && !requestFinished) {
        // The request was never started or was started but not yet finished
        m_requestMgr->removeWaitCondition(req);
    }
    return requestFinished;
}

void QContactJsonDbRequestHandler::handleRequest(QContactAbstractRequest *req)
{
    // Detects what kind of request is at hand, updates the state to "active", calls "addRequest" and
    // "addRequest" from requestManager, sends resultsAvailable signal
    QMutexLocker locker(m_reqStateMutex);
    QContactManager::Error error = QContactManager::NoError;
    if (m_reqList.contains(req)) {
        error = QContactManager::UnspecifiedError;
        qWarning() << Q_FUNC_INFO << "Trying to handle destroyed request: throwing ERROR: " << error;
        return;
    }
    switch (req->type()) {
    case QContactAbstractRequest::ContactSaveRequest: {
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        handleContactSaveRequest(saveReq);
        break;
    }
    case QContactAbstractRequest::ContactFetchRequest: {
        QContactFetchRequest* fetchReq = static_cast<QContactFetchRequest*>(req);
        handleContactFetchRequest(fetchReq);
        break;
    }
    case QContactAbstractRequest::ContactRemoveRequest: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        handleContactRemoveRequest(removeReq);
        break;
    }
    case QContactAbstractRequest::ContactIdFetchRequest: {
        QContactIdFetchRequest* idReq = static_cast<QContactIdFetchRequest*>(req);
        handleContactIdFetchRequest(idReq);
        break;
    }
    default:
        break;
    }
}


void QContactJsonDbRequestHandler::handleContactSaveRequest(QContactSaveRequest* saveReq) {
    //TODO: handle duplicates(?)
    QList<QContact> contacts = saveReq->contacts();
    QContactManager::Error lastError = QContactManager::NoError;
    QMap<int, QContactManager::Error> errorMap;
    QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, lastError, errorMap, QContactAbstractRequest::ActiveState);
    m_requestMgr->addRequest(saveReq, contacts);
    for (int i = 0; i < contacts.size(); i++) {
        QContactManager::Error error = QContactManager::NoError;
        QContact contact = contacts.at(i);
        if (!m_engine->validateContact(contact, &error)) {
            errorMap.insert(i,error);
            lastError = error;
            // For invalid contact in addition to reporting error we also clear the contact id.
            QContactId contactId;
            contact.setId(contactId);
        } else {
            if (!contact.id().isNull()) {
                // Update to existing contact with given id. Fetch complete contact data from jsondb before saving.
                // This preserves possible extra fields in jsondb contact object. Actual update request is made in
                // the response handler for this prefetch request.
                QContactIdFilter idFilter;
                idFilter.add(contact.id());
                QContactFetchRequest *fetchRequest = new QContactFetchRequest(this);
                fetchRequest->setFilter(idFilter);
                QString fetchQuery;
                bool isValid = m_converter->queryFromRequest(fetchRequest, fetchQuery);
                if (isValid) {
                    QString partition = storageLocationToPartition(extractStorageLocation(contact.id()));
                    m_requestMgr->addRequest(fetchRequest);
                    m_requestMgr->addPrefetchRequest(fetchRequest, saveReq);
                    if (!makeJsonDbRequest(fetchRequest,
                                           QContactJsonDbRequestManager::PrefetchForSaveRequest,
                                           i,
                                           partition,
                                           fetchQuery)) {
                        error = QContactManager::TimeoutError;
                        errorMap.insert(i,error);
                        lastError = error;
                    }
                } else {
                    error = QContactManager::BadArgumentError;
                    errorMap.insert(i,error);
                    lastError = error;
                }
            } else {
                // No prefetch needed, just create a new contact.
                QJsonObject newJsonDbItem;
                if (m_converter->toJsonContact(&newJsonDbItem, contact)) {
                    QString partition = storageLocationToPartition(saveReq->storageLocation());
                    if (!makeJsonDbRequest(saveReq,
                                           QContactJsonDbRequestManager::SaveRequest,
                                           i,
                                           partition,
                                           QString(),
                                           QList<QJsonObject>() << newJsonDbItem)) {
                        error = QContactManager::TimeoutError;
                        errorMap.insert(i,error);
                        lastError = error;
                    }
                } else {
                    error = QContactManager::BadArgumentError;
                    errorMap.insert(i,error);
                    lastError = error;
                }
            }
        }
    }
    if (errorMap.size() == contacts.size()) { //None of the contacts could be saved
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
        m_requestMgr->removeRequest(saveReq);
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, lastError, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, lastError, errorMap, QContactAbstractRequest::ActiveState);
    }
}

QString QContactJsonDbRequestHandler::storageLocationToPartition(
        QContactAbstractRequest::StorageLocations storageLocation)
{
    QContactAbstractRequest::StorageLocations targetStorageLocation =
            storageLocationsOrDefault(storageLocation);
    return m_converter->storageLocationsToPartitionNames(targetStorageLocation).first();
}

QContactAbstractRequest::StorageLocations QContactJsonDbRequestHandler::storageLocationsOrDefault(
        QContactAbstractRequest::StorageLocations storageLocation)
{
    return storageLocation ? storageLocation : QContactAbstractRequest::UserDataStorage;
}

QContactAbstractRequest::StorageLocations QContactJsonDbRequestHandler::extractStorageLocation(const QContactId &id)
{
    const QContactEngineId *engineId = QContactManagerEngine::engineId(id);
    return engineId->storageLocation();
}

void QContactJsonDbRequestHandler::handleContactFetchRequest(QContactFetchRequest* req) {

    QContactManager::Error error = QContactManager::NoError;
    QString newJsonDbQuery;
    m_requestMgr->addRequest(req);
    if (m_converter->queryFromRequest(req, newJsonDbQuery)) {
        QContactAbstractRequest::StorageLocations storageLocations = storageLocationsOrDefault(req->storageLocations());
        QStringList partitions = m_converter->storageLocationsToPartitionNames(storageLocations);
        foreach (const QString &partition, partitions) {
            if (!makeJsonDbRequest(req,
                                   QContactJsonDbRequestManager::FetchRequest,
                                   0,
                                   partition,
                                   newJsonDbQuery)) {
                error = QContactManager::TimeoutError;
                break;
            }
        }
    } else
        error = QContactManager::BadArgumentError;

    if (error != QContactManager::NoError) {
        const QList<QContact> emptyContactList;
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactFetchRequest(req,
                                                         emptyContactList,error,QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QContactJsonDbRequestHandler::handleContactRemoveRequest(QContactRemoveRequest* req) {
    QContactManager::Error lastError = QContactManager::NoError;
    QMap<int, QContactManager::Error> errorMap;

    QList<QContactId> contactIds = req->contactIds();
    QContactManagerEngine::updateContactRemoveRequest(req, lastError, errorMap, QContactAbstractRequest::ActiveState);
    m_requestMgr->addRequest(req);

    for (int i = 0; i < contactIds.size(); i++) {
        QContactManager::Error error = QContactManager::NoError;
        QContactId contactId = contactIds.at(i);
        if ( (!(contactId.isNull())) && (contactId.managerUri() == QContactJsonDbStr::managerUri()) ) {
            QString partition = storageLocationToPartition(extractStorageLocation(contactId));

            QJsonObject newJsonDbContact;
            newJsonDbContact.insert(QContactJsonDbStr::type(), QContactJsonDbStr::contactsJsonDbType());
            newJsonDbContact.insert(QContactJsonDbStr::uuid(), m_converter->contactIdToUuid(contactId));
            if (!makeJsonDbRequest(req,
                                   QContactJsonDbRequestManager::RemoveRequest,
                                   i,
                                   partition,
                                   QString(),
                                   QList<QJsonObject>() << newJsonDbContact)) {
                error = QContactManager::TimeoutError;
                errorMap.insert(i,error);
                lastError = error;
            }
        } else {
            // Generate DoesNotExistsError if trying to remove contact with a null id or belonging to
            // another contact manager engine.
            error = QContactManager::DoesNotExistError;
            errorMap.insert(i,error);
            lastError = error;
        }
    }

    if (errorMap.size() == contactIds.size()) {
        //jsondbrequest could not be created at all
        //so we remove it from manager and set its state to finished
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactRemoveRequest(req,
                                                          lastError,errorMap,QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QContactManagerEngine::updateContactRemoveRequest(req, lastError, errorMap, QContactAbstractRequest::ActiveState);
    }
}

void QContactJsonDbRequestHandler::handleContactIdFetchRequest(QContactIdFetchRequest *req)
{
    QContactManager::Error error = QContactManager::NoError;
    m_requestMgr->addRequest(req);
    QString newJsonDbQuery;
    if (m_converter->queryFromRequest(req, newJsonDbQuery)) {
        QContactAbstractRequest::StorageLocations storageLocations = storageLocationsOrDefault(req->storageLocations());
        QStringList partitions = m_converter->storageLocationsToPartitionNames(storageLocations);
        foreach (const QString &partition, partitions) {
            if (!makeJsonDbRequest(req,
                               QContactJsonDbRequestManager::ContactIdFetchRequest,
                               0,
                               partition,
                               newJsonDbQuery)) {
                error = QContactManager::TimeoutError;
                break;
            }
        }
    } else {
        error = QContactManager::BadArgumentError;
    }

    const QList<QContactId> emptyContactIdList;
    if (error!=QContactManager::NoError) {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactIdFetchRequest(req,emptyContactIdList,error,
                                                           QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QContactManagerEngine::updateContactIdFetchRequest(req,emptyContactIdList,error,
                                                           QContactAbstractRequest::ActiveState);
    }
}

void QContactJsonDbRequestHandler::onJsonDbWatcherNotificationsAvailable(QJsonDbWatcher *jsonDbWatcher,
                                                                         QContactAbstractRequest::StorageLocation storageLocation)
{
    // There is no need for mutex locker since we do not access any request from here.
    QList<QJsonDbNotification> notifications = jsonDbWatcher->takeNotifications();
    foreach (const QJsonDbNotification &notification, notifications) {
        QJsonObject jsonDbObject = notification.object();
        QContactId contactId = m_converter->jsonDbNotificationObjectToContactId(jsonDbObject, storageLocation);
        switch (notification.action()) {
        case QJsonDbWatcher::Created: {
            m_ccs.insertAddedContact(contactId);
            startTimer();
            break;
        }
        case QJsonDbWatcher::Updated: {
            m_ccs.insertChangedContact(contactId);
            startTimer();
            break;
        }
        case QJsonDbWatcher::Removed: {
            m_ccs.insertRemovedContact(contactId);
            startTimer();
            break;
        }
        default:
            break;
        }
    }
}

void QContactJsonDbRequestHandler::startTimer()
{
    // There is no need for mutex locker since we do not access any request data from here.

    if (!m_timer) {
        m_timer = new QTimer(this);
        m_timer->setSingleShot(true);
        m_timer->setInterval(TIMEOUT_INTERVAL);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    }
    m_timer->start();
}

void QContactJsonDbRequestHandler::onTimeout()
{
    // There is no need for mutex locker since we do not access any request data from here.

    if (m_engine) {
        m_ccs.emitSignals(m_engine);
        m_ccs.clearAll();
    }
}


void QContactJsonDbRequestHandler::handleResponse(QJsonDbRequest *jsonDbRequest)
{
    QMutexLocker locker(m_reqStateMutex);

    int contactIndex;
    QContactJsonDbRequestManager::RequestType requestType;
    QString partitionName;
    QContactAbstractRequest *req = m_requestMgr->removeRequest(jsonDbRequest, requestType, contactIndex, &partitionName);
    // Handle responses for requests having no QContactAbstractRequest associated.
    switch (requestType) {
    case QContactJsonDbRequestManager::OrphanRequest: {
        qWarning() << Q_FUNC_INFO << "Orphaned request:"<< jsonDbRequest;
        return;
    }
    case QContactJsonDbRequestManager::InvalidRequest: {
        qWarning() << Q_FUNC_INFO << "InvalidrequestType, request:" << jsonDbRequest;
        return;
    }
    default:
         break;
    }
    // Rest of the jsonDbRequest responses should have assosisated QContactAbstractRequest.
    // However, if client already has deleted the request we just give a warning.
    if (!req) {
        qWarning() << Q_FUNC_INFO << "request id" << jsonDbRequest << "missing associated QContactAbstractRequest";
        return;
    }
    switch (requestType) {
    case QContactJsonDbRequestManager::PrefetchForSaveRequest: {
        QContactFetchRequest* preFetchReq = static_cast<QContactFetchRequest*>(req);
        handleContactSavePrefetchResponse(preFetchReq, jsonDbRequest, contactIndex);
        break;
    }
    case QContactJsonDbRequestManager::UpdateRequest:
    case QContactJsonDbRequestManager::SaveRequest: {
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        handleContactSaveResponse(saveReq, jsonDbRequest, contactIndex);
        break;
    }
    case QContactJsonDbRequestManager::FetchRequest: {
        QContactFetchRequest* fetchReq = static_cast<QContactFetchRequest*>(req);
        handleContactFetchResponse(fetchReq, jsonDbRequest, partitionName);
        break;
    }
    case QContactJsonDbRequestManager::RemoveRequest: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        handleContactRemoveResponse(removeReq);
        break;
    }
    case QContactJsonDbRequestManager::ContactIdFetchRequest: {
        QContactIdFetchRequest* idReq = static_cast<QContactIdFetchRequest*>(req);
        handleContactIdFetchResponse(idReq, jsonDbRequest);
        break;
    }
    default:
        break;
   }
}

void QContactJsonDbRequestHandler::onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode connectionError, const QString &message)
{
    //currently there are no error codes in QJsonDbConnection
    //TODO handle them once its there
    qWarning()<<Q_FUNC_INFO<<"error"<<connectionError<<"message"<<message;
}

void QContactJsonDbRequestHandler::onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode error,
                                                        QString message)
{
    qWarning()<<Q_FUNC_INFO<<"error"<<error<<"message"<<message;
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    QContactManager::Error contactError = m_converter->jsonDbRequestErrorToContactError(error);

    int contactIndex;
    QContactJsonDbRequestManager::RequestType jsonDbRequestType;
    QString partitionName;
    QContactAbstractRequest *req = m_requestMgr->removeRequest(request, jsonDbRequestType, contactIndex, &partitionName);

    switch (jsonDbRequestType) {
    case QContactJsonDbRequestManager::OrphanRequest: {
        qWarning() << Q_FUNC_INFO << "Orphaned request with requestId:" << req;
        return;
    }
    case QContactJsonDbRequestManager::InvalidRequest: {
        qWarning() << Q_FUNC_INFO << "Invalid request with requestId:" << req;
        return;
    }
    default:
         break;
    }

    // Rest of the jsonDbRequest responses should have associated QContactAbstractRequest.
    // However, if client already has deleted the request we just give a warning.
    if (!req) {
        qWarning() << Q_FUNC_INFO << "request id" << req << "missing associated QContactAbstractRequest.";
        return;
    }

    switch (jsonDbRequestType) {
    case QContactJsonDbRequestManager::FetchRequest: {
        QList<QContact> emptyContactList;
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), emptyContactList,
                                                              contactError, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), emptyContactList,
                                                              contactError, QContactAbstractRequest::ActiveState);
        }
        break;
    }
        // The following two request types are both related to contact save request.
        // They do share exactly the same handling down below.
    case QContactJsonDbRequestManager::PrefetchForSaveRequest:
    case QContactJsonDbRequestManager::SaveRequest: {
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        QList<QContact> contacts = saveReq->contacts();
        QMap<int, QContactManager::Error> errorMap = saveReq->errorMap();
        if (contactError != QContactManager::NoError) {
            errorMap.insert(contactIndex, contactError);
        }
        qWarning() << Q_FUNC_INFO << "Save request failed for contact index" << contactIndex
                   << "and contact id" << contacts.at(contactIndex).id();
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactSaveRequest(static_cast<QContactSaveRequest*>(req),contacts,
                                                            contactError,errorMap,QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactSaveRequest(static_cast<QContactSaveRequest*>(req),contacts,
                                                            contactError,errorMap,QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::RemoveRequest: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        QMap<int, QContactManager::Error> errorMap = removeReq->errorMap();
        if (contactError != QContactManager::NoError)
            errorMap.insert(contactIndex, contactError);
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactRemoveRequest(removeReq, contactError, errorMap, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            // If request not yet completed, just add error to the requests' error map.
            QContactManagerEngine::updateContactRemoveRequest(removeReq, contactError, errorMap, QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::ContactIdFetchRequest: {
        QContactIdFetchRequest* idFetchReq = static_cast<QContactIdFetchRequest*>(req);
        QList<QContactId> ids;
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactIdFetchRequest(idFetchReq, ids, contactError, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
        break;
    }
    default:
        break;
   }
}

void QContactJsonDbRequestHandler::removeDestroyed(QObject * req)
{   QMutexLocker locker(m_reqStateMutex);
    QContactAbstractRequest* aReq = static_cast<QContactAbstractRequest*>(req);
    if ((aReq)&&(!m_reqList.contains(aReq))&&(m_requestMgr->contains(aReq))) {
        m_reqList.append((aReq));
        m_requestMgr->removeRequest(aReq);
    }
}

void QContactJsonDbRequestHandler::handleContactSaveResponse(QContactSaveRequest* req, QJsonDbRequest *jsonDbRequest, int index)
{
    QContactManager::Error lastError = req->error();
    QMap<int, QContactManager::Error> errorMap = req->errorMap();
    QList<QJsonObject> results = jsonDbRequest->takeResults();
    for (int i = 0; i < results.size(); ++i) {
        QJsonObject object = results.at(i);
        QString jsonUuid = object.value(QContactJsonDbStr::uuid()).toString();
        if (!jsonUuid.isEmpty()) {
            QContact contact = req->contacts().at(index);
            bool isNewContact = (contact.id().isNull() || contact.id().managerUri().isEmpty());
            if (isNewContact) {
                contact.setId(m_converter->uuidtoContactId(jsonUuid, jsonDbRequest->partition()));
            }
            m_requestMgr->addContact(req, contact, index);
        }
    }
    if (m_requestMgr->isRequestCompleted(req)) {
        QList<QContact> contacts = m_requestMgr->contacts(req);
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactSaveRequest(req, contacts, lastError, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QContactJsonDbRequestHandler::handleContactSavePrefetchResponse(QContactFetchRequest *prefetchReq, QJsonDbRequest *jsonDbRequest, int index)
{
    QContactManager::Error lastError = QContactManager::NoError;
    QList<QJsonObject> jsonDbObjectList = jsonDbRequest->takeResults();
    QContactSaveRequest *saveReq = m_requestMgr->removePrefetchRequest(prefetchReq);
    if (!saveReq)
        qWarning()<<Q_FUNC_INFO<<"prefetch request not found";
    for (int i = 0; i < jsonDbObjectList.size(); ++i) {
        QJsonObject object = jsonDbObjectList.at(i);
        if (object.isEmpty()) {
            // An empty response for prefetch request means attempt to update a non-existing contact.
            lastError = QContactManager::DoesNotExistError;
            qWarning() << Q_FUNC_INFO << "Empty prefetch response from jsondb.";
        } else {
            // Convert QContact to jsondb contact over the prefetched jsondb contact and save it.
            QString partition = storageLocationToPartition(
                        extractStorageLocation(saveReq->contacts().at(index).id()));
            if (m_converter->toJsonContact(&object, saveReq->contacts().at(index))) {
                if (!makeJsonDbRequest(saveReq,
                                       QContactJsonDbRequestManager::UpdateRequest,
                                       i,
                                       partition,
                                       QString(),
                                       QList<QJsonObject>() << object))
                    lastError = QContactManager::TimeoutError;

                return;
            } else {
                //  Converter failed to map this QContact to Jsondb contact.
                lastError = QContactManager::BadArgumentError;
            }
        }
    }
    // In a rare case of an error we need to update the error map and last error.
    QMap<int, QContactManager::Error> errorMap = saveReq->errorMap();
    if (jsonDbObjectList.isEmpty()) {
        lastError = QContactManager::DoesNotExistError;
    }
    errorMap.insert(index, lastError);
    QList<QContact> contacts = m_requestMgr->contacts(saveReq);
    if ((!m_requestMgr->pendingPrefetchRequests(saveReq)) &&
            m_requestMgr->isRequestCompleted(saveReq)) {
        // Error happens to the last contact in the save request and the whole request gets finished.
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
        m_requestMgr->removeRequest(saveReq);
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, lastError, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, lastError, errorMap, QContactAbstractRequest::ActiveState);
    }
    qWarning() << Q_FUNC_INFO << "Failed for" << saveReq->contacts().at(index).id();
}


void QContactJsonDbRequestHandler::handleContactFetchResponse(QContactFetchRequest *req, QJsonDbRequest *jsonDbRequest, const QString &partitionName)
{
    QList<QContact> contacts;
    QContactManager::Error error = QContactManager::NoError;
    if (req)
        error = req->error();
    QList<QJsonObject> jsonDbObjectList = jsonDbRequest->takeResults();
    for (int i = 0; i < jsonDbObjectList.size(); ++i) {
        QJsonObject object = jsonDbObjectList.at(i);
        QContact contact;
        if (!object.isEmpty()) {
            m_converter->toQContact(object, &contact, partitionName);
            contacts.append(contact);
        }
    }
    if ((contacts.isEmpty()) || jsonDbObjectList.isEmpty())
        error = QContactManager::DoesNotExistError;
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
    m_requestMgr->removeRequest(req);
    QContactManagerEngine::updateContactFetchRequest (req, contacts, error, QContactAbstractRequest::FinishedState);

    if (waitCondition)
        waitCondition->wakeAll();
}

void QContactJsonDbRequestHandler::handleContactRemoveResponse(QContactRemoveRequest *req)
{
    if (m_requestMgr->isRequestCompleted(req)) {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManager::Error lastError = req->error();
        QMap<int, QContactManager::Error> errorMap = req->errorMap();
        QContactManagerEngine::updateContactRemoveRequest(req, lastError, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QContactJsonDbRequestHandler::handleContactIdFetchResponse(QContactIdFetchRequest* req, QJsonDbRequest *jsonDbRequest)
{
    QList<QContactId>  ids = req->ids();
    QList<QJsonObject> jsonObjects = jsonDbRequest->takeResults();
    for (int i = 0; i < jsonObjects.size(); ++i) {
        QJsonObject object = jsonObjects.at(i);
        QString uuid = object.value(QContactJsonDbStr::uuid()).toString();
        ids.append(m_converter->uuidtoContactId(uuid, jsonDbRequest->partition()));
    }
    QContactManager::Error error = req->error();
    if (ids.isEmpty() || jsonObjects.isEmpty())
        error = QContactManager::DoesNotExistError;

    if (m_requestMgr->isRequestCompleted(req)) {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactIdFetchRequest(req, ids, error, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QContactManagerEngine::updateContactIdFetchRequest(req, ids, error, QContactAbstractRequest::ActiveState);
    }
}

bool QContactJsonDbRequestHandler::makeJsonDbRequest(QContactAbstractRequest *contactRequest,
                                                     QContactJsonDbRequestManager::RequestType jsonDbRequestType,
                                                     int index,
                                                     const QString &partition,
                                                     const QString &query,
                                                     const QList<QJsonObject> &objects)
{
    QJsonDbRequest *request;
    switch (jsonDbRequestType) {
    case QContactJsonDbRequestManager::PrefetchForSaveRequest:
        request = new QJsonDbReadRequest(query, this);
        break;
    case QContactJsonDbRequestManager::SaveRequest:
        request = new QJsonDbCreateRequest(objects,this);
        break;
    case QContactJsonDbRequestManager::UpdateRequest:
        request = new QJsonDbUpdateRequest(objects, this);
        break;
    case QContactJsonDbRequestManager::ContactIdFetchRequest:
        request = new QJsonDbReadRequest(query, this);
        break;
    case QContactJsonDbRequestManager::RemoveRequest:
        request = new QJsonDbRemoveRequest(objects,this);
        break;
    case QContactJsonDbRequestManager::FetchRequest:
        request = new QJsonDbReadRequest(query, this);
        break;
    default:
        request = NULL;
        break;
    }

    request->setPartition(partition);
    connect(request, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)),
            this, SLOT(onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode,
                                            QString)));
    connect(request, SIGNAL(finished()), this, SLOT(onJsonDbRequestFinished()));

    if (m_jsonDbConnection->send(request)) {
        m_requestMgr->addRequest(request, jsonDbRequestType, contactRequest, index);
        return true;
    } else {
        delete request;
        return false;
    }
}

void QContactJsonDbRequestHandler::onJsonDbRequestFinished()
{
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (request)
        handleResponse(request);
}

QContactJsonDbPartitionWatcher::QContactJsonDbPartitionWatcher(QContactJsonDbRequestHandler *requestHandler,
                                                               QJsonDbWatcher *jsonDbWatcher,
                                                               QContactAbstractRequest::StorageLocation storageLocation)
    : QObject(requestHandler), m_requestHandler(requestHandler), m_jsonDbWatcher(jsonDbWatcher), m_storageLocation(storageLocation)
{
    connect(m_jsonDbWatcher, SIGNAL(error(QtJsonDb::QJsonDbWatcher::ErrorCode,QString)),
            this, SLOT(onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode,QString)));
    connect(m_jsonDbWatcher, SIGNAL(notificationsAvailable(int)),
            this, SLOT(onJsonDbWatcherNotificationsAvailable()));
}

void QContactJsonDbPartitionWatcher::onJsonDbWatcherNotificationsAvailable()
{
    m_requestHandler->onJsonDbWatcherNotificationsAvailable(m_jsonDbWatcher, m_storageLocation);
}

void QContactJsonDbPartitionWatcher::onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode error,
                                                          QString message)
{
    // TODO handle the error when partition is supported
    if (error!=QJsonDbWatcher::NoError)
        qCritical()<<Q_FUNC_INFO<<"error"<<error<<"message"<<message;
}


#include "moc_qcontactjsondbrequesthandler.cpp"

QTCONTACTS_END_NAMESPACE
