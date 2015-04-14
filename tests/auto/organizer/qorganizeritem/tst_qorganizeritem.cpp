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
#include <QtOrganizer/qorganizeritemid.h>

//TESTED_COMPONENT=src/organizer

QTORGANIZER_USE_NAMESPACE

static inline QOrganizerItemId makeId(const QString &managerName, uint id)
{
    return QOrganizerItemId(QStringLiteral("qtorganizer:%1:").arg(managerName), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}


class tst_QOrganizerItem: public QObject
{
Q_OBJECT

public:
    tst_QOrganizerItem();
    virtual ~tst_QOrganizerItem();

private slots:
    void details();
    void displayLabel();
    void description();
    void comments();
    void tags();
    void type();
    void emptiness();
    void idComparison();
    void idHash();
    void idStringFunctions();
    void hash();
    void datastream();
    void traits();
    void idTraits();
    void debugOutput();

    void event();
    void todo();
    void journal();
    void note();
    void eventOccurrence();
    void todoOccurrence();
    void testDebugStreamOut();
    void itemsCompare();
};

tst_QOrganizerItem::tst_QOrganizerItem()
{
}

tst_QOrganizerItem::~tst_QOrganizerItem()
{
}

void tst_QOrganizerItem::details()
{
    // Check that detail keys are unique, regardless of order of initialisation
    // First, construct the detail first, then the contact
    QOrganizerItemComment note;
    note.setComment("Example Note");
    QOrganizerItem keyTest;
    QVERIFY(keyTest.saveDetail(&note));
    QList<QOrganizerItemDetail> allDetails = keyTest.details();
    QList<int> detailKeys;
    foreach (const QOrganizerItemDetail& det, allDetails) {
        int currKey = det.key();
        QVERIFY(!detailKeys.contains(currKey));
        detailKeys.append(currKey);
    }
    // Second, construct the detail after the contact has been constructed
    QOrganizerItemLocation adr;
    adr.setLabel("this is a test address");
    QVERIFY(keyTest.saveDetail(&adr));
    allDetails = keyTest.details();
    detailKeys.clear();
    foreach (const QOrganizerItemDetail& det, allDetails) {
        int currKey = det.key();
        QVERIFY(!detailKeys.contains(currKey));
        detailKeys.append(currKey);
    }

    // now test for default construction sanity
    QOrganizerItem oi;

    // Test there are no details (apart from type) by default
    QVERIFY(oi.isEmpty() == true);
    QCOMPARE(oi.details().count(), 1);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeLocation).count(), 0);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeLocation).count(), 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeLocation).count(), 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeLocation).count(), 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    // Test retrieving the default details (type)
    QList<QOrganizerItemDetail> details = oi.details();
    QVERIFY(details.at(0).type() == QOrganizerItemDetail::TypeItemType);

    QOrganizerItemDetail detail = oi.detail(QOrganizerItemDetail::TypeExtendedDetail);
    QVERIFY(detail.isEmpty());

    // retrieve the first detail using the empty definition name accessor.
    detail = oi.detail();
    QVERIFY(detail == details.at(0));

    QVERIFY(oi.details(QOrganizerItemDetail::TypeExtendedDetail).count() == 0);

    // Add a detail
    QOrganizerItemLocation a;
    a.setLabel("another test address label");
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi.isEmpty() == false);

    QVERIFY(oi.details().count() == 2);

    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.detail(QOrganizerItemDetail::TypeLocation)), a);

    // Remove detail
    QVERIFY(oi.removeDetail(&a));
    QCOMPARE(oi.details().count(), 1);
    QVERIFY(oi.isEmpty() == true);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeLocation).count(), 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    // Try removing it again
    QVERIFY(!oi.removeDetail(&a));

    // Add again, and remove a different way (retrieved copy)
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi.isEmpty() == false);
    QCOMPARE(oi.details().count(), 2);
    QOrganizerItemLocation a2 = oi.detail(QOrganizerItemDetail::TypeLocation);
    QCOMPARE(a, a2);

    QVERIFY(oi.removeDetail(&a2));
    QCOMPARE(oi.details().count(), 1);
    QVERIFY(oi.isEmpty() == true);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    QCOMPARE(a, a2);

    // Add again again, and remove a different way (base class)
    QVERIFY(oi.saveDetail(&a));
    QCOMPARE(oi.details().count(), 2);
    QOrganizerItemDetail a3 = oi.detail(QOrganizerItemDetail::TypeLocation);
    QVERIFY(a == a3);

    QVERIFY(oi.removeDetail(&a3));
    QCOMPARE(oi.details().count(), 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    QVERIFY(a == a3);

    // now we want to add multiple details of the same type, and test that retrieval works correctly.
    a2 = QOrganizerItemLocation();
    a2.setLabel("22222");
    a2.setValue(100, QVariant("22222-2")); // non-existing field
    oi.saveDetail(&a);
    oi.saveDetail(&a2);
    QCOMPARE(oi.details().count(), 3);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 2);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 2);
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.detail(QOrganizerItemDetail::TypeLocation)), a);
    QVERIFY(oi.removeDetail(&a2));

    // Now try adding a detail to multiple contacts

    QOrganizerItem oi2;
    QVERIFY(oi2.isEmpty() == true);
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi2.saveDetail(&a));
    QVERIFY(oi2.isEmpty() == false);

    QCOMPARE(oi.details().count(), 2);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.detail(QOrganizerItemDetail::TypeLocation)), a);

    QCOMPARE(oi2.details().count(), 2);
    QVERIFY(oi2.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(oi2.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(!oi2.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi2.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi2.detail(QOrganizerItemDetail::TypeLocation)), a);

    // Now try removing it from one
    QVERIFY(oi.removeDetail(&a));

    // Make sure it's gone from the first contact
    QVERIFY(oi.isEmpty() == true);
    QCOMPARE(oi.details().count(), 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    // but not the second
    QVERIFY(oi2.isEmpty() == false);
    QCOMPARE(oi2.details().count(), 2);
    QVERIFY(oi2.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(oi2.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(!oi2.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi2.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi2.detail(QOrganizerItemDetail::TypeLocation)), a);

    // Now remove it from the second as well
    QVERIFY(oi2.removeDetail(&a));

    // Make sure it's gone from both
    QCOMPARE(oi.details().count(), 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    QCOMPARE(oi2.details().count(), 1);
    QVERIFY(oi2.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi2.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi2.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi2.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    // add a, add b, remove a, add a, remove b, remove a
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi2.saveDetail(&a));
    QVERIFY(oi.removeDetail(&a));
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi2.removeDetail(&a));
    QVERIFY(oi.removeDetail(&a));

    // Now add a detail with the same values twice
    QOrganizerItemLocation one;
    QOrganizerItemLocation two;

    one.setLabel("12345");
    two.setLabel("12345");

    // add it once
    QVERIFY(oi.saveDetail(&one));
    QCOMPARE(oi.details().count(), 2);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.detail(QOrganizerItemDetail::TypeLocation)), one);

    // add it twice
    QVERIFY(oi.saveDetail(&two));
    QCOMPARE(oi.details().count(), 3);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 2);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 2);
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.detail(QOrganizerItemDetail::TypeLocation)), one);
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.details(QOrganizerItemDetail::TypeLocation)[0]), one);
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.details(QOrganizerItemDetail::TypeLocation)[1]), two);

    // Remove it once
    QVERIFY(oi.removeDetail(&one));
    QCOMPARE(oi.details().count(), 2);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 1);
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(!oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QCOMPARE(static_cast<QOrganizerItemLocation>(oi.detail(QOrganizerItemDetail::TypeLocation)), two);

    // Remove it twice
    QVERIFY(oi.removeDetail(&two));
    QCOMPARE(oi.details().count(), 1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 0);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation).isEmpty());

    // Null pointer tests
    QVERIFY(oi.saveDetail(0) == false);
    QVERIFY(oi.removeDetail(0) == false);

    // Reference tests...
    QOrganizerItemDetail& ref = one;
    QVERIFY(oi.saveDetail(&one));
    QVERIFY(ref == one);
    one.setLabel("56678");
    QVERIFY(oi.saveDetail(&one));
    QVERIFY(ref == one);

    // Retrieve the detail again and modify it
    QOrganizerItemLocation three = oi.detail(QOrganizerItemDetail::TypeLocation);
    QVERIFY(ref == three);
    QVERIFY(one == three);
    three.setLabel("542343");
    QVERIFY(oi.saveDetail(&three));

    // Now see if we got any updates to ref/one
    QVERIFY(ref == one);
    QVERIFY(ref != three);

    // test saving of a detail with an empty field.
    QOrganizerItemLocation four;
    four.setLabel("");
    oi.saveDetail(&four);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).count() == 2);
    QVERIFY(!four.values().isEmpty()); // an empty qstring is not invalid; make sure it exists in the detail.

    // ensure that clearing a contact's details works correctly
    QOrganizerItemPriority priorityDetail;
    priorityDetail.setPriority(QOrganizerItemPriority::VeryHighPriority);
    oi.saveDetail(&priorityDetail);
    QCOMPARE(oi.detail(QOrganizerItemDetail::TypePriority).value(QOrganizerItemPriority::FieldPriority).toInt(), static_cast<int>(QOrganizerItemPriority::VeryHighPriority));
    QVERIFY(oi.details().size() > 0);
    QVERIFY(!oi.isEmpty());
    QOrganizerItemId oldId = oi.id();
    oi.clearDetails();
    QCOMPARE(oi.details().size(), 1); // always has an item type.
    QCOMPARE(oi.detail(QOrganizerItemDetail::TypePriority).value(QOrganizerItemPriority::FieldPriority).toString(), QString());
    QVERIFY(oi.isEmpty());
    QCOMPARE(oi.id(), oldId); // id shouldn't change.
}

