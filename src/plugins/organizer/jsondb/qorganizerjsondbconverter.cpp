/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qorganizerjsondbconverter.h"
#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring.h"
#include "qorganizer.h"

Q_USE_JSONDB_NAMESPACE

QTORGANIZER_BEGIN_NAMESPACE

const int QOrganizerJsonDbConverter::enumMapEnd(-1212);

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerPriorityEnumMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerItemPriority::UnknownPriority,        QString(QStringLiteral("UnknownPriority"))},
        {QOrganizerItemPriority::ExtremelyHighPriority,  QString(QStringLiteral("HighestPriority"))},
        {QOrganizerItemPriority::ExtremelyHighPriority,  QString(QStringLiteral("ExtremelyHighPriority"))},
        {QOrganizerItemPriority::VeryHighPriority,       QString(QStringLiteral("VeryHighPriority"))},
        {QOrganizerItemPriority::HighPriority,           QString(QStringLiteral("HighPriority"))},
        {QOrganizerItemPriority::MediumPriority,         QString(QStringLiteral("MediumPriority"))},
        {QOrganizerItemPriority::LowPriority,            QString(QStringLiteral("LowPriority"))},
        {QOrganizerItemPriority::VeryLowPriority,        QString(QStringLiteral("VeryLowPriority"))},
        {QOrganizerItemPriority::ExtremelyLowPriority,   QString(QStringLiteral("ExtremelyLowPriority"))},
        {QOrganizerItemPriority::LowestPriority,         QString(QStringLiteral("LowestPriority"))},
        {enumMapEnd,                                     QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerFrequencyEnumMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerRecurrenceRule::Invalid,              QString(QStringLiteral("Invalid"))},
        {QOrganizerRecurrenceRule::Daily,                QString(QStringLiteral("Daily"))},
        {QOrganizerRecurrenceRule::Weekly,               QString(QStringLiteral("Weekly"))},
        {QOrganizerRecurrenceRule::Monthly,              QString(QStringLiteral("Monthly"))},
        {QOrganizerRecurrenceRule::Yearly,               QString(QStringLiteral("Yearly"))},
        {enumMapEnd,                                     QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerDayEnumMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {Qt::Monday,                                     QString(QStringLiteral("Monday"))},
        {Qt::Tuesday,                                    QString(QStringLiteral("Tuesday"))},
        {Qt::Wednesday,                                  QString(QStringLiteral("Wednesday"))},
        {Qt::Thursday,                                   QString(QStringLiteral("Thursday"))},
        {Qt::Friday,                                     QString(QStringLiteral("Friday"))},
        {Qt::Saturday,                                   QString(QStringLiteral("Saturday"))},
        {Qt::Sunday,                                     QString(QStringLiteral("Sunday"))},
        {enumMapEnd,                                     QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerMonthEnumMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerRecurrenceRule::January,              QString(QStringLiteral("January"))},
        {QOrganizerRecurrenceRule::February,             QString(QStringLiteral("February"))},
        {QOrganizerRecurrenceRule::March,                QString(QStringLiteral("March"))},
        {QOrganizerRecurrenceRule::April,                QString(QStringLiteral("April"))},
        {QOrganizerRecurrenceRule::May,                  QString(QStringLiteral("May"))},
        {QOrganizerRecurrenceRule::June,                 QString(QStringLiteral("June"))},
        {QOrganizerRecurrenceRule::July,                 QString(QStringLiteral("July"))},
        {QOrganizerRecurrenceRule::August,               QString(QStringLiteral("August"))},
        {QOrganizerRecurrenceRule::September,            QString(QStringLiteral("September"))},
        {QOrganizerRecurrenceRule::October,              QString(QStringLiteral("October"))},
        {QOrganizerRecurrenceRule::November,             QString(QStringLiteral("November"))},
        {QOrganizerRecurrenceRule::December,             QString(QStringLiteral("December"))},
        {enumMapEnd,                                     QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerParticipationStatusMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerEventAttendee::StatusUnknown,         QString(QStringLiteral("Unknown"))},
        {QOrganizerEventAttendee::StatusAccepted,        QString(QStringLiteral("Accepted"))},
        {QOrganizerEventAttendee::StatusDeclined,        QString(QStringLiteral("Declined"))},
        {QOrganizerEventAttendee::StatusTentative,       QString(QStringLiteral("Tentative"))},
        {QOrganizerEventAttendee::StatusDelegated,       QString(QStringLiteral("Delegated"))},
        {QOrganizerEventAttendee::StatusInProcess,       QString(QStringLiteral("InProcess"))},
        {QOrganizerEventAttendee::StatusCompleted,       QString(QStringLiteral("Completed"))},
        {enumMapEnd,                                     QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerParticipationRoleMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerEventAttendee::RoleUnknown,             QString(QStringLiteral("Unknown"))},
        {QOrganizerEventAttendee::RoleOrganizer,           QString(QStringLiteral("Organizer"))},
        {QOrganizerEventAttendee::RoleChairperson,         QString(QStringLiteral("Chairperson"))},
        {QOrganizerEventAttendee::RoleHost,                QString(QStringLiteral("Host"))},
        {QOrganizerEventAttendee::RoleRequiredParticipant, QString(QStringLiteral("RequiredParticipant"))},
        {QOrganizerEventAttendee::RoleOptionalParticipant, QString(QStringLiteral("OptionalParticipant"))},
        {QOrganizerEventAttendee::RoleNonParticipant,      QString(QStringLiteral("NonParticipant"))},
        {enumMapEnd,                                       QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerResponseRequirementMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerEventRsvp::ResponseNotRequired,         QString(QStringLiteral("NotRequired"))},
        {QOrganizerEventRsvp::ResponseRequired,            QString(QStringLiteral("Required"))},
        {enumMapEnd,                                       QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerItemTypeMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerItemType::TypeUndefined,                 QString(QStringLiteral("Unknown"))},
        {QOrganizerItemType::TypeEvent,                     QString(QStringLiteral("Event"))},
        {QOrganizerItemType::TypeEventOccurrence,           QString(QStringLiteral("EventOccurrence"))},
        {QOrganizerItemType::TypeTodo,                      QString(QStringLiteral("Todo"))},
        {QOrganizerItemType::TypeTodoOccurrence,            QString(QStringLiteral("TodoOccurrence"))},
        {QOrganizerItemType::TypeJournal,                   QString(QStringLiteral("Journal"))},
        {QOrganizerItemType::TypeNote,                      QString(QStringLiteral("Note"))},
        {enumMapEnd,                                        QString::null}
    };
    return map;
}

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerTodoStatusMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerTodoProgress::StatusNotStarted,          QString(QStringLiteral("NotStarted"))},
        {QOrganizerTodoProgress::StatusInProgress,          QString(QStringLiteral("InProgress"))},
        {QOrganizerTodoProgress::StatusComplete,            QString(QStringLiteral("Complete"))},
        {enumMapEnd,                                        QString::null}
    };
    return map;
}

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

    case JsonDbError::InvalidRequest:// update request with invalid uuid
    case JsonDbError::MissingObject:// update request with invalid uuid
    case JsonDbError::MissingType://Remove no-exist item get this error from jsondb
    case JsonDbError::MissingUUID:
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
    QVariantMap objectToParse;

    // must handle type before reaching the loop
    QString jsonDbType = object.value(QOrganizerJsonDbStr::jsonDbType()).toString();
    if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventType()) {
        item->setType(QOrganizerItemType::TypeEvent);
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbTodoType()) {
        item->setType(QOrganizerItemType::TypeTodo);
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventViewType()) {
        item->setType(QOrganizerItemType::TypeEvent);
        item->setExtendedDetailData(QOrganizerJsonDbStr::eventIsSynthetic(), true);

        // the data is stored in the "value" field, so dirty code here ;)
        objectToParse = object.value(QOrganizerJsonDbStr::jsonDbValue()).toMap();
        objectToParse.insert(QOrganizerJsonDbStr::jsonDbUuid(), object.value(QOrganizerJsonDbStr::jsonDbUuid()));
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventOccurrenceType()) {
        item->setType(QOrganizerItemType::TypeEventOccurrence);
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbTodoOccurrenceType()) {
        item->setType(QOrganizerItemType::TypeTodoOccurrence);
    } else {
        return false;
    }

    if (objectToParse.isEmpty())
        objectToParse = object;

    // other mandatory fields
    bool hasCollectionId(false);
    bool hasGuid(false);
    bool hasItemId(false);
    bool hasItemVersion(false);

    // go through all fields
    QMap<QString, QVariant>::const_iterator i = objectToParse.constBegin();
    while (i != objectToParse.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
            QString jsonDbUuid = i.value().toString();
            if (jsonDbUuid.isEmpty())
                return false;
            item->setId(QOrganizerItemId(new QOrganizerJsonDbItemId(jsonDbUuid)));
            hasItemId = true;
        } else if (i.key() == QOrganizerJsonDbStr::itemCollectionUuid()) {
            QString jsonDbCollectionId = i.value().toString();
            if (jsonDbCollectionId.isEmpty())
                return false;
            item->setCollectionId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonDbCollectionId)));
            hasCollectionId = true;
        } else if (i.key() == QOrganizerJsonDbStr::itemGuid()) {
            QString guid = i.value().toString();
            if (guid.isEmpty())
                return false;
            item->setGuid(guid);
            hasGuid = true;
        } else if (i.key() == QOrganizerJsonDbStr::itemDisplayName()) {
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
                priority.setPriority(static_cast<QOrganizerItemPriority::Priority>(stringToEnum(organizerPriorityEnumMap(), jsonDbPriority)));
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
        } else if (i.key() == QOrganizerJsonDbStr::itemExceptionDates()) {
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
                if (item->type() == QOrganizerItemType::TypeEvent
                        || item->type() == QOrganizerItemType::TypeEventOccurrence) {
                    QOrganizerEventTime eventTime = item->detail(QOrganizerEventTime::DefinitionName);
                    eventTime.setStartDateTime(startTime);
                    item->saveDetail(&eventTime);
                } else if (item->type() == QOrganizerItemType::TypeTodo
                           || item->type() == QOrganizerItemType::TypeTodoOccurrence) {
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
        } else if (i.key() == QOrganizerJsonDbStr::eventAttendees()) {
            QVariantList attendeeObjectList = i.value().toList();
            foreach (const QVariant &attendeeObject, attendeeObjectList) {
                QOrganizerEventAttendee attendee;
                jsonDbObjectToAttendeeDetail(attendeeObject.toMap(), &attendee);
                if (!attendee.isEmpty())
                    item->saveDetail(&attendee);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvp()) {
            QVariantMap rsvpMap = i.value().toMap();
            if (!rsvpMap.isEmpty()) {
                // custom fields are supported for RSVP
                QOrganizerEventRsvp rsvp;
                QOrganizerItemExtendedDetail extendedDetail;
                jsonDbObjectToRsvpDetail(rsvpMap, &rsvp, &extendedDetail);
                if (!rsvp.isEmpty())
                    item->saveDetail(&rsvp);
                if (!extendedDetail.isEmpty())
                    item->saveDetail((&extendedDetail));
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemOccurrenceParent()) {
            QOrganizerItemParent parentDetail = item->detail(QOrganizerItemParent::DefinitionName);
            parentDetail.setParentId(QOrganizerItemId(new QOrganizerJsonDbItemId(i.value().toString())));
            item->saveDetail(&parentDetail);
        } else if (i.key() == QOrganizerJsonDbStr::itemOccurrenceOriginalDate()) {
            QDate originalDate = QDate::fromString(i.value().toString(), Qt::ISODate);
            if (originalDate.isValid()) {
                QOrganizerItemParent parentDetail = item->detail(QOrganizerItemParent::DefinitionName);
                parentDetail.setOriginalDate(originalDate);
                item->saveDetail(&parentDetail);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemReminder()) {
            QVariantMap reminderMap = i.value().toMap();
            if (!reminderMap.isEmpty()) {
                // custom fields are supported for reminders
                QOrganizerItemAudibleReminder audibleReminder;
                QOrganizerItemExtendedDetail extendedDetail;
                jsonDbObjectToAudibleReminderDetail(reminderMap, &audibleReminder, &extendedDetail);
                if (!audibleReminder.isEmpty())
                    item->saveDetail(&audibleReminder);
                if (!extendedDetail.isEmpty())
                    item->saveDetail((&extendedDetail));
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventLocation()) {
            QVariantMap locationMap = i.value().toMap();
            if (!locationMap.isEmpty()) {
                // custom fields are supported for reminders
                QOrganizerItemLocation location;
                QOrganizerItemExtendedDetail extendedDetail;
                jsonDbObjectToLocationDetail(locationMap, &location, &extendedDetail);
                if (!location.isEmpty())
                    item->saveDetail(&location);
                if (!extendedDetail.isEmpty())
                    item->saveDetail((&extendedDetail));
            }
        } else if (i.key() == QOrganizerJsonDbStr::jsonDbVersion()) {
            QOrganizerItemVersion itemVersion;
            jsonDbVersionToItemVersion(i.value().toString(), &itemVersion);
            if (!itemVersion.isEmpty()) {
                item->saveDetail(&itemVersion);
                hasItemVersion = true;
            }
        } else if (i.key() == QOrganizerJsonDbStr::todoFinishedDateTime()) {
            QDateTime finishedDateTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (finishedDateTime.isValid()) {
                QOrganizerTodoProgress todoProgress = item->detail(QOrganizerItemDetail::TypeTodoProgress);
                todoProgress.setFinishedDateTime(finishedDateTime);
                item->saveDetail(&todoProgress);
            }
        } else if (i.key() == QOrganizerJsonDbStr::todoProgressPercentage()) {
            int progressPercentage = i.value().toInt();
            if (progressPercentage >= 0 && progressPercentage <= 100) {
                QOrganizerTodoProgress todoProgress = item->detail(QOrganizerItemDetail::TypeTodoProgress);
                todoProgress.setPercentageComplete(progressPercentage);
                item->saveDetail(&todoProgress);
            }
        } else if (i.key() == QOrganizerJsonDbStr::todoStatus()) {
            QOrganizerTodoProgress::Status todoStatus =
                    static_cast<QOrganizerTodoProgress::Status>(stringToEnum(organizerTodoStatusMap(), i.value().toString()));
            QOrganizerTodoProgress todoProgress = item->detail(QOrganizerItemDetail::TypeTodoProgress);
            todoProgress.setStatus(todoStatus);
            item->saveDetail(&todoProgress);
        } else if (i.key() == QOrganizerJsonDbStr::jsonDbType()) {
            // skip already handled before the loop
        }  else if (i.key().at(0) == QChar('_')) {
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

    // view object is guaranteed to be correct when generated, and missing several mandatory fields as nomral objects
    if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventViewType())
        return true;

    return hasCollectionId && hasGuid && hasItemId && hasItemVersion;
}

bool QOrganizerJsonDbConverter::itemToJsonDbObject(const QOrganizerItem& item, QVariantMap* object) const
{
    const QList<QOrganizerItemDetail> details = item.details();

    // the first detail should always be item type
    QOrganizerItemType::ItemType itemType = static_cast<QOrganizerItemType::ItemType>(details.at(0).value(QOrganizerItemType::FieldType).toInt());
    switch (itemType) {
    case QOrganizerItemType::TypeEvent:
        object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::jsonDbEventType());
        break;

    case QOrganizerItemType::TypeEventOccurrence:
        object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::jsonDbEventOccurrenceType());
        break;

    case QOrganizerItemType::TypeTodo:
        object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::jsonDbTodoType());
        break;

    case QOrganizerItemType::TypeTodoOccurrence:
        object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::jsonDbTodoOccurrenceType());
        break;

//    case QOrganizerItemType::TypeUndefined:
//    case QOrganizerItemType::TypeJournal:
//    case QOrganizerItemType::TypeNote:
    default:
        return false;
    }

    // item ID
    if (!item.id().isNull())
        object->insert(QOrganizerJsonDbStr::jsonDbUuid(), QOrganizerManagerEngine::engineItemId(item.id())->toString());

    // collection ID has already been generated in QOrganizerJsonDbRequestThread::handleItemSaveRequest() if needed
    object->insert(QOrganizerJsonDbStr::itemCollectionUuid(),
                   QOrganizerManagerEngine::engineCollectionId(item.collectionId())->toString());


    // certain details that allow multiple instances
    QStringList comments;
    QStringList tags;
    QVariantList attendees;

    // go through all the supported details
    for (int i = 1; i < details.size(); ++i) {
        switch (details.at(i).type()) {
        case QOrganizerItemDetail::TypeComment: {
            QString comment = details.at(i).value(QOrganizerItemComment::FieldComment).toString();
            if (!comment.isEmpty())
                comments.append(comment);
            break;
        }

        case QOrganizerItemDetail::TypeDescription: {
            QString description = details.at(i).value(QOrganizerItemDescription::FieldDescription).toString();
            if (!description.isEmpty())
                object->insert(QOrganizerJsonDbStr::itemDescription(), description);
            break;
        }

        case QOrganizerItemDetail::TypeDisplayLabel: {
            QString displayLabel = details.at(i).value(QOrganizerItemDisplayLabel::FieldLabel).toString();
            if (!displayLabel.isEmpty())
                object->insert(QOrganizerJsonDbStr::itemDisplayName(), displayLabel);
            break;
        }

        case QOrganizerItemDetail::TypeExtendedDetail: {
            QVariant property;
            extendedDetailToJsonDbProperty(details.at(i), &property);
            if (!property.isNull()) {
                // custom fields are allowed for reminder, rsvp, and location
                // in such cases, those values in extended details will be merged with reminder (or other) object in JsonDb
                // for all other cases, the extended detail is stored as "name: data" in JsonDb
                QString name = details.at(i).value(QOrganizerItemExtendedDetail::FieldExtendedDetailName).toString();
                if (name == QOrganizerJsonDbStr::itemReminder()
                    || (itemType == QOrganizerItemType::TypeEvent && (name == QOrganizerJsonDbStr::eventRsvp()
                                                                      || name == QOrganizerJsonDbStr::eventLocation()))) {
                    QVariantMap existing = object->value(name).toMap();
                    if (existing.isEmpty()) {
                        object->insert(name, property);
                    } else {
                        QVariantMap::const_iterator i = existing.constBegin();
                        QVariantMap newExtendedDetailProperty = property.toMap();
                        while (i != existing.constEnd()) {
                            newExtendedDetailProperty.insert(i.key(), i.value());
                            ++i;
                        }
                        object->insert(name, newExtendedDetailProperty);
                    }
                    break;
                }
                object->insert(name, property);
            }
            break;
        }

        case QOrganizerItemDetail::TypeGuid:
            // GUID has already been generated in QOrganizerJsonDbRequestThread::handleItemSaveRequest() if needed
            object->insert(QOrganizerJsonDbStr::itemGuid(), details.at(i).value(QOrganizerItemGuid::FieldGuid).toString());
            break;

        case QOrganizerItemDetail::TypeLocation: {
            // in JsonDb, only events can have locations
            if (itemType != QOrganizerItemType::TypeEvent)
                break;
            QVariantMap locationObject;
            locationDetailToJsonDbObject(details.at(i), &locationObject);
            if (!locationObject.isEmpty()) {
                // check for possible custom fields
                QVariantMap location = object->value(QOrganizerJsonDbStr::eventLocation()).toMap();
                if (location.isEmpty()) {
                    object->insert(QOrganizerJsonDbStr::eventLocation(), locationObject);
                } else {
                    QVariantMap::const_iterator i = locationObject.constBegin();
                    while (i != locationObject.constEnd()) {
                        location.insert(i.key(), i.value());
                        ++i;
                    }
                    object->insert(QOrganizerJsonDbStr::eventLocation(), location);
                }
            }
            break;
        }

        case QOrganizerItemDetail::TypePriority: {
            if (!details.at(i).isEmpty()) {
                object->insert(QOrganizerJsonDbStr::itemPriority(),
                               enumToString(organizerPriorityEnumMap(),
                                            static_cast<QOrganizerItemPriority::Priority>(details.at(i).value(QOrganizerItemPriority::FieldPriority).toInt())));
            }
            break;
        }

        case QOrganizerItemDetail::TypeRecurrence: {
            if (itemType != QOrganizerItemType::TypeEvent && itemType != QOrganizerItemType::TypeTodo)
                break;
            const QMap<int, QVariant> values = details.at(i).values();
            QMap<int, QVariant>::const_iterator j = values.constBegin();
            while (j != values.constEnd()) {
                switch (j.key()) {
                case QOrganizerItemRecurrence::FieldExceptionDates: {
                    QStringList exceptionDates;
                    QSet<QDate> dates = j.value().value<QSet<QDate> >();
                    foreach (const QDate &date, dates)
                        exceptionDates.append(date.toString(Qt::ISODate));
                    if (!exceptionDates.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemExceptionDates(), exceptionDates);
                    break;
                }
                case QOrganizerItemRecurrence::FieldRecurrenceDates: {
                    QStringList recurrenceDates;
                    QSet<QDate> dates = j.value().value<QSet<QDate> >();
                    foreach (const QDate &date, dates)
                        recurrenceDates.append(date.toString(Qt::ISODate));
                    if (!recurrenceDates.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemRecurrenceDates(), recurrenceDates);
                    break;
                }
                case QOrganizerItemRecurrence::FieldExceptionRules: {
                    QVariantList exceptionRules;
                    QSet<QOrganizerRecurrenceRule> rules = j.value().value<QSet<QOrganizerRecurrenceRule> >();
                    foreach (const QOrganizerRecurrenceRule &rule, rules) {
                        QVariantMap exceptionRuleObject;
                        recurrenceRuleToJsonDbObject(rule, &exceptionRuleObject);
                        if (!exceptionRuleObject.isEmpty())
                            exceptionRules.append(exceptionRuleObject);
                    }
                    if (!exceptionRules.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemExceptionRules(), exceptionRules);
                    break;
                }
                case QOrganizerItemRecurrence::FieldRecurrenceRules: {
                    QVariantList recurrenceRules;
                    QSet<QOrganizerRecurrenceRule> rules = j.value().value<QSet<QOrganizerRecurrenceRule> >();
                    foreach (const QOrganizerRecurrenceRule &rule, rules) {
                        QVariantMap recurrenceRuleObject;
                        recurrenceRuleToJsonDbObject(rule, &recurrenceRuleObject);
                        if (!recurrenceRuleObject.isEmpty())
                            recurrenceRules.append(recurrenceRuleObject);
                    }
                    if (!recurrenceRules.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemRecurrenceRules(), recurrenceRules);
                    break;
                }
                default:
                    break;
                }
                ++j;
            }
            break;
        }

        case QOrganizerItemDetail::TypeTag: {
            QString tag = details.at(i).value(QOrganizerItemTag::FieldTag).toString();
            if (!tag.isEmpty())
                tags.append(tag);
            break;
        }

        case QOrganizerItemDetail::TypeAudibleReminder: {
            QVariantMap reminderObject;
            audibleReminderDetailToJsonDbObject(details.at(i), &reminderObject);
            if (!reminderObject.isEmpty()) {
                // check for possible custom fields
                QVariantMap reminder = object->value(QOrganizerJsonDbStr::itemReminder()).toMap();
                if (reminder.isEmpty()) {
                    object->insert(QOrganizerJsonDbStr::itemReminder(), reminderObject);
                } else {
                    QVariantMap::const_iterator i = reminderObject.constBegin();
                    while (i != reminderObject.constEnd()) {
                        reminder.insert(i.key(), i.value());
                        ++i;
                    }
                    object->insert(QOrganizerJsonDbStr::itemReminder(), reminder);
                }
            }
            break;
        }

        case QOrganizerItemDetail::TypeVersion: {
            QString jsonDbVersion;
            itemVersionToJsonDbVersion(details.at(i), &jsonDbVersion);
            if (!jsonDbVersion.isEmpty())
                object->insert(QOrganizerJsonDbStr::jsonDbVersion(), jsonDbVersion);
            break;
        }

        case QOrganizerItemDetail::TypeEventAttendee: {
            if (itemType != QOrganizerItemType::TypeEvent)
                break;
            QVariantMap attendeeObject;
            attendeeDetailToJsonDbObject(details.at(i), &attendeeObject);
            if (!attendeeObject.isEmpty())
                attendees.append(attendeeObject);
            break;
        }

        case QOrganizerItemDetail::TypeEventRsvp: {
            if (itemType != QOrganizerItemType::TypeEvent)
                break;
            QVariantMap rsvpObject;
            rsvpDetailToJsonDbObject(details.at(i), &rsvpObject);
            if (!rsvpObject.isEmpty()) {
                // check for possible custom fields
                QVariantMap rsvp = object->value(QOrganizerJsonDbStr::eventRsvp()).toMap();
                if (rsvp.isEmpty()) {
                    object->insert(QOrganizerJsonDbStr::eventRsvp(), rsvpObject);
                } else {
                    QVariantMap::const_iterator i = rsvpObject.constBegin();
                    while (i != rsvpObject.constEnd()) {
                        rsvp.insert(i.key(), i.value());
                        ++i;
                    }
                    object->insert(QOrganizerJsonDbStr::eventRsvp(), rsvp);
                }
            }
            break;
        }

        case QOrganizerItemDetail::TypeParent: {
            if (itemType != QOrganizerItemType::TypeEventOccurrence && itemType != QOrganizerItemType::TypeTodoOccurrence)
                break;
            const QMap<int, QVariant> values = details.at(i).values();
            QMap<int, QVariant>::const_iterator j = values.constBegin();
            while (j != values.constEnd()) {
                switch (j.key()) {
                case QOrganizerItemParent::FieldParentId: {
                    QOrganizerItemId parentId = j.value().value<QOrganizerItemId>();
                    if (!parentId.isNull())
                        object->insert(QOrganizerJsonDbStr::itemOccurrenceParent(), QOrganizerManagerEngine::engineItemId(parentId)->toString());
                    break;
                }
                case QOrganizerItemParent::FieldOriginalDate: {
                    QDate originalDate = j.value().toDate();
                    if (originalDate.isValid())
                        object->insert(QOrganizerJsonDbStr::itemOccurrenceOriginalDate(), originalDate.toString(Qt::ISODate));
                    break;
                }
                default:
                    break;
                }
                ++j;
            }
            break;
        }

        case QOrganizerItemDetail::TypeEventTime: {
            if (itemType != QOrganizerItemType::TypeEvent && itemType != QOrganizerItemType::TypeEventOccurrence)
                break;
            const QMap<int, QVariant> values = details.at(i).values();
            QMap<int, QVariant>::const_iterator j = values.constBegin();
            while (j != values.constEnd()) {
                switch (j.key()) {
                case QOrganizerEventTime::FieldStartDateTime: {
                    QDateTime startDateTime = j.value().toDateTime();
                    if (startDateTime.isValid())
                        object->insert(QOrganizerJsonDbStr::eventStartDateTime(), startDateTime.toUTC().toString(Qt::ISODate));
                    break;
                }
                case QOrganizerEventTime::FieldEndDateTime: {
                    QDateTime endDateTime = j.value().toDateTime();
                    if (endDateTime.isValid())
                        object->insert(QOrganizerJsonDbStr::eventEndDateTime(), endDateTime.toUTC().toString(Qt::ISODate));
                    break;
                }
                case QOrganizerEventTime::FieldAllDay: {
                    QVariant isAllDay = j.value();
                    if (isAllDay.canConvert(QVariant::Bool))
                        object->insert(QOrganizerJsonDbStr::eventIsAllDay(), isAllDay.toBool());
                    break;
                }
                default:
                    break;
                }
                ++j;
            }
            break;
        }

        case QOrganizerItemDetail::TypeTodoTime: {
            if (itemType != QOrganizerItemType::TypeTodo && itemType != QOrganizerItemType::TypeTodoOccurrence)
                break;
            const QMap<int, QVariant> values = details.at(i).values();
            QMap<int, QVariant>::const_iterator j = values.constBegin();
            while (j != values.constEnd()) {
                switch (j.key()) {
                case QOrganizerTodoTime::FieldStartDateTime: {
                    QDateTime startDateTime = j.value().toDateTime();
                    if (startDateTime.isValid())
                        object->insert(QOrganizerJsonDbStr::todoStartDateTime(), startDateTime.toUTC().toString(Qt::ISODate));
                    break;
                }
                case QOrganizerTodoTime::FieldDueDateTime: {
                    QDateTime dueDateTime = j.value().toDateTime();
                    if (dueDateTime.isValid())
                        object->insert(QOrganizerJsonDbStr::todoDueDateTime(), dueDateTime.toUTC().toString(Qt::ISODate));
                    break;
                }
                case QOrganizerTodoTime::FieldAllDay: {
                    QVariant isAllDay = j.value();
                    if (isAllDay.canConvert(QVariant::Bool))
                        object->insert(QOrganizerJsonDbStr::todoIsAllDay(), isAllDay.toBool());
                    break;
                }
                default:
                    break;
                }
                ++j;
            }
            break;
        }

        case QOrganizerItemDetail::TypeTodoProgress: {
            if (itemType != QOrganizerItemType::TypeTodo)
                break;const QMap<int, QVariant> values = details.at(i).values();
            QMap<int, QVariant>::const_iterator j = values.constBegin();
            while (j != values.constEnd()) {
                switch (j.key()) {
                case QOrganizerTodoProgress::FieldFinishedDateTime: {
                    QDateTime finishedDateTime = j.value().toDateTime();
                    if (finishedDateTime.isValid())
                        object->insert(QOrganizerJsonDbStr::todoFinishedDateTime(), finishedDateTime.toUTC().toString(Qt::ISODate));
                    break;
                }
                case QOrganizerTodoProgress::FieldPercentageComplete: {
                    int percentageComplete = j.value().toInt();
                    if (percentageComplete >= 0 && percentageComplete <= 100)
                        object->insert(QOrganizerJsonDbStr::todoProgressPercentage(), percentageComplete);
                    break;
                }
                case QOrganizerTodoProgress::FieldStatus: {
                    object->insert(QOrganizerJsonDbStr::todoStatus(),
                                   enumToString(organizerTodoStatusMap(), j.value().toInt()));
                    break;
                }
                default:
                    break;
                }
                ++j;
            }
            break;
        }

//        case QOrganizerItemDetail::TypeUndefined:
//        case QOrganizerItemDetail::TypeClassification:
//        case QOrganizerItemDetail::TypeItemType:
//        case QOrganizerItemDetail::TypeTimestamp:
//        case QOrganizerItemDetail::TypeReminder:
//        case QOrganizerItemDetail::TypeEmailReminder:
//        case QOrganizerItemDetail::TypeVisualReminder:
//        case QOrganizerItemDetail::TypeJournalTime:
        default:
            break;
        }
    }

    if (!comments.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemComments(), comments);

    if (!tags.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemTags(), tags);

    if (!attendees.isEmpty())
        object->insert(QOrganizerJsonDbStr::eventAttendees(), attendees);

    return true;
}

