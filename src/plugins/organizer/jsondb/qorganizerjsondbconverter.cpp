/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "qorganizerjsondbstring_p.h"
#include "qorganizer.h"

#include <private/jsondb-strings_p.h>

Q_USE_JSONDB_NAMESPACE

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

    case JsonDbError::MissingTYPE://Remove no-exist item get this error from jsondb
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
    QSet<QOrganizerRecurrenceRule> recurrenceRules;
    QSet<QOrganizerRecurrenceRule> exceptionRules;
    QSet<QDate> recurrenceDates;
    QSet<QDate> exceptionDates;

    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty()) {
        return false;
    }
    item->setId(QOrganizerItemId(new QOrganizerJsonDbItemId(jsonUuid)));
    item->setGuid(jsonUuid);

    QString jsonCollectionUuid = object.value(QOrganizerJsonDbStr::ItemCollectionId).toString();
    item->setCollectionId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonCollectionUuid)));

    if (object.contains(QOrganizerJsonDbStr::ItemRecurrenceRules)) {
        QVariantList recurrenceRulesList = object.value(QOrganizerJsonDbStr::ItemRecurrenceRules).toList();
        foreach (QVariant recurrenceRule, recurrenceRulesList) {
            QOrganizerRecurrenceRule rule;
            jsonDbObjectToRecurrenceRule(recurrenceRule.toMap(), &rule);
            recurrenceRules.insert(rule);
        }
    }

    if (object.contains(QOrganizerJsonDbStr::ItemExceptionRules)) {
        QVariantList exceptionRulesList = object.value(QOrganizerJsonDbStr::ItemExceptionRules).toList();
        foreach (QVariant exceptionRule, exceptionRulesList) {
            QOrganizerRecurrenceRule rule;
            jsonDbObjectToRecurrenceRule(exceptionRule.toMap(), &rule);
            exceptionRules.insert(rule);
        }
    }

    if (object.contains(QOrganizerJsonDbStr::ItemRecurrenceDates)) {
        QVariantList recurrenceDatesList = object.value(QOrganizerJsonDbStr::ItemRecurrenceDates).toList();
        foreach (QVariant recurrenceDate, recurrenceDatesList)
            recurrenceDates.insert(recurrenceDate.toDate());
    }

    if (object.contains(QOrganizerJsonDbStr::ItemExceptioneDates)) {
        QVariantList exceptionDatesList = object.value(QOrganizerJsonDbStr::ItemExceptioneDates).toList();
        foreach (QVariant exceptionDate, exceptionDatesList)
            exceptionDates.insert(exceptionDate.toDate());
    }

    QString displayLabel = object.value(QOrganizerJsonDbStr::ItemName).toString();
    if (!displayLabel.isEmpty())
        item->setDisplayLabel(displayLabel);

    QString description = object.value(QOrganizerJsonDbStr::ItemDescription).toString();
    if (!description.isEmpty())
        item->setDescription(description);

    QStringList comments(object.value(QOrganizerJsonDbStr::ItemComments).toStringList());
    if (!comments.isEmpty())
        item->setComments(comments);

    QStringList tags(object.value(QOrganizerJsonDbStr::ItemTags).toStringList());
    if (!tags.isEmpty())
        item->setTags(tags);

    QVariantMap location = object.value(QOrganizerJsonDbStr::ItemLocation).toMap();
    if (!location.isEmpty()) {
        QOrganizerItemLocation locationDetail;

        QString label = location.value(QOrganizerJsonDbStr::ItemLocationLabel).toString();
        if (!label.isEmpty())
            locationDetail.setLabel(label);

        double latitude = location.value(QOrganizerJsonDbStr::ItemLocationLatitude).toDouble();
        locationDetail.setLatitude(latitude);

        double longitude = location.value(QOrganizerJsonDbStr::ItemLocationLongitude).toDouble();
        locationDetail.setLongitude(longitude);

        item->saveDetail(&locationDetail);
    }

    //Visual reminder
    QVariantMap jsondbVisualReminder = object.value(QOrganizerJsonDbStr::ItemVisualReminder).toMap();
    if (!jsondbVisualReminder.isEmpty()) {
        QOrganizerItemVisualReminder visualReminder;
        jsonDbObjectToItemReminderDetailCommon(jsondbVisualReminder, &visualReminder);
        QString message = jsondbVisualReminder.value(QOrganizerJsonDbStr::ItemVisualReminderMessage).toString();
        if (!message.isEmpty())
            visualReminder.setMessage(message);
        QString dataUrlStr = jsondbVisualReminder.value(QOrganizerJsonDbStr::ItemVisualReminderDataUrl).toString();
        if (!dataUrlStr.isEmpty())
            visualReminder.setDataUrl(QUrl(dataUrlStr));
        item->saveDetail(&visualReminder);
    }
    //Audible reminder
    QVariantMap jsondbAudibleReminder = object.value(QOrganizerJsonDbStr::ItemAudibleReminder).toMap();
    if (!jsondbAudibleReminder.isEmpty()) {
        QOrganizerItemAudibleReminder audibleReminder;
        jsonDbObjectToItemReminderDetailCommon(jsondbAudibleReminder, &audibleReminder);
        QString dataUrlStr = jsondbAudibleReminder.value(QOrganizerJsonDbStr::ItemAudibleReminderDataUrl).toString();
        if (!dataUrlStr.isEmpty()) {
            QUrl url(dataUrlStr);
            audibleReminder.setDataUrl(url);
        }
        item->saveDetail(&audibleReminder);
    }
    //Email reminder
    QVariantMap jsondbEmailReminder = object.value(QOrganizerJsonDbStr::ItemEmailReminder).toMap();
    if (!jsondbEmailReminder.isEmpty()) {
        QOrganizerItemEmailReminder emailReminder;
        jsonDbObjectToItemReminderDetailCommon(jsondbEmailReminder, &emailReminder);
        QString subject = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderSubject).toString();
        if (!subject.isEmpty())
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldSubject, subject);
        QString body = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderBody).toString();
        if (!body.isEmpty())
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldBody, body);
        if (jsondbEmailReminder.contains(QOrganizerJsonDbStr::ItemEmailReminderAttachments)) {
            QVariantList attachments = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderAttachments).toList();
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldAttachments, attachments);
        }
        if (jsondbEmailReminder.contains(QOrganizerJsonDbStr::ItemEmailReminderRecipients)) {
            QStringList recipients = jsondbEmailReminder.value(QOrganizerJsonDbStr::ItemEmailReminderRecipients).toStringList();
            emailReminder.setRecipients(recipients);
        }
        item->saveDetail(&emailReminder);
    }

    // custom details
    QMap<QString, QVariant> customDetailsMap = object.value(QOrganizerJsonDbStr::ItemCustomFields).toMap();
    if (!customDetailsMap.isEmpty()) {
        QList<QOrganizerItemCustomDetail> customDetails;
        jsonDbObjectToCustomDetails(customDetailsMap, customDetails);
        foreach (QOrganizerItemCustomDetail detail, customDetails)
            item->saveDetail(&detail);
    }

    //Get "_type" value
    QString typeStr = object.value(JsonDbString::kTypeStr).toString();
    if (QOrganizerJsonDbStr::Event == typeStr) {
        QOrganizerEvent* event = static_cast<QOrganizerEvent*>(item);
        event->setType(QOrganizerItemType::TypeEvent);

        QDateTime startTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::EventStartDateTime).toString(), Qt::ISODate);
        if (startTime.isValid ())
            event->setStartDateTime(startTime);

        QDateTime endTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::EventEndDateTime).toString(), Qt::ISODate);
        if (endTime.isValid ())
            event->setEndDateTime(endTime);

        QVariant isAllDay = object.value(QOrganizerJsonDbStr::EventIsAllDay);
        if (!isAllDay.isNull())
            event->setAllDay(isAllDay.toBool());

        if (object.contains(QOrganizerJsonDbStr::ItemPriority)) {
            QString priority = object.value(QOrganizerJsonDbStr::ItemPriority).toString();
            if (!priority.isEmpty())
                event->setPriority(static_cast<QOrganizerItemPriority::Priority>(
                                       stringToEnum(qt_organizerPriorityEnumMap, priority)));
        }

        if (!recurrenceRules.isEmpty())
            event->setRecurrenceRules(recurrenceRules);

        if (!exceptionRules.isEmpty())
            event->setExceptionRules(exceptionRules);

        if (!recurrenceDates.isEmpty())
            event->setRecurrenceDates(recurrenceDates);

        if (!exceptionDates.isEmpty())
            event->setExceptionDates(exceptionDates);

    } else if (QOrganizerJsonDbStr::Todo == typeStr) {
        QOrganizerTodo* todo = static_cast<QOrganizerTodo*>(item);
        todo->setType(QOrganizerItemType::TypeTodo);

        QDateTime startTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::TodoStartDateTime).toString(), Qt::ISODate);
        if (startTime.isValid ())
            todo->setStartDateTime(startTime);

        QDateTime dueTime = QDateTime::fromString(object.value(QOrganizerJsonDbStr::TodoDueDateTime).toString(), Qt::ISODate);
        if (dueTime.isValid ())
            todo->setDueDateTime(dueTime);

        if (object.value(QOrganizerJsonDbStr::EventIsAllDay).toBool())
            todo->setAllDay(object.value(QOrganizerJsonDbStr::TodoIsAllDay).toBool());

        if (object.contains(QOrganizerJsonDbStr::ItemPriority)) {
            QString priority = object.value(QOrganizerJsonDbStr::ItemPriority).toString();
            if (!priority.isEmpty())
                todo->setPriority(static_cast<QOrganizerItemPriority::Priority>(
                                      stringToEnum(qt_organizerPriorityEnumMap, priority)));
        }

        if (!recurrenceRules.isEmpty())
            todo->setRecurrenceRules(recurrenceRules);

        if (!exceptionRules.isEmpty())
            todo->setExceptionRules(exceptionRules);

        if (!recurrenceDates.isEmpty())
            todo->setRecurrenceDates(recurrenceDates);

        if (!exceptionDates.isEmpty())
            todo->setExceptionDates(exceptionDates);

    } else {
        //Item type is not supported
        return false;
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
        object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::Event);

        QOrganizerEventTime eventTime = event.detail(QOrganizerEventTime::DefinitionName);
        if (eventTime.hasValue(QOrganizerEventTime::FieldStartDateTime))
            object->insert(QOrganizerJsonDbStr::EventStartDateTime, eventTime.startDateTime().toString(Qt::ISODate));
        if (eventTime.hasValue(QOrganizerEventTime::FieldEndDateTime))
            object->insert(QOrganizerJsonDbStr::EventEndDateTime, eventTime.endDateTime().toString(Qt::ISODate));
        if (eventTime.hasValue(QOrganizerEventTime::FieldAllDay))
            object->insert(QOrganizerJsonDbStr::EventIsAllDay, eventTime.isAllDay());

        recurrenceRules = event.recurrenceRules();
        exceptionRules = event.exceptionRules();
        recurrenceDates = event.recurrenceDates();
        exceptionDates = event.exceptionDates();
    } else if (item.type() == QOrganizerItemType::TypeTodo) {
        QOrganizerTodo todo = static_cast<QOrganizerTodo>(item);
        object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::Todo);

        QOrganizerTodoTime todoTime = todo.detail(QOrganizerTodoTime::DefinitionName);
        if (todoTime.hasValue(QOrganizerTodoTime::FieldStartDateTime))
            object->insert(QOrganizerJsonDbStr::TodoStartDateTime, todoTime.startDateTime().toString(Qt::ISODate));
        if (todoTime.hasValue(QOrganizerTodoTime::FieldDueDateTime))
            object->insert(QOrganizerJsonDbStr::TodoDueDateTime, todoTime.dueDateTime().toString(Qt::ISODate));
        if (todoTime.hasValue(QOrganizerTodoTime::FieldAllDay))
            object->insert(QOrganizerJsonDbStr::TodoIsAllDay, todoTime.isAllDay());

        recurrenceRules = todo.recurrenceRules();
        exceptionRules = todo.exceptionRules();
        recurrenceDates = todo.recurrenceDates();
        exceptionDates = todo.exceptionDates();
    } else {
        return false;
    }

    if (!item.id().isNull()) {
        QString jsonUuid = item.id().toString().remove (QOrganizerJsonDbStr::ManagerName);
        object->insert (JsonDbString::kUuidStr, jsonUuid);
    }
    object->insert(QOrganizerJsonDbStr::ItemCollectionId,
                   item.collectionId().toString().remove(QOrganizerJsonDbStr::ManagerName));

    QString displayLabel(item.displayLabel());
    if (!displayLabel.isEmpty())
        object->insert(QOrganizerJsonDbStr::ItemName, displayLabel);

    QString description(item.description());
    if (!description.isEmpty())
        object->insert (QOrganizerJsonDbStr::ItemDescription, description);

    QStringList comments(item.comments());
    if (!comments.isEmpty())
        object->insert(QOrganizerJsonDbStr::ItemComments, comments);

    QStringList tags(item.tags());
    if (!tags.isEmpty())
        object->insert(QOrganizerJsonDbStr::ItemTags, tags);

    QOrganizerItemLocation locationDetail = item.detail(QOrganizerItemLocation::DefinitionName);
    if (!locationDetail.isEmpty()) {
        QVariantMap locationObject;
        locationObject.insert(QOrganizerJsonDbStr::ItemLocationLabel, locationDetail.label());
        locationObject.insert(QOrganizerJsonDbStr::ItemLocationLatitude, locationDetail.latitude());
        locationObject.insert(QOrganizerJsonDbStr::ItemLocationLongitude, locationDetail.longitude());
        object->insert(QOrganizerJsonDbStr::ItemLocation, locationObject);
    }

    QOrganizerItemPriority priorityDetail = item.detail(QOrganizerItemPriority::DefinitionName);
    if (!priorityDetail.isEmpty()) {
        object->insert(QOrganizerJsonDbStr::ItemPriority, enumToString(qt_organizerPriorityEnumMap, priorityDetail.priority()));
    }

    //Audio,visual,email reminder details
    QOrganizerItemVisualReminder visualReminder = item.detail(QOrganizerItemVisualReminder::DefinitionName);
    if (!visualReminder.isEmpty()) {
        QVariantMap reminderObject;
        itemReminderDetailToJsonDbObject(visualReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::ItemVisualReminder, reminderObject);
    }

    QOrganizerItemAudibleReminder audibleReminder = item.detail(QOrganizerItemAudibleReminder::DefinitionName);
    if (!audibleReminder.isEmpty()) {
        QVariantMap reminderObject;
        itemReminderDetailToJsonDbObject(audibleReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::ItemAudibleReminder, reminderObject);
    }

    QOrganizerItemEmailReminder emailReminder = item.detail(QOrganizerItemEmailReminder::DefinitionName);
    if (!emailReminder.isEmpty()) {
        QVariantMap reminderObject;
        itemReminderDetailToJsonDbObject(emailReminder, reminderObject);
        object->insert(QOrganizerJsonDbStr::ItemEmailReminder, reminderObject);
    }

    // recurrence
    if (!recurrenceRules.isEmpty()) {
        QVariantList recurrenceRulesList;
        foreach (QOrganizerRecurrenceRule recurrenceRule, recurrenceRules) {
            QVariantMap recurrenceRuleObject;
            recurrenceRuleToJsonDbObject(recurrenceRule, &recurrenceRuleObject);
            recurrenceRulesList.append(recurrenceRuleObject);
        }
        object->insert(QOrganizerJsonDbStr::ItemRecurrenceRules, recurrenceRulesList);
    }

    if (!exceptionRules.isEmpty()) {
        QVariantList exceptionRulesList;
        foreach (QOrganizerRecurrenceRule exceptionRule, exceptionRules) {
            QVariantMap exceptionRuleObject;
            recurrenceRuleToJsonDbObject(exceptionRule, &exceptionRuleObject);
            exceptionRulesList.append(exceptionRuleObject);
        }
        object->insert(QOrganizerJsonDbStr::ItemExceptionRules, exceptionRulesList);
    }

    if (!recurrenceDates.isEmpty()) {
        QVariantList recurrenceDatesList;
        foreach (QDate recurrenceDate, recurrenceDates) {
            recurrenceDatesList.append(recurrenceDate.toString(Qt::ISODate));
        }
        object->insert(QOrganizerJsonDbStr::ItemRecurrenceDates, recurrenceDatesList);
    }

    if (!exceptionDates.isEmpty()) {
        QVariantList exceptionDatesList;
        foreach (QDate exceptionDate, exceptionDates) {
            exceptionDatesList.append(exceptionDate.toString(Qt::ISODate));
        }
        object->insert(QOrganizerJsonDbStr::ItemExceptioneDates, exceptionDatesList);
    }

    // custom details
    QList<QOrganizerItemCustomDetail> customDetails = item.details<QOrganizerItemCustomDetail>();
    if (!customDetails.isEmpty()) {
        QVariantMap map;
        customDetailsToJsonDbObject(customDetails, map);
        if (!map.isEmpty())
            object->insert(QOrganizerJsonDbStr::ItemCustomFields, map);
    }

    return true;
}

