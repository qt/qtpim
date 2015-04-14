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

#include <QtTest/QtTest>
#include <QDateTime>
#include <QtContacts>

#include "qcontactmanagerdataholder.h"

QTCONTACTS_USE_NAMESPACE

// to get QFETCH to work with the template expression...
typedef QMap<QString,QString> tst_QContactManagerDetails_QStringMap;
Q_DECLARE_METATYPE(tst_QContactManagerDetails_QStringMap)


class tst_QContactManagerDetails : public QObject
{
Q_OBJECT

public:
    tst_QContactManagerDetails();
    virtual ~tst_QContactManagerDetails();

private:
    void addManagers();

    template <typename T>
    void removeDetail( QContact& contact )
    {
        T detail = contact.detail<T>();
        contact.removeDetail( &detail );
    }

    bool saveAndLoadContact( QContactManager *cm, QContact &original, QContact &loaded );
    void saveAndVerifyContact( QContactManager *cm, QContact &original );

    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testAddress();
    void testAddress_data() {addManagers();}
    void testAnniversary();
    void testAnniversary_data() {addManagers();}
    void testAvatar();
    void testAvatar_data() {addManagers();}
    void testBirthday();
    void testBirthday_data() {addManagers();}
    void testDisplayLabel();
    void testDisplayLabel_data() {addManagers();}
    void testEmailAddress();
    void testEmailAddress_data() {addManagers();}
    void testEmptyExtendedDetail();
    void testEmptyExtendedDetail_data() {addManagers();}
    void testExtendedDetail();
    void testExtendedDetail_data() {addManagers();}
    void testGuid();
    void testGuid_data() {addManagers();}
    void testName();
    void testName_data() {addManagers();}
    void testNickName();
    void testNickName_data() {addManagers();}
    void testOrganisation();
    void testOrganisation_data() {addManagers();}
    void testOnlineAccount();
    void testOnlineAccount_data() {addManagers();}
    void testPhoneNumber();
    void testPhoneNumber_data() {addManagers();}
    void testSyncTarget();
    void testSyncTarget_data() {addManagers();}
    void testUrl();
    void testUrl_data() {addManagers();}
    void testRingtone();
    void testRingtone_data() {addManagers();}

private slots:

};

tst_QContactManagerDetails::tst_QContactManagerDetails()
{
}

tst_QContactManagerDetails::~tst_QContactManagerDetails()
{
}

void tst_QContactManagerDetails::addManagers()
{
    QTest::addColumn<QString>("uri");

    QStringList managers = QContactManager::availableManagers();

    /* Known one that will not pass */
    managers.removeAll("invalid");
    managers.removeAll("testdummy");
    managers.removeAll("teststaticdummy");
    managers.removeAll("maliciousplugin");

    foreach(QString mgr, managers) {
        QMap<QString, QString> params;
        QTest::newRow(QString("mgr='%1'").arg(mgr).toLatin1().constData()) << QContactManager::buildUri(mgr, params);
    }
}

void tst_QContactManagerDetails::initTestCase()
{
    managerDataHolder.reset(new QContactManagerDataHolder());
}

void tst_QContactManagerDetails::cleanupTestCase()
{
    managerDataHolder.reset(0);
}

bool tst_QContactManagerDetails::saveAndLoadContact( QContactManager *cm, QContact &original, QContact &loaded )
{
    // Save contact
    if( cm->saveContact(&original) == false )
        return false;

    // Check the id
    if (original.id().isNull())
        return false;

    // Load same contact from database
    loaded = cm->contact(original.id());
    if( cm->error() )
        return false;

    // Ignore some details which are not relevant and will mess
    // up direct comparison between two contacts.
    removeDetail<QContactGuid>(original);
    removeDetail<QContactGuid>(loaded);
    removeDetail<QContactTimestamp>(original);
    removeDetail<QContactTimestamp>(loaded);
    removeDetail<QContactVersion>(loaded);

    return true;
}