void QOrganizerJsonDbConverter::attendeeDetailToJsonDbObject(const QOrganizerEventAttendee &attendeeDetail, QVariantMap *object) const
{
    const QMap<int, QVariant> detailValues = attendeeDetail.values();
    QMap<int, QVariant>::const_iterator i = detailValues.constBegin();
    QString value;
    QString jsonDbField;
    while (i != detailValues.constEnd()) {
        if (i.key() == QOrganizerEventAttendee::FieldName) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::eventAttendeeName();
        } else if (i.key() == QOrganizerEventAttendee::FieldEmailAddress) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::eventAttendeeEmailAddress();
        } else if (i.key() == QOrganizerEventAttendee::FieldAttendeeId) {
            value= i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::eventAttendeeUuid();
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationRole) {
            value = enumToString(organizerParticipationRoleMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::eventAttendeeParticipationRole();
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationStatus) {
            value = enumToString(organizerParticipationStatusMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::eventAttendeeParticipationStatus();
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
        if (i.key() == QOrganizerJsonDbStr::eventAttendeeName()) {
            value = i.value().toString();
            if (!value.isEmpty())
                attendeeDetail->setName(value);
        } else if (i.key() == QOrganizerJsonDbStr::eventAttendeeEmailAddress()) {
            value = i.value().toString();
            if (!value.isEmpty())
                attendeeDetail->setEmailAddress(value);
        } else if (i.key() == QOrganizerJsonDbStr::eventAttendeeUuid()) {
            value = i.value().toString();
            if (!value.isEmpty())
                attendeeDetail->setAttendeeId(value);
        } else if (i.key() == QOrganizerJsonDbStr::eventAttendeeParticipationRole()) {
            int intValue = stringToEnum(organizerParticipationRoleMap(), i.value().toString());
            attendeeDetail->setParticipationRole(static_cast<QOrganizerEventAttendee::ParticipationRole>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::eventAttendeeParticipationStatus()) {
            int intValue = stringToEnum(organizerParticipationStatusMap(), i.value().toString());
            attendeeDetail->setParticipationStatus(static_cast<QOrganizerEventAttendee::ParticipationStatus>(intValue));
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::rsvpDetailToJsonDbObject(const QOrganizerEventRsvp &rsvpDetail, QVariantMap *object) const
{
    const QMap<int, QVariant> rsvpValues = rsvpDetail.values();
    QMap<int, QVariant>::const_iterator i = rsvpValues.constBegin();
    QString value;
    QString jsonDbField;
    while (i != rsvpValues.constEnd()) {
        if (i.key() == QOrganizerEventRsvp::FieldOrganizerName) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::eventRsvpOrganizerName();
        } else if (i.key() == QOrganizerEventRsvp::FieldOrganizerEmail) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::eventRsvpOrganizerEmail();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseDeadline) {
            value = i.value().toDate().toString(Qt::ISODate);
            jsonDbField = QOrganizerJsonDbStr::eventRsvpResponseDeadline();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseDate) {
            value = i.value().toDate().toString(Qt::ISODate);
            jsonDbField = QOrganizerJsonDbStr::eventRsvpResponseDate();
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationRole) {
            value = enumToString(organizerParticipationRoleMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::eventRsvpParticipationRole();
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationStatus) {
            value = enumToString(organizerParticipationStatusMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::eventRsvpParticipationStatus();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseRequirement) {
            value = enumToString(organizerResponseRequirementMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::eventRsvpResponseRequirement();
        }
        if (!value.isEmpty()) {
            object->insert(jsonDbField, value);
            value.clear();
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToRsvpDetail(const QVariantMap &object, QOrganizerEventRsvp *rsvpDetail,
                                                         QOrganizerItemExtendedDetail *extendedDetail) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QVariantMap customFields;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::eventRsvpOrganizerName()) {
            QString value = i.value().toString();
            if (!value.isEmpty())
                rsvpDetail->setOrganizerName(value);
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpOrganizerEmail()) {
            QString value = i.value().toString();
            if (!value.isEmpty())
                rsvpDetail->setOrganizerEmail(value);
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpResponseDeadline()) {
            QDate date = i.value().toDate();
            if (date.isValid())
                rsvpDetail->setResponseDeadline(date);
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpResponseDate()) {
            QDate date = i.value().toDate();
            if (date.isValid())
                rsvpDetail->setResponseDate(date);
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpParticipationRole()) {
            int intValue = stringToEnum(organizerParticipationRoleMap(), i.value().toString());
            rsvpDetail->setParticipationRole(static_cast<QOrganizerEventAttendee::ParticipationRole>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpParticipationStatus()) {
            int intValue = stringToEnum(organizerParticipationStatusMap(), i.value().toString());
            rsvpDetail->setParticipationStatus(static_cast<QOrganizerEventAttendee::ParticipationStatus>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpResponseRequirement()) {
            int intValue = stringToEnum(organizerResponseRequirementMap(), i.value().toString());
            rsvpDetail->setResponseRequirement(static_cast<QOrganizerEventRsvp::ResponseRequirement>(intValue));
        } else {
            customFields.insert(i.key(), i.value());
        }
        ++i;
    }
    if (!customFields.isEmpty()) {
        extendedDetail->setName(QOrganizerJsonDbStr::eventRsvp());
        extendedDetail->setData(customFields);
    }
}

void QOrganizerJsonDbConverter::locationDetailToJsonDbObject(const QOrganizerItemLocation &locationDetail, QVariantMap *object) const
{
    const QMap<int, QVariant> values = locationDetail.values();
    QMap<int, QVariant>::const_iterator i = values.constBegin();
    QVariantMap geoObject;
    while (i != values.constEnd()) {
        switch (i.key()) {
        case QOrganizerItemLocation::FieldLabel: {
            QString label = i.value().toString();
            if (!label.isEmpty())
                object->insert(QOrganizerJsonDbStr::eventLocationDisplayName(), label);
            break;
        }
        case QOrganizerItemLocation::FieldLatitude: {
            QVariant latitude = i.value();
            if (latitude.canConvert(QVariant::Double))
                geoObject.insert(QOrganizerJsonDbStr::eventLocationGeoLatitude(), latitude.toDouble());
            break;
        }
        case QOrganizerItemLocation::FieldLongitude: {
            QVariant longitude = i.value();
            if (longitude.canConvert(QVariant::Double))
                geoObject.insert(QOrganizerJsonDbStr::eventLocationGeoLongitude(), longitude.toDouble());
            break;
        }
        default:
            break;
        }
        ++i;
    }
    if (!geoObject.isEmpty())
        object->insert(QOrganizerJsonDbStr::eventLocationGeo(), geoObject);
}

void QOrganizerJsonDbConverter::jsonDbObjectToLocationDetail(const QVariantMap &object, QOrganizerItemLocation *locationDetail,
                                                             QOrganizerItemExtendedDetail *extendedDetail) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QVariantMap customFields;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::eventLocationDisplayName()) {
            QString label = i.value().toString();
            if (!label.isEmpty())
                locationDetail->setLabel(label);
        } else if (i.key() == QOrganizerJsonDbStr::eventLocationGeo()) {
            QVariantMap geoObject = i.value().toMap();
            QMap<QString, QVariant>::const_iterator j = geoObject.constBegin();
            while (j != geoObject.constEnd()) {
                if (j.key() == QOrganizerJsonDbStr::eventLocationGeoLatitude()) {
                    QVariant latitude = j.value();
                    if (latitude.canConvert(QVariant::Double))
                        locationDetail->setLatitude(latitude.toDouble());
                } else if (j.key() == QOrganizerJsonDbStr::eventLocationGeoLongitude()) {
                    QVariant longitude = j.value();
                    if (longitude.canConvert(QVariant::Double))
                        locationDetail->setLongitude(longitude.toDouble());
                }

                ++j;
            }
        } else {
            customFields.insert(i.key(), i.value());
        }

        ++i;
    }
    if (!customFields.isEmpty()) {
        extendedDetail->setName(QOrganizerJsonDbStr::eventLocation());
        extendedDetail->setData(customFields);
    }
}

bool QOrganizerJsonDbConverter::jsonDbObjectToCollection(const QVariantMap& object, QOrganizerCollection* collection, bool *isDefaultCollection)
{
    bool hasCollectionId(false);

    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
            QString jsonUuid = i.value().toString();
            if (jsonUuid.isEmpty())
                return false;
            collection->setId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonUuid)));
            hasCollectionId = true;
        } else if (i.key() == QOrganizerJsonDbStr::collectionDisplayName()) {
            QString name = i.value().toString();
            if (!name.isEmpty())
                collection->setMetaData(QOrganizerCollection::KeyName, name);
        } else if (i.key() == QOrganizerJsonDbStr::collectionDescription()) {
            QString description = i.value().toString();
            if (!description.isEmpty())
                collection->setMetaData(QOrganizerCollection::KeyDescription, description);
        } else if (i.key() == QOrganizerJsonDbStr::collectionColor()) {
            QString color = i.value().toString();
            if (!color.isEmpty())
                collection->setMetaData(QOrganizerCollection::KeyColor, color);
        } else if (i.key() == QOrganizerJsonDbStr::collectionImageUrl()) {
            QString image = i.value().toString();
            if (!image.isEmpty())
                collection->setMetaData(QOrganizerCollection::KeyImage, image);
        } else if (i.key() == QOrganizerJsonDbStr::collectionDefaultFlag()) {
            QVariant isDefault = i.value();
            if (isDefault.canConvert(QVariant::Bool))
                *isDefaultCollection = isDefault.toBool();
        } else {
            // custom meta data
            if (!i.key().isEmpty() && !i.value().isNull())
                collection->setMetaData(i.key(), i.value());
        }

        ++i;
    }

    return hasCollectionId;
}