bool QOrganizerJsonDbConverter::jsonDbObjectToCollection(const QVariantMap& object, QOrganizerCollection* collection, bool& isDefaultCollection)
{
    QString jsonUuid = object.value(JsonDbString::kUuidStr).toString();
    if (jsonUuid.isEmpty()) {
        return false;
    }

    collection->setId(QOrganizerCollectionId(new QOrganizerJsonDbCollectionId(jsonUuid)));

    QString name = object.value(QOrganizerJsonDbStr::CollectionName).toString();
    if (!name.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyName, name);

    QString description = object.value(QOrganizerJsonDbStr::CollectionDescription).toString();
    if (!description.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyDescription, description);

    QString color = object.value(QOrganizerJsonDbStr::CollectionColor).toString();
    if (!color.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyColor, color);

    QString image = object.value(QOrganizerJsonDbStr::CollectionImage).toString();
    if (!image.isEmpty())
        collection->setMetaData(QOrganizerCollection::KeyImage, image);

    QVariantMap jsonMetaData = object.value(QOrganizerJsonDbStr::CollectionCustomFields).toMap();
    if (!jsonMetaData.isEmpty()) {
        QList<QString> metaDataKeys = jsonMetaData.keys();
        foreach (QString key, metaDataKeys)
            collection->setMetaData(key, jsonMetaData.value(key));
    }

    isDefaultCollection = object.value(QOrganizerJsonDbStr::CollectionDefaultFlag).toBool();
    return true;
}

