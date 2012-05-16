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

#include <QtOrganizer/qorganizermanagerengine.h>
#include <QtOrganizer/qorganizeritemfilters.h>

#include <QtCore/qjsonarray.h>

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
        {QOrganizerItemType::TypeUndefined,                 QString(QStringLiteral("Undefined"))},
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

const QOrganizerJsonDbEnumConversionData *QOrganizerJsonDbConverter::organizerStorageLocationMap()
{
    static const QOrganizerJsonDbEnumConversionData map[] = {
        {QOrganizerAbstractRequest::UserDataStorage,        QString(QStringLiteral("com.nokia.mt.User"))},
        {QOrganizerAbstractRequest::SystemStorage,          QString(QStringLiteral("com.nokia.mt.System"))},
        {enumMapEnd,                                        QString::null}
    };
    return map;
}

QOrganizerJsonDbConverter::QOrganizerJsonDbConverter()
{
}

QOrganizerManager::Error QOrganizerJsonDbConverter::jsonDbConnectionErrorToOrganizerError(QJsonDbConnection::ErrorCode error) const
{
    switch (error) {
    case QJsonDbConnection::NoError:
        return QOrganizerManager::NoError;
    }
    return QOrganizerManager::NoError;
}

QOrganizerManager::Error QOrganizerJsonDbConverter::jsonDbRequestErrorToOrganizerError(QJsonDbRequest::ErrorCode error) const
{
    // TODO: Add more error codes when they are available in jsondb
    // currently there is no error code for missing UUID
    switch (error) {
    case QJsonDbRequest::NoError:
        return QOrganizerManager::NoError;
    case QJsonDbRequest::MissingObject:
        return QOrganizerManager::DoesNotExistError;
    case QJsonDbRequest::MissingType:
    case QJsonDbRequest::MissingQuery:
    case QJsonDbRequest::InvalidLimit:
        return QOrganizerManager::BadArgumentError;
    case QJsonDbRequest::InvalidPartition:
        // FIXME; We propably need to add more finegrained error inspection
        // related to partition accesses, now there is only InvalidPartition.
        return QOrganizerManager::InvalidStorageLocationError;
    case QJsonDbRequest::DatabaseConnectionError:
        return QOrganizerManager::UnspecifiedError;
    default:
        return QOrganizerManager::UnspecifiedError;
    }
}

