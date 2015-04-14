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

#include <QtCore/qset.h>

#include <QtOrganizer/qorganizer.h>

//TESTED_COMPONENT=src/organizer

QTORGANIZER_USE_NAMESPACE
class tst_QOrganizerItemDetail : public QObject
{
Q_OBJECT

public:
    tst_QOrganizerItemDetail();
    virtual ~tst_QOrganizerItemDetail();

public slots:
    void init();
    void cleanup();

private slots:
    void classHierarchy();
    void assignment();
    void templates();
    void values();
    void hash();
    void datastream();
    void traits();
    void keys();
};

tst_QOrganizerItemDetail::tst_QOrganizerItemDetail()
{
}

tst_QOrganizerItemDetail::~tst_QOrganizerItemDetail()
{
}

void tst_QOrganizerItemDetail::init()
{
}

void tst_QOrganizerItemDetail::cleanup()
{
}

/* Test class that doesn't do the right thing */
class NonMacroCustomDetail : public QOrganizerItemDetail
{
public:
    NonMacroCustomDetail() : QOrganizerItemDetail(QOrganizerItemDetail::TypeUndefined) {}
    void doAssign(const QOrganizerItemDetail& other) {assign(other, QOrganizerItemDetail::TypeUndefined);}
    NonMacroCustomDetail(const QOrganizerItemDetail& other)
        : QOrganizerItemDetail(other, QOrganizerItemDetail::TypeUndefined) {}
};

class NonMacroCustomDetail2 : public QOrganizerItemDetail
{
public:
    NonMacroCustomDetail2() : QOrganizerItemDetail(QOrganizerItemDetail::TypeUndefined) {}
    void doAssign(const QOrganizerItemDetail& other) {assign(other, QOrganizerItemDetail::TypeUndefined);}
    NonMacroCustomDetail2(const QOrganizerItemDetail& other)
        : QOrganizerItemDetail(other, QOrganizerItemDetail::TypeUndefined) {}
};

