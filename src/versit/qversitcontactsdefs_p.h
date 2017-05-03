/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
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

#include <QtContacts/qcontactdetails.h>

#include <QtVersit/private/qversitdefs_p.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

//! [Property name mappings]
// Mappings from versit property names to Qt contact details
// For the case that there are multiple property names corresponding to the same detail, put the
// primary one first and it will be the one used when exporting.
const VersitContactDetailMapping versitContactDetailMappings[] = {
    {"ADR",           QContactAddress::Type,
                      -1},
    {"BDAY",          QContactBirthday::Type,
                      QContactBirthday::FieldBirthday},
    {"CATEGORIES",    QContactTag::Type,
                      QContactTag::FieldTag},
    {"FN",            QContactDisplayLabel::Type,
                      QContactDisplayLabel::FieldLabel},
    {"GEO",           QContactGeoLocation::Type,
                      -1},
    {"EMAIL",         QContactEmailAddress::Type,
                      QContactEmailAddress::FieldEmailAddress},
    {"IMPP",          QContactOnlineAccount::Type,
                      -1},
    {"LOGO",          QContactOrganization::Type,
                      QContactOrganization::FieldLogoUrl},
    {"N",             QContactName::Type,
                      -1},
    {"NICKNAME",      QContactNickname::Type,
                      QContactNickname::FieldNickname},
    {"NOTE",          QContactNote::Type,
                      QContactNote::FieldNote},
    {"PHOTO",         QContactAvatar::Type,
                      -1},
    {"ORG",           QContactOrganization::Type,
                      QContactOrganization::FieldName},
    {"REV",           QContactTimestamp::Type,
                      -1},
    {"ROLE",          QContactOrganization::Type,
                      QContactOrganization::FieldRole},
    {"SOUND",         QContactRingtone::Type,
                      QContactRingtone::FieldAudioRingtoneUrl},
    {"TEL",           QContactPhoneNumber::Type,
                      QContactPhoneNumber::FieldNumber},
    {"TITLE",         QContactOrganization::Type,
                      QContactOrganization::FieldTitle},
    {"UID",           QContactGuid::Type,
                      QContactGuid::FieldGuid},
    {"URL",           QContactUrl::Type,
                      QContactUrl::FieldUrl},
    {"X-ABUID",       QContactGuid::Type,
                      QContactGuid::FieldGuid},
    {"X-AIM",         QContactOnlineAccount::Type,
                      -1},
    {"X-ANNIVERSARY", QContactAnniversary::Type,
                      -1},
    {"X-ASSISTANT",   QContactOrganization::Type,
                      QContactOrganization::FieldAssistantName},
    {"X-ASSISTANT-TEL", QContactPhoneNumber::Type,
                      QContactPhoneNumber::SubTypeAssistant},
    {"X-CHILDREN",    QContactFamily::Type,
                      QContactFamily::FieldChildren},
    {"X-EPOCSECONDNAME",QContactNickname::Type,
                      QContactNickname::FieldNickname},
    {"X-EVOLUTION-SPOUSE", QContactFamily::Type,
                      QContactFamily::FieldSpouse},
    {"X-EVOLUTION-ANNIVERSARY", QContactAnniversary::Type,
                      -1},
    {"X-GADUGADU",    QContactOnlineAccount::Type,
                      -1},
    {"X-GENDER",      QContactGender::Type,
                      QContactGender::FieldGender},
    {"X-ICQ",         QContactOnlineAccount::Type,
                      -1},
    {"X-IMPP",        QContactOnlineAccount::Type,
                      -1},
    {"X-JABBER",      QContactOnlineAccount::Type,
                      -1},
    {"X-KADDRESSBOOK-X-SPOUSENAME", QContactFamily::Type,
                      QContactFamily::FieldSpouse},
    {"X-KADDRESSBOOK-X-ANNIVERSARY", QContactAnniversary::Type,
                      -1},
    {"X-KADDRESSBOOK-X-IMADDRESS", QContactOnlineAccount::Type,
                      -1},
    {"X-MS-CARDPICTURE", QContactAvatar::Type,
                      -1},
    {"X-MS-IMADDRESS", QContactOnlineAccount::Type,
                      -1},
    {"X-MSN",         QContactOnlineAccount::Type,
                      -1},
    {"X-NICKNAME",    QContactNickname::Type,
                      QContactNickname::FieldNickname},
    {"X-QQ",          QContactOnlineAccount::Type,
                      -1},
    {"X-QTPROJECT-EXTENDED-DETAIL", QContactExtendedDetail::Type,
                      -1},
    {"X-QTPROJECT-FAVORITE", QContactFavorite::Type,
                      -1},
    {"X-QTPROJECT-VERSION", QContactVersion::Type,
                      -1},
    {"X-SIP",         QContactOnlineAccount::Type,
                      -1},
    {"X-SKYPE",       QContactOnlineAccount::Type,
                      -1},
    {"X-SKYPE-USERNAME", QContactOnlineAccount::Type,
                      -1},
    {"X-SPOUSE",      QContactFamily::Type,
                      QContactFamily::FieldSpouse},
    {"X-YAHOO",       QContactOnlineAccount::Type,
                      -1}
};
//! [Property name mappings]

// Mappings from versit TYPE parameters to Qt contact detail contexts
const VersitContextMapping versitContextMappings[] = {
    {"HOME", QContactDetail::ContextHome},
    {"WORK", QContactDetail::ContextWork},
    {"OTHER", QContactDetail::ContextOther}
};

//! [Property type parameter mappings]
// Mappings from versit TYPE parameters to Qt contact detail subtypes
const VersitSubTypeMapping versitSubTypeMappings[] = {
    {"DOM",    QContactDetail::TypeAddress, QContactAddress::SubTypeDomestic},
    {"INTL",   QContactDetail::TypeAddress, QContactAddress::SubTypeInternational},
    {"POSTAL", QContactDetail::TypeAddress, QContactAddress::SubTypePostal},
    {"PARCEL", QContactDetail::TypeAddress, QContactAddress::SubTypeParcel},
    {"VOICE",  QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeVoice},
    {"CELL",   QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeMobile},
    {"MODEM",  QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeModem},
    {"CAR",    QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeCar},
    {"VIDEO",  QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeVideo},
    {"FAX",    QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeFax},
    {"BBS",    QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeBulletinBoardSystem},
    {"PAGER",  QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypePager},
    {"ISDN",   QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeIsdn},
    {"VOICE",  QContactDetail::TypePhoneNumber, QContactPhoneNumber::SubTypeLandline},
    {"SWIS",   QContactDetail::TypeOnlineAccount, QContactOnlineAccount::SubTypeVideoShare},
    {"VOIP",   QContactDetail::TypeOnlineAccount, QContactOnlineAccount::SubTypeSipVoip}
};
//! [Property type parameter mappings]

QT_END_NAMESPACE_VERSIT

#endif // QVERSITCONTACTSDEFS_P_H
