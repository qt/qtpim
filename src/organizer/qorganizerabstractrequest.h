/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QORGANIZERABSTRACTREQUEST_H
#define QORGANIZERABSTRACTREQUEST_H

#include <qorganizermanager.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerManagerEngine;
class QOrganizerAbstractRequestPrivate;
class Q_ORGANIZER_EXPORT QOrganizerAbstractRequest : public QObject
{
    Q_OBJECT

public:
    ~QOrganizerAbstractRequest();

    enum State {
        InactiveState = 0,
        ActiveState,
        CanceledState,
        FinishedState
    };

    State state() const;
    bool isInactive() const;
    bool isActive() const;
    bool isFinished() const;
    bool isCanceled() const;

    QOrganizerManager::Error error() const;

    enum RequestType {
        InvalidRequest = 0,
        ItemOccurrenceFetchRequest,
        ItemFetchRequest,
        ItemFetchForExportRequest,
        ItemIdFetchRequest,
        ItemFetchByIdRequest,
        ItemRemoveRequest,
        ItemRemoveByIdRequest,
        ItemSaveRequest,
        CollectionFetchRequest,
        CollectionRemoveRequest,
        CollectionSaveRequest
    };

    RequestType type() const;

    QOrganizerManager* manager() const;
    void setManager(QOrganizerManager *manager);

    enum StorageLocation {
        UserDataStorage = 0x1,
        SystemStorage = 0x2
    };
    Q_DECLARE_FLAGS(StorageLocations, StorageLocation)

public Q_SLOTS:
    bool start();
    bool cancel();
    bool waitForFinished(int msecs = 0);

Q_SIGNALS:
    void stateChanged(QOrganizerAbstractRequest::State newState);
    void resultsAvailable();

protected:
    QOrganizerAbstractRequest(QOrganizerAbstractRequestPrivate *other, QObject *parent = 0);
    QOrganizerAbstractRequestPrivate *d_ptr;

private:
    QOrganizerAbstractRequest(QObject *parent = 0) : QObject(parent), d_ptr(0) {}
    Q_DISABLE_COPY(QOrganizerAbstractRequest)
    friend class QOrganizerManagerEngine;
    friend class QOrganizerAbstractRequestPrivate;

#ifndef QT_NO_DEBUG_STREAM
    friend Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerAbstractRequest &request);
#endif
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QOrganizerAbstractRequest::StorageLocations)

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERABSTRACTREQUEST_H
