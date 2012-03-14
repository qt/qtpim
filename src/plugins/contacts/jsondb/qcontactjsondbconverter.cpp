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

#include <QList>
#include <QVariantMap>
#include <QVariantList>
#include <QDebug>
#include <QCryptographicHash>
#include <QUuid>

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
#include "qcontactidfilter.h"

#include "qcontactjsondbconverter.h"
#include "qcontactjsondbglobal.h"
#include "qcontactjsondbstring.h"
#include "qcontactjsondbid.h"

QTCONTACTS_BEGIN_NAMESPACE

const int QContactJsonDbConverter::jsonDbVersionLength(10);

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

    stringValue = object.value(QContactJsonDbStr::uuid()).toString();
    if (stringValue.isEmpty()) {
        return false;
    }
    contact->setId(QContactId(new QContactJsonDbId(stringValue)));

    // TODO: other types
    contact->setType(QContactType::TypeContact);

    // Go through all fields in loop.
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QContactPersonId* personid = new QContactPersonId();

    while (i != object.constEnd()) {
        if (i.key() == detailsToJsonMapping.value(QContactPersonId::Type)) {
            //personid
            stringValue = object[detailsToJsonMapping.value(QContactPersonId::Type)].toString();
            if (!stringValue.isEmpty()) {
                personid->setPersonId(stringValue);
            }
            else { // if no personid is stored in backend, we return the local
                personid->setPersonId(contact->id().toString());
            }
        } else if (i.key() == QContactJsonDbStr::version()) {
            //version
            QContactVersion* contactVersion = new QContactVersion();
            jsonDbVersionToContactVersion(i.value().toString(), contactVersion);
            detailList << contactVersion;
        } else if (i.key() == detailsToJsonMapping.value(QContactName::Type)) {
            //name
            QContactName* name = new QContactName();
            map = object[detailsToJsonMapping.value(QContactName::Type)].value<QVariantMap>();
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
            stringValue = map[contactNameFieldsMapping.value(QContactName::FieldCustomLabel)].toString();
            if (!stringValue.isEmpty())
                name->setCustomLabel(stringValue);
            detailList << name;
        } else if (i.key() == detailsToJsonMapping.value(QContactGender::Type)) {
            //gender
            QContactGender* gender = new QContactGender();
            map = object[detailsToJsonMapping.value(QContactGender::Type)].value<QVariantMap>();
            stringValue = map[detailsToJsonMapping.value(QContactGender::Type)].toString();
            if (!stringValue.isEmpty())
                gender->setGender(static_cast<QContactGender::GenderField>(genderValuesMapping.key(stringValue)));
            detailList << gender;
        } else if (i.key() == detailsToJsonMapping.value(QContactOrganization::Type)) {
            //organization
            QContactOrganization* organization = new QContactOrganization();
            array = object[detailsToJsonMapping.value(QContactOrganization::Type)].value<QVariantList>();
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
        } else if (i.key() == QContactJsonDbStr::contactDetails()) {
            map = object[QContactJsonDbStr::contactDetails()].value<QVariantMap>();

            //birthday
            QString dateString;
            dateString = map[detailsToJsonMapping.value(QContactBirthday::Type)].toString();
            if (!dateString.isEmpty()) {
                QDateTime date = toContactDate(dateString);
                QContactBirthday* birthday = new QContactBirthday();
                birthday->setDateTime(date);
                detailList << birthday;
            }

            //avatar
            QString avatarUrlString;
            avatarUrlString = map[detailsToJsonMapping.value(QContactAvatar::Type)].toString();
            if (!avatarUrlString.isEmpty()) {
                QUrl avatarUrl(avatarUrlString);
                QContactAvatar* avatar = new QContactAvatar();
                avatar->setImageUrl(avatarUrl);
                detailList << avatar;
            }

            //ringtone
            QString ringtoneUrlString;
            ringtoneUrlString = map[detailsToJsonMapping.value(QContactRingtone::Type)].toString();
            if (!ringtoneUrlString.isEmpty()) {
                QUrl ringtoneUrl(ringtoneUrlString);
                QContactRingtone* ringtone = new QContactRingtone();
                ringtone->setAudioRingtoneUrl(ringtoneUrl);
                detailList << ringtone;
            }

            //nickname
            QString nickString;
            nickString = map[detailsToJsonMapping.value(QContactNickname::Type)].toString();
            if (!nickString.isEmpty()) {
                QContactNickname* nick = new QContactNickname();
                nick->setNickname(nickString);
                detailList << nick;
            }

            //note
            QString noteString;
            noteString = map[detailsToJsonMapping.value(QContactNote::Type)].toString();
            if (!noteString.isEmpty()) {
                QContactNote* note = new QContactNote();
                note->setNote(noteString);
                detailList << note;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactEmailAddress::Type)) {
            //email
            array = object[detailsToJsonMapping.value(QContactEmailAddress::Type)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactEmailAddress* email = new QContactEmailAddress;
                map = array[i].value<QVariantMap>();
                email->setEmailAddress(map["value"].toString());
                updateContexts(map,email);
                detailList << email;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactPhoneNumber::Type)) {
            //phone number
            array = object[detailsToJsonMapping.value(QContactPhoneNumber::Type)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactPhoneNumber* number = new QContactPhoneNumber;
                map = array[i].value<QVariantMap>();
                number->setNumber(map["value"].toString().remove(QContactJsonDbStr::phoneNumberUriScheme()));
                stringValue = map["context"].toString();
                if (stringValue == QContactJsonDbStr::contextHome() ||
                        stringValue == QContactJsonDbStr::contextWork() ||
                        stringValue == QContactJsonDbStr::contextOther()) {
                    updateContexts(map, number);
                }
                stringValue = map["subType"].toString();
                if (stringValue == QContactJsonDbStr::subTypeFax()) {
                    QList<int> myType;
                    myType << QContactPhoneNumber::SubTypeFax;
                    number->setSubTypes(myType);
                } else if (stringValue == QContactJsonDbStr::subTypeCell()) {
                    QList<int> myType;
                    myType << QContactPhoneNumber::SubTypeMobile;
                    number->setSubTypes(myType);
                } else if (stringValue == QContactJsonDbStr::subTypeVideo()) {
                    QList<int> myType;
                    myType << QContactPhoneNumber::SubTypeVideo;
                    number->setSubTypes(myType);
                } else if (stringValue == QContactJsonDbStr::subTypeLandline()) {
                    QList<int> myType;
                    myType << QContactPhoneNumber::SubTypeLandline;
                    number->setSubTypes(myType);
                };
                detailList << number;
            }
        } else if (i.key() == detailsToJsonMapping.value(QContactAddress::Type)) {
            //address
            array = object[detailsToJsonMapping.value(QContactAddress::Type)].value<QVariantList>();
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
        } else if (i.key() == detailsToJsonMapping.value(QContactUrl::Type)) {
            //url
            array = object[detailsToJsonMapping.value(QContactUrl::Type)].value<QVariantList>();
            for (int i = 0; i < array.size(); ++i) {
                QContactUrl* url = new QContactUrl;
                map = array[i].value<QVariantMap>();
                url->setUrl(map["value"].toString());
                if (updateContexts(map, url)) {
                    url->setSubType(static_cast<QContactUrl::SubType>(url->contexts().first()));//TODO decide if we use "Context" or "SubTypes" to store the jsondb SubTypes
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
         personid->setPersonId(contact->id().toString());
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
                     << error << "for contact " << object.value(QContactJsonDbStr::uuid()).toString();
        QContactManagerEngine::setContactDisplayLabel(contact, "");
    };
    if (contact->isEmpty()) {
        return false;
    } else {
        return true;
    };
}

bool QContactJsonDbConverter::toJsonContact(QVariantMap* object, const QContact& contact) {
    QList<QContactDetail> details = contact.details();
    QContactDetail detail;
    QContactName* name;
    QContactNickname* nick;
    QContactEmailAddress* email;
    QContactPhoneNumber* number;
    QContactAddress* address;
    QContactUrl* url;
    QContactVersion* version;
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

    if (!contact.id().isNull()) {
        QString jsonUuid = contact.id().toString().remove (QContactJsonDbStr::managerName());
        object->insert (QContactJsonDbStr::uuid(), jsonUuid);
    }
    // get all available contact details.
    object->insert(QContactJsonDbStr::type(), QContactJsonDbStr::contactsJsonDbType());

    // Quickfix for preserving possible extra fields in jsondb contact.
    // Wipe QContact fields that may be empty/deleted, preserve all other data.
    if (!object->empty()) {
        object->remove(detailsToJsonMapping.value(QContactName::Type));
        object->remove(detailsToJsonMapping.value(QContactPersonId::Type));
        object->remove(detailsToJsonMapping.value(QContactGender::Type));
        object->remove(detailsToJsonMapping.value(QContactOrganization::Type));
        object->remove(detailsToJsonMapping.value(QContactEmailAddress::Type));
        object->remove(detailsToJsonMapping.value(QContactPhoneNumber::Type));
        object->remove(detailsToJsonMapping.value(QContactAddress::Type));
        object->remove(detailsToJsonMapping.value(QContactUrl::Type));
        embeddedDetailsMap = object->value(QContactJsonDbStr::contactDetails()).toMap();
        object->remove(QContactJsonDbStr::contactDetails());
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactBirthday::Type));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactAvatar::Type));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactRingtone::Type));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactNickname::Type));
        embeddedDetailsMap.remove(detailsToJsonMapping.value(QContactNote::Type));
        //  Preseserve possible extra contact details jsondb contact object may have.
        if (!embeddedDetailsMap.isEmpty()) {
            object->insert(QContactJsonDbStr::contactDetails(), embeddedDetailsMap);
        }
    }
    // End of Quickfix

    for(int i = 0; i < details.size(); ++i) {
        detail = details.at(i);
        // name
        if (detail.type() == QContactName::Type) {
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
            if (!name->customLabel().isEmpty())
                nameMap[contactNameFieldsMapping.value(QContactName::FieldCustomLabel)] = name->customLabel();
            object->insert(detailsToJsonMapping.value(QContactName::Type), nameMap);
        }
        //personid
        else if (detail.type() == QContactPersonId::Type) {
            personid = static_cast<QContactPersonId *>(&detail);
            if(!personid->personId().isEmpty() )
                object->insert(detailsToJsonMapping.value(QContactPersonId::Type),personid->personId());
        }
        //gender
        else if (detail.type() == QContactGender::Type) {
            QVariantMap genderMap;
            gender = static_cast<QContactGender *>(&detail);
            genderMap[detailsToJsonMapping.value(QContactGender::Type)] = genderValuesMapping.value(gender->gender());
            object->insert(detailsToJsonMapping.value(QContactGender::Type), genderMap);
        }
        //organization
        else if (detail.type() == QContactOrganization::Type) {
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
            organizations.append(organizationMap);
            object->insert(detailsToJsonMapping.value(QContactOrganization::Type), organizations);
        }
        // birthday
        else if (detail.type() == QContactBirthday::Type) {
            birthday = static_cast<QContactBirthday *>(&detail);
            QDateTime date = birthday->dateTime();
            QString dateString = toJsonDate(date);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactBirthday::Type)] = dateString;
            object->insert(QContactJsonDbStr::contactDetails(), embeddedDetailsMap);
        }
        //avatar
        else if (detail.type() == QContactAvatar::Type) {
            avatar = static_cast<QContactAvatar *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactAvatar::Type)] = avatar->imageUrl().toString();
            object->insert(QContactJsonDbStr::contactDetails(), embeddedDetailsMap);
        }
        //ringtone
        else if (detail.type() == QContactRingtone::Type) {
            ringtone = static_cast<QContactRingtone *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactRingtone::Type)] = ringtone->audioRingtoneUrl().toString();
            object->insert(QContactJsonDbStr::contactDetails(), embeddedDetailsMap);
        }
        // nickname
        else if (detail.type() == QContactNickname::Type) {
            nick = static_cast<QContactNickname *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactNickname::Type)] = nick->nickname();
            object->insert(QContactJsonDbStr::contactDetails(), embeddedDetailsMap);
        }
        //note
        else if (detail.type() == QContactNote::Type) {

            note = static_cast<QContactNote *>(&detail);
            embeddedDetailsMap[detailsToJsonMapping.value(QContactNote::Type)] = note->note();
            object->insert(QContactJsonDbStr::contactDetails(), embeddedDetailsMap);
        }
        // email
        else if (detail.type() == QContactEmailAddress::Type) {
            QVariantMap emailMap;
            email = static_cast<QContactEmailAddress *>(&detail);
            emailMap["value"] = email->emailAddress();
            updateContexts(*email, &emailMap);
            emails.append(emailMap);
            object->insert(detailsToJsonMapping.value(QContactEmailAddress::Type), emails);
        }
        // phone number
        else if (detail.type() == QContactPhoneNumber::Type) {
            QVariantMap phoneMap;
            number = static_cast<QContactPhoneNumber *>(&detail);
            phoneMap["value"] = QContactJsonDbStr::phoneNumberUriScheme() + number->number();
            updateContexts(*number, &phoneMap);
            QList<int> subTypes = number->subTypes();
            if (!subTypes.empty())
                phoneMap["subType"] = phoneNumbersSubtypesMapping.value(number->subTypes().first());
            phoneNumbers.append(phoneMap);
            object->insert(detailsToJsonMapping.value(QContactPhoneNumber::Type), phoneNumbers);
        }
        // address
        else if (detail.type() == QContactAddress::Type) {
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
            object->insert(detailsToJsonMapping.value(QContactAddress::Type), addresses);
        }
        // url
        else if (detail.type() == QContactUrl::Type) {
            QVariantMap urlMap;
            url = static_cast<QContactUrl *>(&detail);
            urlMap["value"] = url->url();
            updateContexts(*url, &urlMap);
            urls.append(urlMap);
            object->insert(detailsToJsonMapping.value(QContactUrl::Type), urls);
        }
        // version
        else if ( (detail.type() == QContactVersion::Type) ) {
            version = static_cast<QContactVersion *>(&detail);
            QString jsonDbVersion;
            contactVersionToJsonDbVersion(*version, &jsonDbVersion);
            if (!jsonDbVersion.isEmpty())
                object->insert(QContactJsonDbStr::version(), jsonDbVersion);
        }
        // extended details
        else if (detail.type() == QContactExtendedDetail::Type) {
            extendedDetail = static_cast<QContactExtendedDetail *>(&detail);
            QString extDetailName = extendedDetail->name();
            if ( (!extDetailName.isEmpty()) && ((extDetailName.at(0) != QChar('_'))) ) {
                QVariant property;
                extendedDetailToJsonDbProperty(*extendedDetail, property);
                if (!property.isNull())
                    object->insert(extDetailName, property);
            }
        }
        // display label
        else if (detail.type() == QContactDisplayLabel::Type || detail.type() == QContactType::Type) {
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
    if (stringValue == QContactJsonDbStr::contextHome()) {
        detail->setContexts(QContactDetail::ContextHome);
        return true;
    } else if (stringValue == QContactJsonDbStr::contextWork()) {
        detail->setContexts(QContactDetail::ContextWork);
        return true;
    } else if (stringValue == QContactJsonDbStr::contextOther()) {
        detail->setContexts(QContactDetail::ContextOther);
        return true;
    }
    return false;
}

