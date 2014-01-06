/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qdeclarativecontactsortorder_p.h"
#include <QDebug>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
   \qmltype SortOrder
    \instantiates QDeclarativeContactSortOrder
   \brief The SortOrder element defines how a list of contacts should be ordered according to some criteria.

   \ingroup qml-contacts-main
   \inqmlmodule QtContacts 5.0

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
    if (m_detail != detailType) {
        m_detail = detailType;
        emit sortOrderChanged();
    }

}

int QDeclarativeContactSortOrder::detail() const
{
    return m_detail;
}
/*!
  \qmlproperty int SortOrder::field

  This property holds the detail field type of the details which will be inspected to perform sorting.
  For each detail elements, there are predefined field types.
  */
void QDeclarativeContactSortOrder::setField(const int fieldType)
{
    if (m_field != fieldType) {
        m_field = fieldType;
        emit sortOrderChanged();
    }
}

int QDeclarativeContactSortOrder::field() const
{
    return m_field;
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

void QDeclarativeContactSortOrder::componentComplete()
{
    setSortOrder(sortOrder());
}

QContactSortOrder QDeclarativeContactSortOrder::sortOrder()
{
    m_sortOrder.setDetailType(static_cast<QContactDetail::DetailType>(m_detail), m_field);
    return m_sortOrder;
}

void QDeclarativeContactSortOrder::setSortOrder(const QContactSortOrder& sortOrder)
{
    m_sortOrder = sortOrder;
    m_field = sortOrder.detailField();
    m_detail = sortOrder.detailType();
    emit sortOrderChanged();
}

#include "moc_qdeclarativecontactsortorder_p.cpp"

QT_END_NAMESPACE
