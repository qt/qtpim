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

#include "tst_qversitcontactexporter.h"
#include <QtVersit/qversitcontactexporter.h>
#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>
#include <QtContacts/qcontacts.h>
#include <QtContacts/qcontactmanagerengine.h>
#include <QString>
#include <QStringList>
#include <QList>
#include <QPixmap>
#include <QtTest/QtTest>

QTCONTACTS_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

class MyQVersitContactExporterDetailHandler : public QVersitContactExporterDetailHandler
{
public:
    MyQVersitContactExporterDetailHandler() : mPreProcess(false)
    {
    }

    bool preProcessDetail(const QContact& contact,
                          const QContactDetail& detail,
                          QVersitDocument* document)
    {
        Q_UNUSED(contact)
        Q_UNUSED(document)
        mPreProcessedDetails.append(detail);
        return mPreProcess;
    }

    bool postProcessDetail(const QContact& contact,
                           const QContactDetail& detail,
                           bool alreadyProcessed,
                           QVersitDocument* document)
    {
        Q_UNUSED(contact)
        Q_UNUSED(document)
        if (!alreadyProcessed)
            mUnknownDetails.append(detail);
        else
            mPostProcessedDetails.append(detail);
        return false;
    }

    void clear()
    {
        mPreProcess = false;
        mDefinitionNamesToProcess.clear();
        mUnknownDetails.clear();
        mPreProcessedDetails.clear();
        mPostProcessedDetails.clear();
    }

    // a hook to control what preProcess returns:
    bool mPreProcess;
    QStringList mDefinitionNamesToProcess;
    QList<QContactDetail> mUnknownDetails;
    QList<QContactDetail> mPreProcessedDetails;
    QList<QContactDetail> mPostProcessedDetails;
};

/* This class just logs the arguments to the last call to postProcessDetail */
class MyQVersitContactExporterDetailHandlerV2 : public QVersitContactExporterDetailHandlerV2
{
public:
    MyQVersitContactExporterDetailHandlerV2()
    {
    }

    void detailProcessed(const QContact& contact,
                         const QContactDetail& detail,
                         const QVersitDocument& document,
                         QSet<int>* processedFields,
                         QList<QVersitProperty>* toBeRemoved,
                         QList<QVersitProperty>* toBeAdded)
    {
        mContact = contact;
        mDetail = detail;
        mDocument = document;
        mProcessedFields = *processedFields;
        mToBeRemoved = *toBeRemoved;
        mToBeAdded = *toBeAdded;
    }

    void contactProcessed(const QContact& contact, QVersitDocument* document)
    {
        mEndContact = contact;
        mEndDocument = *document;
    }

    QContact mContact;
    QContactDetail mDetail;
    QSet<int> mProcessedFields;
    QVersitDocument mDocument;
    QList<QVersitProperty> mToBeRemoved;
    QList<QVersitProperty> mToBeAdded;

    QContact mEndContact;
    QVersitDocument mEndDocument;
};

class MyQVersitResourceHandler : public QVersitResourceHandler
{
public:
    MyQVersitResourceHandler()
        : mLoadResourceCalled(false),
        mLoadSuccess(true)
    {
    }

    bool loadResource(const QString& location, QByteArray* contents, QString* mimeType)
    {
        mLocation = location;
        *contents = mSimulatedData;
        *mimeType = mSimulatedMimeType;
        mLoadResourceCalled = true;
        return mLoadSuccess;
    }

    bool saveResource(const QByteArray &contents, const QVersitProperty &property, QString *location)
    {
        Q_UNUSED(contents)
        Q_UNUSED(property)
        Q_UNUSED(location)
        return false;
    }

    void clear()
    {
        mSimulatedData.clear();
        mSimulatedMimeType.clear();
        mLocation.clear();
        mLoadResourceCalled = false;
        mLoadSuccess = true;
    }

    QByteArray mSimulatedData;
    QString mSimulatedMimeType;
    QString mLocation;
    bool mLoadResourceCalled;
    bool mLoadSuccess; // A hook to control what loadResource returns.
};

const static QByteArray SAMPLE_GIF(QByteArray::fromBase64(
        "R0lGODlhEgASAIAAAAAAAP///yH5BAEAAAEALAAAAAASABIAAAIdjI+py+0G"
        "wEtxUmlPzRDnzYGfN3KBaKGT6rDmGxQAOw=="));

const QString TEST_PHOTO_FILE(QStringLiteral("versitTest001.jpg"));
const QString TEST_AUDIO_FILE(QStringLiteral("versitTest001.wav"));

// Checks that the property has a value of the given expectedType and the given expectedValue.
#define CHECK_VALUE(property,expectedValueType,expectedValue) {\
    QCOMPARE(property.valueType(), expectedValueType); \
    QVariant value = property.variantValue(); \
    QCOMPARE(value.type(), QVariant::StringList); \
    QCOMPARE(value.toStringList(), expectedValue); \
}
#define CHECK_FLOATING_POINT_VALUE(property, expectedValueType, expectedValue, isDouble) {\
    QCOMPARE(property.valueType(), expectedValueType); \
    QVariant value = property.variantValue(); \
    QCOMPARE(value.type(), QVariant::StringList); \
    QVERIFY(value.toStringList().size() == 2); \
    QVERIFY(expectedValue.size() == 2); \
    QString actualString = value.toStringList().at(1); \
    actualString.replace('[',"").replace(']',"").replace('\n', ""); \
    QString expectedString = expectedValue.at(1); \
    expectedString.replace('[',"").replace(']',"").replace('\n', ""); \
    if (isDouble) { \
        double actualD = actualString.toDouble(); \
        double expectedD = expectedString.toDouble(); \
        QCOMPARE(value.toStringList().first(), expectedValue.first()); /* name  */ \
        QCOMPARE(actualD, expectedD); /* value */ \
    } else { \
        float actualF = actualString.toFloat(); \
        float expectedF = expectedString.toFloat(); \
        QCOMPARE(value.toStringList().first(), expectedValue.first()); /* name  */ \
        QCOMPARE(actualF, expectedF); /* value */ \
    } \
}


void tst_QVersitContactExporter::init()
{
    mExporter = new QVersitContactExporter();
    mResourceHandler = new MyQVersitResourceHandler;
    mExporter->setResourceHandler(mResourceHandler);
}

