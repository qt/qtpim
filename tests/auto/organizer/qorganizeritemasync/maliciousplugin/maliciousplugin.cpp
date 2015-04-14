/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "maliciousplugin_p.h"

#include <QThread>
#include <QMutex>
#include <QSet>
#include <QDebug>

#include <QtOrganizer/qorganizeritemrequests.h>
#include <QtOrganizer/qorganizermanager.h>

#ifndef MALICIOUSPLUGINTARGET
#define MALICIOUSPLUGINTARGET organizer_maliciousplugin
#endif

#ifndef MALICIOUSPLUGINNAME
#define MALICIOUSPLUGINNAME maliciousplugin
#endif

#define makestr(x) (#x)
#define makename(x) makestr(x)

QTORGANIZER_USE_NAMESPACE

class MaliciousThreadObject : public QObject
{
    Q_OBJECT

public:
    MaliciousThreadObject() {}

public slots:
    void activateRequest(QOrganizerAbstractRequest *req)
    {
        mutex.lock();
        if (activeRequests.contains(req))
            QOrganizerManagerEngine::updateRequestState(req, QOrganizerAbstractRequest::ActiveState);
        mutex.unlock();
    }

    void finishRequest(QOrganizerAbstractRequest *req)
    {
        QOrganizerManager::Error errorResult = QOrganizerManager::NoError;
        QMap<int, QOrganizerManager::Error> errorMap;
        QList<QOrganizerItemId> idResult;
        QList<QOrganizerItem> itemResult;
        QList<QOrganizerCollection> collectionResult;

        mutex.lock();
        if (activeRequests.contains(req)) {
            switch (req->type()) {
            case QOrganizerAbstractRequest::ItemSaveRequest:
                QOrganizerManagerEngine::updateItemSaveRequest(static_cast<QOrganizerItemSaveRequest *>(req), itemResult, errorResult, errorMap, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemFetchRequest:
                QOrganizerManagerEngine::updateItemFetchRequest(static_cast<QOrganizerItemFetchRequest *>(req), itemResult, errorResult, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemIdFetchRequest:
                QOrganizerManagerEngine::updateItemIdFetchRequest(static_cast<QOrganizerItemIdFetchRequest *>(req), idResult, errorResult, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemFetchByIdRequest:
                QOrganizerManagerEngine::updateItemFetchByIdRequest(static_cast<QOrganizerItemFetchByIdRequest *>(req), itemResult, errorResult, errorMap, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemFetchForExportRequest:
                QOrganizerManagerEngine::updateItemFetchForExportRequest(static_cast<QOrganizerItemFetchForExportRequest *>(req), itemResult, errorResult, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemOccurrenceFetchRequest:
                QOrganizerManagerEngine::updateItemOccurrenceFetchRequest(static_cast<QOrganizerItemOccurrenceFetchRequest *>(req), itemResult, errorResult, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemRemoveRequest:
                QOrganizerManagerEngine::updateItemRemoveRequest(static_cast <QOrganizerItemRemoveRequest *>(req), errorResult, errorMap, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::ItemRemoveByIdRequest:
                QOrganizerManagerEngine::updateItemRemoveByIdRequest(static_cast <QOrganizerItemRemoveByIdRequest *>(req), errorResult, errorMap, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::CollectionSaveRequest:
                QOrganizerManagerEngine::updateCollectionSaveRequest(static_cast <QOrganizerCollectionSaveRequest *>(req), collectionResult, errorResult, errorMap, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::CollectionFetchRequest:
                QOrganizerManagerEngine::updateCollectionFetchRequest(static_cast<QOrganizerCollectionFetchRequest *>(req), collectionResult, errorResult, QOrganizerAbstractRequest::FinishedState);
                break;

            case QOrganizerAbstractRequest::CollectionRemoveRequest:
                QOrganizerManagerEngine::updateCollectionRemoveRequest(static_cast<QOrganizerCollectionRemoveRequest *>(req), errorResult, errorMap, QOrganizerAbstractRequest::FinishedState);
                break;

            default:
                break;
            }
        }
        activeRequests.remove(req);
        mutex.unlock();
    }

public:
    QMutex mutex;
    QSet<QOrganizerAbstractRequest *> activeRequests;
};


class MaliciousThread : public QThread
{
    Q_OBJECT

public:
    MaliciousThread();
    ~MaliciousThread();

    QObject *eventLoopQuitHack;
};

MaliciousThread::MaliciousThread()
{
    eventLoopQuitHack = new QObject;
    eventLoopQuitHack->moveToThread(this);
    connect(eventLoopQuitHack, SIGNAL(destroyed(QObject*)), SLOT(quit()), Qt::DirectConnection);
}

MaliciousThread::~MaliciousThread()
{
    eventLoopQuitHack->deleteLater();
    wait();
}


MaliciousAsyncManagerEngine::MaliciousAsyncManagerEngine()
    : QOrganizerManagerEngine()
{
    thread = new MaliciousThread();
    threadObject = new MaliciousThreadObject();
    threadObject->moveToThread(thread);

    connect(this, SIGNAL(doStartRequest(QOrganizerAbstractRequest*)), threadObject, SLOT(activateRequest(QOrganizerAbstractRequest*)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(doFinishRequest(QOrganizerAbstractRequest*)), threadObject, SLOT(finishRequest(QOrganizerAbstractRequest*)), Qt::BlockingQueuedConnection);

    thread->start();
}

MaliciousAsyncManagerEngine::~MaliciousAsyncManagerEngine()
{
    delete thread;
    delete threadObject;
}

QString MaliciousAsyncManagerEngine::managerName() const
{
    return QString(makename(MALICIOUSPLUGINNAME));
}

bool MaliciousAsyncManagerEngine::startRequest(QOrganizerAbstractRequest *req)
{
    threadObject->mutex.lock();
    threadObject->activeRequests.insert(req);
    threadObject->mutex.unlock();

    // Spawn a thread to do stuff on another thread
    emit doStartRequest(req);
    emit doFinishRequest(req);

    return true;
}

bool MaliciousAsyncManagerEngine::cancelRequest(QOrganizerAbstractRequest *req)
{
    updateRequestState(req, QOrganizerAbstractRequest::CanceledState);
    QOrganizerManagerEngine::cancelRequest(req);
    return true;
}

void MaliciousAsyncManagerEngine::requestDestroyed(QOrganizerAbstractRequest *req)
{
    threadObject->mutex.lock();
    threadObject->activeRequests.remove(req);
    threadObject->mutex.unlock();
    QOrganizerManagerEngine::requestDestroyed(req);
}


QString MaliciousEngineFactory::managerName() const
{
    return QString(makename(MALICIOUSPLUGINNAME));
}

QOrganizerManagerEngine *MaliciousEngineFactory::engine(const QMap<QString, QString> &parameters, QOrganizerManager::Error *error)
{
    Q_UNUSED(parameters);
    *error = QOrganizerManager::NoError;
    return new MaliciousAsyncManagerEngine();
}

#include "maliciousplugin.moc"
