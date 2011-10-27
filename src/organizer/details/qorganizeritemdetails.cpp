/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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

#include <qorganizeritemdetails.h>
#include <qorganizeritemdetailfilter.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerItemDescription
    \brief The QOrganizerItemDescription class contains some arbitrary information which is relevant to the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemDescription::DefinitionName

    The constant string which identifies the definition of details which are long descriptions.
 */
const QString QOrganizerItemDescription::DefinitionName(QStringLiteral("Description"));

/*!
    \variable QOrganizerItemDescription::FieldDescription

    The constant key for which the description value is stored in details of
    the QOrganizerItemDescription type.
 */
const QString QOrganizerItemDescription::FieldDescription(QStringLiteral("Description"));

/*!
    \fn QOrganizerItemDescription::setDescription(const QString& description)

    Sets a description associated with an organizer item to \a description.
 */
void QOrganizerItemDescription::setDescription(const QString& description)
{
    setValue(FieldDescription, description);
}

/*!
    \fn QOrganizerItemDescription::description() const

    Returns a string for a description associated with an organizer item.
 */
QString QOrganizerItemDescription::description() const
{
    return value(FieldDescription).toString();
}


/*!
    \class QOrganizerItemDisplayLabel
    \brief The QOrganizerItemDisplayLabel class contains the backend-synthesized display label of the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
*/

/*!
    Returns a filter suitable for finding items with a display label containing the specified
    \a substring.
 */
QOrganizerItemFilter QOrganizerItemDisplayLabel::match(const QString &substring)
{
    QOrganizerItemDetailFilter f;
    f.setDetailDefinitionName(QOrganizerItemDisplayLabel::DefinitionName,
                              QOrganizerItemDisplayLabel::FieldLabel);
    f.setValue(substring);
    f.setMatchFlags(QOrganizerItemFilter::MatchContains);

    return f;
}

/*!
    \fn void QOrganizerItemDisplayLabel::setLabel(const QString& label)

    Sets the display label of the organizer item to \a label.
 */
void QOrganizerItemDisplayLabel::setLabel(const QString &label)
{
    setValue(FieldLabel, label);
}

/*!
    \fn QString QOrganizerItemDisplayLabel::label() const

    Returns the display label of the organizer item.
 */
QString QOrganizerItemDisplayLabel::label() const
{
    return value(FieldLabel).toString();
}

/*!
    \variable QOrganizerItemDisplayLabel::DefinitionName

    The constant string which identifies the definition of details
    which contain a display label of an organizer item.
 */
const QString QOrganizerItemDisplayLabel::DefinitionName(QStringLiteral("DisplayLabel"));

/*!
    \variable QOrganizerItemDisplayLabel::FieldLabel

    The constant key for which the display label value is stored in
    details of the QOrganizerItemDisplayLabel type.
 */
const QString QOrganizerItemDisplayLabel::FieldLabel(QStringLiteral("Label"));

/*!
    \class QOrganizerEventTime
    \brief The QOrganizerEventTime class contains the start and end dates and times of a recurring event series,
           or occurrence of an event.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerEventTime::DefinitionName

    The constant string which identifies the definition of details
    which contain a start and end timestamps of an event series or event occurrence.
 */
const QString QOrganizerEventTime::DefinitionName(QStringLiteral("EventTime"));

/*!
    \variable QOrganizerEventTime::FieldEndDateTime

    The constant key for which the end date and time value is stored in
    details of the QOrganizerEventTime type.
 */
const QString QOrganizerEventTime::FieldEndDateTime(QStringLiteral("EndDateTime"));

/*!
    \variable QOrganizerEventTime::FieldStartDateTime

    The constant key for which the start date and time value is stored in
    details of the QOrganizerEventTime type.
 */
const QString QOrganizerEventTime::FieldStartDateTime(QStringLiteral("StartDateTime"));

/*!
    \variable QOrganizerEventTime::FieldAllDay

    The constant key for the specification of whether the time is significant in the
    start datetime of the QOrganizerEventTime type.
 */
const QString QOrganizerEventTime::FieldAllDay(QStringLiteral("AllDay"));

/*!
    \fn QOrganizerEventTime::startDateTime() const

    Returns the event time's start date and time as QDateTime.
    For all-day events, the time part is meaningless.

    \sa QOrganizerEvent::setStartDateTime()
 */
QDateTime QOrganizerEventTime::startDateTime() const
{
    return value<QDateTime>(FieldStartDateTime);
}

/*!
    \fn QOrganizerEventTime::setStartDateTime(const QDateTime &startDateTime)

    Sets the event time's start date and time to \a startDateTime.
    For all-day events, the time part can be set to any valid value.

    \sa QOrganizerEvent::startDateTime()
 */
void QOrganizerEventTime::setStartDateTime(const QDateTime &startDateTime)
{
    setValue(FieldStartDateTime, startDateTime);
}

/*!
    \fn QOrganizerEventTime::setEndDateTime(const QDateTime &endDateTime)

    Sets the event time's due date and time to \a endDateTime.
    For all-day events, the time part can be set to any valid value, and the date is to be
    interpreted as the inclusive end date.

    \sa QOrganizerEvent::endDateTime()
 */
void QOrganizerEventTime::setEndDateTime(const QDateTime &endDateTime)
{
    setValue(FieldEndDateTime, endDateTime);
}

/*!
    \fn QOrganizerEventTime::endDateTime() const

    Returns the event time's due date and time as QDateTime.
    For all-day events, the time part is meaningless, and the date is to be interpreted as the
    inclusive end date.

    \sa QOrganizerEvent::setEndDateTime()
 */
QDateTime QOrganizerEventTime::endDateTime() const
{
    return value<QDateTime>(FieldEndDateTime);
}

/*!
    \fn QOrganizerEventTime::setAllDay(bool isAllDay)

    Sets the all-day status of the event to \a isAllDay.
    If the event is an all-day event, no time is considered to be
    specified for the event, even if a start or end date time set
    for the event has a time component.
 */
void QOrganizerEventTime::setAllDay(bool isAllDay)
{
    setValue(FieldAllDay, isAllDay);
}

/*!
    \fn QOrganizerEventTime::isAllDay() const

    Returns true if a specific time was specified for the event.
    Returns false if the event is an all-day event.
 */