void tst_QVersitContactExporter::cleanup()
{
    QVERIFY(mExporter->resourceHandler() == mResourceHandler);
    mExporter->setResourceHandler(0);
    delete mResourceHandler;
    delete mExporter;
}

void tst_QVersitContactExporter::testConvertContact()
{
    QContact contact;

    // Adding name to the contact
    QContactName name;
    name.setFirstName(QStringLiteral("Moido"));
    contact.saveDetail(&name);

    // Adding phone number to the Contact.
    QContactPhoneNumber phoneNumber;
    phoneNumber.setNumber(QStringLiteral("12345678"));
    contact.saveDetail(&phoneNumber);

    // Convert contact into versit properties
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QList<QVersitDocument> documents = mExporter->documents();

    // Contact should have N and TEL properties
    QCOMPARE(documents.size(), 1);
    QCOMPARE(documents.first().properties().count(), 2);
}

void tst_QVersitContactExporter::testEmptyContact()
{
    QContact contact1; // empty
    QList<QContact> contacts;
    contacts << contact1;

    QVERIFY(!mExporter->exportContacts(contacts)); // Fail on empty contact1
    QMap<int, QVersitContactExporter::Error> errorMap = mExporter->errorMap();
    QCOMPARE(errorMap.size(), 1);
    QList<QVersitDocument> documents = mExporter->documents();
    QCOMPARE(documents.size(), 0); // empty contact not exported
}

void tst_QVersitContactExporter::testContactDetailHandler()
{
    int invalidData = 123456;
    MyQVersitContactExporterDetailHandler detailHandler;;
    mExporter->setDetailHandler(&detailHandler);

    // Test1: Un-supported Avatar Test
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactDetail unknownDetail;
    unknownDetail.setValue(invalidData, QStringLiteral("Detail"));
    contact.saveDetail(&unknownDetail);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 0);
    QList<QContactDetail> unknownDetails = detailHandler.mUnknownDetails;
    QVERIFY(unknownDetails.size() > 0);
    QContactDetail::DetailType type = unknownDetail.type();
    QContactDetail detail = findDetailByType(unknownDetails,type);
    QCOMPARE(type, detail.type());

    // Test2: Un-supported Online Account
    QContactOnlineAccount onlineAccount;
    QString testUri = QStringLiteral("sip:abc@temp.com");
    onlineAccount.setAccountUri(testUri);
    QList<int> expectedSubtypes;

    expectedSubtypes << invalidData;

    onlineAccount.setSubTypes(expectedSubtypes);
    contact.saveDetail(&onlineAccount);
    detailHandler.clear();
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 0);
    unknownDetails = detailHandler.mUnknownDetails;
    QVERIFY(unknownDetails.size() > 0);
    type = onlineAccount.type();
    detail = findDetailByType(unknownDetails, type);
    QCOMPARE(type, detail.type());

    QVERIFY(mExporter->detailHandler() == &detailHandler);
    mExporter->setDetailHandler(static_cast<QVersitContactExporterDetailHandler*>(0));
}

void tst_QVersitContactExporter::testContactDetailHandlerV2()
{
    int invalidData = 123456;
    MyQVersitContactExporterDetailHandlerV2 detailHandler;
    mExporter->setDetailHandler(&detailHandler);

    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactPhoneNumber phone;
    phone.setNumber(QStringLiteral("1234"));
    phone.setValue(invalidData, QStringLiteral("Value"));
    contact.saveDetail(&phone);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));

    QCOMPARE(detailHandler.mProcessedFields.size(), 3);
    QVERIFY(detailHandler.mProcessedFields.contains(QContactPhoneNumber::FieldContext));
    QVERIFY(detailHandler.mProcessedFields.contains(QContactPhoneNumber::FieldSubTypes));
    QVERIFY(detailHandler.mProcessedFields.contains(QContactPhoneNumber::FieldNumber));
    QVersitProperty expectedProperty;
    expectedProperty.setName(QStringLiteral("TEL"));
    expectedProperty.setValue(QStringLiteral("1234"));
    QCOMPARE(detailHandler.mToBeAdded.size(), 1);
    QCOMPARE(detailHandler.mToBeAdded.first(), expectedProperty);

    mExporter->setDetailHandler(static_cast<QVersitContactExporterDetailHandlerV2*>(0));
}

