/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include <qorganizerjsondbstring.h>
QTORGANIZER_BEGIN_NAMESPACE

//Jsondb manager name
const QString QOrganizerJsonDbStr::ManagerName(QStringLiteral("qtorganizer:jsondb::"));
const QString QOrganizerJsonDbStr::ManagerUri(QStringLiteral("qtorganizer:jsondb:"));
const QString QOrganizerJsonDbStr::JsonDbEngineId(QStringLiteral("jsondb"));

const QString QOrganizerJsonDbStr::JsonDbSchemaPrefix(QStringLiteral("com.nokia.mp.organizer."));

//Organizer item string
const QString QOrganizerJsonDbStr::ItemName(QStringLiteral("displayName"));
const QString QOrganizerJsonDbStr::ItemCollectionId(QStringLiteral("collectionId"));
const QString QOrganizerJsonDbStr::ItemDescription(QStringLiteral("description"));
const QString QOrganizerJsonDbStr::ItemComments(QStringLiteral("comments"));
const QString QOrganizerJsonDbStr::ItemLocation(QStringLiteral("location"));
const QString QOrganizerJsonDbStr::ItemPriority(QStringLiteral("Priority"));
const QString QOrganizerJsonDbStr::ItemTags(QStringLiteral("tags"));
const QString QOrganizerJsonDbStr::ItemReminder(QStringLiteral("reminder"));
const QString QOrganizerJsonDbStr::ItemGuid(QStringLiteral("guid"));

//Location sub properties
const QString QOrganizerJsonDbStr::ItemLocationLabel(QStringLiteral("displayName"));
const QString QOrganizerJsonDbStr::ItemLocationGeo(QStringLiteral("geo"));
const QString QOrganizerJsonDbStr::ItemLocationGeoLatitude(QStringLiteral("latitude"));
const QString QOrganizerJsonDbStr::ItemLocationGeoLongitude(QStringLiteral("longitude"));

//Item reminder properties
const QString QOrganizerJsonDbStr::ItemReminderSecBeforeStart(QStringLiteral("secondsBeforeStart"));
const QString QOrganizerJsonDbStr::ItemReminderRepCount(QStringLiteral("repetitionCount"));
const QString QOrganizerJsonDbStr::ItemReminderRepDelay(QStringLiteral("repetitionDelay"));
const QString QOrganizerJsonDbStr::ItemReminderDataUrl(QStringLiteral("dataUrl"));

//Event item
const QString QOrganizerJsonDbStr::Event(QStringLiteral("com.nokia.mp.organizer.Event"));
const QString QOrganizerJsonDbStr::EventStartDateTime(QStringLiteral("startDateTime"));
const QString QOrganizerJsonDbStr::EventEndDateTime(QStringLiteral("endDateTime"));
const QString QOrganizerJsonDbStr::EventIsAllDay(QStringLiteral("isAllDay"));
const QString QOrganizerJsonDbStr::EventLocation(QStringLiteral(""));
const QString QOrganizerJsonDbStr::EventInviter(QStringLiteral(""));
const QString QOrganizerJsonDbStr::EventAttendees(QStringLiteral(""));

//To-do item
const QString QOrganizerJsonDbStr::Todo(QStringLiteral("com.nokia.mp.organizer.Todo"));
const QString QOrganizerJsonDbStr::TodoStartDateTime(QStringLiteral("startDateTime"));
const QString QOrganizerJsonDbStr::TodoDueDateTime(QStringLiteral("dueDateTime"));
const QString QOrganizerJsonDbStr::TodoIsAllDay(QStringLiteral("isAllDay"));

//Collection properties
const QString QOrganizerJsonDbStr::Collection(QStringLiteral("com.nokia.mp.organizer.Collection"));
const QString QOrganizerJsonDbStr::CollectionDefaultFlag(QStringLiteral("isDefaultCollection"));
const QString QOrganizerJsonDbStr::CollectionName(QStringLiteral("displayName"));
const QString QOrganizerJsonDbStr::CollectionDescription(QStringLiteral("description"));
const QString QOrganizerJsonDbStr::CollectionColor(QStringLiteral("color"));
const QString QOrganizerJsonDbStr::CollectionImage(QStringLiteral("image"));
const QString QOrganizerJsonDbStr::CollectionCustomFields(QStringLiteral("customFields"));
const QString QOrganizerJsonDbStr::DefaultCollectionName(QStringLiteral("defaultCollection"));

// Recurrence sub properties
const QString QOrganizerJsonDbStr::ItemRecurrenceDates(QStringLiteral("recurrenceDates"));
const QString QOrganizerJsonDbStr::ItemExceptioneDates(QStringLiteral("exceptionDates"));
const QString QOrganizerJsonDbStr::ItemExceptionRules(QStringLiteral("exceptionRules"));
const QString QOrganizerJsonDbStr::ItemRecurrenceRules(QStringLiteral("recurrenceRules"));


