/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
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

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    name: "OrganizerItemTests"
    id: organizerItemTests
    property int waitTime : 200

    QOrganizerTestUtility {
        id: utility
    }

    SignalSpy {
        id: itemChangedSpy
        signalName: "itemChanged"
    }

    OrganizerItem {
        id: item
    }

    Event {
        id: event
    }

    EventOccurrence {
        id: eventOccurrence
    }

    Journal {
        id: journal
    }

    Note {
        id: note
    }

    Todo {
        id: todo
    }

    TodoOccurrence {
        id: todoOccurrence
    }

    Comment {
        id: comment
        comment: "Code Less"
    }

    EventAttendee {
        id: eventAttendee
    }

    ExtendedDetail {
        id: extendedDetail
        name: "extendedDetail1"
        data: "data 1"
    }

    function cleanup() {
        itemChangedSpy.clear()
        itemChangedSpy.target = null
    }

    function test_todoOccurrence() {
        itemChangedSpy.target = todoOccurrence;
        var count = 0;

        compare(todoOccurrence.itemType, Type.TodoOccurrence)

        compare(todoOccurrence.allDay, false)
        todoOccurrence.allDay = true;
        itemChangedSpy.wait(waitTime);
        compare(todoOccurrence.allDay, true)
        compare(itemChangedSpy.count, ++count)
        todoOccurrence.allDay = false;
        itemChangedSpy.wait(waitTime);
        compare(todoOccurrence.allDay, false)
        compare(itemChangedSpy.count, ++count)

        todoOccurrence.percentageComplete = 89
        compare(todoOccurrence.percentageComplete, 89)

        skip('TODO should be fixed conversion between local time and UTC to avoid a double conversion')

        var originalDate = new Date("2008-12-28")
        todoOccurrence.originalDate = originalDate
        compare(todoOccurrence.originalDate, originalDate)

        var startDateTime = new Date("1991-08-25T20:57:08Z")
        todoOccurrence.startDateTime = startDateTime
        compare(todoOccurrence.startDateTime, startDateTime)

        var dueDateTime = new Date("2008-12-28 15:22:33 GMT+0300")
        todoOccurrence.dueDateTime = dueDateTime
        compare(todoOccurrence.dueDateTime, dueDateTime)

        var finishedDateTime = new Date("1995-05-20 11:22:33 GMT+0200")
        todoOccurrence.finishedDateTime = finishedDateTime
        compare(todoOccurrence.finishedDateTime, finishedDateTime)

        todoOccurrence.priority = Priority.VeryHigh
        compare(todoOccurrence.priority, Priority.VeryHigh)

        todoOccurrence.status = TodoProgress.InProgress
        compare(todoOccurrence.status, TodoProgress.InProgress)

        todoOccurrence.parentId = todo.itemId
        compare(todoOccurrence.parentId, todo.itemId)
    }

    function test_todo() {
        compare(todo.itemType, Type.Todo)

        todo.allDay = true;
        compare(todo.allDay, true)

        todo.percentageComplete = 64
        compare(todo.percentageComplete, 64)

        var startDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        todo.startDateTime = startDateTime
        compare(todo.startDateTime, startDateTime)

        var dueDateTime = new Date("2008-12-28 15:22:33 GMT+0300")
        todo.dueDateTime = dueDateTime
        compare(todo.dueDateTime, dueDateTime)

        var finishedDateTime = new Date("1995-05-20 11:22:33 GMT+0200")
        todo.finishedDateTime = finishedDateTime
        compare(todo.finishedDateTime, finishedDateTime)

        todo.priority = Priority.VeryHigh
        compare(todo.priority, Priority.VeryHigh)

        todo.status = TodoProgress.InProgress
        compare(todo.status, TodoProgress.InProgress)

        // recurrence
        var recurrenceDates = new Array()
        recurrenceDates[0] = new Date("2005-06-28")
        recurrenceDates[1] = new Date("2005-12-19")
        compare(todo.details(Detail.Recurrence).length, 0)
        todo.recurrence.recurrenceDates = recurrenceDates
        compare(todo.details(Detail.Recurrence).length, 1)
        compare(todo.recurrence.recurrenceDates.length, 2)
    }

    function test_note() {
        compare(note.itemType, Type.Note)
    }

    function test_journal() {
        compare(journal.itemType, Type.Journal)

        var dateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        journal.dateTime = dateTime
        compare(journal.dateTime, dateTime)
    }

    function test_eventOccurrence() {
        itemChangedSpy.target = eventOccurrence
        var count = 0;

        compare(eventOccurrence.allDay, false)
        eventOccurrence.allDay = true;
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.allDay, true)
        compare(itemChangedSpy.count, ++count)
        eventOccurrence.allDay = false;
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.allDay, false)
        compare(itemChangedSpy.count, ++count)

        compare(eventOccurrence.itemType, Type.EventOccurrence)

        eventOccurrence.parentId = event.itemId;
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.parentId, event.itemId)
        compare(itemChangedSpy.count, ++count)

        skip('TODO should be fixed conversion between local time and UTC to avoid a double conversion')

        var originalDate = new Date("2008-12-28")
        eventOccurrence.originalDate = originalDate
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.originalDate, utility.toUTCMidnight(originalDate))
        compare(itemChangedSpy.count, ++count)

        var startDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        eventOccurrence.startDateTime = startDateTime
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.startDateTime, startDateTime)
        compare(itemChangedSpy.count, ++count)

        var endDateTime = new Date("1995-05-20 11:22:33 GMT+0200")
        eventOccurrence.endDateTime = endDateTime
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.endDateTime, endDateTime)
        compare(itemChangedSpy.count, ++count)

        eventOccurrence.priority = Priority.VeryHigh
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.priority, Priority.VeryHigh)
        compare(itemChangedSpy.count, ++count)

        eventOccurrence.location = "Tampere"
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.location, "Tampere")
        compare(itemChangedSpy.count, ++count)

    }

    function test_event() {
        itemChangedSpy.target = event;
        var count = 0;

        compare(event.itemType, Type.Event);

        event.allDay = true;
        itemChangedSpy.wait(waitTime);
        compare(event.allDay, true)
        compare(itemChangedSpy.count, ++count)

        //Following format does not work: GMT+0000 is not accepted by javascript
        //event.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        var startDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        event.startDateTime = startDateTime
        itemChangedSpy.wait(waitTime);
        compare(event.startDateTime, startDateTime)
        compare(itemChangedSpy.count, ++count)

        var endDateTime = new Date("1995-05-20 11:22:33 GMT+0200")
        event.endDateTime = endDateTime
        itemChangedSpy.wait(waitTime);
        compare(event.endDateTime, endDateTime)
        compare(itemChangedSpy.count, ++count)

        event.priority = Priority.VeryHigh
        itemChangedSpy.wait(waitTime);
        compare(event.priority, Priority.VeryHigh)
        compare(itemChangedSpy.count, ++count)

        event.location = "Tampere"
        itemChangedSpy.wait(waitTime);
        compare(event.location, "Tampere")
        compare(itemChangedSpy.count, ++count)

        // recurrence
        var recurrenceDates = new Array()
        recurrenceDates[0] = new Date("2005-06-28")
        recurrenceDates[1] = new Date("2005-12-19")
        compare(event.details(Detail.Recurrence).length, 0)
        event.recurrence.recurrenceDates = recurrenceDates
        itemChangedSpy.wait(waitTime);
        compare(event.details(Detail.Recurrence).length, 1)
        compare(event.recurrence.recurrenceDates.length, 2)
        compare(itemChangedSpy.count, ++count)

        // attendee
        eventAttendee.name = "new attendee"
        eventAttendee.emailAddress = "new.attendee@qt.com"
        eventAttendee.attendeeId = "123444455555"
        eventAttendee.participationStatus = EventAttendee.StatusAccepted
        eventAttendee.participationRole = EventAttendee.RoleRequiredParticipant
        event.setDetail(eventAttendee)
        itemChangedSpy.wait(waitTime);
        compare(event.details(Detail.EventAttendee).length, 1)
        compare(event.detail(Detail.EventAttendee).name, "new attendee")
        compare(event.detail(Detail.EventAttendee).emailAddress, "new.attendee@qt.com")
        compare(event.detail(Detail.EventAttendee).participationStatus, EventAttendee.StatusAccepted)
        compare(event.detail(Detail.EventAttendee).participationRole, EventAttendee.RoleRequiredParticipant)
        compare(itemChangedSpy.count, ++count)

        // add one more dynamic created attendee
        var eventAttendee2 = utility.create_testobject("import QtTest 1.0;import QtOrganizer 5.0;"
                 + "EventAttendee {}"
                 , organizerItemTests);
        event.setDetail(eventAttendee2)
        itemChangedSpy.wait(waitTime);
        compare(event.details(Detail.EventAttendee).length, 2)
        compare(itemChangedSpy.count, ++count)

        // remove one attendee
        event.removeDetail(eventAttendee)
        itemChangedSpy.wait(waitTime);
        compare(event.details(Detail.EventAttendee).length, 1)
        compare(itemChangedSpy.count, ++count)

        // remove last attendee
        event.removeDetail(eventAttendee2)
        itemChangedSpy.wait(waitTime);
        compare(event.details(Detail.EventAttendee).length, 0)
        compare(itemChangedSpy.count, ++count)

        // attenddees property
        event.attendees = [eventAttendee];
        itemChangedSpy.wait(waitTime);
        compare(event.attendees.length, 1)
        compare(itemChangedSpy.count, ++count)

        // attendees append test
        event.attendees = [eventAttendee, eventAttendee2];
        itemChangedSpy.wait(waitTime);
        compare(event.attendees.length, 2)
        count += 3;//clear + 2 * append signals
        compare(itemChangedSpy.count, count)

        event.attendees = [eventAttendee2];
        itemChangedSpy.wait(waitTime);
        compare(event.attendees.length, 1)
        count += 2;//clear + append signals
        compare(itemChangedSpy.count, count)

        // attendees clear
        event.attendees = [];
        itemChangedSpy.wait(waitTime);
        compare(event.attendees.length, 0)
        count += 1;//clear
        compare(itemChangedSpy.count, count)
    }

    function test_item() {
        itemChangedSpy.target = item
        var count = 0;

        // empty OrganizerItem
        compare(item.modified, false)

        // access Description
        item.description = "Qt Open Governance"
        itemChangedSpy.wait(waitTime);
        compare(item.description, "Qt Open Governance")
        compare(item.detail(Detail.Description).description, "Qt Open Governance")
        compare(item.details(Detail.Description).length, 1)
        compare(itemChangedSpy.count, ++count)

        // access DisplayLabel
        item.displayLabel = "http://qt-project.org/"
        itemChangedSpy.wait(waitTime);
        compare(item.displayLabel, "http://qt-project.org/")
        compare(item.detail(Detail.DisplayLabel).label, "http://qt-project.org/")
        compare(item.details(Detail.DisplayLabel).length, 1)
        compare(itemChangedSpy.count, ++count)

        // access Guid
        item.guid = "b7dd2d61-fcb3-1170-e314-899ac5e91c7c"
        itemChangedSpy.wait(waitTime);
        compare(item.guid, "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
        compare(item.detail(Detail.Guid).guid, "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
        compare(item.details(Detail.Guid).length, 1)
        compare(itemChangedSpy.count, ++count)

        // add new Detail
        item.setDetail(comment)
        itemChangedSpy.wait(waitTime);
        compare(item.detail(Detail.Comment).comment, "Code Less")
        compare(item.details(Detail.Comment).length, 1)
        compare(item.itemDetails.length, 4)
        compare(itemChangedSpy.count, ++count)

        // update existing Detail
        comment.comment = "Create More"
        item.setDetail(comment)
        itemChangedSpy.wait(waitTime);
        compare(item.detail(Detail.Comment).comment, "Create More")
        compare(item.details(Detail.Comment).length, 1)
        compare(itemChangedSpy.count, ++count)

        // delete one existing Detail
        item.removeDetail(comment)
        itemChangedSpy.wait(waitTime);
        compare(item.details(Detail.Comment).length, 0)
        compare(itemChangedSpy.count, ++count)

        // delete one no-existing Detail
        item.removeDetail(comment)
        compare(item.details(Detail.Comment).length, 0)
        compare(itemChangedSpy.count, count)

        // remove all existing Details
        item.clearDetails();
        itemChangedSpy.wait(waitTime);
        compare(item.itemDetails.length, 0)
        compare(itemChangedSpy.count, ++count)

        // remove all existing Details again will not get any signal
        item.clearDetails();
        compare(item.itemDetails.length, 0)
        compare(itemChangedSpy.count, count)

        // extended detail
        item.setDetail(extendedDetail)
        itemChangedSpy.wait(waitTime);
        compare(item.details(Detail.ExtendedDetail).length, 1)
        compare(item.detail(Detail.ExtendedDetail).name, "extendedDetail1")
        compare(item.detail(Detail.ExtendedDetail).data, "data 1")
        compare(itemChangedSpy.count, ++count)
    }
}
