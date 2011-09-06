/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
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

#include "qorganizerjsondbrequestthread.h"
#include "qorganizerjsondbid.h"
#include "qorganizerjsondbengine.h"
#include "qorganizerjsondbrequestmanager.h"
#include "qorganizerjsondbstring_p.h"
#include "qorganizer.h"

#include <jsondb-client.h>
#include <private/jsondb-connection_p.h>
#include <private/jsondb-strings_p.h>
#include <jsondb-error.h>
Q_USE_JSONDB_NAMESPACE

#include <QDebug>


QOrganizerJsonDbRequestThread::QOrganizerJsonDbRequestThread()
    : m_engine(0), m_jsonDb(0), m_requestMgr(0), m_reqStateMutex(0)
{
}

QOrganizerJsonDbRequestThread::~QOrganizerJsonDbRequestThread()
{
    delete m_reqStateMutex;
    m_reqStateMutex = 0;
    delete m_requestMgr;
    m_requestMgr = 0;
    delete m_jsonDb;
    m_jsonDb = 0;
    m_collectionsIdList.clear();
}

void QOrganizerJsonDbRequestThread::run()
{
    m_reqStateMutex = new QMutex();
    m_jsonConnection = new JsonDbConnection(this);
    m_jsonConnection->connectToServer();
    m_jsonDb = new JsonDbClient(m_jsonConnection);
    m_requestMgr = new QOrganizerJsonDbRequestManager();

    connect(m_jsonDb, SIGNAL(notified(const QString&, const QVariant&, const QString&)),
            this, SLOT(onNotified(const QString&, const QVariant&, const QString&)));
    connect(m_jsonDb, SIGNAL(response(int, const QVariant)), this,
            SLOT(onResponse(int, const QVariant)));
    connect(m_jsonDb, SIGNAL(error(int, int, const QString&)), this,
            SLOT(onError(int, int, const QString&)));

    emit initialized();
    QThread::run();
}

void QOrganizerJsonDbRequestThread::setEngine(QOrganizerJsonDbEngine *engine)
{
    // This function is called by the QOrganizerJsonDbEngine thread (main thread)
    m_engine = engine;
}

void QOrganizerJsonDbRequestThread::addRequest(QOrganizerAbstractRequest* req)
{
    // This function is called by the QOrganizerJsonDbEngine thread (main thread)
    QMutexLocker locker(m_reqStateMutex);
    m_requestMgr->addRequest(req);
}

bool QOrganizerJsonDbRequestThread::waitForRequestFinished(QOrganizerAbstractRequest *req, int msecs)
{
    // This function is called by the QOrganizerJsonDbEngine thread (main thread)

    // TODO: timeout handling
    Q_UNUSED(msecs);

    QMutexLocker locker(m_reqStateMutex);
    QWaitCondition waitCondition;

    if (req->state() == QOrganizerAbstractRequest::FinishedState)
        return true;
    else if (req->state() == QOrganizerAbstractRequest::CanceledState)
        return false;

    // Request might still be inactive if this function is called immediatelly after sending a signal to
    // handleRequest slot. The signal goes to event loop and might be handled later than this function is executed.
    m_requestMgr->setWaitCondition(req, &waitCondition);
    bool requestFinished;

    if (msecs <= 0)
        requestFinished = waitCondition.wait(m_reqStateMutex);
    else
        requestFinished = waitCondition.wait(m_reqStateMutex, msecs);

    m_requestMgr->removeWaitCondition(req);
    return requestFinished;
}

void QOrganizerJsonDbRequestThread::requestDestroyed(QOrganizerAbstractRequest* req)
{
    // This function is called by the QOrganizerJsonDbEngine thread (main thread)
    QMutexLocker locker(m_reqStateMutex);
    m_requestMgr->setDeleted(req);
}

QOrganizerCollection QOrganizerJsonDbRequestThread::defaultCollection() const
{
    // This function is called by the QOrganizerJsonDbEngine thread (main thread)
    return m_defaultCollection;
}

void QOrganizerJsonDbRequestThread::setDefaultCollection(QOrganizerCollection collection)
{
    // This function is called by the QOrganizerJsonDbEngine thread (main thread)
    m_defaultCollection = collection;
}

