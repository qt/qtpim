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

QTORGANIZER_BEGIN_NAMESPACE

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
    QList<QOrganizerItem> items = saveReq->items();
    m_requestMgr->setActive(saveReq);

    for (int i = 0; i < items.size(); i++) {
        QOrganizerItem item = items.at(i);
        bool itemIsNew = item.id().isNull();
        bool errorFound = false;

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!itemIsNew && (managerUri != item.id().managerUri())) {
            error = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }

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
            if (m_converter.itemToJsonDbObject(item, &newJsonDbItem)) {
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
            m_requestMgr->updateRequestData(saveReq, error, i, item);
            // Is this the last item in the list and have other items already been handled
            if (i == items.size() - 1 && m_requestMgr->isRequestCompleted(saveReq)) {
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
    }
}

void QOrganizerJsonDbRequestThread::handleItemFetchRequest(QOrganizerItemFetchRequest *fetchReq)
{
    m_requestMgr->setActive(fetchReq);

    //Jsondb query [?type="com.nokia.mp.organizer.Item"]
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;//"=\"OrganizerItem\"]";
    //Apply Filter and get jsondb query expression
    QString filterString;
    if (compoundFilterToJsondbQuery(fetchReq->filter(), filterString)) {
        newJsonDbQuery += filterString;
        int trId = m_jsonDb->query(newJsonDbQuery);
        m_requestMgr->addTransaction(fetchReq, trId);
    } else {
        //Invalid filter means empty items list could get from Jsondb
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchReq);
        QOrganizerManager::Error error = QOrganizerManager::BadArgumentError;
        QList<QOrganizerItem> items;
        m_requestMgr->removeRequest(fetchReq);
        QOrganizerManagerEngine::updateItemFetchRequest(fetchReq, items, error, QOrganizerAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }
}

void QOrganizerJsonDbRequestThread::handleItemIdFetchRequest(QOrganizerItemIdFetchRequest *idFetchReq)
{
    m_requestMgr->setActive(idFetchReq);

    //jsondb query [?type="com.nokia.mp.organizer.Item"][=_uuid]
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;
    newJsonDbQuery += ITEM_ID_RESULT_STRING;

    //Apply Filter and get jsondb query expression
    QString filterString;
    if (compoundFilterToJsondbQuery(idFetchReq->filter(), filterString)) {
        newJsonDbQuery += filterString;
        int trId = m_jsonDb->query(newJsonDbQuery);
        m_requestMgr->addTransaction(idFetchReq, trId);
    } else {
        //Invalid filter means empty items list
        QWaitCondition* waitCondition = m_requestMgr->waitCondition(idFetchReq);
        QOrganizerManager::Error error = QOrganizerManager::BadArgumentError;
        QList<QOrganizerItemId> itemIds;
        m_requestMgr->removeRequest(idFetchReq);
        QOrganizerManagerEngine::updateItemIdFetchRequest(idFetchReq, itemIds, error, QOrganizerAbstractRequest::FinishedState);
        if (waitCondition)
            waitCondition->wakeAll();
    }

}

