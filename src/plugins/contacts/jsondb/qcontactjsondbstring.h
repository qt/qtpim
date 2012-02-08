/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Pim Module
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

#ifndef QCONTACTJSONDBSTRING_H
#define QCONTACTJSONDBSTRING_H

#include <QtContacts/qcontactsglobal.h>
#include <QString>

QTCONTACTS_BEGIN_NAMESPACE

class QContactJsonDbStr
{
public:
    // Version
    const static int ContactJsonDbEngineVersion = 1.0;

    // Engine meta-data
    inline const static QString contactJsonDbEngineName() { return QStringLiteral("jsondb"); }
    inline const static QString managerName() { return QStringLiteral("qtcontacts:jsondb::"); }
    inline const static QString managerUri() { return QStringLiteral("qtcontacts:jsondb:"); }

    // Jsondb column names
    inline const static QString type() { return QStringLiteral("_type"); }
    inline const static QString uuid() { return QStringLiteral("_uuid"); }
    inline const static QString version() { return QStringLiteral("_version"); }

    // Jsondb field names
    inline const static QString nameDefinitionName() { return QStringLiteral("name"); }
    inline const static QString genderDefinitionName() { return QStringLiteral("gender"); }
    inline const static QString phoneNumberDefinitionName() { return QStringLiteral("phones"); }
    inline const static QString emailAddressDefinitionName() { return QStringLiteral("emails"); }
    inline const static QString urlDefinitionName() { return QStringLiteral("urls"); }
    inline const static QString nicknameDefinitionName() { return QStringLiteral("nickname"); }
    inline const static QString addressDefinitionName() { return QStringLiteral("addresses"); }
    inline const static QString birthdayDefinitionName() { return QStringLiteral("birthday"); }
    inline const static QString avatarDefinitionName() { return QStringLiteral("photoUrl"); }
    inline const static QString ringtoneDefinitionName() { return QStringLiteral("ringtoneUrl"); }
    inline const static QString organizationDefinitionName() { return QStringLiteral("organization"); }
    inline const static QString noteDefinitionName() { return QStringLiteral("note"); }
    inline const static QString personIdDefinitionName() { return QStringLiteral("personid"); }
    inline const static QString nameFieldCustomLabel() { return QStringLiteral("customLabel"); }
    inline const static QString nameFieldFirstName() { return QStringLiteral("firstName"); }
    inline const static QString nameFieldLastName() { return QStringLiteral("lastName"); }
    inline const static QString nameFieldMiddleName() { return QStringLiteral("middleName"); }
    inline const static QString nameFieldPrefix() { return QStringLiteral("prefix"); }
    inline const static QString nameFieldSuffix() { return QStringLiteral("suffix"); }
    inline const static QString addressFieldCountry() { return QStringLiteral("country"); }
    inline const static QString addressFieldRegion() { return QStringLiteral("region"); }
    inline const static QString addressFieldLocality() { return QStringLiteral("locality"); }
    inline const static QString addressFieldPostcode() { return QStringLiteral("postcode"); }
    inline const static QString addressFieldPostOfficeBox() { return QStringLiteral("postOfficeBox"); }
    inline const static QString addressFieldStreet() { return QStringLiteral("street"); }
    inline const static QString organizationFieldName() { return QStringLiteral("name"); }
    inline const static QString organizationFieldDepartment() { return QStringLiteral("department"); }
    inline const static QString organizationFieldTitle() { return QStringLiteral("title"); }
    inline const static QString organizationFieldRole() { return QStringLiteral("role"); }
    inline const static QString organizationFieldAssistantName() { return QStringLiteral("assistantName"); }
    inline const static QString organizationFieldLogoUrl() { return QStringLiteral("logoUrl"); }
    inline const static QString organizationFieldStartDate() { return QStringLiteral("startDate"); }
    inline const static QString organizationFieldEndDate() { return QStringLiteral("endDate"); }

    // Jsondb Contexts and Subtypes
    inline const static QString contextHome() { return QStringLiteral("home"); }
    inline const static QString contextWork() { return QStringLiteral("work"); }
    inline const static QString contextOther() { return QStringLiteral("other"); }
    inline const static QString subTypePref() { return QStringLiteral("pref"); }
    inline const static QString subTypeFax() { return QStringLiteral("fax"); }
    inline const static QString subTypeCell() { return QStringLiteral("cell"); }
    inline const static QString subTypeVideo() { return QStringLiteral("video"); }
    inline const static QString subTypeLandline() { return QStringLiteral("landline"); }
    inline const static QString contactDetails() { return QStringLiteral("details"); }
    inline const static QString phoneNumberUriScheme() { return QStringLiteral("tel:"); }

    // Jsondb actions
    inline const static QString actionCreate() { return QStringLiteral("create"); }
    inline const static QString actionUpdate() { return QStringLiteral("update"); }
    inline const static QString actionRemove() { return QStringLiteral("remove"); }

    // Queries related
    inline const static QString defaultPartition() { return QStringLiteral("com.nokia.qtjsondb.System"); }
    inline const static QString contactsJsonDbType() { return QStringLiteral("com.nokia.mp.contacts.Contact"); }
    inline const static QString contactsJsonDbNotificationQuery() { return QStringLiteral("[?_type in [\"com.nokia.mp.contacts.Contact\"]]"); }

};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBSTRING_H