void QOrganizerJsonDbRequestThread::handleRequest(QOrganizerAbstractRequest *req)
{
    QMutexLocker locker(m_reqStateMutex);

    QOrganizerJsonDbRequestManager::HandlingStatus status = m_requestMgr->requestStatus(req);
    if (status == QOrganizerJsonDbRequestManager::Deleted) {
        m_requestMgr->removeRequest(req);
        return;
    } else if (status == QOrganizerJsonDbRequestManager::Invalid) {
        return;
    }

    switch (req->type()) {
    case QOrganizerAbstractRequest::ItemSaveRequest:
    {
        handleItemSaveRequest(static_cast<QOrganizerItemSaveRequest*>(req));
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchRequest:
    {
        handleItemFetchRequest(static_cast<QOrganizerItemFetchRequest*>(req));
        break;
    }
    case QOrganizerAbstractRequest::ItemIdFetchRequest:
    {
        handleItemIdFetchRequest(static_cast<QOrganizerItemIdFetchRequest*>(req));
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchByIdRequest:
    {
        handleItemFetchByIdRequest(static_cast<QOrganizerItemFetchByIdRequest*>(req));
        break;
    }
    case QOrganizerAbstractRequest::ItemRemoveRequest:
    {
        handleItemRemoveRequest(static_cast <QOrganizerItemRemoveRequest *> (req));
        break;
    }
    case QOrganizerAbstractRequest::CollectionSaveRequest:
    {
        handleCollectionSaveRequest(static_cast <QOrganizerCollectionSaveRequest *> (req));
        break;
    }
    case QOrganizerAbstractRequest::CollectionFetchRequest:
    {
        handleCollectionFetchRequest(static_cast<QOrganizerCollectionFetchRequest*>(req));
        break;
    }
    case QOrganizerAbstractRequest::CollectionRemoveRequest:
    {
        handleCollectionRemoveRequest(static_cast<QOrganizerCollectionRemoveRequest*>(req));
        break;
    }

    default:
        break;
    }
}


void QOrganizerJsonDbRequestThread::onNotified(const QString &notifyUuid, const QVariant &object, const QString &action)
{
    Q_UNUSED(notifyUuid);
    Q_UNUSED(object);
    Q_UNUSED(action);
}

void QOrganizerJsonDbRequestThread::onResponse(int trId, const QVariant &object)
{
    QMutexLocker locker(m_reqStateMutex);
    QOrganizerManager::Error error = handleErrorResponse(object, JsonDbError::NoError);
    handleResponse(trId, error, object);
}

void QOrganizerJsonDbRequestThread::onError(int trId, int errorCode, const QString &message)
{
    Q_UNUSED(message);
    QMutexLocker locker(m_reqStateMutex);
    QVariant object;
    QOrganizerManager::Error error = handleErrorResponse(object, errorCode);
    handleResponse(trId, error, object);
}

void QOrganizerJsonDbRequestThread::handleResponse(int trId, QOrganizerManager::Error error, const QVariant &object)
{
    int index;
    QOrganizerAbstractRequest* req = m_requestMgr->removeTransaction(trId, index);
    if (!req) {
        return;
    }
    if (m_requestMgr->requestStatus(req) == QOrganizerJsonDbRequestManager::Deleted) {
        m_requestMgr->removeRequest(req);
        return;
    }

    switch (req->type()) {
    case QOrganizerAbstractRequest::ItemSaveRequest:
    {
        handleItemSaveResponse(static_cast<QOrganizerItemSaveRequest*>(req), object, index, error);
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchRequest:
    {
        handleItemFetchResponse(static_cast<QOrganizerItemFetchRequest*>(req), object, error);
        break;
    }
    case QOrganizerAbstractRequest::ItemIdFetchRequest:
    {
        handleItemIdFetchResponse(static_cast<QOrganizerItemIdFetchRequest*>(req), object, error);
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchByIdRequest:
    {
        handleItemFetchByIdResponse(static_cast<QOrganizerItemFetchByIdRequest*>(req), object, error);
        break;
    }
    case QOrganizerAbstractRequest::ItemRemoveRequest:
    {
        handleItemRemoveResponse(static_cast<QOrganizerItemRemoveRequest*>(req), object, index, error);
        break;
    }
    case QOrganizerAbstractRequest::CollectionSaveRequest:
    {
        handleCollectionSaveResponse(static_cast<QOrganizerCollectionSaveRequest*>(req), object, index, error);
        break;
    }
    case QOrganizerAbstractRequest::CollectionFetchRequest:
    {
        handleCollectionFetchResponse(static_cast<QOrganizerCollectionFetchRequest*>(req), object, error);
        break;
    }
    case QOrganizerAbstractRequest::CollectionRemoveRequest:
    {
        handleCollectionRemoveResponse (static_cast<QOrganizerCollectionRemoveRequest*>(req), object, index, error);
        break;
    }
    default:
        break;
    }
}

void QOrganizerJsonDbRequestThread::handleItemSaveRequest(QOrganizerItemSaveRequest* saveReq)
{
    QOrganizerManager::Error error = QOrganizerManager::NoError;
    bool errorFound = false;
    QList<QOrganizerItem> items = saveReq->items();
    m_requestMgr->setActive(saveReq);

    for (int i = 0; i < items.size(); i++) {
        QOrganizerItem item = items.at(i);

        bool itemIsNew = item.id().isNull();

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!itemIsNew && (managerUri != item.id().managerUri())) {
            error = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }
        // ensure that the organizeritem's details conform to their definitions
        if (!errorFound && !m_engine->validateItem(item, &error))
            errorFound = true;

        //Check the collection id validation
        if (!item.collectionId().isNull()) {
            QOrganizerCollectionId collectionID = item.collectionId();
            //If we could find the collection id in collection id list
            if (!m_collectionsIdList.contains(collectionID)) {
                //Collection id is invalid
                error = QOrganizerManager::BadArgumentError;
                errorFound = true;
            }
        } else {
            item.setCollectionId(defaultCollection().id());
        }

        QVariantMap newJsonDbItem;
        if (!errorFound) {
            if (convertItemToJsonDbObject(item, &newJsonDbItem)) {
                int trId;
                if (itemIsNew)
                    trId = m_jsonDb->create(newJsonDbItem);
                else
                    trId = m_jsonDb->update(newJsonDbItem);

                m_requestMgr->addTransaction(saveReq, trId, i);
            } else {
                errorFound = true;
                error = QOrganizerManager::InvalidItemTypeError;
            }
        }

        if (errorFound) {
            QMap<int, QOrganizerManager::Error> errorMap;
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
            m_requestMgr->removeRequest(saveReq);
            QOrganizerManagerEngine::updateItemSaveRequest(saveReq, items, error, errorMap, QOrganizerAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        }
    }
}

void QOrganizerJsonDbRequestThread::handleItemFetchRequest(QOrganizerItemFetchRequest *fetchReq)
{
    QList<QOrganizerItem> items;
    m_requestMgr->setActive(fetchReq);

    //jsondb query [?type="com.nokia.mp.organizer.Item"]
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;//"=\"OrganizerItem\"]";
    //Apply Filter and get expression
    newJsonDbQuery += filterToJsondbQuery (fetchReq->filter());
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_requestMgr->addTransaction(fetchReq, trId);
}

void QOrganizerJsonDbRequestThread::handleItemIdFetchRequest(QOrganizerItemIdFetchRequest *idFetchReq)
{
    QList<QOrganizerItem> items;
    m_requestMgr->setActive(idFetchReq);

    //jsondb query [?type="com.nokia.mp.organizer.Item"][=_uuid]
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;
    newJsonDbQuery += ITEM_ID_RESULT_STRING;
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_requestMgr->addTransaction(idFetchReq, trId);
}

void QOrganizerJsonDbRequestThread::handleItemFetchByIdRequest(QOrganizerItemFetchByIdRequest *fetchByIdReq)
{
    QList<QOrganizerItem> items;
    QList<QOrganizerItemId> ids = fetchByIdReq->ids();
    QString itemQuery = "[?";
    for (int i = 0; i < ids.size(); i++) {
        itemQuery += ITEM_IDS_QUERY_STRING (ids[i].toString().remove (QOrganizerJsonDbStr::ManagerName));
    }
    // remove the last " | "
    itemQuery.truncate(itemQuery.length() - 3);
    itemQuery += "]";
    m_requestMgr->setActive(fetchByIdReq);
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;
    newJsonDbQuery += itemQuery;
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_requestMgr->addTransaction(fetchByIdReq, trId);
}

void QOrganizerJsonDbRequestThread::handleItemRemoveRequest(QOrganizerItemRemoveRequest *removeReq)
{
    //Add the remove request into request queue
    m_requestMgr->setActive (removeReq);

    //Get remove item ids list
    QList<QOrganizerItemId> itemIdList = removeReq->itemIds ();

    //loop to remove all the items from the remove items ids list
    for (int i = 0; i < itemIdList.size(); i++) {
        //Get the uuid of the removed item and remove the "qtorgainizer:jsondb::" prefix
        QString jsonUuid = itemIdList.at(i).toString();
        jsonUuid = jsonUuid.remove (QOrganizerJsonDbStr::ManagerName);

        QVariantMap JsonDbRemoveItem;
        JsonDbRemoveItem.insert (JsonDbString::kUuidStr, jsonUuid);

        //Remove the item from JSONDB
        int trId = m_jsonDb->remove (JsonDbRemoveItem);

        //Attach the transaction id / reference id to request m_transactionMap, i is the index of the item.
        m_requestMgr->addTransaction (removeReq, trId, i);
    }
}

void QOrganizerJsonDbRequestThread::handleCollectionSaveRequest(QOrganizerCollectionSaveRequest *collectionSaveReq)
{
    QOrganizerManager::Error error = QOrganizerManager::NoError;
    bool errorFound = false;
    QList<QOrganizerCollection> collections = collectionSaveReq->collections();
    m_requestMgr->setActive(collectionSaveReq);

    for (int i = 0; i < collections.size(); i++) {
        QOrganizerCollection collection = collections.at(i);

        bool collectionIsNew = collection.id().isNull();

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!collectionIsNew && (managerUri != collection.id().managerUri())) {
            error = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }

        QVariantMap newJsonDbCollection;
        if (!errorFound) {
            if (convertCollectionToJsonDbObject (collection, &newJsonDbCollection)) {
                int trId;
                if (collectionIsNew)
                    trId = m_jsonDb->create(newJsonDbCollection);
                else
                    trId = m_jsonDb->update(newJsonDbCollection);

                m_requestMgr->addTransaction(collectionSaveReq, trId, i);
            } else {
                // NOTE!!! check this!!! no errors this far...
                errorFound = true;
                error = QOrganizerManager::InvalidCollectionError;
            }
        }

        if (errorFound) {
            QMap<int, QOrganizerManager::Error> errorMap;
            QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionSaveReq);
            m_requestMgr->removeRequest(collectionSaveReq);
            QOrganizerManagerEngine::updateCollectionSaveRequest(collectionSaveReq, collections, error, errorMap, QOrganizerAbstractRequest::FinishedState);
            if (waitCondition)
                waitCondition->wakeAll();
        }
    }
}

void QOrganizerJsonDbRequestThread::handleCollectionFetchRequest(QOrganizerCollectionFetchRequest *collectionFetchReq)
{
    QList<QOrganizerCollection> collections;
    m_requestMgr->setActive(collectionFetchReq);
    //jsondb query [?_type="com.nokia.mp.organizer.Collection"]
    QString newJsonDbQuery = COLLECTION_QUERY_STRING;
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_requestMgr->addTransaction(collectionFetchReq, trId);
}

void QOrganizerJsonDbRequestThread::handleCollectionRemoveRequest(QOrganizerCollectionRemoveRequest* collectionRemoveReq)
{   
    //Add the remove request into request queue
    m_requestMgr->setActive (collectionRemoveReq);
    //Get reuest remove collection ids list
    QList<QOrganizerCollectionId> removeCollectionIdReqList = collectionRemoveReq->collectionIds();
    //List after remove the invalid collection ids such as empty id and default collection id
    //This is for removing the collection items,
    QSet<QOrganizerCollectionId> removeCollectionIdValidSet;
    //Jsondb remove request list
    QVariantList removeCollectionJsonDbList;
    QOrganizerManager::Error error;

    //Check the request list and build jsondb remove list
    for (int i = 0; i < removeCollectionIdReqList.size(); i++) {
        QString uuid = removeCollectionIdReqList.at(i).toString().remove (QOrganizerJsonDbStr::ManagerName);
        if (uuid.isEmpty()) {
            //Uuid is missing
            error = QOrganizerManager::BadArgumentError;
            //Set error list
            m_requestMgr->updateRequestData(collectionRemoveReq, error, i);
            continue;
        }

        if (uuid != defaultCollectionId ()) {
            //This is a valid item and insert one id to jsondb remove list
            QVariantMap removeItem;
            removeItem.insert (JsonDbString::kUuidStr, uuid);
            removeCollectionJsonDbList << removeItem;
            removeCollectionIdValidSet.insert (removeCollectionIdReqList.at(i));
        } else {
            //Default collection can not be removed
            error = QOrganizerManager::PermissionsError;
            m_requestMgr->updateRequestData (collectionRemoveReq, error, i);
            QString warning = QOrganizerJsonDbStr::WarningDefaultCollectionRemove;
            qWarning () << warning;
        }
    }

    if (removeCollectionJsonDbList.size() > 0) {
        //Send request to jsondb
        int trId = m_jsonDb->remove (removeCollectionJsonDbList);
        //Add the collections remove jsondb transaction id into request list 0 to mark the collection remove sub request
        m_requestMgr->addTransaction (collectionRemoveReq, trId, 0);

        //Remove the items that are belong to collections
        QString jsondbQuery = ALL_ITEM_QUERY_STRING;

        //Set the filter for the jsondb query string
        QOrganizerItemCollectionFilter collectonfilter;
        collectonfilter.setCollectionIds (removeCollectionIdValidSet);
        jsondbQuery += filterToJsondbQuery (collectonfilter);

        //Send items remove request to jsondb
        trId = m_jsonDb->remove (jsondbQuery);
        //Add the items remove jsondb transaction id into request 1 to mark the items remove sub request
        m_requestMgr->addTransaction (collectionRemoveReq, trId, 1);

    } else {//The request list is empty. Nothing need to do with Jsondb and no need to wait for the response
        QOrganizerManager::Error latestError;
        QMap<int, QOrganizerManager::Error> errorMap;
        m_requestMgr->requestData(collectionRemoveReq, &latestError, &errorMap);
        //Change the request status
        QOrganizerManagerEngine::updateCollectionRemoveRequest (collectionRemoveReq, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
        //Reset waitCondition
        QWaitCondition* waitCondition = m_requestMgr->waitCondition (collectionRemoveReq);
        if (waitCondition)
            waitCondition->wakeAll ();
        //Remove the request
        m_requestMgr->removeRequest (collectionRemoveReq);
    }
}

QString QOrganizerJsonDbRequestThread::defaultCollectionId () const
{
    const QString id = m_defaultCollection.id().toString().remove(QOrganizerJsonDbStr::ManagerName);

    if (id.isEmpty()) {
        QString warning = QOrganizerJsonDbStr::WarningCollectionMissing;
        qWarning() << warning;
    }
    return id;
}

void QOrganizerJsonDbRequestThread::handleItemSaveResponse(QOrganizerItemSaveRequest *saveReq, const QVariant &object,
                                                           int index, QOrganizerManager::Error error)
{
    QOrganizerItem item = saveReq->items().at(index);
    if (error == QOrganizerManager::NoError) {
        if (item.id().isNull()) {
            // this is a new item
            QString jsonUuid = object.toMap().value(JsonDbString::kUuidStr).toString();
            if (!jsonUuid.isEmpty()) {
                QOrganizerJsonDbItemId* itemId = new QOrganizerJsonDbItemId;
                itemId->setItemId(jsonUuid);
                item.setId(QOrganizerItemId(itemId));
                //set globally unique identifier
                item.setGuid(jsonUuid);
                m_ics.insertAddedItem(item.id());
            }
            if (item.collectionId().isNull()) {
                item.setCollectionId (defaultCollection().id());
            }
        } else {
            // this is modified item
            m_ics.insertChangedItem(item.id());
        }
    }
    m_requestMgr->updateRequestData(saveReq, error, index, item);
    if (m_requestMgr->isRequestCompleted(saveReq)) {
        QOrganizerManager::Error latestError;
        QMap<int, QOrganizerManager::Error> errorMap;
        QList<QOrganizerItem> items;
        m_requestMgr->requestData(saveReq, &latestError, &errorMap,&items);
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
        m_requestMgr->removeRequest(saveReq);
        QOrganizerManagerEngine::updateItemSaveRequest(saveReq, items, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
        m_ics.emitSignals(m_engine);
        m_ics.clearAddedItems();
        m_ics.clearChangedItems();
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QOrganizerJsonDbRequestThread::handleItemFetchResponse(QOrganizerItemFetchRequest *fetchReq, const QVariant &object, QOrganizerManager::Error error)
{
    QList<QOrganizerItem> items;
    // Object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of QVariantMaps
    // Each representing one retrieved OrganizerItem

    QVariantList jsonDbObjectList;
    if (!object.isNull()) {
        jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();
    }

    for (int i = 0; i < jsonDbObjectList.size(); i++) {
        QOrganizerItem item;
        if (convertJsonDbObjectToItem(jsonDbObjectList.at(i).toMap(), &item)) {
            if (QOrganizerManagerEngine::isItemBetweenDates(item, fetchReq->startDate(), fetchReq->endDate()))
                items.append(item);
        }
    }
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchReq);
    m_requestMgr->removeRequest(fetchReq);
    QOrganizerManagerEngine::updateItemFetchRequest(fetchReq, items, error, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemIdFetchResponse(QOrganizerItemIdFetchRequest *idFetchReq, const QVariant &object, QOrganizerManager::Error error)
{
    QList<QOrganizerItemId> ids;
    // object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of strings??
    // each representing one retrieved OrganizerItemId

    QVariantList jsonDbObjectList;
    if (!object.isNull()) {
        jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();
    }

    for (int i = 0; i < jsonDbObjectList.size(); i++) {
        QOrganizerItemId itemId(new QOrganizerJsonDbItemId(jsonDbObjectList.at(i).toString()));
        ids.append(itemId);
    }
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(idFetchReq);
    m_requestMgr->removeRequest(idFetchReq);
    QOrganizerManagerEngine::updateItemIdFetchRequest(idFetchReq, ids, error, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemFetchByIdResponse(QOrganizerItemFetchByIdRequest *fetchByIdReq, const QVariant &object, QOrganizerManager::Error error)
{
    QList<QOrganizerItem> items;
    QMap<int, QOrganizerManager::Error> errorMap;
    QMap<QOrganizerItemId, QOrganizerItem> itemMap;
    QList<QOrganizerItemId> ids = fetchByIdReq->ids();

    // object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of QVariantMaps
    // each representing one retrieved OrganizerItem

    QVariantList jsonDbObjectList;
    if (!object.isNull()) {
        jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();
    }

    int i;
    for (i = 0; i < jsonDbObjectList.size(); i++) {
        QOrganizerItem item;
        if (convertJsonDbObjectToItem(jsonDbObjectList.at(i).toMap(), &item))
            itemMap.insert(item.id(), item);
    }
    for (i = 0; i < ids.size(); i++) {
        QOrganizerItem item = itemMap.value(ids[i], QOrganizerItem());
        items.append(item);
        if (item.isEmpty()) {
            error = QOrganizerManager::DoesNotExistError;
            errorMap.insert(i, QOrganizerManager::DoesNotExistError);
        }
    }

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchByIdReq);
    m_requestMgr->removeRequest(fetchByIdReq);
    QOrganizerManagerEngineV2::updateItemFetchByIdRequest(fetchByIdReq, items, error, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemRemoveResponse(QOrganizerItemRemoveRequest *removeReq, const QVariant &object, int index, QOrganizerManager::Error error)
{
    //Get the number of items that was removed from JSONDB
    int delCount = object.toMap().value (JsonDbString::kCountStr).toInt ();
    if (QOrganizerManager::NoError == error) {
        //Check the count for the result. the count may > 0
        if (delCount > 1) {
            //More than 1 item delete from jsondb!! same uuid refers to multipul items!
            QString warning = QOrganizerJsonDbStr::WarningMoreItemDelete;
            qWarning() << delCount << warning;
        } else {// successfull delete
            m_ics.insertRemovedItem(removeReq->itemIds().at(index));
        }
    } else {
        //something wrong with jsondb request and update the error map
        m_requestMgr->updateRequestData(removeReq, error, index);
    }

    //One remove request may contain more than 1 items, make sure all are processed!
    if (m_requestMgr->isRequestCompleted(removeReq)) {
        //Get request latest error and error map
        QOrganizerManager::Error latestError;
        QMap<int, QOrganizerManager::Error> errorMap;
        m_requestMgr->requestData(removeReq, &latestError, &errorMap);
        //Change the states of the the request to FinishedState and update error map
        QOrganizerManagerEngine::updateItemRemoveRequest(removeReq, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
        m_ics.emitSignals(m_engine);
        m_ics.clearRemovedItems();
        //Clear the request wait condition
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(removeReq);
        if (waitCondition)
            waitCondition->wakeAll();
        //Remove the request from request queue
        m_requestMgr->removeRequest(removeReq);
    }
}

void QOrganizerJsonDbRequestThread::handleCollectionSaveResponse(QOrganizerCollectionSaveRequest *collectionSaveReq, const QVariant &object, int index, QOrganizerManager::Error error)
{
    QOrganizerCollection collection = collectionSaveReq->collections().at(index);
    if (error == QOrganizerManager::NoError) {
        if (collection.id().isNull()) {
            //This is a new item
            QString jsonUuid = object.toMap().value(JsonDbString::kUuidStr).toString();
            if (!jsonUuid.isEmpty()) {
                QOrganizerJsonDbCollectionId* collectionId = new QOrganizerJsonDbCollectionId;
                collectionId->setCollectionId(jsonUuid);
                collection.setId(QOrganizerCollectionId(collectionId));
                m_ccs.insertAddedCollection (collection.id());
                //Update memory list
                m_collectionsIdList.append (collection.id());
            }
        } else {
            // this is modified item
            m_ccs.insertChangedCollection(collection.id());
            //Check if default collection is changed
            if (collection.id() == m_defaultCollection.id())
                m_defaultCollection = collection;
        }
    }
    m_requestMgr->updateRequestData(collectionSaveReq, error, index, QOrganizerItem(), collection);
    if (m_requestMgr->isRequestCompleted(collectionSaveReq)) {
        QOrganizerManager::Error latestError;
        QMap<int, QOrganizerManager::Error> errorMap;
        QList<QOrganizerCollection> collections;
        m_requestMgr->requestData(collectionSaveReq, &latestError, &errorMap, 0, &collections);
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionSaveReq);
        m_requestMgr->removeRequest(collectionSaveReq);
        QOrganizerManagerEngine::updateCollectionSaveRequest(collectionSaveReq, collections, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
        m_ccs.emitSignals(m_engine);
        m_ccs.clearAddedCollections();
        m_ccs.clearChangedCollections();
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QOrganizerJsonDbRequestThread::handleCollectionFetchResponse(QOrganizerCollectionFetchRequest *collectionFetchReq, const QVariant &object, QOrganizerManager::Error error)
{
    // object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of QVariantMaps
    // each representing one retrieved OrganizerCollection
    m_collectionsIdList.clear();
    QVariantList jsonDbObjectList;
    QList<QOrganizerCollection> collections;
    if (!object.isNull()) {
        jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();
    }

    QOrganizerCollection collection;
    for (int i = 0; i < jsonDbObjectList.size(); i++) {
        if (convertJsonDbObjectToCollection (jsonDbObjectList.at(i).toMap(), &collection)) {
            collections.append (collection);
            m_collectionsIdList.append (collection.id());
        }
    }
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionFetchReq);
    m_requestMgr->removeRequest(collectionFetchReq);
    QOrganizerManagerEngine::updateCollectionFetchRequest(collectionFetchReq, collections, error, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}


void QOrganizerJsonDbRequestThread::handleCollectionRemoveResponse(QOrganizerCollectionRemoveRequest* collectionRemoveReq, const QVariant &object, int index, QOrganizerManager::Error error)
{
    //Result map
    QVariant result = object;
    //Jsondb delete item count
    int deletedCount = result.toMap().value(JsonDbString::kCountStr).toInt();

    if (0 == index) {//Collection remove request response
        if (QOrganizerManager::NoError == error) {
            //Request collection remove count
            QList<QOrganizerCollectionId> removeList = collectionRemoveReq->collectionIds();
            int requestCount = removeList.size();
            //Check if something goes wrong even we did not get any error code
            if (deletedCount == requestCount) {
                //Everything goes fine and make the collection change set
                m_ccs.insertRemovedCollections (removeList);
            } else {
                //Should never happen because jsondb error list is empty except the default collection is in the list
                //Remove the default collection items from the list
                int defaultCollectionCount = removeList.removeAll(m_defaultCollection.id());
                //Check again if it is because of default collection issue
                if (deletedCount + defaultCollectionCount == requestCount) {
                    //Make the collection change set
                    m_ccs.insertRemovedCollections (removeList);
                } else {   //Something unkown is wrong!
                    QString warning = QOrganizerJsonDbStr::WarningCollectionRemove;
                    qWarning() << deletedCount << warning << requestCount;
                }
            }
            //Update memory list
            foreach (QOrganizerCollectionId id, removeList)
                m_collectionsIdList.removeAll (id);
        } else {//Error happened and not all collection are removed
            //Figure out the removed and non-removed collections: This could be implemented in a seperate function
            QList<QVariant> jsonErrorList = object.toMap().value (JsonDbString::kErrorStr).toList();

            //Create removed collection list
            QList <QOrganizerCollectionId> removedItemList;
            foreach (const QOrganizerCollectionId& id, collectionRemoveReq->collectionIds())
                removedItemList.append(id);

            //Error item index in request list
            int errorIndex = 0;

            //Check each error and update error map
            for (int i = 0; i < jsonErrorList.size(); i++) {
                QString uuid = jsonErrorList.at(i).toMap().value(JsonDbString::kUuidStr).toString();
                //Find out the item index, the search index should be started from the previous error item index
                for (int j = errorIndex; j < collectionRemoveReq->collectionIds().size(); j++) {
                    QString candidate_uuid = collectionRemoveReq->collectionIds().at(j).toString().remove (QOrganizerJsonDbStr::ManagerName);
                    if (uuid == candidate_uuid) {
                        //Found the error item in request list
                        errorIndex = j;
                        JsonDbError::ErrorCode errorCode = static_cast<JsonDbError::ErrorCode>(jsonErrorList.at(i).toMap().value(JsonDbString::kErrorStr).toInt());
                        error = convertJsondbErrorToOrganizerError (errorCode);
                        //Insert error to error list
                        m_requestMgr->updateRequestData (collectionRemoveReq, error, j);
                        removedItemList.removeAt(j);
                        break;
                    }
                }
            }
            //Remove the default collection items from the list
            QOrganizerCollection defaultCollection = m_engine->defaultCollection (&error);
            removedItemList.removeAll (defaultCollection.id());

            //Update the collection change set
            m_ccs.insertRemovedCollections(removedItemList);
            //Update memory list
            foreach (QOrganizerCollectionId id, removedItemList)
                m_collectionsIdList.removeAll (id);
        }
    } else if (1 == index) {//Collection items remove request response
        //Check if some items are delete from request
        if (deletedCount > 0) {
            QList<QVariant> jsoItemList = object.toMap().value (JsonDbString::kDataStr).toList();
            //Make the item change set
            for (int i = 0; i < jsoItemList.size(); i++) {
                //Get delete item id
                QString uuid = QOrganizerJsonDbStr::ManagerName
                    + jsoItemList.at(i).toMap().value (JsonDbString::kUuidStr).toString();
                QOrganizerItemId itemId = QOrganizerItemId::fromString (uuid);
                m_ics.insertRemovedItem (itemId);
            }
        }
    }

    if (m_requestMgr->isRequestCompleted (collectionRemoveReq)) {
        QMap<int, QOrganizerManager::Error> errorMap;
        m_requestMgr->requestData (collectionRemoveReq, &error, &errorMap);
        //Change the request status
        QOrganizerManagerEngine::updateCollectionRemoveRequest (collectionRemoveReq, error, errorMap, QOrganizerAbstractRequest::FinishedState);

        //Emit collection change signal if something is removed from jsondb
        m_ccs.emitSignals(m_engine);
        m_ccs.clearRemovedCollections();
        m_ics.emitSignals(m_engine);
        m_ics.clearRemovedItems();
        //Reset waitCondition
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionRemoveReq);
        if (waitCondition)
            waitCondition->wakeAll();
        //Remove the request
        m_requestMgr->removeRequest(collectionRemoveReq);
    }
}

QOrganizerManager::Error QOrganizerJsonDbRequestThread::handleErrorResponse(const QVariant &object, int errorCode)
{
    JsonDbError::ErrorCode jsonErrorCode;

    QVariant jsonErrorObject = object.toMap().value(JsonDbString::kErrorStr);
    if (jsonErrorObject.isNull() || jsonErrorObject.toList().isEmpty()) {
        jsonErrorCode = static_cast<JsonDbError::ErrorCode>(errorCode);
    } else {
        jsonErrorCode = static_cast<JsonDbError::ErrorCode>(jsonErrorObject.toList().at(0).toMap().value(JsonDbString::kCodeStr).toInt());
    }
    return convertJsondbErrorToOrganizerError (jsonErrorCode);

}

QOrganizerManager::Error QOrganizerJsonDbRequestThread::convertJsondbErrorToOrganizerError (JsonDbError::ErrorCode jsonErrorCode)
{
    QOrganizerManager::Error error;
    switch (jsonErrorCode) {
    case JsonDbError::NoError:
        error = QOrganizerManager::NoError;
        break;

    case JsonDbError::DatabaseError:
        error = QOrganizerManager::UnspecifiedError;
        break;

    case JsonDbError::QuotaExceeded:
        error = QOrganizerManager::OutOfMemoryError;
        break;

    case JsonDbError::OperationNotPermitted:
        error = QOrganizerManager::PermissionsError;
        break;

    case JsonDbError::InvalidRequest:
        // update request with invalid uuid
        error = QOrganizerManager::DoesNotExistError;
        break;

    case JsonDbError::MissingObject:
        // update request with invalid uuid
        error = QOrganizerManager::DoesNotExistError;
        break;

    default:
        error = QOrganizerManager::UnspecifiedError;
        break;
    }
    return error;
}

bool QOrganizerJsonDbRequestThread::convertJsonDbObjectToItem(const QVariantMap& object, QOrganizerItem* item) const
{
    QSet<QOrganizerRecurrenceRule> recurrenceRules;
    QSet<QOrganizerRecurrenceRule> exceptionRules;
    QSet<QDate> recurrenceDates;
    QSet<QDate> exceptionDates;

    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty()) {
        return false;
    }
    item->setId(QOrganizerItemId(new QOrganizerJsonDbItemId(jsonUuid)));
    item->setGuid(jsonUuid);

    QString jsonCollectionUuid = object.value(QOrganizerJsonDbStr::ItemCollectionId).toString();
    item->setCollectionId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonCollectionUuid)));

    if (object.contains(QOrganizerJsonDbStr::ItemRecurrenceRules)) {
        QVariantList recurrenceRulesList = object.value(QOrganizerJsonDbStr::ItemRecurrenceRules).toList();
        foreach (QVariant recurrenceRule, recurrenceRulesList) {
            QOrganizerRecurrenceRule rule;
            convertJsonDbObjectToRecurrenceRule(recurrenceRule.toMap(), &rule);
            recurrenceRules.insert(rule);
        }
    }

    if (object.contains(QOrganizerJsonDbStr::ItemExceptionRules)) {
        QVariantList exceptionRulesList = object.value(QOrganizerJsonDbStr::ItemExceptionRules).toList();
        foreach (QVariant exceptionRule, exceptionRulesList) {
            QOrganizerRecurrenceRule rule;
            convertJsonDbObjectToRecurrenceRule(exceptionRule.toMap(), &rule);
            exceptionRules.insert(rule);
        }
    }

    if (object.contains(QOrganizerJsonDbStr::ItemRecurrenceDates)) {
        QVariantList recurrenceDatesList = object.value(QOrganizerJsonDbStr::ItemRecurrenceDates).toList();
        foreach (QVariant recurrenceDate, recurrenceDatesList)
            recurrenceDates.insert(recurrenceDate.toDate());
    }

    if (object.contains(QOrganizerJsonDbStr::ItemExceptioneDates)) {
        QVariantList exceptionDatesList = object.value(QOrganizerJsonDbStr::ItemExceptioneDates).toList();
        foreach (QVariant exceptionDate, exceptionDatesList)
            exceptionDates.insert(exceptionDate.toDate());
    }

    QString displayLabel = object.value(QOrganizerJsonDbStr::ItemName).toString();
    if (!displayLabel.isEmpty())
        item->setDisplayLabel(displayLabel);

    QString description = object.value(QOrganizerJsonDbStr::ItemDescription).toString();
    if (!description.isEmpty())
        item->setDescription(description);

    QStringList comments(object.value(QOrganizerJsonDbStr::ItemComments).toStringList());
    if (!comments.isEmpty())
        item->setComments(comments);

    QVariantMap location = object.value(QOrganizerJsonDbStr::ItemLocation).toMap();
    if (!location.isEmpty()) {
        QOrganizerItemLocation locationDetail;

        QString label = location.value(QOrganizerJsonDbStr::ItemLocationLabel).toString();
        if (!label.isEmpty())
            locationDetail.setLabel(label);

        double latitude = location.value(QOrganizerJsonDbStr::ItemLocationLatitude).toDouble();
        locationDetail.setLatitude(latitude);

        double longitude = location.value(QOrganizerJsonDbStr::ItemLocationLongitude).toDouble();
        locationDetail.setLongitude(longitude);

        item->saveDetail(&locationDetail);
    }

    //Visual reminder
    QVariantMap jsondbVisualReminder = object.value(QOrganizerJsonDbStr::ItemVisualReminder).toMap();
    if (!jsondbVisualReminder.isEmpty()) {
        QOrganizerItemVisualReminder visualReminder;
        convertJsonDbObjectToItemReminderDetailCommon(jsondbVisualReminder, &visualReminder);
        QString message = jsondbVisualReminder.value(QOrganizerJsonDbStr::ItemVisualReminderMessage).toString();
        if (!message.isEmpty())
            visualReminder.setMessage(message);
        QString dataUrlStr = jsondbVisualReminder.value(QOrganizerJsonDbStr::ItemVisualReminderDataUrl).toString();
        if (!dataUrlStr.isEmpty())
            visualReminder.setDataUrl(QUrl(dataUrlStr));
        item->saveDetail(&visualReminder);
    }
    //Audible reminder
    QVariantMap jsondbAudibleReminder = object.value(QOrganizerJsonDbStr::ItemAudibleReminder).toMap();
    if (!jsondbAudibleReminder.isEmpty()) {
        QOrganizerItemAudibleReminder audibleReminder;
        convertJsonDbObjectToItemReminderDetailCommon(jsondbAudibleReminder, &audibleReminder);
        QString dataUrlStr = jsondbAudibleReminder.value(QOrganizerJsonDbStr::ItemAudibleReminderDataUrl).toString();
        if (!dataUrlStr.isEmpty()) {
            QUrl url(dataUrlStr);
            audibleReminder.setDataUrl(url);
        }
        item->saveDetail(&audibleReminder);
    }
    //Email reminder
    QVariantMap jsondbEmailReminder = object.value(QOrganizerJsonDbStr::ItemEmailReminder).toMap();
    if (!jsondbEmailReminder.isEmpty()) {
        QOrganizerItemEmailReminder emailReminder;
        convertJsonDbObjectToItemReminderDetailCommon(jsondbEmailReminder, &emailReminder);
        QString subject = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderSubject).toString();
        if (!subject.isEmpty())
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldSubject, subject);
        QString body = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderBody).toString();
        if (!body.isEmpty())
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldBody, body);
        if (jsondbEmailReminder.contains(QOrganizerJsonDbStr::ItemEmailReminderAttachments)) {
            QVariantList attachments = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderAttachments).toList();
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldAttachments, attachments);
        }
        if (jsondbEmailReminder.contains(QOrganizerJsonDbStr::ItemEmailReminderRecipients)) {
            QStringList recipients = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderRecipients).toStringList();
            emailReminder.setRecipients(recipients);
        }
        item->saveDetail(&emailReminder);
    }

    //Get "_type" value
    QString typeStr = object.value(JsonDbString::kTypeStr).toString();
    if (QOrganizerJsonDbStr::Event == typeStr) {
        QOrganizerEvent* event = static_cast<QOrganizerEvent*>(item);
        event->setType(QOrganizerItemType::TypeEvent);

        QDateTime startTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::EventStartDateTime).toString(), Qt::ISODate);
        if (startTime.isValid ())
            event->setStartDateTime(startTime);

        QDateTime endTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::EventEndDateTime).toString(), Qt::ISODate);
        if (endTime.isValid ())
            event->setEndDateTime(endTime);

        if (object.value(QOrganizerJsonDbStr::EventIsAllDay).toBool())
            event->setAllDay(true);

        if (object.contains(QOrganizerJsonDbStr::ItemPriority)) {
            QString priority = object.value(QOrganizerJsonDbStr::ItemPriority).toString();
            if (!priority.isEmpty())
                event->setPriority(convertJsonDbPriorityToEnum(priority));
        }

        if (!recurrenceRules.isEmpty())
            event->setRecurrenceRules(recurrenceRules);

        if (!exceptionRules.isEmpty())
            event->setExceptionRules(exceptionRules);

        if (!recurrenceDates.isEmpty())
            event->setRecurrenceDates(recurrenceDates);

        if (!exceptionDates.isEmpty())
            event->setExceptionDates(exceptionDates);

    } else if (QOrganizerJsonDbStr::Todo == typeStr) {
        QOrganizerTodo* todo = static_cast<QOrganizerTodo*>(item);
        todo->setType(QOrganizerItemType::TypeTodo);

        QDateTime startTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::TodoStartDateTime).toString(), Qt::ISODate);
        if (startTime.isValid ())
            todo->setStartDateTime(startTime);

        QDateTime dueTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::TodoDueDateTime).toString(), Qt::ISODate);
        if (dueTime.isValid ())
            todo->setDueDateTime(dueTime);

        if (object.value(QOrganizerJsonDbStr::EventIsAllDay).toBool())
            todo->setAllDay(object.value(QOrganizerJsonDbStr::TodoIsAllDay).toBool());

        if (object.contains(QOrganizerJsonDbStr::ItemPriority)) {
            QString priority = object.value(QOrganizerJsonDbStr::ItemPriority).toString();
            if (!priority.isEmpty())
                todo->setPriority(convertJsonDbPriorityToEnum(priority));
        }

        if (!recurrenceRules.isEmpty())
            todo->setRecurrenceRules(recurrenceRules);

        if (!exceptionRules.isEmpty())
            todo->setExceptionRules(exceptionRules);

        if (!recurrenceDates.isEmpty())
            todo->setRecurrenceDates(recurrenceDates);

        if (!exceptionDates.isEmpty())
            todo->setExceptionDates(exceptionDates);

    } else {
        //Item type is not supported
        return false;
    }

    return true;
}

