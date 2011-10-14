/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "qorganizerjsondbdatastorage.h"

#include <QMutexLocker>

#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring_p.h"

#include <jsondb-client.h>
#include <private/jsondb-connection_p.h>
#include <private/jsondb-strings_p.h>
#include <jsondb-error.h>
Q_USE_JSONDB_NAMESPACE
QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbDataStorage::QOrganizerJsonDbDataStorage()
    : m_waitMutex(0)
    , m_jsonDb(0)
    , m_jsonConnection(0)
{
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::saveItems(QMap<int, QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    initRequestData(SaveItems, errorMap, error);
    m_resultItems = items;
    processRequest();
    clearRequestData();
}

QList<QOrganizerItem> QOrganizerJsonDbDataStorage::items(const QDateTime &startDate, const QDateTime &endDate, const QOrganizerItemFilter &filter,
                                                         const QList<QOrganizerItemSortOrder> &sortOrders, const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error, FetchType type)
{
    Q_UNUSED(sortOrders);
    Q_UNUSED(fetchHint);
    initRequestData(Items, 0, error);
    m_start = startDate;
    m_end = endDate;
    m_filter = filter;
    m_fetchType = type;
    processRequest();
    QList<QOrganizerItem> fetchedItems = m_items;
    clearRequestData();
    return fetchedItems;
}

QList<QOrganizerItem> QOrganizerJsonDbDataStorage::itemsById(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error *error)
{
    initRequestData(ItemsById, errorMap, error);
    m_itemIds = itemIds;
    processRequest();
    QList<QOrganizerItem> fetchedItems = m_items;
    clearRequestData();
    return fetchedItems;
}

void QOrganizerJsonDbDataStorage::removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    initRequestData(RemoveItems, errorMap, error);
    m_itemIds = itemIds;
    processRequest();
    clearRequestData();
}

QList<QOrganizerItemId> QOrganizerJsonDbDataStorage::removeItemsByCollectionId(const QList<QOrganizerCollectionId>& collectionIds)
{
    initRequestData(RemoveItemsByCollectionId, 0, 0);
    m_removeItemCollectionIds = collectionIds;
    processRequest();
    QList<QOrganizerItemId> deletedItemIds = m_itemIds;
    clearRequestData();
    return deletedItemIds;
}

void QOrganizerJsonDbDataStorage::saveCollections(QMap<int, QOrganizerCollection>* collections, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    initRequestData(SaveCollections, errorMap, error);
    m_resultCollections = collections;
    m_isDefaultCollection = false;
    processRequest();
    clearRequestData();
}

QList<QOrganizerCollection> QOrganizerJsonDbDataStorage::collections(QOrganizerManager::Error *error)
{
    initRequestData(Collections, 0, error);
    processRequest();
    QList<QOrganizerCollection> fetchedCollections = m_collections;
    clearRequestData();
    return fetchedCollections;
}

