/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/qtest.h>

#include <qorganizeritemdetails.h>
#include <qorganizeritems.h>
#include <qorganizermanager.h>

QTORGANIZER_USE_NAMESPACE

class tst_QOrganizerE2E : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testMegaItem_data() { addManager(); }
    void testMegaItem();

    void testUnsupportedItemType_data() { addManager(); }
    void testUnsupportedItemType();

    void testInvalidParentId_data() { addManager(); }
    void testInvalidParentId();

private:
    void addManager();

    QOrganizerItem createItem(const QString &itemType);
    QOrganizerItemDetail createDetail(const QString &definitionName);

    // <manager, items> pair for existing items
    QMap<QString, QList<QOrganizerItem> > existingItems;
};

void tst_QOrganizerE2E::initTestCase()
{
    // back-up all existing items
    QStringList availableManagers(QOrganizerManager::availableManagers());
    foreach (const QString &manager, availableManagers) {
        QOrganizerManager organizerManager(manager);
        QList<QOrganizerItem> items(organizerManager.items());
        if (items.size() > 0) {
            existingItems.insert(manager, items);
            QList<QOrganizerItemId> itemIds(organizerManager.itemIds());
            organizerManager.removeItems(itemIds);
        }
    }
}

void tst_QOrganizerE2E::cleanupTestCase()
{
    // remove all items generated when running tests
    QStringList availableManagers(QOrganizerManager::availableManagers());
    foreach (const QString &manager, availableManagers) {
        QOrganizerManager organizerManager(manager);
        organizerManager.removeItems(organizerManager.itemIds());
    }

    // restore all existing items
    QMap<QString, QList<QOrganizerItem> >::iterator i = existingItems.begin();
    while (i != existingItems.end()) {
        QOrganizerManager organizerManager(i.key());
        organizerManager.saveItems(&(i.value()));
        ++i;
    }
}

void tst_QOrganizerE2E::testMegaItem()
{
    QFETCH(QString, managerName);
    QOrganizerManager organizerManager(managerName);

    QStringList supportedItemTypes(organizerManager.supportedItemTypes());
    foreach (const QString &itemType, supportedItemTypes) {
        QOrganizerItem referenceItem = createItem(itemType);
        QCOMPARE(referenceItem.type(), itemType);

        QStringList supportedDetails(organizerManager.supportedItemDetails(itemType));
        foreach (const QString &supportedDetail, supportedDetails) {
            QOrganizerItemDetail detail = createDetail(supportedDetail);
            if (detail.definitionName() != supportedDetail)
                continue;

            QVERIFY(referenceItem.saveDetail(&detail));
        }

        // set parent for occurrence
        if (itemType == QOrganizerItemType::TypeEventOccurrence || itemType == QOrganizerItemType::TypeTodoOccurrence) {
            bool parentItemAvailable(false);
            QList<QOrganizerItem> items(organizerManager.items());
            foreach (const QOrganizerItem &item, items) {
                if ((itemType == QOrganizerItemType::TypeEventOccurrence && item.type() == QOrganizerItemType::TypeEvent)
                    || (itemType == QOrganizerItemType::TypeTodoOccurrence && item.type() == QOrganizerItemType::TypeTodo)) {
                    QOrganizerItemParent parent;
                    parent.setOriginalDate(QDate(1996, 9, 24));
                    parent.setParentId(item.id());
                    QVERIFY(referenceItem.saveDetail(&parent));
                    parentItemAvailable = true;
                    break;
                }
            }
            if (!parentItemAvailable)
                continue;
        }

        // so that the referenceItem stays the same
        QOrganizerItem itemToSave(referenceItem);
        QVERIFY(organizerManager.saveItem(&itemToSave));

        QOrganizerItem itemRead = organizerManager.item(itemToSave.id());

        // special details
        QCOMPARE(itemRead.type(), referenceItem.type());

        if (organizerManager.supportedItemDetails(itemType).contains(QOrganizerItemGuid::DefinitionName))
            QCOMPARE(itemRead.details<QOrganizerItemGuid>().size(), 1);

        if (organizerManager.supportedItemDetails(itemType).contains(QOrganizerItemTimestamp::DefinitionName))
            QCOMPARE(itemRead.details<QOrganizerItemTimestamp>().size(), 1);

        QCOMPARE(itemRead.details<QOrganizerItemReminder>().size(), 0);

        // all details should saved and loaded correctly
        foreach (const QOrganizerItemDetail &detail, referenceItem.details())
            QCOMPARE(itemRead.detail(detail.definitionName()), detail);
    }
}