void tst_QOrganizerItem::displayLabel()
{
    QOrganizerItem oi;

    QString label = oi.displayLabel();
    QVERIFY(label.isEmpty());
    oi.setDisplayLabel("Test Event Display Label");
    QVERIFY(oi.displayLabel() == QString("Test Event Display Label"));

    /* Remove the detail via removeDetail */
    QOrganizerItemDisplayLabel old;
    QCOMPARE(oi.details().count(), 2); // type + label
    QVERIFY(!oi.removeDetail(&old)); // should fail.
    QVERIFY(oi.isEmpty() == false);
    QVERIFY(oi.details().count() == 2); // it should not be removed!

    QOrganizerItemDisplayLabel displayLabel;
    displayLabel.setLabel("test label");
    oi.setDisplayLabel(displayLabel.label());
    QVERIFY(oi.displayLabel() == displayLabel.label());

    /* Test self assign */
    oi.operator =(oi);
    QVERIFY(oi.details().count() == 2);
    QVERIFY(oi.isEmpty() == false);
}


void tst_QOrganizerItem::description()
{
    QOrganizerItem oi;

    QString description = oi.description();
    QVERIFY(description.isEmpty());
    oi.setDescription("Test Event Description");
    QVERIFY(oi.description() == QString("Test Event Description"));

    /* Remove the detail via removeDetail */
    QOrganizerItemDescription old;
    QCOMPARE(oi.details().count(), 2); // type + description
    QVERIFY(!oi.removeDetail(&old)); // should fail.
    QVERIFY(oi.isEmpty() == false);
    QVERIFY(oi.details().count() == 2); // it should not be removed!

    QOrganizerItemDescription descr;
    descr.setDescription("test description");
    oi.setDescription(descr.description());
    QVERIFY(oi.description() == descr.description());

    /* Test self assign */
    oi.operator =(oi);
    QVERIFY(oi.details().count() == 2);
    QVERIFY(oi.isEmpty() == false);
}

