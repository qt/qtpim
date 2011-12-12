/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtPIM module of the Qt Toolkit.
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

#include <QList>
#include <QVariantMap>
#include <QVariantList>
#include <QDebug>
#include <QCryptographicHash>

#include <private/jsondb-strings_p.h>
QT_ADDON_JSONDB_USE_NAMESPACE

#include "qcontactdetail.h"
#include "qcontacttype.h"
#include "qcontactid.h"
#include "qcontactname.h"
#include "qcontactaddress.h"
#include "qcontactnickname.h"
#include "qcontactemailaddress.h"
#include "qcontactphonenumber.h"
#include "qcontactmanager.h"
#include "qcontactmanagerengine.h"
#include "qcontactid.h"
#include "qcontacturl.h"
#include "qcontactabstractrequest.h"

#include "qcontactdetailfilter.h"
#include "qcontactchangelogfilter.h"
#include "qcontactactionfilter.h"
#include "qcontactrelationshipfilter.h"
#include "qcontactintersectionfilter.h"
#include "qcontactunionfilter.h"
#include "qcontactlocalidfilter.h"

#include "qcontactjsondbconverter.h"
#include "qcontactjsondbglobal.h"

QTCONTACTS_BEGIN_NAMESPACE

Q_DEFINE_LATIN1_CONSTANT(ContextHome, "home");
Q_DEFINE_LATIN1_CONSTANT(ContextWork, "work");
Q_DEFINE_LATIN1_CONSTANT(ContextOther, "other");
Q_DEFINE_LATIN1_CONSTANT(SubTypePref, "pref");
Q_DEFINE_LATIN1_CONSTANT(SubTypeFax, "fax");
Q_DEFINE_LATIN1_CONSTANT(SubTypeCell, "cell");
Q_DEFINE_LATIN1_CONSTANT(SubTypeVideo, "video");
Q_DEFINE_LATIN1_CONSTANT(SubTypeLandline, "landline");
Q_DEFINE_LATIN1_CONSTANT(ContactDetails, "details");
Q_DEFINE_LATIN1_CONSTANT(PhoneNumberUriScheme, "tel:");

QContactJsonDbConverter::QContactJsonDbConverter()
{
    initializeMappings();
}

QContactJsonDbConverter::~QContactJsonDbConverter()
{
}

