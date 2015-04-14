/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qversitorganizerimporter_p.h"

#include <QtOrganizer/qorganizer.h>

#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>
#include <QtVersit/private/qversitutils_p.h>

#include "qversitorganizerdefs_p.h"
#include "qversitorganizerhandler.h"
#include "qversitorganizerpluginloader_p.h"
#include "qversittimezonehandler.h"

QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSITORGANIZER

QVersitOrganizerImporterPrivate::QVersitOrganizerImporterPrivate(const QString& profile) :
    mPropertyHandler(NULL),
    mTimeZoneHandler(NULL),
    mDurationSpecified(false)
{
    int versitPropertyCount =
        sizeof(versitOrganizerDetailMappings)/sizeof(VersitOrganizerDetailMapping);
    for (int i = 0; i < versitPropertyCount; i++) {
        mPropertyMappings.insert(
                QLatin1String(versitOrganizerDetailMappings[i].versitPropertyName),
                QPair<QOrganizerItemDetail::DetailType, int>(
                    versitOrganizerDetailMappings[i].detailType,
                    versitOrganizerDetailMappings[i].detailField));
    }

    mPluginPropertyHandlers = QVersitOrganizerPluginLoader::instance()->createOrganizerHandlers(profile);
    mTimeZoneHandler = QVersitOrganizerPluginLoader::instance()->timeZoneHandler();
}

QVersitOrganizerImporterPrivate::~QVersitOrganizerImporterPrivate()
{
    foreach (QVersitOrganizerHandler* pluginHandler, mPluginPropertyHandlers) {
        delete pluginHandler;
    }
}

bool QVersitOrganizerImporterPrivate::importDocument(
        const QVersitDocument& topLevel,
        const QVersitDocument& subDocument,
        QOrganizerItem* item,
        QVersitOrganizerImporter::Error* error)
{
    if (subDocument.componentType() == QStringLiteral("VEVENT")) {
        item->setType(QOrganizerItemType::TypeEvent);
    } else if (subDocument.componentType() == QStringLiteral("VTODO")) {
        item->setType(QOrganizerItemType::TypeTodo);
    } else if (subDocument.componentType() == QStringLiteral("VJOURNAL")) {
        item->setType(QOrganizerItemType::TypeJournal);
    } else if (subDocument.componentType() == QStringLiteral("VTIMEZONE")) {
        mTimeZones.addTimeZone(importTimeZone(subDocument));
        *error = QVersitOrganizerImporter::NoError;
        return false;
    } else {
        *error = QVersitOrganizerImporter::InvalidDocumentError;
        return false;
    }
    const QList<QVersitProperty> properties = subDocument.properties();
    if ( (subDocument.subDocuments().isEmpty()) && (properties.isEmpty())) {
        *error = QVersitOrganizerImporter::EmptyDocumentError;
        return false;
    }
    foreach (const QVersitProperty& property, properties) {
        importProperty(subDocument, property, item);
    }

    if (!subDocument.subDocuments().isEmpty()) {
        foreach (const QVersitDocument &nestedSubDoc, subDocument.subDocuments())
            foreach (const QVersitProperty &nestedProp, nestedSubDoc.properties()) {
                importProperty(nestedSubDoc, nestedProp, item);
                if (nestedSubDoc.componentType() == QStringLiteral("VALARM"))
                    break;
            }
    }
    // run plugin handlers
    foreach (QVersitOrganizerImporterPropertyHandler* handler, mPluginPropertyHandlers) {
        handler->subDocumentProcessed(topLevel, subDocument, item);
    }
    // run property handlers
    if (mPropertyHandler) {
        mPropertyHandler->subDocumentProcessed(topLevel, subDocument, item);
    }
    return true;
}

