/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTJSONDBREQUESTHANDLER_H
#define QCONTACTJSONDBREQUESTHANDLER_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <qcontactabstractrequest.h>
#include <qcontactsaverequest.h>
#include <qcontactfetchrequest.h>
#include <qcontactidfetchrequest.h>
#include <qcontactremoverequest.h>
#include <qcontact.h>
#include <qcontactchangeset.h>

#include <jsondb-global.h>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

Q_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbClient;
class JsonDbConnection;
Q_ADDON_JSONDB_END_NAMESPACE
Q_USE_JSONDB_NAMESPACE

QTCONTACTS_BEGIN_NAMESPACE

class QContactJsonDbRequestManager;
class QContactJsonDbEngine;
class QContactJsonDbConverter;
class QContactJsonDbRequestHandler : public QObject
{
    Q_OBJECT
public:
    QContactJsonDbRequestHandler();
    virtual ~QContactJsonDbRequestHandler();

    void setEngine(QContactJsonDbEngine* engine);

public slots:
    bool waitForRequestFinished(QContactAbstractRequest* req, int msecs);
    void handleRequest(QContactAbstractRequest* req);
    void onNotified(const QString& notifyUuid, const QVariant& object, const QString& action);
    void onResponse(int id, const QVariant& object);
    void onError(int id, int code, const QString& message);
    void init();
    void removeDestroyed(QObject *);

private slots:
    void onTimeout();

private:
    void handleContactSaveRequest(QContactSaveRequest* req);
    void handleContactFetchRequest(QContactFetchRequest* req);
    void handleContactRemoveRequest(QContactRemoveRequest* req);
    void handleContactIdFetchRequest(QContactIdFetchRequest* req);
    void sendJsonDbNotificationsRequest();

    void handleContactSaveResponse(QContactSaveRequest *req, const QVariant &object, int index);
    void handleContactSavePrefetchResponse(QContactSaveRequest *req, const QVariant &object, int index);
    void handleContactFetchResponse(QContactFetchRequest *req, const QVariant &object);
    void handleContactRemoveResponse(QContactRemoveRequest *req);
    void handleContactIdFetchResponse(QContactIdFetchRequest *req, const QVariant &object);
    void handleJsonDbNotificationsRequestError(QContactManager::Error error);

    void startTimer();
    QString convertContactIdToUuid(QContactId &id);
    QContactId convertUuidtoContactId(QString &id);

    QContactJsonDbEngine* m_engine;
    JsonDbClient* m_jsonDb;
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
    bool m_jsonDbNotificationsRequested;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBREQUESTHANDLER_H