void QOrganizerJsonDbDataStorage::removeCollections(const QMap<int, QOrganizerCollectionId>& collectionIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    initRequestData(RemoveCollections, errorMap, error);
    m_removeCollectionIds = collectionIds;
    processRequest();
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::createDefaultCollection(QOrganizerCollection* defaultCollection, QOrganizerManager::Error* error)
{
    QMap<int, QOrganizerManager::Error> errorMap;
    initRequestData(SaveCollections, &errorMap, error);
    QMap<int, QOrganizerCollection> collections;
    collections.insert(0, *defaultCollection);
    m_resultCollections = &collections;
    m_isDefaultCollection = true;
    processRequest();
    *defaultCollection = m_resultCollections->value(0);
    m_defaultCollection = *defaultCollection;
    clearRequestData();
}

QOrganizerCollection QOrganizerJsonDbDataStorage::defaultCollection()
{
    if (m_defaultCollection.id().isNull()) {
        // m_defaultCollection is updated every time collections are fetched
        QOrganizerManager::Error error;
        collections(&error);
    }
    return m_defaultCollection;
}

QSet<QOrganizerCollectionId> QOrganizerJsonDbDataStorage::collectionIds()
{
    if (m_collectionIds.isEmpty()) {
        // m_collectionIds is updated every time collections are fetched
        QOrganizerManager::Error error;
        collections(&error);
    }
    return m_collectionIds;
}

void QOrganizerJsonDbDataStorage::run()
{
    m_waitMutex = new QMutex();
    m_jsonConnection = new JsonDbConnection(this);
    m_jsonConnection->connectToServer();
    m_jsonDb = new JsonDbClient(m_jsonConnection);

    connect(this, SIGNAL(requestInitialized()), this, SLOT(handleRequest()));
    connect(m_jsonDb, SIGNAL(notified(const QString&, const QVariant&, const QString&)),
            this, SLOT(onNotified(const QString&, const QVariant&, const QString&)));
    connect(m_jsonDb, SIGNAL(response(int, const QVariant)), this,
            SLOT(onResponse(int, const QVariant)));
    connect(m_jsonDb, SIGNAL(error(int, int, const QString&)), this,
            SLOT(onError(int, int, const QString&)));

    emit threadInitialized();
    QThread::run();
}

////////////////////////////////////////////////////////////////////////////////
// These functions are run in the context of QOrganizerJsonDbDataStorage thread
////////////////////////////////////////////////////////////////////////////////

void QOrganizerJsonDbDataStorage::onResponse(int trId, const QVariant &object)
{
    QOrganizerManager::Error error = handleErrorResponse(object, JsonDbError::NoError);
    handleResponse(trId, error, object);
}

void QOrganizerJsonDbDataStorage::onError(int trId, int errorCode, const QString &message)
{
    Q_UNUSED(message);
    QVariant object;
    QOrganizerManager::Error error = handleErrorResponse(object, errorCode);
    handleResponse(trId, error, object);
}

void QOrganizerJsonDbDataStorage::onNotified(const QString& notifyUuid, const QVariant& object, const QString& action)
{
    Q_UNUSED(notifyUuid);
    Q_UNUSED(object);
    Q_UNUSED(action);
}

void QOrganizerJsonDbDataStorage::handleRequest()
{
    switch (m_requestType) {
    case SaveItems:
        handleSaveItemsRequest();
        break;
    case Items:
        handleItemsRequest();
        break;
    case ItemsById:
        handleItemsByIdRequest();
        break;
    case RemoveItems:
        handleRemoveItemsRequest();
        break;
    case RemoveItemsByCollectionId:
        handleRemoveItemsByCollectionIdRequest();
        break;
    case SaveCollections:
        handleSaveCollectionsRequest();
        break;
    case Collections:
        handleCollectionsRequest();
        break;
    case RemoveCollections:
        handleRemoveCollectionsRequest();
        break;
    default:
        break;
    }
}

void QOrganizerJsonDbDataStorage::handleResponse(int trId, QOrganizerManager::Error error, const QVariant& object)
{
    int index;
    if (m_transactionIds.contains(trId)) {
        index = m_transactionIds.value(trId);
        m_transactionIds.remove(trId);
    } else {
        // This might happen if handling of some request was stopped due to timeout
        // Then remaining responses may come when no request is processed and m_transactionIds
        // has been cleared
        return;
    }

    switch (m_requestType) {
    case SaveItems:
        handleSaveItemsResponse(index, error, object);
        break;
    case Items:
        handleItemsResponse(error, object);
        break;
    case ItemsById:
        handleItemsByIdResponse(error, object);
        break;
    case RemoveItems:
        handleRemoveItemsResponse(index, error);
        break;
    case RemoveItemsByCollectionId:
        handleRemoveItemsByCollectionIdResponse(object);
        break;
    case SaveCollections:
        handleSaveCollectionsResponse(index, error, object);
        break;
    case Collections:
        handleCollectionsResponse(error, object);
        break;
    case RemoveCollections:
        handleRemoveCollectionsResponse(error, object);
        break;
    case Invalid:
        // no active request at the moment, internal variables have been cleared and some pointers have
        // NULL value, so response is just ignored
    default:
        break;
    }
}

void QOrganizerJsonDbDataStorage::handleSaveItemsRequest()
{
    bool requestSent = false;
    QMapIterator<int, QOrganizerItem> i(*m_resultItems);
    while (i.hasNext()) {
        i.next();
        QOrganizerItem item = i.value();
        bool itemIsNew = item.id().isNull();
        QVariantMap jsonDbItem;
        if (m_converter.itemToJsonDbObject(item, &jsonDbItem)) {
            int trId;
            if (itemIsNew)
                trId = m_jsonDb->create(jsonDbItem);
            else
                trId = m_jsonDb->update(jsonDbItem);
            m_transactionIds.insert(trId, i.key());
            requestSent = true;
        } else {
            *m_error = QOrganizerManager::InvalidItemTypeError;
            m_errorMap->insert(i.key(), *m_error);
        }
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveItemsResponse(int index, QOrganizerManager::Error error, const QVariant& object)
{
    QOrganizerItem item = m_resultItems->value(index);
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
            }
        }
        m_resultItems->insert(index, item);
    } else {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_transactionIds.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleItemsRequest()
{
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;
    switch (m_fetchType) {
    case FetchItems:
        break;
    case FetchItemIds:
        // TODO: We should use jsondb query to correctly fetch items within certain
        // time period. Then it would be enough to get just a list of uuids from db
        // For now we fetch the whole item, even though more optimal would be to fetch
        // only uuid, startdate and enddate fields

        //jsondb query [?type="com.nokia.mp.organizer.Item"][=_uuid]
        //newJsonDbQuery += ITEM_ID_RESULT_STRING;
        break;
    default:
        break;
    }

    //Apply Filter and get jsondb query expression
    QString filterString;
    if (compoundFilterToJsondbQuery(m_filter, filterString)) {
        newJsonDbQuery += filterString;
        int trId = m_jsonDb->query(newJsonDbQuery);
        // we don't care about item index when fetching items --> 0 index
        m_transactionIds.insert(trId, 0);
    } else {
        *m_error = QOrganizerManager::BadArgumentError;
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleItemsResponse(QOrganizerManager::Error error, const QVariant& object)
{
    if (error == QOrganizerManager::NoError) {
        // Object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of QVariantMaps
        // Each representing one retrieved OrganizerItem
        QVariantList jsonDbObjectList;
        if (!object.isNull())
            jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();

        for (int i = 0; i < jsonDbObjectList.size(); i++) {
            QOrganizerItem item;
            if (m_converter.jsonDbObjectToItem(jsonDbObjectList.at(i).toMap(), &item)) {
                if (QOrganizerManagerEngine::isItemBetweenDates(item, m_start, m_end))
                    m_items.append(item);
            }
        }
    } else {
        *m_error = error;
    }
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleItemsByIdRequest()
{
    QString itemQuery = "[?";
    for (int i = 0; i < m_itemIds.size(); i++)
        itemQuery += ITEM_IDS_QUERY_STRING (m_itemIds[i].toString().remove(QOrganizerJsonDbStr::ManagerName));

    // remove the last " | "
    itemQuery.truncate(itemQuery.length() - 3);
    itemQuery += "]";
    QString newJsonDbQuery = ALL_ITEM_QUERY_STRING;
    newJsonDbQuery += itemQuery;
    int trId = m_jsonDb->query(newJsonDbQuery);
    m_transactionIds.insert(trId, 0);
}

void QOrganizerJsonDbDataStorage::handleItemsByIdResponse(QOrganizerManager::Error error, const QVariant &object)
{
    if (error == QOrganizerManager::NoError) {
        QMap<QOrganizerItemId, QOrganizerItem> itemMap;
        QVariantList jsonDbObjectList;
        if (!object.isNull())
            jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();

        int i;
        for (i = 0; i < jsonDbObjectList.size(); i++) {
            QOrganizerItem item;
            if (m_converter.jsonDbObjectToItem(jsonDbObjectList.at(i).toMap(), &item))
                itemMap.insert(item.id(), item);
        }
        for (i = 0; i < m_itemIds.size(); i++) {
            QOrganizerItem item = itemMap.value(m_itemIds[i], QOrganizerItem());
            m_items.append(item);
            if (item.id().isNull()) {
                *m_error = QOrganizerManager::DoesNotExistError;
                m_errorMap->insert(i, *m_error);
            }
        }
    } else {
        *m_error = error;
    }
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsRequest()
{
    //loop to remove all the items from the remove items ids list
    for (int i = 0; i < m_itemIds.size(); i++) {
        //Get the uuid by removing the "qtorgainizer:jsondb::" prefix from id
        QString jsonUuid = m_itemIds.at(i).toString();
        jsonUuid = jsonUuid.remove(QOrganizerJsonDbStr::ManagerName);

        QVariantMap jsonDbRemoveItem;
        jsonDbRemoveItem.insert(JsonDbString::kUuidStr, jsonUuid);

        //Remove the item from JSONDB
        int trId = m_jsonDb->remove(jsonDbRemoveItem);
        m_transactionIds.insert(trId, i);
    }
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsResponse(int index, QOrganizerManager::Error error)
{
    if (error != QOrganizerManager::NoError) {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_transactionIds.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsByCollectionIdRequest()
{
    //Remove the items that are belong to collections
    QString jsondbQuery = ALL_ITEM_QUERY_STRING;
    //Set the filter for the jsondb query string
    QOrganizerItemCollectionFilter collectonFilter;
    collectonFilter.setCollectionIds(QSet<QOrganizerCollectionId>::fromList(m_removeItemCollectionIds));
    QString filterString;
    singleFilterToJsondbQuery(collectonFilter, filterString);
    jsondbQuery += filterString;
    int trId = m_jsonDb->remove(jsondbQuery);
    m_transactionIds.insert(trId, 0);
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsByCollectionIdResponse(const QVariant &object)
{
    int deletedCount = object.toMap().value(JsonDbString::kCountStr).toInt();
    //Check if some items are delete from request
    if (deletedCount > 0) {
        QList<QVariant> jsoItemList = object.toMap().value(JsonDbString::kDataStr).toList();
        //Make the item change set
        for (int i = 0; i < jsoItemList.size(); i++) {
            //Get delete item id
            QString uuid = QOrganizerJsonDbStr::ManagerName + jsoItemList.at(i).toMap().value(JsonDbString::kUuidStr).toString();
            QOrganizerItemId itemId = QOrganizerItemId::fromString(uuid);
            m_itemIds.append(itemId);
        }
    }
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveCollectionsRequest()
{
    bool requestSent = false;
    QMapIterator<int, QOrganizerCollection> i(*m_resultCollections);
    while (i.hasNext()) {
        i.next();
        QOrganizerCollection collection = i.value();
        bool collectionIsNew = collection.id().isNull();
        QVariantMap jsonDbCollection;
        if (m_converter.collectionToJsonDbObject(collection, m_isDefaultCollection, &jsonDbCollection)) {
            int trId;
            if (collectionIsNew)
                trId = m_jsonDb->create(jsonDbCollection);
            else
                trId = m_jsonDb->update(jsonDbCollection);

            m_transactionIds.insert(trId, i.key());
            requestSent = true;
        } else {
            *m_error = QOrganizerManager::InvalidItemTypeError;
            m_errorMap->insert(i.key(), *m_error);
        }
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveCollectionsResponse(int index, QOrganizerManager::Error error, const QVariant &object)
{
    QOrganizerCollection collection = m_resultCollections->value(index);
    if (error == QOrganizerManager::NoError) {
        if (collection.id().isNull()) {
            // this is a new collection
            QString jsonUuid = object.toMap().value(JsonDbString::kUuidStr).toString();
            if (!jsonUuid.isEmpty()) {
                QOrganizerJsonDbCollectionId* collectionId = new QOrganizerJsonDbCollectionId;
                collectionId->setCollectionId(jsonUuid);
                collection.setId(QOrganizerCollectionId(collectionId));
                m_collectionIds.insert(collection.id());
                // have we modified default collection
                if (collection.id() == m_defaultCollection.id())
                    m_defaultCollection = collection;
            }
        }
        m_resultCollections->insert(index, collection);
    } else {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_transactionIds.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleCollectionsRequest()
{
    QString newJsonDbQuery = COLLECTION_QUERY_STRING;
    int trId = m_jsonDb->query(newJsonDbQuery);
    // we don't care about item index when fetching collections --> 0 index
    m_transactionIds.insert(trId, 0);
}

void QOrganizerJsonDbDataStorage::handleCollectionsResponse(QOrganizerManager::Error error, const QVariant& object)
{
    if (error == QOrganizerManager::NoError) {
        m_collectionIds.clear();
        // Object is a QVariant containing QVariantMap("data", QVariantList). QVariantList is the list of QVariantMaps
        // Each representing one retrieved OrganizerItem

        QVariantList jsonDbObjectList;
        if (!object.isNull())
            jsonDbObjectList = object.toMap().value(JsonDbString::kDataStr).toList();

        for (int i = 0; i < jsonDbObjectList.size(); i++) {
            QOrganizerCollection collection;
            bool isDefaultCollection;
            if (m_converter.jsonDbObjectToCollection(jsonDbObjectList.at(i).toMap(), &collection, isDefaultCollection)) {
                if (isDefaultCollection) {
                    // Keeping default collection up-to-date in memory
                    m_defaultCollection = collection;
                }
                m_collections.append(collection);
                m_collectionIds.insert(collection.id());
            }
        }
    } else {
        *m_error = error;
    }
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveCollectionsRequest()
{
    QVariantList collectionJsonDbUuids;
    foreach (QOrganizerCollectionId collectionId, m_removeCollectionIds) {
        //Get the uuid by removing the "qtorgainizer:jsondb::" prefix from id
        QString jsonUuid = collectionId.toString();
        jsonUuid = jsonUuid.remove(QOrganizerJsonDbStr::ManagerName);
        QVariantMap uuidObject;
        uuidObject.insert(JsonDbString::kUuidStr, jsonUuid);
        collectionJsonDbUuids.append(uuidObject);
    }
    //Remove the item from JSONDB
    int trId = m_jsonDb->remove(collectionJsonDbUuids);
    m_transactionIds.insert(trId, 0);
}

void QOrganizerJsonDbDataStorage::handleRemoveCollectionsResponse(QOrganizerManager::Error error, const QVariant &object)
{
    int deletedCount = object.toMap().value(JsonDbString::kCountStr).toInt();
    if (QOrganizerManager::NoError == error) {
        //Request collection remove count
        int requestCount = m_removeCollectionIds.size();
        //Check if something goes wrong even we did not get any error code
        if (deletedCount != requestCount) {
            QString warning = QOrganizerJsonDbStr::WarningCollectionRemove;
            qWarning() << deletedCount << warning << requestCount;
        }
    } else {
        //Error happened and not all collection were removed
        //Figure out the removed and non-removed collections
        QList<QVariant> jsonErrorList = object.toMap().value(JsonDbString::kErrorStr).toList();

        //Check each error and update error map
        for (int i = 0; i < jsonErrorList.size(); i++) {
            QString uuid = QOrganizerJsonDbStr::ManagerName + jsonErrorList.at(i).toMap().value(JsonDbString::kUuidStr).toString();
            QOrganizerCollectionId collectionId = QOrganizerCollectionId::fromString(uuid);
            //Find out the item index
            int errorIndex = m_removeCollectionIds.key(collectionId, -1);
            if (errorIndex != -1) {
                JsonDbError::ErrorCode errorCode = static_cast<JsonDbError::ErrorCode>(jsonErrorList.at(i).toMap().value(JsonDbString::kErrorStr).toInt());
                QOrganizerManager::Error removeError = m_converter.jsondbErrorToOrganizerError(errorCode);
                m_errorMap->insert(errorIndex, removeError);
            }
        }
    }
    //Update memory list
    QMapIterator<int, QOrganizerCollectionId> j(m_removeCollectionIds);
    while (j.hasNext()) {
        j.next();
        if (!m_errorMap->contains(j.key()))
            m_collectionIds.remove(j.value());
    }
     m_syncWaitCondition.wakeAll();
}

QOrganizerManager::Error QOrganizerJsonDbDataStorage::handleErrorResponse(const QVariant &object, int errorCode)
{
    JsonDbError::ErrorCode jsonErrorCode;

    QVariant jsonErrorObject = object.toMap().value(JsonDbString::kErrorStr);
    if (jsonErrorObject.isNull() || jsonErrorObject.toList().isEmpty())
        jsonErrorCode = static_cast<JsonDbError::ErrorCode>(errorCode);
    else
        jsonErrorCode = static_cast<JsonDbError::ErrorCode>(jsonErrorObject.toList().at(0).toMap().value(JsonDbString::kCodeStr).toInt());

    return m_converter.jsondbErrorToOrganizerError (jsonErrorCode);
}

bool QOrganizerJsonDbDataStorage::compoundFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr)
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
            } else {//For union filter, single filter invalid means we could skip this filter
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

bool QOrganizerJsonDbDataStorage::singleFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr)
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
                jsonDbQueryStr += id.toString().remove(QOrganizerJsonDbStr::ManagerName);
                jsonDbQueryStr += "\",\""; // ","
                validIdCount ++;
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

void QOrganizerJsonDbDataStorage::processRequest()
{
    emit requestInitialized();
    QMutexLocker locker(m_waitMutex);
    int msecs = 2000; // TODO: handle timeout nicely
    bool requestFinished = m_syncWaitCondition.wait(m_waitMutex, msecs);
    if (!requestFinished) {
        qWarning() << "Timeout, not response received!!!";
        // timeout TODO: check which items have completed and update error map accordingly...
    }
}

void QOrganizerJsonDbDataStorage::initRequestData(RequestType requestType, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    m_requestType = requestType;
    m_errorMap = errorMap;
    m_error = error;
    m_transactionIds.clear();
    m_resultItems = 0;
    m_resultCollections = 0;
    m_items.clear();
    m_fetchType = NoFetch;
    m_itemIds.clear();
    m_collections.clear();
    m_removeItemCollectionIds.clear();
    m_removeCollectionIds.clear();
    m_isDefaultCollection = false;
}

void QOrganizerJsonDbDataStorage::clearRequestData()
{
    m_requestType = Invalid;
    m_errorMap = 0;
    m_error = 0;
    m_transactionIds.clear();
    m_resultItems = 0;
    m_resultCollections = 0;
    m_items.clear();
    m_fetchType = NoFetch;
    m_itemIds.clear();
    m_collections.clear();
    m_removeItemCollectionIds.clear();
    m_removeCollectionIds.clear();
    m_isDefaultCollection = false;
}

#include "moc_qorganizerjsondbdatastorage.cpp"

QTORGANIZER_END_NAMESPACE
