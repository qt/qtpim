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

#include <QtContacts/qcontacts.h>
#include <QtContacts/QContactDetail>

//TESTED_COMPONENT=src/contacts

QTCONTACTS_USE_NAMESPACE
class tst_QContactDetails : public QObject
{
Q_OBJECT

public:
    tst_QContactDetails();
    virtual ~tst_QContactDetails();

public slots:
    void init();
    void cleanup();

private slots:
    // leaf class testing
    void address();
    void anniversary();
    void avatar();
    void birthday();
    void displayLabel();
    void emailAddress();
    void extendedDetail();
    void family();
    void favorite();
    void gender();
    void geolocation();
    void globalPresence();
    void guid();
    void hobby();
    void name();
    void nickname();
    void note();
    void onlineAccount();
    void organization();
    void phoneNumber();
    void presence();
    void ringtone();
    void syncTarget();
    void tag();
    void timestamp();
    void type();
    void url();
    void version();
};

tst_QContactDetails::tst_QContactDetails()
{
}

tst_QContactDetails::~tst_QContactDetails()
{
}

void tst_QContactDetails::init()
{
}

void tst_QContactDetails::cleanup()
{
}

void tst_QContactDetails::address()
{
    QContact c;
    QContactAddress a1, a2;

    QVERIFY(a1.isEmpty());
    QVERIFY(a2.isEmpty());

    // test property set
    QCOMPARE(a1.street(), QString());
    a1.setStreet("68 Imaginary Avenue");
    a1.setCountry("Australia");
    QCOMPARE(a1.street(), QString("68 Imaginary Avenue"));
    QCOMPARE(a1.country(), QString("Australia"));
    a1.setStreet("1234");
    QCOMPARE(a1.street(), QString("1234"));
    a1.setPostOfficeBox("PO Box 5678");
    QCOMPARE(a1.postOfficeBox(), QString("PO Box 5678"));


    // Sub types
    a1.setSubTypes(QList<int>() << QContactAddress::SubTypeDomestic);
    QCOMPARE(a1.subTypes(), QList<int>() << QContactAddress::SubTypeDomestic);

    QList<int> sl;
    sl << QContactAddress::SubTypeParcel << QContactAddress::SubTypePostal;
    a1.setSubTypes(sl);
    QCOMPARE(a1.subTypes(), sl);

    // test property add
    QVERIFY(c.saveDetail(&a1));
    QCOMPARE(c.details(QContactAddress::Type).count(), 1);
    QCOMPARE(QContactAddress(c.details(QContactAddress::Type).value(0)).street(), a1.street());
    a2.setStreet("Test");
    QVERIFY(c.saveDetail(&a2));
    QCOMPARE(c.details(QContactAddress::Type).count(), 2);

    // test property update
    a1.setValue(QContactAddress::FieldContext, QContactDetail::ContextWork);
    a1.setStreet("12345");
    QVERIFY(c.saveDetail(&a1));

    // test property remove
    QVERIFY(c.removeDetail(&a1));
    QCOMPARE(c.details(QContactAddress::Type).count(), 1);
    QVERIFY(c.removeDetail(&a2));
    QVERIFY(!c.removeDetail(&a2)); // cannot remove twice
    QCOMPARE(c.details(QContactAddress::Type).count(), 0);

    QContactAddress a3;

    // set street
    a3.setStreet("Test");
    QCOMPARE(a3.street(), QString("Test"));
    a3.setStreet("");

    // set locality
    a3.setLocality("Test");
    QCOMPARE(a3.locality(), QString("Test"));
    a3.setLocality("");

    // set region
    a3.setRegion("Test");
    QCOMPARE(a3.region(), QString("Test"));
    a3.setRegion("");

    // set postcode
    a3.setPostcode("Test");
    QCOMPARE(a3.postcode(), QString("Test"));
    a3.setPostcode("");

    // set country
    a3.setCountry("Test");
    QCOMPARE(a3.country(), QString("Test"));
    a3.setCountry("");

    // subtypes
    a3.setSubTypes(QList<int>() << QContactAddress::SubTypeDomestic);
    QCOMPARE(a3.subTypes(), QList<int>() << QContactAddress::SubTypeDomestic);
    a3.setSubTypes(QList<int>() << QContactAddress::SubTypeInternational << QContactAddress::SubTypePostal);
    QCOMPARE(a3.subTypes(), QList<int>() << QContactAddress::SubTypeInternational << QContactAddress::SubTypePostal);

    // test comparison of two addresses
    QContactAddress a4;
    a4.setStreet("test");
    a4.setLocality("test");
    a4.setRegion("test");
    a4.setCountry("test");
    a4.setSubTypes(QList<int>() << QContactAddress::SubTypeInternational << QContactAddress::SubTypePostal);
    QContactAddress a5;
    a5.setStreet("test");
    a5.setLocality("test");
    a5.setRegion("test");
    a5.setCountry("test");
    a5.setSubTypes(QList<int>() << QContactAddress::SubTypeInternational << QContactAddress::SubTypePostal);
    QVERIFY(a4 == a5);
    a4.setCountry("");
    QVERIFY(a4 != a5);
    a4.setCountry("test");
    QVERIFY(a4 == a5);
    a5.setPostcode("test");
    QVERIFY(a4 != a5);
    a4.setPostcode("test");
    QVERIFY(a4 == a5);
    a4.setSubTypes(QList<int>() << QContactAddress::SubTypeDomestic);
    QVERIFY(a4 != a5);
    a5.setSubTypes(QList<int>() << QContactAddress::SubTypePostal);
    QVERIFY(a4 != a5);
    a5.setSubTypes(QList<int>() << QContactAddress::SubTypeDomestic);
    QVERIFY(a4 == a5);
}

void tst_QContactDetails::anniversary()
{
    QContact c;
    QContactAnniversary a1, a2;
    QDateTime currDateTime = QDateTime::currentDateTime();
    QDate currDate = currDateTime.date();
    QDateTime snippedDateTime = QDateTime(currDate);

    // test property set
    a1.setCalendarId("1234");
    QCOMPARE(a1.calendarId(), QString("1234"));
    QCOMPARE(a1.value(QContactAnniversary::FieldCalendarId).toString(), QString("1234"));
    a1.setEvent("4321");
    QCOMPARE(a1.event(), QString("4321"));
    QCOMPARE(a1.value(QContactAnniversary::FieldEvent).toString(), QString("4321"));
    a1.setSubType(QContactAnniversary::SubTypeWedding);
    QCOMPARE(a1.subType(), QContactAnniversary::SubTypeWedding);
    QCOMPARE(static_cast<QContactAnniversary::SubType>(a1.value<int>(QContactAnniversary::FieldSubType)), QContactAnniversary::SubTypeWedding);

    a1.setOriginalDate(currDate);
    QCOMPARE(a1.originalDate(), currDate);
    QCOMPARE(a1.originalDateTime(), snippedDateTime);
    QCOMPARE(a1.value<QDate>(QContactAnniversary::FieldOriginalDate), currDate);
    QCOMPARE(a1.value<QDateTime>(QContactAnniversary::FieldOriginalDate), snippedDateTime);

    a1.setOriginalDateTime(currDateTime);
    QCOMPARE(a1.originalDate(), currDate);
    QCOMPARE(a1.originalDateTime(), currDateTime);
    QCOMPARE(a1.value<QDate>(QContactAnniversary::FieldOriginalDate), currDate);
    QCOMPARE(a1.value<QDateTime>(QContactAnniversary::FieldOriginalDate), currDateTime);

    // test property add
    QVERIFY(c.saveDetail(&a1));
    QCOMPARE(c.details(QContactAnniversary::Type).count(), 1);
    QCOMPARE(QContactAnniversary(c.details(QContactAnniversary::Type).value(0)).event(), a1.event());

    // test property update
    a1.setValue(QContactAnniversary::FieldContext, QContactDetail::ContextWork);
    a1.setCalendarId("12345");
    QVERIFY(c.saveDetail(&a1));
    QCOMPARE(c.details(QContactAnniversary::Type).value(0).value(QContactAnniversary::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactAnniversary::Type).value(0).value(QContactAnniversary::FieldCalendarId).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&a1));
    QCOMPARE(c.details(QContactAnniversary::Type).count(), 0);
    QVERIFY(c.saveDetail(&a2));
    QCOMPARE(c.details(QContactAnniversary::Type).count(), 1);
    QVERIFY(c.removeDetail(&a2));
    QCOMPARE(c.details(QContactAnniversary::Type).count(), 0);
    QVERIFY(c.removeDetail(&a2) == false);
    QCOMPARE(c.details(QContactAnniversary::Type).count(), 0);
}

