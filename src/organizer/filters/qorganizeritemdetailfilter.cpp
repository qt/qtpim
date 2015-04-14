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

#include "qorganizeritemdetailfilter.h"
#include "qorganizeritemdetailfilter_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemDetailFilter
    \brief The QOrganizerItemDetailFilter class provides a filter based around a detail value criterion.
    \inmodule QtOrganizer
    \ingroup organizer-filters

    It may be used to select organizeritems which contain a detail identical to that used by
    this filter for matching.
 */

Q_IMPLEMENT_ORGANIZERITEMFILTER_PRIVATE(QOrganizerItemDetailFilter)

/*!
    \fn QOrganizerItemDetailFilter::QOrganizerItemDetailFilter(const QOrganizerItemFilter &other)

    Constructs a copy of \a other if possible, otherwise constructs a new detail filter.
 */

/*!
    Constructs a new detail filter.
 */
QOrganizerItemDetailFilter::QOrganizerItemDetailFilter()
    : QOrganizerItemFilter(new QOrganizerItemDetailFilterPrivate)
{
}

/*!
    Sets the detail that will be matched to \a detail.

    \sa detail()
 */
void QOrganizerItemDetailFilter::setDetail(const QOrganizerItemDetail &detail)
{
    Q_D(QOrganizerItemDetailFilter);
    d->m_detail = detail;
}

/*!
    Returns the detail which will be matched by this filter.

    \sa setDetail()
 */
QOrganizerItemDetail QOrganizerItemDetailFilter::detail() const
{
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_detail;
}

QT_END_NAMESPACE_ORGANIZER
