/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

//TESTED_COMPONENT=src/versit

#include "tst_qversitcontactimporter.h"
#include <QtVersit/qversitcontactimporter.h>
#include <QtVersit/private/qversitcontactimporter_p.h>
#include <QtVersit/qversitproperty.h>
#include <QtVersit/qversitdocument.h>
#include <QtContacts/qcontacts.h>
#include <QtTest/QtTest>
#include <QDir>

QTCONTACTS_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

class MyQVersitContactImporterPropertyHandler : public QVersitContactImporterPropertyHandler
{
public:
    MyQVersitContactImporterPropertyHandler()
        : mPreProcess(false)
    {
    }

    bool preProcessProperty(const QVersitDocument& document,
                            const QVersitProperty& property,
                            int contactIndex,
                            QContact* contact)
    {
        Q_UNUSED(document)
        Q_UNUSED(contact)
        Q_UNUSED(contactIndex);
        mPreProcessedProperties.append(property);
        return mPreProcess;
    }

    bool postProcessProperty(const QVersitDocument& document,
                             const QVersitProperty& property,
                             bool alreadyProcessed,
                             int contactIndex,
                             QContact* contact)
    {
        Q_UNUSED(document)
        Q_UNUSED(contact)
        Q_UNUSED(contactIndex)
        if (!alreadyProcessed)
            mUnknownProperties.append(property);
        else
            mPostProcessedProperties.append(property);
        return false;
    }

    void clear()
    {
        mPreProcess = false;
        mPropertyNamesToProcess.clear();
        mUnknownProperties.clear();
        mPreProcessedProperties.clear();
        mPostProcessedProperties.clear();
    }

    // a hook to control what preProcess returns:
    bool mPreProcess;
    QStringList mPropertyNamesToProcess;
    QList<QVersitProperty> mUnknownProperties;
    QList<QVersitProperty> mPreProcessedProperties;
    QList<QVersitProperty> mPostProcessedProperties;
};

class MyQVersitResourceHandler : public QVersitResourceHandler
{
public:
    MyQVersitResourceHandler() : mIndex(0)
    {
    }

    bool saveResource(const QByteArray& contents, const QVersitProperty& property,
                      QString* location)
    {
        Q_UNUSED(property);
        *location = QString::number(mIndex++);
        mObjects.insert(*location, contents);
        return true;
    }

    bool loadResource(const QString &location, QByteArray *contents, QString *mimeType)
    {
        Q_UNUSED(location)
        Q_UNUSED(contents)
        Q_UNUSED(mimeType)
        return false;
    }

    void clear()
    {
        mIndex = 0;
        mObjects.clear();
    }

    int mIndex;
    QMap<QUrl, QByteArray> mObjects;
};

const static QByteArray SAMPLE_GIF(QByteArray::fromBase64(
        "R0lGODlhEgASAIAAAAAAAP///yH5BAEAAAEALAAAAAASABIAAAIdjI+py+0G"
        "wEtxUmlPzRDnzYGfN3KBaKGT6rDmGxQAOw=="));

const static QByteArray NOKIA_GIF(QByteArray::fromBase64(
        "R0lGODdhOAAKAIQRAAAvwQAwwwAwxAAxxwAyygAzywAzzBBHwC9nz0+A0HCf35+/4LDQ78/f79/o"
        "8O/v8PD3/////////////////////////////////////////////////////////////ywAAAAA"
        "OAAKAAAFsCAiik9kRqPJHIfhGixjisuJpqk9Inb0vjaBC0UwFH+uhM+gNBUCw6Wh92vYDAXkCZhF"
        "apMmA3Qajppav6tr8TqUp0DqEIwtqsmRR/Kl2A4RfFKCcnBMbYR+Uw5xg2lAjIlLCS88dyYNLn1S"
        "TYwvk3NmkXSQLgVvXmQuBCcQXlI7Io9MpyWCbKgublgCNgxfP0eOs6dvUgsPyMgvEAUAeCafUWhe"
        "bpI2LQMFenuhZy8hADs="));

void tst_QVersitContactImporter::init()
{
    mImporter = new QVersitContactImporter();
    mResourceHandler = new MyQVersitResourceHandler();
    mImporter->setResourceHandler(mResourceHandler);
    mPropertyHandler = new MyQVersitContactImporterPropertyHandler();
    mImporter->setPropertyHandler(mPropertyHandler);
}

void tst_QVersitContactImporter::cleanup()
{
    QVERIFY(mImporter->propertyHandler() == mPropertyHandler);
    MyQVersitContactImporterPropertyHandler* nullPtr = NULL;
    mImporter->setPropertyHandler(nullPtr);
    delete mPropertyHandler;
    QVERIFY(mImporter->resourceHandler() == mResourceHandler);
    mImporter->setResourceHandler(0);
    delete mResourceHandler;
    delete mImporter;
}

