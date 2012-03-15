/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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
#include "qorganizerjsondbdatastorage.h"

#include <QMutexLocker>

#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring.h"

QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbDataStorage::QOrganizerJsonDbDataStorage()
    : m_waitMutex(0)
    , m_jsonDbConnection(0)
    , m_jsonDbWatcher(0)
{
    clearRequestData();
}

QOrganizerJsonDbDataStorage::~QOrganizerJsonDbDataStorage()
{
    delete m_waitMutex;
}

void QOrganizerJsonDbDataStorage::saveItems(QMap<int, QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    if (items->isEmpty())
        return;
    initRequestData(SaveItems, errorMap, error);
    m_resultItems = items;
    processRequest();
    clearRequestData();
}

QList<QOrganizerItem> QOrganizerJsonDbDataStorage::items(const QDateTime &startDate, const QDateTime &endDate, const QOrganizerItemFilter &filter,
                                                         const QList<QOrganizerItemSortOrder> &sortOrders, const QOrganizerItemFetchHint &fetchHint,
                                                         QOrganizerManager::Error *error, FetchType type, const QOrganizerItemId &parentId)
{
    Q_UNUSED(sortOrders);
    Q_UNUSED(fetchHint);
    initRequestData(Items, 0, error);
    m_start = startDate;
    m_end = endDate;
    m_filter = filter;
    if (type == FetchItemOccurrences)
        m_parentItemId = parentId;
    m_fetchType = type;
    processRequest();
    QList<QOrganizerItem> fetchedItems = m_items;
    clearRequestData();
    return fetchedItems;
}

QList<QOrganizerItem> QOrganizerJsonDbDataStorage::itemsById(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error *error)
{
    if (itemIds.isEmpty())
        return QList<QOrganizerItem>();
    initRequestData(ItemsById, errorMap, error);
    m_itemIds = itemIds;
    processRequest();
    QList<QOrganizerItem> fetchedItems = m_items;
    clearRequestData();
    return fetchedItems;
}

void QOrganizerJsonDbDataStorage::removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    if (itemIds.isEmpty())
        return;
    initRequestData(RemoveItems, errorMap, error);
    m_itemIds = itemIds;
    processRequest();
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::saveCollections(QMap<int, QOrganizerCollection>* collections, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    if (collections->isEmpty())
        return;
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
    if (collectionIds.isEmpty())
        return;
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

QString QOrganizerJsonDbDataStorage::alarmId(const QOrganizerItemId *itemId, QOrganizerManager::Error *error)
{
    if (itemId->isNull())
        return QString();
    initRequestData(AlarmId, 0, error);
    m_itemIds.append(*itemId);
    processRequest();
    QString ret = m_alarmId;
    clearRequestData();
    return ret;
}

void QOrganizerJsonDbDataStorage::saveAlarm(const QOrganizerItem *item, const QString *alarmUuid, QOrganizerManager::Error *error)
{
    initRequestData(SaveAlarm, 0, error);
    m_items.append(*item);
    if (!alarmUuid->isEmpty())
        m_alarmId = *alarmUuid;
    processRequest();
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::removeAlarm(const QString *alarmUuid, QOrganizerManager::Error *error)
{
    initRequestData(RemoveAlarm, 0, error);
    m_alarmId = *alarmUuid;
    processRequest();
    clearRequestData();
}

void QOrganizerJsonDbDataStorage::run()
{
    m_waitMutex = new QMutex();

    m_jsonDbConnection = new QJsonDbConnection(this);
    connect(m_jsonDbConnection, SIGNAL(error(QtJsonDb::QJsonDbConnection::ErrorCode,QString)),
            this, SLOT(onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode,QString)));
    m_jsonDbConnection->connectToServer();

    m_jsonDbWatcher = new QJsonDbWatcher(this);
    m_jsonDbWatcher->setWatchedActions(QJsonDbWatcher::All);
    m_jsonDbWatcher->setQuery(QOrganizerJsonDbStr::jsonDbNotificationQuery());
    connect(m_jsonDbWatcher, SIGNAL(error(QtJsonDb::QJsonDbWatcher::ErrorCode,QString)),
            this, SLOT(onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode,QString)));
    connect(m_jsonDbWatcher, SIGNAL(notificationsAvailable(int)),
            this, SLOT(onJsonDbWatcherNotificationsAvailable()));
    m_jsonDbConnection->addWatcher(m_jsonDbWatcher);

    connect(this, SIGNAL(requestInitialized()), this, SLOT(handleRequest()));
    emit threadInitialized();
    QThread::run();
}

