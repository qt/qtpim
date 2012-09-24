/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
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
#include <qcontactmanager.h>
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

    QString storageLocationToPartition(QContactAbstractRequest::StorageLocation storageLocation);
    QContactAbstractRequest::StorageLocations storageLocationsOrDefault(
            QContactAbstractRequest::StorageLocations storageLocation);
    QContactAbstractRequest::StorageLocation extractStorageLocation(const QContactId &id);
    QContactAbstractRequest::StorageLocations extractStorageLocations(const QList<QContactId> &contactIds);
    inline static QContactAbstractRequest::StorageLocations supportedStorageLocations()
    {
        return QContactAbstractRequest::UserDataStorage | QContactAbstractRequest::SystemStorage;
    }

    void handleContactSaveRequest(QContactSaveRequest* req);
    void handleContactFetchRequest(QContactFetchRequest* req);
    void handleContactRemoveRequest(QContactRemoveRequest* req);
    void handleContactIdFetchRequest(QContactIdFetchRequest* req);
    void handleContactFetchByIdRequest(QContactFetchByIdRequest* req);

    void handleContactSaveResponse(QContactSaveRequest* saveReq, QJsonDbRequest *request, int contactIndex);
    void handleContactSavePrefetchResponse(QContactFetchRequest *prefetchReq, QJsonDbRequest *request, int contactIndex);
    void handleContactFetchResponse(QContactFetchRequest* fetchReq, QJsonDbRequest *request, const QString &partitionName);
    void handleContactRemoveResponse(QContactRemoveRequest* removeReq);
    void handleContactIdFetchResponse(QContactIdFetchRequest* idReq, QJsonDbRequest *request);
    void handleContactFetchByIdResponse(QContactFetchByIdRequest *req, QJsonDbRequest *jsonDbRequest, const QString &partitionName);

    QList<QContact> orderedContacts(const QList<QContactId> &ids, const QList<QContact> &contacts,
                                    QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *lastError);

    bool makeJsonDbRequest(QContactAbstractRequest *contactRequest,
                           QContactJsonDbRequestManager::RequestType jsonDbRequestType,
                           int index,
                           const QString &partition,
                           const QString &query = QString(),
                           const QList<QJsonObject> &objects = QList<QJsonObject>());

    void startTimer();

    inline static QContactManager::Error errorPrecedence(QContactManager::Error firstError, QContactManager::Error secondError)
    {
        // Currently this makes a simple precedence for error codes.
        // In case of nonidentical mapping is needed you need to modify this.
        return (firstError < secondError) ? secondError : firstError;
    }

private:
    Q_DISABLE_COPY(QContactJsonDbRequestHandler)

private:
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

    // For maintaining storage locations availability status.
    QContactAbstractRequest::StorageLocations m_availableStorageLocations;
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
    Q_DISABLE_COPY(QContactJsonDbPartitionWatcher)

private:
    QContactJsonDbRequestHandler *m_requestHandler;
    QJsonDbWatcher *m_jsonDbWatcher;
    QContactAbstractRequest::StorageLocation m_storageLocation;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBREQUESTHANDLER_H