bool QContactJsonDbConverter::toQContact(const QVariantMap& object, QContact* contact, const QContactJsonDbEngine& engine) {
    QList<QContactDetail*> detailList;
    QVariantMap map;
    QVariantList array;
    QString stringValue;

    stringValue = object.value(JsonDbString::kUuidStr).toString();
    if (stringValue.isEmpty()) {
        return false;
    }

    // TODO: other types
    contact->setType(QContactType::TypeContact);

    QContactId newId = convertId(stringValue, engine);
    contact->setId(newId);

    // Go through all fields in loop.
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QContactPersonId* personid = new QContactPersonId();

    while (i != object.constEnd()) {

        if (i.key() == detailsToJsonMapping.value(QContactPersonId::DefinitionName)) {
            //personid
            stringValue = object[detailsToJsonMapping.value(QContactPersonId::DefinitionName)].toString();
            if (!stringValue.isEmpty()) {
                personid->setPersonId(stringValue);
            }
            else { // if no personid is stored in backend, we return the local
                personid->setPersonId(newId.localId());
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactName::DefinitionName)) {
            //name
            QContactName* name = new QContactName();
            map = object[detailsToJsonMapping.value(QContactName::DefinitionName)].value<QVariantMap>();
            stringValue = map[contactNameFieldsMapping.value(QContactName::FieldFirstName)].toString();
            if (!stringValue.isEmpty())
                name->setFirstName(stringValue);
            stringValue = map[contactNameFieldsMapping.value(QContactName::FieldMiddleName)].toString();
            if (!stringValue.isEmpty())
                name->setMiddleName(stringValue);
            stringValue = map[contactNameFieldsMapping.value(QContactName::FieldLastName)].toString();
            if (!stringValue.isEmpty())
                name->setLastName(stringValue);
            stringValue = map[contactNameFieldsMapping.value(QContactName::FieldPrefix)].toString();
            if (!stringValue.isEmpty())
                name->setPrefix(stringValue);
            stringValue = map[contactNameFieldsMapping.value(QContactName::FieldSuffix)].toString();
            if (!stringValue.isEmpty())
                name->setSuffix(stringValue);
            detailList << name;
        } else if (i.key() == detailsToJsonMapping.value(QContactGender::DefinitionName)) {
            //gender
            QContactGender* gender = new QContactGender();
            map = object[detailsToJsonMapping.value(QContactGender::DefinitionName)].value<QVariantMap>();
            stringValue = map[detailsToJsonMapping.value(QContactGender::DefinitionName)].toString();
            if (!stringValue.isEmpty())
                gender->setGender(stringValue);
            detailList << gender;
        } else if (i.key() == detailsToJsonMapping.value(QContactOrganization::DefinitionName)) {
            //organization
            QContactOrganization* organization = new QContactOrganization();
            array = object[detailsToJsonMapping.value(QContactOrganization::DefinitionName)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                map = array[i].value<QVariantMap>();
                // name
                if (!map[organizationFieldsMapping.value(QContactOrganization::FieldName)].toString().isEmpty())
                    organization->setName(map[organizationFieldsMapping.value(QContactOrganization::FieldName)].toString());
                // department
                stringValue = map[organizationFieldsMapping.value(QContactOrganization::FieldDepartment)].toString();
                if (!stringValue.isEmpty())
                    organization->setDepartment(QStringList() << stringValue);
                // title
                if (!map[organizationFieldsMapping.value(QContactOrganization::FieldTitle)].toString().isEmpty())
                    organization->setTitle(map[organizationFieldsMapping.value(QContactOrganization::FieldTitle)].toString());
                // role
                if (!map[organizationFieldsMapping.value(QContactOrganization::FieldRole)].toString().isEmpty())
                    organization->setRole(map[organizationFieldsMapping.value(QContactOrganization::FieldRole)].toString());
                // assistantName
                if (!map[organizationFieldsMapping.value(QContactOrganization::FieldAssistantName)].toString().isEmpty())
                    organization->setAssistantName(map[organizationFieldsMapping.value(QContactOrganization::FieldAssistantName)].toString());
                // logoUrl
                stringValue = map[organizationFieldsMapping.value(QContactOrganization::FieldLogoUrl)].toString();
                if (!stringValue.isEmpty())
                    organization->setLogoUrl(QUrl(stringValue));
                //startDate
                stringValue = map[organizationFieldsMapping.value(QContactOrganization::FieldStartDate)].toString();
                if (!stringValue.isEmpty()) {
                    QDateTime date = toContactDate(stringValue);
                    organization->setStartDate(date);
                }
                //endDate
                stringValue = map[organizationFieldsMapping.value(QContactOrganization::FieldEndDate)].toString();
                if (!stringValue.isEmpty()) {
                    QDateTime date = toContactDate(stringValue);
                    organization->setEndDate(date);
                }
                // Add organization to details
                detailList << organization;
            }
        } else if (i.key() == ContactDetails) {
            map = object[ContactDetails].value<QVariantMap>();

            //birthday
            QString dateString;
            dateString = map[detailsToJsonMapping.value(QContactBirthday::DefinitionName)].toString();
            if (!dateString.isEmpty()) {
                QDateTime date = toContactDate(dateString);
                QContactBirthday* birthday = new QContactBirthday();
                birthday->setDateTime(date);
                detailList << birthday;
            }

            //avatar
            QString avatarUrlString;
            avatarUrlString = map[detailsToJsonMapping.value(QContactAvatar::DefinitionName)].toString();
            if (!avatarUrlString.isEmpty()) {
                QUrl avatarUrl(avatarUrlString);
                QContactAvatar* avatar = new QContactAvatar();
                avatar->setImageUrl(avatarUrl);
                detailList << avatar;
            }

            //ringtone
            QString ringtoneUrlString;
            ringtoneUrlString = map[detailsToJsonMapping.value(QContactRingtone::DefinitionName)].toString();
            if (!ringtoneUrlString.isEmpty()) {
                QUrl ringtoneUrl(ringtoneUrlString);
                QContactRingtone* ringtone = new QContactRingtone();
                ringtone->setAudioRingtoneUrl(ringtoneUrl);
                detailList << ringtone;
            }

            //nickname
            QString nickString;
            nickString = map[detailsToJsonMapping.value(QContactNickname::DefinitionName)].toString();
            if (!nickString.isEmpty()) {
                QContactNickname* nick = new QContactNickname();
                nick->setNickname(nickString);
                detailList << nick;
            }

            //note
            QString noteString;
            noteString = map[detailsToJsonMapping.value(QContactNote::DefinitionName)].toString();
            if (!noteString.isEmpty()) {
                QContactNote* note = new QContactNote();
                note->setNote(noteString);
                detailList << note;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactEmailAddress::DefinitionName)) {
            //email
            array = object[detailsToJsonMapping.value(QContactEmailAddress::DefinitionName)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactEmailAddress* email = new QContactEmailAddress;
                map = array[i].value<QVariantMap>();
                email->setEmailAddress(map["value"].toString());
                updateContexts(map,email);
                detailList << email;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactPhoneNumber::DefinitionName)) {
            //phone number
            array = object[detailsToJsonMapping.value(QContactPhoneNumber::DefinitionName)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactPhoneNumber* number = new QContactPhoneNumber;
                map = array[i].value<QVariantMap>();
                number->setNumber(map["value"].toString().remove(PhoneNumberUriScheme));
                stringValue = map["context"].toString();
                if (stringValue == ContextHome || stringValue == ContextWork || stringValue == ContextOther) {
                    updateContexts(map, number);
                }
                stringValue = map["subType"].toString();
                if (stringValue == SubTypeFax) {
                    number->setSubTypes(QContactPhoneNumber::SubTypeFax);
                } else if (stringValue == SubTypeCell) {
                    number->setSubTypes(QContactPhoneNumber::SubTypeMobile);
                } else if (stringValue == SubTypeVideo) {
                    number->setSubTypes(QContactPhoneNumber::SubTypeVideo);
                } else if (stringValue == SubTypeLandline) {
                    number->setSubTypes(QContactPhoneNumber::SubTypeLandline);
                };
                detailList << number;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactAddress::DefinitionName)) {
            //address
            array = object[detailsToJsonMapping.value(QContactAddress::DefinitionName)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactAddress* address = new QContactAddress;
                map = array[i].value<QVariantMap>();
                if (!map[addressFieldsMapping.value(QContactAddress::FieldStreet)].toString().isEmpty())
                    address->setStreet(map[addressFieldsMapping.value(QContactAddress::FieldStreet)].toString());
                if (!map[addressFieldsMapping.value(QContactAddress::FieldLocality)].toString().isEmpty())
                    address->setLocality(map[addressFieldsMapping.value(QContactAddress::FieldLocality)].toString());
                if (!map[addressFieldsMapping.value(QContactAddress::FieldPostcode)].toString().isEmpty())
                    address->setPostcode(map[addressFieldsMapping.value(QContactAddress::FieldPostcode)].toString());
                if (!map[addressFieldsMapping.value(QContactAddress::FieldPostOfficeBox)].toString().isEmpty())
                    address->setPostOfficeBox(map[addressFieldsMapping.value(QContactAddress::FieldPostOfficeBox)].toString());
                if (!map[addressFieldsMapping.value(QContactAddress::FieldRegion)].toString().isEmpty())
                    address->setRegion(map[addressFieldsMapping.value(QContactAddress::FieldRegion)].toString());
                if (!map[addressFieldsMapping.value(QContactAddress::FieldCountry)].toString().isEmpty())
                    address->setCountry(map[addressFieldsMapping.value(QContactAddress::FieldCountry)].toString());
                updateContexts(map, address);
                detailList << address;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactUrl::DefinitionName)) {
            //url
            array = object[detailsToJsonMapping.value(QContactUrl::DefinitionName)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactUrl* url = new QContactUrl;
                map = array[i].value<QVariantMap>();
                url->setUrl(map["value"].toString());
                if (updateContexts(map, url)) {
                    url->setSubType(url->contexts().first());//TODO decide if we use "Context" or "SubTypes" to store the jsondb SubTypes
                };
                detailList << url;
            }
        } else if (i.key().at(0) == QChar('_')) {
            // skip as it's used internally
        } else {
            // we map anything else to extended details
            QContactExtendedDetail *extendedDetail = new QContactExtendedDetail;
            extendedDetail->setName(i.key());
            extendedDetail->setData(i.value());
            detailList << extendedDetail;
        }
        ++i;
    }

    // Each contact should have at least a uuid and personid in JsonDb
    if (personid->isEmpty())
         personid->setPersonId(newId.localId());
    detailList << personid;

    QContactDetail* detail;
    bool ok = false;
    foreach(detail, detailList){
        if (detail->isEmpty()){
            delete detail;
            continue;
        }
        ok = contact->saveDetail(detail);
        if (!ok){
            delete detail;
            continue;
        }
        delete detail;
    };
    QContactManager::Error error;
    QString displayLabel = engine.synthesizedDisplayLabel(*contact, &error);
    if (error == QContactManager::NoError) {
        QContactManagerEngine::setContactDisplayLabel(contact, displayLabel);
    } else {
        //TODO Error handling(!)
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] ERROR in synthesizing display label: "
                 << error << "for contact " << object.value(JsonDbString::kUuidStr).toString();
        QContactManagerEngine::setContactDisplayLabel(contact, "");
    };
    if (contact->isEmpty()) {
        return false;
    } else {
        return true;
    };
}

