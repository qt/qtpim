/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <qcontacts.h>
#include <QSet>
#include "../qcontactidmock.h"

//TESTED_COMPONENT=src/contacts

QTCONTACTS_USE_NAMESPACE

class tst_QContact: public QObject
{
    Q_OBJECT

public:
    tst_QContact();
    virtual ~tst_QContact();

private slots:
    void details();
    void preferences();
    void relationships();
    void type();
    void tags();
    void emptiness();
    void idLessThan();
    void idHash();
    void hash();
    void datastream();
    void traits();
    void idTraits();
    void equality();
    void inequality();
};

tst_QContact::tst_QContact()
{
}

tst_QContact::~tst_QContact()
{
}

void tst_QContact::details()
{
    // Check that detail keys are unique, regardless of order of initialisation
    // First, construct the detail first, then the contact
    QContactOrganization org;
    org.setTitle("Example Title");
    QContact keyTest;
    QVERIFY(keyTest.saveDetail(&org));
    QList<QContactDetail> allDetails = keyTest.details();
    QList<int> detailKeys;
    foreach (const QContactDetail& det, allDetails) {
        int currKey = det.key();
        QVERIFY(!detailKeys.contains(currKey));
        detailKeys.append(currKey);
    }
    // Second, construct the detail after the contact has been constructed
    QContactPhoneNumber num;
    num.setNumber("123456");
    QVERIFY(keyTest.saveDetail(&num));
    allDetails = keyTest.details();
    detailKeys.clear();
    foreach (const QContactDetail& det, allDetails) {
        int currKey = det.key();
        QVERIFY(!detailKeys.contains(currKey));
        detailKeys.append(currKey);
    }

    // now test for default construction sanity
    QContact c;

    // Test there are no details (apart from type) by default
    QVERIFY(c.isEmpty() == true);
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());

    // Test retrieving the first detail (the contact type)
    QList<QContactDetail> details = c.details();
    QVERIFY(details.at(0).type() == QContactType::Type);

    // Fetch non existent detail
    QContactDetail detail = c.detail(QContactDetail::TypeAddress);

    QVERIFY(detail.isEmpty());
    QVERIFY(detail.type() == QContactDetail::TypeUndefined);

    // retrieve the first detail using the undefined type accessor.
    detail = c.detail(QContactDetail::TypeUndefined);
    QVERIFY(detail == details.at(0));

    QVERIFY(c.details(QContactDetail::TypeAddress).count() == 0);

    // Add a detail
    QContactPhoneNumber p;
    p.setNumber("12345678");
    QVERIFY(c.saveDetail(&p));
    QVERIFY(c.isEmpty() == false);

    QVERIFY(c.details().count() == 2);

    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 1);
        QVERIFY(c.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(!c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c.detail<QContactPhoneNumber>(), p);

    // Remove detail
    QVERIFY(c.removeDetail(&p));
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.isEmpty() == true);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());

    // Try removing it again
    QVERIFY(!c.removeDetail(&p));

    // Add again, and remove a different way (retrieved copy)
    QVERIFY(c.saveDetail(&p));
    QVERIFY(c.isEmpty() == false);
    QVERIFY(c.details().count() == 2);
    QContactPhoneNumber p2 = c.detail(QContactPhoneNumber::Type);
    QCOMPARE(p, p2);

    QVERIFY(c.removeDetail(&p2));
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.isEmpty() == true);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());

    QCOMPARE(p, p2);

    // Add again again, and remove a different way (base class)
    QVERIFY(c.saveDetail(&p));
    QVERIFY(c.details().count() == 2);
    QContactDetail p3 = c.detail(QContactPhoneNumber::Type);
    QVERIFY(p == p3);

    QVERIFY(c.removeDetail(&p3));
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());

    QVERIFY(p == p3);

    // now we want to add multiple details of the same type, and test that retrieval works correctly.
    p2 = QContactPhoneNumber();
    p2.setNumber("22222");
    c.saveDetail(&p);
    c.saveDetail(&p2);
    QVERIFY(c.details().count() == 3);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 2);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 2);
    QVERIFY(!c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c.detail<QContactPhoneNumber>(), p);
    QVERIFY(c.removeDetail(&p2));

    // now try removing a detail for which we've set a preference
    QContactEmailAddress pref;
    pref.setEmailAddress("test@test");
    c.saveDetail(&pref);
    c.setPreferredDetail("SendEmail", pref);
    QVERIFY(c.isPreferredDetail(QString(), pref));
    QVERIFY(c.removeDetail(&pref));
    QVERIFY(!c.isPreferredDetail(QString(), pref));

    // Now try adding a detail to multiple contacts

    QContact c2;
    QVERIFY(c2.isEmpty() == true);
    QVERIFY(c.saveDetail(&p));
    QVERIFY(c2.saveDetail(&p));
    QVERIFY(c2.isEmpty() == false);

    QVERIFY(c.details().count() == 2);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 1);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(!c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c.detail<QContactPhoneNumber>(), p);

    QVERIFY(c2.details().count() == 2);
    QVERIFY(c2.details(QContactPhoneNumber::Type).count() == 1);
    QVERIFY(c2.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(!c2.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c2.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c2.detail<QContactPhoneNumber>(), p);

    // Now try removing it from one
    QVERIFY(c.removeDetail(&p));

    // Make sure it's gone from the first contact
    QVERIFY(c.isEmpty() == true);
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());

    // but not the second
    QVERIFY(c2.isEmpty() == false);
    QVERIFY(c2.details().count() == 2);
    QVERIFY(c2.details(QContactPhoneNumber::Type).count() == 1);
    QVERIFY(c2.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(!c2.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c2.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c2.detail<QContactPhoneNumber>(), p);

    // Now remove it from the second as well
    QVERIFY(c2.removeDetail(&p));

    // Make sure it's gone from both
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());

    QVERIFY(c2.details().count() == 1);
    QVERIFY(c2.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c2.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c2.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c2.detail<QContactPhoneNumber>().isEmpty());

    // add a, add b, remove a, add a, remove b, remove a
    QVERIFY(c.saveDetail(&p));
    QVERIFY(c2.saveDetail(&p));
    QVERIFY(c.removeDetail(&p));
    QVERIFY(c.saveDetail(&p));
    QVERIFY(c2.removeDetail(&p));
    QVERIFY(c.removeDetail(&p));

    // Now add a detail with the same values twice
    QContactPhoneNumber one;
    QContactPhoneNumber two;

    one.setNumber("12345");
    two.setNumber("12345");

    // add it once
    QVERIFY(c.saveDetail(&one));
    QVERIFY(c.details().count() == 2);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 1);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(!c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c.detail<QContactPhoneNumber>(), one);

    // add it twice
    QVERIFY(c.saveDetail(&two));
    QVERIFY(c.details().count() == 3);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 2);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 2);
    QVERIFY(!c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c.detail<QContactPhoneNumber>(), one);
    QCOMPARE(c.details<QContactPhoneNumber>()[0], one);
    QCOMPARE(c.details<QContactPhoneNumber>()[1], two);

    // Remove it once
    QVERIFY(c.removeDetail(&one));
    QVERIFY(c.details().count() == 2);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 1);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(!c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(!c.detail<QContactPhoneNumber>().isEmpty());
    QCOMPARE(c.detail<QContactPhoneNumber>(), two);

    // Remove it twice
    QVERIFY(c.removeDetail(&two));
    QVERIFY(c.details().count() == 1);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 0);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 0);
    QVERIFY(c.detail(QContactPhoneNumber::Type).isEmpty());
    QVERIFY(c.detail<QContactPhoneNumber>().isEmpty());

    // Null pointer tests
    QVERIFY(c.saveDetail(0) == false);
    QVERIFY(c.removeDetail(0) == false);

    // Reference tests...
    QContactDetail& ref = one;
    QVERIFY(c.saveDetail(&one));
    QVERIFY(ref == one);
    one.setNumber("56678");
    QVERIFY(c.saveDetail(&one));
    QVERIFY(ref == one);

    // Retrieve the detail again and modify it
    QContactPhoneNumber three = c.detail<QContactPhoneNumber>();
    QVERIFY(ref == three);
    QVERIFY(one == three);
    three.setNumber("542343");
    QVERIFY(c.saveDetail(&three));

    // Now see if we got any updates to ref/one
    QVERIFY(ref == one);
    QVERIFY(ref != three);

    // test saving of a detail with an empty field.
    QContactPhoneNumber four;
    four.setNumber("");
    c.saveDetail(&four);
    QVERIFY(c.details(QContactPhoneNumber::Type).count() == 2);
    QVERIFY(!four.values().isEmpty()); // an empty qstring is not invalid; make sure it exists in the detail.

    // ensure that clearing a contact's details works correctly
    QContactName nameDetail;
    nameDetail.setFirstName("test");
    c.saveDetail(&nameDetail);
    QCOMPARE(c.detail(QContactName::Type).value(QContactName::FieldFirstName).toString(), QString("test"));
    QVERIFY(c.details().size() > 0);
    QVERIFY(!c.isEmpty());
    QContactId oldId = c.id();
    c.clearDetails();
    QVERIFY(c.details().size() == 1); // contact type.
    QCOMPARE(c.detail(QContactName::Type).value(QContactName::FieldFirstName).toString(), QString());
    QVERIFY(c.isEmpty());
    QCOMPARE(c.id(), oldId); // id shouldn't change.
}