bool QOrganizerEventTime::isAllDay() const
{
    return value<bool>(FieldAllDay);
}


/*!
    \class QOrganizerItemGuid
    \brief The QOrganizerItemGuid class contains the globally unique identifier of the organizer item, which can be used for synchronization purposes.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemGuid::DefinitionName

    The constant string which identifies the definition of details which are globally unique identifiers.
 */
const QString QOrganizerItemGuid::DefinitionName(QStringLiteral("Guid"));

/*!
    \variable QOrganizerItemGuid::FieldGuid

    The constant key for which the globally unique identifier value is
    stored in details of the QOrganizerItemGuid type.
 */
const QString QOrganizerItemGuid::FieldGuid(QStringLiteral("Guid"));

/*!
    \fn QOrganizerItemGuid::guid() const

    Returns the globally unique identifier which is stored in this detail.
 */
QString QOrganizerItemGuid::guid() const
{
    return value(FieldGuid).toString();
}

/*!
    \fn QOrganizerItemGuid::setGuid(const QString& guid)

    Sets the globally unique identifier which is stored in this detail to \a guid.
 */
void QOrganizerItemGuid::setGuid(const QString &guid)
{
    setValue(FieldGuid, guid);
}


/*!
    \class QOrganizerItemParent
    \brief The QOrganizerItemParent class contains information about the event or todo that generated this item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemParent::DefinitionName

    The constant string which identifies the definition of details which contain information which identifies
    the recurrent event of which the organizer item is a particular instance or exception of.
 */
const QString QOrganizerItemParent::DefinitionName(QStringLiteral("Parent"));

/*!
    \variable QOrganizerItemParent::FieldParentId

    The constant key for the field in which the id of the parent recurrent event
    is stored in details of the QOrganizerItemParent type.
 */
const QString QOrganizerItemParent::FieldParentId(QStringLiteral("ParentId"));

/*!
    \variable QOrganizerItemParent::FieldOriginalDate

    The constant key for the field in which the original date of this instance
    or exception according to the recurrent series of the parent item is stored.
 */
const QString QOrganizerItemParent::FieldOriginalDate(QStringLiteral("OriginalDate"));

/*!
    \fn QOrganizerItemParent::parentId() const

    Returns the id of the item instance origin's parent item.
 */
QOrganizerItemId QOrganizerItemParent::parentId() const
{
    return value(FieldParentId).value<QOrganizerItemId>();
}

/*!
    \fn QOrganizerItemParent::setParentId(const QOrganizerItemId& parentId)

    Sets the parent id of this instance origin item to \a parentId.
 */
void QOrganizerItemParent::setParentId(const QOrganizerItemId &parentId)
{
    setValue(FieldParentId, QVariant::fromValue(parentId));
}

/*!
    \fn QOrganizerItemParent::originalDate() const

    Returns the original date of this instance origin item.
 */
QDate QOrganizerItemParent::originalDate() const
{
    return value(FieldOriginalDate).toDate();
}

/*!
    \fn QOrganizerItemParent::setOriginalDate(const QDate& date)

    Sets the origin date to \a date.
 */
void QOrganizerItemParent::setOriginalDate(const QDate &date)
{
    setValue(FieldOriginalDate, date);
}


/*!
    \class QOrganizerJournalTime

    \brief The QOrganizerJournalTime class contains information about
           the date and time for which a journal entry has been created.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerJournalTime::DefinitionName

    The constant string which identifies the definition of details
    which describe the date and time associated with a journal item.
 */
const QString QOrganizerJournalTime::DefinitionName(QStringLiteral("JournalTime"));

/*!
   \variable QOrganizerJournalTime::FieldEntryDateTime

   The constant key for which the date and time of value for a journal entry is
   stored in details of the QOrganizerJournalTime type.
 */
const QString QOrganizerJournalTime::FieldEntryDateTime(QStringLiteral("EntryDateTime"));

/*!
    \fn QOrganizerJournalTime::entryDateTime() const

    Returns the journal entry date and time as QDateTime.
 */
QDateTime QOrganizerJournalTime::entryDateTime() const
{
    return value<QDateTime>(FieldEntryDateTime);
}

/*!
    \fn QOrganizerJournalTime::setEntryDateTime(const QDateTime& entryDateTime)

    Sets the journal entry date and time to \a entryDateTime.
 */
void QOrganizerJournalTime::setEntryDateTime(const QDateTime &entryDateTime)
{
    setValue(FieldEntryDateTime, entryDateTime);
}


/*!
    \class QOrganizerItemLocation

    \brief The QOrganizerItemLocation class contains information about a location which is related to the organizer item in some manner.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    Returns a filter suitable for finding items whose location matches the specified \a substring.
    This filter matches location labels only.  If you wish to match against latitude or longitude
    co-ordinates, use a QContactDetailFilter instead.
 */
QOrganizerItemFilter QOrganizerItemLocation::match(const QString &substring)
{
    QOrganizerItemDetailFilter f;
    f.setDetailDefinitionName(QOrganizerItemLocation::DefinitionName,
                              QOrganizerItemLocation::FieldLabel);
    f.setValue(substring);
    f.setMatchFlags(QOrganizerItemFilter::MatchContains);

    return f;
}

/*!
    \variable QOrganizerItemLocation::DefinitionName

    The constant string which identifies the definition of details
    which describe a location associated with an organizer item.
 */
const QString QOrganizerItemLocation::DefinitionName(QStringLiteral("Location"));

/*!
    \variable QOrganizerItemLocation::FieldLatitude

    The constant key for which the location latitude value is
    stored in details of the QOrganizerItemLocation type.
 */
const QString QOrganizerItemLocation::FieldLatitude(QStringLiteral("Latitude"));

/*!
    \variable QOrganizerItemLocation::FieldLongitude

    The constant key for which the location longitude value is
    stored in details of the QOrganizerItemLocation type.
 */
const QString QOrganizerItemLocation::FieldLongitude(QStringLiteral("Longitude"));

/*!
    \variable QOrganizerItemLocation::FieldLabel

    The constant key for which the location label value is
    stored in details of the QOrganizerItemLocation type.
 */
const QString QOrganizerItemLocation::FieldLabel(QStringLiteral("Label"));

