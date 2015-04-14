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

#include "qtimezones_p.h"

#include <QtOrganizer/qorganizer.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSITORGANIZER

QOrganizerManager* TimeZone::getManager()
{
    // We use the memory engine to do time zone recurrence calculations
    static QOrganizerManager* manager(new QOrganizerManager(QString::fromLatin1("memory")));
    return manager;
}

QDateTime TimeZone::convert(const QDateTime& dateTime) const
{
    Q_ASSERT(isValid());
    QOrganizerManager* manager = getManager();
    int offset = 100000; // impossible value
    QDateTime latestPhase;
    foreach(const TimeZonePhase& phase, mPhases) {
        QOrganizerEvent event;
        event.setStartDateTime(phase.startDateTime());
        event.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << phase.recurrenceRule());
        event.setRecurrenceDates(phase.recurrenceDates());
        QList<QOrganizerItem> occurrences =
            manager->itemOccurrences(event, phase.startDateTime(), dateTime, 500);
        if (!occurrences.isEmpty()) {
            QDateTime phaseStart(static_cast<const QOrganizerEventOccurrence &>(occurrences.last()).startDateTime());
            if (phaseStart > latestPhase) {
                latestPhase = phaseStart;
                offset = phase.utcOffset();
            }
        }
        else {
            offset = phase.utcOffset();
        }
    }
    QDateTime retn(dateTime);
    retn.setTimeSpec(Qt::UTC);
    if (offset >= -86400 && offset <= 86400) // offset must be within -24hours to +24hours
        return retn.addSecs(-offset);
    else
        return retn;
}

QDateTime TimeZones::convert(const QDateTime& dateTime, const QString& tzid) const
{
    if (!mTimeZones.contains(tzid))
        return QDateTime();
    TimeZone tz = mTimeZones.value(tzid);
    if (!tz.isValid())
        return QDateTime();
    return tz.convert(dateTime);
}

QT_END_NAMESPACE_VERSITORGANIZER
