/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactsortorder.h"
#include "qcontactsortorder_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactSortOrder
  \brief The QContactSortOrder class defines how a list of contacts should be ordered according to some criteria

  \inmodule QtContacts

 */

/*!
 * \enum QContactSortOrder::BlankPolicy
 * Enumerates the ways in which the sort order interprets blanks when sorting contacts
 * \value BlanksFirst Considers blank values to evaluate to less than all other values in comparisons
 * \value BlanksLast Considers blank values to evaluate to greater than all other values in comparisons
 */

/*!
 * \fn QContactSortOrder::operator QList<QContactSortOrder>() const
 * Constructs a new list of sort orders containing only the current sort order
 */

/*!
 * \fn QContactSortOrder::operator!=(const QContactSortOrder& other) const
 * Returns true if this sort order is not identical to the \a other sort order
 * \sa operator==()
 */

/*!
 * Constructs a new sort order
 */
QContactSortOrder::QContactSortOrder()
    : d(new QContactSortOrderPrivate())
{
}

/*!
 * Frees any memory in use by this sort order
 */
QContactSortOrder::~QContactSortOrder()
{
}

/*!
 * Constructs a copy of the \a other sort order
 */
QContactSortOrder::QContactSortOrder(const QContactSortOrder& other)
    : d(other.d)
{

}

/*!
 * Assigns this sort order to be equal to \a other
 */
QContactSortOrder& QContactSortOrder::operator=(const QContactSortOrder& other)
{
    if (this != &other) {
        d = other.d;
    }
    return *this;
}

/*!
 * Returns true if the sort order is able to be used to sort a list of contacts; otherwise, returns false
 */
bool QContactSortOrder::isValid() const
{
    return d->m_type != QContactDetail::TypeUndefined;
}

/*!
 * Returns true if this sort order is identical to the \a other sort order
 * \sa operator!=()
 */
bool QContactSortOrder::operator ==(const QContactSortOrder& other) const
{
    if (d.constData()->m_blankPolicy == other.d.constData()->m_blankPolicy &&
        d.constData()->m_direction == other.d.constData()->m_direction &&
        d.constData()->m_sensitivity == other.d.constData()->m_sensitivity &&
        d.constData()->m_type == other.d.constData()->m_type &&
        d.constData()->m_field == other.d.constData()->m_field)
        return true;
    return false;
}


#ifndef QT_NO_DATASTREAM
/*!
 * Writes \a sortOrder to the stream \a out.
 */
QDataStream& operator<<(QDataStream& out, const QContactSortOrder& sortOrder)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QContactSortOrder
    return out << formatVersion
               << static_cast<quint32>(sortOrder.detailType())
               << sortOrder.detailField()
               << static_cast<quint32>(sortOrder.blankPolicy())
               << static_cast<quint32>(sortOrder.direction())
               << static_cast<quint32>(sortOrder.caseSensitivity());
}

/*!
 * Reads a sort order from stream \a in into \a sortOrder.
 */
QDataStream& operator>>(QDataStream& in, QContactSortOrder& sortOrder)
{
    sortOrder = QContactSortOrder();
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        quint32 type;
        int field;
        quint32 blankPolicy;
        quint32 direction;
        quint32 caseSensitivity;
        in >> type >> field >> blankPolicy >> direction >> caseSensitivity;
        sortOrder.setDetailType(QContactDetail::DetailType(type), field);
        sortOrder.setBlankPolicy(QContactSortOrder::BlankPolicy(blankPolicy));
        sortOrder.setDirection(Qt::SortOrder(direction));
        sortOrder.setCaseSensitivity(Qt::CaseSensitivity(caseSensitivity));
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif

#ifndef QT_NO_DEBUG_STREAM
/*!
  Outputs \a sortOrder to the debug stream \a dbg
 */
QDebug operator<<(QDebug dbg, const QContactSortOrder& sortOrder)
{
    dbg.nospace() << "QContactSortOrder("
                  << "detailType=" << static_cast<quint32>(sortOrder.detailType()) << ","
                  << "detailField=" << sortOrder.detailField() << ","
                  << "blankPolicy=" << static_cast<quint32>(sortOrder.blankPolicy()) << ","
                  << "direction=" << static_cast<quint32>(sortOrder.direction()) << ","
                  << "caseSensitivity=" << static_cast<quint32>(sortOrder.caseSensitivity())
                  << ")";
    return dbg.maybeSpace();
}
#endif

/*!
 * Sets the type of the details which will be inspected to perform sorting to \a type
 * and the name of those details' fields which contains the value which contacts will be sorted by to \a field
 *
 * If \a field is not specified, or equal to -1, the contact with a detail of the specified type
 * would appear before or after the contact that lacks a detail of the specified type,
 * according to blankPolicy().
 *
 * \sa detailType(), detailField()
 */
void QContactSortOrder::setDetailType(QContactDetail::DetailType type, int field)
{
    d->m_type = type;
    d->m_field = field;
}

/*!
 * Sets the sort order's policy on blank values with respect to sorting to \a blankPolicy
 * \sa blankPolicy()
 */
void QContactSortOrder::setBlankPolicy(BlankPolicy blankPolicy)
{
    d->m_blankPolicy = blankPolicy;
}

/*!
 * Sets the sort order direction to \a direction
 * \sa direction()
 */
void QContactSortOrder::setDirection(Qt::SortOrder direction)
{
    d->m_direction = direction;
}

/*!
 * Returns the type of the details which will be inspected to perform sorting.
 * Note that if a contact has multiple details of the definition, the result of the sorting
 * is undefined.
 * \sa setDetailType()
 */
QContactDetail::DetailType QContactSortOrder::detailType() const
{
    return d.constData()->m_type;
}

/*!
 * Returns the detail field which the sorting order will be based on.
 * \sa setDetailType()
 */
int QContactSortOrder::detailField() const
{
    return d.constData()->m_field;
}


/*!
 * Returns the blank policy of the sort order
 * \sa setBlankPolicy()
 */
QContactSortOrder::BlankPolicy QContactSortOrder::blankPolicy() const
{
    return d.constData()->m_blankPolicy;
}

/*!
 * Returns the direction of the sort order
 * \sa setDirection()
 */
Qt::SortOrder QContactSortOrder::direction() const
{
    return d.constData()->m_direction;
}

/*!
 * Returns the case sensitivity of the sort order
 * \sa setCaseSensitivity()
 */
Qt::CaseSensitivity QContactSortOrder::caseSensitivity() const
{
    return d.constData()->m_sensitivity;
}

/*!
 * Sets the case sensitivity of the sort order to \a sensitivity
 * \sa caseSensitivity()
 */
void QContactSortOrder::setCaseSensitivity(Qt::CaseSensitivity sensitivity)
{
    d->m_sensitivity = sensitivity;
}

QT_END_NAMESPACE_CONTACTS
