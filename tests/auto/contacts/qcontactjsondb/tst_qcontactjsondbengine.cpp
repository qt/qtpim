/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtCore/QString>
#include <QtTest/QtTest>
#include "qcontactjsondbengine.h"
#include "qcontactjsondbenginefactory.h"
#include "qcontactjsondbbackup.h"
#include "qcontactjsondbglobal.h"
#include "qcontactjsondbconverter.h"
#include "synchronizedjsondbclient.h"
#include "jsondbprocess-oldapi.h"

QTCONTACTS_USE_NAMESPACE

class tst_QContactJsondbEngine : public QObject
{
    Q_OBJECT

public:
    tst_QContactJsondbEngine();
    ~tst_QContactJsondbEngine();
public slots:
    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

private Q_SLOTS:
    void testSelfContactId();
    void testContactIds();
    void testContacts();
    void testContacts_data();
    void testContact();
    void testContact_data();
    void testSaveContact();
    void testSaveContacts();
    void testRemoveContacts();
    void testRemoveContact();
    void testContactDetailFilter();
    void testContactDetailFilter_data();
    void testContactSortOrder();
    void testContactSortOrder_data();
    void testContactUpdate();
    void testExtendedDetailsFromJsonDb();
    void testExtendedDetailsToJsonDb();

private:
    QContactJsonDbEngine* m_engine;
    QContactJsonDbBackup* m_myBackup;
    SynchronizedJsonDbClient* m_dbClient;
    JsonDbProcess m_jsondbProcess;
};

void tst_QContactJsondbEngine::initTestCase()
{
    QVERIFY2(m_jsondbProcess.start(), "Failed to start JsonDb process");
}

void tst_QContactJsondbEngine::init()
{
    // clears the database and loads desired test data
    m_myBackup = new QContactJsonDbBackup;
    m_myBackup->loadTestData(); // TODO: Proper fetching for test data.
}

void tst_QContactJsondbEngine::cleanup()
{
    if (m_myBackup) {
        m_myBackup->cleanJsonDb();
        delete m_myBackup;
    }
}

void tst_QContactJsondbEngine::cleanupTestCase()
{
    m_jsondbProcess.terminate();
}

tst_QContactJsondbEngine::tst_QContactJsondbEngine()
{
    m_engine = new QContactJsonDbEngine();
    m_dbClient = new SynchronizedJsonDbClient();
}

tst_QContactJsondbEngine::~tst_QContactJsondbEngine()
{
    if(m_engine) {
        delete m_engine;
        m_engine = NULL;
    }
    if (m_dbClient) {
        delete m_dbClient;
        m_dbClient = NULL;
    }
}

void tst_QContactJsondbEngine::testSelfContactId() {
    qDebug() << "NOTE: THE IDENTIFICATION FIELD DOES NOT EXIST YET IN JSON SCHEMA, just returns \"not found\"";
    QContactManager myContactManager;
    QContactId myId;
    myId = myContactManager.selfContactId();
    qDebug() << "MyId is:" << myId;
}

void tst_QContactJsondbEngine::testContactIds() {
    QContactManager myContactManager;
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContactId> resultContactIds = myContactManager.contactIds(filter, sortOrders);

    QList<QContact> contacts = myContactManager.contacts(filter, sortOrders); // used as reference. Not possible to use a table here because the IDs change every time the test is triggered...
    QList<QContactId> referenceContactIds;
    foreach (QContact contact, contacts) {
        referenceContactIds.append(contact.id());
    }
    QCOMPARE(resultContactIds.size(), referenceContactIds.size());
    for (int i = 0; i < resultContactIds.size(); i++) {
        QVERIFY(resultContactIds.at(i) == referenceContactIds.at(i));
    }
}

void tst_QContactJsondbEngine::testContacts() {
    QContactManager cm;

    // "all contacts" retrieval
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(filter, sortOrders);
    QCOMPARE(contacts.size(), 5);

    QFETCH(QString, expectedContact1FirstName);
    QFETCH(QString, expectedContact1LastName);
    QFETCH(QString, expectedContact2FirstName);
    QFETCH(QString, expectedContact2LastName);
    QFETCH(QString, expectedContact3FirstName);
    QFETCH(QString, expectedContact3LastName);
    QFETCH(QString, expectedContact4FirstName);
    QFETCH(QString, expectedContact4LastName);
    QFETCH(QString, expectedContact5FirstName);
    QFETCH(QString, expectedContact5LastName);
    QList<QString> expectedContacts;
    expectedContacts << expectedContact1FirstName << expectedContact1LastName << expectedContact2FirstName << expectedContact2LastName << expectedContact3FirstName << expectedContact3LastName << expectedContact4FirstName << expectedContact4LastName << expectedContact5FirstName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 10);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if ( (firstNameString == expectedContacts.at(j)) && (lastNameString == expectedContacts.at(j+1)) ) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }
}

void tst_QContactJsondbEngine::testContacts_data() {
    QTest::addColumn<QString>("expectedContact1FirstName");
    QTest::addColumn<QString>("expectedContact1LastName");
    QTest::addColumn<QString>("expectedContact2FirstName");
    QTest::addColumn<QString>("expectedContact2LastName");
    QTest::addColumn<QString>("expectedContact3FirstName");
    QTest::addColumn<QString>("expectedContact3LastName");
    QTest::addColumn<QString>("expectedContact4FirstName");
    QTest::addColumn<QString>("expectedContact4LastName");
    QTest::addColumn<QString>("expectedContact5FirstName");
    QTest::addColumn<QString>("expectedContact5LastName");
    {
        QTest::newRow("expectedContactsNames")  << "Harry" << "Baker" << "Paul" << "Thomson" << "Julie" << "King" << "Angelina" << "Row" << "Natalie" << "Watson";
    }
}

