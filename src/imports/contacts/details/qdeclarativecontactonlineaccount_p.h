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

#ifndef QDECLARATIVECONTACTONLINEACCOUNT_H
#define QDECLARATIVECONTACTONLINEACCOUNT_H

#include <QtContacts/qcontactonlineaccount.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactOnlineAccount : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString accountUri READ accountUri WRITE setAccountUri NOTIFY valueChanged)
    Q_PROPERTY(QString serviceProvider READ serviceProvider WRITE  setServiceProvider NOTIFY valueChanged)
    Q_PROPERTY(QStringList capabilities READ capabilities WRITE  setCapabilities NOTIFY valueChanged)
    Q_PROPERTY(QList<int> subTypes READ subTypes WRITE  setSubTypes NOTIFY valueChanged)
    Q_PROPERTY(OnlineAccountProtocol protocol READ protocol WRITE setProtocol NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(OnlineAccountSubType)
    Q_ENUMS(OnlineAccountProtocol)

    Q_CLASSINFO("DefaultProperty", "accountUri")
public:
    enum FieldType {
        AccountUri = QContactOnlineAccount::FieldAccountUri,
        ServiceProvider = QContactOnlineAccount::FieldServiceProvider,
        Protocol = QContactOnlineAccount::FieldProtocol,
        Capabilities = QContactOnlineAccount::FieldCapabilities,
        SubTypes = QContactOnlineAccount::FieldSubTypes
    };

    enum OnlineAccountSubType {
        Sip = QContactOnlineAccount::SubTypeSip,
        SipVoip = QContactOnlineAccount::SubTypeSipVoip,
        Impp = QContactOnlineAccount::SubTypeImpp,
        VideoShare= QContactOnlineAccount::SubTypeVideoShare
    };

    enum OnlineAccountProtocol {
        Unknown =  QContactOnlineAccount::ProtocolUnknown,
        Aim = QContactOnlineAccount::ProtocolAim,
        Icq = QContactOnlineAccount::ProtocolIcq,
        Irc = QContactOnlineAccount::ProtocolIrc,
        Jabber = QContactOnlineAccount::ProtocolJabber,
        Msn = QContactOnlineAccount::ProtocolMsn,
        Qq = QContactOnlineAccount::ProtocolQq,
        Skype = QContactOnlineAccount::ProtocolSkype,
        Yahoo = QContactOnlineAccount::ProtocolYahoo
    };

    QDeclarativeContactOnlineAccount(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactOnlineAccount());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }

    DetailType detailType() const
    {
        return QDeclarativeContactDetail::OnlineAccount;
    }

    void setAccountUri(const QString& v)
    {
        if (!readOnly() && v != accountUri()) {
            detail().setValue(QContactOnlineAccount::FieldAccountUri, v);
            emit valueChanged();
        }
    }
    QString accountUri() const {return detail().value(QContactOnlineAccount::FieldAccountUri).toString();}

    void setServiceProvider(const QString& v)
    {
        if (!readOnly() && v != serviceProvider()) {
            detail().setValue(QContactOnlineAccount::FieldServiceProvider, v);
            emit valueChanged();
        }
    }
    QString serviceProvider() const {return detail().value(QContactOnlineAccount::FieldServiceProvider).toString();}

    void setCapabilities(const QStringList& v)
    {
        if (!readOnly() && v.toSet() != capabilities().toSet()) {
            detail().setValue(QContactOnlineAccount::FieldCapabilities, v);
            emit valueChanged();
        }
    }
    QStringList capabilities() const {return detail().value<QStringList>(QContactOnlineAccount::FieldCapabilities);}

    void setSubTypes(const QList<int>& subTypes)
    {
        QList<int> oldList = detail().value< QList<int> >(QContactOnlineAccount::FieldSubTypes);

        if (!readOnly() && subTypes.toSet() != oldList.toSet()) {
            detail().setValue(QContactOnlineAccount::FieldSubTypes, QVariant::fromValue(subTypes));
            emit valueChanged();
        }
    }

    QList<int> subTypes() const
    {
        return detail().value< QList<int> >(QContactOnlineAccount::FieldSubTypes);
    }

    void setProtocol(OnlineAccountProtocol v) {
        if (!readOnly() && v != protocol()) {
            detail().setValue(QContactOnlineAccount::FieldProtocol, static_cast<int>(v));
            emit valueChanged();
        }
    }

    OnlineAccountProtocol protocol() const
    {
        return static_cast<OnlineAccountProtocol>(detail().value<int>(QContactOnlineAccount::FieldProtocol));
    }

signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactOnlineAccount)

#endif // QDECLARATIVECONTACTONLINEACCOUNT_H