bool QOrganizerJsonDbConverter::jsonDbObjectToItem(const QJsonObject &object, QOrganizerItem *item, QOrganizerAbstractRequest::StorageLocation storageLocation) const
{
    QJsonObject objectToParse;

    // must handle type before reaching the loop
    QString jsonDbType = object.value(QOrganizerJsonDbStr::jsonDbType()).toString();
    if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventType()) {
        item->setType(QOrganizerItemType::TypeEvent);
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventOccurrenceType()) {
        item->setType(QOrganizerItemType::TypeEventOccurrence);
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventViewType()) {
        // the data is stored in the "value" field, so dirty code here ;)
        objectToParse = object.value(QOrganizerJsonDbStr::jsonDbValue()).toObject();
        if (!objectToParse.value("isVisible").toBool())
            return false;
        objectToParse.insert(QOrganizerJsonDbStr::jsonDbUuid(), object.value(QOrganizerJsonDbStr::jsonDbUuid()));
        item->setType(QOrganizerItemType::TypeEvent);
        item->setData(QOrganizerJsonDbStr::eventIsSynthetic(), true);
    } else if (jsonDbType == QOrganizerJsonDbStr::jsonDbTodoType()) {
        item->setType(QOrganizerItemType::TypeTodo);
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
    QJsonObject::const_iterator i = objectToParse.constBegin();
    while (i != objectToParse.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
            QString jsonDbUuid = i.value().toString();
            if (jsonDbUuid.isEmpty())
                return false;
            QOrganizerJsonDbItemId *jsondbItemId = new QOrganizerJsonDbItemId();
            jsondbItemId->setJsonDbUuid(jsonDbUuid);
            jsondbItemId->setStorageLocation(storageLocation);
            item->setId(QOrganizerItemId(jsondbItemId));
            hasItemId = true;
        } else if (i.key() == QOrganizerJsonDbStr::itemCollectionUuid()) {
            QString jsonDbCollectionIdStr = i.value().toString();
            if (jsonDbCollectionIdStr.isEmpty())
                return false;

            QOrganizerJsonDbCollectionId *jsondbCollId = new QOrganizerJsonDbCollectionId();
            jsondbCollId->setJsonDbUuid(jsonDbCollectionIdStr);
            jsondbCollId->setStorageLocation(storageLocation);
            item->setCollectionId(QOrganizerCollectionId(jsondbCollId));
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
            QJsonArray array = i.value().toArray();
            if (!array.isEmpty()) {
                QStringList comments;
                for (int j = 0; j < array.size(); ++j) {
                    QString comment = array.at(j).toString();
                    if (!comment.isEmpty())
                        comments.append(comment);
                }
                if (!comments.isEmpty())
                    item->setComments(comments);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemTags()) {
            QJsonArray array = i.value().toArray();
            if (!array.isEmpty()) {
                QStringList tags;
                for (int j = 0; j < array.size(); ++j) {
                    QString tag = array.at(j).toString();
                    if (!tag.isEmpty())
                        tags.append(tag);
                }
                if (!tags.isEmpty())
                    item->setTags(tags);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemPriority()) {
            QString jsonDbPriority = i.value().toString();
            if (!jsonDbPriority.isEmpty()) {
                QOrganizerItemPriority priority;
                priority.setPriority(static_cast<QOrganizerItemPriority::Priority>(stringToEnum(organizerPriorityEnumMap(), jsonDbPriority)));
                item->saveDetail(&priority);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRules()) {
            QSet<QOrganizerRecurrenceRule> recurrenceRules;
            QJsonArray jsonDbRecurrenceRules = i.value().toArray();
            foreach (const QJsonValue &jsonDbRecurrenceRule, jsonDbRecurrenceRules) {
                QOrganizerRecurrenceRule rule;
                jsonDbObjectToRecurrenceRule(jsonDbRecurrenceRule.toObject(), &rule);
                recurrenceRules.insert(rule);
            }

            if (!recurrenceRules.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemDetail::TypeRecurrence);
                recurrence.setRecurrenceRules(recurrenceRules);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemExceptionRules()) {
            QSet<QOrganizerRecurrenceRule> exceptionRules;
            QJsonArray jsonDbExceptionRules = i.value().toArray();
            foreach (const QJsonValue &jsonDbExceptionRule, jsonDbExceptionRules) {
                QOrganizerRecurrenceRule rule;
                jsonDbObjectToRecurrenceRule(jsonDbExceptionRule.toObject(), &rule);
                exceptionRules.insert(rule);
            }

            if (!exceptionRules.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemDetail::TypeRecurrence);
                recurrence.setExceptionRules(exceptionRules);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceDates()) {
            QSet<QDate> recurrenceDates;
            QJsonArray jsonDbRecurrenceDates = i.value().toArray();
            foreach (const QJsonValue &jsonDbRecurrenceDate, jsonDbRecurrenceDates) {
                QDate date = QDate::fromString(jsonDbRecurrenceDate.toString(), Qt::ISODate);
                if (date.isValid())
                    recurrenceDates.insert(date);
            }

            if (!recurrenceDates.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemDetail::TypeRecurrence);
                recurrence.setRecurrenceDates(recurrenceDates);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemExceptionDates()) {
            QSet<QDate> exceptionDates;
            QJsonArray jsonDbExceptionDates = i.value().toArray();
            foreach (const QJsonValue &jsonDbExceptionDate, jsonDbExceptionDates) {
                QDate date = QDate::fromString(jsonDbExceptionDate.toString(), Qt::ISODate);
                if (date.isValid())
                    exceptionDates.insert(date);
            }

            if (!exceptionDates.isEmpty()) {
                QOrganizerItemRecurrence recurrence = item->detail(QOrganizerItemDetail::TypeRecurrence);
                recurrence.setExceptionDates(exceptionDates);
                item->saveDetail(&recurrence);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventStartDateTime()) {
            // EventStartDateTime is the same as TodoStartDateTime, thus a "hack" here ;)
            QDateTime startTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (startTime.isValid()) {
                if (item->type() == QOrganizerItemType::TypeEvent
                        || item->type() == QOrganizerItemType::TypeEventOccurrence) {
                    QOrganizerEventTime eventTime = item->detail(QOrganizerItemDetail::TypeEventTime);
                    eventTime.setStartDateTime(startTime);
                    item->saveDetail(&eventTime);
                } else if (item->type() == QOrganizerItemType::TypeTodo
                           || item->type() == QOrganizerItemType::TypeTodoOccurrence) {
                    QOrganizerTodoTime todoTime = item->detail(QOrganizerItemDetail::TypeTodoTime);
                    todoTime.setStartDateTime(startTime);
                    item->saveDetail(&todoTime);
                }
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventEndDateTime()) {
            QDateTime endTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (endTime.isValid()) {
                QOrganizerEventTime eventTime = item->detail(QOrganizerItemDetail::TypeEventTime);
                eventTime.setEndDateTime(endTime);
                item->saveDetail(&eventTime);
            }
        } else if (i.key() == QOrganizerJsonDbStr::todoDueDateTime()) {
            QDateTime dueTime = QDateTime::fromString(i.value().toString(), Qt::ISODate);
            if (dueTime.isValid()) {
                QOrganizerTodoTime todoTime = item->detail(QOrganizerItemDetail::TypeTodoTime);
                todoTime.setDueDateTime(dueTime);
                item->saveDetail(&todoTime);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventIsAllDay()) {
            // EventIsAllDay is the same as TodoIsAllDay, thus a "hack" here ;)
            bool isAllDay = i.value().toBool();
            if (item->type() == QOrganizerItemType::TypeEvent || item->type() == QOrganizerItemType::TypeEventOccurrence) {
                QOrganizerEventTime eventTime = item->detail(QOrganizerItemDetail::TypeEventTime);
                eventTime.setAllDay(isAllDay);
                item->saveDetail(&eventTime);
            } else if (item->type() == QOrganizerItemType::TypeTodo || item->type() == QOrganizerItemType::TypeTodoOccurrence) {
                QOrganizerTodoTime todoTime = item->detail(QOrganizerItemDetail::TypeTodoTime);
                todoTime.setAllDay(isAllDay);
                item->saveDetail(&todoTime);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventAttendees()) {
            QJsonArray jsonDbAttendees = i.value().toArray();
            foreach (const QJsonValue &jsonDbAttendee, jsonDbAttendees) {
                QOrganizerEventAttendee attendee;
                jsonDbObjectToAttendeeDetail(jsonDbAttendee.toObject(), &attendee);
                if (!attendee.isEmpty())
                    item->saveDetail(&attendee);
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvp()) {
            QJsonObject jsonDbRsvp = i.value().toObject();
            if (!jsonDbRsvp.isEmpty()) {
                // custom fields are supported for RSVP
                QOrganizerEventRsvp rsvp;
                QOrganizerItemExtendedDetail extendedDetail;
                jsonDbObjectToRsvpDetail(jsonDbRsvp, &rsvp, &extendedDetail);
                if (!rsvp.isEmpty())
                    item->saveDetail(&rsvp);
                if (!extendedDetail.isEmpty())
                    item->saveDetail((&extendedDetail));
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemOccurrenceParent()) {
            QOrganizerItemParent parentDetail = item->detail(QOrganizerItemDetail::TypeParent);
            parentDetail.setParentId(QOrganizerItemId(new QOrganizerJsonDbItemId(i.value().toString())));
            item->saveDetail(&parentDetail);
        } else if (i.key() == QOrganizerJsonDbStr::itemOccurrenceOriginalDate()) {
            QDate originalDate = QDate::fromString(i.value().toString(), Qt::ISODate);
            if (originalDate.isValid()) {
                QOrganizerItemParent parentDetail = item->detail(QOrganizerItemDetail::TypeParent);
                parentDetail.setOriginalDate(originalDate);
                item->saveDetail(&parentDetail);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemReminder()) {
            QJsonObject jsonDbReminder = i.value().toObject();
            if (!jsonDbReminder.isEmpty()) {
                // custom fields are supported for reminders
                QOrganizerItemAudibleReminder audibleReminder;
                QOrganizerItemExtendedDetail extendedDetail;
                jsonDbObjectToAudibleReminderDetail(jsonDbReminder, &audibleReminder, &extendedDetail);
                if (!audibleReminder.isEmpty())
                    item->saveDetail(&audibleReminder);
                if (!extendedDetail.isEmpty())
                    item->saveDetail((&extendedDetail));
            }
        } else if (i.key() == QOrganizerJsonDbStr::eventLocation()) {
            QJsonObject jsonDbLocation = i.value().toObject();
            if (!jsonDbLocation.isEmpty()) {
                // custom fields are supported for reminders
                QOrganizerItemLocation location;
                QOrganizerItemExtendedDetail extendedDetail;
                jsonDbObjectToLocationDetail(jsonDbLocation, &location, &extendedDetail);
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
            int progressPercentage = i.value().toDouble();
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
            extendedDetail.setData(i.value().toVariant());
            item->saveDetail(&extendedDetail);
        }
        ++i;
    }

    // view object is guaranteed to be correct when generated, and missing several mandatory fields as nomral objects
    if (jsonDbType == QOrganizerJsonDbStr::jsonDbEventViewType())
        return true;

    return hasCollectionId && hasGuid && hasItemId && hasItemVersion;
}

bool QOrganizerJsonDbConverter::itemToJsonDbObject(const QOrganizerItem &item, QJsonObject *object) const
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
        object->insert(QOrganizerJsonDbStr::jsonDbUuid(), QOrganizerJsonDbItemId(item.id().toString()).jsondbUuid());

    // collection ID has already been generated in QOrganizerJsonDbRequestThread::handleItemSaveRequest() if needed
    if (!item.collectionId().isNull())
        object->insert(QOrganizerJsonDbStr::itemCollectionUuid(), QOrganizerJsonDbCollectionId(item.collectionId().toString()).jsondbUuid());

    // certain details that allow multiple instances
    QJsonArray comments;
    QJsonArray tags;
    QJsonArray attendees;

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
            QString name = details.at(i).value(QOrganizerItemExtendedDetail::FieldName).toString();
            if (name.isEmpty())
                break;

            QJsonValue data = QJsonValue::fromVariant(details.at(i).value(QOrganizerItemExtendedDetail::FieldData));
            if (data.isNull())
                break;

            // custom fields are allowed for reminder, rsvp, and location
            // in such cases, those values in extended details will be merged with reminder (or other) object in JsonDb
            // for all other cases, the extended detail is stored as "name: data" in JsonDb
            if (name == QOrganizerJsonDbStr::itemReminder()
                || ((itemType == QOrganizerItemType::TypeEvent || itemType == QOrganizerItemType::TypeEventOccurrence)
                    && (name == QOrganizerJsonDbStr::eventRsvp() || name == QOrganizerJsonDbStr::eventLocation()))) {
                if (!data.isObject())
                    break;

                QJsonObject existing = object->value(name).toObject();
                if (existing.isEmpty()) {
                    object->insert(name, data);
                } else {
                    // combining the existing value and the value from the extended detail
                    QJsonObject::const_iterator i = existing.constBegin();
                    QJsonObject newExtendedDetailProperty = data.toObject();
                    while (i != existing.constEnd()) {
                        newExtendedDetailProperty.insert(i.key(), i.value());
                        ++i;
                    }
                    object->insert(name, newExtendedDetailProperty);
                }
                break;
            }


            object->insert(name, data);
            break;
        }

        case QOrganizerItemDetail::TypeGuid:
            // GUID has already been generated in QOrganizerJsonDbRequestThread::handleItemSaveRequest() if needed
            object->insert(QOrganizerJsonDbStr::itemGuid(), details.at(i).value(QOrganizerItemGuid::FieldGuid).toString());
            break;

        case QOrganizerItemDetail::TypeLocation: {
            // in JsonDb, only events can have locations
            if (itemType != QOrganizerItemType::TypeEvent && itemType != QOrganizerItemType::TypeEventOccurrence)
                break;
            QJsonObject jsonDbLocation;
            locationDetailToJsonDbObject(details.at(i), &jsonDbLocation);
            if (!jsonDbLocation.isEmpty()) {
                // check for possible custom fields
                QJsonObject location = object->value(QOrganizerJsonDbStr::eventLocation()).toObject();
                if (location.isEmpty()) {
                    object->insert(QOrganizerJsonDbStr::eventLocation(), jsonDbLocation);
                } else {
                    QJsonObject::const_iterator i = jsonDbLocation.constBegin();
                    while (i != jsonDbLocation.constEnd()) {
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
                    QJsonArray exceptionDates;
                    QSet<QDate> dates = j.value().value<QSet<QDate> >();
                    foreach (const QDate &date, dates) {
                        if (date.isValid())
                            exceptionDates.append(date.toString(Qt::ISODate));
                    }
                    if (!exceptionDates.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemExceptionDates(), exceptionDates);
                    break;
                }
                case QOrganizerItemRecurrence::FieldRecurrenceDates: {
                    QJsonArray recurrenceDates;
                    QSet<QDate> dates = j.value().value<QSet<QDate> >();
                    foreach (const QDate &date, dates) {
                        if (date.isValid())
                            recurrenceDates.append(date.toString(Qt::ISODate));
                    }
                    if (!recurrenceDates.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemRecurrenceDates(), recurrenceDates);
                    break;
                }
                case QOrganizerItemRecurrence::FieldExceptionRules: {
                    QJsonArray exceptionRules;
                    QSet<QOrganizerRecurrenceRule> rules = j.value().value<QSet<QOrganizerRecurrenceRule> >();
                    foreach (const QOrganizerRecurrenceRule &rule, rules) {
                        QJsonObject exceptionRuleObject;
                        recurrenceRuleToJsonDbObject(rule, &exceptionRuleObject);
                        if (!exceptionRuleObject.isEmpty())
                            exceptionRules.append(exceptionRuleObject);
                    }
                    if (!exceptionRules.isEmpty())
                        object->insert(QOrganizerJsonDbStr::itemExceptionRules(), exceptionRules);
                    break;
                }
                case QOrganizerItemRecurrence::FieldRecurrenceRules: {
                    QJsonArray recurrenceRules;
                    QSet<QOrganizerRecurrenceRule> rules = j.value().value<QSet<QOrganizerRecurrenceRule> >();
                    foreach (const QOrganizerRecurrenceRule &rule, rules) {
                        QJsonObject recurrenceRuleObject;
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
            QJsonObject jsonDbAudibleReminder;
            audibleReminderDetailToJsonDbObject(details.at(i), &jsonDbAudibleReminder);
            if (!jsonDbAudibleReminder.isEmpty()) {
                // check for possible custom fields
                QJsonObject reminder = object->value(QOrganizerJsonDbStr::itemReminder()).toObject();
                if (reminder.isEmpty()) {
                    object->insert(QOrganizerJsonDbStr::itemReminder(), jsonDbAudibleReminder);
                } else {
                    QJsonObject::const_iterator i = jsonDbAudibleReminder.constBegin();
                    while (i != jsonDbAudibleReminder.constEnd()) {
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
            if (itemType != QOrganizerItemType::TypeEvent && itemType != QOrganizerItemType::TypeEventOccurrence)
                break;
            QJsonObject jsonDbAttendee;
            attendeeDetailToJsonDbObject(details.at(i), &jsonDbAttendee);
            if (!jsonDbAttendee.isEmpty())
                attendees.append(jsonDbAttendee);
            break;
        }

        case QOrganizerItemDetail::TypeEventRsvp: {
            if (itemType != QOrganizerItemType::TypeEvent && itemType != QOrganizerItemType::TypeEventOccurrence)
                break;
            QJsonObject jsonDbRsvp;
            rsvpDetailToJsonDbObject(details.at(i), &jsonDbRsvp);
            if (!jsonDbRsvp.isEmpty()) {
                // check for possible custom fields
                QJsonObject rsvp = object->value(QOrganizerJsonDbStr::eventRsvp()).toObject();
                if (rsvp.isEmpty()) {
                    object->insert(QOrganizerJsonDbStr::eventRsvp(), jsonDbRsvp);
                } else {
                    QJsonObject::const_iterator i = jsonDbRsvp.constBegin();
                    while (i != jsonDbRsvp.constEnd()) {
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
                    if (!parentId.isNull()) {
                        object->insert(QOrganizerJsonDbStr::itemOccurrenceParent(), QOrganizerManagerEngine::engineItemId(parentId)->toString());
                    break;
                    }
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
            if (itemType != QOrganizerItemType::TypeTodo && itemType != QOrganizerItemType::TypeTodoOccurrence)
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

void QOrganizerJsonDbConverter::attendeeDetailToJsonDbObject(const QOrganizerEventAttendee &attendeeDetail, QJsonObject *object) const
{
    const QMap<int, QVariant> detailValues = attendeeDetail.values();
    QMap<int, QVariant>::const_iterator i = detailValues.constBegin();
    while (i != detailValues.constEnd()) {
        if (i.key() == QOrganizerEventAttendee::FieldName) {
            QString name = i.value().toString();
            if (!name.isEmpty())
                object->insert(QOrganizerJsonDbStr::eventAttendeeName(), name);
        } else if (i.key() == QOrganizerEventAttendee::FieldEmailAddress) {
            QString email = i.value().toString();
            if (!email.isEmpty())
                object->insert(QOrganizerJsonDbStr::eventAttendeeEmailAddress(), email);
        } else if (i.key() == QOrganizerEventAttendee::FieldAttendeeId) {
            QString id = i.value().toString();
            if (!id.isEmpty())
                object->insert(QOrganizerJsonDbStr::eventAttendeeUuid(), id);
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationRole) {
            object->insert(QOrganizerJsonDbStr::eventAttendeeParticipationRole(),
                           enumToString(organizerParticipationRoleMap(), i.value().toInt()));
        } else if (i.key() == QOrganizerEventAttendee::FieldParticipationStatus) {
            object->insert(QOrganizerJsonDbStr::eventAttendeeParticipationStatus(),
                           enumToString(organizerParticipationStatusMap(), i.value().toInt()));
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToAttendeeDetail(const QJsonObject &object, QOrganizerEventAttendee *attendeeDetail) const
{
    QJsonObject::const_iterator i = object.constBegin();
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

void QOrganizerJsonDbConverter::rsvpDetailToJsonDbObject(const QOrganizerEventRsvp &rsvpDetail, QJsonObject *object) const
{
    const QMap<int, QVariant> rsvpValues = rsvpDetail.values();
    QMap<int, QVariant>::const_iterator i = rsvpValues.constBegin();
    while (i != rsvpValues.constEnd()) {
        if (i.key() == QOrganizerEventRsvp::FieldOrganizerName) {
            QString organizerName = i.value().toString();
            if (!organizerName.isEmpty())
                object->insert(QOrganizerJsonDbStr::eventRsvpOrganizerName(), organizerName);
        } else if (i.key() == QOrganizerEventRsvp::FieldOrganizerEmail) {
            QString organizerEmail = i.value().toString();
            if (!organizerEmail.isEmpty())
                object->insert(QOrganizerJsonDbStr::eventRsvpOrganizerEmail(), organizerEmail);
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseDeadline) {
            QDate responseDeadline = i.value().toDate();
            if (responseDeadline.isValid())
                object->insert(QOrganizerJsonDbStr::eventRsvpResponseDeadline(), responseDeadline.toString(Qt::ISODate));
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseDate) {
            QDate responseDate = i.value().toDate();
            if (responseDate.isValid())
                object->insert(QOrganizerJsonDbStr::eventRsvpResponseDate(), responseDate.toString(Qt::ISODate));
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationRole) {
            object->insert(QOrganizerJsonDbStr::eventRsvpParticipationRole(),
                           enumToString(organizerParticipationRoleMap(), i.value().toInt()));
        } else if (i.key() == QOrganizerEventRsvp::FieldParticipationStatus) {
            object->insert(QOrganizerJsonDbStr::eventRsvpParticipationStatus(),
                           enumToString(organizerParticipationStatusMap(), i.value().toInt()));
        } else if (i.key() == QOrganizerEventRsvp::FieldResponseRequirement) {
            object->insert(QOrganizerJsonDbStr::eventRsvpResponseRequirement(),
                           enumToString(organizerResponseRequirementMap(), i.value().toInt()));
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToRsvpDetail(const QJsonObject &object, QOrganizerEventRsvp *rsvpDetail,
                                                         QOrganizerItemExtendedDetail *extendedDetail) const
{
    QJsonObject::const_iterator i = object.constBegin();
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
            QDate date = QDate::fromString(i.value().toString(), Qt::ISODate);
            if (date.isValid())
                rsvpDetail->setResponseDeadline(date);
        } else if (i.key() == QOrganizerJsonDbStr::eventRsvpResponseDate()) {
            QDate date = QDate::fromString(i.value().toString(), Qt::ISODate);
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
            customFields.insert(i.key(), i.value().toVariant());
        }
        ++i;
    }
    if (!customFields.isEmpty()) {
        extendedDetail->setName(QOrganizerJsonDbStr::eventRsvp());
        extendedDetail->setData(customFields);
    }
}

void QOrganizerJsonDbConverter::locationDetailToJsonDbObject(const QOrganizerItemLocation &locationDetail, QJsonObject *object) const
{
    const QMap<int, QVariant> values = locationDetail.values();
    QMap<int, QVariant>::const_iterator i = values.constBegin();
    QJsonObject jsonDbGeo;
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
                jsonDbGeo.insert(QOrganizerJsonDbStr::eventLocationGeoLatitude(), latitude.toDouble());
            break;
        }
        case QOrganizerItemLocation::FieldLongitude: {
            QVariant longitude = i.value();
            if (longitude.canConvert(QVariant::Double))
                jsonDbGeo.insert(QOrganizerJsonDbStr::eventLocationGeoLongitude(), longitude.toDouble());
            break;
        }
        default:
            break;
        }
        ++i;
    }
    if (!jsonDbGeo.isEmpty())
        object->insert(QOrganizerJsonDbStr::eventLocationGeo(), jsonDbGeo);
}

void QOrganizerJsonDbConverter::jsonDbObjectToLocationDetail(const QJsonObject &object, QOrganizerItemLocation *locationDetail,
                                                             QOrganizerItemExtendedDetail *extendedDetail) const
{
    QJsonObject::const_iterator i = object.constBegin();
    QVariantMap customFields;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::eventLocationDisplayName()) {
            QString label = i.value().toString();
            if (!label.isEmpty())
                locationDetail->setLabel(label);
        } else if (i.key() == QOrganizerJsonDbStr::eventLocationGeo()) {
            QJsonObject jsonDbGeo = i.value().toObject();
            QJsonObject::const_iterator j = jsonDbGeo.constBegin();
            while (j != jsonDbGeo.constEnd()) {
                if (j.key() == QOrganizerJsonDbStr::eventLocationGeoLatitude()) {
                    if (j.value().isDouble())
                        locationDetail->setLatitude(j.value().toDouble());
                } else if (j.key() == QOrganizerJsonDbStr::eventLocationGeoLongitude()) {
                    if (j.value().isDouble())
                        locationDetail->setLongitude(j.value().toDouble());
                }

                ++j;
            }
        } else {
            customFields.insert(i.key(), i.value().toVariant());
        }

        ++i;
    }
    if (!customFields.isEmpty()) {
        extendedDetail->setName(QOrganizerJsonDbStr::eventLocation());
        extendedDetail->setData(customFields);
    }
}

bool QOrganizerJsonDbConverter::jsonDbObjectToCollection(const QJsonObject &object, QOrganizerCollection *collection, bool *isDefaultCollection, QOrganizerAbstractRequest::StorageLocation storageLocation) const
{
    bool hasCollectionId(false);

    QVariantMap extendedMetaData;
    QJsonObject::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::jsonDbUuid()) {
            QString jsonUuid = i.value().toString();
            if (jsonUuid.isEmpty())
                return false;
            QOrganizerJsonDbCollectionId *jsondbCollectionId = new QOrganizerJsonDbCollectionId();
            jsondbCollectionId->setJsonDbUuid(jsonUuid);
            jsondbCollectionId->setStorageLocation(storageLocation);
            collection->setId(QOrganizerCollectionId(jsondbCollectionId));
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
            if (i.value().isBool())
                *isDefaultCollection = i.value().toBool();
        } else {
            // custom meta data
            if (i.key().at(0) != QChar('_') && !i.key().isEmpty() && !i.value().isNull())
                extendedMetaData.insert(i.key(), i.value().toVariant());
        }

        ++i;
    }

    if (!extendedMetaData.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyExtended, extendedMetaData);

    return hasCollectionId;
}

bool QOrganizerJsonDbConverter::collectionToJsonDbObject(const QOrganizerCollection &collection, bool isDefaultCollection, QJsonObject *object) const
{
    QOrganizerCollectionId collectionId = collection.id();
    if (!collectionId.isNull()) {
        object->insert(QOrganizerJsonDbStr::jsonDbUuid(), QOrganizerJsonDbCollectionId(collectionId.toString()).jsondbUuid());
    }

    object->insert(QOrganizerJsonDbStr::jsonDbType(), QOrganizerJsonDbStr::jsonDbCollectionType());

    if (isDefaultCollection)
        object->insert(QOrganizerJsonDbStr::collectionDefaultFlag(), isDefaultCollection);

    QMap<QOrganizerCollection::MetaDataKey, QVariant> metaData = collection.metaData();
    QMap<QOrganizerCollection::MetaDataKey, QVariant>::const_iterator i = metaData.constBegin();
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
        } else if (i.key() == QOrganizerCollection::KeyExtended) {
            QVariantMap variantMap = i.value().toMap();
            if (!variantMap.isEmpty()) {
                QVariantMap::const_iterator j = variantMap.constBegin();
                while (j != variantMap.constEnd()) {
                    QString valueString = j.value().toString();
                    if (!j.key().isEmpty() && !valueString.isEmpty()
                        && (j.key().at(0) != QChar('_') || j.key() == QOrganizerJsonDbStr::jsonDbVersion())
                        && j.key() != QOrganizerJsonDbStr::collectionDefaultFlag()) {
                        // XXX Should we allow complex data structure, i.e. list or map, for custom meta data?
                        object->insert(j.key(), valueString);
                    }
                    ++j;
                }
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

const QStringList QOrganizerJsonDbConverter::storageLocationsFlagToStrings(const QOrganizerAbstractRequest::StorageLocations storageLocationsFlag)
{
    QStringList storageLocations;

    if (QOrganizerAbstractRequest::UserDataStorage & storageLocationsFlag)
        storageLocations.append(enumToString(organizerStorageLocationMap(), QOrganizerAbstractRequest::UserDataStorage));
    if (QOrganizerAbstractRequest::SystemStorage & storageLocationsFlag)
        storageLocations.append(enumToString(organizerStorageLocationMap(), QOrganizerAbstractRequest::SystemStorage));

    return storageLocations;
}

QOrganizerAbstractRequest::StorageLocation QOrganizerJsonDbConverter::storageLocationStringToEnum(const QString &storageLocation)
{
    return QOrganizerAbstractRequest::StorageLocation(stringToEnum(organizerStorageLocationMap(), storageLocation));
}

QOrganizerAbstractRequest::StorageLocations QOrganizerJsonDbConverter::storageLocationListToFlag(const QList<QOrganizerAbstractRequest::StorageLocation> storageLocationsList)
{
    QOrganizerAbstractRequest::StorageLocations locationsFlag(0);
    foreach (QOrganizerAbstractRequest::StorageLocation location, storageLocationsList) {
        locationsFlag |= location;
    }
    return locationsFlag;
}

void QOrganizerJsonDbConverter::itemVersionToJsonDbVersion(const QOrganizerItemVersion &itemVersion, QString *jsonDbVersion) const
{
    int version = itemVersion.version();
    QByteArray extendedVersion = itemVersion.extendedVersion();

    if (version > 0) {
        *jsonDbVersion = QString::number(version) + QStringLiteral("-") + QString::fromLatin1(extendedVersion.constData());
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToRecurrenceRule(const QJsonObject &object, QOrganizerRecurrenceRule *rule) const
{
    QJsonObject::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleFrequency()) {
            QString frequency = i.value().toString();
            if (!frequency.isEmpty())
                rule->setFrequency(static_cast<QOrganizerRecurrenceRule::Frequency>(stringToEnum(organizerFrequencyEnumMap(), frequency)));
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleInterval()) {
            int interval = i.value().toDouble();
            if (interval >= 0)
                rule->setInterval(interval);
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleLimitCount()) {
            int limitCount = i.value().toDouble();
            if (limitCount >= 0)
                rule->setLimit(limitCount);
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleLimitDate()) {
            QDate limitDate = QDate::fromString(i.value().toString(), Qt::ISODate);
            if (limitDate.isValid())
                rule->setLimit(limitDate);
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleFirstDayOfWeek()) {
            QString firstDayOfWeek = i.value().toString();
            if (!firstDayOfWeek.isEmpty())
                rule->setFirstDayOfWeek(static_cast<Qt::DayOfWeek>(stringToEnum(organizerDayEnumMap(), firstDayOfWeek)));
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRulePositions()) {
            QJsonArray jsonDbPositions = i.value().toArray();
            if (!jsonDbPositions.isEmpty()) {
                QSet<int> positionsSet;
                foreach (const QJsonValue &jsonDbPosition, jsonDbPositions)
                    positionsSet.insert(jsonDbPosition.toDouble());
                rule->setPositions(positionsSet);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfWeek()) {
            QJsonArray jsonDbDaysOfWeek = i.value().toArray();
            if (!jsonDbDaysOfWeek.isEmpty()) {
                QSet<Qt::DayOfWeek> daysOfWeek;
                foreach (const QJsonValue &jsonDbDayOfWeek, jsonDbDaysOfWeek)
                    daysOfWeek.insert(static_cast<Qt::DayOfWeek>(stringToEnum(organizerDayEnumMap(), jsonDbDayOfWeek.toString())));
                rule->setDaysOfWeek(daysOfWeek);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfMonth()) {
            QJsonArray jsonDbDaysOfMonth = i.value().toArray();
            if (!jsonDbDaysOfMonth.isEmpty()) {
                QSet<int> daysOfMonth;
                foreach (const QJsonValue &jsonDbDayOfMonth, jsonDbDaysOfMonth)
                    daysOfMonth.insert(jsonDbDayOfMonth.toDouble());
                rule->setDaysOfMonth(daysOfMonth);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfYear()) {
            QJsonArray jsonDbDaysOfYear = i.value().toArray();
            if (!jsonDbDaysOfYear.isEmpty()) {
                QSet<int> daysOfYear;
                foreach (const QJsonValue &jsonDbDayOfYear, jsonDbDaysOfYear)
                    daysOfYear.insert(jsonDbDayOfYear.toDouble());
                rule->setDaysOfYear(daysOfYear);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleWeeksOfYear()) {
            QJsonArray jsonDbWeeksOfYear = i.value().toArray();
            if (!jsonDbWeeksOfYear.isEmpty()) {
                QSet<int> weeksOfYear;
                foreach (const QJsonValue &jsonDbWeekOfYear, jsonDbWeeksOfYear)
                    weeksOfYear.insert(jsonDbWeekOfYear.toDouble());
                rule->setWeeksOfYear(weeksOfYear);
            }
        } else if (i.key() == QOrganizerJsonDbStr::itemRecurrenceRuleMonthsOfYear()) {
            QJsonArray jsonDbMonthsOfYear = i.value().toArray();
            if (!jsonDbMonthsOfYear.isEmpty()) {
                QSet<QOrganizerRecurrenceRule::Month> monthsOfYear;
                foreach (const QJsonValue &jsonDbMonthOfYear, jsonDbMonthsOfYear)
                    monthsOfYear.insert(static_cast<QOrganizerRecurrenceRule::Month>(stringToEnum(organizerMonthEnumMap(), jsonDbMonthOfYear.toString())));
                rule->setMonthsOfYear(monthsOfYear);
            }
        }

        ++i;
    }
}

void QOrganizerJsonDbConverter::recurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule &rule, QJsonObject *object) const
{
    // Invalid is the default value, so no need to save
    QOrganizerRecurrenceRule::Frequency frequency = rule.frequency();
    if (frequency != QOrganizerRecurrenceRule::Invalid)
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleFrequency(), enumToString(organizerFrequencyEnumMap(), frequency));

    // 1 is the default value, so no need to save
    int interval = rule.interval();
    if (interval > 1)
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleInterval(), rule.interval());

    // only saves the limit value if it's used
    QOrganizerRecurrenceRule::LimitType limitType = rule.limitType();
    if (limitType == QOrganizerRecurrenceRule::CountLimit)
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleLimitCount(), rule.limitCount());
    else if (limitType == QOrganizerRecurrenceRule::DateLimit)
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleLimitDate(), rule.limitDate().toString(Qt::ISODate));

    // Monday is the default value, so no need to save
    Qt::DayOfWeek firstDayOfWeek = rule.firstDayOfWeek();
    if (firstDayOfWeek != Qt::Monday)
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleFirstDayOfWeek(), enumToString(organizerDayEnumMap(), firstDayOfWeek));

    QSet<int> positions = rule.positions();
    if (!positions.isEmpty()) {
        QJsonArray positionsList;
        foreach (int position, positions)
            positionsList.append(position);
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRulePositions(), positionsList);
    }

    QSet<Qt::DayOfWeek> daysOfWeek = rule.daysOfWeek();
    if (!daysOfWeek.isEmpty()) {
        QJsonArray daysOfWeekList;
        foreach (Qt::DayOfWeek day, daysOfWeek)
            daysOfWeekList.append(enumToString(organizerDayEnumMap(), day));
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfWeek(), daysOfWeekList);
    }

    QSet<int> daysOfMonth = rule.daysOfMonth();
    if (!daysOfMonth.isEmpty()) {
        QJsonArray daysOfMonthList;
        foreach (int day, daysOfMonth)
            daysOfMonthList.append(day);
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfMonth(), daysOfMonthList);
    }

    QSet<int> daysOfYear = rule.daysOfYear();
    if (!daysOfYear.isEmpty()) {
        QJsonArray daysOfYearList;
        foreach (int day, daysOfYear)
            daysOfYearList.append(day);
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleDaysOfYear(), daysOfYearList);
    }

    QSet<int> weeksOfYear = rule.weeksOfYear();
    if (!weeksOfYear.isEmpty()) {
        QJsonArray weeksOfYearList;
        foreach (int week, weeksOfYear)
            weeksOfYearList.append(week);
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleWeeksOfYear(), weeksOfYearList);
    }

    QSet<QOrganizerRecurrenceRule::Month> monthsOfYear = rule.monthsOfYear();
    if (!monthsOfYear.isEmpty()) {
        QJsonArray monthsOfYearList;
        foreach (QOrganizerRecurrenceRule::Month month, monthsOfYear)
            monthsOfYearList.append(enumToString(organizerMonthEnumMap(), month));
        object->insert(QOrganizerJsonDbStr::itemRecurrenceRuleMonthsOfYear(), monthsOfYearList);
    }
}

void QOrganizerJsonDbConverter::audibleReminderDetailToJsonDbObject(const QOrganizerItemAudibleReminder &itemReminder, QJsonObject *object) const
{
    const QMap<int, QVariant> reminderValues = itemReminder.values();
    QMap<int, QVariant>::const_iterator i = reminderValues.constBegin();
    while (i != reminderValues.constEnd()) {
        if (i.key() == QOrganizerItemAudibleReminder::FieldSecondsBeforeStart) {
            int secondsBeforeStart = i.value().toInt();
            if (secondsBeforeStart >= 0)
                object->insert(QOrganizerJsonDbStr::itemReminderSecBeforeStart(), secondsBeforeStart);
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldRepetitionCount) {
            int repetitionCount = i.value().toInt();
            if (repetitionCount > 0)
                object->insert(QOrganizerJsonDbStr::itemReminderRepetitionCount(), repetitionCount);
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldRepetitionDelay) {
            int repetitionDelay = i.value().toInt();
            if (repetitionDelay > 0)
                object->insert(QOrganizerJsonDbStr::itemReminderRepetitionDelay(), repetitionDelay);
        } else if (i.key() == QOrganizerItemAudibleReminder::FieldDataUrl) {
            if (i.value().toUrl().isValid())
                object->insert(QOrganizerJsonDbStr::itemReminderDataUrl(), i.value().toString());
        }
        ++i;
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToAudibleReminderDetail(const QJsonObject &object, QOrganizerItemAudibleReminder *itemReminder,
                                                                    QOrganizerItemExtendedDetail *extendedDetail) const
{
    QJsonObject::const_iterator i = object.constBegin();
    QVariantMap customFields;
    while (i != object.constEnd()) {
        if (i.key() == QOrganizerJsonDbStr::itemReminderSecBeforeStart()) {
            int seconds = i.value().toDouble();
            if (seconds >= 0)
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldSecondsBeforeStart, seconds);
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderRepetitionCount()) {
            int repetitionCount = i.value().toDouble();
            if (repetitionCount > 0)
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldRepetitionCount, repetitionCount);
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderRepetitionDelay()) {
            int repetitionDelay = i.value().toDouble();
            if (repetitionDelay > 0)
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldRepetitionDelay, repetitionDelay);
        } else if (i.key() == QOrganizerJsonDbStr::itemReminderDataUrl()) {
            QUrl url(i.value().toString());
            if (url.isValid())
                itemReminder->setValue(QOrganizerItemAudibleReminder::FieldDataUrl, url);
        } else {
            customFields.insert(i.key(), i.value().toVariant());
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

bool QOrganizerJsonDbConverter::compoundFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr, int *typeFilterFlag) const
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
            int subtypeFilterFlag = supportedItemTypeFlag();
            if (compoundFilterToJsondbQuery(filter, &filterStr, &subtypeFilterFlag)) {
                *jsonDbQueryStr += filterStr;
                *typeFilterFlag = *typeFilterFlag & subtypeFilterFlag;
            } else {//For intersection filter, single filter invalid means empty result from jsondb query
                isValidFilter = false;
            }
        }
        return isValidFilter;
    }
    case QOrganizerItemFilter::UnionFilter: {
        const QOrganizerItemUnionFilter uf(filter);
        const QList<QOrganizerItemFilter> filterList = uf.filters();
        int validFilterCount = 0;
        *typeFilterFlag = 0;
        foreach (const QOrganizerItemFilter &filter, filterList){
            //query filter1 filter2 filter3 ...
            //query [?definition="value" | definition="value" | definition="value"]
            QString filterStr;
            int subtypeFilterFlag = supportedItemTypeFlag();
            if (compoundFilterToJsondbQuery(filter, &filterStr, &subtypeFilterFlag)) {
                *jsonDbQueryStr += filterStr;
                validFilterCount ++;
                *typeFilterFlag = *typeFilterFlag | subtypeFilterFlag;
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
        isValidFilter = singleFilterToJsondbQuery(filter, jsonDbQueryStr, typeFilterFlag);
        break;
    }

    if (!isValidFilter)
        jsonDbQueryStr->clear();

    return isValidFilter;
}

bool QOrganizerJsonDbConverter::singleFilterToJsondbQuery(const QOrganizerItemFilter& filter, QString *jsonDbQueryStr, int *typeFilterFlag) const
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
        isValidFilter = detailFilterToJsondbQuery(filter, jsonDbQueryStr, typeFilterFlag);
        break;
    default:
        break;
    }

    return isValidFilter;
}

QString QOrganizerJsonDbConverter::jsonDbNotificationObjectToOrganizerType(const QJsonObject &object) const
{
    return object.value(QOrganizerJsonDbStr::jsonDbType()).toString();
}

QOrganizerItemId QOrganizerJsonDbConverter::jsonDbNotificationObjectToItemId(const QJsonObject &object, QOrganizerAbstractRequest::StorageLocation storageLocation) const
{
    QString jsonDbUuid = object.value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
    if (jsonDbUuid.isEmpty()) {
        return QOrganizerItemId();
    } else {
        QOrganizerJsonDbItemId *jsondbItemId = new QOrganizerJsonDbItemId();
        jsondbItemId->setJsonDbUuid(jsonDbUuid);
        jsondbItemId->setStorageLocation(storageLocation);
        return QOrganizerItemId(jsondbItemId);
    }
}

QOrganizerCollectionId QOrganizerJsonDbConverter::jsonDbNotificationObjectToCollectionId(const QJsonObject &object, QOrganizerAbstractRequest::StorageLocation storageLocation) const
{
    QString jsonUuid = object.value(QOrganizerJsonDbStr::jsonDbUuid()).toString();
    if (jsonUuid.isEmpty()) {
        return QOrganizerCollectionId();
    } else {
        QOrganizerJsonDbCollectionId *jsondbCollectionId = new QOrganizerJsonDbCollectionId();
        jsondbCollectionId->setJsonDbUuid(jsonUuid);
        jsondbCollectionId->setStorageLocation(storageLocation);
        return QOrganizerCollectionId(jsondbCollectionId);
    }
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
                query += idTemplate.arg(QOrganizerJsonDbCollectionId(id.toString()).jsondbUuid());
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
                query += uuidTemplate.arg(QOrganizerJsonDbItemId(id.toString()).jsondbUuid());
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
    } else if (QOrganizerItemDetail::TypeJournalTime == detailType
        || QOrganizerItemDetail::TypeReminder == detailType
        || QOrganizerItemDetail::TypeAudibleReminder == detailType
        || QOrganizerItemDetail::TypeVisualReminder == detailType
        || QOrganizerItemDetail::TypeEmailReminder == detailType
        || QOrganizerItemDetail::TypeRecurrence == detailType
        || QOrganizerItemDetail::TypeTimestamp == detailType
        || QOrganizerItemDetail::TypeEventAttendee == detailType) {
        // filtering certain details/definitions are currently not supported
        isValidFilter = false;
    } else if (QOrganizerItemFilter::MatchExactly != filter.matchFlags()
        && (QOrganizerItemDetail::TypeEventTime == detailType
        || QOrganizerItemDetail::TypeTodoTime == detailType
        || QOrganizerItemDetail::TypeTodoProgress == detailType
        || QOrganizerItemDetail::TypeComment == detailType
        || (QOrganizerItemDetail::TypeLocation == detailType && (QOrganizerItemLocation::FieldLatitude == detailFieldName || QOrganizerItemLocation::FieldLongitude == detailFieldName))
        || QOrganizerItemDetail::TypePriority == detailType
        || QOrganizerItemDetail::TypeItemType == detailType
        || QOrganizerItemDetail::TypeTag == detailType
        || QOrganizerItemDetail::TypeExtendedDetail == detailType
        || (QOrganizerItemDetail::TypeEventRsvp == detailType && (QOrganizerEventRsvp::FieldParticipationStatus == detailFieldName
                                                                  || QOrganizerEventRsvp::FieldParticipationRole == detailFieldName
                                                                  || QOrganizerEventRsvp::FieldResponseRequirement == detailFieldName
                                                                  || QOrganizerEventRsvp::FieldResponseDeadline == detailFieldName
                                                                  || QOrganizerEventRsvp::FieldResponseDate == detailFieldName))
        || QOrganizerItemDetail::TypeParent == detailType)) {
        // filtering matchflags are not supported for all the types
        isValidFilter = false;
    } else if (QVariant::String == filter.value().type()
        && !(QOrganizerItemDetail::TypeComment == detailType
        || QOrganizerItemDetail::TypeDescription == detailType
        || QOrganizerItemDetail::TypeDisplayLabel == detailType
        || QOrganizerItemDetail::TypeGuid == detailType
        || (QOrganizerItemDetail::TypeLocation == detailType && QOrganizerItemLocation::FieldLabel == detailFieldName)
        || QOrganizerItemDetail::TypeTag == detailType
        || QOrganizerItemDetail::TypeExtendedDetail == detailType
        || QOrganizerItemDetail::TypeItemType == detailType
        || (QOrganizerItemDetail::TypeEventRsvp == detailType && (QOrganizerEventRsvp::FieldOrganizerEmail == detailFieldName
                                                                  || QOrganizerEventRsvp::FieldOrganizerName == detailFieldName)))) {
        // filtering with QString needs extra attention, not allowed for all the types
        isValidFilter = false;
    }
    return isValidFilter;
}

bool QOrganizerJsonDbConverter::detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr, int *typeFilterFlag) const
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

        if (QOrganizerItemDetail::TypeEventTime == detailType) {
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

        } else if (QOrganizerItemDetail::TypeTodoTime == detailType) {
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

        } else if (QOrganizerItemDetail::TypeComment == detailType
            && QOrganizerItemComment::FieldComment == detailField) {
            jsonDbQueryStr->append(containsQueryTemplate.arg(QOrganizerJsonDbStr::itemComments()).arg(valueString));

        } else if (QOrganizerItemDetail::TypeDescription == detailType
            && QOrganizerItemDescription::FieldDescription == detailField) {
            jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemDescription()).arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemDetail::TypeDisplayLabel == detailType
            && QOrganizerItemDisplayLabel::FieldLabel == detailField) {
           jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemDisplayName())
                .arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemDetail::TypeGuid == detailType
            && QOrganizerItemGuid::FieldGuid ==  detailField) {
            jsonDbQueryStr->append(matchFlagQueryTemplate
                .arg(QOrganizerJsonDbStr::itemGuid())
                .arg(createMatchFlagQuery(valueString, df.matchFlags())));

        } else if (QOrganizerItemDetail::TypeLocation == detailType) {
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

        } else if (QOrganizerItemDetail::TypePriority == detailType
            && QOrganizerItemPriority::FieldPriority ==  detailField) {
            jsonDbQueryStr->append(equalsQueryTemplate
                .arg(QOrganizerJsonDbStr::itemPriority())
                .arg(enumToString(organizerPriorityEnumMap(), df.value().toInt())));

        } else if (QOrganizerItemDetail::TypeItemType == detailType
            && QOrganizerItemType::FieldType ==  detailField) {
            jsonDbQueryStr->append(equalsQueryTemplate
                .arg(QOrganizerJsonDbStr::jsonDbType())
                .arg(QOrganizerJsonDbStr::jsonDbSchemaPrefix() + enumToString(organizerItemTypeMap(), df.value().toInt())));
            *typeFilterFlag = 0x01 << (df.value().toInt() - QOrganizerItemType::TypeUndefined);

        } else if (QOrganizerItemDetail::TypeTag == detailType
            && QOrganizerItemTag::FieldTag == detailField) {
            jsonDbQueryStr->append(containsQueryTemplate.arg(QOrganizerJsonDbStr::itemTags()).arg(valueString));

        } else if (QOrganizerItemDetail::TypeParent == detailType) {
            if (QOrganizerItemParent::FieldParentId == detailField) {
                const QOrganizerItemEngineId *itemIdPtr = QOrganizerManagerEngine::engineItemId(df.value().value<QOrganizerItemId>());
                if (!itemIdPtr) {
                    isValidFilter = false;
                } else {
                    jsonDbQueryStr->append(equalsQueryTemplate
                        .arg(QOrganizerJsonDbStr::itemOccurrenceParent())
                        .arg(itemIdPtr->toString()));
                }
            } else if (QOrganizerItemParent::FieldOriginalDate == detailField) {
                jsonDbQueryStr->append(equalsQueryTemplate
                    .arg(QOrganizerJsonDbStr::itemOccurrenceOriginalDate()).arg(df.value().toDate().toString(Qt::ISODate)));
            }

        } else if (QOrganizerItemDetail::TypeExtendedDetail == detailType
             && QOrganizerItemExtendedDetail::FieldName ==  detailField) {
            jsonDbQueryStr->append(existsQueryTemplate.arg(valueString));

        } else if (QOrganizerItemDetail::TypeEventRsvp == detailType) {
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

int QOrganizerJsonDbConverter::supportedItemTypeFlag() const
{
//     int typeFilterFlag = 0x01 << QOrganizerItemType::TypeUndefined - QOrganizerItemType::TypeUndefined
//                        + 0x01 << (QOrganizerItemType::TypeEvent - QOrganizerItemType::TypeUndefined)
//                        + 0x01 << (QOrganizerItemType::TypeEventOccurrence - QOrganizerItemType::TypeUndefined);
//                        + 0x01 << (QOrganizerItemType::TypeTodo - QOrganizerItemType::TypeUndefined);
//                        + 0x01 << (QOrganizerItemType::TypeTodoOccurrence - QOrganizerItemType::TypeUndefined);
    return 0xf1;//typeFilterFlag;
}

bool QOrganizerJsonDbConverter::itemTypeFlagToJsonDbEventQuery(const int &flag, QString *jsonDbQueryStr)
{
    if (!flag)
        return false;
    if (supportedItemTypeFlag() == flag)
        *jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryAllEventItems() + *jsonDbQueryStr;
    else if (!((flag >> (QOrganizerItemType::TypeEvent - QOrganizerItemType::TypeUndefined)) & 0x01)
             && !((flag >> (QOrganizerItemType::TypeEventOccurrence - QOrganizerItemType::TypeUndefined)) & 0x01)) {
        return false;
    }
    return true;
}

bool QOrganizerJsonDbConverter::itemTypeFlagToJsonDbTodoQuery(const int &flag, QString *jsonDbQueryStr)
{
    if (!flag)
        return false;
    if (supportedItemTypeFlag() == flag)
        *jsonDbQueryStr = QOrganizerJsonDbStr::jsonDbQueryAllTodoItems() + *jsonDbQueryStr;
    else if (!((flag >> (QOrganizerItemType::TypeTodo - QOrganizerItemType::TypeUndefined)) & 0x01)
             && !((flag >> (QOrganizerItemType::TypeTodoOccurrence - QOrganizerItemType::TypeUndefined)) & 0x01)) {
        return false;
    }
    return true;
}

bool QOrganizerJsonDbConverter::createJsonDbQuery(const QOrganizerItemFilter &filter, const QDateTime &startTime, const QDateTime &endTime, QList<QString> *jsonDbQueryList)
{
    QString filterString;
    int typeFilterFlag = supportedItemTypeFlag();
    if (compoundFilterToJsondbQuery(filter, &filterString, &typeFilterFlag)) {
        if (startTime.isValid() || endTime.isValid()) {
            QString eventJsonDbQuery;
            if (itemTypeFlagToJsonDbEventQuery(typeFilterFlag, &eventJsonDbQuery)) {
                eventJsonDbQuery += filterString;
                if (startTime.isValid())
                    eventJsonDbQuery += QOrganizerJsonDbStr::jsonDbQueryEventEndDateTimeTemplate().arg(startTime.toUTC().toString(Qt::ISODate));
                if (endTime.isValid())
                    eventJsonDbQuery += QOrganizerJsonDbStr::jsonDbQueryEventStartDateTimeTemplate().arg(endTime.toUTC().toString(Qt::ISODate));
                jsonDbQueryList->append(eventJsonDbQuery);
            }

            if (itemTypeFlagToJsonDbTodoQuery(typeFilterFlag, &filterString))
                jsonDbQueryList->append(filterString);
        } else {
            //No time period terms and create one request for both todo and event
            if (supportedItemTypeFlag() == typeFilterFlag)
                filterString = QOrganizerJsonDbStr::jsonDbQueryAllItems() + filterString;
            jsonDbQueryList->append(filterString);
        }
        return true;
    }
    return false;
}

QTORGANIZER_END_NAMESPACE