void tst_QContactManagerDetails::saveAndVerifyContact( QContactManager *cm, QContact &original )
{
    QContact loaded;
    QVERIFY( saveAndLoadContact( cm, original, loaded ) );
    if (original != loaded) {
        qDebug() << "expected: " << original;
        qDebug() << "loaded: " << loaded;
        QCOMPARE( loaded.details().count(), original.details().count() );
        QCOMPARE( loaded, original );
    }
}

void tst_QContactManagerDetails::testAddress()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactAddress a;
    a.setStreet("street");
    a.setLocality("locality");
    a.setRegion("region");
    a.setPostcode("postcode");
    a.setCountry("country");
    a.setPostOfficeBox("POBox");
    c.saveDetail( &a );

    saveAndVerifyContact( cm.data(), c );
    QContact c1;

    QContactAddress a1;
    a1.setStreet("street1");
    a1.setLocality("locality1");
    a1.setRegion("region1");
    a1.setPostcode("postcode1");
    a1.setCountry("country1");
    a1.setPostOfficeBox("POBox1");
    c1.saveDetail( &a1 );

    QContactAddress a2;
    a2.setStreet("street2");
    a2.setLocality("locality2");
    a2.setRegion("region2");
    a2.setPostcode("postcode2");
    a2.setCountry("country2");
    a2.setPostOfficeBox("POBox2");
    c1.saveDetail( &a2 );

    QContact c2;
    saveAndLoadContact( cm.data(), c1, c2 );

    QList<QContactAddress> addresses = c2.details<QContactAddress>();
    QCOMPARE(addresses.count(), 2);
    QCOMPARE(addresses[0], a1);
    QCOMPARE(addresses[1], a2);

    QContact c3;

    // General address
    QContactAddress a3;
    a3.setStreet("street1");
    c3.saveDetail( &a3 );

    // home address
    QContactAddress a4;
    a4.setStreet("street2");
    a4.setContexts( QContactDetail::ContextHome );
    c3.saveDetail( &a4 );

    // work address
    QContactAddress a5;
    a5.setStreet("street3");
    a5.setContexts( QContactDetail::ContextWork );
    c3.saveDetail( &a5 );

    saveAndVerifyContact( cm.data(), c3 );
}

void tst_QContactManagerDetails::testAnniversary()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;
    QContactAnniversary a;

    a.setOriginalDate( QDate(2009,9,9) );
    c.saveDetail( &a );
    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testAvatar()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactAvatar a;
    a.setImageUrl( QUrl::fromLocalFile("c:\\foo\\bar.jpeg") );
    c.saveDetail( &a );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testBirthday()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactBirthday b;
    b.setDateTime( QDateTime(QDate(2001,1,1)) );
    c.saveDetail( &b );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testEmailAddress()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactEmailAddress e;
    e.setEmailAddress( "foo@bar.com" );
    c.saveDetail( &e );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testName()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    QContactName n;
    n.setPrefix( "prefix" );
    n.setFirstName( "first" );
    n.setMiddleName( "middle" );
    n.setLastName( "last" );
    n.setSuffix( "suffix" );
    c.saveDetail( &n );
    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testDisplayLabel()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact originalContact;

    QContactDisplayLabel dl1;
    dl1.setLabel("displayLabel1");
    originalContact.saveDetail(&dl1);

    saveAndVerifyContact(cm.data(), originalContact);

    QContact loadedContact;
    QVERIFY(saveAndLoadContact(cm.data(), originalContact, loadedContact));
    QCOMPARE(loadedContact.details().count(), originalContact.details().count());
    QCOMPARE(loadedContact, originalContact);
}

void tst_QContactManagerDetails::testNickName()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact originalContact;

    QContactNickname n1;
    n1.setNickname("nickname1");
    originalContact.saveDetail(&n1);

    saveAndVerifyContact(cm.data(), originalContact);

    QContactNickname n2;
    n2.setNickname("nickname2");
    originalContact.saveDetail(&n2);

    QContact loadedContact;
    QVERIFY(saveAndLoadContact(cm.data(), originalContact, loadedContact));
    QCOMPARE(loadedContact.details().count(), originalContact.details().count());
    QCOMPARE(loadedContact, originalContact);
}

