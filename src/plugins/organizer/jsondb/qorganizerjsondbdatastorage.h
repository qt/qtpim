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

#ifndef QORGANIZERJSONDBDATASTORAGE_H
#define QORGANIZERJSONDBDATASTORAGE_H

#include <QThread>
#include <QWaitCondition>
#include <QList>

#include "qorganizer.h"
#include "qorganizerjsondbconverter.h"

#include <jsondb-error.h>
#include <jsondb-global.h>

Q_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbClient;
class JsonDbConnection;
Q_ADDON_JSONDB_END_NAMESPACE
Q_USE_JSONDB_NAMESPACE
QTORGANIZER_BEGIN_NAMESPACE

// The purpose of this class is to provide synchronous access to jsondb and also hide jsondb
// specific details such as filter creation and object conversions.
// All public methods of this class are called from the thread of the caller. The functions store
// their parameters to member variables, emit request signal and start to wait until the request has
// been completed. The request signal is handled in the separate thread. Handler functions typically
// start jsondb requests. When responses are received from jsondb, results are stored to member variables
// and waiting caller thread is woken up.

// TODO: notification system


class QOrganizerJsonDbDataStorage: public QThread
{
    Q_OBJECT

public:

    enum FetchType {
        NoFetch,
        FetchItems,
        FetchItemIds,
        FetchItemOccurrences,
        FetchAllParents
    };

    QOrganizerJsonDbDataStorage();

    void saveItems(QMap<int, QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    QList<QOrganizerItem> items(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders,
                                const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error, FetchType type = FetchItems);
    QList<QOrganizerItem> itemsById(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    void removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    QList<QOrganizerItemId> removeItemsByCollectionId(const QList<QOrganizerCollectionId>& collectionIds);

    void saveCollections(QMap<int, QOrganizerCollection>* collections, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    QList<QOrganizerCollection> collections(QOrganizerManager::Error* error);
    void removeCollections(const QMap<int, QOrganizerCollectionId>& collectionIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);

    void createDefaultCollection(QOrganizerCollection* defaultCollection, QOrganizerManager::Error* error);
    QOrganizerCollection defaultCollection();
    QSet<QOrganizerCollectionId> collectionIds();

signals:
    void threadInitialized();
    void requestInitialized();

protected:
    virtual void run();

private slots:
    void handleRequest();
    void onNotified(const QString& notifyUuid, const QVariant& object, const QString& action);
    void onResponse(int trId, const QVariant& object);
    void onError(int trId, int errorCode, const QString& message);

private:

    enum RequestType {
        Invalid,
        SaveItems,
        Items,
        ItemsById,
        RemoveItems,
        RemoveItemsByCollectionId,
        SaveCollections,
        Collections,
        RemoveCollections
    };

    void handleResponse(int trId, QOrganizerManager::Error error, const QVariant& object);
    void handleSaveItemsRequest();
    void handleSaveItemsResponse(int index, QOrganizerManager::Error error, const QVariant& object);
    void handleItemsRequest();
    void handleItemsResponse(QOrganizerManager::Error error, const QVariant& object);
    void handleItemsByIdRequest();
    void handleItemsByIdResponse(QOrganizerManager::Error error, const QVariant& object);
    void handleRemoveItemsRequest();
    void handleRemoveItemsResponse(int index, QOrganizerManager::Error error);
    void handleRemoveItemsByCollectionIdRequest();
    void handleRemoveItemsByCollectionIdResponse(const QVariant& object);
    void handleSaveCollectionsRequest();
    void handleSaveCollectionsResponse(int index, QOrganizerManager::Error error, const QVariant& object);
    void handleCollectionsRequest();
    void handleCollectionsResponse(QOrganizerManager::Error error, const QVariant& object);
    void handleRemoveCollectionsRequest();
    void handleRemoveCollectionsResponse(QOrganizerManager::Error error, const QVariant& object);

    QOrganizerManager::Error handleErrorResponse(const QVariant& object, int errorCode);

    void processRequest();
    void initRequestData(RequestType requestType, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    void clearRequestData();

    // This mutex is used for protecting request data in situations where jsondb is modified by someone
    // else when a request is being processed in this class. Modification causes onNotified slot to be called
    // and it can happen while the one of the public methods is waiting for a request to complete
    // TODO: implement notification system and take the mutex into use
    //QMutex* m_operationMutex;
    QMutex* m_waitMutex;
    QWaitCondition m_syncWaitCondition;

    QOrganizerJsonDbConverter m_converter;
    JsonDbClient* m_jsonDb;
    JsonDbConnection* m_jsonConnection;

    // "collection cache"
    QSet<QOrganizerCollectionId> m_collectionIds;
    QOrganizerCollection m_defaultCollection;

    // request data

    // common
    RequestType m_requestType;
    QMap<int, int> m_transactionIds; // map from transaction id to item/collection index
    QMap<int, QOrganizerManager::Error>* m_errorMap;
    QOrganizerManager::Error* m_error;

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

    // RemoveItems (itemsById)
    QList<QOrganizerItemId> m_itemIds;

    // RemoveItemsByCollectionId
    QList<QOrganizerCollectionId> m_removeItemCollectionIds;

    // SaveCollection
    QMap<int, QOrganizerCollection>* m_resultCollections; // map from collection index to collection
    bool m_isDefaultCollection;

    // Collections
    QList<QOrganizerCollection> m_collections;

    // RemoveCollections
    QMap<int, QOrganizerCollectionId> m_removeCollectionIds;
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBDATASTORAGE_H
