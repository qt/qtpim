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

#include <QEventLoop>
#include <QAbstractSocket>

#include "qorganizerjsondbengine.h"
#include "qorganizerjsondbstring.h"
#include "qorganizer.h"

QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbEngine *QOrganizerJsonDbEngine::createJsonDbEngine(QOrganizerManager::Error* error)
{
    QOrganizerJsonDbEngine* engine = new QOrganizerJsonDbEngine();
    if (engine)
        engine->initEngine(error);

    return engine;
}

QOrganizerJsonDbEngine::QOrganizerJsonDbEngine()
    :d(0)
{

}

QOrganizerJsonDbEngine::~QOrganizerJsonDbEngine()
{
    if (d) {
        d->m_requestHandlerThread->exit();
        d->m_requestHandlerThread->wait();
        delete d;
        d = 0;
    }
}

bool QOrganizerJsonDbEngine::initEngine (QOrganizerManager::Error* error)
{
    bool ret = false;
    d = new QOrganizerJsonDbEngineData;
    if (d) {
        d->m_requestHandlerThread = new QOrganizerJsonDbRequestThread();
        if (d->m_requestHandlerThread) {
            // Register types to be able to send them as parameters in cross-thread signals
            qRegisterMetaType<QOrganizerAbstractRequest::State>("QOrganizerAbstractRequest::State");
            qRegisterMetaType<QList<QOrganizerItemId> >("QList<QOrganizerItemId>");
            qRegisterMetaType<QList<QOrganizerCollectionId> >("QList<QOrganizerCollectionId>");
            qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
            qRegisterMetaType<QOrganizerItemId>("QOrganizerItemId");
            qRegisterMetaType<QOrganizerCollectionId>("QOrganizerCollectionId");
            d->m_requestHandlerThread->moveToThread (d->m_requestHandlerThread);
            // making sure that thread is started before exiting this function
            QEventLoop loop;
            connect(d->m_requestHandlerThread, SIGNAL(initialized()), &loop, SLOT(quit()));
            d->m_requestHandlerThread->start();
            loop.exec();
            connect(this, SIGNAL(requestReceived(QOrganizerAbstractRequest*)),
                    d->m_requestHandlerThread, SLOT(handleRequest(QOrganizerAbstractRequest*)));
            d->m_requestHandlerThread->setEngine(this);
            ret = true;
       } else {
           *error = QOrganizerManager::OutOfMemoryError;
       }
    } else {
       *error = QOrganizerManager::OutOfMemoryError;
    }
    return ret;
}

QString QOrganizerJsonDbEngine::managerName() const
{
    return QOrganizerJsonDbStr::jsonDbEngineId();
}

QMap<QString, QString> QOrganizerJsonDbEngine::managerParameters() const
{
    /* TODO - in case you have any actual parameters that are relevant that you saved in the factory method, return them here */
    return QMap<QString, QString>();
}

int QOrganizerJsonDbEngine::managerVersion() const
{
    /* TODO - implement this appropriately.  This is strictly defined by the engine, so return whatever you like */
    return 1;
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::itemOccurrences(const QOrganizerItem &parentItem,
                                                              const QDateTime &startDateTime,
                                                              const QDateTime &endDateTime, int maxCount,
                                                              const QOrganizerItemFetchHint &fetchHint,
                                                              QOrganizerManager::Error *error)
{
    QOrganizerItemOccurrenceFetchRequest request;
    request.setParentItem(parentItem);
    request.setStartDate(startDateTime);
    request.setEndDate(endDateTime);
    request.setMaxOccurrences(maxCount);
    request.setFetchHint(fetchHint);
    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0))
            *error = request.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return request.itemOccurrences();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

