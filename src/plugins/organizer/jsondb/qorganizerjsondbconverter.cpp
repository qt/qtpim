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

const int QOrganizerJsonDbConverter::jsonDbVersionLength(32);
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
    // must handle type before reaching the loop
    QString jsonDbType = object.value(QOrganizerJsonDbStr::jsonDbType()).toString();
    if (jsonDbType == QOrganizerJsonDbStr::event())
        item->setType(QOrganizerItemType::TypeEvent);
    else if (jsonDbType == QOrganizerJsonDbStr::todo())
        item->setType(QOrganizerItemType::TypeTodo);
    else
        return false;

    // other mandatory fields
    bool hasCollectionId(false);
    bool hasGuid(false);
    bool hasItemId(false);
    bool hasItemVersion(false);

    // go through all fields
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
            QString jsonDbUuid = i.value().toString();
            if (jsonDbUuid.isEmpty())
                return false;
            item->setId(QOrganizerItemId(new QOrganizerJsonDbItemId(jsonDbUuid)));
            hasItemId = true;
        } else if (i.key() == QOrganizerJsonDbStr::itemCollectionId()) {
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
        } else if (i.key() == QOrganizerJsonDbStr::itemName()) {
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
                if (!rsvp.isEmpty())
                    item->saveDetail(&rsvp);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemReminder()) {
            QVariantMap reminderMap = i.value().toMap();
            if (!reminderMap.isEmpty()) {
                QOrganizerItemAudibleReminder audibleReminder;
                jsonDbObjectToAudibleReminderDetail(reminderMap, &audibleReminder);
                if (!audibleReminder.isEmpty())
                    item->saveDetail(&audibleReminder);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemLocation()) {
            QVariantMap locationMap = i.value().toMap();
            if (!locationMap.isEmpty()) {
                QOrganizerItemLocation location;
                jsonDbObjectToLocationDetail(locationMap, &location);
                if (!location.isEmpty())
                    item->saveDetail(&location);
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

    return hasCollectionId && hasGuid && hasItemId && hasItemVersion;
}

bool QOrganizerJsonDbConverter::itemToJsonDbObject(const QOrganizerItem& item, QVariantMap* object) const
{
    const QList<QOrganizerItemDetail> details = item.details();

    // the first detail should always be item type
    QOrganizerItemType::ItemType itemType = static_cast<QOrganizerItemType::ItemType>(details.at(0).value(QOrganizerItemType::FieldType).toInt());
    switch (itemType) {
    case QOrganizerItemType::TypeEvent:
        object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::event());
        break;

    case QOrganizerItemType::TypeTodo:
        object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::todo());
        break;

//    case QOrganizerItemType::TypeUndefined:
//    case QOrganizerItemType::TypeEventOccurrence:
//    case QOrganizerItemType::TypeTodoOccurrence:
//    case QOrganizerItemType::TypeJournal:
//    case QOrganizerItemType::TypeNote:
    default:
        return false;
    }

    // certain details that allow multiple instances
    QStringList comments;
    QStringList exceptionDates;
    QStringList recurrenceDates;
    QStringList tags;
    QVariantList attendees;
    QVariantList exceptionRules;
    QVariantList recurrenceRules;

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
                object->insert(QOrganizerJsonDbStr::itemName(), displayLabel);
            break;
        }

        case QOrganizerItemDetail::TypeExtendedDetail: {
            // TODO use namespace or other techniques to protect pre-defined fields, design needed
            QString name = details.at(i).value(QOrganizerItemExtendedDetail::FieldExtendedDetailName).toString();
            if (name.isEmpty() || name.at(0) == QChar('_'))
                break;

            QVariant property;
            extendedDetailToJsonDbProperty(details.at(i), &property);
            if (!property.isNull())
                object->insert(name, property);
            break;
        }

        case QOrganizerItemDetail::TypeGuid:
            // GUID has already been generated in QOrganizerJsonDbRequestThread::handleItemSaveRequest() if needed
            object->insert(QOrganizerJsonDbStr::itemGuid(), details.at(i).value(QOrganizerItemGuid::FieldGuid).toString());
            break;

        case QOrganizerItemDetail::TypeLocation: {
            QVariantMap locationObject;
            locationDetailToJsonDbObject(details.at(i), &locationObject);
            if (!locationObject.isEmpty())
                object->insert(QOrganizerJsonDbStr::itemLocation(), locationObject);
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
                    QSet<QDate> dates = j.value().value<QSet<QDate> >();
                    foreach (const QDate &date, dates)
                        exceptionDates.append(date.toString(Qt::ISODate));
                    break;
                }
                case QOrganizerItemRecurrence::FieldRecurrenceDates: {
                    QSet<QDate> dates = j.value().value<QSet<QDate> >();
                    foreach (const QDate &date, dates)
                        recurrenceDates.append(date.toString(Qt::ISODate));
                    break;
                }
                case QOrganizerItemRecurrence::FieldExceptionRules: {
                    QSet<QOrganizerRecurrenceRule> rules = j.value().value<QSet<QOrganizerRecurrenceRule> >();
                    foreach (const QOrganizerRecurrenceRule &rule, rules) {
                        QVariantMap exceptionRuleObject;
                        recurrenceRuleToJsonDbObject(rule, &exceptionRuleObject);
                        if (!exceptionRuleObject.isEmpty())
                            exceptionRules.append(exceptionRuleObject);
                    }
                    break;
                }
                case QOrganizerItemRecurrence::FieldRecurrenceRules: {
                    QSet<QOrganizerRecurrenceRule> rules = j.value().value<QSet<QOrganizerRecurrenceRule> >();
                    foreach (const QOrganizerRecurrenceRule &rule, rules) {
                        QVariantMap recurrenceRuleObject;
                        recurrenceRuleToJsonDbObject(rule, &recurrenceRuleObject);
                        if (!recurrenceRuleObject.isEmpty())
                            recurrenceRules.append(recurrenceRuleObject);
                    }
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
            if (!reminderObject.isEmpty())
                object->insert(QOrganizerJsonDbStr::itemReminder(), reminderObject);
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
            if (!rsvpObject.isEmpty())
                object->insert(QOrganizerJsonDbStr::rsvp(), rsvpObject);
            break;
        }

        case QOrganizerItemDetail::TypeEventTime: {
            if (itemType != QOrganizerItemType::TypeEvent)
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
            if (itemType != QOrganizerItemType::TypeTodo)
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
//        case QOrganizerItemDetail::TypeParent:
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

    if (!exceptionDates.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemExceptioneDates(), exceptionDates);

    if (!recurrenceDates.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemRecurrenceDates(), recurrenceDates);

    if (!tags.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemTags(), tags);

    if (!attendees.isEmpty())
        object->insert(QOrganizerJsonDbStr::attendee(), attendees);

    if (!exceptionRules.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemExceptionRules(), exceptionRules);

    if (!recurrenceRules.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRules(), recurrenceRules);

    // item ID
    if (!item.id().isNull())
        object->insert(QOrganizerJsonDbStr::jsonDbUuid(), QOrganizerManagerEngine::engineItemId(item.id())->toString());

    // collection ID has already been generated in QOrganizerJsonDbRequestThread::handleItemSaveRequest() if needed
    object->insert(QOrganizerJsonDbStr::itemCollectionId(),
                   QOrganizerManagerEngine::engineCollectionId(item.collectionId())->toString());

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
            jsonDbField = QOrganizerJsonDbStr::attendeeName();
        } else if (i.key() == QOrganizerEventAttendee::FieldEmailAddress) {
            value = i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::attendeeEmailAddress();
        } else if (i.key() == QOrganizerEventAttendee::FieldAttendeeId) {
            value= i.value().toString();
            jsonDbField = QOrganizerJsonDbStr::attendeeId();
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationRole) {
            value = enumToString(organizerParticipationRoleMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationRole();
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationStatus) {
            value = enumToString(organizerParticipationStatusMap(), i.value().toInt());
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
            int intValue = stringToEnum(organizerParticipationRoleMap(), i.value().toString());
            attendeeDetail->setParticipationRole(static_cast<QOrganizerEventAttendee::ParticipationRole>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::attendeeParticipationStatus()) {
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
            value = enumToString(organizerParticipationRoleMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationRole();
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationStatus) {
            value = enumToString(organizerParticipationStatusMap(), i.value().toInt());
            jsonDbField = QOrganizerJsonDbStr::attendeeParticipationStatus();
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseRequirement) {
            value = enumToString(organizerResponseRequirementMap(), i.value().toInt());
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
            int intValue = stringToEnum(organizerParticipationRoleMap(), i.value().toString());
            rsvpDetail->setParticipationRole(static_cast<QOrganizerEventAttendee::ParticipationRole>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::attendeeParticipationStatus()) {
            int intValue = stringToEnum(organizerParticipationStatusMap(), i.value().toString());
            rsvpDetail->setParticipationStatus(static_cast<QOrganizerEventAttendee::ParticipationStatus>(intValue));
        } else if (i.key() == QOrganizerJsonDbStr::rsvpResponseRequirement()) {
            int intValue = stringToEnum(organizerResponseRequirementMap(), i.value().toString());
            rsvpDetail->setResponseRequirement(static_cast<QOrganizerEventRsvp::ResponseRequirement>(intValue));
        }
        ++i;
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
                object->insert(QOrganizerJsonDbStr::itemLocationLabel(), label);
            break;
        }
        case QOrganizerItemLocation::FieldLatitude: {
            QVariant latitude = i.value();
            if (latitude.canConvert(QVariant::Double))
                geoObject.insert(QOrganizerJsonDbStr::itemLocationGeoLatitude(), latitude.toDouble());
            break;
        }
        case QOrganizerItemLocation::FieldLongitude: {
            QVariant longitude = i.value();
            if (longitude.canConvert(QVariant::Double))
                geoObject.insert(QOrganizerJsonDbStr::itemLocationGeoLongitude(), longitude.toDouble());
            break;
        }
        default:
            break;
        }
        ++i;
    }
    if (!geoObject.isEmpty())
        object->insert(QOrganizerJsonDbStr::itemLocationGeo(), geoObject);
}