bool QOrganizerJsonDbConverter::collectionToJsonDbObject(const QOrganizerCollection& collection, bool isDefaultCollection, QVariantMap* object) const
{
    QOrganizerCollectionId collectionId = collection.id();
    if (!collectionId.isNull()) {
        object->insert(QOrganizerJsonDbStr::jsonDbUuid(),
                       QOrganizerManagerEngine::engineCollectionId(collectionId)->toString());
    }

    object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::jsonDbCollectionType());

    if (isDefaultCollection)
        object->insert(QOrganizerJsonDbStr::collectionDefaultFlag(), isDefaultCollection);

    QVariantMap metaData = collection.metaData();
    QVariantMap::const_iterator i = metaData.constBegin();
    while (i != metaData.constEnd()) {
        if (i.key() == QOrganizerCollection::KeyColor) {
            QString colorString = i.value().toString();
            if (!colorString.isEmpty())
                object->insert(QOrganizerJsonDbStr::collectionColor(), colorString);
        } else if (i.key() == QOrganizerCollection::KeyDescription) {
            QString descriptionString = i.value().toString();
            if (!descriptionString.isEmpty())
                object->insert(QOrganizerJsonDbStr::collectionDescription(), descriptionString);
        } else if (i.key() == QOrganizerCollection::KeyImage) {
            QString imageString = i.value().toString();
            if (!imageString.isEmpty())
                object->insert(QOrganizerJsonDbStr::collectionImageUrl(), imageString);
        } else if (i.key() == QOrganizerCollection::KeyName) {
            QString displayNameString = i.value().toString();
            if (!displayNameString.isEmpty())
                object->insert(QOrganizerJsonDbStr::collectionDisplayName(), displayNameString);
        } else {
            // custom meta data
            QString valueString = i.value().toString();
            if (!i.key().isEmpty() && !valueString.isEmpty()
                && (i.key().at(0) != QChar('_') || i.key() == QOrganizerJsonDbStr::jsonDbVersion())
                && i.key() != QOrganizerJsonDbStr::collectionDefaultFlag()) {
                // XXX Should we allow complex data structure, i.e. list or map, for custom meta data?
                object->insert(i.key(), valueString);
            }
        }

        ++i;
    }

    return true;
}

