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

#include <QCoreApplication>

#include "qorganizerjsondbrequestthread.h"
#include "qorganizerjsondbid.h"
#include "qorganizerjsondbengine.h"
#include "qorganizerjsondbrequestmanager.h"
#include "qorganizerjsondbdatastorage.h"
#include "qorganizerjsondbstring.h"
#include "qorganizer.h"

#include <QtCore/qdebug.h>
#include <QtCore/qtimer.h>
#include <QtCore/quuid.h>

QTORGANIZER_BEGIN_NAMESPACE

const int QOrganizerJsonDbRequestThread::TIMEOUT_INTERVAL(100);
const int QOrganizerJsonDbRequestThread::ALARM_REMOVE_MAXLOOP(10);
const int QOrganizerJsonDbRequestThread::DefaultTimePeriod(1461);
const int QOrganizerJsonDbRequestThread::MaxOccurrenceCount(50);

QOrganizerJsonDbRequestThread::QOrganizerJsonDbRequestThread()
    : m_engine(0)
    , m_storage(0)
    , m_requestMgr(0)
    , m_reqStateMutex(0)
    , m_timer(0)
{
}

QOrganizerJsonDbRequestThread::~QOrganizerJsonDbRequestThread()
{
    delete m_reqStateMutex;
    m_reqStateMutex = 0;
    delete m_requestMgr;
    m_requestMgr = 0;
    m_storage->exit();
    m_storage->wait();
    delete m_storage;
    m_storage = 0;
}

void QOrganizerJsonDbRequestThread::run()
{
    m_reqStateMutex = new QMutex();
    m_requestMgr = new QOrganizerJsonDbRequestManager();
    m_storage = new QOrganizerJsonDbDataStorage();
    m_storage->moveToThread(m_storage);
    QEventLoop loop;
    connect(m_storage, SIGNAL(threadInitialized()), &loop, SLOT(quit()));
    m_storage->start();
    loop.exec();

    connect(m_storage, SIGNAL(itemAdded(QOrganizerItemId)), this, SLOT(onItemAdded(QOrganizerItemId)));
    connect(m_storage, SIGNAL(itemChanged(QOrganizerItemId)), this, SLOT(onItemChanged(QOrganizerItemId)));
    connect(m_storage, SIGNAL(itemRemoved(QOrganizerItemId)), this, SLOT(onItemRemoved(QOrganizerItemId)));
    connect(m_storage, SIGNAL(collectionAdded(QOrganizerCollectionId)), this, SLOT(onCollectionAdded(QOrganizerCollectionId)));
    connect(m_storage, SIGNAL(collectionChanged(QOrganizerCollectionId)), this, SLOT(onCollectionChanged(QOrganizerCollectionId)));
    connect(m_storage, SIGNAL(collectionRemoved(QOrganizerCollectionId)), this, SLOT(onCollectionRemoved(QOrganizerCollectionId)));

    initDefaultCollection();

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
    QMutexLocker locker(m_reqStateMutex);
    return m_storage->defaultCollection();
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
        handleItemSaveRequest(static_cast<QOrganizerItemSaveRequest*>(req));
        break;

    case QOrganizerAbstractRequest::ItemFetchRequest:
        handleItemFetchRequest(static_cast<QOrganizerItemFetchRequest*>(req));
        break;

    case QOrganizerAbstractRequest::ItemIdFetchRequest:
        handleItemIdFetchRequest(static_cast<QOrganizerItemIdFetchRequest*>(req));
        break;

    case QOrganizerAbstractRequest::ItemFetchByIdRequest:
        handleItemFetchByIdRequest(static_cast<QOrganizerItemFetchByIdRequest*>(req));
        break;

    case QOrganizerAbstractRequest::ItemFetchForExportRequest:
        handleItemFetchForExportRequest(static_cast<QOrganizerItemFetchForExportRequest*>(req));
        break;

    case QOrganizerAbstractRequest::ItemOccurrenceFetchRequest:
        handleItemOccurrenceFetchRequest(static_cast<QOrganizerItemOccurrenceFetchRequest*>(req));
        break;

    case QOrganizerAbstractRequest::ItemRemoveRequest:
        handleItemRemoveRequest(static_cast <QOrganizerItemRemoveRequest *> (req));
        break;

    case QOrganizerAbstractRequest::ItemRemoveByIdRequest:
        handleItemRemoveByIdRequest(static_cast <QOrganizerItemRemoveByIdRequest *> (req));
        break;

    case QOrganizerAbstractRequest::CollectionSaveRequest:
        handleCollectionSaveRequest(static_cast <QOrganizerCollectionSaveRequest *> (req));
        break;

    case QOrganizerAbstractRequest::CollectionFetchRequest:
        handleCollectionFetchRequest(static_cast<QOrganizerCollectionFetchRequest*>(req));
        break;

    case QOrganizerAbstractRequest::CollectionRemoveRequest:
        handleCollectionRemoveRequest(static_cast<QOrganizerCollectionRemoveRequest*>(req));
        break;

    default:
        break;
    }
}

void QOrganizerJsonDbRequestThread::onItemAdded(const QOrganizerItemId &itemId)
{
    m_ics.insertAddedItem(itemId);
    startTimer();
}

void QOrganizerJsonDbRequestThread::onItemChanged(const QOrganizerItemId &itemId)
{
    m_ics.insertChangedItem(itemId);
    startTimer();
}

void QOrganizerJsonDbRequestThread::onItemRemoved(const QOrganizerItemId &itemId)
{
    m_ics.insertRemovedItem(itemId);
    startTimer();
}