void tst_QContactJsondbEngine::testContact() {
    QContactManager cm;

    // contact retrieval
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContactId> resultContactIds = cm.contactIds(filter, sortOrders);
    QVERIFY(resultContactIds.size() > 0);
    QContactId id = resultContactIds.at(1); // Just a random ID picked
    QContact contact = cm.contact(id);
    QVERIFY(id == contact.id());

    QFETCH(QString, expectedContact1FirstName);
    QFETCH(QString, expectedContact1LastName);
    QFETCH(QString, expectedContact2FirstName);
    QFETCH(QString, expectedContact2LastName);
    QFETCH(QString, expectedContact3FirstName);
    QFETCH(QString, expectedContact3LastName);
    QFETCH(QString, expectedContact4FirstName);
    QFETCH(QString, expectedContact4LastName);
    QFETCH(QString, expectedContact5FirstName);
    QFETCH(QString, expectedContact5LastName);
    QList<QString> expectedContacts;
    expectedContacts << expectedContact1FirstName << expectedContact1LastName << expectedContact2FirstName << expectedContact2LastName << expectedContact3FirstName << expectedContact3LastName << expectedContact4FirstName << expectedContact4LastName << expectedContact5FirstName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 10);
        bool contactFound = false;
        QString firstNameString = contact.detail<QContactName>().firstName();
        QString lastNameString = contact.detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if ( (firstNameString == expectedContacts.at(j)) && (lastNameString == expectedContacts.at(j+1)) ) contactFound=true;
        }
        QCOMPARE(contactFound, true);
}

void tst_QContactJsondbEngine::testContact_data() {
    QTest::addColumn<QString>("expectedContact1FirstName");
    QTest::addColumn<QString>("expectedContact1LastName");
    QTest::addColumn<QString>("expectedContact2FirstName");
    QTest::addColumn<QString>("expectedContact2LastName");
    QTest::addColumn<QString>("expectedContact3FirstName");
    QTest::addColumn<QString>("expectedContact3LastName");
    QTest::addColumn<QString>("expectedContact4FirstName");
    QTest::addColumn<QString>("expectedContact4LastName");
    QTest::addColumn<QString>("expectedContact5FirstName");
    QTest::addColumn<QString>("expectedContact5LastName");
    {
        QTest::newRow("expectedContactsNames")  << "Harry" << "Baker" << "Paul" << "Thomson" << "Julie" << "King" << "Angelina" << "Row" << "Natalie" << "Watson";
    }
}