bool QOrganizerJsonDbConverter::collectionToJsonDbObject(const QOrganizerCollection& collection, bool isDefaultCollection, QVariantMap* object) const
{
    if (!collection.id().isNull()) {
        QString jsonUuid = collection.id().toString();
        jsonUuid.remove (QOrganizerJsonDbStr::ManagerName);
        object->insert(JsonDbString::kUuidStr, jsonUuid);
    }

    object->insert(JsonDbString::kTypeStr, QOrganizerJsonDbStr::Collection);
    object->insert(QOrganizerJsonDbStr::CollectionDefaultFlag, isDefaultCollection);

    QVariantMap metaData = collection.metaData();
    QVariantMap jsonMetaData;
    QList<QString> metaDataKeys = metaData.keys();
    foreach (QString key, metaDataKeys) {
        //Jsondb accepts string and stringlist type value
        if (collection.metaData(key).canConvert(QVariant::String)) {
            collection.metaData(key).toString();
            if (key == QOrganizerCollection::KeyName)
                object->insert(QOrganizerJsonDbStr::CollectionName, collection.metaData(QOrganizerCollection::KeyName).toString());
            else if (key == QOrganizerCollection::KeyDescription)
                object->insert(QOrganizerJsonDbStr::CollectionDescription, collection.metaData(QOrganizerCollection::KeyDescription).toString());
            else if (key == QOrganizerCollection::KeyColor)
                object->insert(QOrganizerJsonDbStr::CollectionColor, collection.metaData(QOrganizerCollection::KeyColor).toString());
            else if (key == QOrganizerCollection::KeyImage)
                object->insert(QOrganizerJsonDbStr::CollectionImage, collection.metaData(QOrganizerCollection::KeyImage).toString());
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
        object->insert(QOrganizerJsonDbStr::CollectionCustomFields, jsonMetaData);

    return true;
}

void QOrganizerJsonDbConverter::jsonDbObjectToRecurrenceRule(const QVariantMap& object, QOrganizerRecurrenceRule* rule) const
{
    if (object.contains(QOrganizerJsonDbStr::RuleFrequency)) {
        QString frequency = object.value(QOrganizerJsonDbStr::RuleFrequency).toString();
        rule->setFrequency(static_cast<QOrganizerRecurrenceRule::Frequency>(
                               stringToEnum(qt_organizerFrequencyEnumMap, frequency)));
    }
    if (object.contains(QOrganizerJsonDbStr::RuleInterval)) {
        rule->setInterval(object.value(QOrganizerJsonDbStr::RuleInterval).toInt());
    }
    if (object.contains(QOrganizerJsonDbStr::RuleLimitCount)) {
        int limitCount = object.value(QOrganizerJsonDbStr::RuleLimitCount).toInt();
        if (limitCount >= 0)
            rule->setLimit(limitCount);
    }
    if (object.contains(QOrganizerJsonDbStr::RuleLimitDate)) {
        QDate limitDate = object.value(QOrganizerJsonDbStr::RuleLimitDate).toDate();
        if (limitDate.isValid())
            rule->setLimit(limitDate);
    }
    if (object.contains(QOrganizerJsonDbStr::RulePositions)) {
        QVariantList positionsList = object.value(QOrganizerJsonDbStr::RulePositions).toList();
        QSet<int> positions;
        foreach (QVariant position, positions)
            positions.insert(position.toInt());
        rule->setPositions(positions);
    }
    if (object.contains(QOrganizerJsonDbStr::RuleFirstDayOfWeek)) {
        QString firstDayOfWeek = object.value(QOrganizerJsonDbStr::RuleFirstDayOfWeek).toString();
        rule->setFirstDayOfWeek(static_cast<Qt::DayOfWeek>(
                                    stringToEnum(qt_organizerDayEnumMap, firstDayOfWeek)));
    }
    if (object.contains(QOrganizerJsonDbStr::RuleDaysOfWeek)) {
        QVariantList daysOfWeekList = object.value(QOrganizerJsonDbStr::RuleDaysOfWeek).toList();
        QSet<Qt::DayOfWeek> daysOfWeek;
        foreach (QVariant day, daysOfWeekList)
            daysOfWeek.insert(static_cast<Qt::DayOfWeek>(
                                  stringToEnum(qt_organizerDayEnumMap, day.toString())));
        rule->setDaysOfWeek(daysOfWeek);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleDaysOfMonth)) {
        QVariantList daysOfMonthList = object.value(QOrganizerJsonDbStr::RuleDaysOfMonth).toList();
        QSet<int> daysOfMonth;
        foreach (QVariant day, daysOfMonthList)
            daysOfMonth.insert(day.toInt());
        rule->setDaysOfMonth(daysOfMonth);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleDaysOfYear)) {
        QVariantList daysOfYearList = object.value(QOrganizerJsonDbStr::RuleDaysOfYear).toList();
        QSet<int> daysOfYear;
        foreach (QVariant day, daysOfYearList)
            daysOfYear.insert(day.toInt());
        rule->setDaysOfYear(daysOfYear);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleWeeksOfYear)) {
        QVariantList weeksOfYearList = object.value(QOrganizerJsonDbStr::RuleWeeksOfYear).toList();
        QSet<int> weeksOfYear;
        foreach (QVariant week, weeksOfYearList)
            weeksOfYear.insert(week.toInt());
        rule->setWeeksOfYear(weeksOfYear);
    }

    if (object.contains(QOrganizerJsonDbStr::RuleMonthsOfYear)) {
        QVariantList monthsOfYearList = object.value(QOrganizerJsonDbStr::RuleMonthsOfYear).toList();
        QSet<QOrganizerRecurrenceRule::Month> monthsOfYear;
        foreach (QVariant month, monthsOfYearList)
            monthsOfYear.insert(static_cast<QOrganizerRecurrenceRule::Month>(
                                    stringToEnum(qt_organizerMonthEnumMap, month.toString())));
        rule->setMonthsOfYear(monthsOfYear);
    }
}

