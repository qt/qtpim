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

#ifndef QORGANIZERJSONDBSTRING_H
#define QORGANIZERJSONDBSTRING_H

#include <qorganizerglobal.h>

#include <QtCore/qstring.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerJsonDbStr
{
public:
    //Jsondb manager name
    const static QString &managerName();
    const static QString &managerUri();
    const static QString &jsonDbEngineId();

    const static QString &jsonDbSchemaPrefix();

    // JsonDb strings
    const static QString &jsonDbUuid();
    const static QString &jsonDbType();
    const static QString &jsonDbData();
    const static QString &jsonDbCount();
    const static QString &jsonDbError();
    const static QString &jsonDbCode();

    // JsonDb query string
    const static QString &jsonDbQueryAllItems();
    const static QString &jsonDbQueryAllCollections();

    const static QString &jsonDbQueryCollectionIdsTemplate();
    const static QString &jsonDbQueryUuidsTemplate();

    //Organizer item string
    const static QString &itemName();
    const static QString &itemCollectionId();
    const static QString &itemDescription();
    const static QString &itemComments();
    const static QString &itemLocation();
    const static QString &itemPriority();
    const static QString &itemTags();
    const static QString &itemGuid();

    //Location sub properties
    const static QString &itemLocationLabel();
    const static QString &itemLocationGeo();
    const static QString &itemLocationGeoLatitude();
    const static QString &itemLocationGeoLongitude();

    //Item reminder properties
    const static QString &itemReminder();
    const static QString &itemReminderSecBeforeStart();
    const static QString &itemReminderRepCount();
    const static QString &itemReminderRepDelay();
    const static QString &itemReminderDataUrl();

    //Event item
    const static QString &event();
    const static QString &eventStartDateTime();
    const static QString &eventEndDateTime();
    const static QString &eventIsAllDay();

    //To-do item
    const static QString &todo();
    const static QString &todoStartDateTime();
    const static QString &todoDueDateTime();
    const static QString &todoIsAllDay();

    //Collection properties
    const static QString &collection();
    const static QString &collectionDefaultFlag();
    const static QString &collectionName();
    const static QString &collectionDescription();
    const static QString &collectionColor();
    const static QString &collectionImage();
    const static QString &collectionCustomFields();

    const static QString &defaultCollectionName();

    // Recurrence sub properties
    const static QString &itemRecurrenceDates();
    const static QString &itemExceptioneDates();
    const static QString &itemExceptionRules();
    const static QString &itemRecurrenceRules();


    // Recurrence rule properties
    const static QString &ruleFirstDayOfWeek();
    const static QString &ruleFrequency();
    const static QString &ruleInterval();
    const static QString &ruleLimitCount();
    const static QString &ruleLimitDate();
    const static QString &rulePositions();
    const static QString &ruleDaysOfWeek();
    const static QString &ruleDaysOfMonth();
    const static QString &ruleDaysOfYear();
    const static QString &ruleWeeksOfYear();
    const static QString &ruleMonthsOfYear();

    // Attendee properties
    const static QString &attendee();
    const static QString &attendeeName();
    const static QString &attendeeEmailAddress();
    const static QString &attendeeId();
    const static QString &attendeeParticipationStatus();
    const static QString &attendeeParticipationRole();

    // Rsvp properties
    const static QString &rsvp();
    const static QString &rsvpOrganizerName();
    const static QString &rsvpOrganizerEmail();
    const static QString &rsvpResponseRequirement();
    const static QString &rsvpResponseDate();
    const static QString &rsvpResponseDeadline();

    // Alarm Jsondb schema
    const static QString &alarm();
    const static QString &alarmISODate();
    const static QString &alarmSnoozeTime();
    const static QString &alarmSnoozeCount();
    const static QString &alarmIdentifier();
    const static QString &alarmTitle();
    const static QString &alarmDescription();
    const static QString &alarmEventUuid();
    const static QString &alarmCalenderIdentifier();

    //warning string
    const static QString &warningMoreItemDelete();
    const static QString &warningCollectionMissing();
    const static QString &warningCollectionRemove();
    const static QString &warningDefaultCollectionRemove();
    const static QString &warningDefaultCollectionChange();
    const static QString &warningDefaultCollectionCreate();

    // Notification
    const static QString &notificationQuery();
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBSTRING_H

