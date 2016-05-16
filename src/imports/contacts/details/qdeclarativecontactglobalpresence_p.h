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

#ifndef QDECLARATIVECONTACTGLOBALPRESENCE_H
#define QDECLARATIVECONTACTGLOBALPRESENCE_H

#include <QtContacts/qcontactglobalpresence.h>

#include "qdeclarativecontactdetail_p.h"
#include "qdeclarativecontactpresence_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactGlobalPresence : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timestamp READ timestamp WRITE setTimestamp NOTIFY valueChanged)
    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeContactPresence::PresenceStateType state READ presenceState WRITE setPresenceState NOTIFY valueChanged)
    Q_PROPERTY(QString stateText READ presenceStateText WRITE setPresenceStateText NOTIFY valueChanged)
    Q_PROPERTY(QUrl imageUrl READ presenceStateImageUrl WRITE setPresenceStateImageUrl NOTIFY valueChanged)
    Q_PROPERTY(QString customMessage READ customMessage WRITE setCustomMessage NOTIFY valueChanged)
    Q_CLASSINFO("DefaultProperty", "state")
    Q_ENUMS(FieldType)
public:
    enum FieldType {
        Timestamp = QContactGlobalPresence::FieldTimestamp,
        Nickname = QContactGlobalPresence::FieldNickname,
        State = QContactGlobalPresence::FieldPresenceState,
        StateText = QContactGlobalPresence::FieldPresenceStateText,
        ImageUrl = QContactGlobalPresence::FieldPresenceStateImageUrl,
        CustomMessage = QContactGlobalPresence::FieldCustomMessage
    };

    QDeclarativeContactGlobalPresence(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactGlobalPresence());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::GlobalPresence;
    }

    void setTimestamp(const QDateTime& v)
    {
        if (!readOnly() && v != timestamp()) {
            detail().setValue(QContactGlobalPresence::FieldTimestamp, v);
            emit valueChanged();
        }
    }
    QDateTime timestamp() const {return detail().value<QDateTime>(QContactGlobalPresence::FieldTimestamp);}
    void setNickname(const QString& v)
    {
        if (!readOnly() && v != nickname()) {
            detail().setValue(QContactGlobalPresence::FieldNickname, v);
            emit valueChanged();
        }
    }
    QString nickname() const {return detail().value(QContactGlobalPresence::FieldNickname).toString();}
    void setPresenceState(QDeclarativeContactPresence::PresenceStateType v)
    {
        if (!readOnly() && v != presenceState()) {
            detail().setValue(QContactGlobalPresence::FieldPresenceState, static_cast<int>(v));
            emit valueChanged();
        }
    }

    QDeclarativeContactPresence::PresenceStateType presenceState() const
    {
        return static_cast<QDeclarativeContactPresence::PresenceStateType>(detail().value<int>(QContactGlobalPresence::FieldPresenceState));

    }
    void setPresenceStateText(const QString& v)
    {
        if (!readOnly() && v != presenceStateText()) {
            detail().setValue(QContactGlobalPresence::FieldPresenceStateText, v);
            emit valueChanged();
        }
    }
    QString presenceStateText() const {return detail().value(QContactGlobalPresence::FieldPresenceStateText).toString();}
    void setPresenceStateImageUrl(const QUrl& v)
    {
        if (!readOnly() && v != presenceStateImageUrl())  {
            detail().setValue(QContactGlobalPresence::FieldPresenceStateImageUrl, v);
            emit valueChanged();
        }
    }
    QUrl presenceStateImageUrl() const {return detail().value<QUrl>(QContactGlobalPresence::FieldPresenceStateImageUrl);}
    void setCustomMessage(const QString& v)
    {
        if (!readOnly() && v != customMessage()) {
            detail().setValue(QContactGlobalPresence::FieldCustomMessage, v);
            emit valueChanged();
        }
    }
    QString customMessage() const {return detail().value(QContactGlobalPresence::FieldCustomMessage).toString();}
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactGlobalPresence)

#endif // QDECLARATIVECONTACTGLOBALPRESENCE_H
