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

#include <QtOrganizer/qorganizer.h>

//TESTED_COMPONENT=src/organizer

QTORGANIZER_USE_NAMESPACE
class tst_QOrganizerItemDetails : public QObject
{
Q_OBJECT

public:
    tst_QOrganizerItemDetails();
    virtual ~tst_QOrganizerItemDetails();

public slots:
    void init();
    void cleanup();

private slots:
    void compare();

    // leaf class testing
    void description();
    void displayLabel();
    void guid();
    void location();
    void parent();
    void priority();
    void recurrence();
    void tag();
    void timestamp();
    void type();

    void todoProgress();

    // time range
    void eventTime();
    void journalTime();
    void todoTime();

    // reminders
    void reminder();
    void audibleReminder();
    void emailReminder();
    void visualReminder();

    void attendee();
    void rsvp();
    void classification();
    void version();
};

tst_QOrganizerItemDetails::tst_QOrganizerItemDetails()
{
}

tst_QOrganizerItemDetails::~tst_QOrganizerItemDetails()
{
}

void tst_QOrganizerItemDetails::init()
{
}

void tst_QOrganizerItemDetails::cleanup()
{
}

void tst_QOrganizerItemDetails::compare()
{
    QOrganizerItemRecurrence r, r2;
    QVERIFY(r == r2);
    QVERIFY(QOrganizerItemDetail() != r);

    QOrganizerRecurrenceRule testRule;
    testRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    testRule.setLimit(4);
    QSet<QOrganizerRecurrenceRule> rrules;
    rrules << testRule;

    QOrganizerRecurrenceRule exrule;
    exrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    testRule.setLimit(12);
    QSet<QOrganizerRecurrenceRule> exrules;
    exrules << exrule;

    QSet<QDate> rdates, exdates;
    rdates << QDate(2010, 10, 13);
    exdates << QDate(2010, 11, 17) << QDate(2010, 11, 20);

    r.setRecurrenceRules(rrules);
    QVERIFY(r != r2);
    r2.setRecurrenceRules(rrules);
    QVERIFY(r == r2);
    r.setExceptionRules(exrules);
    QVERIFY(r != r2);
    r2.setExceptionRules(exrules);
    QVERIFY(r == r2);
    r.setRecurrenceDates(rdates);
    QVERIFY(r != r2);
    r2.setRecurrenceDates(rdates);
    QVERIFY(r == r2);
    r.setExceptionDates(exdates);
    QVERIFY(r != r2);
    r2.setExceptionDates(exdates);
    QVERIFY(r == r2);

    QOrganizerEvent e, e2;
    QCOMPARE(e.details(QOrganizerItemDetail::TypeRecurrence).size(), 0);
    QVERIFY(e == e2);

    // now save
    QVERIFY(e.saveDetail(&r));
    QCOMPARE(e.details(QOrganizerItemDetail::TypeRecurrence).size(), 1);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) == r);
    QVERIFY(e != e2);

    QVERIFY(e2.saveDetail(&r2));
    QCOMPARE(e2.details(QOrganizerItemDetail::TypeRecurrence).size(), 1);
    QVERIFY(e2.detail(QOrganizerItemDetail::TypeRecurrence) == r2);
    QVERIFY(e == e2);

    // update
    exdates << QDate(2010, 10, 17);
    r.setExceptionDates(exdates);
    QVERIFY(r != r2);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) != r);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) == r2);
    QVERIFY(e.saveDetail(&r));
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) == r);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) != r2);
    QVERIFY(e != e2);

    // remove.
    QVERIFY(e.removeDetail(&r));
    QVERIFY(e != e2);
    QVERIFY(e2.removeDetail(&r2));
    QVERIFY(e == e2);

    QVERIFY(e.saveDetail(&r));
    e2 = QOrganizerEvent();
    QVERIFY(e != e2);
    e2 = e;
    QVERIFY(e == e2);
}

void tst_QOrganizerItemDetails::description()
{
    QOrganizerItemDescription d1;
    QOrganizerItem oi;
    QVERIFY(d1.description().isEmpty());
    QVERIFY(d1.value(QOrganizerItemDescription::FieldDescription).isNull());
    d1.setValue(QOrganizerItemDescription::FieldDescription, "Test");
    QVERIFY(d1.value(QOrganizerItemDescription::FieldDescription) == QString("Test"));
    QVERIFY(d1.description() == QString("Test"));

    QOrganizerItemDescription d2;
    d2.setValue(QOrganizerItemDescription::FieldDescription, "Test 2");

    // test property add -- description is unique.
    QVERIFY(oi.saveDetail(&d2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDescription).count(), 1); // count should be 1
    QVERIFY(oi.saveDetail(&d1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDescription).count(), 1); // count should be 1

    // test property update
    d1.setDescription("Test 3");
    QVERIFY(oi.saveDetail(&d1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDescription).count(), 1); // count should be 1
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeDescription).value(QOrganizerItemDescription::FieldDescription) == QString("Test 3"));

    // test property remove
    QVERIFY(oi.removeDetail(&d1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDescription).count(), 0);
}