void tst_QOrganizerItem::comments()
{
    QOrganizerItem oi;

    QStringList comments = oi.comments();
    QVERIFY(comments.isEmpty());
    oi.addComment("test comment");
    QCOMPARE(oi.comments().size(), 1);
    QVERIFY(oi.details().count() == 2);
    QVERIFY(oi.comments().at(0) == QString("test comment"));
    oi.addComment("another test comment");
    QCOMPARE(oi.comments().size(), 2);
    QVERIFY(oi.details().count() == 3);
    QVERIFY(oi.comments().contains(QString("test comment")));
    QVERIFY(oi.comments().contains(QString("another test comment")));

    oi.clearComments();
    QVERIFY(oi.comments().size() == 0);
    QVERIFY(oi.details().count() == 1); // should have a type detail left.

    QOrganizerItemComment comment;
    comment.setComment("yet another test comment");
    oi.saveDetail(&comment);
    QCOMPARE(oi.comments().size(), 1);
    QVERIFY(oi.details().count() == 2);
    QVERIFY(oi.comments().at(0) == QString("yet another test comment"));

    oi.removeDetail(&comment);
    QVERIFY(oi.comments().size() == 0);
    QVERIFY(oi.details().count() == 1); // should have a type detail left.

    oi.clearComments();
    oi.setComments(QStringList() << "comment 1" << "comment 2");
    QVERIFY(oi.comments().size() == 2);
    QVERIFY(oi.details().count() == 3);

    /* Test self assign */
    oi.operator =(oi);
    QVERIFY(oi.details().count() == 3);
    QVERIFY(oi.isEmpty() == false);
}

void tst_QOrganizerItem::tags()
{
    QOrganizerItem item;
    QVERIFY(item.tags().isEmpty());

    item.addTag("tag 1");
    QStringList tags;
    tags.append("tag 1");
    QCOMPARE(item.tags(), tags);
    QList<QOrganizerItemDetail> tagDetails = item.details(QOrganizerItemDetail::TypeTag);
    QCOMPARE(tagDetails.size(), 1);
    QCOMPARE(tagDetails.first().value(QOrganizerItemTag::FieldTag).toString(), QString(QStringLiteral("tag 1")));

    item.clearTags();
    QVERIFY(item.tags().isEmpty());
    QVERIFY(item.details(QOrganizerItemDetail::TypeTag).isEmpty());

    tags.append("tag 2"); // tags is now "tag 1", "tag 2"
    item.setTags(tags);
    QCOMPARE(item.tags(), tags);
    tagDetails = item.details(QOrganizerItemDetail::TypeTag);
    QCOMPARE(tagDetails.size(), 2);
    QCOMPARE(tagDetails.at(0).value(QOrganizerItemTag::FieldTag).toString(), QString(QStringLiteral("tag 1")));
    QCOMPARE(tagDetails.at(1).value(QOrganizerItemTag::FieldTag).toString(), QString(QStringLiteral("tag 2")));
}

void tst_QOrganizerItem::debugOutput()
{
    QOrganizerCollection c;
    QOrganizerItem item;
    QOrganizerItemDetail d;

    // Test that these can be used as the first argument to qDebug()
    qDebug() << item.id();
    qDebug() << item.collectionId();
    qDebug() << item;
    qDebug() << d;
    qDebug() << c;

    // And that other things can come after them (return type)
    qDebug() << item.id() << "id";
    qDebug() << item.collectionId() << "collection id";
    qDebug() << item << "item";
    qDebug() << d << "detail";
    qDebug() << c << "collection";

    // And for kicks, that other things can come first
    qDebug() << " " << item.id();
    qDebug() << " " << item.collectionId();
    qDebug() << " " << item;
    qDebug() << " " << d;
    qDebug() << " " << c;
}

void tst_QOrganizerItem::type()
{
    QOrganizerItem oi;
    QVERIFY(oi.isEmpty() == true);

    // ensure that the default type is the QOrganizerItemType::TypeUnknown type
    QVERIFY(oi.type() == QOrganizerItemType::TypeUndefined);

    // now set it to be an event via the type mutator, and test that it works
    QOrganizerItemType eventType;
    eventType.setType(QOrganizerItemType::TypeEvent);
    oi.setType(eventType.type());
    QVERIFY(oi.type() == QOrganizerItemType::TypeEvent);

    // set it back to a note, via the string mutator
    oi.setType(QOrganizerItemType::TypeNote);
    QVERIFY(oi.type() == QOrganizerItemType::TypeNote);
    QVERIFY(oi.isEmpty() == true); // type doesn't affect emptiness
}

void tst_QOrganizerItem::emptiness()
{
    QOrganizerItem oi;
    QVERIFY(oi.isEmpty() == true);
    QVERIFY(oi.id().isNull() == true);

    oi.setType(QOrganizerItemType::TypeNote);
    QVERIFY(oi.type() == QOrganizerItemType::TypeNote);
    QVERIFY(oi.isEmpty() == true); // type doesn't affect emptiness
}

