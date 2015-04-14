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

#include "qorganizerevent.h"

#include "qorganizereventtime.h"
#include "qorganizeritemlocation.h"
#include "qorganizeritemrecurrence.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerEvent
    \brief The QOrganizerEvent class provides an event in time which may reoccur
    \inmodule QtOrganizer
    \ingroup organizer-items

    A QOrganizerEvent is an item which occurs at a particular point in time
    and may be associated with a location or have other details.  It may have
    a set of recurrence rules or dates on which the event occurs associated
    with it, and also exceptions to those recurrences.
 */

/*!
    Sets the start date time of the event to \a startDateTime (for recurring events, this applies to
    the first instance).  For all-day events, the time part can be set to any valid value.
*/
void QOrganizerEvent::setStartDateTime(const QDateTime &startDateTime)
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    etr.setStartDateTime(startDateTime);
    saveDetail(&etr);
}

/*!
    Returns the date time at which the event starts (for recurring events, this applies to the first
    instance).  For all-day events, the time part is meaningless.
 */
QDateTime QOrganizerEvent::startDateTime() const
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    return etr.startDateTime();
}

/*!
    Sets the end date time of the event to \a endDateTime (for recurring events, this applies to the
    first instance).  For all-day events, the time part can be set to any valid value, and the date is
    to be interpreted as the inclusive end date.
 */
void QOrganizerEvent::setEndDateTime(const QDateTime &endDateTime)
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    etr.setEndDateTime(endDateTime);
    saveDetail(&etr);
}

/*!
    Returns the date time at which the event ends (for recurring events, this applies to the first
    instance).  For all-day events, the time part is meaningless, and the date is to be interpreted
    as the inclusive end date.
 */
QDateTime QOrganizerEvent::endDateTime() const
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    return etr.endDateTime();
}

/*!
    Sets whether the time-of-day component of the event's start date-time or end date-time is
    insignificant (eg. this is generally set to true for a birthday).  If \a isAllDay is true,
    the time-of-day component is considered insignificant, and the event will be an all-day
    item.
 */
void QOrganizerEvent::setAllDay(bool isAllDay)
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    etr.setAllDay(isAllDay);
    saveDetail(&etr);
}

/*!
    Returns true if and only if the time component of the start date/time or end date/time are
    insignificant.
*/
bool QOrganizerEvent::isAllDay() const
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    return etr.isAllDay();
}

/*!
    Sets the list of dates \a rdates to be dates on which the event occurs.
*/
void QOrganizerEvent::setRecurrenceDates(const QSet<QDate> &rdates)
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    rec.setRecurrenceDates(rdates);
    saveDetail(&rec);
}

/*!
    Sets a single date \a rdate to be the date on which the event occurs.
    \sa setRecurrenceDates()
*/
void QOrganizerEvent::setRecurrenceDate(const QDate &rdate)
{
    setRecurrenceDates(QSet<QDate>() << rdate);
}

/*!
    Returns the list of dates which have been explicitly set as dates on which the event occurs.
*/
QSet<QDate> QOrganizerEvent::recurrenceDates() const
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    return rec.recurrenceDates();
}

/*!
    Sets the list of recurrence rules \a rrules to be the rules which define when the event occurs,
    other than those dates specified explicitly via setRecurrenceDates().
*/
void QOrganizerEvent::setRecurrenceRules(const QSet<QOrganizerRecurrenceRule> &rrules)
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    rec.setRecurrenceRules(rrules);
    saveDetail(&rec);
}

/*!
    Sets a single recurrence rule \a rrule to be the rule which define when the event occurs, other
    than those dates specified explicitly via setRecurrenceDates().
    \sa setRecurrenceRules()
*/
void QOrganizerEvent::setRecurrenceRule(const QOrganizerRecurrenceRule &rrule)
{
    setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << rrule);
}

/*!
    Returns the list of recurrence rules which define when the event occurs.
*/
QSet<QOrganizerRecurrenceRule> QOrganizerEvent::recurrenceRules() const
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    return rec.recurrenceRules();
}

