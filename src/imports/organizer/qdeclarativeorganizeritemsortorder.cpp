/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativeorganizeritemsortorder_p.h"

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass SortOrder QDeclarativeOrganizerItemSortOrder
    \brief The SortOrder element defines how a list of organizer item should be ordered according to some criteria.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
 */
QDeclarativeOrganizerItemSortOrder::QDeclarativeOrganizerItemSortOrder(QObject *parent)
    : QObject(parent)
{
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemSortOrder::classBegin()
{
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemSortOrder::componentComplete()
{
     setSortOrder(sortOrder());
}

/*!
    \qmlproperty string SortOrder::detail

    This property holds the detail definition name of the details which will be inspected to perform sorting.
 */
void QDeclarativeOrganizerItemSortOrder::setDetail(const QVariant &detail)
{
    if (m_detail != detail) {
        m_detail = detail;
        emit sortOrderChanged();
    }
}

QVariant QDeclarativeOrganizerItemSortOrder::detail() const
{
    return m_detail;
}

/*!
    \qmlproperty string SortOrder::field

    This property holds the detail field name of the details which will be inspected to perform sorting.
    For each detail elements, there are predefined field types.
 */
void QDeclarativeOrganizerItemSortOrder::setField(const QVariant &field)
{
    if (m_field !=field) {
        m_field = field;
        emit sortOrderChanged();
    }
}

QVariant QDeclarativeOrganizerItemSortOrder::field() const
{
    return m_field;
}

/*!
    \qmlproperty enumeration SortOrder::blankPolicy

    This property enumerates the ways in which the sort order interprets blanks when sorting organizer.
    \list
    \o SortOrder.BlanksFirst  Considers blank values to evaluate to less than all other values in comparisons.
    \o SortOrder.BlanksLast   Considers blank values to evaluate to greater than all other values in comparisons.
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
    \o Qt.AscendingOrder   The items will be sorted by the ascending order (default).
    \o Qt.DescendingOrder  The items will be sorted by the descending order.
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
    \o Qt.CaseInsensitive  Sets the case sensitivity of the sort order to insensitivity.
    \o Qt.CaseSensitive    Sets the case sensitivity of the sort order to sensitivity (default).
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
    QString ddn;
    if (m_detail.type() != QVariant::String) {
        ddn = QDeclarativeOrganizerItemDetail::definitionName(static_cast<QDeclarativeOrganizerItemDetail::ItemDetailType>(m_detail.toInt()));
    } else {
        ddn = m_detail.toString();
    }

    QString dfn;
    if (m_field.type() != QVariant::String) {
       QDeclarativeOrganizerItemDetail::ItemDetailType dt = QDeclarativeOrganizerItemDetail::detailTypeByDefinitionName(ddn);
       dfn = QDeclarativeOrganizerItemDetail::fieldName(dt, m_field.toInt());
    } else {
        dfn = m_field.toString();
    }

    d.setDetailDefinitionName(ddn, dfn);
    m_detail = ddn;
    m_field = dfn;
    return d;
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemSortOrder::setSortOrder(const QOrganizerItemSortOrder &sortOrder)
{
    d = sortOrder;
    m_field = d.detailFieldName();
    m_detail = d.detailDefinitionName();
    emit sortOrderChanged();
}


#include "moc_qdeclarativeorganizeritemsortorder_p.cpp"

QTORGANIZER_END_NAMESPACE
