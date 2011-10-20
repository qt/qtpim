/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

#include <jsondb-error.h>
#include <jsondb-client.h>
#include <private/jsondb-connection_p.h>
#include <private/jsondb-strings_p.h>

Q_USE_JSONDB_NAMESPACE

QTCONTACTS_USE_NAMESPACE

class tst_QContactJsondbEngine : public QObject
{
    Q_OBJECT

public:
    tst_QContactJsondbEngine();
    ~tst_QContactJsondbEngine();
public slots:
    void init();
    void cleanup();
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

private:
    QContactJsonDbEngine* m_engine;
    QContactJsonDbBackup* m_myBackup;
};

void tst_QContactJsondbEngine::init() {
    // clears the database and loads desired test data

    m_myBackup = new(QContactJsonDbBackup);
    //myBackup.loadTestData();
}

void tst_QContactJsondbEngine::cleanup(){
    delete(m_myBackup);
}

tst_QContactJsondbEngine::tst_QContactJsondbEngine()
{
    m_engine = new QContactJsonDbEngine();
}

tst_QContactJsondbEngine::~tst_QContactJsondbEngine()
{
    if(m_engine) {
        delete m_engine;
        m_engine = NULL;
    }
}

void tst_QContactJsondbEngine::testSelfContactId() {
    qDebug() << "NOTE: THE IDENTIFICATION FIELD DOES NOT EXIST YET IN JSON SCHEMA, just returns \"not found\"";
    QContactManager myContactManager;
    QContactLocalId myId;
    myId = myContactManager.selfContactId();
    qDebug() << "MyId is:" << myId;
}