void tst_QVersitContactImporter::testName()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty nameProperty;
    QStringList value;
    value.append(QStringLiteral("John"));//FirstName
    value.append(QStringLiteral("Citizen"));//LastName
    value.append(QStringLiteral("Anonymous"));//GivenName
    value.append(QStringLiteral("Dr"));//PreFix
    value.append(QStringLiteral("MSc"));//Suffix
    nameProperty.setName(QStringLiteral("N"));
    nameProperty.setValue(value);
    nameProperty.setValueType(QVersitProperty::CompoundType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactName name = (QContactName)contact.detail(QContactName::Type);
    QCOMPARE(name.lastName(),value[0]);
    QCOMPARE(name.firstName(),value[1]);
    QCOMPARE(name.middleName(),value[2]);
    QCOMPARE(name.prefix(),value[3]);
    QCOMPARE(name.suffix(),value[4]);

    // Multiple names, first one will be picked and rest will be discarded
    nameProperty = QVersitProperty();
    QStringList anotherValue;
    anotherValue.append(QStringLiteral("FakeJohn"));//FirstName
    anotherValue.append(QStringLiteral("FakeCitizen"));//LastName
    anotherValue.append(QStringLiteral("FakeAnonymous"));//GivenName
    anotherValue.append(QStringLiteral("FakeDr"));//PreFix
    anotherValue.append(QStringLiteral("FakeMSc"));//Suffix
    nameProperty.setName(QStringLiteral("N"));
    nameProperty.setValue(anotherValue);
    nameProperty.setValueType(QVersitProperty::CompoundType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QList<QContactDetail> names = contact.details(QContactName::Type);
    QCOMPARE(names.count(),1);
    // anotherValue should be discarded, so check for value
    name = (QContactName)names[0];
    QCOMPARE(name.lastName(),value[0]);
    QCOMPARE(name.firstName(),value[1]);
    QCOMPARE(name.middleName(),value[2]);
    QCOMPARE(name.prefix(),value[3]);
    QCOMPARE(name.suffix(),value[4]);
}

// check that it doesn't crash if the FN property comes before the N property.
void tst_QVersitContactImporter::testNameWithFormatted()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty fnProperty;
    fnProperty.setName(QStringLiteral("FN"));
    fnProperty.setValue(QStringLiteral("First Last"));
    document.addProperty(fnProperty);
    QVersitProperty nProperty;
    nProperty.setName(QStringLiteral("N"));
    nProperty.setValue(QStringList() << QStringLiteral("Last") << QStringLiteral("First") << QStringLiteral("Middle") << QStringLiteral("Prefix") << QStringLiteral("Suffix"));
    nProperty.setValueType(QVersitProperty::CompoundType);
    document.addProperty(nProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactName name = contact.detail<QContactName>();
    QCOMPARE(name.firstName(), QStringLiteral("First"));
    QCOMPARE(name.lastName(), QStringLiteral("Last"));
    QCOMPARE(name.middleName(), QStringLiteral("Middle"));
    QCOMPARE(name.prefix(), QStringLiteral("Prefix"));
    QCOMPARE(name.suffix(), QStringLiteral("Suffix"));
    }

void tst_QVersitContactImporter::testAddress()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;
    property.setName(QStringLiteral("ADR"));
    property.setValue(QStringList(QString()));
    property.setValueType(QVersitProperty::CompoundType);

    // Empty value for the address
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactAddress address = contact.detail<QContactAddress>();
    QCOMPARE(address.postOfficeBox(),QString());
    QCOMPARE(address.street(),QString());
    QCOMPARE(address.locality(),QString());
    QCOMPARE(address.region(),QString());
    QCOMPARE(address.postcode(),QString());
    QCOMPARE(address.country(),QString());

    // Address with all the fields filled
    property.setValue(QStringList()
                      << QStringLiteral("PO Box")
                      << QStringLiteral("E")
                      << QStringLiteral("My Street")
                      << QStringLiteral("My Town")
                      << QStringLiteral("My State")
                      << QStringLiteral("12345")
                      << QStringLiteral("My Country")
                      );
    property.setValueType(QVersitProperty::CompoundType);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    address = contact.detail<QContactAddress>();
    QCOMPARE(address.postOfficeBox(),QStringLiteral("PO Box"));
    QCOMPARE(address.street(),QStringLiteral("My Street"));
    QCOMPARE(address.locality(),QStringLiteral("My Town"));
    QCOMPARE(address.region(),QStringLiteral("My State"));
    QCOMPARE(address.postcode(),QStringLiteral("12345"));
    QCOMPARE(address.country(),QStringLiteral("My Country"));

    // Address with TYPE parameters converted to contexts and subtypes
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("HOME"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("WORK"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("DOM"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("INTL"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("POSTAL"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("PARCEL"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("X-EXTENSION"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    address = contact.detail<QContactAddress>();
    QList<int> contexts = address.contexts();
    QCOMPARE(contexts.size(), 2);
    QVERIFY(contexts.contains(QContactDetail::ContextHome));
    QVERIFY(contexts.contains(QContactDetail::ContextWork));
    QList<int> subTypes = address.subTypes();
    QCOMPARE(subTypes.size(), 4);
    QVERIFY(subTypes.contains(QContactAddress::SubTypeDomestic));
    QVERIFY(subTypes.contains(QContactAddress::SubTypeInternational));
    QVERIFY(subTypes.contains(QContactAddress::SubTypePostal));
    QVERIFY(subTypes.contains(QContactAddress::SubTypeParcel));
}

void tst_QVersitContactImporter::testAddressWithoutSubTypes()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;
    property.setName(QStringLiteral("ADR"));
    property.setValue(QStringList()
                      << QStringLiteral("PO Box")
                      << QStringLiteral("E")
                      << QStringLiteral("My Street")
                      << QStringLiteral("My Town")
                      << QStringLiteral("My State")
                      << QStringLiteral("12345")
                      << QStringLiteral("My Country")
                      );
    property.setValueType(QVersitProperty::CompoundType);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactAddress address = contact.detail<QContactAddress>();

    QContactAddress otherAddress;
    otherAddress.setPostOfficeBox(QStringLiteral("PO Box"));
    otherAddress.setStreet(QStringLiteral("My Street"));
    otherAddress.setLocality(QStringLiteral("My Town"));
    otherAddress.setRegion(QStringLiteral("My State"));
    otherAddress.setPostcode(QStringLiteral("12345"));
    otherAddress.setCountry(QStringLiteral("My Country"));

    QCOMPARE(address, otherAddress);
}

void tst_QVersitContactImporter::testOrganizationName()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;

    // Empty value for the organization
    property.setName(QStringLiteral("ORG"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactOrganization organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.name(),QString());
    QCOMPARE(organization.department().count(),0);

    // Organization with single value
    property.setValueType(QVersitProperty::CompoundType);
    property.setValue(QStringList(QStringLiteral("Nokia")));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.name(),QStringLiteral("Nokia"));
    QCOMPARE(organization.department().count(),0);

    // Organization with one Organizational Unit
    property.setValue(QStringList() << QStringLiteral("Nokia") << QStringLiteral("R&D"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.name(),QStringLiteral("Nokia"));
    QCOMPARE(organization.department().count(),1);
    QCOMPARE(organization.department().at(0),QStringLiteral("R&D"));

    // Organization with more Organizational Units
    property.setValue(QStringList()
                      << QStringLiteral("Nokia")
                      << QStringLiteral("R&D")
                      << QStringLiteral("Devices")
                      << QStringLiteral("Qt"));
    property.setValueType(QVersitProperty::CompoundType);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.name(),QStringLiteral("Nokia"));
    QCOMPARE(organization.department().count(),3);
    QCOMPARE(organization.department().at(0),QStringLiteral("R&D"));
    QCOMPARE(organization.department().at(1),QStringLiteral("Devices"));
    QCOMPARE(organization.department().at(2),QStringLiteral("Qt"));
}

void tst_QVersitContactImporter::testOrganizationTitle()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;

    // One title
    property.setName(QStringLiteral("TITLE"));
    QString titleValue(QStringLiteral("Developer"));
    property.setValue(titleValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QList<QContactDetail> organizationDetails =
        contact.details(QContactOrganization::Type);
    QCOMPARE(organizationDetails.count(), 1);
    QContactOrganization organization = static_cast<QContactOrganization>(organizationDetails[0]);
    QCOMPARE(organization.title(),titleValue);

    // Two titles -> two QContactOrganizations created
    property.setName(QStringLiteral("TITLE"));
    QString secondTitleValue(QStringLiteral("Hacker"));
    property.setValue(secondTitleValue);
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    organizationDetails = contact.details(QContactOrganization::Type);
    QCOMPARE(organizationDetails.count(), 2);
    QContactOrganization firstOrganization =
        static_cast<QContactOrganization>(organizationDetails[0]);
    QCOMPARE(firstOrganization.title(),titleValue);
    QContactOrganization secondOrganization =
        static_cast<QContactOrganization>(organizationDetails[1]);
    QCOMPARE(secondOrganization.title(),secondTitleValue);

    // Two titles and one organization name -> two QContactOrganizations created
    property.setName(QStringLiteral("ORG"));
    property.setValueType(QVersitProperty::CompoundType);
    property.setValue(QStringList(QStringLiteral("Nokia")));
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    organizationDetails = contact.details(QContactOrganization::Type);
    QCOMPARE(organizationDetails.count(), 2);
    firstOrganization = static_cast<QContactOrganization>(organizationDetails[0]);
    QCOMPARE(firstOrganization.title(),titleValue);
    QCOMPARE(firstOrganization.name(),QStringLiteral("Nokia"));
    secondOrganization = static_cast<QContactOrganization>(organizationDetails[1]);
    QCOMPARE(secondOrganization.title(),secondTitleValue);
    QCOMPARE(secondOrganization.name(),QString());
}

void tst_QVersitContactImporter::testOrganizationAssistant()
{
    QContact contact;
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;
    property.setName(QStringLiteral("X-ASSISTANT"));
    QString assistantValue(QStringLiteral("Jenny"));
    property.setValue(assistantValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QContactOrganization organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.assistantName(), assistantValue);
}

void tst_QVersitContactImporter::testOrganizationLogo()
{
    QContact contact;
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;

    // Embedded LOGO
    property.setName(QStringLiteral("LOGO"));
    QByteArray logo(QByteArray::fromBase64(
            "R0lGODlhEgASAIAAAAAAAP///yH5BAEAAAEALAAAAAASABIAAAIdjI+py+0G"));
    property.setValue(logo);
    property.insertParameter(QStringLiteral("TYPE"),
                          QStringLiteral("GIF"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QContactOrganization organization = contact.detail<QContactOrganization>();
    QByteArray content = mResourceHandler->mObjects.value(organization.logoUrl().toString());
    QCOMPARE(content, logo);

    // LOGO as a URL
    property.setName(QStringLiteral("LOGO"));
    QString logoUrl(QStringLiteral("http://www.organization.org/logo.gif"));
    property.setValue(logoUrl);
    property.insertParameter(QStringLiteral("VALUE"),QStringLiteral("URL"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.logoUrl().toString(),logoUrl);
}

void tst_QVersitContactImporter::testOrganizationRole()
{
    QContact contact;
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;

    // Setting the role is not yet supported by QContactOrganization
    property.setName(QStringLiteral("ROLE"));
    QString roleValue(QStringLiteral("Very important manager and proud of it"));
    property.setValue(roleValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QContactOrganization organization = contact.detail<QContactOrganization>();
    QCOMPARE(organization.role(), roleValue);
}

void tst_QVersitContactImporter::testTel()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;
    property.setName(QStringLiteral("TEL"));
    QString value(QStringLiteral("+35850987654321"));
    property.setValue(value);

    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("VOICE"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("CELL"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("MODEM"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("CAR"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("VIDEO"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("FAX"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("BBS"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("PAGER"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("HOME"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("WORK"));
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("ISDN"));

    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    const QContactPhoneNumber& phone = contact.detail<QContactPhoneNumber>();
    QCOMPARE(phone.number(),QString(value));

    const QList<int> subTypes = phone.subTypes();
    QCOMPARE(subTypes.count(), 9);
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeVoice));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeMobile));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeModem));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeCar));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeVideo));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeFax));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeBulletinBoardSystem));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypePager));
    QVERIFY(subTypes.contains(QContactPhoneNumber::SubTypeIsdn));

    const QList<int> contexts = phone.contexts();
    QCOMPARE(contexts.count(),2);
    QVERIFY(contexts.contains(QContactDetail::ContextWork));
    QVERIFY(contexts.contains(QContactDetail::ContextHome));
}

