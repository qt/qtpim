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
#include <QtContacts/QContactDetail>
#include <QtContacts/qcontacts.h>

#include <QSet>

//TESTED_COMPONENT=src/contacts

QTCONTACTS_USE_NAMESPACE
class tst_QContactDetail : public QObject
{
Q_OBJECT

public:
    tst_QContactDetail();
    virtual ~tst_QContactDetail();

public slots:
    void init();
    void cleanup();

private slots:
    void classHierarchy();
    void assignment();
    void templates();
    void contexts();
    void values();
    void hash();
    void datastream();
    void traits();
    void keys();
    void detailUris();
};

tst_QContactDetail::tst_QContactDetail()
{
}

tst_QContactDetail::~tst_QContactDetail()
{
}

void tst_QContactDetail::init()
{
}

void tst_QContactDetail::cleanup()
{
}

void tst_QContactDetail::classHierarchy()
{
    QContactDetail f1;
    QContactDetail f2;

    QVERIFY(f1.isEmpty());
    QVERIFY(f2.isEmpty());

    QContactPhoneNumber p1;
    p1.setNumber("123456");
    p1.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeFax << QContactPhoneNumber::SubTypeDtmfMenu);
    QVERIFY(!p1.isEmpty());
    QVERIFY(p1.type() == QContactPhoneNumber::Type);

    QContactName m1;
    m1.setFirstName("Bob");
    QVERIFY(!m1.isEmpty());
    QVERIFY(m1.type() == QContactName::Type);

    QVERIFY(p1 != m1);
    QVERIFY(f1 == f2);

    f1 = p1; // f1 is a phonenumber
    QVERIFY(f1 == p1);

    f1 = f1; // assign to itself
    QVERIFY(f1 == f1);
    QVERIFY(f1 == p1);
    QVERIFY(f1 != f2);
    QVERIFY(p1 != f2);

    p1 = p1; // assign leaf class to itself
    QVERIFY(p1 == p1);
    QVERIFY(f1 == p1);
    QVERIFY(p1 == f1);

    f2 = f1; // f2 = f1 = phonenumber
    QVERIFY(f1 == f2);
    QVERIFY(f2 == f1);
    QVERIFY(f2 == p1);
    QVERIFY(f1 == p1);

    f1 = m1; // f1 = name, f2 = phonenumber
    QVERIFY(f1 == m1);
    QVERIFY(f1 != f2);
    QVERIFY(f2 == p1);

    QContactPhoneNumber p2(f2); // p2 = f2 = phonenumber
    QVERIFY(p1 == p2);
    QVERIFY(p1 == f2);
    QCOMPARE(p2.number(), p1.number());
    QCOMPARE(p2.number(), QString("123456"));
    QCOMPARE(p2.subTypes(), QList<int>() << QContactPhoneNumber::SubTypeFax << QContactPhoneNumber::SubTypeDtmfMenu);

    p2 = p1; // phone number to phone number
    QVERIFY(p1 == p2);
    QVERIFY(p1 == f2);
    QCOMPARE(p2.number(), p1.number());
    QCOMPARE(p2.number(), QString("123456"));

    p2.setNumber("5678"); // NOTE: implicitly shared, this has caused a detach so p1 != 2
    QVERIFY(p1 != p2);
    QVERIFY(p1 == f2);
    QVERIFY(p2 != f2);
    QCOMPARE(p2.number(), QString("5678"));
    QCOMPARE(p1.number(), QString("123456"));

    p2.setNumber(QString("123456"));
    QVERIFY(p1 == p2);
    p2.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeLandline);
    QVERIFY(p1 != p2);
    p1.setSubTypes(QList<int>() << QContactPhoneNumber::SubTypeLandline);
    QVERIFY(p1 == p2);

    /* Bad assignment */
    p2 = m1; // assign a name to a phone number
    QVERIFY(p2 != m1);
    QVERIFY(p2.type() == QContactPhoneNumber::Type); // should still be a phone number though
    QVERIFY(p2.isEmpty());

    /* copy ctor */
    QContactName m2(m1);
    QVERIFY(m2 == m1);

    /* another bad assignment */
    m2 = p2; // phone number to a name
    QVERIFY(m2 != m1);
    QVERIFY(m2.type() == QContactName::Type);
    QVERIFY(m2.isEmpty());

    /* Check contexts are considered for equality */
    p2 = QContactPhoneNumber(); // new id / detach
    p2.setNumber(p1.number());
    p2.setSubTypes(p1.subTypes());
    p2.setContexts(QContactDetail::ContextHome);
    QVERIFY(p1 != p2);
    p2.removeValue(QContactDetail::FieldContext); // note, context is a value.
    QVERIFY(p1 == p2); // different ids but same values should be equal

    /* Copy ctor from valid type */
    QContactDetail f3(p2);
    QVERIFY(f3 == p2);
    QVERIFY(f3.type() == QContactPhoneNumber::Type);

    /* Copy ctor from invalid type */
    QContactPhoneNumber p3(m1);
    QVERIFY(p3 != m1);
    QVERIFY(p3.type() == QContactPhoneNumber::Type);
    QVERIFY(p3.isEmpty());

    /* Copy ctore from invalid type, through base type */
    f3 = m1;
    QContactPhoneNumber p4(f3);
    QVERIFY(p4 != f3);
    QVERIFY(p4.type() == QContactPhoneNumber::Type);
    QVERIFY(p4.isEmpty());

    /* Try a reference */
    p1.setNumber("123456");
    QContactDetail& ref = p1;
    QVERIFY(p1.number() == "123456");
    QVERIFY(p1.value(QContactPhoneNumber::FieldNumber) == "123456");
    QVERIFY(ref.value(QContactPhoneNumber::FieldNumber) == "123456");
    QVERIFY(p1 == ref);
    QVERIFY(ref == p1);

    /* Try changing the original */
    p1.setNumber("56789");
    QVERIFY(p1.number() == "56789");
    QVERIFY(p1.value(QContactPhoneNumber::FieldNumber) == "56789");
    QVERIFY(ref.value(QContactPhoneNumber::FieldNumber) == "56789");
    QVERIFY(p1 == ref);
    QVERIFY(ref == p1);

    /* Try changing the reference */
    ref.setValue(QContactPhoneNumber::FieldNumber, "654321");
    QVERIFY(p1.number() == "654321");
    QVERIFY(p1.value(QContactPhoneNumber::FieldNumber) == "654321");
    QVERIFY(ref.value(QContactPhoneNumber::FieldNumber) == "654321");
    QVERIFY(p1 == ref);
    QVERIFY(ref == p1);
}