void tst_QOrganizerItemDetail::classHierarchy()
{
    QOrganizerItemDetail f1;
    QOrganizerItemDetail f2;

    QVERIFY(f1.isEmpty());
    QVERIFY(f2.isEmpty());

    QOrganizerItemPriority p1;
    p1.setPriority(QOrganizerItemPriority::VeryHighPriority);
    QVERIFY(!p1.isEmpty());
    QVERIFY(p1.type() == QOrganizerItemDetail::TypePriority);

    QOrganizerItemComment m1;
    m1.setComment("Bob");
    QVERIFY(!m1.isEmpty());
    QVERIFY(m1.type() == QOrganizerItemDetail::TypeComment);

    QVERIFY(p1 != m1);
    QVERIFY(f1 == f2);

    f1 = p1; // f1 is a priority
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

    f2 = f1; // f2 = f1 = priority
    QVERIFY(f1 == f2);
    QVERIFY(f2 == f1);
    QVERIFY(f2 == p1);
    QVERIFY(f1 == p1);

    f1 = m1; // f1 = name, f2 = priority
    QVERIFY(f1 == m1);
    QVERIFY(f1 != f2);
    QVERIFY(f2 == p1);

    QOrganizerItemPriority p2(f2); // p2 = f2 = priority
    QVERIFY(p1 == p2);
    QVERIFY(p1 == f2);
    QCOMPARE(p2.priority(), p1.priority());
    QCOMPARE(p2.priority(), QOrganizerItemPriority::VeryHighPriority);

    p2 = p1; // priority to priority
    QVERIFY(p1 == p2);
    QVERIFY(p1 == f2);
    QCOMPARE(p2.priority(), p1.priority());
    QCOMPARE(p2.priority(), QOrganizerItemPriority::VeryHighPriority);

    p2.setPriority(QOrganizerItemPriority::VeryLowPriority); // NOTE: implicitly shared, this has caused a detach so p1 != 2
    QVERIFY(p1 != p2);
    QVERIFY(p1 == f2);
    QVERIFY(p2 != f2);
    QCOMPARE(p2.priority(), QOrganizerItemPriority::VeryLowPriority);
    QCOMPARE(p1.priority(), QOrganizerItemPriority::VeryHighPriority);

    /* Bad assignment */
    p2 = m1; // assign a comment to a priority
    QVERIFY(p2 != m1);
    QVERIFY(p2.type() == QOrganizerItemDetail::TypePriority);
    QVERIFY(p2.isEmpty());

    /* copy ctor */
    QOrganizerItemComment m2(m1);
    QVERIFY(m2 == m1);

    /* another bad assignment */
    m2 = p2; // priority to a comment
    QVERIFY(m2 != m1);
    QVERIFY(m2.type() == QOrganizerItemDetail::TypeComment);
    QVERIFY(m2.isEmpty());

    /* Copy ctor from valid type */
    QOrganizerItemDetail f3(p2);
    QVERIFY(f3 == p2);
    QVERIFY(f3.type() == QOrganizerItemDetail::TypePriority);

    /* Copy ctor from invalid type */
    QOrganizerItemPriority p3(m1);
    QVERIFY(p3 != m1);
    QVERIFY(p3.type() == QOrganizerItemDetail::TypePriority);
    QVERIFY(p3.isEmpty());

    /* Copy ctore from invalid type, through base type */
    f3 = m1;
    QOrganizerItemPriority p4(f3);
    QVERIFY(p4 != f3);
    QVERIFY(p4.type() == QOrganizerItemDetail::TypePriority);
    QVERIFY(p4.isEmpty());

    /* Try a reference */
    p1.setPriority(QOrganizerItemPriority::VeryLowPriority);
    QOrganizerItemDetail& ref = p1;
    QVERIFY(p1.priority() == QOrganizerItemPriority::VeryLowPriority);
    QVERIFY(p1.value(QOrganizerItemPriority::FieldPriority).toInt() == static_cast<int>(QOrganizerItemPriority::VeryLowPriority));
    QVERIFY(ref.value(QOrganizerItemPriority::FieldPriority).toInt() == static_cast<int>(QOrganizerItemPriority::VeryLowPriority));
    QVERIFY(p1 == ref);
    QVERIFY(ref == p1);

    /* Try changing the original */
    p1.setPriority(QOrganizerItemPriority::MediumPriority);
    QVERIFY(p1.priority() == QOrganizerItemPriority::MediumPriority);
    QVERIFY(p1.value(QOrganizerItemPriority::FieldPriority).toInt() == static_cast<int>(QOrganizerItemPriority::MediumPriority));
    QVERIFY(ref.value(QOrganizerItemPriority::FieldPriority).toInt() == static_cast<int>(QOrganizerItemPriority::MediumPriority));
    QVERIFY(p1 == ref);
    QVERIFY(ref == p1);

    /* Try changing the reference */
    ref.setValue(QOrganizerItemPriority::FieldPriority, QOrganizerItemPriority::LowPriority);
    QVERIFY(p1.priority() == QOrganizerItemPriority::LowPriority);
    QVERIFY(p1.value(QOrganizerItemPriority::FieldPriority).toInt() == static_cast<int>(QOrganizerItemPriority::LowPriority));
    QVERIFY(ref.value(QOrganizerItemPriority::FieldPriority).toInt() == static_cast<int>(QOrganizerItemPriority::LowPriority));
    QVERIFY(p1 == ref);
    QVERIFY(ref == p1);

    /* Random other test */
    NonMacroCustomDetail md;
    QVERIFY(md.type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(md.setValue(1, "value"));
    QVERIFY(!md.isEmpty());
    md.doAssign(md); // self assignment
    QVERIFY(!md.isEmpty());
    QVERIFY(md.value(1) == "value");

    QOrganizerItemDetail mdv;
    mdv = md;
    QVERIFY(mdv.type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(mdv.value(1) == "value");

    md = mdv;
    QVERIFY(md.type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(md.value(1) == "value");

    NonMacroCustomDetail2 md2;
    QVERIFY(md2.setValue(1, "value"));
    QVERIFY(md2.type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(md2.value(1) == "value");
    md2.doAssign(md);
    QVERIFY(md2 == md);
    md2 = md;
    QVERIFY(md.type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(md.value(1) == "value");

    // Self assignment
    md2.doAssign(md2);
    QVERIFY(md2.type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(md2.value(1) == "value");

    md.doAssign(md2);
    QVERIFY(md == md2);

    // Assigning something else
    QOrganizerItemPriority pn;
    pn.setPriority(QOrganizerItemPriority::LowestPriority);
    md2.doAssign(pn);
    QVERIFY(md2.isEmpty());
    QVERIFY(md2.type() == QOrganizerItemDetail::TypeUndefined);

    NonMacroCustomDetail mdb(pn);
    QVERIFY(mdb.isEmpty());
    QVERIFY(mdb.type() == QOrganizerItemDetail::TypeUndefined);

    NonMacroCustomDetail2 md2b(pn);
    QVERIFY(md2b.isEmpty());
    QVERIFY(md2b.type() == QOrganizerItemDetail::TypeUndefined);
}

void tst_QOrganizerItemDetail::assignment()
{
    QOrganizerItemPriority p1, p2;
    p1.setPriority(QOrganizerItemPriority::LowPriority);
    p2.setPriority(QOrganizerItemPriority::HighPriority);
    QVERIFY(p1 != p2);

    p1 = p2;
    QVERIFY(p1 == p2);

    QOrganizerItemComment c1;
    c1.setComment("test comment");
    QVERIFY(c1 != p1);
    c1 = p1;
    QVERIFY(c1 != p1); // assignment across types shouldn't work
    QVERIFY(c1.comment() == QString()); // should reset the detail
    QCOMPARE(c1, QOrganizerItemComment());
}

void tst_QOrganizerItemDetail::templates()
{
    QOrganizerItem c;
    QOrganizerItemPriority p1, p2;
    p1.setPriority(QOrganizerItemPriority::HighPriority);
    p2.setPriority(QOrganizerItemPriority::HighestPriority);
    QVERIFY(c.saveDetail(&p1));
    QVERIFY(c.saveDetail(&p2));

    QList<QOrganizerItemDetail> l = c.details(QOrganizerItemDetail::TypePriority);

    QCOMPARE(l.count(), 2);
    QCOMPARE(QOrganizerItemPriority(l.at(0)), p1);
    QCOMPARE(QOrganizerItemPriority(l.at(1)), p2);

    QList<QOrganizerItemDetail> l2 = c.details(QOrganizerItemDetail::TypePriority);
    QCOMPARE(l2.count(), 2);
    QCOMPARE(static_cast<QOrganizerItemPriority>(l2.at(0)), p1);
    QCOMPARE(static_cast<QOrganizerItemPriority>(l2.at(1)), p2);
}

void tst_QOrganizerItemDetail::values()
{
    QOrganizerItemDetail p;

    QCOMPARE(p.values(), (QMap<int, QVariant>()));

    QDateTime dt = QDateTime::currentDateTime();
    QTime t = dt.time();
    t.setHMS(t.hour(), t.minute(), t.second(), 0); // milliseconds don't round trip through ISODate
    dt.setTime(t);
    QDate d = dt.date();

    QDateTime ddt(d); // DateTime version of a Date (QTime())

    p.setValue(101, "This is a string");
    p.setValue(102, d);
    p.setValue(103, dt);
    p.setValue(104, (int)6);

    p.setValue(105, d.toString(Qt::ISODate));
    p.setValue(106, dt.toString(Qt::ISODate));

    // Test the setter that takes a QString
    p.setValue(107, "123");

    // and the setter that takes a QL1C
    p.setValue(QOrganizerItemPriority::FieldPriority, QVariant::fromValue(static_cast<int>(QOrganizerItemPriority::ExtremelyHighPriority)));

    /* Presence test */
    QVERIFY(p.hasValue(101));
    QVERIFY(p.hasValue(102));
    QVERIFY(p.hasValue(103));
    QVERIFY(p.hasValue(104));
    QVERIFY(p.hasValue(105));
    QVERIFY(p.hasValue(106));
    QVERIFY(p.hasValue(107));
    QVERIFY(p.hasValue(QOrganizerItemPriority::FieldPriority));
    QVERIFY(!p.hasValue(666));

    QVERIFY(p.hasValue(QOrganizerItemPriority::FieldPriority));

    /* Variant accessor */
    QCOMPARE(p.value(101), QVariant(QString("This is a string")));
    QCOMPARE(p.value(102), QVariant(d));
    QCOMPARE(p.value(103), QVariant(dt));
    QCOMPARE(p.value(104), QVariant((int)6));
    QCOMPARE(p.value(105), QVariant(d.toString(Qt::ISODate)));
    QCOMPARE(p.value(106), QVariant(dt.toString(Qt::ISODate)));
    QCOMPARE(p.value(107), QVariant(QString("123")));
    QCOMPARE(p.value(QOrganizerItemPriority::FieldPriority).toInt(), static_cast<int>(QOrganizerItemPriority::ExtremelyHighPriority));

    /* Typed accessors */
    QCOMPARE(p.value<QString>(101), QString("This is a string"));
    QCOMPARE(p.value<QString>(102), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(103), dt.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(104), QString("6"));
    QCOMPARE(p.value<QString>(105), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(106), dt.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(107), QString("123"));
    QCOMPARE(p.value<int>(QOrganizerItemPriority::FieldPriority), static_cast<int>(QOrganizerItemPriority::ExtremelyHighPriority));

    /* Now individual original types */
    QCOMPARE(p.value<QDate>(102), d);
    QCOMPARE(p.value<QDateTime>(103), dt);
    QCOMPARE(p.value<int>(104), 6);

    /* Now cross types that should fail */
    QDate id;
    QDateTime idt;
    QCOMPARE(p.value<QDate>(101), id);
    QCOMPARE(p.value<QDate>(104), id);
    QCOMPARE(p.value<QDate>(107), id);
    QCOMPARE(p.value<QDateTime>(101), idt);
    QCOMPARE(p.value<QDateTime>(104), idt);
    QCOMPARE(p.value<QDateTime>(107), idt);
    QCOMPARE(p.value<int>(102), 0);
    QCOMPARE(p.value<int>(103), 0);
    QCOMPARE(p.value<int>(101), 0);
    QCOMPARE(p.value<int>(105), 0);
    QCOMPARE(p.value<int>(106), 0);

    /* Cross types that should work.. */
    QCOMPARE(p.value<int>(107), 123);
    QCOMPARE(p.value<QDate>(105), d);
    QCOMPARE(p.value<QDateTime>(106), dt);
    QCOMPARE(p.value<QDate>(103), d);
    QCOMPARE(p.value<QDate>(106), d);
    QCOMPARE(p.value<QDateTime>(102), ddt);
    QCOMPARE(p.value<QDateTime>(105), ddt);

    /* Now set everything again */
    QMap<int, QVariant> emptyValues;
    QMap<int, QVariant> values = p.values();
    QList<int> keys = values.keys();
    foreach (int key, keys)
        QVERIFY(p.setValue(key, QVariant()));

    QCOMPARE(p.values(), emptyValues);
    QVERIFY(p.values().count() == 0);
    QVERIFY(!p.hasValue(101));
    QVERIFY(!p.hasValue(102));
    QVERIFY(!p.hasValue(103));
    QVERIFY(!p.hasValue(104));
    QVERIFY(!p.hasValue(105));
    QVERIFY(!p.hasValue(106));
    QVERIFY(!p.hasValue(107));
    QVERIFY(!p.hasValue(666));

    QVERIFY(p.value(101).toString() == QString());
    QVERIFY(p.value(101) == QVariant());

    values.insert(101, "This is a string");
    values.insert(102, d);
    values.insert(103, dt);
    values.insert(104, (int)6);

    values.insert(105, d.toString(Qt::ISODate));
    values.insert(106, dt.toString(Qt::ISODate));
    values.insert(107, "123");
    values.insert(101, QString("This is a string"));

    /* Set values */
    keys = values.keys();
    foreach (int key, keys)
        QVERIFY(p.setValue(key, values.value(key)));

    /* Now repeat the tests with our bulk set map */
    QVERIFY(p.hasValue(101));
    QVERIFY(p.hasValue(102));
    QVERIFY(p.hasValue(103));
    QVERIFY(p.hasValue(104));
    QVERIFY(p.hasValue(105));
    QVERIFY(p.hasValue(106));
    QVERIFY(p.hasValue(107));
    QVERIFY(!p.hasValue(666));

    /* String accessors */
    QCOMPARE(p.value(101).toString(), QString("This is a string"));
    QCOMPARE(p.value(102).toString(), d.toString(Qt::ISODate));
    QCOMPARE(p.value(103).toString(), dt.toString(Qt::ISODate));
    QCOMPARE(p.value(104).toString(), QString("6"));
    QCOMPARE(p.value(105).toString(), d.toString(Qt::ISODate));
    QCOMPARE(p.value(106).toString(), dt.toString(Qt::ISODate));
    QCOMPARE(p.value(107).toString(), QString("123"));

    /* Typed accessors, string first */
    QCOMPARE(p.value<QString>(101), QString("This is a string"));
    QCOMPARE(p.value<QString>(102), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(103), dt.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(104), QString("6"));
    QCOMPARE(p.value<QString>(105), d.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(106), dt.toString(Qt::ISODate));
    QCOMPARE(p.value<QString>(107), QString("123"));

    /* Now individual original types */
    QCOMPARE(p.value<QDate>(102), d);
    QCOMPARE(p.value<QDateTime>(103), dt);
    QCOMPARE(p.value<int>(104), 6);

    /* Now cross types that should fail */
    QCOMPARE(p.value<QDate>(101), id);
    QCOMPARE(p.value<QDate>(104), id);
    QCOMPARE(p.value<QDate>(107), id);
    QCOMPARE(p.value<QDateTime>(101), idt);
    QCOMPARE(p.value<QDateTime>(104), idt);
    QCOMPARE(p.value<QDateTime>(107), idt);
    QCOMPARE(p.value<int>(101), 0);
    QCOMPARE(p.value<int>(102), 0);
    QCOMPARE(p.value<int>(103), 0);
    QCOMPARE(p.value<int>(105), 0);
    QCOMPARE(p.value<int>(106), 0);

    /* Cross types that should work.. */
    QCOMPARE(p.value<int>(107), 123);
    QCOMPARE(p.value<QDate>(105), d);
    QCOMPARE(p.value<QDateTime>(106), dt);
    QCOMPARE(p.value<QDate>(103), d);
    QCOMPARE(p.value<QDate>(106), d);
    QCOMPARE(p.value<QDateTime>(102), ddt);
    QCOMPARE(p.value<QDateTime>(105), ddt);

    /* Reset again */
    values = p.values();
    keys = values.keys();
    foreach (int key, keys)
        QVERIFY(p.setValue(key, QVariant()));
    QCOMPARE(p.values(), emptyValues);

    /* Check that we can add a null variant */
    //QVERIFY(p.setValue("nullvariant", QVariant()));
    //QVERIFY(p.hasValue("nullvariant"));
    //QCOMPARE(p.value("nullvariant"), QString());
    //QCOMPARE(p.value("nullvariant"), QVariant());
    //QVERIFY(p.removeValue("nullvariant"));
    //QVERIFY(p.values().count() == 0);

    /* Check that adding a value, then setting it to null updates it */
    //QVERIFY(p.setValue("string", QString("string value")));
    //QCOMPARE(p.values().count(), 1);
    //QCOMPARE(p.value("string"), QString("string value"));
    //QVERIFY(p.setValue("string", QVariant()));
    //QCOMPARE(p.values().count(), 1);
    //QVERIFY(p.hasValue("string"));
    //QVERIFY(p.removeValue("string"));
    //QCOMPARE(p.values().count(), 0);

    /* See if adding a null QString triggers the same behaviour */
    //QVERIFY(p.setValue("string", QString("string value")));
    //QCOMPARE(p.values().count(), 1);
    //QCOMPARE(p.value("string"), QString("string value"));
    //QVERIFY(p.setValue("string", QString()));
    //QCOMPARE(p.values().count(), 1);
    //QVERIFY(p.hasValue("string"));
    //QVERIFY(p.removeValue("string"));
    //QCOMPARE(p.values().count(), 0);

    /* Check adding a null value removes the field */
    p.setValue(101, "stringvalue");
    QVERIFY(p.values().contains(101));
    QVERIFY(p.value(101) == QString("stringvalue"));
    p.setValue(101, QVariant());
    QVERIFY(!p.values().contains(101));

    /* Check adding a field whose value is an empty string */
    p.setValue(101, "");
    QVERIFY(p.values().contains(101));
    QVERIFY(p.value(101) == QString(""));

    /* Check accessing a missing value */
    QCOMPARE(p.value(666).toString(), QString());
    QVERIFY(p.setValue(666, "changed my mind"));
    QCOMPARE(p.value(666).toString(), QString("changed my mind"));

    /* Check removing a missing value */
    QVERIFY(!p.removeValue(777));
    QVERIFY(!p.removeValue(QOrganizerItemComment::FieldComment));

    p.setValue(107, "555");
    p.setValue(QOrganizerItemPriority::FieldPriority, "1234");

    /* Check removing a real value */
    QVERIFY(p.removeValue(101));
    QVERIFY(p.removeValue(107));
    QVERIFY(p.removeValue(QOrganizerItemPriority::FieldPriority));
}

void tst_QOrganizerItemDetail::hash()
{
    QOrganizerItemDetail detail1(QOrganizerItemDetail::TypeComment);
    detail1.setValue(QOrganizerItemComment::FieldComment, "value");
    QOrganizerItemDetail detail2(QOrganizerItemDetail::TypeComment);
    detail2.setValue(QOrganizerItemComment::FieldComment, "value");
    QOrganizerItemDetail detail3(QOrganizerItemDetail::TypeComment);
    detail3.setValue(QOrganizerItemComment::FieldComment, "different value");
    QVERIFY(qHash(detail1) == qHash(detail2));
    QVERIFY(qHash(detail1) != qHash(detail3));
    QSet<QOrganizerItemDetail> set;
    set.insert(detail1);
    set.insert(detail2);
    set.insert(detail3);
    QCOMPARE(set.size(), 2);
}

void tst_QOrganizerItemDetail::datastream()
{
    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    QOrganizerItemDetail detailIn(QOrganizerItemDetail::TypeComment);
    detailIn.setValue(QOrganizerItemComment::FieldComment, "value1");
    stream1 << detailIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QOrganizerItemDetail detailOut;
    stream2 >> detailOut;
    QCOMPARE(detailOut, detailIn);
}

void tst_QOrganizerItemDetail::traits()
{
    QCOMPARE(sizeof(QOrganizerItemDetail), sizeof(void *));
    QTypeInfo<QOrganizerItemDetail> ti;
    QVERIFY(ti.isComplex);
    QVERIFY(!ti.isStatic);
    QVERIFY(!ti.isLarge);
    QVERIFY(!ti.isPointer);
    QVERIFY(!ti.isDummy);
}

void tst_QOrganizerItemDetail::keys()
{
    QOrganizerItemDetail d;
    QOrganizerItemDetail d2;
    QVERIFY(d.key() != d2.key());

    d = d2;
    QVERIFY(d.key() == d2.key());
    d.resetKey();
    QVERIFY(d.key() != d2.key());
}

QTEST_MAIN(tst_QOrganizerItemDetail)
#include "tst_qorganizeritemdetail.moc"
