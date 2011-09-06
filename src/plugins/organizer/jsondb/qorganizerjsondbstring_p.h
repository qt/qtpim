/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
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
#include <qlatin1constant.h>

QTPIM_USE_NAMESPACE

//query [?_type in ["com.nokia.mp.organizer.Event", "com.nokia.mp.organizer.Todo"]]
#define ALL_ITEM_QUERY_STRING      "[?" + JsonDbString::kTypeStr + " in [\"" + QOrganizerJsonDbStr::Event + "\", \"" + QOrganizerJsonDbStr::Todo + "\"]]"
//[=uuid]
#define ITEM_ID_RESULT_STRING      "[=" + JsonDbString::kUuidStr + "]"
//jsondb query [?_type="com.nokia.mp.organizer.Collection"]
#define COLLECTION_QUERY_STRING      "[?" + JsonDbString::kTypeStr + "=\"" + QOrganizerJsonDbStr::Collection + "\"]"
#define ITEM_IDS_QUERY_STRING(x) JsonDbString::kUuidStr + "=\"" + x + "\" | ";

#define ITEM_COLLECTION_ID_QUERY_STRING    "[?" + QOrganizerJsonDbStr::ItemCollectionId + " in [\""

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
    Q_DECLARE_LATIN1_CONSTANT(ItemCustomFields, "customFields");
    Q_DECLARE_LATIN1_CONSTANT(ItemComments,     "comments");
    Q_DECLARE_LATIN1_CONSTANT(ItemLocation,     "location");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriority,     "Priority");
    Q_DECLARE_LATIN1_CONSTANT(ItemVisualReminder, "visualReminder");
    Q_DECLARE_LATIN1_CONSTANT(ItemAudibleReminder,"audibleReminder");
    Q_DECLARE_LATIN1_CONSTANT(ItemEmailReminder,  "emailReminder");

    //Location sub properties
    Q_DECLARE_LATIN1_CONSTANT(ItemLocationLabel,    "label");
    Q_DECLARE_LATIN1_CONSTANT(ItemLocationLatitude, "latitude");
    Q_DECLARE_LATIN1_CONSTANT(ItemLocationLongitude,"longitude");

    //Priority sub properties
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityUnkownPriority,       "UnknownPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityHighestPriority,      "HighestPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityExtremelyhighPriority,"ExtremelyHighPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityVeryhighPriority,     "VeryHighPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityHighPriority,         "HighPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityMediumPriority,       "MediumPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityLowPriority,          "LowPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityVerylowPriority,      "VeryLowPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityExtremelylowPriority, "ExtremelyLowPriority");
    Q_DECLARE_LATIN1_CONSTANT(ItemPriorityLowestPriority,       "LowestPriority");

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
    Q_DECLARE_LATIN1_CONSTANT(TodoDueDateTime,   "endDateTime");
    Q_DECLARE_LATIN1_CONSTANT(TodoIsAllDay,      "isAllDay");

    //Collection properties
    Q_DECLARE_LATIN1_CONSTANT(Collection, "com.nokia.mp.organizer.Collection");
    Q_DECLARE_LATIN1_CONSTANT(CollectionDefaultFlag,  "isDefaultCollection");
    Q_DECLARE_LATIN1_CONSTANT(CollectionName,         "displayName");
    Q_DECLARE_LATIN1_CONSTANT(CollectionDescription,  "description");
    Q_DECLARE_LATIN1_CONSTANT(CollectionColor,        "color");
    Q_DECLARE_LATIN1_CONSTANT(CollectionImage,        "image");

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

    // Recurrence rule frequency emun
    Q_DECLARE_LATIN1_CONSTANT(FrequencyInvalid,"Invalid");
    Q_DECLARE_LATIN1_CONSTANT(FrequencyDaily,"Daily");
    Q_DECLARE_LATIN1_CONSTANT(FrequencyWeekly,"Weekly");
    Q_DECLARE_LATIN1_CONSTANT(FrequencyMonthly,"Monthly");
    Q_DECLARE_LATIN1_CONSTANT(FrequencyYearly,"Yearly");

    // Recurrence rule limit type emun
    Q_DECLARE_LATIN1_CONSTANT(LimitNoLimit,"NoLimit");
    Q_DECLARE_LATIN1_CONSTANT(LimitCount,"CountLimit");
    Q_DECLARE_LATIN1_CONSTANT(LimitDate,"DateLimit");

    // Recurrence rule day of week emun
    Q_DECLARE_LATIN1_CONSTANT(DayMonday,"Monday");
    Q_DECLARE_LATIN1_CONSTANT(DayTuesday,"Tuesday");
    Q_DECLARE_LATIN1_CONSTANT(DayWednesday,"Wednesday");
    Q_DECLARE_LATIN1_CONSTANT(DayThursday,"Thursday");
    Q_DECLARE_LATIN1_CONSTANT(DayFriday,"Friday");
    Q_DECLARE_LATIN1_CONSTANT(DaySaturday,"Saturday");
    Q_DECLARE_LATIN1_CONSTANT(DaySunday,"Sunday");

    // Recurrence rule month emun
    Q_DECLARE_LATIN1_CONSTANT(MonthJanuary,"January");
    Q_DECLARE_LATIN1_CONSTANT(MonthFebruary,"February");
    Q_DECLARE_LATIN1_CONSTANT(MonthMarch,"March");
    Q_DECLARE_LATIN1_CONSTANT(MonthApril,"April");
    Q_DECLARE_LATIN1_CONSTANT(MonthMay,"May");
    Q_DECLARE_LATIN1_CONSTANT(MonthJune,"June");
    Q_DECLARE_LATIN1_CONSTANT(MonthJuly,"July");
    Q_DECLARE_LATIN1_CONSTANT(MonthAugust,"August");
    Q_DECLARE_LATIN1_CONSTANT(MonthSeptember,"September");
    Q_DECLARE_LATIN1_CONSTANT(MonthOctober,"October");
    Q_DECLARE_LATIN1_CONSTANT(MonthNovember,"November");
    Q_DECLARE_LATIN1_CONSTANT(MonthDecember,"December");

    //warning string
    Q_DECLARE_LATIN1_CONSTANT(WarningMoreItemDelete, "Items are delete from jsondb!!");
    Q_DECLARE_LATIN1_CONSTANT(WarningCollectionMissing, "No default collection! error code:");
    Q_DECLARE_LATIN1_CONSTANT(WarningCollectionRemove, " Number of items deleted from jsondb is not equal to request !!");
    Q_DECLARE_LATIN1_CONSTANT(WarningDefaultCollectionRemove, "Default collection can not be removed!");
    Q_DECLARE_LATIN1_CONSTANT(WarningDefaultCollectionChange, "Default collection can not be changed!");
    Q_DECLARE_LATIN1_CONSTANT(WarningDefaultCollectionCreate, "Same name as default collection!");

};

#endif // QORGANIZERJSONDBSTRING_H