bool QContactJsonDbConverter::updateContexts(const QContactDetail& detail, QVariantMap* object) {
    QList<int> contexts = detail.contexts();
    if(contexts.size() == 0)
        return false;
    if(contexts.contains(QContactDetail::ContextHome))
        object->insert("context", QContactJsonDbStr::contextHome());
    else if(contexts.contains(QContactDetail::ContextWork))
        object->insert("context", QContactJsonDbStr::contextWork());
    else if(contexts.contains(QContactDetail::ContextOther))
        object->insert("context", QContactJsonDbStr::contextOther());
    return true;
}

bool QContactJsonDbConverter::queryFromRequest(QContactAbstractRequest *request,QString &newJsonDbQuery)
{
    bool isValidFilter = false;
    if (!request) {
        newJsonDbQuery =  "";
        return isValidFilter;
    }

    newJsonDbQuery = "[?" + QContactJsonDbStr::type() + "=\""+ QContactJsonDbStr::contactsJsonDbType() + "\"]";
    switch (request->type()) {
    case QContactAbstractRequest::ContactSaveRequest: {
        //TODO:
        break;
    }
    case QContactAbstractRequest::ContactFetchRequest: {
        QContactFetchRequest* fetchReq = static_cast<QContactFetchRequest*>(request);
        QContactFilter filter = fetchReq->filter();
        QString filterString;
        isValidFilter = compoundFilterToJsondbQuery(filter,filterString);
        newJsonDbQuery.append(filterString);
        if (!isValidFilter)
            return isValidFilter;
        QList<QContactSortOrder> sorting = fetchReq->sorting();
        newJsonDbQuery.append(convertSortOrder(sorting));
        break;
    }
    case QContactAbstractRequest::ContactIdFetchRequest: {
        newJsonDbQuery.append("[=");
        newJsonDbQuery.append(QContactJsonDbStr::uuid());
        newJsonDbQuery.append("]");
        QContactIdFetchRequest* idReq = static_cast<QContactIdFetchRequest*>(request);
        QContactFilter filter = idReq->filter();
        QString filterString;
        isValidFilter = compoundFilterToJsondbQuery(filter,filterString);
        newJsonDbQuery.append(filterString);
        if (!isValidFilter)
            return isValidFilter;
        QList<QContactSortOrder> sorting = idReq->sorting();
        newJsonDbQuery.append(convertSortOrder(sorting));
        break;
    }
    default:
        break;
    }
    if (qt_debug_jsondb_contacts())
        qDebug() << " JSONDB QUERY: " << newJsonDbQuery;
    return isValidFilter;
}

