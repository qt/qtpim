/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtPIM module of the Qt toolkit.
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

#include "qorganizerjsondbconverter.h"
#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring.h"
#include "qorganizer.h"

#include <private/jsondb-strings_p.h>

Q_USE_JSONDB_NAMESPACE

QTORGANIZER_BEGIN_NAMESPACE

const int enumMapEnd = -1212;
static QOrganizerJsonDbEnumConversionData qt_organizerPriorityEnumMap[] = {
    {QOrganizerItemPriority::UnknownPriority,        QLatin1String("UnknownPriority")},
    {QOrganizerItemPriority::ExtremelyHighPriority,  QLatin1String("HighestPriority")},
    {QOrganizerItemPriority::ExtremelyHighPriority,  QLatin1String("ExtremelyHighPriority")},
    {QOrganizerItemPriority::VeryHighPriority,       QLatin1String("VeryHighPriority")},
    {QOrganizerItemPriority::HighPriority,           QLatin1String("HighPriority")},
    {QOrganizerItemPriority::MediumPriority,         QLatin1String("MediumPriority")},
    {QOrganizerItemPriority::LowPriority,            QLatin1String("LowPriority")},
    {QOrganizerItemPriority::VeryLowPriority,        QLatin1String("VeryLowPriority")},
    {QOrganizerItemPriority::ExtremelyLowPriority,   QLatin1String("ExtremelyLowPriority")},
    {QOrganizerItemPriority::LowestPriority,         QLatin1String("LowestPriority")},
    {enumMapEnd,                                     QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerFrequencyEnumMap[] = {
    {QOrganizerRecurrenceRule::Invalid,              QLatin1String("Invalid")},
    {QOrganizerRecurrenceRule::Daily,                QLatin1String("Daily")},
    {QOrganizerRecurrenceRule::Weekly,               QLatin1String("Weekly")},
    {QOrganizerRecurrenceRule::Monthly,              QLatin1String("Monthly")},
    {QOrganizerRecurrenceRule::Yearly,               QLatin1String("Yearly")},
    {enumMapEnd,                                     QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerDayEnumMap[] = {
    {Qt::Monday,                                     QLatin1String("Monday")},
    {Qt::Tuesday,                                    QLatin1String("Tuesday")},
    {Qt::Wednesday,                                  QLatin1String("Wednesday")},
    {Qt::Thursday,                                   QLatin1String("Thursday")},
    {Qt::Friday,                                     QLatin1String("Friday")},
    {Qt::Saturday,                                   QLatin1String("Saturday")},
    {Qt::Sunday,                                     QLatin1String("Sunday")},
    {enumMapEnd,                                     QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerMonthEnumMap[] = {
    {QOrganizerRecurrenceRule::January,              QLatin1String("January")},
    {QOrganizerRecurrenceRule::February,             QLatin1String("February")},
    {QOrganizerRecurrenceRule::March,                QLatin1String("March")},
    {QOrganizerRecurrenceRule::April,                QLatin1String("April")},
    {QOrganizerRecurrenceRule::May,                  QLatin1String("May")},
    {QOrganizerRecurrenceRule::June,                 QLatin1String("June")},
    {QOrganizerRecurrenceRule::July,                 QLatin1String("July")},
    {QOrganizerRecurrenceRule::August,               QLatin1String("August")},
    {QOrganizerRecurrenceRule::September,            QLatin1String("September")},
    {QOrganizerRecurrenceRule::October,              QLatin1String("October")},
    {QOrganizerRecurrenceRule::November,             QLatin1String("November")},
    {QOrganizerRecurrenceRule::December,             QLatin1String("December")},
    {enumMapEnd,                                     QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerParticipationStatusMap[] = {
    {QOrganizerEventAttendee::StatusUnknown,         QLatin1String("Unknown")},
    {QOrganizerEventAttendee::StatusAccepted,        QLatin1String("Accepted")},
    {QOrganizerEventAttendee::StatusDeclined,        QLatin1String("Declined")},
    {QOrganizerEventAttendee::StatusTentative,       QLatin1String("Tentative")},
    {QOrganizerEventAttendee::StatusDelegated,       QLatin1String("Delegated")},
    {QOrganizerEventAttendee::StatusInProcess,       QLatin1String("InProcess")},
    {QOrganizerEventAttendee::StatusCompleted,       QLatin1String("Completed")},
    {enumMapEnd,                                     QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerParticipationRoleMap[] = {
    {QOrganizerEventAttendee::RoleUnknown,             QLatin1String("Unknown")},
    {QOrganizerEventAttendee::RoleOrganizer,           QLatin1String("Organizer")},
    {QOrganizerEventAttendee::RoleChairperson,         QLatin1String("Chairperson")},
    {QOrganizerEventAttendee::RoleHost,                QLatin1String("Host")},
    {QOrganizerEventAttendee::RoleRequiredParticipant, QLatin1String("RequiredParticipant")},
    {QOrganizerEventAttendee::RoleOptionalParticipant, QLatin1String("OptionalParticipant")},
    {QOrganizerEventAttendee::RoleNonParticipant,      QLatin1String("NonParticipant")},
    {enumMapEnd,                                       QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerResponseRequirementMap[] = {
    {QOrganizerEventRsvp::ResponseNotRequired,         QLatin1String("NotRequired")},
    {QOrganizerEventRsvp::ResponseRequired,            QLatin1String("Required")},
    {enumMapEnd,                                       QLatin1String("")}
};

static QOrganizerJsonDbEnumConversionData qt_organizerItemTypeMap[] = {
    {QOrganizerItemType::TypeUndefined,                   QStringLiteral("Unknown")},
    {QOrganizerItemType::TypeEvent,                     QStringLiteral("Event")},
    {QOrganizerItemType::TypeEventOccurrence,           QStringLiteral("EventOccurrence")},
    {QOrganizerItemType::TypeTodo,                      QStringLiteral("Todo")},
    {QOrganizerItemType::TypeTodoOccurrence,            QStringLiteral("TodoOccurrence")},
    {QOrganizerItemType::TypeJournal,                   QStringLiteral("Journal")},
    {QOrganizerItemType::TypeNote,                      QStringLiteral("Note")},
};

QOrganizerJsonDbConverter::QOrganizerJsonDbConverter()
{
}

QOrganizerManager::Error QOrganizerJsonDbConverter::jsondbErrorToOrganizerError(JsonDbError::ErrorCode jsonErrorCode)
{
    QOrganizerManager::Error error;
    switch (jsonErrorCode) {
    case JsonDbError::NoError:
        error = QOrganizerManager::NoError;
        break;

    case JsonDbError::DatabaseError:
        error = QOrganizerManager::UnspecifiedError;
        break;

    case JsonDbError::QuotaExceeded:
        error = QOrganizerManager::OutOfMemoryError;
        break;

    case JsonDbError::OperationNotPermitted:
        error = QOrganizerManager::PermissionsError;
        break;

    case JsonDbError::InvalidRequest:
        // update request with invalid uuid
        error = QOrganizerManager::DoesNotExistError;
        break;

    case JsonDbError::MissingObject:
        // update request with invalid uuid
        error = QOrganizerManager::DoesNotExistError;
        break;

    case JsonDbError::MissingType://Remove no-exist item get this error from jsondb
        error = QOrganizerManager::DoesNotExistError;
        break;

    default:
        error = QOrganizerManager::UnspecifiedError;
        break;
    }
    return error;
}

bool QOrganizerJsonDbConverter::jsonDbObjectToItem(const QVariantMap& object, QOrganizerItem* item) const
{
    // first handle mandatory fields

    QString jsonDbUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonDbUuid.isEmpty())
        return false;
    item->setId(QOrganizerItemId(new QOrganizerJsonDbItemId(jsonDbUuid)));

    QString guid = object.value(QOrganizerJsonDbStr::itemGuid()).toString();
    if (guid.isEmpty())
        return false;
    item->setGuid(guid);

    QString jsonDbType = object.value(JsonDbString::kTypeStr).toString();
    if (jsonDbType == QOrganizerJsonDbStr::event())
        item->setType(QOrganizerItemType::TypeEvent);
    else if (jsonDbType == QOrganizerJsonDbStr::todo())
        item->setType(QOrganizerItemType::TypeTodo);
    else
        return false;

    QString jsonDbCollectionId = object.value(QOrganizerJsonDbStr::itemCollectionId()).toString();
    if (jsonDbCollectionId.isEmpty())
        return false;
    item->setCollectionId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonDbCollectionId)));

    // now go through all fields
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::itemName()) {
            QString displayLabel = i.value().toString();
            if (!displayLabel.isEmpty())
                item->setDisplayLabel(displayLabel);
        } else if (i.key() == QOrganizerJsonDbStr::itemDescription()) {
            QString description = i.value().toString();
            if (!description.isEmpty())
                item->setDescription(description);
        } else if (i.key() == QOrganizerJsonDbStr::itemComments()) {
            QStringList comments = i.value().toStringList();
            if (!comments.isEmpty())
                item->setComments(comments);
        } else if (i.key() == QOrganizerJsonDbStr::itemTags()) {
            QStringList tags = i.value().toStringList();
            if (!tags.isEmpty())
                item->setTags(tags);
        } else if (i.key() == QOrganizerJsonDbStr::itemPriority()) {
            QString jsonDbPriority = i.value().toString();
            if (!jsonDbPriority.isEmpty()) {
                QOrganizerItemPriority priority;
                priority.setPriority(static_cast<QOrganizerItemPriority::Priority>(stringToEnum(qt_organizerPriorityEnumMap, jsonDbPriority)));
                item->saveDetail(&priority);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRules()) {
            QSet<QOrganizerRecurrenceRule> recurrenceRules;
            QVariantList recurrenceRulesList = i.value().toList();
            foreach (const QVariant &recurrenceRule, recurrenceRulesList) {
                QOrganizerRecurrenceRule rule;
                jsonDbObjectToRecurrenceRule(recurrenceRule.toMap(), &rule);
                recurrenceRules.insert(rule);
            }

            if (!recurrenceRules.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemRecurrence::DefinitionName);
                recurrence.setRecurrenceRules(recurrenceRules);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemExceptionRules()) {
            QSet<QOrganizerRecurrenceRule> exceptionRules;
            QVariantList exceptionRulesList = i.value().toList();
            foreach (const QVariant &exceptionRule, exceptionRulesList) {
                QOrganizerRecurrenceRule rule;
                jsonDbObjectToRecurrenceRule(exceptionRule.toMap(), &rule);
                exceptionRules.insert(rule);
            }

            if (!exceptionRules.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemRecurrence::DefinitionName);
                recurrence.setExceptionRules(exceptionRules);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceDates()) {
            QSet<QDate> recurrenceDates;
            QVariantList recurrenceDatesList = i.value().toList();
            foreach (const QVariant &recurrenceDate, recurrenceDatesList)
                recurrenceDates.insert(recurrenceDate.toDate());

            if (!recurrenceDates.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemRecurrence::DefinitionName);
                recurrence.setRecurrenceDates(recurrenceDates);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemExceptioneDates()) {
            QSet<QDate> exceptionDates;
            QVariantList exceptionDatesList = i.value().toList();
            foreach (const QVariant &exceptionDate, exceptionDatesList)
                exceptionDates.insert(exceptionDate.toDate());

            if (!exceptionDates.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemRecurrence::DefinitionName);
                recurrence.setExceptionDates(exceptionDates);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventStartDateTime()) {
            // EventStartDateTime is the same as TodoStartDateTime, thus a "hack" here ;)
            QDateTime startTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (startTime.isValid()) {
                if (item->type() == QOrganizerItemType::TypeEvent) {
                    QOrganizerEventTime eventTime = item->detail(QOrganizerEventTime::DefinitionName);
                    eventTime.setStartDateTime(startTime);
                    item->saveDetail(&eventTime);
                } else if (item->type() == QOrganizerItemType::TypeTodo) {
                    QOrganizerTodoTime todoTime = item->detail(QOrganizerTodoTime::DefinitionName);
                    todoTime.setStartDateTime(startTime);
                    item->saveDetail(&todoTime);
                }
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventEndDateTime()) {
            QDateTime endTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (endTime.isValid()) {
                QOrganizerEventTime eventTime = item->detail(QOrganizerEventTime::DefinitionName);
                eventTime.setEndDateTime(endTime);
                item->saveDetail(&eventTime);
            }
        } else if (i.key() == QOrganizerJsonDbStr::todoDueDateTime()) {
            QDateTime dueTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (dueTime.isValid()) {
                QOrganizerTodoTime todoTime = item->detail(QOrganizerTodoTime::DefinitionName);
                todoTime.setDueDateTime(dueTime);
                item->saveDetail(&todoTime);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventIsAllDay()) {
            // EventIsAllDay is the same as TodoIsAllDay, thus a "hack" here ;)
            bool isAllDay = i.value().toBool();
            if (item->type() == QOrganizerItemType::TypeEvent) {
                QOrganizerEventTime eventTime = item->detail(QOrganizerEventTime::DefinitionName);
                eventTime.setAllDay(isAllDay);
                item->saveDetail(&eventTime);
            } else if (item->type() == QOrganizerItemType::TypeTodo) {
                QOrganizerTodoTime todoTime = item->detail(QOrganizerTodoTime::DefinitionName);
                todoTime.setAllDay(isAllDay);
                item->saveDetail(&todoTime);
            }
        } else if (i.key() == QOrganizerJsonDbStr::attendee()) {
            QVariantList attendeeObjectList = i.value().toList();
            foreach (const QVariant &attendeeObject, attendeeObjectList) {
                QOrganizerEventAttendee attendee;
                jsonDbObjectToAttendeeDetail(attendeeObject.toMap(), &attendee);
                if (!attendee.isEmpty())
                    item->saveDetail(&attendee);
            }
        } else if (i.key() == QOrganizerJsonDbStr::rsvp()) {
            QVariantMap rsvpMap = i.value().toMap();
            if (!rsvpMap.isEmpty()) {
                QOrganizerEventRsvp rsvp;
                jsonDbObjectToRsvpDetail(rsvpMap, &rsvp);
                item->saveDetail(&rsvp);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemReminder()) {
            QVariantMap reminderMap = i.value().toMap();
            if (!reminderMap.isEmpty()) {
                QOrganizerItemAudibleReminder audibleReminder;
                jsonDbObjectToAudibleReminderDetail(reminderMap, &audibleReminder);
                item->saveDetail(&audibleReminder);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemLocation()) {
            QVariantMap locationMap = i.value().toMap();
            if (!locationMap.isEmpty()) {
                QOrganizerItemLocation location;
                jsonDbObjectToLocationDetail(locationMap, &location);
                item->saveDetail(&location);
            }
        } else if (i.key() == JsonDbString::kUuidStr
            || i.key() == JsonDbString::kTypeStr
            || i.key() == QOrganizerJsonDbStr::itemCollectionId()
            || i.key() == QOrganizerJsonDbStr::itemGuid()) {
            // skip the mandatory fields, as they're already parsed
        } else if (i.key().at(0) == QChar('_')) {
            // skip as it's used internally
        } else {
            // TODO any other fields to filter?
            QOrganizerItemExtendedDetail extendedDetail;
            extendedDetail.setName(i.key());
            extendedDetail.setData(i.value());
            item->saveDetail(&extendedDetail);
        }
        ++i;
    }

    return true;
}

bool QOrganizerJsonDbConverter::itemToJsonDbObject(const QOrganizerItem& item, QVariantMap* object) const
{
    QSet<QOrganizerRecurrenceRule> recurrenceRules;
    QSet<QOrganizerRecurrenceRule> exceptionRules;
    QSet<QDate> recurrenceDates;
    QSet<QDate> exceptionDates;

    if (item.type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent event = static_cast<QOrganizerEvent>(item);
        object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::event());

        QOrganizerEventTime eventTime = event.detail(QOrganizerEventTime::DefinitionName);
        if (eventTime.hasValue(QOrganizerEventTime::FieldStartDateTime))
            object->insert(QOrganizerJsonDbStr::eventStartDateTime(), eventTime.startDateTime().toUTC().toString(Qt::ISODate));
        if (eventTime.hasValue(QOrganizerEventTime::FieldEndDateTime))
            object->insert(QOrganizerJsonDbStr::eventEndDateTime(), eventTime.endDateTime().toUTC().toString(Qt::ISODate));
        if (eventTime.hasValue(QOrganizerEventTime::FieldAllDay))
            object->insert(QOrganizerJsonDbStr::eventIsAllDay(), eventTime.isAllDay());

        recurrenceRules = event.recurrenceRules();
        exceptionRules = event.exceptionRules();
        recurrenceDates = event.recurrenceDates();
        exceptionDates = event.exceptionDates();
    } else if (item.type() == QOrganizerItemType::TypeTodo) {
        QOrganizerTodo todo = static_cast<QOrganizerTodo>(item);
        object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::todo());

        QOrganizerTodoTime todoTime = todo.detail(QOrganizerTodoTime::DefinitionName);
        if (todoTime.hasValue(QOrganizerTodoTime::FieldStartDateTime))
            object->insert(QOrganizerJsonDbStr::todoStartDateTime(), todoTime.startDateTime().toUTC().toString(Qt::ISODate));
        if (todoTime.hasValue(QOrganizerTodoTime::FieldDueDateTime))
            object->insert(QOrganizerJsonDbStr::todoDueDateTime(), todoTime.dueDateTime().toUTC().toString(Qt::ISODate));
        if (todoTime.hasValue(QOrganizerTodoTime::FieldAllDay))
            object->insert(QOrganizerJsonDbStr::todoIsAllDay(), todoTime.isAllDay());

        recurrenceRules = todo.recurrenceRules();
        exceptionRules = todo.exceptionRules();
        recurrenceDates = todo.recurrenceDates();
        exceptionDates = todo.exceptionDates();
    } else {
        return false;
    }

    if (!item.id().isNull()) {
        QString jsonUuid = item.id().toString().remove (QOrganizerJsonDbStr::managerName());
        object->insert (JsonDbString::kUuidStr, jsonUuid);
    }
    object->insert(QOrganizerJsonDbStr::itemCollectionId(),
                   item.collectionId().toString().remove(QOrganizerJsonDbStr::managerName()));

    if (item.guid().isEmpty())
        return false;
    object->insert(QOrganizerJsonDbStr::itemGuid(), item.guid());

    QString displayLabel(item.displayLabel());
    if (!displayLabel.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemName(), displayLabel);

    QString description(item.description());
    if (!description.isEmpty())
        object->insert (QOrganizerJsonDbStr::itemDescription(), description);

    QStringList comments(item.comments());
    if (!comments.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemComments(), comments);

    QStringList tags(item.tags());
    if (!tags.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemTags(), tags);

    // location
    QOrganizerItemLocation locationDetail = item.detail(QOrganizerItemLocation::DefinitionName);
    if (!locationDetail.isEmpty()) {
        QVariantMap locationObject;
        locationDetailToJsonDbObject(locationDetail, &locationObject);
        object->insert(QOrganizerJsonDbStr::itemLocation(), locationObject);
    }

    QOrganizerItemPriority priorityDetail = item.detail(QOrganizerItemPriority::DefinitionName);
    if (!priorityDetail.isEmpty()) {
        object->insert(QOrganizerJsonDbStr::itemPriority(), enumToString(qt_organizerPriorityEnumMap, priorityDetail.priority()));
    }

    QOrganizerItemAudibleReminder audibleReminder = item.detail(QOrganizerItemAudibleReminder::DefinitionName);
    if (!audibleReminder.isEmpty()) {
        QVariantMap reminderObject;
        audibleReminderDetailToJsonDbObject(audibleReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::itemReminder(), reminderObject);
    }

    // recurrence
    if (!recurrenceRules.isEmpty()) {
        QVariantList recurrenceRulesList;
        foreach (QOrganizerRecurrenceRule recurrenceRule, recurrenceRules) {
            QVariantMap recurrenceRuleObject;
            recurrenceRuleToJsonDbObject(recurrenceRule, &recurrenceRuleObject);
            recurrenceRulesList.append(recurrenceRuleObject);
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRules(), recurrenceRulesList);
    }

    if (!exceptionRules.isEmpty()) {
        QVariantList exceptionRulesList;
        foreach (QOrganizerRecurrenceRule exceptionRule, exceptionRules) {
            QVariantMap exceptionRuleObject;
            recurrenceRuleToJsonDbObject(exceptionRule, &exceptionRuleObject);
            exceptionRulesList.append(exceptionRuleObject);
        }
        object->insert(QOrganizerJsonDbStr::itemExceptionRules(), exceptionRulesList);
    }

    if (!recurrenceDates.isEmpty()) {
        QVariantList recurrenceDatesList;
        foreach (QDate recurrenceDate, recurrenceDates) {
            recurrenceDatesList.append(recurrenceDate.toString(Qt::ISODate));
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceDates(), recurrenceDatesList);
    }

    if (!exceptionDates.isEmpty()) {
        QVariantList exceptionDatesList;
        foreach (QDate exceptionDate, exceptionDates) {
            exceptionDatesList.append(exceptionDate.toString(Qt::ISODate));
        }
        object->insert(QOrganizerJsonDbStr::itemExceptioneDates(), exceptionDatesList);
    }

    // attendee
    QList<QOrganizerEventAttendee> attendees = item.details<QOrganizerEventAttendee>();
    if (!attendees.isEmpty()) {
        QVariantList attendeeObjectList;
        foreach (const QOrganizerEventAttendee &attendee, attendees) {
            QVariantMap attendeeObject;
            attendeeDetailToJsonDbObject(attendee, &attendeeObject);
            if (!attendeeObject.isEmpty())
                attendeeObjectList.append(attendeeObject);
        }
        if (!attendeeObjectList.isEmpty())
            object->insert(QOrganizerJsonDbStr::attendee(), attendeeObjectList);
    }

    // rsvp
    // Note: Since there is only one rsvp per event in jsondb-backend, the first found rsvp-detail
    // is taken and rest are ignored
    QOrganizerEventRsvp rsvp = item.detail(QOrganizerEventRsvp::DefinitionName);
    if (!rsvp.isEmpty()) {
        QVariantMap rsvpObject;
        rsvpDetailToJsonDbObject(rsvp, &rsvpObject);
        object->insert(QOrganizerJsonDbStr::rsvp(), rsvpObject);
    }

    // extended details
    QList<QOrganizerItemExtendedDetail> extendedDetails = item.details<QOrganizerItemExtendedDetail>();
    foreach (const QOrganizerItemExtendedDetail &detail, extendedDetails) {
        // TODO use namespace or other techniques to protect pre-defined fields, design needed
        if (detail.name().at(0) == QChar('_'))
            continue;

        QVariant property;
        extendedDetailToJsonDbProperty(detail, property);
        if (!property.isNull())
            object->insert(detail.name(), property);
    }

    return true;
}

void QOrganizerJsonDbConverter::attendeeDetailToJsonDbObject(const QOrganizerEventAttendee &attendeeDetail, QVariantMap *object) const
{
    QMap<int, QVariant> detailValues = attendeeDetail.values();
    QMap<int, QVariant>::const_iterator i = detailValues.constBegin();
    QString value;
    QString jsonDbField;
    while (i != detailValues.constEnd()) {
        if (i.key() == QOrganizerEventAttendee::FieldName) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::attendeeName();
        } else if (i.key() == QOrganizerEventAttendee::FieldEmailAddress) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::attendeeEmailAddress();
        } else if (i.key() == QOrganizerEventAttendee::FieldAttendeeId) {
            value= i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::attendeeId();
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationRole) {
            value = enumToString(qt_organizerParticipationRoleMap, i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationRole();
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationStatus) {
            value = enumToString(qt_organizerParticipationStatusMap, i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationStatus();
        }
        if (!value.isEmpty()) {
            object->insert(jsonDbField, value);
            value.clear();
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToAttendeeDetail(const QVariantMap &object, QOrganizerEventAttendee *attendeeDetail) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QString value;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::attendeeName()) {
            value = i.value().toString();
            if (!value.isEmpty())
                attendeeDetail->setName(value);
        } else if (i.key() == QOrganizerJsonDbStr::attendeeEmailAddress()) {
            value = i.value().toString();
            if (!value.isEmpty())
                attendeeDetail->setEmailAddress(value);
        } else if (i.key() == QOrganizerJsonDbStr::attendeeId()) {
            value = i.value().toString();
            if (!value.isEmpty())
                attendeeDetail->setAttendeeId(value);
        } else if (i.key() == QOrganizerJsonDbStr::attendeeParticipationRole()) {
            int intValue = stringToEnum(qt_organizerParticipationRoleMap, i.value().toString());
            attendeeDetail->setParticipationRole(static_cast<QOrganizerEventAttendee::ParticipationRole>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::attendeeParticipationStatus()) {
            int intValue = stringToEnum(qt_organizerParticipationStatusMap, i.value().toString());
            attendeeDetail->setParticipationStatus(static_cast<QOrganizerEventAttendee::ParticipationStatus>(intValue));
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::rsvpDetailToJsonDbObject(const QOrganizerEventRsvp &rsvpDetail, QVariantMap *object) const
{
    QMap<int, QVariant> rsvpValues = rsvpDetail.values();
    QMap<int, QVariant>::const_iterator i = rsvpValues.constBegin();
    QString value;
    QString jsonDbField;
    while (i != rsvpValues.constEnd()) {
        if (i.key() == QOrganizerEventRsvp::FieldOrganizerName) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::rsvpOrganizerName();
        } else if (i.key() == QOrganizerEventRsvp::FieldOrganizerEmail) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::rsvpOrganizerEmail();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseDeadline) {
            value = i.value().toDate().toString(Qt::ISODate);
            jsonDbField = QOrganizerJsonDbStr::rsvpResponseDeadline();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseDate) {
            value = i.value().toDate().toString(Qt::ISODate);
            jsonDbField = QOrganizerJsonDbStr::rsvpResponseDate();
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationRole) {
            value = enumToString(qt_organizerParticipationRoleMap, i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationRole();
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationStatus) {
            value = enumToString(qt_organizerParticipationStatusMap, i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationStatus();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseRequirement) {
            value = enumToString(qt_organizerResponseRequirementMap, i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::rsvpResponseRequirement();
        }
        if (!value.isEmpty()) {
            object->insert(jsonDbField, value);
            value.clear();
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToRsvpDetail(const QVariantMap &object, QOrganizerEventRsvp *rsvpDetail) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QString value;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::rsvpOrganizerName()) {
            value = i.value().toString();
            if (!value.isEmpty())
                rsvpDetail->setOrganizerName(value);
        } else if (i.key() == QOrganizerJsonDbStr::rsvpOrganizerEmail()) {
            value = i.value().toString();
            if (!value.isEmpty())
                rsvpDetail->setOrganizerEmail(value);
        } else if (i.key() == QOrganizerJsonDbStr::rsvpResponseDeadline()) {
            QDate date = i.value().toDate();
            if (date.isValid())
                rsvpDetail->setResponseDeadline(date);
        } else if (i.key() == QOrganizerJsonDbStr::rsvpResponseDate()) {
            QDate date = i.value().toDate();
            if (date.isValid())
                rsvpDetail->setResponseDate(date);
        } else if (i.key() == QOrganizerJsonDbStr::attendeeParticipationRole()) {
            int intValue = stringToEnum(qt_organizerParticipationRoleMap, i.value().toString());
            rsvpDetail->setParticipationRole(static_cast<QOrganizerEventAttendee::ParticipationRole>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::attendeeParticipationStatus()) {
            int intValue = stringToEnum(qt_organizerParticipationStatusMap, i.value().toString());
            rsvpDetail->setParticipationStatus(static_cast<QOrganizerEventAttendee::ParticipationStatus>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::rsvpResponseRequirement()) {
            int intValue = stringToEnum(qt_organizerResponseRequirementMap, i.value().toString());
            rsvpDetail->setResponseRequirement(static_cast<QOrganizerEventRsvp::ResponseRequirement>(intValue));
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::locationDetailToJsonDbObject(const QOrganizerItemLocation &locationDetail, QVariantMap *object) const
{
    QString label(locationDetail.label());
    if (!label.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemLocationLabel(), label);

    QVariantMap geoObject;
    if (locationDetail.hasValue(QOrganizerItemLocation::FieldLatitude))
        geoObject.insert(QOrganizerJsonDbStr::itemLocationGeoLatitude(), locationDetail.latitude());
    if (locationDetail.hasValue(QOrganizerItemLocation::FieldLongitude))
        geoObject.insert(QOrganizerJsonDbStr::itemLocationGeoLongitude(), locationDetail.longitude());
    if (!geoObject.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemLocationGeo(), geoObject);
}

void QOrganizerJsonDbConverter::jsonDbObjectToLocationDetail(const QVariantMap &object, QOrganizerItemLocation *locationDetail) const
{
    QString label(object.value(QOrganizerJsonDbStr::itemLocationLabel()).toString());
    if (!label.isEmpty())
        locationDetail->setLabel(label);

    QVariantMap geoObject(object.value(QOrganizerJsonDbStr::itemLocationGeo()).toMap());
    if (geoObject.contains(QOrganizerJsonDbStr::itemLocationGeoLatitude())) {
        QVariant latitude(geoObject.value(QOrganizerJsonDbStr::itemLocationGeoLatitude()));
        if (latitude.canConvert(QVariant::Double))
            locationDetail->setLatitude(latitude.toDouble());
    }
    if (geoObject.contains(QOrganizerJsonDbStr::itemLocationGeoLongitude())) {
        QVariant longitude(geoObject.value(QOrganizerJsonDbStr::itemLocationGeoLongitude()));
        if (longitude.canConvert(QVariant::Double))
            locationDetail->setLongitude(longitude.toDouble());
    }
}

bool QOrganizerJsonDbConverter::jsonDbObjectToCollection(const QVariantMap& object, QOrganizerCollection* collection, bool& isDefaultCollection)
{
    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty()) {
        return false;
    }

    collection->setId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonUuid)));

    QString name = object.value(QOrganizerJsonDbStr::collectionName()).toString();
    if (!name.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyName, name);

    QString description = object.value(QOrganizerJsonDbStr::collectionDescription()).toString();
    if (!description.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyDescription, description);

    QString color = object.value(QOrganizerJsonDbStr::collectionColor()).toString();
    if (!color.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyColor, color);

    QString image = object.value(QOrganizerJsonDbStr::collectionImage()).toString();
    if (!image.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyImage, image);

    QVariantMap jsonMetaData = object.value(QOrganizerJsonDbStr::collectionCustomFields()).toMap();
    if (!jsonMetaData.isEmpty()) {
        QList<QString> metaDataKeys = jsonMetaData.keys();
        foreach (QString key, metaDataKeys)
            collection->setMetaData(key, jsonMetaData.value(key));
    }

    isDefaultCollection = object.value(QOrganizerJsonDbStr::collectionDefaultFlag()).toBool();
    return true;
}

bool QOrganizerJsonDbConverter::collectionToJsonDbObject(const QOrganizerCollection& collection, bool isDefaultCollection, QVariantMap* object) const
{
    if (!collection.id().isNull()) {
        QString jsonUuid = collection.id().toString();
        jsonUuid.remove (QOrganizerJsonDbStr::managerName());
        object->insert(JsonDbString::kUuidStr, jsonUuid);
    }

    object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::collection());
    object->insert(QOrganizerJsonDbStr::collectionDefaultFlag(), isDefaultCollection);

    QVariantMap metaData = collection.metaData();
    QVariantMap jsonMetaData;
    QList<QString> metaDataKeys = metaData.keys();
    foreach (QString key, metaDataKeys) {
        //Jsondb accepts string and stringlist type value
        if (collection.metaData(key).canConvert(QVariant::String)) {
            collection.metaData(key).toString();
            if (key == QOrganizerCollection::KeyName)
                object->insert(QOrganizerJsonDbStr::collectionName(), collection.metaData(QOrganizerCollection::KeyName).toString());
            else if (key == QOrganizerCollection::KeyDescription)
                object->insert(QOrganizerJsonDbStr::collectionDescription(), collection.metaData(QOrganizerCollection::KeyDescription).toString());
            else if (key == QOrganizerCollection::KeyColor)
                object->insert(QOrganizerJsonDbStr::collectionColor(), collection.metaData(QOrganizerCollection::KeyColor).toString());
            else if (key == QOrganizerCollection::KeyImage)
                object->insert(QOrganizerJsonDbStr::collectionImage(), collection.metaData(QOrganizerCollection::KeyImage).toString());
            else
                jsonMetaData.insert(key, metaData.value(key).toString());
        } else if (!collection.metaData(key).canConvert(QVariant::StringList)
                   && key != QOrganizerCollection::KeyName
                   && key != QOrganizerCollection::KeyDescription
                   && key != QOrganizerCollection::KeyColor
                   && key != QOrganizerCollection::KeyImage) {
            jsonMetaData.insert(key, metaData.value(key));
        }
    }

    if (!jsonMetaData.isEmpty())
        object->insert(QOrganizerJsonDbStr::collectionCustomFields(), jsonMetaData);

    return true;
}

void QOrganizerJsonDbConverter::jsonDbObjectToRecurrenceRule(const QVariantMap& object, QOrganizerRecurrenceRule* rule) const
{
    if (object.contains(QOrganizerJsonDbStr::ruleFrequency())) {
        QString frequency = object.value(QOrganizerJsonDbStr::ruleFrequency()).toString();
        rule->setFrequency(static_cast<QOrganizerRecurrenceRule::Frequency>(
                               stringToEnum(qt_organizerFrequencyEnumMap, frequency)));
    }
    if (object.contains(QOrganizerJsonDbStr::ruleInterval())) {
        rule->setInterval(object.value(QOrganizerJsonDbStr::ruleInterval()).toInt());
    }
    if (object.contains(QOrganizerJsonDbStr::ruleLimitCount())) {
        int limitCount = object.value(QOrganizerJsonDbStr::ruleLimitCount()).toInt();
        if (limitCount >= 0)
            rule->setLimit(limitCount);
    }
    if (object.contains(QOrganizerJsonDbStr::ruleLimitDate())) {
        QDate limitDate = object.value(QOrganizerJsonDbStr::ruleLimitDate()).toDate();
        if (limitDate.isValid())
            rule->setLimit(limitDate);
    }
    if (object.contains(QOrganizerJsonDbStr::rulePositions())) {
        QVariantList positionsList = object.value(QOrganizerJsonDbStr::rulePositions()).toList();
        QSet<int> positions;
        foreach (QVariant position, positions)
            positions.insert(position.toInt());
        rule->setPositions(positions);
    }
    if (object.contains(QOrganizerJsonDbStr::ruleFirstDayOfWeek())) {
        QString firstDayOfWeek = object.value(QOrganizerJsonDbStr::ruleFirstDayOfWeek()).toString();
        rule->setFirstDayOfWeek(static_cast<Qt::DayOfWeek>(
                                    stringToEnum(qt_organizerDayEnumMap, firstDayOfWeek)));
    }
    if (object.contains(QOrganizerJsonDbStr::ruleDaysOfWeek())) {
        QVariantList daysOfWeekList = object.value(QOrganizerJsonDbStr::ruleDaysOfWeek()).toList();
        QSet<Qt::DayOfWeek> daysOfWeek;
        foreach (QVariant day, daysOfWeekList)
            daysOfWeek.insert(static_cast<Qt::DayOfWeek>(
                                  stringToEnum(qt_organizerDayEnumMap, day.toString())));
        rule->setDaysOfWeek(daysOfWeek);
    }

    if (object.contains(QOrganizerJsonDbStr::ruleDaysOfMonth())) {
        QVariantList daysOfMonthList = object.value(QOrganizerJsonDbStr::ruleDaysOfMonth()).toList();
        QSet<int> daysOfMonth;
        foreach (QVariant day, daysOfMonthList)
            daysOfMonth.insert(day.toInt());
        rule->setDaysOfMonth(daysOfMonth);
    }

    if (object.contains(QOrganizerJsonDbStr::ruleDaysOfYear())) {
        QVariantList daysOfYearList = object.value(QOrganizerJsonDbStr::ruleDaysOfYear()).toList();
        QSet<int> daysOfYear;
        foreach (QVariant day, daysOfYearList)
            daysOfYear.insert(day.toInt());
        rule->setDaysOfYear(daysOfYear);
    }

    if (object.contains(QOrganizerJsonDbStr::ruleWeeksOfYear())) {
        QVariantList weeksOfYearList = object.value(QOrganizerJsonDbStr::ruleWeeksOfYear()).toList();
        QSet<int> weeksOfYear;
        foreach (QVariant week, weeksOfYearList)
            weeksOfYear.insert(week.toInt());
        rule->setWeeksOfYear(weeksOfYear);
    }

    if (object.contains(QOrganizerJsonDbStr::ruleMonthsOfYear())) {
        QVariantList monthsOfYearList = object.value(QOrganizerJsonDbStr::ruleMonthsOfYear()).toList();
        QSet<QOrganizerRecurrenceRule::Month> monthsOfYear;
        foreach (QVariant month, monthsOfYearList)
            monthsOfYear.insert(static_cast<QOrganizerRecurrenceRule::Month>(
                                    stringToEnum(qt_organizerMonthEnumMap, month.toString())));
        rule->setMonthsOfYear(monthsOfYear);
    }
}

void QOrganizerJsonDbConverter::recurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule& rule, QVariantMap* object) const
{
    object->insert(QOrganizerJsonDbStr::ruleFrequency(), enumToString(qt_organizerFrequencyEnumMap, rule.frequency()));
    object->insert(QOrganizerJsonDbStr::ruleInterval(), rule.interval());
    object->insert(QOrganizerJsonDbStr::ruleLimitCount(), rule.limitCount());
    object->insert(QOrganizerJsonDbStr::ruleLimitDate(), rule.limitDate().toString(Qt::ISODate));
    QSet<int> positions = rule.positions();
    if (!positions.isEmpty()) {
        QVariantList positionsList;
        foreach (int position, positions) {
            positionsList.append(position);
        }
        object->insert(QOrganizerJsonDbStr::rulePositions(), positionsList);
    }

    object->insert(QOrganizerJsonDbStr::ruleFirstDayOfWeek(), enumToString(qt_organizerDayEnumMap, rule.firstDayOfWeek()));

    QSet<Qt::DayOfWeek> daysOfWeek = rule.daysOfWeek();
    if (!daysOfWeek.isEmpty()) {
        QVariantList daysOfWeekList;
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            daysOfWeekList.append(enumToString(qt_organizerDayEnumMap, day));
        }
        object->insert(QOrganizerJsonDbStr::ruleDaysOfWeek(), daysOfWeekList);
    }

    QSet<int> daysOfMonth = rule.daysOfMonth();
    if (!daysOfMonth.isEmpty()) {
        QVariantList daysOfMonthList;
        foreach (int day, daysOfMonth) {
            daysOfMonthList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::ruleDaysOfMonth(), daysOfMonthList);
    }

    QSet<int> daysOfYear = rule.daysOfYear();
    if (!daysOfYear.isEmpty()) {
        QVariantList daysOfYearList;
        foreach (int day, daysOfYear) {
            daysOfYearList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::ruleDaysOfYear(), daysOfYearList);
    }

    QSet<int> weeksOfYear = rule.weeksOfYear();
    if (!weeksOfYear.isEmpty()) {
        QVariantList weeksOfYearList;
        foreach (int week, weeksOfYear) {
            weeksOfYearList.append(week);
        }
        object->insert(QOrganizerJsonDbStr::ruleWeeksOfYear(), weeksOfYearList);
    }

    QSet<QOrganizerRecurrenceRule::Month> monthsOfYear = rule.monthsOfYear();
    if (!monthsOfYear.isEmpty()) {
        QVariantList monthsOfYearList;
        foreach (QOrganizerRecurrenceRule::Month month, monthsOfYear) {
            monthsOfYearList.append(enumToString(qt_organizerMonthEnumMap, month));
        }
        object->insert(QOrganizerJsonDbStr::ruleMonthsOfYear(), monthsOfYearList);
    }
}

bool QOrganizerJsonDbConverter::itemToJsondbAlarmObject(const QOrganizerItem &item, QVariantMap &alarmObject) const
{
    bool ret = false;
    QOrganizerItemAudibleReminder audibleReminder = item.detail(QOrganizerItemAudibleReminder::DefinitionName);
    QDateTime alarmStartTime;
    if (item.type() == QOrganizerItemType::TypeEvent || item.type() == QOrganizerItemType::TypeEventOccurrence)
        alarmStartTime = item.detail(QOrganizerEventTime::DefinitionName).value<QDateTime>(QOrganizerEventTime::FieldStartDateTime);
    else if (item.type() == QOrganizerItemType::TypeTodo || item.type() == QOrganizerItemType::TypeTodoOccurrence)
        alarmStartTime = item.detail(QOrganizerTodoTime::DefinitionName).value<QDateTime>(QOrganizerTodoTime::FieldStartDateTime);

    // audibleReminder and start time details are mandatory for alarm object
    if (!audibleReminder.isEmpty() && alarmStartTime.isValid()) {
        // alarm object
        alarmObject.insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::alarm());
        // alarm start time
        if (audibleReminder.hasValue(audibleReminder.FieldSecondsBeforeStart))
            alarmStartTime = alarmStartTime.addSecs(-audibleReminder.secondsBeforeStart());//Nagetive value
        alarmObject.insert(QOrganizerJsonDbStr::alarmISODate(), alarmStartTime.toUTC().toString(Qt::ISODate));
        // repetition count to alarm snooze count
        if (audibleReminder.hasValue(audibleReminder.FieldRepetitionCount))
           alarmObject.insert(QOrganizerJsonDbStr::alarmSnoozeCount(), audibleReminder.repetitionCount());
        // repetition delay to alarm snooze time
        if (audibleReminder.hasValue(audibleReminder.FieldRepetitionDelay))
           alarmObject.insert(QOrganizerJsonDbStr::alarmSnoozeTime(), audibleReminder.repetitionDelay());
        // item Uuid to alarm event Uuid
        alarmObject.insert(QOrganizerJsonDbStr::alarmEventUuid(), item.id().toString().remove(QOrganizerJsonDbStr::managerName()));
        // item displayLabel to alarm title
        QString displayLabel(item.displayLabel());
        if (!displayLabel.isEmpty())
            alarmObject.insert(QOrganizerJsonDbStr::alarmTitle(), displayLabel);
        // item description to alarm description
        QString description(item.description());
        if (!description.isEmpty())
            alarmObject.insert(QOrganizerJsonDbStr::alarmDescription(), description);
        // set calender app as alarm identifier
        alarmObject.insert(QOrganizerJsonDbStr::alarmIdentifier(), QOrganizerJsonDbStr::alarmCalenderIdentifier());
        ret = true;
    }
    return ret;
}

void QOrganizerJsonDbConverter::audibleReminderDetailToJsonDbObject(const QOrganizerItemAudibleReminder &itemReminder, QVariantMap &object) const
{
    QMap<int, QVariant> reminderValues = itemReminder.values();
    QMap<int, QVariant>::const_iterator i = reminderValues.constBegin();
    QVariant value;
    QString jsonDbField;
    while (i != reminderValues.constEnd()) {
        value.clear();
        if (i.key() == QOrganizerItemReminder::FieldSecondsBeforeStart) {
            value = i.value().toInt();
            jsonDbField = QOrganizerJsonDbStr::itemReminderSecBeforeStart();
        } else if (i.key() == QOrganizerItemReminder::FieldRepetitionCount) {
            value = i.value().toInt();
            jsonDbField = QOrganizerJsonDbStr::itemReminderRepCount();
        } else if (i.key() == QOrganizerItemReminder::FieldRepetitionDelay) {
            value = i.value().toInt();
            jsonDbField = QOrganizerJsonDbStr::itemReminderRepDelay();
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldDataUrl) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::itemReminderDataUrl();
        }
        if (value.isValid())
            object.insert(jsonDbField, value);
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToAudibleReminderDetail(const QVariantMap &object, QOrganizerItemAudibleReminder *itemReminder) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    int field;
    QVariant value;
    while (i != object.constEnd()) {
        value.clear();
        if (i.key() == QOrganizerJsonDbStr::itemReminderSecBeforeStart()) {
            field = QOrganizerItemReminder::FieldSecondsBeforeStart;
            value = i.value().toInt();
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderRepCount()) {
            field = QOrganizerItemReminder::FieldRepetitionCount;
            value = i.value().toInt();
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderRepDelay()) {
            field = QOrganizerItemReminder::FieldRepetitionDelay;
            value = i.value().toInt();
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderDataUrl()) {
            field = QOrganizerItemAudibleReminder::FieldDataUrl;
            value = QUrl(i.value().toString());
        }
        if (value.isValid())
            itemReminder->setValue(field, value);
        ++i;
    }
}

int QOrganizerJsonDbConverter::stringToEnum(const QOrganizerJsonDbEnumConversionData* const conversionData, const QString &enumStr) const
{
    int i = 0;
    while (conversionData[i].enumValue != enumMapEnd) {
        if (conversionData[i].enumStr == enumStr)
            return conversionData[i].enumValue;
        i++;
    }
    // first index contains default values
    return conversionData[0].enumValue;
}

QString QOrganizerJsonDbConverter::enumToString(const QOrganizerJsonDbEnumConversionData* const conversionData, int enumValue) const
{
    int i = 0;
    while (conversionData[i].enumValue != enumMapEnd) {
        if (conversionData[i].enumValue == enumValue)
            return conversionData[i].enumStr;
        i++;
    }
    // first index contains default values
    return conversionData[0].enumStr;
}

void QOrganizerJsonDbConverter::extendedDetailToJsonDbProperty(const QOrganizerItemExtendedDetail &extendedDetail, QVariant &property) const
{
    // TODO check potential conflicts with predefined detail
    if (extendedDetail.name().isEmpty() || extendedDetail.data().isNull())
        return;

    if (extendedDetail.data().canConvert(QVariant::String)) {
        property.setValue(extendedDetail.data().toString());
    } else if (extendedDetail.data().type() == QVariant::List) {
        QVariantList variantList;
        dataToList(extendedDetail.data(), variantList);
        property.setValue(variantList);
    } else if (extendedDetail.data().type() == QVariant::Map) {
        QVariantMap variantMap;
        dataToMap(extendedDetail.data(), variantMap);
        property.setValue(variantMap);
    }
}

void QOrganizerJsonDbConverter::dataToList(const QVariant &data, QVariantList &list) const
{
    if (data.type() != QVariant::List)
        return;

    QList<QVariant> originalList(data.toList());
    foreach (const QVariant &variant, originalList) {
        if (variant.isNull())
            continue;

        if (variant.canConvert(QVariant::String)) {
            list.append(variant.toString());
        } else if (variant.type() == QVariant::List) {
            dataToList(variant, list);
        } else if (variant.type() == QVariant::Map) {
            QVariantMap map;
            dataToMap(variant, map);
            list.append(map);
        }
    }
}

void QOrganizerJsonDbConverter::dataToMap(const QVariant &data, QVariantMap &map) const
{
    if (data.type() != QVariant::Map)
        return;

    QMap<QString, QVariant> originalMap(data.toMap());
    QMap<QString, QVariant>::const_iterator i = originalMap.constBegin();
    while (i != originalMap.constEnd()) {
        if (i.value().isValid() && !i.value().isNull()) {
            if (i.value().canConvert(QVariant::String)) {
                map.insert(i.key(), i.value().toString());
            } else if (i.value().type() == QVariant::List) {
                QVariantList list;
                dataToList(i.value(), list);
                map.insert(i.key(), list);
            } else if (i.value().type() == QVariant::Map) {
                QVariantMap embedMap;
                dataToMap(i.value(), embedMap);
                map.insert(i.key(), embedMap);
            }
        }
        ++i;
    }
}

bool QOrganizerJsonDbConverter::compoundFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const
{
    bool isValidFilter = true;
    switch (filter.type()) {
    case QOrganizerItemFilter::IntersectionFilter: {
        const QOrganizerItemIntersectionFilter isf(filter);
        const QList<QOrganizerItemFilter> filterList = isf.filters();
        foreach (const QOrganizerItemFilter &filter, filterList){
            //query filter1 filter2 filter3 ...
            //query [?definition="value"][?definition="value"][?definition="value"]
            QString filterStr;
            if (compoundFilterToJsondbQuery(filter, filterStr))
                jsonDbQueryStr += filterStr;
            else //For intersection filter, single filter invalid means empty result from jsondb query
                isValidFilter = false;
        }
        return isValidFilter;
    }
    case QOrganizerItemFilter::UnionFilter: {
        const QOrganizerItemUnionFilter uf(filter);
        const QList<QOrganizerItemFilter> filterList = uf.filters();
        int validFilterCount = 0;
        foreach (const QOrganizerItemFilter &filter, filterList){
            //query filter1 filter2 filter3 ...
            //query [?definition="value" | definition="value" | definition="value"]
            QString filterStr;
            if (compoundFilterToJsondbQuery(filter, filterStr)) {
                jsonDbQueryStr += filterStr;
                validFilterCount ++;
            } else {//For union filter, single filter invalid means we could skip this filter
                continue;
            }
        }
        if (validFilterCount > 0)
            jsonDbQueryStr.replace(QStringLiteral("][?"), QStringLiteral(" | ")); //replace the "][?" to " | "
        else //no valid filter means empty item list from jsondb
            isValidFilter = false;
        return isValidFilter;
    }
    default:
        isValidFilter = singleFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    }

    if (!isValidFilter)
        jsonDbQueryStr.clear();

    return isValidFilter;
}

bool QOrganizerJsonDbConverter::singleFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString& jsonDbQueryStr) const
{
    bool isValidFilter = true;
    switch (filter.type()) {
    case QOrganizerItemFilter::CollectionFilter:
        isValidFilter = collectionFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    case QOrganizerItemFilter::IdFilter:
        isValidFilter = idFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    case QOrganizerItemFilter::DetailFilter:
        isValidFilter = detailFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    default:
        break;
    }

    return isValidFilter;
}

QString QOrganizerJsonDbConverter::jsonDbNotificationObjectToOrganizerType(const QVariantMap &object) const
{
    return object.value(JsonDbString::kTypeStr).toString();
}

QOrganizerItemId QOrganizerJsonDbConverter::jsonDbNotificationObjectToItemId(const QVariantMap &object) const
{
    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty())
        return QOrganizerItemId();
    else
        return QOrganizerItemId(new QOrganizerJsonDbItemId(jsonUuid));
}

QOrganizerCollectionId QOrganizerJsonDbConverter::jsonDbNotificationObjectToCollectionId(const QVariantMap &object) const
{
    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty())
        return QOrganizerCollectionId();
    else
        return QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonUuid));
}

bool QOrganizerJsonDbConverter::collectionFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const
{
    bool isValidFilter = true;
    const QOrganizerItemCollectionFilter cf(filter);
    const QSet<QOrganizerCollectionId>& ids = cf.collectionIds();
    if (!ids.empty()) {
        const QString idTemplate(QStringLiteral("\"%1\","));
        QString query;
        foreach (const QOrganizerCollectionId &id, ids) {
            if (!id.isNull())
                query += idTemplate.arg(id.toString().remove(QOrganizerJsonDbStr::managerName()));
        }
        if (!query.isEmpty()) {
            query.truncate(query.length() - 1);
            jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryCollectionIdsTemplate().arg(query);
        } else {
            isValidFilter = false;
        }
    } else {
        isValidFilter = false;
    }
    return isValidFilter;
}

bool QOrganizerJsonDbConverter::idFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const
{
    bool isValidFilter = true;
    const QOrganizerItemIdFilter idf(filter);
    const QList<QOrganizerItemId>& ids = idf.ids();
    if (!ids.empty()) {
        const QString uuidTemplate(QStringLiteral("\"%1\","));
        QString query;
        foreach (const QOrganizerItemId &id, ids) {
            if (!id.isNull())
                query += uuidTemplate.arg(id.toString().remove(QOrganizerJsonDbStr::managerName()));
        }
        if (!query.isEmpty()) {
            query.truncate(query.length() - 1);
            jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryUuidsTemplate().arg(query);
        } else {
            isValidFilter = false;
        }
    } else {
        isValidFilter = false;
    }
    return isValidFilter;
}

bool QOrganizerJsonDbConverter::isSupportedDetailFilter(
    const QOrganizerItemDetailFilter &filter, QOrganizerItemDetail::DetailType detailType, int detailFieldName) const
{
    bool isValidFilter = true;

    if (detailType == QOrganizerItemDetail::TypeUndefined && detailFieldName == -1 && filter.value().toString().isEmpty()) {
        // no support when any of the fields is empty
        isValidFilter = false;
    } else if (QOrganizerJournalTime::DefinitionName == detailType
        || QOrganizerTodoProgress::DefinitionName == detailType
        || QOrganizerItemReminder::DefinitionName == detailType
        || QOrganizerItemAudibleReminder::DefinitionName == detailType
        || QOrganizerItemVisualReminder::DefinitionName == detailType
        || QOrganizerItemEmailReminder::DefinitionName == detailType
        || QOrganizerItemParent::DefinitionName == detailType
        || QOrganizerItemRecurrence::DefinitionName == detailType
        || QOrganizerItemTimestamp::DefinitionName == detailType
        || QOrganizerEventAttendee::DefinitionName == detailType) {
        // filtering certain details/definitions are currently not supported
        isValidFilter = false;
    } else if (QOrganizerItemFilter::MatchExactly != filter.matchFlags()
        && (QOrganizerEventTime::DefinitionName == detailType
        || QOrganizerTodoTime::DefinitionName == detailType
        || QOrganizerItemComment::DefinitionName == detailType
        || (QOrganizerItemLocation::DefinitionName == detailType && (QOrganizerItemLocation::FieldLatitude == detailFieldName || QOrganizerItemLocation::FieldLongitude == detailFieldName))
        || QOrganizerItemPriority::DefinitionName == detailType
        || QOrganizerItemType::DefinitionName == detailType
        || QOrganizerItemTag::DefinitionName == detailType
        || QOrganizerItemExtendedDetail::DefinitionName == detailType
        || (QOrganizerEventRsvp::DefinitionName == detailType && (QOrganizerEventRsvp::FieldParticipationStatus == detailFieldName
                                                                            || QOrganizerEventRsvp::FieldParticipationRole == detailFieldName
                                                                            || QOrganizerEventRsvp::FieldResponseRequirement == detailFieldName
                                                                            || QOrganizerEventRsvp::FieldResponseDeadline == detailFieldName
                                                                            || QOrganizerEventRsvp::FieldResponseDate == detailFieldName)))) {
        // filtering matchflags are not supported for all the types
        isValidFilter = false;
    } else if (QVariant::String == filter.value().type()
        && !(QOrganizerItemComment::DefinitionName == detailType
        || QOrganizerItemDescription::DefinitionName == detailType
        || QOrganizerItemDisplayLabel::DefinitionName == detailType
        || QOrganizerItemGuid::DefinitionName == detailType
        || (QOrganizerItemLocation::DefinitionName == detailType && QOrganizerItemLocation::FieldLabel == detailFieldName)
        || QOrganizerItemTag::DefinitionName == detailType
        || QOrganizerItemExtendedDetail::DefinitionName == detailType
        || QOrganizerItemType::DefinitionName == detailType
        || (QOrganizerEventRsvp::DefinitionName == detailType && (QOrganizerEventRsvp::FieldOrganizerEmail == detailFieldName
                                                                            || QOrganizerEventRsvp::FieldOrganizerName == detailFieldName)))) {
        // filtering with QString needs extra attention, not allowed for all the types
        isValidFilter = false;
    }
    return isValidFilter;
}

bool QOrganizerJsonDbConverter::detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const
{
    /*
    Jsondb backend specific notes related to OrganizerItemDetailFilter

    Detail specific:
    - Currently supported details; EventTime, TodoTime, Comment, Description, DisplayLabel,
      Gui, Location, Priority, Type, Tag and Customized(ExtendedDetail).
    - Type-detail is mapped from enum to string, since the C++ side is using strings.
    - Comment- and Tag-details can only be filtered with MatchExactly. No wildcards supported.
    - Customized(ExtendedDetail)-detail can only be used to filter the custom field name, not the data value.
      Used custom field names should be simple and clear, no spaces or special characters. Matchflags
      are not supported either, only exact matching works.
    - No support to filter based only on detail-type without any value. Like filter everything which has Priority-field
      regardless of the value of the field.

    Matchflags:
    - MatchExactly:
      -Works with all the supported detail types/fields (above)
      -Only same object type supported for filtering, ie strings as Dates are not supported
    - MatchFixedString
      - Ignores any wildcard flags as filters as exact string
    - MatchContains, MatchEndsWith, MatchStartsWith, MatchCaseSensitive
      - These are supported only for details having string as a value type
      - Works only if MatchExactly or MatchFixedString are not set
    */

    const QOrganizerItemDetailFilter df(filter);
    const QOrganizerItemDetail::DetailType detailType(df.detailType());
    const int detailField(df.detailField());
    const QString valueString(df.value().toString());

    bool isValidFilter = isSupportedDetailFilter(filter, detailType, detailField);

    if (isValidFilter) { // proceed with jsondb query construction, if filter is valid
        const QString equalsQueryTemplate(QStringLiteral("[?%1=\"%2\"]"));
        const QString containsQueryTemplate(QStringLiteral("[?%1 contains \"%2\"]"));
        const QString matchFlagQueryTemplate(QStringLiteral("[?%1%2\"]"));
        const QString matchFlagQueryTemplate2(QStringLiteral("[?%1.%2%3\"]"));
        const QString matchFlagQueryTemplate3(QStringLiteral("[?%1.%2.%3%4\"]"));
        const QString existsQueryTemplate(QStringLiteral("[?%1 exists]"));

        if (QOrganizerEventTime::DefinitionName == detailType) {
            if (QOrganizerEventTime::FieldStartDateTime ==  detailField) {
                jsonDbQueryStr += equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventStartDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate));
            } else if (QOrganizerEventTime::FieldEndDateTime == detailField) {
                jsonDbQueryStr += equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventEndDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate));
            } else if (QOrganizerEventTime::FieldAllDay == detailField) {
                jsonDbQueryStr += equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventIsAllDay()).arg(valueString);
            }

        } else if (QOrganizerTodoTime::DefinitionName == detailType) {
            if (QOrganizerTodoTime::FieldStartDateTime == detailField) {
                jsonDbQueryStr += equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoStartDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate));
            } else if (QOrganizerTodoTime::FieldDueDateTime == detailField) {
                jsonDbQueryStr += equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoDueDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate));
            } else if (QOrganizerTodoTime::FieldAllDay == detailField) {
                jsonDbQueryStr += equalsQueryTemplate.arg(QOrganizerJsonDbStr::todoIsAllDay()).arg(valueString);
            }

        } else if (QOrganizerItemComment::DefinitionName == detailType
            && QOrganizerItemComment::FieldComment == detailField) {
            jsonDbQueryStr += containsQueryTemplate.arg(QOrganizerJsonDbStr::itemComments()).arg(valueString);

        } else if (QOrganizerItemDescription::DefinitionName == detailType
            && QOrganizerItemDescription::FieldDescription == detailField) {
            jsonDbQueryStr += matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemDescription()).arg(createMatchFlagQuery(valueString, df.matchFlags()));

        } else if (QOrganizerItemDisplayLabel::DefinitionName == detailType
            && QOrganizerItemDisplayLabel::FieldLabel == detailField) {
           jsonDbQueryStr += matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemName())
                .arg(createMatchFlagQuery(valueString, df.matchFlags()));

        } else if (QOrganizerItemGuid::DefinitionName == detailType
            && QOrganizerItemGuid::FieldGuid ==  detailField) {
            jsonDbQueryStr += matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemGuid())
                .arg(createMatchFlagQuery(valueString, df.matchFlags()));

        } else if (QOrganizerItemLocation::DefinitionName == detailType) {
            if (QOrganizerItemLocation::FieldLabel ==  detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                    .arg(QOrganizerJsonDbStr::itemLocation())
                    .arg(QOrganizerJsonDbStr::itemLocationLabel())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags()));
            } else if (QOrganizerItemLocation::FieldLongitude ==  detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate3
                    .arg(QOrganizerJsonDbStr::itemLocation())
                    .arg(QOrganizerJsonDbStr::itemLocationGeo())
                    .arg(QOrganizerJsonDbStr::itemLocationGeoLongitude())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags()));
            } else if (QOrganizerItemLocation::FieldLatitude ==  detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate3
                    .arg(QOrganizerJsonDbStr::itemLocation())
                    .arg(QOrganizerJsonDbStr::itemLocationGeo())
                    .arg(QOrganizerJsonDbStr::itemLocationGeoLatitude())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags()));
            }

        } else if (QOrganizerItemPriority::DefinitionName == detailType
            && QOrganizerItemPriority::FieldPriority ==  detailField) {
            jsonDbQueryStr += equalsQueryTemplate
                .arg(QOrganizerJsonDbStr::itemPriority())
                .arg(enumToString(qt_organizerPriorityEnumMap, df.value().toInt()));

        } else if (QOrganizerItemType::DefinitionName == detailType
            && QOrganizerItemType::FieldType ==  detailField) {
            jsonDbQueryStr += equalsQueryTemplate
                .arg(JsonDbString::kTypeStr)
                .arg(QOrganizerJsonDbStr::jsonDbSchemaPrefix() + enumToString(qt_organizerItemTypeMap, df.value().toInt()));

        } else if (QOrganizerItemTag::DefinitionName == detailType
            && QOrganizerItemTag::FieldTag == detailField) {
            jsonDbQueryStr += containsQueryTemplate.arg(QOrganizerJsonDbStr::itemTags()).arg(valueString);

        } else if (QOrganizerItemExtendedDetail::DefinitionName == detailType
             && QOrganizerItemExtendedDetail::FieldExtendedDetailName ==  detailField) {
            jsonDbQueryStr += existsQueryTemplate.arg(valueString);

        } else if (QOrganizerEventRsvp::DefinitionName == detailType) {
            if (QOrganizerEventRsvp::FieldParticipationStatus == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::attendeeParticipationStatus())
                        .arg(createMatchFlagQuery(enumToString(qt_organizerParticipationStatusMap, df.value().toInt()), df.matchFlags()));
            } else if (QOrganizerEventRsvp::FieldParticipationRole == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::attendeeParticipationRole())
                        .arg(createMatchFlagQuery(enumToString(qt_organizerParticipationRoleMap, df.value().toInt()), df.matchFlags()));
            } else if (QOrganizerEventRsvp::FieldResponseRequirement == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpResponseRequirement())
                        .arg(createMatchFlagQuery(enumToString(qt_organizerResponseRequirementMap, df.value().toInt()), df.matchFlags()));
            } else if (QOrganizerEventRsvp::FieldResponseDeadline == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpResponseDeadline())
                        .arg(createMatchFlagQuery(df.value().toDate().toString(Qt::ISODate), df.matchFlags()));
            } else if (QOrganizerEventRsvp::FieldResponseDate == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpResponseDate())
                        .arg(createMatchFlagQuery(df.value().toDate().toString(Qt::ISODate), df.matchFlags()));
            } else if (QOrganizerEventRsvp::FieldOrganizerName == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpOrganizerName())
                        .arg(createMatchFlagQuery(valueString, df.matchFlags()));
            } else if (QOrganizerEventRsvp::FieldOrganizerEmail == detailField) {
                jsonDbQueryStr += matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpOrganizerEmail())
                        .arg(createMatchFlagQuery(valueString, df.matchFlags()));
            }
        }
    }
    return isValidFilter;
}

