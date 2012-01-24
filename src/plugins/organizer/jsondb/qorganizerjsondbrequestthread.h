/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerJsonDbRequestManager;
class QOrganizerJsonDbEngine;
class QOrganizerJsonDbDataStorage;

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

public slots:
    void handleRequest(QOrganizerAbstractRequest* req);

signals:
    void initialized();

protected:
    virtual void run();

private slots:
    // Since these three slots are triggered either by signals from data storage for notifications
    // from JsonDb, or by the timer, and they are executed only in this thread, no mutex is needed.
    void onItemAdded(const QOrganizerItemId &itemId);
    void onItemChanged(const QOrganizerItemId &itemId);
    void onItemRemoved(const QOrganizerItemId &itemId);
    void onCollectionAdded(const QOrganizerCollectionId &collectionId);
    void onCollectionChanged(const QOrganizerCollectionId &collectionId);
    void onCollectionRemoved(const QOrganizerCollectionId &collectionId);
    void onTimeout();

private:
    void handleItemSaveRequest(QOrganizerItemSaveRequest* saveReq);
    void handleItemFetchRequest(QOrganizerItemFetchRequest* fetchReq);
    void handleItemIdFetchRequest(QOrganizerItemIdFetchRequest* idFetchReq);
    void handleItemFetchByIdRequest(QOrganizerItemFetchByIdRequest* fetchByIdReq);
    void handleItemRemoveRequest(QOrganizerItemRemoveRequest* removeReq);
    void handleCollectionSaveRequest(QOrganizerCollectionSaveRequest* collectionSaveReq);
    void handleCollectionFetchRequest(QOrganizerCollectionFetchRequest* collectionFetchReq);
    void handleCollectionRemoveRequest(QOrganizerCollectionRemoveRequest* collectionRemoveReq);
    void initDefaultCollection();

    // Member variables
    QOrganizerJsonDbEngine* m_engine;
    QOrganizerJsonDbDataStorage* m_storage;
    QOrganizerJsonDbRequestManager* m_requestMgr;
    //Mutex to make the request state changes atomic
    QMutex* m_reqStateMutex;

    // Handle item / collection changes
    // They are only used by the notification system, so no mutex is needed.
    static const int TIMEOUT_INTERVAL;
    QTimer *m_timer;
    QOrganizerItemChangeSet m_ics;
    QOrganizerCollectionChangeSet m_ccs;

    void startTimer();  // Only used by onItemChanged() and onCollectionChanged()
    // Prevent infinite alarm remove loop
    static const int ALARM_REMOVE_MAXLOOP;
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBREQUESTTHREAD_H