bool QOrganizerJsonDbRequestThread::convertItemToJsonDbObject(const QOrganizerItem& item, QVariantMap* object) const
{
    QSet<QOrganizerRecurrenceRule> recurrenceRules;
    QSet<QOrganizerRecurrenceRule> exceptionRules;
    QSet<QDate> recurrenceDates;
    QSet<QDate> exceptionDates;

    if (item.type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(item);
        object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::Event);
        object->insert(QOrganizerJsonDbStr::EventStartDateTime, event.startDateTime().toString(Qt::ISODate));
        object->insert(QOrganizerJsonDbStr::EventEndDateTime, event.endDateTime().toString(Qt::ISODate));
        object->insert(QOrganizerJsonDbStr::EventIsAllDay, event.isAllDay());
        recurrenceRules = event.recurrenceRules();
        exceptionRules = event.exceptionRules();
        recurrenceDates = event.recurrenceDates();
        exceptionDates = event.exceptionDates();
    } else if (item.type() == QOrganizerItemType::TypeTodo) {
        QOrganizerTodo todo = static_cast<QOrganizerTodo>(item);
        object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::Todo);
        object->insert(QOrganizerJsonDbStr::TodoStartDateTime, todo.startDateTime().toString(Qt::ISODate));
        object->insert(QOrganizerJsonDbStr::TodoDueDateTime, todo.dueDateTime().toString(Qt::ISODate));
        object->insert(QOrganizerJsonDbStr::TodoIsAllDay, todo.isAllDay());
        recurrenceRules = todo.recurrenceRules();
        exceptionRules = todo.exceptionRules();
        recurrenceDates = todo.recurrenceDates();
        exceptionDates = todo.exceptionDates();

    } else {
        return false;
    }

    if (!item.id().isNull()) {
        QString jsonUuid = item.id().toString().remove (QOrganizerJsonDbStr::ManagerName);
        object->insert (JsonDbString::kUuidStr, jsonUuid);
    }
    object->insert (QOrganizerJsonDbStr::ItemName, item.displayLabel());
    object->insert (QOrganizerJsonDbStr::ItemDescription, item.description());
    object->insert (QOrganizerJsonDbStr::ItemCollectionId,
                    item.collectionId().toString().remove(QOrganizerJsonDbStr::ManagerName));
    object->insert(QOrganizerJsonDbStr::ItemComments, item.comments());

    QOrganizerItemLocation locationDetail = item.detail(QOrganizerItemLocation::DefinitionName);
    if (!locationDetail.isEmpty()) {
        QVariantMap locationObject;
        locationObject.insert(QOrganizerJsonDbStr::ItemLocationLabel, locationDetail.label());
        locationObject.insert(QOrganizerJsonDbStr::ItemLocationLatitude, locationDetail.latitude());
        locationObject.insert(QOrganizerJsonDbStr::ItemLocationLongitude, locationDetail.longitude());
        object->insert(QOrganizerJsonDbStr::ItemLocation, locationObject);
    }

    QOrganizerItemPriority priorityDetail = item.detail(QOrganizerItemPriority::DefinitionName);
    if (!priorityDetail.isEmpty()) {
        object->insert(QOrganizerJsonDbStr::ItemPriority, convertEnumToJsonDbPriority(priorityDetail.priority()));
    }
    //Audio,visual,email reminder details
    QOrganizerItemVisualReminder visualReminder = item.detail(QOrganizerItemVisualReminder::DefinitionName);
    if (!visualReminder.isEmpty()) {
        QVariantMap reminderObject;
        convertItemReminderDetailToJsonDbObject(visualReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::ItemVisualReminder, reminderObject);
    }
    QOrganizerItemAudibleReminder audibleReminder = item.detail(QOrganizerItemAudibleReminder::DefinitionName);
    if (!audibleReminder.isEmpty()) {
        QVariantMap reminderObject;
        convertItemReminderDetailToJsonDbObject(audibleReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::ItemAudibleReminder, reminderObject);
    }
    QOrganizerItemEmailReminder emailReminder = item.detail(QOrganizerItemEmailReminder::DefinitionName);
    if (!emailReminder.isEmpty()) {
        QVariantMap reminderObject;
        convertItemReminderDetailToJsonDbObject(emailReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::ItemEmailReminder, reminderObject);
    }
    if (!recurrenceRules.isEmpty()) {
        QVariantList recurrenceRulesList;
        foreach (QOrganizerRecurrenceRule recurrenceRule, recurrenceRules) {
            QVariantMap recurrenceRuleObject;
            convertRecurrenceRuleToJsonDbObject(recurrenceRule, &recurrenceRuleObject);
            recurrenceRulesList.append(recurrenceRuleObject);
        }
        object->insert(QOrganizerJsonDbStr::ItemRecurrenceRules, recurrenceRulesList);
    }

    if (!exceptionRules.isEmpty()) {
        QVariantList exceptionRulesList;
        foreach (QOrganizerRecurrenceRule exceptionRule, exceptionRules) {
            QVariantMap exceptionRuleObject;
            convertRecurrenceRuleToJsonDbObject(exceptionRule, &exceptionRuleObject);
            exceptionRulesList.append(exceptionRuleObject);
        }
        object->insert(QOrganizerJsonDbStr::ItemExceptionRules, exceptionRulesList);
    }

    if (!recurrenceDates.isEmpty()) {
        QVariantList recurrenceDatesList;
        foreach (QDate recurrenceDate, recurrenceDates) {
            recurrenceDatesList.append(recurrenceDate.toString(Qt::ISODate));
        }
        object->insert(QOrganizerJsonDbStr::ItemRecurrenceDates, recurrenceDatesList);
    }

    if (!exceptionDates.isEmpty()) {
        QVariantList exceptionDatesList;
        foreach (QDate exceptionDate, exceptionDates) {
            exceptionDatesList.append(exceptionDate.toString(Qt::ISODate));
        }
        object->insert(QOrganizerJsonDbStr::ItemExceptioneDates, exceptionDatesList);
    }
    return true;
}

