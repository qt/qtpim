/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include "qorganizeritemdetails.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qurl.h>

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemDescription
    \brief The QOrganizerItemDescription class contains some arbitrary information which is relevant to the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \enum QOrganizerItemDescription::DescriptionField

    This enumeration defines the fields supported by QOrganizerItemDescription.
    \value FieldDescription  The value stored is a description.
 */

/*!
    Sets a description associated with an organizer item to \a description.
 */
void QOrganizerItemDescription::setDescription(const QString &description)
{
    setValue(FieldDescription, description);
}

/*!
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
    Sets the display label of the organizer item to \a label.
 */
void QOrganizerItemDisplayLabel::setLabel(const QString &label)
{
    setValue(FieldLabel, label);
}

/*!
    Returns the display label of the organizer item.
 */
QString QOrganizerItemDisplayLabel::label() const
{
    return value(FieldLabel).toString();
}

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
    \value StatusUnknown    The status of the attendee is unknown or they have yet to respond.
    \value StatusAccepted   The attendee has responded that they will be attending the event.
    \value StatusDeclined   The attendee has responded that they will not be attending the event.
    \value StatusTentative  The attendee has responded that they may be attending the event.
    \value StatusDelegated  The attendee has delegated attendance at the event to another person.
    \value StatusInProcess  The attendee is currently attending the event.
    \value StatusCompleted  The attendee attended the event.
*/

/*!
    \enum QOrganizerEventAttendee::ParticipationRole
    \value RoleUnknown              The role of the attendee is unknown or they have yet to respond.
    \value RoleOrganizer            The attendee is the organizer of the event.
    \value RoleChairperson          The attendee is the chairperson of the event.
    \value RoleHost                 The attendee is the host of the event.
    \value RoleRequiredParticipant  The attendee is a required participant of the event.
    \value RoleOptionalParticipant  The attendee is an optional participant of the event.
    \value RoleNonParticipant       The attendee is not participating in the event (value included
                                    for informational purposes only, as per iCalendar specification).
*/

/*!
    Sets the name (or title or other label) of the attendee to \a name.
 */
void QOrganizerEventAttendee::setName(const QString &name)
{
    setValue(FieldName, name);
}

/*!
    Returns the name (or title or other label) of the attendee.
 */
QString QOrganizerEventAttendee::name() const
{
    return value(FieldName).toString();
}

/*!
    Sets the email address of the attendee to \a emailAddress.
 */
void QOrganizerEventAttendee::setEmailAddress(const QString &emailAddress)
{
    setValue(FieldEmailAddress, emailAddress);
}

/*!
    Returns the email address of the attendee.
 */
QString QOrganizerEventAttendee::emailAddress() const
{
    return value(FieldEmailAddress).toString();
}

/*!
    Sets the unique identifier of the attendee to \a attendeeId.
 */
void QOrganizerEventAttendee::setAttendeeId(const QString &attendeeId)
{
    setValue(FieldAttendeeId, attendeeId);
}

/*!
    Returns the unique identifier of the attendee.  The format of the identifier
    is platform specific and may be a serialized id, a vCard UID, or something else.
 */
QString QOrganizerEventAttendee::attendeeId() const
{
    return value(FieldAttendeeId).toString();
}

/*!
    Sets the participation status of the attendee in the event to \a status.
 */
void QOrganizerEventAttendee::setParticipationStatus(ParticipationStatus status)
{
    setValue(FieldParticipationStatus, status);
}

/*!
    Returns the participation status of the attendee in the event.
 */
QOrganizerEventAttendee::ParticipationStatus QOrganizerEventAttendee::participationStatus() const
{
    return static_cast<ParticipationStatus>(value(FieldParticipationStatus).toInt());
}

/*!
    Sets the role of the attendee in the event to \a role.
 */
void QOrganizerEventAttendee::setParticipationRole(ParticipationRole role)
{
    setValue(FieldParticipationRole, role);
}

/*!
    Returns the participation role of the attendee in the event.
 */