bool QContactJsonDbConverter::compoundFilterToJsondbQuery(const QContactFilter &filter, QString &jsonDbQueryStr) const
{
    bool isValidFilter = true;
    switch (filter.type()) {
    case QContactFilter::IntersectionFilter: {
        const QContactIntersectionFilter isf(filter);
        const QList<QContactFilter> filterList = isf.filters();
        foreach (const QContactFilter &filter, filterList){
            QString filterStr;
            if (compoundFilterToJsondbQuery(filter, filterStr))
                jsonDbQueryStr += filterStr;
            else //For intersection filter, single filter invalid means empty result from jsondb query
                isValidFilter = false;
        }
        break;
    }
    case QContactFilter::UnionFilter: {
        //not supported yet
        isValidFilter = false;
        break;
    }
    default:
        isValidFilter = singleFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    }
    if (!isValidFilter)
        jsonDbQueryStr.clear();

    if (qt_debug_jsondb_contacts()) {
        if (filter.type() == QContactFilter::IntersectionFilter)
            qDebug()<<"INTERSECTION FILTER PART OF THE QUERY:"<<jsonDbQueryStr;
    }

    return isValidFilter;
}

bool QContactJsonDbConverter::singleFilterToJsondbQuery(const QContactFilter &filter,QString &jsonDbQueryStr) const
{
    bool isValidFilter = true;
    switch (filter.type()) {
    case QContactFilter::ContactDetailFilter: {
        isValidFilter = detailFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    }
    case QContactFilter::IdFilter: {
        isValidFilter = idFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    }
    case QContactFilter::DefaultFilter: {
        jsonDbQueryStr.clear();
        isValidFilter = true;
        break;
    }
    case QContactFilter::InvalidFilter:
    case QContactFilter::ContactDetailRangeFilter:
    case QContactFilter::ChangeLogFilter:
    case QContactFilter::ActionFilter:
    case QContactFilter::RelationshipFilter:
    case QContactFilter::IntersectionFilter:
    case QContactFilter::UnionFilter:
    default: {
        jsonDbQueryStr.clear();
        isValidFilter = false;
        break;
    }
    }
    if (qt_debug_jsondb_contacts())
        qDebug() << "SINGLE FILTER PART OF THE QUERY: " << jsonDbQueryStr;
    return isValidFilter;
}

