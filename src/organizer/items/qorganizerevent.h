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

#ifndef QORGANIZEREVENT_H
#define QORGANIZEREVENT_H

#include <qorganizeritem.h>
#include <qorganizeritempriority.h>
#include <qorganizerrecurrencerule.h>

QTORGANIZER_BEGIN_NAMESPACE

/* Leaf class */

class Q_ORGANIZER_EXPORT QOrganizerEvent : public QOrganizerItem
{
public:
    Q_DECLARE_CUSTOM_ORGANIZER_ITEM(QOrganizerEvent, QOrganizerItemType::TypeEvent)

    void setStartDateTime(const QDateTime &startDateTime);
    QDateTime startDateTime() const;

    void setEndDateTime(const QDateTime &endDateTime);
    QDateTime endDateTime() const;

    void setAllDay(bool isAllDay);
    bool isAllDay() const;

    void setRecurrenceDate(const QDate &rdate);
    void setRecurrenceDates(const QSet<QDate> &rdates);
    QSet<QDate> recurrenceDates() const;

    void setRecurrenceRule(const QOrganizerRecurrenceRule &rrule);
    void setRecurrenceRules(const QSet<QOrganizerRecurrenceRule> &rrules);
    QOrganizerRecurrenceRule recurrenceRule() const;
    QSet<QOrganizerRecurrenceRule> recurrenceRules() const;

    void setExceptionDate(const QDate &exdate);
    void setExceptionDates(const QSet<QDate> &exdates);
    QSet<QDate> exceptionDates() const;

    void setExceptionRule(const QOrganizerRecurrenceRule &xrule);
    void setExceptionRules(const QSet<QOrganizerRecurrenceRule> &exrules);
    QOrganizerRecurrenceRule exceptionRule() const;
    QSet<QOrganizerRecurrenceRule> exceptionRules() const;

    void setPriority(QOrganizerItemPriority::Priority priority);
    QOrganizerItemPriority::Priority priority() const;

    QString location() const;
    void setLocation(const QString &label);
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZEREVENT_H
