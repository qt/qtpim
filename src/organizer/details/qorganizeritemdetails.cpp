/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qorganizeritemdetails.h>
#include <qorganizeritemdetailfilter.h>

/*
    When these conditions are satisfied, QStringLiteral is implemented by
    gcc's statement-expression extension.  However, in this file it will
    not work, because "statement-expressions are not allowed outside functions
    nor in template-argument lists".

    Fall back to the less-performant QLatin1String in this case.
*/
#if defined(QStringLiteral) && defined(QT_UNICODE_LITERAL_II) && defined(Q_CC_GNU) && !defined(Q_COMPILER_LAMBDA)
# undef QStringLiteral
# define QStringLiteral QLatin1String
#endif

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemDescription::DefinitionName(QOrganizerItemDetail::TypeDescription);

/*!
    \enum QOrganizerItemDescription::DescriptionField

    This enumeration defines the fields supported by QOrganizerItemDescription.
    \value FieldDescription  The value stored is a description.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemDisplayLabel::DefinitionName(QOrganizerItemDetail::TypeDisplayLabel);

/*!
    \enum QOrganizerItemDisplayLabel::DisplayLabelField

    This enumeration defines the fields supported by QOrganizerItemDisplayLabel.
    \value FieldLabel  The value stored is a description label.
 */


/*!
    \class QOrganizerEventAttendee
    \brief The QOrganizerEventAttendee class contains information about an attendee of an event
    \inmodule QtOrganizer
    \ingroup organizer-details

    Attendee details contain information such as the display label (name) of an attendee, their
    role in the event, and their participation status.
 */

/*!
    \variable QOrganizerEventAttendee::DefinitionName

    The constant string which identifies the definition of details which contain information of an event attendee.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerEventAttendee::DefinitionName(QOrganizerItemDetail::TypeEventAttendee);

/*!
    \enum QOrganizerEventAttendee::EventAttendeeField

    This enumeration defines the fields supported by QOrganizerEventAttendee.
    \value FieldName                 The value stored describes the name of the attendee.
    \value FieldEmailAddress         The value stored describes the Email address of the attendee.
    \value FieldAttendeeId           The value stored describes the ID of the attendee. It can be e.g.
                                     a serialized contact ID, vCard UID, or any other platform specific
                                     ID.
    \value FieldParticipationStatus  The value stored describes the participation status of the attendee.
    \value FieldParticipationRole    The value stored describes the participation role of the attendee.
 */

/*!
    \enum QOrganizerEventAttendee::ParticipationStatus
    \value StatusUnknown The status of the attendee is unknown or they have yet to respond
    \value StatusAccepted The attendee has responded that they will be attending the event
    \value StatusDeclined The attendee has responded that they will not be attending the event
    \value StatusTentative The attendee has responded that they may be attending the event
    \value StatusDelegated The attendee has delegated attendance at the event to another person
    \value StatusInProcess The attendee is currently attending the event
    \value StatusCompleted The attendee attended the event
*/

/*!
    \enum QOrganizerEventAttendee::ParticipationRole
    \value RoleUnknown The role of the attendee is unknown or they have yet to respond
    \value RoleOrganizer The attendee is the organizer of the event
    \value RoleChairperson The attendee is the chairperson of the event
    \value RoleHost The attendee is the host of the event
    \value RoleRequiredParticipant The attendee is a required participant of the event
    \value RoleOptionalParticipant The attendee is an optional participant of the event
    \value RoleNonParticipant The attendee is not participating in the event (value included for informational purposes only, as per iCalendar specification)
*/

/*!
    \fn QOrganizerEventAttendee::setName(const QString &name)
    Sets the name (or title or other label) of the attendee to \a name.
 */
void QOrganizerEventAttendee::setName(const QString &name)
{
    setValue(FieldName, name);
}
/*!
    \fn QOrganizerEventAttendee::name() const
    Returns the name (or title or other label) of the attendee.
 */
QString QOrganizerEventAttendee::name() const
{
    return value<QString>(FieldName);
}

/*!
    \fn QOrganizerEventAttendee::setEmailAddress(const QString &emailAddress)
    Sets the email address of the attendee to \a emailAddress.
 */
void QOrganizerEventAttendee::setEmailAddress(const QString &emailAddress)
{
    setValue(FieldEmailAddress, emailAddress);
}

/*!
    \fn QOrganizerEventAttendee::emailAddress() const
    Returns the email address of the attendee.
 */