void tst_QContactJsondbEngine::testContactUpdate() {

    // Fetch existing contacts.
    QContactManager cm;
    QContactIdFilter idFilter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(idFilter, sortOrders);
    QVERIFY(contacts.size() > 0);

    // Take first of them as test contact for update test.
    QContact testContact = contacts[0];
    idFilter.add(testContact.id());
    QContactFetchRequest fetchRequest;
    fetchRequest.setFilter(idFilter);

    // Convert QContact to JsonDb contact and insert extra fields to json contact.
    QJsonObject jsonContact;
    QContactJsonDbConverter jsonDbConverter;
    int extraFieldInContact = 3;
    int extraFieldInContactDetails = 5;
    int extraFieldInContactEmails = 8;
    if (jsonDbConverter.toJsonContact(&jsonContact, testContact)) {
        jsonContact.insert("extra3", extraFieldInContact);

        QJsonObject embeddedDetails;
        embeddedDetails["extra5"] = extraFieldInContactDetails;
        jsonContact.insert("details", embeddedDetails);

        QJsonObject embeddedEmail;
        embeddedEmail["extra8"] = extraFieldInContactEmails;
        jsonContact.insert("emails", embeddedEmail);
    }
    // Save test contact with extra fields directly to the jsondb.
    QVariantMap jsonResponse = m_dbClient->update(jsonContact).first().toVariantMap();

    // Update the test contact through the QContacts API with some details.
    QContactName nameDetail = testContact.detail<QContactName>();
    nameDetail.setFirstName("Kary");
    nameDetail.setLastName("Krant");
    testContact.saveDetail(&nameDetail);

    QContactEmailAddress emailDetail;
    testContact.removeDetail(&emailDetail);
    emailDetail.setEmailAddress("Kary.Krant@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    QContactPhoneNumber phoneNumberDetail;
    testContact.removeDetail(&phoneNumberDetail);

    QList<int> currSubtype;
    currSubtype << QContactPhoneNumber::SubTypeMobile;
    phoneNumberDetail.setSubTypes(currSubtype);
    phoneNumberDetail.setNumber("+358507654322");
    testContact.saveDetail(&phoneNumberDetail);
    cm.synthesizeContactDisplayLabel(&testContact);
    cm.saveContact(&testContact);

    // TODO: Check the updated fields are properly there.
    QList<QContact> contactsNow = cm.contacts(idFilter, sortOrders);
    foreach (QContact cntc, contactsNow ) {
        if (cntc.id() == testContact.id()) {
           }
    }

    // Fetch directly from jsondb the test contact data.
    QString finalQuery;
    QVERIFY(jsonDbConverter.queryFromRequest(&fetchRequest, finalQuery));
    QVariantMap finalJsonContact = m_dbClient->query(finalQuery).first().toVariantMap();
    QVERIFY(!finalJsonContact.isEmpty());

    QVariantMap embeddedDetailsMap = finalJsonContact["details"].value<QVariantMap>();
    QVariantMap embeddedEmailMap = finalJsonContact["emails"].value<QVariantMap>();

    // Check that update did not change extra fields in contact object.
    QCOMPARE(finalJsonContact.value("extra3").toInt(),extraFieldInContact);
    // Check that update did not change extra fields in contact embededed details object.
    QCOMPARE(embeddedDetailsMap.value("extra5").toInt(),extraFieldInContactDetails);
    // We do not preserve extra fields for other embedded objects like emails.
    QCOMPARE(embeddedEmailMap.value("extra8"), QVariant());
}


void tst_QContactJsondbEngine::testExtendedDetailsFromJsonDb() {

    // Fetch existing contacts.
    QContactManager cm;
    QContactIdFilter idFilter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(idFilter, sortOrders);
    QVERIFY(contacts.size() > 0);

    // Take first of them as test contact.
    QContact testContact = contacts[0];
    idFilter.add(testContact.id());

    // Convert QContact to JsonDb contact and insert unknown properties to json contact.
    QJsonObject jsonContact;
    QContactJsonDbConverter jsonDbConverter;
    int extraFieldInContactDetails = 5;

    QVERIFY(jsonDbConverter.toJsonContact(&jsonContact, testContact));
    jsonContact.insert("simpleStringDetail", QJsonValue::fromVariant("Simple string as detail data."));

    QVariantMap variantMap;
    variantMap.insert("MapItemInt", 1);
    variantMap.insert("MapItemQString", "QStringData");
    variantMap.insert("MapItemQVariant", QVariant( QString("QStringInQVariant")));
    variantMap.insert("MapItemQVariantList", QVariantList() << QString("QStringInVariantList") << 5 );

    QJsonObject jsonData;
    jsonData.insert("QStringItem", QJsonValue::fromVariant("Content for QStringItem."));
    jsonData.insert("MapInMap", QJsonObject::fromVariantMap(variantMap));
    jsonContact.insert("complexVariantMapDetail", jsonData);
    jsonContact.insert("anotherSimilarComplexVariantMapDetail", jsonData);

    QJsonObject embeddedDetails;
    embeddedDetails["extra5"] = extraFieldInContactDetails;
    jsonContact.insert("details", embeddedDetails);

    // Save test contact with unknown properties directly to the jsondb.
    QVariantMap jsonResponse = m_dbClient->update(jsonContact).first().toVariantMap();

    // Check that the unknown properties come up as extended details.
    QList<QContact> contactsNow = cm.contacts(idFilter, sortOrders);
    foreach (QContact contact, contactsNow ) {
        if (contact.id() == testContact.id()) {
            QList<QContactExtendedDetail> extendedDetails = contact.details<QContactExtendedDetail>();
            QCOMPARE(extendedDetails.size(), 3);
            for (int i = 0; i < extendedDetails.size(); ++i) {
                if (extendedDetails.at(i).name() == "simpleStringDetail") {
                    QCOMPARE(extendedDetails[i].data().toString(), QString("Simple string as detail data."));
                }
                if (extendedDetails.at(i).name() == "complexVariantMapDetail") {
                    QVariantMap extendedDetailItems = extendedDetails[i].data().toMap();
                    QCOMPARE(extendedDetailItems.value("QStringItem").toString(), QString("Content for QStringItem."));
                    QVariantMap returnMap = extendedDetailItems.value("MapInMap").toMap();
                    QCOMPARE(returnMap.value("MapItemInt").toInt(), 1);
                    QCOMPARE(returnMap.value("MapItemQString").toString(), QString("QStringData"));
                    QCOMPARE(returnMap.value("MapItemQVariant").toString(), QString("QStringInQVariant"));
                    QCOMPARE(returnMap.value("MapItemQVariantList").toList().at(0).toString(), QString("QStringInVariantList"));
                    QCOMPARE(returnMap.value("MapItemQVariantList").toList().at(1).toInt(), 5);
                }
                if (extendedDetails.at(i).name() == "anotherSimilarComplexVariantMapDetail") {
                    QVariantMap extendedDetailItems = extendedDetails[i].data().toMap();
                    QCOMPARE(extendedDetailItems.value("QStringItem").toString(), QString("Content for QStringItem."));
                    QVariantMap returnMap = extendedDetailItems.value("MapInMap").toMap();
                    QCOMPARE(returnMap.value("MapItemInt").toInt(), 1);
                    QCOMPARE(returnMap.value("MapItemQString").toString(), QString("QStringData"));
                    QCOMPARE(returnMap.value("MapItemQVariant").toString(), QString("QStringInQVariant"));
                    QCOMPARE(returnMap.value("MapItemQVariantList").toList().at(0).toString(), QString("QStringInVariantList"));
                    QCOMPARE(returnMap.value("MapItemQVariantList").toList().at(1).toInt(), 5);
                }
            }
        }
    }
}

void tst_QContactJsondbEngine::testExtendedDetailsToJsonDb() {

    // Fetch existing contacts.
    QContactManager cm;
    QContactIdFilter idFilter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(idFilter, sortOrders);
    QVERIFY(contacts.size() > 0);

    // Take first of them as test contact for update test.
    QContact testContact = contacts[0];
    idFilter.add(testContact.id());
    QContactFetchRequest fetchRequest;
    fetchRequest.setFilter(idFilter);

    // Add simple string as extended detail.
    QContactExtendedDetail extendedDetail;
    extendedDetail.setName("extendedDetailQString");
    extendedDetail.setData(QString("Simple QString as extended detail."));
    testContact.saveDetail(&extendedDetail);

    // Add QVariantList as extended detail.
    QContactExtendedDetail variantListDetail;
    variantListDetail.setName("extendedDetailQVariantList");
    variantListDetail.setData(QVariantList() << QString("QString in the QVariantlist detail.")
                              << QVariant(1)
                              << QString("Another QString in the QVariantlist detail.")
                              << 2);
    testContact.saveDetail(&variantListDetail);

    // Add exteded details with a complex variantmap data in to test contact and save it.
    QContactExtendedDetail complexExtendedDetail;
    complexExtendedDetail.setName("complexExtendedDetail");
    QVariantMap variantMap;
    variantMap.insert("mapItemInt", 3);
    variantMap.insert("mapItemQString", QString("QString item in QVariantMap"));
    variantMap.insert("mapItemVariant", QVariant(QString("QString item as QVariant in QVariantMap")));
    variantMap.insert("mapItemVariantList", QVariantList()
                      << QString("QString item in QVariantList in QVarianMap")
                      << QVariant(4)
                      << QString("Another QString item in QVariantList in QVariantMap"));
    complexExtendedDetail.setData(variantMap);
    testContact.saveDetail(&complexExtendedDetail);
    cm.saveContact(&testContact);

    //Fetch saved test contact data directly from jsondb.
    QContactJsonDbConverter jsonDbConverter;
    QString finalQuery;
    QVERIFY(jsonDbConverter.queryFromRequest(&fetchRequest, finalQuery));
    QVariantMap jsonContact = m_dbClient->query(finalQuery).first().toVariantMap();

    // Check simple extended detail got correctly to jsondb.
    QCOMPARE(jsonContact["extendedDetailQString"].value<QVariant>().toString(),QString("Simple QString as extended detail."));

    // Check variantlist exteded details got correctly to jsondb.
    QVariantList variantList = jsonContact["extendedDetailQVariantList"].value<QVariant>().toList();
    QCOMPARE(variantList.size(), 4);
    QCOMPARE( variantList[0].toString(), QString("QString in the QVariantlist detail."));
    QCOMPARE( variantList[1].toInt(), 1);
    QCOMPARE( variantList[2].toString(), QString("Another QString in the QVariantlist detail."));
    QCOMPARE( variantList[3].toInt(),2);

    // Check complex extended detail field got correctly to jsondb.
    QVariantMap convertedVariantMap = jsonContact["complexExtendedDetail"].value<QVariant>().toMap();
    QCOMPARE( convertedVariantMap["mapItemInt"].toInt(), 3);
    QCOMPARE( convertedVariantMap["mapItemQString"].toString(),QString("QString item in QVariantMap"));
    QCOMPARE( convertedVariantMap["mapItemVariant"].toString(),QString("QString item as QVariant in QVariantMap"));
    variantList = convertedVariantMap["mapItemVariantList"].toList();
    QCOMPARE(variantList.size(), 3);
    QCOMPARE( variantList[0].toString(), QString("QString item in QVariantList in QVarianMap"));
    QCOMPARE( variantList[1].toInt(), 4);
    QCOMPARE( variantList[2].toString(), QString("Another QString item in QVariantList in QVariantMap"));
}


void tst_QContactJsondbEngine::testSaveContact() {
    QContactManager cm;

    // save a new contact
    int originalCount = cm.contactIds().size();
    QContact testContact;

    QContactName nameDetail;
    nameDetail.setFirstName("Test Contact");
    testContact.saveDetail(&nameDetail);

    QContactPersonId personIdDetail;
    personIdDetail.setPersonId("1234567890123456");
    testContact.saveDetail(&personIdDetail);

    // testing
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(filter, sortOrders);
    bool returnValue = cm.saveContact(&testContact);
    QVERIFY(returnValue == true);

    QContactId testContactId = testContact.id();
    QContact expected = cm.contact(testContactId);

    QCOMPARE(cm.contactIds().size(), originalCount + 1);
    QCOMPARE(testContact.id(), expected.id());
    QCOMPARE(testContact.detail(QContactName::Type), expected.detail(QContactName::Type));
    QCOMPARE(testContact.detail(QContactPersonId::Type), expected.detail(QContactPersonId::Type));
}

void tst_QContactJsondbEngine::testSaveContacts() {
    QContactManager cm;

    // save new contacts
    int originalCount = cm.contactIds().size();
    QContact testContact;
    QContactName nameDetail;
    nameDetail.setFirstName("Test Contact 1");
    testContact.saveDetail(&nameDetail);
    QList<QContact> saveList;
    saveList << testContact;

    QContact testContact2;
    QContactName nameDetail2;
    nameDetail2.setFirstName("Test Contact 2");
    testContact2.saveDetail(&nameDetail2);
    saveList << testContact2;

    // dummy contact that will not be added
    QContact testContact3;
    QContactName nameDetail3;
    nameDetail3.setFirstName("Test Contact 3");
    testContact3.saveDetail(&nameDetail3);

    // testing
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> initialContacts = cm.contacts(filter, sortOrders);
    bool returnValue = cm.saveContacts(&saveList);
    QVERIFY(returnValue == true);
    QList<QContact> contacts = cm.contacts(filter, sortOrders);

    bool contact1Found = false;
    bool contact2Found = false;
    foreach (QContact curr, contacts) {
        if (curr.detail<QContactName>() == nameDetail) contact1Found=true;
        if (curr.detail<QContactName>() == nameDetail2) contact2Found=true;
    }
    QVERIFY(contact1Found == true);
    QVERIFY(contact2Found == true);
    QCOMPARE(cm.contactIds().size(), originalCount + 2);
}

void tst_QContactJsondbEngine::testContactDetailFilter() {
    QContactManager myContactManager;
    QList<QContactSortOrder> sortOrders;

    //                ------------  MATCH FLAG = CONTAINS  ------------

    // retrieve contacts filtering by first name
    QString firstName = "li";
    QContactDetailFilter dfil;
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchContains);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    QList<QContact> contacts = myContactManager.contacts(dfil, sortOrders);
    QCOMPARE(contacts.size(), 3);

    QFETCH(QString, expectedContact3FirstName);
    QFETCH(QString, expectedContact4FirstName);
    QFETCH(QString, expectedContact5FirstName);
    QList<QString> expectedContacts;
    expectedContacts << expectedContact3FirstName << expectedContact4FirstName << expectedContact5FirstName;
    QVERIFY(expectedContacts.size() == 3);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (firstNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by Last name
    QString lastName = "o";
    dfil.setDetailType(QContactName::Type, QContactName::FieldLastName);
    dfil.setValue(lastName);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 3);
    QFETCH(QString, expectedContact2LastName);
    QFETCH(QString, expectedContact4LastName);
    QFETCH(QString, expectedContact5LastName);
    expectedContacts.clear();
    expectedContacts << expectedContact2LastName << expectedContact4LastName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 3);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (lastNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by phone number
    QString phoneNr = "         6543HHHH            HHHHHHHH";
    //The number above will be sanitized by jsondb contacts plugin
    //Hence, the filter will look for contacts matching the
    //sanitized version of phoneNr, i.e., "6543"
    dfil.setDetailType(QContactPhoneNumber::Type, QContactPhoneNumber::FieldNumber);
    dfil.setValue(phoneNr);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QCOMPARE(contacts.size(), 3);
    QFETCH(QString, expectedContact1LastName);  // Contacts 2 and 5 were fetched already
    expectedContacts.clear();
    expectedContacts << expectedContact1LastName << expectedContact2LastName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 3);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (lastNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by email
    QString email = "on";
    dfil.setDetailType(QContactEmailAddress::Type, QContactEmailAddress::FieldEmailAddress);
    dfil.setValue(email);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 2);
    expectedContacts.clear();
    QFETCH(QString, expectedContact2EmailAddress);
    QFETCH(QString, expectedContact5EmailAddress);
    expectedContacts << expectedContact2EmailAddress << expectedContact5EmailAddress;
    QVERIFY(expectedContacts.size() == 2);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        QString emailAddress = "mailto:";
        emailAddress.append(firstNameString + "." + lastNameString + "@ovi.com");
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (emailAddress == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    //                ------------  MATCH FLAG = MATCH EXACTLY  ------------

    // retrieve contacts filtering by first name
    firstName = "Julie";
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchExactly);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchExactly);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 1);
    expectedContacts.clear();
    expectedContacts << expectedContact3FirstName;
    QVERIFY(expectedContacts.size() == 1);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (firstNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by Last name
    lastName = "Thomson";
    dfil.setDetailType(QContactName::Type, QContactName::FieldLastName);
    dfil.setValue(lastName);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchExactly);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 1);
    expectedContacts.clear();
    expectedContacts << expectedContact2LastName;
    QVERIFY(expectedContacts.size() == 1);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (lastNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by phone number
    phoneNr = "+358507654321";
    dfil.setDetailType(QContactPhoneNumber::Type, QContactPhoneNumber::FieldNumber);
    dfil.setValue(phoneNr);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchExactly);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 3);
    expectedContacts.clear();
    expectedContacts << expectedContact1LastName << expectedContact2LastName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 3);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (lastNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by email
    email = "Angelina.Row@ovi.com";
    dfil.setDetailType(QContactEmailAddress::Type, QContactEmailAddress::FieldEmailAddress);
    dfil.setValue(email);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchExactly);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 1);
    expectedContacts.clear();
    QFETCH(QString, expectedContact4EmailAddress);
    expectedContacts.clear();
    expectedContacts << expectedContact4EmailAddress;
    QVERIFY(expectedContacts.size() == 1);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        QString emailAddress = "mailto:";
        emailAddress.append(firstNameString + "." + lastNameString + "@ovi.com");
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (emailAddress == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    // retrieve contacts filtering by local ID
    QContactIdFilter idf;
    QVERIFY(idf.type() == QContactFilter::IdFilter);
    QVERIFY(idf.ids().count() == 0);
    QList<QContactId> ids = myContactManager.contactIds(idf, sortOrders);

    // Retrieve only the first ID
    QContactId firstId = ids.first();
    QList<QContactId> shortList;
    shortList << firstId;
    idf.setIds(shortList);
    QVERIFY(idf.ids() == shortList);

    QContact contact = myContactManager.contact(firstId);
    QVERIFY(firstId == contact.id());
    contacts = myContactManager.contacts(idf, sortOrders);
    QVERIFY(contacts.size() == 1);
    QFETCH(QString, expectedContact1FirstName);
    QFETCH(QString, expectedContact2FirstName);
    QFETCH(QString, expectedContact3LastName);
    expectedContacts.clear();
    expectedContacts << expectedContact1FirstName << expectedContact1LastName << expectedContact2FirstName << expectedContact2LastName << expectedContact3FirstName << expectedContact3LastName << expectedContact4FirstName << expectedContact4LastName << expectedContact5FirstName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 10);
        bool contactFound = false;
        QString firstNameString = contact.detail<QContactName>().firstName();
        QString lastNameString = contact.detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if ( (firstNameString == expectedContacts.at(j)) && (lastNameString == expectedContacts.at(j+1)) ) contactFound=true;
        }
        QCOMPARE(contactFound, true);

    // Retrieve multiple IDs
    idf.setIds(ids);
    QVERIFY(idf.ids() == ids);
    contacts = myContactManager.contacts(idf, sortOrders);
    QVERIFY(contacts.size() == 5);
    expectedContacts.clear();
    expectedContacts << expectedContact1FirstName << expectedContact1LastName << expectedContact2FirstName << expectedContact2LastName << expectedContact3FirstName << expectedContact3LastName << expectedContact4FirstName << expectedContact4LastName << expectedContact5FirstName << expectedContact5LastName;
    QVERIFY(expectedContacts.size() == 10);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        QString lastNameString = contacts.at(i).detail<QContactName>().lastName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if ( (firstNameString == expectedContacts.at(j)) && (lastNameString == expectedContacts.at(j+1)) ) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    //                ------------  MATCH FLAG = MatchStartsWith  ------------

    // retrieve contacts filtering by first name
    firstName = "Jul";
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchStartsWith);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchStartsWith);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 1);
    expectedContacts.clear();
    expectedContacts << expectedContact3FirstName;
    QVERIFY(expectedContacts.size() == 1);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (firstNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }

    //                ------------  MATCH FLAG = MatchEndsWith  ------------

    // retrieve contacts filtering by first name
    firstName = "ie";
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchEndsWith);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchEndsWith);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 2);
    expectedContacts.clear();
    expectedContacts << expectedContact3FirstName  << expectedContact5FirstName;
    QVERIFY(expectedContacts.size() == 2);
    for (int i = 0; i < contacts.size(); i++) {
        bool contactFound = false;
        QString firstNameString = contacts.at(i).detail<QContactName>().firstName();
        for (int j = 0; j < expectedContacts.size(); j++) {
            if (firstNameString == expectedContacts.at(j)) contactFound=true;
        }
        QCOMPARE(contactFound, true);
    }
}

