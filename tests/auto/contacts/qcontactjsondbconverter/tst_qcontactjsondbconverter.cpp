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
#include <QJsonObject>
#include <QContact>
#include <qcontactname.h>
#include <QContactGender>
#include <QContactOrganization>
#include <QContactBirthday>
#include <QContactAvatar>
#include <QContactRingtone>
#include <QContactNickname>
#include <QContactNote>
#include <QContactEmailAddress>
#include <QContactPhoneNumber>
#include <QContactAddress>
#include <QContactUrl>
#include <QContactIdFilter>
#include <QContactId>

#include "qcontactjsondbconverter.h"
#include "qcontactjsondbengine.h"
#include "qcontactjsondbstring.h"
#include "qcontactjsondbid.h"

QTCONTACTS_USE_NAMESPACE

class tst_QcontactJsondbConverter : public QObject
{
    Q_OBJECT
public:
    tst_QcontactJsondbConverter();
private Q_SLOTS:
    void toQContactTest();
    void toJsonContactTest();
    void toQContactsTest();
    void updateContextsTest();
    void queryFromRequestTest();
    void convertCompoundFilterTest();
    void convertSortOrderTest();
    void contactIdToUuidTest();
private:
    void testJsonDetailItems(const QJsonObject& values, const QString& extractField,
                             const QMap<QString, QString>& fields, bool testSize = true);
    void initializeJsonContact(QJsonObject& jsonContact, unsigned int numbering = 0);
    QString convertToISODate(QString date);

private:
    QString m_partitionName;
};


tst_QcontactJsondbConverter::tst_QcontactJsondbConverter()
    : m_partitionName(QContactJsonDbStr::userDataPartition())
{
    // nothing needed
}