void tst_QOrganizerE2E::testUnsupportedItemType()
{
    QFETCH(QString, managerName);
    QOrganizerManager organizerManager(managerName);

    QStringList supportedItemTypes(organizerManager.supportedItemTypes());
    if (!supportedItemTypes.contains(QOrganizerItemType::TypeEvent)) {
        QOrganizerItem event = createItem(QOrganizerItemType::TypeEvent);
        QVERIFY(!organizerManager.saveItem(&event));
    }
    if (!supportedItemTypes.contains(QOrganizerItemType::TypeEventOccurrence)) {
        QOrganizerItem eventOccurence = createItem(QOrganizerItemType::TypeEventOccurrence);
        QVERIFY(!organizerManager.saveItem(&eventOccurence));
    }
    if (!supportedItemTypes.contains(QOrganizerItemType::TypeTodo)) {
        QOrganizerItem todo = createItem(QOrganizerItemType::TypeTodo);
        QVERIFY(!organizerManager.saveItem(&todo));
    }
    if (!supportedItemTypes.contains(QOrganizerItemType::TypeTodoOccurrence)) {
        QOrganizerItem todoOccurence = createItem(QOrganizerItemType::TypeTodoOccurrence);
        QVERIFY(!organizerManager.saveItem(&todoOccurence));
    }
    if (!supportedItemTypes.contains(QOrganizerItemType::TypeJournal)) {
        QOrganizerItem journal = createItem(QOrganizerItemType::TypeJournal);
        QVERIFY(!organizerManager.saveItem(&journal));
    }
    if (!supportedItemTypes.contains(QOrganizerItemType::TypeNote)) {
        QOrganizerItem note = createItem(QOrganizerItemType::TypeNote);
        QVERIFY(!organizerManager.saveItem(&note));
    }
}

void tst_QOrganizerE2E::testInvalidParentId()
{
    QFETCH(QString, managerName);
    QOrganizerManager organizerManager(managerName);

    QStringList supportedItemTypes(organizerManager.supportedItemTypes());
    foreach (const QString &itemType, supportedItemTypes) {
        if (itemType == QOrganizerItemType::TypeEventOccurrence || itemType == QOrganizerItemType::TypeTodoOccurrence) {
            QOrganizerItem item = createItem(itemType);
            QCOMPARE(item.type(), itemType);

            QOrganizerItemParent parent;
            parent.setParentId(QOrganizerItemId());

            QVERIFY(item.saveDetail(&parent));
            QVERIFY(!organizerManager.saveItem(&item));
        }
    }
}

void tst_QOrganizerE2E::addManager()
{
    QTest::addColumn<QString>("managerName");

    QStringList availableManagers = QOrganizerManager::availableManagers();
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    foreach (const QString &manager, availableManagers)
        QTest::newRow(manager.toLatin1().constData()) << manager;
}

QOrganizerItem tst_QOrganizerE2E::createItem(const QString &itemType)
{
    if (itemType == QOrganizerItemType::TypeEvent)
        return QOrganizerEvent();
    else if (itemType == QOrganizerItemType::TypeEventOccurrence)
        return QOrganizerEventOccurrence();
    else if (itemType == QOrganizerItemType::TypeTodo)
        return QOrganizerTodo();
    else if (itemType == QOrganizerItemType::TypeTodoOccurrence)
        return QOrganizerTodoOccurrence();
    else if (itemType == QOrganizerItemType::TypeJournal)
        return QOrganizerJournal();
    else if (itemType == QOrganizerItemType::TypeNote)
        return QOrganizerNote();

    return QOrganizerItem();
}