QOrganizerEventAttendee::ParticipationRole QOrganizerEventAttendee::participationRole() const
{
    return static_cast<ParticipationRole>(value(FieldParticipationRole).toInt());
}

/*!
    \class QOrganizerEventTime
    \brief The QOrganizerEventTime class contains the start and end dates and times of a recurring event series,
           or occurrence of an event.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \enum QOrganizerEventTime::EventTimeField

    This enumeration defines the fields supported by QOrganizerEventTime.
    \value FieldEndDateTime    The value stored describes the end date time of the event.
    \value FieldStartDateTime  The value stored describes the start date time of the event.
    \value FieldAllDay         The value stored describes if the event is an all day event.
 */

/*!
    Returns the event time's start date and time as QDateTime.
    For all-day events, the time part is meaningless.

    \sa QOrganizerEventTime::setStartDateTime()
 */
QDateTime QOrganizerEventTime::startDateTime() const
{
    return value(FieldStartDateTime).toDateTime();
}

/*!
    Sets the event time's start date and time to \a startDateTime.
    For all-day events, the time part can be set to any valid value.

    \sa QOrganizerEventTime::startDateTime()
 */
void QOrganizerEventTime::setStartDateTime(const QDateTime &startDateTime)
{
    setValue(FieldStartDateTime, startDateTime);
}

/*!
    Sets the event time's due date and time to \a endDateTime.
    For all-day events, the time part can be set to any valid value, and the date is to be
    interpreted as the inclusive end date.

    \sa QOrganizerEventTime::endDateTime()
 */
void QOrganizerEventTime::setEndDateTime(const QDateTime &endDateTime)
{
    setValue(FieldEndDateTime, endDateTime);
}

/*!
    Returns the event time's due date and time as QDateTime.
    For all-day events, the time part is meaningless, and the date is to be interpreted as the
    inclusive end date.

    \sa QOrganizerEventTime::setEndDateTime()
 */
QDateTime QOrganizerEventTime::endDateTime() const
{
    return value(FieldEndDateTime).toDateTime();
}

/*!
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
    Returns true if the event is an all-day event, or false otherwise.
 */
bool QOrganizerEventTime::isAllDay() const
{
    return value(FieldAllDay).toBool();
}


/*!
    \class QOrganizerItemGuid
    \brief The QOrganizerItemGuid class contains the globally unique identifier of the organizer item,
           which can be used for synchronization purposes.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \enum QOrganizerItemGuid::GuidField

    This enumeration defines the fields supported by QOrganizerItemGuid.
    \value FieldGuid  The value stored is the global unique identifier of the item.
 */

/*!
    Returns the globally unique identifier which is stored in this detail.
 */
QString QOrganizerItemGuid::guid() const
{
    return value(FieldGuid).toString();
}

/*!
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
    \enum QOrganizerItemParent::ParentField

    This enumeration defines the fields supported by QOrganizerItemParent.
    \value FieldParentId      The value stored describes the ID of the item's parent item.
    \value FieldOriginalDate  The value stored describes the original date of this instance or exception
                              according to the recurrent series of the parent item is stored.
 */

/*!
    Returns the ID of the item instance origin's parent item.
 */
QOrganizerItemId QOrganizerItemParent::parentId() const
{
    return value(FieldParentId).value<QOrganizerItemId>();
}

/*!
    Sets the parent ID of this instance origin item to \a parentId.
 */
void QOrganizerItemParent::setParentId(const QOrganizerItemId &parentId)
{
    setValue(FieldParentId, QVariant::fromValue(parentId));
}

/*!
    Returns the original date of this instance origin item.
 */
QDate QOrganizerItemParent::originalDate() const
{
    return value(FieldOriginalDate).toDate();
}

/*!
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
    \enum QOrganizerJournalTime::JournalTimeField

    This enumeration defines the fields supported by QOrganizerJournalTime.
    \value FieldEntryDateTime  The value stored describes the date time of the journal entry.
 */

/*!
    Returns the journal entry date and time as QDateTime.
 */