void tst_QVersitContactImporter::testTelWithoutSubTypes()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;
    property.setName(QStringLiteral("TEL"));
    QString value(QStringLiteral("+35850987654321"));
    property.setValue(value);

    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    const QContactPhoneNumber& phone = contact.detail<QContactPhoneNumber>();

    QContactPhoneNumber otherPhone;
    otherPhone.setNumber(QStringLiteral("+35850987654321"));

    QCOMPARE(otherPhone, phone);
}

void tst_QVersitContactImporter::testEmail()
{
    QVersitProperty property;
    property.setName(QStringLiteral("EMAIL"));
    QString value(QStringLiteral("john.citizen@example.com"));
    property.setValue(value);
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("WORK"));
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactEmailAddress email = contact.detail<QContactEmailAddress>();
    QCOMPARE(email.emailAddress(),value);
    const QList<int> contexts = email.contexts();
    QCOMPARE(contexts.count(),1);
    QVERIFY(contexts.contains(QContactDetail::ContextWork));

    QCOMPARE(mPropertyHandler->mUnknownProperties.size(), 0);
}

void tst_QVersitContactImporter::testUrl()
{
    QVersitProperty property;
    property.setName(QStringLiteral("URL"));
    QString value(QStringLiteral("http://example.com"));
    property.setValue(value);
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("WORK"));
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactUrl url = contact.detail<QContactUrl>();
    QCOMPARE(url.url(),value);
    const QList<int> contexts = url.contexts();
    QCOMPARE(contexts.count(),1);
    QVERIFY(contexts.contains(QContactDetail::ContextWork));
}

void tst_QVersitContactImporter::testUid()
{
    QVersitProperty property;
    property.setName(QStringLiteral("UID"));
    QString value(QStringLiteral("unique identifier"));
    property.setValue(value);
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactGuid uid = contact.detail<QContactGuid>();
    QCOMPARE(uid.guid(),value);
}

void tst_QVersitContactImporter::testTimeStamp()
{
    // Simple date : ISO 8601 extended format
    QVersitProperty property;
    property.setName(QStringLiteral("REV"));
    QString dateValue(QStringLiteral("1981-05-20"));
    property.setValue(dateValue);
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactTimestamp timeStamp = contact.detail<QContactTimestamp>();
    QCOMPARE(timeStamp.lastModified().date().toString(Qt::ISODate),dateValue);

    // Date and Time : ISO 8601 extended format without utc offset
    QString dateAndTimeValue(QStringLiteral("1981-05-20T23:55:55"));
    property.setValue(dateAndTimeValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    timeStamp = contact.detail<QContactTimestamp>();
    QCOMPARE(timeStamp.lastModified().toString(Qt::ISODate),dateAndTimeValue);

    // Date and Time : ISO 8601 extented format with utc offset
    QString utcOffset(QStringLiteral("Z"));
    QString dateAndTimeWithUtcValue = dateAndTimeValue+utcOffset;
    property.setValue(dateAndTimeWithUtcValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    timeStamp = contact.detail<QContactTimestamp>();
    QCOMPARE(timeStamp.lastModified().toString(Qt::ISODate), dateAndTimeWithUtcValue);
    QCOMPARE(timeStamp.lastModified().timeSpec(), Qt::UTC);

    // Date and Time : ISO 8601 in basic format without utc offset
    dateAndTimeValue = QStringLiteral("19810520T235555");
    property.setValue(dateAndTimeValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    timeStamp = contact.detail<QContactTimestamp>();

    QCOMPARE(timeStamp.lastModified().toString(QStringLiteral("yyyyMMddThhmmss")),
             dateAndTimeValue);

    // Date and Time : ISO 8601 in basic format with utc offset
    dateAndTimeValue = QStringLiteral("19810520T235555");
    dateAndTimeWithUtcValue = dateAndTimeValue+utcOffset;
    property.setValue(dateAndTimeWithUtcValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    timeStamp = contact.detail<QContactTimestamp>();
    QCOMPARE(timeStamp.lastModified().toString(QStringLiteral("yyyyMMddThhmmss")),
             dateAndTimeValue);
    QCOMPARE(timeStamp.lastModified().timeSpec(),Qt::UTC);
}

void tst_QVersitContactImporter::testVersion()
{
    QFETCH(QString, sequenceNumber);
    QFETCH(QString, extendedVersion);
    QFETCH(bool, versionCreated);

    QVersitDocument document(QVersitDocument::VCard30Type);
    addVersionPropertyToDocument(sequenceNumber,extendedVersion,document);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    if (!versionCreated) {
        QCOMPARE(contact.details(QContactVersion::Type).size(), 0);
        return;
    }
    QContactVersion version = (QContactVersion)contact.detail(QContactVersion::Type);
    QCOMPARE(version.sequenceNumber(), sequenceNumber.toInt());
    QCOMPARE(QString(version.extendedVersion()), extendedVersion);
}

void tst_QVersitContactImporter::testVersion_data()
{
    QTest::addColumn<QString>("sequenceNumber");
    QTest::addColumn<QString>("extendedVersion");
    QTest::addColumn<bool>("versionCreated");

    {
        QTest::newRow("proper version") << QString("4711") << QString("134f23dbb2") << true;
        QTest::newRow("all empty") << QString("") << QString("") << false;
        QTest::newRow("sequenceNumber empty") << QString("") << QString("134f23dbb3") << false;
        QTest::newRow("sequenceNumber negative") << QString("-2") << QString("134f23dbb4") << true;
        QTest::newRow("sequenceNumber invalid mix") << QString("2withletters") << QString("134f23dbb5") << false;
        QTest::newRow("extendedVersion empty") << QString("4712") << QString("") << true;
        QTest::newRow("extendedVersion negative") << QString("4713") << QString("-1234567") << true;
    }
}

void tst_QVersitContactImporter::testMultipleVersions()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    addVersionPropertyToDocument(QStringLiteral("1"), QStringLiteral("134f23dbb2"), document);
    addVersionPropertyToDocument(QStringLiteral("2"), QStringLiteral("134f23dbb3"), document);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QCOMPARE(contact.details(QContactVersion::Type).size(), 1);
    QContactVersion version = (QContactVersion)contact.detail(QContactVersion::Type);
    QCOMPARE(version.sequenceNumber(), 1);
    QCOMPARE(version.extendedVersion(), QByteArray("134f23dbb2"));
}

void tst_QVersitContactImporter::addVersionPropertyToDocument(QString sequenceNumber, QString extendedVersion, QVersitDocument &document)
{
    QVersitProperty property;
    property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
    QStringList value;
    value.append(sequenceNumber);
    value.append(extendedVersion);
    property.setValue(value);
    property.setValueType(QVersitProperty::CompoundType);
    document.addProperty(property);
}

void tst_QVersitContactImporter::testAnniversary()
{
    // Date : ISO 8601 extended format
    QVersitProperty property;
    property.setName(QStringLiteral("X-ANNIVERSARY"));
    property.setValue(QStringLiteral("1981-05-20"));
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactAnniversary anniversary = contact.detail<QContactAnniversary>();
    QCOMPARE(anniversary.originalDate(), QDate(1981, 5, 20));
    QCOMPARE(anniversary.value(QContactAnniversary::FieldOriginalDate).type(), QVariant::Date);

    // Evolution format
    property.setName(QStringLiteral("X-EVOLUTION-ANNIVERSARY"));
    property.setValue(QStringLiteral("1981-05-20"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    anniversary = contact.detail<QContactAnniversary>();
    QCOMPARE(anniversary.originalDate(), QDate(1981, 5, 20));
    QCOMPARE(anniversary.value(QContactAnniversary::FieldOriginalDate).type(), QVariant::Date);

    // Date : ISO 8601 in basic format
    property.setName(QStringLiteral("X-ANNIVERSARY"));
    property.setValue(QStringLiteral("19810520"));
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    anniversary = contact.detail<QContactAnniversary>();
    QCOMPARE(anniversary.originalDate(), QDate(1981, 5, 20));
    QCOMPARE(anniversary.value(QContactAnniversary::FieldOriginalDate).type(), QVariant::Date);

    // Date time tests
    property.setName(QStringLiteral("X-ANNIVERSARY"));
    property.setValue(QStringLiteral("19810520T000102")); // localtime
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    anniversary = contact.detail<QContactAnniversary>();
    QCOMPARE(anniversary.originalDate(), QDate(1981, 5, 20));
    QCOMPARE(anniversary.originalDateTime(), QDateTime(QDate(1981, 5, 20), QTime(0,1,2), Qt::LocalTime));
    QCOMPARE(anniversary.value(QContactAnniversary::FieldOriginalDate).type(), QVariant::DateTime);

    property.setValue(QStringLiteral("19810520T010203Z")); // utc
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    anniversary = contact.detail<QContactAnniversary>();
    QCOMPARE(anniversary.originalDate(), QDate(1981, 5, 20));
    QCOMPARE(anniversary.originalDateTime(), QDateTime(QDate(1981, 5, 20), QTime(1,2,3), Qt::UTC));
    QCOMPARE(anniversary.value(QContactAnniversary::FieldOriginalDate).type(), QVariant::DateTime);
}

void tst_QVersitContactImporter::testBirthday()
{
    // Date : ISO 8601 extended format
    QVersitProperty property;
    property.setName(QStringLiteral("BDAY"));
    QString dateValue(QStringLiteral("1981-05-20"));
    property.setValue(dateValue);
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactBirthday bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.date().toString(Qt::ISODate), dateValue);
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::Date);

    // Date : ISO 8601 in basic format
    dateValue = QStringLiteral("19810520");
    property.setValue(dateValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.date(), QDate(1981, 5, 20));
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::Date);

    dateValue = QStringLiteral("1981-05-20");
    property.setValue(dateValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.date(), QDate(1981, 5, 20));
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::Date);

    dateValue = QStringLiteral("19810520T100000");
    property.setValue(dateValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.dateTime(), QDateTime(QDate(1981, 5, 20), QTime(10, 0, 0), Qt::LocalTime));
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::DateTime);

    dateValue = QStringLiteral("1981-05-20T10:00:00");
    property.setValue(dateValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.dateTime(), QDateTime(QDate(1981, 5, 20), QTime(10, 0, 0), Qt::LocalTime));
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::DateTime);

    dateValue = QStringLiteral("19810520T100000Z");
    property.setValue(dateValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.dateTime(), QDateTime(QDate(1981, 5, 20), QTime(10, 0, 0), Qt::UTC));
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::DateTime);

    dateValue = QStringLiteral("1981-05-20T10:00:00Z");
    property.setValue(dateValue);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    bday = contact.detail<QContactBirthday>();
    QCOMPARE(bday.dateTime(), QDateTime(QDate(1981, 5, 20), QTime(10, 0, 0), Qt::UTC));
    QCOMPARE(bday.value(QContactBirthday::FieldBirthday).type(), QVariant::DateTime);
}