void tst_QContactDetails::avatar()
{
    QContact c;
    QContactAvatar a1, a2;

    // test property set
    a1.setImageUrl(QUrl("1234"));
    QCOMPARE(a1.imageUrl(), QUrl("1234"));
    QCOMPARE(a1.value<QUrl>(QContactAvatar::FieldImageUrl), QUrl("1234"));
    a2.setVideoUrl(QUrl("videoUrl"));
    a2.setImageUrl(QUrl("imageUrl"));
    a2.setMetaData(QString("1234"));
    QCOMPARE(a2.videoUrl(), QUrl("videoUrl"));
    QCOMPARE(a2.value<QUrl>(QContactAvatar::FieldVideoUrl), QUrl("videoUrl"));
    QCOMPARE(a2.imageUrl(), QUrl("imageUrl"));
    QCOMPARE(a2.value<QUrl>(QContactAvatar::FieldImageUrl), QUrl("imageUrl"));
    QCOMPARE(a2.metaData(), QString("1234"));
    QCOMPARE(a2.value<QString>(QContactAvatar::FieldMetaData), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&a1));
    QCOMPARE(c.details(QContactAvatar::Type).count(), 1);
    QCOMPARE(QContactAvatar(c.details(QContactAvatar::Type).value(0)).imageUrl(), a1.imageUrl());

    // test property update
    a1.setValue(QContactAvatar::FieldContext, QContactDetail::ContextWork);
    a1.setImageUrl(QUrl("12345"));
    QVERIFY(c.saveDetail(&a1));
    QCOMPARE(c.details(QContactAvatar::Type).value(0).value(QContactAvatar::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactAvatar::Type).value(0).value<QUrl>(QContactAvatar::FieldImageUrl), QUrl("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&a1));
    QCOMPARE(c.details(QContactAvatar::Type).count(), 0);
    QVERIFY(c.saveDetail(&a2));
    QCOMPARE(c.details(QContactAvatar::Type).count(), 1);
    QVERIFY(c.removeDetail(&a2));
    QCOMPARE(c.details(QContactAvatar::Type).count(), 0);
    QVERIFY(c.removeDetail(&a2) == false);
    QCOMPARE(c.details(QContactAvatar::Type).count(), 0);
}

void tst_QContactDetails::birthday()
{
    QContact c;
    QContactBirthday b1, b2, b3;
    QDateTime currDateTime = QDateTime::currentDateTime();
    QDate currDate = currDateTime.date();
    QDateTime snippedDateTime = QDateTime(currDate);

    // test property set
    b1.setDate(currDate);
    QCOMPARE(b1.date(), currDate);
    QCOMPARE(b1.dateTime(), snippedDateTime);
    QCOMPARE(b1.value<QDate>(QContactBirthday::FieldBirthday), currDate);
    QCOMPARE(b1.value<QDateTime>(QContactBirthday::FieldBirthday), snippedDateTime);

    b1.setDateTime(currDateTime);
    QCOMPARE(b1.date(), currDate);
    QCOMPARE(b1.dateTime(), currDateTime);
    QCOMPARE(b1.value<QDate>(QContactBirthday::FieldBirthday), currDate);
    QCOMPARE(b1.value<QDateTime>(QContactBirthday::FieldBirthday), currDateTime);

    b1.setCalendarId("1234");
    QCOMPARE(b1.calendarId(), QString("1234"));
    QCOMPARE(b1.value(QContactBirthday::FieldCalendarId).toString(), QString("1234"));

    b3.setDate(currDate);
    QCOMPARE(QContactDetail(b3).value(QContactBirthday::FieldBirthday), QVariant(currDate));
    b3.setDateTime(currDateTime);
    QCOMPARE(QContactDetail(b3).value(QContactBirthday::FieldBirthday), QVariant(currDateTime));
    b3.setDateTime(QDateTime(currDate, QTime(0,0,0,0)));
    QCOMPARE(QContactDetail(b3).value(QContactBirthday::FieldBirthday).type(), QVariant::Date);    // unfortunate but unavoidable with current impl.
    QCOMPARE(QContactDetail(b3).value<QDateTime>(QContactBirthday::FieldBirthday), b3.dateTime());

    // test property add
    QVERIFY(c.saveDetail(&b1));
    QCOMPARE(c.details(QContactBirthday::Type).count(), 1);
    QCOMPARE(QContactBirthday(c.details(QContactBirthday::Type).value(0)).date(), b1.date());

    // test property update
    b1.setValue(QContactBirthday::FieldContext, QContactDetail::ContextWork);
    b1.setDate(currDate.addDays(3));
    b1.setCalendarId("12345");
    QVERIFY(c.saveDetail(&b1));
    QCOMPARE(c.details(QContactBirthday::Type).value(0).value(QContactBirthday::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactBirthday::Type).value(0).value<QDate>(QContactBirthday::FieldBirthday), currDate.addDays(3));
    QCOMPARE(c.details(QContactBirthday::Type).value(0).value(QContactBirthday::FieldCalendarId).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&b1));
    QCOMPARE(c.details(QContactBirthday::Type).count(), 0);
    QVERIFY(c.saveDetail(&b2));
    QCOMPARE(c.details(QContactBirthday::Type).count(), 1);
    QVERIFY(c.removeDetail(&b2));
    QCOMPARE(c.details(QContactBirthday::Type).count(), 0);
    QVERIFY(c.removeDetail(&b2) == false);
    QCOMPARE(c.details(QContactBirthday::Type).count(), 0);
}

void tst_QContactDetails::displayLabel()
{
    QContact c;
    QContactDisplayLabel d1, d2;

    // test property set
    d1.setLabel("1234");
    QCOMPARE(d1.label(), QString("1234"));
    QCOMPARE(d1.value(QContactDisplayLabel::FieldLabel).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&d1));
    QCOMPARE(c.details(QContactDisplayLabel::Type).count(), 1);
    QCOMPARE(QContactDisplayLabel(c.details(QContactDisplayLabel::Type).value(0)).label(), d1.label());

    // test property update
    d1.setValue(QContactDisplayLabel::FieldContext, QContactDetail::ContextWork);
    d1.setLabel("12345");
    QVERIFY(c.saveDetail(&d1));
    QCOMPARE(c.details(QContactDisplayLabel::Type).value(0).value(QContactDisplayLabel::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactDisplayLabel::Type).value(0).value(QContactDisplayLabel::FieldLabel).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&d1));
    QCOMPARE(c.details(QContactDisplayLabel::Type).count(), 0);
    QVERIFY(c.saveDetail(&d2));
    QCOMPARE(c.details(QContactDisplayLabel::Type).count(), 1);
    QVERIFY(c.removeDetail(&d2));
    QCOMPARE(c.details(QContactDisplayLabel::Type).count(), 0);
    QVERIFY(c.removeDetail(&d2) == false);
    QCOMPARE(c.details(QContactDisplayLabel::Type).count(), 0);
}

void tst_QContactDetails::emailAddress()
{
    QContact c;
    QContactEmailAddress e1, e2;

    // test property set
    e1.setEmailAddress("1234");
    QCOMPARE(e1.emailAddress(), QString("1234"));
    QCOMPARE(e1.value(QContactEmailAddress::FieldEmailAddress).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&e1));
    QCOMPARE(c.details(QContactEmailAddress::Type).count(), 1);
    QCOMPARE(QContactEmailAddress(c.details(QContactEmailAddress::Type).value(0)).emailAddress(), e1.emailAddress());

    // test property update
    e1.setValue(QContactEmailAddress::FieldContext, QContactDetail::ContextWork);
    e1.setEmailAddress("12345");
    QVERIFY(c.saveDetail(&e1));
    QCOMPARE(c.details(QContactEmailAddress::Type).value(0).value(QContactEmailAddress::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactEmailAddress::Type).value(0).value(QContactEmailAddress::FieldEmailAddress).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&e1));
    QCOMPARE(c.details(QContactEmailAddress::Type).count(), 0);
    QVERIFY(c.saveDetail(&e2));
    QCOMPARE(c.details(QContactEmailAddress::Type).count(), 1);
    QVERIFY(c.removeDetail(&e2));
    QCOMPARE(c.details(QContactEmailAddress::Type).count(), 0);
    QVERIFY(c.removeDetail(&e2) == false);
    QCOMPARE(c.details(QContactEmailAddress::Type).count(), 0);
}