QDateTime QOrganizerJournalTime::entryDateTime() const
{
    return value(FieldEntryDateTime).toDateTime();
}

/*!
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
    \enum QOrganizerItemLocation::LocationField

    This enumeration defines the fields supported by QOrganizerItemLocation.
    \value FieldLatitude   The value stored describes the latitude of the location.
    \value FieldLongitude  The value stored describes the longitude of the location.
    \value FieldLabel      The value stored is a label for the location.
 */

/*!
    Returns the latitude value of the location's geocoordinates.
 */
double QOrganizerItemLocation::latitude() const
{
    return value(FieldLatitude).toDouble();
}

/*!
    Sets the latitude value of the location's geocoordinates to \a latitude.

    The equator has a latitude of 0, the North pole has a latitude of 90, and the South pole has a
    latitude of -90. Values out of the range will be ignored.
 */
void QOrganizerItemLocation::setLatitude(double latitude)
{
    if (latitude >= -90 && latitude <= 90)
        setValue(FieldLatitude, latitude);
}

/*!
    Returns the longitude value of the location's geocoordinates.
 */
double QOrganizerItemLocation::longitude() const
{
    return value(FieldLongitude).toDouble();
}

/*!
    Sets the longitude value of the location's geocoordinates to \a longitude.

    The Prime Meridian has a longitude of 0, ranging to 180 eastward and -180 westward. Values out
    of the range will be ignored.
 */
void QOrganizerItemLocation::setLongitude(double longitude)
{
    if (longitude >= -180 && longitude <= 180)
        setValue(FieldLongitude, longitude);
}

/*!
    Returns the human-readable label of the location.
 */
QString QOrganizerItemLocation::label() const
{
    return value(FieldLabel).toString();
}

/*!
    Sets the human-readable label of the location to \a label.
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
    \enum QOrganizerItemComment::CommentField

    This enumeration defines the fields supported by QOrganizerItemComment.
    \value FieldComment  The value is a comment of the item.
 */

/*!
    Sets a comment associated with an organizer item to \a comment.
 */
void QOrganizerItemComment::setComment(const QString &comment)
{
    setValue(FieldComment, comment);
}

/*!
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
    \enum QOrganizerItemPriority::PriorityField

    This enumeration defines the fields supported by QOrganizerItemPriority.
    \value FieldPriority  The value describes the priority of the item.
 */

/*!
    Sets the priority associated with an organizer item to \a priority.
 */
void QOrganizerItemPriority::setPriority(Priority priority)
{
    setValue(FieldPriority, priority);
}

/*!
    Returns the priority associated with an organizer item.
 */
QOrganizerItemPriority::Priority QOrganizerItemPriority::priority() const
{
    return static_cast<Priority>(value(FieldPriority).toInt());
}


/*!
    \class QOrganizerItemRecurrence
    \brief The QOrganizerItemRecurrence class contains a list of rules and dates on which the recurrent item occurs,
           and a list of rules and dates on which exceptions occur.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

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

    Returns true if the \a other recurrence detail is not equal to this detail; otherwise, false.
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

    This enumeration defines the type of the reminder.
    \value NoReminder This reminder is entirely unobtrusive
    \value AudibleReminder This reminder has an audible element
    \value VisualReminder This reminder has a visual element
    \value EmailReminder This reminder has a email element
 */

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
    Sets the number of seconds prior to the activation of the item
    at which the user wants to be reminded of the item to \a seconds.

    The exact datetime of activation of the item depends on the type of
    item: for a QOrganizerTodo or QOrganizerTodoOccurrence it is the
    due date time; for a QOrganizerEvent or QOrganizerEventOccurrence
    it is the start date time.

    The value must be non-negative, and negative values will be ignored.
 */
void QOrganizerItemReminder::setSecondsBeforeStart(int seconds)
{
    if (seconds >= 0)
        setValue(FieldSecondsBeforeStart, seconds);
}

