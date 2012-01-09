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
const QString &QOrganizerJsonDbStr::managerName()
{
    static const QString string(QStringLiteral("qtorganizer:jsondb::"));
    return string;
}

const QString &QOrganizerJsonDbStr::managerUri()
{
    static const QString string(QStringLiteral("qtorganizer:jsondb:"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbEngineId()
{
    static const QString string(QStringLiteral("jsondb"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbSchemaPrefix()
{
    static const QString string(QStringLiteral("com.nokia.mp.organizer."));
    return string;
}

// JsonDb strings
const QString &QOrganizerJsonDbStr::jsonDbUuid()
{
    static const QString string(QStringLiteral("_uuid"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbType()
{
    static const QString string(QStringLiteral("_type"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbData()
{
    static const QString string(QStringLiteral("data"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbCount()
{
    static const QString string(QStringLiteral("count"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbError()
{
    static const QString string(QStringLiteral("error"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbCode()
{
    static const QString string(QStringLiteral("code"));
    return string;
}

// JsonDb query string
const QString &QOrganizerJsonDbStr::jsonDbQueryAllItems()
{
    static const QString string(QStringLiteral("[?_type in [\"com.nokia.mp.organizer.Event\", \"com.nokia.mp.organizer.Todo\"]]"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbQueryAllCollections()
{
    static const QString string(QStringLiteral("[?_type=\"com.nokia.mp.organizer.Collection\"]"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbQueryCollectionIdsTemplate()
{
    static const QString string(QStringLiteral("[?collectionId in [%1]]"));
    return string;
}

const QString &QOrganizerJsonDbStr::jsonDbQueryUuidsTemplate()
{
    static const QString string(QStringLiteral("[?_uuid in [%1]]"));
    return string;
}

//Organizer item string
const QString &QOrganizerJsonDbStr::itemName()
{
    static const QString string(QStringLiteral("displayName"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemCollectionId()
{
    static const QString string(QStringLiteral("collectionId"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemDescription()
{
    static const QString string(QStringLiteral("description"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemComments()
{
    static const QString string(QStringLiteral("comments"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemLocation()
{
    static const QString string(QStringLiteral("location"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemPriority()
{
    static const QString string(QStringLiteral("Priority"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemTags()
{
    static const QString string(QStringLiteral("tags"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemGuid()
{
    static const QString string(QStringLiteral("guid"));
    return string;
}

//Location sub properties
const QString &QOrganizerJsonDbStr::itemLocationLabel()
{
    static const QString string(QStringLiteral("displayName"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemLocationGeo()
{
    static const QString string(QStringLiteral("geo"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemLocationGeoLatitude()
{
    static const QString string(QStringLiteral("latitude"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemLocationGeoLongitude()
{
    static const QString string(QStringLiteral("longitude"));
    return string;
}

//Item reminder properties
const QString &QOrganizerJsonDbStr::itemReminder()
{
    static const QString string(QStringLiteral("reminder"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemReminderSecBeforeStart()
{
    static const QString string(QStringLiteral("secondsBeforeStart"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemReminderRepCount()
{
    static const QString string(QStringLiteral("repetitionCount"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemReminderRepDelay()
{
    static const QString string(QStringLiteral("repetitionDelay"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemReminderDataUrl()
{
    static const QString string(QStringLiteral("dataUrl"));
    return string;
}

//Event item
const QString &QOrganizerJsonDbStr::event()
{
    static const QString string(QStringLiteral("com.nokia.mp.organizer.Event"));
    return string;
}

const QString &QOrganizerJsonDbStr::eventStartDateTime()
{
    static const QString string(QStringLiteral("startDateTime"));
    return string;
}

const QString &QOrganizerJsonDbStr::eventEndDateTime()
{
    static const QString string(QStringLiteral("endDateTime"));
    return string;
}

const QString &QOrganizerJsonDbStr::eventIsAllDay()
{
    static const QString string(QStringLiteral("isAllDay"));
    return string;
}

//To-do item
const QString &QOrganizerJsonDbStr::todo()
{
    static const QString string(QStringLiteral("com.nokia.mp.organizer.Todo"));
    return string;
}

const QString &QOrganizerJsonDbStr::todoStartDateTime()
{
    static const QString string(QStringLiteral("startDateTime"));
    return string;
}

const QString &QOrganizerJsonDbStr::todoDueDateTime()
{
    static const QString string(QStringLiteral("dueDateTime"));
    return string;
}

const QString &QOrganizerJsonDbStr::todoIsAllDay()
{
    static const QString string(QStringLiteral("isAllDay"));
    return string;
}

//Collection properties
const QString &QOrganizerJsonDbStr::collection()
{
    static const QString string(QStringLiteral("com.nokia.mp.organizer.Collection"));
    return string;
}

const QString &QOrganizerJsonDbStr::collectionDefaultFlag()
{
    static const QString string(QStringLiteral("isDefaultCollection"));
    return string;
}

const QString &QOrganizerJsonDbStr::collectionName()
{
    static const QString string(QStringLiteral("displayName"));
    return string;
}

const QString &QOrganizerJsonDbStr::collectionDescription()
{
    static const QString string(QStringLiteral("description"));
    return string;
}

const QString &QOrganizerJsonDbStr::collectionColor()
{
    static const QString string(QStringLiteral("color"));
    return string;
}

const QString &QOrganizerJsonDbStr::collectionImage()
{
    static const QString string(QStringLiteral("image"));
    return string;
}

const QString &QOrganizerJsonDbStr::collectionCustomFields()
{
    static const QString string(QStringLiteral("customFields"));
    return string;
}

const QString &QOrganizerJsonDbStr::defaultCollectionName()
{
    static const QString string(QStringLiteral("defaultCollection"));
    return string;
}

// Recurrence sub properties
const QString &QOrganizerJsonDbStr::itemRecurrenceDates()
{
    static const QString string(QStringLiteral("recurrenceDates"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemExceptioneDates()
{
    static const QString string(QStringLiteral("exceptionDates"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemExceptionRules()
{
    static const QString string(QStringLiteral("exceptionRules"));
    return string;
}

const QString &QOrganizerJsonDbStr::itemRecurrenceRules()
{
    static const QString string(QStringLiteral("recurrenceRules"));
    return string;
}

// Recurrence rule properties
const QString &QOrganizerJsonDbStr::ruleFirstDayOfWeek()
{
    static const QString string(QStringLiteral("firstDayOfWeek"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleFrequency()
{
    static const QString string(QStringLiteral("frequency"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleInterval()
{
    static const QString string(QStringLiteral("interval"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleLimitCount()
{
    static const QString string(QStringLiteral("limitCount"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleLimitDate()
{
    static const QString string(QStringLiteral("limitDate"));
    return string;
}

const QString &QOrganizerJsonDbStr::rulePositions()
{
    static const QString string(QStringLiteral("positions"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleDaysOfWeek()
{
    static const QString string(QStringLiteral("daysOfWeek"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleDaysOfMonth()
{
    static const QString string(QStringLiteral("daysOfMonth"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleDaysOfYear()
{
    static const QString string(QStringLiteral("daysOfYear"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleWeeksOfYear()
{
    static const QString string(QStringLiteral("weeksOfYear"));
    return string;
}

const QString &QOrganizerJsonDbStr::ruleMonthsOfYear()
{
    static const QString string(QStringLiteral("monthsOfYear"));
    return string;
}

// Attendee properties
const QString &QOrganizerJsonDbStr::attendee()
{
    static const QString string(QStringLiteral("attendee"));
    return string;
}

const QString &QOrganizerJsonDbStr::attendeeName()
{
    static const QString string(QStringLiteral("name"));
    return string;
}

const QString &QOrganizerJsonDbStr::attendeeEmailAddress()
{
    static const QString string(QStringLiteral("emailAddress"));
    return string;
}

const QString &QOrganizerJsonDbStr::attendeeId()
{
    static const QString string(QStringLiteral("attendeeId"));
    return string;
}

const QString &QOrganizerJsonDbStr::attendeeParticipationStatus()
{
    static const QString string(QStringLiteral("participationState"));
    return string;
}

const QString &QOrganizerJsonDbStr::attendeeParticipationRole()
{
    static const QString string(QStringLiteral("participationRole"));
    return string;
}

// Rsvp properties
const QString &QOrganizerJsonDbStr::rsvp()
{
    static const QString string(QStringLiteral("rsvp"));
    return string;
}

const QString &QOrganizerJsonDbStr::rsvpOrganizerName()
{
    static const QString string(QStringLiteral("organizerName"));
    return string;
}

const QString &QOrganizerJsonDbStr::rsvpOrganizerEmail()
{
    static const QString string(QStringLiteral("organizerEmail"));
    return string;
}

const QString &QOrganizerJsonDbStr::rsvpResponseRequirement()
{
    static const QString string(QStringLiteral("responseRequirement"));
    return string;
}

const QString &QOrganizerJsonDbStr::rsvpResponseDate()
{
    static const QString string(QStringLiteral("responseDate"));
    return string;
}

const QString &QOrganizerJsonDbStr::rsvpResponseDeadline()
{
    static const QString string(QStringLiteral("responseDeadline"));
    return string;
}

// Alarm object schema
const QString &QOrganizerJsonDbStr::alarm()
{
    static const QString string(QStringLiteral("com.nokia.mt.alarm-daemon.Alarm"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmISODate()
{
    static const QString string(QStringLiteral("isoDate"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmSnoozeTime()
{
    static const QString string(QStringLiteral("snoozeTime"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmSnoozeCount()
{
    static const QString string(QStringLiteral("snoozeCount"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmIdentifier()
{
    static const QString string(QStringLiteral("identifier"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmTitle()
{
    static const QString string(QStringLiteral("title"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmDescription()
{
    static const QString string(QStringLiteral("description"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmEventUuid()
{
    static const QString string(QStringLiteral("eventUuid"));
    return string;
}

const QString &QOrganizerJsonDbStr::alarmCalenderIdentifier()
{
    static const QString string(QStringLiteral("com.nokia.mt.calendar"));
    return string;
}

//warning string
const QString &QOrganizerJsonDbStr::warningMoreItemDelete()
{
    static const QString string(QStringLiteral("Items are delete from jsondb!!"));
    return string;
}

const QString &QOrganizerJsonDbStr::warningCollectionMissing()
{
    static const QString string(QStringLiteral("No default collection! error code:"));
    return string;
}

const QString &QOrganizerJsonDbStr::warningCollectionRemove()
{
    static const QString string(QStringLiteral("Number of items deleted from jsondb is not equal to request!!"));
    return string;
}

const QString &QOrganizerJsonDbStr::warningDefaultCollectionRemove()
{
    static const QString string(QStringLiteral("Default collection can not be removed!"));
    return string;
}

const QString &QOrganizerJsonDbStr::warningDefaultCollectionChange()
{
    static const QString string(QStringLiteral("Default collection can not be changed!"));
    return string;
}

const QString &QOrganizerJsonDbStr::warningDefaultCollectionCreate()
{
    static const QString string(QStringLiteral("Same name as default collection!"));
    return string;
}

// Notification
const QString &QOrganizerJsonDbStr::notificationQuery()
{
    static const QString string(QStringLiteral("[?_type in [\"com.nokia.mp.organizer.Event\", \"com.nokia.mp.organizer.Todo\", \"com.nokia.mp.organizer.Collection\"]]"));
    return string;
}

QTORGANIZER_END_NAMESPACE
