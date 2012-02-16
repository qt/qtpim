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


#ifndef QDECLARATIVECONTACTPHONENUMBER_H
#define QDECLARATIVECONTACTPHONENUMBER_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontactphonenumber.h"
#include <QSet>

QTCONTACTS_BEGIN_NAMESPACE

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

    QDeclarativeContactPhoneNumber(QObject* parent = 0)
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

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactPhoneNumber))

#endif

