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
#include <jsondb-client.h>
#include "synchronizedworker.h"

Q_USE_JSONDB_NAMESPACE

class SynchronizedJsonDbClient
{
public:
    SynchronizedJsonDbClient();
    ~SynchronizedJsonDbClient();
    QVariantMap query(const QString &query);
    QVariantMap create(const QVariantMap &query);
    QVariantMap update(const QVariantMap &query);
    QVariantMap remove(const QVariantMap &query);
private:
    QVariantMap getResults(int trId);
    JsonDbClient* m_dbClient;
    SynchronizedWorker* m_worker;
};

#endif // SYNCHRONIZEDJSONDBCLIENT_H