QString QOrganizerEventAttendee::emailAddress() const
{
    return value<QString>(FieldEmailAddress);
}

/*!
    \fn QOrganizerEventAttendee::setAttendeeId(const QString &attendeeId)
    Sets the unique identifier of the attendee.
 */
void QOrganizerEventAttendee::setAttendeeId(const QString &attendeeId)
{
    setValue(FieldAttendeeId, attendeeId);
}

/*!
    \fn QOrganizerEventAttendee::attendeeId() const
    Returns the unique identifier of the attendee.  The format of the identifier
    is platform specific and may be a serialized id, a vCard UID, or something else.
 */
QString QOrganizerEventAttendee::attendeeId() const
{
    return value<QString>(FieldAttendeeId);
}

/*!
    \fn QOrganizerEventAttendee::setParticipationStatus(ParticipationStatus status)
    Sets the participation status of the attendee in the event to \a status.
 */
void QOrganizerEventAttendee::setParticipationStatus(ParticipationStatus status)
{
    setValue(FieldParticipationStatus, status);
}

/*!
    \fn QOrganizerEventAttendee::participationStatus() const
    Returns the participation status of the attendee in the event.
 */
QOrganizerEventAttendee::ParticipationStatus QOrganizerEventAttendee::participationStatus() const
{
    return static_cast<ParticipationStatus>(value<int>(FieldParticipationStatus));
}

/*!
    \fn QOrganizerEventAttendee::setParticipationRole(ParticipationRole role)
    Sets the role of the attendee in the event to \a role.
 */
void QOrganizerEventAttendee::setParticipationRole(ParticipationRole role)
{
    setValue(FieldParticipationRole, static_cast<int>(role));
}

/*!
    \fn QOrganizerEventAttendee::participationRole() const
    Returns the participation role of the attendee in the event.
 */
QOrganizerEventAttendee::ParticipationRole QOrganizerEventAttendee::participationRole() const
{
    return static_cast<ParticipationRole>(value<int>(FieldParticipationRole));
}

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerEventTime::DefinitionName(QOrganizerItemDetail::TypeEventTime);

/*!
    \enum QOrganizerEventTime::EventTimeField

    This enumeration defines the fields supported by QOrganizerEventTime.
    \value FieldEndDateTime    The value stored describes the end date time of the event.
    \value FieldStartDateTime  The value stored describes the start date time of the event.
    \value FieldAllDay         The value stored describes if the event is an all day event.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemGuid::DefinitionName(QOrganizerItemDetail::TypeGuid);

/*!
    \enum QOrganizerItemGuid::GuidField

    This enumeration defines the fields supported by QOrganizerItemGuid.
    \value FieldGuid  The value stored is the global unique identifier of the item.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemParent::DefinitionName(QOrganizerItemDetail::TypeParent);

/*!
    \enum QOrganizerItemParent::ParentField

    This enumeration defines the fields supported by QOrganizerItemParent.
    \value FieldParentId      The value stored describes the ID of the item's parent item.
    \value FieldOriginalDate  The value stored describes the original date of this instance or exception
                              according to the recurrent series of the parent item is stored.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerJournalTime::DefinitionName(QOrganizerItemDetail::TypeJournalTime);

/*!
    \enum QOrganizerJournalTime::JournalTimeField

    This enumeration defines the fields supported by QOrganizerJournalTime.
    \value FieldEntryDateTime  The value stored describes the date time of the journal entry.
 */

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
    \variable QOrganizerItemLocation::DefinitionName

    The constant string which identifies the definition of details
    which describe a location associated with an organizer item.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemLocation::DefinitionName(QOrganizerItemDetail::TypeLocation);

/*!
    \enum QOrganizerItemLocation::LocationField

    This enumeration defines the fields supported by QOrganizerItemLocation.
    \value FieldLatitude   The value stored describes the latitude of the location.
    \value FieldLongitude  The value stored describes the longitude of the location.
    \value FieldLabel      The value stored is a label for the location.
 */

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
    \variable QOrganizerItemComment::DefinitionName

    The constant string which identifies the definition of details which are comments.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemComment::DefinitionName(QOrganizerItemDetail::TypeComment);

