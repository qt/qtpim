/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
#include "qcontactjsondbglobal.h"

#include <jsondb-error.h>
#include <jsondb-client.h>
#include <private/jsondb-connection_p.h>
#include <private/jsondb-strings_p.h>
Q_USE_JSONDB_NAMESPACE
#include <QDebug>
#include <QString>
#include <QMap>

QContactJsonDbRequestHandler::QContactJsonDbRequestHandler()
    : m_engine(0)
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

    m_jsonConnection = new JsonDbConnection(this);
    m_jsonConnection->connectToServer();
    m_jsonDb = new JsonDbClient(m_jsonConnection);

    connect(m_jsonDb, SIGNAL(notified(const QString&, const QVariant&, const QString&)),
            this, SLOT(onNotified(const QString&, const QVariant&, const QString&)));
    connect(m_jsonDb, SIGNAL(response(int, const QVariant)), this,
            SLOT(onResponse(int, const QVariant)));
    connect(m_jsonDb, SIGNAL(error(int, int, const QString&)), this,
            SLOT(onError(int, int, const QString&)));
}

void QContactJsonDbRequestHandler::setEngine(QContactJsonDbEngine *engine)
{
    m_engine = engine;
}

bool QContactJsonDbRequestHandler::waitForRequestFinished(QtAddOn::Pim::QContactAbstractRequest *req, int msecs)
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
    // "addTransaction" from requestManager, sends resultsAvailable signal
    QMutexLocker locker(m_reqStateMutex);
    QContactManager::Error error = QContactManager::NoError;
    QContactManager::Error lastError = QContactManager::NoError;
    QMap<int, QContactManager::Error> errorMap;
    if (m_reqList.contains(req)) {
        error = QContactManager::UnspecifiedError;
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] Trying to handle destroyed request: throwing ERROR: " << error;
        return;
    }
    switch (req->type()) {
    case QContactAbstractRequest::ContactSaveRequest: {
        //TODO: handle duplicates(?)
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        QList<QContact> contacts = saveReq->contacts();
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, error, errorMap, QContactAbstractRequest::ActiveState);
        m_requestMgr->addRequest(saveReq, contacts);
        bool doesContactExist = false;
        for (int i = 0; i < contacts.size(); i++) {
            QContact contact = contacts.at(i);
            QVariantMap newJsonDbItem;
            error = QContactManager::NoError;
            doesContactExist = !(contact.id().localId().isEmpty() || contact.id().managerUri().isEmpty());

            if (!m_engine->validateContact(contact, &error)) {
                error = QContactManager::InvalidContactTypeError;
                errorMap.insert(i,error);
                lastError = error;
            }
            if (error == QContactManager::NoError) {
                if (m_converter->toJsonContact(&newJsonDbItem, contact)) {
                    int trId;
                    if (doesContactExist) {
                        trId = m_jsonDb->update(newJsonDbItem);
                    } else {
                        trId = m_jsonDb->create(newJsonDbItem);
                    }
                    m_requestMgr->addTransaction(saveReq, trId, i);
                } else {
                    error = QContactManager::BadArgumentError;
                    errorMap.insert(i,error);
                    lastError = error;
                }
            }

        };
        QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, error, errorMap, QContactAbstractRequest::ActiveState);
        /*if (error != QContactManager::NoError) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
            m_requestMgr->removeRequest(saveReq);
            QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, error, errorMap, QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        }*/
        break;
    }
    case QContactAbstractRequest::ContactFetchRequest: {
        QContactFetchRequest* fetchReq = static_cast<QContactFetchRequest*>(req);
        QList<QContact> contacts;
        m_requestMgr->addRequest(fetchReq);
        QString newJsonDbQuery = m_converter->queryFromRequest(req);
        int trId = m_jsonDb->query(newJsonDbQuery);
        m_requestMgr->addTransaction(fetchReq, trId);
        break;
    }
    case QContactAbstractRequest::DetailDefinitionFetchRequest:
    {
      QContactDetailDefinitionFetchRequest* r = static_cast<QContactDetailDefinitionFetchRequest*>(req);
      QContactManager::Error operationError = QContactManager::NoError;
      QMap<int, QContactManager::Error> errorMap;
      QMap<QString, QContactDetailDefinition> requestedDefinitions;
      m_requestMgr->addRequest(r);

      QStringList names = r->definitionNames();
      if (names.isEmpty())
          names = m_engine->detailDefinitions(r->contactType(), &operationError).keys(); // all definitions.

      QContactManager::Error tempError;
      for (int i = 0; i < names.size(); i++) {
          QContactDetailDefinition current = m_engine->detailDefinition(names.at(i), r->contactType(), &tempError);
          requestedDefinitions.insert(names.at(i), current);

          errorMap.insert(i, tempError);
          if (tempError != QContactManager::NoError) {
              operationError = tempError;
          }
      }
      if (m_requestMgr->isRequestCompleted(req)) {
          m_engine->updateDefinitionFetchRequest(r, requestedDefinitions, operationError, errorMap, QContactAbstractRequest::FinishedState);
          QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
          if (waitCondition) {
              waitCondition->wakeAll();
              m_requestMgr->removeRequest(req);
          }
 }
      break;
    }
    case QContactAbstractRequest::ContactRemoveRequest: {
        //TODO: handle duplicates(?)

        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        QList<QContactLocalId> contactIds = removeReq->contactIds();
        QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::ActiveState);
        m_requestMgr->addRequest(removeReq);

        bool transactionsMade = false;
        for (int i = 0; i < contactIds.size(); i++) {
            QContactLocalId contactLocalId = contactIds.at(i);            
            QContactId contactId;
            contactId.setManagerUri(m_engine->managerUri());
            int trId;
            bool contactExist = !(contactLocalId.isEmpty() || contactLocalId == "" || contactId.managerUri().isEmpty()); // || !contactId.managerUri().contains(contactLocalId)
            if (contactExist) {
                QVariantMap newJsonDbItem;
                newJsonDbItem.insert(JsonDbString::kUuidStr, contactLocalId);
                trId = m_jsonDb->remove(newJsonDbItem);
                m_requestMgr->addTransaction(removeReq, trId, i);
                transactionsMade = true;
            } else {
                // Contact not found. DoesNotExistError to be generated for this contact id.
                error = QContactManager::DoesNotExistError;
                errorMap.insert(i,error);
                lastError = error;
            }
        }

        if (transactionsMade) {
            QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::ActiveState);
        } else {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(removeReq);
            m_requestMgr->removeRequest(removeReq);
            QContactManagerEngine::updateContactRemoveRequest(static_cast<QContactRemoveRequest*>(removeReq),
                                                              error,errorMap,QContactAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        }
        break;
    }
    default:
        break;
    }
}