QList<QOrganizerItemId> QOrganizerJsonDbEngine::itemIds(const QOrganizerItemFilter &filter,
                                                        const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                        const QList<QOrganizerItemSortOrder> &sortOrders,
                                                        QOrganizerManager::Error *error)
{
    QOrganizerItemIdFetchRequest idFetchReq;
    idFetchReq.setStartDate(startDateTime);
    idFetchReq.setEndDate(endDateTime);
    idFetchReq.setFilter(filter);
    idFetchReq.setSorting(sortOrders);
    if (startRequest(&idFetchReq)) {
        if (waitForRequestFinished(&idFetchReq, 0))
            *error = idFetchReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return idFetchReq.itemIds();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItemId>();
    }
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                                          const QDateTime &endDateTime, int maxCount,
                                                          const QList<QOrganizerItemSortOrder> &sortOrders,
                                                          const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
{
    QOrganizerItemFetchRequest request;
    request.setFilter(filter);
    request.setStartDate(startDateTime);
    request.setEndDate(endDateTime);
    request.setMaxCount(maxCount);
    request.setSorting(sortOrders);
    request.setFetchHint(fetchHint);
    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0))
            *error = request.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return request.items();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::itemsForExport(const QDateTime &startDateTime,
                                                             const QDateTime &endDateTime,
                                                             const QOrganizerItemFilter &filter,
                                                             const QList<QOrganizerItemSortOrder> &sortOrders,
                                                             const QOrganizerItemFetchHint &fetchHint,
                                                             QOrganizerManager::Error *error)
{
    QOrganizerItemFetchForExportRequest request;
    request.setStartDate(startDateTime);
    request.setEndDate(endDateTime);
    request.setFilter(filter);
    request.setSorting(sortOrders);
    request.setFetchHint(fetchHint);
    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0))
            *error = request.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return request.items();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                                    QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    QOrganizerItemFetchByIdRequest fetchByIdReq;
    fetchByIdReq.setIds(itemIds);
    fetchByIdReq.setFetchHint(fetchHint);
    if (startRequest(&fetchByIdReq)) {
        if (waitForRequestFinished(&fetchByIdReq, 0)) {
            *error = fetchByIdReq.error();
            *errorMap = fetchByIdReq.errorMap();
        } else {
            *error = QOrganizerManager::TimeoutError;
        }
        return fetchByIdReq.items();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

bool QOrganizerJsonDbEngine::saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                                       QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    QOrganizerItemSaveRequest request;
    request.setItems(*items);
    request.setDetailMask(detailMask);
    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0)) {
            *error = request.error();
            *errorMap = request.errorMap();
        } else {
            *error = QOrganizerManager::TimeoutError;
        }
        *items = request.items();
        return (*error == QOrganizerManager::NoError);
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

