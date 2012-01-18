/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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
#include <QVariant>
#include <QDebug>
#include "synchronizedjsondbclient.h"

SynchronizedJsonDbClient::SynchronizedJsonDbClient()
{
    m_dbClient = new JsonDbClient();
    m_worker = new SynchronizedWorker();
}

SynchronizedJsonDbClient::~SynchronizedJsonDbClient()
{
    if (m_worker) {
        delete m_worker;
        m_worker = NULL;
    }
}

QVariantMap SynchronizedJsonDbClient::query(const QString &query)
{
    return getResults(m_dbClient->query(query));
}

QVariantMap SynchronizedJsonDbClient::create(const QVariantMap &query)
{
    return getResults(m_dbClient->create(query));
}

QVariantMap SynchronizedJsonDbClient::update(const QVariantMap &query)
{
    return getResults(m_dbClient->update(query));
}

QVariantMap SynchronizedJsonDbClient::remove(const QVariantMap &query)
{
    return getResults(m_dbClient->remove(query));
}

QVariantMap SynchronizedJsonDbClient::getResults(int trId)
{
    QObject::connect(m_dbClient, SIGNAL(response(int, QVariant)),
                     m_worker, SLOT(onDbResponse(int, QVariant)));
    m_worker->exec(); // Wait for db client to finish
    return m_worker->getDbObject(trId).toMap();
}