bool QContactJsonDbConverter::toJsonContact(QVariantMap* object, const QContact& contact) {

    if(!(contact.id().localId().isEmpty() || contact.id().managerUri().isEmpty())) {
          object->insert(JsonDbString::kUuidStr, convertId(contact.id().localId()));
    }
    object->insert(JsonDbString::kTypeStr, "com.nokia.mp.contacts.Contact");    // get all available contact details.
    QList<QContactDetail> details = contact.details();
    QContactDetail detail;
    QContactName* name;
    QContactNickname* nick;
    QContactEmailAddress* email;
    QContactPhoneNumber* number;
    QContactAddress* address;
    QContactUrl* url;
    QContactOrganization* organization;
    QContactBirthday* birthday;
    QContactAvatar* avatar;
    QContactRingtone* ringtone;
    QContactNote* note;
    QContactGender* gender;
    QContactPersonId* personid;
    QContactExtendedDetail* extendedDetail;
    QVariantList phoneNumbers;
    QVariantList emails;
    QVariantList urls;
    QVariantList organizations;
    QVariantList addresses;
    QVariantMap embeddedDetailsMap;

    // Quickfix for preserving possible extra fields in jsondb contact.
    // Wipe QContact fields that may be empty/deleted, preserve all other data.
    if (!object->empty()) {
        object->remove(detailsToJsonMapping.value(QContactName::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactPersonId::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactGender::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactOrganization::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactEmailAddress::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactPhoneNumber::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactAddress::DefinitionName));
        object->remove(detailsToJsonMapping.value(QContactUrl::DefinitionName));
        embeddedDetailsMap = object->value(ContactDetails).toMap();
        object->remove(ContactDetails);
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactBirthday::DefinitionName));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactAvatar::DefinitionName));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactRingtone::DefinitionName));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactNickname::DefinitionName));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactNote::DefinitionName));
        //  Preseserve possible extra contact details jsondb contact object may have.
        if (!embeddedDetailsMap.isEmpty()) {
            object->insert(ContactDetails, embeddedDetailsMap);
        }
    }
    // End of Quickfix

    for(int i = 0; i < details.size(); ++i) {
        detail = details.at(i);
        // name
        if( (detail.definitionName() == QContactName::DefinitionName) ) {
            QVariantMap nameMap;
            name = static_cast<QContactName *>(&detail);
            if(!name->prefix().isEmpty())
                nameMap[contactNameFieldsMapping.value(QContactName::FieldPrefix)] = name->prefix();
            if(!name->suffix().isEmpty())
                nameMap[contactNameFieldsMapping.value(QContactName::FieldSuffix)] = name->suffix();
            if(!name->lastName().isEmpty())
                nameMap[contactNameFieldsMapping.value(QContactName::FieldLastName)] = name->lastName();
            if(!name->firstName().isEmpty())
                nameMap[contactNameFieldsMapping.value(QContactName::FieldFirstName)] = name->firstName();
            if(!name->middleName().isEmpty())
                nameMap[contactNameFieldsMapping.value(QContactName::FieldMiddleName)] = name->middleName();
            object->insert(detailsToJsonMapping.value(QContactName::DefinitionName), nameMap);
        }
        //personid
        else if( (detail.definitionName() == QContactPersonId::DefinitionName) ) {
            personid = static_cast<QContactPersonId *>(&detail);
            if(!personid->personId().isEmpty() )
                object->insert(detailsToJsonMapping.value(QContactPersonId::DefinitionName),personid->personId());
        }
        //gender
        else if( (detail.definitionName() == QContactGender::DefinitionName) ) {
            QVariantMap genderMap;
            gender = static_cast<QContactGender *>(&detail);
            if(!gender->gender().isEmpty())
                genderMap[detailsToJsonMapping.value(QContactGender::FieldGender)] = gender->gender();
            object->insert(detailsToJsonMapping.value(QContactGender::DefinitionName), genderMap);
        }
        //organization
        else if( (detail.definitionName() == QContactOrganization::DefinitionName) ) {
            QVariantMap organizationMap;
            organization = static_cast<QContactOrganization *>(&detail);
            organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldName)] = organization->name();
            organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldDepartment)] = organization->department();
            organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldTitle)] = organization->title();
            organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldRole)] = organization->role();
            organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldAssistantName)] = organization->assistantName();
            organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldLogoUrl)] = organization->logoUrl().toString();
            QDateTime startDate = organization->startDate();
            if (startDate.isValid()) {
                QString organizationStartDate = toJsonDate(startDate);
                organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldStartDate)] = organizationStartDate;
            }
            QDateTime endDate = organization->endDate();
            if (endDate.isValid()) {
                QString organizationEndDate = toJsonDate(endDate);
                organizationMap[organizationFieldsMapping.value(QContactOrganization::FieldEndDate)] = organizationEndDate;
            }

            updateContexts(*organization, &organizationMap);
            if (!organization->contexts().isEmpty())
                organizationMap["context"] = organization->contexts().first().toLower();
            organizations.append(organizationMap);
            object->insert(detailsToJsonMapping.value(QContactOrganization::DefinitionName), organizations);
        }
        // birthday
        else if( (detail.definitionName() == QContactBirthday::DefinitionName) ) {
            birthday = static_cast<QContactBirthday *>(&detail);
            QDateTime date = birthday->dateTime();
            QString dateString = toJsonDate(date);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactBirthday::DefinitionName)] = dateString;
            object->insert(ContactDetails, embeddedDetailsMap);
        }
        //avatar
        else if( (detail.definitionName() == QContactAvatar::DefinitionName) ) {
            avatar = static_cast<QContactAvatar *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactAvatar::DefinitionName)] = avatar->imageUrl().toString();
            object->insert(ContactDetails, embeddedDetailsMap);
        }
        //ringtone
        else if( (detail.definitionName() == QContactRingtone::DefinitionName) ) {
            ringtone = static_cast<QContactRingtone *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactRingtone::DefinitionName)] = ringtone->audioRingtoneUrl().toString();
            object->insert(ContactDetails, embeddedDetailsMap);
        }
        // nickname
        else if( (detail.definitionName() == QContactNickname::DefinitionName) ) {
            nick = static_cast<QContactNickname *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactNickname::DefinitionName)] = nick->nickname();
            object->insert(ContactDetails, embeddedDetailsMap);
        }
        //note
        else if( (detail.definitionName() == QContactNote::DefinitionName) ) {

            note = static_cast<QContactNote *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactNote::DefinitionName)] = note->note();
            object->insert(ContactDetails, embeddedDetailsMap);
        }
        // email
        else if( (detail.definitionName() == QContactEmailAddress::DefinitionName) ) {
            QVariantMap emailMap;
            email = static_cast<QContactEmailAddress *>(&detail);
            emailMap["value"] = email->emailAddress();
            updateContexts(*email, &emailMap);
            if (!email->contexts().isEmpty())
                emailMap["context"] = email->contexts().first().toLower();//TODO: To fix when moving to new schema
            emails.append(emailMap);
            object->insert(detailsToJsonMapping.value(QContactEmailAddress::DefinitionName), emails);
        }
        // phone number
        else if( (detail.definitionName() == QContactPhoneNumber::DefinitionName) ) {
            QVariantMap phoneMap;
            number = static_cast<QContactPhoneNumber *>(&detail);
            phoneMap["value"] = PhoneNumberUriScheme + number->number();
            updateContexts(*number, &phoneMap);
            QStringList subTypes = number->subTypes();
            if (!subTypes.empty())
                phoneMap["subType"] = phoneNumbersSubtypesMapping.value(number->subTypes().first());
            phoneNumbers.append(phoneMap);
            object->insert(detailsToJsonMapping.value(QContactPhoneNumber::DefinitionName), phoneNumbers);
        }
        // address
        else if( (detail.definitionName() == QContactAddress::DefinitionName) ) {
            QVariantMap addressMap;
            address = static_cast<QContactAddress *>(&detail);
            if(!address->street().isEmpty())
                addressMap[addressFieldsMapping.value(QContactAddress::FieldStreet)] = address->street();
            if(!address->locality().isEmpty())
                addressMap[addressFieldsMapping.value(QContactAddress::FieldLocality)] = address->locality();
            if(!address->postcode().isEmpty())
                addressMap[addressFieldsMapping.value(QContactAddress::FieldPostcode)] = address->postcode();
            if(!address->postOfficeBox().isEmpty())
                addressMap[addressFieldsMapping.value(QContactAddress::FieldPostOfficeBox)] = address->postOfficeBox();
            if(!address->region().isEmpty())
                addressMap[addressFieldsMapping.value(QContactAddress::FieldRegion)] = address->region();
            if(!address->country().isEmpty())
                addressMap[addressFieldsMapping.value(QContactAddress::FieldCountry)] = address->country();
            updateContexts(*address, &addressMap);
            addresses.append(addressMap);
            object->insert(detailsToJsonMapping.value(QContactAddress::DefinitionName), addresses);
        }
        // url
        else if( (detail.definitionName() == QContactUrl::DefinitionName) ) {
            QVariantMap urlMap;
            url = static_cast<QContactUrl *>(&detail);
            urlMap["value"] = url->url();
            updateContexts(*url, &urlMap);
            urls.append(urlMap);
            object->insert(detailsToJsonMapping.value(QContactUrl::DefinitionName), urls);
        }
        // extended details
        else if (detail.definitionName() == QContactExtendedDetail::DefinitionName) {
            extendedDetail = static_cast<QContactExtendedDetail *>(&detail);
            if ( !(extendedDetail->name().at(0) == QChar('_')) ) {
                QVariant property;
                extendedDetailToJsonDbProperty(*extendedDetail, property);
                if (!property.isNull())
                    object->insert(extendedDetail->name(), property);
            }
        }
        // display label
        else if( (detail.definitionName() == QContactDisplayLabel::DefinitionName) || (detail.definitionName() == QContactType::DefinitionName) ) {
            //IGNORED for the moment
        }
        // Invalid or not supported detail
        else {
            // IGNORED for the moment
        }
    }
    return true;
}