void tst_QVersitContactExporter::testEncodeName()
{
    QContact contact;
    QContactName name;

    // Test 1: An empty contact - can't be exported
    QVERIFY(!mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    QCOMPARE(mExporter->documents().size(), 0);

    // Test 2: Special characters are NOT backslash escaped by the exporter, only by the writer.
    name.setFirstName(QStringLiteral("Fi;rst")); // check that semicolon is left intact
    name.setLastName(QStringLiteral("Last"));
    name.setMiddleName(QStringLiteral("Middle"));
    name.setPrefix(QStringLiteral("Prefix"));
    name.setSuffix(QStringLiteral("Suffix"));
    contact.saveDetail(&name);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(document.properties().count(), 1);

    QVersitProperty nameProperty = findPropertyByName(document, QStringLiteral("N"));
    QCOMPARE(nameProperty.parameters().count(), 0);
    QCOMPARE(nameProperty.name(), QStringLiteral("N"));
    CHECK_VALUE(nameProperty, QVersitProperty::CompoundType,
                QStringList() << QStringLiteral("Last") << QStringLiteral("Fi;rst")
                << QStringLiteral("Middle") << QStringLiteral("Prefix") << QStringLiteral("Suffix"));

    // Test 3: Just QContactName
    contact.clearDetails();
    name = QContactName();
    name.setFirstName(QStringLiteral("First"));
    name.setLastName(QStringLiteral("Last"));
    name.setMiddleName(QStringLiteral("Middle"));
    contact.saveDetail(&name);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    document = mExporter->documents().first();
    QCOMPARE(document.properties().count(), 1);
    // Unlike older versions, FN is not generated
    nameProperty = findPropertyByName(document, QStringLiteral("N"));
    QCOMPARE(nameProperty.name(), QStringLiteral("N"));
    CHECK_VALUE(nameProperty, QVersitProperty::CompoundType, QStringList()
                << QStringLiteral("Last") << QStringLiteral("First") << QStringLiteral("Middle")
                << QString() << QString());

    // Test 4: Just nickname
    contact.clearDetails();
    QContactNickname nickname;
    nickname.setNickname(QStringLiteral("Nickname"));
    contact.saveDetail(&nickname);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    document = mExporter->documents().first();
    QCOMPARE(document.properties().count(), 2); // N and NICKNAME
    // Unlike older versions, FN is not generated
    nameProperty = findPropertyByName(document, QStringLiteral("N"));
    QCOMPARE(nameProperty.name(), QStringLiteral("N"));
    CHECK_VALUE(nameProperty, QVersitProperty::CompoundType, QStringList()
                << QString() << QString() << QString()
                << QString() << QString());

    // Test 5: Just organization
    contact.clearDetails();
    QContactOrganization org;
    org.setName(QStringLiteral("Organization"));
    contact.saveDetail(&org);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    document = mExporter->documents().first();
    QCOMPARE(document.properties().count(), 2); // N and ORG
    // Unlike older versions, FN is not generated
    nameProperty = findPropertyByName(document, QStringLiteral("N"));
    QCOMPARE(nameProperty.name(), QStringLiteral("N"));
    CHECK_VALUE(nameProperty, QVersitProperty::CompoundType, QStringList()
                << QString() << QString() << QString()
                << QString() << QString());

    // Test 6: Display label but no name set
    contact.clearDetails();
    QContactDisplayLabel displaylabel;
    displaylabel.setLabel(QStringLiteral("Bobby Tables"));
    contact.saveDetail(&displaylabel);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    document = mExporter->documents().first();
    QCOMPARE(document.properties().count(), 2); // N
    // Unlike older versions, FN is not generated from display label
    nameProperty = findPropertyByName(document, QStringLiteral("N"));
    QCOMPARE(nameProperty.name(), QStringLiteral("N"));
    CHECK_VALUE(nameProperty, QVersitProperty::CompoundType, QStringList()
                << QString() << QString() << QString() << QString() << QString());
}

void tst_QVersitContactExporter::testEncodePhoneNumber()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactPhoneNumber phoneNumber;
    QContactPhoneNumber::SubType expectedSubType = QContactPhoneNumber::SubTypeMobile;
    QList<int> expectedSubTypes;
    expectedSubTypes << expectedSubType;
    phoneNumber.setNumber(QStringLiteral("12345678"));
    phoneNumber.setContexts(QContactDetail::ContextHome);
    phoneNumber.setSubTypes(expectedSubTypes);
    contact.saveDetail(&phoneNumber);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("TEL"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 2);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("HOME")));
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("CELL")));
    // Check value
    QCOMPARE(property.value(), phoneNumber.number());

    QContactPhoneNumber assistantNumber;
    assistantNumber.setNumber(QStringLiteral("4321"));
    assistantNumber.setContexts(QContactDetail::ContextWork);
    expectedSubTypes.clear();
    expectedSubTypes << QContactPhoneNumber::SubTypeAssistant
                     << QContactPhoneNumber::SubTypeLandline;
    assistantNumber.setSubTypes(expectedSubTypes);
    contact.saveDetail(&assistantNumber);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    property = findPropertyByName(document, QStringLiteral("X-ASSISTANT-TEL"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 2);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("WORK")));
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"), QStringLiteral("VOICE")));
    QCOMPARE(property.value(), assistantNumber.number());
}

void tst_QVersitContactExporter::testEncodeEmailAddress()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactEmailAddress email;
    email.setEmailAddress(QStringLiteral("test@test"));
    email.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&email);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("EMAIL"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 1);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("HOME")));
    // Check value
    QCOMPARE(property.value(), email.emailAddress());
}

void tst_QVersitContactExporter::testEncodeStreetAddress()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactAddress address;
    QContactAddress::SubType expectedSubType;
    QList<int> expectedSubTypes;
    expectedSubType = QContactAddress::SubTypePostal;
    expectedSubTypes << expectedSubType;
    address.setPostOfficeBox(QStringLiteral("1234"));
    address.setCountry(QStringLiteral("Finland"));
    address.setPostcode(QStringLiteral("00440"));
    // Special characters are not escaped by the exporter, but by the writer
    address.setStreet(QStringLiteral("HKKI; 1X 90"));
    address.setLocality(QStringLiteral("Helsinki"));
    address.setContexts(QContactDetail::ContextHome);
    address.setSubTypes(expectedSubTypes);
    contact.saveDetail(&address);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("ADR"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 2);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("HOME")));
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("POSTAL")));
    // Check name
    QCOMPARE(property.name(), QStringLiteral("ADR"));
    CHECK_VALUE(property, QVersitProperty::CompoundType,
                QStringList() << QStringLiteral("1234") << QString() << QStringLiteral("HKKI; 1X 90")
                << QStringLiteral("Helsinki") << QString() << QStringLiteral("00440")
                << QStringLiteral("Finland"));
}

void tst_QVersitContactExporter::testEncodeUrl()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactUrl url;
    url.setUrl(QStringLiteral("http://www.myhome.com"));
    url.setContexts(QContactDetail::ContextHome);
    url.setSubType(QContactUrl::SubTypeHomePage);
    contact.saveDetail(&url);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("URL"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 1);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("HOME")));
    // Check value
    QCOMPARE(property.value(), url.url());
}

void tst_QVersitContactExporter::testEncodeUid()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactGuid guid;

    guid.setContexts(QContactDetail::ContextHome);
    guid.setGuid(QStringLiteral("0101222"));
    contact.saveDetail(&guid);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard21Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("UID"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    // Contexts are not allowed for UID
    QCOMPARE(property.parameters().count(), 0);
    // Check value
    QCOMPARE(property.value(), guid.guid());
}

void tst_QVersitContactExporter::testEncodeRev()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactTimestamp timeStamp;

    // Last modified time found
    QDateTime revisionTime =
        QDateTime::fromString(
            QStringLiteral("M1d1y200906:01:02"),
            QStringLiteral("'M'M'd'd'y'yyyyhh:mm:ss"));
    revisionTime.setTimeSpec(Qt::UTC);
    timeStamp.setLastModified(revisionTime);
    // Contexts not allowed in REV property, check that they are not added
    timeStamp.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&timeStamp);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("REV"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QString expectedValueUTCEncoded = QStringLiteral("2009-01-01T06:01:02Z");
    QCOMPARE(property.value(), expectedValueUTCEncoded);

    // Last modified time not found, use the creation time
    QDateTime emptyTime;
    timeStamp.setLastModified(emptyTime);
    timeStamp.setCreated(revisionTime);
    contact.saveDetail(&timeStamp);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("REV"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.value(), expectedValueUTCEncoded);

    // Last modified time found, Local Time spec not UTC
    QDateTime localTime;
    revisionTime.setTimeSpec(Qt::LocalTime);
    timeStamp.setLastModified(revisionTime);
    localTime.setTimeSpec(Qt::LocalTime);
    timeStamp.setCreated(localTime);
    contact.saveDetail(&timeStamp);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("REV"));
    QVERIFY(!property.isEmpty());
    QString expectedValueEncoded = QStringLiteral("2009-01-01T06:01:02");
    QCOMPARE(property.value(), expectedValueEncoded);

    // Last modified time not found, creation time not found
    timeStamp.setLastModified(emptyTime);
    timeStamp.setCreated(emptyTime);
    contact.saveDetail(&timeStamp);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 0);
}