void QVersitOrganizerImporterPrivate::importProperty(
        const QVersitDocument& document,
        const QVersitProperty& property,
        QOrganizerItem* item)
{
    QList<QOrganizerItemDetail> updatedDetails;

    bool success = false;
    if (property.name() == QStringLiteral("CREATED")) {
        success = createTimestampCreated(property, item, &updatedDetails);
    } else if (property.name() == QStringLiteral("LAST-MODIFIED")) {
        success = createTimestampModified(property, item, &updatedDetails);
    } else if (property.name() == QStringLiteral("X-QTPROJECT-VERSION")) {
        success = createVersion(property, item, &updatedDetails);
    } else if (property.name() == QStringLiteral("PRIORITY")) {
        success = createPriority(property, item, &updatedDetails);
    } else if (property.name() == QStringLiteral("COMMENT")) {
        success = createComment(property, &updatedDetails);
    } else if (property.name() == QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL")) {
        success = createExtendedDetail(property, &updatedDetails);
    } else if (mPropertyMappings.contains(property.name())) {
        success = createSimpleDetail(property, item, &updatedDetails);
    } else if (document.componentType() == QStringLiteral("VEVENT")) {
        if (property.name() == QStringLiteral("DTSTART")) {
            success = createStartDateTime(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("DTEND")) {
            success = createEndDateTime(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("DURATION")) {
            success = createDuration(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("RRULE")
               || (property.name() == QStringLiteral("EXRULE"))) {
            success = createRecurrenceRule(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("RDATE")
               || (property.name() == QStringLiteral("EXDATE"))) {
            success = createRecurrenceDates(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("RECURRENCE-ID")) {
            success = createRecurrenceId(property, item, &updatedDetails);
        }
    } else if (document.componentType() == QStringLiteral("VTODO")) {
        if (property.name() == QStringLiteral("DTSTART")) {
            success = createTodoStartDateTime(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("DUE")) {
            success = createDueDateTime(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("RRULE")
               || (property.name() == QStringLiteral("EXRULE"))) {
            success = createRecurrenceRule(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("RDATE")
               || (property.name() == QStringLiteral("EXDATE"))) {
            success = createRecurrenceDates(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("STATUS")) {
            success = createStatus(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("PERCENT-COMPLETE")) {
            success = createPercentageComplete(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("COMPLETED")) {
            success = createFinishedDateTime(property, item, &updatedDetails);
        } else if (property.name() == QStringLiteral("RECURRENCE-ID")) {
            success = createRecurrenceId(property, item, &updatedDetails);
        }
    } else if (document.componentType() == QStringLiteral("VALARM")) {
            success = createItemReminder(document, item, &updatedDetails);
    } else if (document.componentType() == QStringLiteral("VJOURNAL")) {
        if (property.name() == QStringLiteral("DTSTART")) {
            success = createJournalEntryDateTime(property, item, &updatedDetails);
        }
    }

    // run plugin handlers
    foreach (QVersitOrganizerImporterPropertyHandler* handler, mPluginPropertyHandlers) {
        handler->propertyProcessed(document, property, *item, &success, &updatedDetails);
    }
    // run the handler, if set
    if (mPropertyHandler) {
        mPropertyHandler->propertyProcessed(document, property, *item, &success, &updatedDetails);
    }

    foreach (QOrganizerItemDetail detail, updatedDetails) {
        item->saveDetail(&detail);
    }
}

bool QVersitOrganizerImporterPrivate::createSimpleDetail(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails)
{
    if (property.value().isEmpty())
        return false;
    QPair<QOrganizerItemDetail::DetailType, int> mapping = mPropertyMappings[property.name()];
    QOrganizerItemDetail::DetailType definitionName = mapping.first;
    int fieldName = mapping.second;
    QOrganizerItemDetail detail(item->detail(definitionName));
    if (detail.isEmpty())
        detail = QOrganizerItemDetail(definitionName);
    detail.setValue(fieldName, property.value());
    updatedDetails->append(detail);
    return true;
}

bool QVersitOrganizerImporterPrivate::createTimestampCreated(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    QDateTime datetime = parseDateTime(property);
    if (!datetime.isValid())
        return false;
    QOrganizerItemTimestamp timestamp(item->detail(QOrganizerItemDetail::TypeTimestamp));
    timestamp.setCreated(datetime);
    updatedDetails->append(timestamp);
    return true;
}

bool QVersitOrganizerImporterPrivate::createTimestampModified(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    QDateTime datetime = parseDateTime(property);
    if (!datetime.isValid())
        return false;
    QOrganizerItemTimestamp timestamp(item->detail(QOrganizerItemDetail::TypeTimestamp));
    timestamp.setLastModified(datetime);
    updatedDetails->append(timestamp);
    return true;
}

/*!
 * Takes the first value in \a list, or an empty QString is if the list is empty.
 */
QString QVersitOrganizerImporterPrivate::takeFirst(QList<QString>& list) const
{
    return list.empty() ? QString() : list.takeFirst();
}

bool QVersitOrganizerImporterPrivate::createVersion(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {

    QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::CompoundType
            || variant.type() != QVariant::StringList) {
        return false;
    }
    QStringList values = variant.toStringList();

    bool conversionOk;
    QOrganizerItemVersion version(item->detail(QOrganizerItemDetail::TypeVersion));
    version.setVersion(takeFirst(values).toInt(&conversionOk));
    QString extendedVersionString = takeFirst(values);
    if (!extendedVersionString.isEmpty())
        version.setExtendedVersion(extendedVersionString.toLocal8Bit());

    if (conversionOk) {
        updatedDetails->append(version);
        return true;
    } else {
        return false;
    }
}

bool QVersitOrganizerImporterPrivate::createPriority(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().length() != 1)
        return false;

    bool ok;
    int p = property.value().toInt(&ok);
    if (!ok)
        return false;
    QOrganizerItemPriority priority(item->detail(QOrganizerItemDetail::TypePriority));
    priority.setPriority(QOrganizerItemPriority::Priority(p));
    updatedDetails->append(priority);
    return true;
}

bool QVersitOrganizerImporterPrivate::createComment(
        const QVersitProperty& property,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    QOrganizerItemComment comment;
    comment.setComment(property.value());
    updatedDetails->append(comment);
    return true;
}

bool QVersitOrganizerImporterPrivate::createItemReminder(
        const QVersitDocument& valarmDocument,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails)
{
    int repetitionCount = 0;
    int repetitionDelay = 0;
    int secondsBeforeStart = 0;
    bool alreadySetSecondsBeforeStart = false;
    const QList<QVersitProperty> valarmProperties = valarmDocument.properties();
    QString actionValue;
    QVariantList attachValues;
    QString descriptionValue;
    QString summaryValue;
    QStringList attendees;

    foreach (const QVersitProperty &valarmProperty, valarmProperties) {
        if (valarmProperty.name() == QStringLiteral("TRIGGER")) {
            secondsBeforeStart = triggerToSecondsBeforeStart(valarmProperty, *item);
            alreadySetSecondsBeforeStart = true;
        } else if (valarmProperty.name() == QStringLiteral("REPEAT")) {
            repetitionCount = valarmProperty.value().toInt();
        } else if (valarmProperty.name() == QStringLiteral("DURATION")) {
            repetitionDelay = Duration::parseDuration(valarmProperty.value()).toSeconds();
        } else if (valarmProperty.name() == QStringLiteral("ACTION")) {
            actionValue = valarmProperty.value().toUpper();
        } else if (valarmProperty.name() == QStringLiteral("ATTACH")) {
            attachValues.append(valarmProperty.variantValue());
        } else if (valarmProperty.name() == QStringLiteral("DESCRIPTION")) {
            descriptionValue = valarmProperty.value();
        } else if (valarmProperty.name() == QStringLiteral("SUMMARY")) {
            summaryValue = valarmProperty.value();
        } else if (valarmProperty.name() == QStringLiteral("ATTENDEE")) {
            attendees << valarmProperty.value();
        }
    }
    if ((actionValue.isEmpty()) || (!alreadySetSecondsBeforeStart) ) {
        //ACTION and TRIGGER are mandatory in a VALARM component.
        return false;
    } else if (actionValue == QStringLiteral("AUDIO")) {
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setRepetition(repetitionCount, repetitionDelay);
        audibleReminder.setSecondsBeforeStart(secondsBeforeStart);
        if (!attachValues.isEmpty())
            audibleReminder.setDataUrl(QUrl(attachValues.first().toString()));
        updatedDetails->append(audibleReminder);
        return true;
    } else if (actionValue == QStringLiteral("DISPLAY")) {
        if (descriptionValue.isNull())
            return false;//Invalid since REQUIRED properties are not found
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setRepetition(repetitionCount, repetitionDelay);
        visualReminder.setSecondsBeforeStart(secondsBeforeStart);
        if (!descriptionValue.isEmpty()) {
            visualReminder.setMessage(descriptionValue);
            updatedDetails->append(visualReminder);
            return true;
        } else {
            return false;
        }
    } else if (actionValue == QStringLiteral("EMAIL")) {
        if (descriptionValue.isNull() || summaryValue.isNull() || attendees.isEmpty())
            return false;//Invalid since REQUIRED properties are not found
        QOrganizerItemEmailReminder emailReminder;
        emailReminder.setRepetition(repetitionCount, repetitionDelay);
        emailReminder.setSecondsBeforeStart(secondsBeforeStart);
        emailReminder.setValue(QOrganizerItemEmailReminder::FieldBody, descriptionValue);
        emailReminder.setValue(QOrganizerItemEmailReminder::FieldSubject, summaryValue);
        if (!attachValues.isEmpty())
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldAttachments, attachValues);
        emailReminder.setContents(summaryValue, descriptionValue, attachValues);
        emailReminder.setRecipients(attendees);
        updatedDetails->append(emailReminder);
        return true;
    } else { //ACTION property had an invalid value
        return false;
    }
}

int QVersitOrganizerImporterPrivate::triggerToSecondsBeforeStart(const QVersitProperty &triggerProperty, const QOrganizerItem &item)
{
    int result = 0;

    //The default value type for TRIGGER property is DURATION.
    bool encodedAsDuration = true;

    if (!triggerProperty.parameters().isEmpty()) {
        const QString triggerValue = triggerProperty.parameters().value(QStringLiteral("VALUE")).toUpper();
        if (triggerValue == QStringLiteral("DATE-TIME"))
            encodedAsDuration = false;
        else if ( (!triggerValue.isEmpty()) &&
                  (triggerValue != QStringLiteral("DURATION")) ) {
            return 0;//Invalid trigger property...just return default value.
        }
    }

    if (encodedAsDuration) {
        const QString related = triggerProperty.parameters().value(QStringLiteral("RELATED")).toUpper();
        result = Duration::parseDuration(triggerProperty.value()).toSeconds();
        switch (item.type()) {
        case QOrganizerItemType::TypeTodo:
        case QOrganizerItemType::TypeTodoOccurrence: {
            if (related == QStringLiteral("START")) {
                QOrganizerTodoTime todoTime = item.detail(QOrganizerItemDetail::TypeTodoTime);
                QDateTime relativeTrigger = todoTime.startDateTime().addSecs(result);
                result = relativeTrigger.secsTo(todoTime.dueDateTime());
            } else if ( (related.isEmpty()) || (related == QStringLiteral("END")) ) {
                result = (-1 * result);
            }
            break;
        }
        case QOrganizerItemType::TypeEvent:
        case QOrganizerItemType::TypeEventOccurrence: {
            if (related == QStringLiteral("END")) {
                QOrganizerEventTime eventTime = item.detail(QOrganizerItemDetail::TypeEventTime);
                QDateTime relativeTrigger = eventTime.endDateTime().addSecs(result);
                result = relativeTrigger.secsTo(eventTime.startDateTime());
            } else if ( (related.isEmpty()) || (related == QStringLiteral("START")) ) {
                result = (-1 * result);
            }
            break;
        }
        default:
            break;
        }
    } else {
        QDateTime absoluteTrigger = parseDateTime(triggerProperty.value());
        switch (item.type()) {
        case QOrganizerItemType::TypeTodo:
        case QOrganizerItemType::TypeTodoOccurrence: {
            QOrganizerTodoTime todoTime = item.detail(QOrganizerItemDetail::TypeTodoTime);
            result = absoluteTrigger.secsTo(todoTime.dueDateTime());
            break;
        }
        case QOrganizerItemType::TypeEvent:
        case QOrganizerItemType::TypeEventOccurrence: {
            QOrganizerEventTime eventTime = item.detail(QOrganizerItemDetail::TypeEventTime);
            result = absoluteTrigger.secsTo(eventTime.startDateTime());
            break;
        }
        default:
            break;
        }
    }
    return result >= 0 ? result: 0;
}

bool QVersitOrganizerImporterPrivate::createExtendedDetail(
        const QVersitProperty &property,
        QList<QOrganizerItemDetail> *updatedDetails) {
    QOrganizerItemExtendedDetail extendedDetail;
    const QVariant variant = property.variantValue();
    if (property.valueType() != QVersitProperty::CompoundType
            || variant.type() != QVariant::StringList)
        return false;

    QStringList values = variant.toStringList();
    extendedDetail.setName(takeFirst(values));
    QVariant data;
    if (VersitUtils::convertFromJson(takeFirst(values), &data))
        extendedDetail.setData(data);
    else
        return false;

    updatedDetails->append(extendedDetail);
    return true;
}

bool QVersitOrganizerImporterPrivate::createRecurrenceId(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    QDate date = parseDate(property.value());
    if (!date.isValid())
        return false;
    QOrganizerItemParent origin(item->detail(QOrganizerItemDetail::TypeParent));
    origin.setOriginalDate(date);
    updatedDetails->append(origin);
    item->setType(QOrganizerItemType::TypeEventOccurrence);
    return true;
}

/*! Set the startDateTime field of the EventTimeRange detail.  If the end date has been set from a
 * DURATION, it will be updated.
 */
bool QVersitOrganizerImporterPrivate::createStartDateTime(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    bool hasTime;
    QDateTime newStart = parseDateTime(property, &hasTime);
    if (!newStart.isValid())
        return false;
    QOrganizerEventTime etr(item->detail(QOrganizerItemDetail::TypeEventTime));
    if (mDurationSpecified) {
        // Need to fix up the end date to match the duration of the event
        QDateTime start = etr.startDateTime();
        QDateTime end = etr.endDateTime();
        if (!start.isValid()) {
            // not having a start date set is treated as a start date of epoch
            start = QDateTime(QDate(1970, 1, 1));
        }
        // newEnd = end + (newStart - start)
        int durationDays = start.daysTo(newStart);
        QDateTime newEnd = end.addDays(durationDays);
        int durationSecs = start.addDays(durationDays).secsTo(newStart);
        newEnd = newEnd.addSecs(durationSecs);
        etr.setEndDateTime(newEnd);
    }
    etr.setStartDateTime(newStart);
    if (!etr.isAllDay() && !hasTime)
        etr.setAllDay(true);
    updatedDetails->append(etr);
    return true;
}

/*! Set the endDateTime field of the EventTimeRange detail.
 */
bool QVersitOrganizerImporterPrivate::createEndDateTime(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    bool hasTime;
    QDateTime newEnd = parseDateTime(property, &hasTime);
    if (!newEnd.isValid())
        return false;
    QOrganizerEventTime etr(item->detail(QOrganizerItemDetail::TypeEventTime));
    if (!etr.isAllDay() && !hasTime)
        etr.setAllDay(true);

    // In iCalendar, the end date is exclusive while in Qt Organizer, it is inclusive.
    if (etr.isAllDay())
        etr.setEndDateTime(newEnd.addDays(-1));
    else
        etr.setEndDateTime(newEnd);

    updatedDetails->append(etr);
    mDurationSpecified = false;
    return true;
}


/*! Sets the endDateTime field of the EventTimeRange detail using a DURATION property.
 */
bool QVersitOrganizerImporterPrivate::createDuration(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    Duration duration = Duration::parseDuration(property.value());
    if (!duration.isValid())
        return false;
    QOrganizerEventTime etr(item->detail(QOrganizerItemDetail::TypeEventTime));
    QDateTime startTime = etr.startDateTime();
    if (!startTime.isValid()) {
        // not having a start date set is treated as a start date of epoch
        startTime = QDateTime(QDate(1970, 1, 1));
    }
    etr.setEndDateTime(
            startTime.addDays(7*duration.weeks() + duration.days())
                     .addSecs(3600*duration.hours() + 60*duration.minutes() + duration.seconds()));
    updatedDetails->append(etr);
    mDurationSpecified = true;
    return true;
}

/*! Set the StartDateTime field of the TodoTimeRange detail.
 */
bool QVersitOrganizerImporterPrivate::createTodoStartDateTime(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    bool hasTime;
    QDateTime newStart = parseDateTime(property, &hasTime);
    if (!newStart.isValid())
        return false;
    QOrganizerTodoTime ttr(item->detail(QOrganizerItemDetail::TypeTodoTime));
    ttr.setStartDateTime(newStart);
    if (!ttr.isAllDay() && !hasTime)
        ttr.setAllDay(true);
    updatedDetails->append(ttr);
    return true;
}

/*! Set the DueDateTime field of the TodoTimeRange detail.
 */
bool QVersitOrganizerImporterPrivate::createDueDateTime(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    bool hasTime;
    QDateTime newEnd = parseDateTime(property, &hasTime);
    if (!newEnd.isValid())
        return false;
    QOrganizerTodoTime ttr(item->detail(QOrganizerItemDetail::TypeTodoTime));
    ttr.setDueDateTime(newEnd);
    if (!ttr.isAllDay() && !hasTime)
        ttr.setAllDay(true);
    updatedDetails->append(ttr);
    mDurationSpecified = false;
    return true;
}

/*! Set the EntryDateTime field of the JournalTimeRange detail.
 */
bool QVersitOrganizerImporterPrivate::createJournalEntryDateTime(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    QDateTime dateTime = parseDateTime(property);
    if (!dateTime.isValid())
        return false;
    QOrganizerJournalTime jtr(item->detail(QOrganizerItemDetail::TypeJournalTime));
    jtr.setEntryDateTime(dateTime);
    updatedDetails->append(jtr);
    return true;
}

/*! Parses a datetime stored in the \a property as an ISO 8601 datetime in basic format, either in
 * UTC time zone, floating time zone, or (if a TZID parameter exists in \a property), as a foreign
 * time zone (returned as a UTC datetime).  Returns an invalid QDateTime if the string cannot be
 * parsed.
 *
 * \a hasTime is set to true if the parsed date-time has a time, or false if it is a date only.
 * (the time portion is set to some valid but arbitrary value).
 */
QDateTime QVersitOrganizerImporterPrivate::parseDateTime(const QVersitProperty& property,
                                                         bool* hasTime) const
{
    const QMultiHash<QString, QString> parameters = property.parameters();
    if (parameters.find(QStringLiteral("VALUE"), QStringLiteral("DATE")) == parameters.constEnd()) {
        // try parsing a datetime
        if (hasTime)
            *hasTime = true;
        QDateTime datetime(parseDateTime(property.value()));
        if (datetime.isValid() && datetime.timeSpec() == Qt::LocalTime) {
            QMultiHash<QString, QString> params = property.parameters();
            QString tzid = params.value(QStringLiteral("TZID"));
            if (!tzid.isEmpty()) {
                if (tzid.at(0) == QLatin1Char('/') && mTimeZoneHandler)
                    datetime = mTimeZoneHandler->convertTimeZoneToUtc(datetime, tzid);
                else
                    datetime = mTimeZones.convert(datetime, tzid);
            }
        }
        return datetime;
    } else {
        if (hasTime)
            *hasTime = false;
        QDateTime retn;
        retn.setDate(QDate::fromString(property.value(), QStringLiteral("yyyyMMdd")));
        retn.setTime(QTime(0, 0, 0));
        return retn;
    }
}

/*! Parses \a str as an ISO 8601 datetime in basic format, either in UTC timezone or floating
 * timezone.  Returns an invalid QDateTime if the string cannot be parsed.
 */
QDateTime QVersitOrganizerImporterPrivate::parseDateTime(QString str) const
{
    bool utc = str.endsWith(QLatin1Char('Z'), Qt::CaseInsensitive);
    if (utc)
        str.chop(1); // take away z from end;
    QDateTime dt(QDateTime::fromString(str, QStringLiteral("yyyyMMddTHHmmss")));
    if (utc)
        dt.setTimeSpec(Qt::UTC);
    return dt;
}

/*!
 * Imports a RRULE, EXRULE, RDATE or EXDATE property
 */
bool QVersitOrganizerImporterPrivate::createRecurrenceRule(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    QOrganizerRecurrenceRule rule;
    if (!parseRecurRule(property.value(), &rule))
        return false;
    QOrganizerItemRecurrence detail(item->detail(QOrganizerItemDetail::TypeRecurrence));
    if (property.name() == QStringLiteral("RRULE")) {
        detail.setRecurrenceRules(detail.recurrenceRules() << rule);
    } else if (property.name() == QStringLiteral("EXRULE")) {
        detail.setExceptionRules(detail.exceptionRules() << rule);
    }
    updatedDetails->append(detail);
    return true;
}

/*!
 * Parses an iCalendar recurrence rule string \a str and puts the result in \a rule.
 * Return true on success, false on failure.
 */
bool QVersitOrganizerImporterPrivate::parseRecurRule(const QString& str, QOrganizerRecurrenceRule* rule) const
{
    QStringList parts = str.split(QLatin1Char(';'));
    if (parts.size() == 0)
        return false;

    QString freqPart = parts.takeFirst();
    QStringList freqParts = freqPart.split(QLatin1Char('='));
    if (freqParts.size() != 2)
        return false;
    if (freqParts.at(0) != QStringLiteral("FREQ"))
        return false;
    QString freqValue = freqParts.at(1);
    if (freqValue == QStringLiteral("DAILY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Daily);
    } else if (freqValue == QStringLiteral("WEEKLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Weekly);
    } else if (freqValue == QStringLiteral("MONTHLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Monthly);
    } else if (freqValue == QStringLiteral("YEARLY")) {
        rule->setFrequency(QOrganizerRecurrenceRule::Yearly);
    } else {
        return false;
    }

    foreach (const QString& part, parts) {
        QStringList keyValue = part.split(QLatin1Char('='));
        if (keyValue.size() != 2)
            return false;
        parseRecurFragment(keyValue.at(0), keyValue.at(1), rule);
    }
    return true;
}

/*!
 * Parses a fragment of an iCalendar string (the part between the semicolons) and updates \a rule.
 * \a key is the part of the fragment before the equals sign and \a value is the part after.
 */
void QVersitOrganizerImporterPrivate::parseRecurFragment(const QString& key, const QString& value,
                                                         QOrganizerRecurrenceRule* rule) const
{
    if (key == QStringLiteral("INTERVAL")) {
        bool ok;
        int n = value.toInt(&ok);
        if (ok && n >= 1)
            rule->setInterval(n);
    } else if (key == QStringLiteral("COUNT")) {
        bool ok;
        int count = value.toInt(&ok);
        if (ok && count >= 0) {
            rule->setLimit(count);
        }
    } else if (key == QStringLiteral("UNTIL")) {
        QDate date;
        if (value.contains(QLatin1Char('T'))) {
            QDateTime dt = parseDateTime(value);
            date = dt.date();
        } else {
            date = QDate::fromString(value, QStringLiteral("yyyyMMdd"));
        }
        if (date.isValid())
            rule->setLimit(date);
    } else if (key == QStringLiteral("BYDAY")) {
        QSet<Qt::DayOfWeek> days;
        QStringList dayParts = value.split(QLatin1Char(','));
        foreach (QString dayStr, dayParts) {
            if (dayStr.length() < 2) {
                // bad day specifier
                continue;
            } else if (dayStr.length() > 2) {
                // parse something like -2SU, meaning the second-last Sunday
                QString posStr = dayStr;
                dayStr = dayStr.right(2); // dayStr = last two chars
                posStr.chop(2); // posStr = all except last two chars
                bool ok;
                int pos = posStr.toInt(&ok);
                if (!ok)
                    continue;
                rule->setPositions(QSet<int>() << pos);
            }
            int day = parseDayOfWeek(dayStr);
            if (day != -1) {
                days << (Qt::DayOfWeek)day;
            }
        }
        if (!days.isEmpty()) {
            rule->setDaysOfWeek(days);
        }
    } else if (key == QStringLiteral("BYMONTHDAY")) {
        QSet<int> days = parseInts(value, -31, 31);
        if (!days.isEmpty()) {
            rule->setDaysOfMonth(days);
        }
    } else if (key == QStringLiteral("BYWEEKNO")) {
        QSet<int> weeks = parseInts(value, -53, 53);
        if (!weeks.isEmpty()) {
            rule->setWeeksOfYear(weeks);
        }
    } else if (key == QStringLiteral("BYMONTH")) {
        QSet<QOrganizerRecurrenceRule::Month> months;
        QStringList monthParts = value.split(QLatin1Char(','));
        foreach (const QString& monthPart, monthParts) {
            bool ok;
            int month = monthPart.toInt(&ok);
            if (ok && month >= 1 && month <= 12) {
                months << (QOrganizerRecurrenceRule::Month)month;
            }
        }
        if (!months.isEmpty()) {
            rule->setMonthsOfYear(months);
        }
    } else if (key == QStringLiteral("BYYEARDAY")) {
        QSet<int> days = parseInts(value, -366, 366);
        if (!days.isEmpty()) {
            rule->setDaysOfYear(days);
        }
    } else if (key == QStringLiteral("BYSETPOS")) {
        QSet<int> poss = parseInts(value, -366, 366);
        if (!poss.isEmpty()) {
            rule->setPositions(poss);
        }
    } else if (key == QStringLiteral("WKST")) {
        int day = parseDayOfWeek(value);
        if (day != -1) {
            rule->setFirstDayOfWeek((Qt::DayOfWeek)day);
        }
    }
}

/*!
 * Parses and returns a comma-separated list of integers.  Only non-zero values between \a min and
 * \a max (inclusive) are added
 */
QSet<int> QVersitOrganizerImporterPrivate::parseInts(const QString& str, int min, int max) const
{
    QSet<int> values;
    QStringList parts = str.split(QLatin1Char(','));
    foreach (const QString& part, parts) {
        bool ok;
        int value = part.toInt(&ok);
        if (ok && value >= min && value <= max && value != 0) {
            values << value;
        }
    }
    return values;
}

/*!
 * Parses an iCalendar two-character string representing a day of week and returns an int
 * corresponding to Qt::DayOfWeek.  Returns -1 on parse failure.
 */
int QVersitOrganizerImporterPrivate::parseDayOfWeek(const QString& str) const
{
    if (str == QStringLiteral("MO")) {
        return Qt::Monday;
    } else if (str == QStringLiteral("TU")) {
        return Qt::Tuesday;
    } else if (str == QStringLiteral("WE")) {
        return Qt::Wednesday;
    } else if (str == QStringLiteral("TH")) {
        return Qt::Thursday;
    } else if (str == QStringLiteral("FR")) {
        return Qt::Friday;
    } else if (str == QStringLiteral("SA")) {
        return Qt::Saturday;
    } else if (str == QStringLiteral("SU")) {
        return Qt::Sunday;
    } else {
        return -1;
    }
}

/*!
 * Parses an iCalendar RDATE or EXDATE property and updates the recurrenceDates or
 * exceptionDates in the Recurrence detail.
 */
bool QVersitOrganizerImporterPrivate::createRecurrenceDates(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails)
{
    if (property.value().isEmpty())
        return false;
    QSet<QDate> dates;
    if (!parseDates(property.value(), &dates))
        return false;
    QOrganizerItemRecurrence detail(item->detail(QOrganizerItemDetail::TypeRecurrence));
    if (property.name() == QStringLiteral("RDATE")) {
        detail.setRecurrenceDates(detail.recurrenceDates() + dates);
    } else if (property.name() == QStringLiteral("EXDATE")) {
        detail.setExceptionDates(detail.exceptionDates() + dates);
    }
    updatedDetails->append(detail);
    return true;
}

/*!
 * Parses a string like "19970304,19970504,19970704" into a list of QDates
 */
bool QVersitOrganizerImporterPrivate::parseDates(const QString& str, QSet<QDate>* dates) const
{
    QStringList parts = str.split(QLatin1Char(','));
    if (parts.size() == 0)
        return false;

    foreach (QString part, parts) {
        QDate date = parseDate(part);
        if (date.isValid())
            *dates << date;
        else
            return false;
    }
    return true;
}

/*!
 * Parses a date in either yyyyMMdd or yyyyMMddTHHmmss format (in the latter case, ignoring the
 * time)
 */
QDate QVersitOrganizerImporterPrivate::parseDate(QString str) const
{
    int tIndex = str.indexOf(QLatin1Char('T'));
    if (tIndex >= 0) {
        str = str.left(tIndex);
    }
    return QDate::fromString(str, QStringLiteral("yyyyMMdd"));
}

bool QVersitOrganizerImporterPrivate::createStatus(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    QOrganizerTodoProgress::Status status;
    if (property.value() == QStringLiteral("COMPLETED"))
        status = QOrganizerTodoProgress::StatusComplete;
    else if (property.value() == QStringLiteral("NEEDS-ACTION"))
        status = QOrganizerTodoProgress::StatusNotStarted;
    else if (property.value() == QStringLiteral("IN-PROCESS"))
        status = QOrganizerTodoProgress::StatusInProgress;
    else
        return false;

    QOrganizerTodoProgress progress(item->detail(QOrganizerItemDetail::TypeTodoProgress));
    progress.setStatus(status);
    updatedDetails->append(progress);
    return true;
}

bool QVersitOrganizerImporterPrivate::createPercentageComplete(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    bool ok = false;
    int percent = property.value().toInt(&ok);
    if (!ok)
        return false;

    QOrganizerTodoProgress progress(item->detail(QOrganizerItemDetail::TypeTodoProgress));
    progress.setPercentageComplete(percent);
    updatedDetails->append(progress);
    return true;
}

bool QVersitOrganizerImporterPrivate::createFinishedDateTime(
        const QVersitProperty& property,
        QOrganizerItem* item,
        QList<QOrganizerItemDetail>* updatedDetails) {
    if (property.value().isEmpty())
        return false;
    QDateTime datetime = parseDateTime(property);
    if (!datetime.isValid())
        return false;
    QOrganizerTodoProgress progress(item->detail(QOrganizerItemDetail::TypeTodoProgress));
    progress.setFinishedDateTime(datetime);
    updatedDetails->append(progress);
    return true;
}

/*! Parse the iCalendar duration string \a str in an RDP fashion with a two symbol lookahead, and
 * returns a Duration that represents it. */
Duration Duration::parseDuration(QString str)
{
    QString token = nextToken(&str);
    if (token.isEmpty())
        return invalidDuration();

    Duration dur;
    // Accept a + or - if present
    if (token == QStringLiteral("+")) {
        token = nextToken(&str);
    } else if (token == QStringLiteral("-")) {
        dur.setNegative(true);
        token = nextToken(&str);
    } else if (token.isEmpty()) {
        return invalidDuration();
    } else {
        // There was no + or - so keep parsing
    }

    // Accept a P
    if (token != QStringLiteral("P")) {
        return invalidDuration();
    }

    token = nextToken(&str);
    if (token.isEmpty()) {
        return invalidDuration();
    } else if (token == QStringLiteral("T")) {
        // we see a time
        parseDurationTime(&str, &dur);
    } else if (token.at(0).isDigit()) {
        // it's either a date or a week - we're not sure yet
        int value = token.toInt(); // always succeeds because nextToken next returns a mix of digits/nondigits
        token = nextToken(&str);
        if (token == QStringLiteral("D")) {
            // it's a date
            dur.setDays(value);
            token = nextToken(&str);
            // dates optionally define a time
            if (token == QStringLiteral("T"))
                parseDurationTime(&str, &dur);
        } else if (token == QStringLiteral("W")) {
            dur.setWeeks(value);
        } else {
            return invalidDuration();
        }
    } else {
        return invalidDuration();
    }

    // check that there aren't extra characters on the end
    if (!str.isEmpty())
        dur.setValid(false);
    return dur;

}

/*! Parse a duration string starting from after the "T" character.  Removes parsed part from \a str
 * and updates \a dur with the findings.
 */
void Duration::parseDurationTime(QString* str, Duration* dur)
{
    QString token = nextToken(str);
    if (token.isEmpty() || !token.at(0).isDigit())
        dur->setValid(false);

    int value = token.toInt(); // always succeeds

    token = nextToken(str);
    if (token == QStringLiteral("H")) {
        dur->setHours(value);
        if (!str->isEmpty())
            parseDurationMinutes(str, dur);
    } else if (token == QStringLiteral("M")) {
        dur->setMinutes(value);
        if (!str->isEmpty())
            parseDurationSeconds(str, dur);
    } else if (token == QStringLiteral("S")) {
        dur->setSeconds(value);
    }
}

/*! Parse a duration string starting from the part describing the number of minutes.  Removes parsed
 * part from \a str and updates \a dur with the findings.
 */
void Duration::parseDurationMinutes(QString* str, Duration* dur)
{
    QString token = nextToken(str);
    if (token.isEmpty() || !token.at(0).isDigit())
        dur->setValid(false);

    int value = token.toInt(); // always succeeds
    token = nextToken(str);
    if (token != QStringLiteral("M")) {
        dur->setValid(false);
        return;
    }
    dur->setMinutes(value);

    if (!str->isEmpty())
        parseDurationSeconds(str, dur);
}

/*! Parse a duration string starting from the part describing the number of seconds.  Removes parsed
 * part from \a str and updates \a dur with the findings.
 */
void Duration::parseDurationSeconds(QString* str, Duration* dur)
{
    QString token = nextToken(str);
    if (token.isEmpty() || !token.at(0).isDigit())
        dur->setValid(false);

    int value = token.toInt(); // always succeeds
    token = nextToken(str);
    if (token != QStringLiteral("S")) {
        dur->setValid(false);
        return;
    }
    dur->setSeconds(value);
}

/*! Removes and returns a "token" from the start of an iCalendar DURATION string, \a str.  A token
 * is either a single +, - or upper-case letter, or a string of digits.  If \a str is empty, an
 * empty string is returned.  If \a str is not empty but starts with an invalid character, a null
 * string is returned.
 */
QString Duration::nextToken(QString* str)
{
    int len = str->length();
    if (len == 0)
        return QString::fromLatin1(""); // empty (not null) QString
    QChar first = str->at(0);
    if (first == QLatin1Char('+') || first == QLatin1Char('-') || first.isUpper()) {
        QString ret(str->left(1));
        *str = str->mid(1);
        return ret;
    } else if (first.isDigit()) {
        // find the largest n such that the leftmost n characters are digits
        int n = 1;
        for (n = 1; n < len && str->at(n).isDigit(); n++) {
        }
        QString ret = str->left(n);
        *str = str->mid(n);
        return ret;
    } else {
        return QString(); // null QString
    }
}

TimeZone QVersitOrganizerImporterPrivate::importTimeZone(const QVersitDocument& document) const
{
    TimeZone timeZone;
    foreach (const QVersitProperty& property, document.properties()) {
        if (property.name() == QStringLiteral("TZID") && !property.value().isEmpty()) {
            timeZone.setTzid(property.value());
        }
    }
    foreach (const QVersitDocument& subDocument, document.subDocuments()) {
        timeZone.addPhase(importTimeZonePhase(subDocument));
    }
    return timeZone;
}

TimeZonePhase QVersitOrganizerImporterPrivate::importTimeZonePhase(const QVersitDocument& document) const
{
    TimeZonePhase phase;
    phase.setStandard(document.componentType() == QStringLiteral("STANDARD"));

    foreach (const QVersitProperty& property, document.properties()) {
        if (property.name() == QStringLiteral("TZOFFSETTO")) {
            QString value(property.value());
            if (value.size() == 5
                    && (value.at(0) == QLatin1Char('+') || value.at(0) == QLatin1Char('-'))
                    && value.at(1).isDigit()
                    && value.at(2).isDigit()
                    && value.at(3).isDigit()
                    && value.at(4).isDigit()) {
                phase.setUtcOffset((value.at(0) == QLatin1Char('+') ? 1 : -1) * ( // deal with sign
                        value.mid(1, 2).toInt() * 3600 // hours part
                        + value.mid(3, 2).toInt() * 60 // minutes part
                        ));
            }
        } else if (property.name() == QStringLiteral("DTSTART")) {
            QDateTime dt(parseDateTime(property.value()));
            if (dt.isValid() && dt.timeSpec() == Qt::LocalTime) {
                phase.setStartDateTime(dt);
            }
        } else if (property.name() == QStringLiteral("RRULE")) {
            QOrganizerRecurrenceRule rrule;
            if (parseRecurRule(property.value(), &rrule)) {
                phase.setRecurrenceRule(rrule);
            }
        } else if (property.name() == QStringLiteral("RDATE")) {
            QSet<QDate> dates;
            if (parseDates(property.value(), &dates)) {
                phase.setRecurrenceDates(dates);
            }
        }
    }
    return phase;
}

QT_END_NAMESPACE_VERSITORGANIZER
