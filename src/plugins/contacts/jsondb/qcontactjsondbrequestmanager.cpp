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

#include <QDebug>
#include <QMutexLocker>

#include <QThread>

#include "qcontactjsondbrequestmanager.h"

//QTM_USE_NAMESPACE

QContactJsonDbRequestManager::QContactJsonDbRequestManager()
{
    m_operationMutex = new QMutex();
}

QContactJsonDbRequestManager::~QContactJsonDbRequestManager()
{
    delete m_operationMutex;
}

void QContactJsonDbRequestManager::addRequest(QContactAbstractRequest* req, QList<QContact> items)
{
    QMutexLocker locker(m_operationMutex);

    if (!m_activeRequests.contains(req)) {
        QContactRequestData* newData = new QContactRequestData();
        newData->m_contactList = items;
        if (m_inactiveRequests.contains(req)) {
            newData->m_waitCondition = m_inactiveRequests.value(req);
            m_inactiveRequests.remove(req);
        } else {
            newData->m_waitCondition = 0;
        }
        m_activeRequests.insert(req, newData);
    }
}

void QContactJsonDbRequestManager::removeRequest(QContactAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);
    if (m_activeRequests.contains(req)) {
        delete m_activeRequests.value(req);
        m_activeRequests.remove(req);
    }
}

void QContactJsonDbRequestManager::addTransaction(QContactAbstractRequest* req, int trId, int contactIndex)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_transactionMap.insert(trId, contactIndex);
    }
}

QContactAbstractRequest* QContactJsonDbRequestManager::removeTransaction(int trId, int& contactIndex)
{
    QMutexLocker locker(m_operationMutex);

    QList<QContactAbstractRequest*> reqList = m_activeRequests.keys();
    for (int i = 0; i < reqList.size(); i++) {
        QContactAbstractRequest* req = reqList.at(i);
        QMap<int, int>* transactionMap = &(m_activeRequests.value(req)->m_transactionMap);

        if (transactionMap->contains(trId)) {
            contactIndex = transactionMap->value(trId);
            transactionMap->remove(trId);
            return reqList.at(i);
        }
    }
    return 0;
}

bool QContactJsonDbRequestManager::setWaitCondition(QContactAbstractRequest *req, QWaitCondition *waitCondition)
{
    // This function is called by JsonDbEngine, which is in another thread
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_waitCondition = waitCondition;
        return true;
    } else if (!m_inactiveRequests.contains(req)) {
        // Request and wait condition stored here if waitForRequestFinished function is called without starting
        // the request or after starting, but before slot JsonDbEngineRequestHandler::handleRequest is called.
        m_inactiveRequests.insert(req, waitCondition);
        return false;
    }
    //This should never happen...
    return false;
}

QWaitCondition* QContactJsonDbRequestManager::waitCondition(QContactAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        return m_activeRequests.value(req)->m_waitCondition;
    }
    return 0;
}

void QContactJsonDbRequestManager::removeWaitCondition(QContactAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    // The request is found only if waitForRequestFinished function was called for an Inactive request
    // which was never started.

    if (!m_inactiveRequests.contains(req)) {
        m_inactiveRequests.remove(req);
    }
}

void QContactJsonDbRequestManager::addContact(QContactAbstractRequest* req, QContact item, int contactIndex)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_contactList.replace(contactIndex, item);
    }
}

QList<QContact> QContactJsonDbRequestManager::contacts(QContactAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req))
        return m_activeRequests.value(req)->m_contactList;
    else
        return QList<QContact>();
}



bool QContactJsonDbRequestManager::isRequestCompleted(QContactAbstractRequest* req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_activeRequests.contains(req)) {
        return m_activeRequests.value(req)->m_transactionMap.isEmpty();
    }
    return true;
}

bool QContactJsonDbRequestManager::contains(QtAddOn::Pim::QContactAbstractRequest *req) const
{
    QMutexLocker locker(m_operationMutex);
    return m_activeRequests.contains(req);// || m_inactiveRequests.contains(req);
}
