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

#include "qorganizerrecurrencerule.h"
#include "qorganizerrecurrencerule_p.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerRecurrenceRule
    \brief The QOrganizerRecurrenceRule class describes the a rule by which a QOrganizerItem repeats.
    \inmodule QtOrganizer
    \ingroup organizer-main

    This class is a mapping of a subset of the iCalendar RRULE property value, and each field in this
    class corresponds to a fragment of iCalendar's RRULE.  This class supports the same fragments as
    those supported by RRULE, except for describing recurrences on a higher frequency than Daily.
    That is, this class doesn't support hourly, minutely or secondly recurrences, nor does it support
    specifying which hour, minute or second of a day to recur on.  These types of rules are
    unsupported because most calendaring backends don't support them, and it simplifies recurrences
    by enforcing that there can be at most one occurrence of an item per day.

    The general rules for interaction between the fields when generating the occurence dates is as
    follows:
    \list
    \li When a criterion takes a list, the items in the list are unioned together.
        \list
        \li e.g. with [dayOfWeek == Tuesday,Thursday], the event occurs if it is Tuesday or Thursday.
        \endlist

    \li Frequency and specific criteria interact in a more complicated fashion.  For each criterion on a
        larger timespan than the frequency, the dates matching the criterion are intersected with the
        dates resulting from the frequency.
        \list
        \li e.g. [frequency = Daily, month = January] means every day in January. For each criterion
            on a shorter timespan than the frequency, the criterion is unioned.
        \li e.g. [frequency = Weekly, dayOfWeek = Wednesday,Friday] means every Wednesday and Friday of
            every week.
        \endlist
        This makes the frequency field superfluous in many cases when other criteria are present.
        e.g. all of the below mean the same thing:
        \list
        \li [frequency = Daily, dayOfWeek = Monday,Tuesday]
        \li [frequency = Weekly, dayOfWeek = Monday,Tuesday]
        \li [frequency = Monthly, dayOfWeek = Monday,Tuesday]
        \li [frequency = Yearly, dayOfWeek = Monday,Tuesday]
        \endlist
        However, the frequency field may start affecting the result differently when other fields are
        added like interval and positions.

    \li For the purpose of calculating occurrence dates, information not contained in the rule is in some
        cases derived from the startDateTime field of the event that the detail is associated with.
        There are three cases where such derivation is necessary.
        \list
        \li Case 1: frequency == Weekly.  If dayOfWeek is not specified, derive it from the week day that
            the startDateTime occurs on.
        \li Case 2: frequency == Monthly.  If neither dayOfWeek or dayOfMonth is specified, dayOfMonth should
            be derived from the startDateTime
        \li Case 3: frequency == Yearly.  If none of monthOfYear, weekOfYear, dayOfYear, dayOfMonth or dayOfWeek
            are specified, derive monthOfYear and dayOfMonth.  If monthOfYear is specified but not weekOfYear, dayOfYear,
            dayOfMonth or dayOfWeek, then derive dayOfMonth.  If weekOfYear is specified but not dayOfYear,
            dayOfWeek or dayOfMonth, derive dayOfWeek from the startDateTime.
            For any cases not covered here, do not derive any of the fields.
        \endlist
    \endlist

    A recurrence rule may be limited by either count or date, or it may be unlimited.
    If limited by count, the series generated by the rule will have at most \c count
    occurrences.  If limited by date, the series generated by the rule may have occurrences
    up to (and including) the limit \c date.  See \l setLimit() for more information on
    this topic.
 */

/*!
    \enum QOrganizerRecurrenceRule::Frequency

    This enumeration describes how often an item recurs.
    \value Invalid  The entire recurrence rule is invalid.
    \value Daily    The item recurs every day.
    \value Weekly   The item recurs every week.
    \value Monthly  The item recurs every month.
    \value Yearly   The item recurs every year.
 */

/*!
    \enum QOrganizerRecurrenceRule::Month

    This enumeration describes which month an item recurs on.
    \value January
    \value February
    \value March
    \value April
    \value May
    \value June
    \value July
    \value August
    \value September
    \value October
    \value November
    \value December
 */

/*!
    \enum QOrganizerRecurrenceRule::LimitType

    This enumeration describes the limitation of this recurrence rule.
    \value NoLimit     The recurrence rule has no limit specified.
    \value CountLimit  The recurrence rule specifies a certain count of repetitions in the series.
    \value DateLimit   The recurrence rule specifies that the series ends after a particular date.
 */