void QOrganizerJsonDbConverter::jsonDbVersionToItemVersion(const QString &jsonDbVersion, QOrganizerItemVersion *itemVersion) const
{
    QStringList jsonDbVersions = jsonDbVersion.split(QLatin1Char('-'));
    if (jsonDbVersions.size() != 2)
         return;
    int version = jsonDbVersions.at(0).toInt();

    if (version > 0) {
        itemVersion->setVersion(version);
        itemVersion->setExtendedVersion(jsonDbVersions.at(1).toLatin1());
    }
}

void QOrganizerJsonDbConverter::itemVersionToJsonDbVersion(const QOrganizerItemVersion &itemVersion, QString *jsonDbVersion) const
{
    int version = itemVersion.version();
    QByteArray extendedVersion = itemVersion.extendedVersion();

    if (version > 0) {
        *jsonDbVersion = QString::number(version) + QLatin1String("-") + QString::fromLatin1(extendedVersion.constData());
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToRecurrenceRule(const QVariantMap& object, QOrganizerRecurrenceRule* rule) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleFrequency()) {
            QString frequency = i.value().toString();
            if (!frequency.isEmpty())
                rule->setFrequency(static_cast<QOrganizerRecurrenceRule::Frequency>(stringToEnum(organizerFrequencyEnumMap(), frequency)));
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleInterval()) {
            int interval = i.value().toInt();
            if (interval >= 0)
                rule->setInterval(interval);
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleLimitCount()) {
            int limitCount = i.value().toInt();
            if (limitCount >= 0)
                rule->setLimit(limitCount);
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleLimitDate()) {
            QDate limitDate = i.value().toDate();
            if (limitDate.isValid())
                rule->setLimit(limitDate);
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleFirstDayOfWeek()) {
            QString firstDayOfWeek = i.value().toString();
            if (!firstDayOfWeek.isEmpty())
                rule->setFirstDayOfWeek(static_cast<Qt::DayOfWeek>(stringToEnum(organizerDayEnumMap(), firstDayOfWeek)));
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRulePositions()) {
            QVariantList positionsList = i.value().toList();
            if (!positionsList.isEmpty()) {
                QSet<int> positionsSet;
                foreach (const QVariant &position, positionsList)
                    positionsSet.insert(position.toInt());
                rule->setPositions(positionsSet);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfWeek()) {
            QVariantList daysOfWeekList = i.value().toList();
            if (!daysOfWeekList.isEmpty()) {
                QSet<Qt::DayOfWeek> daysOfWeek;
                foreach (const QVariant &day, daysOfWeekList)
                    daysOfWeek.insert(static_cast<Qt::DayOfWeek>(stringToEnum(organizerDayEnumMap(), day.toString())));
                rule->setDaysOfWeek(daysOfWeek);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfMonth()) {
            QVariantList daysOfMonthList = i.value().toList();
            if (!daysOfMonthList.isEmpty()) {
                QSet<int> daysOfMonth;
                foreach (const QVariant &day, daysOfMonthList)
                    daysOfMonth.insert(day.toInt());
                rule->setDaysOfMonth(daysOfMonth);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfYear()) {
            QVariantList daysOfYearList = i.value().toList();
            if (!daysOfYearList.isEmpty()) {
                QSet<int> daysOfYear;
                foreach (const QVariant &day, daysOfYearList)
                    daysOfYear.insert(day.toInt());
                rule->setDaysOfYear(daysOfYear);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleWeeksOfYear()) {
            QVariantList weeksOfYearList = i.value().toList();
            if (!weeksOfYearList.isEmpty()) {
                QSet<int> weeksOfYear;
                foreach (const QVariant &week, weeksOfYearList)
                    weeksOfYear.insert(week.toInt());
                rule->setWeeksOfYear(weeksOfYear);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleMonthsOfYear()) {
            QVariantList monthsOfYearList = i.value().toList();
            if (!monthsOfYearList.isEmpty()) {
                QSet<QOrganizerRecurrenceRule::Month> monthsOfYear;
                foreach (const QVariant &month, monthsOfYearList)
                    monthsOfYear.insert(static_cast<QOrganizerRecurrenceRule::Month>(stringToEnum(organizerMonthEnumMap(), month.toString())));
                rule->setMonthsOfYear(monthsOfYear);
            }
        }

        ++i;
    }
}

void QOrganizerJsonDbConverter::recurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule& rule, QVariantMap* object) const
{
    object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleFrequency(), enumToString(organizerFrequencyEnumMap(), rule.frequency()));
    object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleInterval(), rule.interval());
    object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleLimitCount(), rule.limitCount());
    object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleLimitDate(), rule.limitDate().toString(Qt::ISODate));
    QSet<int> positions = rule.positions();
    if (!positions.isEmpty()) {
        QVariantList positionsList;
        foreach (int position, positions) {
            positionsList.append(position);
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRulePositions(), positionsList);
    }

    object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleFirstDayOfWeek(), enumToString(organizerDayEnumMap(), rule.firstDayOfWeek()));

    QSet<Qt::DayOfWeek> daysOfWeek = rule.daysOfWeek();
    if (!daysOfWeek.isEmpty()) {
        QVariantList daysOfWeekList;
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            daysOfWeekList.append(enumToString(organizerDayEnumMap(), day));
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfWeek(), daysOfWeekList);
    }

    QSet<int> daysOfMonth = rule.daysOfMonth();
    if (!daysOfMonth.isEmpty()) {
        QVariantList daysOfMonthList;
        foreach (int day, daysOfMonth) {
            daysOfMonthList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfMonth(), daysOfMonthList);
    }

    QSet<int> daysOfYear = rule.daysOfYear();
    if (!daysOfYear.isEmpty()) {
        QVariantList daysOfYearList;
        foreach (int day, daysOfYear) {
            daysOfYearList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfYear(), daysOfYearList);
    }

    QSet<int> weeksOfYear = rule.weeksOfYear();
    if (!weeksOfYear.isEmpty()) {
        QVariantList weeksOfYearList;
        foreach (int week, weeksOfYear) {
            weeksOfYearList.append(week);
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleWeeksOfYear(), weeksOfYearList);
    }

    QSet<QOrganizerRecurrenceRule::Month> monthsOfYear = rule.monthsOfYear();
    if (!monthsOfYear.isEmpty()) {
        QVariantList monthsOfYearList;
        foreach (QOrganizerRecurrenceRule::Month month, monthsOfYear) {
            monthsOfYearList.append(enumToString(organizerMonthEnumMap(), month));
        }
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleMonthsOfYear(), monthsOfYearList);
    }
}

