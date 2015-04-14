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

#include "qorganizeritemfetchrequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemFetchRequest
    \brief The QOrganizerItemFetchRequest class allows a client to asynchronously fetch organizer
           items from a backend.
    \inmodule QtOrganizer
    \ingroup organizer-requests

    This request will fetch all the items and occurrences matching the specified criteria.
 */

/*!
    Constructs a new organizer item fetch request whose parent is the specified \a parent.
*/
QOrganizerItemFetchRequest::QOrganizerItemFetchRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemFetchRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerItemFetchRequest::~QOrganizerItemFetchRequest()
{
}

/*!
    Sets the organizer item filter used to determine which organizer items will be retrieved to \a filter.
*/
void QOrganizerItemFetchRequest::setFilter(const QOrganizerItemFilter &filter)
{
    Q_D(QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_filter = filter;
}

/*!
    Sets the sort order of the result to \a sorting.
*/
void QOrganizerItemFetchRequest::setSorting(const QList<QOrganizerItemSortOrder> &sorting)
{
    Q_D(QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_sorting = sorting;
}

/*!
    Sets the fetch hint which may be used by the backend to optimize item retrieval to \a fetchHint.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
void QOrganizerItemFetchRequest::setFetchHint(const QOrganizerItemFetchHint &fetchHint)
{
    Q_D(QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_fetchHint = fetchHint;
}

/*!
    Sets the start period of the request to \a date.

    A default-constructed (invalid) start date time specifies an open start date time (matches anything
    which occurs up until the end date time).
*/
void QOrganizerItemFetchRequest::setStartDate(const QDateTime &date)
{
    Q_D(QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_startDate = date;
}

/*!
    Sets the end period of the request to \a date.

    A default-constructed (invalid) end date time specifies an open end date time (matches anything
    which occurs after the start date time).
*/
void QOrganizerItemFetchRequest::setEndDate(const QDateTime &date)
{
    Q_D(QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_endDate = date;
}

/*!
    Sets the maximum number of items to be fetched to \a maxCount.

    Note that backends will decide how many items are fetched if \a maxCount is negative.
 */
void QOrganizerItemFetchRequest::setMaxCount(int maxCount)
{
    Q_D(QOrganizerItemFetchRequest);
    d->m_maxCount = maxCount;
}

/*!
    Returns the filter that will be used to select organizer items to be returned.
*/
QOrganizerItemFilter QOrganizerItemFetchRequest::filter() const
{
    Q_D(const QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_filter;
}

/*!
    Returns the sort ordering that will be used sort the results of this request.
*/
QList<QOrganizerItemSortOrder> QOrganizerItemFetchRequest::sorting() const
{
    Q_D(const QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_sorting;
}

/*!
    Returns the fetch hint which may be used by the backend to optimize item retrieval.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
QOrganizerItemFetchHint QOrganizerItemFetchRequest::fetchHint() const
{
    Q_D(const QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_fetchHint;
}

/*!
    Returns the date-time which is the lower bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemFetchRequest::startDate() const
{
    Q_D(const QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_startDate;
}

/*!
    Returns the date-time which is the upper bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemFetchRequest::endDate() const
{
    Q_D(const QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_endDate;
}

/*!
    Returns the maximum number of items to return for the request.
 */
int QOrganizerItemFetchRequest::maxCount() const
{
    Q_D(const QOrganizerItemFetchRequest);
    return d->m_maxCount;
}

/*!
    Returns the list of organizer items retrieved by this request.
*/
QList<QOrganizerItem> QOrganizerItemFetchRequest::items() const
{
    Q_D(const QOrganizerItemFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritems;
}

#include "moc_qorganizeritemfetchrequest.cpp"

QT_END_NAMESPACE_ORGANIZER