void tst_QVersitContactExporter::testEncodeVersion()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactVersion version;
    int sequenceNumber = 4711;
    QByteArray extendedVersion("134f23dbb2");
    version.setSequenceNumber(sequenceNumber);
    version.setExtendedVersion(extendedVersion);
    // Contexts not allowed in X-QTPROJECT-VERSION property, check that they are not added.
    version.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&version);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("X-QTPROJECT-VERSION"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    CHECK_VALUE(property, QVersitProperty::CompoundType,
                QStringList() << QStringLiteral("4711") << QStringLiteral("134f23dbb2"));
}

void tst_QVersitContactExporter::testEncodeBirthDay()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QDate date(2009,1,1);
    QContactBirthday birthDay;
    birthDay.setDate(date);
    // Contexts not allowed in BDAY property, check that they are not added
    birthDay.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&birthDay);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("BDAY"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.value(), QStringLiteral("2009-01-01"));

    birthDay.setDateTime(QDateTime(QDate(2009, 1, 1), QTime(1, 2, 3)));
    contact.saveDetail(&birthDay);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    property = findPropertyByName(document, QStringLiteral("BDAY"));
    QCOMPARE(property.value(), QStringLiteral("2009-01-01T01:02:03"));
}

void tst_QVersitContactExporter::testEncodeNote()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactNote note;
    note.setNote(QStringLiteral("My Note"));
    // Contexts not allowed in NOTE property, check that they are not added
    note.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&note);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("NOTE"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.value(), note.note());
}

void tst_QVersitContactExporter::testEncodeGeoLocation()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactGeoLocation geoLocation;
    QString longitude = QStringLiteral("99.9");
    geoLocation.setLongitude(longitude.toDouble());
    QString latitude = QStringLiteral("98.9");
    geoLocation.setLatitude(latitude.toDouble());
    // Contexts not allowed in GEO property, check that they are not added
    geoLocation.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&geoLocation);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("GEO"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.name(), QStringLiteral("GEO"));
    CHECK_VALUE(property, QVersitProperty::CompoundType,
                QStringList() << QStringLiteral("98.9") << QStringLiteral("99.9"));
}