void QOrganizerJsonDbConverter::jsonDbObjectToLocationDetail(const QVariantMap &object, QOrganizerItemLocation *locationDetail) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::itemLocationLabel()) {
            QString label = i.value().toString();
            if (!label.isEmpty())
                locationDetail->setLabel(label);
        } else if (i.key() == QOrganizerJsonDbStr::itemLocationGeo()) {
            QVariantMap geoObject = i.value().toMap();
            QMap<QString, QVariant>::const_iterator j = geoObject.constBegin();
            while (j != geoObject.constEnd()) {
                if (j.key() == QOrganizerJsonDbStr::itemLocationGeoLatitude()) {
                    QVariant latitude = j.value();
                    if (latitude.canConvert(QVariant::Double))
                        locationDetail->setLatitude(latitude.toDouble());
                } else if (j.key() == QOrganizerJsonDbStr::itemLocationGeoLongitude()) {
                    QVariant longitude = j.value();
                    if (longitude.canConvert(QVariant::Double))
                        locationDetail->setLongitude(longitude.toDouble());
                }

                ++j;
            }
        }

        ++i;
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
        } else if (i.key() == QOrganizerJsonDbStr::collectionName()) {
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
        } else if (i.key() == QOrganizerJsonDbStr::collectionImage()) {
            QString image = i.value().toString();
            if (!image.isEmpty())
                collection->setMetaData(QOrganizerCollection::KeyImage, image);
        } else if (i.key() == QOrganizerJsonDbStr::collectionDefaultFlag()) {
            QVariant isDefault = i.value();
            if (isDefault.canConvert(QVariant::Bool))
                *isDefaultCollection = isDefault.toBool();
        } else if (i.key() == QOrganizerJsonDbStr::collectionCustomFields()) {
            QVariantMap customFields = i.value().toMap();
            if (!customFields.isEmpty()) {
                QMap<QString, QVariant>::const_iterator j = customFields.constBegin();
                while (j != customFields.constEnd()) {
                    collection->setMetaData(j.key(), j.value());
                    ++j;
                }
            }
        }

        ++i;
    }

    return hasCollectionId;
}