QOrganizerItemDetail tst_QOrganizerE2E::createDetail(const QString &definitionName)
{
    if (definitionName == QOrganizerEventTime::DefinitionName) {
        QOrganizerEventTime eventTime;
        eventTime.setAllDay(false);
        eventTime.setStartDateTime(QDateTime::fromString(QStringLiteral("1991-08-25T20:57:08+00:00"), Qt::ISODate));
        eventTime.setEndDateTime(QDateTime::fromString(QStringLiteral("1995-05-20T11:22:33+02:00"), Qt::ISODate));
        return eventTime;
    } else if (definitionName == QOrganizerJournalTime::DefinitionName) {
        QOrganizerJournalTime journalTime;
        journalTime.setEntryDateTime(QDateTime::fromString(QStringLiteral("1991-08-25T20:57:08+00:00"), Qt::ISODate));
        return journalTime;
    } else if (definitionName == QOrganizerTodoTime::DefinitionName) {
        QOrganizerTodoTime todoTime;
        todoTime.setAllDay(true);
        todoTime.setStartDateTime(QDateTime::fromString(QStringLiteral("1991-08-25T20:57:08+00:00"), Qt::ISODate));
        todoTime.setDueDateTime(QDateTime::fromString(QStringLiteral("1995-05-20T11:22:33+02:00"), Qt::ISODate));
        return todoTime;
    } else if (definitionName == QOrganizerTodoProgress::DefinitionName) {
        QOrganizerTodoProgress todoProgress;
        todoProgress.setFinishedDateTime(QDateTime::fromString(QStringLiteral("1995-05-20T11:22:33+02:00"), Qt::ISODate));
        todoProgress.setPercentageComplete(64);
        todoProgress.setStatus(QOrganizerTodoProgress::StatusInProgress);
        return todoProgress;
    } else if (definitionName == QOrganizerItemReminder::DefinitionName) {
        // do nothing, because noboday should directly use this
    } else if (definitionName == QOrganizerItemAudibleReminder::DefinitionName) {
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl::fromLocalFile(QStringLiteral("some_random_path")));
        audibleReminder.setRepetition(6, 4);
        audibleReminder.setSecondsBeforeStart(1989);
        return audibleReminder;
    } else if (definitionName == QOrganizerItemVisualReminder::DefinitionName) {
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setDataUrl(QUrl::fromLocalFile(QStringLiteral("yet_another_path")));
        visualReminder.setMessage(QStringLiteral("Qt!!"));
        visualReminder.setRepetition(6, 4);
        visualReminder.setSecondsBeforeStart(1989);
        return visualReminder;
    } else if (definitionName == QOrganizerItemEmailReminder::DefinitionName) {
        QOrganizerItemEmailReminder emailReminder;
        emailReminder.setContents(QStringLiteral("Qt Rocks!!"),
                                  QStringLiteral("Qt - Cross-platform application and UI framework"),
                                  QVariantList() << QStringLiteral("Code once")
                                                 << QStringLiteral("Create more")
                                                 << QStringLiteral("Deploy everywhere"));
        emailReminder.setRecipients(QStringList() << QStringLiteral("Berlin")
                                                  << QStringLiteral("Brisbane")
                                                  << QStringLiteral("Oslo")
                                                  << QStringLiteral("Tampere"));
        emailReminder.setRepetition(6, 4);
        emailReminder.setSecondsBeforeStart(1989);
        return emailReminder;
    } else if (definitionName == QOrganizerItemComment::DefinitionName) {
        QOrganizerItemComment comment;
        comment.setComment(QStringLiteral("Qt Everywhere!"));
        return comment;
    } else if (definitionName == QOrganizerItemDescription::DefinitionName) {
        QOrganizerItemDescription description;
        description.setDescription(QStringLiteral("Qt is cute!"));
        return description;
    } else if (definitionName == QOrganizerItemDisplayLabel::DefinitionName) {
        QOrganizerItemDisplayLabel displayLabel;
        displayLabel.setLabel(QStringLiteral("Qt - Cross-platform application and UI framework"));
        return displayLabel;
    } else if (definitionName == QOrganizerItemGuid::DefinitionName) {
        // do nothing, because it should be set by the back-end engine
    } else if (definitionName == QOrganizerItemLocation::DefinitionName) {
        QOrganizerItemLocation location;
        location.setLabel(QStringLiteral("In the middle of nowhere"));
        location.setLatitude(19.84);
        location.setLongitude(6.4);
        return location;
    } else if (definitionName == QOrganizerItemParent::DefinitionName) {
        // do nothing, because it's handled specially
    } else if (definitionName == QOrganizerItemPriority::DefinitionName) {
        QOrganizerItemPriority priority;
        priority.setPriority(QOrganizerItemPriority::HighPriority);
        return priority;
    } else if (definitionName == QOrganizerItemRecurrence::DefinitionName) {
        QOrganizerItemRecurrence recurrence;

        recurrence.setRecurrenceDates(QSet<QDate>() << QDate(2005, 6, 28) << QDate(2005, 12, 19) << QDate(2006, 10, 4)
                                                    << QDate(2007, 5, 30) << QDate(2008, 5, 6) << QDate(2009, 3, 3)
                                                    << QDate(2009, 12, 1) << QDate(2010, 9, 21));

        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setInterval(1);
        recurrenceRule.setLimit(QOrganizerRecurrenceRule::NoLimit);
        recurrenceRule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>() << QOrganizerRecurrenceRule::September);
        recurrenceRule.setDaysOfMonth(QSet<int>() << 24);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);

        recurrence.setExceptionDates(QSet<QDate>() << QDate(2008, 9, 24) << QDate(2009, 9, 24) << QDate(2010, 9, 24));

        QOrganizerRecurrenceRule exceptionRule;
        exceptionRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        exceptionRule.setInterval(2);
        exceptionRule.setLimit(QOrganizerRecurrenceRule::NoLimit);
        exceptionRule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>() << QOrganizerRecurrenceRule::September);
        exceptionRule.setDaysOfMonth(QSet<int>() << 24);
        recurrence.setExceptionRules(QSet<QOrganizerRecurrenceRule>() << exceptionRule);

        return recurrence;
    } else if (definitionName == QOrganizerItemTimestamp::DefinitionName) {
        // do nothing, because it should be maintained by the back-end engine
    } else if (definitionName == QOrganizerItemType::DefinitionName) {
        // do nothing, because it should not be changed
    } else if (definitionName == QOrganizerItemTag::DefinitionName) {
        QOrganizerItemTag tag;
        tag.setTag(QStringLiteral("Qt Open Governance"));
    } else if (definitionName == QOrganizerItemExtendedDetail::DefinitionName) {
        QOrganizerItemExtendedDetail extendedDetail;
        extendedDetail.setName(QStringLiteral("My-Stupid-Extended-Detail"));
        extendedDetail.setData(QVariantList() << QStringLiteral("Code once") << QStringLiteral("Create more") << QStringLiteral("Deploy everywhere"));
        return extendedDetail;
    } else if (definitionName == QOrganizerEventAttendee::DefinitionName) {
        QOrganizerEventAttendee attendee;
        attendee.setName(QStringLiteral("people"));
        attendee.setContactId(QStringLiteral("123456"));
        attendee.setEmailAddress(QStringLiteral("people@nokia.com"));
        attendee.setParticipationRole(QOrganizerEventAttendee::RoleRequiredParticipant);
        attendee.setParticipationStatus(QOrganizerEventAttendee::StatusAccepted);
        return attendee;
    }

    return QOrganizerItemDetail();
}

QTEST_MAIN(tst_QOrganizerE2E)
#include "tst_qorganizere2e.moc"