bool QOrganizerJsonDbRequestThread::convertJsonDbObjectToCollection(const QVariantMap& object, QOrganizerCollection* collection)
{
    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty()) {
        return false;
    }

    collection->setId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonUuid)));

    QString name = object.value(QOrganizerJsonDbStr::CollectionName).toString();
    if (!name.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyName, name);

    QString description = object.value(QOrganizerJsonDbStr::CollectionDescription).toString();
    if (!description.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyDescription, description);

    QString color = object.value(QOrganizerJsonDbStr::CollectionColor).toString();
    if (!color.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyColor, color);

    QString image = object.value(QOrganizerJsonDbStr::CollectionImage).toString();
    if (!image.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyImage, image);

    bool isDefaultCollection = object.value(QOrganizerJsonDbStr::CollectionDefaultFlag).toBool();
    if (isDefaultCollection) {
        //Default collection create
        m_defaultCollection = *collection;
    }

    return true;
}

bool QOrganizerJsonDbRequestThread::convertCollectionToJsonDbObject(const QOrganizerCollection& collection, QVariantMap* object) const
{
    bool isDefaultCollection = false;
    if (!collection.id().isNull()) {
        QString jsonUuid = collection.id().toString();
        jsonUuid.remove (QOrganizerJsonDbStr::ManagerName);
        if (jsonUuid == defaultCollectionId ())
            isDefaultCollection = true;

        object->insert (JsonDbString::kUuidStr, jsonUuid);
    } else {
        if (collection.metaData (QOrganizerCollection::KeyName) == QOrganizerJsonDbStr::DefaultCollectionName
                && m_defaultCollection.id().isNull())
            //Now we create default collection
            isDefaultCollection = true;
    }

    object->insert (JsonDbString::kTypeStr, QOrganizerJsonDbStr::Collection);
    object->insert (QOrganizerJsonDbStr::CollectionDefaultFlag, isDefaultCollection);
    object->insert (QOrganizerJsonDbStr::CollectionName, collection.metaData (QOrganizerCollection::KeyName));
    object->insert (QOrganizerJsonDbStr::CollectionDescription, collection.metaData (QOrganizerCollection::KeyDescription));
    object->insert (QOrganizerJsonDbStr::CollectionColor, collection.metaData (QOrganizerCollection::KeyColor));
    object->insert (QOrganizerJsonDbStr::CollectionImage, collection.metaData (QOrganizerCollection::KeyImage));
    return true;
}