bool QOrganizerJsonDbConverter::collectionToJsonDbObject(const QOrganizerCollection& collection, bool isDefaultCollection, QVariantMap* object) const
{
    if (!collection.id().isNull())
        object->insert(QOrganizerJsonDbStr::jsonDbUuid(), QOrganizerManagerEngine::engineCollectionId(collection.id())->toString());

    object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::collection());
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

void QOrganizerJsonDbConverter::jsonDbVersionToItemVersion(const QString &jsonDbVersion, QOrganizerItemVersion *itemVersion) const
{
    if (jsonDbVersion.length() == jsonDbVersionLength)
        itemVersion->setExtendedVersion(jsonDbVersion.toLatin1());
}

void QOrganizerJsonDbConverter::itemVersionToJsonDbVersion(const QOrganizerItemVersion &itemVersion, QString *jsonDbVersion) const
{
    QByteArray extendedVersion = itemVersion.extendedVersion();
    if (extendedVersion.length() == jsonDbVersionLength)
        *jsonDbVersion = QString::fromLatin1(extendedVersion.constData());
}

void QOrganizerJsonDbConverter::jsonDbObjectToRecurrenceRule(const QVariantMap& object, QOrganizerRecurrenceRule* rule) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::ruleFrequency()) {
            QString frequency = i.value().toString();
            if (!frequency.isEmpty())
                rule->setFrequency(static_cast<QOrganizerRecurrenceRule::Frequency>(stringToEnum(organizerFrequencyEnumMap(), frequency)));
        } else if (i.key() == QOrganizerJsonDbStr::ruleInterval()) {
            int interval = i.value().toInt();
            if (interval >= 0)
                rule->setInterval(interval);
        } else if (i.key() == QOrganizerJsonDbStr::ruleLimitCount()) {
            int limitCount = i.value().toInt();
            if (limitCount >= 0)
                rule->setLimit(limitCount);
        } else if (i.key() == QOrganizerJsonDbStr::ruleLimitDate()) {
            QDate limitDate = i.value().toDate();
            if (limitDate.isValid())
                rule->setLimit(limitDate);
        } else if (i.key() == QOrganizerJsonDbStr::ruleFirstDayOfWeek()) {
            QString firstDayOfWeek = i.value().toString();
            if (!firstDayOfWeek.isEmpty())
                rule->setFirstDayOfWeek(static_cast<Qt::DayOfWeek>(stringToEnum(organizerDayEnumMap(), firstDayOfWeek)));
        } else if (i.key() == QOrganizerJsonDbStr::rulePositions()) {
            QVariantList positionsList = i.value().toList();
            if (!positionsList.isEmpty()) {
                QSet<int> positionsSet;
                foreach (const QVariant &position, positionsList)
                    positionsSet.insert(position.toInt());
                rule->setPositions(positionsSet);
            }
        } else if (i.key() == QOrganizerJsonDbStr::ruleDaysOfWeek()) {
            QVariantList daysOfWeekList = i.value().toList();
            if (!daysOfWeekList.isEmpty()) {
                QSet<Qt::DayOfWeek> daysOfWeek;
                foreach (const QVariant &day, daysOfWeekList)
                    daysOfWeek.insert(static_cast<Qt::DayOfWeek>(stringToEnum(organizerDayEnumMap(), day.toString())));
                rule->setDaysOfWeek(daysOfWeek);
            }
        } else if (i.key() == QOrganizerJsonDbStr::ruleDaysOfMonth()) {
            QVariantList daysOfMonthList = i.value().toList();
            if (!daysOfMonthList.isEmpty()) {
                QSet<int> daysOfMonth;
                foreach (const QVariant &day, daysOfMonthList)
                    daysOfMonth.insert(day.toInt());
                rule->setDaysOfMonth(daysOfMonth);
            }
        } else if (i.key() == QOrganizerJsonDbStr::ruleDaysOfYear()) {
            QVariantList daysOfYearList = i.value().toList();
            if (!daysOfYearList.isEmpty()) {
                QSet<int> daysOfYear;
                foreach (const QVariant &day, daysOfYearList)
                    daysOfYear.insert(day.toInt());
                rule->setDaysOfYear(daysOfYear);
            }
        } else if (i.key() == QOrganizerJsonDbStr::ruleWeeksOfYear()) {
            QVariantList weeksOfYearList = i.value().toList();
            if (!weeksOfYearList.isEmpty()) {
                QSet<int> weeksOfYear;
                foreach (const QVariant &week, weeksOfYearList)
                    weeksOfYear.insert(week.toInt());
                rule->setWeeksOfYear(weeksOfYear);
            }
        } else if (i.key() == QOrganizerJsonDbStr::ruleMonthsOfYear()) {
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
    object->insert(QOrganizerJsonDbStr::ruleFrequency(), enumToString(organizerFrequencyEnumMap(), rule.frequency()));
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

    object->insert(QOrganizerJsonDbStr::ruleFirstDayOfWeek(), enumToString(organizerDayEnumMap(), rule.firstDayOfWeek()));

    QSet<Qt::DayOfWeek> daysOfWeek = rule.daysOfWeek();
    if (!daysOfWeek.isEmpty()) {
        QVariantList daysOfWeekList;
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            daysOfWeekList.append(enumToString(organizerDayEnumMap(), day));
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
            monthsOfYearList.append(enumToString(organizerMonthEnumMap(), month));
        }
        object->insert(QOrganizerJsonDbStr::ruleMonthsOfYear(), monthsOfYearList);
    }
}

