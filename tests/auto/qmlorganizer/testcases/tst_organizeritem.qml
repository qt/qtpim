/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 2.0

TestCase {
    name: "OrganizerItemTests"
    id: organizerItemTests
    property int waitTime : 200

    QOrganizerTestUtility {
        id: utility
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

    ExtendedDetail {
        id: extendedDetail
        name: "extendedDetail1"
        data: "data 1"
    }

    function test_todoOccurrence() {
        compare(todoOccurrence.itemType, Type.TodoOccurrence)

        todoOccurrence.progressPercentage = 89
        compare(todoOccurrence.progressPercentage, 89)

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

        todo.isAllDay = true;
        compare(todo.isAllDay, true)

        todo.progressPercentage = 64
        compare(todo.progressPercentage, 64)

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
        var itemChangedSpy = utility.create_testobject("import QtTest 1.0;"
                 + "SignalSpy {id : organizerChangedSpy;}"
                 , organizerItemTests);
        itemChangedSpy.target = eventOccurrence;
        itemChangedSpy.signalName = "itemChanged";
        var count = 0;

        compare(eventOccurrence.itemType, Type.EventOccurrence)

        var originalDate = new Date("2008-12-28")
        eventOccurrence.originalDate = originalDate
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.originalDate, originalDate)
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

        eventOccurrence.parentId = event.itemId
        itemChangedSpy.wait(waitTime);
        compare(eventOccurrence.parentId, event.itemId)
        compare(itemChangedSpy.count, ++count)
    }

    function test_event() {
        var itemChangedSpy = utility.create_testobject("import QtTest 1.0;"
                 + "SignalSpy {id : organizerChangedSpy;}"
                 , organizerItemTests);
        itemChangedSpy.target = event;
        itemChangedSpy.signalName = "itemChanged";
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
        //itemChangedSpy.wait(waitTime);
        compare(event.details(Detail.Recurrence).length, 1)
        compare(event.recurrence.recurrenceDates.length, 2)
        //compare(itemChangedSpy.count, ++count)
    }

    function test_item() {
        var itemChangedSpy = utility.create_testobject("import QtTest 1.0;"
                 + "SignalSpy {id : organizerChangedSpy;}"
                 , organizerItemTests);
        itemChangedSpy.target = item;
        itemChangedSpy.signalName = "itemChanged";
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
        compare(item.details(Detail.Customized).length, 1)
        compare(item.detail(Detail.Customized).name, "extendedDetail1")
        compare(item.detail(Detail.Customized).data, "data 1")
        compare(itemChangedSpy.count, ++count)
    }
}
