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

#include "qcontactdetailrangefilter.h"
#include "qcontactdetailrangefilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactDetailRangeFilter
  \brief The QContactDetailRangeFilter class provides a filter based around
  a detail value range criterion.

  \inmodule QtContacts

  \ingroup contacts-filters

  It may be used to select contacts which contain a detail of a particular type
  with a particular value that lies in a range (either open or closed).
 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactDetailRangeFilter);

/*!
 * \fn QContactDetailRangeFilter::QContactDetailRangeFilter(const QContactFilter& other)
 * Constructs a copy of \a other if possible, otherwise constructs a new detail range filter
 */

/*!
 * \enum QContactDetailRangeFilter::RangeFlag
 * Enumerates the semantics of the boundary conditions of the detail range filter
 * \value IncludeLower The filter will match if the field value is equal to or greater than the minimum value
 * \value IncludeUpper The filter will match if the field value is equal to or less than the maximum value
 * \value ExcludeLower The filter will match if the field value is greater than the minimum value (but not equal)
 * \value ExcludeUpper The filter will match if the field value is less than the maximum value (but not equal)
 */

/*!
 * Constructs a new detail range filter
 */
QContactDetailRangeFilter::QContactDetailRangeFilter()
    : QContactFilter(new QContactDetailRangeFilterPrivate)
{
}

/*!
 * Sets the value range criterion of the filter to within \a min and \a max, with boundary conditions specified in the given \a flags.
 * If \a min or \a max is a null variant, that condition will not be checked.  For example, to check for
 * fields with a value greater than 7, you would specify:
 * \code
 * filter.setRange(7, QVariant(), QContactDetailRangeFilter::ExcludeLower);
 * \endcode
 * \sa minValue(), maxValue()
 */
void QContactDetailRangeFilter::setRange(const QVariant& min, const QVariant& max, RangeFlags flags)
{
    Q_D(QContactDetailRangeFilter);
    d->m_minValue = min;
    d->m_maxValue = max;
    d->m_rangeflags = flags;
}

/*!
 * Sets the match flags of the filter criterion to \a flags
 *
 * Not all flags are supported by a range filter.  The supported flags include:
 *
 * \list
 * \li QContactFilter::MatchExactly
 * \li QContactFilter::MatchFixedString
 * \li QContactFilter::MatchCaseSensitive
 * \endlist
 *
 * Unsupported flags will be ignored.
 *
 * \sa matchFlags()
 */
void QContactDetailRangeFilter::setMatchFlags(QContactFilter::MatchFlags flags)
{
    Q_D(QContactDetailRangeFilter);
    flags &= (QContactFilter::MatchExactly | QContactFilter::MatchFixedString | QContactFilter::MatchCaseSensitive);
    d->m_flags = flags;
}

/*!
 * Sets the type of detail which will be matched to \a type, and the field of the detail
 * which will contain the value criterion to \a field.
 *
 * If \a type is QContactDetail::TypeUndefined, the detail filter will match no contacts. If \a field
 * is not specified, or equal to -1, the detail filter acts like a "detail exists" filter; if any
 * detail of the specified type is present in a contact, that contact will match the filter, regardless
 * of what values might be stored in that detail.
 *
 * \sa detailType(), detailField()
 */
void QContactDetailRangeFilter::setDetailType(QContactDetail::DetailType type, int field)
{
    Q_D(QContactDetailRangeFilter);
    d->m_typeId = type;
    d->m_fieldId = field;
}

/*!
 * Returns the match flags of the criterion, which define semantics such as case sensitivity, and exact matching.
 * \sa setMatchFlags()
 */
QContactFilter::MatchFlags QContactDetailRangeFilter::matchFlags() const
{
    Q_D(const QContactDetailRangeFilter);
    return d->m_flags;
}

/*!
 * Returns the type of the details which will be inspected for matching values
 * \sa setDetailType()
 */
QContactDetail::DetailType QContactDetailRangeFilter::detailType() const
{
    Q_D(const QContactDetailRangeFilter);
    return d->m_typeId;
}

/*!
 * Returns the detail field containinig the value which will be matched against the value criterion
 * \sa setDetailType()
 */
int QContactDetailRangeFilter::detailField() const
{
    Q_D(const QContactDetailRangeFilter);
    return d->m_fieldId;
}

/*!
 * Returns the lower bound of the value range criterion.
 * If this value is null, there is no lower bound.
 * If it is valid, the \l rangeFlags() determines whether this value is included in the valid values.
 * \sa setRange()
 */
QVariant QContactDetailRangeFilter::minValue() const
{
    Q_D(const QContactDetailRangeFilter);
    return d->m_minValue;
}

/*!
 * Returns the upper bound of the value range criterion
 * If this value is null, there is no upper bound.
 * If it is valid, the \l rangeFlags() determines whether this value is included in the valid values.
 * \sa setRange()
 */
QVariant QContactDetailRangeFilter::maxValue() const
{
    Q_D(const QContactDetailRangeFilter);
    return d->m_maxValue;
}

/*!
 * Returns a set of flags which defines the boundary condition semantics of the value range criterion
 * \sa setRange()
 */
QContactDetailRangeFilter::RangeFlags QContactDetailRangeFilter::rangeFlags() const
{
    Q_D(const QContactDetailRangeFilter);
    return d->m_rangeflags;
}

QT_END_NAMESPACE_CONTACTS
