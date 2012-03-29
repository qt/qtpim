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

#ifndef QORGANIZERJSONDBDATASTORAGE_H
#define QORGANIZERJSONDBDATASTORAGE_H

#include <QThread>
#include <QWaitCondition>
#include <QList>

#include "qorganizer.h"
#include "qorganizerjsondbconverter.h"

#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbwatcher.h>

QT_USE_NAMESPACE_JSONDB

QTORGANIZER_BEGIN_NAMESPACE

// The purpose of this class is to provide synchronous access to jsondb and also hide jsondb
// specific details such as filter creation and object conversions.
// All public methods of this class are called from the thread of the caller. The functions store
// their parameters to member variables, emit request signal and start to wait until the request has
// been completed. The request signal is handled in the separate thread. Handler functions typically
// start jsondb requests. When responses are received from jsondb, results are stored to member variables
// and waiting caller thread is woken up.


class QOrganizerJsonDbDataStorage: public QThread
{
    Q_OBJECT

public:
    enum FetchType {
        NoFetch,
        FetchItems,
        FetchItemIds,
        FetchItemOccurrences,
        FetchParents
    };

    QOrganizerJsonDbDataStorage();
    ~QOrganizerJsonDbDataStorage();

    void saveItems(QMap<int, QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    QList<QOrganizerItem> items(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders,
                                const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error, FetchType type = FetchItems, const QOrganizerItemId &parentId = QOrganizerItemId());
    QList<QOrganizerItem> itemsById(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    void removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);

    void saveCollections(QMap<int, QOrganizerCollection>* collections, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error, QOrganizerAbstractRequest::StorageLocation storageLocation);
    QList<QOrganizerCollection> collections(QOrganizerManager::Error* error, QOrganizerAbstractRequest::StorageLocations storageLocations);
    void removeCollections(const QMap<int, QOrganizerCollectionId>& collectionIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);

    void createDefaultCollection(QOrganizerCollection* defaultCollection, QOrganizerManager::Error* error);
    QOrganizerCollection defaultCollection();
    QSet<QOrganizerCollectionId> collectionIds();

    void saveAlarm(const QOrganizerItem *item, const QString *alarmUuid, QOrganizerManager::Error *error);
    void removeAlarm(const QString *alarmUuid, QOrganizerManager::Error *error);
    QString alarmId(const QOrganizerItemId *itemId, QOrganizerManager::Error *error);

signals:
    void threadInitialized();
    void requestInitialized();

    void itemAdded(const QOrganizerItemId &itemId);
    void itemChanged(const QOrganizerItemId &itemId);
    void itemRemoved(const QOrganizerItemId &itemId);
    void collectionAdded(const QOrganizerCollectionId &collectionId);
    void collectionChanged(const QOrganizerCollectionId &collectionId);
    void collectionRemoved(const QOrganizerCollectionId &collectionId);

protected:
    virtual void run();

private slots:
    void handleRequest();

    void onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode error, const QString &message);
    void onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode error, const QString &message);
    void onJsonDbRequestFinished();
    void onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode error, const QString &message);
    void onJsonDbWatcherNotificationsAvailable();

private:
    enum RequestType {
        Invalid,
        SaveItems,
        Items,
        ItemsById,
        RemoveItems,
        SaveCollections,
        Collections,
        RemoveCollections,
        AlarmId,
        SaveAlarm,
        RemoveAlarm
    };

    void handleResponse(QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleSaveItemsRequest();
    void handleSaveItemsResponse(int index, QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleItemsRequest();
    void handleItemsResponse(QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleItemsByIdRequest();
    void handleItemsByIdResponse(QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleRemoveItemsRequest();
    void handleRemoveItemsResponse(int index, QOrganizerManager::Error error);
    void handleSaveCollectionsRequest();
    void handleSaveCollectionsResponse(int index, QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleCollectionsRequest();
    void handleCollectionsResponse(QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleRemoveCollectionsRequest();
    void handleRemoveCollectionsResponse(QOrganizerManager::Error error);
    void handleSaveAlarmRequest();
    void handleSaveAlarmResponse(QOrganizerManager::Error error);
    void handleAlarmIdRequest();
    void handleAlarmIdResponse(QOrganizerManager::Error error, QJsonDbRequest *request);
    void handleRemoveAlarmRequest();
    void handleRemoveAlarmResponse(QOrganizerManager::Error error);

    enum JsonDbRequestType {
        JsonDbReadRequest = 0,
        JsonDbCreateRequest,
        JsonDbUpdateRequest,
        JsonDbRemoveRequest
    };
    bool makeJsonDbRequest(JsonDbRequestType jsonDbRequestType, int index, const QString &query = QString(), const QList<QJsonObject> &objects = QList<QJsonObject>(), const QString &storageLocation = QString());

    void processRequest();
    void initRequestData(RequestType requestType, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    void clearRequestData();
    void registerForNotifications();

    QMutex* m_waitMutex;
    QWaitCondition m_syncWaitCondition;

    QOrganizerJsonDbConverter m_converter;
    QJsonDbConnection *m_jsonDbConnection;

    // "collection cache"
    QSet<QOrganizerCollectionId> m_collectionIds;
    QOrganizerCollection m_defaultCollection;

    QString m_notificationObjectUuid;
    QList<QJsonDbWatcher*> m_watchers;

    // request data

    // common
    RequestType m_requestType;
    QMap<QJsonDbRequest *, int> m_requestIndexMap;  // map from request to item / collection index
    QMap<int, QOrganizerManager::Error>* m_errorMap;
    QOrganizerManager::Error* m_error;

    // storage location
    QOrganizerAbstractRequest::StorageLocation m_saveToStorageLocation;
    int m_fetchFromStorageLocations;
    bool m_storageLocationsMissing;

    // SaveItems
    QMap<int, QOrganizerItem>* m_resultItems; // map from item index to item

    // Items
    QList<QOrganizerItem> m_items;
    QDateTime m_start;
    QDateTime m_end;
    QOrganizerItemFilter m_filter;
//    QList<QOrganizerItemSortOrder> m_sortOrders;
//    QOrganizerItemFetchHint m_fetchHint;
    FetchType m_fetchType;
    QOrganizerItemId m_parentItemId;

    // RemoveItems (itemsById)
    QList<QOrganizerItemId> m_itemIds;

    // RemoveItemsByCollectionId
    QList<QOrganizerCollectionId> m_removeItemCollectionIds;

    // RemoveItemsByParentId
    QList<QOrganizerItemId> m_removeItemParentIds;

    // SaveCollection
    QMap<int, QOrganizerCollection>* m_resultCollections; // map from collection index to collection
    bool m_isDefaultCollection;

    // Collections
    QList<QOrganizerCollection> m_collections;

    // RemoveCollections
    QMap<int, QOrganizerCollectionId> m_removeCollectionIds;

    // Alarm Id string
    QString m_alarmId;
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBDATASTORAGE_H
