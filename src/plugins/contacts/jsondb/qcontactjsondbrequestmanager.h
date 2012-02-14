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

#ifndef QCONTACTJSONDBREQUESTMANAGER_H
#define QCONTACTJSONDBREQUESTMANAGER_H
//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include <QMap>
#include <QMutex>
#include <QWaitCondition>
#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbwatcher.h>
#include <QtJsonDb/qjsondbrequest.h>

#include "qcontact.h"
#include "qcontactrequests.h"

QT_USE_NAMESPACE_JSONDB

QTCONTACTS_BEGIN_NAMESPACE

class QContactRequestData
{
    public:
    QContactRequestData() {}
    QList<QContact> m_contactList;
    QMap<QJsonDbRequest*, int> m_jsonDbRequestMap;
    QWaitCondition* m_waitCondition;
};

class QContactJsonDbRequestManager : public QObject
{
    Q_OBJECT

public:

    enum RequestType {
        InvalidRequest = 0,
        OrphanRequest,
        PrefetchForSaveRequest,
        SaveRequest,
        UpdateRequest,
        FetchRequest,
        ContactIdFetchRequest,
        RemoveRequest
    };

    QContactJsonDbRequestManager();
    ~QContactJsonDbRequestManager();

    void addRequest(QContactAbstractRequest* req, QList<QContact> contacts = QList<QContact>());
    void removeRequest(QContactAbstractRequest* req);

    void addPrefetchRequest(QContactFetchRequest *prefetchReq, QContactSaveRequest *saveReq);
    QContactSaveRequest* removePrefetchRequest(QContactFetchRequest *prefetchReq);

    void addRequest(QJsonDbRequest *jsonDbRequest, RequestType requestType, QContactAbstractRequest *req = 0, int contactIndex = -1);
    QContactAbstractRequest* removeRequest(QJsonDbRequest *jsonDbRequest, RequestType &requestType, int &contactIndex, QString *partitionName);

    bool setWaitCondition(QContactAbstractRequest* req, QWaitCondition* waitCondition);
    QWaitCondition* waitCondition(QContactAbstractRequest* req);
    void removeWaitCondition(QContactAbstractRequest* req);

    void addContact(QContactAbstractRequest* req, QContact item, int contactIndex);
    QList<QContact> contacts(QContactAbstractRequest* req);

    bool isRequestCompleted(QContactAbstractRequest* req);
    bool pendingPrefetchRequests(QContactSaveRequest *saveReq);
    bool contains(QContactAbstractRequest* req) const;

signals:

public slots:

private:
    QMap<QContactAbstractRequest*, QContactRequestData* > m_activeRequests;
    QMap<QContactAbstractRequest*, QWaitCondition* > m_inactiveRequests;
    QMap<QContactFetchRequest*, QContactSaveRequest*> m_prefetchRequestsMap;
    QMap<QJsonDbRequest*, QContactJsonDbRequestManager::RequestType> m_jsonDbRequestTypeMap;
    QMutex* m_operationMutex;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBREQUESTMANAGER_H