/*!
    Constructs a QOrganizerRecurrenceRule object describing a weekly recurrence.
 */
QOrganizerRecurrenceRule::QOrganizerRecurrenceRule()
    : d(new QOrganizerRecurrenceRulePrivate)
{
}

/*!
    Destroys the QOrganizerRecurrenceRule object.
 */
QOrganizerRecurrenceRule::~QOrganizerRecurrenceRule()
{
}

/*!
    Constructs a QOrganizerRecurrenceRule object as a copy of \a other.
 */
QOrganizerRecurrenceRule::QOrganizerRecurrenceRule(const QOrganizerRecurrenceRule &other)
    : d(other.d)
{
}

/*!
    Assigns this detail to be equal to \a other.
 */
QOrganizerRecurrenceRule &QOrganizerRecurrenceRule::operator=(const QOrganizerRecurrenceRule &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if this recurrence rule is equal to the \a other; otherwise returns false.

    \sa operator!=()
 */
bool QOrganizerRecurrenceRule::operator==(const QOrganizerRecurrenceRule &other) const
{
    if (d == other.d)
        return true;

    return d->firstDayOfWeek == other.d->firstDayOfWeek
            && d->frequency == other.d->frequency
            && d->interval == other.d->interval
            && d->limitCount == other.d->limitCount
            && d->limitDate == other.d->limitDate
            && d->limitType == other.d->limitType
            && d->positions == other.d->positions
            && d->daysOfMonth == other.d->daysOfMonth
            && d->daysOfWeek == other.d->daysOfWeek
            && d->daysOfYear == other.d->daysOfYear
            && d->monthsOfYear == other.d->monthsOfYear
            && d->weeksOfYear == other.d->weeksOfYear;
}

/*!
    \fn bool QOrganizerRecurrenceRule::operator!=(const QOrganizerRecurrenceRule &other) const

    Returns true if this recurrence rule is not equal to the \a other; otherwise returns false.

    \sa operator==()
*/

/*!
    Sets the frequency with which the item recurs to \a freq.

    This corresponds to the FREQ fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setFrequency(Frequency freq)
{
    d->frequency = freq;
}

/*!
    Returns the frequency with which the item recurs. The default frequency is Invalid.
 */
QOrganizerRecurrenceRule::Frequency QOrganizerRecurrenceRule::frequency() const
{
    return d->frequency;
}

/*!
    Sets the "count" condition of the recurrence rule to \a count. If an end-date was previously
    set, it is removed as count and endDate are mutually exclusive.

    The "count" condition is the maximum number of times the item should recur. Calling clearLimit()
    or setting this to a negative value removes the count condition.

    This corresponds to the COUNT fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setLimit(int count)
{
    if (count < 0) {
        clearLimit();
    } else {
        d->limitType = QOrganizerRecurrenceRule::CountLimit;
        d->limitCount = count;
        d->limitDate = QDate();
    }
}

/*!
    Sets the end-date condition of the recurrence rule to \a date. If a "count" condition was
    previously set, it is removed as count and endDate are mutually exclusive.

    The end-date condition is the date after which the item should not recur. Calling clearLimit()
    or setting this to an invalid date removes the end-date condition.

    This corresponds to the UNTIL fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setLimit(const QDate &date)
{
    if (!date.isValid()) {
        clearLimit();
    } else {
        d->limitType = QOrganizerRecurrenceRule::DateLimit;
        d->limitDate = date;
        d->limitCount = -1;
    }
}

/*!
    Clear any recurrence rule limitation conditions.
 */
void QOrganizerRecurrenceRule::clearLimit()
{
    d->limitType = QOrganizerRecurrenceRule::NoLimit;
    d->limitCount = -1;
    d->limitDate = QDate();
}

/*!
    Returns the type of limitation specified by the recurrence rule. The default limit type is NoLimit
    (i.e. unlimited).
 */
QOrganizerRecurrenceRule::LimitType QOrganizerRecurrenceRule::limitType() const
{
    return d->limitType;
}

/*!
    Returns the "count" condition specified by the recurrence rule. -1 is returned if the "count"
    condition is not set or an end-date condition is currently set.
 */
int QOrganizerRecurrenceRule::limitCount() const
{
    if (d->limitType == QOrganizerRecurrenceRule::CountLimit)
        return d->limitCount;
    return -1;
}

/*!
    Returns the end-date condition specified by the recurrence rule. An invalid date is returned if
    the end-date condition is not set or a "count" condition is currently set.
 */
QDate QOrganizerRecurrenceRule::limitDate() const
{
    if (d->limitType == QOrganizerRecurrenceRule::DateLimit)
        return d->limitDate;
    return QDate();
}

/*!
    Sets the interval, between cycles of length given by frequency(), in which the item should recur
    to \a interval.

    For example, if the frequency() is QOrganizerRecurrenceRule::Daily and the interval is set to 2,
    the item should recur every second day.

    This corresponds to the INTERVAL fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setInterval(int interval)
{
    if (interval > 0)
        d->interval = interval;
}

/*!
    Returns the interval of recurrence. The default interval is 1.
 */
int QOrganizerRecurrenceRule::interval() const
{
    return d->interval;
}

/*!
    Sets the days of week on which the item should recur to \a days.

    This corresponds to the BYDAY fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setDaysOfWeek(const QSet<Qt::DayOfWeek> &days)
{
    d->daysOfWeek = days;
}

/*!
    Returns a set of the days of week that the item should recur on, or an empty set if not specified.
 */
QSet<Qt::DayOfWeek> QOrganizerRecurrenceRule::daysOfWeek() const
{
    return d->daysOfWeek;
}

/*!
    Sets the days of the month on which the item should recur to \a days. Negative values in the set
    represent the number of days from the end of the month. e.g. 1 represents the first day of the
    month and -1 represents the last day of the month.

    This corresponds to the BYMONTHDAY fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setDaysOfMonth(const QSet<int> &days)
{
    d->daysOfMonth = days;
}

/*!
    Returns a set of the days of the month that the item should recur on, or an empty set if not specified.
 */
QSet<int> QOrganizerRecurrenceRule::daysOfMonth() const
{
    return d->daysOfMonth;
}

/*!
    Sets the days of the year on which the item should recur to \a days. Negative values in the set
    represent the number of days from the end of the year. e.g. 1 represents the first day of the
    year and -1 represents the last day of the year.

    This corresponds to the BYYEARDAY fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setDaysOfYear(const QSet<int> &days)
{
    d->daysOfYear = days;
}

/*!
    Returns a set of the days of the year that the item should recur on, or an empty set if not specified.
 */
QSet<int> QOrganizerRecurrenceRule::daysOfYear() const
{
    return d->daysOfYear;
}

/*!
    Sets the months on which the item should recur to \a months.

    This corresponds to the BYMONTHDAY fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setMonthsOfYear(const QSet<Month> &months)
{
    d->monthsOfYear = months;
}

/*!
    Returns a set of the months of the year that the item should recur on, or an empty set if not
    specified.
 */
QSet<QOrganizerRecurrenceRule::Month> QOrganizerRecurrenceRule::monthsOfYear() const
{
    return d->monthsOfYear;
}

/*!
    Sets the weeks of the year on which the item should recur to \a weeks. Negative values in the
    set represent the number of weeks from the end of the year. e.g. 1 represents the first week of
    the year and -1 represents the last week of the year.

    This corresponds to the BYWEEK fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setWeeksOfYear(const QSet<int> &weeks)
{
    d->weeksOfYear = weeks;
}

/*!
    Returns a set of the weeks of the year that the item should recur on, or an empty set if not
    specified.
 */
QSet<int> QOrganizerRecurrenceRule::weeksOfYear() const
{
    return d->weeksOfYear;
}

/*!
    Sets the set of positions that the item should recur on to \a pos. This specifies that the item
    should only recur on the nth occurrence within the set of events otherwise specified by the rule,
    for the values of n in \a pos.

    Negative values in the list represnet a position counting from the end of the set.

    For example, if frequency() == Monthly and months() is the list Monday, Tuesday, Wednesday,
    Thursday, Friday, and positions() == -1, this specifies that the item should recur on the last
    weekday of each month.

    This corresponds to the BYSETPOS fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setPositions(const QSet<int> &pos)
{
    d->positions = pos;
}

/*!
    Returns the position-set of the recurrence rule, or an empty set if not specified.
 */
QSet<int> QOrganizerRecurrenceRule::positions() const
{
    return d->positions;
}

/*!
    Sets the day that the week starts on to \a day, for the purposes of calculating recurrences.
    This is significant when the frequency is Weekly and the interval is greater than 1, or when
    weekOfYear is set.  See the iCalendar spec for examples of its significance.

    If not set, Monday is the first day of a week.

    This corresponds to the BYWKST fragment in iCalendar's RRULE.
 */
void QOrganizerRecurrenceRule::setFirstDayOfWeek(Qt::DayOfWeek day)
{
    d->firstDayOfWeek = day;
}

/*!
    Returns the day that the week starts on.
 */
Qt::DayOfWeek QOrganizerRecurrenceRule::firstDayOfWeek() const
{
    return d->firstDayOfWeek;
}

/*!
    \relates QOrganizerRecurrenceRule
    Returns the hash value for \a key.
 */
uint qHash(const QOrganizerRecurrenceRule &key)
{
    uint hash(0);
    static const unsigned int prime1 = 11;
    static const unsigned int prime2 = 31;
    static const unsigned int prime3 = 47;

    foreach (int day, key.daysOfMonth())
        hash += day;
    hash *= prime1;

    foreach (Qt::DayOfWeek day, key.daysOfWeek())
        hash += day;
    hash *= prime2;

    foreach (int day, key.daysOfYear())
        hash += day;
    hash *= prime3;

    foreach (QOrganizerRecurrenceRule::Month month, key.monthsOfYear())
        hash += month;
    hash *= prime1;

    foreach (int week, key.weeksOfYear())
        hash += week;
    hash *= prime2;

    foreach (int pos, key.positions())
        hash += pos;
    hash *= prime3;

    hash += static_cast<uint>(key.firstDayOfWeek())
            + static_cast<uint>(key.frequency())
            + key.interval()
            + key.limitCount()
            + qHash(key.limitDate())
            + static_cast<uint>(key.limitType());

    return hash * prime1;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerRecurrenceRule
    Outputs \a rule to the debug stream \a dbg.
 */
QDebug operator<<(QDebug dbg, const QOrganizerRecurrenceRule &rule)
{
    dbg.nospace() << "QOrganizerRecurrenceRule(frequency=";
    dbg.nospace() << rule.frequency();
    dbg.nospace() << ",";
    dbg.nospace() << "interval=";
    dbg.nospace() << rule.interval();
    dbg.nospace() << ",";
    switch (rule.limitType()) {
    case QOrganizerRecurrenceRule::CountLimit:
        dbg.nospace() << "limitCount=";
        dbg.nospace() << rule.limitCount();
        break;
    case QOrganizerRecurrenceRule::DateLimit:
        dbg.nospace() << "limitDate=";
        dbg.nospace() << rule.limitDate().toString();
        break;
    case QOrganizerRecurrenceRule::NoLimit:
        dbg.nospace() << "no limit";
        break;
    default:
        break;
    }
    dbg.nospace() << ",daysOfWeek=\"";
    QList<Qt::DayOfWeek> wdays(rule.daysOfWeek().toList());
    std::sort(wdays.begin(), wdays.end());
    foreach (Qt::DayOfWeek day, wdays) {
        dbg.nospace() << static_cast<quint32>(day);
        dbg.space();
    }
    dbg.nospace() << "\"";

    dbg.nospace() << ",daysOfMonth=\"";
    QList<int> mdays(rule.daysOfMonth().toList());
    std::sort(mdays.begin(), mdays.end());
    foreach (int day, mdays) {
        dbg.nospace() << day;
        dbg.space();
    }
    dbg.nospace() << "\"";

    dbg.nospace() << ",daysOfYear=\"";
    QList<int> ydays(rule.daysOfYear().toList());
    std::sort(ydays.begin(), ydays.end());
    foreach (int day, ydays) {
        dbg.nospace() << day;
        dbg.space();
    }
    dbg.nospace() << "\"";

    dbg.nospace() << ",monthsOfYear=\"";
    QList<QOrganizerRecurrenceRule::Month> months(rule.monthsOfYear().toList());
    std::sort(months.begin(), months.end());
    foreach (QOrganizerRecurrenceRule::Month month, months) {
        dbg.nospace() << static_cast<quint32>(month);
        dbg.space();
    }
    dbg.nospace() << "\"";

    dbg.nospace() << ",positions=\"";
    QList<int> positions(rule.positions().toList());
    std::sort(positions.begin(), positions.end());
    foreach (int position, positions) {
        dbg.nospace() << position;
        dbg.space();
    }
    dbg.nospace() << "\",";

    dbg.nospace() << "firstDayOfWeek=";
    dbg.nospace() << static_cast<quint32>(rule.firstDayOfWeek());
    dbg.nospace() << ')';
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

QT_END_NAMESPACE_ORGANIZER