void tst_QContactJsondbEngine::testContactDetailFilter_data() {
    QTest::addColumn<QString>("expectedContact1FirstName");
    QTest::addColumn<QString>("expectedContact1LastName");
    QTest::addColumn<QString>("expectedContact2FirstName");
    QTest::addColumn<QString>("expectedContact2LastName");
    QTest::addColumn<QString>("expectedContact3FirstName");
    QTest::addColumn<QString>("expectedContact3LastName");
    QTest::addColumn<QString>("expectedContact4FirstName");
    QTest::addColumn<QString>("expectedContact4LastName");
    QTest::addColumn<QString>("expectedContact5FirstName");
    QTest::addColumn<QString>("expectedContact5LastName");
    QTest::addColumn<QString>("expectedContact2EmailAddress");
    QTest::addColumn<QString>("expectedContact4EmailAddress");
    QTest::addColumn<QString>("expectedContact5EmailAddress");
    {
        QTest::newRow("expectedContactsNames")  << "Harry" << "Baker" << "Paul" << "Thomson" << "Julie" << "King" << "Angelina" << "Row" << "Natalie" << "Watson" << "mailto:Paul.Thomson@ovi.com" << "mailto:Angelina.Row@ovi.com" << "mailto:Natalie.Watson@ovi.com";
    }
}

