/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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


#ifndef QVERSITCONTACTSDEFS_P_H
#define QVERSITCONTACTSDEFS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qcontacts.h>
#include "qversitdefs_p.h"

QTCONTACTS_BEGIN_NAMESPACE
class QContactAddress;
class QContactAnniversary;
class QContactBirthday;
class QContactDisplayLabel;
class QContactEmailAddress;
class QContactFamily;
class QContactGender;
class QContactGeoLocation;
class QContactGuid;
class QContactName;
class QContactNickname;
class QContactNote;
class QContactOnlineAccount;
class QContactOrganization;
class QContactPhoneNumber;
class QContactRingtone;
class QContactTag;
class QContactThumbnail;
class QContactTimestamp;
class QContactUrl;
QTCONTACTS_END_NAMESPACE

QTCONTACTS_USE_NAMESPACE

QTVERSIT_BEGIN_NAMESPACE

//! [Property name mappings]
// Mappings from versit property names to Qt contact details
// For the case that there are multiple property names corresponding to the same detail, put the
// primary one first and it will be the one used when exporting.
const VersitDetailMapping versitContactDetailMappings[] = {
    {"ADR",           QContactAddress::DefinitionName,
                      QString::null},
    {"BDAY",          QContactBirthday::DefinitionName,
                      QContactBirthday::FieldBirthday},
    {"CATEGORIES",    QContactTag::DefinitionName,
                      QContactTag::FieldTag},
    {"FN",            QContactDisplayLabel::DefinitionName,
                      QString::null},
    {"GEO",           QContactGeoLocation::DefinitionName,
                      QString::null},
    {"EMAIL",         QContactEmailAddress::DefinitionName,
                      QContactEmailAddress::FieldEmailAddress},
    {"IMPP",          QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"LOGO",          QContactOrganization::DefinitionName,
                      QContactOrganization::FieldLogoUrl},
    {"N",             QContactName::DefinitionName,
                      QString::null},
    {"NICKNAME",      QContactNickname::DefinitionName,
                      QContactNickname::FieldNickname},
    {"NOTE",          QContactNote::DefinitionName,
                      QContactNote::FieldNote},
    {"ORG",           QContactOrganization::DefinitionName,
                      QContactOrganization::FieldName},
    {"PHOTO",         QContactThumbnail::DefinitionName,
                      QString::null},
    {"REV",           QContactTimestamp::DefinitionName,
                      QString::null},
    {"ROLE",          QContactOrganization::DefinitionName,
                      QContactOrganization::FieldRole},
    {"SOUND",         QContactRingtone::DefinitionName,
                      QContactRingtone::FieldAudioRingtoneUrl},
    {"TEL",           QContactPhoneNumber::DefinitionName,
                      QContactPhoneNumber::FieldNumber},
    {"TITLE",         QContactOrganization::DefinitionName,
                      QContactOrganization::FieldTitle},
    {"UID",           QContactGuid::DefinitionName,
                      QContactGuid::FieldGuid},
    {"URL",           QContactUrl::DefinitionName,
                      QContactUrl::FieldUrl},
    {"X-ABUID",       QContactGuid::DefinitionName,
                      QContactGuid::FieldGuid},
    {"X-AIM",         QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-ANNIVERSARY", QContactAnniversary::DefinitionName,
                      QString::null},
    {"X-ASSISTANT",   QContactOrganization::DefinitionName,
                      QContactOrganization::FieldAssistantName},
    {"X-ASSISTANT-TEL", QContactPhoneNumber::DefinitionName,
                      QContactPhoneNumber::SubTypeAssistant},
    {"X-CHILDREN",    QContactFamily::DefinitionName,
                      QContactFamily::FieldChildren},
    {"X-EPOCSECONDNAME",QContactNickname::DefinitionName,
                      QContactNickname::FieldNickname},
    {"X-EVOLUTION-SPOUSE", QContactFamily::DefinitionName,
                      QContactFamily::FieldSpouse},
    {"X-EVOLUTION-ANNIVERSARY", QContactAnniversary::DefinitionName,
                      QString::null},
    {"X-GADUGADU",    QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-GENDER",      QContactGender::DefinitionName,
                      QContactGender::FieldGender},
    {"X-ICQ",         QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-IMPP",        QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-JABBER",      QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-KADDRESSBOOK-X-SPOUSENAME", QContactFamily::DefinitionName,
                      QContactFamily::FieldSpouse},
    {"X-KADDRESSBOOK-X-ANNIVERSARY", QContactAnniversary::DefinitionName,
                      QString::null},
    {"X-KADDRESSBOOK-X-IMADDRESS", QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-MS-CARDPICTURE", QContactThumbnail::DefinitionName,
                      QString::null},
    {"X-MS-IMADDRESS", QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-MSN",         QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-NICKNAME",    QContactNickname::DefinitionName,
                      QContactNickname::FieldNickname},
    {"X-QQ",          QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-SIP",         QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-SKYPE",       QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-SKYPE-USERNAME", QContactOnlineAccount::DefinitionName,
                      QString::null},
    {"X-SPOUSE",      QContactFamily::DefinitionName,
                      QContactFamily::FieldSpouse},
    {"X-YAHOO",       QContactOnlineAccount::DefinitionName,
                      QString::null}
};
//! [Property name mappings]

// Mappings from versit TYPE parameters to Qt contact detail contexts
const VersitMapping versitContextMappings[] = {
    {"HOME", QContactDetail::ContextHome},
    {"WORK", QContactDetail::ContextWork},
};

//! [Property type parameter mappings]
// Mappings from versit TYPE parameters to Qt contact detail subtypes
const VersitMapping versitSubTypeMappings[] = {
    {"DOM",    QContactAddress::SubTypeDomestic},
    {"INTL",   QContactAddress::SubTypeInternational},
    {"POSTAL", QContactAddress::SubTypePostal},
    {"PARCEL", QContactAddress::SubTypeParcel},
    {"VOICE",  QContactPhoneNumber::SubTypeVoice},
    {"CELL",   QContactPhoneNumber::SubTypeMobile},
    {"MODEM",  QContactPhoneNumber::SubTypeModem},
    {"CAR",    QContactPhoneNumber::SubTypeCar},
    {"VIDEO",  QContactPhoneNumber::SubTypeVideo},
    {"FAX",    QContactPhoneNumber::SubTypeFax},
    {"BBS",    QContactPhoneNumber::SubTypeBulletinBoardSystem},
    {"PAGER",  QContactPhoneNumber::SubTypePager},
    {"SWIS",   QContactOnlineAccount::SubTypeVideoShare},
    {"VOIP",   QContactOnlineAccount::SubTypeSipVoip}
};
//! [Property type parameter mappings]

QTVERSIT_END_NAMESPACE

#endif // QVERSITCONTACTSDEFS_P_H