bool QOrganizerJsonDbConverter::itemToJsondbAlarmObject(const QOrganizerItem &item, QVariantMap *alarmObject) const
{
    QOrganizerItemId id = item.id();
    if (id.isNull())
        return false;
    alarmObject->insert(QOrganizerJsonDbStr::alarmEventUuid(), QOrganizerManagerEngine::engineItemId(id)->toString());

    QDateTime alarmDueDateTime;
    QOrganizerItemType::ItemType itemType = item.type();
    if (itemType == QOrganizerItemType::TypeEvent || itemType == QOrganizerItemType::TypeEventOccurrence)
        alarmDueDateTime = item.detail(QOrganizerItemDetail::TypeEventTime).value(QOrganizerEventTime::FieldStartDateTime).toDateTime();
    else if (itemType == QOrganizerItemType::TypeTodo || itemType == QOrganizerItemType::TypeTodoOccurrence)
        alarmDueDateTime = item.detail(QOrganizerItemDetail::TypeTodoTime).value(QOrganizerTodoTime::FieldStartDateTime).toDateTime();
    else
        return false;
    if (!alarmDueDateTime.isValid())
        return false;
    alarmObject->insert(QOrganizerJsonDbStr::alarmDueDateTime(), alarmDueDateTime.toUTC().toString(Qt::ISODate));

    QString alarmDisplayName = item.displayLabel();
    if (!alarmDisplayName.isEmpty())
        alarmObject->insert(QOrganizerJsonDbStr::alarmDisplayName(), alarmDisplayName);

    QString alarmDescription = item.description();
    if (!alarmDescription.isEmpty())
        alarmObject->insert(QOrganizerJsonDbStr::alarmDescription(), alarmDescription);

    alarmObject->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::alarm());
    alarmObject->insert(QOrganizerJsonDbStr::alarmEnabled(), QVariant::fromValue(true));
    alarmObject->insert(QOrganizerJsonDbStr::alarmLauncherIdentifier(), QOrganizerJsonDbStr::alarmCalendarIdentifier());
    alarmObject->insert(QOrganizerJsonDbStr::alarmType(), QOrganizerJsonDbStr::alarmTypeCalendar());

    return true;
}