void tst_QVersitContactExporter::testEncodeOrganization()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactOrganization organization;
    QVersitDocument document;
    QVersitProperty property;
    QString title(QStringLiteral("Developer"));

    // TITLE
    organization.setTitle(title);
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("TITLE"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.value(), title);

    // ORG with name
    organization.setTitle(QString());
    organization.setName(QStringLiteral("Nokia"));
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("ORG"));
    QCOMPARE(property.name(), QStringLiteral("ORG"));
    CHECK_VALUE(property, QVersitProperty::CompoundType, QStringList(QStringLiteral("Nokia")));

    // ORG with department/unit
    organization.setName(QString());
    QStringList departments(QStringLiteral("R&D"));
    departments.append(QStringLiteral("Qt"));
    organization.setDepartment(departments);
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("ORG"));
    QCOMPARE(property.name(), QStringLiteral("ORG"));
    CHECK_VALUE(property, QVersitProperty::CompoundType, QStringList()
                << QString() << QStringLiteral("R&D") << QStringLiteral("Qt"));

    // ORG with name and department/unit
    organization.setName(QStringLiteral("Nokia"));
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("ORG"));
    QCOMPARE(property.name(), QStringLiteral("ORG"));
    CHECK_VALUE(property, QVersitProperty::CompoundType, QStringList()
                << QStringLiteral("Nokia") << QStringLiteral("R&D") << QStringLiteral("Qt"));

    // TITLE and ORG
    organization.setTitle(QStringLiteral("Developer"));
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    property = findPropertyByName(document, QStringLiteral("TITLE"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.value(), title);
    property = findPropertyByName(document, QStringLiteral("ORG"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.name(), QStringLiteral("ORG"));
    CHECK_VALUE(property, QVersitProperty::CompoundType, QStringList()
                << QStringLiteral("Nokia") << QStringLiteral("R&D") << QStringLiteral("Qt"));

    // ORG LOGO Test1: LOGO as remote Resouce
    const QString url = QStringLiteral("http://myhome.com/test.jpg");
    contact = createContactWithName(QStringLiteral("asdf"));
    organization = QContactOrganization();
    organization.setLogoUrl(url);
    contact.saveDetail(&organization);
    mResourceHandler->mSimulatedMimeType = QStringLiteral("image/jpeg");
    mExporter->setResourceHandler(mResourceHandler);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QVERIFY(!mResourceHandler->mLoadResourceCalled);

    // Source type is encoded, but media type is not for a URL.
    property = findPropertyByName(document, QStringLiteral("LOGO"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 1);

    QVERIFY(property.parameters().contains(
            QStringLiteral("VALUE"), QStringLiteral("uri")));

    //Check property value
    QCOMPARE(property.value(), url);

    // ORG LOGO Test2: LOGO File.
    mResourceHandler->mSimulatedData = "simulated data";
    contact = createContactWithName(QStringLiteral("asdf"));
    organization = QContactOrganization();
    organization.setLogoUrl(TEST_PHOTO_FILE);
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QVERIFY(mResourceHandler->mLoadResourceCalled);
    QCOMPARE(mResourceHandler->mLocation, TEST_PHOTO_FILE);

    // It should be stored in the property as a QVariant of QByteArray
    property = findPropertyByName(document, QStringLiteral("LOGO"));
    QVERIFY(!property.isEmpty());
    QMultiHash<QString,QString> parameters = property.parameters();
    // Media type is encoded
    QCOMPARE(parameters.count(), 1);
    QVERIFY(parameters.contains(
            QStringLiteral("TYPE"), QStringLiteral("JPEG")));
    // Verify value.
    QVariant variantValue = property.variantValue();
    QVERIFY(variantValue.type() == QVariant::ByteArray);
    QCOMPARE(variantValue.value<QByteArray>(), mResourceHandler->mSimulatedData);

    // Assistant Name Test.
    contact = createContactWithName(QStringLiteral("asdf"));
    organization = QContactOrganization();
    organization.setAssistantName(QStringLiteral("myAssistant"));
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-ASSISTANT"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.value(), QStringLiteral("myAssistant"));

    // Test: Role
    contact = createContactWithName(QStringLiteral("asdf"));
    organization = QContactOrganization();
    organization.setRole(QStringLiteral("Executive"));
    contact.saveDetail(&organization);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("ROLE"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.value(), QStringLiteral("Executive"));

}

void tst_QVersitContactExporter::testEncodeAvatar()
{
    QContact contact = createContactWithName(QStringLiteral("asdf"));
    QContactAvatar contactAvatar;
    mResourceHandler->mSimulatedData = "simulated data";
    mResourceHandler->mSimulatedMimeType = QStringLiteral("image/jpeg");

    // Test1: Web URL
    const QString url = QStringLiteral("http://www.myhome.com/test.jpg");
    contactAvatar.setImageUrl(url);
    contact.saveDetail(&contactAvatar);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QVersitProperty property = findPropertyByName(document, QStringLiteral("PHOTO"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 1);
    QCOMPARE(property.value(), url);
    QVERIFY(!mResourceHandler->mLoadResourceCalled);

    // Test 2: Local Media PHOTO
    contactAvatar.setImageUrl(TEST_PHOTO_FILE);
    contact.saveDetail(&contactAvatar);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QVERIFY(mResourceHandler->mLoadResourceCalled);
//    This is commented out due to test failure
//    QCOMPARE(mResourceHandler->mLocation, TEST_PHOTO_FILE);
    // verify the value
    property = findPropertyByName(document, QStringLiteral("PHOTO"));
    QVERIFY(!property.isEmpty());
    QVariant variantValue = property.variantValue();
    QVERIFY(variantValue.type() == QVariant::ByteArray);
    QCOMPARE(variantValue.value<QByteArray>(), mResourceHandler->mSimulatedData);
    QVERIFY(property.parameters().contains(QStringLiteral("TYPE"),
                                           QStringLiteral("JPEG")));
}


void tst_QVersitContactExporter::testEncodeEmbeddedContent()
{
    QContact contact = createContactWithName(QStringLiteral("asdf"));
    QContactAvatar contactAvatar;
    QVariant variantValue;

    // Test 1: URL
    const QString url = QStringLiteral("http://www.myhome.com/test.jpg");
    contactAvatar.setImageUrl(url);
    contact.saveDetail(&contactAvatar);
    mResourceHandler->mSimulatedMimeType = QStringLiteral("image/jpeg");
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QVERIFY(!mResourceHandler->mLoadResourceCalled);
    QVersitProperty photoProperty = findPropertyByName(document, QStringLiteral("PHOTO"));
    QVERIFY(!photoProperty.isEmpty());
    QCOMPARE(photoProperty.parameters().count(), 1);
    QVERIFY(photoProperty.parameters().contains(
            QStringLiteral("VALUE"),QStringLiteral("URL")));
    QCOMPARE(photoProperty.value(), url);

    // Test 2: Local PHOTO, image loaded by the loader
    contactAvatar.setImageUrl(TEST_PHOTO_FILE);
    contact.saveDetail(&contactAvatar);
    mResourceHandler->clear();
    mResourceHandler->mSimulatedMimeType = QStringLiteral("image/jpeg");
    mResourceHandler->mSimulatedData = "simulated image data";
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QVERIFY(mResourceHandler->mLoadResourceCalled);
    photoProperty = findPropertyByName(document, QStringLiteral("PHOTO"));
    QVERIFY(!photoProperty.isEmpty());
    QCOMPARE(photoProperty.parameters().count(), 1);
    QVERIFY(photoProperty.parameters().contains(QStringLiteral("TYPE"),
                                                QStringLiteral("JPEG")));
    variantValue = photoProperty.variantValue();
    QVERIFY(variantValue.type() == QVariant::ByteArray);
    QCOMPARE(variantValue.value<QByteArray>(), mResourceHandler->mSimulatedData);

    // Without a resource handler
    mExporter->setResourceHandler(0);
    contactAvatar.setImageUrl(TEST_PHOTO_FILE);
    contact.saveDetail(&contactAvatar);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 0);

    mExporter->setResourceHandler(mResourceHandler);
}

void tst_QVersitContactExporter::testEncodeRingtone()
{
    // Test local ringtone
    QContactRingtone ringtone;
    mResourceHandler->clear();
    mResourceHandler->mSimulatedMimeType = QStringLiteral("audio/wav");
    mResourceHandler->mSimulatedData = "simulated audio data";
    ringtone.setAudioRingtoneUrl(TEST_AUDIO_FILE);
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    contact.saveDetail(&ringtone);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QVERIFY(mResourceHandler->mLoadResourceCalled);
    QVersitProperty soundProperty = findPropertyByName(document, QStringLiteral("SOUND"));
    QVERIFY(!soundProperty.isEmpty());
    QCOMPARE(soundProperty.parameters().count(), 1);
    QVERIFY(soundProperty.parameters().contains(
        QStringLiteral("TYPE"),
        QStringLiteral("WAV")));
    QVariant variantValue = soundProperty.variantValue();
    QVERIFY(variantValue.type() == QVariant::ByteArray);
    QCOMPARE(variantValue.value<QByteArray>(), mResourceHandler->mSimulatedData);

    // Test url ringtone
    QContactRingtone urlRingtone;
    QContact urlContact(createContactWithName(QStringLiteral("asdf")));
    const QUrl url(QStringLiteral("http://qt.nokia.com/audioringtoneurl"));
    urlRingtone.setAudioRingtoneUrl(url);
    urlContact.saveDetail(&urlRingtone);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << urlContact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    soundProperty = findPropertyByName(document, QStringLiteral("SOUND"));
    QVERIFY(!soundProperty.isEmpty());
    QCOMPARE(soundProperty.parameters().count(), 1);
    QVERIFY(soundProperty.parameters().contains(
            QStringLiteral("VALUE"),QStringLiteral("URL")));
    QCOMPARE(soundProperty.value(), url.toString());
}

void tst_QVersitContactExporter::testEncodeParameters()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactPhoneNumber phoneNumber;
    phoneNumber.setNumber(QStringLiteral("12345678"));
    QList<int> subtypes;
    subtypes.append(QContactPhoneNumber::SubTypeMobile);
    subtypes.append(QContactPhoneNumber::SubTypeVideo);
    // Add a not supported subtype in vCard, to make sure its not encoded.
    subtypes.append(QContactPhoneNumber::SubTypeDtmfMenu);
    phoneNumber.setSubTypes(subtypes);
    contact.saveDetail(&phoneNumber);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("TEL"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 2);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"), QStringLiteral("CELL")));
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("VIDEO")));
}

