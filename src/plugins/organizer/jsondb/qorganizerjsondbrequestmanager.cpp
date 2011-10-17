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
#include <QMutexLocker>

#include "qorganizerjsondbrequestmanager.h"
#include "qorganizerjsondbid.h"
#include "qorganizeritemsaverequest.h"
#include "qorganizercollectionsaverequest.h"

QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbRequestManager::QOrganizerJsonDbRequestManager()
{
    m_operationMutex = new QMutex();
}

QOrganizerJsonDbRequestManager::~QOrganizerJsonDbRequestManager()
{
    delete m_operationMutex;
}

void QOrganizerJsonDbRequestManager::addRequest(QOrganizerAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (!m_requests.contains(req)) {
        QOrganizerJsonDbRequestData* newData = new QOrganizerJsonDbRequestData();
        newData->m_status = QOrganizerJsonDbRequestManager::Inactive;
        newData->m_latestError = QOrganizerManager::NoError;
        newData->m_waitCondition = 0;

        if (req->type() == QOrganizerAbstractRequest::ItemSaveRequest)
            newData->m_itemList = static_cast<QOrganizerItemSaveRequest*>(req)->items();
        else if (req->type() == QOrganizerAbstractRequest::CollectionSaveRequest)
            newData->m_collectionList = static_cast<QOrganizerCollectionSaveRequest*>(req)->collections();
        m_requests.insert(req, newData);
    }
}

void QOrganizerJsonDbRequestManager::removeRequest(QOrganizerAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        delete m_requests.value(req);
        m_requests.remove(req);
    }
}

void QOrganizerJsonDbRequestManager::addTransaction(QOrganizerAbstractRequest* req, int trId, int index)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        m_requests.value(req)->m_transactionMap.insert(trId, index);
    }
}

QOrganizerAbstractRequest* QOrganizerJsonDbRequestManager::removeTransaction(int trId, int& index)
{
    QMutexLocker locker(m_operationMutex);

    QList<QOrganizerAbstractRequest*> reqList = m_requests.keys();
    for (int i = 0; i < reqList.size(); i++) {
        QOrganizerAbstractRequest* req = reqList.at(i);
        QMap<int, int>* transactionMap = &(m_requests.value(req)->m_transactionMap);

        if (transactionMap->contains(trId)) {
            index = transactionMap->value(trId);
            transactionMap->remove(trId);
            return reqList.at(i);
        }
    }
    return 0;
}

bool QOrganizerJsonDbRequestManager::setActive(QOrganizerAbstractRequest *req)
{
    // This function is called from JsonDbEngine thread
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        m_requests.value(req)->m_status = QOrganizerJsonDbRequestManager::Active;
        return true;
    }
    return false;
}

bool QOrganizerJsonDbRequestManager::setDeleted(QOrganizerAbstractRequest *req)
{
    // This function is called from JsonDbEngine thread
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        m_requests.value(req)->m_status = QOrganizerJsonDbRequestManager::Deleted;
        return true;
    }
    return false;
}


bool QOrganizerJsonDbRequestManager::setWaitCondition(QOrganizerAbstractRequest* req, QWaitCondition* waitCondition)
{
    // This function is called from JsonDbEngine thread
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        m_requests.value(req)->m_waitCondition = waitCondition;
        return true;
    }
    return false;
}

QWaitCondition* QOrganizerJsonDbRequestManager::waitCondition(QOrganizerAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        return m_requests.value(req)->m_waitCondition;
    }
    return 0;
}

void QOrganizerJsonDbRequestManager::removeWaitCondition(QOrganizerAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        m_requests.value(req)->m_waitCondition = 0;
    }
}

bool QOrganizerJsonDbRequestManager::updateRequestData(QOrganizerAbstractRequest* req, QOrganizerManager::Error error, int index,
                                                       QOrganizerItem item, QOrganizerCollection collection)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        QOrganizerJsonDbRequestData* reqData = m_requests.value(req);
        if (req->type() == QOrganizerAbstractRequest::ItemSaveRequest)
            reqData->m_itemList.replace(index, item);
        else if (req->type() == QOrganizerAbstractRequest::CollectionSaveRequest)
            reqData->m_collectionList.replace(index, collection);

        if (error != QOrganizerManager::NoError) {
            reqData->m_latestError = error;
            reqData->m_errorMap.insert(index, error);
        }
        return true;
    }
    return false;
}

bool QOrganizerJsonDbRequestManager::requestData(QOrganizerAbstractRequest* req, QOrganizerManager::Error* latestError, QMap<int, QOrganizerManager::Error>* errorMap,
                                                 QList<QOrganizerItem>* items, QList<QOrganizerCollection>* collections)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        QOrganizerJsonDbRequestData* reqData = m_requests.value(req);
        *latestError = reqData->m_latestError;
        *errorMap = reqData->m_errorMap;
        if (items)
            *items = reqData->m_itemList;
        if (collections)
            *collections = reqData->m_collectionList;
        return true;
    }
    return false;
}

QOrganizerJsonDbRequestManager::HandlingStatus QOrganizerJsonDbRequestManager::requestStatus(QOrganizerAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        return m_requests.value(req)->m_status;
    }
    return QOrganizerJsonDbRequestManager::Invalid;
}


bool QOrganizerJsonDbRequestManager::isRequestCompleted(QOrganizerAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        return m_requests.value(req)->m_transactionMap.isEmpty();
    }
    return true;
}

#include "moc_qorganizerjsondbrequestmanager.cpp"

QTORGANIZER_END_NAMESPACE