void tst_QVersitContactImporter::testGender()
{
    // Check empty property.
    QVersitProperty property1;
    property1.setName(QStringLiteral("X-GENDER"));
    QString val1(QStringLiteral(""));
    property1.setValue(val1);
    QVersitDocument document1 = createDocumentWithProperty(property1);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document1));
    QContact contact1 = mImporter->contacts().first();
    QCOMPARE (contact1, QContact());

    // Check valid values.
    QVersitProperty property2;
    property2.setName(QStringLiteral("X-GENDER"));
    QString val2(QStringLiteral("male"));
    property2.setValue(val2);
    QVersitDocument document2 = createDocumentWithProperty(property2);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document2));
    QContact contact2 = mImporter->contacts().first();
    QContactGender  gender2 = contact2.detail<QContactGender >();
    QCOMPARE(gender2.gender(),QContactGender::GenderMale);

    QVersitProperty property3;
    property3.setName(QStringLiteral("x-gender"));
    QString val3(QStringLiteral("FEMALE"));
    property3.setValue(val3);
    QVersitDocument document3 = createDocumentWithProperty(property3);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document3));
    QContact contact3 = mImporter->contacts().first();
    QContactGender gender3 = contact3.detail<QContactGender >();
    QCOMPARE(gender3.gender(),QContactGender::GenderFemale);

    QVersitProperty property4;
    property4.setName(QStringLiteral("x-Gender"));
    QString val4(QStringLiteral("Unspecified"));
    property4.setValue(val4);
    QVersitDocument document4 = createDocumentWithProperty(property4);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document4));
    QContact contact4 = mImporter->contacts().first();
    QContactGender gender4 = contact4.detail<QContactGender >();
    QCOMPARE(gender4.gender(),QContactGender::GenderUnspecified);

    // Check property having an invalid value.
    QVersitProperty property5;
    property5.setName(QStringLiteral("X-GENDER"));
    QString val5(QStringLiteral("Garbage"));
    property5.setValue(val5);
    QVersitDocument document5 = createDocumentWithProperty(property5);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document5));
    QContact contact5 = mImporter->contacts().first();
    QCOMPARE(contact5, QContact());
}

void tst_QVersitContactImporter::testNickname()
{
    // one value
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty nameProperty;
    nameProperty.setName(QStringLiteral("NICKNAME"));
    nameProperty.setValue(QStringList(QStringLiteral("Homie")));
    nameProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactNickname nickName = (QContactNickname)contact.detail(QContactNickname::Type);
    QCOMPARE(nickName.nickname(), QStringLiteral("Homie"));

    // comma separated values should generate multiple nickname fields
    contact.clearDetails();
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    QStringList multiVal;
    multiVal.append(QStringLiteral("Homie"));
    multiVal.append(QStringLiteral("SuperHero"));
    multiVal.append(QStringLiteral("NukeSpecialist"));
    nameProperty.setName(QStringLiteral("NICKNAME"));
    nameProperty.setValue(multiVal);
    nameProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QList<QContactNickname> nickNames = contact.details<QContactNickname>();
    QCOMPARE(nickNames.count(),3);
    nickName = nickNames[0];
    QCOMPARE(nickName.nickname(),QStringLiteral("Homie"));
    nickName = nickNames[1];
    QCOMPARE(nickName.nickname(),QStringLiteral("SuperHero"));
    nickName = nickNames[2];
    QCOMPARE(nickName.nickname(),QStringLiteral("NukeSpecialist"));

    // X-NICKNAME
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    nameProperty = QVersitProperty();
    nameProperty.setName(QStringLiteral("X-NICKNAME"));
    nameProperty.setValue(QStringList(QStringLiteral("Homie")));
    nameProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    nickName = contact.detail<QContactNickname>();
    QCOMPARE(nickName.nickname(),QStringLiteral("Homie"));

    // both X-NICKNAME and NICKNAME
    nameProperty.setName(QStringLiteral("NICKNAME"));
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    nickNames = contact.details<QContactNickname>();
    QCOMPARE(nickNames.size(), 1);
    QCOMPARE(nickNames.first().nickname(), QStringLiteral("Homie"));
}