bool QOrganizerJsonDbConverter::itemToJsondbAlarmObject(const QOrganizerItem &item, QVariantMap *alarmObject) const
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
        alarmObject->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::alarm());
        // alarm start time
        if (audibleReminder.hasValue(audibleReminder.FieldSecondsBeforeStart))
            alarmStartTime = alarmStartTime.addSecs(-audibleReminder.secondsBeforeStart());//Nagetive value
        alarmObject->insert(QOrganizerJsonDbStr::alarmISODate(), alarmStartTime.toUTC().toString(Qt::ISODate));
        // repetition count to alarm snooze count
        if (audibleReminder.hasValue(audibleReminder.FieldRepetitionCount))
           alarmObject->insert(QOrganizerJsonDbStr::alarmSnoozeCount(), audibleReminder.repetitionCount());
        // repetition delay to alarm snooze time
        if (audibleReminder.hasValue(audibleReminder.FieldRepetitionDelay))
           alarmObject->insert(QOrganizerJsonDbStr::alarmSnoozeTime(), audibleReminder.repetitionDelay());
        // item Uuid to alarm event Uuid
        alarmObject->insert(QOrganizerJsonDbStr::alarmEventUuid(), QOrganizerManagerEngine::engineItemId(item.id())->toString());
        // item displayLabel to alarm title
        QString displayLabel(item.displayLabel());
        if (!displayLabel.isEmpty())
            alarmObject->insert(QOrganizerJsonDbStr::alarmTitle(), displayLabel);
        // item description to alarm description
        QString description(item.description());
        if (!description.isEmpty())
            alarmObject->insert(QOrganizerJsonDbStr::alarmDescription(), description);
        // set calender app as alarm identifier
        alarmObject->insert(QOrganizerJsonDbStr::alarmIdentifier(), QOrganizerJsonDbStr::alarmCalenderIdentifier());
        ret = true;
    }
    return ret;
}