void QOrganizerJsonDbRequestThread::handleItemFetchByIdRequest(QOrganizerItemFetchByIdRequest *fetchByIdReq)
{
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
    QList<QOrganizerCollection> collections = collectionSaveReq->collections();
    m_requestMgr->setActive(collectionSaveReq);

    for (int i = 0; i < collections.size(); i++) {
        QOrganizerCollection collection = collections.at(i);
        bool errorFound = false;
        bool collectionIsNew = collection.id().isNull();

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!collectionIsNew && (managerUri != collection.id().managerUri())) {
            error = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }

        bool isDefaultCollection = false;
        if (collectionIsNew) {
            if (collection.metaData(QOrganizerCollection::KeyName) == QOrganizerJsonDbStr::DefaultCollectionName
                    && m_defaultCollection.id().isNull())
                //Now we create default collection
                isDefaultCollection = true;
        } else {
            QString jsonUuid = collection.id().toString();
            jsonUuid.remove (QOrganizerJsonDbStr::ManagerName);
            if (jsonUuid == defaultCollectionId())
                isDefaultCollection = true;
        }

        QVariantMap newJsonDbCollection;
        if (!errorFound) {
            if (m_converter.collectionToJsonDbObject(collection, isDefaultCollection, &newJsonDbCollection)) {
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
            m_requestMgr->updateRequestData(collectionSaveReq, error, i, QOrganizerItem(), collection);
            // Is this the last collection in the list and have other collections already been handled
            if (i == collections.size() - 1 && m_requestMgr->isRequestCompleted(collectionSaveReq)) {
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
        QOrganizerItemCollectionFilter collectonFilter;
        collectonFilter.setCollectionIds(removeCollectionIdValidSet);
        QString filterString;
        singleFilterToJsondbQuery(collectonFilter, filterString);
        jsondbQuery += filterString;

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
        if (m_converter.jsonDbObjectToItem(jsonDbObjectList.at(i).toMap(), &item)) {
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
        if (m_converter.jsonDbObjectToItem(jsonDbObjectList.at(i).toMap(), &item))
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

    for (int i = 0; i < jsonDbObjectList.size(); i++) {
        QOrganizerCollection collection;
        bool isDefaultCollection;
        if (m_converter.jsonDbObjectToCollection(jsonDbObjectList.at(i).toMap(), &collection, isDefaultCollection)) {
            if (isDefaultCollection) {
                //Default collection create
                m_defaultCollection = collection;
            }
            collections.append(collection);
            m_collectionsIdList.append(collection.id());
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
                    QString candidate_uuid = collectionRemoveReq->collectionIds().at(j).toString().remove(QOrganizerJsonDbStr::ManagerName);
                    if (uuid == candidate_uuid) {
                        //Found the error item in request list
                        errorIndex = j;
                        JsonDbError::ErrorCode errorCode = static_cast<JsonDbError::ErrorCode>(jsonErrorList.at(i).toMap().value(JsonDbString::kErrorStr).toInt());
                        error = m_converter.jsondbErrorToOrganizerError(errorCode);
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
    return m_converter.jsondbErrorToOrganizerError (jsonErrorCode);

}

bool QOrganizerJsonDbRequestThread::compoundFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr)
{
    bool isValidFilter = true;
    switch (filter.type()) {
    case QOrganizerItemFilter::IntersectionFilter: {
        const QOrganizerItemIntersectionFilter isf(filter);
        QList<QOrganizerItemFilter> filterList = isf.filters();
        foreach (QOrganizerItemFilter filter, filterList){
            //query filter1 filter2 filter3 ...
            //query [?definition="value"][?definition="value"][?definition="value"]
            QString filterStr;
            if (singleFilterToJsondbQuery(filter, filterStr))
                jsonDbQueryStr += filterStr;
            else //For intersection filter, single filter invalid means empty result from jsondb query
                isValidFilter = false;
        }
        break;
    }
    case QOrganizerItemFilter::UnionFilter: {
        const QOrganizerItemUnionFilter uf(filter);
        QList<QOrganizerItemFilter> filterList = uf.filters();
        int validFilterCount = 0;
        foreach (QOrganizerItemFilter filter, filterList){
            //query filter1 filter2 filter3 ...
            //query [?definition="value" | definition="value" | definition="value"]
            QString filterStr;
            if (singleFilterToJsondbQuery(filter, filterStr)) {
                jsonDbQueryStr += filterStr;
                validFilterCount ++;
            } else {//For union filter, singel filter invalid means we could skip this filter
                continue;
            }
        }
        if (validFilterCount > 0)
            jsonDbQueryStr.replace("][?", " | "); //replace the "][?" to " | "
        else //no valid filter means empty item list from jsondb
            isValidFilter = false;
        break;
    }
    default:
        isValidFilter = singleFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    }
    if (!isValidFilter)
        jsonDbQueryStr.clear();

    return isValidFilter;
}

bool QOrganizerJsonDbRequestThread::singleFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr)
{
    bool isValidFilter = true;
    switch (filter.type()) {
    case QOrganizerItemFilter::CollectionFilter: {
        const QOrganizerItemCollectionFilter cf(filter);
        const QSet<QOrganizerCollectionId>& ids = cf.collectionIds();
        if (!ids.empty()) {
            //query [?collectionId in ["collection1_uuid1", "collection1_uuid2", ...] ]
            jsonDbQueryStr = ITEM_COLLECTION_ID_QUERY_STRING;
            int validIdCount = 0;
            foreach (const QOrganizerCollectionId id, ids) {
                if (m_collectionsIdList.contains(id)) {
                    jsonDbQueryStr += id.toString().remove(QOrganizerJsonDbStr::ManagerName);
                    jsonDbQueryStr += "\",\""; // ","
                    validIdCount ++;
                }
            }
            if (validIdCount > 0) {
                jsonDbQueryStr += "]]";
                //change last "collection_uuid","]] to "collection_uuid"]]
                jsonDbQueryStr.replace(",\"]]", "]]");
            } else {
                isValidFilter = false;
            }
        } else {
            isValidFilter = false;
        }
        break;
    }
    case QOrganizerItemFilter::IdFilter: {
        const QOrganizerItemIdFilter idf(filter);
        const QList<QOrganizerItemId>& ids = idf.ids();
        if (!ids.empty()) {
            //query [?_uuid in ["uuid1", "uuid2", ...]]
            jsonDbQueryStr = ITEM_IDS_LIST_QUERY_STRING;
            int validIdCount = 0;
            foreach (const QOrganizerItemId id, ids) {
                if (!id.isNull()) {
                    jsonDbQueryStr += id.toString().remove(QOrganizerJsonDbStr::ManagerName);
                    jsonDbQueryStr += "\",\""; // add "," between item ids
                    validIdCount ++;
                }
            }
            if (validIdCount > 0) {
                jsonDbQueryStr += "]]";
                jsonDbQueryStr.replace(",\"]]", "]]"); //change last "uuid","]] to "uuid"]]
            } else {
                isValidFilter = false;
            }
        } else {
            isValidFilter = false;
        }
        break;
    }
    default:
        break;
    }

    return isValidFilter;
}

#include "moc_qorganizerjsondbrequestthread.cpp"

QTORGANIZER_END_NAMESPACE