/*!
    \fn QOrganizerItemLocation::latitude() const

    Returns the latitude value of the location's geocoordinates
 */
double QOrganizerItemLocation::latitude() const
{
    return value<double>(FieldLatitude);
}

/*!
    \fn QOrganizerItemLocation::setLatitude(double latitude)

    Sets the latitude value of the location's geocoordinates to \a latitude
 */
void QOrganizerItemLocation::setLatitude(double latitude)
{
    setValue(FieldLatitude, latitude);
}

/*!
    \fn QOrganizerItemLocation::longitude() const

    Returns the longitude value of the location's geocoordinates
 */
double QOrganizerItemLocation::longitude() const
{
    return value<double>(FieldLongitude);
}

/*!
    \fn QOrganizerItemLocation::setLongitude(double longitude)

    Sets the longitude value of the location's geocoordinates to \a longitude
 */
void QOrganizerItemLocation::setLongitude(double longitude)
{
    setValue(FieldLongitude, longitude);
}

/*!
    \fn QOrganizerItemLocation::label() const

    Returns the human-readable label of the location
 */
QString QOrganizerItemLocation::label() const
{
    return value(FieldLabel).toString();
}

/*!
    \fn QOrganizerItemLocation::setLabel(const QString& label)

    Sets the human-readable label of the location to \a label
 */
void QOrganizerItemLocation::setLabel(const QString &label)
{
    setValue(FieldLabel, label);
}


/*!
    \class QOrganizerItemComment
    \brief The QOrganizerItemComment class contains some arbitrary information which is relevant to the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    Returns a filter suitable for finding items with a comment containing the specified
    \a substring.
 */
QOrganizerItemFilter QOrganizerItemComment::match(const QString &substring)
{
    QOrganizerItemDetailFilter f;
    f.setDetailDefinitionName(QOrganizerItemComment::DefinitionName,
                              QOrganizerItemComment::FieldComment);
    f.setValue(substring);
    f.setMatchFlags(QOrganizerItemFilter::MatchContains);

    return f;
}

/*!
    \variable QOrganizerItemComment::DefinitionName

    The constant string which identifies the definition of details which are comments.
 */
const QString QOrganizerItemComment::DefinitionName(QStringLiteral("Comment"));

/*!
    \variable QOrganizerItemComment::FieldComment

    The constant key for which the comment value is stored in details of
    the QOrganizerItemComment type.
 */
const QString QOrganizerItemComment::FieldComment(QStringLiteral("Comment"));

/*!
    \fn QOrganizerItemComment::setComment(const QString &comment)

    Sets a comment associated with an organizer item to \a comment.
 */
void QOrganizerItemComment::setComment(const QString &comment)
{
    setValue(FieldComment, comment);
}

/*!
    \fn QOrganizerItemComment::comment() const

    Returns a string for a comment associated with an organizer item.
 */
QString QOrganizerItemComment::comment() const
{
    return value(FieldComment).toString();
}


/*!
    \class QOrganizerItemPriority
    \brief The QOrganizerItemPriority class contains the priority of the organizer item, which may be used to resolve scheduling conflicts.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    Returns a filter suitable for finding items of the specified \a priority.
 */
QOrganizerItemFilter QOrganizerItemPriority::match(QOrganizerItemPriority::Priority priority)
{
    QOrganizerItemDetailFilter f;
    f.setDetailDefinitionName(QOrganizerItemPriority::DefinitionName,
                              QOrganizerItemPriority::FieldPriority);
    f.setValue(priority);
    f.setMatchFlags(QOrganizerItemFilter::MatchExactly);

    return f;
}

/*!
    \enum QOrganizerItemPriority::Priority
    \value UnknownPriority
    \value HighestPriority
    \value ExtremelyHighPriority
    \value VeryHighPriority
    \value HighPriority
    \value MediumPriority
    \value LowPriority
    \value VeryLowPriority
    \value ExtremelyLowPriority
    \value LowestPriority
 */

/*!
    \variable QOrganizerItemPriority::DefinitionName

    The constant string which identifies the definition of details which contain the priority of an organizer item.
 */
const QString QOrganizerItemPriority::DefinitionName(QStringLiteral("Priority"));

/*!
    \variable QOrganizerItemPriority::FieldPriority

    The constant key for which the priority value is stored in details of
    the QOrganizerItemPriority type.
 */
const QString QOrganizerItemPriority::FieldPriority(QStringLiteral("Priority"));

/*!
    \fn QOrganizerItemPriority::setPriority(Priority priority)

    Sets the priority associated with an organizer item to \a priority.
 */
void QOrganizerItemPriority::setPriority(Priority priority)
{
    setValue(FieldPriority, static_cast<int>(priority));
}

/*!
    \fn Priority QOrganizerItemPriority::priority() const

    Returns the priority associated with an organizer item.
 */
QOrganizerItemPriority::Priority QOrganizerItemPriority::priority() const
{
    return static_cast<Priority>(value<int>(FieldPriority));
}


/*!
    \class QOrganizerItemRecurrence
    \brief The QOrganizerItemRecurrence class contains a list of rules and dates on which the recurrent item occurs,
           and a list of rules and dates on which exceptions occur.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemRecurrence::DefinitionName

    The constant string which identifies the definition of details which are organizer item recurrence specifications.
 */
const QString QOrganizerItemRecurrence::DefinitionName(QStringLiteral("Recurrence"));

/*!
    Returns true if the \a other recurrence detail is equal to this detail; otherwise, false.

    Since the data types stored in this detail are custom data types, the base class
    operator==() doesn't know how to perform the comparison without calling this function.
    However, it means that if (in the future) a backend were to extend the detail with
    more fields, this operator== would no longer work; it'd have to be updated to compare
    the other fields also.
 */
bool QOrganizerItemRecurrence::operator==(const QOrganizerItemRecurrence &other) const
{
    return accessConstraints() == other.accessConstraints()
           && recurrenceRules() == other.recurrenceRules()
           && exceptionRules() == other.exceptionRules()
           && recurrenceDates() == other.recurrenceDates()
           && exceptionDates() == other.exceptionDates();
}

/*!
    \fn QOrganizerItemRecurrence::operator!=(const QOrganizerItemRecurrence &other) const

    Returns true if the \a other recurrence detail is equal to this detail; otherwise, false.
    Implemented in terms of operator==() for QOrganizerItemRecurrence detail.
 */