void tst_QOrganizerItem::idComparison()
{
    QOrganizerItemId id1(makeId("a", 1));
    QOrganizerItemId id2(makeId("a", 1));
    QVERIFY(!(id1 < id2));
    QVERIFY(!(id2 < id1));
    QVERIFY(id1 == id2);

    QOrganizerItemId id3(makeId("a", 2));
    QOrganizerItemId id4(makeId("b", 1));
    QOrganizerItemId id5(makeId("b", 2));
    QVERIFY((((id1 < id3) && !(id3 < id1)) || ((id3 < id1) && !(id1 < id3))) && (id1 != id3));
    QVERIFY((((id1 < id4) && !(id4 < id1)) || ((id4 < id1) && !(id1 < id4))) && (id1 != id4));
    QVERIFY((((id3 < id4) && !(id4 < id3)) || ((id4 < id3) && !(id3 < id4))) && (id3 != id4));
    QVERIFY((((id1 < id5) && !(id5 < id1)) || ((id5 < id1) && !(id1 < id5))) && (id3 != id4));

    QOrganizerItemId id6;
    QOrganizerItemId id7(QString(), "1");
    QOrganizerItemId id8(QString(), "2");
    QOrganizerItemId id9(QStringLiteral("qtorganizer:basic:"), "");
    QVERIFY(id6.isNull());
    QVERIFY(id7.isNull());
    QVERIFY(id8.isNull());
    QVERIFY(id9.isNull());
    QVERIFY(id6 == id7);
    QVERIFY(!(id6 < id7));
    QVERIFY(id7 == id6);
    QVERIFY(!(id7 < id6));
    QVERIFY(id7 == id8);
    QVERIFY(!(id7 < id8));
    QVERIFY(id8 == id7);
    QVERIFY(!(id9 < id8));
    QVERIFY(id8 == id9);
    QVERIFY(!(id8 < id9));
    QVERIFY(id9 == id8);
    QVERIFY(!(id9 < id8));

    QVERIFY(!(id1 == id6));
    QVERIFY(!(id1 < id6));
    QVERIFY(id6 < id1);
    QVERIFY(!(id1 == id7));
    QVERIFY(!(id1 < id7));
    QVERIFY(id7 < id1);
    QVERIFY(!(id1 == id8));
    QVERIFY(!(id1 < id8));
    QVERIFY(id8 < id1);
    QVERIFY(!(id1 == id9));
    QVERIFY(!(id1 < id9));
    QVERIFY(id9 < id1);
}

void tst_QOrganizerItem::idHash()
{
    QOrganizerItemId id1(makeId("a", 1));
    QOrganizerItemId id2(makeId("a", 1));
    QOrganizerItemId id3(makeId("b", 1));
    QOrganizerItemId id4(makeId("a", 2));
    // note that the hash function ignores the managerUri
    QCOMPARE(qHash(id1), qHash(id2));
    QCOMPARE(qHash(id1), qHash(id3));
    QVERIFY(qHash(id1) != qHash(id4));

    QSet<QOrganizerItemId> set;
    set.insert(id1);
    set.insert(id2);
    set.insert(id3);
    set.insert(id4);
    QCOMPARE(set.size(), 3);
}

void tst_QOrganizerItem::idStringFunctions()
{
    // TODO: review test
    QOrganizerItemId id1(makeId("a", 1));
    QOrganizerItemId id2(makeId("a", 1));
    QOrganizerItemId id3(makeId("b", 1));
    QOrganizerItemId id4(makeId("a", 2));
    QVERIFY(qHash(id1) == qHash(id2));
    QVERIFY(qHash(id1) != qHash(id4));

    // note that the toString and fromString functions are
    // engine and id specific.  This test merely checks that
    // the API is hooked up correctly.

    QVERIFY(id1.toString() == id2.toString());
    QVERIFY(id1.toString() != id3.toString());
    QVERIFY(id1.toString() != id4.toString());
    QVERIFY(id3.toString() != id4.toString());

    // this should "work" -- string of the correct format
    const uint numericId2 = 2u;
    const QByteArray localId2 = QByteArray(reinterpret_cast<const char *>(&numericId2), sizeof(uint));
    QString prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString("::") + localId2.toHex();
    QOrganizerItemId rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format and one parameter, but requires a working backend
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + QString("key=value") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format and some parameters, but requires a working backend
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + QString("key=value&key2=value2") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format but misses the value for a parameter
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + QString("key=value&key2=") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string misses a field (the parameters)
    prebuiltidstring = QString("qtorganizer") + QString(":") + QString("a") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerItemId()); // invalid so should be null.

    // this string misses two fields (params plus manager uri)
    prebuiltidstring = QString("qtorganizer") + QString(":") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerItemId()); // invalid so should be null.

    // this string misses the prefix (qtorganizer)
    prebuiltidstring = QString("notorganizer") + QString(":") + QString("a") + QString("::") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerItemId()); // invalid so should be null.

    // this string misses the manager uri
    prebuiltidstring = QString("notorganizer") + QString(":::") + localId2.toHex();
    rebuiltid = QOrganizerItemId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QOrganizerItemId()); // invalid so should be null.
}

void tst_QOrganizerItem::hash()
{
    // TODO: review test
    QOrganizerItemId id(makeId("a", 1));
    QOrganizerItem oi1;
    oi1.setId(id);
    QOrganizerItemDetail detail1(QOrganizerItemDetail::TypeComment);
    detail1.setValue(QOrganizerItemComment::FieldComment, "value");
    oi1.saveDetail(&detail1);
    QOrganizerItem oi2;
    oi2.setId(id);
    oi2.saveDetail(&detail1);
    QOrganizerItem oi3;
    oi3.setId(id);
    QOrganizerItemDetail detail3(QOrganizerItemDetail::TypeComment);
    detail3.setValue(QOrganizerItemComment::FieldComment, "another value");
    oi3.saveDetail(&detail3);
    QOrganizerItem oi4; // no details
    oi4.setId(id);
    QOrganizerItem oi5;
    oi5.setId(id);
    oi5.saveDetail(&detail1);qDebug()<<__LINE__;
    QVERIFY(qHash(oi1) == qHash(oi2));qDebug()<<__LINE__;
    QVERIFY(qHash(oi1) != qHash(oi3));qDebug()<<__LINE__;
    QVERIFY(qHash(oi1) != qHash(oi4));qDebug()<<__LINE__;
    QVERIFY(qHash(oi1) == qHash(oi5));qDebug()<<__LINE__;
}

