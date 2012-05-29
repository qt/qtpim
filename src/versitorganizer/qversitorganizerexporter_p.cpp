/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
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

#include <qversitorganizerexporter.h>
#include "qversitorganizerexporter_p.h"
#include <qorganizer.h>
#include "qversitorganizerdefs_p.h"
#include <qversitdocument.h>
#include <qversitproperty.h>
#include "qversitorganizerpluginloader_p.h"

QTVERSITORGANIZER_BEGIN_NAMESPACE

QVersitOrganizerExporterPrivate::QVersitOrganizerExporterPrivate(const QString& profile) :
    mDetailHandler(NULL),
    mTimeZoneHandler(NULL)
{
    int versitPropertyCount =
        sizeof(versitOrganizerDetailMappings)/sizeof(VersitOrganizerDetailMapping);
    for (int i = 0; i < versitPropertyCount; i++) {
        mPropertyMappings.insert(
                versitOrganizerDetailMappings[i].detailType,
                QPair<int, QString>(
                    versitOrganizerDetailMappings[i].detailField,
                    QLatin1String(versitOrganizerDetailMappings[i].versitPropertyName)));
    }

    mPluginDetailHandlers = QVersitOrganizerPluginLoader::instance()->createOrganizerHandlers(profile);
    mTimeZoneHandler = QVersitOrganizerPluginLoader::instance()->timeZoneHandler();
}

QVersitOrganizerExporterPrivate::~QVersitOrganizerExporterPrivate()
{
    foreach (QVersitOrganizerHandler* pluginHandler, mPluginDetailHandlers) {
        delete pluginHandler;
    }
}

bool QVersitOrganizerExporterPrivate::exportItem(
    const QOrganizerItem& item,
    QVersitDocument* document,
    QVersitOrganizerExporter::Error* error)
{
    if (item.isEmpty()) {
        *error = QVersitOrganizerExporter::EmptyOrganizerError;
        return false;
    }
    if (item.type() == QOrganizerItemType::TypeEvent
        || item.type() == QOrganizerItemType::TypeEventOccurrence) {
        document->setComponentType(QLatin1String("VEVENT"));
    } else if (item.type() == QOrganizerItemType::TypeTodo
        || item.type() == QOrganizerItemType::TypeTodoOccurrence) {
        document->setComponentType(QLatin1String("VTODO"));
    } else if (item.type() == QOrganizerItemType::TypeJournal) {
        document->setComponentType(QLatin1String("VJOURNAL"));
    } else {
        *error = QVersitOrganizerExporter::UnknownComponentTypeError;
        return false;
    }
    QList<QOrganizerItemDetail> allDetails = item.details();
    if (allDetails.isEmpty()) {
        *error = QVersitOrganizerExporter::EmptyOrganizerError;
        return false;
    }
    foreach (const QOrganizerItemDetail& detail, allDetails) {
        exportDetail(item, detail, document);
    }

    // run plugin handlers
    foreach (QVersitOrganizerExporterDetailHandler* handler, mPluginDetailHandlers) {
        handler->itemProcessed(item, document);
    }
    // run the handler, if set
    if (mDetailHandler) {
        mDetailHandler->itemProcessed(item, document);
    }
    if (item.type() == QOrganizerItemType::TypeEventOccurrence
            && !documentContainsUidAndRecurrenceId(*document)) {
        *error = QVersitOrganizerExporter::UnderspecifiedOccurrenceError;
        return false;
    }
    return true;
}