bool QContactJsonDbConverter::detailFilterToJsondbQuery(const QContactFilter &filter, QString &jsonDbQueryStr) const
{
    //bool isValidFilter = isSupportedDetailFilter(filter, detailType, detailField);
    //TODO  isSupportedDetailFilter implementation to be done, as of now we assume that support
    //filters for all detail types
    bool isValidFilter  = true;
    QContactDetailFilter detailFilter(filter);
    if (detailFilter.detailType() == QContactDetail::TypeUndefined) {
        //If definitionName is empty, the detail filter will match no contacts
        jsonDbQueryStr = "";
        return false;
    }
    QString jsondbField = detailsToJsonMapping.value(detailFilter.detailType());
    if (!jsondbField.isEmpty() && detailFilter.detailField()<0) {
        // If fieldName or value is empty, the detail filter acts like a "detail exists" filter
        jsonDbQueryStr.append("[?" + jsondbField + " exists]");
        return true;
    }
    // Filter by name (first or last)
    if (detailFilter.detailType() == QContactName::Type)
    {
        if (qt_debug_jsondb_contacts())
            qDebug() << "Filter by name";
        jsonDbQueryStr.append("[?" + jsondbField + "." + contactNameFieldsMapping.value(detailFilter.detailField()));
        QString paramValue = detailFilter.value().toString();
        createMatchFlagQuery(jsonDbQueryStr, detailFilter.matchFlags(), paramValue);
    }
    // Filter by phone number
    else if (detailFilter.detailType() == QContactPhoneNumber::Type)
    {
        if (qt_debug_jsondb_contacts())
            qDebug() << "Filter by phone number";
        if (detailFilter.detailField() == QContactPhoneNumber::FieldNumber) {
            jsonDbQueryStr.append("[?" + jsondbField + ".0.value");
            QString paramValue = detailFilter.value().toString();
            createMatchFlagQuery(jsonDbQueryStr, detailFilter.matchFlags(), paramValue, QContactJsonDbStr::phoneNumberUriScheme());
        } else {
            //We do not support currently filtering by other fields than number
            isValidFilter = false;
        }
    }
    // Filter by email address
    else if (detailFilter.detailType() == QContactEmailAddress::Type)
    {
        if (qt_debug_jsondb_contacts())
            qDebug() << "Filter by email address";
        if (detailFilter.detailField() == QContactEmailAddress::FieldEmailAddress) {
            jsonDbQueryStr.append("[?" + jsondbField + ".0.value" );
            QString paramValue = detailFilter.value().toString();
            createMatchFlagQuery(jsonDbQueryStr, detailFilter.matchFlags(), paramValue);
        } else {
            //We do not support currently filter by other fields than email address
            isValidFilter = false;
        }
    }
    // Filter by Url
    else if (detailFilter.detailType() == QContactUrl::Type)
    {
        if (qt_debug_jsondb_contacts())
            qDebug() << "Filter by url";
        if (detailFilter.detailField() == QContactUrl::FieldUrl) {
            jsonDbQueryStr.append("[?" + jsondbField + ".0.value" );
            QString paramValue = detailFilter.value().toString();
            createMatchFlagQuery(jsonDbQueryStr, detailFilter.matchFlags(), paramValue);
        } else {
            //We do not support currently filter by other fields than Url
            isValidFilter = false;
        }
    }
    // Default case: return all the contacts
    else {
        // No need to add anything to the already present query:   query [?_type="com.nokia.mt.contacts.Contact"]
        qWarning() << "Detail" <<  detailFilter.detailType()
                   << "not supported by filtering";
        qWarning() << "Query string: " << jsonDbQueryStr;
        isValidFilter = false;
    }
    return isValidFilter;
}