void tst_QOrganizerItem::datastream()
{
    // item datastreaming
    QByteArray buffer;
    QOrganizerEvent itemIn;
    itemIn.addComment("test comment");
    QOrganizerItem itemOut;
    QOrganizerItemId originalId;
    QOrganizerCollectionId originalCollectionId;

    // first, stream an item with a complete id
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        QOrganizerManager om("memory");
        QVERIFY(om.saveItem(&itemIn)); // fill in its ID
        originalId = itemIn.id();
        originalCollectionId = itemIn.collectionId();
        stream1 << itemIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> itemOut;
        //QCOMPARE(itemOut, itemIn); // can't do QCOMPARE because detail keys get changed.
        QVERIFY(itemOut.details() == itemIn.details());
        QVERIFY(itemOut.id() == itemIn.id());
    }

    // TODO : review tests
    // second, stream an item with an id with the mgr uri set, local id null
/*    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        QOrganizerItemId modifiedId = originalId;
        modifiedId.setLocalId(QOrganizerItemLocalId());
        itemIn.setId(modifiedId);
        stream1 << itemIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> itemOut;
        //QCOMPARE(itemOut, itemIn); // can't do QCOMPARE because detail keys get changed.
        QVERIFY(itemOut.details() == itemIn.details());
        QVERIFY(itemOut.id() == itemIn.id());
    }

    // third, stream an item with an id with the mgr uri null, local id set
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        QOrganizerItemId modifiedId = originalId;
        modifiedId.setManagerUri(QString()); // this will clear the local id!
        modifiedId.setLocalId(originalId.localId()); // so reset it and make sure things don't fall over.
        itemIn.setId(modifiedId);
        stream1 << itemIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> itemOut;
        //QCOMPARE(itemOut, itemIn); // can't do QCOMPARE because detail keys get changed.
        QVERIFY(itemOut.details() == itemIn.details());
        QVERIFY(itemOut.id() != itemIn.id()); // in this case, with null mgr uri, the id doesn't get serialized.
    }*/

    // fourth, stream an item with null ids
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        itemIn.setId(QOrganizerItemId());
        itemIn.setCollectionId(QOrganizerCollectionId());
        stream1 << itemIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> itemOut;
        //QCOMPARE(itemOut, itemIn); // can't do QCOMPARE because detail keys get changed.
        QVERIFY(itemOut.details() == itemIn.details());
        QVERIFY(itemOut.id() == itemIn.id());
    }

    // fifth, stream an item with a collection id
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        itemIn.setId(QOrganizerItemId());
        itemIn.setCollectionId(originalCollectionId);
        stream1 << itemIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> itemOut;
        //QCOMPARE(itemOut, itemIn); // can't do QCOMPARE because detail keys get changed.
        QVERIFY(itemOut.details() == itemIn.details());
        QVERIFY(itemOut.collectionId() == itemIn.collectionId());
        QVERIFY(itemOut.id() == itemIn.id());
    }

    // id datastreaming
    buffer.clear();
    QOrganizerItemId inputId;
    QOrganizerItemId outputId;

    // first, stream the whole id (mgr uri set, local id set)
    {
        inputId = originalId;
        QString serializedId = inputId.toString();
        outputId = QOrganizerItemId::fromString(serializedId);
        QCOMPARE(inputId, outputId);

        inputId = originalId;
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    // TODO : review tests
    // second, stream a partial id (mgr uri null, local id set)
/*    {
        inputId.setManagerUri(QString());
        inputId.setLocalId(originalId.localId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;

        // because the manager uri is null, we cannot stream it in.
        QVERIFY(outputId.isNull());
        QVERIFY(!inputId.isNull());
    }

    // third, stream a partial id (mgr uri set, local id null).
    {
        inputId.setManagerUri(originalId.managerUri());
        inputId.setLocalId(QOrganizerItemLocalId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }*/

    // fourth, stream a null id
    {
        inputId = QOrganizerItemId();
        QString serializedId = inputId.toString();
        outputId = QOrganizerItemId::fromString(serializedId);
        QCOMPARE(inputId, outputId);

        inputId = QOrganizerItemId();
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    // TODO : review tests
    // fifth, stream an id after changing it's manager uri string.
/*    {
        inputId.setManagerUri(originalId.managerUri());
        inputId.setLocalId(originalId.localId());
        inputId.setManagerUri("test manager uri"); // should clear the local id.
        QVERIFY(inputId.localId() == QOrganizerItemLocalId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    // sixth, stream an id after changing it's manager uri string, and resetting the local id.
    // this should cause great problems, because the manager doesn't exist so it shouldn't
    // be able to deserialize.  Make sure it's handled gracefully.
    {
        inputId.setManagerUri(originalId.managerUri());
        inputId.setManagerUri("test manager uri"); // should clear the local id.
        inputId.setLocalId(originalId.localId());
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QVERIFY(outputId.isNull());
    }*/
}

void tst_QOrganizerItem::traits()
{
    QCOMPARE(sizeof(QOrganizerItem), sizeof(void *));
    QVERIFY(QTypeInfo<QOrganizerItem>::isComplex);
    QVERIFY(!QTypeInfo<QOrganizerItem>::isStatic);
    QVERIFY(!QTypeInfo<QOrganizerItem>::isLarge);
    QVERIFY(!QTypeInfo<QOrganizerItem>::isPointer);
    QVERIFY(!QTypeInfo<QOrganizerItem>::isDummy);
}

void tst_QOrganizerItem::idTraits()
{
    QCOMPARE(sizeof(QOrganizerItemId), 2*sizeof(void *));
    QVERIFY(QTypeInfo<QOrganizerItemId>::isComplex);
    QVERIFY(!QTypeInfo<QOrganizerItemId>::isStatic);
    QVERIFY(QTypeInfo<QOrganizerItemId>::isLarge);
    QVERIFY(!QTypeInfo<QOrganizerItemId>::isPointer);
    QVERIFY(!QTypeInfo<QOrganizerItemId>::isDummy);
}

void tst_QOrganizerItem::event()
{
    QOrganizerEvent testEvent;
    QCOMPARE(testEvent.type(), QOrganizerItemType::TypeEvent);

    testEvent.setLocation("test address");
    QCOMPARE(testEvent.location(), QString("test address"));

    testEvent.setStartDateTime(QDateTime(QDate::currentDate()));
    QCOMPARE(testEvent.startDateTime(), QDateTime(QDate::currentDate()));
    testEvent.setEndDateTime(QDateTime(QDate::currentDate().addDays(1)));
    QCOMPARE(testEvent.endDateTime(), QDateTime(QDate::currentDate().addDays(1)));
    QVERIFY(!testEvent.isAllDay()); // default to not all day
    testEvent.setAllDay(true);
    QVERIFY(testEvent.isAllDay());

    testEvent.setPriority(QOrganizerItemPriority::VeryHighPriority);
    QCOMPARE(testEvent.priority(), QOrganizerItemPriority::VeryHighPriority);
    testEvent.setPriority(QOrganizerItemPriority::VeryLowPriority);
    QCOMPARE(testEvent.priority(), QOrganizerItemPriority::VeryLowPriority);


    QSet<QDate> rdates;
    testEvent.setRecurrenceDate(QDate::currentDate());
    QCOMPARE(testEvent.recurrenceDates(), QSet<QDate>() << QDate::currentDate());
    rdates << QDate::currentDate() << QDate::currentDate().addDays(3) << QDate::currentDate().addDays(8);
    testEvent.setRecurrenceDates(rdates);
    QCOMPARE(testEvent.recurrenceDates(), rdates);

    QSet<QDate> exdates;
    testEvent.setExceptionDate(QDate::currentDate());
    QCOMPARE(testEvent.exceptionDates(), QSet<QDate>() << QDate::currentDate());
    exdates << QDate::currentDate().addDays(3);
    testEvent.setExceptionDates(exdates);
    QCOMPARE(testEvent.exceptionDates(), exdates);

    QSet<QOrganizerRecurrenceRule> rrules;
    QOrganizerRecurrenceRule rrule;

    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::NoLimit);
    QVERIFY(rrule.limitCount() == -1);
    QVERIFY(rrule.limitDate().isNull());

    rrule.setLimit(1);
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::CountLimit);
    QVERIFY(rrule.limitCount() == 1);
    QVERIFY(rrule.limitDate().isNull());

    rrule.setLimit(-1);
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::NoLimit);
    QVERIFY(rrule.limitCount() == -1);
    QVERIFY(rrule.limitDate().isNull());

    rrule.setLimit(0);
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::CountLimit);
    QVERIFY(rrule.limitCount() == 0);
    QVERIFY(rrule.limitDate().isNull());

    rrule.setLimit(-100);
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::NoLimit);
    QVERIFY(rrule.limitCount() == -1);
    QVERIFY(rrule.limitDate().isNull());

    rrule.setLimit(QDate());
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::NoLimit);
    QVERIFY(rrule.limitCount() == -1);
    QVERIFY(rrule.limitDate().isNull());

    rrule.setLimit(QDate(2010, 10, 6));
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::DateLimit);
    QVERIFY(rrule.limitCount() == -1);
    QVERIFY(rrule.limitDate() == QDate(2010, 10, 6));

    rrule.setLimit(QDate(2010, 13, 34));
    QVERIFY(rrule.limitType() == QOrganizerRecurrenceRule::NoLimit);
    QVERIFY(rrule.limitCount() == -1);
    QVERIFY(rrule.limitDate() == QDate());


    rrule.setLimit(2);
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrules << rrule;
    testEvent.setRecurrenceRule(rrule);
    QVERIFY(testEvent.recurrenceRules() == rrules);
    QVERIFY(testEvent.recurrenceRule() == rrule);
    testEvent.setRecurrenceRule(rrule);
    QVERIFY(testEvent.recurrenceRules() == rrules);
    QVERIFY(testEvent.recurrenceRule() == rrule);

    QSet<QOrganizerRecurrenceRule> exrules;
    QOrganizerRecurrenceRule exrule;
    exrule.setLimit(1);
    exrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    exrules << exrule;
    testEvent.setExceptionRules(exrules);
    QVERIFY(testEvent.exceptionRules() == exrules);
    QVERIFY(testEvent.exceptionRule() == exrule);
    testEvent.setExceptionRule(exrule);
    QVERIFY(testEvent.exceptionRules() == exrules);
    QVERIFY(testEvent.exceptionRule() == exrule);
}