void QOrganizerJsonDbRequestThread::convertJsonDbObjectToRecurrenceRule(const QVariantMap& object, QOrganizerRecurrenceRule* rule) const
{
    QString frequency = object.value(QOrganizerJsonDbStr::RuleFrequency, QOrganizerJsonDbStr::FrequencyInvalid).toString();
    rule->setFrequency(convertJsonDbFrequencyToEnum(frequency));
    rule->setInterval(object.value(QOrganizerJsonDbStr::RuleInterval, 1).toInt());

    int limitCount = object.value(QOrganizerJsonDbStr::RuleLimitCount, -1).toInt();
    if (limitCount >= 0)
        rule->setLimit(limitCount);
    QDate limitDate = object.value(QOrganizerJsonDbStr::RuleLimitDate, QDate()).toDate();
    if (limitDate.isValid())
        rule->setLimit(limitDate);

    if (object.contains(QOrganizerJsonDbStr::RulePositions)) {
        QVariantList positionsList = object.value(QOrganizerJsonDbStr::RulePositions).toList();
        QSet<int> positions;
        foreach (QVariant position, positions)
            positions.insert(position.toInt());
        rule->setPositions(positions);
    }

    QString firstDayOfWeek = object.value(QOrganizerJsonDbStr::RuleFirstDayOfWeek, QOrganizerJsonDbStr::DayMonday).toString();
    rule->setFirstDayOfWeek(convertJsonDbDayOfWeekToEnum(firstDayOfWeek));

    if (object.contains(QOrganizerJsonDbStr::RuleDaysOfWeek)) {
        QVariantList daysOfWeekList = object.value(QOrganizerJsonDbStr::RuleDaysOfWeek).toList();
        QSet<Qt::DayOfWeek> daysOfWeek;
        foreach (QVariant day, daysOfWeekList)
            daysOfWeek.insert(convertJsonDbDayOfWeekToEnum(day.toString()));
        rule->setDaysOfWeek(daysOfWeek);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleDaysOfMonth)) {
        QVariantList daysOfMonthList = object.value(QOrganizerJsonDbStr::RuleDaysOfMonth).toList();
        QSet<int> daysOfMonth;
        foreach (QVariant day, daysOfMonthList)
            daysOfMonth.insert(day.toInt());
        rule->setDaysOfMonth(daysOfMonth);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleDaysOfYear)) {
        QVariantList daysOfYearList = object.value(QOrganizerJsonDbStr::RuleDaysOfYear).toList();
        QSet<int> daysOfYear;
        foreach (QVariant day, daysOfYearList)
            daysOfYear.insert(day.toInt());
        rule->setDaysOfYear(daysOfYear);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleWeeksOfYear)) {
        QVariantList weeksOfYearList = object.value(QOrganizerJsonDbStr::RuleWeeksOfYear).toList();
        QSet<int> weeksOfYear;
        foreach (QVariant week, weeksOfYearList)
            weeksOfYear.insert(week.toInt());
        rule->setWeeksOfYear(weeksOfYear);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleMonthsOfYear)) {
        QVariantList monthsOfYearList = object.value(QOrganizerJsonDbStr::RuleMonthsOfYear).toList();
        QSet<QOrganizerRecurrenceRule::Month> monthsOfYear;
        foreach (QVariant month, monthsOfYearList)
            monthsOfYear.insert(convertJsonDbMonthToEnum(month.toString()));
        rule->setMonthsOfYear(monthsOfYear);
    }
}

