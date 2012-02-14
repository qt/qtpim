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

#ifndef SYNCHRONIZEDJSONDBCLIENT_H
#define SYNCHRONIZEDJSONDBCLIENT_H

#include <QVariantMap>
#include <QString>
#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbrequest.h>
#include <qcontactsglobal.h>
#include "synchronizedworker.h"
#include "qcontactjsondbstring.h"

QT_USE_NAMESPACE_JSONDB

QTCONTACTS_BEGIN_NAMESPACE

class SynchronizedJsonDbClient
{
public:
    SynchronizedJsonDbClient();
    ~SynchronizedJsonDbClient();

    QList<QJsonObject> query(const QString &query, QString partition = QContactJsonDbStr::userDataPartition());
    QList<QJsonObject> create(const QJsonObject &query, QString partition = QContactJsonDbStr::userDataPartition());
    QList<QJsonObject> update(const QJsonObject &query, QString partition = QContactJsonDbStr::userDataPartition());
    QList<QJsonObject> remove(const QJsonObject &query, QString partition = QContactJsonDbStr::userDataPartition());

private:
    QList<QJsonObject> getResults(QJsonDbRequest *request);

    QJsonDbConnection *m_jsonDbConnection;
    SynchronizedWorker* m_worker;
};

QTCONTACTS_END_NAMESPACE

#endif // SYNCHRONIZEDJSONDBCLIENT_H
