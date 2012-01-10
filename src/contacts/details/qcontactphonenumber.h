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


#ifndef QCONTACTPHONENUMBER_H
#define QCONTACTPHONENUMBER_H

#include <QString>

#include <qcontactsglobal.h>
#include "qcontactdetail.h"
#include "qcontactfilter.h"

QTCONTACTS_BEGIN_NAMESPACE

/* Leaf class */
//! [0]
class Q_CONTACTS_EXPORT QContactPhoneNumber : public QContactDetail
{
public:
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactPhoneNumber, "PhoneNumber")
    const static QString FieldNumber;
    const static QString FieldSubTypes;
    const static QString SubTypeLandline;
    const static QString SubTypeMobile;
    const static QString SubTypeFax;
    const static QString SubTypePager;
    const static QString SubTypeVoice;
    const static QString SubTypeModem;
    const static QString SubTypeVideo;
    const static QString SubTypeCar;
    const static QString SubTypeBulletinBoardSystem;
    const static QString SubTypeMessagingCapable;
    const static QString SubTypeAssistant;
    const static QString SubTypeDtmfMenu;

    void setNumber(const QString& number) {setValue(FieldNumber, number);}
    QString number() const {return value(FieldNumber).toString();}

    void setSubTypes(const QStringList& subTypes) {setValue(FieldSubTypes, subTypes);}
    void setSubTypes(const QString& subType) {setValue(FieldSubTypes, QStringList(subType));}
    QStringList subTypes() const {return value<QStringList>(FieldSubTypes);}

    // Convenience filter
    static QContactFilter match(const QString& number);
};
//! [0]

QTCONTACTS_END_NAMESPACE

#endif