void QOrganizerJsonDbConverter::recurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule& rule, QVariantMap* object) const
{
    object->insert(QOrganizerJsonDbStr::RuleFrequency, enumToString(qt_organizerFrequencyEnumMap, rule.frequency()));
    object->insert(QOrganizerJsonDbStr::RuleInterval, rule.interval());
    object->insert(QOrganizerJsonDbStr::RuleLimitCount, rule.limitCount());
    object->insert(QOrganizerJsonDbStr::RuleLimitDate, rule.limitDate().toString(Qt::ISODate));
    QSet<int> positions = rule.positions();
    if (!positions.isEmpty()) {
        QVariantList positionsList;
        foreach (int position, positions) {
            positionsList.append(position);
        }
        object->insert(QOrganizerJsonDbStr::RulePositions, positionsList);
    }

    object->insert(QOrganizerJsonDbStr::RuleFirstDayOfWeek, enumToString(qt_organizerDayEnumMap, rule.firstDayOfWeek()));

    QSet<Qt::DayOfWeek> daysOfWeek = rule.daysOfWeek();
    if (!daysOfWeek.isEmpty()) {
        QVariantList daysOfWeekList;
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            daysOfWeekList.append(enumToString(qt_organizerDayEnumMap, day));
        }
        object->insert(QOrganizerJsonDbStr::RuleDaysOfWeek, daysOfWeekList);
    }

    QSet<int> daysOfMonth = rule.daysOfMonth();
    if (!daysOfMonth.isEmpty()) {
        QVariantList daysOfMonthList;
        foreach (int day, daysOfMonth) {
            daysOfMonthList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::RuleDaysOfMonth, daysOfMonthList);
    }

    QSet<int> daysOfYear = rule.daysOfYear();
    if (!daysOfYear.isEmpty()) {
        QVariantList daysOfYearList;
        foreach (int day, daysOfYear) {
            daysOfYearList.append(day);
        }
        object->insert(QOrganizerJsonDbStr::RuleDaysOfYear, daysOfYearList);
    }

    QSet<int> weeksOfYear = rule.weeksOfYear();
    if (!weeksOfYear.isEmpty()) {
        QVariantList weeksOfYearList;
        foreach (int week, weeksOfYear) {
            weeksOfYearList.append(week);
        }
        object->insert(QOrganizerJsonDbStr::RuleWeeksOfYear, weeksOfYearList);
    }

    QSet<QOrganizerRecurrenceRule::Month> monthsOfYear = rule.monthsOfYear();
    if (!monthsOfYear.isEmpty()) {
        QVariantList monthsOfYearList;
        foreach (QOrganizerRecurrenceRule::Month month, monthsOfYear) {
            monthsOfYearList.append(enumToString(qt_organizerMonthEnumMap, month));
        }
        object->insert(QOrganizerJsonDbStr::RuleMonthsOfYear, monthsOfYearList);
    }
}