void tst_QContactJsondbEngine::testContactSortOrder() {
    QContactManager myContactManager;

    // sort order
    QContactSortOrder sortOrder;
    sortOrder.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    QList<QContactSortOrder> sortOrders;
    sortOrders.append(sortOrder);

    // retrieve all the contacts and sort first names alphabetically
    QContactFilter filter;
    QList<QContact> contacts = myContactManager.contacts(filter, sortOrders);
    QCOMPARE(contacts.size(), 5);

    QFETCH(QString, expectedContact1FirstName);
    QFETCH(QString, expectedContact2FirstName);
    QFETCH(QString, expectedContact3FirstName);
    QFETCH(QString, expectedContact4FirstName);
    QFETCH(QString, expectedContact5FirstName);
    QList<QString> expectedContacts;
    expectedContacts << expectedContact4FirstName << expectedContact1FirstName << expectedContact3FirstName << expectedContact5FirstName << expectedContact2FirstName;
    QCOMPARE(expectedContacts.size(), 5);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) expected ordered contacts: " << expectedContacts;
    for (int i = 0; i < contacts.size(); i++) {
        QVERIFY (contacts.at(i).detail<QContactName>().firstName() == expectedContacts.at(i));
    }

    // now sort last names alphabetically
    sortOrder.setDetailType(QContactName::Type, QContactName::FieldLastName);
    sortOrders.clear();
    sortOrders.append(sortOrder);
    contacts = myContactManager.contacts(filter, sortOrders);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) contacts sorted by surname: " << contacts;
    QVERIFY(contacts.size() == 5);

    expectedContacts.clear();
    QFETCH(QString, expectedContact1LastName);
    QFETCH(QString, expectedContact2LastName);
    QFETCH(QString, expectedContact3LastName);
    QFETCH(QString, expectedContact4LastName);
    QFETCH(QString, expectedContact5LastName);
    expectedContacts << expectedContact1LastName << expectedContact3LastName << expectedContact4LastName << expectedContact2LastName << expectedContact5LastName;
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) EXPECTED contacts sorted by surname: " << expectedContacts;
    bool found = false;
    for (int i = 0; i < contacts.size(); i++) {
        QVERIFY (contacts.at(i).detail<QContactName>().lastName() == expectedContacts.at(i));
    }

    // now sort emails alphabetically
    sortOrder.setDetailType(QContactEmailAddress::Type, QContactEmailAddress::FieldEmailAddress);
    sortOrders.clear();
    sortOrders.append(sortOrder);
    contacts = myContactManager.contacts(filter, sortOrders);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) contacts sorted by email address: " << contacts;
    QVERIFY(contacts.size() == 5);

    expectedContacts.clear();
    QFETCH(QString, expectedContact1EmailAddress);
    QFETCH(QString, expectedContact2EmailAddress);
    QFETCH(QString, expectedContact3EmailAddress);
    QFETCH(QString, expectedContact4EmailAddress);
    QFETCH(QString, expectedContact5EmailAddress);
    expectedContacts << expectedContact4EmailAddress << expectedContact1EmailAddress << expectedContact3EmailAddress << expectedContact5EmailAddress << expectedContact2EmailAddress;
    QVERIFY(expectedContacts.size() == 5);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) EXPECTED contacts sorted by email: " << expectedContacts;
    found = false;
    for (int i = 0; i < contacts.size(); i++) {
        QVERIFY (contacts.at(i).detail<QContactEmailAddress>().emailAddress() == expectedContacts.at(i));
    }

    // retrieve contacts filtering them by first name, then sort the results alphabetically
    sortOrder.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    sortOrders.clear();
    sortOrders.append(sortOrder);
    QString firstName = "li";
    QContactDetailFilter dfil;
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchContains);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    contacts = myContactManager.contacts(dfil, sortOrders);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) contacts filtered and sorted by first name: " << contacts;
    expectedContacts.clear();
    QVERIFY(contacts.size() == 3);

    expectedContacts << expectedContact4FirstName << expectedContact3FirstName << expectedContact5FirstName;
    QVERIFY(expectedContacts.size() == 3);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) expected filtered and ordered contacts: " << expectedContacts;
    for (int i = 0; i < expectedContacts.size(); i++) {
        QVERIFY (contacts.at(i).detail<QContactName>().firstName() == expectedContacts.at(i));
    }

    // now sort the filtered results by email address
    sortOrder.setDetailType(QContactEmailAddress::Type, QContactEmailAddress::FieldEmailAddress);
    sortOrders.clear();
    sortOrders.append(sortOrder);
    firstName = "";
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchContains);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    contacts = myContactManager.contacts(dfil, sortOrders);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) filtered contacts sorted by email address: " << contacts;
    QVERIFY(contacts.size() == 5);
}

