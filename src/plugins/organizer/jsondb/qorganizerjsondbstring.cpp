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

#include <qorganizerjsondbstring_p.h>

//Jsondb manager name
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ManagerName,    "qtorganizer:jsondb::");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ManagerUri,     "qtorganizer:jsondb:");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::JsonDbEngineId, "jsondb");

//Organizer item string
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemName,         "displayName");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemCollectionId, "collectionId");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemDescription,  "description");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemCustomFields, "customFields");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemComments,     "comments");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemLocation,     "location");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriority,     "Priority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemVisualReminder, "visualReminder");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemAudibleReminder,"audibleReminder");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemEmailReminder,  "emailReminder");

//Location sub properties
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemLocationLabel,    "label");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemLocationLatitude, "latitude");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemLocationLongitude,"longitude");

//Priority sub properties
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityUnkownPriority,       "UnknownPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityHighestPriority,      "HighestPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityExtremelyhighPriority,"ExtremelyHighPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityVeryhighPriority,     "VeryHighPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityHighPriority,         "HighPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityMediumPriority,       "MediumPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityLowPriority,          "LowPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityVerylowPriority,      "VeryLowPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityExtremelylowPriority, "ExtremelyLowPriority");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemPriorityLowestPriority,       "LowestPriority");

//Item reminder properties
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemReminderSecBeforeStart,  "secondsBeforeStart");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemReminderRepCount,        "repetitionCount");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemReminderRepDelay,        "repetitionDelay");
//Item audible remibder
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemAudibleReminderDataUrl,  "dataUrl");
//Item email reminder
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemEmailReminderSubject,    "subject");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemEmailReminderBody,       "body");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemEmailReminderAttachments,"attachments");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemEmailReminderRecipients, "recipients");
//Item visual reminder
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemVisualReminderMessage,   "message");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemVisualReminderDataUrl,   "dataUrl");

//Event item
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::Event, "com.nokia.mp.organizer.Event");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::EventStartDateTime,  "startDateTime");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::EventEndDateTime,    "endDateTime");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::EventIsAllDay,       "isAllDay");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::EventLocation,       "");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::EventInviter,        "");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::EventAttendees,      "");

//To-do item
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::Todo, "com.nokia.mp.organizer.Todo");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::TodoStartDateTime, "startDateTime");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::TodoDueDateTime,   "endDateTime");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::TodoIsAllDay,      "isAllDay");

//Collection properties
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::Collection, "com.nokia.mp.organizer.Collection");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::CollectionDefaultFlag,  "isDefaultCollection");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::CollectionName,         "displayName");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::CollectionDescription,  "description");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::CollectionColor,        "color");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::CollectionImage,        "image");

Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DefaultCollectionName, "defaultCollection");

// Recurrence sub properties
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemRecurrenceDates,"recurrenceDates");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemExceptioneDates,"exceptionDates");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemExceptionRules, "exceptionRules");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::ItemRecurrenceRules,"recurrenceRules");


// Recurrence rule properties
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleFirstDayOfWeek,"firstDayOfWeek");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleFrequency,"frequency");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleInterval, "interval");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleLimitCount,"limitCount");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleLimitDate,"limitDate");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RulePositions,"positions");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleDaysOfWeek,"daysOfWeek");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleDaysOfMonth,"daysOfMonth");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleDaysOfYear,"daysOfYear");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleWeeksOfYear,"weeksOfYear");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::RuleMonthsOfYear,"monthsOfYear");

// Recurrence rule frequency emun
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::FrequencyInvalid,"Invalid");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::FrequencyDaily,"Daily");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::FrequencyWeekly,"Weekly");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::FrequencyMonthly,"Monthly");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::FrequencyYearly,"Yearly");

// Recurrence rule limit type emun
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::LimitNoLimit,"NoLimit");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::LimitCount,"CountLimit");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::LimitDate,"DateLimit");

// Recurrence rule day of week emun
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DayMonday,"Monday");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DayTuesday,"Tuesday");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DayWednesday,"Wednesday");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DayThursday,"Thursday");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DayFriday,"Friday");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DaySaturday,"Saturday");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::DaySunday,"Sunday");

// Recurrence rule month emun
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthJanuary,"January");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthFebruary,"February");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthMarch,"March");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthApril,"April");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthMay,"May");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthJune,"June");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthJuly,"July");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthAugust,"August");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthSeptember,"September");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthOctober,"October");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthNovember,"November");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::MonthDecember,"December");

//warning string
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::WarningMoreItemDelete, "Items are delete from jsondb!!");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::WarningCollectionMissing, "No default collection! error code:");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::WarningCollectionRemove, " Number of items deleted from jsondb is not equal to request !!");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::WarningDefaultCollectionRemove, "Default collection can not be removed!");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::WarningDefaultCollectionChange, "Default collection can not be changed!");
Q_DEFINE_LATIN1_CONSTANT(QOrganizerJsonDbStr::WarningDefaultCollectionCreate, "Same name as default collection!");