void tst_QOrganizerItem::todo()
{
    QOrganizerTodo testTodo;
    QCOMPARE(testTodo.type(), QOrganizerItemType::TypeTodo);

    QCOMPARE(testTodo.status(), QOrganizerTodoProgress::StatusNotStarted);
    testTodo.setStatus(QOrganizerTodoProgress::StatusInProgress);
    QCOMPARE(testTodo.status(), QOrganizerTodoProgress::StatusInProgress);

    QCOMPARE(testTodo.progressPercentage(), 0);
    testTodo.setProgressPercentage(50);
    QCOMPARE(testTodo.progressPercentage(), 50);
    testTodo.setStatus(QOrganizerTodoProgress::StatusComplete);
    QCOMPARE(testTodo.progressPercentage(), 50); // XXX TODO: should this update automatically?

    QDateTime currentDateTime = QDateTime::currentDateTime();
    testTodo.setStartDateTime(currentDateTime);
    QCOMPARE(testTodo.startDateTime(), currentDateTime);
    testTodo.setDueDateTime(QDateTime(QDate::currentDate()));
    QCOMPARE(testTodo.dueDateTime(), QDateTime(QDate::currentDate()));
    testTodo.setFinishedDateTime(QDateTime(QDate::currentDate().addDays(1)));
    QCOMPARE(testTodo.finishedDateTime(), QDateTime(QDate::currentDate().addDays(1)));

    QVERIFY(testTodo.isAllDay() == false); // default should not be all day event.
    testTodo.setAllDay(true);
    QVERIFY(testTodo.isAllDay() == true);
    testTodo.setAllDay(false);
    QVERIFY(testTodo.isAllDay() == false);

    testTodo.setPriority(QOrganizerItemPriority::VeryHighPriority);
    QCOMPARE(testTodo.priority(), QOrganizerItemPriority::VeryHighPriority);
    testTodo.setPriority(QOrganizerItemPriority::VeryLowPriority);
    QCOMPARE(testTodo.priority(), QOrganizerItemPriority::VeryLowPriority);

    QSet<QDate> rdates;
    rdates << QDate::currentDate() << QDate::currentDate().addDays(3) << QDate::currentDate().addDays(8);
    testTodo.setRecurrenceDates(rdates);
    QCOMPARE(testTodo.recurrenceDates(), rdates);

    QSet<QDate> exdates;
    exdates << QDate::currentDate().addDays(3);
    testTodo.setExceptionDates(exdates);
    QCOMPARE(testTodo.exceptionDates(), exdates);

    QSet<QOrganizerRecurrenceRule> rrules;
    QOrganizerRecurrenceRule rrule;
    rrule.setLimit(2);
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrules << rrule;
    testTodo.setRecurrenceRules(rrules);
    QVERIFY(testTodo.recurrenceRules() == rrules);
    QVERIFY(testTodo.recurrenceRule() == rrule);
    testTodo.setRecurrenceRule(rrule);
    QVERIFY(testTodo.recurrenceRules() == rrules);
    QVERIFY(testTodo.recurrenceRule() == rrule);

    QSet<QOrganizerRecurrenceRule> exrules;
    QOrganizerRecurrenceRule exrule;
    exrule.setLimit(1);
    exrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    exrules << exrule;
    testTodo.setExceptionRules(exrules);
    QVERIFY(testTodo.exceptionRules() == exrules);
    QVERIFY(testTodo.exceptionRule() == exrule);
    testTodo.setExceptionRule(exrule);
    QVERIFY(testTodo.exceptionRules() == exrules);
    QVERIFY(testTodo.exceptionRule() == exrule);
}

