/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef QCONTACTADDRESS_H
#define QCONTACTADDRESS_H

#include <qcontactsglobal.h>
#include <qcontactdetail.h>
#include <qcontact.h>

QTCONTACTS_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactAddress : public QContactDetail
{
public:
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactAddress, "Address")
    const static QString FieldStreet;
    const static QString FieldLocality;
    const static QString FieldRegion;
    const static QString FieldPostcode;
    const static QString FieldCountry;
    const static QString FieldSubTypes;
    const static QString FieldPostOfficeBox;
    const static QString SubTypeParcel;
    const static QString SubTypePostal;
    const static QString SubTypeDomestic;
    const static QString SubTypeInternational;

    void setStreet(const QString& street) {setValue(FieldStreet, street);}
    QString street() const {return value(FieldStreet).toString();}
    void setLocality(const QString& locality) {setValue(FieldLocality, locality);}
    QString locality() const {return value(FieldLocality).toString();}
    void setRegion(const QString& region) {setValue(FieldRegion, region);}
    QString region() const {return value(FieldRegion).toString();}
    void setPostcode(const QString& postcode) {setValue(FieldPostcode, postcode);}
    QString postcode() const {return value(FieldPostcode).toString();}
    void setCountry(const QString& country) {setValue(FieldCountry, country);}
    QString country() const {return value(FieldCountry).toString();}
    void setPostOfficeBox(const QString& postOfficeBox) {setValue(FieldPostOfficeBox, postOfficeBox);}
    QString postOfficeBox() const {return value(FieldPostOfficeBox).toString();}

    void setSubTypes(const QStringList& subTypes) {setValue(FieldSubTypes, subTypes);}
    void setSubTypes(const QString& subType) {setValue(FieldSubTypes, QStringList(subType));}
    QStringList subTypes() const {return value<QStringList>(FieldSubTypes);}

    // Convenience filter
    static QContactFilter match(const QString& subString);
};

QTCONTACTS_END_NAMESPACE

#endif