bool QOrganizerJsonDbEngine::removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                                         QOrganizerManager::Error *error)
{
    QOrganizerItemRemoveRequest removeReq;
    removeReq.setItemIds(itemIds);
    if (startRequest(&removeReq)) {
        if (waitForRequestFinished(&removeReq, 0)) {
            *errorMap = removeReq.errorMap();
            *error = removeReq.error();
        } else {
            *error = QOrganizerManager::TimeoutError;
        }
        return (*error == QOrganizerManager::NoError);
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

QOrganizerCollection QOrganizerJsonDbEngine::defaultCollection(QOrganizerManager::Error* error)
{
    *error = QOrganizerManager::NoError;
    return d->m_requestHandlerThread->defaultCollection();
}

QOrganizerCollection QOrganizerJsonDbEngine::collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    *error = QOrganizerManager::NoError;
    QList<QOrganizerCollection> collectionList = collections(error);
    for (int i = 0; i < collectionList.size(); i++) {
        if (collectionList[i].id() == collectionId)
            return collectionList[i];
    }
    *error = QOrganizerManager::DoesNotExistError;
    return QOrganizerCollection();
}

QList<QOrganizerCollection> QOrganizerJsonDbEngine::collections(QOrganizerManager::Error* error)
{
    QOrganizerCollectionFetchRequest collectionFetchReq;
    if (startRequest(&collectionFetchReq)) {
        if (waitForRequestFinished(&collectionFetchReq, 0))
            *error = collectionFetchReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return collectionFetchReq.collections();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerCollection>();
    }
}

bool QOrganizerJsonDbEngine::saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error)
{
    QOrganizerCollectionSaveRequest collectionSaveReq;
    collectionSaveReq.setCollection(*collection);
    if (startRequest(&collectionSaveReq)) {
        if (waitForRequestFinished(&collectionSaveReq, 0))
            *error = collectionSaveReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        *collection = collectionSaveReq.collections().at(0);
        return *error == QOrganizerManager::NoError;
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

bool QOrganizerJsonDbEngine::removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    //Check if it is the default collection
    // TODO: move this check to request handler thread
    if (defaultCollection (error).id() == collectionId) {
        *error = QOrganizerManager::PermissionsError;
        return false;
    }

    QOrganizerCollectionRemoveRequest collectionRemoveReq;
    collectionRemoveReq.setCollectionId(collectionId);
    if (startRequest(&collectionRemoveReq)) {
        if (waitForRequestFinished(&collectionRemoveReq, 0))
            *error = collectionRemoveReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return *error == QOrganizerManager::NoError;
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

bool QOrganizerJsonDbEngine::startRequest(QOrganizerAbstractRequest* req)
{
    QList<QOrganizerItem> itemList;
    QList<QOrganizerItemId> idList;
    QList<QOrganizerCollection> collectionList;
    QOrganizerManager::Error error = QOrganizerManager::NoError;
    QMap<int, QOrganizerManager::Error> errorMap;
    d->m_requestHandlerThread->addRequest(req);

    switch (req->type()) {
    case QOrganizerAbstractRequest::ItemFetchRequest: {
        QOrganizerManagerEngine::updateItemFetchRequest(static_cast<QOrganizerItemFetchRequest*>(req),
                                                        itemList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemIdFetchRequest: {
        QOrganizerManagerEngine::updateItemIdFetchRequest(static_cast<QOrganizerItemIdFetchRequest*>(req),
                                                          idList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchByIdRequest: {
        QOrganizerManagerEngine:: updateItemFetchByIdRequest(static_cast<QOrganizerItemFetchByIdRequest*>(req),
                                                              itemList, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemSaveRequest: {
        QOrganizerItemSaveRequest* saveReq = static_cast<QOrganizerItemSaveRequest*>(req);
        itemList = saveReq->items();
        QOrganizerManagerEngine::updateItemSaveRequest(saveReq, itemList, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemRemoveRequest: {
        QOrganizerManagerEngine::updateItemRemoveRequest(static_cast<QOrganizerItemRemoveRequest*>(req),
                                                         error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::CollectionFetchRequest: {
        QOrganizerManagerEngine::updateCollectionFetchRequest(static_cast<QOrganizerCollectionFetchRequest*>(req),
                                                              collectionList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::CollectionSaveRequest: {
        QOrganizerCollectionSaveRequest* collectionSaveReq = static_cast<QOrganizerCollectionSaveRequest*>(req);
        collectionList = collectionSaveReq->collections();
        QOrganizerManagerEngine::updateCollectionSaveRequest(collectionSaveReq, collectionList, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::CollectionRemoveRequest: {
        QOrganizerCollectionRemoveRequest* collectionRemoveReq = static_cast<QOrganizerCollectionRemoveRequest*>(req);
        QOrganizerManagerEngine::updateCollectionRemoveRequest(collectionRemoveReq, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    default:
        return false;
    }

    emit requestReceived(req);
    return true;
}

bool QOrganizerJsonDbEngine::cancelRequest(QOrganizerAbstractRequest* req)
{
    /*
        TODO

        Cancel an in progress async request.  If not possible, return false from here.
    */
    return QOrganizerManagerEngine::cancelRequest(req);
}

bool QOrganizerJsonDbEngine::waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs)
{
    return d->m_requestHandlerThread->waitForRequestFinished(req, msecs);
}

void QOrganizerJsonDbEngine::requestDestroyed(QOrganizerAbstractRequest* req)
{
    d->m_requestHandlerThread->requestDestroyed(req);
}

QList<QOrganizerItemFilter::FilterType> QOrganizerJsonDbEngine::supportedFilters() const
{
    QList<QOrganizerItemFilter::FilterType> supported;

    supported << QOrganizerItemFilter::InvalidFilter
              << QOrganizerItemFilter::DetailFilter
              << QOrganizerItemFilter::IntersectionFilter
              << QOrganizerItemFilter::UnionFilter
              << QOrganizerItemFilter::IdFilter
              << QOrganizerItemFilter::CollectionFilter
              << QOrganizerItemFilter::DefaultFilter;

    return supported;
}

QList<QOrganizerItemDetail::DetailType> QOrganizerJsonDbEngine::supportedItemDetails(QOrganizerItemType::ItemType itemType) const
{
    QList<QOrganizerItemDetail::DetailType> supportedDetails;
    supportedDetails << QOrganizerItemType::DefinitionName
                     << QOrganizerItemGuid::DefinitionName
//                     << QOrganizerItemTimestamp::DefinitionName
                     << QOrganizerItemDisplayLabel::DefinitionName
                     << QOrganizerItemDescription::DefinitionName
                     << QOrganizerItemComment::DefinitionName
                     << QOrganizerItemTag::DefinitionName
                     << QOrganizerItemExtendedDetail::DefinitionName
                     << QOrganizerItemVersion::DefinitionName;

    if (itemType == QOrganizerItemType::TypeEvent) {
        supportedDetails << QOrganizerItemRecurrence::DefinitionName
                         << QOrganizerEventTime::DefinitionName
                         << QOrganizerItemPriority::DefinitionName
                         << QOrganizerItemLocation::DefinitionName
                         << QOrganizerItemReminder::DefinitionName
                         << QOrganizerItemAudibleReminder::DefinitionName
                         << QOrganizerEventAttendee::DefinitionName
                         << QOrganizerEventRsvp::DefinitionName;
    } else if (itemType == QOrganizerItemType::TypeTodo) {
        supportedDetails << QOrganizerItemRecurrence::DefinitionName
                         << QOrganizerTodoTime::DefinitionName
                         << QOrganizerItemPriority::DefinitionName
                         << QOrganizerTodoProgress::DefinitionName
                         << QOrganizerItemReminder::DefinitionName
                         << QOrganizerItemAudibleReminder::DefinitionName;
    } else if (itemType == QOrganizerItemType::TypeEventOccurrence) {
        supportedDetails << QOrganizerItemParent::DefinitionName
                         << QOrganizerEventTime::DefinitionName
                         << QOrganizerItemPriority::DefinitionName
                         << QOrganizerItemLocation::DefinitionName
                         << QOrganizerItemReminder::DefinitionName
                         << QOrganizerItemAudibleReminder::DefinitionName
                         << QOrganizerEventAttendee::DefinitionName
                         << QOrganizerEventRsvp::DefinitionName;
    } else if (itemType == QOrganizerItemType::TypeTodoOccurrence) {
        supportedDetails << QOrganizerItemParent::DefinitionName
                         << QOrganizerTodoTime::DefinitionName
                         << QOrganizerItemPriority::DefinitionName
                         << QOrganizerTodoProgress::DefinitionName
                         << QOrganizerItemReminder::DefinitionName
                         << QOrganizerItemAudibleReminder::DefinitionName;
    } else {
        // We don't support Journal and Note, yet ;)
        supportedDetails.clear();
    }

    return supportedDetails;
}

QList<QOrganizerItemType::ItemType> QOrganizerJsonDbEngine::supportedItemTypes() const
{
    QList<QOrganizerItemType::ItemType> ret;

    ret << QOrganizerItemType::TypeEvent;
//    ret << QOrganizerItemType::TypeEventOccurrence;
//    ret << QOrganizerItemType::TypeJournal;
//    ret << QOrganizerItemType::TypeNote;
    ret << QOrganizerItemType::TypeTodo;
//    ret << QOrganizerItemType::TypeTodoOccurrence;

    return  ret;
}

#include "moc_qorganizerjsondbengine.cpp"

QTORGANIZER_END_NAMESPACE

Q_DECLARE_METATYPE(QTORGANIZER_PREPEND_NAMESPACE(QOrganizerAbstractRequest::State))
Q_DECLARE_METATYPE(QList<QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemId)>)
