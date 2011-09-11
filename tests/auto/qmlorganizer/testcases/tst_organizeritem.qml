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
import QtAddOn.organizer 2.0

TestCase {
    name: "OrganizerItemTests"

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

    function test_todoOccurrence() {
        compare(todoOccurrence.itemType, Type.TodoOccurrence)

        todoOccurrence.progressPercentage = 89
        compare(todoOccurrence.progressPercentage, 89)

        todoOccurrence.originalDate = "2008-12-28"
        compare(todoOccurrence.originalDate, "2008-12-28")

        todoOccurrence.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(todoOccurrence.startDateTime, "1991-08-25 20:57:08 GMT+0000")

        todoOccurrence.dueDateTime = "2008-12-28 15:22:33 GMT+0300"
        compare(todoOccurrence.dueDateTime, "2008-12-28 15:22:33 GMT+0300")

        todoOccurrence.finishedDateTime = "1995-05-20 11:22:33 GMT+0200"
        compare(todoOccurrence.finishedDateTime, "1995-05-20 11:22:33 GMT+0200")

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

        todo.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(todo.startDateTime, "1991-08-25 20:57:08 GMT+0000")

        todo.dueDateTime = "2008-12-28 15:22:33 GMT+0300"
        compare(todo.dueDateTime, "2008-12-28 15:22:33 GMT+0300")

        todo.finishedDateTime = "1995-05-20 11:22:33 GMT+0200"
        compare(todo.finishedDateTime, "1995-05-20 11:22:33 GMT+0200")

        todo.priority = Priority.VeryHigh
        compare(todo.priority, Priority.VeryHigh)

        todo.status = TodoProgress.InProgress
        compare(todo.status, TodoProgress.InProgress)
    }

    function test_note() {
        compare(note.itemType, Type.Note)
    }

    function test_journal() {
        compare(journal.itemType, Type.Journal)

        journal.dateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(journal.dateTime, "1991-08-25 20:57:08 GMT+0000")
    }

    function test_eventOccurrence() {
        compare(eventOccurrence.itemType, Type.EventOccurrence)

        eventOccurrence.originalDate = "2008-12-28"
        compare(eventOccurrence.originalDate, "2008-12-28")

        eventOccurrence.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(eventOccurrence.startDateTime, "1991-08-25 20:57:08 GMT+0000")

        eventOccurrence.endDateTime = "1995-05-20 11:22:33 GMT+0200"
        compare(eventOccurrence.endDateTime, "1995-05-20 11:22:33 GMT+0200")

        eventOccurrence.priority = Priority.VeryHigh
        compare(eventOccurrence.priority, Priority.VeryHigh)

        eventOccurrence.location = "Tampere"
        compare(eventOccurrence.location, "Tampere")

        eventOccurrence.parentId = event.itemId
        compare(eventOccurrence.parentId, event.itemId)
    }

    function test_event() {
        compare(event.itemType, Type.Event)

        event.allDay = true;
        compare(event.allDay, true)

        event.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(event.startDateTime, "1991-08-25 20:57:08 GMT+0000")

        event.endDateTime = "1995-05-20 11:22:33 GMT+0200"
        compare(event.endDateTime, "1995-05-20 11:22:33 GMT+0200")

        event.priority = Priority.VeryHigh
        compare(event.priority, Priority.VeryHigh)

        event.location = "Tampere"
        compare(event.location, "Tampere")
    }

    function test_item() {
        // empty OrganizerItem
        compare(item.modified, false)

        // access Description
        item.description = "Qt Open Governance"
        compare(item.description, "Qt Open Governance")
        compare(item.detail(Detail.Description).description, "Qt Open Governance")
        compare(item.details(Detail.Description).length, 1)

        // access DisplayLabel
        item.displayLabel = "http://qt-project.org/"
        compare(item.displayLabel, "http://qt-project.org/")
        compare(item.detail(Detail.DisplayLabel).label, "http://qt-project.org/")
        compare(item.details(Detail.DisplayLabel).length, 1)

        // access Guid
        item.guid = "b7dd2d61-fcb3-1170-e314-899ac5e91c7c"
        compare(item.guid, "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
        compare(item.detail(Detail.Guid).guid, "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
        compare(item.details(Detail.Guid).length, 1)

        // add new Detail
        item.setDetail(comment)
        compare(item.detail(Detail.Comment).comment)
        compare(item.details(Detail.Comment).length)
        compare(item.itemDetails.length)

        // update existing Detail
        comment.comment = "Create More"
        item.setDetail(comment)
        compare(item.detail(Detail.Comment).comment, "Create More")
        compare(item.details(Detail.Comment).length, 1)

        // delete one existing Detail
        item.removeDetail(comment)
        compare(item.details(Detail.Comment).length, 0)

        // remove all existing Details
        compare(item.itemDetails.length, 0)
    }
}
