/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#ifndef QORGANIZERJSONDBREQUESTTHREAD_H
#define QORGANIZERJSONDBREQUESTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "qorganizer.h"
#include "qorganizerjsondbconverter.h"

QTPIM_USE_NAMESPACE
#include <jsondb-error.h>
#include <jsondb-global.h>

Q_ADDON_JSONDB_BEGIN_NAMESPACE
class JsonDbClient;
class JsonDbConnection;
Q_ADDON_JSONDB_END_NAMESPACE
Q_USE_JSONDB_NAMESPACE

class QOrganizerJsonDbRequestManager;
class QOrganizerJsonDbEngine;


class QOrganizerJsonDbRequestThread : public QThread
{
    Q_OBJECT
public:
    QOrganizerJsonDbRequestThread();
    virtual ~QOrganizerJsonDbRequestThread();

    void setEngine(QOrganizerJsonDbEngine* engine);
    void addRequest(QOrganizerAbstractRequest* req);
    bool waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs);
    void requestDestroyed(QOrganizerAbstractRequest* req);
    QOrganizerCollection defaultCollection() const;
    void setDefaultCollection(QOrganizerCollection collection);
    virtual void run();

public slots:
    void handleRequest(QOrganizerAbstractRequest* req);
    void onNotified(const QString& notifyUuid, const QVariant& object, const QString& action);
    void onResponse(int trId, const QVariant& object);
    void onError(int trId, int errorCode, const QString& message);

signals:
    void initialized();

private:
    void handleResponse(int trId, QOrganizerManager::Error error, const QVariant& object);

    void handleItemSaveRequest(QOrganizerItemSaveRequest* saveReq);
    void handleItemFetchRequest(QOrganizerItemFetchRequest* fetchReq);
    void handleItemIdFetchRequest(QOrganizerItemIdFetchRequest* idFetchReq);
    void handleItemFetchByIdRequest(QOrganizerItemFetchByIdRequest* fetchByIdReq);
    void handleItemRemoveRequest(QOrganizerItemRemoveRequest* removeReq);
    void handleCollectionSaveRequest(QOrganizerCollectionSaveRequest* collectionSaveReq);
    void handleCollectionFetchRequest(QOrganizerCollectionFetchRequest* collectionFetchReq);
    void handleCollectionRemoveRequest(QOrganizerCollectionRemoveRequest* collectionRemoveReq);

    void handleItemSaveResponse(QOrganizerItemSaveRequest* saveReq, const QVariant &object, int index, QOrganizerManager::Error error);
    void handleItemFetchResponse(QOrganizerItemFetchRequest* fetchReq, const QVariant &object, QOrganizerManager::Error error);
    void handleItemIdFetchResponse(QOrganizerItemIdFetchRequest* idFetchReq, const QVariant &object, QOrganizerManager::Error error);
    void handleItemFetchByIdResponse(QOrganizerItemFetchByIdRequest* fetchByIdReq, const QVariant &object, QOrganizerManager::Error error);
    void handleItemRemoveResponse(QOrganizerItemRemoveRequest* removeReq, const QVariant &object, int index, QOrganizerManager::Error error);
    void handleCollectionSaveResponse(QOrganizerCollectionSaveRequest* collectionSaveReq, const QVariant &object, int index, QOrganizerManager::Error error);
    void handleCollectionFetchResponse(QOrganizerCollectionFetchRequest* collectionFetchReq, const QVariant &object, QOrganizerManager::Error error);
    void handleCollectionRemoveResponse(QOrganizerCollectionRemoveRequest* collectionRemoveReq, const QVariant &object, int index, QOrganizerManager::Error error);

    QOrganizerManager::Error handleErrorResponse(const QVariant& object, int errorCode);

    //Interpret the filter to Jsondb query string, return true if filter is valid.
    bool singleFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr);
    bool compoundFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr);

    //Get default collection ID from engine
    QString defaultCollectionId() const;

    // Member variables
    QOrganizerJsonDbEngine* m_engine;
    QOrganizerJsonDbConverter m_converter;
    JsonDbClient* m_jsonDb;
    JsonDbConnection* m_jsonConnection;
    QOrganizerJsonDbRequestManager* m_requestMgr;
    //Mutex to make the request state changes atomic
    QMutex* m_reqStateMutex;
    QOrganizerItemChangeSet m_ics;
    QOrganizerCollectionChangeSet m_ccs;
    //Keep collection id list in memory for fast visit
    QList<QOrganizerCollectionId> m_collectionsIdList;
    //Default collection
    QOrganizerCollection m_defaultCollection;
};

#endif // QORGANIZERJSONDBREQUESTTHREAD_H
