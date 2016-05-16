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

#ifndef QDECLARATIVECONTACTPHONENUMBER_H
#define QDECLARATIVECONTACTPHONENUMBER_H

#include <QtContacts/qcontactphonenumber.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class  QDeclarativeContactPhoneNumber : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString number READ number WRITE setNumber NOTIFY valueChanged)
    Q_PROPERTY(QList<int> subTypes READ subTypes WRITE setSubTypes NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(PhoneNumberSubType)

    Q_CLASSINFO("DefaultProperty", "number")
public:
    enum FieldType {
        Number = QContactPhoneNumber::FieldNumber,
        SubTypes = QContactPhoneNumber::FieldSubTypes
    };

    enum PhoneNumberSubType {
        Landline = QContactPhoneNumber::SubTypeLandline,
        Mobile = QContactPhoneNumber::SubTypeMobile,
        Fax = QContactPhoneNumber::SubTypeFax,
        Pager = QContactPhoneNumber::SubTypePager,
        Voice = QContactPhoneNumber::SubTypeVoice,
        Modem = QContactPhoneNumber::SubTypeModem,
        Video = QContactPhoneNumber::SubTypeVideo,
        Car = QContactPhoneNumber::SubTypeCar,
        BulletinBoardSystem = QContactPhoneNumber::SubTypeBulletinBoardSystem,
        MessagingCapable = QContactPhoneNumber::SubTypeMessagingCapable,
        Assistant = QContactPhoneNumber::SubTypeAssistant,
        DtmfMenu = QContactPhoneNumber::SubTypeDtmfMenu
    };

    QDeclarativeContactPhoneNumber(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactPhoneNumber());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::PhoneNumber;
    }

    void setNumber(const QString& v)
    {
        if (!readOnly() && v != number()) {
            detail().setValue(QContactPhoneNumber::FieldNumber, v);
            emit valueChanged();
        }
    }
    QString number() const {return detail().value(QContactPhoneNumber::FieldNumber).toString();}


    void setSubTypes(const QList<int>& subTypes)
    {
        QList<int> oldList = detail().value< QList<int> >(QContactPhoneNumber::FieldSubTypes);

        if (!readOnly() && subTypes.toSet() != oldList.toSet()) {
            detail().setValue(QContactPhoneNumber::FieldSubTypes, QVariant::fromValue(subTypes));
            emit valueChanged();
        }
    }

    QList<int> subTypes() const
    {
        return detail().value< QList<int> >(QContactPhoneNumber::FieldSubTypes);
    }

signals:
    void valueChanged();

};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactPhoneNumber)

#endif // QDECLARATIVECONTACTPHONENUMBER_H