void tst_QOrganizerItem::journal()
{
    QOrganizerJournal testJournal;
    QCOMPARE(testJournal.type(), QOrganizerItemType::TypeJournal);

    QDateTime currDateTime = QDateTime::currentDateTime();
    testJournal.setDateTime(currDateTime);
    QCOMPARE(testJournal.dateTime(), currDateTime);
}

void tst_QOrganizerItem::note()
{
    QOrganizerNote testNote;
    QCOMPARE(testNote.type(), QOrganizerItemType::TypeNote);
}

void tst_QOrganizerItem::eventOccurrence()
{
    QOrganizerEventOccurrence testEventOccurrence;
    QCOMPARE(testEventOccurrence.type(), QOrganizerItemType::TypeEventOccurrence);

    testEventOccurrence.setLocation("test address");
    QCOMPARE(testEventOccurrence.location(), QString("test address"));

    testEventOccurrence.setStartDateTime(QDateTime(QDate::currentDate()));
    QCOMPARE(testEventOccurrence.startDateTime(), QDateTime(QDate::currentDate()));
    testEventOccurrence.setEndDateTime(QDateTime(QDate::currentDate().addDays(1)));
    QCOMPARE(testEventOccurrence.endDateTime(), QDateTime(QDate::currentDate().addDays(1)));

    testEventOccurrence.setPriority(QOrganizerItemPriority::VeryHighPriority);
    QCOMPARE(testEventOccurrence.priority(), QOrganizerItemPriority::VeryHighPriority);
    testEventOccurrence.setPriority(QOrganizerItemPriority::VeryLowPriority);
    QCOMPARE(testEventOccurrence.priority(), QOrganizerItemPriority::VeryLowPriority);

    // the parent id and original date time must be tested properly in the manager unit test
    // but we will test the API mutator/accessor functions here.
    QOrganizerItemId id;
    testEventOccurrence.setParentId(id);
    QCOMPARE(testEventOccurrence.parentId(), id);
    QDateTime originalDateTime = QDateTime::currentDateTime();
    testEventOccurrence.setOriginalDate(originalDateTime.date());
    QCOMPARE(testEventOccurrence.originalDate(), originalDateTime.date());
}