bool QContactJsonDbConverter::updateContexts(const QVariantMap &object, QContactDetail *detail)
{
    if (!detail)
        return false;
    QString stringValue = object["context"].toString();
    if (stringValue == ContextHome) {
        detail->setContexts(QContactDetail::ContextHome);
        return true;
    } else if (stringValue == ContextWork) {
        detail->setContexts(QContactDetail::ContextWork);
        return true;
    } else if (stringValue == ContextOther) {
        detail->setContexts(QContactDetail::ContextOther);
        return true;
    }
    return false;
}

bool QContactJsonDbConverter::updateContexts(const QContactDetail& detail, QVariantMap* object) {
    QStringList contexts = detail.contexts();
    if(contexts.size() == 0)
        return false;
    if(contexts.contains(QContactDetail::ContextHome))
        object->insert("context", ContextHome);
    else if(contexts.contains(QContactDetail::ContextWork))
        object->insert("context", ContextWork);
    else if(contexts.contains(QContactDetail::ContextOther))
        object->insert("context", ContextOther);
    return true;
}

QString QContactJsonDbConverter::queryFromRequest(QContactAbstractRequest *request)
{
    if (!request)
        return "";
    QString newJsonDbQuery = "[?" + JsonDbString::kTypeStr + "=\"com.nokia.mp.contacts.Contact\"]";
    switch (request->type()) {
    case QContactAbstractRequest::ContactSaveRequest: {
        //TODO:
        break;
    }
    case QContactAbstractRequest::ContactFetchRequest: {
        QContactFetchRequest* fetchReq = static_cast<QContactFetchRequest*>(request);
        QContactFilter filter = fetchReq->filter();
        newJsonDbQuery.append(convertFilter(filter));
        QList<QContactSortOrder> sorting = fetchReq->sorting();
        newJsonDbQuery.append(convertSortOrder(sorting));
        return newJsonDbQuery;
        break;
    }
    case QContactAbstractRequest::ContactLocalIdFetchRequest: {
        newJsonDbQuery.append("[=");
        newJsonDbQuery.append(JsonDbString::kUuidStr);
        newJsonDbQuery.append("]");
        QContactLocalIdFetchRequest* idReq = static_cast<QContactLocalIdFetchRequest*>(request);
        QContactFilter filter = idReq->filter();
        newJsonDbQuery.append(convertFilter(filter));
        QList<QContactSortOrder> sorting = idReq->sorting();
        newJsonDbQuery.append(convertSortOrder(sorting));
        return newJsonDbQuery;
        break;
    }
    default:
        break;
    }
    if (qt_debug_jsondb_contacts())
        qDebug() << " JSONDB QUERY: " << newJsonDbQuery;
    return newJsonDbQuery;
}

