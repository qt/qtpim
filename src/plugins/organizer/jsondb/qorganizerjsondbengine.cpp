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

#include <QEventLoop>
#include <QAbstractSocket>

#include "qorganizerjsondbengine.h"
#include "qorganizerjsondbstring_p.h"
#include "qorganizer.h"

//QTM_USE_NAMESPACE

QOrganizerJsonDbEngine *QOrganizerJsonDbEngine::createJsonDbEngine(QOrganizerManager::Error* error)
{
    QOrganizerJsonDbEngine* engine = new QOrganizerJsonDbEngine ();
    if (engine) {
        if (engine->initEngine (error))
            engine->initCollection (error);
    }
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

void QOrganizerJsonDbEngine::initCollection(QOrganizerManager::Error* error)
{
    //Fetch the collection list
    QOrganizerCollectionFetchRequest collectionFetchReq;
    *error = QOrganizerManager::NoError;
    const_cast<QOrganizerJsonDbEngine*>(this)->startRequest(&collectionFetchReq);

    if (const_cast<QOrganizerJsonDbEngine*>(this)->waitForRequestFinished(&collectionFetchReq, 0)) {
        *error = collectionFetchReq.error();
        //Check if we get default collection
        if (defaultCollection(error).id().isNull()) {
            //Create new default collection
            QOrganizerCollectionSaveRequest collectionSaveReq;
            QOrganizerCollection defaultCollection;
            defaultCollection.setMetaData(QOrganizerCollection::KeyName, QOrganizerJsonDbStr::DefaultCollectionName);
            collectionSaveReq.setCollection(defaultCollection);
            const_cast<QOrganizerJsonDbEngine*>(this)->startRequest(&collectionSaveReq);
            if (const_cast<QOrganizerJsonDbEngine*>(this)->waitForRequestFinished(&collectionSaveReq, 0)) {
                *error = collectionSaveReq.error();
                if (*error == QOrganizerManager::NoError)
                    d->m_requestHandlerThread->setDefaultCollection(collectionSaveReq.collections().at(0));
            }
        }
    }
}

QString QOrganizerJsonDbEngine::managerName() const
{
    return QOrganizerJsonDbStr::JsonDbEngineId;
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

QList<QOrganizerItem> QOrganizerJsonDbEngine::itemOccurrences(const QOrganizerItem& parentItem, const QDateTime& periodStart, const QDateTime& periodEnd, int maxCount, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const
{
    /*
        TODO

        This function should create a list of occurrences that occur in the time period from the supplied item,
        generated from the parent item.

        The periodStart should always be valid, and either the periodEnd or the maxCount will be valid (if periodEnd is
        valid, use that.  Otherwise use the count).  It's permissible to limit the number of items returned...

        Basically, if the parent item is an Event, a list of EventOccurrences should be returned.  Similarly for
        Todo/TodoOccurrence.

        If there are no instances, return an empty list.

        The returned items should have a QOrganizerItemParent detail that points to the parentItem and the
        original instance that the event would have occurred on (e.g. with an exception).

        They should not have recurrence information details in them.

        We might change the signature to split up the periodStart + periodEnd / periodStart + maxCount cases.
    */

    return QOrganizerManagerEngine::itemOccurrences(parentItem, periodStart, periodEnd, maxCount, fetchHint, error);
}

QList<QOrganizerItemId> QOrganizerJsonDbEngine::itemIds(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, QOrganizerManager::Error* error) const
{
    QOrganizerItemIdFetchRequest idFetchReq;
    idFetchReq.setStartDate(startDate);
    idFetchReq.setEndDate(endDate);
    idFetchReq.setFilter(filter);
    idFetchReq.setSorting(sortOrders);
    *error = QOrganizerManager::NoError;

    const_cast<QOrganizerJsonDbEngine*>(this)->startRequest(&idFetchReq);
    if (const_cast<QOrganizerJsonDbEngine*>(this)->waitForRequestFinished(&idFetchReq, 0)) {
        *error = idFetchReq.error();
        return idFetchReq.itemIds();
    }

    return QList<QOrganizerItemId>();
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::items(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const
{
    /*
        TODO

        Given the supplied filter and sort order, fetch the list of items [not instances] that correspond, and return them.

        If you don't support the filter or sort orders, you can fetch a partially (or un-) filtered list and ask the helper
        functions to filter and sort it for you.

        The fetch hint suggests how much of the item to fetch.  You can ignore the fetch hint and fetch everything (but you must
        fetch at least what is mentioned in the fetch hint).
    */

//    Q_UNUSED(fetchHint);
//    *error = QOrganizerManager::NotSupportedError; // TODO <- remove this

//    QList<QOrganizerItem> partiallyFilteredItems; // = ..., your code here.. [TODO]
//    QList<QOrganizerItem> ret;

//    foreach(const QOrganizerItem& item, partiallyFilteredItems) {
//        if (QOrganizerManagerEngine::isItemBetweenDates(item, startDate, endDate) && QOrganizerManagerEngine::testFilter(filter, item)) {
//            QOrganizerManagerEngine::addSorted(&ret, item, sortOrders);
//        }
//    }

//    /* An alternative formulation, depending on how your engine is implemented is just:

//        foreach(const QOrganizerItemId& id, itemIds(filter, sortOrders, error)) {
//            ret.append(item(id, fetchHint, error);
//        }
//     */

//    return ret;

    QOrganizerItemFetchRequest fetchReq;
    fetchReq.setStartDate(startDate);
    fetchReq.setEndDate(endDate);
    fetchReq.setFilter(filter);
    fetchReq.setSorting(sortOrders);
    fetchReq.setFetchHint(fetchHint);
    *error = QOrganizerManager::NoError;

    const_cast<QOrganizerJsonDbEngine*>(this)->startRequest(&fetchReq);
    if (const_cast<QOrganizerJsonDbEngine*>(this)->waitForRequestFinished(&fetchReq, 0)) {
        *error = fetchReq.error();
        return fetchReq.items();
    }
    return QList<QOrganizerItem>();


}

QOrganizerItem QOrganizerJsonDbEngine::item(const QOrganizerItemId& itemId, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const
{
    /*
        TODO

        Fetch a single item by id.

        The fetch hint suggests how much of the item to fetch.  You can ignore the fetch hint and fetch everything (but you must
        fetch at least what is mentioned in the fetch hint).

    */

    QOrganizerItemFetchByIdRequest fetchByIdReq;
    QList<QOrganizerItemId> ids;
    ids.append(itemId);
    fetchByIdReq.setIds(ids);
    fetchByIdReq.setFetchHint(fetchHint);
    *error = QOrganizerManager::NoError;

    const_cast<QOrganizerJsonDbEngine*>(this)->startRequest(&fetchByIdReq);
    if (const_cast<QOrganizerJsonDbEngine*>(this)->waitForRequestFinished(&fetchByIdReq, 0))
        *error = fetchByIdReq.error();

    if (fetchByIdReq.items().isEmpty())
        return QOrganizerItem();
    else
        return fetchByIdReq.items().at(0);
}


bool QOrganizerJsonDbEngine::saveItems(QList<QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    QOrganizerItemSaveRequest saveReq;
    saveReq.setItems(*items);
    startRequest(&saveReq);
    if (waitForRequestFinished(&saveReq, 0)) {

        for (int i = 0; i < saveReq.items().size(); i++)
            items->replace(i, saveReq.items()[i]);

        *errorMap = saveReq.errorMap();
        *error = saveReq.error();
    }
    return *error == QOrganizerManager::NoError;
}

bool QOrganizerJsonDbEngine::removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    QOrganizerItemRemoveRequest removeReq;
    removeReq.setItemIds(itemIds);
    startRequest(&removeReq);

    if (waitForRequestFinished(&removeReq, 0)) {
        //DoesNotExistError should be used if the id refers to a non existent item.
        *errorMap = removeReq.errorMap();
        *error = removeReq.error();
    }
    return *error == QOrganizerManager::NoError;
}

QMap<QString, QOrganizerItemDetailDefinition> QOrganizerJsonDbEngine::detailDefinitions(const QString& itemType, QOrganizerManager::Error* error) const
{
    /* TODO - once you know what your engine will support, implement this properly.  One way is to call the base version, and add/remove things as needed */
    *error = QOrganizerManager::NoError;
    return schemaDefinitions().value(itemType);
}

QOrganizerItemDetailDefinition QOrganizerJsonDbEngine::detailDefinition(const QString& definitionId, const QString& itemType, QOrganizerManager::Error* error) const
{
    /* TODO - the default implementation just calls the base detailDefinitions function.  If that's inefficent, implement this */
    return QOrganizerManagerEngine::detailDefinition(definitionId, itemType, error);
}

bool QOrganizerJsonDbEngine::saveDetailDefinition(const QOrganizerItemDetailDefinition& def, const QString& itemType, QOrganizerManager::Error* error)
{
    /* TODO - if you support adding custom fields, do that here.  Otherwise call the base functionality. */
    return QOrganizerManagerEngine::saveDetailDefinition(def, itemType, error);
}

bool QOrganizerJsonDbEngine::removeDetailDefinition(const QString& definitionId, const QString& itemType, QOrganizerManager::Error* error)
{
    /* TODO - if you support removing custom fields, do that here.  Otherwise call the base functionality. */
    return QOrganizerManagerEngine::removeDetailDefinition(definitionId, itemType, error);
}


QOrganizerCollection QOrganizerJsonDbEngine::defaultCollection(QOrganizerManager::Error* error) const
{
    *error = QOrganizerManager::NoError;
    return d->m_requestHandlerThread->defaultCollection();
}

QOrganizerCollection QOrganizerJsonDbEngine::collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error) const
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

QList<QOrganizerCollection> QOrganizerJsonDbEngine::collections(QOrganizerManager::Error* error) const
{
    QOrganizerCollectionFetchRequest collectionFetchReq;
    *error = QOrganizerManager::NoError;

    const_cast<QOrganizerJsonDbEngine*>(this)->startRequest(&collectionFetchReq);
    if (const_cast<QOrganizerJsonDbEngine*>(this)->waitForRequestFinished(&collectionFetchReq, 0)) {
        *error = collectionFetchReq.error();
        return collectionFetchReq.collections();
    }
    return QList<QOrganizerCollection>();
}

bool QOrganizerJsonDbEngine::saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error)
{
    QOrganizerCollectionSaveRequest collectionSaveReq;
    collectionSaveReq.setCollection(*collection);
    startRequest(&collectionSaveReq);
    if (waitForRequestFinished(&collectionSaveReq, 0)) {
        // take the only collection
        *collection = collectionSaveReq.collections().at(0);
        *error = collectionSaveReq.error();
    }
    return *error == QOrganizerManager::NoError;
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
    startRequest(&collectionRemoveReq);
    if (waitForRequestFinished(&collectionRemoveReq, 0)) {
        *error = collectionRemoveReq.error();
    }
    return *error == QOrganizerManager::NoError;
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
        QOrganizerManagerEngineV2:: updateItemFetchByIdRequest(static_cast<QOrganizerItemFetchByIdRequest*>(req),
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
        break;
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

bool QOrganizerJsonDbEngine::hasFeature(QOrganizerManager::ManagerFeature feature, const QString& itemType) const
{
    // TODO - the answer to the question may depend on the type
    Q_UNUSED(itemType);
    switch(feature) {
        case QOrganizerManager::MutableDefinitions:
            // TODO If you support save/remove detail definition, return true
            return false;

        case QOrganizerManager::Anonymous:
            // TODO if this engine is anonymous (e.g. no other engine can share the data) return true
            // (mostly for an in memory engine)
            return false;
        case QOrganizerManager::ChangeLogs:
            // TODO if this engine supports filtering by last modified/created/removed timestamps, return true
            return false;
    }
    return false;
}

bool QOrganizerJsonDbEngine::isFilterSupported(const QOrganizerItemFilter& filter) const
{
    // TODO if you engine can nativelconst QString& notifyUuid, const QVariant& object, const QString& actiony support the filter, return true.  Otherwise you should emulate support in the item{Ids} functions.
    Q_UNUSED(filter);
    return false;
}

QList<int> QOrganizerJsonDbEngine::supportedDataTypes() const
{
    QList<int> ret;
    // TODO - tweak which data types this engine understands
    ret << QVariant::String;
    ret << QVariant::Date;
    ret << QVariant::DateTime;
    ret << QVariant::Time;

    return ret;
}

QStringList QOrganizerJsonDbEngine::supportedItemTypes() const
{
    // TODO - return which [predefined] types this engine supports
    QStringList ret;

    ret << QOrganizerItemType::TypeEvent;
//    ret << QOrganizerItemType::TypeEventOccurrence;
//    ret << QOrganizerItemType::TypeJournal;
//    ret << QOrganizerItemType::TypeNote;
    ret << QOrganizerItemType::TypeTodo;
//    ret << QOrganizerItemType::TypeTodoOccurrence;

    return  ret;
}


Q_DECLARE_METATYPE(QOrganizerAbstractRequest::State)
Q_DECLARE_METATYPE(QList<QOrganizerItemId>)


