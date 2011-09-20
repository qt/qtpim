/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

//query [?_type in ["com.nokia.mp.organizer.Event", "com.nokia.mp.organizer.Todo"]]
#define ALL_ITEM_QUERY_STRING      "[?" + JsonDbString::kTypeStr + " in [\"" + QOrganizerJsonDbStr::Event + "\", \"" + QOrganizerJsonDbStr::Todo + "\"]]"
//[=uuid]
#define ITEM_ID_RESULT_STRING      "[=" + JsonDbString::kUuidStr + "]"
//jsondb query [?_type="com.nokia.mp.organizer.Collection"]
#define COLLECTION_QUERY_STRING      "[?" + JsonDbString::kTypeStr + "=\"" + QOrganizerJsonDbStr::Collection + "\"]"
#define ITEM_IDS_QUERY_STRING(x) JsonDbString::kUuidStr + "=\"" + x + "\" | ";

#define ITEM_COLLECTION_ID_QUERY_STRING    "[?" + QOrganizerJsonDbStr::ItemCollectionId + " in [\""
#define ITEM_IDS_LIST_QUERY_STRING         "[?" + JsonDbString::kUuidStr + " in [\""

class QOrganizerJsonDbStr
{
public:
    //Jsondb manager name
    const static QString ManagerName;
    const static QString ManagerUri;
    const static QString JsonDbEngineId;

    const static QString JsonDbSchemaPrefix;

    //Organizer item string
    const static QString ItemName;
    const static QString ItemCollectionId;
    const static QString ItemDescription;
    const static QString ItemComments;
    const static QString ItemLocation;
    const static QString ItemPriority;
    const static QString ItemVisualReminder;
    const static QString ItemAudibleReminder;
    const static QString ItemEmailReminder;
    const static QString ItemTags;
    const static QString ItemGuid;

    //Location sub properties
    const static QString ItemLocationLabel;
    const static QString ItemLocationLatitude;
    const static QString ItemLocationLongitude;

    //Item reminder properties
    const static QString ItemReminderSecBeforeStart;
    const static QString ItemReminderRepCount;
    const static QString ItemReminderRepDelay;
    //Item audible remibder
    const static QString ItemAudibleReminderDataUrl;
    //Item email reminder
    const static QString ItemEmailReminderSubject;
    const static QString ItemEmailReminderBody;
    const static QString ItemEmailReminderAttachments;
    const static QString ItemEmailReminderRecipients;
    //Item visual reminder
    const static QString ItemVisualReminderMessage;
    const static QString ItemVisualReminderDataUrl;

    //Event item
    const static QString Event;
    const static QString EventStartDateTime;
    const static QString EventEndDateTime;
    const static QString EventIsAllDay;
    const static QString EventLocation;
    const static QString EventInviter;
    const static QString EventAttendees;

    //To-do item
    const static QString Todo;
    const static QString TodoStartDateTime;
    const static QString TodoDueDateTime;
    const static QString TodoIsAllDay;

    //Collection properties
    const static QString Collection;
    const static QString CollectionDefaultFlag;
    const static QString CollectionName;
    const static QString CollectionDescription;
    const static QString CollectionColor;
    const static QString CollectionImage;
    const static QString CollectionCustomFields;

    const static QString DefaultCollectionName;

    // Recurrence sub properties
    const static QString ItemRecurrenceDates;
    const static QString ItemExceptioneDates;
    const static QString ItemExceptionRules;
    const static QString ItemRecurrenceRules;


    // Recurrence rule properties
    const static QString RuleFirstDayOfWeek;
    const static QString RuleFrequency;
    const static QString RuleInterval;
    const static QString RuleLimitCount;
    const static QString RuleLimitDate;
    const static QString RulePositions;
    const static QString RuleDaysOfWeek;
    const static QString RuleDaysOfMonth;
    const static QString RuleDaysOfYear;
    const static QString RuleWeeksOfYear;
    const static QString RuleMonthsOfYear;

    // Attendee properties
    const static QString Attendee;
    const static QString AttendeeName;
    const static QString AttendeeEmailAddress;
    const static QString AttendeeContactId;
    const static QString AttendeeParticipationStatus;
    const static QString AttendeeParticipationRole;

    //warning string
    const static QString WarningMoreItemDelete;
    const static QString WarningCollectionMissing;
    const static QString WarningCollectionRemove;
    const static QString WarningDefaultCollectionRemove;
    const static QString WarningDefaultCollectionChange;
    const static QString WarningDefaultCollectionCreate;

};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBSTRING_H

