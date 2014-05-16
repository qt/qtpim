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

#ifndef QCONTACTABSTRACTREQUEST_H
#define QCONTACTABSTRACTREQUEST_H

#include <QtCore/qobject.h>

#include <QtContacts/qcontactmanager.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactManagerEngine;

class QContactAbstractRequestPrivate;
class Q_CONTACTS_EXPORT QContactAbstractRequest : public QObject
{
    Q_OBJECT

public:
    ~QContactAbstractRequest();

    Q_ENUMS(State)
    enum State {
        InactiveState = 0,   // operation not yet started
        ActiveState,         // operation started, not yet finished
        CanceledState,       // operation is finished due to cancellation
        FinishedState        // operation either completed successfully or failed.  No further results will become available.
    };

    State state() const; // replaces status()
    inline bool isInactive() const { return state() == QContactAbstractRequest::InactiveState; }
    inline bool isActive() const { return state() == QContactAbstractRequest::ActiveState; }
    inline bool isFinished() const { return state() == QContactAbstractRequest::FinishedState; }
    inline bool isCanceled() const { return state() == QContactAbstractRequest::CanceledState; }

    QContactManager::Error error() const;

    Q_ENUMS(RequestType)
    enum RequestType {
        InvalidRequest = 0,
        ContactFetchRequest,
        ContactIdFetchRequest,
        ContactRemoveRequest,
        ContactSaveRequest,
        RelationshipFetchRequest,
        RelationshipRemoveRequest,
        RelationshipSaveRequest,
        ContactFetchByIdRequest
    };

    RequestType type() const;

    /* Which manager we want to perform the asynchronous request */
    QContactManager* manager() const;
    void setManager(QContactManager* manager);

public Q_SLOTS:
    /* Verbs */
    bool start();
    bool cancel();

    /* waiting for stuff */
    bool waitForFinished(int msecs = 0);

Q_SIGNALS:
    void stateChanged(QContactAbstractRequest::State newState);
    void resultsAvailable();

protected:
    QContactAbstractRequest(QContactAbstractRequestPrivate* otherd, QObject* parent = 0);
    QContactAbstractRequestPrivate* d_ptr;

private:
    QContactAbstractRequest(QObject* parent_ = 0) : QObject(parent_), d_ptr(0) {}
    Q_DISABLE_COPY(QContactAbstractRequest)
    friend class QContactManagerEngine;
    friend class QContactAbstractRequestPrivate;

#ifndef QT_NO_DEBUG_STREAM
    Q_CONTACTS_EXPORT friend QDebug operator<<(QDebug dbg, const QContactAbstractRequest& request);
#endif

};

#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactAbstractRequest& request);
#endif

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTABSTRACTREQUEST_H
