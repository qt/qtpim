/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERJSONDBSTRING_H
#define QORGANIZERJSONDBSTRING_H

#include <qorganizerglobal.h>

#include <QtCore/qstring.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerJsonDbStr
{
public:
    //Jsondb manager name
    inline const static QString managerName() { return QStringLiteral("qtorganizer:jsondb::"); }

    inline const static QString managerUri() { return QStringLiteral("qtorganizer:jsondb:"); }
    inline const static QString jsonDbEngineId() { return QStringLiteral("jsondb"); }

    inline const static QString jsonDbSchemaPrefix() { return QStringLiteral("com.nokia.mp.organizer."); }

    // JsonDb strings
    inline const static QString jsonDbUuid() { return QStringLiteral("_uuid"); }
    inline const static QString jsonDbType() { return QStringLiteral("_type"); }
    inline const static QString jsonDbData() { return QStringLiteral("data"); }
    inline const static QString jsonDbCount() { return QStringLiteral("count"); }
    inline const static QString jsonDbError() { return QStringLiteral("error"); }
    inline const static QString jsonDbCode() { return QStringLiteral("code"); }
    inline const static QString jsonDbVersion() {return QStringLiteral("_version"); }
    inline const static QString jsonDbValue() { return QStringLiteral("value"); }

    // JsonDb query string
    inline const static QString jsonDbQueryAllItems() { return QStringLiteral("[?_type in [\"com.nokia.mp.organizer.Event\", \"com.nokia.mp.organizer.Todo\"]]"); }
    inline const static QString jsonDbQueryEventViews() { return QStringLiteral("[?_type=\"com.nokia.mp.organizer.EventView\"]"); }
    inline const static QString jsonDbQueryAllCollections() { return QStringLiteral("[?_type=\"com.nokia.mp.organizer.Collection\"]"); }

    inline const static QString jsonDbQueryCollectionIdsTemplate() { return QStringLiteral("[?collectionId in [%1]]"); }
    inline const static QString jsonDbQueryUuidsTemplate() { return QStringLiteral("[?_uuid in [%1]]"); }

    // event views
    inline const static QString jsonDbEventViewType() { return QStringLiteral("com.nokia.mp.organizer.EventView"); }
    inline const static QString eventIsSynthetic() { return QStringLiteral("isSynthetic"); }

    //Organizer item string
    inline const static QString itemName() { return QStringLiteral("displayName"); }
    inline const static QString itemCollectionId() { return QStringLiteral("collectionId"); }
    inline const static QString itemDescription() { return QStringLiteral("description"); }
    inline const static QString itemComments() { return QStringLiteral("comments"); }
    inline const static QString itemLocation() { return QStringLiteral("location"); }
    inline const static QString itemPriority() { return QStringLiteral("Priority"); }
    inline const static QString itemTags() { return QStringLiteral("tags"); }
    inline const static QString itemGuid() { return QStringLiteral("guid"); }

    //Location sub properties
    inline const static QString itemLocationLabel() { return QStringLiteral("displayName"); }
    inline const static QString itemLocationGeo() { return QStringLiteral("geo"); }
    inline const static QString itemLocationGeoLatitude() { return QStringLiteral("latitude"); }
    inline const static QString itemLocationGeoLongitude() { return QStringLiteral("longitude"); }

    //Item reminder properties
    inline const static QString itemReminder() { return QStringLiteral("reminder"); }
    inline const static QString itemReminderSecBeforeStart() { return QStringLiteral("secondsBeforeStart"); }
    inline const static QString itemReminderRepCount() { return QStringLiteral("repetitionCount"); }
    inline const static QString itemReminderRepDelay() { return QStringLiteral("repetitionDelay"); }
    inline const static QString itemReminderDataUrl() { return QStringLiteral("dataUrl"); }

    //Event item
    inline const static QString event() { return QStringLiteral("com.nokia.mp.organizer.Event"); }
    inline const static QString eventStartDateTime() { return QStringLiteral("startDateTime"); }
    inline const static QString eventEndDateTime() { return QStringLiteral("endDateTime"); }
    inline const static QString eventIsAllDay() { return QStringLiteral("isAllDay"); }

    //To-do item
    inline const static QString todo() { return QStringLiteral("com.nokia.mp.organizer.Todo"); }
    inline const static QString todoStartDateTime() { return QStringLiteral("startDateTime"); }
    inline const static QString todoDueDateTime() { return QStringLiteral("dueDateTime"); }
    inline const static QString todoIsAllDay() { return QStringLiteral("isAllDay"); }
    inline const static QString todoFinishedDateTime() { return QStringLiteral("finishedDateTime"); }
    inline const static QString todoProgressPercentage() { return QStringLiteral("progressPercentage"); }
    inline const static QString todoStatus() { return QStringLiteral("status"); }

    //Collection properties
    inline const static QString collection() { return QStringLiteral("com.nokia.mp.organizer.Collection"); }
    inline const static QString collectionDefaultFlag() { return QStringLiteral("isDefaultCollection"); }
    inline const static QString collectionName() { return QStringLiteral("displayName"); }
    inline const static QString collectionDescription() { return QStringLiteral("description"); }
    inline const static QString collectionColor() { return QStringLiteral("color"); }
    inline const static QString collectionImage() { return QStringLiteral("image"); }
    inline const static QString collectionCustomFields() { return QStringLiteral("customFields"); }

    inline const static QString defaultCollectionName() { return QStringLiteral("defaultCollection"); }

    // Recurrence sub properties
    inline const static QString itemRecurrenceDates() { return QStringLiteral("recurrenceDates"); }
    inline const static QString itemExceptioneDates() { return QStringLiteral("exceptionDates"); }
    inline const static QString itemExceptionRules() { return QStringLiteral("exceptionRules"); }
    inline const static QString itemRecurrenceRules() { return QStringLiteral("recurrenceRules"); }


    // Recurrence rule properties
    inline const static QString ruleFirstDayOfWeek() { return QStringLiteral("firstDayOfWeek"); }
    inline const static QString ruleFrequency() { return QStringLiteral("frequency"); }
    inline const static QString ruleInterval() { return QStringLiteral("interval"); }
    inline const static QString ruleLimitCount() { return QStringLiteral("limitCount"); }
    inline const static QString ruleLimitDate() { return QStringLiteral("limitDate"); }
    inline const static QString rulePositions() { return QStringLiteral("positions"); }
    inline const static QString ruleDaysOfWeek() { return QStringLiteral("daysOfWeek"); }
    inline const static QString ruleDaysOfMonth() { return QStringLiteral("daysOfMonth"); }
    inline const static QString ruleDaysOfYear() { return QStringLiteral("daysOfYear"); }
    inline const static QString ruleWeeksOfYear() { return QStringLiteral("weeksOfYear"); }
    inline const static QString ruleMonthsOfYear() { return QStringLiteral("monthsOfYear"); }

    // Attendee properties
    inline const static QString attendee() { return QStringLiteral("attendee"); }
    inline const static QString attendeeName() { return QStringLiteral("name"); }
    inline const static QString attendeeEmailAddress() { return QStringLiteral("emailAddress"); }
    inline const static QString attendeeId() { return QStringLiteral("attendeeId"); }
    inline const static QString attendeeParticipationStatus() { return QStringLiteral("participationState"); }
    inline const static QString attendeeParticipationRole() { return QStringLiteral("participationRole"); }

    // Rsvp properties
    inline const static QString rsvp() { return QStringLiteral("rsvp"); }
    inline const static QString rsvpOrganizerName() { return QStringLiteral("organizerName"); }
    inline const static QString rsvpOrganizerEmail() { return QStringLiteral("organizerEmail"); }
    inline const static QString rsvpResponseRequirement() { return QStringLiteral("responseRequirement"); }
    inline const static QString rsvpResponseDate() { return QStringLiteral("responseDate"); }
    inline const static QString rsvpResponseDeadline() { return QStringLiteral("responseDeadline"); }

    // Alarm Jsondb schema
    inline const static QString alarm() { return QStringLiteral("com.nokia.mt.alarm-daemon.Alarm"); }
    inline const static QString alarmISODate() { return QStringLiteral("isoDate"); }
    inline const static QString alarmSnoozeTime() { return QStringLiteral("snoozeTime"); }
    inline const static QString alarmSnoozeCount() { return QStringLiteral("snoozeCount"); }
    inline const static QString alarmIdentifier() { return QStringLiteral("identifier"); }
    inline const static QString alarmTitle() { return QStringLiteral("title"); }
    inline const static QString alarmDescription() { return QStringLiteral("description"); }
    inline const static QString alarmEventUuid() { return QStringLiteral("eventUuid"); }
    inline const static QString alarmCalenderIdentifier() { return QStringLiteral("com.nokia.mt.calendar"); }

    //warning string
    inline const static QString warningMoreItemDelete() { return QStringLiteral("Items are delete from jsondb!!"); }
    inline const static QString warningCollectionMissing() { return QStringLiteral("No default collection! error code:"); }
    inline const static QString warningCollectionRemove() { return QStringLiteral("Number of items deleted from jsondb is not equal to request!!"); }
    inline const static QString warningDefaultCollectionRemove() { return QStringLiteral("Default collection can not be removed!"); }
    inline const static QString warningDefaultCollectionChange() { return QStringLiteral("Default collection can not be changed!"); }
    inline const static QString warningDefaultCollectionCreate() { return QStringLiteral("Same name as default collection!"); }

    // Notification
    inline const static QString notificationQuery() { return QStringLiteral("[?_type in [\"com.nokia.mp.organizer.Event\", \"com.nokia.mp.organizer.Todo\", \"com.nokia.mp.organizer.Collection\"]]"); }
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBSTRING_H