////////////////////////////////////////////////////////////////////////////////
// These functions are run in the context of QOrganizerJsonDbDataStorage thread
////////////////////////////////////////////////////////////////////////////////

void QOrganizerJsonDbDataStorage::onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode error, const QString &message)
{
    Q_UNUSED(message)

    // currently there's no real errors from JsonDb
    Q_ASSERT(m_converter.jsonDbConnectionErrorToOrganizerError(error) == QOrganizerManager::NoError);
}

void QOrganizerJsonDbDataStorage::onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode error, const QString &message)
{
    Q_UNUSED(message)
    // if the error() signal is emitted, the finished() signal won't be emitted, so need to call handleResponse()
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (request)
        handleResponse(m_converter.jsonDbRequestErrorToOrganizerError(error), request);
}

void QOrganizerJsonDbDataStorage::onJsonDbRequestFinished()
{
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (request)
        handleResponse(QOrganizerManager::NoError, request);
}

void QOrganizerJsonDbDataStorage::onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode error, const QString &message)
{
    // TODO handle the error when partition is supported
    Q_UNUSED(error)
    Q_UNUSED(message)
}

void QOrganizerJsonDbDataStorage::onJsonDbWatcherNotificationsAvailable()
{
    // no mutex is needed since this slot doesn't touch any member variables

    QList<QJsonDbNotification> notifications = m_jsonDbWatcher->takeNotifications();
    for (int i = 0; i < notifications.size(); ++i) {
        QJsonObject jsonDbObject = notifications.at(i).object();
        QString jsonDbType = m_converter.jsonDbNotificationObjectToOrganizerType(jsonDbObject);
        if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventType()
            || jsonDbType == QOrganizerJsonDbStr::jsonDbEventOccurrenceType()
            || jsonDbType == QOrganizerJsonDbStr::jsonDbTodoType()
            || jsonDbType == QOrganizerJsonDbStr::jsonDbTodoOccurrenceType()) {
            switch (notifications.at(i).action()) {
            case QJsonDbWatcher::Created:
                emit itemAdded(m_converter.jsonDbNotificationObjectToItemId(jsonDbObject));
                break;
            case QJsonDbWatcher::Updated:
                emit itemChanged(m_converter.jsonDbNotificationObjectToItemId(jsonDbObject));
                break;
            case QJsonDbWatcher::Removed:
                emit itemRemoved(m_converter.jsonDbNotificationObjectToItemId(jsonDbObject));
                break;
            default:
                break;
            }
        } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbCollectionType()) {
            switch (notifications.at(i).action()) {
            case QJsonDbWatcher::Created:
                emit collectionAdded(m_converter.jsonDbNotificationObjectToCollectionId(jsonDbObject));
                break;
            case QJsonDbWatcher::Updated:
                emit collectionChanged(m_converter.jsonDbNotificationObjectToCollectionId(jsonDbObject));
                break;
            case QJsonDbWatcher::Removed:
                emit collectionRemoved(m_converter.jsonDbNotificationObjectToCollectionId(jsonDbObject));
                break;
            default:
                break;
            }
        }
    }
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
    case SaveCollections:
        handleSaveCollectionsRequest();
        break;
    case Collections:
        handleCollectionsRequest();
        break;
    case RemoveCollections:
        handleRemoveCollectionsRequest();
        break;
    case AlarmId:
        handleAlarmIdRequest();
        break;
    case SaveAlarm:
        handleSaveAlarmRequest();
        break;
    case RemoveAlarm:
        handleRemoveAlarmRequest();
        break;
    default:
        break;
    }
}

