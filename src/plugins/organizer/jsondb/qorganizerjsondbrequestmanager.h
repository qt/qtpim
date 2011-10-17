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

#ifndef QORGANIZERJSONDBREQUESTMANAGER_H
#define QORGANIZERJSONDBREQUESTMANAGER_H

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

#include "qorganizeritem.h"
#include "qorganizercollection.h"
#include "qorganizerabstractrequest.h"

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerJsonDbRequestData;

class QOrganizerJsonDbRequestManager : public QObject
{
    Q_OBJECT

public:

    // HandlingStatus gives info about in which phase the handling of the request is.
    // The status is not necessarily in sync with the request status (QOrganizerAbstractRequest::state())
    enum HandlingStatus {
        Inactive = 0,   // request handling not yet started
        Active,         // request handling started, not yet finished
        Canceled,       // request handling should be finished due to cancellation
        Deleted,        // request deleted
        Invalid         // status of a non-existing request
    };

    QOrganizerJsonDbRequestManager();
    ~QOrganizerJsonDbRequestManager();

    void addRequest(QOrganizerAbstractRequest* req);
    void removeRequest(QOrganizerAbstractRequest* req);

    void addTransaction(QOrganizerAbstractRequest* req, int trId, int index = -1);
    QOrganizerAbstractRequest* removeTransaction(int trId, int& index);

    bool setActive(QOrganizerAbstractRequest* req);
    bool setDeleted(QOrganizerAbstractRequest* req);

    bool setWaitCondition(QOrganizerAbstractRequest* req, QWaitCondition* waitCondition);
    QWaitCondition* waitCondition(QOrganizerAbstractRequest* req);
    void removeWaitCondition(QOrganizerAbstractRequest* req);

    bool updateRequestData(QOrganizerAbstractRequest* req, QOrganizerManager::Error error, int index,
                           QOrganizerItem item = QOrganizerItem(), QOrganizerCollection collection = QOrganizerCollection());
    bool requestData(QOrganizerAbstractRequest* req, QOrganizerManager::Error* latestError, QMap<int, QOrganizerManager::Error>* errorMap,
                     QList<QOrganizerItem>* items = 0, QList<QOrganizerCollection>* collections = 0);

    QOrganizerJsonDbRequestManager::HandlingStatus requestStatus(QOrganizerAbstractRequest* req);
    bool isRequestCompleted(QOrganizerAbstractRequest* req);

private:
    QMap<QOrganizerAbstractRequest*, QOrganizerJsonDbRequestData* > m_requests;
    QMutex* m_operationMutex;
};

class QOrganizerJsonDbRequestData
{
    public:

    QOrganizerJsonDbRequestData() {}
    QList<QOrganizerItem> m_itemList;
    QList<QOrganizerCollection> m_collectionList;
    QOrganizerManager::Error m_latestError;
    QMap<int, QOrganizerManager::Error> m_errorMap;
    QMap<int, int> m_transactionMap;
    QWaitCondition* m_waitCondition;
    QOrganizerJsonDbRequestManager::HandlingStatus m_status;
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBREQUESTMANAGER_H
