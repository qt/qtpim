/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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

#include "qorganizeritemdetailrangefilter.h"
#include "qorganizeritemdetailrangefilter_p.h"
#include "qorganizeritemfilter_p.h"
#include "qorganizermanager.h"

QTORGANIZER_BEGIN_NAMESPACE

/*!
  \class QOrganizerItemDetailRangeFilter
  \brief The QOrganizerItemDetailRangeFilter class provides a filter based around
  a detail value range criterion.
  \inmodule QtOrganizer
  \ingroup organizer-filters

  It may be used to select organizeritems which contain a detail of a particular definition with a particular value
 */

Q_IMPLEMENT_ORGANIZERITEMFILTER_PRIVATE(QOrganizerItemDetailRangeFilter);

/*!
 * \fn QOrganizerItemDetailRangeFilter::QOrganizerItemDetailRangeFilter(const QOrganizerItemFilter& other)
 * Constructs a copy of \a other if possible, otherwise constructs a new detail range filter
 */

/*!
 * \enum QOrganizerItemDetailRangeFilter::RangeFlag
 * Enumerates the semantics of the boundary conditions of the detail range filter
 * \value IncludeLower
 * \value IncludeUpper
 * \value ExcludeLower
 * \value ExcludeUpper
 */

/*!
 * Constructs a new detail range filter
 */
QOrganizerItemDetailRangeFilter::QOrganizerItemDetailRangeFilter()
    : QOrganizerItemFilter(new QOrganizerItemDetailRangeFilterPrivate)
{
}

/*!
 * Sets the value range criterion of the filter to within \a min and \a max, with boundary conditions specified in the given \a flags
 * \sa minValue(), maxValue()
 */
void QOrganizerItemDetailRangeFilter::setRange(const QVariant& min, const QVariant& max, RangeFlags flags)
{
    Q_D(QOrganizerItemDetailRangeFilter);
    d->m_minValue = min;
    d->m_maxValue = max;
    d->m_rangeflags = flags;
}

/*!
  Sets the match flags of the filter criterion to \a flags

  Not all flags are supported by a range filter.  The supported flags include:

  \list
  \o QOrganizerItemFilter::MatchExactly
  \o QOrganizerItemFilter::MatchFixedString
  \o QOrganizerItemFilter::MatchCaseSensitive
  \endlist

  Unsupported flags will be ignored.

  \sa matchFlags()
 */
void QOrganizerItemDetailRangeFilter::setMatchFlags(QOrganizerItemFilter::MatchFlags flags)
{
    Q_D(QOrganizerItemDetailRangeFilter);
    flags &= (QOrganizerItemFilter::MatchExactly | QOrganizerItemFilter::MatchFixedString | QOrganizerItemFilter::MatchCaseSensitive);
    d->m_flags = flags;
}

/*!
    Sets the type of detail which will be matched to \a detailType, and the field of the detail
    which will contain the value criterion to \a field.

    \sa detailType(), detailField()
 */
void QOrganizerItemDetailRangeFilter::setDetail(QOrganizerItemDetail::DetailType detailType, int field)
{
    Q_D(QOrganizerItemDetailRangeFilter);
    if (detailType != QOrganizerItemDetail::TypeUndefined && field >= 0) {
        d->m_detailType = detailType;
        d->m_detailField = field;
    } else {
        d->m_detailType = QOrganizerItemDetail::TypeUndefined;
        d->m_detailField = -1;
    }
}

/*!
    To be removed soon, use setDetail() instead.
 */
void QOrganizerItemDetailRangeFilter::setDetailDefinitionName(QOrganizerItemDetail::DetailType definitionName, int field)
{
    setDetail(definitionName, field);
}

/*!
 * Returns the match flags of the criterion, which define semantics such as case sensitivity, prefix matching, exact matching, etc.
 * \sa setMatchFlags()
 */
QOrganizerItemFilter::MatchFlags QOrganizerItemDetailRangeFilter::matchFlags() const
{
    Q_D(const QOrganizerItemDetailRangeFilter);
    return d->m_flags;
}

/*!
    Returns the type of the detail which will be inspected for matching values.

    \sa setDetail()
 */
QOrganizerItemDetail::DetailType QOrganizerItemDetailRangeFilter::detailType() const
{
    Q_D(const QOrganizerItemDetailRangeFilter);
    return d->m_detailType;
}

/*!
    This is to be removed soon, please use detailType() instead.
 */
QOrganizerItemDetail::DetailType QOrganizerItemDetailRangeFilter::detailDefinitionName() const
{
    return detailType();
}

/*!
    Returns the detail field containing the value which will be matched against the value criterion.

    \sa setDetail()
 */
int QOrganizerItemDetailRangeFilter::detailField() const
{
    Q_D(const QOrganizerItemDetailRangeFilter);
    return d->m_detailField;
}

/*!
    This is to be removed soon, please use detailField() instead.
 */
int QOrganizerItemDetailRangeFilter::detailFieldName() const
{
    return detailField();
}

/*!
 * Returns the lower bound of the value range criterion
 * \sa setRange()
 */
QVariant QOrganizerItemDetailRangeFilter::minValue() const
{
    Q_D(const QOrganizerItemDetailRangeFilter);
    return d->m_minValue;
}

/*!
 * Returns the upper bound of the value range criterion
 * \sa setRange()
 */
QVariant QOrganizerItemDetailRangeFilter::maxValue() const
{
    Q_D(const QOrganizerItemDetailRangeFilter);
    return d->m_maxValue;
}

/*!
 * Returns a set of flags which defines the boundary condition semantics of the value range criterion
 * \sa setRange()
 */
QOrganizerItemDetailRangeFilter::RangeFlags QOrganizerItemDetailRangeFilter::rangeFlags() const
{
    Q_D(const QOrganizerItemDetailRangeFilter);
    return d->m_rangeflags;
}

QTORGANIZER_END_NAMESPACE