void QOrganizerJsonDbConverter::audibleReminderDetailToJsonDbObject(const QOrganizerItemAudibleReminder &itemReminder, QVariantMap *object) const
{
    const QMap<int, QVariant> reminderValues = itemReminder.values();
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
            object->insert(jsonDbField, value);
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

void QOrganizerJsonDbConverter::extendedDetailToJsonDbProperty(const QOrganizerItemExtendedDetail &extendedDetail, QVariant *property) const
{
    // TODO check potential conflicts with predefined detail
    if (extendedDetail.name().isEmpty() || extendedDetail.data().isNull())
        return;

    if (extendedDetail.data().canConvert(QVariant::String)) {
        property->setValue(extendedDetail.data().toString());
    } else if (extendedDetail.data().type() == QVariant::List) {
        QVariantList variantList;
        dataToList(extendedDetail.data(), &variantList);
        property->setValue(variantList);
    } else if (extendedDetail.data().type() == QVariant::Map) {
        QVariantMap variantMap;
        dataToMap(extendedDetail.data(), &variantMap);
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

        if (variant.canConvert(QVariant::String)) {
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
            if (i.value().canConvert(QVariant::String)) {
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
            *jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryCollectionIdsTemplate().arg(query);
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
        || QOrganizerItemParent::DefinitionName == detailType
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

bool QOrganizerJsonDbConverter::detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const
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
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventStartDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerEventTime::FieldEndDateTime == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::eventEndDateTime()).arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerEventTime::FieldAllDay == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
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
                jsonDbQueryStr->append(equalsQueryTemplate.arg(QOrganizerJsonDbStr::todoIsAllDay()).arg(valueString));
            }

        } else if (QOrganizerItemDetail::TypeTodoProgress == detailType) {
            if (QOrganizerTodoProgress::FieldFinishedDateTime == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::todoFinishedDateTime())
                    .arg(df.value().toDateTime().toUTC().toString(Qt::ISODate)));
            } else if (QOrganizerTodoProgress::FieldPercentageComplete == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
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
                .arg(QOrganizerJsonDbStr::itemName())
                .arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemGuid::DefinitionName == detailType
            && QOrganizerItemGuid::FieldGuid ==  detailField) {
            jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemGuid())
                .arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemLocation::DefinitionName == detailType) {
            if (QOrganizerItemLocation::FieldLabel ==  detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                    .arg(QOrganizerJsonDbStr::itemLocation())
                    .arg(QOrganizerJsonDbStr::itemLocationLabel())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags())));
            } else if (QOrganizerItemLocation::FieldLongitude ==  detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate3
                    .arg(QOrganizerJsonDbStr::itemLocation())
                    .arg(QOrganizerJsonDbStr::itemLocationGeo())
                    .arg(QOrganizerJsonDbStr::itemLocationGeoLongitude())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags())));
            } else if (QOrganizerItemLocation::FieldLatitude ==  detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate3
                    .arg(QOrganizerJsonDbStr::itemLocation())
                    .arg(QOrganizerJsonDbStr::itemLocationGeo())
                    .arg(QOrganizerJsonDbStr::itemLocationGeoLatitude())
                    .arg(createMatchFlagQuery(valueString, df.matchFlags())));
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

        } else if (QOrganizerItemExtendedDetail::DefinitionName == detailType
             && QOrganizerItemExtendedDetail::FieldExtendedDetailName ==  detailField) {
            jsonDbQueryStr->append(existsQueryTemplate.arg(valueString));

        } else if (QOrganizerEventRsvp::DefinitionName == detailType) {
            if (QOrganizerEventRsvp::FieldParticipationStatus == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::attendeeParticipationStatus())
                        .arg(createMatchFlagQuery(enumToString(organizerParticipationStatusMap(), df.value().toInt()), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldParticipationRole == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::attendeeParticipationRole())
                        .arg(createMatchFlagQuery(enumToString(organizerParticipationRoleMap(), df.value().toInt()), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldResponseRequirement == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpResponseRequirement())
                        .arg(createMatchFlagQuery(enumToString(organizerResponseRequirementMap(), df.value().toInt()), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldResponseDeadline == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpResponseDeadline())
                        .arg(createMatchFlagQuery(df.value().toDate().toString(Qt::ISODate), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldResponseDate == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpResponseDate())
                        .arg(createMatchFlagQuery(df.value().toDate().toString(Qt::ISODate), df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldOrganizerName == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpOrganizerName())
                        .arg(createMatchFlagQuery(valueString, df.matchFlags())));
            } else if (QOrganizerEventRsvp::FieldOrganizerEmail == detailField) {
                jsonDbQueryStr->append(matchFlagQueryTemplate2
                        .arg(QOrganizerJsonDbStr::rsvp())
                        .arg(QOrganizerJsonDbStr::rsvpOrganizerEmail())
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
