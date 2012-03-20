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

#include "qversitcontactsdefs_p.h"
#include "qversitcontactimporter_p.h"
#include <qversitdocument.h>
#include <qversitproperty.h>
#include "qversitpluginsearch_p.h"

#include <qcontactmanagerengine.h>
#include <qcontact.h>
#include <qcontactdetail.h>
#include <qcontactname.h>
#include <qcontactphonenumber.h>
#include <qcontactaddress.h>
#include <qcontactemailaddress.h>
#include <qcontactorganization.h>
#include <qcontacturl.h>
#include <qcontactguid.h>
#include <qcontacttimestamp.h>
#include <qcontactanniversary.h>
#include <qcontactbirthday.h>
#include <qcontactgender.h>
#include <qcontactnickname.h>
#include <qcontactavatar.h>
#include <qcontactgeolocation.h>
#include <qcontactnote.h>
#include <qcontactonlineaccount.h>
#include <qcontactfamily.h>
#include <qcontactdisplaylabel.h>
#include <qcontactringtone.h>
#include <qversitcontacthandler.h>
#include "qversitcontactpluginloader_p.h"

#include <QHash>
#include <QFile>

QTVERSIT_BEGIN_NAMESPACE

/*!
 * Constructor.
 */
QVersitContactImporterPrivate::QVersitContactImporterPrivate(const QStringList& profiles) :
    mPropertyHandler(NULL),
    mPropertyHandler2(NULL),
    mPropertyHandlerVersion(0),
    mDefaultResourceHandler(new QVersitDefaultResourceHandler),
    mResourceHandler(mDefaultResourceHandler)
{
    // Contact detail mappings
    int versitPropertyCount =
        sizeof(versitContactDetailMappings)/sizeof(VersitContactDetailMapping);
    for (int i=0; i < versitPropertyCount; i++) {
        QString versitPropertyName =
            QLatin1Literal(versitContactDetailMappings[i].versitPropertyName);
        QPair<QContactDetail::DetailType, int> contactDetail;
        contactDetail.first =
            versitContactDetailMappings[i].detailType;
        contactDetail.second =
            versitContactDetailMappings[i].detailField;
        mDetailMappings.insert(versitPropertyName,contactDetail);
    }

    // Context mappings
    int contextCount = sizeof(versitContextMappings)/sizeof(VersitContextMapping);
    for (int i=0; i < contextCount; i++) {
        mContextMappings.insert(
            versitContextMappings[i].contactContext,
            QLatin1String(versitContextMappings[i].versitString));
    }

    // Subtype mappings
    int subTypeCount = sizeof(versitSubTypeMappings)/sizeof(VersitSubTypeMapping);
    for (int i=0; i < subTypeCount; i++) {
        mSubTypeMappings.insert(
            QPair<QContactDetail::DetailType, int>(
                        versitSubTypeMappings[i].detailType,
                        versitSubTypeMappings[i].contactSubType),
            QLatin1String(versitSubTypeMappings[i].versitString));
    }

    mPluginPropertyHandlers = QVersitContactPluginLoader::instance()->createContactHandlers(profiles);
}

/*!
 * Destructor.
 */
QVersitContactImporterPrivate::~QVersitContactImporterPrivate()
{
    delete mDefaultResourceHandler;
    foreach (QVersitContactHandler* pluginHandler, mPluginPropertyHandlers) {
        delete pluginHandler;
    }
}

/*!
 * Generates a QContact from \a versitDocument.
 */