void tst_QVersitContactImporter::testAvatarUrl()
{
    QVersitProperty property;
    property.setName(QStringLiteral("PHOTO"));
    QString value(QStringLiteral("http://example.com/example.jpg"));
    property.setValue(value);
    property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("URL"));

    QVersitDocument document(QVersitDocument::VCard30Type);
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactAvatar avatar = contact.detail<QContactAvatar>();
    QCOMPARE(avatar.imageUrl(), QUrl(QStringLiteral("http://example.com/example.jpg")));


    // A URL disguised inside a QByteArray.
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    property.clear();
    property.setName(QStringLiteral("PHOTO"));
    property.setValue(QByteArray("http://example.com/example.jpg"));
    property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("URL"));
    property.insertParameter(QStringLiteral("CHARSET"), QStringLiteral("UTF-8"));
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    avatar = contact.detail<QContactAvatar>();
    QCOMPARE(avatar.imageUrl(), QUrl(QStringLiteral("http://example.com/example.jpg")));
}

void tst_QVersitContactImporter::testAvatarInvalid()
{
    // An avatar that's a QVersitDocument?  It shouldn't work.
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;
    property.setName(QStringLiteral("PHOTO"));
    QVersitDocument nestedDocument;
    property.setValue(QVariant::fromValue(nestedDocument));
    property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("URL"));
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QCOMPARE(contact.details(QContactAvatar::Type).size(), 0);

    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    property.clear();
    property.setName(QStringLiteral("PHOTO"));
    property.setValue(QVariant::fromValue(nestedDocument));
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QCOMPARE(contact.details(QContactAvatar::Type).size(), 0);
}