void QOrganizerJsonDbConverter::audibleReminderDetailToJsonDbObject(const QOrganizerItemAudibleReminder &itemReminder, QVariantMap *object) const
{
    const QMap<int, QVariant> reminderValues = itemReminder.values();
    QMap<int, QVariant>::const_iterator i = reminderValues.constBegin();
    while (i != reminderValues.constEnd()) {
        if (i.key() == QOrganizerItemAudibleReminder::FieldSecondsBeforeStart) {
            if (i.value().toInt() > 0)
                object->insert(QOrganizerJsonDbStr::itemReminderSecBeforeStart(), i.value());
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldRepetitionCount) {
            if (i.value().toInt() > 0)
                object->insert(QOrganizerJsonDbStr::itemReminderRepetitionCount(), i.value());
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldRepetitionDelay) {
            if (i.value().toInt() > 0)
                object->insert(QOrganizerJsonDbStr::itemReminderRepetitionDelay(), i.value());
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldDataUrl) {
            if (i.value().toUrl().isValid())
                object->insert(QOrganizerJsonDbStr::itemReminderDataUrl(), i.value());
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToAudibleReminderDetail(const QVariantMap &object, QOrganizerItemAudibleReminder *itemReminder,
                                                                    QOrganizerItemExtendedDetail *extendedDetail) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    QVariantMap customFields;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::itemReminderSecBeforeStart()) {
            bool ok = false;
            int seconds = i.value().toInt(&ok);
            if (ok && seconds >= 0)
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldSecondsBeforeStart, seconds);
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderRepetitionCount()) {
            int repetitionCount = i.value().toInt();
            if (repetitionCount > 0)
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldRepetitionCount, repetitionCount);
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderRepetitionDelay()) {
            int repetitionDelay = i.value().toInt();
            if (repetitionDelay > 0)
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldRepetitionDelay, repetitionDelay);
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderDataUrl()) {
            QUrl url = i.value().toUrl();
            if (url.isValid())
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldDataUrl, url);
        } else {
            customFields.insert(i.key(), i.value());
        }
        ++i;
    }
    if (!customFields.isEmpty()) {
        extendedDetail->setName(QOrganizerJsonDbStr::itemReminder());
        extendedDetail->setData(customFields);
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

void QOrganizerJsonDbConverter::extendedDetailToJsonDbProperty(const QOrganizerItemExtendedDetail &extendedDetail, QVariant *property) const
{
    QString name = extendedDetail.name();
    QVariant data = extendedDetail.data();
    if (name.isEmpty() || data.isNull())
        return;

    // check potential conflicts with predefined detail
    // XXX this should be removed once we get the item validation funtion
    if (name.at(0) == QChar('_')
        || (name == QOrganizerJsonDbStr::itemReminder() && data.type() != QVariant::Map)
        || (name == QOrganizerJsonDbStr::eventRsvp() && data.type() != QVariant::Map)
        || (name == QOrganizerJsonDbStr::eventLocation() && data.type() != QVariant::Map)
        || name == QOrganizerJsonDbStr::itemDisplayName() || name == QOrganizerJsonDbStr::itemCollectionUuid()
        || name == QOrganizerJsonDbStr::itemDescription() || name == QOrganizerJsonDbStr::itemComments()
        || name == QOrganizerJsonDbStr::itemPriority() || name == QOrganizerJsonDbStr::itemTags()
        || name == QOrganizerJsonDbStr::itemGuid()
        || name == QOrganizerJsonDbStr::itemRecurrenceDates() || name == QOrganizerJsonDbStr::itemRecurrenceRules()
        || name == QOrganizerJsonDbStr::itemExceptionDates() || name == QOrganizerJsonDbStr::itemExceptionRules()
        || name == QOrganizerJsonDbStr::eventStartDateTime() /*|| name == QOrganizerJsonDbStr::todoStartDateTime()*/
        || name == QOrganizerJsonDbStr::eventEndDateTime() || name == QOrganizerJsonDbStr::todoDueDateTime()
        || name == QOrganizerJsonDbStr::eventIsAllDay() /*|| name == QOrganizerJsonDbStr::todoIsAllDay()*/
        || name == QOrganizerJsonDbStr::eventAttendees()
        || name == QOrganizerJsonDbStr::todoFinishedDateTime() || name == QOrganizerJsonDbStr::todoProgressPercentage()
        || name == QOrganizerJsonDbStr::todoStatus()) {
        return;
    }

    if (data.type() < QVariant::Map) {
        // JsonDb can directly save these types: bool, int, uint, qlonglong, qulonglong, double, QChar
        // so no conversion is needed
        property->setValue(data);
    } else if (data.canConvert(QVariant::String)) {
        property->setValue(data.toString());
    } else if (data.type() == QVariant::List) {
        QVariantList variantList;
        dataToList(data, &variantList);
        property->setValue(variantList);
    } else if (data.type() == QVariant::Map) {
        QVariantMap variantMap;
        dataToMap(data, &variantMap);
        property->setValue(variantMap);
    }
}

void QOrganizerJsonDbConverter::dataToList(const QVariant &data, QVariantList *list) const
{
    if (data.type() != QVariant::List)
        return;

    QList<QVariant> originalList(data.toList());
    foreach (const QVariant &variant, originalList) {
        if (variant.isNull())
            continue;

        if (variant.type() < QVariant::Map) {
            // JsonDb can directly save these types: bool, int, uint, qlonglong, qulonglong, double, QChar
            // so no conversion is needed
            list->append(variant);
        } else if (variant.canConvert(QVariant::String)) {
            list->append(variant.toString());
        } else if (variant.type() == QVariant::List) {
            dataToList(variant, list);
        } else if (variant.type() == QVariant::Map) {
            QVariantMap map;
            dataToMap(variant, &map);
            list->append(map);
        }
    }
}

void QOrganizerJsonDbConverter::dataToMap(const QVariant &data, QVariantMap *map) const
{
    if (data.type() != QVariant::Map)
        return;

    QMap<QString, QVariant> originalMap(data.toMap());
    QMap<QString, QVariant>::const_iterator i = originalMap.constBegin();
    while (i != originalMap.constEnd()) {
        if (i.value().isValid() && !i.value().isNull()) {
            if (i.value().type() < QVariant::Map) {
                // JsonDb can directly save these types: bool, int, uint, qlonglong, qulonglong, double, QChar
                // so no conversion is needed
                map->insert(i.key(), i.value());
            } else if (i.value().canConvert(QVariant::String)) {
                map->insert(i.key(), i.value().toString());
            } else if (i.value().type() == QVariant::List) {
                QVariantList list;
                dataToList(i.value(), &list);
                map->insert(i.key(), list);
            } else if (i.value().type() == QVariant::Map) {
                QVariantMap embedMap;
                dataToMap(i.value(), &embedMap);
                map->insert(i.key(), embedMap);
            }
        }
        ++i;
    }
}

bool QOrganizerJsonDbConverter::compoundFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const
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
            if (compoundFilterToJsondbQuery(filter, &filterStr))
                *jsonDbQueryStr += filterStr;
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
            if (compoundFilterToJsondbQuery(filter, &filterStr)) {
                *jsonDbQueryStr += filterStr;
                validFilterCount ++;
            } else {//For union filter, single filter invalid means we could skip this filter
                continue;
            }
        }
        if (validFilterCount > 0)
            jsonDbQueryStr->replace(QStringLiteral("][?"), QStringLiteral(" | ")); //replace the "][?" to " | "
        else //no valid filter means empty item list from jsondb
            isValidFilter = false;
        return isValidFilter;
    }
    default:
        isValidFilter = singleFilterToJsondbQuery(filter, jsonDbQueryStr);
        break;
    }

    if (!isValidFilter)
        jsonDbQueryStr->clear();

    return isValidFilter;
}