bool QVersitContactImporterPrivate::importContact(
        const QVersitDocument& document, int contactIndex, QContact* contact,
        QVersitContactImporter::Error* error)
{
    if (document.componentType() != QStringLiteral("VCARD")
        && document.type() != QVersitDocument::VCard21Type
        && document.type() != QVersitDocument::VCard30Type) {
        *error = QVersitContactImporter::InvalidDocumentError;
        return false;
    }
    const QList<QVersitProperty> properties = document.properties();
    if (properties.size() == 0) {
        *error = QVersitContactImporter::EmptyDocumentError;
        return false;
    }

    // First, do the properties with PREF set so they appear first in the contact details
    foreach (const QVersitProperty& property, properties) {
        QStringList typeParameters = property.parameters().values(QStringLiteral("TYPE"));
        if (typeParameters.contains(QStringLiteral("PREF"), Qt::CaseInsensitive))
            importProperty(document, property, contactIndex, contact);
    }
    // ... then, do the rest of the properties.
    foreach (const QVersitProperty& property, properties) {
        QStringList typeParameters = property.parameters().values(QStringLiteral("TYPE"));
        if (!typeParameters.contains(QStringLiteral("PREF"), Qt::CaseInsensitive))
            importProperty(document, property, contactIndex, contact);
    }

    contact->setType(QContactType::TypeContact);
    QContactManagerEngine::setContactDisplayLabel(contact, QVersitContactImporterPrivate::synthesizedDisplayLabel(*contact));

    mRestoreHandler.documentProcessed();
    // run plugin handlers
    foreach (QVersitContactImporterPropertyHandlerV2* handler, mPluginPropertyHandlers) {
        handler->documentProcessed(document, contact);
    }
    // run the v2 handler, if set
    if (mPropertyHandler2 && mPropertyHandlerVersion > 1) {
        mPropertyHandler2->documentProcessed(document, contact);
    }

    return true;
}