void tst_QContact::preferences()
{
    QContact c;

    // test first set
    QContactDetail det(QContactDetail::TypeExtendedDetail);
    c.saveDetail(&det);
    QCOMPARE(c.isPreferredDetail("testAction", det), false);

    QCOMPARE(c.setPreferredDetail("testAction", det), true);

    QCOMPARE(c.isPreferredDetail("testAction", det), true);

    QCOMPARE(c.isPreferredDetail(QString(), det), true);

    QCOMPARE(c.preferredDetail("testAction"), det);

    // test replacement
    QContactDetail det2(QContactDetail::TypeExtendedDetail);
    c.saveDetail(&det2);
    QCOMPARE(c.isPreferredDetail("testAction", det2), false);

    QCOMPARE(c.setPreferredDetail("testAction", det2), true);

    QCOMPARE(c.isPreferredDetail("testAction", det2), true);

    QCOMPARE(c.isPreferredDetail("testAction", det), false);

    QCOMPARE(c.preferredDetail("testAction"), det2);

    // test for detail that is not part of the contact
    QContactDetail det3(QContactDetail::TypeEmailAddress);
    QCOMPARE(c.setPreferredDetail("testAction", det3), false);

    QCOMPARE(c.preferredDetail("testAction"), det2); // shouldn't have changed.

    // test invalid set
    QCOMPARE(c.setPreferredDetail(QString(), det3), false);

    QCOMPARE(c.setPreferredDetail(QString(), QContactDetail()), false);

    QCOMPARE(c.setPreferredDetail("testAction", QContactDetail()), false);

    QCOMPARE(c.preferredDetail("testAction"), det2); // shouldn't have changed.

    // test invalid query
    QContactDetail det4;
    c.saveDetail(&det4);
    QCOMPARE(c.isPreferredDetail(QString(), QContactDetail()), false);

    QCOMPARE(c.isPreferredDetail(QString(), det4), false); // valid detail, but no pref set.

    QCOMPARE(c.isPreferredDetail("testAction", QContactDetail()), false);

    // test retrieving preferred details
    QContactDetail pd = c.preferredDetail(QString());
    QVERIFY(pd.isEmpty());
    pd = c.preferredDetail("testAction");
    QVERIFY(pd == det2); // shouldn't have changed.

    // test for preference for action that hasn't been added
    QVERIFY(c.preferredDetail("NonexistentAction").isEmpty());

    // Remove a non preferred detail
    QContactDetail det2copy(QContactDetail::TypeExtendedDetail);
    QVERIFY(c.saveDetail(&det2copy));

    QVERIFY(c.isPreferredDetail("testAction", det2) == true);
    QVERIFY(c.isPreferredDetail("testAction", det2copy) == false);
    QVERIFY(c.removeDetail(&det2copy));
    QVERIFY(c.isPreferredDetail("testAction", det2) == true);
    QVERIFY(c.isPreferredDetail("testAction", det2copy) == false);

    // Add it again
    QVERIFY(c.saveDetail(&det2copy));
    QVERIFY(c.isPreferredDetail("testAction", det2) == true);
    QVERIFY(c.isPreferredDetail("testAction", det2copy) == false);

    // Remove the preferred detail (the copy should not become preferred)
    QVERIFY(c.removeDetail(&det2));
    QVERIFY(c.isPreferredDetail("testAction", det2) == false);
    QVERIFY(c.isPreferredDetail("testAction", det2copy) == false);
}

