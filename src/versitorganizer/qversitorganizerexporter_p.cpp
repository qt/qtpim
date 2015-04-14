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

#include "qversitorganizerexporter.h"
#include "qversitorganizerexporter_p.h"

#include <QtCore/qdebug.h>

#include <QtOrganizer/qorganizer.h>

#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>
#include <QtVersit/private/qversitutils_p.h>

#include "qversitorganizerdefs_p.h"
#include "qversitorganizerpluginloader_p.h"

QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSITORGANIZER

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
        document->setComponentType(QStringLiteral("VEVENT"));
    } else if (item.type() == QOrganizerItemType::TypeTodo
        || item.type() == QOrganizerItemType::TypeTodoOccurrence) {
        document->setComponentType(QStringLiteral("VTODO"));
    } else if (item.type() == QOrganizerItemType::TypeJournal) {
        document->setComponentType(QStringLiteral("VJOURNAL"));
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
    } else if (detail.type() == QOrganizerItemDetail::TypeExtendedDetail) {
        encodeExtendedDetail(detail, &generatedProperties, &processedFields);
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
    QVersitProperty property = takeProperty(document, QStringLiteral("DTSTART"), removedProperties);
    property.setName(QStringLiteral("DTSTART"));
    if (isAllDay) {
        property.setValue(etr.startDateTime().date().toString(QStringLiteral("yyyyMMdd")));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
    } else {
        property.setValue(encodeDateTime(etr.startDateTime()));
    }
    *generatedProperties << property;

    property = takeProperty(document, QStringLiteral("DTEND"), removedProperties);
    property.setName(QStringLiteral("DTEND"));
    if (isAllDay) {
        // In iCalendar, the end date is exclusive while in Qt Organizer, it is inclusive.
        property.setValue(etr.endDateTime().date().addDays(1).toString(QStringLiteral("yyyyMMdd")));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
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
    QVersitProperty property = takeProperty(document, QStringLiteral("DTSTART"), removedProperties);
    property.setName(QStringLiteral("DTSTART"));
    if (isAllDay) {
        property.setValue(ttr.startDateTime().date().toString(QStringLiteral("yyyyMMdd")));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
    } else {
        property.setValue(encodeDateTime(ttr.startDateTime()));
    }
    *generatedProperties << property;

    property = takeProperty(document, QStringLiteral("DUE"), removedProperties);
    property.setName(QStringLiteral("DUE"));
    if (isAllDay) {
        property.setValue(ttr.dueDateTime().date().toString(QStringLiteral("yyyyMMdd")));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
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
    QVersitProperty property = takeProperty(document, QStringLiteral("DTSTART"), removedProperties);
    property.setName(QStringLiteral("DTSTART"));
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
    QVersitProperty property = takeProperty(document, QStringLiteral("CREATED"), removedProperties);
    property.setName(QStringLiteral("CREATED"));
    property.setValue(encodeDateTime(timestamp.created().toUTC()));
    *generatedProperties << property;

    property = takeProperty(document, QStringLiteral("LAST-MODIFIED"), removedProperties);
    property.setName(QStringLiteral("LAST-MODIFIED"));
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
    QVersitProperty property = takeProperty(document, QStringLiteral("X-QTPROJECT-VERSION"), removedProperties);
    property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
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
            encodeRecurRule(QStringLiteral("RRULE"), rrule, generatedProperties);
        }
        *processedFields << QOrganizerItemRecurrence::FieldRecurrenceRules;
    }
    if (!exrules.isEmpty()) {
        foreach (const QOrganizerRecurrenceRule& exrule, exrules) {
            encodeRecurRule(QStringLiteral("EXRULE"), exrule, generatedProperties);
        }
        *processedFields << QOrganizerItemRecurrence::FieldExceptionRules;
    }
    if (!rdates.isEmpty()) {
        encodeRecurDates(QStringLiteral("RDATE"), item, rdates, document, removedProperties, generatedProperties);
        *processedFields << QOrganizerItemRecurrence::FieldRecurrenceDates;
    }
    if (!exdates.isEmpty()) {
        encodeRecurDates(QStringLiteral("EXDATE"), item, exdates, document, removedProperties, generatedProperties);
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
    QString value = QStringLiteral("FREQ=");
    switch (rule.frequency()) {
        case QOrganizerRecurrenceRule::Daily:
            value.append(QStringLiteral("DAILY"));
            break;
        case QOrganizerRecurrenceRule::Weekly:
            value.append(QStringLiteral("WEEKLY"));
            break;
        case QOrganizerRecurrenceRule::Monthly:
            value.append(QStringLiteral("MONTHLY"));
            break;
        case QOrganizerRecurrenceRule::Yearly:
            value.append(QStringLiteral("YEARLY"));
            break;
        case QOrganizerRecurrenceRule::Invalid:
        default:
            return;
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::CountLimit) {
        value.append(QStringLiteral(";COUNT="));
        value.append(QString::number(rule.limitCount()));
    }
    if (rule.limitType() == QOrganizerRecurrenceRule::DateLimit) {
        value.append(QStringLiteral(";UNTIL="));
        value.append(rule.limitDate().toString(QStringLiteral("yyyyMMdd")));
    }
    if (rule.interval() > 1) {
        value.append(QStringLiteral(";INTERVAL="));
        value.append(QString::number(rule.interval()));
    }
    if (!rule.daysOfWeek().isEmpty()) {
        value.append(QStringLiteral(";BYDAY="));
        bool first = true;
        QList<Qt::DayOfWeek> daysOfWeek(QList<Qt::DayOfWeek>::fromSet(rule.daysOfWeek()));
        std::sort(daysOfWeek.begin(), daysOfWeek.end());
        foreach (Qt::DayOfWeek day, daysOfWeek) {
            if (!first)
                value.append(QStringLiteral(","));
            first = false;
            value.append(weekString(day));
        }
    }
    if (!rule.daysOfMonth().isEmpty()) {
        value.append(QStringLiteral(";BYMONTHDAY="));
        appendInts(&value, rule.daysOfMonth());
    }
    if (!rule.daysOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYYEARDAY="));
        appendInts(&value, rule.daysOfYear());
    }
    if (!rule.weeksOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYWEEKNO="));
        appendInts(&value, rule.weeksOfYear());
    }
    if (!rule.monthsOfYear().isEmpty()) {
        value.append(QStringLiteral(";BYMONTH="));
        bool first = true;
        QList<QOrganizerRecurrenceRule::Month> months(
                QList<QOrganizerRecurrenceRule::Month>::fromSet(rule.monthsOfYear()));
        std::sort(months.begin(), months.end());
        foreach (QOrganizerRecurrenceRule::Month month, months) {
            if (!first)
                value.append(QStringLiteral(","));
            first = false;
            value.append(QString::number(month));
        }
    }
    if (!rule.positions().isEmpty()) {
        value.append(QStringLiteral(";BYSETPOS="));
        appendInts(&value, rule.positions());
    }
    if (rule.firstDayOfWeek() != Qt::Monday && rule.firstDayOfWeek() > 0) {
        value.append(QStringLiteral(";WKST="));
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
    std::sort(intList.begin(), intList.end());
    foreach (int n, intList) {
        if (!first)
            str->append(QStringLiteral(","));
        first = false;
        str->append(QString::number(n));
    }
}

/*! Converts \a day to a two-character iCalendar string */
QString QVersitOrganizerExporterPrivate::weekString(Qt::DayOfWeek day) {
    switch (day) {
        case Qt::Monday:
            return QStringLiteral("MO");
        case Qt::Tuesday:
            return QStringLiteral("TU");
        case Qt::Wednesday:
            return QStringLiteral("WE");
        case Qt::Thursday:
            return QStringLiteral("TH");
        case Qt::Friday:
            return QStringLiteral("FR");
        case Qt::Saturday:
            return QStringLiteral("SA");
        case Qt::Sunday:
            return QStringLiteral("SU");
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
    property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
    QString value = property.value();
    bool valueIsEmpty = value.isEmpty();

    QList<QDate> dateList(QList<QDate>::fromSet(dates));
    std::sort(dateList.begin(), dateList.end());
    foreach (const QDate& dt, dateList) {
        QString str;
        if (dt.isValid()) {
            str = dt.toString(QStringLiteral("yyyyMMdd"));
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
        takeProperty(document, QStringLiteral("PRIORITY"), removedProperties);
    property.setName(QStringLiteral("PRIORITY"));
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
        takeProperty(document, QStringLiteral("RECURRENCE-ID"), removedProperties);
    property.setName(QStringLiteral("RECURRENCE-ID"));
    property.setValue(instanceOrigin.originalDate().toString(QStringLiteral("yyyyMMdd")));
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
            takeProperty(document, QStringLiteral("COMPLETED"), removedProperties);
        property.setName(QStringLiteral("COMPLETED"));
        property.setValue(todoProgress.finishedDateTime().toString(QStringLiteral("yyyyMMddTHHmmss")));
        *generatedProperties << property;
        *processedFields << QOrganizerTodoProgress::FieldFinishedDateTime;
    }

    if (todoProgress.hasValue(QOrganizerTodoProgress::FieldPercentageComplete)) {
        QVersitProperty property =
            takeProperty(document, QStringLiteral("PERCENT-COMPLETE"), removedProperties);
        property.setName(QStringLiteral("PERCENT-COMPLETE"));
        property.setValue(QString::number(todoProgress.percentageComplete()));
        *generatedProperties << property;
        *processedFields << QOrganizerTodoProgress::FieldPercentageComplete;
    }

    if (todoProgress.hasValue(QOrganizerTodoProgress::FieldStatus)) {
        QVersitProperty property =
            takeProperty(document, QStringLiteral("STATUS"), removedProperties);
        property.setName(QStringLiteral("STATUS"));
        switch (todoProgress.status()) {
            case QOrganizerTodoProgress::StatusNotStarted:
                property.setValue(QStringLiteral("NEEDS-ACTION"));
                break;
            case QOrganizerTodoProgress::StatusInProgress:
                property.setValue(QStringLiteral("IN-PROCESS"));
                break;
            case QOrganizerTodoProgress::StatusComplete:
                property.setValue(QStringLiteral("COMPLETED"));
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
    property.setName(QStringLiteral("COMMENT"));
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
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(attachUrl.toString());
        valarmDocument.addProperty(property);
        *processedFields << QOrganizerItemAudibleReminder::FieldDataUrl;
    }
    property.setValueType(QVersitProperty::VersitDocumentType);
    property.setValue(QVariant::fromValue(valarmDocument));
    property.setName(QStringLiteral("VALARM"));
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
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(attachment);
        valarmDocument.addProperty(property);
        *processedFields << QOrganizerItemEmailReminder::FieldAttachments;
    }

    if (emailReminder.hasValue(QOrganizerItemEmailReminder::FieldRecipients)) {
        property.setName(QStringLiteral("ATTENDEE"));
        foreach (const QString &recipient, emailReminder.recipients()) {
            property.setValue(recipient);
            valarmDocument.addProperty(property);
        }
        *processedFields << QOrganizerItemEmailReminder::FieldRecipients;
    }

    // DESCRIPTION and SUMMARY properties are not optional,
    // so we add them anyway even when empty
    property.setName(QStringLiteral("DESCRIPTION"));
    property.setValue(emailReminder.body());
    valarmDocument.addProperty(property);
    *processedFields << QOrganizerItemEmailReminder::FieldBody;
    property.setName(QStringLiteral("SUMMARY"));
    property.setValue(emailReminder.subject());
    valarmDocument.addProperty(property);
    *processedFields << QOrganizerItemEmailReminder::FieldSubject;

    property.setValueType(QVersitProperty::VersitDocumentType);
    property.setValue(QVariant::fromValue(valarmDocument));
    property.setName(QStringLiteral("VALARM"));
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
        property.setName(QStringLiteral("X-QTPROJECT-ATTACH"));
        property.setValue(attachUrl.toString());
        valarmDocument.addProperty(property);
        *processedFields << QOrganizerItemAudibleReminder::FieldDataUrl;
    }

    //DESCRIPTION property is not optional, so we add it anyway even when empty
    property.setName(QStringLiteral("DESCRIPTION"));
    property.setValue(message);
    valarmDocument.addProperty(property);
    *processedFields << QOrganizerItemVisualReminder::FieldMessage;
    property.setValueType(QVersitProperty::VersitDocumentType);
    property.setValue(QVariant::fromValue(valarmDocument));
    property.setName(QStringLiteral("VALARM"));
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
    valarmDocument.setComponentType(QStringLiteral("VALARM"));
    property.setName(QStringLiteral("ACTION"));
    switch (reminder.type()) {
    case QOrganizerItemDetail::TypeAudibleReminder:
        property.setValue(QStringLiteral("AUDIO"));
        break;
    case QOrganizerItemDetail::TypeVisualReminder:
        property.setValue(QStringLiteral("DISPLAY"));
        break;
    case QOrganizerItemDetail::TypeEmailReminder:
        property.setValue(QStringLiteral("EMAIL"));
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
            property.setName(QStringLiteral("TRIGGER"));
            property.setValue(QString(QStringLiteral("-PT%1S")).arg(
                                  QString::number(valueIter.value().toInt())));
            switch (item.type()) {
            case QOrganizerItemType::TypeEvent:
            case QOrganizerItemType::TypeEventOccurrence:
                property.insertParameter(QStringLiteral("RELATED"), QStringLiteral("START"));
                break;
            case QOrganizerItemType::TypeTodo:
            case QOrganizerItemType::TypeTodoOccurrence:
                property.insertParameter(QStringLiteral("RELATED"), QStringLiteral("END"));
                break;
            default:
                break;
            }
            triggerFound = true;
            break;
        }
        case QOrganizerItemReminder::FieldRepetitionCount: {
            property.setName(QStringLiteral("REPEAT"));
            property.setValue(valueIter.value().toString());
            break;
        }
        case QOrganizerItemReminder::FieldRepetitionDelay: {
            property.setName(QStringLiteral("DURATION"));
            property.setValue(QString(QStringLiteral("PT%1S")).arg(
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
        property.setName(QStringLiteral("TRIGGER"));
        property.setValue(QString(QStringLiteral("-PT%1S")).arg(
                              QString::number(0)));
        reminderProperties << property;
    }
    valarmDocument.setProperties(reminderProperties);
    return valarmDocument;
}

void QVersitOrganizerExporterPrivate::encodeExtendedDetail(
        const QOrganizerItemDetail &detail,
        QList<QVersitProperty> *generatedProperties,
        QSet<int> *processedFields)
{
    const QOrganizerItemExtendedDetail &extendedDetail = static_cast<const QOrganizerItemExtendedDetail &>(detail);
    QVersitProperty property;
    property.setName(QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL"));
    QString dataAsJson;
    if (VersitUtils::convertToJson(extendedDetail.data(), &dataAsJson)) {
        property.setValue(QStringList() << extendedDetail.name() << dataAsJson);
    } else {
        qWarning() << Q_FUNC_INFO << "Failed to export an extended detail";
        return;
    }
    property.setValueType(QVersitProperty::CompoundType);
    *generatedProperties << property;
    *processedFields << QOrganizerItemExtendedDetail::FieldName
                     << QOrganizerItemExtendedDetail::FieldData;
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
        return dateTime.toString(QStringLiteral("yyyyMMddTHHmmssZ"));
    else
        return dateTime.toString(QStringLiteral("yyyyMMddTHHmmss"));
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
        if (name == QStringLiteral("UID")) {
            if (hasRecurId)
                return true;
            hasUid = true;
        } else if (name == QStringLiteral("RECURRENCE-ID")) {
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

QT_END_NAMESPACE_VERSITORGANIZER
