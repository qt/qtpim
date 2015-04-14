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

#include "qorganizertodooccurrence.h"

#include <QtCore/qdatetime.h>

#include "qorganizeritemid.h"
#include "qorganizeritemparent.h"
#include "qorganizertodotime.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerTodoOccurrence
    \brief The QOrganizerTodoOccurrence class provides an occurrence of a task which should be completed
    \inmodule QtOrganizer
    \ingroup organizer-items

    A todo occurrence is a specific instance of a todo item.  An occurrence
    which is retrieved from a manager may not actually be persisted in that
    manager (for example, it may be generated automatically from the
    recurrence rule of the parent todo stored in the manager), in which case
    it will have a zero-id and differ from the parent todo only in its start
    date.  Alternatively, it may be persisted in the manager (that is, the
    client has saved the occurrence previously) where it is stored as an exception
    to its parent todo.
 */

/*!
    Sets the date time at which the task should be started to \a startDateTime.  For all-day tasks,
    the time part can be set to any valid value.
 */
void QOrganizerTodoOccurrence::setStartDateTime(const QDateTime &startDateTime)
{
    QOrganizerTodoTime ttr = detail(QOrganizerItemDetail::TypeTodoTime);
    ttr.setStartDateTime(startDateTime);
    saveDetail(&ttr);
}

/*!
    Returns the date time at which the task should be started.  For all-day tasks, the time part is
    meaningless.
 */
QDateTime QOrganizerTodoOccurrence::startDateTime() const
{
    QOrganizerTodoTime ttr = detail(QOrganizerItemDetail::TypeTodoTime);
    return ttr.startDateTime();
}

/*!
    Sets the date time by which the task should be completed to \a dueDateTime.  For all-day tasks,
    the time part can be set to any valid value.
 */
void QOrganizerTodoOccurrence::setDueDateTime(const QDateTime &dueDateTime)
{
    QOrganizerTodoTime ttr = detail(QOrganizerItemDetail::TypeTodoTime);
    ttr.setDueDateTime(dueDateTime);
    saveDetail(&ttr);
}

/*!
    Returns the date time by which the task should be completed.  For all-day tasks, the time part is
    meaningless.
 */
QDateTime QOrganizerTodoOccurrence::dueDateTime() const
{
    QOrganizerTodoTime ttr = detail(QOrganizerItemDetail::TypeTodoTime);
    return ttr.dueDateTime();
}

/*!
    Sets the todo occurrence's parent to be the todo identified by the
    given \a parentId.
 */
void QOrganizerTodoOccurrence::setParentId(const QOrganizerItemId &parentId)
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    origin.setParentId(parentId);
    saveDetail(&origin);
}

/*!
    Returns the id of the todo which is this occurrence's parent.
 */
QOrganizerItemId QOrganizerTodoOccurrence::parentId() const
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    return origin.parentId();
}

/*!
    Sets the date at which this occurrence was originally going to occur,
    to the given \a date.
 */
void QOrganizerTodoOccurrence::setOriginalDate(const QDate &date)
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    origin.setOriginalDate(date);
    saveDetail(&origin);
}

/*!
    Returns the date at which the occurrence was originally going to occur.
 */
QDate QOrganizerTodoOccurrence::originalDate() const
{
    QOrganizerItemParent origin = detail(QOrganizerItemDetail::TypeParent);
    return origin.originalDate();
}

/*!
    Sets the priority of the todo occurrence to \a priority.
 */
void QOrganizerTodoOccurrence::setPriority(QOrganizerItemPriority::Priority priority)
{
    QOrganizerItemPriority pd = detail(QOrganizerItemDetail::TypePriority);
    pd.setPriority(priority);
    saveDetail(&pd);
}

/*!
    Returns the priority of the todo occurrence..
 */
QOrganizerItemPriority::Priority QOrganizerTodoOccurrence::priority() const
{
    QOrganizerItemPriority pd = detail(QOrganizerItemDetail::TypePriority);
    return pd.priority();
}

/*!
    Sets the percentage of progress completed on the task described
    by the todo occurrence item to \a percentage. Note that the given
    \a percentage must be between 0 and 100, otherwise ignored.
 */
void QOrganizerTodoOccurrence::setProgressPercentage(int percentage)
{
    if (percentage >= 0 && percentage <= 100) {
        QOrganizerTodoProgress tp = detail(QOrganizerItemDetail::TypeTodoProgress);
        tp.setPercentageComplete(percentage);
        saveDetail(&tp);
    }
}

/*!
    Returns the percentage of progress completed on the task described
    by the todo occurrence.
 */
int QOrganizerTodoOccurrence::progressPercentage() const
{
    QOrganizerTodoProgress tp = detail(QOrganizerItemDetail::TypeTodoProgress);
    return tp.percentageComplete();
}

/*!
    Sets the progress status of the todo occurrence to \a status.
 */
void QOrganizerTodoOccurrence::setStatus(QOrganizerTodoProgress::Status status)
{
    QOrganizerTodoProgress tp = detail(QOrganizerItemDetail::TypeTodoProgress);
    tp.setStatus(status);
    saveDetail(&tp);
}

/*!
    Returns the progress status of the task described by the todo occurrence.
 */
QOrganizerTodoProgress::Status QOrganizerTodoOccurrence::status() const
{
    QOrganizerTodoProgress tp = detail(QOrganizerItemDetail::TypeTodoProgress);
    return tp.status();
}

/*!
    Sets the date and time at which the task described by the todo occurrence was completed to \a finishedDateTime.
 */
void QOrganizerTodoOccurrence::setFinishedDateTime(const QDateTime &finishedDateTime)
{
    QOrganizerTodoProgress tp = detail(QOrganizerItemDetail::TypeTodoProgress);
    tp.setFinishedDateTime(finishedDateTime);
    saveDetail(&tp);
}

/*!
    Returns the date and time at which the task described by the todo occurrence was completed.
 */
QDateTime QOrganizerTodoOccurrence::finishedDateTime() const
{
    QOrganizerTodoProgress tp = detail(QOrganizerItemDetail::TypeTodoProgress);
    return tp.finishedDateTime();
}

QT_END_NAMESPACE_ORGANIZER