// Recurrence rule properties
const QString QOrganizerJsonDbStr::RuleFirstDayOfWeek(QStringLiteral("firstDayOfWeek"));
const QString QOrganizerJsonDbStr::RuleFrequency(QStringLiteral("frequency"));
const QString QOrganizerJsonDbStr::RuleInterval(QStringLiteral("interval"));
const QString QOrganizerJsonDbStr::RuleLimitCount(QStringLiteral("limitCount"));
const QString QOrganizerJsonDbStr::RuleLimitDate(QStringLiteral("limitDate"));
const QString QOrganizerJsonDbStr::RulePositions(QStringLiteral("positions"));
const QString QOrganizerJsonDbStr::RuleDaysOfWeek(QStringLiteral("daysOfWeek"));
const QString QOrganizerJsonDbStr::RuleDaysOfMonth(QStringLiteral("daysOfMonth"));
const QString QOrganizerJsonDbStr::RuleDaysOfYear(QStringLiteral("daysOfYear"));
const QString QOrganizerJsonDbStr::RuleWeeksOfYear(QStringLiteral("weeksOfYear"));
const QString QOrganizerJsonDbStr::RuleMonthsOfYear(QStringLiteral("monthsOfYear"));

// Attendee properties
const QString QOrganizerJsonDbStr::Attendee(QStringLiteral("attendee"));
const QString QOrganizerJsonDbStr::AttendeeName(QStringLiteral("name"));
const QString QOrganizerJsonDbStr::AttendeeEmailAddress(QStringLiteral("emailAddress"));
const QString QOrganizerJsonDbStr::AttendeeId(QStringLiteral("attendeeId"));
const QString QOrganizerJsonDbStr::AttendeeParticipationStatus(QStringLiteral("participationState"));
const QString QOrganizerJsonDbStr::AttendeeParticipationRole(QStringLiteral("participationRole"));

// Rsvp properties
const QString QOrganizerJsonDbStr::Rsvp(QStringLiteral("rsvp"));
const QString QOrganizerJsonDbStr::RsvpOrganizerName(QStringLiteral("organizerName"));
const QString QOrganizerJsonDbStr::RsvpOrganizerEmail(QStringLiteral("organizerEmail"));
const QString QOrganizerJsonDbStr::RsvpResponseRequirement(QStringLiteral("responseRequirement"));
const QString QOrganizerJsonDbStr::RsvpResponseDate(QStringLiteral("responseDate"));
const QString QOrganizerJsonDbStr::RsvpResponseDeadline(QStringLiteral("responseDeadline"));

// Alarm object schema
const QString QOrganizerJsonDbStr::Alarm(QStringLiteral("com.nokia.mt.alarm-daemon.Alarm"));
const QString QOrganizerJsonDbStr::AlarmISODate(QStringLiteral("isoDate"));
const QString QOrganizerJsonDbStr::AlarmSnoozeTime(QStringLiteral("snoozeTime"));
const QString QOrganizerJsonDbStr::AlarmSnoozeCount(QStringLiteral("snoozeCount"));
const QString QOrganizerJsonDbStr::AlarmIdentifier(QStringLiteral("identifier"));
const QString QOrganizerJsonDbStr::AlarmTitle(QStringLiteral("title"));
const QString QOrganizerJsonDbStr::AlarmDescription(QStringLiteral("description"));
const QString QOrganizerJsonDbStr::AlarmEventUuid(QStringLiteral("eventUuid"));
const QString QOrganizerJsonDbStr::AlarmCalenderIdentifier(QStringLiteral("com.nokia.mt.calendar"));

//warning string
const QString QOrganizerJsonDbStr::WarningMoreItemDelete(QStringLiteral("Items are delete from jsondb!!"));
const QString QOrganizerJsonDbStr::WarningCollectionMissing(QStringLiteral("No default collection! error code:"));
const QString QOrganizerJsonDbStr::WarningCollectionRemove(QStringLiteral(" Number of items deleted from jsondb is not equal to request !!"));
const QString QOrganizerJsonDbStr::WarningDefaultCollectionRemove(QStringLiteral("Default collection can not be removed!"));
const QString QOrganizerJsonDbStr::WarningDefaultCollectionChange(QStringLiteral("Default collection can not be changed!"));
const QString QOrganizerJsonDbStr::WarningDefaultCollectionCreate(QStringLiteral("Same name as default collection!"));

// Notification
const QString QOrganizerJsonDbStr::NotificationQuery(QStringLiteral("[?_type in [\"com.nokia.mp.organizer.Event\", \"com.nokia.mp.organizer.Todo\", \"com.nokia.mp.organizer.Collection\"]]"));

QTORGANIZER_END_NAMESPACE
