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
    name: "ItemDetailTests"

    Detail {
        id: emptyDetail
    }

    EventTime {
        id: eventTime
    }

    Comment {
        id: comment
    }

    Description {
        id: description
    }

    DisplayLabel {
        id: displayLabel
    }

    Guid {
        id: guid
    }

    Location {
        id: location
    }

    Parent {
        id: parent
    }

    Priority {
        id: priority
    }

    Tag {
        id: tag
    }

    Timestamp {
        id: timestamp
    }

    Type {
        id: type
    }

    JournalTime {
        id: journalTime
    }

    TodoProgress {
        id: todoProgress
    }

    TodoTime {
        id: todoTime
    }

    Reminder {
        id: reminder
    }

    AudibleReminder {
        id: audibleReminder
    }

    EmailReminder {
        id: emailReminder
    }

    VisualReminder {
        id: visualReminder
    }

    Recurrence {
        id: recurrence
    }

    function test_recurrence() {
    }

    function test_visualReminder() {
        compare(visualReminder.type, Detail.VisualReminder)
        compare(visualReminder.reminderType, Reminder.VisualReminder)
        compare(visualReminder.repetitionCount, 0)
        compare(visualReminder.repetitionDelay, 0)
        compare(visualReminder.secondsBeforeStart, 0)
        compare(visualReminder.message, "")
        compare(visualReminder.dataUrl, "")

        visualReminder.repetitionCount = 19
        compare(visualReminder.repetitionCount, 19)

        visualReminder.repetitionDelay = 89
        compare(visualReminder.repetitionDelay, 89)

        visualReminder.secondsBeforeStart = 64
        compare(visualReminder.secondsBeforeStart, 64)

        visualReminder.message = "Qt Open Governance"
        compare(visualReminder.message, "Qt Open Governance")

        visualReminder.dataUrl = "http://qt-project.org/"
        compare(visualReminder.dataUrl, "http://qt-project.org/")
    }

    function test_emailReminder() {
        compare(emailReminder.type, Detail.EmailReminder)
        compare(emailReminder.reminderType, Reminder.EmailReminder)
        compare(emailReminder.repetitionCount, 0)
        compare(emailReminder.repetitionDelay, 0)
        compare(emailReminder.secondsBeforeStart, 0)
        compare(emailReminder.body, "")
        compare(emailReminder.subject, "")
        compare(emailReminder.recipients.length, 0)
        compare(emailReminder.attachments. length, 0)

        emailReminder.repetitionCount = 19
        compare(emailReminder.repetitionCount, 19)

        emailReminder.repetitionDelay = 89
        compare(emailReminder.repetitionDelay, 89)

        emailReminder.secondsBeforeStart = 64
        compare(emailReminder.secondsBeforeStart, 64)

        emailReminder.body = "Qt - Cross-platform application and UI framework"
        compare(emailReminder.body, "Qt - Cross-platform application and UI framework")

        emailReminder.subject = "Qt!"
        compare(emailReminder.subject, "Qt!")

        emailReminder.recipients = ["Berlin", "Brisbane", "Oslo", "Tampere"]
        compare(emailReminder.recipients.length, 3)

        emailReminder.attachments = [1, 2, "345"]
        compare(emailReminder.attachments.length, 3)
    }

    function test_audibleReminder() {
        compare(audibleReminder.type, Detail.AudibleReminder)
        compare(audibleReminder.reminderType, Reminder.AudibleReminder)
        compare(audibleReminder.repetitionCount, 0)
        compare(audibleReminder.repetitionDelay, 0)
        compare(audibleReminder.secondsBeforeStart, 0)
        compare(audibleReminder.dataUrl, "")

        audibleReminder.repetitionCount = 19
        compare(audibleReminder.repetitionCount, 19)

        audibleReminder.repetitionDelay = 89
        compare(audibleReminder.repetitionDelay, 89)

        audibleReminder.secondsBeforeStart = 64
        compare(audibleReminder.secondsBeforeStart, 64)

        audibleReminder.dataUrl = "http://qt-project.org/"
        compare(audibleReminder.dataUrl, "http://qt-project.org/")
    }

    function test_reminder() {
        compare(reminder.type, Detail.Reminder)
        compare(reminder.reminderType, Reminder.NoReminder)
        compare(reminder.repetitionCount, 0)
        compare(reminder.repetitionDelay, 0)
        compare(reminder.secondsBeforeStart, 0)

        reminder.repetitionCount = 19
        compare(reminder.repetitionCount, 19)

        reminder.repetitionDelay = 89
        compare(reminder.repetitionDelay, 89)

        reminder.secondsBeforeStart = 64
        compare(reminder.secondsBeforeStart, 64)
    }

    function test_todoTime() {
        compare(todoTime.type, Detail.TodoTime)
        compare(todoTime.allDay, false)

        todoTime.allDay = true
        compare(todoTime.allDay, true)

        todoTime.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(todoTime.startDateTime, "1991-08-25 20:57:08 GMT+0000")

        todoTime.dueDateTime = "1995-05-20 11:22:33 GMT+0200"
        compare(todoTime.dueDateTime, "1995-05-20 11:22:33 GMT+0200")
    }

    function test_todoProgress() {
        compare(todoProgress.type, Detail.TodoProgress)

        todoProgress.finishedDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(todoProgress.finishedDateTime, "1991-08-25 20:57:08 GMT+0000")

        todoProgress.percentage = 64
        compare(todoProgress.percentage, 64)

        todoProgress.status = TodoProgress.InProgress
        compare(todoProgress.status, TodoProgress.InProgress)
    }

    function test_journalTime() {
        compare(journalTime.type, Detail.JournalTime)

        journalTime.entryDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(journalTime.entryDateTime, "1991-08-25 20:57:08 GMT+0000")
    }

    function test_type() {
        compare(type.type, Detail.Type)

        type.itemType = Type.TodoOccurrence
        compare(type.itemType, Type.TodoOccurrence)
    }

    function test_timestamp() {
        compare(timestamp.type, Detail.Timestamp)

        timestamp.created = "1991-08-25 20:57:08 GMT+0000"
        compare(timestamp.created, "1991-08-25 20:57:08 GMT+0000")

        timestamp.lastModified = "1995-05-20 11:22:33 GMT+0200"
        compare(timestamp.lastModified, "1995-05-20 11:22:33 GMT+0200")
    }

    function test_tag() {
        compare(tag.type, Detail.Tag)

        tag.tag = "Qt"
        compare(tag.tag, "Qt")
    }

    function test_priority() {
        compare(priority.type, Detail.Priority)

        priority.priority = Priority.Medium
        compare(priority.priority, Priority.Medium)
    }

    function test_parent() {
        compare(parent.type, Detail.Parent)

        parent.originalDate = "2008-12-28"
        compare(parent.originalDate, "2008-12-28")
    }

    function test_location() {
        compare(location.type, Detail.Location)

        location.latitude = 1.2
        compare(location.latitude, 1.2)

        location.longitude = 3.4
        compare(location.longitude, 3.4)

        location.label = "Zion"
        compare(location.label, "Zion")
    }

    function test_guid() {
        compare(guid.type, Detail.Guid)

        guid.guid = "b7dd2d61-fcb3-1170-e314-899ac5e91c7c"
        compare(guid.guid, "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
    }

    function test_displayLabel() {
        compare(displayLabel.type, Detail.DisplayLabel)

        displayLabel.label = "Deploy Everywhere"
        compare(displayLabel.label, "Deploy Everywhere")
    }

    function test_description() {
        compare(description.type, Detail.Description)

        description.description = "Create More"
        compare(description.description, "Create More")
    }

    function test_comment() {
        compare(comment.type, Detail.Comment)

        comment.comment = "Code Less"
        compare(comment.comment, "Code Less")
    }

    function test_eventTime() {
        compare(eventTime.type, Detail.EventTime)

        eventTime.isAllDay = true
        compare(eventTime.isAllDay, true)

        eventTime.startDateTime = "1991-08-25 20:57:08 GMT+0000"
        compare(eventTime.startDateTime, "1991-08-25 20:57:08 GMT+0000")

        eventTime.endDateTime = "1995-05-20 11:22:33 GMT+0200"
        compare(eventTime.endDateTime, "1995-05-20 11:22:33 GMT+0200")
    }

    function test_emptyDetail() {
        compare(emptyDetail.readOnly, false)
        compare(emptyDetail.removable, true)
    }
}