void tst_QContactDetail::assignment()
{
    QContactPhoneNumber p1, p2;
    p1.setNumber("12345");
    p2.setNumber("54321");
    QVERIFY(p1 != p2);

    p1 = p2;
    QVERIFY(p1 == p2);

    QContactManagerEngine::setDetailProvenance(&p2, QStringLiteral("test-provenance"));
    QCOMPARE(p2.provenance(), QStringLiteral("test-provenance"));
    QCOMPARE(p1.provenance(), QString());
    QVERIFY(p1 == p2); // provenance shouldn't affect comparison
    p1 = p2;           // but should be copied on copy/assignment
    QCOMPARE(p2.provenance(), p1.provenance());

    QContactEmailAddress e1;
    e1.setEmailAddress("test@nokia.com");
    QVERIFY(e1 != p1);
    e1 = p1;
    QVERIFY(e1 != p1); // assignment across types shouldn't work
    QVERIFY(e1.emailAddress() == QString()); // should reset the detail
    QCOMPARE(e1, QContactEmailAddress());

    QContactManagerEngine::setDetailAccessConstraints(&p2, QContactDetail::Irremovable);
    QVERIFY(p1 != p2);
}

void tst_QContactDetail::templates()
{
    QContact c;
    QContactPhoneNumber p1, p2;
    p1.setNumber("1234");
    p2.setNumber("5678");
    QVERIFY(c.saveDetail(&p1));
    QVERIFY(c.saveDetail(&p2));

    QList<QContactDetail> l = c.details(QContactPhoneNumber::Type);

    QCOMPARE(l.count(), 2);
    QCOMPARE(QContactPhoneNumber(l.at(0)), p1);
    QCOMPARE(QContactPhoneNumber(l.at(1)), p2);

    QList<QContactPhoneNumber> l2 = c.details<QContactPhoneNumber>();
    QCOMPARE(l2.count(), 2);
    QCOMPARE(l2.at(0), p1);
    QCOMPARE(l2.at(1), p2);
}