void tst_QContactDetails::extendedDetail()
{
    QContact c;
    QContactExtendedDetail extD1, extD2;
    QVariant v1;
    // test property set
    v1.setValue(1);
    extD1.setName("detail1");
    extD1.setValue(QContactExtendedDetail::FieldData, v1);

    QCOMPARE(extD1.name(), QString("detail1"));
    QCOMPARE(extD1.data(), v1);

    // test property add
    QVERIFY(c.saveDetail(&extD1));
    QCOMPARE(c.details(QContactExtendedDetail::Type).count(), 1);
    QCOMPARE(QContactExtendedDetail(c.details(QContactExtendedDetail::Type).value(0)).name(), extD1.name());

    // test property update
    extD1.setValue(QContactExtendedDetail::FieldContext, QContactDetail::ContextWork);
    extD1.setName("newDetail1");
    extD1.setData(v1);
    QVERIFY(c.saveDetail(&extD1));
    QCOMPARE(c.details(QContactExtendedDetail::Type).value(0).value(QContactExtendedDetail::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactExtendedDetail::Type).value(0).value(QContactExtendedDetail::FieldData).toInt(), v1.toInt());
    QCOMPARE(c.details(QContactExtendedDetail::Type).value(0).value(QContactExtendedDetail::FieldName).toString(), QString("newDetail1"));

    // test property remove
    QVERIFY(c.removeDetail(&extD1));
    QCOMPARE(c.details(QContactExtendedDetail::Type).count(), 0);
    extD2 = extD1;

    QVERIFY(c.saveDetail(&extD2));
    QCOMPARE(c.details(QContactExtendedDetail::Type).count(), 1);
    QVERIFY(c.removeDetail(&extD2));
    QCOMPARE(c.details(QContactExtendedDetail::Type).count(), 0);
    QVERIFY(c.removeDetail(&extD2) == false);
    QCOMPARE(c.details(QContactExtendedDetail::Type).count(), 0);
}

void tst_QContactDetails::family()
{
    QContact c;
    QContactFamily f1, f2;

    // test property set
    f1.setSpouse("1234");
    QCOMPARE(f1.spouse(), QString("1234"));
    QCOMPARE(f1.value(QContactFamily::FieldSpouse).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&f1));
    QCOMPARE(c.details(QContactFamily::Type).count(), 1);
    QCOMPARE(QContactFamily(c.details(QContactFamily::Type).value(0)).spouse(), f1.spouse());

    // test property update
    f1.setValue(QContactFamily::FieldContext, QContactDetail::ContextWork);
    f1.setSpouse("12345");
    f1.setChildren(QStringList("54321"));
    QVERIFY(c.saveDetail(&f1));
    QCOMPARE(c.details(QContactFamily::Type).value(0).value(QContactFamily::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactFamily::Type).value(0).value(QContactFamily::FieldSpouse).toString(), QString("12345"));
    QCOMPARE(c.details(QContactFamily::Type).value(0).value<QStringList>(QContactFamily::FieldChildren), QStringList("54321"));

    // test property remove
    f2.setSpouse("1111");
    QVERIFY(c.removeDetail(&f1));
    QCOMPARE(c.details(QContactFamily::Type).count(), 0);
    QVERIFY(c.saveDetail(&f2));
    QCOMPARE(c.details(QContactFamily::Type).count(), 1);
    QVERIFY(c.removeDetail(&f2));
    QCOMPARE(c.details(QContactFamily::Type).count(), 0);
    QVERIFY(c.removeDetail(&f2) == false);
    QCOMPARE(c.details(QContactFamily::Type).count(), 0);
}

void tst_QContactDetails::favorite()
{
    QContact c;
    QContactFavorite f1, f2;

    // first, ensure that the default value is "false"
    QCOMPARE(c.detail<QContactFavorite>().isFavorite(), false);
    QCOMPARE(c.detail<QContactFavorite>().index(), 0); // 0 being "no index" :-/  -1 would be better, but not default...
    QVERIFY(!f1.isFavorite());

    // test property set
    f1.setFavorite(true);
    QCOMPARE(f1.isFavorite(), true);
    QCOMPARE(f1.value<bool>(QContactFavorite::FieldFavorite), true);

    // test property add
    QVERIFY(c.saveDetail(&f1));
    QCOMPARE(c.details(QContactFavorite::Type).count(), 1);
    QCOMPARE(QContactFavorite(c.details(QContactFavorite::Type).value(0)).isFavorite(), f1.isFavorite());

    // test property update
    f1.setValue(QContactFavorite::FieldContext, QContactDetail::ContextWork);
    f1.setFavorite(false);
    f1.setIndex(5);
    QVERIFY(c.saveDetail(&f1));
    QCOMPARE(c.details(QContactFavorite::Type).value(0).value(QContactFavorite::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactFavorite::Type).value(0).value<bool>(QContactFavorite::FieldFavorite), false);
    QCOMPARE(c.details(QContactFavorite::Type).value(0).value<int>(QContactFavorite::FieldIndex), 5);
    QCOMPARE(c.details<QContactFavorite>().value(0).index(), 5);

    // test property remove
    f2.setFavorite(true);
    QVERIFY(c.removeDetail(&f1));
    QCOMPARE(c.details(QContactFavorite::Type).count(), 0);
    QVERIFY(c.saveDetail(&f2));
    QCOMPARE(c.details(QContactFavorite::Type).count(), 1);
    QVERIFY(c.removeDetail(&f2));
    QCOMPARE(c.details(QContactFavorite::Type).count(), 0);
    QVERIFY(c.removeDetail(&f2) == false);
    QCOMPARE(c.details(QContactFavorite::Type).count(), 0);
}

void tst_QContactDetails::gender()
{
    QContact c;
    QContactGender g1, g2;

    // test property set
    g1.setGender(QContactGender::GenderFemale);
    QCOMPARE(g1.gender(), QContactGender::GenderFemale);

    // test property add
    QVERIFY(c.saveDetail(&g1));
    QCOMPARE(c.details(QContactGender::Type).count(), 1);
    QCOMPARE(QContactGender(c.details(QContactGender::Type).value(0)).gender(), g1.gender());

    // test property update
    g1.setValue(QContactGender::FieldContext, QContactDetail::ContextWork);
    g1.setGender(QContactGender::GenderMale);
    QVERIFY(c.saveDetail(&g1));
    QCOMPARE(c.details(QContactGender::Type).value(0).value(QContactGender::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactGender::Type).value(0).value(QContactGender::FieldGender).toInt(), (int)QContactGender::GenderMale);

    // test property remove
    QVERIFY(c.removeDetail(&g1));
    QCOMPARE(c.details(QContactGender::Type).count(), 0);
    QVERIFY(c.saveDetail(&g2));
    QCOMPARE(c.details(QContactGender::Type).count(), 1);
    QVERIFY(c.removeDetail(&g2));
    QCOMPARE(c.details(QContactGender::Type).count(), 0);
    QVERIFY(c.removeDetail(&g2) == false);
    QCOMPARE(c.details(QContactGender::Type).count(), 0);
}