bool QOrganizerJsonDbConverter::singleFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString *jsonDbQueryStr) const
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
    return object.value(QOrganizerJsonDbStr::jsonDbType()).toString();
}

QOrganizerItemId QOrganizerJsonDbConverter::jsonDbNotificationObjectToItemId(const QVariantMap &object) const
{
    QString jsonUuid = object.value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
    if (jsonUuid.isEmpty())
        return QOrganizerItemId();
    else
        return QOrganizerItemId(new QOrganizerJsonDbItemId(jsonUuid));
}

QOrganizerCollectionId QOrganizerJsonDbConverter::jsonDbNotificationObjectToCollectionId(const QVariantMap &object) const
{
    QString jsonUuid = object.value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
    if (jsonUuid.isEmpty())
        return QOrganizerCollectionId();
    else
        return QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonUuid));
}

bool QOrganizerJsonDbConverter::collectionFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const
{
    bool isValidFilter = true;
    const QOrganizerItemCollectionFilter cf(filter);
    const QSet<QOrganizerCollectionId>& ids = cf.collectionIds();
    if (!ids.empty()) {
        const QString idTemplate(QStringLiteral("\"%1\","));
        QString query;
        foreach (const QOrganizerCollectionId &id, ids) {
            if (!id.isNull())
                query += idTemplate.arg(QOrganizerManagerEngine::engineCollectionId(id)->toString());
        }
        if (!query.isEmpty()) {
            query.truncate(query.length() - 1);
            *jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryCollectionUuidsTemplate().arg(query);
        } else {
            isValidFilter = false;
        }
    } else {
        isValidFilter = false;
    }
    return isValidFilter;
}