void QOrganizerJsonDbRequestThread::convertRecurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule& rule, QVariantMap* object) const
{
    object->insert(QOrganizerJsonDbStr::RuleFrequency, convertEnumToJsonDbFrequency(rule.frequency()));
    object->insert(QOrganizerJsonDbStr::RuleInterval, rule.interval());
    object->insert(QOrganizerJsonDbStr::RuleLimitCount, rule.limitCount());
    object->insert(QOrganizerJsonDbStr::RuleLimitDate, rule.limitDate().toString(Qt::ISODate));
    QSet<int> positions = rule.positions();
    if (!positions.isEmpty()) {
        QVariantList positionsList;
        foreach (int position, positions) {
            positionsList.append(position);
        }
        object->insert(QOrganizerJsonDbStr::RulePositions, positionsList);
    }

    object->insert(QOrganizerJsonDbStr::RuleFirstDayOfWeek, convertEnumToJsonDbDayOfWeek(rule.firstDayOfWeek()));

    QSet<Qt::DayOfWeek> daysOfWeek = rule.daysOfWeek();
    if (!daysOfWeek.isEmpty()) {
        QVariantList daysOfWeekList;
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            daysOfWeekList.append(convertEnumToJsonDbDayOfWeek(day));
        }
        object->insert(QOrganizerJsonDbStr::RuleDaysOfWeek, daysOfWeekList);
    }

    QSet<int> daysOfMonth = rule.daysOfMonth();
    if (!daysOfMonth.isEmpty()) {
        QVariantList daysOfMonthList;
        foreach (int day, daysOfMonth) {
            daysOfMonthList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::RuleDaysOfMonth, daysOfMonthList);
    }

    QSet<int> daysOfYear = rule.daysOfYear();
    if (!daysOfYear.isEmpty()) {
        QVariantList daysOfYearList;
        foreach (int day, daysOfYear) {
            daysOfYearList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::RuleDaysOfYear, daysOfYearList);
    }

    QSet<int> weeksOfYear = rule.weeksOfYear();
    if (!weeksOfYear.isEmpty()) {
        QVariantList weeksOfYearList;
        foreach (int week, weeksOfYear) {
            weeksOfYearList.append(week);
        }
        object->insert(QOrganizerJsonDbStr::RuleWeeksOfYear, weeksOfYearList);
    }

    QSet<QOrganizerRecurrenceRule::Month> monthsOfYear = rule.monthsOfYear();
    if (!monthsOfYear.isEmpty()) {
        QVariantList monthsOfYearList;
        foreach (QOrganizerRecurrenceRule::Month month, monthsOfYear) {
            monthsOfYearList.append(convertEnumToJsonDbMonth(month));
        }
        object->insert(QOrganizerJsonDbStr::RuleMonthsOfYear, monthsOfYearList);
    }
}

