/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#include "qversitcontactexporter.h"
#include "qversitcontactexporter_p.h"

#include <QtCore/qdebug.h>
#include <QtCore/qurl.h>

#include <QtContacts/qcontactdetail.h>

#include "qversitcontactimporter_p.h"
#include "qversitcontactsdefs_p.h"
#include "qversitutils_p.h"
#include "qversitcontacthandler.h"
#include "qversitcontactpluginloader_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

/*!
 * Constructor.
 */
QVersitContactExporterPrivate::QVersitContactExporterPrivate(const QStringList& profiles) :
    mDetailHandler(NULL),
    mDetailHandler2(NULL),
    mDetailHandlerVersion(0),
    mDefaultResourceHandler(new QVersitDefaultResourceHandler),
    mResourceHandler(mDefaultResourceHandler)
{
    // Detail mappings
    int versitPropertyCount =
        sizeof(versitContactDetailMappings)/sizeof(VersitContactDetailMapping);
    // Put them in in reverse order so the entries at the top of the list take precedence
    for (int i = versitPropertyCount-1; i >= 0; i--) {
        mPropertyMappings.insert(
                versitContactDetailMappings[i].detailType,
                    QPair<int, QString>(
                        versitContactDetailMappings[i].detailField,
                        QLatin1String(versitContactDetailMappings[i].versitPropertyName)));
    }

    // Contexts mappings
    int contextCount = sizeof(versitContextMappings)/sizeof(VersitContextMapping);
    for (int i=0; i < contextCount; i++) {
        mContextMappings.insert(
                versitContextMappings[i].contactContext,
                QLatin1String(versitContextMappings[i].versitString));
    }

    // Subtypes mappings
    int subTypeCount = sizeof(versitSubTypeMappings)/sizeof(VersitSubTypeMapping);
    for (int i=0; i < subTypeCount; i++) {
        mSubTypeMappings.insert(
                    QPair<QContactDetail::DetailType, int>(
                        versitSubTypeMappings[i].detailType, versitSubTypeMappings[i].contactSubType),
                        QLatin1String(versitSubTypeMappings[i].versitString));
    }
    mPluginDetailHandlers = QVersitContactPluginLoader::instance()->createContactHandlers(profiles);
}

/*!
 * Destructor.
 */
QVersitContactExporterPrivate::~QVersitContactExporterPrivate()
{
    delete mDefaultResourceHandler;
    foreach (QVersitContactHandler* pluginHandler, mPluginDetailHandlers) {
        delete pluginHandler;
    }
}


/*!
 * Export QT Contact into Versit Document.
 */
