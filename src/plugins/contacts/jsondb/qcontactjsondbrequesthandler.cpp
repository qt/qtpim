/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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

    // Be optimistic, reduce them later on error.
    m_availableStorageLocations = supportedStorageLocations();
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
    case QContactAbstractRequest::ContactFetchByIdRequest: {
        QContactFetchByIdRequest* fetchReq = static_cast<QContactFetchByIdRequest*>(req);
        handleContactFetchByIdRequest(fetchReq);
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
    m_requestMgr->addRequest(saveReq, contacts);
    for (int i = 0; i < contacts.size(); i++) {
        QContactManager::Error error = QContactManager::NoError;
        QContact contact = contacts.at(i);

        const QContactAbstractRequest::StorageLocation storageLocationForTheContact =
                contact.id().isNull() ? saveReq->storageLocation() : extractStorageLocation(contact.id());
        const QString partition = storageLocationToPartition(storageLocationForTheContact);
        if (partition.isEmpty()) {
            error = QContactManager::InvalidStorageLocationError;
            errorMap.insert(i,error);
            lastError = errorPrecedence(lastError, error);
            continue;
        }

        if (!m_engine->validateContact(contact, &error)) {
            errorMap.insert(i,error);
            lastError = errorPrecedence(lastError, error);
            // For an invalid contact in addition to reporting error we also clear the contact id.
            QContactId contactId;
            contact.setId(contactId);
            continue;
        }
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
                m_requestMgr->addRequest(fetchRequest);
                m_requestMgr->addPrefetchRequest(fetchRequest, saveReq);
                if (!makeJsonDbRequest(fetchRequest,
                                       QContactJsonDbRequestManager::PrefetchForSaveRequest,
                                       i,
                                       partition,
                                       fetchQuery)) {
                    error = QContactManager::TimeoutError;
                    errorMap.insert(i,error);
                    lastError = errorPrecedence( lastError, error);
                }
            } else {
                error = QContactManager::BadArgumentError;
                errorMap.insert(i,error);
                lastError = errorPrecedence(lastError, error);
            }
        } else {
            // No prefetch needed, just create a new contact.
            QJsonObject newJsonDbItem;
            if (m_converter->toJsonContact(&newJsonDbItem, contact, saveReq->typeMask())) {
                if (!makeJsonDbRequest(saveReq,
                                       QContactJsonDbRequestManager::SaveRequest,
                                       i,
                                       partition,
                                       QString(),
                                       QList<QJsonObject>() << newJsonDbItem)) {
                    error = QContactManager::TimeoutError;
                    errorMap.insert(i,error);
                    lastError = errorPrecedence(lastError, error);
                }
            } else {
                error = QContactManager::BadArgumentError;
                errorMap.insert(i,error);
                lastError = errorPrecedence(lastError, error);
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

QString QContactJsonDbRequestHandler::storageLocationToPartition(QContactAbstractRequest::StorageLocation storageLocation)
{
    const QStringList partitions = m_converter->storageLocationsToPartitionNames(QContactAbstractRequest::StorageLocations(storageLocation));
    if (partitions.isEmpty())
        return QString();
    else
        return partitions.first();
}

QContactAbstractRequest::StorageLocations QContactJsonDbRequestHandler::storageLocationsOrDefault(
        QContactAbstractRequest::StorageLocations storageLocation)
{
    return storageLocation ? storageLocation : QContactAbstractRequest::UserDataStorage;
}

QContactAbstractRequest::StorageLocation QContactJsonDbRequestHandler::extractStorageLocation(const QContactId &id)
{
    const QContactEngineId *engineId = QContactManagerEngine::engineId(id);
    if (engineId)
        return engineId->storageLocation();
    else
        return QContactAbstractRequest::StorageLocation(0);
}

QContactAbstractRequest::StorageLocations QContactJsonDbRequestHandler::extractStorageLocations(const QList<QContactId> &contactIds)
{
    QContactAbstractRequest::StorageLocations storageLocations(0);
    foreach (const QContactId &id, contactIds) {
        storageLocations |= extractStorageLocation(id);
    }
    return storageLocations;
}

void QContactJsonDbRequestHandler::handleContactFetchRequest(QContactFetchRequest* req) {

    QContactManager::Error error = QContactManager::NoError;
    QString newJsonDbQuery;
    m_requestMgr->addRequest(req);
    if (m_converter->queryFromRequest(req, newJsonDbQuery)) {
        QContactAbstractRequest::StorageLocations storageLocations = storageLocationsOrDefault(req->storageLocations());
        if (storageLocations & ~m_availableStorageLocations)
            error = QContactManager::InvalidStorageLocationError;
        QStringList partitions = m_converter->storageLocationsToPartitionNames(storageLocations);
        if (!partitions.isEmpty()) {
            foreach (const QString &partition, partitions) {
                if (!makeJsonDbRequest(req,
                                       QContactJsonDbRequestManager::FetchRequest,
                                       0,
                                       partition,
                                       newJsonDbQuery)) {
                    error = errorPrecedence(error, QContactManager::TimeoutError);
                    break;
                }
            }
        } else {
            error = errorPrecedence(error, QContactManager::InvalidStorageLocationError);
        }
    } else {
        error = QContactManager::BadArgumentError;
    }

    const QList<QContact> emptyContactList;
    if (error != QContactManager::NoError) {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactFetchRequest(req,
                                                         emptyContactList,error,QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }  else {
    QContactManagerEngine::updateContactFetchRequest(req, emptyContactList, error,
                                                           QContactAbstractRequest::ActiveState);
    }
}

void QContactJsonDbRequestHandler::handleContactFetchByIdRequest(QContactFetchByIdRequest* req) {
    QContactManager::Error error = QContactManager::NoError;
    QMap<int, QtContacts::QContactManager::Error> errorMap;
    QList<QContact> emptyContactList;
    QString newJsonDbQuery;
    m_requestMgr->addRequest(req);
    m_converter->queryFromRequest(req, newJsonDbQuery); // Always ok for fetch by id requests as an empty query is also ok.
    QContactAbstractRequest::StorageLocations storageLocations = extractStorageLocations(req->contactIds());
    if (storageLocations & ~m_availableStorageLocations)
        error = QContactManager::InvalidStorageLocationError;
    QStringList partitions = m_converter-> storageLocationsToPartitionNames(storageLocations);
    if (!partitions.isEmpty()) {
        foreach (const QString &partition, partitions) {
            if (!makeJsonDbRequest(req,
                                   QContactJsonDbRequestManager::FetchByIdRequest,
                                   0,
                                   partition,
                                   newJsonDbQuery)) {
                error = errorPrecedence(error, QContactManager::TimeoutError);
                break;
            }
        }
    } else {
        // None of the ids had valid partition specifie, prepare request with errors and empty contacts.
        // Here we keep DoesNotExistError for empty contact ids to keep consistency with memory backend
        // and existing test asset for null operations.
        for (int index = 0;index < req->contactIds().size();index++) {
            emptyContactList << QContact();
            if (req->contactIds().at(index).isNull()) {
                errorMap.insert(index, QContactManager::DoesNotExistError);
                error = errorPrecedence(error, QContactManager::DoesNotExistError);
            } else {
                errorMap.insert(index, QContactManager::InvalidStorageLocationError);
                error = errorPrecedence(error, QContactManager::InvalidStorageLocationError);
            }
        }
    }

    if (error != QContactManager::NoError) {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactFetchByIdRequest(req, emptyContactList, error, errorMap, QContactAbstractRequest::FinishedState);
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
        QContactId contactId = contactIds.at(i);
        QContactManager::Error error = QContactManager::NoError;
        if ( (!(contactId.isNull())) && (contactId.managerUri() == QContactJsonDbStr::managerUri()) ) {
            QContactAbstractRequest::StorageLocation storageLocation = QContactAbstractRequest::StorageLocation(extractStorageLocation(contactId));
            QString partition = storageLocationToPartition(storageLocation);
            if ((storageLocation & ~m_availableStorageLocations) || (partition.isEmpty())) {
                error = QContactManager::InvalidStorageLocationError;
                errorMap.insert(i, error);
                lastError = errorPrecedence(lastError, error);
            } else {
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
                    lastError = errorPrecedence(lastError, error);
                }
            }
        } else {
            // Set DoesNotExistsError for an id if trying to remove contact with a null
            // id or id is belonging to another contact manager engine.
            error = QContactManager::DoesNotExistError;
            errorMap.insert(i,error);
            lastError = errorPrecedence(lastError, error);
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
        if (storageLocations & ~m_availableStorageLocations)
            error = QContactManager::InvalidStorageLocationError;
        QStringList partitions = m_converter->storageLocationsToPartitionNames(storageLocations);
        if (!partitions.isEmpty()) {
            foreach (const QString &partition, partitions) {
                if (!makeJsonDbRequest(req,
                                       QContactJsonDbRequestManager::ContactIdFetchRequest,
                                       0,
                                       partition,
                                       newJsonDbQuery)) {
                    error = errorPrecedence(error, QContactManager::TimeoutError);
                    break;
                }
            }
        } else {
            error = QContactManager::InvalidStorageLocationError;
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
    if (!m_timer->isActive())
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

    // For recovering from no partitions available state.
    m_availableStorageLocations |= m_converter->partitionNameToStorageLocation(partitionName);

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
    case QContactJsonDbRequestManager::FetchByIdRequest: {
        QContactFetchByIdRequest* fetchByIdReq = static_cast<QContactFetchByIdRequest*>(req);
        handleContactFetchByIdResponse(fetchByIdReq, jsonDbRequest, partitionName);
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

    QContactManager::Error contactError = m_converter->jsonDbRequestErrorToContactError(error);
    if (request && (contactError == QContactManager::InvalidStorageLocationError)) {
        const QContactAbstractRequest::StorageLocation storageLocation =
                m_converter->partitionNameToStorageLocation(request->partition());
        if (storageLocation & ~supportedStorageLocations())
            qWarning() << Q_FUNC_INFO << "Unsupported storage location value:" << storageLocation;
        m_availableStorageLocations &= ~storageLocation;
        if (!m_availableStorageLocations)
            // No partitions available in jsondb, indicate in the error.
            contactError = errorPrecedence(contactError, QContactManager::MissingPlatformRequirementsError);
            qCritical("QContacts - JsonDb backend - all storage locations unavailable.");
    }
    QContactManager::Error errorStatus = errorPrecedence(req->error(), contactError);

    switch (jsonDbRequestType) {
    case QContactJsonDbRequestManager::FetchRequest: {
        QList<QContact> emptyContactList;
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), emptyContactList,
                                                              errorStatus, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), emptyContactList,
                                                              errorStatus, QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::FetchByIdRequest: {
        QContactFetchByIdRequest *fetchByIdrequest = static_cast<QContactFetchByIdRequest*>(req);
        if (m_requestMgr->isRequestCompleted(req)) {
            // There may be already contacts fetched for this request before jsondb error.
            QMap<int, QContactManager::Error> errorMap = fetchByIdrequest->errorMap();
            QContactManager::Error errorToDiscard = QContactManager::NoError;
            QList<QContact> contacts = orderedContacts(fetchByIdrequest->contactIds(), fetchByIdrequest->contacts(), &errorMap, &errorToDiscard);
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactFetchByIdRequest(fetchByIdrequest, contacts, errorStatus,
                                                                 errorMap, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactFetchByIdRequest(fetchByIdrequest, fetchByIdrequest->contacts(),
                                                                 errorStatus, fetchByIdrequest->errorMap(),
                                                                 QContactAbstractRequest::ActiveState);
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
        if (contacts.size() > contactIndex) {
            qWarning() << Q_FUNC_INFO << "Save request failed for contact at index" << contactIndex
                       << "and contact id" << contacts.at(contactIndex).id();
        } else {
            qWarning() << Q_FUNC_INFO << "Save request failed for contact at index" << contactIndex;
        }
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
            QContactManagerEngine::updateContactRemoveRequest(removeReq, errorStatus, errorMap, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            // If request not yet completed, just add error to the requests' error map.
            QContactManagerEngine::updateContactRemoveRequest(removeReq, errorStatus, errorMap, QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::ContactIdFetchRequest: {
        QContactIdFetchRequest* idFetchReq = static_cast<QContactIdFetchRequest*>(req);
        QList<QContactId> ids;
        // There may be already contact ids fetched for this request before jsondb error.
        if (idFetchReq)
            ids = idFetchReq->ids();
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactIdFetchRequest(idFetchReq, ids, contactError, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactIdFetchRequest(idFetchReq, ids, contactError, QContactAbstractRequest::ActiveState);
        }

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
    foreach (const QJsonObject &result, results) {
        QString jsonUuid = result.value(QContactJsonDbStr::uuid()).toString();
        if (!jsonUuid.isEmpty()) {
            QContact contact = req->contacts().at(index);
            bool isNewContact = (contact.id().isNull() || contact.id().managerUri().isEmpty());
            if (isNewContact) {
                contact.setId(m_converter->uuidtoContactId(jsonUuid, jsonDbRequest->partition()));
            }
            m_requestMgr->addContact(req, contact, index);
        }
    }
    if (!m_requestMgr->pendingPrefetchRequests(req) && m_requestMgr->isRequestCompleted(req)) {
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
    QContactManager::Error error = QContactManager::NoError;
    QContactSaveRequest *saveReq = m_requestMgr->removePrefetchRequest(prefetchReq);
    if (!saveReq) {
        qWarning() << Q_FUNC_INFO << "prefetch request not found";
        return;
    }
    QList<QJsonObject> results = jsonDbRequest->takeResults();
    QJsonObject result;
    // use a local variable instead of const reference in foreach since it is modified in the loop
    foreach (result, results) {
        if (result.isEmpty()) {
            // An empty response for prefetch request means attempt to update a non-existing contact.
            error = errorPrecedence(error, QContactManager::DoesNotExistError);
            qWarning() << Q_FUNC_INFO << "Empty prefetch response from jsondb.";
        } else {
            // Convert QContact to jsondb contact over the prefetched jsondb contact and save it.
            QString partition = storageLocationToPartition(
                        extractStorageLocation(saveReq->contacts().at(index).id()));
            if (!m_converter->toJsonContact(&result, saveReq->contacts().at(index),saveReq->typeMask())) {
                qWarning() << Q_FUNC_INFO << "Conversion from QContact to QJsonObject failed.";
                // Converter failed to map this QContact to Jsondb contact.
                error = errorPrecedence(error, QContactManager::BadArgumentError);
            } else {
                // Make save request for this contact.
                if (!makeJsonDbRequest(saveReq,
                                       QContactJsonDbRequestManager::UpdateRequest,
                                       index,
                                       partition,
                                       QString(),
                                       QList<QJsonObject>() << result)) {
                    error = errorPrecedence(error, QContactManager::TimeoutError);
                } else {
                    return;
                }
            }
        }
    }
    // In a rare case of an error we need to update the error map and last error.
    QMap<int, QContactManager::Error> errorMap = saveReq->errorMap();
    if (results.isEmpty()) {
        error = errorPrecedence(error, QContactManager::DoesNotExistError);
    }
    errorMap.insert(index, error);
    error = errorPrecedence(error, saveReq->error());
    QList<QContact> contacts = m_requestMgr->contacts(saveReq);
    if ((!m_requestMgr->pendingPrefetchRequests(saveReq)) &&
            m_requestMgr->isRequestCompleted(saveReq)) {
        // Error happens to the last contact in the save request and the whole request gets finished.
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
        m_requestMgr->removeRequest(saveReq);
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, error, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, error, errorMap, QContactAbstractRequest::ActiveState);
    }
    qWarning() << Q_FUNC_INFO << "Failed for" << saveReq->contacts().at(index).id();
}


void QContactJsonDbRequestHandler::handleContactFetchResponse(QContactFetchRequest *req, QJsonDbRequest *jsonDbRequest, const QString &partitionName)
{
    QList<QContact> contacts;
    QContactManager::Error error = QContactManager::NoError;
    if (req) {
        error = req->error();
        contacts = req->contacts();
    }
    QList<QJsonObject> results = jsonDbRequest->takeResults();
    foreach (const QJsonObject &result, results) {
        if (!result.isEmpty()) {
            QContact contact;
            m_converter->toQContact(result, &contact, partitionName);
            contacts.append(contact);
        }
    }
    if ((contacts.isEmpty()) || results.isEmpty())
        error = errorPrecedence(error, QContactManager::DoesNotExistError);
    if (m_requestMgr->isRequestCompleted(req)) {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactFetchRequest (req, contacts, error, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();

    } else {
        QContactManagerEngine::updateContactFetchRequest(req, contacts, error, QContactAbstractRequest::ActiveState);
    }

}

void QContactJsonDbRequestHandler::handleContactFetchByIdResponse(QContactFetchByIdRequest *req, QJsonDbRequest *jsonDbRequest, const QString &partitionName)
{
    QList<QContact> fetchedContacts;
    QContactManager::Error error = QContactManager::NoError;
    if (req) {
        error = req->error();
        fetchedContacts = req->contacts();
    }
    QList<QJsonObject> resultsFromResponse = jsonDbRequest->takeResults();
    foreach (const QJsonObject &result, resultsFromResponse) {
        if (!result.isEmpty()) {
            QContact contact;
            if (m_converter->toQContact(result, &contact, partitionName))
                fetchedContacts.append(contact);
        }
    }
    if (m_requestMgr->isRequestCompleted(req)) {
        QList<QContact> contacts;
        QMap<int, QContactManager::Error> errorMap;
        QList<QContactId> idsFromRequest;
        if (req)
            idsFromRequest = req->contactIds();
        QContactManager::Error errorFromOrdering = QContactManager::NoError;
        contacts = orderedContacts(idsFromRequest, fetchedContacts, &errorMap, &errorFromOrdering);
        error = errorPrecedence(error, errorFromOrdering);
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactFetchByIdRequest(req, contacts, error, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    } else {
        QMap<int, QContactManager::Error> emptyErrorMap; // Error map can be properly updated only when request is completely finished.
        QContactManagerEngine::updateContactFetchByIdRequest(req, fetchedContacts, error, emptyErrorMap, QContactAbstractRequest::ActiveState);
    }
}

QList<QContact> QContactJsonDbRequestHandler::orderedContacts(const QList<QContactId> &ids,
                                                              const QList<QContact> &contacts,
                                                              QMap<int, QContactManager::Error> *errorMap,
                                                              QContactManager::Error *lastError)
{
    Q_ASSERT(errorMap);
    Q_ASSERT(lastError);
    // Order contacts in the same order with given ids.
    // First build an index into contacts, value is index into unordered list.
    QHash<QContactId, int> idToIndexMap;
    for (int i = 0; i < contacts.size(); i++) {
        idToIndexMap.insert(contacts.at(i).id(), i);
    }

    // Then ind the order in which the contacts results should be presented
    // and build ordered contact list and related error map.
    int index=0;
    QList<QContact> ordered;
    foreach (const QContactId &id, ids) {
        if (!idToIndexMap.contains(id)) {
            errorMap->insert(index, QContactManager::DoesNotExistError);
            *lastError = QContactManager::DoesNotExistError;
            ordered.append(QContact());
        } else {
            ordered.append(contacts.at(idToIndexMap[id]));
        }
        index++;
    }
    return ordered;
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
    QList<QContactId>  ids;
    QContactManager::Error error = QContactManager::NoError;
    if (req) {
        ids = req->ids();
        error = req->error();
    }
    QList<QJsonObject> results = jsonDbRequest->takeResults();
    foreach (const QJsonObject &result, results) {
        QString uuid = result.value(QContactJsonDbStr::uuid()).toString();
        ids.append(m_converter->uuidtoContactId(uuid, jsonDbRequest->partition()));
    }
    if (ids.isEmpty() || results.isEmpty())
        error = errorPrecedence(error, QContactManager::DoesNotExistError);
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
    case QContactJsonDbRequestManager::FetchByIdRequest:
        request = new QJsonDbReadRequest(query, this);
        break;
    default:
        request = NULL;
        break;
    }
    QJsonDbWriteRequest *writeRequest = qobject_cast<QJsonDbWriteRequest*>(request);
    if (writeRequest)
        writeRequest->setConflictResolutionMode(QJsonDbWriteRequest::Replace);
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