void tst_QOrganizerItemDetails::displayLabel()
{
    QOrganizerItemDisplayLabel d1;
    QOrganizerItem oi;

    QVERIFY(d1.label().isEmpty());
    QVERIFY(d1.value(QOrganizerItemDisplayLabel::FieldLabel).isNull());
    d1.setValue(QOrganizerItemDisplayLabel::FieldLabel, "Test");
    QVERIFY(d1.value(QOrganizerItemDisplayLabel::FieldLabel) == QString("Test"));
    QVERIFY(d1.label() == QString("Test"));

    QOrganizerItemDisplayLabel d2;
    d2.setValue(QOrganizerItemDisplayLabel::FieldLabel, "Test 2");

    // test property add - should update.
    QVERIFY(oi.saveDetail(&d2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDisplayLabel).count(), 1);
    QVERIFY(oi.saveDetail(&d1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDisplayLabel).count(), 1);

    // test property update - should update
    d1.setLabel("Test 3");
    QVERIFY(oi.saveDetail(&d1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDisplayLabel).count(), 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeDisplayLabel).value(QOrganizerItemDisplayLabel::FieldLabel) == QString("Test 3"));

    // test property remove
    QVERIFY(oi.removeDetail(&d1)); // can remove display label.  it's unique but not required.
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeDisplayLabel).count(), 0);
}


void tst_QOrganizerItemDetails::guid()
{
    QOrganizerItem oi;
    QOrganizerItemGuid g1, g2;

    // test property set
    g1.setGuid("1234");
    QCOMPARE(g1.guid(), QString("1234"));
    QCOMPARE(g1.value(QOrganizerItemGuid::FieldGuid).toString(), QString("1234"));

    // test property add
    QVERIFY(oi.saveDetail(&g1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).count(), 1);
    QCOMPARE(QOrganizerItemGuid(oi.details(QOrganizerItemDetail::TypeGuid).value(0)).guid(), g1.guid());

    // test property update
    g1.setValue(101,"label1");
    g1.setGuid("12345");
    QVERIFY(oi.saveDetail(&g1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).value(0).value(101).toString(), QString("label1"));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).value(0).value(QOrganizerItemGuid::FieldGuid).toString(), QString("12345"));

    // test property remove
    QVERIFY(oi.removeDetail(&g1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).count(), 0);
    QVERIFY(oi.saveDetail(&g2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).count(), 1);
    QVERIFY(oi.removeDetail(&g2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).count(), 0);
    QVERIFY(oi.removeDetail(&g2) == false);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeGuid).count(), 0);
}

void tst_QOrganizerItemDetails::location()
{
    QOrganizerItemLocation l1;
    QOrganizerItem oi;
    QVERIFY(l1.isEmpty());

    l1.setLatitude(89.999);
    QVERIFY(!l1.isEmpty());
    QVERIFY(l1.latitude() == 89.999);
    l1.setValue(QOrganizerItemLocation::FieldLatitude, -89.12345);
    QVERIFY(l1.value(QOrganizerItemLocation::FieldLatitude) == -89.12345);
    l1.setLatitude(-90.12345);
    l1.setLatitude(91);
    QVERIFY(l1.latitude() == -89.12345);

    l1.setLongitude(179);
    QVERIFY(l1.longitude() == 179);
    l1.setValue(QOrganizerItemLocation::FieldLongitude, -179.54321);
    QVERIFY(l1.value(QOrganizerItemLocation::FieldLongitude) == -179.54321);
    l1.setLongitude(180.1);
    l1.setLongitude(-180.1);
    QVERIFY(l1.longitude() == -179.54321);

    l1.setLabel("test");
    QVERIFY(l1.label() == QString("test"));

    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).size() == 0);
    QVERIFY(oi.saveDetail(&l1));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation) == l1);

    l1.setLabel("test 2");
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation) != l1);
    QVERIFY(oi.saveDetail(&l1)); // update.
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).size() == 1); // should update, not add another.
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeLocation) == l1);

    oi.removeDetail(&l1);
    QVERIFY(oi.details(QOrganizerItemDetail::TypeLocation).size() == 0);
}

void tst_QOrganizerItemDetails::parent()
{
    QOrganizerItemParent p;
    QOrganizerItem oi;

    // hrm, how to test parent local id?

    QVERIFY(p.isEmpty());
    p.setOriginalDate(QDate(2010, 10, 13));
    QVERIFY(p.originalDate() == QDate(2010, 10, 13));
    QVERIFY(!p.isEmpty());

    QVERIFY(oi.saveDetail(&p));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeParent).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeParent) == p);

    p.setOriginalDate(QDate(2010, 10, 14));
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeParent) != p);
    QVERIFY(oi.saveDetail(&p)); // update
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeParent) == p);

    QVERIFY(oi.removeDetail(&p));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeParent).size() == 0);
}

