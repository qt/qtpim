/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QVERSITTIMEZONEHANDLER_H
#define QVERSITTIMEZONEHANDLER_H

#include <QtCore/qobject.h>
#include <QtCore/qdatetime.h>

#include <QtVersitOrganizer/qversitorganizerglobal.h>

QT_BEGIN_NAMESPACE_VERSITORGANIZER

class Q_VERSIT_ORGANIZER_EXPORT QVersitTimeZoneHandler
{
public:
    virtual ~QVersitTimeZoneHandler() {}

    /*!
      \fn QDateTime QVersitTimeZoneHandler::convertTimeZoneToUtc(const QDateTime& datetime, const QString& timeZoneName);
      Converts \a datetime to the local time for the time zone identified by \a timeZoneName.  The UTC
      offset of the time zone should be calculated taking into account the daylight savings rules that
      might apply at \a datetime.

      \a datetime must be specified with Qt::UTC.  The returned QDateTime will be specified with
      Qt::LocalTime.  Note that unlike the usual semantic of Qt::LocalTime, it does not represent a time
      in the system's local time zone.

      An invalid QDateTime is returned on failure (eg. if \a timeZoneName is unknown).
     */
    virtual QDateTime convertTimeZoneToUtc(const QDateTime& datetime, const QString& timeZoneName) = 0;

    /*!
      \fn QDateTime QVersitTimeZoneHandler::convertUtcToTimeZone(const QDateTime& datetime, const QString& timeZoneName);
      Converts \a datetime to UTC, treating \a datetime as if it were a local time for a time zone
      identified by \a timeZoneName.  The UTC offset of the time zone should be calculated taking into
      account the daylight savings rules that might apply at \a datetime.

      \a datetime must be specified with Qt::LocalTime.  Note that unlike the usual semantic of
      Qt::LocalTime, \a datetime does not represent a time in the system's local time zone.
      The returned QDateTime will be specified with Qt::UTC.

      An invalid QDateTime is returned on failure (eg. if \a timeZoneName is unknown).
     */
    virtual QDateTime convertUtcToTimeZone(const QDateTime& datetime, const QString& timeZoneName) = 0;
};

QT_END_NAMESPACE_VERSITORGANIZER

#define QT_VERSIT_TIMEZONE_HANDLER_INTERFACE "org.qt-project.Qt.QVersitTimeZoneHandler"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtVersitOrganizer::QVersitTimeZoneHandler, QT_VERSIT_TIMEZONE_HANDLER_INTERFACE)
QT_END_NAMESPACE
#endif