void tst_QContactJsondbEngine::testContactSortOrder_data() {
    QTest::addColumn<QString>("expectedContact1FirstName");
    QTest::addColumn<QString>("expectedContact1LastName");
    QTest::addColumn<QString>("expectedContact2FirstName");
    QTest::addColumn<QString>("expectedContact2LastName");
    QTest::addColumn<QString>("expectedContact3FirstName");
    QTest::addColumn<QString>("expectedContact3LastName");
    QTest::addColumn<QString>("expectedContact4FirstName");
    QTest::addColumn<QString>("expectedContact4LastName");
    QTest::addColumn<QString>("expectedContact5FirstName");
    QTest::addColumn<QString>("expectedContact5LastName");
    QTest::addColumn<QString>("expectedContact1EmailAddress");
    QTest::addColumn<QString>("expectedContact2EmailAddress");
    QTest::addColumn<QString>("expectedContact3EmailAddress");
    QTest::addColumn<QString>("expectedContact4EmailAddress");
    QTest::addColumn<QString>("expectedContact5EmailAddress");
    {
        QTest::newRow("expectedContactsNames")  << "Harry" << "Baker" << "Paul" << "Thomson" << "Julie" << "King" << "Angelina" << "Row" << "Natalie" << "Watson" << "Harry.Baker@ovi.com" << "Paul.Thomson@ovi.com" << "Julie.King@ovi.com" << "Angelina.Row@ovi.com" << "Natalie.Watson@ovi.com";
    }
}