QString QContactJsonDbConverter::convertFilter(const QContactFilter &filter) const {
    QString newJsonDbQuery;
    switch (filter.type()) {
    case QContactFilter::ContactDetailFilter: {
        QContactDetailFilter detailFilter;
        detailFilter = (QContactDetailFilter) filter;
        if (detailFilter.detailDefinitionName().isEmpty()) {
            //If definitionName is empty, the detail filter will match no contacts
            return "";
        }
        QString jsondbField = detailsToJsonMapping.value(detailFilter.detailDefinitionName());
        if (!jsondbField.isEmpty() && detailFilter.detailFieldName().isEmpty()) {
            // If fieldName or value is empty, the detail filter acts like a "detail exists" filter
            newJsonDbQuery.append("[?" + jsondbField + " exists]");
            break;
        }
        // Filter by name (first or last)
        if (detailFilter.detailDefinitionName() == QContactName::DefinitionName)
        {
             if (qt_debug_jsondb_contacts())
                 qDebug() << "Filter by name";
             newJsonDbQuery.append("[?" + jsondbField + "." + contactNameFieldsMapping.value(detailFilter.detailFieldName()));
             QString paramValue = detailFilter.value().toString();
             createMatchFlagQuery(newJsonDbQuery, detailFilter.matchFlags(), paramValue);
        }
        // Filter by phone number
        else if (detailFilter.detailDefinitionName() == QContactPhoneNumber::DefinitionName)
        {
             if (qt_debug_jsondb_contacts())
                 qDebug() << "Filter by phone number";
             newJsonDbQuery.append("[?" + jsondbField + ".0.value");
             QString paramValue = detailFilter.value().toString();
             createMatchFlagQuery(newJsonDbQuery, detailFilter.matchFlags(), paramValue, PhoneNumberUriScheme);
        }
        // Filter by email address
        else if (detailFilter.detailDefinitionName() == QContactEmailAddress::DefinitionName)
        {
             if (qt_debug_jsondb_contacts())
                 qDebug() << "Filter by email address";
             newJsonDbQuery.append("[?" + jsondbField + ".0.value" );
             QString paramValue = detailFilter.value().toString();
             createMatchFlagQuery(newJsonDbQuery, detailFilter.matchFlags(), paramValue);
        }
        // Filter by Url
        else if (detailFilter.detailDefinitionName() == QContactUrl::DefinitionName)
        {
            if (qt_debug_jsondb_contacts())
                qDebug() << "Filter by url";
            newJsonDbQuery.append("[?" + jsondbField + ".0.value" );
            QString paramValue = detailFilter.value().toString();
            createMatchFlagQuery(newJsonDbQuery, detailFilter.matchFlags(), paramValue);
        }
        // Default case: return all the contacts
        else {
                // No need to add anything to the already present query:   query [?_type="com.nokia.mp.contacts.Contact"]
                qWarning() << "Detail" <<  detailFilter.detailDefinitionName()
                           << "not supported by filtering, returning all the contacts !!!";
                qWarning() << "Query string: " << newJsonDbQuery;
        }
        break;
    }
    case QContactFilter::ContactDetailRangeFilter: {
        break;
    }
    case QContactFilter::LocalIdFilter: {
        QContactLocalIdFilter idFilter = (QContactLocalIdFilter) filter;
        QList<QContactLocalId> ids = idFilter.ids();
        if (!ids.isEmpty()) {
            newJsonDbQuery.append("[?" + JsonDbString::kUuidStr +
                                  " in [");
            foreach (QContactLocalId id, ids) {
                newJsonDbQuery.append("\"" + convertId(id) + "\"");
                newJsonDbQuery.append(",");
            }
            newJsonDbQuery.chop(1);
            newJsonDbQuery.append("]]");
        }
        break;
    }
    default:
        break;
    }
    if (qt_debug_jsondb_contacts())
        qDebug() << "FILTER PART OF THE QUERY: " << newJsonDbQuery;
    return newJsonDbQuery;
}