void tst_QContactDetails::geolocation()
{
    QContact c;
    QContactGeoLocation g1, g2;

    // test property set
    g1.setLabel("1234");
    QCOMPARE(g1.label(), QString("1234"));
    QCOMPARE(g1.value(QContactGeoLocation::FieldLabel).toString(), QString("1234"));
    g1.setAccuracy(3.2);
    QCOMPARE(g1.accuracy(), 3.2);
    QCOMPARE(g1.value(QContactGeoLocation::FieldAccuracy), QVariant(3.2));
    g1.setAltitude(3.3);
    QCOMPARE(g1.altitude(), 3.3);
    QCOMPARE(g1.value(QContactGeoLocation::FieldAltitude), QVariant(3.3));
    g1.setAltitudeAccuracy(3.4);
    QCOMPARE(g1.altitudeAccuracy(), 3.4);
    QCOMPARE(g1.value(QContactGeoLocation::FieldAltitudeAccuracy), QVariant(3.4));
    g1.setHeading(3.5);
    QCOMPARE(g1.heading(), 3.5);
    QCOMPARE(g1.value(QContactGeoLocation::FieldHeading), QVariant(3.5));
    g1.setLatitude(3.6);
    QCOMPARE(g1.latitude(), 3.6);
    QCOMPARE(g1.value(QContactGeoLocation::FieldLatitude), QVariant(3.6));
    g1.setLongitude(3.7);
    QCOMPARE(g1.longitude(), 3.7);
    QCOMPARE(g1.value(QContactGeoLocation::FieldLongitude), QVariant(3.7));
    QDateTime current = QDateTime::currentDateTime();
    g1.setTimestamp(current);
    QCOMPARE(g1.timestamp(), current);
    QCOMPARE(g1.value(QContactGeoLocation::FieldTimestamp), QVariant(current));
    g1.setSpeed(3.8);
    QCOMPARE(g1.speed(), 3.8);
    QCOMPARE(g1.value(QContactGeoLocation::FieldSpeed), QVariant(3.8));

    // test property add
    QVERIFY(c.saveDetail(&g1));
    QCOMPARE(c.details(QContactGeoLocation::Type).count(), 1);
    QCOMPARE(QContactGeoLocation(c.details(QContactGeoLocation::Type).value(0)).label(), g1.label());

    // test property update
    g1.setLabel("12345");
    QVERIFY(c.saveDetail(&g1));
    QCOMPARE(c.details(QContactGeoLocation::Type).value(0).value(QContactGeoLocation::FieldLabel).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&g1));
    QCOMPARE(c.details(QContactGeoLocation::Type).count(), 0);
    QVERIFY(c.saveDetail(&g2));
    QCOMPARE(c.details(QContactGeoLocation::Type).count(), 1);
    QVERIFY(c.removeDetail(&g2));
    QCOMPARE(c.details(QContactGeoLocation::Type).count(), 0);
    QVERIFY(c.removeDetail(&g2) == false);
    QCOMPARE(c.details(QContactGeoLocation::Type).count(), 0);
}

void tst_QContactDetails::globalPresence()
{
    QContact c;
    QContactGlobalPresence p1, p2;

    // test property set
    QDateTime ts = QDateTime::currentDateTime();
    p1.setTimestamp(ts);
    p1.setNickname(QString("nick"));
    p1.setPresenceState(QContactPresence::PresenceExtendedAway);
    p1.setPresenceStateText("1234");
    p1.setPresenceStateImageUrl(QUrl("http://example.com/someimage.png"));
    p1.setCustomMessage("gone fishing!");
    QCOMPARE(p1.timestamp(), ts);
    QCOMPARE(p1.value<QDateTime>(QContactGlobalPresence::FieldTimestamp), ts);
    QCOMPARE(p1.nickname(), QString("nick"));
    QCOMPARE(p1.value(QContactGlobalPresence::FieldNickname).toString(), QString("nick"));
    QCOMPARE(p1.presenceState(), QContactPresence::PresenceExtendedAway);
    QCOMPARE(p1.value<int>(QContactGlobalPresence::FieldPresenceState), static_cast<int>(QContactPresence::PresenceExtendedAway));
    QCOMPARE(p1.presenceStateText(), QString("1234"));
    QCOMPARE(p1.value(QContactGlobalPresence::FieldPresenceStateText).toString(), QString("1234"));
    QCOMPARE(p1.presenceStateImageUrl(), QUrl("http://example.com/someimage.png"));
    QCOMPARE(p1.value<QUrl>(QContactGlobalPresence::FieldPresenceStateImageUrl), QUrl("http://example.com/someimage.png"));
    QCOMPARE(p1.customMessage(), QString("gone fishing!"));
    QCOMPARE(p1.value(QContactGlobalPresence::FieldCustomMessage).toString(), QString("gone fishing!"));

    // test property add
    QVERIFY(c.saveDetail(&p1));
    QCOMPARE(c.details(QContactGlobalPresence::Type).count(), 1);
    QCOMPARE(QContactGlobalPresence(c.details(QContactGlobalPresence::Type).value(0)).presenceStateText(), p1.presenceStateText());

    // test property update
    p1.setValue(QContactGlobalPresence::FieldContext, QContactDetail::ContextWork);
    p1.setPresenceStateText("12345");
    QVERIFY(c.saveDetail(&p1));
    QCOMPARE(c.details(QContactGlobalPresence::Type).value(0).value(QContactGlobalPresence::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactGlobalPresence::Type).value(0).value(QContactGlobalPresence::FieldPresenceStateText).toString(), QString("12345"));

    p2.setTimestamp(ts);
    p2.setNickname(QString("test"));
    p2.setPresenceState(QContactPresence::PresenceAvailable);
    p2.setPresenceStateText("online");
    p2.setPresenceStateImageUrl(QUrl("http://example.com/someimage.png"));
    p2.setCustomMessage("C is for generic biscuit-type pastry product!");

    // now test that the setPresenceStateImageUrl function doesn't escape spaces and so forth.
    QString imgUrlStr(QStringLiteral("http://example.com/some image.png"));
    QUrl imgUrl(imgUrlStr);
    QContactPresence p3;
    p3.setPresenceStateImageUrl(imgUrl);
    QCOMPARE(p3.presenceStateImageUrl(), imgUrl);
    QCOMPARE(p3.presenceStateImageUrl().toString(), imgUrlStr);

    // test property remove
    QVERIFY(c.removeDetail(&p1));
    QCOMPARE(c.details(QContactGlobalPresence::Type).count(), 0);
    QVERIFY(c.saveDetail(&p2));
    QCOMPARE(c.details(QContactGlobalPresence::Type).count(), 1);
    QVERIFY(c.removeDetail(&p2));
    QCOMPARE(c.details(QContactGlobalPresence::Type).count(), 0);
    QVERIFY(c.removeDetail(&p2) == false);
    QCOMPARE(c.details(QContactGlobalPresence::Type).count(), 0);
}

void tst_QContactDetails::guid()
{
    QContact c;
    QContactGuid g1, g2;

    // test property set
    g1.setGuid("1234");
    QCOMPARE(g1.guid(), QString("1234"));
    QCOMPARE(g1.value(QContactGuid::FieldGuid).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&g1));
    QCOMPARE(c.details(QContactGuid::Type).count(), 1);
    QCOMPARE(QContactGuid(c.details(QContactGuid::Type).value(0)).guid(), g1.guid());

    // test property update
    g1.setValue(QContactGuid::FieldContext, QContactDetail::ContextWork);
    g1.setGuid("12345");
    QVERIFY(c.saveDetail(&g1));
    QCOMPARE(c.details(QContactGuid::Type).value(0).value(QContactGuid::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactGuid::Type).value(0).value(QContactGuid::FieldGuid).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&g1));
    QCOMPARE(c.details(QContactGuid::Type).count(), 0);
    QVERIFY(c.saveDetail(&g2));
    QCOMPARE(c.details(QContactGuid::Type).count(), 1);
    QVERIFY(c.removeDetail(&g2));
    QCOMPARE(c.details(QContactGuid::Type).count(), 0);
    QVERIFY(c.removeDetail(&g2) == false);
    QCOMPARE(c.details(QContactGuid::Type).count(), 0);
}

void tst_QContactDetails::hobby()
{
    QContact c;
    QContactHobby h1, h2;

    // test property set
    h1.setHobby("1234");
    QCOMPARE(h1.hobby(), QString("1234"));
    QCOMPARE(h1.value(QContactHobby::FieldHobby).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&h1));
    QCOMPARE(c.details(QContactHobby::Type).count(), 1);
    QCOMPARE(QContactHobby(c.details(QContactHobby::Type).value(0)).hobby(), h1.hobby());

    // test property update
    h1.setValue(QContactHobby::FieldContext, QContactDetail::ContextWork);
    h1.setHobby("12345");
    QVERIFY(c.saveDetail(&h1));
    QCOMPARE(c.details(QContactHobby::Type).value(0).value(QContactHobby::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactHobby::Type).value(0).value(QContactHobby::FieldHobby).toString(), QString("12345"));

    // test property remove
    h2.setHobby("1111");
    QVERIFY(c.removeDetail(&h1));
    QCOMPARE(c.details(QContactHobby::Type).count(), 0);
    QVERIFY(c.saveDetail(&h2));
    QCOMPARE(c.details(QContactHobby::Type).count(), 1);
    QVERIFY(c.removeDetail(&h2));
    QCOMPARE(c.details(QContactHobby::Type).count(), 0);
    QVERIFY(c.removeDetail(&h2) == false);
    QCOMPARE(c.details(QContactHobby::Type).count(), 0);
}

