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

    case QOrganizerAbstractRequest::ItemRemoveRequest:
        handleItemRemoveRequest(static_cast <QOrganizerItemRemoveRequest *> (req));
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
    m_requestMgr->setActive(saveReq);

    for (int i = 0; i < items.size(); i++) {
        QOrganizerItem item = items.at(i);
        bool itemIsNew = item.id().isNull();
        bool errorFound = false;

        QString managerUri = QOrganizerManager::buildUri(m_engine->managerName(), m_engine->managerParameters());
        // check manager uri if is the same with the engine uri
        if (!itemIsNew && (managerUri != item.id().managerUri())) {
            latestError = QOrganizerManager::BadArgumentError;
            errorFound = true;
        }

// TODO: to be replaced by new validity check
//        // ensure that the organizeritem's details conform to their definitions
//        if (!errorFound && !m_engine->validateItem(item, &latestError))
//            errorFound = true;

        //Check the collection id validation
        QOrganizerCollectionId collectionId = item.collectionId();
        if (!collectionId.isNull()) {
            //If we could find the collection id in collection id list
            if (!m_storage->collectionIds().contains(collectionId)) {
                //Collection id is invalid
                latestError = QOrganizerManager::BadArgumentError;
                errorFound = true;
            }
        } else {
            item.setCollectionId(m_storage->defaultCollection().id());
        }

        // generate GUID if needed
        if (item.guid().isEmpty())
            item.setGuid(QUuid::createUuid().toString());

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
            }
        }
    }
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
    QList<QOrganizerItem> items = m_storage->items(fetchReq->startDate(), fetchReq->endDate(), fetchReq->filter(), fetchReq->sorting(),
                                                   fetchReq->fetchHint(), &latestError);
    QWaitCondition* waitCondition = m_requestMgr->waitCondition(fetchReq);
    m_requestMgr->removeRequest(fetchReq);
    QOrganizerManagerEngine::updateItemFetchRequest(fetchReq, items, latestError, QOrganizerAbstractRequest::FinishedState);
    if (waitCondition)
        waitCondition->wakeAll();
}

void QOrganizerJsonDbRequestThread::handleItemIdFetchRequest(QOrganizerItemIdFetchRequest *idFetchReq)
{
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    m_requestMgr->setActive(idFetchReq);
    QList<QOrganizerItem> items = m_storage->items(idFetchReq->startDate(), idFetchReq->endDate(), idFetchReq->filter(), idFetchReq->sorting(),
                                                   QOrganizerItemFetchHint(), &latestError, QOrganizerJsonDbDataStorage::FetchItemIds);
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

void QOrganizerJsonDbRequestThread::handleItemRemoveRequest(QOrganizerItemRemoveRequest *removeReq)
{
    QMap<int, QOrganizerManager::Error> errorMap;
    QOrganizerManager::Error latestError = QOrganizerManager::NoError;
    QList<QOrganizerItemId> itemIds = removeReq->itemIds();
    m_requestMgr->setActive(removeReq);
    if (!itemIds.isEmpty())
        m_storage->removeItems(itemIds, &errorMap, &latestError);

    QOrganizerManager::Error alarmError;
    QString alarmId;
    int index = 0;
    foreach (const QOrganizerItemId &id, itemIds) {
        if (!errorMap.contains(index)) {
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
            if (QOrganizerManager::NoError != alarmError) {
                latestError = alarmError;
                errorMap.insert(index, alarmError);
            }
        }
        ++index;
    }

    QWaitCondition* waitCondition = m_requestMgr->waitCondition(removeReq);
    m_requestMgr->removeRequest(removeReq);
    QOrganizerManagerEngine::updateItemRemoveRequest(removeReq, latestError, errorMap, QOrganizerAbstractRequest::FinishedState);
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

#include "moc_qorganizerjsondbrequestthread.cpp"

QTORGANIZER_END_NAMESPACE