/*!
    Returns the number of seconds prior to the activation of the item
    at which the user wants to be reminded of the item.

    The exact datetime of activation of the item depends on the type of
    item: for a QOrganizerTodo or QOrganizerTodoOccurrence it is the
    due date time; for a QOrganizerEvent or QOrganizerEventOccurrence
    it is the start date time.
 */
int QOrganizerItemReminder::secondsBeforeStart() const
{
    return value(FieldSecondsBeforeStart).toInt();
}

/*!
    Returns the number of times the user should be reminded of the item.

    \sa repetitionDelay()
 */
int QOrganizerItemReminder::repetitionCount() const
{
    return value(FieldRepetitionCount).toInt();
}

/*!
    Returns the delay (in seconds) between each repetition of the reminder.

    \sa repetitionCount()
 */
int QOrganizerItemReminder::repetitionDelay() const
{
    return value(FieldRepetitionDelay).toInt();
}

/*!
    Sets the number of repetitions of the reminderto \a count, and the delay (in seconds)
    between each repetition of the reminder to \a delaySeconds.

    Both \a count and \a delaySeconds must be positive numbers, otherwise both will be ignored.

    \sa repetitionCount(), repetitionDelay()
*/
void QOrganizerItemReminder::setRepetition(int count, int delaySeconds)
{
    if (count > 0 && delaySeconds >= 0) {
        setValue(FieldRepetitionCount, count);
        setValue(FieldRepetitionDelay, delaySeconds);
    }
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
    \enum QOrganizerItemAudibleReminder::AudibleReminderField

    This enumeration defines the fields supported by QOrganizerItemAudibleReminder.
    \value FieldDataUrl  The value stored describes URL of the sound to be played when the reminder is triggered.
 */

/*!
    Sets the url of the audible data which should be played to \a dataUrl.
 */
void QOrganizerItemAudibleReminder::setDataUrl(const QUrl &dataUrl)
{
    setValue(FieldDataUrl, dataUrl);
}

/*!
    Returns the url of the audible data which should be played.
 */
QUrl QOrganizerItemAudibleReminder::dataUrl() const
{
    return value(FieldDataUrl).toUrl();
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
    \enum QOrganizerItemEmailReminder::EmailReminderField

    This enumeration defines the fields supported by QOrganizerItemEmailReminder.
    \value FieldSubject      The value stored describes the subject of the Email, which the user wishes to be sent as a reminder.
    \value FieldBody         The value stored describes the body of the Email, which the user wishes to be sent as a reminder.
    \value FieldAttachments  The value stored describes the attachments of the Email, which the user wishes to be sent as a reminder.
    \value FieldRecipients   The value stored describes the recipients of the Email, which the user wishes to be sent as a reminder.
 */

/*!
    Sets the contents of the email reminder to be the given \a subject, \a body and \a attachments.
 */
void QOrganizerItemEmailReminder::setContents(const QString &subject, const QString &body, const QVariantList &attachments)
{
    setValue(FieldSubject, subject); setValue(FieldBody, body); setValue(FieldAttachments, attachments);
}

/*!
    Returns the subject of the email.
 */
QString QOrganizerItemEmailReminder::subject() const
{
    return value(FieldSubject).toString();
}

/*!
    Returns the body of the email.
 */
QString QOrganizerItemEmailReminder::body() const
{
    return value(FieldBody).toString();
}

/*!
    Returns the attachments of the email.
 */
QVariantList QOrganizerItemEmailReminder::attachments() const
{
    return value(FieldAttachments).toList();
}

/*!
    Sets the list of recipients that the user wishes to be sent an email as part of the reminder
    to \a recipients.
 */
void QOrganizerItemEmailReminder::setRecipients(const QStringList &recipients)
{
    setValue(FieldRecipients, recipients);
}

/*!
    Returns the list of recipients that the user wishes to be sent an email as part of the reminder.
 */
QStringList QOrganizerItemEmailReminder::recipients() const
{
    return value(FieldRecipients).toStringList();
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
    \enum QOrganizerItemVisualReminder::VisualReminderField

    This enumeration defines the fields supported by QOrganizerItemVisualReminder.
    \value FieldMessage  The value stored describes the message to be shown when the reminder is triggered.
    \value FieldDataUrl  The value stored describes URL of the video to be played when the reminder is triggered.
 */

/*!
    Sets the message which the user wishes to be displayed as part of the reminder to \a message.
*/
void QOrganizerItemVisualReminder::setMessage(const QString &message)
{
    setValue(FieldMessage, message);
}

/*!
    Returns the message which the user wishes to be displayed as part of the reminder.
*/
QString QOrganizerItemVisualReminder::message() const
{
    return value(FieldMessage).toString();
}

/*!
    Sets the url of the visual data which the user wishes to be displayed as part of the reminder to \a dataUrl.
*/
void QOrganizerItemVisualReminder::setDataUrl(const QUrl &dataUrl)
{
    setValue(FieldDataUrl, dataUrl);
}

/*!
    Returns the url of the visual data which the user wishes to be displayed as part of the reminder.
*/
QUrl QOrganizerItemVisualReminder::dataUrl() const
{
    return value(FieldDataUrl).toUrl();
}


/*!
    \class QOrganizerItemTag
    \brief The QOrganizerItemTag class contains some arbitrary tag which is relevant to the organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \enum QOrganizerItemTag::TagField

    This enumeration defines the fields supported by QOrganizerItemTag.
    \value FieldTag  The value stored is a tag of the item.
 */

/*!
    Sets a tag associated with an organizer item to \a tag.
 */
void QOrganizerItemTag::setTag(const QString &tag)
{
    setValue(FieldTag, tag);
}

/*!
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
    \enum QOrganizerItemTimestamp::TimestampField

    This enumeration defines the fields supported by QOrganizerItemTimestamp.
    \value FieldCreated      The value stored describes the time the item is created.
    \value FieldLastModified  The value stored describes the last time the item is modified.
 */

/*!
    Returns the creation timestamp saved in this detail.
 */
QDateTime QOrganizerItemTimestamp::created() const
{
    return value(FieldCreated).toDateTime();
}

/*!
    Returns the last-modified timestamp saved in this detail.
 */
QDateTime QOrganizerItemTimestamp::lastModified() const
{
    return value(FieldLastModified).toDateTime();
}

/*!
    Sets the creation timestamp saved in this detail to \a timestamp.
 */
void QOrganizerItemTimestamp::setCreated(const QDateTime &timestamp)
{
    setValue(FieldCreated, timestamp);
}

/*!
    Sets the last-modified timestamp saved in this detail to \a timestamp.
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
    Returns the todo progress item's current status as QOrganizerTodoProgress::Status.
 */
QOrganizerTodoProgress::Status QOrganizerTodoProgress::status() const
{
    return static_cast<Status>(value(FieldStatus).toInt());
}

/*!
    Sets the todo progress item's current status to \a status.
 */
void QOrganizerTodoProgress::setStatus(Status status)
{
    setValue(FieldStatus, status);
}

/*!
    Returns the todo progress item's finished date and timeas QDateTime.
 */
QDateTime QOrganizerTodoProgress::finishedDateTime() const
{
    return value(FieldFinishedDateTime).toDateTime();
}

/*!
    Sets the todo progress item's finished date and time to \a finishedDateTime.
 */
void QOrganizerTodoProgress::setFinishedDateTime(const QDateTime &finishedDateTime)
{
    setValue(FieldFinishedDateTime, finishedDateTime);
}

/*!
    Returns the todo progress item's completion percentage.
 */
int QOrganizerTodoProgress::percentageComplete() const
{
    return value(FieldPercentageComplete).toInt();
}

/*!
    Sets the todo progress item's completion percentage to \a percentage. The \a percentage must
    be between 0 and 100, and values out of the range will be ignored.
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
    \enum QOrganizerTodoTime::TodoTimeField

    This enumeration defines the fields supported by QOrganizerTodoTime.
    \value FieldStartDateTime  The value stored describes the time when the TODO item should be started.
    \value FieldDueDateTime    The value stored describes the time when the TODO item should be finished.
    \value FieldAllDay         The value stored describes if it is an all day TODO item.
 */

/*!
    Returns the todo time's start date and time as QDateTime.
    For all-day tasks, the time part is meaningless.
 */
QDateTime QOrganizerTodoTime::startDateTime() const
{
    return value(FieldStartDateTime).toDateTime();
}

/*!
    Sets the todo time's start date and time to \a startDateTime.
    For all-day tasks, the time part can be set to any valid value.
 */
void QOrganizerTodoTime::setStartDateTime(const QDateTime &startDateTime)
{
    setValue(FieldStartDateTime, startDateTime);
}

/*!
    Returns the todo time's due date and time as QDateTime.
    For all-day tasks, the time part is meaningless.
 */
QDateTime QOrganizerTodoTime::dueDateTime() const
{
    return value(FieldDueDateTime).toDateTime();
}

/*!
    Sets the todo time's due date and time to \a dueDateTime.
    For all-day tasks, the time part can be set to any valid value.
 */
void QOrganizerTodoTime::setDueDateTime(const QDateTime &dueDateTime)
{
    setValue(FieldDueDateTime, dueDateTime);
}

/*!
    Sets the all-day status of the TODO to \a isAllDay.
    If the tasks is an all-day TODO, no time is considered to be
    specified for the todo, even if the start date time set
    for the todo has a time component.
 */
void QOrganizerTodoTime::setAllDay(bool isAllDay)
{
    setValue(FieldAllDay, isAllDay);
}

/*!
    Returns true if the todo is an all-day TODO, or false otherwise.
 */
bool QOrganizerTodoTime::isAllDay() const
{
    return value(FieldAllDay).toBool();
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
   Sets the participation status of the user of the calendar in the event to \a status.
 */
void QOrganizerEventRsvp::setParticipationStatus(QOrganizerEventAttendee::ParticipationStatus status)
{
    setValue(FieldParticipationStatus, status);
}

/*!
   Returns the participation status of the user of the calendar in the event.
 */
QOrganizerEventAttendee::ParticipationStatus QOrganizerEventRsvp::participationStatus() const
{
    return static_cast<QOrganizerEventAttendee::ParticipationStatus>(value(FieldParticipationStatus).toInt());
}

/*!
   Sets the role of the user of the calendar in the event to \a role.
 */
void QOrganizerEventRsvp::setParticipationRole(QOrganizerEventAttendee::ParticipationRole role)
{
    setValue(FieldParticipationRole, role);
}

/*!
   Returns the participation role of the user of the calendar in the event.
 */
QOrganizerEventAttendee::ParticipationRole QOrganizerEventRsvp::participationRole() const
{
    return static_cast<QOrganizerEventAttendee::ParticipationRole>(value(FieldParticipationRole).toInt());
}

/*!
   \enum QOrganizerEventRsvp::ResponseRequirement
   \value ResponseNotRequired The organizer does not require the calendar user to respond to the invitation
   \value ResponseRequired The organizer requires the calendar user to respond to the invitation
*/

/*!
   Sets the response requirement for the invitation to \a responseRequirement.
 */
void QOrganizerEventRsvp::setResponseRequirement(ResponseRequirement responseRequirement)
{
    setValue(FieldResponseRequirement, responseRequirement);
}

/*!
   Returns the response requirement of the invitation.
 */
QOrganizerEventRsvp::ResponseRequirement QOrganizerEventRsvp::responseRequirement() const
{
    return static_cast<ResponseRequirement>(value(FieldResponseRequirement).toInt());
}

/*!
   Sets the date by which the user was requested to have responded to the invitation to the event to \a date.
 */
void QOrganizerEventRsvp::setResponseDeadline(const QDate &date)
{
    setValue(FieldResponseDeadline, date);
}

/*!
   Returns the date by which the user was requested to have responded to the invitation to the event.
 */
QDate QOrganizerEventRsvp::responseDeadline() const
{
    return value(FieldResponseDeadline).toDate();
}

/*!
   Sets the date at which the user responded to the invitation to the event to \a date.
 */
void QOrganizerEventRsvp::setResponseDate(const QDate &date)
{
    setValue(FieldResponseDate, date);
}

/*!
   Returns the date at which user responded to the invitation to the event.
 */
QDate QOrganizerEventRsvp::responseDate() const
{
    return value(FieldResponseDate).toDate();
}

/*!
   Sets the name of the organizer of the event (who sent the invitation) to \a name.
 */
void QOrganizerEventRsvp::setOrganizerName(const QString &name)
{
    setValue(FieldOrganizerName, name);
}

/*!
   Returns the name of the organizer of the event.
 */
QString QOrganizerEventRsvp::organizerName() const
{
    return value(FieldOrganizerName).toString();
}

/*!
   Sets the email address of the organizer of the event (who sent the invitation) to \a email.
 */
void QOrganizerEventRsvp::setOrganizerEmail(const QString &email)
{
    setValue(FieldOrganizerEmail, email);
}

/*!
   Returns the email address of the organizer of the event.
 */
QString QOrganizerEventRsvp::organizerEmail() const
{
    return value(FieldOrganizerEmail).toString();
}

/*!
    \class QOrganizerItemClassification
    \brief The QOrganizerItemClassification class is for defining the classification of an organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details

    This can be used as a part of security model for the organizer.
 */

/*!
    \enum QOrganizerItemClassification::ClassificationField

    This enumeration defines the fields supported by QOrganizerItemClassification.
    \value FieldClassification  The value stored describes the classification of an item.
 */

/*!
   \enum QOrganizerItemClassification::AccessClassification
   \value AccessPublic The item can be accessed by everybody
   \value AccessConfidential The access to the item is restricted
   \value AccessPrivate Only private access allowed for the item
*/

/*!
   Sets the classification of the item \a classification.
 */
void QOrganizerItemClassification::setClassification(AccessClassification classification)
{
    setValue(FieldClassification, classification);
}

/*!
   Returns classification of the item.
 */
QOrganizerItemClassification::AccessClassification QOrganizerItemClassification::classification() const
{
    return static_cast<AccessClassification>(value(FieldClassification).toInt());
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
    \enum QOrganizerItemExtendedDetail::ExtendedDetailField

    This enumeration defines the fields supported by QOrganizerItemExtendedDetail.
    \value FieldName  The value stored describes the name of this extended detail.
    \value FieldData  The value stored describes the data stored in this extended detail.
 */

/*!
    Sets the \a name of this extended detail.
 */
void QOrganizerItemExtendedDetail::setName(const QString &name)
{
    setValue(FieldName, name);
}

/*!
    Gets the name of this extended detail.
 */
QString QOrganizerItemExtendedDetail::name() const
{
    return value(FieldName).toString();
}

/*!
    Sets the \a data of the extended detail.
 */
void QOrganizerItemExtendedDetail::setData(const QVariant &data)
{
    setValue(FieldData, data);
}

/*!
    Gets the data of this extended detail.
 */
QVariant QOrganizerItemExtendedDetail::data() const
{
    return value(FieldData);
}

/*!
    \class QOrganizerItemVersion
    \brief The QOrganizerItemVersion class provides the versioning information of an organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-details
 */

/*!
    \enum QOrganizerItemVersion::VersionField

    This enumeration defines the fields supported by QOrganizerItemVersion.
    \value FieldVersion          The value stored describes the integer version of an organizer item.
                                 It can be used as the sequence number as per iCalendar spec.
    \value FieldExtendedVersion  The value stored describes the extended version of an organizer item.
                                 It can be used to represent the version stored in the back-end.
 */

/*!
    Sets the integer \a version. The \a version must be a positive number, otherwise ignored.
 */
void QOrganizerItemVersion::setVersion(int version)
{
    if (version > 0)
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

QT_END_NAMESPACE_ORGANIZER