QString QContactJsonDbConverter::convertSortOrder(const QList<QContactSortOrder> &sortOrders) const {
    QString newJsonDbQuery;
    foreach (QContactSortOrder sortOrder, sortOrders) {
        if (sortOrder.detailDefinitionName() == QContactName::DefinitionName) {
            sortOrder.direction() == Qt::AscendingOrder ? newJsonDbQuery.append("[/") : newJsonDbQuery.append("[\\");
            newJsonDbQuery.append(detailsToJsonMapping.value(QContactName::DefinitionName));
            if (sortOrder.detailFieldName() == QContactName::FieldFirstName) {
                newJsonDbQuery.append("." + contactNameFieldsMapping.value(QContactName::FieldFirstName) + "]");
            } else if (sortOrder.detailFieldName() == QContactName::FieldLastName) {
                newJsonDbQuery.append("." + contactNameFieldsMapping.value(QContactName::FieldLastName) + "]");
            }
        }
        else if (sortOrder.detailDefinitionName() == QContactEmailAddress::DefinitionName) {
            sortOrder.direction() == Qt::AscendingOrder ? newJsonDbQuery.append("[/") : newJsonDbQuery.append("[\\");
            newJsonDbQuery.append(detailsToJsonMapping.value(QContactEmailAddress::DefinitionName));
            newJsonDbQuery.append("."); // + "0" + "." + "value" + "]");
            newJsonDbQuery.append("0");
            newJsonDbQuery.append(".");
            newJsonDbQuery.append("value");
            newJsonDbQuery.append("]");
        }
    }
    if (qt_debug_jsondb_contacts())
        qDebug() << "SORTING PART OF THE QUERY: " << newJsonDbQuery;
    return newJsonDbQuery;
}

