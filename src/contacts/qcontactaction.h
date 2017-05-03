/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTACTION_H
#define QCONTACTACTION_H

#include <QtCore/qmap.h>
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvariant.h>

#include <QtContacts/qcontact.h>
#include <QtContacts/qcontactactiontarget.h>
#include <QtContacts/qcontactactiondescriptor.h>
#include <QtContacts/qcontactdetail.h>
#include <QtContacts/qcontactfilter.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactActionData;
class Q_CONTACTS_EXPORT QContactAction : public QObject
{
    Q_OBJECT

public:
    virtual ~QContactAction() = 0;

    /* Initiate the asynchronous action on the given list of contacts (and optionally, per-contact-details) with the given parameters */
    virtual bool invokeAction(const QList<QContactActionTarget>& targets, const QVariantMap& parameters = QVariantMap()) = 0;

    /* The possible states of an action */
    enum State {
        InactiveState = 0,      // operation not yet started
        ActiveState,            // operation started, not yet finished
        FinishedState,          // operation successfully completed
        FinishedDetachedState,  // operation started, no further information available - name under discussion.
        FinishedWithErrorState  // operation finished, but error occurred
    };

    virtual State state() const = 0;

    /* Returns the most recently received result, or an empty QVariantMap if no results received */
    virtual QVariantMap results() const = 0;

    /* Convenience functions */
    bool invokeAction(const QContactActionTarget& target, const QVariantMap& parameters = QVariantMap())
    {
        return invokeAction(QList<QContactActionTarget>() << target, parameters);
    }
    bool invokeAction(const QContact& contact, const QContactDetail& detail = QContactDetail(), const QVariantMap& parameters = QVariantMap())
    {
        return invokeAction(QList<QContactActionTarget>() << QContactActionTarget(contact, detail), parameters);
    }

    // common actions
    inline static const QString ActionCall() {return QStringLiteral("call");};
    inline static const QString ActionEmail() {return QStringLiteral("email");};
    inline static const QString ActionSms() {return QStringLiteral("sms");};
    inline static const QString ActionMms() {return QStringLiteral("mms");};
    inline static const QString ActionChat() {return QStringLiteral("chat");};
    inline static const QString ActionVideoCall() {return QStringLiteral("videocall");};
    inline static const QString ActionOpenInEditor() {return QStringLiteral("edit");};
    inline static const QString ActionOpenInViewer() {return QStringLiteral("view");};

Q_SIGNALS:
    void stateChanged(QContactAction::State);
    void resultsAvailable();

public:
    /* return a list of names of actions which are available */
    static QStringList availableActions(const QString& serviceName = QString());

    /* return a list of descriptors for action implementations matching the given criteria */
    static QList<QContactActionDescriptor> actionDescriptors(const QString& actionName = QString());

    /* return a pointer to an implementation of the action identified by the given descriptor */
    static QContactAction* action(const QContactActionDescriptor& descriptor);
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTACTION_H