void tst_QContactDetail::contexts()
{
    QContactDetail d;
    QVERIFY(d.contexts().count() == 0);

    // test set contexts
    d.setContexts(QContactDetail::ContextWork);
    QVERIFY(d.contexts().count() == 1);
    QVERIFY(d.contexts().contains(QContactDetail::ContextWork));
    QVERIFY(!d.contexts().contains(QContactDetail::ContextOther));
    QVERIFY(!d.contexts().contains(QContactDetail::ContextHome));

    QList<int> contexts;
    contexts.append(QContactDetail::ContextHome);
    contexts.append(QContactDetail::ContextOther);
    d.setContexts(contexts);
    QVERIFY(d.contexts().count() == 2);
    QVERIFY(!d.contexts().contains(QContactDetail::ContextWork));
    QVERIFY(d.contexts().contains(QContactDetail::ContextOther));
    QVERIFY(d.contexts().contains(QContactDetail::ContextHome));
    QCOMPARE(d.contexts(), contexts);

    // test that contexts are values.
    QCOMPARE(d.value<QList<int> >(QContactDetail::FieldContext), d.contexts());
}

void tst_QContactDetail::values()
{
    QContactDetail p;
    QMap<int, QVariant> emptyValues;
    QCOMPARE(p.values(), emptyValues);

    QDateTime dt = QDateTime::currentDateTime();
    QTime t = dt.time();
    t.setHMS(t.hour(), t.minute(), t.second(), 0); // milliseconds don't round trip through ISODate
    dt.setTime(t);
    QDate d = dt.date();

    QDateTime ddt(d); // DateTime version of a Date (QTime())

    QVERIFY(p.setValue(QContactAddress::FieldStreet, "This is a string"));
    QVERIFY(p.setValue(QContactAddress::FieldLocality, d));
    QVERIFY(p.setValue(QContactAddress::FieldRegion, dt));
    QVERIFY(p.setValue(QContactAddress::FieldPostcode, (int)6));
    QVERIFY(p.setValue(QContactAddress::FieldSubTypes, d.toString(Qt::ISODate)));
    QVERIFY(p.setValue(QContactAddress::FieldPostOfficeBox, dt.toString(Qt::ISODate)));

    // Presence test (const char * version)
    QVERIFY(p.hasValue(QContactAddress::FieldStreet));
    QVERIFY(p.hasValue(QContactAddress::FieldLocality));
    QVERIFY(p.hasValue(QContactAddress::FieldRegion));
    QVERIFY(p.hasValue(QContactAddress::FieldPostcode));
    QVERIFY(p.hasValue(QContactAddress::FieldSubTypes));
    QVERIFY(p.hasValue(QContactAddress::FieldPostOfficeBox));
    QVERIFY(!p.hasValue(QContactAddress::FieldCountry));

    // string accessors with const char* key
    QCOMPARE(p.value(QContactAddress::FieldStreet).toString(), QString("This is a string"));
    QCOMPARE(p.value(QContactAddress::FieldLocality).toString(), d.toString(Qt::ISODate));
    QCOMPARE(p.value(QContactAddress::FieldRegion).toString(), dt.toString(Qt::ISODate));
    QCOMPARE(p.value(QContactAddress::FieldPostcode).toString(), QString("6"));
    QCOMPARE(p.value(QContactAddress::FieldSubTypes).toString(), d.toString(Qt::ISODate));
    QCOMPARE(p.value(QContactAddress::FieldPostOfficeBox).toString(), dt.toString(Qt::ISODate));

    // Variant accessor with const char * key
    QCOMPARE(p.value(QContactAddress::FieldStreet), QVariant(QString("This is a string")));
    QCOMPARE(p.value(QContactAddress::FieldLocality), QVariant(d));
    QCOMPARE(p.value(QContactAddress::FieldRegion), QVariant(dt));
    QCOMPARE(p.value(QContactAddress::FieldPostcode), QVariant((int)6));
    QCOMPARE(p.value(QContactAddress::FieldSubTypes), QVariant(d.toString(Qt::ISODate)));
    QCOMPARE(p.value(QContactAddress::FieldPostOfficeBox), QVariant(dt.toString(Qt::ISODate)));

    /* Typed accessors, string first, const char* key */
    QCOMPARE(p.value<QString>(QContactAddress::FieldStreet), QString("This is a string"));
    QCOMPARE(p.value<QString>(QContactAddress::FieldLocality), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(QContactAddress::FieldRegion), dt.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(QContactAddress::FieldPostcode), QString("6"));
    QCOMPARE(p.value<QString>(QContactAddress::FieldSubTypes), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(QContactAddress::FieldPostOfficeBox), dt.toString(Qt::ISODate));

    /* Now individual original types */
    QCOMPARE(p.value<QDate>(QContactAddress::FieldLocality), d);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldRegion), dt);
    QCOMPARE(p.value<int>(QContactAddress::FieldPostcode), 6);

    /* Now cross types that should fail */
    QDate id;
    QDateTime idt;
    QCOMPARE(p.value<QDate>(QContactAddress::FieldStreet), id);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldPostcode), id);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldStreet), idt);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldPostcode), idt);
    QCOMPARE(p.value<int>(QContactAddress::FieldLocality), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldRegion), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldStreet), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldSubTypes), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldPostOfficeBox), 0);

    /* Cross types that should work.. */
    QCOMPARE(p.value<int>(QContactAddress::FieldPostcode), 6);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldSubTypes), d);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldPostOfficeBox), dt);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldRegion), d);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldPostOfficeBox), d);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldLocality), ddt);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldSubTypes), ddt);

    /* Now set everything again */
    QMap<int, QVariant> values = p.values();
    QList<int> keys = values.keys();
    foreach (int key, keys)
        QVERIFY(p.setValue(key, QVariant()));

    QCOMPARE(p.values(), emptyValues);
    QVERIFY(p.values().count() == 0);
    QVERIFY(!p.hasValue(QContactAddress::FieldStreet));
    QVERIFY(!p.hasValue(QContactAddress::FieldLocality));
    QVERIFY(!p.hasValue(QContactAddress::FieldRegion));
    QVERIFY(!p.hasValue(QContactAddress::FieldPostcode));
    QVERIFY(!p.hasValue(QContactAddress::FieldSubTypes));
    QVERIFY(!p.hasValue(QContactAddress::FieldPostOfficeBox));

    QVERIFY(p.value(QContactAddress::FieldStreet).toString() == QString());
    QVERIFY(p.value(QContactAddress::FieldStreet) == QVariant());

    values.insert(QContactAddress::FieldStreet, "This is a string");
    values.insert(QContactAddress::FieldLocality, d);
    values.insert(QContactAddress::FieldRegion, dt);
    values.insert(QContactAddress::FieldPostcode, (int)6);

    values.insert(QContactAddress::FieldSubTypes, d.toString(Qt::ISODate));
    values.insert(QContactAddress::FieldPostOfficeBox, dt.toString(Qt::ISODate));
    values.insert(QContactAddress::FieldStreet, QString("This is a string"));

    /* Set values */
    keys = values.keys();
    foreach (int key, keys)
        QVERIFY(p.setValue(key, values.value(key)));

    /* Now repeat the tests with our bulk set map */
    QVERIFY(p.hasValue(QContactAddress::FieldStreet));
    QVERIFY(p.hasValue(QContactAddress::FieldLocality));
    QVERIFY(p.hasValue(QContactAddress::FieldRegion));
    QVERIFY(p.hasValue(QContactAddress::FieldPostcode));
    QVERIFY(p.hasValue(QContactAddress::FieldSubTypes));
    QVERIFY(p.hasValue(QContactAddress::FieldPostOfficeBox));

    /* String accessors */
    QCOMPARE(p.value(QContactAddress::FieldStreet).toString(), QString("This is a string"));
    QCOMPARE(p.value(QContactAddress::FieldLocality).toString(), d.toString(Qt::ISODate));
    QCOMPARE(p.value(QContactAddress::FieldRegion).toString(), dt.toString(Qt::ISODate));
    QCOMPARE(p.value(QContactAddress::FieldPostcode).toString(), QString("6"));
    QCOMPARE(p.value(QContactAddress::FieldSubTypes).toString(), d.toString(Qt::ISODate));
    QCOMPARE(p.value(QContactAddress::FieldPostOfficeBox).toString(), dt.toString(Qt::ISODate));

    /* Typed accessors, string first */
    QCOMPARE(p.value<QString>(QContactAddress::FieldStreet), QString("This is a string"));
    QCOMPARE(p.value<QString>(QContactAddress::FieldLocality), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(QContactAddress::FieldRegion), dt.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(QContactAddress::FieldPostcode), QString("6"));
    QCOMPARE(p.value<QString>(QContactAddress::FieldSubTypes), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(QContactAddress::FieldPostOfficeBox), dt.toString(Qt::ISODate));

    /* Now individual original types */
    QCOMPARE(p.value<QDate>(QContactAddress::FieldLocality), d);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldRegion), dt);
    QCOMPARE(p.value<int>(QContactAddress::FieldPostcode), 6);

    /* Now cross types that should fail */
    QCOMPARE(p.value<QDate>(QContactAddress::FieldStreet), id);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldPostcode), id);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldStreet), idt);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldPostcode), idt);
    QCOMPARE(p.value<int>(QContactAddress::FieldLocality), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldRegion), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldStreet), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldSubTypes), 0);
    QCOMPARE(p.value<int>(QContactAddress::FieldPostOfficeBox), 0);

    /* Cross types that should work.. */
    QCOMPARE(p.value<QDate>(QContactAddress::FieldSubTypes), d);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldPostOfficeBox), dt);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldRegion), d);
    QCOMPARE(p.value<QDate>(QContactAddress::FieldPostOfficeBox), d);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldLocality), ddt);
    QCOMPARE(p.value<QDateTime>(QContactAddress::FieldSubTypes), ddt);

    /* Reset again */
    values = p.values();
    keys = values.keys();
    foreach (int key, keys)
        QVERIFY(p.setValue(key, QVariant()));
    QCOMPARE(p.values(), emptyValues);

    /* Check adding a null value removes the field */
    p.setValue(QContactAddress::FieldStreet, "stringvalue");
    QVERIFY(p.values().contains(QContactAddress::FieldStreet));
    QVERIFY(p.value(QContactAddress::FieldStreet) == QString("stringvalue"));
    p.setValue(QContactAddress::FieldStreet, QVariant());
    QVERIFY(!p.values().contains(QContactAddress::FieldStreet));

    /* Check adding a field whose value is an empty string */
    p.setValue(QContactAddress::FieldStreet, "");
    QVERIFY(p.values().contains(QContactAddress::FieldStreet));
    QVERIFY(p.value(QContactAddress::FieldStreet) == QString(""));

    /* Check accessing a missing value */
    QCOMPARE(p.value(QContactAddress::FieldStreet).toString(), QString());
    QVERIFY(p.setValue(QContactAddress::FieldStreet, "changed my mind"));
    QCOMPARE(p.value(QContactAddress::FieldStreet).toString(), QString("changed my mind"));

    /* Check removing a missing value */
    QVERIFY(!p.removeValue(QContactAddress::FieldCountry));

    p.setValue(QContactAddress::FieldCountry, "555");
    p.setValue(QContactAddress::FieldPostOfficeBox, "1234");

    /* Check removing a real value */
    QVERIFY(p.removeValue(QContactAddress::FieldStreet));
    QVERIFY(p.removeValue(QContactAddress::FieldCountry));
    QVERIFY(p.removeValue(QContactAddress::FieldPostOfficeBox));
}