void QVersitContactExporterPrivate::exportContact(
    const QContact& contact,
    QVersitDocument& document)
{
    QList<QContactDetail> allDetails = contact.details();
    foreach (const QContactDetail& detail, allDetails) {
        if (mDetailHandler
            && mDetailHandler->preProcessDetail(contact, detail, &document))
            continue;

        QList<QVersitProperty> removedProperties;
        QList<QVersitProperty> generatedProperties;
        QSet<int> processedFields;

        switch (detail.type()) {
        case QContactDetail::TypeAddress:
            encodeAddress(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeAnniversary:
            encodeAnniversary(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeAvatar:
            encodeAvatar(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeBirthday:
            encodeBirthDay(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeDisplayLabel:
            encodeDisplayLabel(detail, document, &removedProperties, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeEmailAddress:
            encodeEmail(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeExtendedDetail:
            encodeExtendedDetail(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeFamily:
            encodeFamily(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeFavorite:
            encodeFavorite(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeGender:
            encodeGender(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeGeoLocation:
            encodeGeoLocation(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeGuid:
            encodeUid(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeName:
            encodeName(detail, document, &removedProperties, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeNickname:
            encodeNickname(detail, document, &removedProperties, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeNote:
            encodeNote(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeOnlineAccount:
            encodeOnlineAccount(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeOrganization:
            encodeOrganization(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypePhoneNumber:
            encodePhoneNumber(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeRingtone:
            encodeRingtone(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeTag:
            encodeTag(detail, document, &removedProperties, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeTimestamp:
            encodeRev(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeUrl:
            encodeUrl(detail, &generatedProperties, &processedFields);
            break;
        case QContactDetail::TypeVersion:
            encodeVersion(detail, &generatedProperties, &processedFields);
            break;
        default:
            break;
        }

        // run plugin handlers
        foreach (QVersitContactExporterDetailHandlerV2* handler, mPluginDetailHandlers) {
            handler->detailProcessed(contact, detail, document,
                                     &processedFields, &removedProperties, &generatedProperties);
        }
        // run the v2 handler, if set
        if (mDetailHandler2 && mDetailHandlerVersion > 1) {
            mDetailHandler2->detailProcessed(contact, detail, document,
                                             &processedFields, &removedProperties, &generatedProperties);
        }

        foreach(const QVersitProperty& property, removedProperties) {
            document.removeProperty(property);
        }
        foreach(const QVersitProperty& property, generatedProperties) {
            document.addProperty(property);
        }

        if (mDetailHandler && mDetailHandlerVersion == 1) {
            mDetailHandler->postProcessDetail(contact, detail, !processedFields.isEmpty(), &document);
        }
    }

    // run plugin handlers
    foreach (QVersitContactExporterDetailHandlerV2* handler, mPluginDetailHandlers) {
        handler->contactProcessed(contact, &document);
    }
    // run the v2 handler, if set
    if (mDetailHandler2 && mDetailHandlerVersion > 1) {
        mDetailHandler2->contactProcessed(contact, &document);
    }

    ensureDocumentContainsName(&document);
    return;
}

/*!
 * Adds to \a document an empty "N" property if it doesn't already have one.
 */
void QVersitContactExporterPrivate::ensureDocumentContainsName(QVersitDocument* document)
{
    bool containsN = false;
    foreach (const QVersitProperty& property, document->properties()) {
        const QString& name = property.name();
        if (name == QStringLiteral("N")) {
            containsN = true;
        }
    }

    if (!containsN) {
        QVersitProperty nProperty;
        nProperty.setValueType(QVersitProperty::CompoundType);
        nProperty.setName(QStringLiteral("N"));
        nProperty.setValue(QStringList() << QString() << QString()
                           << QString() << QString() << QString());
        document->addProperty(nProperty);
    }
}

/*!
 * Encode Contact Name Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeName(
    const QContactDetail& detail,
    const QVersitDocument& document,
    QList<QVersitProperty>* removedProperties,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    Q_UNUSED(document);
    Q_UNUSED(removedProperties);
    const QContactName &contactName = static_cast<const QContactName &>(detail);
    if (!contactName.lastName().isEmpty()
        || !contactName.firstName().isEmpty()
        || !contactName.middleName().isEmpty()
        || !contactName.prefix().isEmpty()
        || !contactName.suffix().isEmpty()) {
        QVersitProperty property;
        property.setName(mPropertyMappings.value(detail.type()).second);
        property.setValue(QStringList()
                          << contactName.lastName()
                          << contactName.firstName()
                          << contactName.middleName()
                          << contactName.prefix()
                          << contactName.suffix());
        property.setValueType(QVersitProperty::CompoundType);
        *generatedProperties << property;
    }

    *processedFields << QContactName::FieldLastName
                     << QContactName::FieldFirstName
                     << QContactName::FieldMiddleName
                     << QContactName::FieldPrefix
                     << QContactName::FieldSuffix;
}

/*!
 * Encode Phone Number Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodePhoneNumber(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactPhoneNumber &phoneNumber = static_cast<const QContactPhoneNumber &>(detail);
    QList<int> subTypes = phoneNumber.subTypes();
    QList<int> phoneNumberContextInt;

    for (int i=0; i<phoneNumber.contexts().count(); i++)
         phoneNumberContextInt << phoneNumber.contexts().at(i);

    QVersitProperty property;
    if (subTypes.contains(QContactPhoneNumber::SubTypeAssistant))
        property.setName(QStringLiteral("X-ASSISTANT-TEL"));
    else
        property.setName(QStringLiteral("TEL"));
    encodeParameters(property, detail.type(), phoneNumberContextInt, subTypes);
    property.setValue(phoneNumber.number());
    *generatedProperties << property;
    *processedFields << QContactPhoneNumber::FieldContext
                      << QContactPhoneNumber::FieldSubTypes
                      << QContactPhoneNumber::FieldNumber;
}

/*!
 * Encode Email Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeEmail(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactEmailAddress &emailAddress = static_cast<const QContactEmailAddress &>(detail);
    QList<int> emailAddressContextInt;

    for (int i=0; i<emailAddress.contexts().count(); i++)
         emailAddressContextInt << emailAddress.contexts().at(i);

    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    encodeParameters(property, detail.type(), emailAddressContextInt);
    property.setValue(emailAddress.emailAddress());
    *generatedProperties << property;
    *processedFields << QContactEmailAddress::FieldContext
                      << QContactEmailAddress::FieldEmailAddress;
}

/*!
 * Encode Address Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeAddress(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactAddress &address = static_cast<const QContactAddress &>(detail);
    QList<int> addressContextInt;

    for (int i=0; i<address.contexts().count(); i++)
         addressContextInt << address.contexts().at(i);

    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    encodeParameters(property, detail.type(), addressContextInt, address.subTypes());
    property.setValue(QStringList()
                      << address.postOfficeBox()
                      << QString() // Leave out the extended address field
                      << address.street()
                      << address.locality()
                      << address.region()
                      << address.postcode()
                      << address.country());
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QContactAddress::FieldContext
                      << QContactAddress::FieldSubTypes
                      << QContactAddress::FieldPostOfficeBox
                      << QContactAddress::FieldStreet
                      << QContactAddress::FieldLocality
                      << QContactAddress::FieldRegion
                      << QContactAddress::FieldPostcode
                      << QContactAddress::FieldCountry;
}

/*!
 * Encode URL Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeUrl(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactUrl &contactUrl = static_cast<const QContactUrl &>(detail);
    QList<int> contactUrlContextInt;

    for (int i=0; i<contactUrl.contexts().count(); i++)
         contactUrlContextInt << contactUrl.contexts().at(i);

    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    encodeParameters(property, detail.type(), contactUrlContextInt);
    // The vCard specifications do not define any TYPEs for URL property.
    // No need to try to convert the subtypes to TYPEs.
    property.setValue(contactUrl.url());
    *generatedProperties << property;
    *processedFields << QContactUrl::FieldContext
                      << QContactUrl::FieldUrl;
}

/*!
 * Encode Uid Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeUid(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactGuid &uid = static_cast<const QContactGuid &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    property.setValue(uid.guid());
    *generatedProperties << property;
    *processedFields << QContactGuid::FieldGuid;
}

/*!
 * Encode REV Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeRev(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactTimestamp &rev = static_cast<const QContactTimestamp &>(detail);
    QString value;
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    if ( rev.lastModified().toString(Qt::ISODate).size() ) {
        if ( rev.lastModified().timeSpec() == Qt::UTC ) {
            value = rev.lastModified().toString(Qt::ISODate);
            if( !value.endsWith(QLatin1Char('Z'), Qt::CaseInsensitive) ) {
                value += QLatin1Char('Z');
            }
        }
        else {
            value = rev.lastModified().toString(Qt::ISODate);
        }
        property.setValue(value);
        *generatedProperties << property;
        *processedFields << QContactTimestamp::FieldModificationTimestamp;
    } else if ( rev.created().toString(Qt::ISODate).size()) {
        if ( rev.created().timeSpec() == Qt::UTC ) {
            value = rev.created().toString(Qt::ISODate);
            if( !value.endsWith(QLatin1Char('Z'), Qt::CaseInsensitive) ) {
                value += QLatin1Char('Z');
            }
        }
        else {
            value = rev.created().toString(Qt::ISODate);
        }
        property.setValue(value);
        *generatedProperties << property;
        *processedFields << QContactTimestamp::FieldCreationTimestamp;
    }
}

/*!
 * Encode Contact Version Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeVersion(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactVersion &version = static_cast<const QContactVersion &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    QStringList values(QString::number(version.sequenceNumber()));
    values.append(QString::fromLocal8Bit(version.extendedVersion()));
    property.setValue(values);
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QContactVersion::FieldSequenceNumber
                      << QContactVersion::FieldExtendedVersion;
}

/*!
 * Encode BirthDay Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeBirthDay(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactBirthday &bday = static_cast<const QContactBirthday &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    QVariant variant = bday.value(QContactBirthday::FieldBirthday);
    QString value;
    if (variant.type() == QVariant::Date) {
        value = variant.toDate().toString(Qt::ISODate);
    } else if (variant.type() == QVariant::DateTime) {
        value = variant.toDateTime().toString(Qt::ISODate);
    } else {
        return;
    }
    property.setValue(value);
    *generatedProperties << property;
    *processedFields << QContactBirthday::FieldBirthday;
}

/*!
 * Encodes displaylabel property information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeDisplayLabel(
    const QContactDetail &detail,
    const QVersitDocument& document,
    QList<QVersitProperty>* removedProperties,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactDisplayLabel &displaylabelDetail = static_cast<const QContactDisplayLabel &>(detail);
    QVersitProperty property =
        VersitUtils::takeProperty(document, QStringLiteral("FN"), removedProperties);
    property.setName(QStringLiteral("FN"));
    property.setValue(displaylabelDetail.label());
    *generatedProperties << property;
    *processedFields << QContactDisplayLabel::FieldLabel;
}

/*!
 * Encode Comment i.e. Note Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeNote(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactNote &contactNote = static_cast<const QContactNote &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    property.setValue(contactNote.note());
    *generatedProperties << property;
    *processedFields << QContactNote::FieldNote;
}

/*!
 * Encode Geo Prpoperties Field Information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeGeoLocation(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactGeoLocation &geoLocation = static_cast<const QContactGeoLocation &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    property.setValue(QStringList() << QString::number(geoLocation.latitude())
                      << QString::number(geoLocation.longitude()));
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QContactGeoLocation::FieldLongitude
                      << QContactGeoLocation::FieldLatitude;
}

/*!
 * Encode organization properties to the versit document
 */
void QVersitContactExporterPrivate::encodeOrganization(
    const QContactDetail& detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactOrganization &organization = static_cast<const QContactOrganization &>(detail);
    if (organization.title().length() > 0) {
        QVersitProperty property;
        property.setName(QStringLiteral("TITLE"));
        property.setValue(organization.title());
        *generatedProperties << property;
        *processedFields << QContactOrganization::FieldTitle;
    }
    if (organization.name().length() > 0 || organization.department().size() > 0) {
        QVersitProperty property;
        property.setName(QStringLiteral("ORG"));
        QStringList values(organization.name());
        values.append(organization.department());
        property.setValue(values);
        property.setValueType(QVersitProperty::CompoundType);
        *generatedProperties << property;
        *processedFields << QContactOrganization::FieldName
                          << QContactOrganization::FieldDepartment;
    }
    if (organization.logoUrl().isValid()) {
        QVersitProperty property;
        if (encodeContentFromFile(organization.logoUrl().toString(), property)) {
            property.setName(QStringLiteral("LOGO"));
            *generatedProperties << property;
            *processedFields << QContactOrganization::FieldLogoUrl;
        }
    }
    if (organization.assistantName().length() > 0) {
        QVersitProperty property;
        property.setName(QStringLiteral("X-ASSISTANT"));
        property.setValue(organization.assistantName());
        *generatedProperties << property;
        *processedFields << QContactOrganization::FieldAssistantName;
    }

    if (organization.role().length() > 0) {
        QVersitProperty property;
        property.setName(QStringLiteral("ROLE"));
        property.setValue(organization.role());
        *generatedProperties << property;
        *processedFields << QContactOrganization::FieldRole;
    }
}

void QVersitContactExporterPrivate::encodeRingtone(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactRingtone &ringtone = static_cast<const QContactRingtone &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    QUrl audioUrl(ringtone.audioRingtoneUrl());
    // Url value
    if (!audioUrl.scheme().isEmpty() && !audioUrl.host().isEmpty() &&
            audioUrl.scheme() != QStringLiteral("file")) {
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("URL"));
        property.setValue(audioUrl.toString());
        *generatedProperties << property;
        *processedFields << QContactRingtone::FieldAudioRingtoneUrl;
    // Local value
    } else if (encodeContentFromFile(ringtone.audioRingtoneUrl().toLocalFile(), property)) {
        *generatedProperties << property;
        *processedFields << QContactRingtone::FieldAudioRingtoneUrl;
    }
}

/*!
 * Encode avatar URIs into the Versit Document
 */
void QVersitContactExporterPrivate::encodeAvatar(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    QVersitProperty property;
    property.setName(QStringLiteral("PHOTO"));
    const QContactAvatar &contactAvatar = static_cast<const QContactAvatar &>(detail);
    QUrl imageUrl(contactAvatar.imageUrl());
    // XXX: fix up this mess: checking the scheme here and in encodeContentFromFile,
    // organisation logo and ringtone are QStrings but avatar is a QUrl
    if (!imageUrl.scheme().isEmpty()
            && !imageUrl.host().isEmpty()
            && imageUrl.scheme() != QStringLiteral("file")) {
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("URL"));
        property.setValue(imageUrl.toString());
        *generatedProperties << property;
        *processedFields << QContactAvatar::FieldImageUrl;
    } else {
        if (encodeContentFromFile(contactAvatar.imageUrl().toLocalFile(), property)) {
            *generatedProperties << property;
            *processedFields << QContactAvatar::FieldImageUrl;
        }
    }
}

/*!
 * Encode gender property information into Versit Document
 */
void QVersitContactExporterPrivate::encodeGender(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactGender &gender = static_cast<const QContactGender &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    switch (gender.gender()) {
    case QContactGender::GenderMale:
        property.setValue(QStringLiteral("Male"));
        break;
    case QContactGender::GenderFemale:
        property.setValue(QStringLiteral("Female"));
        break;
    case QContactGender::GenderUnspecified:
        property.setValue(QStringLiteral("Unspecified"));
        break;
    default:
        // May only happen if new gender values are added to QContactGender
        // without adding them support above.
        qWarning() << "Trying to encode unknown gender value.";
        return;
    }
    *generatedProperties << property;
    *processedFields << QContactGender::FieldGender;
}

/*!
 * Encodes nickname property information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeNickname(
    const QContactDetail &detail,
    const QVersitDocument& document,
    QList<QVersitProperty>* removedProperties,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactNickname &nicknameDetail = static_cast<const QContactNickname &>(detail);
    QVersitProperty property =
        VersitUtils::takeProperty(document, QStringLiteral("X-NICKNAME"), removedProperties);
    property.setName(QStringLiteral("X-NICKNAME"));
    QStringList value(property.variantValue().toStringList());
    value.append(nicknameDetail.nickname());
    property.setValue(value);
    property.setValueType(QVersitProperty::ListType);
    *generatedProperties << property;
    *processedFields << QContactNickname::FieldNickname;
}

/*!
 * Encodes a contact tag into the Versit Document
 */
void QVersitContactExporterPrivate::encodeTag(
    const QContactDetail &detail,
    const QVersitDocument& document,
    QList<QVersitProperty>* removedProperties,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactTag &tagDetail = static_cast<const QContactTag &>(detail);
    QVersitProperty property =
        VersitUtils::takeProperty(document, QStringLiteral("CATEGORIES"), removedProperties);
    property.setName(QStringLiteral("CATEGORIES"));
    QStringList value(property.variantValue().toStringList());
    value.append(tagDetail.tag());
    property.setValue(value);
    property.setValueType(QVersitProperty::ListType);
    *generatedProperties << property;
    *processedFields << QContactTag::FieldTag;
}

/*!
 * Encode anniversary information into Versit Document
 */
void QVersitContactExporterPrivate::encodeAnniversary(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactAnniversary &anniversary = static_cast<const QContactAnniversary &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    property.setValue(anniversary.originalDate().toString(Qt::ISODate));
    *generatedProperties << property;
    *processedFields << QContactAnniversary::FieldOriginalDate;
}

/*!
 * Encode online account information into the Versit Document
 */
void QVersitContactExporterPrivate::encodeOnlineAccount(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactOnlineAccount &onlineAccount = static_cast<const QContactOnlineAccount &>(detail);
    QList<int> subTypes = onlineAccount.subTypes();

    QContactOnlineAccount::Protocol protocol = onlineAccount.protocol();

    QString propertyName;

    if (protocol == QContactOnlineAccount::ProtocolJabber) {
        propertyName = QStringLiteral("X-JABBER");
    } else if (protocol == QContactOnlineAccount::ProtocolAim) {
        propertyName = QStringLiteral("X-AIM");
    } else if (protocol == QContactOnlineAccount::ProtocolIcq) {
        propertyName = QStringLiteral("X-ICQ");
    } else if (protocol == QContactOnlineAccount::ProtocolMsn) {
        propertyName = QStringLiteral("X-MSN");
    } else if (protocol == QContactOnlineAccount::ProtocolQq) {
        propertyName = QStringLiteral("X-QQ");
    } else if (protocol == QContactOnlineAccount::ProtocolYahoo) {
        propertyName = QStringLiteral("X-YAHOO");
    } else if (protocol == QContactOnlineAccount::ProtocolSkype) {
        propertyName = QStringLiteral("X-SKYPE");
    } else if (subTypes.contains(QContactOnlineAccount::SubTypeSip) ||
               subTypes.contains(QContactOnlineAccount::SubTypeSipVoip) ||
               subTypes.contains(QContactOnlineAccount::SubTypeVideoShare)) {
        propertyName = QStringLiteral("X-SIP");
    } else if (subTypes.contains(QContactOnlineAccount::SubTypeImpp)) {
        propertyName = QStringLiteral("X-IMPP");
    }

    if (!propertyName.isEmpty()) {
        QList<int> onlineAccountContextInt;

        for (int i=0; i<onlineAccount.contexts().count(); i++)
             onlineAccountContextInt << onlineAccount.contexts().at(i);

        QVersitProperty property;
        encodeParameters(property, detail.type(), onlineAccountContextInt, subTypes);
        property.setName(propertyName);
        property.setValue(onlineAccount.accountUri());
        *generatedProperties << property;
        *processedFields << QContactOnlineAccount::FieldSubTypes
                         << QContactOnlineAccount::FieldAccountUri;
    }
}

/*!
 * Encode family versit property if its supported in Versit Document
 */
void QVersitContactExporterPrivate::encodeFamily(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactFamily &family = static_cast<const QContactFamily &>(detail);

    if (family.spouse().size()) {
        QVersitProperty property;
        property.setName(QStringLiteral("X-SPOUSE"));
        property.setValue(family.spouse());
        *generatedProperties << property;
        *processedFields << QContactFamily::FieldSpouse;
    }

    if (family.children().size()) {
        QVersitProperty property;
        property.setName(QStringLiteral("X-CHILDREN"));
        property.setValue(family.children());
        property.setValueType(QVersitProperty::ListType);
        *generatedProperties << property;
        *processedFields << QContactFamily::FieldChildren;
    }
}

/*!
 * Encode favorite versit property if its supported in Versit Document
 */
void QVersitContactExporterPrivate::encodeFavorite(
        const QContactDetail &detail,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QContactFavorite &favorite = static_cast<const QContactFavorite &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(detail.type()).second);
    QString isFavorite = favorite.isFavorite() ? QStringLiteral("true") : QStringLiteral("false");
    property.setValue(QStringList() << isFavorite
                      << QString::number(favorite.index()));
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QContactFavorite::FieldFavorite
                     << QContactFavorite::FieldIndex;
}

/*!
 * Encode extended detail into the Versit Document
 */
void QVersitContactExporterPrivate::encodeExtendedDetail(
    const QContactDetail &detail,
    QList<QVersitProperty>* generatedProperties,
    QSet<int>* processedFields)
{
    const QContactExtendedDetail &extendedDetail =
            static_cast<const QContactExtendedDetail &>(detail);
    QVersitProperty property;
    property.setName(mPropertyMappings.value(extendedDetail.type()).second);
    QString dataAsJson;
    if (VersitUtils::convertToJson(extendedDetail.data(), &dataAsJson)) {
        property.setValue(QStringList() << extendedDetail.name() << dataAsJson);
    } else {
        qWarning() << Q_FUNC_INFO << "Failed to export an extended detail";
        return;
    }
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QContactExtendedDetail::FieldName
                     << QContactExtendedDetail::FieldData;
}

/*!
 * Check if \a resourceIdentifier represents a valid remote resource
 */
bool QVersitContactExporterPrivate::isValidRemoteUrl(
    const QString& resourceIdentifier)
{
    QUrl remoteResource(resourceIdentifier);
    if ((!remoteResource.scheme().isEmpty() &&
         !remoteResource.host().isEmpty()) ||
        resourceIdentifier.contains(QStringLiteral("www."), Qt::CaseInsensitive))
        return true;
    return false;
}

/*!
 * Encode parameters to \a property
 */
void QVersitContactExporterPrivate::encodeParameters(
    QVersitProperty& property,
    const QContactDetail::DetailType detailType,
    const QList<int>& contexts,
    const QList<int>& subTypes)
{
    QList<int> contextsList(contexts); // Contexts should be encoded first
    QList<int> subtypesList(subTypes); // Contexts should be encoded first

    while (!contextsList.isEmpty()) {
        int value = contextsList.takeLast();
        QString mappedValue = mContextMappings.value(value);
        if (mappedValue.length() > 0) {
            // QVersitProperty::addParameter inserts into beginning.
            // This is why the last value is taken from the list
            property.insertParameter(QStringLiteral("TYPE"),mappedValue);
        }
    }

    QSet<int> inserted;
    while (!subtypesList.isEmpty()) {
        int value = subtypesList.takeLast();
        if (inserted.contains(value))
            continue;

        inserted.insert(value);
        QString mappedValue = mSubTypeMappings.value(QPair<QContactDetail::DetailType, int>(
                                                          detailType, value));
        if (mappedValue.length() > 0) {
            // QVersitProperty::addParameter inserts into beginning.
            // This is why the last value is taken from the list
            property.insertParameter(QStringLiteral("TYPE"),mappedValue);
        }
    }
}

/*!
 * Encode embedded content from the given \a resourcePath into \a property.
 */
bool QVersitContactExporterPrivate::encodeContentFromFile(const QString& resourcePath,
                                                          QVersitProperty& property)
{
    bool encodeContent = false;
    QVariant value;
    QByteArray imageData;
    QString mimeType;
    if (isValidRemoteUrl( resourcePath )) {
        encodeContent = true;
        value.setValue(resourcePath);
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("uri"));
    } else if (mResourceHandler
               && mResourceHandler->loadResource(resourcePath, &imageData, &mimeType)) {
        value.setValue(imageData);
        if (!mimeType.isEmpty()) {
            // If mimeType is (eg.) "image/jpeg", set type parameter to "JPEG"
            int slashIndex = mimeType.indexOf(QLatin1Char('/'));
            if (slashIndex >= 0)
                property.insertParameter(QStringLiteral("TYPE"),
                                         mimeType.remove(0, slashIndex+1).toUpper());
        }
        encodeContent = true;
    } else {
        // The file doesn't exist. Don't encode the path to a local file.
    }
    property.setValue(value);
    return encodeContent;
}

QT_END_NAMESPACE_VERSIT