void tst_QContactDetails::name()
{
    QContact c;
    QContactName n1, n2;

    QVERIFY(n1.isEmpty());
    QVERIFY(n2.isEmpty());

    // test property set
    n1.setPrefix("Dr");
    n1.setFirstName("Freddy");
    n1.setMiddleName("William Preston");
    n1.setLastName("Gumboots");
    n1.setSuffix("Esquire");
    n1.setCustomLabel("Frederick The Example");
    QCOMPARE(n1.prefix(), QString("Dr"));
    QCOMPARE(n1.firstName(), QString("Freddy"));
    QCOMPARE(n1.middleName(), QString("William Preston"));
    QCOMPARE(n1.lastName(), QString("Gumboots"));
    QCOMPARE(n1.suffix(), QString("Esquire"));
    QCOMPARE(n1.customLabel(), QString("Frederick The Example"));

    // Values based (QString)
    QCOMPARE(n1.value(QContactName::FieldPrefix).toString(), QString("Dr"));
    QCOMPARE(n1.value(QContactName::FieldFirstName).toString(), QString("Freddy"));
    QCOMPARE(n1.value(QContactName::FieldMiddleName).toString(), QString("William Preston"));
    QCOMPARE(n1.value(QContactName::FieldLastName).toString(), QString("Gumboots"));
    QCOMPARE(n1.value(QContactName::FieldSuffix).toString(), QString("Esquire"));
    QCOMPARE(n1.value(QContactName::FieldCustomLabel).toString(), QString("Frederick The Example"));

    // test property add
    QVERIFY(c.saveDetail(&n1));
    QCOMPARE(c.details(QContactName::Type).count(), 1);
    n2.setFirstName("Billy");
    n2.setLastName("Galoshes");
    QVERIFY(c.saveDetail(&n2));
    QCOMPARE(c.details(QContactName::Type).count(), 2);

    // test property update
    n1.setValue(QContactName::FieldContext, QContactDetail::ContextWork);
    QVERIFY(c.saveDetail(&n1));

    // test property remove
    QVERIFY(c.removeDetail(&n1)); // remove
    QCOMPARE(c.details(QContactName::Type).count(), 1);
    QVERIFY(c.saveDetail(&n2));   // save but already added; count remains the same.
    QCOMPARE(c.details(QContactName::Type).count(), 1);
    QVERIFY(c.removeDetail(&n2)); // remove it
    QCOMPARE(c.details(QContactName::Type).count(), 0);
    QVERIFY(!c.removeDetail(&n2));// remove now fails
    QVERIFY(c.saveDetail(&n1));   // save the original name
    QCOMPARE(c.details(QContactName::Type).count(), 1);
}

void tst_QContactDetails::nickname()
{
    QContact c;
    QContactNickname n1, n2;

    // test property set
    n1.setNickname("1234");
    QCOMPARE(n1.nickname(), QString("1234"));
    QCOMPARE(n1.value(QContactNickname::FieldNickname).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&n1));
    QCOMPARE(c.details(QContactNickname::Type).count(), 1);
    QCOMPARE(QContactNickname(c.details(QContactNickname::Type).value(0)).nickname(), n1.nickname());

    // test property update
    n1.setValue(QContactNickname::FieldContext, QContactDetail::ContextWork);
    n1.setNickname("12345");
    QVERIFY(c.saveDetail(&n1));
    QCOMPARE(c.details(QContactNickname::Type).value(0).value(QContactNickname::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactNickname::Type).value(0).value(QContactNickname::FieldNickname).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&n1));
    QCOMPARE(c.details(QContactNickname::Type).count(), 0);
    QVERIFY(c.saveDetail(&n2));
    QCOMPARE(c.details(QContactNickname::Type).count(), 1);
    QVERIFY(c.removeDetail(&n2));
    QCOMPARE(c.details(QContactNickname::Type).count(), 0);
    QVERIFY(c.removeDetail(&n2) == false);
    QCOMPARE(c.details(QContactNickname::Type).count(), 0);
}

void tst_QContactDetails::note()
{
    QContact c;
    QContactNote n1, n2;

    // test property set
    n1.setNote("lorem ipsum");
    QCOMPARE(n1.note(), QString("lorem ipsum"));
    QCOMPARE(n1.value(QContactNote::FieldNote).toString(), QString("lorem ipsum"));

    // test property add
    QVERIFY(c.saveDetail(&n1));
    QCOMPARE(c.details(QContactNote::Type).count(), 1);
    QCOMPARE(QContactNote(c.details(QContactNote::Type).value(0)).note(), n1.note());

    // test property update
    n1.setValue(QContactNote::FieldContext, QContactDetail::ContextWork);
    n1.setNote("orange gypsum");
    QVERIFY(c.saveDetail(&n1));
    QCOMPARE(c.details(QContactNote::Type).value(0).value(QContactNote::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactNote::Type).value(0).value(QContactNote::FieldNote).toString(), QString("orange gypsum"));

    // test property remove
    QVERIFY(c.removeDetail(&n1));
    QCOMPARE(c.details(QContactNote::Type).count(), 0);
    QVERIFY(c.saveDetail(&n2));
    QCOMPARE(c.details(QContactNote::Type).count(), 1);
    QVERIFY(c.removeDetail(&n2));
    QCOMPARE(c.details(QContactNote::Type).count(), 0);
    QVERIFY(c.removeDetail(&n2) == false);
    QCOMPARE(c.details(QContactNote::Type).count(), 0);
}

void tst_QContactDetails::onlineAccount()
{
    QContact c;
    QContactOnlineAccount o1, o2;

    // test property set
    o1.setAccountUri("test@nokia.com");
    QCOMPARE(o1.accountUri(), QString("test@nokia.com"));
    QCOMPARE(o1.value(QContactOnlineAccount::FieldAccountUri).toString(), QString("test@nokia.com"));
    o1.setProtocol(QContactOnlineAccount::ProtocolJabber);
    QCOMPARE(o1.protocol(), QContactOnlineAccount::ProtocolJabber);

    // Sub types
    o1.setSubTypes(QList<int>() << QContactOnlineAccount::SubTypeSip);
    QCOMPARE(o1.subTypes(), QList<int>() << QContactOnlineAccount::SubTypeSip);

    QList<int> sl;
    sl << QContactOnlineAccount::SubTypeImpp << QContactOnlineAccount::SubTypeVideoShare;
    o1.setSubTypes(sl);
    QCOMPARE(o1.subTypes(), sl);

    o1 = QContactOnlineAccount();
    o1.setAccountUri("test@nokia.com");

    // test property add
    QVERIFY(c.saveDetail(&o1));
    QCOMPARE(c.details(QContactOnlineAccount::Type).count(), 1);
    QCOMPARE(QContactOnlineAccount(c.details(QContactOnlineAccount::Type).value(0)).accountUri(), o1.accountUri());
    QCOMPARE(QContactOnlineAccount(c.details(QContactOnlineAccount::Type).value(0)).accountUri(), o1.accountUri());

    // test property update
    QStringList caps;
    caps << "cap1" << "cap3" << "cap4";
    o1.setValue(QContactOnlineAccount::FieldContext, QContactDetail::ContextWork);
    o1.setAccountUri("test2@nokia.com");
    o1.setServiceProvider("some provider");
    o1.setCapabilities(caps);
    QVERIFY(c.saveDetail(&o1));
    QCOMPARE(c.details(QContactOnlineAccount::Type).value(0).value(QContactOnlineAccount::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactOnlineAccount::Type).value(0).value(QContactOnlineAccount::FieldAccountUri).toString(), QString("test2@nokia.com"));
    QCOMPARE(c.details(QContactOnlineAccount::Type).value(0).value<QStringList>(QContactOnlineAccount::FieldCapabilities), caps);
    QCOMPARE(c.details<QContactOnlineAccount>().value(0).capabilities(), caps);
    QCOMPARE(c.details(QContactOnlineAccount::Type).value(0).value(QContactOnlineAccount::FieldServiceProvider).toString(), QString("some provider"));
    QCOMPARE(c.details<QContactOnlineAccount>().value(0).serviceProvider(), QString("some provider"));

    // test property remove
    QVERIFY(c.removeDetail(&o1));
    QCOMPARE(c.details(QContactOnlineAccount::Type).count(), 0);
    QVERIFY(c.saveDetail(&o2));
    QCOMPARE(c.details(QContactOnlineAccount::Type).count(), 1);
    QVERIFY(c.removeDetail(&o2));
    QCOMPARE(c.details(QContactOnlineAccount::Type).count(), 0);
    QVERIFY(c.removeDetail(&o2) == false);
    QCOMPARE(c.details(QContactOnlineAccount::Type).count(), 0);
}