void tst_QVersitContactImporter::testGeo()
{
    // some positive values
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty nameProperty;
    QStringList val;
    val.append(QStringLiteral("45.32")); // Latitude
    val.append(QStringLiteral("18.53"));// Longtitude
    nameProperty.setName(QStringLiteral("GEO"));
    nameProperty.setValue(val);
    nameProperty.setValueType(QVersitProperty::CompoundType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactGeoLocation geo = (QContactGeoLocation)contact.detail(QContactGeoLocation::Type);
    QString str;
    str.setNum(geo.latitude(),'.',2);
    QCOMPARE(str,val[0]);
    str.setNum(geo.longitude(),'.',2);
    QCOMPARE(str,val[1]);

    // some negative values
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    nameProperty = QVersitProperty();
    val.append(QStringLiteral("-45.32")); // Latitude
    val.append(QStringLiteral("18.53"));// Longtitude
    nameProperty.setName(QStringLiteral("GEO"));
    nameProperty.setValue(val);
    nameProperty.setValueType(QVersitProperty::CompoundType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    geo = (QContactGeoLocation)contact.detail(QContactGeoLocation::Type);
    str.setNum(geo.latitude(),'.',2);
    QCOMPARE(str,val[0]);
    str.setNum(geo.longitude(),'.',2);
    QCOMPARE(str,val[1]);
}

void tst_QVersitContactImporter::testNote()
{
    // single line value
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty nameProperty;
    QString val(QStringLiteral("I will not sleep at my work -John"));
    nameProperty.setName(QStringLiteral("NOTE"));
    nameProperty.setValue(val);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactNote note = (QContactNote)contact.detail(QContactNote::Type);
    QCOMPARE(note.note(),val);

    // Multiline value and quoted printable encoding
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    nameProperty = QVersitProperty();
    val = QStringLiteral("My Dad acts like he belongs,=0D=0AHe belongs in the zoo.=0D=0A");
    nameProperty.setName(QStringLiteral("NOTE"));
    nameProperty.setValue(val);
    QMultiHash<QString,QString> params;
    params.insert(QStringLiteral("QUOTED-PRINTABLE"),val);
    nameProperty.setParameters(params);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    note = (QContactNote)contact.detail(QContactNote::Type);
    QCOMPARE(note.note(),val);
}

void tst_QVersitContactImporter::testDisplayLabel()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty fnProperty;
    fnProperty.setName(QStringLiteral("FN"));
    fnProperty.setValue(QStringLiteral("fn"));
    document.addProperty(fnProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    contact = mImporter->contacts().first();
    QContactDisplayLabel displayLabel = contact.detail<QContactDisplayLabel>();
    QCOMPARE(displayLabel.label(), QStringLiteral("fn"));
}

void tst_QVersitContactImporter::testOnlineAccount()
{
    QString accountUri(QStringLiteral("sip:john.citizen@example.com"));
    QVersitDocument document;

    // Plain X-SIP, no TYPE ->
    QVersitProperty property;
    property.setName(QStringLiteral("X-SIP"));
    property.setValue(accountUri);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactOnlineAccount onlineAccount = contact.detail<QContactOnlineAccount>();
    QCOMPARE(onlineAccount.accountUri(),accountUri);
    QList<int> subTypes = onlineAccount.subTypes();
    QCOMPARE(subTypes.count(),1);
    QVERIFY(subTypes.first() == QContactOnlineAccount::SubTypeSip);

    // X-SIP;SWIS
    property = QVersitProperty();
    property.setName(QStringLiteral("X-SIP"));
    property.setValue(accountUri);
    QMultiHash<QString,QString> params;
    params.insert(QStringLiteral("TYPE"),QStringLiteral("SWIS"));
    property.setParameters(params);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    onlineAccount =  contact.detail<QContactOnlineAccount>();
    QCOMPARE(onlineAccount.accountUri(),accountUri);
    subTypes = onlineAccount.subTypes();
    QCOMPARE(subTypes.count(),1);
    QVERIFY(subTypes.first() == QContactOnlineAccount::SubTypeVideoShare);

    // X-SIP;VOIP
    property = QVersitProperty();
    property.setName(QStringLiteral("X-SIP"));
    property.setValue(accountUri);
    params.clear();
    params.insert(QStringLiteral("TYPE"),QStringLiteral("VOIP"));
    property.setParameters(params);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    onlineAccount =  contact.detail<QContactOnlineAccount>();
    QCOMPARE(onlineAccount.accountUri(),accountUri);
    subTypes = onlineAccount.subTypes();
    QCOMPARE(subTypes.count(),1);
    QVERIFY(subTypes.first() == QContactOnlineAccount::SubTypeSipVoip);

    // X-IMPP
    property = QVersitProperty();
    property.setName(QStringLiteral("X-IMPP"));
    property.setValue(accountUri);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    onlineAccount =  contact.detail<QContactOnlineAccount>();
    QCOMPARE(onlineAccount.accountUri(),accountUri);
    subTypes = onlineAccount.subTypes();
    QCOMPARE(subTypes.count(),1);
    QVERIFY(subTypes.first() == QContactOnlineAccount::SubTypeImpp);

    // IMPP
    property = QVersitProperty();
    property.setName(QStringLiteral("IMPP"));
    property.setValue(accountUri);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    onlineAccount =  contact.detail<QContactOnlineAccount>();
    QCOMPARE(onlineAccount.accountUri(),accountUri);
    subTypes = onlineAccount.subTypes();
    QCOMPARE(subTypes.count(),1);
    QVERIFY(subTypes.first() == QContactOnlineAccount::SubTypeImpp);

    // X-JABBER
    property = QVersitProperty();
    property.setName(QStringLiteral("X-JABBER"));
    property.setValue(accountUri);
    document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    onlineAccount =  contact.detail<QContactOnlineAccount>();
    QCOMPARE(onlineAccount.accountUri(),accountUri);
    subTypes = onlineAccount.subTypes();
    QCOMPARE(subTypes.count(),1);
    QVERIFY(subTypes.first() == QContactOnlineAccount::SubTypeImpp);
    QVERIFY(onlineAccount.protocol() == QContactOnlineAccount::ProtocolJabber);

    document = QVersitDocument(QVersitDocument::VCard30Type);
    property.setName("X-AIM");
    property.setValue("a");
    document.addProperty(property);
    property.setName("X-ICQ");
    property.setValue("b");
    document.addProperty(property);
    property.setName("X-MSN");
    property.setValue("c");
    document.addProperty(property);
    property.setName("X-QQ");
    property.setValue("d");
    document.addProperty(property);
    property.setName("X-YAHOO");
    property.setValue("e");
    document.addProperty(property);
    property.setName("X-SKYPE");
    property.setValue("f");
    document.addProperty(property);
    property.setName("X-SKYPE-USERNAME");
    property.setValue("g");
    document.addProperty(property);
    property.setName("X-MS-IMADDRESS");
    property.setValue("h");
    document.addProperty(property);
    property.setName("X-KADDRESSBOOK-X-IMADDRESS");
    property.setValue("i");
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QList<QContactOnlineAccount> onlineAccounts = contact.details<QContactOnlineAccount>();
    QCOMPARE(onlineAccounts.size(), 9);
    QVERIFY(onlineAccounts[0].protocol() == QContactOnlineAccount::ProtocolAim);
    QVERIFY(onlineAccounts[0].accountUri() == "a");
    QVERIFY(onlineAccounts[1].protocol() == QContactOnlineAccount::ProtocolIcq);
    QVERIFY(onlineAccounts[1].accountUri() == "b");
    QVERIFY(onlineAccounts[2].protocol() == QContactOnlineAccount::ProtocolMsn);
    QVERIFY(onlineAccounts[2].accountUri() == "c");
    QVERIFY(onlineAccounts[3].protocol() == QContactOnlineAccount::ProtocolQq);
    QVERIFY(onlineAccounts[3].accountUri() == "d");
    QVERIFY(onlineAccounts[4].protocol() == QContactOnlineAccount::ProtocolYahoo);
    QVERIFY(onlineAccounts[4].accountUri() == "e");
    QVERIFY(onlineAccounts[5].protocol() == QContactOnlineAccount::ProtocolSkype);
    QVERIFY(onlineAccounts[5].accountUri() == "f");
    QVERIFY(onlineAccounts[6].protocol() == QContactOnlineAccount::ProtocolSkype);
    QVERIFY(onlineAccounts[6].accountUri() == "g");
    QVERIFY(onlineAccounts[7].protocol() == QContactOnlineAccount::ProtocolUnknown);
    QVERIFY(onlineAccounts[7].accountUri() == "h");
    QVERIFY(onlineAccounts[8].protocol() == QContactOnlineAccount::ProtocolUnknown);
    QVERIFY(onlineAccounts[8].accountUri() == "i");
}

void tst_QVersitContactImporter::testFamily()
{
    // Interesting : kid but no wife :)
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty nameProperty;
    QString val(QStringLiteral("Jane")); // one is enough
    nameProperty.setName(QStringLiteral("X-CHILDREN"));
    nameProperty.setValue(QStringList(val));
    nameProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactFamily family = (QContactFamily)contact.detail(QContactFamily::Type);
    QStringList children = family.children();
    QCOMPARE(children.count(),1); // ensure no other kids in list
    QCOMPARE(family.spouse(),QString()); // make sure no wife
    QCOMPARE(children[0],val); // ensure it is your kid

    // Critical : wife but no kids , happy hours
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    nameProperty = QVersitProperty();
    nameProperty.setName(QStringLiteral("X-SPOUSE"));
    val = QStringLiteral("Jenny");
    nameProperty.setValue(val);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    family = (QContactFamily)contact.detail(QContactFamily::Type);
    children = family.children();
    QCOMPARE(children.count(),0); // list should be empty as you know
    QCOMPARE(family.spouse(),val); // make sure thats your wife:(

    // Hopeless : couple of kids and wife
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    // Add kids first
    nameProperty = QVersitProperty();
    nameProperty.setName(QStringLiteral("X-CHILDREN"));
    QStringList kidsVal;
    kidsVal.append(QStringLiteral("James"));
    kidsVal.append(QStringLiteral("Jake"));
    kidsVal.append(QStringLiteral("Jane"));
    nameProperty.setValue(kidsVal);
    nameProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(nameProperty);
    // Add wife next
    val = QStringLiteral("Jenny");
    nameProperty = QVersitProperty();
    nameProperty.setName(QStringLiteral("X-SPOUSE"));
    nameProperty.setValue(val);
    document.addProperty(nameProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    family = (QContactFamily)contact.detail(QContactFamily::Type);
    children = family.children();
    QCOMPARE(children.count(),3); // too late , count them now.
    // painfull but ensure they are your kids
    QCOMPARE(children, kidsVal);
    QCOMPARE(family.spouse(),val); // make sure thats your wife:(
}

void tst_QVersitContactImporter::testFavorite()
{
    QFETCH(QString, favoriteValue);
    QFETCH(QString, indexValue);
    QFETCH(bool, favoriteCreated);
    QVersitDocument document(QVersitDocument::VCard30Type);
    addFavoritePropertyToDocument(favoriteValue, indexValue, document);

    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));

    QContact contact = mImporter->contacts().first();
    if (!favoriteCreated) {
        QCOMPARE(contact.details(QContactFavorite::Type).size(), 0);
        return;
    }
    QContactFavorite favorite = (QContactFavorite)contact.detail(QContactFavorite::Type);
    QString actualFavoriteValue = favorite.isFavorite() ? QStringLiteral("true") : QStringLiteral("false");
    QCOMPARE(actualFavoriteValue, favoriteValue);
    QCOMPARE(QString::number(favorite.index()), indexValue);
}

void tst_QVersitContactImporter::testFavorite_data()
{
    QTest::addColumn<QString>("favoriteValue");
    QTest::addColumn<QString>("indexValue");
    QTest::addColumn<bool>("favoriteCreated");

    {
        QTest::newRow("favorite true") << QString("true") << QString("1") << true;
        QTest::newRow("favorite false") << QString("false") << QString("1") << true;
        QTest::newRow("favorite invalid") << QString("invalid") << QString("1") << false;
        QTest::newRow("favorite empty") << QString("") << QString("1") << false;
        QTest::newRow("index negative") << QString("true") << QString("-1") << true;
        QTest::newRow("index multiple digits") << QString("true") << QString("10") << true;
        QTest::newRow("index invalid") << QString("true") << QString("invalid") << false;
        QTest::newRow("index invalid mix") << QString("true") << QString("2letters") << false;
        QTest::newRow("index empty") << QString("true") << QString("") << false;
    }
}

void tst_QVersitContactImporter::testMultipleFavorites()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    addFavoritePropertyToDocument(QStringLiteral("true"), QStringLiteral("1"), document);
    addFavoritePropertyToDocument(QStringLiteral("false"), QStringLiteral("2"), document);

    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));

    QContact contact = mImporter->contacts().first();
    QCOMPARE(contact.details(QContactFavorite::Type).size(), 1);
    QContactFavorite favorite = (QContactFavorite)contact.detail(QContactFavorite::Type);
    QCOMPARE(favorite.isFavorite(), true);
    QCOMPARE(favorite.index(), 1);
}

void tst_QVersitContactImporter::addFavoritePropertyToDocument(QString favorite, QString index, QVersitDocument &document)
{
    QVersitProperty property;
    property.setName(QStringLiteral("X-QTPROJECT-FAVORITE"));
    QStringList value;
    value.append(favorite);
    value.append(index);
    property.setValue(value);
    property.setValueType(QVersitProperty::CompoundType);
    document.addProperty(property);
}