void tst_QOrganizerItemDetails::priority()
{
    QOrganizerItemPriority p;
    QOrganizerItem oi;

    QVERIFY(p.isEmpty());
    p.setValue(QOrganizerItemPriority::FieldPriority, QOrganizerItemPriority::VeryHighPriority);
    QVERIFY(p.priority() == QOrganizerItemPriority::VeryHighPriority);
    QVERIFY(oi.saveDetail(&p));
    QVERIFY(oi.details(QOrganizerItemDetail::TypePriority).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypePriority) == p);

    p.setPriority(QOrganizerItemPriority::VeryLowPriority);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypePriority) != p);
    QVERIFY(oi.saveDetail(&p));
    QVERIFY(oi.details(QOrganizerItemDetail::TypePriority).size() == 1); // update shouldn't add another detail.
    QVERIFY(oi.detail(QOrganizerItemDetail::TypePriority) == p);

    QVERIFY(oi.removeDetail(&p));
    QVERIFY(oi.details(QOrganizerItemDetail::TypePriority).size() == 0);
}

void tst_QOrganizerItemDetails::recurrence()
{
    QOrganizerItemRecurrence r;
    QOrganizerEvent e;

    QOrganizerRecurrenceRule testRule;
    testRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    testRule.setLimit(4);
    QSet<QOrganizerRecurrenceRule> rrules;
    rrules << testRule;

    QOrganizerRecurrenceRule exrule;
    exrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    testRule.setLimit(12);
    QSet<QOrganizerRecurrenceRule> exrules;
    exrules << exrule;

    QSet<QDate> rdates, exdates;
    rdates << QDate(2010, 10, 13);
    exdates << QDate(2010, 11, 17) << QDate(2010, 11, 20);

    QVERIFY(e.details(QOrganizerItemDetail::TypeRecurrence).size() == 0);
    r.setRecurrenceRules(rrules);
    QVERIFY(r.recurrenceRules() == rrules);
    QVERIFY(r.recurrenceDates().isEmpty());
    QVERIFY(r.exceptionRules().isEmpty());

    r.setExceptionRules(exrules);
    QVERIFY(r.recurrenceRules() == rrules);
    QVERIFY(r.exceptionRules() == exrules);
    QVERIFY(r.exceptionDates().isEmpty());

    r.setRecurrenceDates(rdates);
    QVERIFY(r.recurrenceDates() == rdates);
    QVERIFY(r.exceptionDates().isEmpty());
    QVERIFY(r.recurrenceRules() == rrules);
    QVERIFY(r.exceptionRules() == exrules);

    r.setExceptionDates(exdates);
    QVERIFY(r.recurrenceDates() == rdates);
    QVERIFY(r.exceptionDates() == exdates);
    QVERIFY(r.recurrenceRules() == rrules);
    QVERIFY(r.exceptionRules() == exrules);

    // now save.
    QVERIFY(e.saveDetail(&r));
    QVERIFY(e.details(QOrganizerItemDetail::TypeRecurrence).size() == 1);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) == r);

    // update
    exdates << QDate(2010, 10, 17);
    r.setExceptionDates(exdates);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) != r);
    QVERIFY(e.saveDetail(&r));
    QVERIFY(e.detail(QOrganizerItemDetail::TypeRecurrence) == r);
    QVERIFY(e.details(QOrganizerItemDetail::TypeRecurrence).size() == 1); // should update, not add another.

    // remove.
    QVERIFY(e.removeDetail(&r));
    QVERIFY(e.details(QOrganizerItemDetail::TypeRecurrence).size() == 0);
}

void tst_QOrganizerItemDetails::tag()
{
    QOrganizerItem oi;
    QOrganizerItemTag t1, t2;

    // test property set
    t1.setTag("red");
    QCOMPARE(t1.tag(), QString("red"));
    QCOMPARE(t1.value(QOrganizerItemTag::FieldTag).toString(), QString("red"));

    // test property add
    QVERIFY(oi.saveDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).count(), 1);
    QCOMPARE(QOrganizerItemTag(oi.details(QOrganizerItemDetail::TypeTag).value(0)).tag(), t1.tag());
    QVERIFY(oi.saveDetail(&t2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).count(), 2);

    // test property update
    t1.setValue(101,"label1");
    t1.setTag("green");
    QVERIFY(oi.saveDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).value(0).value(101).toString(), QString("label1"));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).value(0).value(QOrganizerItemTag::FieldTag).toString(), QString("green"));

    // test property remove
    QVERIFY(oi.removeDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).count(), 1);
    QVERIFY(oi.removeDetail(&t2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).count(), 0);
    QVERIFY(oi.removeDetail(&t2) == false);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTag).count(), 0);
}