void QContactJsonDbRequestHandler::onNotified(const QString &notifyUuid, const QVariant &object, const QString &action)
{
    Q_UNUSED(notifyUuid);
    Q_UNUSED(object);
    Q_UNUSED(action);
}

void QContactJsonDbRequestHandler::onResponse(int id, const QVariant &object)
{
    QMutexLocker locker(m_reqStateMutex);

    QContactManager::Error error = QContactManager::NoError;
    QMap<int, QContactManager::Error> errorMap;
    int contactIndex;
    QContactAbstractRequest* req = m_requestMgr->removeTransaction(id, contactIndex);

    if (!req) {
        qWarning() << Q_FUNC_INFO << "request not found trid:" << id;
        return;
    }
    switch (req->type()) {
    case QContactAbstractRequest::ContactSaveRequest: {
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        errorMap = saveReq->errorMap();
        QString jsonUuid = object.toMap().value("_uuid").toString();
        if (!jsonUuid.isEmpty()) {
            QContact contact = saveReq->contacts().at(contactIndex);
            bool isNewContact = (contact.id().localId().isEmpty() || contact.id().managerUri().isEmpty());
            if (isNewContact) {
                QContactId contactId;
                contactId.setLocalId (jsonUuid);
                contactId.setManagerUri(m_engine->managerUri());
                contact.setId(contactId);
                m_ccs.insertAddedContact(contactId.localId());
                QString displayLabel = m_engine->synthesizedDisplayLabel(contact, &error);
                if (error == QContactManager::NoError) {
                    QContactManagerEngine::setContactDisplayLabel(&contact, displayLabel);
                } else {
                    //TODO Error handling(!)
                    errorMap.insert(contactIndex, error);
                    if (qt_debug_jsondb_contacts())
                        qDebug() << "[QContactJsonDb] ERROR in synthesizing display label: "
                             << error << "for contact " << contact;
                    //QContactManagerEngine::setContactDisplayLabel(contact, QString(""));
                };
            }
            else {
                m_ccs.insertChangedContact(contact.id().localId());
            }
            m_requestMgr->addContact(req, contact, contactIndex);
        }

        if (m_requestMgr->isRequestCompleted(req)) {
            QList<QContact> contacts = m_requestMgr->contacts(req);
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactSaveRequest(saveReq, contacts, error, errorMap, QContactAbstractRequest::FinishedState);
            m_ccs.emitSignals(m_engine);
            m_ccs.clearAddedContacts();
            m_ccs.clearChangedContacts();
            if (waitCondition)
                waitCondition->wakeAll();
        }
        break;
    }
    case QContactAbstractRequest::ContactFetchRequest: {
        QList<QContact> contacts;
        // object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of QVariantMaps
        // each representing one retrieved Contact card.

        QVariantList jsonDbObjectList;
        if (!object.isNull()) {
            jsonDbObjectList = object.toMap().value("data").toList();
            m_converter->toQContacts(jsonDbObjectList, contacts, *m_engine, error);
        }
        if ((contacts.isEmpty()) || object.isNull())
            error = QContactManager::DoesNotExistError;
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
        m_requestMgr->removeRequest(req);
        QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), contacts, error, QContactAbstractRequest::FinishedState);

        if (waitCondition)
            waitCondition->wakeAll();
        break;
    }
    case QContactAbstractRequest::ContactRemoveRequest: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        errorMap = removeReq->errorMap();
        QString jsonUuid = object.toMap().value("_uuid").toString();

        if (!jsonUuid.isEmpty()) {
            QContactLocalId contactLocalId = removeReq->contactIds().at(contactIndex);
            QContactId contactId;
            contactId.setLocalId (jsonUuid);
            contactId.setManagerUri(m_engine->managerUri());
            //contact.setId(contactId);
            m_ccs.insertRemovedContact(contactLocalId);
            // (CB) Check if the following part is needed !!
            /*
                QString displayLabel = m_engine->synthesizedDisplayLabel(contact, &error);
                if (error == QContactManager::NoError) {
                    QContactManagerEngine::setContactDisplayLabel(&contact, displayLabel);
                } else {
                    //TODO Error handling(!)
                    errorMap.insert(contactIndex, error);
                    qDebug() << "ERROR in synthesizing display label: "
                             << error << "for contact " << contact;
                    //QContactManagerEngine::setContactDisplayLabel(contact, QString(""));
                };
            */
            // (CB) Check if the following line is needed !!
            //m_requestMgr->addContact(req, contact, contactIndex);
        }

        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::FinishedState);
            m_ccs.emitSignals(m_engine);
            m_ccs.clearRemovedContacts();
            if (waitCondition)
                waitCondition->wakeAll();
        }
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
    int itemIndex;
    QContactAbstractRequest* req = m_requestMgr->removeTransaction(id, itemIndex);
    if (!req) {
        qWarning() << Q_FUNC_INFO << "request not found";
        return;
    }
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

        if (qt_debug_jsondb_contacts()) {
            qDebug() << "[QContactJsonDb] Error generated from jsonDB: " + message;
        }
    }

    switch (req->type()) {
    case QContactAbstractRequest::ContactFetchRequest: {
        QList<QContact> contacts;
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), contacts,
                                                          error, QContactAbstractRequest::FinishedState);
            if (waitCondition) {
                waitCondition->wakeAll();
            }
        } else {
            QContactManagerEngine::updateContactFetchRequest (static_cast<QContactFetchRequest*>(req), contacts,
                                                          error, QContactAbstractRequest::ActiveState);
        }

        break;
    }
    case QContactAbstractRequest::ContactSaveRequest: {
        QList<QContact> contacts;
        QContactSaveRequest* saveReq = static_cast<QContactSaveRequest*>(req);
        QMap<int, QContactManager::Error> errorMap = saveReq->errorMap();
        if (error != QContactManager::NoError) {
            errorMap.insert(itemIndex, error);
        }
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactSaveRequest(static_cast<QContactSaveRequest*>(req),contacts,
                                                            error,errorMap,QContactAbstractRequest::FinishedState);
            if (waitCondition) {
                waitCondition->wakeAll();
            }
        } else {
            QContactManagerEngine::updateContactSaveRequest(static_cast<QContactSaveRequest*>(req),contacts,
                                                            error,errorMap,QContactAbstractRequest::ActiveState);
        }

        break;
    }
    case QContactAbstractRequest::ContactRemoveRequest: {
        QContactRemoveRequest* removeReq = static_cast<QContactRemoveRequest*>(req);
        QMap<int, QContactManager::Error> errorMap = removeReq->errorMap();
        if (error != QContactManager::NoError) {
            errorMap.insert(itemIndex, error);
        }
        if (m_requestMgr->isRequestCompleted(req)) {
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(req);
            m_requestMgr->removeRequest(req);
            QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::FinishedState);
            m_ccs.emitSignals(m_engine);
            m_ccs.clearRemovedContacts();
            if (waitCondition)
                waitCondition->wakeAll();
        } else {
            // If request not yet completed, just add error to the requests' error map.
            QContactManagerEngine::updateContactRemoveRequest(removeReq, error, errorMap, QContactAbstractRequest::ActiveState);
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
