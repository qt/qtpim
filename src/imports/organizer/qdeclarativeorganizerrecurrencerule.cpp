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

#include "qdeclarativeorganizerrecurrencerule_p.h"

#include <QtQml/qqmlinfo.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype RecurrenceRule
    \instantiates QDeclarativeOrganizerRecurrenceRule
    \brief The RecurrenceRule element represents a rule by which a organizer item repeats.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main
*/

/*!
  \qmlsignal RecurrenceRule::onRecurrenceRuleChanged()

  This signal is emitted, when any of the RecurrenceRule's properties have been changed.
 */

/*!
    \internal
 */
QDeclarativeOrganizerRecurrenceRule::QDeclarativeOrganizerRecurrenceRule(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty enumeration RecurrenceRule::frequency

    This property holds the frequency with which the item recurs, the value can be one of:
    \list
    \li RecurrenceRule.Invalid - (default).
    \li RecurrenceRule.Daily
    \li RecurrenceRule.Weekly
    \li RecurrenceRule.Monthly
    \li RecurrenceRule.Yearly
    \endlist
 */
void QDeclarativeOrganizerRecurrenceRule::setFrequency(Frequency freq)
{
    if (freq != frequency()) {
        m_rule.setFrequency(static_cast<QOrganizerRecurrenceRule::Frequency>(freq));
        emit recurrenceRuleChanged();
    }
}

QDeclarativeOrganizerRecurrenceRule::Frequency QDeclarativeOrganizerRecurrenceRule::frequency() const
{
    return static_cast<Frequency>(m_rule.frequency());
}

/*!
    \qmlproperty variant RecurrenceRule::limit

    This property holds the limit condition of the recurrence rule, the value can be a limit date
    and time or a limit count. The default is no limit.
 */
void QDeclarativeOrganizerRecurrenceRule::setLimit(const QVariant &value)
{
    if (!value.isValid() ||
        (static_cast<QMetaType::Type>(value.type()) == QMetaType::VoidStar && value.value<void *>() == 0)) {
        if (m_rule.limitType() != QOrganizerRecurrenceRule::NoLimit) {
            m_rule.clearLimit();
            emit recurrenceRuleChanged();
        }
    } else if (value.type() == QVariant::DateTime) {
        QDate v = value.toDateTime().toUTC().date();
        if (v != m_rule.limitDate()) {
            m_rule.setLimit(v);
            emit recurrenceRuleChanged();
        }
    } else if (value.type() == QVariant::Date) {
        QDate v = value.value<QDate>();
        if (v != m_rule.limitDate()) {
            m_rule.setLimit(v);
            emit recurrenceRuleChanged();
        }
    }  else if ((value.type() == QVariant::Int) || (value.type() == QVariant::Double)) {
        int v = value.value<int>();
        if (v != m_rule.limitCount()) {
            m_rule.setLimit(v);
            emit recurrenceRuleChanged();
        }
    } else {
        // TODO throw an error event
        qmlInfo(this) << tr("Invalid recurrence rule limit; value ,") <<  value << tr(", did not match one of the types: date, integer or double");
    }
}

QVariant QDeclarativeOrganizerRecurrenceRule::limit() const
{
    if (m_rule.limitType() == QOrganizerRecurrenceRule::CountLimit)
        return QVariant::fromValue(m_rule.limitCount());
    else if (m_rule.limitType() == QOrganizerRecurrenceRule::DateLimit)
        return QVariant::fromValue(m_rule.limitDate());

    //NoLimit
    return QVariant();
}

/*!
    \qmlproperty int RecurrenceRule::interval

    This property holds the interval of recurrence.  The default interval is 1.
 */
void QDeclarativeOrganizerRecurrenceRule::setInterval(int interval)
{
    if (interval != m_rule.interval()) {
        m_rule.setInterval(interval);
        emit recurrenceRuleChanged();
    }
}

int QDeclarativeOrganizerRecurrenceRule::interval() const
{
    return m_rule.interval();
}

/*!
    \qmlproperty list<variant> RecurrenceRule::daysOfWeek

    This property holds a list of the days of week that the item should recur on.
 */
void QDeclarativeOrganizerRecurrenceRule::setDaysOfWeek(const QVariantList &days)
{
    QSet<Qt::DayOfWeek> saved;
    foreach (const QVariant &day, days)
        saved << static_cast<Qt::DayOfWeek>(day.value<int>());
    if (saved != m_rule.daysOfWeek()) {
        m_rule.setDaysOfWeek(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList QDeclarativeOrganizerRecurrenceRule::daysOfWeek() const
{
    QVariantList days;
    foreach (Qt::DayOfWeek day, m_rule.daysOfWeek())
        days << day;
    return days;
}

/*!
    \qmlproperty list<variant> RecurrenceRule::daysOfMonth

    This property holds a list of the days of the month that the item should recur on.
 */
void QDeclarativeOrganizerRecurrenceRule::setDaysOfMonth(const QVariantList &days)
{
    QSet<int> saved;
    foreach (const QVariant &day, days)
        saved << day.value<int>();
    if (saved != m_rule.daysOfMonth()) {
        m_rule.setDaysOfMonth(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList QDeclarativeOrganizerRecurrenceRule::daysOfMonth() const
{
    QVariantList days;
    foreach (int day, m_rule.daysOfMonth())
        days << day;
    return days;
}

/*!
    \qmlproperty list<variant> RecurrenceRule::daysOfYear

    This property holds a list of the days of the year that the item should recur on.
 */
void QDeclarativeOrganizerRecurrenceRule::setDaysOfYear(const QVariantList &days)
{
    QSet<int> saved;
    foreach (const QVariant &day, days)
        saved << day.value<int>();
    if (saved != m_rule.daysOfYear()) {
        m_rule.setDaysOfYear(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList QDeclarativeOrganizerRecurrenceRule::daysOfYear() const
{
    QVariantList days;
    foreach (int day, m_rule.daysOfYear())
        days << day;
    return days;
}

/*!
    \qmlproperty list<int> RecurrenceRule::monthsOfYear

    This property holds a list of the months that the item should recur on, the list element value can be one of:
    \list
    \li RecurrenceRule.January
    \li RecurrenceRule.February
    \li RecurrenceRule.March
    \li RecurrenceRule.April
    \li RecurrenceRule.May
    \li RecurrenceRule.June
    \li RecurrenceRule.July
    \li RecurrenceRule.August
    \li RecurrenceRule.September
    \li RecurrenceRule.October
    \li RecurrenceRule.November
    \li RecurrenceRule.December
    \endlist
 */
void QDeclarativeOrganizerRecurrenceRule::setMonthsOfYear(const QVariantList &months)
{
    QSet<QOrganizerRecurrenceRule::Month> saved;
    foreach (const QVariant &day, months)
        saved << static_cast<QOrganizerRecurrenceRule::Month>(day.value<int>());
    if (saved != m_rule.monthsOfYear()) {
        m_rule.setMonthsOfYear(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList QDeclarativeOrganizerRecurrenceRule::monthsOfYear() const
{
    QVariantList ms;
    foreach (int m, m_rule.monthsOfYear())
        ms << m;
    return ms;
}

/*!
  \qmlproperty list<int> RecurrenceRule::weeksOfYear

  This property holds a list of the weeks of the year that the item should recur on.
  */
void QDeclarativeOrganizerRecurrenceRule::setWeeksOfYear(const QVariantList &weeks)
{
    QSet<int> saved;
    foreach (const QVariant &week, weeks)
        saved << week.value<int>();
    if (saved != m_rule.weeksOfYear()) {
        m_rule.setWeeksOfYear(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList QDeclarativeOrganizerRecurrenceRule::weeksOfYear() const
{
    QVariantList weeks;
    foreach (int week, m_rule.weeksOfYear())
        weeks << week;
    return weeks;
}

/*!
    \qmlproperty list<int> RecurrenceRule::positions

    This property holds the position-list of the recurrence rule.
  */
void QDeclarativeOrganizerRecurrenceRule::setPositions(const QVariantList &pos)
{
    if (pos != positions()) {
        QSet<int> saved;
        foreach (const QVariant &p, pos)
            saved << p.value<int>();
        m_rule.setPositions(saved);
        emit recurrenceRuleChanged();
    }
}

QVariantList QDeclarativeOrganizerRecurrenceRule::positions() const
{
    QVariantList pos;
    foreach (int p, m_rule.positions())
        pos << p;
    return pos;
}

/*!
    \qmlproperty enumeration RecurrenceRule::firstDayOfWeek

    This property holds the day that the week starts on.  If not set, this is Monday. The value can be one of:
    \list
    \li Qt.Monday
    \li Qt.Tuesday
    \li Qt.Wednesday
    \li Qt.Thursday
    \li Qt.Friday
    \li Qt.Saturday
    \li Qt.Sunday
    \endlist
 */
void QDeclarativeOrganizerRecurrenceRule::setFirstDayOfWeek(Qt::DayOfWeek day)
{
    if (day != firstDayOfWeek()) {
        m_rule.setFirstDayOfWeek(day);
        emit recurrenceRuleChanged();
    }
}

Qt::DayOfWeek QDeclarativeOrganizerRecurrenceRule::firstDayOfWeek() const
{
    return m_rule.firstDayOfWeek();
}

/*!
    \internal
 */
QOrganizerRecurrenceRule QDeclarativeOrganizerRecurrenceRule::rule() const
{
    return m_rule;
}

/*!
    \internal
 */
void QDeclarativeOrganizerRecurrenceRule::setRule(const QOrganizerRecurrenceRule &rule)
{
    m_rule = rule;
}

#include "moc_qdeclarativeorganizerrecurrencerule_p.cpp"

QT_END_NAMESPACE