#include <quuid.h>
QContactId QContactJsonDbConverter::convertId(const QString& id, const QContactJsonDbEngine& engine) const {
    QContactId newId;
    //QContactLocalId localId = QT_PREPEND_NAMESPACE(qHash)(id);
    QContactLocalId localId = id;
    newId.setLocalId(localId);
    newId.setManagerUri(engine.managerUri());
    return  newId;
}

QString QContactJsonDbConverter::convertId(const QContactLocalId& id) const {
    // TODO: CHECK THIS AND DO IT PROPERLY !!
    return id;
}

void QContactJsonDbConverter::initializeMappings()
{
    detailsToJsonMapping.insert(QContactName::DefinitionName, "name");
    detailsToJsonMapping.insert(QContactGender::DefinitionName, "gender");
    detailsToJsonMapping.insert(QContactPhoneNumber::DefinitionName, "phones");
    detailsToJsonMapping.insert(QContactEmailAddress::DefinitionName, "emails");
    detailsToJsonMapping.insert(QContactUrl::DefinitionName, "urls");
    detailsToJsonMapping.insert(QContactNickname::DefinitionName,"nickname");
    detailsToJsonMapping.insert(QContactAddress::DefinitionName,"addresses");
    detailsToJsonMapping.insert(QContactBirthday::DefinitionName,"birthday");
    detailsToJsonMapping.insert(QContactAvatar::DefinitionName,"photoUrl");
    detailsToJsonMapping.insert(QContactRingtone::DefinitionName,"ringtoneUrl");
    detailsToJsonMapping.insert(QContactOrganization::DefinitionName,"organization");
    detailsToJsonMapping.insert(QContactNote::DefinitionName, "note");
    detailsToJsonMapping.insert(QContactPersonId::DefinitionName, "personid");
    contactNameFieldsMapping.insert(QContactName::FieldCustomLabel,"customLabel");
    contactNameFieldsMapping.insert(QContactName::FieldFirstName,"firstName");
    contactNameFieldsMapping.insert(QContactName::FieldLastName,"lastName");
    contactNameFieldsMapping.insert(QContactName::FieldMiddleName,"middleName");
    contactNameFieldsMapping.insert(QContactName::FieldPrefix,"prefix");
    contactNameFieldsMapping.insert(QContactName::FieldSuffix,"suffix");
    //contactNameFieldsMapping.insert(QContactPhoneNumber::FieldPhoneNumbers,"phoneNumbers");
    addressFieldsMapping.insert(QContactAddress::FieldCountry,"country");
    addressFieldsMapping.insert(QContactAddress::FieldRegion,"region");
    addressFieldsMapping.insert(QContactAddress::FieldLocality,"locality");
    addressFieldsMapping.insert(QContactAddress::FieldPostcode,"postcode");
    addressFieldsMapping.insert(QContactAddress::FieldPostOfficeBox,"postOfficeBox");
    addressFieldsMapping.insert(QContactAddress::FieldStreet,"street");
    organizationFieldsMapping.insert(QContactOrganization::FieldName,"name");
    organizationFieldsMapping.insert(QContactOrganization::FieldDepartment,"department");
    organizationFieldsMapping.insert(QContactOrganization::FieldTitle,"title");
    organizationFieldsMapping.insert(QContactOrganization::FieldRole,"role");
    organizationFieldsMapping.insert(QContactOrganization::FieldAssistantName,"assistantName");
    organizationFieldsMapping.insert(QContactOrganization::FieldLogoUrl,"logoUrl");
    organizationFieldsMapping.insert(QContactOrganization::FieldStartDate,"startDate");
    organizationFieldsMapping.insert(QContactOrganization::FieldEndDate,"endDate");
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeMobile, SubTypeCell);
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeFax, SubTypeFax);
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeVideo, SubTypeVideo);
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeLandline, SubTypeLandline);
    //TODO: FINISH THE MAPPING(S)
    //MISSING DETAILS / FIELDS (TO BE ADDED ALSO TO PARSING LOGIC):
    // - QContactTimestamp
    // - QContactRingtone
    // - QContactAvatar
    // - QContactOrganization
    // - QContactGender
    // - QContactOnlineAccount
    // - QContactThumbnail
    // - QContactBirthday
    // TODO: Decide what to do with NickName (seems to be out of scope from latest (
}