void tst_QcontactJsondbConverter::toQContactTest()
{
    QContactJsonDbConverter converter;
    QContactJsonDbEngine engine;
    QJsonObject jsonContact;
    QContact contact;
    QJsonObject jsonData;
    QContactDetail detail;

    // test name (set in initialization)
    initializeJsonContact(jsonContact);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactName::Type);
    QVERIFY(!detail.isEmpty());
    QContactName* name = static_cast<QContactName*>(&detail);
    QVERIFY(name != NULL);
    QCOMPARE(name->firstName(), QString("John"));
    QCOMPARE(name->lastName(), QString("Doe"));
    QCOMPARE(name->middleName(), QString("Tom"));
    // cleanup
    contact.clearDetails();


    // gender
    initializeJsonContact(jsonContact);
    jsonData.insert("gender", QString("male"));
    jsonContact.insert("gender", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactGender::Type);
    QVERIFY(!detail.isEmpty());
    QContactGender* gender = static_cast<QContactGender*>(&detail);
    QVERIFY(gender != NULL);
    QCOMPARE(gender->gender(), QContactGender::GenderMale);
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // organization
    QDateTime startDate = QDateTime::fromString("1970-01-01", Qt::ISODate).toUTC();
    QDateTime endDate = QDateTime::fromString("2038-01-19", Qt::ISODate).toUTC();
    initializeJsonContact(jsonContact);
    jsonData.insert("name", QString("ACME"));
    jsonData.insert("department", QString("Spy"));
    jsonData.insert("title", QString("Vice President"));
    jsonData.insert("role", QString("Superhero"));
    jsonData.insert("assistantName", QString("Daisy Duck"));
    jsonData.insert("logoUrl", QString("http://www.acme.com/logo.jpg"));
    jsonData.insert("startDate",convertToISODate("1970-01-01"));
    jsonData.insert("endDate",convertToISODate("2038-01-19"));
    QJsonArray organizationData;
    organizationData.append(jsonData);
    jsonContact.insert("organization", organizationData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactOrganization::Type);
    QVERIFY(!detail.isEmpty());
    QContactOrganization* org = static_cast<QContactOrganization*>(&detail);
    QVERIFY(org != NULL);
    QCOMPARE(org->name(), QString("ACME"));
    QCOMPARE(org->department().at(0), QString("Spy"));
    QCOMPARE(org->title(), QString("Vice President"));
    QCOMPARE(org->role(), QString("Superhero"));
    QCOMPARE(org->assistantName(), QString("Daisy Duck"));
    QCOMPARE(org->logoUrl(), QUrl("http://www.acme.com/logo.jpg"));
    QCOMPARE(org->startDate(),startDate);
    QCOMPARE(org->endDate(),endDate);
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();



    // birthday
    initializeJsonContact(jsonContact);
    QDateTime birthDayDate = QDateTime::fromString("1979-11-22", Qt::ISODate).toUTC();
    jsonData.insert("birthday", convertToISODate("1979-11-22"));
    jsonContact.insert("details", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactBirthday::Type);
    QVERIFY(!detail.isEmpty());
    QContactBirthday* bd = static_cast<QContactBirthday*>(&detail);
    QVERIFY(bd != NULL);
    QCOMPARE(bd->dateTime(), birthDayDate);
    // cleanup
    contact.clearDetails();

    // avatar
    initializeJsonContact(jsonContact);
    jsonData.insert("photoUrl", QString("http://www.acme.com/logo.jpg"));
    jsonContact.insert("details", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactAvatar::Type);
    QVERIFY(!detail.isEmpty());
    QContactAvatar* avatar = static_cast<QContactAvatar*>(&detail);
    QVERIFY(avatar != NULL);
    QCOMPARE(avatar->imageUrl(), QUrl("http://www.acme.com/logo.jpg"));
    // cleanup
    contact.clearDetails();


    // ringtone
    initializeJsonContact(jsonContact);
    jsonData.insert("ringtoneUrl", QString("http://www.acme.com/ring.mp3"));
    jsonContact.insert("details", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactRingtone::Type);
    QVERIFY(!detail.isEmpty());
    QContactRingtone* ring = static_cast<QContactRingtone*>(&detail);
    QVERIFY(ring != NULL);
    QCOMPARE(ring->audioRingtoneUrl(), QUrl("http://www.acme.com/ring.mp3"));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // nickname
    initializeJsonContact(jsonContact);
    jsonData.insert("nickname", QString("Chupacabra"));
    jsonContact.insert("details", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactNickname::Type);
    QVERIFY(!detail.isEmpty());
    QContactNickname* nick = static_cast<QContactNickname*>(&detail);
    QVERIFY(nick != NULL);
    QCOMPARE(nick->nickname(), QString("Chupacabra"));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();


    // note
    initializeJsonContact(jsonContact);
    jsonData.insert("note", QString("test note"));
    jsonContact.insert("details", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactNote::Type);
    QVERIFY(!detail.isEmpty());
    QContactNote* note = static_cast<QContactNote*>(&detail);
    QVERIFY(note != NULL);
    QCOMPARE(note->note(), QString("test note"));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // email
    initializeJsonContact(jsonContact);
    jsonData.insert("value", QString("john@doe.com"));
    jsonData.insert("context", QString("home"));
    QJsonArray emails;
    emails.append(jsonData);
    jsonContact.insert("emails", emails);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactEmailAddress::Type);
    QVERIFY(!detail.isEmpty());
    QContactEmailAddress* email = static_cast<QContactEmailAddress*>(&detail);
    QVERIFY(email != NULL);
    QCOMPARE(email->emailAddress(), QString("john@doe.com"));
    QVERIFY(email->contexts().size() == 1);
    QVERIFY(email->contexts()[0] == QContactEmailAddress::ContextHome);
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // phone number
    initializeJsonContact(jsonContact);
    jsonData.insert("value", QString("0507654321"));
    jsonData.insert("subType", QString("cell"));
    QJsonArray phones;
    phones.append(jsonData);
    jsonContact.insert("phones", phones);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactPhoneNumber::Type);
    QVERIFY(!detail.isEmpty());
    QContactPhoneNumber* phone = static_cast<QContactPhoneNumber*>(&detail);
    QVERIFY(phone != NULL);
    QCOMPARE(phone->number(), QString("0507654321"));
    QVERIFY(phone->subTypes().size() == 1);
    QVERIFY(phone->subTypes()[0] == QContactPhoneNumber::SubTypeMobile);
    // cleanup
    contact.clearDetails();
    phones = QJsonArray();
    jsonData = QJsonObject();

    //let's try with a phone number containing a few invalid characters
    jsonData.insert("value",
                    QString("          +123(45678)90-+++abcdefghijklmnopqrstuvwxyz#*+++++          "));
    jsonData.insert("subType", QString("cell"));
    phones.append(jsonData);
    jsonContact.insert("phones", phones);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactPhoneNumber::Type);
    QVERIFY(!detail.isEmpty());
    phone = static_cast<QContactPhoneNumber*>(&detail);
    QVERIFY(phone != NULL);
    QCOMPARE(phone->number(), QString("+123(45678)90-abcdpw#*"));
    QVERIFY(phone->subTypes().size() == 1);
    QVERIFY(phone->subTypes()[0] == QContactPhoneNumber::SubTypeMobile);
    // cleanup
    jsonData = QJsonObject();
    phones = QJsonArray();
    contact.clearDetails();

    //let's try with an invalid phone number
    jsonData.insert("value",
                    QString("          efghijk\"\"][[[]]]          "));
    jsonData.insert("subType", QString("cell"));
    phones.append(jsonData);
    jsonContact.insert("phones", phones);
    QVERIFY(!converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactPhoneNumber::Type);
    QVERIFY(detail.isEmpty());
    phone = static_cast<QContactPhoneNumber*>(&detail);
    QVERIFY(phone->isEmpty());
    contact.clearDetails();
    //yet another invalid phone number (too long)
    QString tooLongNumber;
    for (int i=0; i<100; i++) {
        tooLongNumber += QString::number(i/10);
    }
    jsonData = QJsonObject();
    phones = QJsonArray();
    jsonData.insert("value", tooLongNumber);
    phones.append(jsonData);
    jsonContact.insert("phones", phones);
    QVERIFY(!converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactPhoneNumber::Type);
    QVERIFY(detail.isEmpty());
    phone = static_cast<QContactPhoneNumber*>(&detail);
    QVERIFY(phone->isEmpty());
    // cleanup
    jsonData = QJsonObject();
    phones = QJsonArray();
    contact.clearDetails();
    //last but not least invalid phone number (empty)
    QString emptyNumber;
    jsonData.insert("value", emptyNumber);
    phones.append(jsonData);
    jsonContact.insert("phones", phones);
    QVERIFY(!converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactPhoneNumber::Type);
    QVERIFY(detail.isEmpty());
    phone = static_cast<QContactPhoneNumber*>(&detail);
    QVERIFY(phone->isEmpty());
    // cleanup
    jsonData = QJsonObject();
    contact.clearDetails();

    // address
    initializeJsonContact(jsonContact);
    jsonData.insert("country", QString("Finland"));
    jsonData.insert("postOfficeBox", QString("347"));
    jsonData.insert("postcode", QString("33101"));
    jsonData.insert("locality", QString("Tampere"));
    jsonData.insert("region", QString("Pirkanmaa"));
    jsonData.insert("street", QString("PL 347"));
    QJsonArray addresses;
    addresses.append(jsonData);
    jsonContact.insert("addresses", addresses);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactAddress::Type);
    QVERIFY(!detail.isEmpty());
    QContactAddress* addr = static_cast<QContactAddress*>(&detail);
    QVERIFY(addr != NULL);
    QCOMPARE(addr->country(), QString("Finland"));
    QCOMPARE(addr->postOfficeBox(), QString("347"));
    QCOMPARE(addr->postcode(), QString("33101"));
    QCOMPARE(addr->locality(), QString("Tampere"));
    QCOMPARE(addr->region(), QString("Pirkanmaa"));
    QCOMPARE(addr->street(), QString("PL 347"));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();


    // url
    initializeJsonContact(jsonContact);
    jsonData.insert("value", QString("http://www.acme.com"));
    QJsonArray urls;
    urls.append(jsonData);
    jsonContact.insert("urls", urls);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactUrl::Type);
    QVERIFY(!detail.isEmpty());
    QContactUrl* url = static_cast<QContactUrl*>(&detail);
    QVERIFY(url != NULL);
    QCOMPARE(url->url(), QString("http://www.acme.com"));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // version
    initializeJsonContact(jsonContact);
    QString sequenceNumberString (QStringLiteral("1"));
    QString versionString (QStringLiteral("1234567890"));
    QString jsonVersion(sequenceNumberString + "-" + versionString);
    QByteArray expectedExtendedVersion(versionString.toLatin1());

    jsonContact.insert("_version", jsonVersion);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactVersion::Type);
    QVERIFY(!detail.isEmpty());
    QContactVersion* version = static_cast<QContactVersion*>(&detail);
    QVERIFY(version != NULL);
    QCOMPARE(version->extendedVersion(), expectedExtendedVersion);
    QCOMPARE(version->sequenceNumber(), sequenceNumberString.toInt());
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // extended detail with simple string data
    initializeJsonContact(jsonContact);
    jsonContact.insert("simpleStringDetail", QString("Simple string as detail data."));
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    QList<QContactExtendedDetail> extendedDetails = contact.details<QContactExtendedDetail>();
    QCOMPARE(extendedDetails[0].name(), QString("simpleStringDetail"));
    QCOMPARE(extendedDetails[0].data().toString(), QString("Simple string as detail data."));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // extended detail with QJsonObject as data
    initializeJsonContact(jsonContact);
    jsonData.insert("Item1", QString("Content for list item 1."));
    jsonData.insert("Item2", int(2));
    jsonData.insert("Item3", QString("Content for list item 3."));
    jsonData.insert("Item4", QString("Content for list item 4."));
    jsonContact.insert("jsonObjectDetail", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactExtendedDetail::Type);
    QContactExtendedDetail* extendedDetail = static_cast<QContactExtendedDetail*>(&detail);
    QCOMPARE(extendedDetail->name(), QString("jsonObjectDetail"));
    QJsonObject extendedDetailItems = QJsonObject::fromVariantMap(extendedDetail->data().toMap());
    QCOMPARE(extendedDetailItems.value("Item1").toString(), QString("Content for list item 1."));
    QCOMPARE(extendedDetailItems.value("Item2").toVariant().toInt(), 2);
    QCOMPARE(extendedDetailItems.value("Item3").toString(), QString("Content for list item 3."));
    QCOMPARE(extendedDetailItems.value("Item4").toString(), QString("Content for list item 4."));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // extended detail with QJsonArray as data
    initializeJsonContact(jsonContact);
    QStringList extendedDetailsStringList;
    extendedDetailsStringList<<"QStringInListFirst"<<"QStringInListSecond";
    QJsonArray array = QJsonArray::fromStringList(extendedDetailsStringList);
    jsonContact.insert("jsonArrayDetail", array);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactExtendedDetail::Type);
    extendedDetail = static_cast<QContactExtendedDetail*>(&detail);
    QCOMPARE(extendedDetail->name(), QString("jsonArrayDetail"));
    QJsonArray extendedDetailListItems = QJsonArray::fromVariantList(extendedDetail->data().toList());
    QCOMPARE(extendedDetailListItems.count(),2);
    QCOMPARE(extendedDetailListItems.at(0).toString(), QString("QStringInListFirst"));
    QCOMPARE(extendedDetailListItems.at(1).toString(), QString("QStringInListSecond"));
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();

    // extended detail with complex structure as data
    initializeJsonContact(jsonContact);
    QVariantMap variantMap;
    QJsonArray array2;
    QString expectedStringFromVariantList(QStringLiteral("QStringInVariantList"));
    int expectedIntFromVariantList = 5;
    array2.append(expectedStringFromVariantList);
    array2.append(expectedIntFromVariantList);
    variantMap.insert("ItemInt", 1);
    variantMap.insert("ItemQString", QString("QStringData"));
    variantMap.insert("ItemQVariant", QJsonValue( QString("QStringInQVariant")).toVariant());
    variantMap.insert("ItemQVariantList", array2.toVariantList() );
    jsonData.insert("QStringItem", QString("Content for QStringItem."));
    jsonData.insert("arrayInMap", QJsonObject::fromVariantMap(variantMap));
    jsonContact.insert("complexObjectDetail", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, m_partitionName));
    detail = contact.detail(QContactExtendedDetail::Type);
    extendedDetail = static_cast<QContactExtendedDetail*>(&detail);
    QCOMPARE(extendedDetail->name(), QString("complexObjectDetail"));
    extendedDetailItems = QJsonObject::fromVariantMap(extendedDetail->data().toMap());
    QCOMPARE(extendedDetailItems.value("QStringItem").toString(), QString("Content for QStringItem."));
    QJsonObject returnedObject = extendedDetailItems.value("arrayInMap").toObject();
    QCOMPARE(returnedObject.value("ItemInt").toVariant().toInt(), 1);
    QCOMPARE(returnedObject.value("ItemQString").toString(), QString("QStringData"));
    QCOMPARE(returnedObject.value("ItemQVariant").toString(), QString("QStringInQVariant"));
    QCOMPARE(returnedObject.value("ItemQVariantList").toVariant().toList().size(), array2.size());
    QCOMPARE(returnedObject.value("ItemQVariantList").toVariant().toList().at(0).toString(), expectedStringFromVariantList);
    QCOMPARE(returnedObject.value("ItemQVariantList").toVariant().toList().at(1).toInt(), expectedIntFromVariantList);
    // cleanup
    contact.clearDetails();
    jsonData = QJsonObject();
}