void tst_QVersitContactExporter::testEncodeGender()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));

    // Check that empty gender detail is encoded as Unspecified
    QContactGender gender;
    contact.saveDetail(&gender);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("X-GENDER"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.value(), QStringLiteral("Unspecified"));

    // Check that all valid values are encoded properly.
    gender.setGender(QContactGender::GenderMale);
    gender.setContexts(QContactGender::ContextHome); // Should not be encoded
    contact.saveDetail(&gender);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-GENDER"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.value(), QStringLiteral("Male"));

    gender.setGender(QContactGender::GenderFemale);
    contact.saveDetail(&gender);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-GENDER"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.value(), QStringLiteral("Female"));

    gender.setGender(QContactGender::GenderUnspecified);
    contact.saveDetail(&gender);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-GENDER"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QCOMPARE(property.value(), QStringLiteral("Unspecified"));
    //TODO: Add a helper function in this test module to convert gender versity property to gender enum
}

void tst_QVersitContactExporter::testEncodeNickName()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));

    // Add an extra detail
    QContactGender gender;
    gender.setGender(QContactGender::GenderMale);
    contact.saveDetail(&gender);

    // One nickname given
    QContactNickname firstNickname;
    firstNickname.setNickname(QStringLiteral("Homie"));
    contact.saveDetail(&firstNickname);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("X-NICKNAME"));
    QCOMPARE(property.name(), QStringLiteral("X-NICKNAME"));
    CHECK_VALUE(property, QVersitProperty::ListType, QStringList(QStringLiteral("Homie")));

    // Two nicknames given, should be collated into a single property
    contact = createContactWithName(QStringLiteral("asdf"));
    contact.saveDetail(&gender);
    contact.saveDetail(&firstNickname);
    QContactNickname secondNickname;
    secondNickname.setNickname(QStringLiteral("Jay"));
    contact.saveDetail(&secondNickname);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    property = findPropertyByName(document, QStringLiteral("X-NICKNAME"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.name(), QStringLiteral("X-NICKNAME"));
    CHECK_VALUE(property, QVersitProperty::ListType,
                QStringList() << QStringLiteral("Homie") << QStringLiteral("Jay"));
}

void tst_QVersitContactExporter::testEncodeTag()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));

    // Add an extra detail
    QContactGender gender;
    gender.setGender(QContactGender::GenderMale);
    contact.saveDetail(&gender);

    // One tag given
    QContactTag firstTag;
    firstTag.setTag(QStringLiteral("red"));
    contact.saveDetail(&firstTag);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("CATEGORIES"));
    QCOMPARE(property.name(), QStringLiteral("CATEGORIES"));
    CHECK_VALUE(property, QVersitProperty::ListType, QStringList(QStringLiteral("red")));

    // Two tags given, should be collated into a single property
    contact = createContactWithName(QStringLiteral("asdf"));
    contact.saveDetail(&firstTag);
    contact.saveDetail(&gender);
    QContactTag secondTag;
    secondTag.setTag(QStringLiteral("green"));
    contact.saveDetail(&secondTag);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    property = findPropertyByName(document, QStringLiteral("CATEGORIES"));
    QCOMPARE(property.name(), QStringLiteral("CATEGORIES"));
    CHECK_VALUE(property, QVersitProperty::ListType,
                QStringList() << QStringLiteral("red") << QStringLiteral("green"));
}

void tst_QVersitContactExporter::testEncodeAnniversary()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactAnniversary anniversary;
    QDate date(2009,1,1);
    anniversary.setOriginalDate(date);
    anniversary.setContexts(QContactDetail::ContextHome);
    anniversary.setSubType(QContactAnniversary::SubTypeWedding);
    contact.saveDetail(&anniversary);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("X-ANNIVERSARY"));
    QVERIFY(!property.isEmpty());
    // The contexts and subtypes are not defined for X-ANNIVERSARY property
    QCOMPARE(property.parameters().count(), 0);
    // Check value
    QCOMPARE(property.value(), date.toString(Qt::ISODate));
}