void tst_QContactDetails::organization()
{
    QContact c;
    QContactOrganization o1, o2;

    // test property set
    o1.setName("organization one");
    QCOMPARE(o1.name(), QString("organization one"));
    QCOMPARE(o1.value(QContactOrganization::FieldName).toString(), QString("organization one"));

    o1.setDepartment(QStringList(QString("department one")));
    QCOMPARE(o1.department(), QStringList(QString("department one")));
    QCOMPARE(o1.value<QStringList>(QContactOrganization::FieldDepartment), QStringList(QString("department one")));

    o1.setLocation("location one");
    QCOMPARE(o1.location(), QString("location one"));
    QCOMPARE(o1.value(QContactOrganization::FieldLocation).toString(), QString("location one"));

    o1.setLogoUrl(QUrl("logo one"));
    QCOMPARE(o1.logoUrl(), QUrl("logo one"));
    QCOMPARE(o1.value<QUrl>(QContactOrganization::FieldLogoUrl), QUrl("logo one"));

    o1.setTitle("title one");
    QCOMPARE(o1.title(), QString("title one"));
    QCOMPARE(o1.value(QContactOrganization::FieldTitle).toString(), QString("title one"));

    o1.setAssistantName("assistant one");
    QCOMPARE(o1.assistantName(), QString("assistant one"));
    QCOMPARE(o1.value(QContactOrganization::FieldAssistantName).toString(), QString("assistant one"));

    // test property add
    QVERIFY(c.saveDetail(&o1));
    QCOMPARE(c.details(QContactOrganization::Type).count(), 1);
    QCOMPARE(QContactOrganization(c.details(QContactOrganization::Type).value(0)).name(), o1.name());

    // test property update
    o1.setName("organization two");
    QVERIFY(c.saveDetail(&o1));
    QCOMPARE(c.details(QContactOrganization::Type).value(0).value(QContactOrganization::FieldName).toString(), QString("organization two"));

    // test property remove
    QVERIFY(c.removeDetail(&o1));
    QCOMPARE(c.details(QContactOrganization::Type).count(), 0);
    QVERIFY(c.saveDetail(&o2));
    QCOMPARE(c.details(QContactOrganization::Type).count(), 1);
    QVERIFY(c.removeDetail(&o2));
    QCOMPARE(c.details(QContactOrganization::Type).count(), 0);
    QVERIFY(c.removeDetail(&o2) == false);
    QCOMPARE(c.details(QContactOrganization::Type).count(), 0);

    // organization-specific API testing
    o1.setDepartment(QStringList(QString("Imaginary Dept")));
    o1.setLocation("Utopia");
    o1.setLogoUrl(QUrl("logo.png"));
    o1.setName("Utopian Megacorporation");
    o1.setTitle("Generic Employee");
    c.saveDetail(&o1);
    QVERIFY(c.detail(QContactOrganization::Type).value<QStringList>(QContactOrganization::FieldDepartment) == QStringList(QString("Imaginary Dept")));
    QVERIFY(c.detail(QContactOrganization::Type).value(QContactOrganization::FieldLocation).toString() == QString("Utopia"));
    QVERIFY(c.detail(QContactOrganization::Type).value<QUrl>(QContactOrganization::FieldLogoUrl) == QUrl("logo.png"));
    QVERIFY(c.detail(QContactOrganization::Type).value(QContactOrganization::FieldName).toString() == QString("Utopian Megacorporation"));
    QVERIFY(c.detail(QContactOrganization::Type).value(QContactOrganization::FieldTitle).toString() == QString("Generic Employee"));
}

void tst_QContactDetails::phoneNumber()
{
    QContact c;
    QContactPhoneNumber p1, p2;

    // test property set
    p1.setNumber("1234");
    QCOMPARE(p1.number(), QString("1234"));
    QCOMPARE(p1.value(QContactPhoneNumber::FieldNumber).toString(), QString("1234"));

    // normalized number
    p1.setNormalizedNumber("123");
    QCOMPARE(p1.normalizedNumber(), QString("123"));
    QCOMPARE(p1.value(QContactPhoneNumber::FieldNormalizedNumber).toString(), QString("123"));

    // Sub types
    p1.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeCar);
    QCOMPARE(p1.subTypes(), QList<int>() << QContactPhoneNumber::SubTypeCar);

    QList<int> sl;
    sl << QContactPhoneNumber::SubTypeMobile << QContactPhoneNumber::SubTypeFax;
    p1.setSubTypes(sl);
    QCOMPARE(p1.subTypes(), sl);

    // test property add
    QVERIFY(c.saveDetail(&p1));
    QCOMPARE(c.details(QContactPhoneNumber::Type).count(), 1);
    QCOMPARE(QContactPhoneNumber(c.details(QContactPhoneNumber::Type).value(0)).number(), p1.number());

    // test property update
    p1.setValue(QContactPhoneNumber::FieldContext, QContactDetail::ContextWork);
    p1.setNumber("12345");
    QVERIFY(c.saveDetail(&p1));
    QCOMPARE(c.details(QContactPhoneNumber::Type).value(0).value(QContactPhoneNumber::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactPhoneNumber::Type).value(0).value(QContactPhoneNumber::FieldNumber).toString(), QString("12345"));

    p1.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeDtmfMenu);
    c.saveDetail(&p1);

    QCOMPARE(c.detail(QContactPhoneNumber::Type).value<QList<int> >(QContactPhoneNumber::FieldSubTypes), QList<int>() << QContactPhoneNumber::SubTypeDtmfMenu);
    p1.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeModem << QContactPhoneNumber::SubTypeFax);
    c.saveDetail(&p1);
    QVERIFY(c.detail(QContactPhoneNumber::Type).value<QList<int> >(QContactPhoneNumber::FieldSubTypes) == p1.subTypes());

    // test property remove
    QVERIFY(c.removeDetail(&p1));
    QCOMPARE(c.details(QContactPhoneNumber::Type).count(), 0);
    QVERIFY(c.saveDetail(&p2));
    QCOMPARE(c.details(QContactPhoneNumber::Type).count(), 1);
    QVERIFY(c.removeDetail(&p2));
    QCOMPARE(c.details(QContactPhoneNumber::Type).count(), 0);
    QVERIFY(c.removeDetail(&p2) == false);
    QCOMPARE(c.details(QContactPhoneNumber::Type).count(), 0);

    // test comparison of two phone numbers
    QContactPhoneNumber p3;
    p3.setNumber("1234");
    p3.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeModem << QContactPhoneNumber::SubTypeFax);
    QContactPhoneNumber p4;
    p4.setNumber("1234");
    p4.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeModem << QContactPhoneNumber::SubTypeFax);
    QVERIFY(p3 == p4);
    p3.setNumber("");
    QVERIFY(p3 != p4);
    p3.setNumber("1234");
    QVERIFY(p3 == p4);
    p4.setNormalizedNumber("1234");
    QVERIFY(p3 != p4);
    p3.setNormalizedNumber("1234");
    QVERIFY(p3 == p4);
    p3.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeModem);
    QVERIFY(p3 != p4);
    p4.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeDtmfMenu);
    QVERIFY(p3 != p4);
    p3.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeDtmfMenu);
    QVERIFY(p3 == p4);
}