void QOrganizerJsonDbDataStorage::handleResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    int index = -1;
    QMap<QJsonDbRequest *, int>::iterator i = m_requestIndexMap.find(request);
    if (i != m_requestIndexMap.end()) {
        index = i.value();
        m_requestIndexMap.erase(i);
    } else {
        // It might fall here if handling of some request was stopped due to timeout,
        // then remaining responses may come when no request is processed and m_requestIndexMap
        // has been cleared. In such cases, do nothing.
        return;
    }

    switch (m_requestType) {
    case SaveItems:
        handleSaveItemsResponse(index, error, request);
        break;
    case Items:
        handleItemsResponse(error, request);
        break;
    case ItemsById:
        handleItemsByIdResponse(error, request);
        break;
    case RemoveItems:
        handleRemoveItemsResponse(index, error);
        break;
    case SaveCollections:
        handleSaveCollectionsResponse(index, error, request);
        break;
    case Collections:
        handleCollectionsResponse(error, request);
        break;
    case RemoveCollections:
        handleRemoveCollectionsResponse(error);
        break;
    case AlarmId:
        handleAlarmIdResponse(error, request);
        break;
    case SaveAlarm:
        handleSaveAlarmResponse(error);
        break;
    case RemoveAlarm:
        handleRemoveAlarmResponse(error);
        break;
    case Invalid:
        // no active request at the moment, internal variables have been cleared and some pointers have
        // NULL value, so response is just ignored
    default:
        break;
    }

    delete request;
}