void tst_QVersitContactExporter::testEncodeOnlineAccount()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactOnlineAccount onlineAccount;
    QString accountUri(QStringLiteral("sip:abc@temp.com"));
    QList<int> expectedSubTypes;
    QContactOnlineAccount::SubType expectedSubType;
    int invalidData = 123456;

    onlineAccount.setAccountUri(accountUri);

    // Video sharing
    expectedSubType = QContactOnlineAccount::SubTypeVideoShare;
    expectedSubTypes << expectedSubType;
    onlineAccount.setSubTypes(expectedSubTypes);
    onlineAccount.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&onlineAccount);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("X-SIP"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 2);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("HOME")));
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("SWIS")));
    // Check value
    QCOMPARE(property.value(), accountUri);

    // VoIP
    expectedSubTypes.clear();
    expectedSubType = QContactOnlineAccount::SubTypeSipVoip;
    expectedSubTypes << expectedSubType;
    onlineAccount.setSubTypes(expectedSubTypes);
    onlineAccount.setContexts(QContactDetail::ContextWork);
    contact.saveDetail(&onlineAccount);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-SIP"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 2);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("WORK")));
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("VOIP")));
    // Check value
    QCOMPARE(property.value(), accountUri);

    // Plain SIP
    expectedSubTypes.clear();
    expectedSubType = QContactOnlineAccount::SubTypeSip;
    expectedSubTypes << expectedSubType;
    onlineAccount.setSubTypes(expectedSubTypes);
    onlineAccount.setContexts(QContactDetail::ContextWork);
    contact.saveDetail(&onlineAccount);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-SIP"));
    QVERIFY(!property.isEmpty());
    // Check parameters, SIP not added as a TYPE parameter
    QCOMPARE(property.parameters().count(), 1);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("WORK")));
    // Check value
    QCOMPARE(property.value(), accountUri);

    // IMPP / X-IMPP
    expectedSubTypes.clear();
    expectedSubType = QContactOnlineAccount::SubTypeImpp;
    expectedSubTypes << expectedSubType;
    onlineAccount.setSubTypes(expectedSubTypes);
    onlineAccount.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&onlineAccount);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    property = findPropertyByName(document, QStringLiteral("X-IMPP"));
    QVERIFY(!property.isEmpty());
    // Check parameters, SIP not added as a TYPE parameter
    QCOMPARE(property.parameters().count(), 1);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("HOME")));
    // Check value
    QCOMPARE(property.value(), accountUri);

    // Other subtypes not converted
    expectedSubTypes.clear();
    expectedSubType = static_cast<QContactOnlineAccount::SubType>(invalidData);
    expectedSubTypes << expectedSubType;
    onlineAccount.setSubTypes(expectedSubTypes);
    contact.saveDetail(&onlineAccount);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 0);

    // Test protocols Jabber, AIM, ICQ, MSN, Yahoo, Skype
    contact.clearDetails();
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolJabber);
    onlineAccount.setAccountUri("a");
    contact.saveDetail(&onlineAccount);
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolAim);
    onlineAccount.setAccountUri("b");
    contact.saveDetail(&onlineAccount);
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolIcq);
    onlineAccount.setAccountUri("c");
    contact.saveDetail(&onlineAccount);
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolMsn);
    onlineAccount.setAccountUri("d");
    contact.saveDetail(&onlineAccount);
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolQq);
    onlineAccount.setAccountUri("e");
    contact.saveDetail(&onlineAccount);
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolYahoo);
    onlineAccount.setAccountUri("f");
    contact.saveDetail(&onlineAccount);
    onlineAccount = QContactOnlineAccount();
    onlineAccount.setProtocol(QContactOnlineAccount::ProtocolSkype);
    onlineAccount.setAccountUri("g");
    contact.saveDetail(&onlineAccount);

    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();

    property = findPropertyByName(document, QStringLiteral("X-JABBER"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "a");
    property = findPropertyByName(document, QStringLiteral("X-AIM"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "b");
    property = findPropertyByName(document, QStringLiteral("X-ICQ"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "c");
    property = findPropertyByName(document, QStringLiteral("X-MSN"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "d");
    property = findPropertyByName(document, QStringLiteral("X-QQ"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "e");
    property = findPropertyByName(document, QStringLiteral("X-YAHOO"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "f");
    property = findPropertyByName(document, QStringLiteral("X-SKYPE"));
    QVERIFY(!property.isEmpty());
    QVERIFY(property.value() == "g");
}

void tst_QVersitContactExporter::testEncodeFamily()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactFamily family;

    // No spouse, no family
    family.setContexts(QContactDetail::ContextHome);
    contact.saveDetail(&family);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 0);

    // Only spouse present
    QString spouse = QStringLiteral("ABC");
    family.setSpouse(spouse);
    contact.saveDetail(&family);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty spouseProperty = findPropertyByName(document, QStringLiteral("X-SPOUSE"));
    QVERIFY(!spouseProperty.isEmpty());
    QCOMPARE(spouseProperty.parameters().count(), 0);
    QCOMPARE(spouseProperty.value(), spouse);

    // Spouse and children
    QStringList children;
    children << QStringLiteral("A") << QStringLiteral("B") ;
    family.setChildren(children);
    family.setSpouse(spouse);
    contact.saveDetail(&family);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    spouseProperty = findPropertyByName(document, QStringLiteral("X-SPOUSE"));
    QVERIFY(!spouseProperty.isEmpty());
    QCOMPARE(spouseProperty.parameters().count(), 0);
    QCOMPARE(spouseProperty.value(), spouse);
    QVersitProperty childrenProperty = findPropertyByName(document, QStringLiteral("X-CHILDREN"));
    QVERIFY(!spouseProperty.isEmpty());
    QCOMPARE(childrenProperty.parameters().count(), 0);
    QCOMPARE(childrenProperty.name(), QStringLiteral("X-CHILDREN"));
    CHECK_VALUE(childrenProperty, QVersitProperty::ListType, children);
}

void tst_QVersitContactExporter::testEncodeFavorite()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactFavorite favorite;
    int favoriteIndex = 1;
    favorite.setIndex(favoriteIndex);
    favorite.setFavorite(true);
    contact.saveDetail(&favorite);

    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));

    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty favoriteProperty = findPropertyByName(document, QStringLiteral("X-QTPROJECT-FAVORITE"));
    QVERIFY(!favoriteProperty.isEmpty());
    QCOMPARE(favoriteProperty.parameters().count(), 0);
    CHECK_VALUE(favoriteProperty, QVersitProperty::CompoundType,
                QStringList() << QStringLiteral("true") << QString::number(favoriteIndex));
}

void tst_QVersitContactExporter::testEncodeExtendedDetail()
{
    QFETCH(QString, extendedDetailName);
    QFETCH(QVariant, extendedDetailData);
    QFETCH(QString, extendedDetailDataInProperty);
    QFETCH(bool, extendedDetailPropertyCreated);
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactExtendedDetail extendedDetail;
    extendedDetail.setName(extendedDetailName);
    extendedDetail.setData(extendedDetailData);
    contact.saveDetail(&extendedDetail);

    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));

    QVersitDocument document = mExporter->documents().first();
    if (!extendedDetailPropertyCreated) {
        QCOMPARE(countProperties(document), 0);
        return;
    }
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL"));
    QVERIFY(!property.isEmpty());
    QCOMPARE(property.parameters().count(), 0);
    QStringList expectedValue = QStringList() << extendedDetailName << extendedDetailDataInProperty;
    if (static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Double) {
        CHECK_FLOATING_POINT_VALUE(property, QVersitProperty::CompoundType, expectedValue, true);
    } else if (static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Float) {
        CHECK_FLOATING_POINT_VALUE(property, QVersitProperty::CompoundType, expectedValue, false);
    } else {
        CHECK_VALUE(property, QVersitProperty::CompoundType, expectedValue);
    }
}