void tst_QContactDetail::hash()
{
    QContactExtendedDetail detail1;
    detail1.setName("key");
    detail1.setData(QVariant("value"));
    QContactExtendedDetail detail2;
    detail2.setName("key");
    detail2.setData(QVariant("value"));
    QContactExtendedDetail detail3;
    detail3.setName("key");
    detail3.setData(QVariant("different value"));
    QVERIFY(qHash(detail1) == qHash(detail2));
    QVERIFY(qHash(detail1) != qHash(detail3));
    QSet<QContactDetail> set;
    set.insert(detail1);
    set.insert(detail2);
    set.insert(detail3);
    QCOMPARE(set.size(), 2);
}

void tst_QContactDetail::datastream()
{
    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    QContactExtendedDetail detailIn;
    detailIn.setName("key1");
    detailIn.setData(QVariant("value1"));
    detailIn.setName("key2");
    detailIn.setData(QVariant("value2"));
    stream1 << detailIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QContactExtendedDetail detailOut;
    stream2 >> detailOut;
    QCOMPARE(detailOut, detailIn);
}

void tst_QContactDetail::traits()
{
    QCOMPARE(sizeof(QContactDetail), sizeof(void *));
    QVERIFY(QTypeInfo<QContactDetail>::isComplex);
    QVERIFY(!QTypeInfo<QContactDetail>::isStatic);
    QVERIFY(!QTypeInfo<QContactDetail>::isLarge);
    QVERIFY(!QTypeInfo<QContactDetail>::isPointer);
    QVERIFY(!QTypeInfo<QContactDetail>::isDummy);
}

void tst_QContactDetail::keys()
{
    QContactDetail d;
    QContactDetail d2;
    QVERIFY(d.key() != d2.key());

    d = d2;
    QVERIFY(d.key() == d2.key());
    d.resetKey();
    QVERIFY(d.key() != d2.key());
}

void tst_QContactDetail::detailUris()
{
    QContactDetail d;
    QVERIFY(d.detailUri().isEmpty());

    d.setDetailUri("I'm a detail uri");
    QVERIFY(d.detailUri() == "I'm a detail uri");

    d.setDetailUri(QString());
    QVERIFY(d.detailUri().isEmpty());

    QVERIFY(d.linkedDetailUris().isEmpty());

    d.setLinkedDetailUris("5555");
    QVERIFY(d.linkedDetailUris().count() == 1);
    QVERIFY(d.linkedDetailUris().count("5555") == 1);

    QStringList sl;
    sl << "6666" << "7777";
    d.setLinkedDetailUris(sl);
    QVERIFY(d.linkedDetailUris().count() == 2);
    QVERIFY(d.linkedDetailUris() == sl);
}

QTEST_MAIN(tst_QContactDetail)
#include "tst_qcontactdetail.moc"
