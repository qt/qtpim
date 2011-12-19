/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Mobility Components.
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

#include <jsondb-error.h>
#include <jsondb-client.h>
#include <QDebug>
#include <QString>
#include <QMap>
#include <QTimer>

QTCONTACTS_BEGIN_NAMESPACE

const int QContactJsonDbRequestHandler::TIMEOUT_INTERVAL(100);

QContactJsonDbRequestHandler::QContactJsonDbRequestHandler()
    : m_engine(0), m_timer(0), m_jsonDbNotificationsRequested(false)
{
}

QContactJsonDbRequestHandler::~QContactJsonDbRequestHandler()
{
    if (m_reqStateMutex)
        delete m_reqStateMutex;
    if (m_requestMgr)
        delete m_requestMgr;
    if (m_jsonDb)
        delete m_jsonDb;
    if (m_converter)
        delete m_converter;
}

void QContactJsonDbRequestHandler::init()
{
    m_reqStateMutex = new QMutex();
    m_requestMgr = new QContactJsonDbRequestManager();
    m_converter = new QContactJsonDbConverter();
    m_jsonDb = new JsonDbClient;
    connect(m_jsonDb, SIGNAL(notified(const QString&, const QVariant&, const QString&)),
            this, SLOT(onNotified(const QString&, const QVariant&, const QString&)));
    connect(m_jsonDb, SIGNAL(response(int, const QVariant)), this,
            SLOT(onResponse(int, const QVariant)));
    connect(m_jsonDb, SIGNAL(error(int, int, const QString&)), this,
            SLOT(onError(int, int, const QString&)));

    // Request notifications from JsonDb.
    sendJsonDbNotificationsRequest();
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
    // Ensure notifications from jsondb are on or request for them is already pending.
    if (!m_jsonDbNotificationsRequested) {
        sendJsonDbNotificationsRequest();
    }

    // Detects what kind of request is at hand, updates the state to "active", calls "addRequest" and
    // "addTransaction" from requestManager, sends resultsAvailable signal
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

void QContactJsonDbRequestHandler::handleContactSaveRequest(QContactSaveRequest* req) {
    //TODO: handle duplicates(?)
    QList<QContact> contacts = req->contacts();
    QContactManager::Error lastError = QContactManager::NoError;
    QMap<int, QContactManager::Error> errorMap;
    QContactManagerEngine::updateContactSaveRequest(req, contacts, lastError, errorMap, QContactAbstractRequest::ActiveState);
    m_requestMgr->addRequest(req, contacts);
    bool transactionsMade = false;
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
                // This preserves possible extra fields in jsondb contact object.
                QContactIdFilter localIdFilter;
                localIdFilter.add(contact.id());
                QContactFetchRequest fetchRequest;
                fetchRequest.setFilter(localIdFilter);
                QString fetchQuery = m_converter->queryFromRequest(&fetchRequest);
                int trId = m_jsonDb->query(fetchQuery);
                m_requestMgr->addTransaction(trId, QContactJsonDbRequestManager::PrefetchForSaveTransaction, req, i);
                transactionsMade = true;
            } else {
                // If contact local id was empty, we make a create transaction for a new contact.
                QVariantMap newJsonDbItem;
                if (m_converter->toJsonContact(&newJsonDbItem, contact)) {
                    int trId = m_jsonDb->create(newJsonDbItem);
                    m_requestMgr->addTransaction(trId, QContactJsonDbRequestManager::SaveTransaction, req, i);
                    transactionsMade = true;
                } else {
                    error = QContactManager::BadArgumentError;
                    errorMap.insert(i,error);
                    lastError = error;
                }
            }
        }
    }
    if (transactionsMade) {
        QContactManagerEngine::updateContactSaveRequest(req, contacts, lastError, errorMap, QContactAbstractRequest::ActiveState);
    } else {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactSaveRequest(req, contacts, lastError, errorMap, QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QContactJsonDbRequestHandler::handleContactFetchRequest(QContactFetchRequest* req) {
    m_requestMgr->addRequest(req);
    QString newJsonDbQuery = m_converter->queryFromRequest(req);
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_requestMgr->addTransaction(trId, QContactJsonDbRequestManager::FetchTransaction, req);
}

void QContactJsonDbRequestHandler::handleContactRemoveRequest(QContactRemoveRequest* req) {
    QContactManager::Error error = QContactManager::NoError;
    QContactManager::Error lastError = QContactManager::NoError;
    QMap<int, QContactManager::Error> errorMap;

    QList<QContactId> contactIds = req->contactIds();
    QContactManagerEngine::updateContactRemoveRequest(req, error, errorMap, QContactAbstractRequest::ActiveState);
    m_requestMgr->addRequest(req);

    bool transactionsMade = false;
    for (int i = 0; i < contactIds.size(); i++) {
        QContactId contactId = contactIds.at(i);
        int trId;
        if (!(contactId.isNull())) {
            QVariantMap newJsonDbItem;
            newJsonDbItem.insert(QContactJsonDbStr::type(), QContactJsonDbStr::contactsJsonDbType());
            newJsonDbItem.insert(QContactJsonDbStr::uuid(), convertContactIdToUuid(contactId));
            trId = m_jsonDb->remove(newJsonDbItem);
            m_requestMgr->addTransaction(trId, QContactJsonDbRequestManager::RemoveTransaction, req, i);
            transactionsMade = true;
        } else {
            // Generate DoesNotExistsError if trying to remove contact with an empty contactId.
            error = QContactManager::DoesNotExistError;
            errorMap.insert(i,error);
            lastError = error;
        }
    }

    if (transactionsMade) {
        QContactManagerEngine::updateContactRemoveRequest(req, error, errorMap, QContactAbstractRequest::ActiveState);
    } else {
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactRemoveRequest(req,
                                                          error,errorMap,QContactAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QContactJsonDbRequestHandler::handleContactIdFetchRequest(QContactIdFetchRequest *req)
{
    m_requestMgr->addRequest(req);
    QString newJsonDbQuery = m_converter->queryFromRequest(req);
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_requestMgr->addTransaction(trId, QContactJsonDbRequestManager::LocalIdFetchTransaction, req);
}

void QContactJsonDbRequestHandler::onNotified(const QString &notifyUuid, const QVariant &object, const QString &action)
{
    // There is no need for mutex locker since we do not access any request from here.

    QVariantMap jsonDbObject(object.toMap());
    if (jsonDbObject.isEmpty() || notifyUuid.isEmpty())
        return;

    QString jsonType(m_converter->jsonDbNotificationObjectToContactType(jsonDbObject));
    if (jsonType == QContactJsonDbStr::contactsJsonDbType()) {

        QContactId contactId = m_converter->jsonDbNotificationObjectToContactId(jsonDbObject);
        if (contactId.isNull())
            return;

        if (action == QContactJsonDbStr::actionCreate()) {
            m_ccs.insertAddedContact(contactId);
            startTimer();
        }
        else if (action == QContactJsonDbStr::actionUpdate()) {
            m_ccs.insertChangedContact(contactId);
            startTimer();
        }
        else if (action == QContactJsonDbStr::actionRemove()) {
            m_ccs.insertRemovedContact(contactId);
            startTimer();
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

void QContactJsonDbRequestHandler::onResponse(int transactionId, const QVariant &object)
{
    QMutexLocker locker(m_reqStateMutex);

    int contactIndex;
    QContactJsonDbRequestManager::TransactionType transactionType;
    QContactAbstractRequest* req = m_requestMgr->removeTransaction( transactionId, transactionType, contactIndex);
    // Handle responses for transactions having no QContactAbstractRequest associated.
    switch (transactionType) {
    case QContactJsonDbRequestManager::NotificationsTransaction: {
        // Notification request already removed from request manager and requires no more processing.
        return;
    }
    case QContactJsonDbRequestManager::OrphanTransaction: {
        qWarning() << Q_FUNC_INFO << "Orphaned transaction with transactionId:" << transactionId;
        return;
    }
    case QContactJsonDbRequestManager::InvalidTransaction: {
        qWarning() << Q_FUNC_INFO << "InvalidTransactionType, trId:" << transactionId;
        return;
    }
    default:
         break;
    }
    // Rest of the transaction responses should have assosisated QContactAbstractRequest.
    // However, if client already has deleted the request we just give a warning.
    if (!req) {
        qWarning() << Q_FUNC_INFO << "Transaction id" << transactionId << "missing associated QContactAbstractRequest, response object:" << object;
        return;
    }
    switch (transactionType) {
    case QContactJsonDbRequestManager::PrefetchForSaveTransaction: {
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        handleContactSavePrefetchResponse(saveReq, object, contactIndex);
        break;
    }
    case QContactJsonDbRequestManager::SaveTransaction: {
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        handleContactSaveResponse(saveReq, object, contactIndex);
        break;
    }
    case QContactJsonDbRequestManager::FetchTransaction: {
        QContactFetchRequest* fetchReq = static_cast<QContactFetchRequest*>(req);
        handleContactFetchResponse(fetchReq, object);
        break;
    }
    case QContactJsonDbRequestManager::RemoveTransaction: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        handleContactRemoveResponse(removeReq);
        break;
    }
    case QContactJsonDbRequestManager::LocalIdFetchTransaction: {
        QContactIdFetchRequest* idReq = static_cast<QContactIdFetchRequest*>(req);
        handleContactIdFetchResponse(idReq, object);
        break;
    }
    default:
        break;
   }
}

void QContactJsonDbRequestHandler::onError(int id, int code, const QString &message)
{
    QMutexLocker locker(m_reqStateMutex);
    QContactManager::Error error = QContactManager::NoError;
    switch (code) {
        case JsonDbError::NoError: {
            break;
        }
        case JsonDbError::InvalidMessage: {
            error = QContactManager::BadArgumentError;
            break;
        }
        case JsonDbError::OperationNotPermitted: {
            error = QContactManager::PermissionsError;;
            break;
        }
        case JsonDbError::QuotaExceeded: {
             error = QContactManager::OutOfMemoryError;
             break;
        }

        case JsonDbError::DatabaseError: {
             error = QContactManager::UnspecifiedError;
             break;
        }

        case JsonDbError::InvalidRequest: {
             // update request with invalid uuid
             error = QContactManager::DoesNotExistError;
             break;
        }

        default: { // Unhandled errors are mapped to UnspecifiedError
            error = QContactManager::UnspecifiedError;
            break;
        }
    }

    if (error != QContactManager::NoError) {
        // Note: The JsonDb returns a QContactManager::UnspecifiedError when trying to remove a non-existing contact
        //       and emits the message: "Cannot remove non-existing object"
        //       Capture this message to ensure proper error code emission by the contactsRemove function (DoesNotExistError)
        if (message=="Cannot remove non-existing object") {
            error = QContactManager::DoesNotExistError;
        }

        qWarning() << Q_FUNC_INFO << "Error generated from jsonDB: " + message;
    }
    int contactIndex;
    QContactJsonDbRequestManager::TransactionType transactionType;
    QContactAbstractRequest* req = m_requestMgr->removeTransaction(id, transactionType, contactIndex);
    // Handle responses for transactions having no QContactAbstractRequest associated.
    switch (transactionType) {
    case QContactJsonDbRequestManager::NotificationsTransaction:  {
        handleJsonDbNotificationsRequestError(error);
        return;
    }
    case QContactJsonDbRequestManager::OrphanTransaction: {
        qWarning() << Q_FUNC_INFO << "Orphaned transaction with transactionId:" << id;
        return;
    }
    case QContactJsonDbRequestManager::InvalidTransaction: {
        qWarning() << Q_FUNC_INFO << "InvalidTransactionType, trId:" << id;
        return;
    }
    default:
         break;
    }
    // Rest of the transaction responses should have assosisated QContactAbstractRequest.
    // However, if client already has deleted the request we just give a warning.
    if (!req) {
        qWarning() << Q_FUNC_INFO << "Transaction id" << id << "missing associated QContactAbstractRequest.";
        return;
    }
    switch (transactionType) {
    case QContactJsonDbRequestManager::FetchTransaction: {
        QList<QContact> contacts;
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), contacts,
                                                          error, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), contacts,
                                                          error, QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::PrefetchForSaveTransaction:
    case QContactJsonDbRequestManager::SaveTransaction: {
        QList<QContact> contacts;
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        QMap<int, QContactManager::Error> errorMap = saveReq->errorMap();
        if (error != QContactManager::NoError) {
            errorMap.insert(contactIndex, error);
        }
        qWarning() << Q_FUNC_INFO << "Save request failed for contact index" << contactIndex
                   << "and contact id" << saveReq->contacts().at(contactIndex).id();
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactSaveRequest(static_cast<QContactSaveRequest*>(req),contacts,
                                                            error,errorMap,QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            QContactManagerEngine::updateContactSaveRequest(static_cast<QContactSaveRequest*>(req),contacts,
                                                            error,errorMap,QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::RemoveTransaction: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        QMap<int, QContactManager::Error> errorMap = removeReq->errorMap();
        if (error != QContactManager::NoError)
            errorMap.insert(contactIndex, error);
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            // If request not yet completed, just add error to the requests' error map.
            QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::ActiveState);
        }
        break;
    }
    case QContactJsonDbRequestManager::LocalIdFetchTransaction: {
        QContactIdFetchRequest* idFetchReq = static_cast<QContactIdFetchRequest*>(req);
        QList<QContactId> ids;
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactIdFetchRequest(idFetchReq, ids, error, QContactAbstractRequest::FinishedState);
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

void QContactJsonDbRequestHandler::handleContactSaveResponse(QContactSaveRequest *req, const QVariant &object, int index)
{
    QContactManager::Error lastError = req->error();

    QMap<int, QContactManager::Error> errorMap = req->errorMap();
    QString jsonUuid = object.toMap().value(QContactJsonDbStr::uuid()).toString();
    if (!jsonUuid.isEmpty()) {
        QContact contact = req->contacts().at(index);
        QContactManager::Error error = QContactManager::NoError;
        bool isNewContact = (contact.id().isNull() || contact.id().managerUri().isEmpty());
        if (isNewContact) {
            contact.setId(convertUuidtoContactId(jsonUuid));
            QString displayLabel = m_engine->synthesizedDisplayLabel(contact, &error);
            if (error == QContactManager::NoError) {
                QContactManagerEngine::setContactDisplayLabel(&contact, displayLabel);
            } else {
                errorMap.insert(index, error);
                lastError = error;
                if (qt_debug_jsondb_contacts())
                    qDebug() << Q_FUNC_INFO << "ERROR in synthesizing display label: "
                         << error << "for contact " << contact;
                QContactManagerEngine::setContactDisplayLabel(&contact, QString(""));
            };
        }
        m_requestMgr->addContact(req, contact, index);
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

void QContactJsonDbRequestHandler::handleContactSavePrefetchResponse(QContactSaveRequest *req, const QVariant &object, int index)
{
    QVariantList jsonDbObjectList;
    QVariantMap newJsonDbItem;
    if (!object.isNull()) {
        jsonDbObjectList = object.toMap().value("data").toList();
        if (!jsonDbObjectList.isEmpty()) {
            newJsonDbItem = jsonDbObjectList.at(0).toMap();
            if (m_converter->toJsonContact(&newJsonDbItem, req->contacts().at(index))) {
                int transactionId = m_jsonDb->update(newJsonDbItem);
                m_requestMgr->addTransaction(transactionId, QContactJsonDbRequestManager::SaveTransaction, req, index);
                return;
            }
        }
    }
    qWarning() << Q_FUNC_INFO << "Empty response from jsondb for contact id" << req->contacts().at(index).id();
}


void QContactJsonDbRequestHandler::handleContactFetchResponse(QContactFetchRequest *req, const QVariant &object)
{
    // Object is a QVariant containing QVariantMap("data", QVariantList).
    // QVariantList is the list of QVariantMaps each representing one retrieved Contact card.
    QList<QContact> contacts;
    QContactManager::Error error;
    QVariantList jsonDbObjectList;
    if (!object.isNull()) {
        jsonDbObjectList = object.toMap().value("data").toList();
        m_converter->toQContacts(jsonDbObjectList, contacts, *m_engine, error);
    }
    if ((contacts.isEmpty()) || object.isNull())
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

void QContactJsonDbRequestHandler::handleContactIdFetchResponse(QContactIdFetchRequest *req, const QVariant &object)
{
    QList<QContactId> ids;
    QVariantList jsonObjects;
    if (!object.isNull()) {
        jsonObjects = object.toMap().value("data").toList();
    }
    foreach (const QVariant &jsonObject, jsonObjects) {
        QString uuid = jsonObject.toString();
        ids.append(convertUuidtoContactId(uuid));
    }
    QContactManager::Error error = QContactManager::NoError;
    if (ids.isEmpty() || object.isNull())
        error = QContactManager::DoesNotExistError;
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
    m_requestMgr->removeRequest(req);
    QContactManagerEngine::updateContactIdFetchRequest(req, ids, error, QContactAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QContactJsonDbRequestHandler::sendJsonDbNotificationsRequest()
{
    if (!m_jsonDbNotificationsRequested) {
        int transactionId = m_jsonDb->notify(JsonDbClient::NotifyTypes(JsonDbClient::NotifyCreate | JsonDbClient::NotifyUpdate | JsonDbClient::NotifyRemove),
                                             QContactJsonDbStr::contactsJsonDbNotificationQuery());
        m_requestMgr->addTransaction(transactionId, QContactJsonDbRequestManager::NotificationsTransaction);
        if (qt_debug_jsondb_contacts())
            qDebug() << Q_FUNC_INFO << "Notification request transaction id: " << transactionId;
        m_jsonDbNotificationsRequested = true;
    }
}

void QContactJsonDbRequestHandler::handleJsonDbNotificationsRequestError(QContactManager::Error error)
{
    m_jsonDbNotificationsRequested = false;
    qWarning() << Q_FUNC_INFO << "Registering for jsondb notifications failed with error:" << error;
}

QString QContactJsonDbRequestHandler::convertContactIdToUuid(QContactId &id)
{
    return id.toString().remove(QContactJsonDbStr::managerName());
}

QContactId QContactJsonDbRequestHandler::convertUuidtoContactId(QString &id)
{
    return QContactId(new QContactJsonDbId(id));
}

#include "moc_qcontactjsondbrequesthandler.cpp"

QTCONTACTS_END_NAMESPACE
