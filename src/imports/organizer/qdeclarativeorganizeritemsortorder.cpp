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

#include "qdeclarativeorganizeritemsortorder_p.h"

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype SortOrder
    \instantiates QDeclarativeOrganizerItemSortOrder
    \brief The SortOrder element defines how a list of organizer item should be ordered according to some criteria.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
 */
QDeclarativeOrganizerItemSortOrder::QDeclarativeOrganizerItemSortOrder(QObject *parent)
    : QObject(parent)
{
}

/*!
  \qmlsignal SortOrder::onSortOrderChanged()

  This signal is emitted, when any of the SortOrder's properties have been changed.
 */

/*!
    \qmlproperty string SortOrder::detail

    This property holds the detail type of which the sorting will be performed to. The value should
    be the enumeration value of Detail::type.
 */
void QDeclarativeOrganizerItemSortOrder::setDetail(QDeclarativeOrganizerItemDetail::DetailType detail)
{
    if (detail != static_cast<QDeclarativeOrganizerItemDetail::DetailType>(d.detailType())) {
        d.setDetail(static_cast<QOrganizerItemDetail::DetailType>(detail), d.detailField());
        emit sortOrderChanged();
    }
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemSortOrder::detail() const
{
    return static_cast<QDeclarativeOrganizerItemDetail::DetailType>(d.detailType());
}

/*!
    \qmlproperty string SortOrder::field

    This property holds the detail field type of which the sorting will be performed to. The value
    should be the filld enumeration value defined in each detail element.

    \sa EventTime, JournalTime, TodoTime, TodoProgress, Reminder, AudibleReminder, VisualReminder,
        EmailReminder, Comment, Description, DisplayLabel, Guid, Location, Parent, Priority, Recurrence,
        Timestamp, ItemType, Tag
 */
void QDeclarativeOrganizerItemSortOrder::setField(int field)
{
    if (field != d.detailField()) {
        d.setDetail(d.detailType(), field);
        emit sortOrderChanged();
    }
}

int QDeclarativeOrganizerItemSortOrder::field() const
{
    return d.detailField();
}

/*!
    \qmlproperty enumeration SortOrder::blankPolicy

    This property enumerates the ways in which the sort order interprets blanks when sorting organizer.
    \list
    \li SortOrder.BlanksFirst  Considers blank values to evaluate to less than all other values in comparisons.
    \li SortOrder.BlanksLast   Considers blank values to evaluate to greater than all other values in comparisons.
    \endlist
 */
void QDeclarativeOrganizerItemSortOrder::setBlankPolicy(BlankPolicy policy)
{
    if (policy != blankPolicy()) {
        d.setBlankPolicy(static_cast<QOrganizerItemSortOrder::BlankPolicy>(policy));
        emit sortOrderChanged();
    }
}

QDeclarativeOrganizerItemSortOrder::BlankPolicy QDeclarativeOrganizerItemSortOrder::blankPolicy() const
{
    return static_cast<QDeclarativeOrganizerItemSortOrder::BlankPolicy>(d.blankPolicy());
}

/*!
    \qmlproperty enumeration SortOrder::direction

    This property holds the direction of the sort order, the value can be one of:
    \list
    \li Qt.AscendingOrder   The items will be sorted by the ascending order (default).
    \li Qt.DescendingOrder  The items will be sorted by the descending order.
    \endlist
 */
void QDeclarativeOrganizerItemSortOrder::setDirection(Qt::SortOrder newDirection)
{
    if (newDirection != direction()) {
        d.setDirection(newDirection);
        emit sortOrderChanged();
    }
}

Qt::SortOrder QDeclarativeOrganizerItemSortOrder::direction() const
{
    return d.direction();
}

/*!
    \qmlproperty enumeration SortOrder::caseSensitivity

    This property holds the case sensitivity of the sort order, the value can be one of:
    \list
    \li Qt.CaseInsensitive  Sets the case sensitivity of the sort order to insensitivity.
    \li Qt.CaseSensitive    Sets the case sensitivity of the sort order to sensitivity (default).
    \endlist
 */
void QDeclarativeOrganizerItemSortOrder::setCaseSensitivity(Qt::CaseSensitivity newSensitivity)
{
    if (newSensitivity != caseSensitivity()) {
        d.setCaseSensitivity(newSensitivity);
        emit sortOrderChanged();
    }
}

Qt::CaseSensitivity QDeclarativeOrganizerItemSortOrder::caseSensitivity() const
{
    return d.caseSensitivity();
}

/*!
    \internal
 */
QOrganizerItemSortOrder QDeclarativeOrganizerItemSortOrder::sortOrder()
{
    return d;
}

#include "moc_qdeclarativeorganizeritemsortorder_p.cpp"

QT_END_NAMESPACE
