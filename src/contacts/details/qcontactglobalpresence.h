/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QCONTACTGLOBALPRESENCE_H
#define QCONTACTGLOBALPRESENCE_H

#include <QtCore/qdatetime.h>
#include <QtCore/qurl.h>

#include <QtContacts/qcontactdetail.h>
#include <QtContacts/qcontactpresence.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactFilter;

/* Leaf class */
class Q_CONTACTS_EXPORT QContactGlobalPresence : public QContactDetail
{
public:
#ifndef Q_QDOC
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactGlobalPresence)
#else
    static const DetailType Type;
#endif

    enum GlobalPresenceField {
        FieldTimestamp = 0,
        FieldNickname,
        FieldPresenceState,
        FieldPresenceStateText,
        FieldPresenceStateImageUrl,
        FieldCustomMessage
    };

    void setTimestamp(const QDateTime& timestamp) {setValue(FieldTimestamp, timestamp);}
    QDateTime timestamp() const {return value<QDateTime>(FieldTimestamp);}
    void setNickname(const QString& nickname) {setValue(FieldNickname, nickname);}
    QString nickname() const {return value(FieldNickname).toString();}
    void setPresenceState(QContactPresence::PresenceState presenceState) {setValue(FieldPresenceState, static_cast<int>(presenceState));}
    QContactPresence::PresenceState presenceState() const {return static_cast<QContactPresence::PresenceState>(value<int>(FieldPresenceState));}
    void setPresenceStateText(const QString& presenceStateText) {setValue(FieldPresenceStateText, presenceStateText);}
    QString presenceStateText() const {return value(FieldPresenceStateText).toString();}
    void setPresenceStateImageUrl(const QUrl& presenceStateImageUrl) {setValue(FieldPresenceStateImageUrl, presenceStateImageUrl);}
    QUrl presenceStateImageUrl() const {return value<QUrl>(FieldPresenceStateImageUrl);}
    void setCustomMessage(const QString& customMessage) {setValue(FieldCustomMessage, customMessage);}
    QString customMessage() const {return value(FieldCustomMessage).toString();}

    // convenience filtering functions
    static QContactFilter match(QContactPresence::PresenceState state);
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTGLOBALPRESENCE_H
