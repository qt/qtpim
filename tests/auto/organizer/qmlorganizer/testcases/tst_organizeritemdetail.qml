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
    name: "ItemDetailTests"
    id: organizerItemDetailTests
    property int waitTime : 200

    QOrganizerTestUtility {
        id: utility
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

    Timestamp {//id "timestamp" conflict something
        id: timestamp1
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

    ExtendedDetail {
        id: extendedDetail
    }

    EventAttendee {
        id: eventAttendee
    }

    EventRsvp {
        id: eventRsvp
    }

    Classification {
        id: classification
    }

    Version {
        id: version
    }

    function test_version() {
        compare(version.type, Detail.Version)

        compare(version.version, 0)
        compare(version.value(Version.FieldVersion), undefined)
        version.version = 64
        compare(version.version, 64)
        compare(version.value(Version.FieldVersion), 64)

        compare(version.extendedVersion, "")
        compare(version.value(Version.FieldExtendedVersion), undefined)
        version.extendedVersion = "Qt rules"
        compare(version.extendedVersion, "Qt rules")
        compare(version.value(Version.FieldExtendedVersion), "Qt rules")
    }

    function test_extendedDetail() {
        compare(extendedDetail.type, Detail.ExtendedDetail)

        compare(extendedDetail.name, "")
        compare(extendedDetail.value(ExtendedDetail.FieldName), undefined)
        extendedDetail.name = "Qt"
        compare(extendedDetail.name, "Qt")
        compare(extendedDetail.value(ExtendedDetail.FieldName), "Qt")

        compare(extendedDetail.data, undefined)
        compare(extendedDetail.value(ExtendedDetail.FieldData), undefined)
        extendedDetail.data = "Everywhere"
        compare(extendedDetail.data, "Everywhere");
        compare(extendedDetail.value(ExtendedDetail.FieldData), "Everywhere")
    }

    function test_recurrence() {
    }

    function test_recurrenceDateArrays_data() {
        return [
            {tag: "basic date object", testValue: [new Date(2012, 2, 16, 11, 00, 00)]},
            {tag: "date object from string", testValue: [new Date("January 1, 1986")], expectedValue: ['1986-01-01']},
            {tag: "date object from ISO date", testValue: [new Date('2014-01-01')]},
            {tag: "datetime object from string", testValue: [new Date("October 13, 1975 11:13:00")]},
            {tag: "date string at year boundary lower", testValue: ['2013-01-01']},
            {tag: "date string at year boundary upper", testValue: ['2013-12-31']},
            {tag: "datetime string", testValue: ['2013-10-23T23:55:00']},
            {tag: "datetime string ISO 8601   Z", testValue: ['1997-07-16T19:20:30.45Z']},
            {tag: "datetime string ISO 8601 +01", testValue: ['1997-07-16T19:20:30.45+01:00']},
            {tag: "datetime string ISO 8601 +10", testValue: ['1997-07-16T19:20:30.45+10:00']},
            {tag: "datetime string ISO 8601 -01", testValue: ['1997-07-16T19:20:30.45-01:00']},
            {tag: "datetime string ISO 8601 -10", testValue: ['1997-07-16T19:20:30.45-10:00']},
            {tag: "datetime string ISO 8601 +10 at boundary", testValue: ['1997-01-01T00:00:01.45+10:00']},
            {tag: "datetime string ISO 8601 -10 at boundary", testValue: ['1997-12-31T23:59:59.45-10:00']}
        ]
    }
    function test_recurrenceDateArrays(data) {

        console.log()//print the separate cases
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests)
        var tempRecurrence = utility.create_testobject("import QtOrganizer 5.0; Recurrence{}", organizerItemDetailTests)
        detailChangedSpy.target = tempRecurrence;
        detailChangedSpy.signalName = "detailChanged"

        var testDate = (typeof data.testValue[0] == 'string') ? new Date(data.testValue[0]) : data.testValue[0];
        var expectedDate = (data.hasOwnProperty('expectedValue') && typeof data.expectedValue[0] == 'string') ? new Date(data.expectedValue[0]) : testDate;
        var testDateUTCMidnight = utility.toUTCMidnight(expectedDate);

        if (isNaN(testDate.getTime())) {
            warn("test \"" + data.tag + "\" contains incorrect date");
            return;
        }

        // recurrenceDates
        tempRecurrence.recurrenceDates = data.testValue
        detailChangedSpy.wait(waitTime)
        compare(tempRecurrence.recurrenceDates[0], testDateUTCMidnight)
        var rdates = tempRecurrence.value(Recurrence.FieldRecurrenceDates);
        compare(rdates[0], testDateUTCMidnight);
        compare(detailChangedSpy.count, 1)

        // exceptionDates
        tempRecurrence.setValue(Recurrence.FieldExceptionDates, data.testValue)
        detailChangedSpy.wait(waitTime)
        compare(tempRecurrence.exceptionDates[0], testDateUTCMidnight)
        var edates = tempRecurrence.value(Recurrence.FieldExceptionDates);
        compare(edates[0], testDateUTCMidnight);
        compare(detailChangedSpy.count, 2)
    }


    function test_visualReminder() {
        compare(visualReminder.type, Detail.VisualReminder)
        compare(visualReminder.reminderType, Reminder.VisualReminder)

        compare(visualReminder.repetitionCount, 0)
        compare(visualReminder.value(VisualReminder.FieldRepetitionCount), undefined)
        compare(visualReminder.repetitionDelay, 0)
        compare(visualReminder.value(VisualReminder.FieldRepetitionDelay), undefined)
        compare(visualReminder.secondsBeforeStart, 0)
        compare(visualReminder.value(VisualReminder.FieldSecondsBeforeStart), undefined)
        compare(visualReminder.message, "")
        compare(visualReminder.value(VisualReminder.FieldMessage), undefined)
        compare(visualReminder.dataUrl, "")
        compare(visualReminder.value(VisualReminder.FieldDataUrl), undefined)

        visualReminder.repetitionCount = 19
        compare(visualReminder.repetitionCount, 19)
        compare(visualReminder.value(VisualReminder.FieldRepetitionCount), 19)

        visualReminder.repetitionDelay = 89
        compare(visualReminder.repetitionDelay, 89)
        compare(visualReminder.value(VisualReminder.FieldRepetitionDelay), 89)

        visualReminder.secondsBeforeStart = 64
        compare(visualReminder.secondsBeforeStart, 64)
        compare(visualReminder.value(VisualReminder.FieldSecondsBeforeStart), 64)

        visualReminder.message = "Qt Open Governance"
        compare(visualReminder.message, "Qt Open Governance")
        compare(visualReminder.value(VisualReminder.FieldMessage), "Qt Open Governance")

        visualReminder.dataUrl = "http://qt-project.org/"
        compare(visualReminder.dataUrl, "http://qt-project.org/")
        compare(visualReminder.value(VisualReminder.FieldDataUrl), "http://qt-project.org/")
    }

    function test_emailReminder() {
        compare(emailReminder.type, Detail.EmailReminder)
        compare(emailReminder.reminderType, Reminder.EmailReminder)

        compare(emailReminder.repetitionCount, 0)
        compare(emailReminder.value(EmailReminder.FieldRepetitionCount), undefined)
        compare(emailReminder.repetitionDelay, 0)
        compare(emailReminder.value(EmailReminder.FieldRepetitionDelay), undefined)
        compare(emailReminder.secondsBeforeStart, 0)
        compare(emailReminder.value(EmailReminder.FieldSecondsBeforeStart), undefined)
        compare(emailReminder.body, "")
        compare(emailReminder.value(EmailReminder.FieldBody), undefined)
        compare(emailReminder.subject, "")
        compare(emailReminder.value(EmailReminder.FieldSubject), undefined)
        compare(emailReminder.recipients.length, 0)
        compare(emailReminder.value(EmailReminder.FieldRecipients), undefined)
        compare(emailReminder.attachments. length, 0)
        compare(emailReminder.value(EmailReminder.FieldAttachments), undefined)

        emailReminder.repetitionCount = 19
        compare(emailReminder.repetitionCount, 19)
        compare(emailReminder.value(EmailReminder.FieldRepetitionCount), 19)

        emailReminder.repetitionDelay = 89
        compare(emailReminder.repetitionDelay, 89)
        compare(emailReminder.value(EmailReminder.FieldRepetitionDelay), 89)

        emailReminder.secondsBeforeStart = 64
        compare(emailReminder.secondsBeforeStart, 64)
        compare(emailReminder.value(EmailReminder.FieldSecondsBeforeStart), 64)

        emailReminder.body = "Qt - Cross-platform application and UI framework"
        compare(emailReminder.body, "Qt - Cross-platform application and UI framework")
        compare(emailReminder.value(EmailReminder.FieldBody), "Qt - Cross-platform application and UI framework")

        emailReminder.subject = "Qt!"
        compare(emailReminder.subject, "Qt!")
        compare(emailReminder.value(EmailReminder.FieldSubject), "Qt!")

        emailReminder.recipients = ["Berlin", "Brisbane", "Oslo", "Tampere"]
        compare(emailReminder.recipients, ["Berlin", "Brisbane", "Oslo", "Tampere"])
        compare(emailReminder.value(EmailReminder.FieldRecipients), ["Berlin", "Brisbane", "Oslo", "Tampere"])

        emailReminder.attachments = [1, 2, "345"]
        compare(emailReminder.attachments, [1, 2, "345"])
        compare(emailReminder.value(EmailReminder.FieldAttachments), [1, 2, "345"])
    }

    function test_audibleReminder() {
        compare(audibleReminder.type, Detail.AudibleReminder)
        compare(audibleReminder.reminderType, Reminder.AudibleReminder)

        compare(audibleReminder.repetitionCount, 0)
        compare(audibleReminder.value(AudibleReminder.FieldRepetitionCount), undefined)
        compare(audibleReminder.repetitionDelay, 0)
        compare(audibleReminder.value(AudibleReminder.FieldRepetitionDelay), undefined)
        compare(audibleReminder.secondsBeforeStart, 0)
        compare(audibleReminder.value(AudibleReminder.FieldSecondsBeforeStart), undefined)
        compare(audibleReminder.dataUrl, "")
        compare(audibleReminder.value(AudibleReminder.FieldDataUrl), undefined)

        audibleReminder.repetitionCount = 19
        compare(audibleReminder.repetitionCount, 19)
        compare(audibleReminder.value(AudibleReminder.FieldRepetitionCount), 19)

        audibleReminder.repetitionDelay = 89
        compare(audibleReminder.repetitionDelay, 89)
        compare(audibleReminder.value(AudibleReminder.FieldRepetitionDelay), 89)

        audibleReminder.secondsBeforeStart = 64
        compare(audibleReminder.secondsBeforeStart, 64)
        compare(audibleReminder.value(AudibleReminder.FieldSecondsBeforeStart), 64)

        audibleReminder.dataUrl = "http://qt-project.org/"
        compare(audibleReminder.dataUrl, "http://qt-project.org/")
        compare(audibleReminder.value(AudibleReminder.FieldDataUrl), "http://qt-project.org/")
    }

    function test_reminder() {
        compare(reminder.type, Detail.Reminder)
        compare(reminder.reminderType, Reminder.NoReminder)

        compare(reminder.repetitionCount, 0)
        compare(reminder.value(Reminder.FieldRepetitionCount), undefined)
        compare(reminder.repetitionDelay, 0)
        compare(reminder.value(Reminder.FieldRepetitionDelay), undefined)
        compare(reminder.secondsBeforeStart, 0)
        compare(reminder.value(Reminder.FieldSecondsBeforeStart), undefined)

        reminder.repetitionCount = 19
        compare(reminder.repetitionCount, 19)
        compare(reminder.value(Reminder.FieldRepetitionCount), 19)

        reminder.repetitionDelay = 89
        compare(reminder.repetitionDelay, 89)
        compare(reminder.value(Reminder.FieldRepetitionDelay), 89)

        reminder.secondsBeforeStart = 64
        compare(reminder.secondsBeforeStart, 64)
        compare(reminder.value(Reminder.FieldSecondsBeforeStart), 64)
    }

    function test_todoTime() {
        compare(todoTime.type, Detail.TodoTime)

        compare(todoTime.allDay, false)
        compare(todoTime.value(TodoTime.FieldAllDay), undefined)
        todoTime.allDay = true
        compare(todoTime.allDay, true)
        compare(todoTime.value(TodoTime.FieldAllDay), true)

        compare(todoTime.value(TodoTime.FieldStartDateTime), undefined)
        var startDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        todoTime.startDateTime = startDateTime
        compare(todoTime.startDateTime, startDateTime)
        compare(todoTime.value(TodoTime.FieldStartDateTime), startDateTime)

        compare(todoTime.value(TodoTime.FieldDueDateTime), undefined)
        var dueDateTime = new Date("1995-05-20 11:22:33 GMT+0200")
        todoTime.dueDateTime = dueDateTime
        compare(todoTime.dueDateTime, dueDateTime)
        compare(todoTime.value(TodoTime.FieldDueDateTime), dueDateTime)
    }

    function test_todoProgress() {
        compare(todoProgress.type, Detail.TodoProgress)

        compare(todoProgress.value(TodoProgress.FieldFinishedDateTime), undefined)
        var finishedDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        todoProgress.finishedDateTime = finishedDateTime
        compare(todoProgress.finishedDateTime, finishedDateTime)
        compare(todoProgress.value(TodoProgress.FieldFinishedDateTime), finishedDateTime)

        compare(todoProgress.value(TodoProgress.FieldPercentageComplete), undefined)
        todoProgress.percentageComplete = 64
        compare(todoProgress.percentageComplete, 64)
        compare(todoProgress.value(TodoProgress.FieldPercentageComplete), 64)

        compare(todoProgress.value(TodoProgress.FieldStatus), undefined)
        todoProgress.status = TodoProgress.InProgress
        compare(todoProgress.status, TodoProgress.InProgress)
        compare(todoProgress.value(TodoProgress.FieldStatus), TodoProgress.InProgress)
    }

    function test_journalTime() {
        compare(journalTime.type, Detail.JournalTime)

        compare(journalTime.value(JournalTime.FieldEntryDateTime), undefined)
        var entryDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        journalTime.entryDateTime = entryDateTime
        compare(journalTime.entryDateTime, entryDateTime)
        compare(journalTime.value(JournalTime.FieldEntryDateTime), journalTime.entryDateTime)
    }

    function test_type() {
        compare(type.type, Detail.ItemType)

        compare(type.value(Type.FieldType), undefined)
        type.itemType = Type.TodoOccurrence
        compare(type.itemType, Type.TodoOccurrence)
    }

    function test_timestamp() {
        compare(timestamp1.type, Detail.Timestamp)

        compare(timestamp1.value(Timestamp.FieldCreated), undefined)
        var timestamp = new Date("1991-08-25 20:57:08 GMT+0000")
        timestamp.created = timestamp
        compare(timestamp.created, timestamp)

        compare(timestamp1.value(Timestamp.FieldLastModified), undefined)
        var lastModified = new Date("1995-05-20 11:22:33 GMT+0200")
        timestamp.lastModified = lastModified
        compare(timestamp.lastModified, lastModified)
    }

    function test_tag() {
        compare(tag.type, Detail.Tag)

        compare(tag.value(Tag.FieldTag), undefined)
        tag.tag = "Qt"
        compare(tag.tag, "Qt")
        compare(tag.value(Tag.FieldTag), "Qt")
    }

    function test_priority() {
        compare(priority.type, Detail.Priority)

        compare(priority.value(Priority.FieldPriority), undefined)
        priority.priority = Priority.Medium
        compare(priority.priority, Priority.Medium)
        compare(priority.value(Priority.FieldPriority), Priority.Medium)
    }

    function test_parent() {
        compare(parent.type, Detail.Parent)

        compare(parent.value(Parent.FieldOriginalDate), undefined)
        var originalDate = new Date("2008-12-28")
        parent.originalDate = originalDate
        compare(parent.originalDate, originalDate)
        compare(parent.value(Parent.FieldOriginalDate), originalDate)

        var originalDate2 = new Date("2008-01-01")
        var originalDate2UTC = utility.toUTCMidnight(originalDate2)
        parent.setValue(Parent.FieldOriginalDate, originalDate2)
        compare(parent.originalDate, originalDate2UTC)
        compare(parent.value(Parent.FieldOriginalDate), originalDate2UTC)

        parent.setValue(Parent.FieldOriginalDate, "2008-01-01")
        compare(parent.originalDate, originalDate2UTC)
        compare(parent.value(Parent.FieldOriginalDate), originalDate2UTC)

        var originalDate3 = new Date("January 1, 2008")
        parent.setValue(Parent.FieldOriginalDate, originalDate3)
        compare(parent.originalDate, originalDate2UTC)
        compare(parent.value(Parent.FieldOriginalDate), originalDate2UTC)
    }

    function test_location() {
        compare(location.type, Detail.Location)

        compare(location.value(Location.FieldLatitude), undefined)
        location.latitude = 1.2
        compare(location.latitude, 1.2)
        compare(location.value(Location.FieldLatitude), 1.2)

        compare(location.value(Location.FieldLongitude), undefined)
        location.longitude = 3.4
        compare(location.longitude, 3.4)
        compare(location.value(Location.FieldLongitude), 3.4)

        compare(location.value(Location.FieldLabel), undefined)
        location.label = "Zion"
        compare(location.label, "Zion")
        compare(location.value(Location.FieldLabel), "Zion")
    }

    function test_guid() {
        compare(guid.type, Detail.Guid)

        compare(guid.value(Guid.FieldGuid), undefined)
        guid.guid = "b7dd2d61-fcb3-1170-e314-899ac5e91c7c"
        compare(guid.guid, "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
        compare(guid.value(Guid.FieldGuid), "b7dd2d61-fcb3-1170-e314-899ac5e91c7c")
    }

    function test_displayLabel() {
        compare(displayLabel.type, Detail.DisplayLabel)

        compare(displayLabel.value(DisplayLabel.FieldLabel), undefined)
        displayLabel.label = "Deploy Everywhere"
        compare(displayLabel.label, "Deploy Everywhere")
        compare(displayLabel.value(DisplayLabel.FieldLabel), "Deploy Everywhere")
    }

    function test_description() {
        compare(description.type, Detail.Description)

        compare(description.value(Description.FieldDescription), undefined)
        description.description = "Create More"
        compare(description.description, "Create More")
        compare(description.value(Description.FieldDescription), "Create More")
    }

    function test_comment() {
        compare(comment.type, Detail.Comment)

        compare(comment.value(Comment.FieldComment), undefined)
        comment.comment = "Code Less"
        compare(comment.comment, "Code Less")
        compare(comment.value(Comment.FieldComment), "Code Less")
    }

    function test_eventTime() {
        compare(eventTime.type, Detail.EventTime)

        compare(eventTime.value(EventTime.FieldAllDay), undefined)
        eventTime.allDay = true
        compare(eventTime.allDay, true)
        compare(eventTime.value(EventTime.FieldAllDay), true)

        compare(eventTime.value(EventTime.FieldStartDateTime), undefined)
        var startDateTime = new Date("1991-08-25 20:57:08 GMT+0000")
        eventTime.startDateTime = startDateTime
        compare(eventTime.startDateTime, startDateTime)
        compare(eventTime.value(EventTime.FieldStartDateTime), startDateTime)

        compare(eventTime.value(EventTime.FieldEndDateTime), undefined)
        var endDateTime = new Date("1995-05-20 11:22:33 GMT+0200")
        eventTime.endDateTime = endDateTime
        compare(eventTime.endDateTime, endDateTime)
        compare(eventTime.value(EventTime.FieldEndDateTime), endDateTime)
    }

    function test_attendeeDetail() {
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;"
                 + "SignalSpy {}"
                 , organizerItemDetailTests);
        detailChangedSpy.target = eventAttendee;
        detailChangedSpy.signalName = "detailChanged";
        var count = 0;

        compare(eventAttendee.type, Detail.EventAttendee)
        eventAttendee.name = "new attendee"
        detailChangedSpy.wait(waitTime)
        compare(eventAttendee.name, "new attendee")
        compare(detailChangedSpy.count, ++count)

        eventAttendee.emailAddress = "new.attendee@qt.com"
        detailChangedSpy.wait(waitTime)
        compare(eventAttendee.emailAddress, "new.attendee@qt.com")
        compare(detailChangedSpy.count, ++count)

        eventAttendee.attendeeId = "123444455555"
        detailChangedSpy.wait(waitTime)
        compare(eventAttendee.attendeeId, "123444455555")
        compare(detailChangedSpy.count, ++count)

        eventAttendee.participationStatus = EventAttendee.StatusAccepted
        detailChangedSpy.wait(waitTime)
        compare(eventAttendee.participationStatus, EventAttendee.StatusAccepted)
        compare(detailChangedSpy.count, ++count)

        eventAttendee.participationRole = EventAttendee.RoleRequiredParticipant
        detailChangedSpy.wait(waitTime)
        compare(eventAttendee.participationRole, EventAttendee.RoleRequiredParticipant)
        compare(detailChangedSpy.count, ++count)

        // set same value
        eventAttendee.participationRole = EventAttendee.RoleRequiredParticipant
        compare(eventAttendee.participationRole, EventAttendee.RoleRequiredParticipant)
        // no signal has been emited
        compare(detailChangedSpy.count, count)
    }

    function test_rsvpGeneralTesting() {
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;"
                 + "SignalSpy {}"
                 , organizerItemDetailTests);
        detailChangedSpy.target = eventRsvp;
        detailChangedSpy.signalName = "detailChanged";
        var count = 0;

        compare(eventRsvp.type, Detail.EventRsvp)

        // default value checks
        compare(eventRsvp.organizerName, "")
        compare(eventRsvp.organizerEmail, "")
        compare(eventRsvp.participationStatus, EventAttendee.StatusUnknown)
        compare(eventRsvp.participationRole, EventAttendee.RoleUnknown)
        compare(eventRsvp.responseRequirement, EventRsvp.ResponseNotRequired)

        // no change tests are here to minimize the delays of testing
        // organizerName - no change on value
        eventRsvp.organizerName = eventRsvp.organizerName
        compare(eventRsvp.organizerName, eventRsvp.organizerName)
        compare(detailChangedSpy.count, count)
        // organizerEmail - no change on value
        eventRsvp.organizerEmail = eventRsvp.organizerEmail
        compare(eventRsvp.organizerEmail, eventRsvp.organizerEmail)
        compare(detailChangedSpy.count, count)
        // participationStatus - no change on value
        eventRsvp.participationStatus = eventRsvp.participationStatus
        compare(eventRsvp.participationStatus, eventRsvp.participationStatus)
        compare(detailChangedSpy.count, count)
        // participationRole - no change on value
        eventRsvp.participationRole = eventRsvp.participationRole
        compare(eventRsvp.participationRole, eventRsvp.participationRole)
        compare(detailChangedSpy.count, count)
        // responseRequirement - no change on value
        eventRsvp.responseRequirement = eventRsvp.responseRequirement
        compare(eventRsvp.responseRequirement, eventRsvp.responseRequirement)
        compare(detailChangedSpy.count, count)
    }

    function test_rsvpStringProperties_data() {
        return [
            {tag: "string", testValue: "short string"},
            {tag: "long string", testValue: "here goes the long string name but what to have here? Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed posuere interdum sem. Quisque ligula eros ullamcorper quis, lacinia quis facilisis sed sapien. Mauris varius diam vitae arcu. Sed arcu lectus auctor vitae, consectetuer et venenatis eget velit. Sed augue orci, lacinia eu tincidunt et eleifend nec lacus. Donec ultricies nisl ut felis, suspendisse potenti. Lorem ipsum ligula ut hendrerit mollis, ipsum erat vehicula risus, eu suscipit sem libero nec erat. Aliquam erat volutpat. Sed congue augue vitae neque. Nulla consectetuer porttitor pede. Fusce purus morbi tortor magna condimentum vel, placerat id blandit sit amet tortor. Mauris sed libero. Suspendisse facilisis nulla in lacinia laoreet, lorem velit accumsan velit vel mattis libero nisl et sem. Proin interdum maecenas massa turpis sagittis in, interdum non lobortis vitae massa. Quisque purus lectus, posuere eget imperdiet nec sodales id arcu. Vestibulum elit pede dictum eu, viverra non tincidunt eu ligula. Nam molestie nec tortor. Donec placerat leo sit amet velit. Vestibulum id justo ut vitae massa. Proin in dolor mauris consequat aliquam. Donec ipsum, vestibulum ullamcorper venenatis augue. Aliquam tempus nisi in auctor vulputate, erat felis pellentesque augue nec, pellentesque lectus justo nec erat. Aliquam et nisl. Quisque sit amet dolor in justo pretium condimentum. Vivamus placerat lacus vel vehicula scelerisque, dui enim adipiscing lacus sit amet sagittis, libero enim vitae mi. In neque magna posuere, euismod ac tincidunt tempor est. Ut suscipit nisi eu purus. Proin ut pede mauris eget ipsum. Integer vel quam nunc commodo consequat. Integer ac eros eu tellus dignissim viverra. Maecenas erat aliquam erat volutpat. Ut venenatis ipsum quis turpis. Integer cursus scelerisque lorem. Sed nec mauris id quam blandit consequat. Cras nibh mi hendrerit vitae, dapibus et aliquam et magna. Nulla vitae elit. Mauris consectetuer odio vitae augue."},
            {tag: "number", testValue: 123},
        ]
    }
    function test_rsvpStringProperties(data) {
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests)
        var tempEventRsvp = utility.create_testobject("import QtOrganizer 5.0; EventRsvp{}", organizerItemDetailTests)
        detailChangedSpy.target = tempEventRsvp
        detailChangedSpy.signalName = "detailChanged"

        // organizerName
        tempEventRsvp.organizerName = data.testValue
        detailChangedSpy.wait(waitTime)
        compare(tempEventRsvp.organizerName, data.testValue.toString())
        compare(detailChangedSpy.count, 1)

        // organizerEmail
        tempEventRsvp.organizerEmail = data.testValue
        detailChangedSpy.wait(waitTime)
        compare(tempEventRsvp.organizerEmail, data.testValue.toString())
        compare(detailChangedSpy.count, 2)
    }

    function test_rsvpDateProperties_data() {
        return [
            {tag: "basic date object", testValue: new Date(2012, 2, 16, 11, 00, 00)},
            {tag: "date object from string", testValue: new Date("January 1, 1986"), expectedValue: '1986-01-01'},
            {tag: "date object from ISO date", testValue: new Date('2014-01-01')},
            {tag: "datetime object from string", testValue: new Date("October 13, 1975 11:13:00")},
            {tag: "date string at year boundary lower", testValue: '2013-01-01'},
            {tag: "date string at year boundary upper", testValue: '2013-12-31'},
            {tag: "datetime string", testValue: '2013-10-23T23:55:00'},
            {tag: "datetime string ISO 8601   Z", testValue: '1997-07-16T19:20:30.45Z'},
            {tag: "datetime string ISO 8601 +01", testValue: '1997-07-16T19:20:30.45+01:00'},
            {tag: "datetime string ISO 8601 +10", testValue: '1997-07-16T19:20:30.45+10:00'},
            {tag: "datetime string ISO 8601 -01", testValue: '1997-07-16T19:20:30.45-01:00'},
            {tag: "datetime string ISO 8601 -10", testValue: '1997-07-16T19:20:30.45-10:00'},
            {tag: "datetime string ISO 8601 +10 at boundary", testValue: '1997-01-01T00:00:01.45+10:00'},
            {tag: "datetime string ISO 8601 -10 at boundary", testValue: '1997-12-31T23:59:59.45-10:00'}
        ]
    }
    function test_rsvpDateProperties(data) {

        console.log()//print the separate cases

        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests)
        var tempEventRsvp = utility.create_testobject("import QtOrganizer 5.0; EventRsvp{}", organizerItemDetailTests)
        detailChangedSpy.target = tempEventRsvp
        detailChangedSpy.signalName = "detailChanged"

        var testDate = (typeof data.testValue == 'string') ? new Date(data.testValue) : data.testValue;
        var expectedDate = (data.hasOwnProperty('expectedValue') && typeof data.expectedValue == 'string') ? new Date(data.expectedValue) : testDate;
        var testDateUTCMidnight = utility.toUTCMidnight(expectedDate);

        if (isNaN(testDate.getTime())) {
            warn("test \"" + data.tag + "\" contains incorrect date");
            return;
        }

        // responseDeadline
        tempEventRsvp.responseDeadline = data.testValue
        detailChangedSpy.wait(waitTime)
        compare(tempEventRsvp.value(EventRsvp.FieldResponseDeadline), testDateUTCMidnight);
        compare(tempEventRsvp.responseDeadline.toString(), testDateUTCMidnight.toString())
        compare(detailChangedSpy.count, 1)

        // responseDate
        tempEventRsvp.setValue(EventRsvp.FieldResponseDate, data.testValue);
        detailChangedSpy.wait(waitTime)
        compare(tempEventRsvp.responseDate.toString(), testDateUTCMidnight.toString())
        compare(tempEventRsvp.value(EventRsvp.FieldResponseDate), testDateUTCMidnight);
        compare(detailChangedSpy.count, 2)
    }

    function test_rsvpParticipationStatus_data() {
        return [
            {tag: "StatusUnknown", testValue: EventAttendee.StatusUnknown},
            {tag: "StatusAccepted", testValue: EventAttendee.StatusAccepted},
            {tag: "StatusDeclined", testValue: EventAttendee.StatusDeclined},
            {tag: "StatusTentative", testValue: EventAttendee.StatusTentative},
            {tag: "StatusDelegated", testValue: EventAttendee.StatusDelegated},
            {tag: "StatusInProcess", testValue: EventAttendee.StatusInProcess},
            {tag: "StatusCompleted", testValue: EventAttendee.StatusCompleted},
            {tag: "non existing - number", testValue: 123},
        ]
    }
    function test_rsvpParticipationStatus(data) {
        console.log()//print the separate cases
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests)
        var tempEventRsvp = utility.create_testobject("import QtOrganizer 5.0; EventRsvp{}", organizerItemDetailTests)
        detailChangedSpy.target = tempEventRsvp
        detailChangedSpy.signalName = "detailChanged"
        var defaultValue = EventAttendee.StatusUnknown == data.testValue ? true : false;

        tempEventRsvp.participationStatus = data.testValue
        if (!defaultValue)
            detailChangedSpy.wait(waitTime)
        compare(detailChangedSpy.count, defaultValue ? 0 : 1)
        compare(tempEventRsvp.participationStatus, data.testValue)
    }

    function test_rsvpParticipationRole_data() {
        return [
            {tag: "RoleUnknown", testValue: EventAttendee.RoleUnknown},
            {tag: "RoleOrganizer", testValue: EventAttendee.RoleOrganizer},
            {tag: "RoleChairperson", testValue: EventAttendee.RoleChairperson},
            {tag: "RoleHost", testValue: EventAttendee.RoleHost},
            {tag: "RoleRequiredParticipant", testValue: EventAttendee.RoleRequiredParticipant},
            {tag: "RoleOptionalParticipant", testValue: EventAttendee.RoleOptionalParticipant},
            {tag: "RoleNonParticipant", testValue: EventAttendee.RoleNonParticipant},
            {tag: "non existing - number", testValue: 123},
        ]
    }
    function test_rsvpParticipationRole(data) {
        console.log()//print the separate cases
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests)
        var tempEventRsvp = utility.create_testobject("import QtOrganizer 5.0; EventRsvp{}", organizerItemDetailTests)
        detailChangedSpy.target = tempEventRsvp
        detailChangedSpy.signalName = "detailChanged"
        var defaultValue = EventAttendee.RoleUnknown == data.testValue ? true : false;

        tempEventRsvp.participationRole = data.testValue
        if (!defaultValue)
            detailChangedSpy.wait(waitTime)
        compare(detailChangedSpy.count, defaultValue ? 0 : 1)
        compare(tempEventRsvp.participationRole, data.testValue)
    }

    function test_rsvpResponseRequirement_data() {
        return [
            {tag: "ResponseNotRequired", testValue: EventRsvp.ResponseNotRequired},
            {tag: "ResponseRequired", testValue: EventRsvp.ResponseRequired},
            {tag: "non existing - number", testValue: 123},
        ]
    }
    function test_rsvpResponseRequirement(data) {
        console.log()//print the separate cases
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests)
        var tempEventRsvp = utility.create_testobject("import QtOrganizer 5.0; EventRsvp{}", organizerItemDetailTests)
        detailChangedSpy.target = tempEventRsvp
        detailChangedSpy.signalName = "detailChanged"
        var defaultValue = EventRsvp.ResponseNotRequired == data.testValue ? true : false;

        tempEventRsvp.responseRequirement = data.testValue
        if (!defaultValue)
            detailChangedSpy.wait(waitTime)
        compare(detailChangedSpy.count, defaultValue ? 0 : 1)
        compare(tempEventRsvp.responseRequirement, data.testValue)
    }

    function test_classificationGeneralTesting() {
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests);
        detailChangedSpy.target = classification;
        detailChangedSpy.signalName = "detailChanged";
        compare(classification.type, Detail.Classification)

        // default value check
        compare(classification.classification, Classification.AccessPublic)

        // no change on value
        classification.classification = classification.classification

        compare(detailChangedSpy.count, 0)
    }

    function test_classification_data() {
        return [
            {tag: "AccessPrivate", testValue: Classification.AccessPrivate},
            {tag: "AccessConfidential", testValue: Classification.AccessConfidential}
        ]
    }
    function test_classification(data) {
        var detailChangedSpy = utility.create_testobject("import QtTest 1.0;SignalSpy{}", organizerItemDetailTests);
        var tempClassification = utility.create_testobject("import QtOrganizer 5.0; Classification{}", organizerItemDetailTests)
        detailChangedSpy.target = tempClassification;
        detailChangedSpy.signalName = "detailChanged";

        // classification
        tempClassification.classification = data.testValue
        detailChangedSpy.wait(waitTime)
        compare(tempClassification.classification, data.testValue)
        compare(detailChangedSpy.count, 1)
    }
}