/*!
    Returns a recurrence rule which has been explicitly set for the event.
    Note: if more than one rule exists, the order of the rules is undefined, so any one could be returned.
 */
QOrganizerRecurrenceRule QOrganizerEvent::recurrenceRule() const
{
    QSet<QOrganizerRecurrenceRule> rrules = recurrenceRules();
    if (!rrules.isEmpty())
        return *rrules.begin();
    return QOrganizerRecurrenceRule();
}

/*!
    Sets the given list of dates \a exdates to be dates on which the event explicitly does not occur,
    even if the recurrence rules suggest that the event should occur on those dates.  Any previously
    specified exception dates will be cleared when this function is called.
 */
void QOrganizerEvent::setExceptionDates(const QSet<QDate> &exdates)
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    rec.setExceptionDates(exdates);
    saveDetail(&rec);
}

/*!
    Sets the given single date \a exdate to be the date on which the event explicitly does not occur,
    event if the recurrence rules suggest that the event should occur on this date.  Any previously
    specified exception dates will be cleared when this function is called.
    \sa setExceptionDates()
 */
void QOrganizerEvent::setExceptionDate(const QDate &exdate)
{
    setExceptionDates(QSet<QDate>() << exdate);
}

/*!
    Returns the list of dates on which the event explicitly does not occur despite
    the recurrence rules for the event
*/
QSet<QDate> QOrganizerEvent::exceptionDates() const
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    return rec.exceptionDates();
}

/*!
    Sets the given list of recurrence rules \a exrules to be the rules which define when
    the event does not occur.  Any previously specified exception rules will be cleared
    when this function is called.
*/
void QOrganizerEvent::setExceptionRules(const QSet<QOrganizerRecurrenceRule> &exrules)
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    rec.setExceptionRules(exrules);
    saveDetail(&rec);
}

/*!
    Sets the given single recurrence rules \a xrule to be the rule which defines when
    the event does not occur.  Any previously specified exception rules will be cleared
    when this function is called.
    \sa setExceptionRules()
 */
void QOrganizerEvent::setExceptionRule(const QOrganizerRecurrenceRule &xrule)
{
    setExceptionRules(QSet<QOrganizerRecurrenceRule>() << xrule);
}

/*!
    Returns an exception rule which has been explicitly set for the event.

    Note: if more than one exception rule exists, the order of the rules is undefined, so any one
    could be returned.
 */
QOrganizerRecurrenceRule QOrganizerEvent::exceptionRule() const
{
    QSet<QOrganizerRecurrenceRule> exrules = exceptionRules();
    if (!exrules.isEmpty())
        return *exrules.begin();
    return QOrganizerRecurrenceRule();
}

/*!
    Returns the list of exception rules for the event.
*/
QSet<QOrganizerRecurrenceRule> QOrganizerEvent::exceptionRules() const
{
    QOrganizerItemRecurrence rec = detail(QOrganizerItemDetail::TypeRecurrence);
    return rec.exceptionRules();
}

/*!
    Sets the priority of this event to \a priority.
*/
void QOrganizerEvent::setPriority(QOrganizerItemPriority::Priority priority)
{
    QOrganizerItemPriority pd = detail(QOrganizerItemDetail::TypePriority);
    pd.setPriority(priority);
    saveDetail(&pd);
}

/*!
    Returns the priority of the event.
*/
QOrganizerItemPriority::Priority QOrganizerEvent::priority() const
{
    QOrganizerItemPriority pd = detail(QOrganizerItemDetail::TypePriority);
    return pd.priority();
}

/*!
    Returns the label of the location at which the event occurs.
*/
QString QOrganizerEvent::location() const
{
    QOrganizerItemLocation ld = detail(QOrganizerItemDetail::TypeLocation);
    return ld.label();
}

/*!
    Sets the label of the location at which the event occurs to \a label.
*/
void QOrganizerEvent::setLocation(const QString &label)
{
    QOrganizerItemLocation ld = detail(QOrganizerItemDetail::TypeLocation);
    ld.setLabel(label);
    saveDetail(&ld);
}

QT_END_NAMESPACE_ORGANIZER