void tst_QcontactJsondbConverter::toQContactsTest()
{
    QList<QJsonObject> contacts;
    // create bunch of contacts and convert those
    for(int i = 1; i < 10; ++i) {
        QJsonObject contact;
        QJsonObject phoneObject;
        QJsonObject emailObject;
        // add name field
        initializeJsonContact(contact, i);
        // add phone number
        phoneObject.insert("value", QString("050765432") + QString().setNum(i));
        phoneObject.insert("subType", QString("home"));
        QJsonArray phones;
        phones.append(phoneObject);
        contact.insert("phones", phones);
        // add email address
        emailObject.insert("value", QString().setNum(i) + "john@doe.com");
        emailObject.insert("subType", QString("home"));
        QJsonArray emails;
        emails.append(emailObject);
        contact.insert("emails", emails);
        contacts.append(contact);
    }
    QContactJsonDbConverter converter;
    QContactJsonDbEngine engine;
    QContactManager::Error error;
    QList<QContact> qcontacts;
    QContact qcontact;
    QContactDetail detail;
    QString number;
    // convert
    QVERIFY(converter.toQContacts(contacts, qcontacts, error, m_partitionName));
    QCOMPARE(error, QContactManager::NoError);
    QCOMPARE(qcontacts.size(), contacts.size());
    int i = 1;
    foreach(qcontact, qcontacts) {
        number = QString().setNum(i++);
        detail = qcontact.detail(QContactName::Type);
        QVERIFY(!detail.isEmpty());
        QContactName* name = static_cast<QContactName*>(&detail);
        QVERIFY(name != NULL);
        QCOMPARE(name->firstName(), "John" + number);
        QCOMPARE(name->lastName(), "Doe" + number);
        QCOMPARE(name->middleName(), "Tom" + number);
        detail = qcontact.detail(QContactPhoneNumber::Type);
        QVERIFY(!detail.isEmpty());
        QContactPhoneNumber* phone = static_cast<QContactPhoneNumber*>(&detail);
        QVERIFY(phone != NULL);
        QCOMPARE(phone->number(), "050765432" + number);
        detail = qcontact.detail(QContactEmailAddress::Type);
        QVERIFY(!detail.isEmpty());
        QContactEmailAddress* email = static_cast<QContactEmailAddress*>(&detail);
        QVERIFY(email != NULL);
        QCOMPARE(email->emailAddress(), number + "john@doe.com");
    }
}