void tst_QContact::relationships()
{
    QContact c;

    // boring test, because the default contact has no relationships
    // we test this more convincingly in the QContactManager tests.
    QList<QContact> related = c.relatedContacts();
    QVERIFY(related.isEmpty());

    related = c.relatedContacts(QContactRelationship::HasMember());
    QVERIFY(related.isEmpty());

    related = c.relatedContacts(QContactRelationship::HasMember(), QContactRelationship::First);
    QVERIFY(related.isEmpty());

    QList<QContactRelationship> relationshipList = c.relationships();
    QVERIFY(relationshipList.isEmpty());

    relationshipList = c.relationships(QContactRelationship::HasMember());
    QVERIFY(relationshipList.isEmpty());
}

void tst_QContact::type()
{
    QContact c;
    QVERIFY(c.isEmpty() == true);

    // ensure that the default type is the QContactType::TypeContact type
    QVERIFY(c.type() == QContactType::TypeContact);

    // now set it to be a group via the type mutator, and test that it works
    c.setType(QContactType::TypeGroup);
    QCOMPARE(c.type(), QContactType::TypeGroup);

    // set it back to a contact, via the string mutator
    c.setType(QContactType::TypeContact);
    QCOMPARE(c.type(), QContactType::TypeContact);
    QVERIFY(c.isEmpty() == true); // type doesn't affect emptiness
}