bool QContactJsonDbConverter::toQContacts(const QVariantList& jsonObjects, QList<QContact>& convertedContacts, const QContactJsonDbEngine& engine, QContactManager::Error& error)
{//TODO: ERROR HANDLING
    if (jsonObjects.isEmpty()) {
        error = QContactManager::DoesNotExistError;
        return false;
    }
    for (int i = 0; i < jsonObjects.size(); i++) {
        QContact contact;
        if (this->toQContact(jsonObjects.at(i).toMap(), &contact, engine)) {
            convertedContacts.append(contact);
        }
    }
    error = QContactManager::NoError;
    return true;
}

void QContactJsonDbConverter::createMatchFlagQuery(QString& queryString, QContactFilter::MatchFlags flags, const QString& value, const QString& UriScheme) const
{
    switch (flags) {
        // The "contains" word is not recognized by JsonDb, we are forced to use a workaround here
        case QContactFilter::MatchContains: queryString.append("=~\"/*" + value + "*/wi\"]"); break;
        case QContactFilter::MatchFixedString: break;
        case QContactFilter::MatchCaseSensitive: break;
        case QContactFilter::MatchExactly: queryString.append("=\"" + UriScheme + value + "\"]"); break;
        case QContactFilter::MatchStartsWith: queryString.append(" =~ \"/"+ UriScheme + value + "*/wi\"]"); break;
        case QContactFilter::MatchEndsWith: queryString.append(" =~ \"/*" + value + "/wi\"]"); break;
        default:
          break;
    }
}

QString QContactJsonDbConverter::toJsonDate(const QDateTime& date) const
{
    QString dateString = date.toString(Qt::ISODate) + "Z";
    return dateString;
}

QDateTime QContactJsonDbConverter::toContactDate(const QString &dateString) const
{
    //removes the ending "Z" character required by Jsondb DateTime format.
    QDateTime date = QDateTime::fromString(dateString.left(dateString.length()-1),Qt::ISODate);
    return date;
}

void QContactJsonDbConverter::extendedDetailToJsonDbProperty(const QContactExtendedDetail &extendedDetail, QVariant &property) const
{
    // TODO check potential conflicts with predefined detail
    if (extendedDetail.name().isEmpty() || extendedDetail.data().isNull()) {
        return;
    } else if ((extendedDetail.data().type() > QVariant::Invalid) && (extendedDetail.data().type() < QVariant::Map)) {
        // This covers types  Bool, int, uint, qlonglong, qulonglong, double, QChar.
        // These types need no manual conversion to string for storing to jsondb.
        // For efficiency condition above relies on numeric values for these types in QVariant.
        property.setValue(extendedDetail.data());
    } else if (extendedDetail.data().type() == QVariant::DateTime) {
        QDateTime dateTime = extendedDetail.data().toDateTime();
        property.setValue(toJsonDate(dateTime));
    } else if (extendedDetail.data().canConvert(QVariant::String)) {
        property.setValue(extendedDetail.data().toString());
    } else if (extendedDetail.data().type() == QVariant::List) {
        QVariantList variantList;
        dataToList(extendedDetail.data(), variantList);
        property.setValue(variantList);
    } else if (extendedDetail.data().type() == QVariant::Map) {
        QVariantMap variantMap;
        dataToMap(extendedDetail.data(), variantMap);
        property.setValue(variantMap);
    }
}

void QContactJsonDbConverter::dataToList(const QVariant &data, QVariantList &list) const
{
    if (data.type() != QVariant::List) {
        return;
    }

    QList<QVariant> originalList(data.toList());
    foreach (const QVariant &variant, originalList) {
        if (variant.isNull())
            continue;
        if ((variant.type() > QVariant::Invalid) && (variant.type() < QVariant::Map)) {
            // This covers types  Bool, int, uint, qlonglong, qulonglong, double, QChar.
            list.append(variant);
        } else if (variant.canConvert(QVariant::String)) {
            list.append(variant.toString());
        } else if (variant.type() == QVariant::List) {
            QVariantList subList;
            dataToList(variant, subList);
            list.insert(list.size(), subList);
        } else if (variant.type() == QVariant::Map) {
            QVariantMap map;
            dataToMap(variant, map);
            list.append(map);
        }
    }
}

void QContactJsonDbConverter::dataToMap(const QVariant &data, QVariantMap &map) const
{
    if (data.type() != QVariant::Map)
        return;

    QMap<QString, QVariant> originalMap(data.toMap());
    QMap<QString, QVariant>::const_iterator i = originalMap.constBegin();
    while (i != originalMap.constEnd()) {
        if (i.value().isValid() && !i.value().isNull()) {
            if ((i.value().type() > QVariant::Invalid) && (i.value().type() < QVariant::Map)) {
                // This covers types  Bool, int, uint, qlonglong, qulonglong, double, QChar.
                map.insert(i.key(), i.value());
            } else if (i.value().canConvert(QVariant::String)) {
                map.insert(i.key(), i.value().toString());
            } else if (i.value().type() == QVariant::List) {
                QVariantList list;
                dataToList(i.value(), list);
                map.insert(i.key(), list);
            } else if (i.value().type() == QVariant::Map) {
                QVariantMap embedMap;
                dataToMap(i.value(), embedMap);
                map.insert(i.key(), embedMap);
            }
        }
        ++i;
    }
}

QTCONTACTS_END_NAMESPACE