/*!
    \enum QOrganizerItemComment::CommentField

    This enumeration defines the fields supported by QOrganizerItemComment.
    \value FieldComment  The value is a comment of the item.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemPriority::DefinitionName(QOrganizerItemDetail::TypePriority);

/*!
    \enum QOrganizerItemPriority::PriorityField

    This enumeration defines the fields supported by QOrganizerItemPriority.
    \value FieldPriority  The value describes the priority of the item.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemRecurrence::DefinitionName(QOrganizerItemDetail::TypeRecurrence);

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
    return recurrenceRules() == other.recurrenceRules()
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
    \enum QOrganizerItemRecurrence::RecurrenceField

    This enumeration defines the fields supported by QOrganizerItemRecurrence.
    \value FieldRecurrenceRules  The value stored describes the rules for when an item should recur.
    \value FieldRecurrenceDates  The value stored describes the dates for when an item should recur.
    \value FieldExceptionRules   The value stored describes the rules for when an item should not recur.
    \value FieldExceptionDates   The value stored describes the dates for when an item should not recur.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemReminder::DefinitionName(QOrganizerItemDetail::TypeReminder);

/*!
    \enum QOrganizerItemReminder::ReminderField

    This enumeration defines the fields supported by QOrganizerItemReminder.
    \value FieldSecondsBeforeStart  The value stored describes the time in seconds prior to the item's
                                    start time, when the reminder should be triggered.
    \value FieldRepetitionCount     The value stored describes the number of repetitions of the reminder.
    \value FieldRepetitionDelay     The value stored describes the delays in seconds between repetitions
                                    of the reminder.
 */

/*!
    Returns the reminder type of this reminder for an organizer item.
 */