/*!
    \variable QOrganizerItemRecurrence::FieldRecurrenceRules

    The constant key for the value which is stored in details of the
    QOrganizerItemRecurrence type which describes the rules for when an
    item should recur.

    When multiple rules are specified, the list of recurrence dates are
    calculated separately for each rule and the results are unioned.
 */
const QString QOrganizerItemRecurrence::FieldRecurrenceRules(QStringLiteral("RecurrenceRules"));

/*!
    \variable QOrganizerItemRecurrence::FieldRecurrenceDates

    The constant key for the value which is stored in details of the
    QOrganizerItemRecurrence type which describes the dates on which an
    item should recur.
 */
const QString QOrganizerItemRecurrence::FieldRecurrenceDates(QStringLiteral("RecurrenceDates"));

/*!
    \variable QOrganizerItemRecurrence::FieldExceptionRules

    The constant key for the value which is stored in details of the
    QOrganizerItemRecurrence type which describes the rules for when an
    item should not recur.

    If a recurrence rule or the recurrence dates list specifies that an item should occur on a
    particular date and any of the exception rules include that date, the item should not occur on
    that date.
 */
const QString QOrganizerItemRecurrence::FieldExceptionRules(QStringLiteral("ExceptionRules"));

/*!
    \variable QOrganizerItemRecurrence::FieldExceptionDates

    The constant key for the value which is stored in details of the
    QOrganizerItemRecurrence type which describes the dates on which an
    item should not recur.

    If a recurrence rule or the recurrence dates list specifies that an item should occur on a
    particular date and that date appears in the exception dates list, the item should not occur on
    that date.
 */
const QString QOrganizerItemRecurrence::FieldExceptionDates(QStringLiteral("ExceptionDates"));

/*!
    Returns the set of recurrence dates.
 */
QSet<QDate> QOrganizerItemRecurrence::recurrenceDates() const
{
    return value(FieldRecurrenceDates).value<QSet<QDate> >();
}

/*!
    Sets the set of recurrence dates to \a rdates.
 */
void QOrganizerItemRecurrence::setRecurrenceDates(const QSet<QDate> &rdates)
{
    setValue(FieldRecurrenceDates, QVariant::fromValue(rdates));
}

/*!
    Returns the set of exception rules.
 */
QSet<QOrganizerRecurrenceRule> QOrganizerItemRecurrence::exceptionRules() const
{
    return value(FieldExceptionRules).value<QSet<QOrganizerRecurrenceRule> >();
}

/*!
    Sets the set of exception rules to \a xrules.
 */
void QOrganizerItemRecurrence::setExceptionRules(const QSet<QOrganizerRecurrenceRule> &xrules)
{
    setValue(FieldExceptionRules, QVariant::fromValue(xrules));
}

/*!
    Returns the set of recurrence rules.
 */
QSet<QOrganizerRecurrenceRule> QOrganizerItemRecurrence::recurrenceRules() const
{
    return value(FieldRecurrenceRules).value<QSet<QOrganizerRecurrenceRule> >();
}

/*!
    Sets the set of recurrence rules to \a rrules.
 */
void QOrganizerItemRecurrence::setRecurrenceRules(const QSet<QOrganizerRecurrenceRule> &rrules)
{
    setValue(FieldRecurrenceRules, QVariant::fromValue(rrules));
}

/*!
    Returns the set of exception dates.
 */
QSet<QDate> QOrganizerItemRecurrence::exceptionDates() const
{
    return value(FieldExceptionDates).value<QSet<QDate> >();
}

/*!
    Sets the set of exception dates to \a xdates.
 */
void QOrganizerItemRecurrence::setExceptionDates(const QSet<QDate> &xdates)
{
    setValue(FieldExceptionDates, QVariant::fromValue(xdates));
}


/*!
    \class QOrganizerItemReminder
    \brief The QOrganizerItemReminder class contains information about when and how the user wants to reminded of the item
    \inmodule QtOrganizer
    \ingroup organizer-details

    Note that the Organizer API does not enforce that the user is reminded of the item;
    rather, it simply allows clients to store and manipulate data which might be used
    by the platform to implement alarms and reminders.
 */

/*!
    \enum QOrganizerItemReminder::ReminderType
    \value NoReminder This reminder is entirely unobtrusive
    \value AudibleReminder This reminder has an audible element
    \value VisualReminder This reminder has a visual element
    \value EmailReminder This reminder has a email element
 */

/*!
    \variable QOrganizerItemReminder::DefinitionName

    The constant string which identifies the definition of details which contain reminder information of an organizer item.
 */
const QString QOrganizerItemReminder::DefinitionName(QStringLiteral("Reminder"));

/*!
    \variable QOrganizerItemReminder::FieldSecondsBeforeStart

    The constant key for which time delta (in seconds prior to the item activation time)
    at which the user should be reminded of the item is stored in details of the QOrganizerItemReminder type.
 */
const QString QOrganizerItemReminder::FieldSecondsBeforeStart(QStringLiteral("SecondsBeforeStart"));

/*!
    \variable QOrganizerItemReminder::FieldRepetitionCount

    The constant key for which the number of repetitions of the reminder
    is stored in details of the QOrganizerItemReminder type.
 */
const QString QOrganizerItemReminder::FieldRepetitionCount(QStringLiteral("RepetitionCount"));

/*!
    \variable QOrganizerItemReminder::FieldRepetitionDelay

    The constant key for which the delay (in seconds) between repetitions of the reminder
    is stored in details of the QOrganizerItemReminder type.
 */
const QString QOrganizerItemReminder::FieldRepetitionDelay(QStringLiteral("RepetitionDelay"));

/*!
    Returns the reminder type of this reminder for an organizer item.
 */
QOrganizerItemReminder::ReminderType QOrganizerItemReminder::reminderType() const
{
    if (definitionName() == QOrganizerItemAudibleReminder::DefinitionName)
        return QOrganizerItemReminder::AudibleReminder;
    else if (definitionName() == QOrganizerItemEmailReminder::DefinitionName)
        return QOrganizerItemReminder::EmailReminder;
    else if (definitionName() == QOrganizerItemVisualReminder::DefinitionName)
        return QOrganizerItemReminder::VisualReminder;

    return QOrganizerItemReminder::NoReminder;
}

