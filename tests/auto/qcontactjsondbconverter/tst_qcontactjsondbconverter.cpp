#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QVariantMap>
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
#include <private/jsondb-strings_p.h>

#include "qcontactjsondbconverter.h"
#include "qcontactjsondbengine.h"

#define JSONDB_CONTACT_TYPE "Contact"


class tst_QcontactJsondbConverter : public QObject
{
    Q_OBJECT
public:
    tst_QcontactJsondbConverter();
private Q_SLOTS:
    void toQContactTest();
    void toQContactsTest();
    void toJsonContactTest();
    void updateContextsTest();
    void queryFromRequestTest();
    void convertFilterTest();
    void convertSortOrderTest();
    void convertIdTest();
private:
    void testJsonDetailItems(const QVariantMap& values, const QString& extractField,
                             const QMap<QString, QString>& fields, bool testSize = true);
    void initializeJsonContact(QVariantMap& jsonContact, unsigned int numbering = 0);
};





tst_QcontactJsondbConverter::tst_QcontactJsondbConverter()
{
}





void tst_QcontactJsondbConverter::toQContactTest()
{
    QContactJsonDbConverter converter;
    QContactJsonDbEngine engine;
    QVariantMap jsonContact;
    QContact contact;
    QVariantMap jsonData;
    QContactDetail detail;

    // test name (set in initialization)
    initializeJsonContact(jsonContact);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactName::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactName* name = static_cast<QContactName*>(&detail);
    QVERIFY(name != NULL);
    QCOMPARE(name->firstName(), QString("John"));
    QCOMPARE(name->lastName(), QString("Doe"));
    QCOMPARE(name->middleName(), QString("Tom"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // gender
    initializeJsonContact(jsonContact);
    jsonData.insert("gender", "Male");
    jsonContact.insert("gender", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactGender::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactGender* gender = static_cast<QContactGender*>(&detail);
    QVERIFY(gender != NULL);
    QCOMPARE(gender->gender(), QString(QContactGender::GenderMale.latin1()));
    // cleanup
    jsonData.clear();
    contact.clearDetails();

    
    // organization
    initializeJsonContact(jsonContact);
    jsonData.insert("name", "ACME");
    jsonData.insert("department", "Spy");
    jsonData.insert("title", "Vice President");
    jsonData.insert("role", "Superhero");
    jsonData.insert("assistantName", "Daisy Duck");
    jsonData.insert("logoUrl", "http://www.acme.com/logo.jpg");
    jsonContact.insert("organization", QVariantList() << jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactOrganization::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactOrganization* org = static_cast<QContactOrganization*>(&detail);
    QVERIFY(org != NULL);
    QCOMPARE(org->name(), QString("ACME"));
    QCOMPARE(org->department().at(0), QString("Spy"));
    QCOMPARE(org->title(), QString("Vice President"));
    QCOMPARE(org->role(), QString("Superhero"));
    QCOMPARE(org->assistantName(), QString("Daisy Duck"));
    QCOMPARE(org->logoUrl(), QUrl("http://www.acme.com/logo.jpg"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();
    

    // birthday
    initializeJsonContact(jsonContact);
    jsonData.insert("birthday", "1979-11-22T00:00:00");
    jsonContact.insert("contactDetails", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactBirthday::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactBirthday* bd = static_cast<QContactBirthday*>(&detail);
    QVERIFY(bd != NULL);
    QString dateStr("%1-%2-%3");
    dateStr = dateStr.arg(bd->date().year()).arg(bd->date().month()).arg(bd->date().day());
    QCOMPARE(dateStr, QString("1979-11-22"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // avatar
    initializeJsonContact(jsonContact);
    jsonData.insert("photoUrl", "http://www.acme.com/logo.jpg");
    jsonContact.insert("contactDetails", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactAvatar::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactAvatar* avatar = static_cast<QContactAvatar*>(&detail);
    QVERIFY(avatar != NULL);
    QCOMPARE(avatar->imageUrl(), QUrl("http://www.acme.com/logo.jpg"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // ringtone
    initializeJsonContact(jsonContact);
    jsonData.insert("ringtoneUrl", "http://www.acme.com/ring.mp3");
    jsonContact.insert("contactDetails", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactRingtone::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactRingtone* ring = static_cast<QContactRingtone*>(&detail);
    QVERIFY(ring != NULL);
    QCOMPARE(ring->audioRingtoneUrl(), QUrl("http://www.acme.com/ring.mp3"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // nickname
    initializeJsonContact(jsonContact);
    jsonData.insert("nickname", "Chupacabra");
    jsonContact.insert("contactDetails", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactNickname::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactNickname* nick = static_cast<QContactNickname*>(&detail);
    QVERIFY(nick != NULL);
    QCOMPARE(nick->nickname(), QString("Chupacabra"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // note
    initializeJsonContact(jsonContact);
    jsonData.insert("note", "test note");
    jsonContact.insert("contactDetails", jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactNote::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactNote* note = static_cast<QContactNote*>(&detail);
    QVERIFY(note != NULL);
    QCOMPARE(note->note(), QString("test note"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // email
    initializeJsonContact(jsonContact);
    jsonData.insert("value", "john@doe.com");
    jsonData.insert("subType", "home");
    jsonContact.insert("emails", QVariantList() << jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactEmailAddress::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactEmailAddress* email = static_cast<QContactEmailAddress*>(&detail);
    QVERIFY(email != NULL);
    QCOMPARE(email->emailAddress(), QString("john@doe.com"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // phone number
    initializeJsonContact(jsonContact);
    jsonData.insert("value", "0507654321");
    jsonData.insert("subType", "home");
    jsonContact.insert("phones", QVariantList() << jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactPhoneNumber::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactPhoneNumber* phone = static_cast<QContactPhoneNumber*>(&detail);
    QVERIFY(phone != NULL);
    QCOMPARE(phone->number(), QString("0507654321"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();


    // address
    initializeJsonContact(jsonContact);
    jsonData.insert("country", "Finland");
    jsonData.insert("postOfficeBox", "347");
    jsonData.insert("postcode", "33101");
    jsonData.insert("locality", "Tampere");
    jsonData.insert("region", "Pirkanmaa");
    jsonData.insert("street", "PL 347");
    jsonContact.insert("addresses", QVariantList() << jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactAddress::DefinitionName);
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
    jsonData.clear();
    contact.clearDetails();


    // url
    initializeJsonContact(jsonContact);
    jsonData.insert("value", "http://www.acme.com");
    jsonContact.insert("urls", QVariantList() << jsonData);
    QVERIFY(converter.toQContact(jsonContact, &contact, engine));
    detail = contact.detail(QContactUrl::DefinitionName);
    QVERIFY(!detail.isEmpty());
    QContactUrl* url = static_cast<QContactUrl*>(&detail);
    QVERIFY(url != NULL);
    QCOMPARE(url->url(), QString("http://www.acme.com"));
    // cleanup
    jsonData.clear();
    contact.clearDetails();
}





void tst_QcontactJsondbConverter::toQContactsTest()
{
    QVariantList contactList;
    QVariantMap contact;
    // create bunch of contacts and convert those
    for(int i = 1; i < 10; ++i) {
        //initializeJsonContact();

        contact.clear();
    }
}





void tst_QcontactJsondbConverter::toJsonContactTest()
{
    QContactJsonDbConverter converter;
    QVariantMap jsonContact;
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
    testJsonDetailItems(jsonContact, JsonDbString::kNameStr, testFields);
    // cleanup
    contact.clearDetails();
    jsonContact.clear();
    testFields.clear();


    // Test Gender
    QContactGender gender;
    gender.setGender("Male");
    contact.saveDetail(&gender);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("gender", "Male");
    // test fields
    testJsonDetailItems(jsonContact, "gender", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact.clear();
    testFields.clear();


    // Test organization
    QContactOrganization organization;
    organization.setName("ACME");
    organization.setDepartment(QStringList() << "Spy");
    organization.setTitle("Vice President");
    organization.setRole("Superhero");
    organization.setAssistantName("Daisy Duck");
    organization.setLogoUrl(QUrl("http://www.acme.com/logo.jpg"));
    organization.setContexts("test context");
    contact.saveDetail(&organization);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("name", "ACME");
    testFields.insert("department", "Spy");
    testFields.insert("title", "Vice President");
    testFields.insert("role", "Superhero");
    testFields.insert("assistantName", "Daisy Duck");
    testFields.insert("logoUrl", "http://www.acme.com/logo.jpg");
    testFields.insert("context", "test context");
    // test fields
    testJsonDetailItems(jsonContact, "organization", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact.clear();
    testFields.clear();


    // Test contact details
    // birthday
    QContactBirthday birthday;
    birthday.setDate(QDate(1979, 11, 22));
    contact.saveDetail(&birthday);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("birthday", "1979-11-22T00:00:00");
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
    testJsonDetailItems(jsonContact, "contactDetails", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact.clear();
    testFields.clear();


    // email
    QContactEmailAddress email;
    email.setEmailAddress("john@doe.com");
    email.setContexts("test context");
    contact.saveDetail(&email);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("value", "john@doe.com");
    testFields.insert("context", "test context");
    testJsonDetailItems(jsonContact, "emails", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact.clear();
    testFields.clear();


    // phone number
    QContactPhoneNumber number;
    number.setNumber("0507654321");
    contact.saveDetail(&number);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    testFields.insert("value", "0507654321");
    testJsonDetailItems(jsonContact, "phones", testFields);
    //cleanup
    contact.clearDetails();
    jsonContact.clear();
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
    jsonContact.clear();
    testFields.clear();


    // url
    QContactUrl url;
    url.setUrl("http://www.acme.com");
    contact.saveDetail(&url);
    QVERIFY(converter.toJsonContact(&jsonContact, contact));
    // test fields
    testFields.insert("value", "http://www.acme.com");
    testJsonDetailItems(jsonContact, "urls", testFields);
    // cleanup
    contact.clearDetails();
    jsonContact.clear();
    testFields.clear();


    /* Tested:
       +QContactAddress,
        QContactAnniversary,
       +QContactAvatar,
       +QContactBirthday,
        QContactDisplayLabel,
       +QContactEmailAddress,
        QContactFamily,
        QContactFavorite,
       +QContactGender,
        QContactGeoLocation,
        QContactGlobalPresence,
        QContactGuid,
        QContactHobby,
       +QContactName,
       +QContactNickname,
       +QContactNote,
        QContactOnlineAccount,
       +QContactOrganization,
       +QContactPhoneNumber,
        QContactPresence,
       +QContactRingtone,
        QContactSyncTarget,
        QContactTag,
        QContactThumbnail,
        QContactTimestamp,
        QContactType,
       +QContactUrl.
     */
}





void tst_QcontactJsondbConverter::updateContextsTest()
{
}





void tst_QcontactJsondbConverter::queryFromRequestTest()
{
}





void tst_QcontactJsondbConverter::convertFilterTest()
{
}





void tst_QcontactJsondbConverter::convertSortOrderTest()
{
}





void tst_QcontactJsondbConverter::convertIdTest()
{
}





void tst_QcontactJsondbConverter::testJsonDetailItems(const QVariantMap& values, const QString& extractField,
                                                      const QMap<QString, QString>& fields, bool testSize)
{
    // result should be type field and value field
    QCOMPARE(values.size(), 2);
    QCOMPARE(values[JsonDbString::kTypeStr].value<QString>(),
             QString(JSONDB_CONTACT_TYPE));
    // extract data values
    QVariantMap jsonTmp;

    // ############
    QList<QString> keys = values.keys();
    QString j;
    foreach(j, keys) {
        //qDebug(j.toAscii());
    }
    // ############

    if(values[extractField].type() == QMetaType::QVariantList) {
       QVariantList list = values[extractField].value<QVariantList>();
       jsonTmp = list[0].value<QVariantMap>();
    } else {
       jsonTmp = values[extractField].value<QVariantMap>();
    }
    // extract data should have as many fields as compared data
    if(testSize)
        QCOMPARE(jsonTmp.size(), fields.size());
    QMapIterator<QString, QString> i(fields);
    while(i.hasNext()) {
        i.next();
        QCOMPARE(jsonTmp[i.key()].value<QString>(), i.value());
    }
}





void tst_QcontactJsondbConverter::initializeJsonContact(QVariantMap& jsonContact, unsigned int numbering)
{
    jsonContact.clear();
    jsonContact.insert(JsonDbString::kTypeStr, QString(JSONDB_CONTACT_TYPE));
    jsonContact.insert(JsonDbString::kUuidStr, "123");
    QVariantMap jsonData;
    QString number("");
    if(numbering > 0) {
        number = QString().setNum(numbering);
    }
    jsonData.insert("firstName", "John" + number);
    jsonData.insert("lastName", "Doe" + number);
    jsonData.insert("middleName", "Tom" + number);
    jsonContact.insert(JsonDbString::kNameStr, jsonData);
}


QTEST_APPLESS_MAIN(tst_QcontactJsondbConverter);

#include "tst_qcontactjsondbconverter.moc"