void tst_QContactDetails::presence()
{
    QContact c;
    QContactPresence p1, p2;

    // test property set
    QDateTime ts = QDateTime::currentDateTime();
    p1.setTimestamp(ts);
    p1.setNickname(QString("nick"));
    p1.setPresenceState(QContactPresence::PresenceExtendedAway);
    p1.setPresenceStateText("1234");
    p1.setPresenceStateImageUrl(QUrl("http://example.com/someimage.png"));
    p1.setCustomMessage("gone fishing!");
    QCOMPARE(p1.timestamp(), ts);
    QCOMPARE(p1.value<QDateTime>(QContactPresence::FieldTimestamp), ts);
    QCOMPARE(p1.nickname(), QString("nick"));
    QCOMPARE(p1.value(QContactPresence::FieldNickname).toString(), QString("nick"));
    QCOMPARE(p1.presenceState(), QContactPresence::PresenceExtendedAway);
    QCOMPARE(p1.value<int>(QContactPresence::FieldPresenceState), static_cast<int>(QContactPresence::PresenceExtendedAway));
    QCOMPARE(p1.presenceStateText(), QString("1234"));
    QCOMPARE(p1.value(QContactPresence::FieldPresenceStateText).toString(), QString("1234"));
    QCOMPARE(p1.presenceStateImageUrl(), QUrl("http://example.com/someimage.png"));
    QCOMPARE(p1.value<QUrl>(QContactPresence::FieldPresenceStateImageUrl), QUrl("http://example.com/someimage.png"));
    QCOMPARE(p1.customMessage(), QString("gone fishing!"));
    QCOMPARE(p1.value(QContactPresence::FieldCustomMessage).toString(), QString("gone fishing!"));

    // test property add
    QVERIFY(c.saveDetail(&p1));
    QCOMPARE(c.details(QContactPresence::Type).count(), 1);
    QCOMPARE(QContactPresence(c.details(QContactPresence::Type).value(0)).presenceStateText(), p1.presenceStateText());

    // test property update
    p1.setValue(QContactPresence::FieldContext, QContactDetail::ContextWork);
    p1.setPresenceStateText("12345");
    QVERIFY(c.saveDetail(&p1));
    QCOMPARE(c.details(QContactPresence::Type).value(0).value(QContactPresence::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactPresence::Type).value(0).value(QContactPresence::FieldPresenceStateText).toString(), QString("12345"));

    p2.setTimestamp(ts);
    p2.setNickname(QString("test"));
    p2.setPresenceState(QContactPresence::PresenceAvailable);
    p2.setPresenceStateText("online");
    p2.setPresenceStateImageUrl(QUrl("http://example.com/someimage.png"));
    p2.setCustomMessage("C is for generic biscuit-type pastry product!");

    // test property remove
    QVERIFY(c.removeDetail(&p1));
    QCOMPARE(c.details(QContactPresence::Type).count(), 0);
    QVERIFY(c.saveDetail(&p2));
    QCOMPARE(c.details(QContactPresence::Type).count(), 1);
    QVERIFY(c.removeDetail(&p2));
    QCOMPARE(c.details(QContactPresence::Type).count(), 0);
    QVERIFY(c.removeDetail(&p2) == false);
    QCOMPARE(c.details(QContactPresence::Type).count(), 0);
}

void tst_QContactDetails::ringtone()
{
    QContact c;
    QContactRingtone r1, r2;

    // test property set
    r1.setAudioRingtoneUrl(QUrl("audioUrl"));
    QCOMPARE(r1.audioRingtoneUrl(), QUrl("audioUrl"));
    QCOMPARE(r1.value<QUrl>(QContactRingtone::FieldAudioRingtoneUrl), QUrl("audioUrl"));

    // and the other fields
    r2.setVideoRingtoneUrl(QUrl("videoUrl"));
    QCOMPARE(r2.videoRingtoneUrl(), QUrl("videoUrl"));
    QCOMPARE(r2.value<QUrl>(QContactRingtone::FieldVideoRingtoneUrl), QUrl("videoUrl"));
    r2.setVibrationRingtoneUrl(QUrl("vibrationUrl"));
    QCOMPARE(r2.vibrationRingtoneUrl(), QUrl("vibrationUrl"));
    QCOMPARE(r2.value<QUrl>(QContactRingtone::FieldVibrationRingtoneUrl), QUrl("vibrationUrl"));

    // test property add
    QVERIFY(c.saveDetail(&r1));
    QCOMPARE(c.details(QContactRingtone::Type).count(), 1);
    QCOMPARE(QContactRingtone(c.details(QContactRingtone::Type).value(0)).audioRingtoneUrl(), r1.audioRingtoneUrl());

    // test property update
    r1.setValue(QContactRingtone::FieldContext, QContactDetail::ContextWork);
    r1.setAudioRingtoneUrl(QUrl("audioUrl2"));
    QVERIFY(c.saveDetail(&r1));
    QCOMPARE(c.details(QContactRingtone::Type).value(0).value(QContactRingtone::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactRingtone::Type).value(0).value<QUrl>(QContactRingtone::FieldAudioRingtoneUrl), QUrl("audioUrl2"));

    // test property remove
    QVERIFY(c.removeDetail(&r1));
    QCOMPARE(c.details(QContactRingtone::Type).count(), 0);
    QVERIFY(c.saveDetail(&r2));
    QCOMPARE(c.details(QContactRingtone::Type).count(), 1);
    QVERIFY(c.removeDetail(&r2));
    QCOMPARE(c.details(QContactRingtone::Type).count(), 0);
    QVERIFY(c.removeDetail(&r2) == false);
    QCOMPARE(c.details(QContactRingtone::Type).count(), 0);
}

void tst_QContactDetails::syncTarget()
{
    QContact c;
    QContactSyncTarget s1, s2;

    // test property set
    s1.setSyncTarget("1234");
    QCOMPARE(s1.syncTarget(), QString("1234"));
    QCOMPARE(s1.value(QContactSyncTarget::FieldSyncTarget).toString(), QString("1234"));

    // test property add
    QVERIFY(c.saveDetail(&s1));
    QCOMPARE(c.details(QContactSyncTarget::Type).count(), 1);
    QCOMPARE(QContactSyncTarget(c.details(QContactSyncTarget::Type).value(0)).syncTarget(), s1.syncTarget());

    // test property update
    s1.setValue(QContactSyncTarget::FieldContext, QContactDetail::ContextWork);
    s1.setSyncTarget("12345");
    QVERIFY(c.saveDetail(&s1));
    QCOMPARE(c.details(QContactSyncTarget::Type).value(0).value(QContactSyncTarget::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactSyncTarget::Type).value(0).value(QContactSyncTarget::FieldSyncTarget).toString(), QString("12345"));

    // test property remove
    QVERIFY(c.removeDetail(&s1));
    QCOMPARE(c.details(QContactSyncTarget::Type).count(), 0);
    QVERIFY(c.saveDetail(&s2));
    QCOMPARE(c.details(QContactSyncTarget::Type).count(), 1);
    QVERIFY(c.removeDetail(&s2));
    QCOMPARE(c.details(QContactSyncTarget::Type).count(), 0);
    QVERIFY(c.removeDetail(&s2) == false);
    QCOMPARE(c.details(QContactSyncTarget::Type).count(), 0);
}

void tst_QContactDetails::tag()
{
    QContact c;
    QContactTag t1, t2;

    // test property set
    t1.setTag("red");
    QCOMPARE(t1.tag(), QString("red"));
    QCOMPARE(t1.value(QContactTag::FieldTag).toString(), QString("red"));

    // test property add
    QVERIFY(c.saveDetail(&t1));
    QCOMPARE(c.details(QContactTag::Type).count(), 1);
    QCOMPARE(QContactTag(c.details(QContactTag::Type).value(0)).tag(), t1.tag());
    QVERIFY(c.saveDetail(&t2));
    QCOMPARE(c.details(QContactTag::Type).count(), 2);

    // test property update
    t1.setValue(QContactTag::FieldContext, QContactDetail::ContextWork);
    t1.setTag("green");
    QVERIFY(c.saveDetail(&t1));
    QCOMPARE(c.details(QContactTag::Type).value(0).value(QContactTag::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactTag::Type).value(0).value(QContactTag::FieldTag).toString(), QString("green"));

    // test property remove
    QVERIFY(c.removeDetail(&t1));
    QCOMPARE(c.details(QContactTag::Type).count(), 1);
    QVERIFY(c.removeDetail(&t2));
    QCOMPARE(c.details(QContactTag::Type).count(), 0);
    QVERIFY(c.removeDetail(&t2) == false);
    QCOMPARE(c.details(QContactTag::Type).count(), 0);
}