void tst_QContact::tags()
{
    QContact c;
    QVERIFY(c.tags().isEmpty());

    c.addTag("tag 1");
    QStringList tags;
    tags.append("tag 1");
    QCOMPARE(c.tags(), tags);
    QList<QContactTag> tagDetails = c.details<QContactTag>();
    QCOMPARE(tagDetails.size(), 1);
    QCOMPARE(tagDetails.first().tag(), QStringLiteral("tag 1"));

    c.clearTags();
    QVERIFY(c.tags().isEmpty());
    QVERIFY(c.details<QContactTag>().isEmpty());

    tags.append("tag 2"); // tags is now "tag 1", "tag 2"
    c.setTags(tags);
    QCOMPARE(c.tags(), tags);
    tagDetails = c.details<QContactTag>();
    QCOMPARE(tagDetails.size(), 2);
    QCOMPARE(tagDetails.at(0).tag(), QStringLiteral("tag 1"));
    QCOMPARE(tagDetails.at(1).tag(), QStringLiteral("tag 2"));
}

void tst_QContact::emptiness()
{
    QContact c;
    QVERIFY(c.isEmpty() == true);
    c.setType(QContactType::TypeContact);
    QVERIFY(c.type() == QContactType::TypeContact);
    QVERIFY(c.isEmpty() == true); // type doesn't affect emptiness
}

void tst_QContact::idLessThan()
{
    QContactId id1 = QContactIdMock::createId("a", 1);
    QContactId id2 = QContactIdMock::createId("a", 1);
    QVERIFY(!(id1 < id2));
    QVERIFY(!(id2 < id1));
    QContactId id3 = QContactIdMock::createId("a", 2);
    QContactId id4 = QContactIdMock::createId("b", 1);
    QContactId id5 = QContactIdMock::createId("", 2);
    QVERIFY(id1 < id3);
    QVERIFY(!(id3 < id1));
    QVERIFY(id1 < id4);
    QVERIFY(!(id4 < id1));
    QVERIFY(id3 < id4);
    QVERIFY(!(id4 < id3));
    QVERIFY(id5 < id1);
    QVERIFY(!(id1 < id5));
}

void tst_QContact::idHash()
{
    QContactId id1 = QContactIdMock::createId("a", 1);
    QContactId id2 = QContactIdMock::createId("a", 1);
    QContactId id3 = QContactIdMock::createId("b", 1);
    QVERIFY(qHash(id1) == qHash(id2));
    QVERIFY(qHash(id1) != qHash(id3));
    QSet<QContactId> set;
    set.insert(id1);
    set.insert(id2);
    set.insert(id3);
    QCOMPARE(set.size(), 2);
}