bool QOrganizerJsonDbConverter::idFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const
{
    bool isValidFilter = true;
    const QOrganizerItemIdFilter idf(filter);
    const QList<QOrganizerItemId>& ids = idf.ids();
    if (!ids.empty()) {
        const QString uuidTemplate(QStringLiteral("\"%1\","));
        QString query;
        foreach (const QOrganizerItemId &id, ids) {
            if (!id.isNull())
                query += uuidTemplate.arg(QOrganizerManagerEngine::engineItemId(id)->toString());
        }
        if (!query.isEmpty()) {
            query.truncate(query.length() - 1);
            *jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryUuidsTemplate().arg(query);
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
        || QOrganizerItemReminder::DefinitionName == detailType
        || QOrganizerItemAudibleReminder::DefinitionName == detailType
        || QOrganizerItemVisualReminder::DefinitionName == detailType
        || QOrganizerItemEmailReminder::DefinitionName == detailType
        || QOrganizerItemRecurrence::DefinitionName == detailType
        || QOrganizerItemTimestamp::DefinitionName == detailType
        || QOrganizerEventAttendee::DefinitionName == detailType) {
        // filtering certain details/definitions are currently not supported
        isValidFilter = false;
    } else if (QOrganizerItemFilter::MatchExactly != filter.matchFlags()
        && (QOrganizerEventTime::DefinitionName == detailType
        || QOrganizerTodoTime::DefinitionName == detailType
        || QOrganizerTodoProgress::DefinitionName == detailType
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
                                                                            || QOrganizerEventRsvp::FieldResponseDate == detailFieldName))
        || QOrganizerItemParent::DefinitionName == detailType)) {
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

bool QOrganizerJsonDbConverter::detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const
{
    /*
    Jsondb backend specific notes related to OrganizerItemDetailFilter

    Detail specific:
    - Currently supported details; EventTime, TodoTime, Comment, Description, DisplayLabel,
      Gui, Location, Priority, Type, Tag, Parent and Customized(ExtendedDetail).
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
        const QString equalsQueryTemplate2(QStringLiteral("[?%1=%2]"));
        const QString equalsQueryTemplate3(QStringLiteral("[?%1.%2.%3=%4]"));
        const QString containsQueryTemplate(QStringLiteral("[?%1 contains \"%2\"]"));
        const QString matchFlagQueryTemplate(QStringLiteral("[?%1%2\"]"));
        const QString matchFlagQueryTemplate2(QStringLiteral("[?%1.%2%3\"]"));
        const QString existsQueryTemplate(QStringLiteral("[?%1 exists]"));

        if (QOrganizerEventTime::DefinitionName == detailType) {
            if (QOrganizerEventTime::FieldStartDateTime ==  detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventStartDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerEventTime::FieldEndDateTime == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventEndDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerEventTime::FieldAllDay == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate2
                    .arg(QOrganizerJsonDbStr::eventIsAllDay()).arg(valueString));
            }

        } else if (QOrganizerTodoTime::DefinitionName == detailType) {
            if (QOrganizerTodoTime::FieldStartDateTime == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoStartDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerTodoTime::FieldDueDateTime == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoDueDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerTodoTime::FieldAllDay == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate2
                    .arg(QOrganizerJsonDbStr::todoIsAllDay()).arg(valueString));
            }

        } else if (QOrganizerItemDetail::TypeTodoProgress == detailType) {
            if (QOrganizerTodoProgress::FieldFinishedDateTime == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoFinishedDateTime())
                    .arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerTodoProgress::FieldPercentageComplete == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate2
                    .arg(QOrganizerJsonDbStr::todoProgressPercentage())
                    .arg(df.value().toInt()));
            } else if (QOrganizerTodoProgress::FieldStatus == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoStatus())
                    .arg(enumToString(organizerTodoStatusMap(), df.value().toInt())));
            }

        } else if (QOrganizerItemComment::DefinitionName == detailType
            && QOrganizerItemComment::FieldComment == detailField) {
            jsonDbQueryStr->append(containsQueryTemplate.arg(QOrganizerJsonDbStr::itemComments()).arg(valueString));

        } else if (QOrganizerItemDescription::DefinitionName == detailType
            && QOrganizerItemDescription::FieldDescription == detailField) {
            jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemDescription()).arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemDisplayLabel::DefinitionName == detailType
            && QOrganizerItemDisplayLabel::FieldLabel == detailField) {
           jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemDisplayName())
                .arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemGuid::DefinitionName == detailType
            && QOrganizerItemGuid::FieldGuid ==  detailField) {
            jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemGuid())
                .arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemLocation::DefinitionName == detailType) {
            if (QOrganizerItemLocation::FieldLabel ==  detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                    .arg(QOrganizerJsonDbStr::eventLocation())
                    .arg(QOrganizerJsonDbStr::eventLocationDisplayName())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags())));
            } else if (QOrganizerItemLocation::FieldLongitude ==  detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate3
                    .arg(QOrganizerJsonDbStr::eventLocation())
                    .arg(QOrganizerJsonDbStr::eventLocationGeo())
                    .arg(QOrganizerJsonDbStr::eventLocationGeoLongitude())
                    .arg(valueString));
            } else if (QOrganizerItemLocation::FieldLatitude ==  detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate3
                    .arg(QOrganizerJsonDbStr::eventLocation())
                    .arg(QOrganizerJsonDbStr::eventLocationGeo())
                    .arg(QOrganizerJsonDbStr::eventLocationGeoLatitude())
                    .arg(valueString));
            }

        } else if (QOrganizerItemPriority::DefinitionName == detailType
            && QOrganizerItemPriority::FieldPriority ==  detailField) {
            jsonDbQueryStr->append(equalsQueryTemplate
                .arg(QOrganizerJsonDbStr::itemPriority())
                .arg(enumToString(organizerPriorityEnumMap(), df.value().toInt())));

        } else if (QOrganizerItemType::DefinitionName == detailType
            && QOrganizerItemType::FieldType ==  detailField) {
            jsonDbQueryStr->append(equalsQueryTemplate
                .arg(QOrganizerJsonDbStr::jsonDbType())
                .arg(QOrganizerJsonDbStr::jsonDbSchemaPrefix() + enumToString(organizerItemTypeMap(), df.value().toInt())));

        } else if (QOrganizerItemTag::DefinitionName == detailType
            && QOrganizerItemTag::FieldTag == detailField) {
            jsonDbQueryStr->append(containsQueryTemplate.arg(QOrganizerJsonDbStr::itemTags()).arg(valueString));

        } else if (QOrganizerItemParent::DefinitionName == detailType) {
            if (QOrganizerItemParent::FieldParentId == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::itemOccurrenceParent()).arg(df.value().value<QOrganizerItemId>().toString().remove(QOrganizerJsonDbStr::managerName())));
            } else if (QOrganizerItemParent::FieldOriginalDate == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::itemOccurrenceOriginalDate()).arg(df.value().toDate().toString(Qt::ISODate)));
            }

        } else if (QOrganizerItemExtendedDetail::DefinitionName == detailType
             && QOrganizerItemExtendedDetail::FieldExtendedDetailName ==  detailField) {
            jsonDbQueryStr->append(existsQueryTemplate.arg(valueString));

        } else if (QOrganizerEventRsvp::DefinitionName == detailType) {
            if (QOrganizerEventRsvp::FieldParticipationStatus == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpParticipationStatus())
                        .arg(createMatchFlagQuery(enumToString(organizerParticipationStatusMap(), df.value().toInt()), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldParticipationRole == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpParticipationRole())
                        .arg(createMatchFlagQuery(enumToString(organizerParticipationRoleMap(), df.value().toInt()), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldResponseRequirement == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpResponseRequirement())
                        .arg(createMatchFlagQuery(enumToString(organizerResponseRequirementMap(), df.value().toInt()), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldResponseDeadline == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpResponseDeadline())
                        .arg(createMatchFlagQuery(df.value().toDate().toString(Qt::ISODate), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldResponseDate == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpResponseDate())
                        .arg(createMatchFlagQuery(df.value().toDate().toString(Qt::ISODate), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldOrganizerName == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpOrganizerName())
                        .arg(createMatchFlagQuery(valueString, df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldOrganizerEmail == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::eventRsvp())
                        .arg(QOrganizerJsonDbStr::eventRsvpOrganizerEmail())
                        .arg(createMatchFlagQuery(valueString, df.matchFlags())));
            }
        }
    }
    return isValidFilter;
}

QString QOrganizerJsonDbConverter::createMatchFlagQuery(const QString &value, QOrganizerItemFilter::MatchFlags flags) const
{
    // with current enumeration, 3 means both MatchContains and MatchEndsWith (QTBUG-20035)
    const uint realMatchType = flags & 0x0F;
    QString queryWithWildCards;
    if (QOrganizerItemFilter::MatchExactly == flags)
        queryWithWildCards = QStringLiteral("=\"");
    else
        queryWithWildCards = QStringLiteral("=~\"/");

    if (QOrganizerItemFilter::MatchContains == realMatchType || QOrganizerItemFilter::MatchEndsWith == realMatchType)
        queryWithWildCards += QStringLiteral("*");

    queryWithWildCards += value;

    if (QOrganizerItemFilter::MatchContains == realMatchType || QOrganizerItemFilter::MatchStartsWith == realMatchType)
        queryWithWildCards += QStringLiteral("*");

    if (QOrganizerItemFilter::MatchExactly != flags && !(QOrganizerItemFilter::MatchFixedString & flags)) {
        queryWithWildCards += QStringLiteral("/w");
        if (!(QOrganizerItemFilter::MatchCaseSensitive & flags))
            queryWithWildCards += QStringLiteral("i");
    }
    return queryWithWildCards;
}

QTORGANIZER_END_NAMESPACE