/*!
    \fn QOrganizerItemReminder::setSecondsBeforeStart(int seconds)

    Sets the number of seconds prior to the activation of the item
    at which the user wants to be reminded of the item to \a seconds.
    The exact datetime of activation of the item depends on the type of
    item: for a QOrganizerTodo or QOrganizerTodoOccurrence it is the
    due date time; for a QOrganizerEvent or QOrganizerEventOccurrence
    it is the start date time.

    Calling this function will clear the absolute date time, if set.
 */
void QOrganizerItemReminder::setSecondsBeforeStart(int seconds)
{
    setValue(FieldSecondsBeforeStart, seconds);
}

/*!
    \fn int QOrganizerItemReminder::secondsBeforeStart() const

    Returns the number of seconds prior to the activation of the item
    at which the user wants to be reminded of the item.
    The exact datetime of activation of the item depends on the type of
    item: for a QOrganizerTodo or QOrganizerTodoOccurrence it is the
    due date time; for a QOrganizerEvent or QOrganizerEventOccurrence
    it is the start date time.
 */
int QOrganizerItemReminder::secondsBeforeStart() const
{
    return value<int>(FieldSecondsBeforeStart);
}

/*!
    \fn int QOrganizerItemReminder::repetitionCount() const

    Returns the number of times the user should be reminded of the item.

    \sa repetitionDelay()
 */
int QOrganizerItemReminder::repetitionCount() const
{
    return value<int>(FieldRepetitionCount);
}

/*!
    \fn int QOrganizerItemReminder::repetitionDelay() const

    Returns the delay (in seconds) between each repetition of the reminder.

    \sa repetitionCount()
 */
int QOrganizerItemReminder::repetitionDelay() const
{
    return value<int>(FieldRepetitionDelay);
}

/*!
    \fn QOrganizerItemReminder::setRepetition(int count, int delaySeconds)

    Sets the number of repetitions of the reminderto \a count, and the delay (in seconds)
    between each repetition of the reminder to \a delaySeconds.

    \sa repetitionCount(), repetitionDelay()
*/
void QOrganizerItemReminder::setRepetition(int count, int delaySeconds)
{
    setValue(FieldRepetitionCount, count); setValue(FieldRepetitionDelay, delaySeconds);
}

/*!
    \macro Q_DECLARE_CUSTOM_ORGANIZER_REMINDER_DETAIL
    \relates QOrganizerItemReminder

    Macro for simplifying declaring custom (leaf) reminder detail classes.

    The first argument is the name of the class, and the second argument
    is a Latin-1 string literal naming the detail type, and the third argument
    is the reminder type of the leaf reminder detail class.

    If you are creating a convenience class for a type of QOrganizerItemReminder,
    you should use this macro when declaring your class to ensure that
    it interoperates with other organizer item functionality.
 */


/*!
    \class QOrganizerItemAudibleReminder
    \brief The QOrganizerItemAudibleReminder class contains information about an audible reminder of an item.
    \inmodule QtOrganizer
    \ingroup organizer-details

    An audible reminder is a reminder which alerts the user about the item, with sound.
    Note that the Organizer API does not enforce that the sound data is played,
    or that any other sort of reminder occurs; rather, it simply allows clients
    to store and manipulate data which might be used by the platform to
    implement alarms and reminders.
 */

/*!
    \variable QOrganizerItemAudibleReminder::DefinitionName

    The constant string which identifies the definition of details which contain audible reminder information of an organizer item.
 */
const QString QOrganizerItemAudibleReminder::DefinitionName(QStringLiteral("AudibleReminder"));

/*!
    \variable QOrganizerItemAudibleReminder::FieldDataUrl

    The constant key for which the value of the sound data url is stored.
 */
const QString QOrganizerItemAudibleReminder::FieldDataUrl(QStringLiteral("DataUrl"));

/*!
    \fn QOrganizerItemAudibleReminder::setDataUrl(const QUrl& dataUrl)

    Sets the url of the audible data which should be played to \a dataUrl.
 */
void QOrganizerItemAudibleReminder::setDataUrl(const QUrl &dataUrl)
{
    setValue(FieldDataUrl, dataUrl);
}

/*!
    \fn QUrl QOrganizerItemAudibleReminder::dataUrl() const

    Returns the url of the audible data which should be played.
 */
QUrl QOrganizerItemAudibleReminder::dataUrl() const
{
    return value<QUrl>(FieldDataUrl);
}


/*!
    \class QOrganizerItemEmailReminder
    \brief The QOrganizerItemEmailReminder class contains information about an email reminder of an item.
    \inmodule QtOrganizer
    \ingroup organizer-details

    An email reminder is a reminder which alerts the user (or other users) about the item,
    by sending an email.
    Note that the Organizer API does not enforce that the email is sent,
    or that any other sort of reminder occurs; rather, it simply allows clients
    to store and manipulate data which might be used by the platform to
    implement alarms and reminders.
 */

/*!
    \variable QOrganizerItemEmailReminder::DefinitionName

    The constant string which identifies the definition of details which contain email reminder information of an organizer item.
 */
const QString QOrganizerItemEmailReminder::DefinitionName(QStringLiteral("EmailReminder"));

/*!
    \variable QOrganizerItemEmailReminder::FieldSubject

    The constant key for which the subject field of the email which the user wishes to be sent as a reminder, is stored.
 */
const QString QOrganizerItemEmailReminder::FieldSubject(QStringLiteral("Subject"));

/*!
    \variable QOrganizerItemEmailReminder::FieldBody

    The constant key for which the body field of the email which the user wishes to be sent as a reminder, is stored.
 */
const QString QOrganizerItemEmailReminder::FieldBody(QStringLiteral("Body"));

/*!
    \variable QOrganizerItemEmailReminder::FieldAttachments

    The constant key for which the attachments of the email which the user wishes to be sent as a reminder, is stored.
 */
const QString QOrganizerItemEmailReminder::FieldAttachments(QStringLiteral("Attachments"));

/*!
    \variable QOrganizerItemEmailReminder::FieldRecipients

    The constant key for which the recipients field of the email which the user wishes to be sent as a reminder, is stored.
 */