void tst_QContactJsondbEngine::testRemoveContacts() {
    QContactManager cm;

    // Test error generation when passing empty Id list
    QList<QContactId> emptyList;
    bool returnValue = cm.removeContacts(emptyList);
    QVERIFY(returnValue == false);
    QVERIFY(cm.error() == 10); // Check if the generated error is in fact "BadArgumentError"

    // Test error generation when passing invalid Ids: empty string
    QList<QContactId> voidList;
    voidList << QContactId();
    returnValue = cm.removeContacts(voidList);
    QVERIFY(returnValue == false);
    QVERIFY(cm.error() == 1); // Check if the generated error is in fact "DoesNotExistError"

    // TODO: This is the same as above, clean or use real invalid id.
    // Test error generation when passing invalid Ids: non existing string
    QList<QContactId> invalidList;
    invalidList << QContactId();
    qDebug() << "(non existing contact Id) TO REMOVE: " << invalidList;
    returnValue = cm.removeContacts(invalidList);
    QVERIFY(returnValue == false);

    QVERIFY(cm.error() == 1); // Check whether the generated error code is in fact "DoesNotExistError"

    // Remove only two contacts
    QList<QContact> saveList;
    QContactName nameDetail;
    QContact contactToRemove1;
    nameDetail.setFirstName("Contact To Remove 1");
    contactToRemove1.saveDetail(&nameDetail);
    saveList << contactToRemove1;
    cm.saveContact(&contactToRemove1);
    QContactId contactToRemove1Id = contactToRemove1.id();

    QContact contactToRemove2;
    nameDetail.setFirstName("Contact To Remove 2");
    contactToRemove2.saveDetail(&nameDetail);
    saveList << contactToRemove2;
    cm.saveContact(&contactToRemove2);
    QContactId contactToRemove2Id = contactToRemove2.id();

    QList<QContactId> contactsToRemove;
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(filter, sortOrders);
    QContactName nameDetail1;
    nameDetail1.setFirstName("Contact To Remove 1");
    QContactName nameDetail2;
    nameDetail2.setFirstName("Contact To Remove 2");
    foreach (QContact curr, contacts) {
        if (curr.detail<QContactName>() == nameDetail1) contactsToRemove << curr.id();
        if (curr.detail<QContactName>() == nameDetail2) contactsToRemove << curr.id();
    }
    if (qt_debug_jsondb_contacts())
        qDebug() << "(two contacts) TO REMOVE: " << contactsToRemove;
    returnValue = cm.removeContacts(contactsToRemove);
    QVERIFY(returnValue == true);

    QVERIFY(cm.error() == 0); // Check if there are errors
    //QCOMPARE(cm.contactIds().size(), originalCount - 2);  // Does not work when there are multiple contacts with the same name
    bool contact1Found = false;
    bool contact2Found = false;
    contacts = cm.contacts(filter, sortOrders); // Update the contact list after removal
    foreach (QContact curr, contacts) {
        if (curr.detail<QContactName>() == nameDetail1) contact1Found=true;
        if (curr.detail<QContactName>() == nameDetail2) contact2Found=true;
    }
    QVERIFY(contact1Found == false);
    QVERIFY(contact2Found == false);

    QList<QContactId> toRemove = cm.contactIds();
    if (qt_debug_jsondb_contacts())
        qDebug() << "TO REMOVE: " << toRemove;
    returnValue = cm.removeContacts(toRemove);
    QVERIFY(returnValue == true || cm.error() == QContactManager::BadArgumentError);  // It currently gives BadArgumentError if empty list given (no contacts in cm).
    QList<QContactId> contactsLeft = cm.contactIds();
    qDebug() << "NOTE: the error message by doSyncRequest in function contactIds is due to the fact that there are no contacts left in the database";
    QVERIFY(contactsLeft.isEmpty()); // Check that all the contacts have been removed

    // save two new contacts (just not to leave the database empty)
    QContact newContact1;
    nameDetail.setFirstName("new contact 1");
    newContact1.saveDetail(&nameDetail);
    saveList << newContact1;
    cm.saveContact(&newContact1);
    QContact newContact2;
    nameDetail.setFirstName("new contact 2");
    newContact2.saveDetail(&nameDetail);
    saveList << newContact2;
    cm.saveContact(&newContact2);
}

void tst_QContactJsondbEngine::testRemoveContact() {
    QContactManager cm;

    // save a new contact
    QContact contactToRemove;
    QContactName nameDetail;
    nameDetail.setFirstName("Contact To Remove");
    contactToRemove.saveDetail(&nameDetail);
    cm.saveContact(&contactToRemove);
    QContactId contactToRemoveId = contactToRemove.id();

    // Remove the contact
    bool returnValue = cm.removeContact(contactToRemoveId);
    QVERIFY(returnValue == true);
    QVERIFY(cm.error() == 0); // Check if there are errors
    bool contactToRemoveFound = false;
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(filter, sortOrders); // Update the contact list after removal
    foreach (QContact curr, contacts) {
        if (curr.detail<QContactName>() == nameDetail) contactToRemoveFound=true;
    }
    QVERIFY(contactToRemoveFound == false);
}

QTEST_MAIN(tst_QContactJsondbEngine);

#include "tst_qcontactjsondbengine.moc"