void tst_QOrganizerItem::todoOccurrence()
{
    QOrganizerTodoOccurrence testTodoOccurrence;
    QCOMPARE(testTodoOccurrence.type(), QOrganizerItemType::TypeTodoOccurrence);

    QCOMPARE(testTodoOccurrence.status(), QOrganizerTodoProgress::StatusNotStarted);
    testTodoOccurrence.setStatus(QOrganizerTodoProgress::StatusInProgress);
    QCOMPARE(testTodoOccurrence.status(), QOrganizerTodoProgress::StatusInProgress);

    QCOMPARE(testTodoOccurrence.progressPercentage(), 0);
    testTodoOccurrence.setProgressPercentage(50);
    QCOMPARE(testTodoOccurrence.progressPercentage(), 50);
    testTodoOccurrence.setStatus(QOrganizerTodoProgress::StatusComplete);
    QCOMPARE(testTodoOccurrence.progressPercentage(), 50); // XXX TODO: should this update automatically?

    testTodoOccurrence.setStartDateTime(QDateTime(QDate::currentDate()));
    QCOMPARE(testTodoOccurrence.startDateTime(), QDateTime(QDate::currentDate()));
    testTodoOccurrence.setDueDateTime(QDateTime(QDate::currentDate()));
    QCOMPARE(testTodoOccurrence.dueDateTime(), QDateTime(QDate::currentDate()));
    testTodoOccurrence.setFinishedDateTime(QDateTime(QDate::currentDate().addDays(1)));
    QCOMPARE(testTodoOccurrence.finishedDateTime(), QDateTime(QDate::currentDate().addDays(1)));

    testTodoOccurrence.setPriority(QOrganizerItemPriority::VeryHighPriority);
    QCOMPARE(testTodoOccurrence.priority(), QOrganizerItemPriority::VeryHighPriority);
    testTodoOccurrence.setPriority(QOrganizerItemPriority::VeryLowPriority);
    QCOMPARE(testTodoOccurrence.priority(), QOrganizerItemPriority::VeryLowPriority);

    // the parent id and original date time must be tested properly in the manager unit test
    // but we will test the API mutator/accessor functions here.
    QOrganizerItemId id;
    testTodoOccurrence.setParentId(id);
    QCOMPARE(testTodoOccurrence.parentId(), id);
    QDateTime originalDateTime = QDateTime::currentDateTime();
    testTodoOccurrence.setOriginalDate(originalDateTime.date());
    QCOMPARE(testTodoOccurrence.originalDate(), originalDateTime.date());
}



void tst_QOrganizerItem::testDebugStreamOut()
{
    QOrganizerRecurrenceRule rrule;
    // Testing the empty case
    QTest::ignoreMessage(QtDebugMsg, "QOrganizerRecurrenceRule(frequency=0,interval=1,no limit,daysOfWeek=\"\",daysOfMonth=\"\",daysOfYear=\"\",monthsOfYear=\"\",positions=\"\",firstDayOfWeek=1)");
    qDebug() << rrule;

    // Testing a completely filled case
    rrule.setLimit(1);
    rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    rrule.setInterval(10);
    rrule.setLimit(20);
    QSet<Qt::DayOfWeek> days;
    days << Qt::Thursday << Qt::Friday;
    rrule.setDaysOfWeek(days);
    QSet<int> daysM;
    daysM << 1 << 2 << 3;
    rrule.setDaysOfMonth(daysM);
    QSet<int> daysY;
    daysY << 5 << 6 << 7 << 8;
    rrule.setDaysOfYear(daysY);
    QSet<QOrganizerRecurrenceRule::Month> months;
    months << QOrganizerRecurrenceRule::January << QOrganizerRecurrenceRule::February;
    rrule.setMonthsOfYear(months);
    QSet<int> pos;
    pos << -1;
    rrule.setPositions(pos);
    Qt::DayOfWeek firstDay;
    firstDay = Qt::Tuesday;
    rrule.setFirstDayOfWeek(firstDay);
    QTest::ignoreMessage(QtDebugMsg, "QOrganizerRecurrenceRule(frequency=2,interval=10,limitCount=20,daysOfWeek=\"4 5 \",daysOfMonth=\"1 2 3 \",daysOfYear=\"5 6 7 8 \",monthsOfYear=\"1 2 \",positions=\"-1 \",firstDayOfWeek=2)");
    qDebug() << rrule;
}

void tst_QOrganizerItem::itemsCompare()
{
    QOrganizerItemDisplayLabel displayLabel;
    displayLabel.setLabel("test label");
    QOrganizerItemDescription descr;
    descr.setDescription("test description");
    QOrganizerItemComment comment;
    comment.setComment("yet another test comment");
    QOrganizerItemLocation location;
    location.setLabel("another test address label");
    QOrganizerEventAttendee a;
    a.setAttendeeId("123456");
    a.setEmailAddress("people@qt-project.org");
    a.setName("people");

    QOrganizerEventAttendee a1;
    a1.setAttendeeId("777777");
    a1.setName("people1");
    a1.setEmailAddress("people1@qt-project.org");

    QOrganizerEventAttendee a2;
    a2.setAttendeeId("9jjjkkk");
    a2.setName("people2");
    a2.setEmailAddress("people1@qt-project.org");

    QOrganizerEventAttendee a3;
    a2.setAttendeeId("ppppp");
    a2.setName("people3");
    a2.setEmailAddress("people3@qt-project.org");

    QOrganizerEventTime etr;
    etr.setStartDateTime(QDateTime(QDate::currentDate()));
    etr.setEndDateTime(QDateTime(QDate::currentDate().addDays(1)));

    QOrganizerItem item1;
    QOrganizerItem item2;
    item1.saveDetail(&displayLabel);
    item1.saveDetail(&descr);
    item1.saveDetail(&comment);
    item1.saveDetail(&location);
    item1.saveDetail(&etr);

    item2.saveDetail(&etr);
    item2.saveDetail(&location);
    item2.saveDetail(&comment);
    item2.saveDetail(&descr);
    QVERIFY(item1 != item2);
    item2.saveDetail(&displayLabel);
    QVERIFY(item1 == item2);

    // Identical details compare test
    // Same type details but different amount
    item1.addTag("TestTag1");
    item1.addTag("TestTag2");
    item1.saveDetail(&a);

    item2.addTag("TestTag1");
    item2.saveDetail(&a);
    item2.saveDetail(&a1);
    QVERIFY(item1 != item2);

    // Make they same but different orders
    item1.saveDetail(&a1);
    item2.addTag("TestTag2");
    QVERIFY(item1 == item2);

    item1.saveDetail(&a2);
    item2.saveDetail(&a3);
    QVERIFY(item1 != item2);
    item1.saveDetail(&a3);
    item2.saveDetail(&a2);
    QVERIFY(item1 == item2);
}

QTEST_MAIN(tst_QOrganizerItem)
#include "tst_qorganizeritem.moc"
