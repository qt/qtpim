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

#include "jsondbprocess.h"

JsonDbProcess::JsonDbProcess()
    : jsondbWorkingDirectory( QCoreApplication::applicationDirPath() + "/jsondbXXXXXX" )
{
}

bool JsonDbProcess::start(bool forceNewProcess, QStringList &wantedPartitions)
{
    // Start new process only if there is no existing JsonDb process
    if (system("pidof jsondb") > 0 || forceNewProcess) {

        if (!jsondbWorkingDirectory.isValid()) {
            qWarning() << Q_FUNC_INFO << "JsonDb working directory is not valid.";
            return false;
        }

        QString jsondbPath = QLibraryInfo::location(QLibraryInfo::BinariesPath) + "/jsondb";
        if (!QFileInfo(jsondbPath).exists()) {
            qWarning() << Q_FUNC_INFO << "Cannot find JsonDb binaries.";
            return false;
        }

        m_process.start(jsondbPath, QStringList() << (jsondbWorkingDirectory.path() + "/"));
        if (!m_process.waitForStarted()) {
            qWarning() << Q_FUNC_INFO << m_process.errorString();
            return false;
        }
    }

#if defined(QT_NO_JSONDB)
    Q_UNUSED(wantedPartitions);
#else
    m_CreationRequestAmount = 0;
    m_wantedPartitions = wantedPartitions;
    if (m_wantedPartitions.count()) {
        QEventLoop partitionCheckLoop;
        connect(this, SIGNAL(quitEventLoop()), &partitionCheckLoop, SLOT(quit()));
        connect(&m_JsonDbConnection, SIGNAL(error(QtJsonDb::QJsonDbConnection::ErrorCode,QString)),
                this, SLOT(onConnection(QtJsonDb::QJsonDbConnection::ErrorCode,QString)));
        connect(&m_JsonDbConnection, SIGNAL(connected()), this, SLOT(onConnection()));
        m_JsonDbConnection.connectToServer();
        partitionCheckLoop.exec();
    }
#endif//QT_NO_JSONDB

    return true;
}

void JsonDbProcess::terminate()
{
    m_process.terminate();
    if (m_process.state() != QProcess::NotRunning && !m_process.waitForFinished()) {
        qWarning() << Q_FUNC_INFO << "JsonDb did not terminate cleanly.  Killing.";
        m_process.kill();
    }
}

#if !defined(QT_NO_JSONDB)
void JsonDbProcess::onConnection(QtJsonDb::QJsonDbConnection::ErrorCode error, const QString &message)
{
    if (error == QJsonDbConnection::NoError) {
        checkPartitions();
    } else {
        qWarning() << Q_FUNC_INFO << message;
        emit quitEventLoop();
    }
}

void JsonDbProcess::checkPartitions()
{
    QJsonDbRequest *request = new QJsonDbReadRequest(QStringLiteral("[?_type=\"Partition\"]"), this);
    connect(request, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)),
            this, SLOT(onPartitionsChecked(QtJsonDb::QJsonDbRequest::ErrorCode,QString)));
    connect(request, SIGNAL(finished()), this, SLOT(onPartitionsChecked()));
    if (!m_JsonDbConnection.send(request)) {
        delete request;
        qWarning() << Q_FUNC_INFO << "Could not create partition-query request.";
        emit quitEventLoop();
    }
}

void JsonDbProcess::onPartitionsChecked(QtJsonDb::QJsonDbRequest::ErrorCode error, const QString &message)
{
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (request && error == QJsonDbRequest::NoError) {

        foreach (const QJsonObject& jsonObject, request->takeResults()) {
            m_wantedPartitions.removeOne(jsonObject.value(QStringLiteral("name")).toString());
        }

        if (m_wantedPartitions.count() == 0) {
            emit quitEventLoop();
            return;
        }

        foreach (const QString& missingPartition, m_wantedPartitions) {
            qWarning()<<"Creating missing "<<missingPartition<<"-partition.";
            QJsonObject jsondbObj;
            jsondbObj.insert(QStringLiteral("_type"), QStringLiteral("Partition"));
            jsondbObj.insert(QStringLiteral("name"), missingPartition);

            QJsonDbCreateRequest *partCreationReq = new QJsonDbCreateRequest(jsondbObj, this);
            connect(partCreationReq, SIGNAL(error(QtJsonDb::QJsonDbRequest::ErrorCode,QString)),
                    this, SLOT(onPartitionsCreated(QtJsonDb::QJsonDbRequest::ErrorCode,QString)));
            connect(partCreationReq, SIGNAL(finished()), this, SLOT(onPartitionsCreated()));
            if (!m_JsonDbConnection.send(partCreationReq)) {
                delete partCreationReq;
                qWarning() << Q_FUNC_INFO << "Could not create partition-creation request.";
                emit quitEventLoop();
            } else {
                m_CreationRequestAmount++;
            }
        }

    } else {
        qWarning() << Q_FUNC_INFO << message;
        emit quitEventLoop();
    }
    delete request;
}

void JsonDbProcess::onPartitionsCreated(QtJsonDb::QJsonDbRequest::ErrorCode error, const QString &message)
{
    QJsonDbRequest *request = qobject_cast<QJsonDbRequest *>(sender());
    if (error != QJsonDbRequest::NoError) {
        qWarning() << Q_FUNC_INFO << message;
    }
    m_CreationRequestAmount--;
    delete request;

    if (!m_CreationRequestAmount) {
        emit quitEventLoop();
    }
}
#endif//QT_NO_JSONDB

#include "moc_jsondbprocess.cpp"


