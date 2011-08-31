#include <QStringList>
#include <private/jsondb-connection_p.h>
#include <private/jsondb-strings_p.h>

#include "qcontactjsondbbackup.h"
#include "qcontactjsondbconverter.h"

QContactJsonDbBackup::QContactJsonDbBackup()
{
//qDebug() << "INITIAL data: " << m_backupData;
int dummyValue = 0;
    backupJsonDb();
//qDebug() << "Backed-up data: " << m_backupData;
//dummyValue=wasteSomeTime();
//qDebug() << "Dummyvalue:" << dummyValue;
    clearJsonDb();
//dummyValue=wasteSomeTime();
//qDebug() << "Dummyvalue:" << dummyValue;
/*
    revertJsonDb();
qDebug() << "Database after restore: " << m_backupData;
dummyValue=wasteSomeTime();
qDebug() << "Dummyvalue:" << dummyValue;
*/

 // TODO(?): loadTestData() and the revertJsonDb have issues. It looks like the Uuid field creates problems when trying to add/restore contacts: Illegal result 2 "New object should not have _uuid" 
// Using a new version for the time being...
    loadTestData();
}





QContactJsonDbBackup::~QContactJsonDbBackup()
{
/*
    clearJsonDb();
    revertJsonDb();
*/
}





bool QContactJsonDbBackup::loadTestData() {
/*
    QStringList uuids;
    QStringList firstNames;
    QStringList surNames;
    QStringList emails;
    QStringList phones;

    uuids << "aaaaaa" << "bbbbbb" << "cccccc" << "dddddd" << "eeeeee" << "ffffff";
    firstNames << "Adam" << "Broc" << "Carey" << "David" << "Eric" << "Frank";
    surNames << "Cianciarulo" << "Glover" << "Hart" << "Pingree" << "Eggens" << "Latham";
    emails << "adam.cianciarulo@mx.com" << "broc.glover@mx.com" << "carey.hart@mx.com"
           << "david.pingree@mx.com" << "eric.eggens@mx.com" << "frank.latham@mx.com";
    phones << "1111111" << "2222222" << "3333333" << "4444444" << "5555555" << "6666666";

    QVariantList contacts;
    for(int i = 0; i < uuids.size(); ++i) {
        QVariantMap map;
        //map.insert(JsonDbString::kUuidStr, uuids.at(i));
        map.insert(JsonDbString::kTypeStr, "Contact");
        map.insert(JsonDbString::kVersionStr, "V0" + uuids.at(i));
        QVariantMap name;
        name.insert("firstName", firstNames.at(i));
        name.insert("lastName", surNames.at(i));
        map.insert("name", name);
        QVariantMap email;
        email.insert("subType", "home");
        email.insert("value", emails.at(i));
        map.insert("emails", email);
        QVariantMap phone;
        phone.insert("subType", "home");
        phone.insert("value", phones.at(i));
        map.insert("phoneNumbers", phone);
        map.insert("photoUuid", "0");
        contacts << map;
    }
    return doRequest(contacts, true);
*/

/*
    for (int i = 0; i < numContacts; i++) {
        QContact c;
        QContactName name;
        name.setFirstName("John");
        name.setMiddleName(QString::number(i));
        name.setLastName("Doe");
        QContactPhoneNumber phone;
        QString number = "555-100"+QString::number(i);
        phone.setNumber(number);
        c.saveDetail(&name);
        c.saveDetail(&phone);
        contacts.append(c);
    }
*/

    QContactManager cm;
    QContactSaveRequest csr;

    // save new contacts
    int originalCount = 5;
    QContact testContact;
    QContactName nameDetail = testContact.detail<QContactName>();
    nameDetail.setFirstName("Harry");
    nameDetail.setLastName("Baker");
    testContact.saveDetail(&nameDetail);
    QContactEmailAddress emailDetail;
    emailDetail.setEmailAddress("Harry.Baker@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    QContactPhoneNumber phoneNumberDetail;
    phoneNumberDetail.setSubTypes(QContactPhoneNumber::SubTypeMobile);
    phoneNumberDetail.setNumber("+358507654321");
    testContact.saveDetail(&phoneNumberDetail);
    QList<QContact> saveList;
    saveList << testContact;

    nameDetail.setFirstName("Paul");
    nameDetail.setLastName("Thomson");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Paul.Thomson@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(QContactPhoneNumber::SubTypeMobile);
    phoneNumberDetail.setNumber("+358507654321");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    nameDetail.setFirstName("Julie");
    nameDetail.setLastName("King");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Julie.King@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(QContactPhoneNumber::SubTypeMobile);
    phoneNumberDetail.setNumber("+3585012345");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    nameDetail.setFirstName("Angelina");
    nameDetail.setLastName("Row");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Angelina.Row@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(QContactPhoneNumber::SubTypeMobile);
    phoneNumberDetail.setNumber("+3585012345");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    nameDetail.setFirstName("Natalie");
    nameDetail.setLastName("Watson");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Natalie.Watson@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(QContactPhoneNumber::SubTypeMobile);
    phoneNumberDetail.setNumber("+358507654321");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    csr.setManager(QContactManager::fromUri(cm.managerUri()));
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    //csr.setContact(testContact);
    csr.setContacts(saveList);
    csr.start();
    csr.waitForFinished();
    csr.isFinished();

    QList<QContact> expected = csr.contacts();
    return true;
}





bool QContactJsonDbBackup::backupJsonDb() {
    //QString query = "[?_type=\"Contact\"]";
    QString query = "[?_type=\"com.nokia.mp.contacts.Contact\"]";
    QVariantMap map = JsonDbConnection::makeQueryRequest(query);
    map = JsonDbConnection::instance()->sync(map).value<QVariantMap>();
    m_backupData = map["data"].value<QVariantList>();
    return true;
}





bool QContactJsonDbBackup::clearJsonDb() {
    return doRequest(m_backupData, false);
}




bool QContactJsonDbBackup::revertJsonDb() {
    return doRequest(m_backupData, true);
    m_backupData.clear();
}




bool QContactJsonDbBackup::doRequest(const QVariantList& objects, bool isInsert) {
    QVariantMap item;
    QString query;
    QVariantMap map;
    for(int i = 0; i < objects.size(); ++i) {
        item = objects[i].value<QVariantMap>();
        if(isInsert) {
            map = JsonDbConnection::makeCreateRequest(item);
        } else {
            map = JsonDbConnection::makeRemoveRequest(item);
        }
        JsonDbConnection::instance()->sync(map);
    }
    return true;
}


int QContactJsonDbBackup::wasteSomeTime() {
    int x=0;
    int y=0;
    for (int i=0; i<30; i++)
    {
        for (int j=0; j<100000000; j++)
        {
            y=(j*j);
        }
        x=i+y;
    }
    return x;
}