void QOrganizerJsonDbConverter::itemReminderDetailToJsonDbObject(const QOrganizerItemReminder& itemReminder, QVariantMap& reminderObject) const
{
    //Item reminder common part
    if (itemReminder.hasValue(itemReminder.FieldSecondsBeforeStart))
        reminderObject.insert(QOrganizerJsonDbStr::ItemReminderSecBeforeStart, itemReminder.secondsBeforeStart());
    if (itemReminder.hasValue(itemReminder.FieldRepetitionCount))
        reminderObject.insert(QOrganizerJsonDbStr::ItemReminderRepCount, itemReminder.repetitionCount());
    if (itemReminder.hasValue(itemReminder.FieldRepetitionDelay))
        reminderObject.insert(QOrganizerJsonDbStr::ItemReminderRepDelay, itemReminder.repetitionDelay());

    //Audio, visual, Email sepcified properties
    switch (itemReminder.reminderType()) {
    case QOrganizerItemReminder::NoReminder :
        break;
    case QOrganizerItemReminder::VisualReminder : {
        if (itemReminder.hasValue(QOrganizerItemVisualReminder::FieldMessage))
            reminderObject.insert(QOrganizerJsonDbStr::ItemVisualReminderMessage, itemReminder.value(QOrganizerItemVisualReminder::FieldMessage));
        if (itemReminder.hasValue(QOrganizerItemVisualReminder::FieldDataUrl))
            reminderObject.insert(QOrganizerJsonDbStr::ItemVisualReminderDataUrl, itemReminder.value(QOrganizerItemVisualReminder::FieldDataUrl));
        break;
    }
    case QOrganizerItemReminder::AudibleReminder : {
        if (itemReminder.hasValue(QOrganizerItemAudibleReminder::FieldDataUrl))
            reminderObject.insert(QOrganizerJsonDbStr::ItemAudibleReminderDataUrl, itemReminder.value(QOrganizerItemAudibleReminder::FieldDataUrl));
        break;
    }
    case QOrganizerItemReminder::EmailReminder : {
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldSubject))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderSubject, itemReminder.value(QOrganizerItemEmailReminder::FieldSubject));
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldBody))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderBody, itemReminder.value(QOrganizerItemEmailReminder::FieldBody));
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldAttachments))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderAttachments, itemReminder.variantValue(QOrganizerItemEmailReminder::FieldAttachments));//QVariantList
        if (itemReminder.hasValue(QOrganizerItemEmailReminder::FieldRecipients))
            reminderObject.insert(QOrganizerJsonDbStr::ItemEmailReminderRecipients, itemReminder.variantValue(QOrganizerItemEmailReminder::FieldRecipients));//QStringList
        break;
    }
    }
}