void tst_QContactManagerDetails::testOrganisation()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactOrganization o;
    o.setName( "Foreign legion" );
    o.setTitle( "Bicycle mechanic" );
    c.saveDetail( &o );

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testOnlineAccount()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    QContactOnlineAccount o;
    o.setAccountUri( "john@example.com" );
    o.setProtocol(QContactOnlineAccount::ProtocolJabber);
    c.saveDetail( &o );
    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testPhoneNumber()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    // General landline number
    QContactPhoneNumber n2;
    n2.setNumber( "2" );
    n2.setSubTypes( QList<int>() << QContactPhoneNumber::SubTypeLandline );
    c.saveDetail( &n2 );

    saveAndVerifyContact( cm.data(), c );

    QContact c2;

    // home mobile number
    QContactPhoneNumber n1;
    n1.setNumber( "1" );
    n1.setSubTypes( QList<int>() << QContactPhoneNumber::SubTypeMobile );
    n1.setContexts( QContactDetail::ContextHome );
    c2.saveDetail( &n1 );

    // work landline number
    QContactPhoneNumber n3;
    n3.setNumber( "3" );
    n3.setSubTypes( QList<int>() << QContactPhoneNumber::SubTypeLandline );
    n3.setContexts( QContactDetail::ContextWork );
    c2.saveDetail( &n3 );

    saveAndVerifyContact( cm.data(), c2 );
}

void tst_QContactManagerDetails::testSyncTarget()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    QContactSyncTarget syncTarget;
    syncTarget.setSyncTarget("TestSyncTarget" );
    c.saveDetail( &syncTarget );
    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testUrl()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact originalContact;

    QContactUrl u;
    u.setUrl("http://failblog.org");
    u.setSubType(QContactUrl::SubTypeHomePage);
    originalContact.saveDetail(&u);

    QContact loadedContact;
    QVERIFY(saveAndLoadContact(cm.data(), originalContact, loadedContact));
    QCOMPARE( loadedContact.details().count(), originalContact.details().count() );
    QCOMPARE( loadedContact, originalContact );
}

void tst_QContactManagerDetails::testRingtone()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QUrl rtUrl(QUrl::fromLocalFile("/home/user/sample.wav"));

    QContact c;
    QContactRingtone rt;
    rt.setAudioRingtoneUrl(rtUrl);
    c.saveDetail(&rt);

    saveAndVerifyContact( cm.data(), c );
}

void tst_QContactManagerDetails::testEmptyExtendedDetail()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    // Adding an empty extended detail
    QContactExtendedDetail emptyDetail;
    QVERIFY(c.saveDetail(&emptyDetail));
    saveAndVerifyContact(cm.data(), c);
}

void tst_QContactManagerDetails::testExtendedDetail()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact c;

    QContactExtendedDetail extendedDetail1, extendedDetail2;
    extendedDetail1.setName("testExtDetailVariantList");
    extendedDetail1.setData(QVariantList() << QString("QString in the QVariantlist detail.")
              << QVariant(1)
              << QString("Another QString in the QVariantlist detail.")
              << 2);
    QVERIFY(c.saveDetail(&extendedDetail1));
    saveAndVerifyContact(cm.data(), c);

    // Adding two extended details to same contact
    QContact c2;
    extendedDetail2.setName("testExtDetailInt");
    extendedDetail2.setData(2);
    QVERIFY (c2.saveDetail(&extendedDetail2));
    QVERIFY (c2.saveDetail(&extendedDetail1));
    saveAndVerifyContact(cm.data(), c2);

    // Adding same details but in different order
    QContact c3;
    QVERIFY (c3.saveDetail(&extendedDetail1));
    QVERIFY (c3.saveDetail(&extendedDetail2));
    saveAndVerifyContact(cm.data(), c3);
}

void tst_QContactManagerDetails::testGuid()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QContact c;

    QContactGuid guid;
    guid.setGuid("TestGUID" );
    c.saveDetail( &guid );
    saveAndVerifyContact( cm.data(), c );
}

QTEST_MAIN(tst_QContactManagerDetails)
#include "tst_qcontactmanagerdetails.moc"
