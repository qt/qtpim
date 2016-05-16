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

#ifndef QDECLARATIVECONTACTADDRESS_H
#define QDECLARATIVECONTACTADDRESS_H

#include <QtContacts/qcontactaddress.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactAddress : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString street READ street WRITE setStreet NOTIFY valueChanged)
    Q_PROPERTY(QString locality READ locality WRITE setLocality NOTIFY valueChanged)
    Q_PROPERTY(QString region READ region WRITE setRegion  NOTIFY valueChanged)
    Q_PROPERTY(QString postcode READ postcode WRITE setPostcode  NOTIFY valueChanged)
    Q_PROPERTY(QString country READ country WRITE setCountry  NOTIFY valueChanged)
    Q_PROPERTY(QList<int> subTypes READ subTypes WRITE setSubTypes NOTIFY valueChanged)
    Q_PROPERTY(QString postOfficeBox READ postOfficeBox WRITE setPostOfficeBox  NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(AddressSubType)
public:
    enum FieldType {
        Street = QContactAddress::FieldStreet,
        Locality = QContactAddress::FieldLocality,
        Region = QContactAddress::FieldRegion,
        Postcode = QContactAddress::FieldPostcode,
        Country = QContactAddress::FieldCountry,
        SubTypes = QContactAddress::FieldSubTypes,
        PostOfficeBox = QContactAddress::FieldPostOfficeBox
    };

    enum AddressSubType {
        Parcel = QContactAddress::SubTypeParcel,
        Postal = QContactAddress::SubTypePostal,
        Domestic = QContactAddress::SubTypeDomestic,
        International = QContactAddress::SubTypeInternational
    };

    QDeclarativeContactAddress(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactAddress());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }

    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Address;
    }

    void setStreet(const QString& v)
    {
        if (!readOnly() && v != street()) {
            detail().setValue(QContactAddress::FieldStreet, v);
            emit valueChanged();
        }
    }
    QString street() const {return detail().value(QContactAddress::FieldStreet).toString();}
    void setLocality(const QString& v)
    {
        if (!readOnly() && v != locality()) {
            detail().setValue(QContactAddress::FieldLocality, v);
            emit valueChanged();
        }
    }
    QString locality() const {return detail().value(QContactAddress::FieldLocality).toString();}
    void setRegion(const QString& v)
    {
        if (!readOnly() && v != region()) {
            detail().setValue(QContactAddress::FieldRegion, v);
            emit valueChanged();
        }
    }
    QString region() const {return detail().value(QContactAddress::FieldRegion).toString();}
    void setPostcode(const QString& v)
    {
        if (!readOnly() && v != postcode()) {
            detail().setValue(QContactAddress::FieldPostcode, v);
            emit valueChanged();
        }
    }
    QString postcode() const {return detail().value(QContactAddress::FieldPostcode).toString();}
    void setCountry(const QString& v)
    {
        if (!readOnly() && v != country()) {
            detail().setValue(QContactAddress::FieldCountry, v);
            emit valueChanged();
        }
    }
    QString country() const {return detail().value(QContactAddress::FieldCountry).toString();}
    void setPostOfficeBox(const QString& v)
    {
        if (!readOnly() && v != postOfficeBox()) {
            detail().setValue(QContactAddress::FieldPostOfficeBox, v);
            emit valueChanged();
        }
    }
    QString postOfficeBox() const {return detail().value(QContactAddress::FieldPostOfficeBox).toString();}

    void setSubTypes(const QList<int>& subTypes)
    {
        QList<int>  oldList = detail().value< QList<int>  >(QContactAddress::FieldSubTypes);

        if (!readOnly() && subTypes.toSet() != oldList.toSet()) {
            detail().setValue(QContactAddress::FieldSubTypes, QVariant::fromValue(subTypes));
            emit valueChanged();
        }
    }

    QList<int> subTypes() const
    {
        return detail().value< QList<int> >(QContactAddress::FieldSubTypes);
    }
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactAddress)

#endif // QDECLARATIVECONTACTADDRESS_H
