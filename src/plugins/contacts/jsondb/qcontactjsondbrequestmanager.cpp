/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>
#include <QMutexLocker>

#include <QThread>

#include "qcontactjsondbrequestmanager.h"

QTCONTACTS_BEGIN_NAMESPACE

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

void QContactJsonDbRequestManager::addTransaction(int trId, TransactionType transactionType, QContactAbstractRequest *req, int contactIndex)
{
    QMutexLocker locker(m_operationMutex);

    m_transactionTypeMap.insert(trId, transactionType);
    if (m_activeRequests.contains(req)) {
        m_activeRequests.value(req)->m_transactionMap.insert(trId, contactIndex);
    }
}

QContactAbstractRequest* QContactJsonDbRequestManager::removeTransaction(int trId, TransactionType &transactionType, int &contactIndex)
{
    QMutexLocker locker(m_operationMutex);

    if (m_transactionTypeMap.contains(trId)) {
        transactionType = m_transactionTypeMap.value(trId);
        m_transactionTypeMap.remove(trId);
        // Notifications transaction never has QContactAbsractRequest assosiated.
        if (transactionType == NotificationsTransaction)
            return 0;
    } else {
        qWarning() << Q_FUNC_INFO << "Could not find transaction:" << trId;
        transactionType = InvalidTransaction;
        return 0;
    }
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
    // Request for this transaction already deleted so mark transaction as orphan.
    transactionType = OrphanTransaction;
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

bool QContactJsonDbRequestManager::contains(QContactAbstractRequest *req) const
{
    QMutexLocker locker(m_operationMutex);
    return m_activeRequests.contains(req);// || m_inactiveRequests.contains(req);
}

#include "moc_qcontactjsondbrequestmanager.cpp"

QTCONTACTS_END_NAMESPACE
