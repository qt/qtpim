/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QCONTACTJSONDBREQUESTHANDLER_H
#define QCONTACTJSONDBREQUESTHANDLER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbwatcher.h>
#include <QtJsonDb/qjsondbrequest.h>
#include <qcontactabstractrequest.h>
#include <qcontactsaverequest.h>
#include <qcontactfetchrequest.h>
#include <qcontactidfetchrequest.h>
#include <qcontactremoverequest.h>
#include <qcontact.h>
#include <qcontactchangeset.h>

#include "qcontactjsondbrequestmanager.h"

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE


QT_USE_NAMESPACE_JSONDB

QTCONTACTS_BEGIN_NAMESPACE

class QContactJsonDbEngine;
class QContactJsonDbConverter;
class QContactJsonDbRequestHandler : public QObject
{
    Q_OBJECT
public:
    QContactJsonDbRequestHandler();
    virtual ~QContactJsonDbRequestHandler();

    void setEngine(QContactJsonDbEngine* engine);

    void onJsonDbWatcherNotificationsAvailable(QJsonDbWatcher *jsonDbWatcher,
                                               QContactAbstractRequest::StorageLocation storageLocation);

public slots:
    bool waitForRequestFinished(QContactAbstractRequest* req, int msecs);
    void handleRequest(QContactAbstractRequest* req);
    void handleResponse(QJsonDbRequest *request);
    void init();
    void removeDestroyed(QObject *);

    void onJsonDbConnectionError(QtJsonDb::QJsonDbConnection::ErrorCode error, const QString &message);
    void onJsonDbRequestError(QtJsonDb::QJsonDbRequest::ErrorCode error,
                              QString message);
    void onJsonDbRequestFinished();

private slots:
    void onTimeout();

private:
    void createWatcherForStorageLocation(QContactAbstractRequest::StorageLocation storageLocation);

    void handleContactSaveRequest(QContactSaveRequest* req);
    QString storageLocationToPartition(QContactAbstractRequest::StorageLocations storageLocation);
    QContactAbstractRequest::StorageLocations storageLocationsOrDefault(
            QContactAbstractRequest::StorageLocations storageLocation);
    QContactAbstractRequest::StorageLocations extractStorageLocation(const QContactId &id);
    void handleContactFetchRequest(QContactFetchRequest* req);
    void handleContactRemoveRequest(QContactRemoveRequest* req);
    void handleContactIdFetchRequest(QContactIdFetchRequest* req);

    void handleContactSaveResponse(QContactSaveRequest* saveReq, QJsonDbRequest *request, int contactIndex);
    void handleContactSavePrefetchResponse(QContactFetchRequest *prefetchReq, QJsonDbRequest *request, int contactIndex);
    void handleContactFetchResponse(QContactFetchRequest* fetchReq, QJsonDbRequest *request, const QString &partitionName);
    void handleContactRemoveResponse(QContactRemoveRequest* removeReq);
    void handleContactIdFetchResponse(QContactIdFetchRequest* idReq, QJsonDbRequest *request);

    bool makeJsonDbRequest(QContactAbstractRequest *contactRequest,
                           QContactJsonDbRequestManager::RequestType jsonDbRequestType,
                           int index,
                           const QString &partition,
                           const QString &query = QString(),
                           const QList<QJsonObject> &objects = QList<QJsonObject>());

    void startTimer();

    QContactJsonDbEngine* m_engine;
    QJsonDbConnection *m_jsonDbConnection;
    QContactJsonDbRequestManager* m_requestMgr;
    QContactJsonDbConverter* m_converter;
    // Mutex to make request state changes atomic.
    // Main thread access the same requests we store in m_reqList.
    QMutex* m_reqStateMutex;
    QList <QContactAbstractRequest*> m_reqList;

    // For contact change notifications.
    QContactChangeSet m_ccs;
    static const int TIMEOUT_INTERVAL;
    QTimer *m_timer;
};

class QContactJsonDbPartitionWatcher : public QObject
{
    Q_OBJECT
public:
    QContactJsonDbPartitionWatcher(QContactJsonDbRequestHandler *requestHandler,
                                   QJsonDbWatcher *jsonDbWatcher,
                                   QContactAbstractRequest::StorageLocation storageLocation);

private slots:
    void onJsonDbWatcherNotificationsAvailable();
    void onJsonDbWatcherError(QtJsonDb::QJsonDbWatcher::ErrorCode error, QString message);

private:
    QContactJsonDbRequestHandler *m_requestHandler;
    QJsonDbWatcher *m_jsonDbWatcher;
    QContactAbstractRequest::StorageLocation m_storageLocation;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBREQUESTHANDLER_H
