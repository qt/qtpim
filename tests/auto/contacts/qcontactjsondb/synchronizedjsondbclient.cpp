/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QEventLoop>
#include <QVariant>
#include <QDebug>
#include <QtJsonDb/qjsondbreadrequest.h>
#include <QtJsonDb/qjsondbwriterequest.h>
#include "qcontactjsondbstring.h"
#include "synchronizedjsondbclient.h"

#include "qcontactjsondbstring.h"

QTCONTACTS_USE_NAMESPACE

SynchronizedJsonDbClient::SynchronizedJsonDbClient()
{
    m_jsonDbConnection = new QJsonDbConnection();
    m_worker = new SynchronizedWorker();
    QObject::connect(m_jsonDbConnection, SIGNAL(error(QtJsonDb::QJsonDbConnection::ErrorCode,QString)),
            m_worker, SLOT(onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode,QString)));
    m_jsonDbConnection->connectToServer();
}

SynchronizedJsonDbClient::~SynchronizedJsonDbClient()
{
    delete m_worker;
    delete m_jsonDbConnection;
}

QList<QJsonObject> SynchronizedJsonDbClient::query(const QString &query, QString partition)
{
    QJsonDbRequest *request;
    request = new QJsonDbReadRequest(query, m_worker);
    request->setPartition(partition);
    return getResults(request);
}

QList<QJsonObject> SynchronizedJsonDbClient::create(const QJsonObject &objects, QString partition)
{
    QJsonDbRequest *request;
    request = new QJsonDbCreateRequest(objects, m_worker);
    request->setPartition(partition);
    return getResults(request);
}

QList<QJsonObject> SynchronizedJsonDbClient::update(const QJsonObject &objects, QString partition)
{
    QJsonDbRequest *request = new QJsonDbUpdateRequest(objects, m_worker);
    request->setPartition(partition);
    return getResults(request);
}

QList<QJsonObject> SynchronizedJsonDbClient::remove(const QJsonObject &objects, QString partition)
{
    QJsonDbRequest *request = new QJsonDbRemoveRequest(objects,m_worker);
    request->setPartition(partition);
    return getResults(request);
}

QList<QJsonObject> SynchronizedJsonDbClient::getResults(QJsonDbRequest *request)
{
    QObject::connect(request, SIGNAL(finished()), m_worker, SLOT(onJsonDbRequestFinished()));

    QObject::connect(request, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)),
                     m_worker, SLOT(onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode, QString)));

    if (m_jsonDbConnection->send(request)) {
        m_worker->exec(); // Wait for db client to finish
        QList<QJsonObject> results = request->takeResults();
        return results;
    } else {
        return QList<QJsonObject>();
    }
}