QString QOrganizerJsonDbRequestThread::filterToJsondbQuery (const QOrganizerItemFilter filter)
{
    QString jsonDbQueryStr;
    switch (filter.type()) {
    case QOrganizerItemFilter::CollectionFilter: {
        const QOrganizerItemCollectionFilter cf(filter);
        const QSet<QOrganizerCollectionId>& ids = cf.collectionIds();
        //query [?collectionId in ["collection1_uuid1", "collection1_uuid2", ...] ]
        jsonDbQueryStr = ITEM_COLLECTION_ID_QUERY_STRING;
        foreach (const QOrganizerCollectionId id, ids) {
            jsonDbQueryStr += id.toString().remove (QOrganizerJsonDbStr::ManagerName);
            jsonDbQueryStr += "\",\""; // ","
        }
        jsonDbQueryStr += "]]";
        //change last "collection_uuid","]] to "collection_uuid"]]
        jsonDbQueryStr.replace(",\"]]", "]]");
        break;
    }
    default:
        break;
    }

    return jsonDbQueryStr;
}

void QOrganizerJsonDbRequestThread::convertItemReminderDetailToJsonDbObject(const QOrganizerItemReminder& itemReminder, QVariantMap& reminderObject) const
{
    //Item reminder common part
    if (itemReminder.hasValue(itemReminder.FieldSecondsBeforeStart))
        reminderObject.insert(QOrganizerJsonDbStr::ItemReminderSecBeforeStart, itemReminder.secondsBeforeStart());
    if (itemReminder.hasValue(itemReminder.FieldRepetitionCount))
        reminderObject.insert(QOrganizerJsonDbStr::ItemReminderRepCount, itemReminder.repetitionCount());
    if (itemReminder.hasValue(itemReminder.FieldRepetitionDelay))
        reminderObject.insert(QOrganizerJsonDbStr::ItemReminderRepDelay, itemReminder.repetitionDelay());

    //Audio, visual, Email sepcified properties
    switch (itemReminder.reminderType()) {
    case QOrganizerItemReminder::NoReminder :
        break;
    case QOrganizerItemReminder::VisualReminder : {
        if (itemReminder.hasValue(QOrganizerItemVisualReminder::FieldMessage))
            reminderObject.insert(QOrganizerJsonDbStr::ItemVisualReminderMessage, itemReminder.value(QOrganizerItemVisualReminder::FieldMessage));
        if (itemReminder.hasValue(QOrganizerItemVisualReminder::FieldDataUrl))
            reminderObject.insert(QOrganizerJsonDbStr::ItemVisualReminderDataUrl, itemReminder.value(QOrganizerItemVisualReminder::FieldDataUrl));
        break;
    }
    case QOrganizerItemReminder::AudibleReminder : {
        if (itemReminder.hasValue(QOrganizerItemAudibleReminder::FieldDataUrl))
            reminderObject.insert(QOrganizerJsonDbStr::ItemAudibleReminderDataUrl, itemReminder.value(QOrganizerItemAudibleReminder::FieldDataUrl));
        break;
    }
    case QOrganizerItemReminder::EmailReminder : {
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldSubject))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderSubject, itemReminder.value(QOrganizerItemEmailReminder::FieldSubject));
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldBody))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderBody, itemReminder.value(QOrganizerItemEmailReminder::FieldBody));
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldAttachments))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderAttachments, itemReminder.variantValue(QOrganizerItemEmailReminder::FieldAttachments));//QVariantList
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldRecipients))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderRecipients, itemReminder.variantValue(QOrganizerItemEmailReminder::FieldRecipients));//QStringList
        break;
    }
    }
}

void QOrganizerJsonDbRequestThread::convertJsonDbObjectToItemReminderDetailCommon(const QVariantMap& object, QOrganizerItemReminder* itemReminder) const
{
    //Common details exist in all reminder types
    if (object.contains(QOrganizerJsonDbStr::ItemReminderSecBeforeStart)) {
        int secBeforeStart = object.value(QOrganizerJsonDbStr::ItemReminderSecBeforeStart).toInt();
        itemReminder->setSecondsBeforeStart(secBeforeStart);
    }

    if (object.contains(QOrganizerJsonDbStr::ItemReminderRepCount)) {
        int repetitionCount = object.value(QOrganizerJsonDbStr::ItemReminderRepCount).toInt();
        itemReminder->setValue(QOrganizerItemReminder::FieldRepetitionCount, repetitionCount);
    }
    if (object.contains(QOrganizerJsonDbStr::ItemReminderRepDelay)) {
        int repetitionDelay = object.value(QOrganizerJsonDbStr::ItemReminderRepDelay).toInt();
        itemReminder->setValue(QOrganizerItemReminder::FieldRepetitionDelay, repetitionDelay);
    }
}

QOrganizerItemPriority::Priority QOrganizerJsonDbRequestThread::convertJsonDbPriorityToEnum(const QString& jsonPriority) const
{
    if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityUnkownPriority)
        return QOrganizerItemPriority::UnknownPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityHighestPriority)
        return QOrganizerItemPriority::HighestPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityExtremelyhighPriority)
        return QOrganizerItemPriority::ExtremelyHighPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityVeryhighPriority)
        return QOrganizerItemPriority::VeryHighPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityHighPriority)
        return QOrganizerItemPriority::HighPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityMediumPriority)
        return QOrganizerItemPriority::MediumPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityLowPriority)
        return QOrganizerItemPriority::LowPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityVerylowPriority)
        return QOrganizerItemPriority::VeryLowPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityExtremelylowPriority)
        return QOrganizerItemPriority::ExtremelyLowPriority;
    else if (jsonPriority == QOrganizerJsonDbStr::ItemPriorityLowestPriority)
        return QOrganizerItemPriority::LowestPriority;
    else
        return QOrganizerItemPriority::UnknownPriority;
}