void tst_QContactJsondbEngine::testContactIds() {
    QContactManager myContactManager;
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContactLocalId> resultContactIds = myContactManager.contactIds(filter, sortOrders);

    QList<QContact> contacts = myContactManager.contacts(filter, sortOrders); // used as reference. Not possible to use a table here because the IDs change every time the test is triggered...
    QList<QContactLocalId> referenceContactIds;
    foreach (QContact contact, contacts) {
        referenceContactIds.append(contact.localId());
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
    QVERIFY(contacts.size() == 5);

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
    QList<QContactLocalId> resultContactIds = cm.contactIds(filter, sortOrders);
    QContactLocalId id = resultContactIds.at(1); // Just a random ID picked
    QContact contact = cm.contact(id);
    QVERIFY(id == contact.id().localId());

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
    QContactLocalIdFilter localIdFilter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(localIdFilter, sortOrders);

    // Take first of them as test contact for update test.
    QContact testContact = contacts[0];
    localIdFilter.add(testContact.localId());
    QContactFetchRequest fetchRequest;
    fetchRequest.setFilter(localIdFilter);

    // Convert QContact to JsonDb contact and insert extra fields to json contact.
    QVariantMap jsonContact;
    QContactJsonDbConverter jsonDbConverter;
    int extraFieldInContact = 3;
    int extraFieldInContactDetails = 5;
    int extraFieldInContactEmails = 8;
    if (jsonDbConverter.toJsonContact(&jsonContact, testContact)) {
        jsonContact.insert("extra3", extraFieldInContact);

        QVariantMap embeddedDetailsMap;
        embeddedDetailsMap["extra5"] = extraFieldInContactDetails;
        jsonContact.insert("details", embeddedDetailsMap);

        QVariantMap embeddedEmailMap;
        embeddedEmailMap["extra8"] = extraFieldInContactEmails;
        jsonContact.insert("emails", embeddedEmailMap);
    }

    // Save test contact with extra fields directly to the jsondb.
    JsonDbConnection* jsonConnection = new JsonDbConnection(this);
    jsonConnection->connectToServer();
    QVariantMap jsonResponse = JsonDbConnection::makeUpdateRequest(jsonContact);
    jsonResponse = JsonDbConnection::instance()->sync(jsonResponse).value<QVariantMap>();

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

    phoneNumberDetail.setSubTypes(QContactPhoneNumber::SubTypeMobile);
    phoneNumberDetail.setNumber("+358507654322");
    testContact.saveDetail(&phoneNumberDetail);
    cm.synthesizeContactDisplayLabel(&testContact);
    cm.saveContact(&testContact);

    // TODO: Check the updated fields are properly there.
    QList<QContact> contactsNow = cm.contacts(localIdFilter, sortOrders);
    foreach (QContact cntc, contactsNow ) {
        if (cntc.localId() == testContact.localId()) {
           }
    }

    // Fetch directly from jsondb the test contact data.
    QString finalQuery = jsonDbConverter.queryFromRequest(&fetchRequest);
    QVariantMap finalJsonDbItem = JsonDbConnection::makeQueryRequest(finalQuery);
    finalJsonDbItem = JsonDbConnection::instance()->sync(finalJsonDbItem).value<QVariantMap>();
    QVERIFY(!finalJsonDbItem.isEmpty());
    QVariantList jsonDbObjectList = finalJsonDbItem.value("data").toList();
    QVERIFY(!jsonDbObjectList.isEmpty());
    QVariantMap finalJsonContact = jsonDbObjectList.at(0).toMap();
    QVariantMap embeddedDetailsMap = finalJsonContact["details"].value<QVariantMap>();
    QVariantMap embeddedEmailMap = finalJsonContact["emails"].value<QVariantMap>();

    // Check that update did not change extra fields in contact object.
    QCOMPARE(finalJsonContact.value("extra3").toInt(),extraFieldInContact);
    // Check that update did not change extra fields in contact embededed details object.
    QCOMPARE(embeddedDetailsMap.value("extra5").toInt(),extraFieldInContactDetails);
    // We do not preserve extra fields for other embedded objects like emails.
    QCOMPARE(embeddedEmailMap.value("extra8"), QVariant());

    delete jsonConnection;
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

    QContactLocalId testContactId = testContact.localId();
    QContact expected = cm.contact(testContactId);

    QCOMPARE(cm.contactIds().size(), originalCount + 1);
    QCOMPARE(testContact.localId(), expected.localId());
    QCOMPARE(testContact.detail(QContactName::DefinitionName), expected.detail(QContactName::DefinitionName));
    QCOMPARE(testContact.detail(QContactPersonId::DefinitionName), expected.detail(QContactPersonId::DefinitionName));
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
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
    dfil.setValue(firstName);
    dfil.setMatchFlags(QContactFilter::MatchContains);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    QList<QContact> contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 3);

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
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldLastName);
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
    QString phoneNr = "6543";
    dfil.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
    dfil.setValue(phoneNr);
    QVERIFY(dfil.matchFlags() == QContactFilter::MatchContains);
    contacts = myContactManager.contacts(dfil, sortOrders);
    QVERIFY(contacts.size() == 3);
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
    dfil.setDetailDefinitionName(QContactEmailAddress::DefinitionName, QContactEmailAddress::FieldEmailAddress);
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
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
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
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldLastName);
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
    dfil.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
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
    dfil.setDetailDefinitionName(QContactEmailAddress::DefinitionName, QContactEmailAddress::FieldEmailAddress);
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
    QContactLocalIdFilter idf;
    QVERIFY(idf.type() == QContactFilter::LocalIdFilter);
    QVERIFY(idf.ids().count() == 0);
    QList<QContactLocalId> ids = myContactManager.contactIds(idf, sortOrders);

    // Retrieve only the first ID
    QContactLocalId firstId = ids.first();
    QList<QContactLocalId> shortList;
    shortList << firstId;
    idf.setIds(shortList);
    QVERIFY(idf.ids() == shortList);

    QContact contact = myContactManager.contact(firstId);
    QVERIFY(firstId == contact.id().localId());
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
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
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
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
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
    sortOrder.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
    QList<QContactSortOrder> sortOrders;
    sortOrders.append(sortOrder);

    // retrieve all the contacts and sort first names alphabetically
    QContactFilter filter;
    QList<QContact> contacts = myContactManager.contacts(filter, sortOrders);
    QVERIFY(contacts.size() == 5);

    QFETCH(QString, expectedContact1FirstName);
    QFETCH(QString, expectedContact2FirstName);
    QFETCH(QString, expectedContact3FirstName);
    QFETCH(QString, expectedContact4FirstName);
    QFETCH(QString, expectedContact5FirstName);
    QList<QString> expectedContacts;
    expectedContacts << expectedContact4FirstName << expectedContact1FirstName << expectedContact3FirstName << expectedContact5FirstName << expectedContact2FirstName;
    QVERIFY(expectedContacts.size() == 5);
    if (qt_debug_jsondb_contacts())
        qDebug() << " (TEST) expected ordered contacts: " << expectedContacts;
    for (int i = 0; i < contacts.size(); i++) {
        QVERIFY (contacts.at(i).detail<QContactName>().firstName() == expectedContacts.at(i));
    }

    // now sort last names alphabetically
    sortOrder.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldLastName);
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
    sortOrder.setDetailDefinitionName(QContactEmailAddress::DefinitionName, QContactEmailAddress::FieldEmailAddress);
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
    sortOrder.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
    sortOrders.clear();
    sortOrders.append(sortOrder);
    QString firstName = "li";
    QContactDetailFilter dfil;
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
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
    sortOrder.setDetailDefinitionName(QContactEmailAddress::DefinitionName, QContactEmailAddress::FieldEmailAddress);
    sortOrders.clear();
    sortOrders.append(sortOrder);
    firstName = "";
    dfil.setDetailDefinitionName(QContactName::DefinitionName, QContactName::FieldFirstName);
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
    QList<QContactLocalId> emptyList;
    qDebug() << "(no contacts) TO REMOVE: " << emptyList;
    bool returnValue = cm.removeContacts(emptyList);
    QVERIFY(returnValue == false);
    QVERIFY(cm.error() == 10); // Check if the generated error is in fact "BadArgumentError"

    // Test error generation when passing invalid Ids: empty string
    QList<QContactLocalId> voidList;
    voidList << "";
    qDebug() << "(fake contact Id: empty string) TO REMOVE: " << voidList;
    returnValue = cm.removeContacts(voidList);
    qDebug() << "NOTE: the error message by doSyncRequest in function removeContacts is related to a feature in JsonDb";
    QVERIFY(returnValue == false);
    QVERIFY(cm.error() == 1); // Check if the generated error is in fact "DoesNotExistError"

    // Test error generation when passing invalid Ids: non existing string
    QList<QContactLocalId> invalidList;
    invalidList << "00000000-0000-0000-0000-0000000000000000";
    qDebug() << "(non existing contact Id) TO REMOVE: " << invalidList;
    returnValue = cm.removeContacts(invalidList);
    QVERIFY(returnValue == false);

    QSKIP("TODO: Gives now unspecified error which is waiting fix in the backend", SkipSingle);
    QVERIFY(cm.error() == 1); // Check whether the generated error code is in fact "DoesNotExistError"

    // Remove only two contacts
    QList<QContact> saveList;
    QContactName nameDetail;
    QContact contactToRemove1;
    nameDetail.setFirstName("Contact To Remove 1");
    contactToRemove1.saveDetail(&nameDetail);
    saveList << contactToRemove1;
    cm.saveContact(&contactToRemove1);
    QContactLocalId contactToRemove1Id = contactToRemove1.localId();

    QContact contactToRemove2;
    nameDetail.setFirstName("Contact To Remove 2");
    contactToRemove2.saveDetail(&nameDetail);
    saveList << contactToRemove2;
    cm.saveContact(&contactToRemove2);
    QContactLocalId contactToRemove2Id = contactToRemove2.localId();

    QList<QContactLocalId> contactsToRemove;
    QContactFilter filter;
    QList<QContactSortOrder> sortOrders;
    QList<QContact> contacts = cm.contacts(filter, sortOrders);
    QContactName nameDetail1;
    nameDetail1.setFirstName("Contact To Remove 1");
    QContactName nameDetail2;
    nameDetail2.setFirstName("Contact To Remove 2");
    foreach (QContact curr, contacts) {
        if (curr.detail<QContactName>() == nameDetail1) contactsToRemove << curr.id().localId();
        if (curr.detail<QContactName>() == nameDetail2) contactsToRemove << curr.id().localId();
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

    QList<QContactLocalId> toRemove = cm.contactIds();
    if (qt_debug_jsondb_contacts())
        qDebug() << "TO REMOVE: " << toRemove;
    returnValue = cm.removeContacts(toRemove);
    QVERIFY(returnValue == true || cm.error() == QContactManager::BadArgumentError);  // It currently gives BadArgumentError if empty list given (no contacts in cm).
    QList<QContactLocalId> contactsLeft = cm.contactIds();
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
    QContactLocalId contactToRemoveId = contactToRemove.localId();

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
