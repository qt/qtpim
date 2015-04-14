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

#include "qorganizeritemidfetchrequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemIdFetchRequest
    \brief The QOrganizerItemIdFetchRequest class allows a client to asynchronously fetch organizer
           item IDs from a backend.
    \inmodule QtOrganizer
    \ingroup organizer-requests
 */

/*!
    Constructs a new organizer item ID fetch request whose parent is the specified \a parent.
*/
QOrganizerItemIdFetchRequest::QOrganizerItemIdFetchRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemIdFetchRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerItemIdFetchRequest::~QOrganizerItemIdFetchRequest()
{
}

/*!
    Sets the filter which will be used to select the organizer items whose IDs will be returned to \a filter.
*/
void QOrganizerItemIdFetchRequest::setFilter(const QOrganizerItemFilter &filter)
{
    Q_D(QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_filter = filter;
}

/*!
    Sets the future sort ordering of the result of the request to \a sorting.
*/
void QOrganizerItemIdFetchRequest::setSorting(const QList<QOrganizerItemSortOrder> &sorting)
{
    Q_D(QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_sorting = sorting;
}

/*!
    Sets the start period of the request to \a date.

    A default-constructed (invalid) start date time specifies an open start date time (matches anything
    which occurs up until the end date time).
*/
void QOrganizerItemIdFetchRequest::setStartDate(const QDateTime &date)
{
    Q_D(QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_startDate = date;
}

/*!
    Sets the end period of the request to \a date.

    A default-constructed (invalid) end date time specifies an open end date time (matches anything
    which occurs after the start date time).
*/
void QOrganizerItemIdFetchRequest::setEndDate(const QDateTime &date)
{
    Q_D(QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_endDate = date;
}

/*!
    Returns the filter which will be used to select the organizer items whose IDs will be returned.
*/
QOrganizerItemFilter QOrganizerItemIdFetchRequest::filter() const
{
    Q_D(const QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_filter;
}

/*!
    Returns the sort ordering which will be used to sort the result.
*/
QList<QOrganizerItemSortOrder> QOrganizerItemIdFetchRequest::sorting() const
{
    Q_D(const QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_sorting;
}

/*!
    Returns the date-time which is the lower bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemIdFetchRequest::startDate() const
{
    Q_D(const QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_startDate;
}

/*!
    Returns the date-time which is the upper bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemIdFetchRequest::endDate() const
{
    Q_D(const QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_endDate;
}

/*!
    Returns the list of IDs of organizer items retrieved by this request.
*/
QList<QOrganizerItemId> QOrganizerItemIdFetchRequest::itemIds() const
{
    Q_D(const QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_ids;
}

#include "moc_qorganizeritemidfetchrequest.cpp"

QT_END_NAMESPACE_ORGANIZER