void tst_QVersitContactImporter::testSound()
{
    // Test embedded sound file
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty soundProperty;
    QMultiHash<QString,QString> param;
    param.insert(QStringLiteral("TYPE"),QStringLiteral("WAVE"));
    soundProperty.setName(QStringLiteral("SOUND"));
    QByteArray val("111110000011111");
    soundProperty.setValue(val);
    soundProperty.setParameters(param);
    document.addProperty(soundProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactRingtone ringtone = contact.detail<QContactRingtone>();
    QByteArray content = mResourceHandler->mObjects.value(ringtone.audioRingtoneUrl());
    QCOMPARE(content, val);

    // Test sound file as URL
    document.clear();
    soundProperty.clear();
    soundProperty.setName(QStringLiteral("SOUND"));
    QString soundUrl(QStringLiteral("http://qt.nokia.com/audioringtoneurl"));
    soundProperty.setValue(soundUrl);
    soundProperty.insertParameter(QStringLiteral("VALUE"),QStringLiteral("URL"));
    document = createDocumentWithProperty(soundProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    ringtone = contact.detail<QContactRingtone>();
    QCOMPARE(ringtone.audioRingtoneUrl().toString(), soundUrl);
}

void tst_QVersitContactImporter::testTag()
{
    // one value
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty tagProperty;
    tagProperty.setName(QStringLiteral("CATEGORIES"));
    tagProperty.setValue(QStringList(QStringLiteral("red")));
    tagProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(tagProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactTag tagDetail = contact.detail<QContactTag>();
    QCOMPARE(tagDetail.tag(), QStringLiteral("red"));

    // multiple values
    tagProperty.setName(QStringLiteral("CATEGORIES"));
    tagProperty.setValue(QStringList()
                         << QStringLiteral("red")  // duplicate from previous property should be pruned
                         << QStringLiteral("green")
                         << QStringLiteral("blue")
                         << QStringLiteral("blue"));  // duplicates should be pruned
    tagProperty.setValueType(QVersitProperty::ListType);
    document.addProperty(tagProperty);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QList<QContactTag> tagDetails = contact.details<QContactTag>();
    QCOMPARE(tagDetails.count(), 3);
    QCOMPARE(tagDetails.at(0).tag(), QStringLiteral("red"));
    QCOMPARE(tagDetails.at(1).tag(), QStringLiteral("green"));
    QCOMPARE(tagDetails.at(2).tag(), QStringLiteral("blue"));
}

void tst_QVersitContactImporter::testExtendedDetail()
{
    QFETCH(QString, extendedDetailName);
    QFETCH(QVariant, extendedDetailData);
    QFETCH(QString, extendedDetailDataInProperty);
    QFETCH(bool, extendedDetailCreated);
    QVersitDocument document(QVersitDocument::VCard30Type);
    addExtendedDetailPropertyToDocument(extendedDetailName, extendedDetailDataInProperty, document);

    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));

    QContact contact = mImporter->contacts().first();
    if (!extendedDetailCreated) {
        QCOMPARE(contact.details(QContactExtendedDetail::Type).size(), 0);
        return;
    }
    QContactExtendedDetail extendedDetail = (QContactExtendedDetail)contact.detail(QContactExtendedDetail::Type);
    QCOMPARE(extendedDetail.name(), extendedDetailName);
    QCOMPARE(extendedDetail.data(), extendedDetailData);
    QCOMPARE(extendedDetail.data().type(), extendedDetailData.type());
}

void tst_QVersitContactImporter::testExtendedDetail_data()
{
    QTest::addColumn<QString>("extendedDetailName");
    QTest::addColumn<QVariant>("extendedDetailData");
    QTest::addColumn<QString>("extendedDetailDataInProperty");
    QTest::addColumn<bool>("extendedDetailCreated");

    QString jsonArrayWith("[\n    %1\n]\n");
    QString jsonArrayWithString = jsonArrayWith.arg("\"%1\"");
    {
        QTest::newRow("string data")
                << QString("name")
                << QVariant(QString("data"))
                << jsonArrayWithString.arg("data")
                << true;
        QTest::newRow("string data, empty")
                << QString("name")
                << QVariant(QString(""))
                << jsonArrayWithString.arg("")
                << true;
        QTest::newRow("string data, containing reserved characters")
                << QString("name")
                << QVariant(QString(",;:\\"))
                << jsonArrayWithString.arg(",;:\\\\")
                << true;
    }

    {
        QTest::newRow("double data")
                << QString("name")
                << QVariant((double)2.0)
                << jsonArrayWith.arg("2")
                << true;
        QTest::newRow("double data, negative")
                << QString("name")
                << QVariant((double)-1.0)
                << jsonArrayWith.arg("-1")
                << true;
        QTest::newRow("double data, multiple digits")
                << QString("name")
                << QVariant((double)10.2)
                << jsonArrayWith.arg("10.2")
                << true;
    }

    {
        QTest::newRow("boolean data")
                << QString("name")
                << QVariant(true)
                << jsonArrayWith.arg("true")
                << true;
    }

    {
        QTest::newRow("datetime data, imported as a string")
                << QString("name")
                << QVariant(QString("1997-07-16T19:20:30+01:00"))
                << jsonArrayWithString.arg("1997-07-16T19:20:30+01:00")
                << true;
    }

    {
        QTest::newRow("list data")
                << QString("name")
                << QVariant(QVariantList() << QString("string 1") << QString("string 2"))
                << QString("[\n    [\n        \"string 1\",\n        \"string 2\"\n    ]\n]\n")
                << true;
    }

    {
        QVariantMap map;
        map["key 1"] = QString("string 1");
        map["key 2"] = QString("string 2");
        QTest::newRow("map data")
                << QString("name")
                << QVariant(map)
                << QString("[\n    {\n        \"key 1\": \"string 1\",\n        \"key 2\": \"string 2\"\n    }\n]\n")
                << true;
    }

    {
        QVariantMap map;
        map["key"] = QVariantList() << (double)1 << (double)2;
        QTest::newRow("map data, containing a nested list")
                << QString("name")
                << QVariant(map)
                << QString("[\n    {\n        \"key\": [\n            1,\n            2\n        ]\n    }\n]\n")
                << true;
    }

    {
        QTest::newRow("empty string as name")
                << QString("")
                << QVariant(QString("data"))
                << jsonArrayWithString.arg("data")
                << true;
        QTest::newRow("name containing reserved characters")
                << QString(",;:\\")
                << QVariant(QString("data"))
                << jsonArrayWithString.arg("data")
                << true;
    }

    {
        QTest::newRow("null denoting an empty variant")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("null")
                << true;
        QTest::newRow("compact json with extra whitespace removed")
                << QString("name")
                << QVariant(QString("data"))
                << QString("[\"data\"]")
                << true;
    }

    {
        QTest::newRow("invalid property value: empty json array as property value")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("")
                << false;
        QTest::newRow("invalid property value: invalid json value")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("invalid")
                << false;
    }
}

void tst_QVersitContactImporter::addExtendedDetailPropertyToDocument(QString name, QString data, QVersitDocument &document)
{
    QVersitProperty property;
    property.setName(QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL"));
    property.setValue(QStringList()
                      << name
                      << data);
    property.setValueType(QVersitProperty::CompoundType);
    document.addProperty(property);
}

void tst_QVersitContactImporter::testMultipleExtendedDetails()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QString jsonArrayWith("[\n    %1\n]\n");
    QString jsonArrayWithString = jsonArrayWith.arg("\"%1\"");
    addExtendedDetailPropertyToDocument("detailName1", jsonArrayWithString.arg("detailData1"), document);
    addExtendedDetailPropertyToDocument("detailName2", jsonArrayWithString.arg("detailData2"), document);

    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));

    QContact contact = mImporter->contacts().first();
    QCOMPARE(contact.details(QContactDetail::TypeExtendedDetail).size(), 2);
    QList<QContactDetail> actualDetails = contact.details(QContactDetail::TypeExtendedDetail);
    QContactExtendedDetail extendedDetail1 = (QContactExtendedDetail)actualDetails.at(0);
    QContactExtendedDetail extendedDetail2 = (QContactExtendedDetail)actualDetails.at(1);
    QCOMPARE(extendedDetail1.name(), QString("detailName1"));
    QCOMPARE(extendedDetail1.data().toString(), QString("detailData1"));
    QCOMPARE(extendedDetail2.name(), QString("detailName2"));
    QCOMPARE(extendedDetail2.data().toString(), QString("detailData2"));
}