const QString QOrganizerItemEmailReminder::FieldRecipients(QStringLiteral("Recipients"));

/*!
    \fn QOrganizerItemEmailReminder::setContents(const QString& subject, const QString& body, const QVariantList& attachments)

    Sets the contents of the email reminder to be the given \a subject, \a body and \a attachments.
 */
void QOrganizerItemEmailReminder::setContents(const QString &subject, const QString &body, const QVariantList &attachments)
{
    setValue(FieldSubject, subject); setValue(FieldBody, body); setValue(FieldAttachments, attachments);
}

/*!
    \fn QString QOrganizerItemEmailReminder::subject() const

    Returns the subject of the email.
 */
QString QOrganizerItemEmailReminder::subject() const
{
    return value(FieldSubject).toString();
}

/*!
    \fn QString QOrganizerItemEmailReminder::body() const

    Returns the body of the email.
 */
QString QOrganizerItemEmailReminder::body() const
{
    return value(FieldBody).toString();
}

/*!
    \fn QVariantList QOrganizerItemEmailReminder::attachments() const

    Returns the attachments of the email.
 */
QVariantList QOrganizerItemEmailReminder::attachments() const
{
    return value<QVariantList>(FieldAttachments);
}

/*!
    \fn QOrganizerItemEmailReminder::setRecipients(const QStringList& recipients)

    Sets the list of recipients that the user wishes to be sent an email as part of the reminder
    to \a recipients.
 */
void QOrganizerItemEmailReminder::setRecipients(const QStringList &recipients)
{
    setValue(FieldRecipients, recipients);
}

/*!
    \fn QStringList QOrganizerItemEmailReminder::recipients() const

    Returns the list of recipients that the user wishes to be sent an email as part of the reminder.
 */
QStringList QOrganizerItemEmailReminder::recipients() const
{
    return value<QStringList>(FieldRecipients);
}


/*!
    \class QOrganizerItemVisualReminder
    \brief The QOrganizerItemVisualReminder class contains information about a visual reminder of an item.
    \inmodule QtOrganizer
    \ingroup organizer-details

    A visual reminder is a reminder which alerts the user about the item, with a message, image or video.
    Note that the Organizer API does not enforce that the visual data is displayed,
    or that any other sort of reminder occurs; rather, it simply allows clients
    to store and manipulate data which might be used by the platform to
    implement alarms and reminders.
 */

/*!
    \variable QOrganizerItemVisualReminder::DefinitionName

    The constant string which identifies the definition of details which contain visual reminder information of an organizer item.
 */
const QString QOrganizerItemVisualReminder::DefinitionName(QStringLiteral("VisualReminder"));

/*!
    \variable QOrganizerItemVisualReminder::FieldMessage

    The constant key for which the value of the message to be displayed is stored in details of the QOrganizerItemVisualReminder type.
 */
const QString QOrganizerItemVisualReminder::FieldMessage(QStringLiteral("Message"));

/*!
    \variable QOrganizerItemVisualReminder::FieldDataUrl

    The constant key for which the value of the visual data url is stored.
 */
const QString QOrganizerItemVisualReminder::FieldDataUrl(QStringLiteral("DataUrl"));

/*!
    \fn QOrganizerItemVisualReminder::setMessage(const QString& message)

    Sets the message which the user wishes to be displayed as part of the reminder to \a message.
*/
void QOrganizerItemVisualReminder::setMessage(const QString &message)
{
    setValue(FieldMessage, message);
}

/*!
    \fn QString QOrganizerItemVisualReminder::message() const

    Returns the message which the user wishes to be displayed as part of the reminder.
*/
QString QOrganizerItemVisualReminder::message() const
{
    return value<QString>(FieldMessage);
}

/*!
    \fn QOrganizerItemVisualReminder::setDataUrl(const QUrl& dataUrl)

    Sets the url of the visual data which the user wishes to be displayed as part of the reminder to \a dataUrl.
*/
void QOrganizerItemVisualReminder::setDataUrl(const QUrl &dataUrl)
{
    setValue(FieldDataUrl, dataUrl);
}

/*!
    \fn QUrl QOrganizerItemVisualReminder::dataUrl() const

    Returns the url of the visual data which the user wishes to be displayed as part of the reminder.
*/
QUrl QOrganizerItemVisualReminder::dataUrl() const
{
    return value<QUrl>(FieldDataUrl);
}


/*!
    \class QOrganizerItemTag
    \brief The QOrganizerItemTag class contains some arbitrary tag which is relevant to the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    Returns a filter suitable for finding items with a tag containing the specified
    \a substring.
 */
QOrganizerItemFilter QOrganizerItemTag::match(const QString &substring)
{
    QOrganizerItemDetailFilter f;
    f.setDetailDefinitionName(QOrganizerItemTag::DefinitionName,
                              QOrganizerItemTag::FieldTag);
    f.setValue(substring);
    f.setMatchFlags(QOrganizerItemFilter::MatchContains);

    return f;
}

/*!
    \variable QOrganizerItemTag::DefinitionName

    The constant string which identifies the definition of details which are tags.
 */
const QString QOrganizerItemTag::DefinitionName(QStringLiteral("Tag"));

/*!
    \variable QOrganizerItemTag::FieldTag

    The constant key for which the tag value is stored in details of
    the QOrganizerItemTag type.
 */
const QString QOrganizerItemTag::FieldTag(QStringLiteral("Tag"));

/*!
    \variable QOrganizerItemTag::Anniversary

    The constant string for anniversary and birthday tag.
*/
const QString QOrganizerItemTag::Anniversary(QStringLiteral("Anniversary"));

/*!
    \fn QOrganizerItemTag::setTag(const QString &tag)

    Sets a tag associated with an organizer item to \a tag.
 */
void QOrganizerItemTag::setTag(const QString &tag)
{
    setValue(FieldTag, tag);
}

/*!
    \fn QOrganizerItemTag::tag() const

    Returns the tag associated with an organizer item which is stored in this detail.
 */
QString QOrganizerItemTag::tag() const
{
    return value(FieldTag).toString();
}