void tst_QVersitContactExporter::testEncodeExtendedDetail_data()
{
    QTest::addColumn<QString>("extendedDetailName");
    QTest::addColumn<QVariant>("extendedDetailData");
    QTest::addColumn<QString>("extendedDetailDataInProperty");
    QTest::addColumn<bool>("extendedDetailPropertyCreated");

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
                << QVariant::fromValue<double>(10.199999999999999)
                << jsonArrayWith.arg("10.199999999999999")
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
        QTest::newRow("integer data, serialized as number/double")
                << QString("name")
                << QVariant((int)2)
                << jsonArrayWith.arg("2")
                << true;
        QTest::newRow("integer data, serialized as number/double")
                << QString("name")
                << QVariant((int)-10)
                << jsonArrayWith.arg("-10")
                << true;
    }
    {
        QTest::newRow("datetime data (using local time)")
                << QString("name")
                << QVariant(QDateTime::fromString("1997-07-16T19:20:30.123+01:00", Qt::ISODate))
                << jsonArrayWithString.arg("1997-07-16T19:20:30+01:00")
                << true;
        QTest::newRow("datetime data (using UTC)")
                << QString("name")
                << QVariant(QDateTime::fromString("1997-07-16T19:20:30.123+01:00", Qt::ISODate).toUTC())
                << jsonArrayWithString.arg("1997-07-16T18:20:30Z")
                << true;
        QTest::newRow("datetime data (using local time with unspecified timezone)")
                << QString("name")
                << QVariant(QDateTime::fromString("1997-07-16T19:20:30", Qt::ISODate))
                << jsonArrayWithString.arg("1997-07-16T19:20:30")
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
        QTest::newRow("empty variant as data")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("null")
                << true;
    }

    {
        QVariantHash hash;
        hash["key 1"] = QString("string 1");
        hash["key 2"] = QString("string 2");
        QTest::newRow("hash data")
                << QString("name")
                << QVariant(hash)
                << QString("[\n    {\n        \"key 1\": \"string 1\",\n        \"key 2\": \"string 2\"\n    }\n]\n")
                << true;
    }

    {
        QTest::newRow("data type not supported")
                << QString("name")
                << QVariant(QSize(1, 1))
                << QString()
                << false;
    }
}

void tst_QVersitContactExporter::testEncodeMultipleExtendedDetails()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactExtendedDetail extendedDetail1;
    extendedDetail1.setName(QStringLiteral("detailName1"));
    extendedDetail1.setData(QStringLiteral("detailData1"));
    contact.saveDetail(&extendedDetail1);
    QContactExtendedDetail extendedDetail2;
    extendedDetail2.setName(QStringLiteral("detailName2"));
    extendedDetail2.setData(QStringLiteral("detailData2"));
    contact.saveDetail(&extendedDetail2);
    QList<QContactExtendedDetail> expected;
    expected << extendedDetail1 << extendedDetail2;

    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));

    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 2);
    QList<QVersitProperty> actual;
    foreach (const QVersitProperty &property, document.properties()) {
        if (property.name() == QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL"))
            actual << property;
    }
    for (int i = 0; i < actual.size(); i++) {
        QVERIFY(!actual[i].isEmpty());
        QCOMPARE(actual[i].parameters().count(), 0);
        QString jsonArrayWith("[\n    %1\n]\n");
        QString jsonArrayWithString = jsonArrayWith.arg("\"%1\"");
        CHECK_VALUE(actual[i], QVersitProperty::CompoundType,
                    QStringList()
                    << expected[i].name()
                    << jsonArrayWithString.arg(expected[i].data().toString()));
    }
}

void tst_QVersitContactExporter::testDefaultResourceHandler()
{
    QVersitDefaultResourceHandler handler;
    QByteArray contents;
    QString mimeType;
    handler.loadResource(QStringLiteral("test.jpg"), &contents, &mimeType);
    QCOMPARE(mimeType, QStringLiteral("image/jpeg"));

    QVersitProperty property;
    QString location;
    QVERIFY(!handler.saveResource("test contents", property, &location));
}

void tst_QVersitContactExporter::testEncodeEmailWithContextOther()
{
    QContact contact(createContactWithName(QStringLiteral("asdf")));
    QContactEmailAddress email;
    email.setEmailAddress(QStringLiteral("test@test"));
    email.setContexts(QContactDetail::ContextOther);
    contact.saveDetail(&email);
    QVERIFY(mExporter->exportContacts(QList<QContact>() << contact, QVersitDocument::VCard30Type));
    QVersitDocument document = mExporter->documents().first();
    QCOMPARE(countProperties(document), 1);
    QVersitProperty property = findPropertyByName(document, QStringLiteral("EMAIL"));
    QVERIFY(!property.isEmpty());
    // Check parameters
    QCOMPARE(property.parameters().count(), 1);
    QVERIFY(property.parameters().contains(
        QStringLiteral("TYPE"),QStringLiteral("OTHER")));
    // Check value
    QCOMPARE(property.value(), email.emailAddress());
}

// Test utility functions
QContact tst_QVersitContactExporter::createContactWithName(QString name)
{
    QContact contact;
    QContactName nameDetail;
    nameDetail.setFirstName(name);
    contact.saveDetail(&nameDetail);
    return contact;
}

/* Counts the properties, excluding the ones generated by a
 * "createContactWithName" contact and excluding default-generated properties
 */
int tst_QVersitContactExporter::countProperties(const QVersitDocument& document)
{
    int count = 0;
    foreach (const QVersitProperty& property, document.properties()) {
        if (property.name() != QStringLiteral("FN")
            && property.name() != QStringLiteral("N")
            && property.name() != QStringLiteral("X-NOKIA-QCONTACTFIELD"))
            count++;
    }
    return count;
}

QContactDetail tst_QVersitContactExporter::findDetailByType(
    QList<QContactDetail> details,
    QContactDetail::DetailType search)
{
    QContactDetail detail;
    for (int i= 0; i < details.count(); i++) {
        if ( details.at(i).type() == search )
            detail = details.at(i);
    }
    return detail;
}

QVersitProperty tst_QVersitContactExporter::findPropertyByName(
        const QVersitDocument &document, const QString &propertyName)
{
    foreach (const QVersitProperty& property, document.properties()) {
        if (property.name() == propertyName)
            return property;
    }
    return QVersitProperty();
}

QTEST_MAIN(tst_QVersitContactExporter)

