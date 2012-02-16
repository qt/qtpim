/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactsortorder.h"
#include "qcontactsortorder_p.h"
#include "qcontactdetail.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QDebug>
#endif

QTCONTACTS_BEGIN_NAMESPACE

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
    /* We clear both when one is empty, so we only need to check one */
    if (d.constData()->m_type == QContactDetail::TypeUndefined)
        return false;
    return true;
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
 * \sa detailType(), detailField()
 */
void QContactSortOrder::setDetailType(QContactDetail::DetailType type, int field)
{
    if (type == QContactDetail::TypeUndefined || field == -1) {
        d->m_type = QContactDetail::TypeUndefined;
        d->m_field = -1;
    } else {
        d->m_type = type;
        d->m_field = field;
    }
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

QTCONTACTS_END_NAMESPACE