void tst_QVersitContactImporter::testPref()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property1;
    property1.setName(QStringLiteral("TEL"));
    property1.setValue(QStringLiteral("1"));
    document.addProperty(property1);
    QVersitProperty property2;
    property2.setName(QStringLiteral("TEL"));
    property2.setValue(QStringLiteral("2"));
    property2.insertParameter(QStringLiteral("TYPE"), QStringLiteral("PREF"));
    document.addProperty(property2);
    QVersitProperty property3;
    property3.setName(QStringLiteral("TEL"));
    property3.setValue(QStringLiteral("3"));
    property3.insertParameter(QStringLiteral("TYPE"), QStringLiteral("PREF"));
    document.addProperty(property3);
    QVersitProperty property4;
    property4.setName(QStringLiteral("TEL"));
    property4.setValue(QStringLiteral("4"));
    document.addProperty(property4);

    // Test that pref details comes first.
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactPhoneNumber firstNumber = contact.detail<QContactPhoneNumber>();
    QCOMPARE(firstNumber.number(), QStringLiteral("2"));
    QList<QContactPhoneNumber> numbers = contact.details<QContactPhoneNumber>();
    QCOMPARE(numbers.at(0).number(), QStringLiteral("2"));
    QCOMPARE(numbers.at(1).number(), QStringLiteral("3"));
    QCOMPARE(numbers.at(2).number(), QStringLiteral("1"));
    QCOMPARE(numbers.at(3).number(), QStringLiteral("4"));
}

void tst_QVersitContactImporter::testPropertyHandler()
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    QVersitProperty property;

    // No unconverted properties, no converted properties either.  Fails with EmptyDocumentError
    QVERIFY(!mImporter->importDocuments(QList<QVersitDocument>() << document));
    QCOMPARE(mPropertyHandler->mUnknownProperties.size(), 0);
    QCOMPARE(mPropertyHandler->mPreProcessedProperties.size(), 0);
    QCOMPARE(mPropertyHandler->mPostProcessedProperties.size(), 0);

    // No unconverted properties, one converted property
    mPropertyHandler->clear();
    property.setName(QStringLiteral("FN"));
    property.setValue(QStringLiteral("John Citizen"));
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QCOMPARE(mPropertyHandler->mUnknownProperties.size(), 0);
    QCOMPARE(mPropertyHandler->mPreProcessedProperties.size(), 1);
    QCOMPARE(mPropertyHandler->mPostProcessedProperties.size(), 1);

    // Set the handler to override handling of the property
    mPropertyHandler->clear();
    mPropertyHandler->mPreProcess = true;
    document.clear();
    document.setType(QVersitDocument::VCard30Type);
    property.setName(QStringLiteral("FN"));
    property.setValue(QStringLiteral("John Citizen"));
    document.addProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    contact = mImporter->contacts().first();
    QCOMPARE(mPropertyHandler->mUnknownProperties.size(), 0);
    QCOMPARE(mPropertyHandler->mPreProcessedProperties.size(), 1);
    QCOMPARE(mPropertyHandler->mPostProcessedProperties.size(), 0);
    QContactDetail nameDetail = contact.detail(QContactName::Type);
    QVERIFY(nameDetail.isEmpty());

    // One unknown property
    mPropertyHandler->clear();
    property.setName(QStringLiteral("X-EXTENSION-1"));
    property.setValue(QStringLiteral("extension value 1"));
    document.addProperty(property);
    mImporter->importDocuments(QList<QVersitDocument>() << document);
    QList<QVersitProperty> unknownProperties = mPropertyHandler->mUnknownProperties;
    QCOMPARE(unknownProperties.count(), 1);
    QCOMPARE(unknownProperties[0].name(), QStringLiteral("X-EXTENSION-1"));
    QCOMPARE(unknownProperties[0].value(), QStringLiteral("extension value 1"));

    // Two unknown properties
    mPropertyHandler->clear();
    property.setName(QStringLiteral("X-EXTENSION-2"));
    property.setValue(QStringLiteral("extension value 2"));
    document.addProperty(property);
    mImporter->importDocuments(QList<QVersitDocument>() << document);
    unknownProperties = mPropertyHandler->mUnknownProperties;
    QCOMPARE(unknownProperties.count(), 2);
    QCOMPARE(unknownProperties[0].name(), QStringLiteral("X-EXTENSION-1"));
    QCOMPARE(unknownProperties[0].value(), QStringLiteral("extension value 1"));
    QCOMPARE(unknownProperties[1].name(), QStringLiteral("X-EXTENSION-2"));
    QCOMPARE(unknownProperties[1].value(), QStringLiteral("extension value 2"));
}

void tst_QVersitContactImporter::testInvalidDocument()
{
    // invalid document (invalid type)
    QList<QVersitDocument> documents;
    QVersitDocument document(QVersitDocument::InvalidType);
    QVersitProperty nameProperty;
    nameProperty.setName(QStringLiteral("FN"));
    nameProperty.setValue(QStringLiteral("John Citizen"));
    document.addProperty(nameProperty);
    documents.append(document);

    // valid document in the same list
    QVersitProperty telProperty;
    telProperty.setName(QStringLiteral("TEL"));
    telProperty.setValue(QStringLiteral("1234"));
    document.addProperty(telProperty);
    document.setType(QVersitDocument::VCard21Type);
    documents.append(document);
    QVERIFY(!mImporter->importDocuments(documents));
    QMap<int, QVersitContactImporter::Error> errorMap = mImporter->errorMap();
    QCOMPARE(errorMap.size(), 1);
    QVERIFY(errorMap.contains(0));
    QVERIFY(errorMap.value(0) == QVersitContactImporter::InvalidDocumentError);
    QList<QContact> contacts = mImporter->contacts();
    QCOMPARE(contacts.size(), 1);
    QContactPhoneNumber phoneDetail = contacts.first().detail<QContactPhoneNumber>();
    QCOMPARE(phoneDetail.number(), QStringLiteral("1234"));

    // empty document
    document.clear();
    document.setType(QVersitDocument::VCard21Type);
    QVERIFY(!mImporter->importDocuments(QList<QVersitDocument>() << document));
    errorMap = mImporter->errorMap();
    QCOMPARE(errorMap.size(), 1);
    QCOMPARE(errorMap.value(0), QVersitContactImporter::EmptyDocumentError);
    QCOMPARE(mImporter->errors(), errorMap);

}

void tst_QVersitContactImporter::testEmailWithContextOther()
{
    QVersitProperty property;
    property.setName(QStringLiteral("EMAIL"));
    QString value(QStringLiteral("john.citizen@example.com"));
    property.setValue(value);
    property.insertParameter(QStringLiteral("TYPE"),QStringLiteral("OTHER"));
    QVersitDocument document = createDocumentWithProperty(property);
    QVERIFY(mImporter->importDocuments(QList<QVersitDocument>() << document));
    QContact contact = mImporter->contacts().first();
    QContactEmailAddress email = contact.detail<QContactEmailAddress>();
    QCOMPARE(email.emailAddress(),value);
    const QList<int> contexts = email.contexts();
    QCOMPARE(contexts.count(),1);
    QVERIFY(contexts.contains(QContactDetail::ContextOther));
}

QVersitDocument tst_QVersitContactImporter::createDocumentWithProperty(
    const QVersitProperty& property)
{
    QVersitDocument document(QVersitDocument::VCard30Type);
    document.addProperty(property);
    return document;
}

QVersitDocument tst_QVersitContactImporter::createDocumentWithNameAndPhoto(
    const QString& name,
    QByteArray image,
    const QString& imageType)
{
    QVersitDocument document(QVersitDocument::VCard30Type);

    QVersitProperty nameProperty;
    nameProperty.setName(QStringLiteral("FN"));
    nameProperty.setValue(name);
    document.addProperty(nameProperty);

    QVersitProperty property;
    property.setName(QStringLiteral("PHOTO"));
    property.setValue(image);
    if (imageType != QString()) {
        property.insertParameter(QStringLiteral("TYPE"), imageType);
    }
    document.addProperty(property);

    return document;
}

QTEST_MAIN(tst_QVersitContactImporter)