void QOrganizerJsonDbRequestThread::onCollectionAdded(const QOrganizerCollectionId &collectionId)
{
    m_ccs.insertAddedCollection(collectionId);
    startTimer();
}

void QOrganizerJsonDbRequestThread::onCollectionChanged(const QOrganizerCollectionId &collectionId)
{
    m_ccs.insertChangedCollection(collectionId);
    startTimer();
}

void QOrganizerJsonDbRequestThread::onCollectionRemoved(const QOrganizerCollectionId &collectionId)
{
    m_ccs.insertRemovedCollection(collectionId);
    startTimer();
}

void QOrganizerJsonDbRequestThread::startTimer()
{
    if (!m_timer) {
        m_timer = new QTimer(this);
        m_timer->setSingleShot(true);
        m_timer->setInterval(TIMEOUT_INTERVAL);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    }

    m_timer->start();
}

void QOrganizerJsonDbRequestThread::onTimeout()
{
    if (m_engine) {
        m_ics.emitSignals(m_engine);
        m_ics.clearAll();
        m_ccs.emitSignals(m_engine);
        m_ccs.clearAll();
    }
}

void QOrganizerJsonDbRequestThread::handleItemSaveRequest(QOrganizerItemSaveRequest* saveReq)
{
    QMap<int, QOrganizerItem> itemMap;
    QMap<int, bool> itemIsNewStatusMap;
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    QList<QOrganizerItem> items = saveReq->items();
    QMap<int, QOrganizerItem> parentItemMap;
    QMap<int, QOrganizerManager::Error> parentErrorMap;
    QOrganizerManager::Error parentError = QOrganizerManager::NoError;

    m_requestMgr->setActive(saveReq);

    for (int i = 0; i < items.size(); i++) {
        QOrganizerItem item = items.at(i);
        QOrganizerItem parentItem;
        bool itemIsNew = item.id().isNull();
        bool itemIsOccurrence = (item.type() == QOrganizerItemType::TypeEventOccurrence || item.type() == QOrganizerItemType::TypeTodoOccurrence);
        bool errorFound = false;

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!itemIsNew && (managerUri != item.id().managerUri())) {
            latestError = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }

        // TODO: to be replaced by new validity check, collection id and guid should not be mandatory fields
        //       this checks e.g. that occurrences have parent ids and original dates?
        //        // ensure that the organizeritem's details conform to their definitions
        //        if (!errorFound && !m_engine->validateItem(item, &latestError))
        //            errorFound = true;

        if (!errorFound) {
            if (itemIsOccurrence) {
                parentItem = fetchParentItem(item);
                if (parentItem.isEmpty()) {
                    latestError = QOrganizerManager::InvalidOccurrenceError;
                    errorFound = true;
                }
            }
        }

        if (!errorFound) {
            if (itemIsOccurrence && !fixParentReferences(&item, parentItem)) {
                latestError = QOrganizerManager::InvalidOccurrenceError;
                errorFound = true;
            }
        }

        if (!errorFound) {
            if (itemIsOccurrence && !typesAreRelated(item.type(), parentItem.type())) {
                latestError = QOrganizerManager::InvalidOccurrenceError;
                errorFound = true;
            }
        }

        if (!errorFound) {
            if (!fixGuidReferences(&item, parentItem)) {
                latestError = QOrganizerManager::InvalidOccurrenceError;
                errorFound = true;
            }
        }

        if (!errorFound) {
            if (!fixCollectionReferences(&item, parentItem)) {
                latestError = QOrganizerManager::InvalidCollectionError;
                errorFound = true;
            }
        }

        if (!errorFound && itemIsOccurrence) {
            // add exception date to parent item
            QOrganizerItemParent parentDetail = item.detail(QOrganizerItemParent::DefinitionName);
            QDate originalDate = parentDetail.originalDate();
            QOrganizerItemRecurrence recurrenceDetail = parentItem.detail(QOrganizerItemRecurrence::DefinitionName);
            QSet<QDate> exceptionDates = recurrenceDetail.exceptionDates();
            if (!exceptionDates.contains(originalDate))
                exceptionDates.insert(originalDate);

            if (parentItem.type() == QOrganizerItemType::TypeEvent) {
                QOrganizerEvent *parentEvent = static_cast<QOrganizerEvent*>(&parentItem);
                parentEvent->setExceptionDates(exceptionDates);
            } else if (parentItem.type() == QOrganizerItemType::TypeTodo) {
                QOrganizerTodo *parentTodo = static_cast<QOrganizerTodo*>(&parentItem);
                parentTodo->setExceptionDates(exceptionDates);
            }
            parentItemMap.insert(i, parentItem);
            QOrganizerItemRecurrence rec = parentItem.detail(QOrganizerItemRecurrence::DefinitionName);
        }
        // else if (!errorFound && !itemIsOccurrence) {
        // What to do when updating a parent item, i.e. one with recurrence? should we e.g. check if any exception dates have been removed and
        // remove those exceptions from db?
        // NOTE: currently there is work-around for this: some cross-checking is done when fetching items
        // }


        // check for view object
        if (item.extendedDetailData(QOrganizerJsonDbStr::eventIsSynthetic()).toBool()) {
            item.setExtendedDetailData(QOrganizerJsonDbStr::eventIsSynthetic(), false);
            item.setId(QOrganizerItemId());
            itemIsNew = true;
        }

        // remove version in case the item ID is reset
        if (itemIsNew) {
            QOrganizerItemVersion version = item.detail(QOrganizerItemDetail::TypeVersion);
            if (!version.isEmpty())
                item.removeDetail(&version);
        }

        if (errorFound) {
            errorMap.insert(i, latestError);
        } else {
            itemMap.insert(i, item);
            itemIsNewStatusMap.insert(i, itemIsNew);
        }
    }

    // save items
    if (!itemMap.isEmpty()) {
        m_storage->saveItems(&itemMap, &errorMap, &latestError);
        QOrganizerManager::Error alarmError;
        QString alarmId;
        QOrganizerItemId itemId;
        QMapIterator<int, QOrganizerItem> i(itemMap);
        while (i.hasNext()) {
            i.next();
            if (!errorMap.contains(i.key())) {
                alarmError = QOrganizerManager::NoError;
                alarmId.clear();
                items.replace(i.key(), i.value()); // always replacing because of version updating
                if (!itemIsNewStatusMap.value(i.key())) {
                    // Query alarm object if the item is not new one
                    itemId = i.value().id();
                    alarmId = m_storage->alarmId(&itemId, &alarmError);
                    if (QOrganizerManager::InvalidDetailError == alarmError && !alarmId.isEmpty()) {
                        int count = 0;
                        do {//Try to delete all the alarm objects
                            m_storage->removeAlarm(&alarmId, &alarmError);
                            alarmId = m_storage->alarmId(&itemId, &alarmError);
                            ++count;//Prevent infinite loop
                        } while (!alarmId.isEmpty() && count < ALARM_REMOVE_MAXLOOP);
                    }
                }

                if (QOrganizerManager::NoError == alarmError) {
                    if (!i.value().detail(QOrganizerItemAudibleReminder::DefinitionName).isEmpty())
                        m_storage->saveAlarm(&i.value(), &alarmId, &alarmError);// Save/Update alarm object for the saved item
                    else if (!alarmId.isEmpty())
                        m_storage->removeAlarm(&alarmId, &alarmError);
                }

                if (QOrganizerManager::NoError != alarmError) {
                    latestError = alarmError;
                    errorMap.insert(i.key(), alarmError);
                }
            } else {
                // the item was not saved, let's not save the parent item either
                parentItemMap.remove(i.key());
            }
        }
    }
    // save parent items with modified exception dates
    if (!parentItemMap.isEmpty())
        m_storage->saveItems(&parentItemMap, &parentErrorMap, &parentError);

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(saveReq);
    m_requestMgr->removeRequest(saveReq);
    QOrganizerManagerEngine::updateItemSaveRequest(saveReq, items, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemFetchRequest(QOrganizerItemFetchRequest *fetchReq)
{
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    m_requestMgr->setActive(fetchReq);
    QList<QOrganizerItem> items = internalItems(fetchReq->startDate(), fetchReq->endDate(), fetchReq->filter(), fetchReq->sorting(),
                                                fetchReq->fetchHint(), &latestError, false);
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchReq);
    m_requestMgr->removeRequest(fetchReq);
    QOrganizerManagerEngine::updateItemFetchRequest(fetchReq, items.mid(0, fetchReq->maxCount()), latestError, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemIdFetchRequest(QOrganizerItemIdFetchRequest *idFetchReq)
{
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    m_requestMgr->setActive(idFetchReq);
    QList<QOrganizerItem> items = internalItems(idFetchReq->startDate(), idFetchReq->endDate(), idFetchReq->filter(), idFetchReq->sorting(),
                                                QOrganizerItemFetchHint(), &latestError, true);
    QList<QOrganizerItemId> ids;
    for (int i = 0; i < items.length(); i++) {
        ids.append(items[i].id());
    }

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(idFetchReq);
    m_requestMgr->removeRequest(idFetchReq);
    QOrganizerManagerEngine::updateItemIdFetchRequest(idFetchReq, ids, latestError, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemFetchByIdRequest(QOrganizerItemFetchByIdRequest *fetchByIdReq)
{
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    m_requestMgr->setActive(fetchByIdReq);
    QList<QOrganizerItem> items = m_storage->itemsById(fetchByIdReq->ids(), &errorMap, &latestError);
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchByIdReq);
    m_requestMgr->removeRequest(fetchByIdReq);
    QOrganizerManagerEngine::updateItemFetchByIdRequest(fetchByIdReq, items, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemFetchForExportRequest(QOrganizerItemFetchForExportRequest *fetchForExportReq)
{
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    m_requestMgr->setActive(fetchForExportReq);
    QList<QOrganizerItem> items = internalItems(fetchForExportReq->startDate(), fetchForExportReq->endDate(), fetchForExportReq->filter(), fetchForExportReq->sorting(),
                                                fetchForExportReq->fetchHint(), &latestError, true);
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchForExportReq);
    m_requestMgr->removeRequest(fetchForExportReq);
    QOrganizerManagerEngine::updateItemFetchForExportRequest(fetchForExportReq, items, latestError, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}


void QOrganizerJsonDbRequestThread::handleItemOccurrenceFetchRequest(QOrganizerItemOccurrenceFetchRequest *occurrenceFetchReq)
{
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    m_requestMgr->setActive(occurrenceFetchReq);

    QList<QOrganizerItem> items = internalItemOccurrences(occurrenceFetchReq->parentItem(), occurrenceFetchReq->startDate(), occurrenceFetchReq->endDate(), occurrenceFetchReq->fetchHint(), occurrenceFetchReq->maxOccurrences(), true, true, 0, &latestError);
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(occurrenceFetchReq);
    m_requestMgr->removeRequest(occurrenceFetchReq);
    QOrganizerManagerEngine::updateItemOccurrenceFetchRequest(occurrenceFetchReq, items, latestError, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemRemoveRequest(QOrganizerItemRemoveRequest *removeReq)
{
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    QMultiMap<QOrganizerItemId, QDate> exceptionDates;
    // map from original remove req items list index to parent id
    QMap<int, QOrganizerItemId> parentIds;
    // map from original remove req items list index to item id
    QMap<int, QOrganizerItemId> itemIds;
    // map from parent item list index to parent item
    QMap<int, QOrganizerItem> modifiedParents;

    QList<QOrganizerItem> items = removeReq->items();
    m_requestMgr->setActive(removeReq);
    QOrganizerItem item;
    int i;
    for (i = 0; i < items.size(); i++) {
        item = items[i];
        if ((item.type() == QOrganizerItemType::TypeEventOccurrence
             || item.type() == QOrganizerItemType::TypeTodoOccurrence)
                && item.id().isNull()) {
            QOrganizerItemParent parentDetail = item.detail(QOrganizerItemParent::DefinitionName);
            if (!parentDetail.parentId().isNull() && parentDetail.originalDate().isValid()) {
                exceptionDates.insert(parentDetail.parentId(), parentDetail.originalDate());
                parentIds.insert(i, parentDetail.parentId());
            } else {
                latestError = QOrganizerManager::InvalidOccurrenceError;
                errorMap.insert(i, latestError);
            }
        } else {
            itemIds.insert(i, item.id());
        }
    }

    // handle normal events
    QMap<int, QOrganizerManager::Error> removeErrorMap;
    QOrganizerManager::Error removeError = QOrganizerManager::NoError;
    QList<QOrganizerItemId> itemIdsList = itemIds.values();
    removeItems(itemIdsList, &removeError, &removeErrorMap);
    if (!removeErrorMap.isEmpty()) {
        int itemIdsValuesIndex = 0;
        QMap<int, QOrganizerItemId>::const_iterator iterator = itemIds.constBegin();
        while (iterator != itemIds.constEnd()) {
            if (removeErrorMap.contains(itemIdsValuesIndex)) {
                latestError = removeErrorMap.value(itemIdsValuesIndex);
                errorMap.insert(iterator.key(), latestError);
            }
            itemIdsValuesIndex++;
        }
    }

    // check if any of the removed "normal" items was the parent of an occurrence to be removed
    if (!exceptionDates.isEmpty()) {
        foreach (const QOrganizerItemId &id, itemIdsList) {
            if (exceptionDates.contains(id))
                exceptionDates.remove(id);
        }
    }

    // handle generated item occurrences: add new exception date to item's parent item recurrence detail
    QMap<int, QOrganizerManager::Error> fetchErrorMap;
    QOrganizerManager::Error fetchError = QOrganizerManager::NoError;

    QList<QOrganizerItem> parentItems = m_storage->itemsById(exceptionDates.keys(), &fetchErrorMap, &fetchError);
    for (i = 0; i < parentItems.size(); i++) {
        if (!fetchErrorMap.contains(i)) {
            QOrganizerItem parentItem = parentItems[i];
            QList<QDate> addDateList = exceptionDates.values(parentItem.id());
            if (addDateList.isEmpty())
                continue;
            QOrganizerItemRecurrence recurrenceDetail = parentItem.detail(QOrganizerItemRecurrence::DefinitionName);
            QSet<QDate> exceptionDateSet = recurrenceDetail.exceptionDates();
            for (int j = 0; j < addDateList.size(); j++)
                exceptionDateSet.insert(addDateList[j]);

            recurrenceDetail.setExceptionDates(exceptionDateSet);
            parentItem.saveDetail(&recurrenceDetail);

            modifiedParents.insert(i, parentItem);
        }
    }

    QMap<int, QOrganizerManager::Error> saveErrorMap;
    QOrganizerManager::Error saveError = QOrganizerManager::NoError;
    if (!modifiedParents.isEmpty())
        m_storage->saveItems(&modifiedParents, &saveErrorMap, &saveError);

    if (!fetchErrorMap.isEmpty() || !saveErrorMap.isEmpty()) {
        for (i = 0; i < parentItems.size(); i++) {
            if (fetchErrorMap.contains(i) || saveErrorMap.contains(i)) {
                QOrganizerItemId parentId = parentItems[i].id();
                // find the indexes in the original items list where this parent item was the parent of occurrence
                QList<int> originalIndexes = parentIds.keys(parentId);
                for (int j = 0; j < originalIndexes.size(); j++) {
                    latestError = QOrganizerManager::InvalidOccurrenceError;
                    errorMap.insert(j, latestError);
                }
            }
        }
    }

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(removeReq);
    m_requestMgr->removeRequest(removeReq);
    QOrganizerManagerEngine::updateItemRemoveRequest(removeReq, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemRemoveByIdRequest(QOrganizerItemRemoveByIdRequest *removeByIdReq)
{
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    QList<QOrganizerItemId> itemIds = removeByIdReq->itemIds();

    m_requestMgr->setActive(removeByIdReq);
    removeItems(itemIds, &latestError, &errorMap);

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(removeByIdReq);
    m_requestMgr->removeRequest(removeByIdReq);
    QOrganizerManagerEngine::updateItemRemoveByIdRequest(removeByIdReq, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}


void QOrganizerJsonDbRequestThread::handleCollectionSaveRequest(QOrganizerCollectionSaveRequest *collectionSaveReq)
{
    QMap<int, QOrganizerCollection> collectionMap;
    QMap<int, bool> collectionIsNewStatusMap;
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    QList<QOrganizerCollection> collections = collectionSaveReq->collections();
    m_requestMgr->setActive(collectionSaveReq);

    for (int i = 0; i < collections.size(); i++) {
        QOrganizerCollection collection = collections.at(i);
        bool collectionIsNew = collection.id().isNull();
        bool errorFound = false;

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!collectionIsNew && (managerUri != collection.id().managerUri())) {
            latestError = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }

        if (errorFound) {
            errorMap.insert(i, latestError);
        } else {
            collectionMap.insert(i, collection);
            collectionIsNewStatusMap.insert(i, collectionIsNew);
        }
    }
    if (!collectionMap.isEmpty()) {
        m_storage->saveCollections(&collectionMap, &errorMap, &latestError);
        QMapIterator<int, QOrganizerCollection> i(collectionMap);
        while (i.hasNext()) {
            i.next();
            if (!errorMap.contains(i.key())) {
                if (collectionIsNewStatusMap.value(i.key()))
                    collections.replace(i.key(), i.value());
            }
        }
    }
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionSaveReq);
    m_requestMgr->removeRequest(collectionSaveReq);
    QOrganizerManagerEngine::updateCollectionSaveRequest(collectionSaveReq, collections, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleCollectionFetchRequest(QOrganizerCollectionFetchRequest *collectionFetchReq)
{
    m_requestMgr->setActive(collectionFetchReq);
    QOrganizerManager::Error error = QOrganizerManager::NoError;
    QList<QOrganizerCollection> collections = m_storage->collections(&error);
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionFetchReq);
    m_requestMgr->removeRequest(collectionFetchReq);
    QOrganizerManagerEngine::updateCollectionFetchRequest(collectionFetchReq, collections, error, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleCollectionRemoveRequest(QOrganizerCollectionRemoveRequest* collectionRemoveReq)
{   
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    QList<QOrganizerCollectionId> collectionIds = collectionRemoveReq->collectionIds();
    m_requestMgr->setActive(collectionRemoveReq);
    //List after removing the invalid collection ids such as empty id and default collection id
    //This is for removing the collection items,
    QMap<int, QOrganizerCollectionId> validCollectionIds;

    //Check the request list and build jsondb remove list
    for (int i = 0; i < collectionIds.size(); i++) {
        QOrganizerCollectionId collectionId = collectionIds[i];
        if (collectionId == m_storage->defaultCollection().id()) {
            //Default collection cannot be removed
            latestError = QOrganizerManager::PermissionsError;
            errorMap.insert(i, latestError);
            QString warning = QOrganizerJsonDbStr::warningDefaultCollectionRemove();
            qWarning () << warning;
        } else if (m_storage->collectionIds().contains(collectionIds.at(i))) {
            validCollectionIds.insert(i, collectionIds[i]);
        } else {
            latestError = QOrganizerManager::BadArgumentError;
            errorMap.insert(i, latestError);
        }
    }

    if (!validCollectionIds.isEmpty()) {
        m_storage->removeCollections(validCollectionIds, &errorMap, &latestError);
        QList<QOrganizerCollectionId> removedCollectionIds;
        for (int i = 0; i < collectionIds.size(); i++) {
            if (!errorMap.contains(i))
                removedCollectionIds.append(collectionIds.at(i));
        }
        if (!removedCollectionIds.isEmpty()) {
            QList<QOrganizerItemId> idList = m_storage->removeItemsByCollectionId(removedCollectionIds);
            if (!idList.isEmpty()) {// remove alarm objects
                QOrganizerManager::Error alarmError;
                QString alarmId;
                foreach (const QOrganizerItemId &id, idList) {
                    alarmError = QOrganizerManager::NoError;
                    alarmId = m_storage->alarmId(&id, &alarmError);
                    if (QOrganizerManager::NoError == alarmError && !alarmId.isEmpty())
                        m_storage->removeAlarm(&alarmId, &alarmError);
                    else if (QOrganizerManager::InvalidDetailError == alarmError && !alarmId.isEmpty()) {
                        int count = 0;
                        do {//Try to delete all the alarm objects
                            m_storage->removeAlarm(&alarmId, &alarmError);
                            alarmId = m_storage->alarmId(&id, &alarmError);
                            ++count;//Prevent infinite loop
                        } while (!alarmId.isEmpty() && count < ALARM_REMOVE_MAXLOOP);
                    }
                }
            }
        }
    }

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(collectionRemoveReq);
    QOrganizerManagerEngine::updateCollectionRemoveRequest(collectionRemoveReq, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::initDefaultCollection()
{
    QOrganizerCollection defaultCollection = m_storage->defaultCollection();
    if (defaultCollection.id().isNull()) {
        QOrganizerManager::Error error;
        defaultCollection.setMetaData(QOrganizerCollection::KeyName, QOrganizerJsonDbStr::defaultCollectionDisplayName());
        m_storage->createDefaultCollection(&defaultCollection, &error);
    }
}


// Save helpers

QOrganizerItem QOrganizerJsonDbRequestThread::fetchParentItem(const QOrganizerItem &occurrence)
{
    QList<QOrganizerItem> tmpParentItems;
    QList<QOrganizerItemId> parentItemIdList;
    QMap<int, QOrganizerManager::Error> parentErrorMap;
    QOrganizerManager::Error parentError = QOrganizerManager::NoError;
    QOrganizerItemParent parentDetail = occurrence.detail(QOrganizerItemParent::DefinitionName);
    if (!parentDetail.isEmpty() && parentDetail.hasValue(QOrganizerItemParent::FieldParentId)) {
        parentItemIdList.append(parentDetail.parentId());
        tmpParentItems = m_storage->itemsById(parentItemIdList, &parentErrorMap, &parentError);
        if (parentError == QOrganizerManager::NoError && tmpParentItems.length() > 0)
            return tmpParentItems[0];
    }
    // if parent was not found based on id, try with guid
    if (!occurrence.guid().isEmpty()) {
        QOrganizerItemDetailFilter guidFilter;
        guidFilter.setDetail(QOrganizerItemGuid::DefinitionName, QOrganizerItemGuid::FieldGuid);
        guidFilter.setValue(occurrence.guid());

        parentError = QOrganizerManager::NoError;
        tmpParentItems = m_storage->items(QDateTime(), QDateTime(), guidFilter, QList<QOrganizerItemSortOrder>(), QOrganizerItemFetchHint(), &parentError);
        if (parentError == QOrganizerManager::NoError && tmpParentItems.length() > 0)
            return tmpParentItems[0];
    }
    return QOrganizerItem();
}

/*!
   Returns true if and only if \a occurrenceType is the "Occurrence" version of \a parentType.
 */
bool QOrganizerJsonDbRequestThread::typesAreRelated(QOrganizerItemType::ItemType occurrenceType, QOrganizerItemType::ItemType parentType)
{
    return ((parentType == QOrganizerItemType::TypeEvent
                && occurrenceType == QOrganizerItemType::TypeEventOccurrence)
            || (parentType == QOrganizerItemType::TypeTodo
                && occurrenceType == QOrganizerItemType::TypeTodoOccurrence));
}

bool QOrganizerJsonDbRequestThread::fixParentReferences(QOrganizerItem *item, const QOrganizerItem &parentItem)
{
//    bool itemIsOccurrence = !parentItem.isEmpty();

    QOrganizerItemParent parentDetail = item->detail(QOrganizerItemParent::DefinitionName);
    if (!parentDetail.hasValue(QOrganizerItemParent::FieldOriginalDate))
        return false;

    if (!parentDetail.hasValue(QOrganizerItemParent::FieldParentId)) {
        parentDetail.setParentId(parentItem.id());
        item->saveDetail(&parentDetail);
    }
    return true;
}

bool QOrganizerJsonDbRequestThread::fixGuidReferences(QOrganizerItem *item, const QOrganizerItem &parentItem)
{
    bool itemIsOccurrence = !parentItem.isEmpty();

    if (!itemIsOccurrence && item->guid().isEmpty()) {
        item->setGuid(QUuid::createUuid().toString());
        return true;
    }
    if (itemIsOccurrence) {
        // item is an occurrence
        if (parentItem.guid().isEmpty()) {
            return false;
        }
        else if (item->guid().isEmpty()) {
            item->setGuid(parentItem.guid());
            return true;
        } else if (item->guid() != parentItem.guid()) {
            return false;
        }
    }
    return true;
}

bool QOrganizerJsonDbRequestThread::fixCollectionReferences(QOrganizerItem *item, const QOrganizerItem &parentItem)
{
    bool itemIsOccurrence = !parentItem.isEmpty();
    QOrganizerCollectionId collectionId = item->collectionId();
    QOrganizerCollectionId parentCollectionId = parentItem.collectionId();
    if (itemIsOccurrence && (parentCollectionId.isNull() || !m_storage->collectionIds().contains(parentCollectionId)))
        return false;

    if (!collectionId.isNull()) {
        //If we could find the collection id in collection id list
        if (!m_storage->collectionIds().contains(collectionId))
            return false;

        if (itemIsOccurrence) {
            // Does this occurrence have different collection id than it's parent
            if (collectionId != parentCollectionId)
                return false;
        }
    } else {
        if (itemIsOccurrence)
            item->setCollectionId(parentCollectionId);
        else
            item->setCollectionId(m_storage->defaultCollection().id());
    }
    return true;
}

// Fetch helpers

QList<QOrganizerItem> QOrganizerJsonDbRequestThread::internalItems(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error, bool forExport) const
{
    QList<QOrganizerItem> timeUndefined;
    QSet<QOrganizerItemId> parentsAdded;
    QSet<QOrganizerItemId> parentsToBeAdded;
    QMultiMap<QDateTime, QOrganizerItem> defaultSorted;
    QList<QOrganizerItem> sorted;
    QMap<QOrganizerItemId, QList<QDate> > exceptionDateMap;
    bool isDefaultFilter = (filter.type() == QOrganizerItemFilter::DefaultFilter);

    // fetch all parents stored to db
    QList<QOrganizerItem> parentItems = m_storage->items(QDateTime(), QDateTime(), QOrganizerItemFilter(), QList<QOrganizerItemSortOrder>(), fetchHint,
                                                         error, QOrganizerJsonDbDataStorage::FetchParents);
    // fetch all items (normal items and exception occurrences) stored in the given time period
    // apply filters
    QList<QOrganizerItem> items = m_storage->items(startDate, endDate, filter, sortOrders, fetchHint, error, QOrganizerJsonDbDataStorage::FetchItems);

    // generate occurrences for all parent items
    foreach (QOrganizerItem parent, parentItems) {
        if (!parent.detail(QOrganizerItemRecurrence::DefinitionName).isEmpty()) {
            QOrganizerManager::Error recError = QOrganizerManager::NoError;
            QList<QDate> exceptionDates;
            QList<QOrganizerItem> recItems = internalItemOccurrences(parent, startDate, endDate, fetchHint, forExport ? 1 : QOrganizerJsonDbRequestThread::MaxOccurrenceCount,
                                                                     false, false, &exceptionDates, &recError);
            if (!exceptionDates.isEmpty())
                exceptionDateMap.insert(parent.id(), exceptionDates);
            QOrganizerItem toAdd;
            foreach (const QOrganizerItem &occurrence, recItems) {
                if (!isDefaultFilter) {
                    if (!QOrganizerManagerEngine::testFilter(filter, occurrence))
                        continue;
                }
                // if forExport is true, this loop is executed max. once
                if (forExport) {
                    parentsAdded.insert(parent.id());
                    toAdd = parent;
                } else {
                    toAdd = occurrence;
                }

                if (sortOrders.isEmpty()) {
                    if (!QOrganizerManagerEngine::addDefaultSorted(&defaultSorted, toAdd))
                        timeUndefined.append(toAdd);
                } else {
                    QOrganizerManagerEngine::addSorted(&sorted, toAdd, sortOrders);
                }
            }
        }
    }

    // add all normal items and exception occurrences to return list
    foreach (const QOrganizerItem &item, items) {
        // this is either Event or Todo
        // or exception EventOccurrence or TodoOccurrence which has been stored to database

        if (!item.detail(QOrganizerItemRecurrence::DefinitionName).isEmpty()) {
            // parent items have already been handled
            continue;
        }
        QOrganizerItemParent parentDetail = item.detail(QOrganizerItemParent::DefinitionName);
        if (!parentDetail.isEmpty() && !exceptionDateMap.value(parentDetail.parentId()).contains(parentDetail.originalDate()))
            continue;

        if (forExport && !parentDetail.isEmpty()) {
            QOrganizerItemId parentId = parentDetail.parentId();
            if (!parentId.isNull())
                parentsToBeAdded.insert(parentId);
        }
        if (sortOrders.isEmpty()) {
            if (!QOrganizerManagerEngine::addDefaultSorted(&defaultSorted, item))
                timeUndefined.append(item);
        } else {
            QOrganizerManagerEngine::addSorted(&sorted, item, sortOrders);
        }
    }

    if (forExport && !parentsToBeAdded.isEmpty()) {
        foreach (QOrganizerItem item, parentItems) {
            if (parentsToBeAdded.contains(item.id()) && !parentsAdded.contains(item.id())) {
                if (sortOrders.isEmpty())
                    QOrganizerManagerEngine::addDefaultSorted(&defaultSorted, item);
                else
                    QOrganizerManagerEngine::addSorted(&sorted, item, sortOrders);
            }
        }
    }

    if (sortOrders.isEmpty()) {
        sorted = defaultSorted.values();
        sorted.append(timeUndefined);
    }
    return sorted;
}

QList<QOrganizerItem> QOrganizerJsonDbRequestThread::internalItemOccurrences(const QOrganizerItem &parentItem, const QDateTime &periodStart, const QDateTime &periodEnd, const QOrganizerItemFetchHint &fetchHint,
                                                                             int maxCount, bool includeExceptions, bool sortItems, QList<QDate> *exceptionDates, QOrganizerManager::Error *error) const
{
    // given the generating item, grab it's QOrganizerItemRecurrence detail (if it exists), and calculate all of the dates within the given period.
    QDateTime realPeriodStart(periodStart);
    QDateTime realPeriodEnd(periodEnd);
    QDateTime initialDateTime;
    if (parentItem.type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent evt = parentItem;
        initialDateTime = evt.startDateTime();
    } else if (parentItem.type() == QOrganizerItemType::TypeTodo) {
        QOrganizerTodo todo = parentItem;
        initialDateTime = todo.startDateTime();
    } else {
        // erm... not a recurring item in our schema...
        return QList<QOrganizerItem>();
    }

    if (realPeriodStart.isValid() && initialDateTime.isValid()) {
        if (initialDateTime > realPeriodStart)
            realPeriodStart = initialDateTime;
    } else if (initialDateTime.isValid()) {
        realPeriodStart = initialDateTime;
    }

    if (!periodEnd.isValid()) {
        // If no endDateTime is given, we'll only generate items that occur within the next 4 years of realPeriodStart.
        if (realPeriodStart.isValid())
            realPeriodEnd = realPeriodStart.addDays(QOrganizerJsonDbRequestThread::DefaultTimePeriod);
        else
            realPeriodEnd = QDateTime::currentDateTimeUtc().addDays(QOrganizerJsonDbRequestThread::DefaultTimePeriod);
    }

    if (realPeriodStart > realPeriodEnd) {
        *error = QOrganizerManager::BadArgumentError;
        return QList<QOrganizerItem>();
    }

    QList<QOrganizerItem> retn;
    QList<QOrganizerItem> xoccurrences;
    QOrganizerItemRecurrence recur = parentItem.detail(QOrganizerItemRecurrence::DefinitionName);
    if (includeExceptions) {
        // first, retrieve all persisted instances (exceptions) which occur between the specified datetimes.
        QOrganizerItemFilter filter;
        QOrganizerItemSortOrder sortOrder;
        xoccurrences = m_storage->items(realPeriodStart, realPeriodEnd, filter, sortOrder, fetchHint, error, QOrganizerJsonDbDataStorage::FetchItemOccurrences, parentItem.id());
    }

    // then, generate the required (unchanged) instances from the parentItem.
    // before doing that, we have to find out all of the exception dates.
    QList<QDate> xdates;
    foreach (const QDate &xdate, recur.exceptionDates()) {
        xdates += xdate;
    }
    QSet<QOrganizerRecurrenceRule> xrules = recur.exceptionRules();
    foreach (const QOrganizerRecurrenceRule& xrule, xrules) {
        if (xrule.frequency() != QOrganizerRecurrenceRule::Invalid
                && ((xrule.limitType() != QOrganizerRecurrenceRule::DateLimit) || (xrule.limitDate() >= realPeriodStart.date()))) {
            // we cannot skip it, since it applies in the given time period.
            QList<QDateTime> xdatetimes = QOrganizerManagerEngine::generateDateTimes(initialDateTime, xrule, realPeriodStart, realPeriodEnd, QOrganizerJsonDbRequestThread::MaxOccurrenceCount);
            foreach (const QDateTime& xdatetime, xdatetimes) {
                xdates += xdatetime.date();
            }
        }
    }
    // now generate a list of rdates (from the recurrenceDates and recurrenceRules)
    QList<QDateTime> rdates;
    foreach (const QDate& rdate, recur.recurrenceDates()) {
        rdates += QDateTime(rdate, initialDateTime.time());
    }
    bool hasValidRule = false;
    QSet<QOrganizerRecurrenceRule> rrules = recur.recurrenceRules();
    foreach (const QOrganizerRecurrenceRule& rrule, rrules) {
        if (rrule.frequency() != QOrganizerRecurrenceRule::Invalid) {
            hasValidRule = true;
            if ((rrule.limitType() != QOrganizerRecurrenceRule::DateLimit) || (rrule.limitDate() >= realPeriodStart.date())) {
                // we cannot skip it, since it applies in the given time period.
                rdates += QOrganizerManagerEngine::generateDateTimes(initialDateTime, rrule, realPeriodStart, realPeriodEnd, QOrganizerJsonDbRequestThread::MaxOccurrenceCount);
            }
        }
    }
    // now order the contents of retn by date
    qSort(rdates);
    if (!hasValidRule && initialDateTime.isValid() && qBinaryFind(rdates, initialDateTime) == rdates.constEnd()) {
        rdates.prepend(initialDateTime);
    }
    // now for each rdate which isn't also an xdate
    foreach (const QDateTime& rdate, rdates) {
        if ((rdate >= realPeriodStart && rdate <= realPeriodEnd)
                || (!realPeriodStart.isValid() && !realPeriodEnd.isValid() && rdate.isValid())) {
            if (!xdates.contains(rdate.date())) {
                // generate the required instance and add it to the return list.
                retn.append(QOrganizerManagerEngine::generateOccurrence(parentItem, rdate));
            } else if (includeExceptions) {
                for (int i = 0; i < xoccurrences.size(); i++) {
                    QOrganizerItemParent parentDetail = xoccurrences[i].detail(QOrganizerItemParent::DefinitionName);
                    if (parentDetail.originalDate() == rdate.date())
                        retn.append(xoccurrences[i]);
                }
            } else if (exceptionDates) {
                exceptionDates->append(rdate.date());
            }

        }
    }
    if (sortItems) {
        // should we always sort if a maxCount is given?
        QMultiMap<QDateTime, QOrganizerItem> defaultSorted;
        foreach (QOrganizerItem item, retn)
            QOrganizerManagerEngine::addDefaultSorted(&defaultSorted, item);

        retn = defaultSorted.values();
    }
    // and return the first maxCount entries.
    return retn.mid(0, maxCount);
}


void QOrganizerJsonDbRequestThread::removeItems(const QList<QOrganizerItemId> &itemIds, QOrganizerManager::Error *error, QMap<int, QOrganizerManager::Error> *errorMap)
{
    QMap<int, QOrganizerManager::Error> fetchErrorMap;
    QOrganizerManager::Error fetchError = QOrganizerManager::NoError;
    QList<QOrganizerItemId> removedParentIds;

    if (!itemIds.isEmpty()) {
        // fetch items to find out if there are any persisted occurrences or parent items
        // among them
        QList<QOrganizerItem> items = m_storage->itemsById(itemIds, &fetchErrorMap, &fetchError);
        foreach (QOrganizerItem item, items) {
            if ((item.type() == QOrganizerItemType::TypeEvent || item.type() == QOrganizerItemType::TypeTodo)
                    && !item.detail(QOrganizerItemRecurrence::DefinitionName).isEmpty()) {
                removedParentIds.append(item.id());
            }
        }
        m_storage->removeItems(itemIds, errorMap, error);
        removeAlarmObjects(itemIds, *errorMap);

        // remove all persisted occurrences of removed parent items
        if (!removedParentIds.isEmpty()) {
            QList<QOrganizerItemId> removedOccurrences = m_storage->removeItemsByParentId(removedParentIds);
            if (!removedOccurrences.isEmpty())
                removeAlarmObjects(removedOccurrences, QMap<int, QOrganizerManager::Error>());
        }
    }
}

void QOrganizerJsonDbRequestThread::removeAlarmObjects(const QList<QOrganizerItemId> &itemIds, const QMap<int, QOrganizerManager::Error> &errorMap)
{
    QOrganizerManager::Error alarmError;
    QString alarmId;
    int index = 0;

    foreach (const QOrganizerItemId &id, itemIds) {
        if (!errorMap.contains(index)) {
            alarmError = QOrganizerManager::NoError;
            alarmId = m_storage->alarmId(&id, &alarmError);
            if (QOrganizerManager::NoError == alarmError && !alarmId.isEmpty()) {
                m_storage->removeAlarm(&alarmId, &alarmError);
            } else if (QOrganizerManager::InvalidDetailError == alarmError && !alarmId.isEmpty()) {
                int count = 0;
                do {
                    //Try to delete all the alarm objects
                    m_storage->removeAlarm(&alarmId, &alarmError);
                    alarmId = m_storage->alarmId(&id, &alarmError);
                    ++count;
                } while (!alarmId.isEmpty() && count < ALARM_REMOVE_MAXLOOP);
            }
            // not inserting an error if event is successfully removed but alarm object cannot be removed
//            if (QOrganizerManager::NoError != alarmError) {
//                latestError = alarmError;
//                errorMap.insert(index, alarmError);
//            }
        }
        ++index;
    }
}


#include "moc_qorganizerjsondbrequestthread.cpp"

QTORGANIZER_END_NAMESPACE
