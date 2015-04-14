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

#include "qorganizereventoccurrence.h"

#include <QtCore/qdatetime.h>

#include "qorganizereventtime.h"
#include "qorganizeritemlocation.h"
#include "qorganizeritemparent.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerEventOccurrence
    \brief The QOrganizerEventOccurrence class provides an occurrence of an event.
    \inmodule QtOrganizer
    \ingroup organizer-items

    An event occurrence is where the occurrence differs from the generating
    event in some way.  An occurrence which is retrieved from a manager may not
    actually be persisted in that manager (for example, it may be generated
    automatically from the recurrence rule of the parent event stored in the
    manager), in which case it will have a zero-id and differ from the parent
    event only in its start date. Alternatively, it may be persisted in the
    manager (that is, the client has saved the occurrence previously) where it
    is stored as an exception to its parent event.
 */

/*!
    Sets the start date time of the event occurrence to \a startDateTime.
    For all-day events, the time part is meaningless.
 */
void QOrganizerEventOccurrence::setStartDateTime(const QDateTime &startDateTime)
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    etr.setStartDateTime(startDateTime);
    saveDetail(&etr);
}

/*!
    Returns the date time at which the event occurrence begins.
    For all-day events, the time part can be set to any valid value.
 */
QDateTime QOrganizerEventOccurrence::startDateTime() const
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    return etr.startDateTime();
}

/*!
    Sets the end date time of the event occurrence to \a endDateTime.
    For all-day events, the time part can be set to any valid value, and the date is to be interpreted
    as the inclusive end date.
 */
void QOrganizerEventOccurrence::setEndDateTime(const QDateTime &endDateTime)
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    etr.setEndDateTime(endDateTime);
    saveDetail(&etr);
}

/*!
    Returns the date time at which the event occurrence ends.
    For all-day events, the time part is meaningless, and the date is to be interpreted
    as the inclusive end date.
 */
QDateTime QOrganizerEventOccurrence::endDateTime() const
{
    QOrganizerEventTime etr = detail(QOrganizerItemDetail::TypeEventTime);
    return etr.endDateTime();
}

/*!
    Sets the event occurrence's parent to be the event identified by the
    given \a parentId.
 */
void QOrganizerEventOccurrence::setParentId(const QOrganizerItemId &parentId)
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    origin.setParentId(parentId);
    saveDetail(&origin);
}

/*!
    Returns the id of the event which is this occurrence's parent.
 */
QOrganizerItemId QOrganizerEventOccurrence::parentId() const
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    return origin.parentId();
}

/*!
    Sets the date at which this occurrence was originally going to occur,
    to the given \a date.
 */
void QOrganizerEventOccurrence::setOriginalDate(const QDate &date)
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    origin.setOriginalDate(date);
    saveDetail(&origin);
}

/*!
    Returns the date at which the occurrence was originally going to occur.
 */
QDate QOrganizerEventOccurrence::originalDate() const
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    return origin.originalDate();
}

/*!
    Sets the priority of the event occurrence to \a priority.
 */
void QOrganizerEventOccurrence::setPriority(QOrganizerItemPriority::Priority priority)
{
    QOrganizerItemPriority pd = detail(QOrganizerItemDetail::TypePriority);
    pd.setPriority(priority);
    saveDetail(&pd);
}

/*!
    Returns the priority of the event occurrence.
 */
QOrganizerItemPriority::Priority QOrganizerEventOccurrence::priority() const
{
    QOrganizerItemPriority pd = detail(QOrganizerItemDetail::TypePriority);
    return pd.priority();
}

/*!
    Returns the label of the location at which the event occurrence is held.
 */
QString QOrganizerEventOccurrence::location() const
{
    QOrganizerItemLocation ld = detail(QOrganizerItemDetail::TypeLocation);
    return ld.label();
}

/*!
    Sets the label of the location at which the event occurrence is held to \a label.
 */
void QOrganizerEventOccurrence::setLocation(const QString &label)
{
    QOrganizerItemLocation ld = detail(QOrganizerItemDetail::TypeLocation);
    ld.setLabel(label);
    saveDetail(&ld);
}

QT_END_NAMESPACE_ORGANIZER