QString QOrganizerJsonDbRequestThread::convertEnumToJsonDbPriority(const QOrganizerItemPriority::Priority& priority) const
{
    switch (priority) {
    case QOrganizerItemPriority::UnknownPriority:
        return QOrganizerJsonDbStr::ItemPriorityUnkownPriority;
    case QOrganizerItemPriority::HighestPriority:
        return QOrganizerJsonDbStr::ItemPriorityHighestPriority;
    case QOrganizerItemPriority::ExtremelyHighPriority:
        return QOrganizerJsonDbStr::ItemPriorityExtremelyhighPriority;
    case QOrganizerItemPriority::VeryHighPriority:
        return QOrganizerJsonDbStr::ItemPriorityVeryhighPriority;
    case QOrganizerItemPriority::HighPriority:
        return QOrganizerJsonDbStr::ItemPriorityHighPriority;
    case QOrganizerItemPriority::MediumPriority:
        return QOrganizerJsonDbStr::ItemPriorityMediumPriority;
    case QOrganizerItemPriority::LowPriority:
        return QOrganizerJsonDbStr::ItemPriorityLowPriority;
    case QOrganizerItemPriority::VeryLowPriority:
        return QOrganizerJsonDbStr::ItemPriorityVerylowPriority;
    case QOrganizerItemPriority::ExtremelyLowPriority:
        return QOrganizerJsonDbStr::ItemPriorityExtremelylowPriority;
    case QOrganizerItemPriority::LowestPriority:
        return QOrganizerJsonDbStr::ItemPriorityLowestPriority;
    default:
        return QString ();
    }
}

QOrganizerRecurrenceRule::Frequency QOrganizerJsonDbRequestThread::convertJsonDbFrequencyToEnum(const QString& jsonFrequency) const
{
    if (jsonFrequency == QOrganizerJsonDbStr::FrequencyInvalid)
        return QOrganizerRecurrenceRule::Invalid;
    else if (jsonFrequency == QOrganizerJsonDbStr::FrequencyDaily)
        return QOrganizerRecurrenceRule::Daily;
    else if (jsonFrequency == QOrganizerJsonDbStr::FrequencyWeekly)
        return QOrganizerRecurrenceRule::Weekly;
    else if (jsonFrequency == QOrganizerJsonDbStr::FrequencyMonthly)
        return QOrganizerRecurrenceRule::Monthly;
    else if (jsonFrequency == QOrganizerJsonDbStr::FrequencyYearly)
        return QOrganizerRecurrenceRule::Yearly;
    else
        return QOrganizerRecurrenceRule::Invalid;
}

QString QOrganizerJsonDbRequestThread::convertEnumToJsonDbFrequency(const QOrganizerRecurrenceRule::Frequency& frequency) const
{
    switch (frequency) {
    case QOrganizerRecurrenceRule::Invalid:
        return QOrganizerJsonDbStr::FrequencyInvalid;
    case QOrganizerRecurrenceRule::Daily:
        return QOrganizerJsonDbStr::FrequencyDaily;
    case QOrganizerRecurrenceRule::Weekly:
        return QOrganizerJsonDbStr::FrequencyWeekly;
    case QOrganizerRecurrenceRule::Monthly:
        return QOrganizerJsonDbStr::FrequencyMonthly;
    case QOrganizerRecurrenceRule::Yearly:
        return QOrganizerJsonDbStr::FrequencyYearly;
    default:
        return QOrganizerJsonDbStr::FrequencyInvalid;
    }
}

QOrganizerRecurrenceRule::LimitType QOrganizerJsonDbRequestThread::convertJsonDbLimitTypeToEnum(const QString& jsonLimitType) const
{
    if (jsonLimitType == QOrganizerJsonDbStr::LimitNoLimit)
        return QOrganizerRecurrenceRule::NoLimit;
    else if (jsonLimitType == QOrganizerJsonDbStr::LimitCount)
        return QOrganizerRecurrenceRule::CountLimit;
    else if (jsonLimitType == QOrganizerJsonDbStr::LimitDate)
        return QOrganizerRecurrenceRule::DateLimit;
    else
        return QOrganizerRecurrenceRule::NoLimit;
}

QString QOrganizerJsonDbRequestThread::convertEnumToJsonDbLimitType(const QOrganizerRecurrenceRule::LimitType& limitType) const
{
    switch (limitType) {
    case QOrganizerRecurrenceRule::NoLimit:
        return QOrganizerJsonDbStr::LimitNoLimit;
    case QOrganizerRecurrenceRule::CountLimit:
        return QOrganizerJsonDbStr::LimitCount;
    case QOrganizerRecurrenceRule::DateLimit:
        return QOrganizerJsonDbStr::LimitDate;
    default:
        return QOrganizerJsonDbStr::LimitNoLimit;
    }
}

Qt::DayOfWeek QOrganizerJsonDbRequestThread::convertJsonDbDayOfWeekToEnum(const QString& jsonDayOfWeek) const
{
    if (jsonDayOfWeek == QOrganizerJsonDbStr::DayMonday)
        return Qt::Monday;
    else if (jsonDayOfWeek == QOrganizerJsonDbStr::DayTuesday)
        return Qt::Tuesday;
    else if (jsonDayOfWeek == QOrganizerJsonDbStr::DayWednesday)
        return Qt::Wednesday;
    else if (jsonDayOfWeek == QOrganizerJsonDbStr::DayThursday)
        return Qt::Thursday;
    else if (jsonDayOfWeek == QOrganizerJsonDbStr::DayFriday)
        return Qt::Friday;
    else if (jsonDayOfWeek == QOrganizerJsonDbStr::DaySaturday)
        return Qt::Saturday;
    else if (jsonDayOfWeek == QOrganizerJsonDbStr::DaySunday)
        return Qt::Sunday;
    else
        return Qt::Monday;
}

QString QOrganizerJsonDbRequestThread::convertEnumToJsonDbDayOfWeek(const Qt::DayOfWeek& dayOfWeek) const
{
    switch (dayOfWeek) {
    case Qt::Monday:
        return QOrganizerJsonDbStr::DayMonday;
    case Qt::Tuesday:
        return QOrganizerJsonDbStr::DayTuesday;
    case Qt::Wednesday:
        return QOrganizerJsonDbStr::DayWednesday;
    case Qt::Thursday:
        return QOrganizerJsonDbStr::DayThursday;
    case Qt::Friday:
        return QOrganizerJsonDbStr::DayFriday;
    case Qt::Saturday:
        return QOrganizerJsonDbStr::DaySaturday;
    case Qt::Sunday:
        return QOrganizerJsonDbStr::DaySunday;
    default:
        return QOrganizerJsonDbStr::DayMonday;
    }
}

QOrganizerRecurrenceRule::Month QOrganizerJsonDbRequestThread::convertJsonDbMonthToEnum(const QString& jsonMonth) const
{
    if (jsonMonth == QOrganizerJsonDbStr::MonthJanuary)
        return QOrganizerRecurrenceRule::January;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthFebruary)
        return QOrganizerRecurrenceRule::February;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthMarch)
        return QOrganizerRecurrenceRule::March;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthApril)
        return QOrganizerRecurrenceRule::April;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthMay)
        return QOrganizerRecurrenceRule::May;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthJune)
        return QOrganizerRecurrenceRule::June;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthJuly)
        return QOrganizerRecurrenceRule::July;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthAugust)
        return QOrganizerRecurrenceRule::August;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthSeptember)
        return QOrganizerRecurrenceRule::September;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthOctober)
        return QOrganizerRecurrenceRule::October;
    else if (jsonMonth == QOrganizerJsonDbStr::MonthNovember)
        return QOrganizerRecurrenceRule::November;
    else // (jsonMonth == QOrganizerJsonDbStr::MonthDecember)
        return QOrganizerRecurrenceRule::December;
}

QString QOrganizerJsonDbRequestThread::convertEnumToJsonDbMonth(const QOrganizerRecurrenceRule::Month& month) const
{
    switch (month) {
    case QOrganizerRecurrenceRule::January:
        return QOrganizerJsonDbStr::MonthJanuary;
    case QOrganizerRecurrenceRule::February:
        return QOrganizerJsonDbStr::MonthFebruary;
    case QOrganizerRecurrenceRule::March:
        return QOrganizerJsonDbStr::MonthMarch;
    case QOrganizerRecurrenceRule::April:
        return QOrganizerJsonDbStr::MonthApril;
    case QOrganizerRecurrenceRule::May:
        return QOrganizerJsonDbStr::MonthMay;
    case QOrganizerRecurrenceRule::June:
        return QOrganizerJsonDbStr::MonthJune;
    case QOrganizerRecurrenceRule::July:
        return QOrganizerJsonDbStr::MonthJuly;
    case QOrganizerRecurrenceRule::August:
        return QOrganizerJsonDbStr::MonthAugust;
    case QOrganizerRecurrenceRule::September:
        return QOrganizerJsonDbStr::MonthSeptember;
    case QOrganizerRecurrenceRule::October:
        return QOrganizerJsonDbStr::MonthOctober;
    case QOrganizerRecurrenceRule::November:
        return QOrganizerJsonDbStr::MonthNovember;
    default: // QOrganizerRecurrenceRule::December:
        return QOrganizerJsonDbStr::MonthDecember;
    }
}