bool QContactJsonDbConverter::idFilterToJsondbQuery(const QContactFilter &filter, QString &newJsonDbQuery) const
{
    QContactIdFilter idFilter(filter);
    QList<QContactId> ids = idFilter.ids();
    if (!ids.isEmpty()) {
        newJsonDbQuery.append("[?" + QContactJsonDbStr::uuid() +
                              " in [");
        foreach (const QContactId &id, ids) {
            newJsonDbQuery.append("\"" + convertId(id) + "\"");
            newJsonDbQuery.append(",");
        }
        newJsonDbQuery.chop(1);
        newJsonDbQuery.append("]]");
    }
    return true;
}

QString QContactJsonDbConverter::convertSortOrder(const QList<QContactSortOrder> &sortOrders) const {
    QString newJsonDbQuery;
    foreach (QContactSortOrder sortOrder, sortOrders) {
        if (sortOrder.detailType() == QContactName::Type) {
            sortOrder.direction() == Qt::AscendingOrder ? newJsonDbQuery.append("[/") : newJsonDbQuery.append("[\\");
            newJsonDbQuery.append(detailsToJsonMapping.value(QContactName::Type));
            if (sortOrder.detailField() == QContactName::FieldFirstName) {
                newJsonDbQuery.append("." + contactNameFieldsMapping.value(QContactName::FieldFirstName) + "]");
            } else if (sortOrder.detailField() == QContactName::FieldLastName) {
                newJsonDbQuery.append("." + contactNameFieldsMapping.value(QContactName::FieldLastName) + "]");
            }
        }
        else if (sortOrder.detailType() == QContactEmailAddress::Type) {
            sortOrder.direction() == Qt::AscendingOrder ? newJsonDbQuery.append("[/") : newJsonDbQuery.append("[\\");
            newJsonDbQuery.append(detailsToJsonMapping.value(QContactEmailAddress::Type));
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

QString QContactJsonDbConverter::convertId(const QContactId &id) const
{
    if (id.isNull())
        return QString();

    return id.toString().remove(QContactJsonDbStr::managerName());
}

void QContactJsonDbConverter::initializeMappings()
{
    detailsToJsonMapping.insert(QContactName::Type, QContactJsonDbStr::nameDefinitionName());
    detailsToJsonMapping.insert(QContactGender::Type, QContactJsonDbStr::genderDefinitionName());
    detailsToJsonMapping.insert(QContactPhoneNumber::Type, QContactJsonDbStr::phoneNumberDefinitionName());
    detailsToJsonMapping.insert(QContactEmailAddress::Type, QContactJsonDbStr::emailAddressDefinitionName());
    detailsToJsonMapping.insert(QContactUrl::Type, QContactJsonDbStr::urlDefinitionName());
    detailsToJsonMapping.insert(QContactNickname::Type, QContactJsonDbStr::nicknameDefinitionName());
    detailsToJsonMapping.insert(QContactAddress::Type, QContactJsonDbStr::addressDefinitionName());
    detailsToJsonMapping.insert(QContactBirthday::Type, QContactJsonDbStr::birthdayDefinitionName());
    detailsToJsonMapping.insert(QContactAvatar::Type, QContactJsonDbStr::avatarDefinitionName());
    detailsToJsonMapping.insert(QContactRingtone::Type, QContactJsonDbStr::ringtoneDefinitionName());
    detailsToJsonMapping.insert(QContactOrganization::Type,QContactJsonDbStr::organizationDefinitionName());
    detailsToJsonMapping.insert(QContactNote::Type, QContactJsonDbStr::noteDefinitionName());
    detailsToJsonMapping.insert(QContactPersonId::Type, QContactJsonDbStr::personIdDefinitionName());
    contactNameFieldsMapping.insert(QContactName::FieldCustomLabel, QContactJsonDbStr::nameFieldCustomLabel());
    contactNameFieldsMapping.insert(QContactName::FieldFirstName, QContactJsonDbStr::nameFieldFirstName());
    contactNameFieldsMapping.insert(QContactName::FieldLastName, QContactJsonDbStr::nameFieldLastName());
    contactNameFieldsMapping.insert(QContactName::FieldMiddleName, QContactJsonDbStr::nameFieldMiddleName());
    contactNameFieldsMapping.insert(QContactName::FieldPrefix, QContactJsonDbStr::nameFieldPrefix());
    contactNameFieldsMapping.insert(QContactName::FieldSuffix, QContactJsonDbStr::nameFieldSuffix());
    addressFieldsMapping.insert(QContactAddress::FieldCountry, QContactJsonDbStr::addressFieldCountry());
    addressFieldsMapping.insert(QContactAddress::FieldRegion, QContactJsonDbStr::addressFieldRegion());
    addressFieldsMapping.insert(QContactAddress::FieldLocality, QContactJsonDbStr::addressFieldLocality());
    addressFieldsMapping.insert(QContactAddress::FieldPostcode, QContactJsonDbStr::addressFieldPostcode());
    addressFieldsMapping.insert(QContactAddress::FieldPostOfficeBox, QContactJsonDbStr::addressFieldPostOfficeBox());
    addressFieldsMapping.insert(QContactAddress::FieldStreet, QContactJsonDbStr::addressFieldStreet());
    organizationFieldsMapping.insert(QContactOrganization::FieldName, QContactJsonDbStr::organizationFieldName());
    organizationFieldsMapping.insert(QContactOrganization::FieldDepartment, QContactJsonDbStr::organizationFieldDepartment());
    organizationFieldsMapping.insert(QContactOrganization::FieldTitle, QContactJsonDbStr::organizationFieldTitle());
    organizationFieldsMapping.insert(QContactOrganization::FieldRole, QContactJsonDbStr::organizationFieldRole());
    organizationFieldsMapping.insert(QContactOrganization::FieldAssistantName, QContactJsonDbStr::organizationFieldAssistantName());
    organizationFieldsMapping.insert(QContactOrganization::FieldLogoUrl, QContactJsonDbStr::organizationFieldLogoUrl());
    organizationFieldsMapping.insert(QContactOrganization::FieldStartDate, QContactJsonDbStr::organizationFieldStartDate());
    organizationFieldsMapping.insert(QContactOrganization::FieldEndDate, QContactJsonDbStr::organizationFieldEndDate());
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeMobile, QContactJsonDbStr::subTypeCell());
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeFax, QContactJsonDbStr::subTypeFax());
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeVideo, QContactJsonDbStr::subTypeVideo());
    phoneNumbersSubtypesMapping.insert(QContactPhoneNumber::SubTypeLandline, QContactJsonDbStr::subTypeLandline());
    genderValuesMapping.insert(QContactGender::GenderMale, QContactJsonDbStr::genderMale());
    genderValuesMapping.insert(QContactGender::GenderFemale, QContactJsonDbStr::genderFemale());
    genderValuesMapping.insert(QContactGender::GenderUnspecified, QContactJsonDbStr::genderOther());
    contextsToJsonMapping.insert(QContactDetail::ContextHome, QContactJsonDbStr::contextHome());
    contextsToJsonMapping.insert(QContactDetail::ContextWork, QContactJsonDbStr::contextWork());
    contextsToJsonMapping.insert(QContactDetail::ContextOther, QContactJsonDbStr::contextOther());
    //TODO: FINISH THE MAPPING(S)
    //MISSING DETAILS / FIELDS (TO BE ADDED ALSO TO PARSING LOGIC):
    // - QContactTimestamp
    // - QContactOnlineAccount
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

QString QContactJsonDbConverter::jsonDbNotificationObjectToContactType(const QVariantMap &object) const
{
    return object.value(QContactJsonDbStr::type()).toString();
}

QContactId QContactJsonDbConverter::jsonDbNotificationObjectToContactId(const QVariantMap &object) const
{
    QString jsonUuid = object.value(QContactJsonDbStr::uuid()).toString();
    if (jsonUuid.isEmpty())
        return QContactId();
    else
        return QContactId(new QContactJsonDbId(jsonUuid));
}

void QContactJsonDbConverter::jsonDbVersionToContactVersion(const QString &jsonDbVersion, QContactVersion *contactVersion) const
{
    QStringList jsonDbVersions = jsonDbVersion.split(QLatin1Char('-'));
    if (jsonDbVersions.size() != 2)
        return;
    int sequenceNumber = jsonDbVersions.at(0).toInt();
    if (sequenceNumber > 0 && jsonDbVersions.at(1).length() == jsonDbVersionLength) {
        contactVersion->setSequenceNumber(sequenceNumber);
        contactVersion->setExtendedVersion(jsonDbVersions.at(1).toLatin1());
    }
}

void QContactJsonDbConverter::contactVersionToJsonDbVersion(const QContactVersion &contactVersion, QString *jsonDbVersion) const
{
    int sequenceNumber = contactVersion.sequenceNumber();
    QByteArray extendedVersion = contactVersion.extendedVersion();
    if (sequenceNumber > 0 && extendedVersion.length() == jsonDbVersionLength) {
        *jsonDbVersion = QString::number(sequenceNumber) + QLatin1String("-") + QString::fromLatin1(extendedVersion.constData());
    }
}

QTCONTACTS_END_NAMESPACE