void tst_QcontactJsondbConverter::toJsonContactTest()
{
    QContactJsonDbConverter converter;
    QJsonObject jsonContact;
    QContact contact;
    QMap<QString, QString> testFields;

    // Test name conversion
    QContactName name;
    name.setFirstName("John");
    name.setLastName("Doe");
    name.setMiddleName("Tom");
    contact.saveDetail(&name);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("firstName", "John");
    testFields.insert("lastName", "Doe");
    testFields.insert("middleName", "Tom");
    // test fields
    testJsonDetailItems(jsonContact, QContactJsonDbStr::nameDefinitionName(), testFields);
    // cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();

    // Test Gender
    QContactGender gender;
    gender.setGender(QContactGender::GenderMale);
    contact.saveDetail(&gender);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("gender", "male");

    // test fields
    testJsonDetailItems(jsonContact, "gender", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();

    // Test organization
    QContactOrganization organization;
    organization.setName("ACME");
    organization.setDepartment(QStringList() << "Spy");
    organization.setTitle("Vice President");
    organization.setRole("Superhero");
    organization.setAssistantName("Daisy Duck");
    organization.setLogoUrl(QUrl("http://www.acme.com/logo.jpg"));
    organization.setStartDate(QDateTime::fromString("Thu Jan 1 00:01:02 1998"));
    organization.setEndDate(QDateTime::fromString("Thu Jan 1 00:01:02 1998"));
    QList<int> OrganizationContextList;
    OrganizationContextList << QContactDetail::ContextWork;
    organization.setContexts(OrganizationContextList);
    contact.saveDetail(&organization);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    QString expectedStartDate,expectedEndDate;
    expectedStartDate = organization.startDate().toUTC().toString(Qt::ISODate);
    expectedEndDate = organization.endDate().toUTC().toString(Qt::ISODate);
    testFields.insert("name", "ACME");
    testFields.insert("department", "Spy");
    testFields.insert("title", "Vice President");
    testFields.insert("role", "Superhero");
    testFields.insert("assistantName", "Daisy Duck");
    testFields.insert("logoUrl", "http://www.acme.com/logo.jpg");
    testFields.insert("startDate", expectedStartDate);
    testFields.insert("endDate", expectedEndDate);
    testFields.insert("context", "work");
    // test fields
    testJsonDetailItems(jsonContact, "organization", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();


    // Test contact details
    // birthday
    QContactBirthday birthday;
    birthday.setDate(QDate(1979, 11, 22));
    contact.saveDetail(&birthday);
    QString expectedBirthDayDate;
    expectedBirthDayDate = birthday.dateTime().toUTC().toString(Qt::ISODate);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("birthday", expectedBirthDayDate);
    // avatar
    QContactAvatar avatar;
    avatar.setImageUrl(QUrl("http://www.acme.com/logo.jpg"));
    contact.saveDetail(&avatar);
    testFields.insert("photoUrl", "http://www.acme.com/logo.jpg");
    // ringtone
    QContactRingtone ringtone;
    ringtone.setAudioRingtoneUrl(QUrl("http://www.acme.com/ring.mp3"));
    contact.saveDetail(&ringtone);
    testFields.insert("ringtoneUrl", "http://www.acme.com/ring.mp3");
    // nickname
    QContactNickname nick;
    nick.setNickname("Chupacabra");
    contact.saveDetail(&nick);
    testFields.insert("nickname", "Chupacabra");
    // note
    QContactNote note;
    note.setNote("test note");
    contact.saveDetail(&note);
    testFields.insert("note", "test note");
    // convert
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test fields
    testJsonDetailItems(jsonContact, "details", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();

    // email
    QContactEmailAddress email;
    email.setEmailAddress("john@doe.com");
    email.setContexts(QContactEmailAddress::ContextHome);
    contact.saveDetail(&email);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("value", "john@doe.com");
    testFields.insert("context", "home");
    testJsonDetailItems(jsonContact, "emails", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();

    // phone number
    QContactPhoneNumber number;
    number.setNumber("0507654321");
    QList<int> phoneNumberContextList;
    phoneNumberContextList << QContactPhoneNumber::ContextWork;
    number.setContexts(phoneNumberContextList);
    contact.saveDetail(&number);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("value", "0507654321");
    testFields.insert("context", "work");
    testJsonDetailItems(jsonContact, "phones", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();
    //let's try with a phone number containing a few invalid characters
    number.setNumber("          +123(45678)90-+++abcdefghijklmnopqrstuvwxyz#*+++++          ");
    number.setContexts(phoneNumberContextList);
    contact.saveDetail(&number);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("value", "+123(45678)90-abcdpw#*");
    testFields.insert("context", "work");
    testJsonDetailItems(jsonContact, "phones", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();
    //let's try with an invalid phone number
    number.setNumber("          efghijk\"\"][[[]]]          ");
    number.setContexts(phoneNumberContextList);
    contact.saveDetail(&number);
    QVERIFY(!converter.toJsonContact(&jsonContact, contact));
    contact.clearDetails();
    //yet another invalid phone number (too long)
    QString tooLongNumber;
    for (int i=0; i<100; i++) {
        tooLongNumber += QString::number(i/10);
    }
    number.setNumber(tooLongNumber);
    contact.saveDetail(&number);
    QVERIFY(!converter.toJsonContact(&jsonContact, contact));
    //cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();
    //last but not least invalid phone number (empty)
    QString emptyNumber;
    number.setNumber(emptyNumber);
    contact.saveDetail(&number);
    QVERIFY(!converter.toJsonContact(&jsonContact, contact));
    //cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();


    // Test address conversion
    QContactAddress address;
    address.setCountry("Finland");
    address.setPostcode("33101");
    address.setStreet("PL 347");
    address.setPostOfficeBox("347");
    address.setRegion("Pirkanmaa");
    address.setLocality("Tampere");
    contact.saveDetail(&address);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test fields
    testFields.insert("country", "Finland");
    testFields.insert("postOfficeBox", "347");
    testFields.insert("postcode", "33101");
    testFields.insert("locality", "Tampere");
    testFields.insert("region", "Pirkanmaa");
    testFields.insert("street", "PL 347");
    // test fields
    testJsonDetailItems(jsonContact, "addresses", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();


    // Test url conversion
    QContactUrl url;
    url.setUrl("http://www.acme.com");
    contact.saveDetail(&url);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test fields
    testFields.insert("value", "http://www.acme.com");
    testJsonDetailItems(jsonContact, "urls", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
    testFields.clear();

    // Test version conversion
    QContactVersion version;
    QString sequenceNumberString (QStringLiteral("1"));
    QString versionString (QStringLiteral("1234567890"));
    QString expectedJsonVersion(sequenceNumberString + "-" + versionString);
    QByteArray expectedExtendedVersion(versionString.toLatin1());
    version.setSequenceNumber(1);
    version.setExtendedVersion(expectedExtendedVersion);
    contact.saveDetail(&version);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test fields
    QCOMPARE((jsonContact.value(QContactJsonDbStr::version())).toString(),
             expectedJsonVersion);
    // cleanup
    contact.clearDetails();

    // extended detail with a simple string as data.
    QContactExtendedDetail extendedDetail;
    extendedDetail.setName("extendedDetail1");
    extendedDetail.setData("Simple data for an extened detail.");
    contact.saveDetail(&extendedDetail);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test size, type and actual field
    QCOMPARE((jsonContact.value(QContactJsonDbStr::type())).toString(), QContactJsonDbStr::contactsJsonDbType());
    QCOMPARE((jsonContact.value("extendedDetail1")).toString(), QString("Simple data for an extened detail."));
    // cleanup
    contact.clearDetails();

    // extended detail with QJsonArray as data
    QContactExtendedDetail listExtendedDetail;
    listExtendedDetail.setName("listExtendedDetail");
    QStringList stringList;
    stringList << "QStringInListFirst" <<"QStringInListSecond";
    QJsonArray extendedDetailListItems = QJsonArray::fromStringList(stringList);
    listExtendedDetail.setData(extendedDetailListItems.toVariantList());
    contact.saveDetail(&listExtendedDetail);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    QCOMPARE(jsonContact.value(QContactJsonDbStr::type()).toString(), QContactJsonDbStr::contactsJsonDbType());
    QJsonArray convertedVariantList = jsonContact.value("listExtendedDetail").toArray();
    QCOMPARE(convertedVariantList.count(),2);
    QCOMPARE(convertedVariantList.at(0).toString(), QString("QStringInListFirst"));
    QCOMPARE(convertedVariantList.at(1).toString(), QString("QStringInListSecond"));
    // cleanup
    contact.clearDetails();

    // extended detail with a complex variantmap data
    QContactExtendedDetail complexExtendedDetail;
    complexExtendedDetail.setName("complexExtendedDetail");
    QVariantMap variantMap;
    variantMap.insert("mapItemInt", 1);
    variantMap.insert("mapItemQString", QString("QStringItem"));
    variantMap.insert("mapItemVariant", QVariant(QString("QStringItemInVariant")));
    variantMap.insert("mapItemVariantList", QVariantList() << QString("QStringItemInVariantList")
                      << QVariant(5)
                      << QString("AnotherQStringItemInVariantList"));
    complexExtendedDetail.setData(variantMap);
    contact.saveDetail(&complexExtendedDetail);
    initializeJsonContact(jsonContact);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test size, type and actual extended detail fields
    QCOMPARE(jsonContact.size(), 3);
    QCOMPARE(jsonContact.value(QContactJsonDbStr::type()).toString(), QContactJsonDbStr::contactsJsonDbType());
    QJsonObject convertedObject = jsonContact.value("complexExtendedDetail").toObject();
    QCOMPARE( convertedObject.value("mapItemInt").toVariant().toInt(), 1);
    QCOMPARE( convertedObject["mapItemQString"].toString(),QString("QStringItem"));
    QCOMPARE( convertedObject["mapItemVariant"].toString(),QString("QStringItemInVariant"));
    QJsonArray jsonArray = convertedObject.value("mapItemVariantList").toArray();
    QCOMPARE( jsonArray.at(0).toString(), QString("QStringItemInVariantList"));
    QCOMPARE( jsonArray.at(1).toVariant().toInt(), 5);
    QCOMPARE( jsonArray.at(2).toString(), QString("AnotherQStringItemInVariantList"));
    // cleanup
    contact.clearDetails();
    jsonContact = QJsonObject();
}

void tst_QcontactJsondbConverter::updateContextsTest()
{
    QContactJsonDbConverter converter;
    QContactJsonDbEngine engine;
    QStringList contexts;
    contexts << "home" << "work" << "other";
    QList<int> qcontexts;
    qcontexts << QContactDetail::ContextHome
              << QContactDetail::ContextWork
              << QContactDetail::ContextOther;
    QString context;
    QContactDetail qdetail;
    QJsonObject detail;
    int i = 0;
    // test every context
    foreach(context, contexts) {
        detail.insert("value", QString("0507654321"));
        detail.insert("context", context);
        QVERIFY(converter.updateContexts(detail, &qdetail));
        QVERIFY(qdetail.contexts().size() == 1);
        QVERIFY(qdetail.contexts()[0] == qcontexts.at(i++));
    }
}


void tst_QcontactJsondbConverter::queryFromRequestTest()
{
    QContactJsonDbConverter converter;
    // ContactSaveRequest
    // Functionality still missing

    // ContactFetchRequest with different filters
    QContactFetchRequest request;
    // Contactdetail with field, without value
    QContactDetailFilter detailFilter;
    detailFilter.setDetailType(QContactName::Type);
    request.setFilter(detailFilter);
    QString jsonDbQuery;
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?name exists]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
    // Contactdetail with field and value
    detailFilter.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    detailFilter.setValue("John");
    request.setFilter(detailFilter);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?name.firstName=\"John\"]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
    // Contactdetail with matchflags
    // Contains flag
    detailFilter.setMatchFlags(QContactFilter::MatchContains);
    request.setFilter(detailFilter);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?name.firstName=~\"/*John*/wi\"]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
    // Exactly flag
    detailFilter.setMatchFlags(QContactFilter::MatchExactly);
    request.setFilter(detailFilter);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?name.firstName=\"John\"]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
    // Starts with flag
    detailFilter.setMatchFlags(QContactFilter::MatchStartsWith);
    request.setFilter(detailFilter);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?name.firstName =~ \"/John*/wi\"]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
    // Ends with flag
    detailFilter.setMatchFlags(QContactFilter::MatchEndsWith);
    request.setFilter(detailFilter);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?name.firstName =~ \"/*John/wi\"]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();

    // Detail range filtering
    // Functionality still missing

    // contactId filtering
    QContactIdFilter idFilter;
    QUuid filterUuid("123");
    QString expectedUuid = filterUuid.toString();
    QContactJsonDbId *engineId = new QContactJsonDbId(filterUuid, QContactAbstractRequest::UserDataStorage);
    QContactId testId (engineId);
    QList<QContactId> ids;
    ids.append(testId);
    idFilter.setIds(ids);
    request.setFilter(idFilter);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?_uuid in [\"" + expectedUuid + "\"]]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();

    // Sortings
    QContactSortOrder sort;
    // Sort by firstname, descending
    sort.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    sort.setDirection(Qt::DescendingOrder);
    request.setSorting(QList<QContactSortOrder>() << sort);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?_uuid in [\"" + expectedUuid + "\"]][\\name.firstName]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
    // Sort by lastname, ascending
    sort.setDetailType(QContactName::Type, QContactName::FieldLastName);
    sort.setDirection(Qt::AscendingOrder);
    request.setSorting(QList<QContactSortOrder>() << sort);
    QVERIFY(converter.queryFromRequest(&request,jsonDbQuery));
    QCOMPARE(jsonDbQuery,
             QString("[?_type=\"%1\"][?_uuid in [\"" + expectedUuid + "\"]][/name.lastName]").arg(QContactJsonDbStr::contactsJsonDbType()));
    jsonDbQuery.clear();
}


void tst_QcontactJsondbConverter::convertCompoundFilterTest()
{
    QContactJsonDbConverter converter;

    // prepare ContactId filter
    QContactIdFilter idFilter;
    QUuid filterUuid("123");
    QString expectedUuid = filterUuid.toString();
    QContactJsonDbId *engineId = new QContactJsonDbId(filterUuid, QContactAbstractRequest::UserDataStorage);
    QContactId testId (engineId);
    QList<QContactId> ids;
    ids.append(testId);
    idFilter.setIds(ids);

    //prepare detail filter
    QContactDetailFilter detailFilter;
    detailFilter.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    detailFilter.setValue("John");

    //prepare Intersection of contactIdFilter and detailFilter
    QContactIntersectionFilter isf;
    isf.append(detailFilter);
    isf.append(idFilter);

    // now convert the Intersection Filter to Jsondb query
    QContactFilter filter(isf);
    QString filterStr;
    QVERIFY(converter.compoundFilterToJsondbQuery(filter,filterStr));
    QCOMPARE(filterStr,
             QString("[?name.firstName=\"John\"][?_uuid in [\"" + expectedUuid + "\"]]"));
    filterStr.clear();
}


void tst_QcontactJsondbConverter::convertSortOrderTest()
{
    // Functionality still missing
}

void tst_QcontactJsondbConverter::contactIdToUuidTest()
{
    QContactJsonDbConverter converter;
    QUuid uuid("123");
    QContactJsonDbId *engineId = new QContactJsonDbId(uuid, QContactAbstractRequest::UserDataStorage);
    QContactId qid(engineId);
    QString jsonUuid;
    jsonUuid = converter.contactIdToUuid(qid);
    QCOMPARE(jsonUuid, uuid.toString());
}

void tst_QcontactJsondbConverter::testJsonDetailItems(const QJsonObject& values, const QString& extractField,
                                                      const QMap<QString, QString>& fields, bool testSize)
{
    // result should be type field and value field
    QCOMPARE(values.size(), 2);
    QCOMPARE(values.value(QContactJsonDbStr::type()).toString(), QContactJsonDbStr::contactsJsonDbType());
    // extract data values
    QJsonObject jsonTmp;

    if (values.value(extractField).isArray()) {
        QJsonArray array = values.value(extractField).toArray();
        jsonTmp = array[0].toObject();
    } else {
        jsonTmp = values.value(extractField).toObject();
    }
    // extract data should have as many fields as compared data
    if(testSize)
        QCOMPARE(jsonTmp.size(), fields.size());
    QMapIterator<QString, QString> i(fields);
    while(i.hasNext()) {
        i.next();
        QCOMPARE(jsonTmp.value(i.key()).toString(), i.value());
    }
}

void tst_QcontactJsondbConverter::initializeJsonContact(QJsonObject& jsonContact, unsigned int numbering)
{
    jsonContact = QJsonObject();
    jsonContact.insert(QContactJsonDbStr::type(), QContactJsonDbStr::contactsJsonDbType());
    jsonContact.insert(QContactJsonDbStr::uuid(), QString("123"));
    QJsonObject jsonData;
    QString number("");
    if(numbering > 0) {
        number = QString().setNum(numbering);
    }
    jsonData.insert("firstName", QString("John" + number));
    jsonData.insert("lastName", QString("Doe" + number));
    jsonData.insert("middleName", QString("Tom" + number));
    jsonContact.insert(QContactJsonDbStr::nameDefinitionName(), jsonData);
}

QString tst_QcontactJsondbConverter::convertToISODate(QString date)
{
    QDateTime dateTime;
    dateTime = QDateTime::fromString(date, Qt::ISODate).toUTC();
    return dateTime.toUTC().toString(Qt::ISODate);
}


QTEST_APPLESS_MAIN(tst_QcontactJsondbConverter);

#include "tst_qcontactjsondbconverter.moc"