void tst_QContactDetails::timestamp()
{
    QContact c;
    QContactTimestamp t1, t2;
    QDateTime modified = QDateTime::currentDateTime();
    QDateTime created = modified.addSecs(-43);
    QDateTime deleted = modified.addSecs(20);

    // test property set
    t1.setCreated(created);
    t1.setDeleted(deleted);
    QCOMPARE(t1.created(), created);
    QCOMPARE(t1.value(QContactTimestamp::FieldCreationTimestamp).toDateTime(), created);
    QCOMPARE(t1.deleted(), deleted);
    QCOMPARE(t1.value(QContactTimestamp::FieldDeletionTimestamp).toDateTime(), deleted);

    // test property add
    QVERIFY(c.saveDetail(&t1));
    QCOMPARE(c.details(QContactTimestamp::Type).count(), 1);
    QCOMPARE(QContactTimestamp(c.details(QContactTimestamp::Type).value(0)).created(), t1.created());
    QCOMPARE(QContactTimestamp(c.details(QContactTimestamp::Type).value(0)).deleted(), t1.deleted());

    // test property update
    t1.setValue(QContactTimestamp::FieldContext, QContactDetail::ContextWork);
    t1.setLastModified(modified);
    QVERIFY(c.saveDetail(&t1));
    QCOMPARE(c.details(QContactTimestamp::Type).value(0).value(QContactTimestamp::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactTimestamp::Type).value(0).value(QContactTimestamp::FieldCreationTimestamp).toDateTime(), created);
    QCOMPARE(c.details(QContactTimestamp::Type).value(0).value(QContactTimestamp::FieldModificationTimestamp).toDateTime(), modified);
    QCOMPARE(c.details(QContactTimestamp::Type).value(0).value(QContactTimestamp::FieldDeletionTimestamp).toDateTime(), deleted);

    // test property remove
    QVERIFY(c.removeDetail(&t1));
    QCOMPARE(c.details(QContactTimestamp::Type).count(), 0);
    t2.setCreated(created.addSecs(15));
    QVERIFY(c.saveDetail(&t2));
    QCOMPARE(c.details(QContactTimestamp::Type).count(), 1);
    QVERIFY(c.removeDetail(&t2));
    QCOMPARE(c.details(QContactTimestamp::Type).count(), 0);
    QVERIFY(c.removeDetail(&t2) == false);
    QCOMPARE(c.details(QContactTimestamp::Type).count(), 0);
}

void tst_QContactDetails::type()
{
    QContact c;
    QContactType t1, t2;

    // test property set
    t1.setType(QContactType::TypeGroup);
    QCOMPARE(t1.type(), QContactType::TypeGroup);
    QCOMPARE(static_cast<QContactType::TypeValues>(t1.value<int>(QContactType::FieldType)), QContactType::TypeGroup);

    // test property add
    QVERIFY(c.saveDetail(&t1));
    QCOMPARE(c.details(QContactType::Type).count(), 1);
    QCOMPARE(QContactType(c.details(QContactType::Type).value(0)).type(), t1.type());

    // test property update
    t1.setType(QContactType::TypeContact);
    QVERIFY(c.saveDetail(&t1));
    QCOMPARE(static_cast<QContactType::TypeValues>(c.details(QContactType::Type).value(0).value<int>(QContactType::FieldType)), QContactType::TypeContact);

    // test property remove
    QVERIFY(!c.removeDetail(&t1)); // cannot remove type
    QCOMPARE(c.details(QContactType::Type).count(), 1);
    t2.setType(QContactType::TypeGroup);
    QVERIFY(c.saveDetail(&t2)); // overwrites t1
    QCOMPARE(c.details(QContactType::Type).count(), 1);
    QVERIFY(!c.removeDetail(&t2)); // cannot remove type - "succeeds" but count remains unchanged
    QCOMPARE(c.details(QContactType::Type).count(), 1);
    QVERIFY(!c.removeDetail(&t2));
    QCOMPARE(c.details(QContactType::Type).count(), 1);
}

void tst_QContactDetails::url()
{
    QContact c;
    QContactUrl u1, u2;

    // test property set
    u1.setUrl("1234");
    QCOMPARE(u1.url(), QString("1234"));
    QCOMPARE(u1.value(QContactUrl::FieldUrl).toString(), QString("1234"));

    u1.setSubType(QContactUrl::SubTypeHomePage);
    QCOMPARE(u1.subType(), QContactUrl::SubTypeHomePage);
    QCOMPARE(static_cast<QContactUrl::SubType>(u1.value<int>(QContactUrl::FieldSubType)), QContactUrl::SubTypeHomePage);

    // test property add
    QVERIFY(c.saveDetail(&u1));
    QCOMPARE(c.details(QContactUrl::Type).count(), 1);
    QCOMPARE(QContactUrl(c.details(QContactUrl::Type).value(0)).url(), u1.url());

    // test property update
    u1.setValue(QContactUrl::FieldContext, QContactDetail::ContextWork);
    u1.setUrl("12345");
    QVERIFY(c.saveDetail(&u1));
    QCOMPARE(c.details(QContactUrl::Type).value(0).value(QContactUrl::FieldContext).value<QList<int> >(), QList<int>() << QContactDetail::ContextWork);
    QCOMPARE(c.details(QContactUrl::Type).value(0).value(QContactUrl::FieldUrl).toString(), QString("12345"));

    // now as above, but with the QUrl setter.
    QUrl urlValue("http://www.example.com");
    QContactUrl u3;
    u3.setUrl(urlValue);
    QCOMPARE(u3.url(), urlValue.toString());
    QVERIFY(c.saveDetail(&u3));
    QVERIFY(c.details(QContactUrl::Type).contains(u3));
    u3.setUrl(QString(QStringLiteral("http://www.anotherexample.com")));
    QCOMPARE(u3.url(), QString(QStringLiteral("http://www.anotherexample.com")));
    QVERIFY(c.saveDetail(&u3));

    // test property remove
    QVERIFY(c.removeDetail(&u3));
    QVERIFY(c.removeDetail(&u1));
    QCOMPARE(c.details(QContactUrl::Type).count(), 0);
    QVERIFY(c.saveDetail(&u2));
    QCOMPARE(c.details(QContactUrl::Type).count(), 1);
    QVERIFY(c.removeDetail(&u2));
    QCOMPARE(c.details(QContactUrl::Type).count(), 0);
    QVERIFY(c.removeDetail(&u2) == false);
    QCOMPARE(c.details(QContactUrl::Type).count(), 0);
}

void tst_QContactDetails::version()
{
    QContact c;
    QContactVersion v1, v2;
    QByteArray extendedVersion("Qt rules!");
    QByteArray anotherExtendedVersion("Qt rules again!");
    // test property set
    v1.setSequenceNumber(64);
    QCOMPARE(v1.sequenceNumber(), 64);
    QCOMPARE(v1.value(QContactVersion::FieldSequenceNumber).toInt(), 64);
    v1.setExtendedVersion(extendedVersion);
    QCOMPARE(v1.extendedVersion(), extendedVersion);
    QCOMPARE(v1.value(QContactVersion::FieldExtendedVersion).toByteArray(), extendedVersion);

    // test property add
    QVERIFY(c.saveDetail(&v1));
    QCOMPARE(c.details(QContactVersion::Type).count(), 1);
    QCOMPARE(QContactVersion(c.details(QContactVersion::Type).value(0)).sequenceNumber(), v1.sequenceNumber());
    QCOMPARE(QContactVersion(c.details(QContactVersion::Type).value(0)).extendedVersion(), v1.extendedVersion());
    QVERIFY(c.saveDetail(&v2));
    QCOMPARE(c.details(QContactVersion::Type).count(), 2);

    // test property update
    v1.setSequenceNumber(65);
    v1.setExtendedVersion(anotherExtendedVersion);
    QVERIFY(c.saveDetail(&v1));
    QCOMPARE(c.details(QContactVersion::Type).value(0).value(QContactVersion::FieldSequenceNumber).toInt(), 65);
    QCOMPARE(c.details(QContactVersion::Type).value(0).value(QContactVersion::FieldExtendedVersion).toByteArray(),
             anotherExtendedVersion);

    // test property remove
    QVERIFY(c.removeDetail(&v1));
    QCOMPARE(c.details(QContactVersion::Type).count(), 1);
    QVERIFY(c.removeDetail(&v2));
    QCOMPARE(c.details(QContactVersion::Type).count(), 0);
    QVERIFY(c.removeDetail(&v2) == false);
    QCOMPARE(c.details(QContactVersion::Type).count(), 0);
}

QTEST_MAIN(tst_QContactDetails)
#include "tst_qcontactdetails.moc"
