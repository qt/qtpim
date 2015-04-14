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

#include "qorganizeritemsortorder.h"
#include "qorganizeritemsortorder_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemSortOrder
    \brief The QOrganizerItemSortOrder class defines how a list of organizer items should be ordered according to some criteria.
    \inmodule QtOrganizer
    \ingroup organizer-filters
 */

/*!
    \enum QOrganizerItemSortOrder::BlankPolicy

    Enumerates the ways in which the sort order interprets blanks when sorting organizer items.
    \value BlanksFirst  Considers blank values to evaluate to less than all other values in comparisons.
    \value BlanksLast   Considers blank values to evaluate to greater than all other values in comparisons.
 */

/*!
    \fn QOrganizerItemSortOrder::operator QList<QOrganizerItemSortOrder>() const

    Constructs a new list of sort orders containing only the current sort order.
 */

/*!
    \fn QOrganizerItemSortOrder::operator!=(const QOrganizerItemSortOrder &other) const

    Returns true if this sort order is not identical to the \a other sort order.

    \sa operator==()
 */

/*!
    Constructs a new sort order
 */
QOrganizerItemSortOrder::QOrganizerItemSortOrder()
    : d(new QOrganizerItemSortOrderPrivate())
{
}

/*!
    Frees any memory in use by this sort order
 */
QOrganizerItemSortOrder::~QOrganizerItemSortOrder()
{
}

/*!
    Constructs a copy of the \a other sort order.
 */
QOrganizerItemSortOrder::QOrganizerItemSortOrder(const QOrganizerItemSortOrder &other)
    : d(other.d)
{
}

/*!
    Assigns this sort order to be equal to \a other.
 */
QOrganizerItemSortOrder &QOrganizerItemSortOrder::operator=(const QOrganizerItemSortOrder &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if the sort order is able to be used to sort a list of organizer items; otherwise, returns false.
 */
bool QOrganizerItemSortOrder::isValid() const
{
    return d->m_detailType != QOrganizerItemDetail::TypeUndefined;
}

/*!
    Returns true if this sort order is identical to the \a other sort order.

    \sa operator!=()
 */
bool QOrganizerItemSortOrder::operator ==(const QOrganizerItemSortOrder &other) const
{
    if (d == other.d)
        return true;

    return d->m_blankPolicy == other.d->m_blankPolicy
            && d->m_direction == other.d->m_direction
            && d->m_sensitivity == other.d->m_sensitivity
            && d->m_detailType == other.d->m_detailType
            && d->m_detailField == other.d->m_detailField;
}

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerItemSortOrder
    Writes \a sortOrder to the stream \a out.
 */
QDataStream &operator<<(QDataStream &out, const QOrganizerItemSortOrder &sortOrder)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QOrganizerItemSortOrder
    return out << formatVersion
               << sortOrder.detailType()
               << sortOrder.detailField()
               << static_cast<quint32>(sortOrder.blankPolicy())
               << static_cast<quint32>(sortOrder.direction())
               << static_cast<quint32>(sortOrder.caseSensitivity());
}

/*!
    \relates QOrganizerItemSortOrder
    Reads a sort order from stream \a in into \a sortOrder.
 */
QDataStream &operator>>(QDataStream &in, QOrganizerItemSortOrder &sortOrder)
{
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        quint32 detailType;
        int fieldName;
        quint32 blankPolicy;
        quint32 direction;
        quint32 caseSensitivity;
        in >> detailType >> fieldName >> blankPolicy >> direction >> caseSensitivity;
        sortOrder.setDetail(static_cast<QOrganizerItemDetail::DetailType>(detailType), fieldName);
        sortOrder.setBlankPolicy(static_cast<QOrganizerItemSortOrder::BlankPolicy>(blankPolicy));
        sortOrder.setDirection(static_cast<Qt::SortOrder>(direction));
        sortOrder.setCaseSensitivity(static_cast<Qt::CaseSensitivity>(caseSensitivity));
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerItemSortOrder
    Outputs \a sortOrder to the debug stream \a dbg.
 */
QDebug operator<<(QDebug dbg, const QOrganizerItemSortOrder &sortOrder)
{
    dbg.nospace() << "QOrganizerItemSortOrder(";
    dbg.nospace() << "detailType=";
    dbg.nospace() << sortOrder.detailType();
    dbg.nospace() << ",";
    dbg.nospace() << "detailField=";
    dbg.nospace() << sortOrder.detailField();
    dbg.nospace() << ",";
    dbg.nospace() << "blankPolicy=";
    dbg.nospace() << static_cast<quint32>(sortOrder.blankPolicy());
    dbg.nospace() << ",";
    dbg.nospace() << "direction=";
    dbg.nospace() << static_cast<quint32>(sortOrder.direction());
    dbg.nospace() << ",";
    dbg.nospace() << "caseSensitivity=";
    dbg.nospace() << static_cast<quint32>(sortOrder.caseSensitivity());
    dbg.nospace() << ")";
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

/*!
    Sets the type of detail which will be inspected for sorting to \a detailType, and the field of
    the detail to \a field.

    If \a field is not specified, or equal to -1, the organizer item with a detail of the specified type
    would appear before or after the organizer item that lacks a detail of the specified type,
    according to blankPolicy().

    \sa detailType(), detailField()
 */
void QOrganizerItemSortOrder::setDetail(QOrganizerItemDetail::DetailType detailType, int field)
{
    d->m_detailType = detailType;
    d->m_detailField = field;
}

/*!
    Sets the sort order's policy on blank values with respect to sorting to \a blankPolicy.

    \sa blankPolicy()
 */
void QOrganizerItemSortOrder::setBlankPolicy(BlankPolicy blankPolicy)
{
    d->m_blankPolicy = blankPolicy;
}

/*!
    Sets the sort order direction to \a direction.

    \sa direction()
 */
void QOrganizerItemSortOrder::setDirection(Qt::SortOrder direction)
{
    d->m_direction = direction;
}

/*!
    Returns the type of the detail which will be inspected to perform sorting.

    \sa setDetail()
 */
QOrganizerItemDetail::DetailType QOrganizerItemSortOrder::detailType() const
{
    return d->m_detailType;
}

/*!
    Returns the detail field which will be inspected to perform sorting.

    \sa setDetail()
 */
int QOrganizerItemSortOrder::detailField() const
{
    return d->m_detailField;
}

/*!
    Returns the blank policy of the sort order.

    \sa setBlankPolicy()
 */
QOrganizerItemSortOrder::BlankPolicy QOrganizerItemSortOrder::blankPolicy() const
{
    return d->m_blankPolicy;
}

/*!
    Returns the direction of the sort order.

    \sa setDirection()
 */
Qt::SortOrder QOrganizerItemSortOrder::direction() const
{
    return d->m_direction;
}

/*!
    Returns the case sensitivity of the sort order.

    \sa setCaseSensitivity()
 */
Qt::CaseSensitivity QOrganizerItemSortOrder::caseSensitivity() const
{
    return d->m_sensitivity;
}

/*!
    Sets the case sensitivity of the sort order to \a sensitivity.

    \sa caseSensitivity()
 */
void QOrganizerItemSortOrder::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    d->m_sensitivity = sensitivity;
}

QT_END_NAMESPACE_ORGANIZER