void tst_QOrganizerItemDetails::timestamp()
{
    QOrganizerItem oi;
    QOrganizerItemTimestamp t1, t2;
    QDateTime modified = QDateTime::currentDateTime();
    QDateTime created = modified.addSecs(-43);

    // test property set
    t1.setCreated(created);
    QCOMPARE(t1.created(), created);
    QCOMPARE(t1.value(QOrganizerItemTimestamp::FieldCreated).toDateTime(), created);

    // test property add
    QVERIFY(oi.saveDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).count(), 1);
    QCOMPARE(QOrganizerItemTimestamp(oi.details(QOrganizerItemDetail::TypeTimestamp).value(0)).created(), t1.created());

    // test property update
    t1.setValue(101,"label1");
    t1.setLastModified(modified);
    QVERIFY(oi.saveDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).value(0).value(101).toString(), QString("label1"));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).value(0).value(QOrganizerItemTimestamp::FieldCreated).toDateTime(), created);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).value(0).value(QOrganizerItemTimestamp::FieldLastModified).toDateTime(), modified);

    // test property remove
    QVERIFY(oi.removeDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).count(), 0);
    t2.setCreated(created.addSecs(15));
    QVERIFY(oi.saveDetail(&t2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).count(), 1);
    QVERIFY(oi.removeDetail(&t2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).count(), 0);
    QVERIFY(oi.removeDetail(&t2) == false);
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeTimestamp).count(), 0);
}

void tst_QOrganizerItemDetails::todoProgress()
{
    QOrganizerTodoProgress p;
    QOrganizerTodo t;

    QVERIFY(p.status() == QOrganizerTodoProgress::StatusNotStarted);
    QVERIFY(p.isEmpty());
    p.setStatus(QOrganizerTodoProgress::StatusInProgress);
    QVERIFY(!p.isEmpty());
    QVERIFY(p.status() == QOrganizerTodoProgress::StatusInProgress);

    p.setPercentageComplete(50);
    QVERIFY(p.percentageComplete() == 50);
    p.setPercentageComplete(200); // this should fail
    QVERIFY(p.percentageComplete() == 50); // value should remain unchanged.

    p.setFinishedDateTime(QDateTime(QDate(2010, 10, 13)));
    QVERIFY(p.finishedDateTime() == QDateTime(QDate(2010, 10, 13)));
    QVERIFY(p.status() != QOrganizerTodoProgress::StatusComplete); // XXX TODO: should this be automatic?
    p.setPercentageComplete(100);
    QVERIFY(p.percentageComplete() == 100);
    QVERIFY(p.status() != QOrganizerTodoProgress::StatusComplete); // XXX TODO: should this be automatic?

    // add
    QVERIFY(t.saveDetail(&p));
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoProgress).size() == 1);
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoProgress) == p);

    // update
    p.setStatus(QOrganizerTodoProgress::StatusComplete);
    QVERIFY(p.status() == QOrganizerTodoProgress::StatusComplete);
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoProgress) != p);
    QVERIFY(t.saveDetail(&p));
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoProgress).size() == 1); // update, not add another
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoProgress) == p);

    // remove
    QVERIFY(t.removeDetail(&p));
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoProgress).size() == 0);
}

void tst_QOrganizerItemDetails::type()
{
    QOrganizerItem oi;
    QOrganizerItemType t1, t2;

    QOrganizerEvent e;
    QOrganizerNote n;
    QOrganizerTodo t;
    QOrganizerJournal j;
    QOrganizerEventOccurrence eo;
    QOrganizerTodoOccurrence to;
    QVERIFY(e.type() == QOrganizerItemType::TypeEvent);
    QVERIFY(n.type() == QOrganizerItemType::TypeNote);
    QVERIFY(t.type() == QOrganizerItemType::TypeTodo);
    QVERIFY(j.type() == QOrganizerItemType::TypeJournal);
    QVERIFY(eo.type() == QOrganizerItemType::TypeEventOccurrence);
    QVERIFY(to.type() == QOrganizerItemType::TypeTodoOccurrence);

    // test property set
    t1.setType(QOrganizerItemType::TypeEvent);
    QCOMPARE(t1.type(), QOrganizerItemType::TypeEvent);
    QCOMPARE(static_cast<QOrganizerItemType::ItemType>(t1.value(QOrganizerItemType::FieldType).toInt()), QOrganizerItemType::TypeEvent);

    // test property add
    QVERIFY(oi.saveDetail(&t1));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeItemType).count(), 1);
    QCOMPARE(QOrganizerItemType(oi.details(QOrganizerItemDetail::TypeItemType).value(0)).type(), t1.type());

    // test property update
    t1.setType(QOrganizerItemType::TypeTodo);
    QVERIFY(oi.saveDetail(&t1));
    QCOMPARE(static_cast<QOrganizerItemType::ItemType>(oi.details(QOrganizerItemDetail::TypeItemType).value(0).value(QOrganizerItemType::FieldType).toInt()), QOrganizerItemType::TypeTodo);

    // test property remove
    QVERIFY(!oi.removeDetail(&t1)); // cannot remove type
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeItemType).count(), 1);
    t2.setType(QOrganizerItemType::TypeNote);
    QVERIFY(oi.saveDetail(&t2)); // overwrites t1
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeItemType).count(), 1);
    QVERIFY(!oi.removeDetail(&t2)); // cannot remove type - "succeeds" but count remains unchanged
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeItemType).count(), 1);
    QVERIFY(!oi.removeDetail(&t2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeItemType).count(), 1);
}