void tst_QContact::hash()
{
    QContactId id = QContactIdMock::createId("a", 1);
    QContact contact1;
    contact1.setId(id);
    QContactDetail detail1(QContactDetail::TypeExtendedDetail);
    detail1.setValue(QContactDetail::FieldContext, "value");
    contact1.saveDetail(&detail1);
    QContact contact2;
    contact2.setId(id);
    contact2.saveDetail(&detail1);
    QContact contact3;
    contact3.setId(id);
    QContactDetail detail3(QContactDetail::TypeExtendedDetail);
    detail3.setValue(QContactDetail::FieldContext, "another value");
    contact3.saveDetail(&detail3);
    QContact contact4; // no details
    contact4.setId(id);
    QContact contact5; // preferred details and relationships shouldn't affect the hash
    contact5.setId(id);
    contact5.saveDetail(&detail1);
    contact5.setPreferredDetail("action", detail1);
    QContactRelationship rel;
    QContactManagerEngine::setContactRelationships(&contact5, QList<QContactRelationship>() << rel);
    QVERIFY(qHash(contact1) == qHash(contact2));
    QVERIFY(qHash(contact1) != qHash(contact3));
    QVERIFY(qHash(contact1) != qHash(contact4));
    QVERIFY(qHash(contact1) == qHash(contact5));
}

void tst_QContact::datastream()
{
    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    QContact contactIn;
    QContactId id = QContactIdMock::createId("manager", 1234);
    contactIn.setId(id);
    QContactPhoneNumber phone;
    phone.setNumber("5678");
    contactIn.saveDetail(&phone);
    stream1 << contactIn;

    QVERIFY(buffer.size() > 0);

    /* TODO: fix me?
    QDataStream stream2(buffer);
    QContact contactOut;
    stream2 >> contactOut;
    QCOMPARE(contactOut, contactIn);*/
}

void tst_QContact::traits()
{
    QVERIFY(sizeof(QContact) == sizeof(void *));
    QVERIFY(QTypeInfo<QContact>::isComplex);
    QVERIFY(!QTypeInfo<QContact>::isStatic);
    QVERIFY(!QTypeInfo<QContact>::isLarge);
    QVERIFY(!QTypeInfo<QContact>::isPointer);
    QVERIFY(!QTypeInfo<QContact>::isDummy);
}

void tst_QContact::idTraits()
{
    QVERIFY(sizeof(QContactId) == sizeof(void *));
    QVERIFY(QTypeInfo<QContactId>::isComplex);
    QVERIFY(!QTypeInfo<QContactId>::isStatic);
    QVERIFY(!QTypeInfo<QContactId>::isLarge);
    QVERIFY(!QTypeInfo<QContactId>::isPointer);
    QVERIFY(!QTypeInfo<QContactId>::isDummy);
}

void tst_QContact::equality()
{
    QContactName name;
    name.setFirstName("John");
    name.setLastName("Doe");
    QContactPhoneNumber number;
    number.setNumber("7654321");
    QContactEmailAddress email;
    email.setEmailAddress("john.doe@nokia.com");
    QContactExtendedDetail xdetail;
    xdetail.setName("shoesize");
    xdetail.setData("45");
    // Setup two identical contacts
    QContact one, two;
    one.saveDetail(&name);
    one.saveDetail(&number);
    one.saveDetail(&email);
    one.saveDetail(&xdetail);
    two.saveDetail(&xdetail);
    two.saveDetail(&email);
    two.saveDetail(&number);
    two.saveDetail(&name);

    QVERIFY(one == two);
}

void tst_QContact::inequality()
{
    QContactId id = QContactIdMock::createId("a", 123);
    QContactName name;
    name.setFirstName("John");
    name.setLastName("Doe");
    QContactPhoneNumber number;
    number.setNumber("7654321");
    QContactEmailAddress email;
    email.setEmailAddress("john.doe@nokia.com");
    QContactExtendedDetail xdetail;
    xdetail.setName("shoesize");
    xdetail.setData("45");
    // Setup two contacts
    QContact one, two;
    one.setId(id);
    QVERIFY(one != two);
    two.setId(id);
    QVERIFY(one == two);
    // insert different amount of details
    one.saveDetail(&name);
    one.saveDetail(&number);
    two.saveDetail(&number);
    QVERIFY(one != two);
    two.clearDetails();
    // same amount of details with different types
    two.saveDetail(&number);
    two.saveDetail(&email);
    QVERIFY(one != two);
    two.clearDetails();
    // same amount of details with different value
    name.setFirstName("Jim");
    two.saveDetail(&name);
    two.saveDetail(&number);
    QVERIFY(one != two);
    two.clearDetails();
    name.setFirstName("John");
    // different types of details with same value
    email.setEmailAddress("7654321");
    two.saveDetail(&name);
    two.saveDetail(&email);
    QVERIFY(one != two);
}

QTEST_MAIN(tst_QContact)
#include "tst_qcontact.moc"
