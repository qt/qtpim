/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include "qdeclarativeorganizerrecurrencerule_p.h"
#include <QtDeclarative/qdeclarativeinfo.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass RecurrenceRule QDeclarativeOrganizerRecurrenceRule
    \brief The RecurrenceRule element represents a rule by which a organizer item repeats.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main
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
    \o RecurrenceRule.Invalid - (default).
    \o RecurrenceRule.Daily
    \o RecurrenceRule.Weekly
    \o RecurrenceRule.Monthly
    \o RecurrenceRule.Yearly
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
    if (!value.isValid()) {
        if (m_rule.limitType() != QOrganizerRecurrenceRule::NoLimit) {
            m_rule.clearLimit();
            emit recurrenceRuleChanged();
        }
    } else if (value.type() == QVariant::Date || value.type() == QVariant::DateTime) {
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
    \o RecurrenceRule.January
    \o RecurrenceRule.February
    \o RecurrenceRule.March
    \o RecurrenceRule.April
    \o RecurrenceRule.May
    \o RecurrenceRule.June
    \o RecurrenceRule.July
    \o RecurrenceRule.August
    \o RecurrenceRule.September
    \o RecurrenceRule.October
    \o RecurrenceRule.November
    \o RecurrenceRule.December
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
    \o Qt.Monday
    \o Qt.Tuesday
    \o Qt.Wednesday
    \o Qt.Thursday
    \o Qt.Friday
    \o Qt.Saturday
    \o Qt.Sunday
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

QTORGANIZER_END_NAMESPACE