/*!
    \class QOrganizerItemTimestamp
    \brief The QOrganizerItemTimestamp class contains the creation and last-modified timestamp associated with the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemTimestamp::DefinitionName

    The constant string which identifies the definition of details which are organizer synchronisation timestamps.
 */
const QString QOrganizerItemTimestamp::DefinitionName(QStringLiteral("Timestamp"));

/*!
    \variable QOrganizerItemTimestamp::FieldModificationTimestamp

    The constant key for the value which is stored in details of the
    QOrganizerItemTimestamp type which describes the last modification date
    and time of an organizer item.
 */
const QString QOrganizerItemTimestamp::FieldModificationTimestamp(QStringLiteral("ModificationTimestamp"));

/*!
    \variable QOrganizerItemTimestamp::FieldCreationTimestamp

    The constant key for the value which is stored in details of the
    QOrganizerItemTimestamp type which describes the creation date and time
    of an organizer item.
 */
const QString QOrganizerItemTimestamp::FieldCreationTimestamp(QStringLiteral("CreationTimestamp"));

/*!
    \fn QOrganizerItemTimestamp::created() const

    Returns the creation timestamp saved in this detail.
 */
QDateTime QOrganizerItemTimestamp::created() const
{
    return value<QDateTime>(FieldCreationTimestamp);
}

/*!
    \fn QOrganizerItemTimestamp::lastModified() const

    Returns the last-modified timestamp saved in this detail.
 */
QDateTime QOrganizerItemTimestamp::lastModified() const
{
    return value<QDateTime>(FieldModificationTimestamp);
}

/*!
    \fn QOrganizerItemTimestamp::setCreated(const QDateTime& dateTime)

    Sets the creation timestamp saved in this detail to \a dateTime.
 */
void QOrganizerItemTimestamp::setCreated(const QDateTime &timestamp)
{
    setValue(FieldCreationTimestamp, timestamp);
}

/*!
    \fn QOrganizerItemTimestamp::setLastModified(const QDateTime& dateTime)

    Sets the last-modified timestamp saved in this detail to \a dateTime.
 */
void QOrganizerItemTimestamp::setLastModified(const QDateTime &timestamp)
{
    setValue(FieldModificationTimestamp, timestamp);
}


/*!
    \class QOrganizerTodoProgress
    \brief The QOrganizerTodoProgress class contains information about the progress of a todo item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerTodoProgress::DefinitionName

    The constant string which identifies the definition of details which contain progress information about a todo item.
 */
const QString QOrganizerTodoProgress::DefinitionName(QStringLiteral("TodoProgress"));

/*!
    \variable QOrganizerTodoProgress::FieldStatus

    The constant key of the value which describes the current completion status of the
    todo item.
 */
const QString QOrganizerTodoProgress::FieldStatus(QStringLiteral("Status"));

/*!
    \variable QOrganizerTodoProgress::FieldPercentageComplete

    The constant key of the value which contains the current completion percentage of the
    todo item.
 */
const QString QOrganizerTodoProgress::FieldPercentageComplete(QStringLiteral("PercentageComplete"));

/*!
    \variable QOrganizerTodoProgress::FieldFinishedDateTime

    The constant key of the date time value which contains the date and time at which the
    todo item was completed.
 */
const QString QOrganizerTodoProgress::FieldFinishedDateTime(QStringLiteral("FinishedDateTime"));

/*!
    \enum QOrganizerTodoProgress::Status
    Enumerates the various possible types of todo item status
    \value StatusNotStarted The todo item hasn't been started yet
    \value StatusInProgress The todo item is current in progress
    \value StatusComplete The todo item has finished
 */

/*!
    \fn QOrganizerTodoProgress::status() const

    Returns the todo progress item's current status as QOrganizerTodoProgress::Status.
 */
QOrganizerTodoProgress::Status QOrganizerTodoProgress::status() const
{
    return (Status) value<int>(FieldStatus);
}

/*!
    \fn QOrganizerTodoProgress::setStatus(Status status)

    Sets the todo progress item's current status to \a status.
 */
void QOrganizerTodoProgress::setStatus(Status status)
{
    setValue(FieldStatus, (int) status);
}

/*!
    \fn QOrganizerTodoProgress::finishedDateTime() const

    Returns the todo progress item's finished date and timeas QDateTime.
 */
QDateTime QOrganizerTodoProgress::finishedDateTime() const
{
    return value<QDateTime>(FieldFinishedDateTime);
}

/*!
    \fn QOrganizerTodoProgress::setFinishedDateTime(const QDateTime& finishedDateTime)

    Sets the todo progress item's finished date and time to \a finishedDateTime.
 */
void QOrganizerTodoProgress::setFinishedDateTime(const QDateTime &finishedDateTime)
{
    setValue(FieldFinishedDateTime, finishedDateTime);
}

/*!
    \fn QOrganizerTodoProgress::percentageComplete() const

    Returns the todo progress item's completion percentage.
 */
int QOrganizerTodoProgress::percentageComplete() const
{
    return value<int>(FieldPercentageComplete);
}

/*!
    \fn QOrganizerTodoProgress::setPercentageComplete(int percentage)

    Sets the todo progress item's completion percentage to \a percentage.
 */
void QOrganizerTodoProgress::setPercentageComplete(int percentage)
{
    if (percentage >=0 && percentage <= 100)
        setValue(FieldPercentageComplete, percentage);
}


/*!
    \class QOrganizerTodoTime
    \brief The QOrganizerTodoTime class contains information about the time range of a todo item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerTodoTime::DefinitionName

    The constant string which identifies the definition of details which contain time-range information about a todo item.
 */
const QString QOrganizerTodoTime::DefinitionName(QStringLiteral("TodoTime"));

/*!
    \variable QOrganizerTodoTime::FieldStartDateTime

    The constant key of the date time value which describes the date at which
    the todo should be started.
 */
const QString QOrganizerTodoTime::FieldStartDateTime(QStringLiteral("StartDateTime"));

/*!
    \variable QOrganizerTodoTime::FieldDueDateTime

    The constant key of the date time value which describes the latest date at which
    the todo should be completed.
 */
const QString QOrganizerTodoTime::FieldDueDateTime(QStringLiteral("DueDateTime"));

/*!
    \variable QOrganizerTodoTime::FieldAllDay

    The constant key for the specification of whether the time is significant in the
    start datetime of the QOrganizerTodoTime type.
 */
