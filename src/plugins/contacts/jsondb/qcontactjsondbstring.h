/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Pim Module
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTJSONDBSTRING_H
#define QCONTACTJSONDBSTRING_H

#include <QtContacts/qcontactsglobal.h>
#include <QString>

QT_BEGIN_NAMESPACE_CONTACTS

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
    inline const static QString displayLabelDefinitionName() { return QStringLiteral("displayLabel"); }
    inline const static QString addressDefinitionName() { return QStringLiteral("addresses"); }
    inline const static QString birthdayDefinitionName() { return QStringLiteral("birthday"); }
    inline const static QString avatarDefinitionName() { return QStringLiteral("photoUrl"); }
    inline const static QString ringtoneDefinitionName() { return QStringLiteral("ringtoneUrl"); }
    inline const static QString organizationDefinitionName() { return QStringLiteral("organizations"); }
    inline const static QString noteDefinitionName() { return QStringLiteral("note"); }
    inline const static QString syncTargetDefinitionName() { return QStringLiteral("synctarget"); }
    inline const static QString guidDefinitionName() { return QStringLiteral("guid"); }
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

    //JsonDb enums
    inline const static QString genderMale() { return QStringLiteral("male"); }
    inline const static QString genderFemale() { return QStringLiteral("female"); }
    inline const static QString genderOther() { return QStringLiteral("other"); }

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

    // Jsondb actions
    inline const static QString actionCreate() { return QStringLiteral("create"); }
    inline const static QString actionUpdate() { return QStringLiteral("update"); }
    inline const static QString actionRemove() { return QStringLiteral("remove"); }

    // partition names
    inline const static QString userDataPartition() { return QStringLiteral("com.nokia.mt.User"); }
    inline const static QString systemPartition() { return QStringLiteral("com.nokia.mt.System"); }

    // Queries related
    inline const static QString contactsJsonDbType() { return QStringLiteral("com.nokia.mt.contacts.Contact"); }
    inline const static QString contactsJsonDbNotificationQuery() { return QStringLiteral("[?_type in [\"com.nokia.mt.contacts.Contact\"]]"); }
    inline const static QString uuidSelectQuery() { return QStringLiteral("[={_uuid:_uuid}]"); }
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTJSONDBSTRING_H