void tst_QOrganizerItemDetails::eventTime()
{
    QOrganizerEventTime r;
    QOrganizerEvent e;

    QVERIFY(r.isEmpty());
    QVERIFY(!r.isAllDay());

    r.setStartDateTime(QDateTime(QDate(2010, 10, 13)));
    QVERIFY(r.startDateTime() == QDateTime(QDate(2010, 10, 13)));
    QVERIFY(!r.isAllDay());
    r.setAllDay(true);
    QVERIFY(r.isAllDay());

    r.setEndDateTime(QDateTime(QDate(2010, 10, 14)));
    QVERIFY(r.endDateTime() == QDateTime(QDate(2010, 10, 14)));

    // add
    QOrganizerEventTime r2;
    r2.setStartDateTime(QDateTime(QDate(2010, 10, 23)));
    r2.setEndDateTime(QDateTime(QDate(2010, 10, 24)));
    QVERIFY(e.saveDetail(&r));
    QVERIFY(e.details(QOrganizerItemDetail::TypeEventTime).size() == 1);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeEventTime) == r);
    QVERIFY(e.saveDetail(&r2)); // note that we don't enforce uniqueness in organizeritem; that's done by the manager!
    QVERIFY(e.details(QOrganizerItemDetail::TypeEventTime).size() == 2);
    QVERIFY(e.removeDetail(&r2));
    QVERIFY(e.details(QOrganizerItemDetail::TypeEventTime).size() == 1);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeEventTime) == r);

    // update
    r.setAllDay(false);
    QVERIFY(e.detail(QOrganizerItemDetail::TypeEventTime) != r);
    QVERIFY(e.saveDetail(&r));
    QVERIFY(e.detail(QOrganizerItemDetail::TypeEventTime) == r);
    QVERIFY(e.details(QOrganizerItemDetail::TypeEventTime).size() == 1);

    // remove
    QVERIFY(e.removeDetail(&r));
    QVERIFY(e.details(QOrganizerItemDetail::TypeEventTime).size() == 0);
}

void tst_QOrganizerItemDetails::journalTime()
{
    QOrganizerJournalTime r;
    QOrganizerJournal j;

    QVERIFY(r.isEmpty());
    r.setEntryDateTime(QDateTime(QDate(2010, 10, 13)));
    QVERIFY(r.entryDateTime() == QDateTime(QDate(2010, 10, 13)));

    // add
    QOrganizerJournalTime r2;
    r2.setEntryDateTime(QDateTime(QDate(2010, 10, 23)));
    QVERIFY(j.saveDetail(&r));
    QVERIFY(j.details(QOrganizerItemDetail::TypeJournalTime).size() == 1);
    QVERIFY(j.detail(QOrganizerItemDetail::TypeJournalTime) == r);
    QVERIFY(j.saveDetail(&r2)); // note that we don't enforce uniqueness in organizeritem; that's done by the manager!
    QVERIFY(j.details(QOrganizerItemDetail::TypeJournalTime).size() == 2);
    QVERIFY(j.removeDetail(&r2));
    QVERIFY(j.details(QOrganizerItemDetail::TypeJournalTime).size() == 1);
    QVERIFY(j.detail(QOrganizerItemDetail::TypeJournalTime) == r);

    // update
    r.setEntryDateTime(QDateTime(QDate(2010, 11, 05)));
    QVERIFY(j.detail(QOrganizerItemDetail::TypeJournalTime) != r);
    QVERIFY(j.saveDetail(&r));
    QVERIFY(j.detail(QOrganizerItemDetail::TypeJournalTime) == r);
    QVERIFY(j.details(QOrganizerItemDetail::TypeJournalTime).size() == 1);

    // remove
    QVERIFY(j.removeDetail(&r));
    QVERIFY(j.details(QOrganizerItemDetail::TypeJournalTime).size() == 0);
}

void tst_QOrganizerItemDetails::todoTime()
{
    QOrganizerTodoTime r;
    QOrganizerTodo t;

    QVERIFY(r.isEmpty());
    QVERIFY(!r.isAllDay());

    r.setStartDateTime(QDateTime(QDate(2010, 10, 13)));
    QVERIFY(r.startDateTime() == QDateTime(QDate(2010, 10, 13)));
    QVERIFY(!r.isAllDay());
    r.setAllDay(true);
    QVERIFY(r.isAllDay());

    r.setDueDateTime(QDateTime(QDate(2010, 10, 14)));
    QVERIFY(r.dueDateTime() == QDateTime(QDate(2010, 10, 14)));

    // add
    QOrganizerTodoTime r2;
    r2.setStartDateTime(QDateTime(QDate(2010, 10, 23)));
    r2.setDueDateTime(QDateTime(QDate(2010, 10, 24)));
    QVERIFY(t.saveDetail(&r));
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoTime).size() == 1);
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoTime) == r);
    QVERIFY(t.saveDetail(&r2)); // note that we don't enforce uniqueness in organizeritem; that's done by the manager!
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoTime).size() == 2);
    QVERIFY(t.removeDetail(&r2));
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoTime).size() == 1);
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoTime) == r);

    // update
    r.setAllDay(false);
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoTime) != r);
    QVERIFY(t.saveDetail(&r));
    QVERIFY(t.detail(QOrganizerItemDetail::TypeTodoTime) == r);
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoTime).size() == 1);

    // remove
    QVERIFY(t.removeDetail(&r));
    QVERIFY(t.details(QOrganizerItemDetail::TypeTodoTime).size() == 0);
}