const QString QOrganizerTodoTime::FieldAllDay(QStringLiteral("AllDay"));

/*!
    \fn QOrganizerTodoTime::startDateTime() const

    Returns the todo time's start date and time as QDateTime.
    For all-day tasks, the time part is meaningless.
 */
QDateTime QOrganizerTodoTime::startDateTime() const
{
    return value<QDateTime>(FieldStartDateTime);
}

/*!
    \fn QOrganizerTodoTime::setStartDateTime(const QDateTime& startDateTime)

    Sets the todo time's start date and time to \a startDateTime.
    For all-day tasks, the time part can be set to any valid value.
 */
void QOrganizerTodoTime::setStartDateTime(const QDateTime &startDateTime)
{
    setValue(FieldStartDateTime, startDateTime);
}

/*!
    \fn QOrganizerTodoTime::dueDateTime() const

    Returns the todo time's due date and time as QDateTime.
    For all-day tasks, the time part is meaningless.
 */
QDateTime QOrganizerTodoTime::dueDateTime() const
{
    return value<QDateTime>(FieldDueDateTime);
}

/*!
    \fn QOrganizerTodoTime::setDueDateTime(const QDateTime& dueDateTime)

    Sets the todo time's due date and time to \a dueDateTime.
    For all-day tasks, the time part can be set to any valid value.
 */
void QOrganizerTodoTime::setDueDateTime(const QDateTime &dueDateTime)
{
    setValue(FieldDueDateTime, dueDateTime);
}

/*!
    \fn QOrganizerTodoTime::setAllDay(bool isAllDay)

    Sets the all-day status of the todo to \a isAllDay.
    If the tasks is an all-day todo, no time is considered to be
    specified for the todo, even if the start date time set
    for the todo has a time component.
 */
void QOrganizerTodoTime::setAllDay(bool isAllDay)
{
    setValue(FieldAllDay, isAllDay);
}

/*!
    \fn QOrganizerTodoTime::isAllDay() const

    Returns true if a specific time was specified for the todo.
    Returns false if the todo is an all-day todo.
 */
bool QOrganizerTodoTime::isAllDay() const
{
    return value<bool>(FieldAllDay);
}


/*!
    \class QOrganizerItemType
    \brief The QOrganizerItemType class describes the type of the organizer item.  This detail may be automatically synthesized by the backend depending on other details in the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemType::DefinitionName

    The constant string which identifies the definition of details which identify the type of the organizer item.
 */
const QString QOrganizerItemType::DefinitionName(QStringLiteral("Type"));

/*!
    \variable QOrganizerItemType::FieldType

    The constant key for the type value which is stored in details of
    the QOrganizerItemType definition.
 */
const QString QOrganizerItemType::FieldType(QStringLiteral("Type"));

/*!
    \variable QOrganizerItemType::TypeEvent

    The constant attribute value which describes the organizer item as being an event.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeEvent, "Event")

/*!
    \variable QOrganizerItemType::TypeEventOccurrence

    The constant attribute value which describes the organizer item as being an occurrence of an event.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeEventOccurrence, "EventOccurrence")

/*!
    \variable QOrganizerItemType::TypeJournal

    The constant attribute value which describes the organizer item as being a journal.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeJournal, "Journal")

/*!
    \variable QOrganizerItemType::TypeNote

    The constant attribute value which describes the organizer item as being a note.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeNote, "Note")

/*!
    \variable QOrganizerItemType::TypeTodo

    The constant attribute value which describes the organizer item as being a todo.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeTodo, "Todo")

/*!
    \variable QOrganizerItemType::TypeTodoOccurrence

    The constant attribute value which describes the organizer item as being an occurrence of a todo.
 */
Q_DEFINE_LATIN1_CONSTANT(QOrganizerItemType::TypeTodoOccurrence, "TodoOccurrence")

/*!
    \fn QOrganizerItemType::type() const

    Returns the organizer item type value stored in this detail.
 */
QString QOrganizerItemType::type() const
{
    return value(FieldType).toString();
}

/*!
    \fn QOrganizerItemType::setType(const QString &type)

    Sets the type of the organizer item to be the give \a type.
 */
void QOrganizerItemType::setType(const QString &type)
{
    setValue(FieldType, type);
}


/*!
    \class QOrganizerItemExtendedDetail
    \brief The QOrganizerItemExtendedDetail class provides the possibility to save extended details to the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details

    Different back-end engines may or may not support extended details for different item types. Even
    if supported, they may accept different QVariant types as the data.
 */

/*!
    \variable QOrganizerItemExtendedDetail::DefinitionName

    The constant string which identifies the definition of details which are extended details.
 */
const QString QOrganizerItemExtendedDetail::DefinitionName(QStringLiteral("ExtendedDetail"));

/*!
    \variable QOrganizerItemExtendedDetail::FieldDetailName

    Contains the name of the extended detail.
 */
const QString QOrganizerItemExtendedDetail::FieldExtendedDetailName(QStringLiteral("ExtendedDetailName"));

/*!
    \variable QOrganizerItemExtendedDetail::FieldExtendedDetailData

    Contains the data this extended detail.
 */
const QString QOrganizerItemExtendedDetail::FieldExtendedDetailData(QStringLiteral("ExtendedDetailData"));

/*!
    \fn void QOrganizerItemExtendedDetail::setName(const QString &name)

    Sets the \a name of this extended detail.
 */
void QOrganizerItemExtendedDetail::setName(const QString &name)
{
    setValue(FieldExtendedDetailName, name);
}

/*!
    \fn QString QOrganizerItemExtendedDetail::name() const

    Gets the name of this extended detail.
 */
QString QOrganizerItemExtendedDetail::name() const
{
    return value(FieldExtendedDetailName).toString();
}

/*!
    \fn void setDetailData(const QVariant &data)

    Sets the \a data of the extended detail.
 */
void QOrganizerItemExtendedDetail::setData(const QVariant &data)
{
    setValue(FieldExtendedDetailData, data);
}

/*!
    \fn QVariant data() const

    Gets the data of this extended detail.
 */
QVariant QOrganizerItemExtendedDetail::data() const
{
    return value(FieldExtendedDetailData);
}

QTORGANIZER_END_NAMESPACE