QString QOrganizerJsonDbConverter::createMatchFlagQuery(const QString &value, QOrganizerItemFilter::MatchFlags flags) const
{
    // with current enumeration, 3 means both MatchContains and MatchEndsWith (QTBUG-20035)
    const uint realMatchType = flags & 0x0F;
    QString queryWithWildCards((QOrganizerItemFilter::MatchExactly == flags || QOrganizerItemFilter::MatchFixedString & flags)
        ? QStringLiteral("=\"") : QStringLiteral("=~\"/"));
    queryWithWildCards += (QOrganizerItemFilter::MatchContains == realMatchType || QOrganizerItemFilter::MatchEndsWith == realMatchType)
        ? QStringLiteral("*") : QStringLiteral("");
    queryWithWildCards += value;
    queryWithWildCards += (QOrganizerItemFilter::MatchContains == realMatchType || QOrganizerItemFilter::MatchStartsWith == realMatchType)
        ? QStringLiteral("*") : QStringLiteral("");

    if (QOrganizerItemFilter::MatchExactly != flags && !(QOrganizerItemFilter::MatchFixedString & flags)) {
        queryWithWildCards += QStringLiteral("/w");
        queryWithWildCards += QOrganizerItemFilter::MatchCaseSensitive & flags ? QStringLiteral("") : QStringLiteral("i");
    }

    return queryWithWildCards;
}

QTORGANIZER_END_NAMESPACE