void tst_QOrganizerItemDetails::reminder()
{
    QOrganizerItemReminder r;
    QOrganizerItem oi;

    QVERIFY(r.isEmpty());
    QVERIFY(r.reminderType() == QOrganizerItemReminder::NoReminder);

    QVERIFY(r.secondsBeforeStart() == 0);
    QVERIFY(r.repetitionCount() == 0);
    QVERIFY(r.repetitionDelay() == 0);

    r.setSecondsBeforeStart(45);
    QVERIFY(!r.isEmpty());
    r.removeValue(QOrganizerItemReminder::FieldSecondsBeforeStart);
    QVERIFY(r.isEmpty());

    r.setRepetition(3, 100);
    QVERIFY(r.repetitionCount() == 3);
    QVERIFY(r.repetitionDelay() == 100);
    QVERIFY(!r.isEmpty());

    r.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(r.secondsBeforeStart() == 30);

    // add
    QVERIFY(oi.details(QOrganizerItemDetail::TypeReminder).size() == 0);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeReminder).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeReminder) == r);

    // update
    r.setSecondsBeforeStart(300);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeReminder) != r);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeReminder).size() == 1); // should update, not add another
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeReminder) == r);

    // remove
    QVERIFY(oi.removeDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeReminder).size() == 0);
}

void tst_QOrganizerItemDetails::audibleReminder()
{
    QOrganizerItemAudibleReminder r;
    QOrganizerItem oi;

    QVERIFY(r.isEmpty());
    QVERIFY(r.reminderType() == QOrganizerItemReminder::AudibleReminder);

    QVERIFY(r.secondsBeforeStart() == 0);
    QVERIFY(r.repetitionCount() == 0);
    QVERIFY(r.repetitionDelay() == 0);

    r.setSecondsBeforeStart(45);
    QVERIFY(!r.isEmpty());
    r.removeValue(QOrganizerItemReminder::FieldSecondsBeforeStart);
    QVERIFY(r.isEmpty());

    QOrganizerItemReminder baseR;
    baseR = r;
    QVERIFY(baseR.isEmpty());
    baseR.setSecondsBeforeStart(50);
    QVERIFY(baseR.secondsBeforeStart() == 50);
    QVERIFY(r.secondsBeforeStart() == 0);
    QVERIFY(!baseR.isEmpty());
    QVERIFY(r.isEmpty());

    r.setRepetition(3, 100);
    QVERIFY(r.repetitionCount() == 3);
    QVERIFY(r.repetitionDelay() == 100);
    QVERIFY(!r.isEmpty());

    r.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(r.secondsBeforeStart() == 30);

    // add
    QVERIFY(oi.details(QOrganizerItemDetail::TypeAudibleReminder).size() == 0);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeAudibleReminder).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeAudibleReminder) == r);

    // update
    r.setSecondsBeforeStart(300);
    r.setDataUrl(QUrl("http://www.test.com"));
    QVERIFY(r.dataUrl() == QUrl("http://www.test.com"));
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeAudibleReminder) != r);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeAudibleReminder).size() == 1); // should update, not add another
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeAudibleReminder) == r);

    // remove
    QVERIFY(oi.removeDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeAudibleReminder).size() == 0);
}

void tst_QOrganizerItemDetails::emailReminder()
{
    QOrganizerItemEmailReminder r;
    QOrganizerItem oi;

    QVERIFY(r.isEmpty());
    QVERIFY(r.reminderType() == QOrganizerItemReminder::EmailReminder);

    QVERIFY(r.secondsBeforeStart() == 0);
    QVERIFY(r.repetitionCount() == 0);
    QVERIFY(r.repetitionDelay() == 0);

    r.setRepetition(3, 100);
    QVERIFY(r.repetitionCount() == 3);
    QVERIFY(r.repetitionDelay() == 100);
    QVERIFY(!r.isEmpty());

    r.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(r.secondsBeforeStart() == 30);

    // add
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEmailReminder).size() == 0);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEmailReminder).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEmailReminder) == r);

    // update
    r.setSecondsBeforeStart(300);
    r.setContents("subject", "body", QVariantList());
    r.setRecipients(QStringList() << "recipient" << "other recipient");
    QVERIFY(r.subject() == QString("subject"));
    QVERIFY(r.body() == QString("body"));
    QVERIFY(r.attachments() == QVariantList());
    QVERIFY(r.recipients() == (QStringList() << "recipient" << "other recipient"));
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEmailReminder) != r);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEmailReminder).size() == 1); // should update, not add another
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEmailReminder) == r);

    // remove
    QVERIFY(oi.removeDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEmailReminder).size() == 0);
}