QOrganizerItemReminder::ReminderType QOrganizerItemReminder::reminderType() const
{
    if (type() == QOrganizerItemDetail::TypeAudibleReminder)
        return QOrganizerItemReminder::AudibleReminder;
    else if (type() == QOrganizerItemDetail::TypeEmailReminder)
        return QOrganizerItemReminder::EmailReminder;
    else if (type() == QOrganizerItemDetail::TypeVisualReminder)
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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemAudibleReminder::DefinitionName(QOrganizerItemDetail::TypeAudibleReminder);

/*!
    \enum QOrganizerItemAudibleReminder::AudibleReminderField

    This enumeration defines the fields supported by QOrganizerItemAudibleReminder.
    \value FieldDataUrl  The value stored describes URL of the sound to be played when the reminder is triggered.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemEmailReminder::DefinitionName(QOrganizerItemDetail::TypeEmailReminder);

/*!
    \enum QOrganizerItemEmailReminder::EmailReminderField

    This enumeration defines the fields supported by QOrganizerItemEmailReminder.
    \value FieldSubject      The value stored describes the subject of the Email, which the user wishes to be sent as a reminder.
    \value FieldBody         The value stored describes the body of the Email, which the user wishes to be sent as a reminder.
    \value FieldAttachments  The value stored describes the attachments of the Email, which the user wishes to be sent as a reminder.
    \value FieldRecipients   The value stored describes the recipients of the Email, which the user wishes to be sent as a reminder.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemVisualReminder::DefinitionName(QOrganizerItemDetail::TypeVisualReminder);

/*!
    \enum QOrganizerItemVisualReminder::VisualReminderField

    This enumeration defines the fields supported by QOrganizerItemVisualReminder.
    \value FieldMessage  The value stored describes the message to be shown when the reminder is triggered.
    \value FieldDataUrl  The value stored describes URL of the video to be played when the reminder is triggered.
 */

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
    \variable QOrganizerItemTag::DefinitionName

    The constant string which identifies the definition of details which are tags.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemTag::DefinitionName(QOrganizerItemDetail::TypeTag);

/*!
    \enum QOrganizerItemTag::TagField

    This enumeration defines the fields supported by QOrganizerItemTag.
    \value FieldTag  The value stored is a tag of the item.
 */

/*!
    \variable QOrganizerItemTag::Anniversary

    The constant string for anniversary and birthday tag.
*/
const QString QOrganizerItemTag::Anniversary(QStringLiteral("ANNIVERSARY"));

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemTimestamp::DefinitionName(QOrganizerItemDetail::TypeTimestamp);

/*!
    \enum QOrganizerItemTimestamp::TimestampField

    This enumeration defines the fields supported by QOrganizerItemTimestamp.
    \value FieldCreated      The value stored describes the time the item is created.
    \value FieldLastModified  The value stored describes the last time the item is modified.
 */

/*!
    \fn QOrganizerItemTimestamp::created() const

    Returns the creation timestamp saved in this detail.
 */
QDateTime QOrganizerItemTimestamp::created() const
{
    return value<QDateTime>(FieldCreated);
}

/*!
    \fn QOrganizerItemTimestamp::lastModified() const

    Returns the last-modified timestamp saved in this detail.
 */
QDateTime QOrganizerItemTimestamp::lastModified() const
{
    return value<QDateTime>(FieldLastModified);
}

/*!
    \fn QOrganizerItemTimestamp::setCreated(const QDateTime& dateTime)

    Sets the creation timestamp saved in this detail to \a dateTime.
 */
void QOrganizerItemTimestamp::setCreated(const QDateTime &timestamp)
{
    setValue(FieldCreated, timestamp);
}

/*!
    \fn QOrganizerItemTimestamp::setLastModified(const QDateTime& dateTime)

    Sets the last-modified timestamp saved in this detail to \a dateTime.
 */
void QOrganizerItemTimestamp::setLastModified(const QDateTime &timestamp)
{
    setValue(FieldLastModified, timestamp);
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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerTodoProgress::DefinitionName(QOrganizerItemDetail::TypeTodoProgress);

/*!
    \enum QOrganizerTodoProgress::TodoProgressField

    This enumeration defines the fields supported by QOrganizerTodoProgress.
    \value FieldStatus              The value stored describes the status of the TODO item.
    \value FieldPercentageComplete  The value stored describes the current completion percentage of the TODO item.
    \value FieldFinishedDateTime    The value stored describes the date time at which this TODO item is finished.
 */

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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerTodoTime::DefinitionName(QOrganizerItemDetail::TypeTodoTime);

/*!
    \enum QOrganizerTodoTime::TodoTimeField

    This enumeration defines the fields supported by QOrganizerTodoTime.
    \value FieldStartDateTime  The value stored describes the time when the TODO item should be started.
    \value FieldDueDateTime    The value stored describes the time when the TODO item should be finished.
    \value FieldAllDay         The value stored describes if it is an all day TODO item.
 */

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
    \brief The QOrganizerItemType class describes the type of the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \enum QOrganizerItemType::ItemType

    This enumeration describes the type of the organizer item.

    \value TypeUndefined        This item is of an unknown type.
    \value TypeEvent            This item is an event.
    \value TypeEventOccurrence  This item is an event occurrence.
    \value TypeTodo             This item is a TODO.
    \value TypeTodoOccurrence   This item is a TODO occurrence.
    \value TypeJournal          This item is a journal.
    \value TypeNote             This item is a note.
 */

/*!
    \variable QOrganizerItemType::DefinitionName

    The constant string which identifies the definition of details which identify the type of the organizer item.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemType::DefinitionName(QOrganizerItemDetail::TypeItemType);

/*!
    \enum QOrganizerItemType::ItemTypeField

    This enumeration defines the fields supported by QOrganizerItemType.
    \value FieldType  The value stored describes the type of the item.
 */

/*!
    Returns the organizer item type value stored in this detail.
 */
QOrganizerItemType::ItemType QOrganizerItemType::type() const
{
    return static_cast<ItemType>(value(FieldType).toInt());
}

/*!
    Sets the type of the organizer item to be the give \a type.
 */
void QOrganizerItemType::setType(QOrganizerItemType::ItemType type)
{
    setValue(FieldType, type);
}


/*!
   \class QOrganizerEventRsvp
   \brief The QOrganizerEventRsvp class contains RSVP information for an event, applicable to the user of the calendar
   \inmodule QtOrganizer
   \ingroup organizer-details

   RSVP detail contain information such as the role of the calendar user in the event,
   the participation status of the calendar user in the event, the date by which the
   user is requested to respond to the invitation, the date at which the user did
   respond to the invitation, the name of the organizer of the event, and the contact
   details of the organizer of the event.
 */

/*!
   \variable QOrganizerEventRsvp::DefinitionName
   The constant string which identifies the definition of details which are event invitation RSVP details.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerEventRsvp::DefinitionName(QOrganizerItemDetail::TypeEventRsvp);

/*!
    \enum QOrganizerEventRsvp::EventRsvpField

    This enumeration defines the fields supported by QOrganizerEventRsvp.
    \value FieldParticipationStatus  The value stored describes the pariticipation status of the user for this event.
    \value FieldParticipationRole    The value stored describes the pariticipation role of the user for this event.
    \value FieldResponseRequirement  The value stored describes the if the user is required to respond this event invitation.
    \value FieldResponseDeadline     The value stored describes when the user should respond to this event invitation.
    \value FieldResponseDate         The value stored describes when the user responds to this event invitation.
    \value FieldOrganizerName        The value stored describes the organizer's name of this event.
    \value FieldOrganizerEmail       The value stored describes the organizer's Email of this event.
 */

/*!
   \fn QOrganizerEventRsvp::setParticipationStatus(QOrganizerEventAttendee::ParticipationStatus status)
   Sets the participation status of the user of the calendar in the event to \a status.
 */
void QOrganizerEventRsvp::setParticipationStatus(QOrganizerEventAttendee::ParticipationStatus status)
{
    setValue(FieldParticipationStatus, static_cast<int>(status));
}

/*!
   \fn QOrganizerEventRsvp::participationStatus() const
   Returns the participation status of the user of the calendar in the event.
 */
QOrganizerEventAttendee::ParticipationStatus QOrganizerEventRsvp::participationStatus() const
{
    return static_cast<QOrganizerEventAttendee::ParticipationStatus>(value<int>(FieldParticipationStatus));
}

/*!
   \fn QOrganizerEventRsvp::setParticipationRole(QOrganizerEventAttendee::ParticipationRole role)
   Sets the role of the user of the calendar in the event to \a role.
 */
void QOrganizerEventRsvp::setParticipationRole(QOrganizerEventAttendee::ParticipationRole role)
{
    setValue(FieldParticipationRole, static_cast<int>(role));
}

/*!
   \fn QOrganizerEventRsvp::participationRole() const
   Returns the participation role of the user of the calendar in the event.
 */
QOrganizerEventAttendee::ParticipationRole QOrganizerEventRsvp::participationRole() const
{
    return static_cast<QOrganizerEventAttendee::ParticipationRole>(value<int>(FieldParticipationRole));
}

/*!
   \enum QOrganizerEventRsvp::ResponseRequirement
   \value ResponseNotRequired The organizer does not require the calendar user to respond to the invitation
   \value ResponseRequired The organizer requires the calendar user to respond to the invitation
*/

/*!
   \fn QOrganizerEventRsvp::setResponseRequirement(QOrganizerEventRsvp::ResponseRequirement responseRequirement)
   Sets the response requirement for the invitation to \a responseRequirement.
 */
void QOrganizerEventRsvp::setResponseRequirement(ResponseRequirement responseRequirement)
{
    setValue(FieldResponseRequirement, static_cast<int>(responseRequirement));
}

/*!
   \fn QOrganizerEventRsvp::responseRequirement() const
   Returns the response requirement of the invitation.
 */
QOrganizerEventRsvp::ResponseRequirement QOrganizerEventRsvp::responseRequirement() const
{
    return static_cast<ResponseRequirement>(value<int>(FieldResponseRequirement));
}

/*!
   \fn QOrganizerEventRsvp::setResponseDeadline(const QDate &date)
   Sets the date by which the user was requested to have responded to the invitation to the event to \a date.
 */
void QOrganizerEventRsvp::setResponseDeadline(const QDate &date)
{
    setValue(FieldResponseDeadline, date);
}

/*!
   \fn QOrganizerEventRsvp::responseDeadline() const
   Returns the date by which the user was requested to have responded to the invitation to the event.
 */
QDate QOrganizerEventRsvp::responseDeadline() const
{
    return value<QDate>(FieldResponseDeadline);
}

/*!
   \fn QOrganizerEventRsvp::setResponseDate(const QDate &date)
   Sets the date at which the user responded to the invitation to the event to \a date.
 */
void QOrganizerEventRsvp::setResponseDate(const QDate &date)
{
    setValue(FieldResponseDate, date);
}

/*!
   \fn QOrganizerEventRsvp::responseDate() const
   Returns the date at which user responded to the invitation to the event.
 */
QDate QOrganizerEventRsvp::responseDate() const
{
    return value<QDate>(FieldResponseDate);
}

/*!
   \fn QOrganizerEventRsvp::setOrganizerName(const QString &name)
   Sets the name of the organizer of the event (who sent the invitation) to \a name.
 */
void QOrganizerEventRsvp::setOrganizerName(const QString &name)
{
    setValue(FieldOrganizerName, name);
}

/*!
   \fn QOrganizerEventRsvp::organizerName() const
   Returns the name of the organizer of the event.
 */
QString QOrganizerEventRsvp::organizerName() const
{
    return value<QString>(FieldOrganizerName);
}

/*!
   \fn QOrganizerEventRsvp::setOrganizerEmail(const QString &emailAddress)
   Sets the email address of the organizer of the event (who sent the invitation) to \a emailAddress.
 */
void QOrganizerEventRsvp::setOrganizerEmail(const QString &email)
{
    setValue(FieldOrganizerEmail, email);
}

/*!
   \fn QOrganizerEventRsvp::organizerEmail() const
   Returns the email address of the organizer of the event.
 */
QString QOrganizerEventRsvp::organizerEmail() const
{
    return value<QString>(FieldOrganizerEmail);
}

/*!
    \class QOrganizerItemClassification
    \brief The QOrganizerItemClassification class is for defining the classification of an organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details

    This can be used as a part of security model for the organizer.
 */

/*!
   \variable QOrganizerItemClassification::DefinitionName

   The constant string which identifies the definition of Classification-detail.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemClassification::DefinitionName(QOrganizerItemDetail::TypeClassification);

/*!
    \enum QOrganizerItemClassification::ClassificationField

    This enumeration defines the fields supported by QOrganizerItemClassification.
    \value FieldClassification  The value stored describes the classification of an item.
 */

/*!
    \variable QOrganizerItemClassification::Public

    The constant string for public access.
*/
const QString QOrganizerItemClassification::Public(QStringLiteral("PUBLIC"));

/*!
    \variable QOrganizerItemClassification::Private

    The constant string for private access.
*/
const QString QOrganizerItemClassification::Private(QStringLiteral("PRIVATE"));

/*!
    \variable QOrganizerItemClassification::Confidential

    The constant string for confidential access.
*/
const QString QOrganizerItemClassification::Confidential(QStringLiteral("CONFIDENTIAL"));

/*!
   \fn QOrganizerItemClassification::setClassification(const QString &classification)

   Sets the classification of the item \a classification.
 */
void QOrganizerItemClassification::setClassification(const QString &classification)
{
    setValue(FieldClassification, classification);
}

/*!
   \fn QOrganizerItemClassification::classification() const

   Returns classification of the event.
 */
QString QOrganizerItemClassification::classification() const
{
    return value<QString>(FieldClassification);
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

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemExtendedDetail::DefinitionName(QOrganizerItemDetail::TypeExtendedDetail);

/*!
    \enum QOrganizerItemExtendedDetail::ExtendedDetailField

    This enumeration defines the fields supported by QOrganizerItemExtendedDetail.
    \value FieldExtendedDetailName  The value stored describes the name of this extended detail.
    \value FieldExtendedDetailData  The value stored describes the data stored in this extended detail.
 */

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

/*!
    \class QOrganizerItemVersion
    \brief The QOrganizerItemVersion class provides the versioning information of an organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \variable QOrganizerItemVersion::DefinitionName

    The constant string which identifies the definition of details which are versions.

    Note that this is to be removed soon, please use QOrganizerItemDetail::DetailType instead.
 */
const QOrganizerItemDetail::DetailType QOrganizerItemVersion::DefinitionName(QOrganizerItemDetail::TypeVersion);

/*!
    \enum QOrganizerItemVersion::VersionField

    This enumeration defines the fields supported by QOrganizerItemVersion.
    \value FieldVersion          The value stored describes the integer version of an organizer item.
                                 It can be used as the sequence number as per iCalendar spec.
    \value FieldExtendedVersion  The value stored describes the extended version of an organizer item.
                                 It can be used to represent the version stored in the back-end.
 */

/*!
    Sets the integer \a version.
 */
void QOrganizerItemVersion::setVersion(int version)
{
    setValue(FieldVersion, version);
}

/*!
    Gets the integer version.
 */
int QOrganizerItemVersion::version() const
{
    return value(FieldVersion).toInt();
}

/*!
    Sets the \a extendedVersion.
 */
void QOrganizerItemVersion::setExtendedVersion(const QByteArray &extendedVersion)
{
    setValue(FieldExtendedVersion, extendedVersion);
}

/*!
    Gets the extended version.
 */
QByteArray QOrganizerItemVersion::extendedVersion() const
{
    return value(FieldExtendedVersion).toByteArray();
}

QTORGANIZER_END_NAMESPACE