void QOrganizerJsonDbConverter::jsonDbObjectToItemReminderDetailCommon(const QVariantMap& object, QOrganizerItemReminder* itemReminder) const
{
    //Common details exist in all reminder types
    if (object.contains(QOrganizerJsonDbStr::ItemReminderSecBeforeStart)) {
        int secBeforeStart = object.value(QOrganizerJsonDbStr::ItemReminderSecBeforeStart).toInt();
        itemReminder->setSecondsBeforeStart(secBeforeStart);
    }

    if (object.contains(QOrganizerJsonDbStr::ItemReminderRepCount)) {
        int repetitionCount = object.value(QOrganizerJsonDbStr::ItemReminderRepCount).toInt();
        itemReminder->setValue(QOrganizerItemReminder::FieldRepetitionCount, repetitionCount);
    }
    if (object.contains(QOrganizerJsonDbStr::ItemReminderRepDelay)) {
        int repetitionDelay = object.value(QOrganizerJsonDbStr::ItemReminderRepDelay).toInt();
        itemReminder->setValue(QOrganizerItemReminder::FieldRepetitionDelay, repetitionDelay);
    }
}

int QOrganizerJsonDbConverter::stringToEnum(const QOrganizerJsonDbEnumConversionData* const conversionData, QString enumStr) const
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

