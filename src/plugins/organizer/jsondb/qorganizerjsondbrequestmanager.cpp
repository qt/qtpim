/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qorganizerjsondbrequestmanager.h"

#include <QtCore/qmutex.h>

QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbRequestManager::QOrganizerJsonDbRequestManager()
{
    m_operationMutex = new QMutex();
}

QOrganizerJsonDbRequestManager::~QOrganizerJsonDbRequestManager()
{
    for (QMap<QOrganizerAbstractRequest *, QOrganizerJsonDbRequestData *>::const_iterator i = m_requests.begin(); i != m_requests.end(); ++i)
        delete i.value();

    delete m_operationMutex;
}

void QOrganizerJsonDbRequestManager::addRequest(QOrganizerAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (!m_requests.contains(req)) {
        QOrganizerJsonDbRequestData *newData = new QOrganizerJsonDbRequestData();
        newData->m_status = QOrganizerJsonDbRequestManager::Inactive;
        newData->m_waitCondition = 0;
        m_requests.insert(req, newData);
    }
}

void QOrganizerJsonDbRequestManager::removeRequest(QOrganizerAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        delete m_requests.value(req);
        m_requests.remove(req);
    }
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

bool QOrganizerJsonDbRequestManager::setWaitCondition(QOrganizerAbstractRequest *req, QWaitCondition *waitCondition)
{
    // This function is called from JsonDbEngine thread
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req)) {
        m_requests.value(req)->m_waitCondition = waitCondition;
        return true;
    }
    return false;
}

QWaitCondition *QOrganizerJsonDbRequestManager::waitCondition(QOrganizerAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req))
        return m_requests.value(req)->m_waitCondition;
    return 0;
}

void QOrganizerJsonDbRequestManager::removeWaitCondition(QOrganizerAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req))
        m_requests.value(req)->m_waitCondition = 0;
}

QOrganizerJsonDbRequestManager::HandlingStatus QOrganizerJsonDbRequestManager::requestStatus(QOrganizerAbstractRequest *req)
{
    QMutexLocker locker(m_operationMutex);

    if (m_requests.contains(req))
        return m_requests.value(req)->m_status;
    return QOrganizerJsonDbRequestManager::Invalid;
}

QTORGANIZER_END_NAMESPACE