void QOrganizerJsonDbDataStorage::handleSaveItemsRequest()
{
    bool requestSent = false;
    QMap<int, QOrganizerItem>::const_iterator i = m_resultItems->begin();
    while (i != m_resultItems->end()) {
        bool itemIsNew = i.value().id().isNull();
        QJsonObject jsonDbItem;
        if (m_converter.itemToJsonDbObject(i.value(), &jsonDbItem)) {
            JsonDbRequestType requestType;
            if (itemIsNew)
                requestType = JsonDbCreateRequest;
            else
                requestType = JsonDbUpdateRequest;
            if (makeJsonDbRequest(requestType, i.key(), QString(), QList<QJsonObject>() << jsonDbItem))
                requestSent = true;
        } else {
            *m_error = QOrganizerManager::InvalidItemTypeError;
            m_errorMap->insert(i.key(), *m_error);
        }

        ++i;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveItemsResponse(int index, QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        if (results.size() > 0) {
            QOrganizerItem item = m_resultItems->value(index);
            QJsonObject jsonDbItem = results.at(0);
            bool gotUuid = !item.id().isNull();
            bool gotVersion = false;
            QJsonObject::const_iterator i = jsonDbItem.constBegin();
            while (i != jsonDbItem.constEnd()) {
                if (!gotUuid && i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
                    // it's a new item, and we need to set the item ID
                    gotUuid = true;
                    QString jsonDbUuid = i.value().toString();
                    if (!jsonDbUuid.isEmpty()) {
                        QOrganizerJsonDbItemId* itemId = new QOrganizerJsonDbItemId;
                        itemId->setItemId(jsonDbUuid);
                        item.setId(QOrganizerItemId(itemId));
                    }
                } else if (!gotVersion && i.key() == QOrganizerJsonDbStr::jsonDbVersion()) {
                    gotVersion = true;
                    QString jsonDbVersion = i.value().toString();
                    if (!jsonDbVersion.isEmpty()) {
                        QOrganizerItemVersion itemVersion = item.detail(QOrganizerItemDetail::TypeVersion);
                        m_converter.jsonDbVersionToItemVersion(jsonDbVersion, &itemVersion);
                        item.saveDetail(&itemVersion);
                    }
                }
                if (gotUuid && gotVersion)
                    break;
                ++i;
            }

            m_resultItems->insert(index, item);
        }
    } else {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleItemsRequest()
{
    QString jsonDbQuery;
    switch (m_fetchType) {
    case FetchItems:
        // break;
    case FetchItemIds:
    {
        // TODO: We should use jsondb query to correctly fetch items within certain
        // time period. Then it would be enough to get just a list of uuids from db
        // For now we fetch the whole item, even though more optimal would be to fetch
        // only uuid, startdate and enddate fields

        // for now, this is common for FetchItems and FetchItemIds
        jsonDbQuery = QOrganizerJsonDbStr::jsonDbQueryAllItems();
        //Apply Filter and get jsondb query expression
        QString filterString;
        if (m_converter.compoundFilterToJsondbQuery(m_filter, &filterString)) {
            jsonDbQuery += filterString;

            if (makeJsonDbRequest(JsonDbReadRequest, 0, jsonDbQuery)) {
                // can't query normal object and view object at the same time
                // TODO only query view objects for when needed
                makeJsonDbRequest(JsonDbReadRequest, 0, QOrganizerJsonDbStr::jsonDbQueryEventViews() + filterString);
            } else {
                m_syncWaitCondition.wakeAll();
            }
        } else {
            *m_error = QOrganizerManager::BadArgumentError;
            m_syncWaitCondition.wakeAll();
        }
        break;
    }
    case FetchParents:
    {
        // This is used for fetching all parent items
        if (!makeJsonDbRequest(JsonDbReadRequest, 0, QOrganizerJsonDbStr::jsonDbQueryParentItems()))
            m_syncWaitCondition.wakeAll();

        // TODO: handle view objects
        break;
    }
    case FetchItemOccurrences:
    {
        jsonDbQuery = QOrganizerJsonDbStr::jsonDbQueryOccurrenceItemsByParent().arg(m_parentItemId.toString().remove(QOrganizerJsonDbStr::managerName()));
        if (!makeJsonDbRequest(JsonDbReadRequest, 0, jsonDbQuery))
            m_syncWaitCondition.wakeAll();

        // TODO: handle view objects
        break;
    }
    default:
        break;
    }
}

void QOrganizerJsonDbDataStorage::handleItemsResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        for (int i = 0; i < results.size(); ++i) {
            QOrganizerItem item;
            if (m_converter.jsonDbObjectToItem(results.at(i), &item)) {
                // FIXME: Comparing item dates should be done in database.
                if (QOrganizerManagerEngine::isItemBetweenDates(item, m_start, m_end)
                        || (m_fetchType == FetchParents && !item.detail(QOrganizerItemDetail::TypeRecurrence).isEmpty()))
                    m_items.append(item);
            }
        }
    } else {
        *m_error = error;
    }

    // when querying view objects, 2 queries are fired, so need to check
    if (m_requestIndexMap.isEmpty())
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleItemsByIdRequest()
{
    const QString uuidTemplate(QStringLiteral("\"%1\","));
    QString itemQuery;
    for (int i = 0; i < m_itemIds.size(); ++i)
        itemQuery += uuidTemplate.arg(m_itemIds.at(i).isNull() ? QString() : QOrganizerManagerEngine::engineItemId(m_itemIds.at(i))->toString());

    // remove the last ","
    itemQuery.truncate(itemQuery.length() - 1);

    QString newJsonDbQuery(QOrganizerJsonDbStr::jsonDbQueryAllItems());
    newJsonDbQuery.append(QOrganizerJsonDbStr::jsonDbQueryUuidsTemplate().arg(itemQuery));

    if (!makeJsonDbRequest(JsonDbReadRequest, 0, newJsonDbQuery))
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleItemsByIdResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        QMap<QOrganizerItemId, QOrganizerItem> itemMap;
        for (int i = 0; i < results.size(); ++i) {
            QOrganizerItem item;
            if (m_converter.jsonDbObjectToItem(results.at(i), &item))
                itemMap.insert(item.id(), item);
        }

        for (int i = 0; i < m_itemIds.size(); ++i) {
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
    bool requestSent = false;
    for (int i = 0; i < m_itemIds.size(); ++i) {
        QJsonObject jsonDbItem;
        if (m_itemIds.at(i).isNull()) {
            *m_error = QOrganizerManager::DoesNotExistError;
            m_errorMap->insert(i, QOrganizerManager::DoesNotExistError);
            continue;
        }
        QString jsonDbUuid = QOrganizerManagerEngine::engineItemId(m_itemIds.at(i))->toString();
        jsonDbItem.insert(QOrganizerJsonDbStr::jsonDbUuid(), jsonDbUuid);
        if (makeJsonDbRequest(JsonDbRemoveRequest, i, QString(), QList<QJsonObject>() << jsonDbItem))
            requestSent = true;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveItemsResponse(int index, QOrganizerManager::Error error)
{
    if (error != QOrganizerManager::NoError) {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleSaveCollectionsRequest()
{
    bool requestSent = false;
    QMap<int, QOrganizerCollection>::const_iterator i = m_resultCollections->constBegin();
    while (i != m_resultCollections->constEnd()) {
        QOrganizerCollection collection = i.value();
        bool collectionIsNew = collection.id().isNull();

        // check whether this is a default collection
        // if we are creating a new default collection, then m_isDefaultCollection is true
        // if we are modifying an existing default collection, then collection id should be the same
        // as m_defaultCollection id.
        bool convertToDefaultCollection;
        if (collectionIsNew)
            convertToDefaultCollection = m_isDefaultCollection;
        else
            convertToDefaultCollection = (collection.id() == m_defaultCollection.id());

        QJsonObject jsonDbCollection;
        if (m_converter.collectionToJsonDbObject(collection, convertToDefaultCollection, &jsonDbCollection)) {
            JsonDbRequestType requestType;
            if (collectionIsNew)
                requestType = JsonDbCreateRequest;
            else
                requestType = JsonDbUpdateRequest;
            if (makeJsonDbRequest(requestType, i.key(), QString(), QList<QJsonObject>() << jsonDbCollection))
                requestSent = true;
        } else {
            *m_error = QOrganizerManager::InvalidCollectionError;
            m_errorMap->insert(i.key(), *m_error);
        }
        ++i;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveCollectionsResponse(int index, QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QOrganizerCollection collection = m_resultCollections->value(index);
        if (collection.id().isNull()) {
            QList<QJsonObject> results = request->takeResults();
            if (results.size() > 0) {
                QString jsonUuid = results.at(0).value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
                if (!jsonUuid.isEmpty()) {
                    QOrganizerJsonDbCollectionId *collectionId = new QOrganizerJsonDbCollectionId;
                    collectionId->setCollectionId(jsonUuid);
                    collection.setId(QOrganizerCollectionId(collectionId));
                    m_collectionIds.insert(collection.id());
                    // have we modified default collection
                    if (collection.id() == m_defaultCollection.id())
                        m_defaultCollection = collection;
                }
            }
            m_resultCollections->insert(index, collection);
        }
    } else {
        *m_error = error;
        m_errorMap->insert(index, *m_error);
    }
    if (m_requestIndexMap.isEmpty()) {
        // All transactions have been handled, request has been completed
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleCollectionsRequest()
{
    if (!makeJsonDbRequest(JsonDbReadRequest, 0, QOrganizerJsonDbStr::jsonDbQueryAllCollections()))
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleCollectionsResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        m_collectionIds.clear();
        QList<QJsonObject> results = request->takeResults();
        for (int i = 0; i < results.size(); ++i) {
            QOrganizerCollection collection;
            bool isDefaultCollection(false);
            if (m_converter.jsonDbObjectToCollection(results.at(i), &collection, &isDefaultCollection)) {
                if (isDefaultCollection)
                    m_defaultCollection = collection;
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
    QMap<int, QOrganizerCollectionId>::const_iterator i = m_removeCollectionIds.constBegin();
    QList<QJsonObject> jsonDbCollections;
    jsonDbCollections.reserve(m_removeCollectionIds.size());
    while (i != m_removeCollectionIds.constEnd()) {
        QJsonObject jsonDbObject;
        QString jsonDbUuid = QOrganizerManagerEngine::engineCollectionId(i.value())->toString();
        jsonDbObject.insert(QOrganizerJsonDbStr::jsonDbUuid(), jsonDbUuid);
        jsonDbCollections.append(jsonDbObject);
        ++i;
    }

    if (!makeJsonDbRequest(JsonDbRemoveRequest, 0, QString(), QList<QJsonObject>() << jsonDbCollections))
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveCollectionsResponse(QOrganizerManager::Error error)
{
    if (error == QOrganizerManager::NoError) {
        QMap<int, QOrganizerCollectionId>::const_iterator i = m_removeCollectionIds.constBegin();
        while (i != m_removeCollectionIds.constEnd()) {
            m_collectionIds.remove(i.value());
            ++i;
        }
    } else {
        *m_error = error;
        QMap<int, QOrganizerCollectionId>::const_iterator i = m_removeCollectionIds.constBegin();
        while (i != m_removeCollectionIds.constEnd()) {
            m_errorMap->insert(i.key(), error);
            ++i;
        }
    }
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleAlarmIdRequest()
{
    if (!makeJsonDbRequest(JsonDbReadRequest, 0,
                           QOrganizerJsonDbStr::jsonDbQueryAlarmsTemplate()
                               .arg(QOrganizerManagerEngine::engineItemId(m_itemIds[0])->toString()))) {
        m_syncWaitCondition.wakeAll();
    }
}

void QOrganizerJsonDbDataStorage::handleAlarmIdResponse(QOrganizerManager::Error error, QJsonDbRequest *request)
{
    if (error == QOrganizerManager::NoError) {
        QList<QJsonObject> results = request->takeResults();
        if (results.size() == 1) {
            m_alarmId = results.at(0).value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
        } else if (results.size() > 1) {
            m_alarmId = results.at(0).value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
            *m_error = QOrganizerManager::InvalidDetailError;
            qWarning("More than one alarm for one event!");
        }
    } else {
        *m_error = error;
    }
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveAlarmRequest()
{
    bool requestSent = false;
    QJsonObject jsonDbAlarm;
    if (m_converter.itemToJsondbAlarmObject(m_items.at(0), &jsonDbAlarm)) {
        JsonDbRequestType requestType;
        if (m_alarmId.isEmpty()) {
            requestType = JsonDbCreateRequest;
        } else {
            jsonDbAlarm.insert(QOrganizerJsonDbStr::jsonDbUuid(), m_alarmId);
            requestType = JsonDbUpdateRequest;
        }

        requestSent = makeJsonDbRequest(requestType, 0, QString(), QList<QJsonObject>() << jsonDbAlarm);
    } else {
        *m_error = QOrganizerManager::InvalidDetailError;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleSaveAlarmResponse(QOrganizerManager::Error error)
{
    if (error != QOrganizerManager::NoError)
        *m_error = error;
    m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveAlarmRequest()
{
    bool requestSent = false;
    if (!m_alarmId.isEmpty()) {
        QJsonObject jsonDbAlarm;
        jsonDbAlarm.insert(QOrganizerJsonDbStr::jsonDbUuid(), m_alarmId);
        requestSent = makeJsonDbRequest(JsonDbRemoveRequest, 0, QString(), QList<QJsonObject>() << jsonDbAlarm);
    } else {
        *m_error = QOrganizerManager::InvalidDetailError;
    }
    if (!requestSent)
        m_syncWaitCondition.wakeAll();
}

void QOrganizerJsonDbDataStorage::handleRemoveAlarmResponse(QOrganizerManager::Error error)
{
    if (QOrganizerManager::NoError != error)
        *m_error = error;
    m_syncWaitCondition.wakeAll();
}

bool QOrganizerJsonDbDataStorage::makeJsonDbRequest(JsonDbRequestType jsonDbRequestType, int index, const QString &query, const QList<QJsonObject> &objects)
{
    QJsonDbRequest *request;
    switch (jsonDbRequestType) {
    case JsonDbReadRequest:
        request = new QJsonDbReadRequest(query, this);
        break;
    case JsonDbCreateRequest:
        request = new QJsonDbCreateRequest(objects, this);
        break;
    case JsonDbUpdateRequest:
        request = new QJsonDbUpdateRequest(objects, this);
        break;
    case JsonDbRemoveRequest:
        request = new QJsonDbRemoveRequest(objects, this);
        break;
    }
    connect(request, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)),
            this, SLOT(onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode,QString)));
    connect(request, SIGNAL(finished()), this, SLOT(onJsonDbRequestFinished()));
    if (m_jsonDbConnection->send(request)) {
        m_requestIndexMap.insert(request, index);
        return true;
    } else {
        delete request;
        *m_error = QOrganizerManager::UnspecifiedError;
        if (m_errorMap)
            m_errorMap->insert(index, QOrganizerManager::UnspecifiedError);
    }
    return false;
}

void QOrganizerJsonDbDataStorage::processRequest()
{
    emit requestInitialized();
    QMutexLocker locker(m_waitMutex);
    int msecs = 10000; // TODO: handle timeout nicely
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
    m_requestIndexMap.clear();
    m_resultItems = 0;
    m_resultCollections = 0;
    m_items.clear();
    QOrganizerItemId tmpId;
    m_parentItemId = tmpId;
    m_fetchType = NoFetch;
    m_itemIds.clear();
    m_collections.clear();
    m_removeItemCollectionIds.clear();
    m_removeItemParentIds.clear();
    m_removeCollectionIds.clear();
    m_isDefaultCollection = false;
    m_alarmId.clear();
}

void QOrganizerJsonDbDataStorage::clearRequestData()
{
    m_requestType = Invalid;
    m_errorMap = 0;
    m_error = 0;
    m_requestIndexMap.clear();
    m_resultItems = 0;
    m_resultCollections = 0;
    m_items.clear();
    QOrganizerItemId tmpId;
    m_parentItemId = tmpId;
    m_fetchType = NoFetch;
    m_itemIds.clear();
    m_collections.clear();
    m_removeItemCollectionIds.clear();
    m_removeItemParentIds.clear();
    m_removeCollectionIds.clear();
    m_isDefaultCollection = false;
    m_alarmId.clear();
}

#include "moc_qorganizerjsondbdatastorage.cpp"

QTORGANIZER_END_NAMESPACE