void QOrganizerJsonDbConverter::customDetailsToJsonDbObject(const QList<QOrganizerItemCustomDetail> &customDetails, QVariantMap &object) const
{
    foreach (const QOrganizerItemCustomDetail &customDetail, customDetails) {
        if (customDetail.name().isEmpty() || customDetail.data().isNull())
            continue;
        if (customDetail.data().canConvert(QVariant::String)) {
            object.insert(customDetail.name(), customDetail.data().toString());
        } else if (customDetail.data().type() == QVariant::List) {
            QVariantList variantList;
            dataToList(customDetail.data(), variantList);
            object.insert(customDetail.name(), variantList);
        } else if (customDetail.data().type() == QVariant::Map) {
            QVariantMap variantMap;
            dataToMap(customDetail.data(), variantMap);
            object.insert(customDetail.name(), variantMap);
        } else {
            continue;
        }
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

void QOrganizerJsonDbConverter::jsonDbObjectToCustomDetails(const QVariantMap &object, QList<QOrganizerItemCustomDetail> &customDetails) const
{
    QMap<QString, QVariant>::const_iterator i = object.constBegin();
    while (i != object.constEnd()) {
        if (!i.key().isNull() && !i.value().isNull()) {
            QOrganizerItemCustomDetail customDetail;
            customDetail.setName(i.key());
            customDetail.setData(i.value());
            customDetails.append(customDetail);
        }
        ++i;
    }
}