void QVersitOrganizerExporterPrivate::exportDetail(
        const QOrganizerItem& item,
        const QOrganizerItemDetail& detail,
        QVersitDocument* document)
{
    QList<QVersitProperty> removedProperties;
    QList<QVersitProperty> generatedProperties;
    QSet<int> processedFields;

    if (detail.type() == QOrganizerItemDetail::TypeEventTime) {
        encodeEventTimeRange(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeTodoTime) {
        encodeTodoTimeRange(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeJournalTime) {
        encodeJournalTimeRange(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeTimestamp) {
        encodeTimestamp(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeRecurrence) {
        encodeRecurrence(item, detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypePriority) {
        encodePriority(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeParent) {
        encodeInstanceOrigin(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeTodoProgress) {
        encodeTodoProgress(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeComment) {
        encodeComment(detail, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeVersion) {
        encodeVersion(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeAudibleReminder) {
        encodeAudibleReminder(item, detail, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeVisualReminder) {
        encodeVisualReminder(item, detail, &generatedProperties, &processedFields);
    } else if (detail.type() == QOrganizerItemDetail::TypeEmailReminder) {
        encodeEmailReminder(item, detail, &generatedProperties, &processedFields);
    } else if (mPropertyMappings.contains(detail.type())) {
        encodeSimpleProperty(detail, *document, &removedProperties, &generatedProperties, &processedFields);
    }

    // run the plugin handler
    foreach (QVersitOrganizerExporterDetailHandler* handler, mPluginDetailHandlers) {
        handler->detailProcessed(item, detail, *document,
                                 &processedFields, &removedProperties, &generatedProperties);
    }
    // run the detail handler, if set
    if (mDetailHandler) {
        mDetailHandler->detailProcessed(item, detail, *document,
                                        &processedFields, &removedProperties, &generatedProperties);
    }

    foreach(const QVersitProperty& property, removedProperties) {
        document->removeProperty(property);
    }
    foreach(const QVersitProperty& property, generatedProperties) {
        if (property.valueType() == QVersitProperty::VersitDocumentType) {
            QVersitDocument subDocument(QVersitDocument::ICalendar20Type);
            subDocument = property.value<QVersitDocument>();
            document->addSubDocument(subDocument);
        } else {
            document->addProperty(property);
        }
    }
}

void QVersitOrganizerExporterPrivate::encodeEventTimeRange(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerEventTime &etr = static_cast<const QOrganizerEventTime &>(detail);
    bool isAllDay = etr.isAllDay();
    QVersitProperty property = takeProperty(document, QLatin1String("DTSTART"), removedProperties);
    property.setName(QLatin1String("DTSTART"));
    if (isAllDay) {
        property.setValue(etr.startDateTime().date().toString(QLatin1String("yyyyMMdd")));
        property.insertParameter(QLatin1String("VALUE"), QLatin1String("DATE"));
    } else {
        property.setValue(encodeDateTime(etr.startDateTime()));
    }
    *generatedProperties << property;

    property = takeProperty(document, QLatin1String("DTEND"), removedProperties);
    property.setName(QLatin1String("DTEND"));
    if (isAllDay) {
        // In iCalendar, the end date is exclusive while in Qt Organizer, it is inclusive.
        property.setValue(etr.endDateTime().date().addDays(1).toString(QLatin1String("yyyyMMdd")));
        property.insertParameter(QLatin1String("VALUE"), QLatin1String("DATE"));
    } else {
        property.setValue(encodeDateTime(etr.endDateTime()));
    }
    *generatedProperties << property;
    *processedFields << QOrganizerEventTime::FieldStartDateTime
                     << QOrganizerEventTime::FieldEndDateTime;
}

void QVersitOrganizerExporterPrivate::encodeTodoTimeRange(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerTodoTime &ttr = static_cast<const QOrganizerTodoTime &>(detail);
    bool isAllDay = ttr.isAllDay();
    QVersitProperty property = takeProperty(document, QLatin1String("DTSTART"), removedProperties);
    property.setName(QLatin1String("DTSTART"));
    if (isAllDay) {
        property.setValue(ttr.startDateTime().date().toString(QLatin1String("yyyyMMdd")));
        property.insertParameter(QLatin1String("VALUE"), QLatin1String("DATE"));
    } else {
        property.setValue(encodeDateTime(ttr.startDateTime()));
    }
    *generatedProperties << property;

    property = takeProperty(document, QLatin1String("DUE"), removedProperties);
    property.setName(QLatin1String("DUE"));
    if (isAllDay) {
        property.setValue(ttr.dueDateTime().date().toString(QLatin1String("yyyyMMdd")));
        property.insertParameter(QLatin1String("VALUE"), QLatin1String("DATE"));
    } else {
        property.setValue(encodeDateTime(ttr.dueDateTime()));
    }
    *generatedProperties << property;
    *processedFields << QOrganizerTodoTime::FieldStartDateTime
                     << QOrganizerTodoTime::FieldDueDateTime;
}

void QVersitOrganizerExporterPrivate::encodeJournalTimeRange(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerJournalTime &jtr = static_cast<const QOrganizerJournalTime &>(detail);
    QVersitProperty property = takeProperty(document, QLatin1String("DTSTART"), removedProperties);
    property.setName(QLatin1String("DTSTART"));
    property.setValue(encodeDateTime(jtr.entryDateTime()));
    *generatedProperties << property;
    *processedFields << QOrganizerJournalTime::FieldEntryDateTime;
}

void QVersitOrganizerExporterPrivate::encodeTimestamp(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerItemTimestamp &timestamp = static_cast<const QOrganizerItemTimestamp &>(detail);
    QVersitProperty property = takeProperty(document, QLatin1String("CREATED"), removedProperties);
    property.setName(QLatin1String("CREATED"));
    property.setValue(encodeDateTime(timestamp.created().toUTC()));
    *generatedProperties << property;

    property = takeProperty(document, QLatin1String("LAST-MODIFIED"), removedProperties);
    property.setName(QLatin1String("LAST-MODIFIED"));
    property.setValue(encodeDateTime(timestamp.lastModified().toUTC()));
    *generatedProperties << property;
    *processedFields << QOrganizerItemTimestamp::FieldCreated
                     << QOrganizerItemTimestamp::FieldLastModified;
}

void QVersitOrganizerExporterPrivate::encodeVersion(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerItemVersion &version = static_cast<const QOrganizerItemVersion &>(detail);
    QVersitProperty property = takeProperty(document, QLatin1String("X-QTPROJECT-VERSION"), removedProperties);
    property.setName(QLatin1String("X-QTPROJECT-VERSION"));
    QStringList values(QString::number(version.version()));
    values.append(QString::fromLocal8Bit(version.extendedVersion()));
    property.setValue(values);
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QOrganizerItemVersion::FieldVersion
                      << QOrganizerItemVersion::FieldExtendedVersion;
}

void QVersitOrganizerExporterPrivate::encodeRecurrence(
        const QOrganizerItem& item,
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerItemRecurrence &recurrence = static_cast<const QOrganizerItemRecurrence &>(detail);
    QSet<QOrganizerRecurrenceRule> rrules = recurrence.recurrenceRules();
    QSet<QOrganizerRecurrenceRule> exrules = recurrence.exceptionRules();
    QSet<QDate> rdates = recurrence.recurrenceDates();
    QSet<QDate> exdates = recurrence.exceptionDates();
    if (!rrules.isEmpty()) {
        foreach (const QOrganizerRecurrenceRule& rrule, rrules) {
            encodeRecurRule(QLatin1String("RRULE"), rrule, generatedProperties);
        }
        *processedFields << QOrganizerItemRecurrence::FieldRecurrenceRules;
    }
    if (!exrules.isEmpty()) {
        foreach (const QOrganizerRecurrenceRule& exrule, exrules) {
            encodeRecurRule(QLatin1String("EXRULE"), exrule, generatedProperties);
        }
        *processedFields << QOrganizerItemRecurrence::FieldExceptionRules;
    }
    if (!rdates.isEmpty()) {
        encodeRecurDates(QLatin1String("RDATE"), item, rdates, document, removedProperties, generatedProperties);
        *processedFields << QOrganizerItemRecurrence::FieldRecurrenceDates;
    }
    if (!exdates.isEmpty()) {
        encodeRecurDates(QLatin1String("EXDATE"), item, exdates, document, removedProperties, generatedProperties);
        *processedFields << QOrganizerItemRecurrence::FieldExceptionDates;
    }
}

/*! Encodes an iCalendar recurrence specification from \a rule. */
void QVersitOrganizerExporterPrivate::encodeRecurRule(
        const QString& propertyName,
        const QOrganizerRecurrenceRule& rule,
        QList<QVersitProperty>* generatedProperties)
{
    QVersitProperty property;
    property.setName(propertyName);
    QString value = QLatin1String("FREQ=");
    switch (rule.frequency()) {
        case QOrganizerRecurrenceRule::Daily:
            value.append(QLatin1String("DAILY"));
            break;
        case QOrganizerRecurrenceRule::Weekly:
            value.append(QLatin1String("WEEKLY"));
            break;
        case QOrganizerRecurrenceRule::Monthly:
            value.append(QLatin1String("MONTHLY"));
            break;
        case QOrganizerRecurrenceRule::Yearly:
            value.append(QLatin1String("YEARLY"));
            break;
        case QOrganizerRecurrenceRule::Invalid:
        default:
            return;
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::CountLimit) {
        value.append(QLatin1String(";COUNT="));
        value.append(QString::number(rule.limitCount()));
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::DateLimit) {
        value.append(QLatin1String(";UNTIL="));
        value.append(rule.limitDate().toString(QLatin1String("yyyyMMdd")));
    }
    if (rule.interval() > 1) {
        value.append(QLatin1String(";INTERVAL="));
        value.append(QString::number(rule.interval()));
    }
    if (!rule.daysOfWeek().isEmpty()) {
        value.append(QLatin1String(";BYDAY="));
        bool first = true;
        QList<Qt::DayOfWeek> daysOfWeek(QList<Qt::DayOfWeek>::fromSet(rule.daysOfWeek()));
        qSort(daysOfWeek);
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            if (!first)
                value.append(QLatin1String(","));
            first = false;
            value.append(weekString(day));
        }
    }
    if (!rule.daysOfMonth().isEmpty()) {
        value.append(QLatin1String(";BYMONTHDAY="));
        appendInts(&value, rule.daysOfMonth());
    }
    if (!rule.daysOfYear().isEmpty()) {
        value.append(QLatin1String(";BYYEARDAY="));
        appendInts(&value, rule.daysOfYear());
    }
    if (!rule.weeksOfYear().isEmpty()) {
        value.append(QLatin1String(";BYWEEKNO="));
        appendInts(&value, rule.weeksOfYear());
    }
    if (!rule.monthsOfYear().isEmpty()) {
        value.append(QLatin1String(";BYMONTH="));
        bool first = true;
        QList<QOrganizerRecurrenceRule::Month> months(
                QList<QOrganizerRecurrenceRule::Month>::fromSet(rule.monthsOfYear()));
        qSort(months);
        foreach (QOrganizerRecurrenceRule::Month month, months) {
            if (!first)
                value.append(QLatin1String(","));
            first = false;
            value.append(QString::number(month));
        }
    }
    if (!rule.positions().isEmpty()) {
        value.append(QLatin1String(";BYSETPOS="));
        appendInts(&value, rule.positions());
    }
    if (rule.firstDayOfWeek() != Qt::Monday && rule.firstDayOfWeek() > 0) {
        value.append(QLatin1String(";WKST="));
        value.append(weekString(rule.firstDayOfWeek()));
    }
    property.setValue(value);
    property.setValueType(QVersitProperty::PreformattedType);
    *generatedProperties << property;
}

/*! Joins \a list together with commas and appends the result to \a str.
 */
void QVersitOrganizerExporterPrivate::appendInts(QString* str, const QSet<int>& ints) {
    bool first = true;
    QList<int> intList(QList<int>::fromSet(ints));
    qSort(intList);
    foreach (int n, intList) {
        if (!first)
            str->append(QLatin1String(","));
        first = false;
        str->append(QString::number(n));
    }
}

/*! Converts \a day to a two-character iCalendar string */
QString QVersitOrganizerExporterPrivate::weekString(Qt::DayOfWeek day) {
    switch (day) {
        case Qt::Monday:
            return QLatin1String("MO");
        case Qt::Tuesday:
            return QLatin1String("TU");
        case Qt::Wednesday:
            return QLatin1String("WE");
        case Qt::Thursday:
            return QLatin1String("TH");
        case Qt::Friday:
            return QLatin1String("FR");
        case Qt::Saturday:
            return QLatin1String("SA");
        case Qt::Sunday:
            return QLatin1String("SU");
        default:
            return QString();
    }
}

void QVersitOrganizerExporterPrivate::encodeRecurDates(
        const QString& propertyName,
        const QOrganizerItem& item,
        const QSet<QDate>& dates,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties)
{
    Q_UNUSED(item)

    QVersitProperty property;
    property = takeProperty(document, propertyName, removedProperties);
    property.setName(propertyName);
    property.insertParameter(QLatin1String("VALUE"), QLatin1String("DATE"));
    QString value = property.value();
    bool valueIsEmpty = value.isEmpty();

    QList<QDate> dateList(QList<QDate>::fromSet(dates));
    qSort(dateList);
    foreach (const QDate& dt, dateList) {
        QString str;
        if (dt.isValid()) {
            str = dt.toString(QLatin1String("yyyyMMdd"));
            if (!valueIsEmpty)
                value += QLatin1Char(',');
            value += str;
            valueIsEmpty = false;
        }
    }
    property.setValue(value);
    *generatedProperties << property;
}

void QVersitOrganizerExporterPrivate::encodePriority(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerItemPriority &priority = static_cast<const QOrganizerItemPriority &>(detail);
    QVersitProperty property =
        takeProperty(document, QLatin1String("PRIORITY"), removedProperties);
    property.setName(QLatin1String("PRIORITY"));
    property.setValue(QString::number(priority.priority()));
    *generatedProperties << property;
    *processedFields << QOrganizerItemPriority::FieldPriority;
}

void QVersitOrganizerExporterPrivate::encodeInstanceOrigin(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerItemParent &instanceOrigin = static_cast<const QOrganizerItemParent &>(detail);
    QVersitProperty property =
        takeProperty(document, QLatin1String("RECURRENCE-ID"), removedProperties);
    property.setName(QLatin1String("RECURRENCE-ID"));
    property.setValue(instanceOrigin.originalDate().toString(QLatin1String("yyyyMMdd")));
    *generatedProperties << property;
    *processedFields << QOrganizerItemParent::FieldOriginalDate;
}

void QVersitOrganizerExporterPrivate::encodeTodoProgress(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerTodoProgress &todoProgress = static_cast<const QOrganizerTodoProgress &>(detail);

    if (todoProgress.finishedDateTime().isValid()) {
        QVersitProperty property =
            takeProperty(document, QLatin1String("COMPLETED"), removedProperties);
        property.setName(QLatin1String("COMPLETED"));
        property.setValue(todoProgress.finishedDateTime().toString(QLatin1String("yyyyMMddTHHmmss")));
        *generatedProperties << property;
        *processedFields << QOrganizerTodoProgress::FieldFinishedDateTime;
    }

    if (todoProgress.hasValue(QOrganizerTodoProgress::FieldPercentageComplete)) {
        QVersitProperty property =
            takeProperty(document, QLatin1String("PERCENT-COMPLETE"), removedProperties);
        property.setName(QLatin1String("PERCENT-COMPLETE"));
        property.setValue(QString::number(todoProgress.percentageComplete()));
        *generatedProperties << property;
        *processedFields << QOrganizerTodoProgress::FieldPercentageComplete;
    }

    if (todoProgress.hasValue(QOrganizerTodoProgress::FieldStatus)) {
        QVersitProperty property =
            takeProperty(document, QLatin1String("STATUS"), removedProperties);
        property.setName(QLatin1String("STATUS"));
        switch (todoProgress.status()) {
            case QOrganizerTodoProgress::StatusNotStarted:
                property.setValue(QLatin1String("NEEDS-ACTION"));
                break;
            case QOrganizerTodoProgress::StatusInProgress:
                property.setValue(QLatin1String("IN-PROCESS"));
                break;
            case QOrganizerTodoProgress::StatusComplete:
                property.setValue(QLatin1String("COMPLETED"));
                break;
            default:
                return;
        }
        *generatedProperties << property;
        *processedFields << QOrganizerTodoProgress::FieldStatus;
    }
}

void QVersitOrganizerExporterPrivate::encodeComment(
        const QOrganizerItemDetail& detail,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    const QOrganizerItemComment &comment = static_cast<const QOrganizerItemComment &>(detail);
    QVersitProperty property;
    property.setName(QLatin1String("COMMENT"));
    property.setValue(comment.comment());
    *generatedProperties << property;
    *processedFields << QOrganizerItemComment::FieldComment;
}

void QVersitOrganizerExporterPrivate::encodeAudibleReminder(
        const QOrganizerItem &item,
        const QOrganizerItemDetail &detail,
        QList<QVersitProperty> *generatedProperties,
        QSet<int> *processedFields)
{
    const QOrganizerItemAudibleReminder &audibleReminder = static_cast<const QOrganizerItemAudibleReminder &>(detail);
    QVersitProperty property;
    QVersitDocument valarmDocument = encodeItemReminderCommonFields(item, audibleReminder, processedFields);

    const QUrl attachUrl = audibleReminder.dataUrl();
    if (!attachUrl.isEmpty()) {
        property.setName(QLatin1String("ATTACH"));
        property.setValue(attachUrl.toString());
        valarmDocument.addProperty(property);
        *processedFields << QOrganizerItemAudibleReminder::FieldDataUrl;
    }
    property.setValueType(QVersitProperty::VersitDocumentType);
    property.setValue(QVariant::fromValue(valarmDocument));
    property.setName(QLatin1String("VALARM"));
    *generatedProperties << property;
}

void QVersitOrganizerExporterPrivate::encodeEmailReminder(
        const QOrganizerItem &item,
        const QOrganizerItemDetail &detail,
        QList<QVersitProperty> *generatedProperties,
        QSet<int> *processedFields)
{
    const QOrganizerItemEmailReminder &emailReminder = static_cast<const QOrganizerItemEmailReminder &>(detail);
    QVersitProperty property;
    QVersitDocument valarmDocument = encodeItemReminderCommonFields(item, emailReminder, processedFields);

    foreach (const QVariant &attachment, emailReminder.attachments()) {
        property.setName(QLatin1String("ATTACH"));
        property.setValue(attachment);
        valarmDocument.addProperty(property);
        *processedFields << QOrganizerItemEmailReminder::FieldAttachments;
    }

    if (emailReminder.hasValue(QOrganizerItemEmailReminder::FieldRecipients)) {
        property.setName(QLatin1String("ATTENDEE"));
        foreach (const QString &recipient, emailReminder.recipients()) {
            property.setValue(recipient);
            valarmDocument.addProperty(property);
        }
        *processedFields << QOrganizerItemEmailReminder::FieldRecipients;
    }

    // DESCRIPTION and SUMMARY properties are not optional,
    // so we add them anyway even when empty
    property.setName(QLatin1String("DESCRIPTION"));
    property.setValue(emailReminder.body());
    valarmDocument.addProperty(property);
    *processedFields << QOrganizerItemEmailReminder::FieldBody;
    property.setName(QLatin1String("SUMMARY"));
    property.setValue(emailReminder.subject());
    valarmDocument.addProperty(property);
    *processedFields << QOrganizerItemEmailReminder::FieldSubject;

    property.setValueType(QVersitProperty::VersitDocumentType);
    property.setValue(QVariant::fromValue(valarmDocument));
    property.setName(QLatin1String("VALARM"));
    *generatedProperties << property;
}

void QVersitOrganizerExporterPrivate::encodeVisualReminder(
        const QOrganizerItem &item,
        const QOrganizerItemDetail &detail,
        QList<QVersitProperty> *generatedProperties,
        QSet<int> *processedFields)
{
    const QOrganizerItemVisualReminder &visualReminder = static_cast<const QOrganizerItemVisualReminder &>(detail);
    QVersitProperty property;
    QVersitDocument valarmDocument = encodeItemReminderCommonFields(item, visualReminder, processedFields);

    const QUrl attachUrl = visualReminder.dataUrl();
    const QString message = visualReminder.message();

    if (!attachUrl.isEmpty()) {
        //ICAL specs do not include ATTACH property for DISPLAY VALARM components
        //Hence, we add it here as an (optional) extended QTPROJECT-specific property
        property.setName(QLatin1String("X-QTPROJECT-ATTACH"));
        property.setValue(attachUrl.toString());
        valarmDocument.addProperty(property);
        *processedFields << QOrganizerItemAudibleReminder::FieldDataUrl;
    }

    //DESCRIPTION property is not optional, so we add it anyway even when empty
    property.setName(QLatin1String("DESCRIPTION"));
    property.setValue(message);
    valarmDocument.addProperty(property);
    *processedFields << QOrganizerItemVisualReminder::FieldMessage;
    property.setValueType(QVersitProperty::VersitDocumentType);
    property.setValue(QVariant::fromValue(valarmDocument));
    property.setName(QLatin1String("VALARM"));
    *generatedProperties << property;
}

QVersitDocument QVersitOrganizerExporterPrivate::encodeItemReminderCommonFields(
        const QOrganizerItem &item,
        const QOrganizerItemReminder &reminder,
        QSet<int> *processedFields)
{
    QVersitProperty property;
    QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
    QList<QVersitProperty> reminderProperties;
    valarmDocument.setComponentType(QLatin1String("VALARM"));
    property.setName(QLatin1String("ACTION"));
    switch (reminder.type()) {
    case QOrganizerItemDetail::TypeAudibleReminder:
        property.setValue(QLatin1String("AUDIO"));
        break;
    case QOrganizerItemDetail::TypeVisualReminder:
        property.setValue(QLatin1String("DISPLAY"));
        break;
    case QOrganizerItemDetail::TypeEmailReminder:
        property.setValue(QLatin1String("EMAIL"));
        break;
    default:
        return QVersitDocument();
    }
    reminderProperties << property;
    QMap<int, QVariant>::const_iterator valueIter = reminder.values().constBegin();
    bool triggerFound = false;
    while (valueIter != reminder.values().constEnd()) {
        switch (valueIter.key()) {
        case QOrganizerItemReminder::FieldSecondsBeforeStart: {
            property.setName(QLatin1String("TRIGGER"));
            property.setValue(QString(QLatin1String("-PT%1S")).arg(
                                  QString::number(valueIter.value().toInt())));
            switch (item.type()) {
            case QOrganizerItemType::TypeEvent:
            case QOrganizerItemType::TypeEventOccurrence:
                property.insertParameter(QLatin1String("RELATED"), QLatin1String("START"));
                break;
            case QOrganizerItemType::TypeTodo:
            case QOrganizerItemType::TypeTodoOccurrence:
                property.insertParameter(QLatin1String("RELATED"), QLatin1String("END"));
                break;
            default:
                break;
            }
            triggerFound = true;
            break;
        }
        case QOrganizerItemReminder::FieldRepetitionCount: {
            property.setName(QLatin1String("REPEAT"));
            property.setValue(valueIter.value().toString());
            break;
        }
        case QOrganizerItemReminder::FieldRepetitionDelay: {
            property.setName(QLatin1String("DURATION"));
            property.setValue(QString(QLatin1String("PT%1S")).arg(
                                  QString::number(valueIter.value().toInt())));
            break;
        }
        default:
            valueIter ++;
            property.clear();
            continue;
            break;
        }
        reminderProperties << property;
        *processedFields << valueIter.key();
        property.clear();
        valueIter ++;
    }
    if (!triggerFound) {
        property.setName(QLatin1String("TRIGGER"));
        property.setValue(QString(QLatin1String("-PT%1S")).arg(
                              QString::number(0)));
        reminderProperties << property;
    }
    valarmDocument.setProperties(reminderProperties);
    return valarmDocument;
}

void QVersitOrganizerExporterPrivate::encodeSimpleProperty(
        const QOrganizerItemDetail& detail,
        const QVersitDocument& document,
        QList<QVersitProperty>* removedProperties,
        QList<QVersitProperty>* generatedProperties,
        QSet<int>* processedFields)
{
    QPair<int, QString> fieldPropertyMap = mPropertyMappings[detail.type()];
    const int& fieldName = fieldPropertyMap.first;
    const QString& propertyName = fieldPropertyMap.second;
    QVersitProperty property =
        takeProperty(document, propertyName, removedProperties);
    property.setName(propertyName);
    property.setValue(detail.value(fieldName));
    *generatedProperties << property;
    *processedFields << fieldName;
}

/*! Formats \a dateTime in ISO 8601 basic format */
QString QVersitOrganizerExporterPrivate::encodeDateTime(const QDateTime& dateTime)
{
    if (dateTime.timeSpec() == Qt::UTC)
        return dateTime.toString(QLatin1String("yyyyMMddTHHmmssZ"));
    else
        return dateTime.toString(QLatin1String("yyyyMMddTHHmmss"));
}

/*!
 * Returns true if and only if \a document has both the UID and the RECURRENCE-ID properties
 */
bool QVersitOrganizerExporterPrivate::documentContainsUidAndRecurrenceId(const QVersitDocument &document)
{
    bool hasUid = false;
    bool hasRecurId = false;
    foreach (const QVersitProperty& property, document.properties()) {
        const QString& name = property.name();
        if (name == QLatin1String("UID")) {
            if (hasRecurId)
                return true;
            hasUid = true;
        } else if (name == QLatin1String("RECURRENCE-ID")) {
            if (hasUid)
                return true;
            hasRecurId = true;
        }
    }
    return false;
}

/*!
 * Finds a property in the \a document with the given \a propertyName, adds it to \a toBeRemoved,
 * and returns it.
 */
QVersitProperty QVersitOrganizerExporterPrivate::takeProperty(
        const QVersitDocument& document,
        const QString& propertyName,
        QList<QVersitProperty>* toBeRemoved) {
    foreach (const QVersitProperty& currentProperty, document.properties()) {
        if (currentProperty.name() == propertyName) {
            *toBeRemoved << currentProperty;
            return currentProperty;
        }
    }
    return QVersitProperty();
}

QTVERSITORGANIZER_END_NAMESPACE
