/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#include "qdeclarativecontactsortorder_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
   \qmltype SortOrder
    \instantiates QDeclarativeContactSortOrder
   \brief The SortOrder element defines how a list of contacts should be ordered according to some criteria.

   \ingroup qml-contacts-main
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactSortOrder
   \sa ContactModel
 */

QDeclarativeContactSortOrder::QDeclarativeContactSortOrder(QObject* parent)
    :QObject(parent)
{
}
/*!
  \qmlproperty enumeration SortOrder::detail

  This property holds the detail type of the details which will be inspected to perform sorting.

  \sa ContactDetail::type
  */
void QDeclarativeContactSortOrder::setDetail(const int detailType)
{
    if (detailType != static_cast<QDeclarativeContactDetail::DetailType>(m_sortOrder.detailType())) {
        m_sortOrder.setDetailType(static_cast<QContactDetail::DetailType>(detailType), m_sortOrder.detailField());
        emit sortOrderChanged();
    }
}

int QDeclarativeContactSortOrder::detail() const
{
    return static_cast<QDeclarativeContactDetail::DetailType>(m_sortOrder.detailType());
}

/*!
  \qmlproperty int SortOrder::field

  This property holds the detail field type of the details which will be inspected to perform sorting.
  For each detail elements, there are predefined field types.
  */
void QDeclarativeContactSortOrder::setField(const int fieldType)
{
    if (fieldType != m_sortOrder.detailField()) {
        m_sortOrder.setDetailType(m_sortOrder.detailType(), fieldType);
        emit sortOrderChanged();
    }
}

int QDeclarativeContactSortOrder::field() const
{
    return m_sortOrder.detailField();
}

/*!
  \qmlproperty enumeration SortOrder::blankPolicy
  This property enumerates the ways in which the sort order interprets blanks when sorting contacts.
  \list
  \li SortOrder.BlanksFirst - Considers blank values to evaluate to less than all other values in comparisons.
  \li SortOrder.BlanksLast - Considers blank values to evaluate to greater than all other values in comparisons.
  \endlist
 */
QDeclarativeContactSortOrder::BlankPolicy QDeclarativeContactSortOrder::blankPolicy() const
{
    return static_cast<QDeclarativeContactSortOrder::BlankPolicy>(m_sortOrder.blankPolicy());
}

void QDeclarativeContactSortOrder::setBlankPolicy(QDeclarativeContactSortOrder::BlankPolicy blankPolicy)
{
    if (blankPolicy != static_cast<QDeclarativeContactSortOrder::BlankPolicy>(m_sortOrder.blankPolicy())) {
        m_sortOrder.setBlankPolicy(static_cast<QContactSortOrder::BlankPolicy>(blankPolicy));
        emit sortOrderChanged();
    }
}
/*!
  \qmlproperty enumeration SortOrder::direction

  This property holds the direction of the sort order, the value can be one of:
  \list
  \li Qt.AscendingOrder - (default)
  \li Qt.DescendingOrder
  \endlist
  */
Qt::SortOrder QDeclarativeContactSortOrder::direction() const
{
    return m_sortOrder.direction();
}
void QDeclarativeContactSortOrder::setDirection(Qt::SortOrder direction)
{
    if (direction != m_sortOrder.direction()) {
        m_sortOrder.setDirection(direction);
        emit sortOrderChanged();
    }
}
/*!
  \qmlproperty enumeration SortOrder::caseSensitivity

  This property holds the case sensitivity of the sort order, the value can be one of:
  \list
  \li Qt.CaseInsensitive
  \li Qt.CaseSensitive - (default)
  \endlist
  */
Qt::CaseSensitivity QDeclarativeContactSortOrder::caseSensitivity() const
{
    return m_sortOrder.caseSensitivity();
}
void QDeclarativeContactSortOrder::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    if (sensitivity != m_sortOrder.caseSensitivity()) {
        m_sortOrder.setCaseSensitivity(sensitivity);
        emit sortOrderChanged();
    }
}

QContactSortOrder QDeclarativeContactSortOrder::sortOrder()
{
    return m_sortOrder;
}

void QDeclarativeContactSortOrder::setSortOrder(const QContactSortOrder& sortOrder)
{
    m_sortOrder = sortOrder;
    emit sortOrderChanged();
}

#include "moc_qdeclarativecontactsortorder_p.cpp"

QT_END_NAMESPACE