void QVersitContactImporterPrivate::importProperty(
        const QVersitDocument& document, const QVersitProperty& property, int contactIndex,
        QContact* contact)
{
    if (mPropertyHandler
        && mPropertyHandlerVersion == 1
        && mPropertyHandler->preProcessProperty(document, property, contactIndex, contact))
        return;

    QPair<QContactDetail::DetailType, int> detailDefinition =
        mDetailMappings.value(property.name());
    QContactDetail::DetailType detailType = detailDefinition.first;

    QList<QContactDetail> updatedDetails;

    bool success = false;

    // The following functions create and save the details to the contact
    switch (detailType) {
    case QContactDetail::TypeAddress:
        success = createAddress(property, contact, &updatedDetails); // pass in group
        break;
    case QContactDetail::TypeAnniversary:
        success = createAnniversary(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeAvatar:
        success = createAvatar(property, contact, &updatedDetails);
    case QContactDetail::TypeBirthday:
        success = createBirthday(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeDisplayLabel:
        success = createCustomLabel(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeFamily:
        success = createFamily(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeGender:
        success = createGender(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeGeoLocation:
        success = createGeoLocation(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeName:
        success = createName(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeNickname:
        success = createNicknames(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeOnlineAccount:
        success = createOnlineAccount(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeOrganization:
        success = createOrganization(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypePhoneNumber:
        success = createPhone(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeRingtone:
        success = createRingtone(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeTag:
        success = createTags(property, contact, &updatedDetails);
        break;
    case QContactDetail::TypeTimestamp:
        success = createTimeStamp(property, contact, &updatedDetails);
        break;
    default:
        // Look up mDetailMappings for a simple mapping from property to detail.
        success = createNameValueDetail(property, contact, &updatedDetails);
        break;
    }

    if (mRestoreHandler.propertyProcessed(property, &updatedDetails))
        success = true;

    // run plugin handlers
    foreach (QVersitContactImporterPropertyHandlerV2* handler, mPluginPropertyHandlers) {
        handler->propertyProcessed(document, property, *contact, &success, &updatedDetails);
    }
    // run the v2 handler, if set
    if (mPropertyHandler2 && mPropertyHandlerVersion > 1) {
        mPropertyHandler2->propertyProcessed(document, property, *contact, &success, &updatedDetails);
    }

    foreach (QContactDetail detail, updatedDetails) {
        contact->saveDetail(&detail);
    }

    // run the v1 handler, if set
    if (mPropertyHandler && mPropertyHandlerVersion == 1)
        mPropertyHandler->postProcessProperty(document, property, success, contactIndex, contact);
}
/*!
 * Creates a QContactName from \a property
 */
bool QVersitContactImporterPrivate::createName(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    QContactName name;
    QContactDetail detail = contact->detail(QContactName::Type);
    if (!detail.isEmpty()) {
        // If multiple name properties exist,
        // discard all except the first occurrence
        if (!detail.value(QContactName::FieldFirstName).toString().isEmpty())
            return false;
        else
            name = QContactName(static_cast<QContactName>(detail));
    }

    QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::CompoundType
            || variant.type() != QVariant::StringList)
        return false;
    QStringList values = variant.toStringList();
    QString value(takeFirst(values));
    if (!value.isEmpty())
        name.setLastName(value);
    value = takeFirst(values);
    if (!value.isEmpty())
        name.setFirstName(value);
    value = takeFirst(values);
    if (!value.isEmpty())
        name.setMiddleName(value);
    value = takeFirst(values);
    if (!value.isEmpty())
        name.setPrefix(value);
    value = takeFirst(values);
    if (!value.isEmpty())
        name.setSuffix(value);

    saveDetailWithContext(updatedDetails, name, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactPhoneNumber from \a property
 */
bool QVersitContactImporterPrivate::createPhone(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactPhoneNumber phone;
    QString value(property.value());
    if (value.isEmpty())
        return false;
    phone.setNumber(property.value());
    QStringList subTypes(extractSubTypes(property));
    QList<int> subTypesInt;

    foreach (const QString &stringValue, subTypes) {
        if (!mContextMappings.values().contains(stringValue)) {
            int mappedValue = mSubTypeMappings.key(stringValue).second;
            subTypesInt << mappedValue;
        }
    }

    if (property.name() == QStringLiteral("X-ASSISTANT-TEL"))
        subTypesInt << QContactPhoneNumber::SubTypeAssistant;
    phone.setSubTypes(subTypesInt);
    saveDetailWithContext(updatedDetails, phone, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactAddress from \a property
 */
bool QVersitContactImporterPrivate::createAddress(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactAddress address;

    QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::CompoundType
            || variant.type() != QVariant::StringList)
        return false;
    QStringList addressParts = variant.toStringList();
    QString value(takeFirst(addressParts));
    if (!value.isEmpty())
        address.setPostOfficeBox(value);
    // There is no setter for the Extended Address in QContactAddress:
    if (!addressParts.isEmpty())
        addressParts.removeFirst();
    value = takeFirst(addressParts);
    if (!value.isEmpty())
        address.setStreet(value);
    value = takeFirst(addressParts);
    if (!value.isEmpty())
        address.setLocality(value);
    value = takeFirst(addressParts);
    if (!value.isEmpty())
        address.setRegion(value);
    value = takeFirst(addressParts);
    if (!value.isEmpty())
        address.setPostcode(value);
    value = takeFirst(addressParts);
    if (!value.isEmpty())
        address.setCountry(value);
    QStringList subTypes(extractSubTypes(property));
    QList<int> subTypesInt;

    foreach (const QString &stringValue, subTypes) {
        int mappedValue = mSubTypeMappings.key(stringValue).second;
        subTypesInt << mappedValue;
    }

    address.setSubTypes(subTypesInt);

    saveDetailWithContext(updatedDetails, address, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactOrganization from \a property
 */
bool QVersitContactImporterPrivate::createOrganization(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    QContactOrganization organization;
    QPair<QContactDetail::DetailType, int> detailTypeAndFieldName =
        mDetailMappings.value(property.name());
    int fieldName = detailTypeAndFieldName.second;
    QList<QContactOrganization> organizations = contact->details<QContactOrganization>();
    foreach(const QContactOrganization& current, organizations) {
        if (current.value(fieldName).toString().length() == 0) {
            organization = current;
            break;
        }
    }
    if (fieldName == QContactOrganization::FieldName) {
        setOrganizationNames(organization, property);
    } else if (fieldName == QContactOrganization::FieldTitle) {
        organization.setTitle(property.value());
    } else if (fieldName == QContactOrganization::FieldRole) {
        organization.setRole(property.value());
    } else if (fieldName == QContactOrganization::FieldLogoUrl) {
        setOrganizationLogo(organization, property);
    } else if (fieldName == QContactOrganization::FieldAssistantName) {
        organization.setAssistantName(property.value());
    } else {
        return false;
    }

    saveDetailWithContext(updatedDetails, organization, extractContexts(property));
    return true;
}

/*!
 * Set the organization name and department(s) from \a property.
 */
void QVersitContactImporterPrivate::setOrganizationNames(
    QContactOrganization& organization, const QVersitProperty& property) const
{
    QVariant variant = property.variantValue();
    if (property.valueType() == QVersitProperty::CompoundType
        && variant.type() == QVariant::StringList) {
        QStringList values = variant.toStringList();
        QString name(takeFirst(values));
        if (!name.isEmpty())
            organization.setName(name);
        if (!values.isEmpty())
            organization.setDepartment(values);
    }
}

/*!
 * Set the organization logo from \a property.
 */
void QVersitContactImporterPrivate::setOrganizationLogo(
    QContactOrganization& org, const QVersitProperty& property) const
{
    QString location;
    QByteArray data;
    saveDataFromProperty(property, &location, &data);
    if (!location.isEmpty())
        org.setLogoUrl(QUrl(location));
}

/*!
 * Creates a QContactTimeStamp from \a property
 */
bool QVersitContactImporterPrivate::createTimeStamp(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactTimestamp timeStamp;
    QString value(property.value());
    QDateTime dateTime = parseDateTime(value);
    if (!dateTime.isValid())
        return false;
    timeStamp.setLastModified(dateTime);
    saveDetailWithContext(updatedDetails, timeStamp, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactAnniversary from \a property
 */
bool QVersitContactImporterPrivate::createAnniversary(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactAnniversary anniversary;
    bool justDate = false;
    QDateTime dateTime = parseDateTime(property.value(), &justDate);
    if (!dateTime.isValid())
        return false;
    if (justDate)
        anniversary.setOriginalDate(dateTime.date());
    else
        anniversary.setOriginalDateTime(dateTime);
    saveDetailWithContext(updatedDetails, anniversary, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactBirthday from \a property
 */
bool QVersitContactImporterPrivate::createBirthday(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactBirthday bday;
    bool justDate = false;
    QDateTime dateTime = parseDateTime(property.value(), &justDate);
    if (!dateTime.isValid())
        return false;
    if (justDate)
        bday.setDate(dateTime.date());
    else
        bday.setDateTime(dateTime);
    saveDetailWithContext(updatedDetails, bday, extractContexts(property));
    return true;
}

/*!
 * Creates QContactNicknames from \a property
 */
bool QVersitContactImporterPrivate::createNicknames(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::ListType
            || variant.type() != QVariant::StringList)
        return false;
    QStringList values = variant.toStringList();
    QList<int> contexts = extractContexts(property);

    // We don't want to make duplicates of existing nicknames
    QSet<QString> existingNicknames;
    foreach (const QContactNickname& nickname, contact->details<QContactNickname>()) {
        existingNicknames.insert(nickname.nickname());
    }
    foreach(const QString& value, values) {
        if (!value.isEmpty() && !existingNicknames.contains(value)) {
            QContactNickname nickname;
            nickname.setNickname(value);
            saveDetailWithContext(updatedDetails, nickname, contexts);
            existingNicknames.insert(value);
        }
    }
    return true;
}

/*!
 * Creates QContactTags from \a property
 */
bool QVersitContactImporterPrivate::createTags(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::ListType
            || variant.type() != QVariant::StringList)
        return false;
    QStringList values = variant.toStringList();
    QList<int> contexts = extractContexts(property);

    // We don't want to make duplicates of existing tags
    QSet<QString> existingTags;
    foreach (const QContactTag& tag, contact->details<QContactTag>()) {
        existingTags.insert(tag.tag());
    }
    foreach(const QString& value, values) {
        if (!value.isEmpty() && !existingTags.contains(value)) {
            QContactTag tag;
            tag.setTag(value);
            saveDetailWithContext(updatedDetails, tag, contexts);
            existingTags.insert(value);
        }
    }
    return true;
}

/*!
 * Creates a QContactOnlineAccount from \a property
 */
bool QVersitContactImporterPrivate::createOnlineAccount(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactOnlineAccount onlineAccount;
    QString value(property.value());
    if (value.isEmpty())
        return false;
    onlineAccount.setAccountUri(property.value());
    if (property.name() == QStringLiteral("X-SIP")) {
        QStringList subTypes = extractSubTypes(property);
        QList<int> subTypesInt;

        foreach (const QString &stringValue, subTypes) {
            int mappedValue = mSubTypeMappings.key(stringValue).second;
            subTypesInt << mappedValue;
        }
        if (subTypes.isEmpty())
            subTypesInt.append(QContactOnlineAccount::SubTypeSip);
        onlineAccount.setSubTypes(subTypesInt);
    } else if (property.name() == QStringLiteral("X-IMPP") ||
               property.name() == QStringLiteral("IMPP")) {
        QList<int> subTypeImppList;
        subTypeImppList << QContactOnlineAccount::SubTypeImpp;
        onlineAccount.setSubTypes(subTypeImppList);
    } else if (property.name() == QStringLiteral("X-JABBER")) {
        QList<int> subTypeImppList;
        subTypeImppList << QContactOnlineAccount::SubTypeImpp;
        onlineAccount.setSubTypes(subTypeImppList);
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolJabber);
    } else if (property.name() == QStringLiteral("X-AIM")) {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolAim);
    } else if (property.name() == QStringLiteral("X-ICQ")) {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolIcq);
    } else if (property.name() == QStringLiteral("X-MSN")) {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolMsn);
    } else if (property.name() == QStringLiteral("X-QQ")) {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolQq);
    } else if (property.name() == QStringLiteral("X-YAHOO")) {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolYahoo);
    } else if (property.name() == QStringLiteral("X-SKYPE") ||
            property.name() == QStringLiteral("X-SKYPE-USERNAME")) {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolSkype);
    } else {
        onlineAccount.setValue(QContactOnlineAccount::FieldProtocol,
                               QContactOnlineAccount::ProtocolUnknown);
    }

    saveDetailWithContext(updatedDetails, onlineAccount, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactRingtone from \a property
 */
bool QVersitContactImporterPrivate::createRingtone(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QString location;
    QByteArray data;
    if (saveDataFromProperty(property, &location, &data) && !location.isEmpty()) {
        QContactRingtone ringtone;
        ringtone.setAudioRingtoneUrl(location);
        saveDetailWithContext(updatedDetails, ringtone, extractContexts(property));
        return true;
    }
    return false;
}

/*!
 * Creates a QContactAvatar from \a property
 */
bool QVersitContactImporterPrivate::createAvatar(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QString location;
    QByteArray data;
    bool success = false;

    if (saveDataFromProperty(property, &location, &data) && !location.isEmpty()) {
        QContactAvatar avatar;
        avatar.setImageUrl(location);
        saveDetailWithContext(updatedDetails, avatar, extractContexts(property));
        success = true;
    }
    return success;
}

/*!
 * Creates a QContactGeoLocation from \a property
 */
bool QVersitContactImporterPrivate::createGeoLocation(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QContactGeoLocation geo;
    QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::CompoundType
            || variant.type() != QVariant::StringList)
        return false;
    QStringList values = variant.toStringList();
    bool ok1;
    geo.setLatitude(takeFirst(values).toDouble(&ok1));
    bool ok2;
    geo.setLongitude(takeFirst(values).toDouble(&ok2));

    if (ok1 && ok2) {
        saveDetailWithContext(updatedDetails, geo, extractContexts(property));
        return true;
    } else {
        return false;
    }
}

/*!
 * Creates a QContactFamily from \a property
 */
bool QVersitContactImporterPrivate::createFamily(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    QString val = property.value();
    QContactFamily family = contact->detail<QContactFamily>();
    if (property.name() == QStringLiteral("X-SPOUSE")) {
        if (val.isEmpty())
            return false;
        family.setSpouse(val);
    } else if (property.name() == QStringLiteral("X-CHILDREN")) {
        QVariant variant = property.variantValue();
        if (property.valueType() != QVersitProperty::ListType
                || variant.type() != QVariant::StringList)
            return false;
        QStringList values = variant.toStringList();
        if (values.isEmpty())
            return false;
        family.setChildren(values);
    } else {
        return false;
    }

    saveDetailWithContext(updatedDetails, family, extractContexts(property));
    return true;
}

/*!
 * Creates a QContactGender from \a property
 */
bool QVersitContactImporterPrivate::createGender(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    QString val = property.value();
    QContactGender gender = contact->detail<QContactGender>();
    if (property.name() == QStringLiteral("X-GENDER")) {
        if (val.isEmpty()) {
            return false;
        } else {
            if (val == QStringLiteral("Male")) {
                gender.setGender(QContactGender::GenderMale);
            } else if (val == QStringLiteral("Female")) {
                gender.setGender(QContactGender::GenderFemale);
            } else {
                gender.setGender(QContactGender::GenderUnspecified);
            }
        }
    } else {
        return false;
    }

    saveDetailWithContext(updatedDetails, gender, extractContexts(property));
    return true;
}

/*!
 * Creates a simple name-value contact detail.
 */
bool QVersitContactImporterPrivate::createNameValueDetail(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    QString value(property.value());
    if (value.isEmpty())
        return false;
    QPair<QContactDetail::DetailType, int> nameAndValueType =
        mDetailMappings.value(property.name());
    if (nameAndValueType.first == QContactDetail::TypeUndefined)
        return false;

    QContactDetail detail(nameAndValueType.first);
    detail.setValue(nameAndValueType.second, value);

    saveDetailWithContext(updatedDetails, detail, extractContexts(property));
    return true;
}

/*!
 * Find an existing QContactName and set the CustomLabel field on it
 */
bool QVersitContactImporterPrivate::createCustomLabel(
    const QVersitProperty& property,
    QContact* contact,
    QList<QContactDetail>* updatedDetails)
{
    QString label(property.value());
    if (!label.isEmpty()) {
        QContactName name;
        QContactName existingName = contact->detail<QContactName>();
        if (!existingName.isEmpty()) {
            name = existingName;
        }

        name.setCustomLabel(property.value());

        saveDetailWithContext(updatedDetails, name, extractContexts(property));
        return true;
    } else {
        return false;
    }
}

/*!
 * Extracts the list of contexts from \a types
 */
QList<int> QVersitContactImporterPrivate::extractContexts(
    const QVersitProperty& property) const
{
    QStringList types = property.parameters().values(QStringLiteral("TYPE"));
    QList<int> contexts;
    foreach (const QString& type, types) {
        QString value = type.toUpper();
        if (mContextMappings.values().contains(value))
            contexts << mContextMappings.key(value);
    }
    return contexts;
}

/*!
 * Extracts the list of subtypes from \a property
 */
QStringList QVersitContactImporterPrivate::extractSubTypes(
    const QVersitProperty& property) const
{
    QStringList types = property.parameters().values(QStringLiteral("TYPE"));
    QStringList subTypes;
    foreach (const QString& type, types) {
        QString subType = type.toUpper();
        if (subType.length() > 0)
            subTypes += subType;
    }
    return subTypes;
}

/*!
 * Takes the first value in \a list, or an empty QString is if the list is empty.
 */
QString QVersitContactImporterPrivate::takeFirst(QList<QString>& list) const
{
    return list.empty() ? QString() : list.takeFirst();
}

/*!
 * Parses a date and time from text
 */
QDateTime QVersitContactImporterPrivate::parseDateTime(QString value, bool *justDate) const
{
    bool hasTime = false;
    bool utc = value.endsWith(QLatin1Char('Z'), Qt::CaseInsensitive);
    if (utc)
        value.chop(1); // take away z from end;

    QDateTime dateTime;
    if (value.contains(QLatin1Char('-'))) {
        dateTime = QDateTime::fromString(value,Qt::ISODate);
        hasTime = dateTime.isValid() && value.contains(QLatin1Char('T'));
    } else {
        switch (value.length()) {
        case 8:
            dateTime = QDateTime::fromString(value, QStringLiteral("yyyyMMdd"));
            break;
        case 15:
            dateTime = QDateTime::fromString(value, QStringLiteral("yyyyMMddThhmmss"));
            hasTime = true;
            break;
        // default: return invalid
        }
    }

    if (utc)
        dateTime.setTimeSpec(Qt::UTC);

    if (justDate)
        *justDate = !hasTime && !utc; // UTC implies a time of midnight

    return dateTime;
}

/*!
 * Extracts either a location (URI/filepath) from a \a property, or data (eg. if it was base64
 * encoded).  If the property contains data, an attempt is made to save it and the location of the
 * saved resource is recovered to *\a location.  The data is stored into *\a data.
 */
bool QVersitContactImporterPrivate::saveDataFromProperty(const QVersitProperty &property,
                                                            QString *location,
                                                            QByteArray *data) const
{
    bool found = false;
    const QString valueParam = property.parameters().value(QStringLiteral("VALUE")).toUpper();
    QVariant variant(property.variantValue());
    if (variant.type() == QVariant::String
        || valueParam == QStringLiteral("URL")
        || valueParam == QStringLiteral("URI")) {
        *location = property.value();
        found |= !location->isEmpty();
    } else if (variant.type() == QVariant::ByteArray) {
        *data = variant.toByteArray();
        if (!data->isEmpty()) {
            found = true;
            *location = saveContentToFile(property, *data);
        }
    }
    return found;
}

/*!
 * Writes \a data to a file and returns the filename.  \a property specifies the context in which
 * the data was found.
 */
QString QVersitContactImporterPrivate::saveContentToFile(
    const QVersitProperty& property, const QByteArray& data) const
{
    QString filename;
    bool ok = false;
    if (mResourceHandler)
        ok = mResourceHandler->saveResource(data, property, &filename);
    return ok ? filename : QString();
}

/*!
 * Adds \a detail to the \a updatedDetails list.  Also sets the contexts to \a contexts if it is not
 * empty.
 */
void QVersitContactImporterPrivate::saveDetailWithContext(
        QList<QContactDetail>* updatedDetails,
        QContactDetail detail,
        const QList<int>& contexts)
{
    if (!contexts.isEmpty())
        detail.setContexts(contexts);
    updatedDetails->append(detail);
}

/*! Synthesize the display label from the name of the contact, or, failing that, the nickname of
the contact, or failing that, the organisation of the contact.
Returns the synthesized display label.
 */
QString QVersitContactImporterPrivate::synthesizedDisplayLabel(const QContact& contact)
{
    /* XXX This is copied and modified from QContactManagerEngine.  This should be made a public
       static function in QCME and called here */
    QList<QContactName> allNames = contact.details<QContactName>();

    const QString space(QStringLiteral(" "));

    // synthesize the display label from the name.
    foreach (const QContactName& name, allNames) {
        if (!name.customLabel().isEmpty()) {
            // default behaviour is to allow the user to define a custom display label.
            return name.customLabel();
        }

        QString result;
        if (!name.value(QContactName::FieldPrefix).toString().trimmed().isEmpty()) {
            result += name.value(QContactName::FieldPrefix).toString();
        }
        if (!name.value(QContactName::FieldFirstName).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldFirstName).toString();
        }
        if (!name.value(QContactName::FieldMiddleName).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldMiddleName).toString();
        }
        if (!name.value(QContactName::FieldLastName).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldLastName).toString();
        }
        if (!name.value(QContactName::FieldSuffix).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldSuffix).toString();
        }
        if (!result.isEmpty())
            return result;
    }

    QList<QContactNickname> allNicknames = contact.details<QContactNickname>();
    foreach (const QContactNickname& nickname, allNicknames) {
        if (!nickname.nickname().isEmpty())
            return nickname.nickname();
    }

    /* Well, we had no non empty names. if we have orgs, fall back to those */
    QList<QContactOrganization> allOrgs = contact.details<QContactOrganization>();
    foreach (const QContactOrganization& org, allOrgs) {
        if (!org.name().isEmpty())
            return org.name();
    }

    return QString();
}

QTVERSIT_END_NAMESPACE
