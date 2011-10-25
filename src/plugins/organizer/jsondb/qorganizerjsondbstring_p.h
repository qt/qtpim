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
#include <qlatin1constant.h>

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
    Q_DECLARE_LATIN1_CONSTANT(ManagerName,    "qtorganizer:jsondb::");
    Q_DECLARE_LATIN1_CONSTANT(ManagerUri,     "qtorganizer:jsondb:");
    Q_DECLARE_LATIN1_CONSTANT(JsonDbEngineId, "jsondb");

    //Organizer item string
    Q_DECLARE_LATIN1_CONSTANT(ItemName,         "displayName");
    Q_DECLARE_LATIN1_CONSTANT(ItemCollectionId, "collectionId");
    Q_DECLARE_LATIN1_CONSTANT(ItemDescription,  "description");
    Q_DECLARE_LATIN1_CONSTANT(ItemComments,     "comments");
    Q_DECLARE_LATIN1_CONSTANT(ItemLocation,     "location");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriority,     "Priority");
    Q_DECLARE_LATIN1_CONSTANT(ItemVisualReminder, "visualReminder");
    Q_DECLARE_LATIN1_CONSTANT(ItemAudibleReminder,"audibleReminder");
    Q_DECLARE_LATIN1_CONSTANT(ItemEmailReminder,  "emailReminder");
    Q_DECLARE_LATIN1_CONSTANT(ItemTags,         "tags");
    Q_DECLARE_LATIN1_CONSTANT(ItemGuid,         "guid");

    //Location sub properties
    Q_DECLARE_LATIN1_CONSTANT(ItemLocationLabel,    "label");
    Q_DECLARE_LATIN1_CONSTANT(ItemLocationLatitude, "latitude");
    Q_DECLARE_LATIN1_CONSTANT(ItemLocationLongitude,"longitude");

    //Item reminder properties
    Q_DECLARE_LATIN1_CONSTANT(ItemReminderSecBeforeStart,  "secondsBeforeStart"); //SecondsBeforeStart
    Q_DECLARE_LATIN1_CONSTANT(ItemReminderRepCount,        "repetitionCount"); //RepetitionCount
    Q_DECLARE_LATIN1_CONSTANT(ItemReminderRepDelay,        "repetitionDelay"); //RepetitionDelay
    //Item audible remibder
    Q_DECLARE_LATIN1_CONSTANT(ItemAudibleReminderDataUrl,  "dataUrl");
    //Item email reminder
    Q_DECLARE_LATIN1_CONSTANT(ItemEmailReminderSubject,    "subject");
    Q_DECLARE_LATIN1_CONSTANT(ItemEmailReminderBody,       "body");
    Q_DECLARE_LATIN1_CONSTANT(ItemEmailReminderAttachments,"attachments");
    Q_DECLARE_LATIN1_CONSTANT(ItemEmailReminderRecipients, "recipients");
    //Item visual reminder
    Q_DECLARE_LATIN1_CONSTANT(ItemVisualReminderMessage,   "message");
    Q_DECLARE_LATIN1_CONSTANT(ItemVisualReminderDataUrl,   "dataUrl");

    //Event item
    Q_DECLARE_LATIN1_CONSTANT(Event, "com.nokia.mp.organizer.Event");
    Q_DECLARE_LATIN1_CONSTANT(EventStartDateTime,  "startDateTime");
    Q_DECLARE_LATIN1_CONSTANT(EventEndDateTime,    "endDateTime");
    Q_DECLARE_LATIN1_CONSTANT(EventIsAllDay,       "isAllDay");
    Q_DECLARE_LATIN1_CONSTANT(EventLocation,       "");
    Q_DECLARE_LATIN1_CONSTANT(EventInviter,        "");
    Q_DECLARE_LATIN1_CONSTANT(EventAttendees,      "");

    //To-do item
    Q_DECLARE_LATIN1_CONSTANT(Todo, "com.nokia.mp.organizer.Todo");
    Q_DECLARE_LATIN1_CONSTANT(TodoStartDateTime, "startDateTime");
    Q_DECLARE_LATIN1_CONSTANT(TodoDueDateTime,   "dueDateTime");
    Q_DECLARE_LATIN1_CONSTANT(TodoIsAllDay,      "isAllDay");

    //Collection properties
    Q_DECLARE_LATIN1_CONSTANT(Collection, "com.nokia.mp.organizer.Collection");
    Q_DECLARE_LATIN1_CONSTANT(CollectionDefaultFlag,  "isDefaultCollection");
    Q_DECLARE_LATIN1_CONSTANT(CollectionName,         "displayName");
    Q_DECLARE_LATIN1_CONSTANT(CollectionDescription,  "description");
    Q_DECLARE_LATIN1_CONSTANT(CollectionColor,        "color");
    Q_DECLARE_LATIN1_CONSTANT(CollectionImage,        "image");
    Q_DECLARE_LATIN1_CONSTANT(CollectionCustomFields, "customFields");

    Q_DECLARE_LATIN1_CONSTANT(DefaultCollectionName, "defaultCollection");

    // Recurrence sub properties
    Q_DECLARE_LATIN1_CONSTANT(ItemRecurrenceDates,"recurrenceDates");
    Q_DECLARE_LATIN1_CONSTANT(ItemExceptioneDates,"exceptionDates");
    Q_DECLARE_LATIN1_CONSTANT(ItemExceptionRules, "exceptionRules");
    Q_DECLARE_LATIN1_CONSTANT(ItemRecurrenceRules,"recurrenceRules");


    // Recurrence rule properties
    Q_DECLARE_LATIN1_CONSTANT(RuleFirstDayOfWeek,"firstDayOfWeek");
    Q_DECLARE_LATIN1_CONSTANT(RuleFrequency,"frequency");
    Q_DECLARE_LATIN1_CONSTANT(RuleInterval, "interval");
    Q_DECLARE_LATIN1_CONSTANT(RuleLimitCount,"limitCount");
    Q_DECLARE_LATIN1_CONSTANT(RuleLimitDate,"limitDate");
    Q_DECLARE_LATIN1_CONSTANT(RulePositions,"positions");
    Q_DECLARE_LATIN1_CONSTANT(RuleDaysOfWeek,"daysOfWeek");
    Q_DECLARE_LATIN1_CONSTANT(RuleDaysOfMonth,"daysOfMonth");
    Q_DECLARE_LATIN1_CONSTANT(RuleDaysOfYear,"daysOfYear");
    Q_DECLARE_LATIN1_CONSTANT(RuleWeeksOfYear,"weeksOfYear");
    Q_DECLARE_LATIN1_CONSTANT(RuleMonthsOfYear,"monthsOfYear");

    //warning string
    Q_DECLARE_LATIN1_CONSTANT(WarningMoreItemDelete, "Items are delete from jsondb!!");
    Q_DECLARE_LATIN1_CONSTANT(WarningCollectionMissing, "No default collection! error code:");
    Q_DECLARE_LATIN1_CONSTANT(WarningCollectionRemove, " Number of items deleted from jsondb is not equal to request !!");
    Q_DECLARE_LATIN1_CONSTANT(WarningDefaultCollectionRemove, "Default collection can not be removed!");
    Q_DECLARE_LATIN1_CONSTANT(WarningDefaultCollectionChange, "Default collection can not be changed!");
    Q_DECLARE_LATIN1_CONSTANT(WarningDefaultCollectionCreate, "Same name as default collection!");

};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBSTRING_H