void tst_QOrganizerItemDetails::visualReminder()
{
    QOrganizerItemVisualReminder r;
    QOrganizerItem oi;

    QVERIFY(r.isEmpty());
    QVERIFY(r.reminderType() == QOrganizerItemReminder::VisualReminder);

    QVERIFY(r.secondsBeforeStart() == 0);
    QVERIFY(r.repetitionCount() == 0);
    QVERIFY(r.repetitionDelay() == 0);

    r.setRepetition(3, 100);
    QVERIFY(r.repetitionCount() == 3);
    QVERIFY(r.repetitionDelay() == 100);
    QVERIFY(!r.isEmpty());

    r.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(r.secondsBeforeStart() == 30);

    // add
    QVERIFY(oi.details(QOrganizerItemDetail::TypeVisualReminder).size() == 0);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeVisualReminder).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeVisualReminder) == r);

    // update
    r.setSecondsBeforeStart(300);
    r.setMessage("test");
    r.setDataUrl(QUrl("http://www.test.com"));
    QVERIFY(r.message() == QString("test"));
    QVERIFY(r.dataUrl() == QUrl("http://www.test.com"));
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeVisualReminder) != r);
    QVERIFY(oi.saveDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeVisualReminder).size() == 1); // should update, not add another
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeVisualReminder) == r);

    // remove
    QVERIFY(oi.removeDetail(&r));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeVisualReminder).size() == 0);
}

void tst_QOrganizerItemDetails::attendee()
{
    QOrganizerEventAttendee a;
    QOrganizerItem oi;

    QVERIFY(a.isEmpty());
    QVERIFY(a.name().isEmpty());
    QVERIFY(a.emailAddress().isEmpty());
    QVERIFY(a.attendeeId().isEmpty());
    QVERIFY(a.participationRole() == 0);
    QVERIFY(a.participationStatus() == 0);

    a.setAttendeeId("123456");
    QVERIFY(a.attendeeId() == QString("123456"));
    a.setEmailAddress("people@nokia.com");
    QVERIFY(a.emailAddress() == QString("people@nokia.com"));
    a.setName("people");
    QVERIFY(a.name() == QString("people"));
    a.setParticipationRole(QOrganizerEventAttendee::RoleRequiredParticipant);
    QVERIFY(a.participationRole() == QOrganizerEventAttendee::RoleRequiredParticipant);
    a.setParticipationStatus(QOrganizerEventAttendee::StatusAccepted);
    QVERIFY(a.participationStatus() == QOrganizerEventAttendee::StatusAccepted);
    // add
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEventAttendee).size() == 0);
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEventAttendee).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEventAttendee) == a);
    // update
    a.setAttendeeId("54321");
    a.setName("newpeople");
    a.setEmailAddress("newpeople@nokia.com");
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEventAttendee) != a);
    QVERIFY(oi.saveDetail(&a));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEventAttendee).size() == 1);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEventAttendee) == a);

    // add one more attendee
    QOrganizerEventAttendee a1;
    a1.setAttendeeId("777777");
    a1.setName("people1");
    a1.setEmailAddress("people1@nokia.com");
    QVERIFY(oi.saveDetail(&a1));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEventAttendee).size() == 2);

    // remove
    QVERIFY(oi.removeDetail(&a));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEventAttendee).size() == 1);
    QVERIFY(oi.removeDetail(&a1));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeEventAttendee).size() == 0);
}

void tst_QOrganizerItemDetails::rsvp()
{
    QOrganizerEventRsvp rsvp;
    QOrganizerItem oi;

    QVERIFY(rsvp.isEmpty());
    QVERIFY(rsvp.organizerName().isEmpty());
    QVERIFY(rsvp.organizerEmail().isEmpty());
    QVERIFY(rsvp.responseDate().isNull());
    QVERIFY(!rsvp.responseDate().isValid());
    QVERIFY(rsvp.responseDeadline().isNull());
    QVERIFY(!rsvp.responseDeadline().isValid());
    QVERIFY(rsvp.participationRole() == 0);
    QVERIFY(rsvp.participationStatus() == 0);
    QVERIFY(rsvp.responseRequirement() == 0);

    rsvp.setOrganizerName("Donald Duck");
    QVERIFY(rsvp.organizerName() == QString("Donald Duck"));
    rsvp.setOrganizerEmail("don@duck.com");
    QVERIFY(rsvp.organizerEmail() == QString("don@duck.com"));
    QDate testDate(2010, 10, 10);
    rsvp.setResponseDate(testDate);
    QVERIFY(!rsvp.responseDate().isNull());
    QVERIFY(rsvp.responseDate().isValid());
    QCOMPARE(testDate, rsvp.responseDate());
    rsvp.setResponseDeadline(testDate);
    QVERIFY(!rsvp.responseDeadline().isNull());
    QVERIFY(rsvp.responseDeadline().isValid());
    QCOMPARE(testDate, rsvp.responseDeadline());
    rsvp.setParticipationRole(QOrganizerEventAttendee::RoleOrganizer);
    QVERIFY(rsvp.participationRole() == QOrganizerEventAttendee::RoleOrganizer);
    rsvp.setParticipationStatus(QOrganizerEventAttendee::StatusAccepted);
    QVERIFY(rsvp.participationStatus() == QOrganizerEventAttendee::StatusAccepted);
    rsvp.setResponseRequirement(QOrganizerEventRsvp::ResponseRequired);
    QVERIFY(rsvp.responseRequirement() == QOrganizerEventRsvp::ResponseRequired);

    // add
    QCOMPARE(0, oi.details(QOrganizerItemDetail::TypeEventRsvp).size());
    QVERIFY(oi.saveDetail(&rsvp));
    QCOMPARE(1, oi.details(QOrganizerItemDetail::TypeEventRsvp).size());
    QVERIFY(rsvp == oi.detail(QOrganizerItemDetail::TypeEventRsvp));

    // update
    rsvp.setOrganizerName("Mickey Mouse");
    rsvp.setOrganizerEmail("mick@mouse.com");
    QDate testDate2(2011, 11, 11);
    rsvp.setResponseDate(testDate2);
    rsvp.setResponseDeadline(testDate2);
    rsvp.setParticipationRole(QOrganizerEventAttendee::RoleChairperson);
    rsvp.setParticipationStatus(QOrganizerEventAttendee::StatusDelegated);
    rsvp.setResponseRequirement(QOrganizerEventRsvp::ResponseNotRequired);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEventRsvp) != rsvp);
    QVERIFY(oi.saveDetail(&rsvp));
    QCOMPARE(1, oi.details(QOrganizerItemDetail::TypeEventRsvp).size());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeEventRsvp) == rsvp);

    // add another rsvp - not supported by all the backends
    QOrganizerEventRsvp rsvp2;
    rsvp2.setOrganizerName("Goofy");
    QVERIFY(oi.saveDetail(&rsvp2));
    QCOMPARE(2, oi.details(QOrganizerItemDetail::TypeEventRsvp).size());

    // remove
    QVERIFY(oi.removeDetail(&rsvp2));
    QCOMPARE(1, oi.details(QOrganizerItemDetail::TypeEventRsvp).size());
    QVERIFY(oi.removeDetail(&rsvp));
    QCOMPARE(0, oi.details(QOrganizerItemDetail::TypeEventRsvp).size());
}

void tst_QOrganizerItemDetails::classification()
{
    QOrganizerItemClassification classification;
    QOrganizerItem oi;

    QVERIFY(classification.isEmpty());

    // setters/getters
    classification.setClassification(QOrganizerItemClassification::AccessPublic);
    QVERIFY(classification.classification() == QOrganizerItemClassification::AccessPublic);
    classification.setClassification(QOrganizerItemClassification::AccessPrivate);
    QVERIFY(classification.classification() == QOrganizerItemClassification::AccessPrivate);
    classification.setClassification(QOrganizerItemClassification::AccessConfidential);
    QVERIFY(classification.classification() == QOrganizerItemClassification::AccessConfidential);

    // add
    QCOMPARE(0, oi.details(QOrganizerItemDetail::TypeClassification).size());
    QVERIFY(oi.saveDetail(&classification));
    QCOMPARE(1, oi.details(QOrganizerItemDetail::TypeClassification).size());
    QCOMPARE(classification, static_cast<QOrganizerItemClassification>(oi.detail(QOrganizerItemDetail::TypeClassification)));

    // update
    classification.setValue(QOrganizerItemClassification::FieldClassification, QOrganizerItemClassification::AccessPrivate);
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeClassification) != classification);
    QVERIFY(oi.saveDetail(&classification));
    QCOMPARE(1, oi.details(QOrganizerItemDetail::TypeClassification).size());
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeClassification) == classification);

    // try adding another, amount of details should stay the same
    QOrganizerItemClassification classification2;
    classification2.setClassification(QOrganizerItemClassification::AccessConfidential);
    QVERIFY(oi.saveDetail(&classification2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeClassification).size(), 1);

    // remove
    QVERIFY(oi.removeDetail(&classification2));
    QCOMPARE(oi.details(QOrganizerItemDetail::TypeClassification).size(), 0);
}

void tst_QOrganizerItemDetails::version()
{
    QOrganizerItemVersion version;
    QCOMPARE(version.type(), QOrganizerItemDetail::TypeVersion);
    QVERIFY(!version.hasValue(QOrganizerItemVersion::FieldVersion));
    QVERIFY(!version.hasValue(QOrganizerItemVersion::FieldExtendedVersion));

    version.setVersion(64);
    QCOMPARE(version.version(), 64);
    QVERIFY(version.hasValue(QOrganizerItemVersion::FieldVersion));
    QVERIFY(!version.hasValue(QOrganizerItemVersion::FieldExtendedVersion));

    QByteArray extendedVersion("Qt rules!");
    version.setExtendedVersion(extendedVersion);
    QCOMPARE(version.extendedVersion(), extendedVersion);
    QVERIFY(version.hasValue(QOrganizerItemVersion::FieldVersion));
    QVERIFY(version.hasValue(QOrganizerItemVersion::FieldExtendedVersion));
}

QTEST_MAIN(tst_QOrganizerItemDetails)
#include "tst_qorganizeritemdetails.moc"
