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

#include "qorganizeritemfetchforexportrequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemFetchForExportRequest
    \brief The QOrganizerItemFetchForExportRequest class allows a client to asynchronously fetch
           organizer items for export from a backend.
    \inmodule QtOrganizer
    \ingroup organizer-requests

    This request will only fetch parent items and persisted exceptions which match the specified
    criteria, and no generated occurrences will be fetched.
 */

/*!
    Constructs a new organizer item fetch for export request whose parent is the specified \a parent.
*/
QOrganizerItemFetchForExportRequest::QOrganizerItemFetchForExportRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemFetchForExportRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerItemFetchForExportRequest::~QOrganizerItemFetchForExportRequest()
{
}

/*!
    Sets the organizer item filter used to determine which organizer items will be retrieved to \a filter.
*/
void QOrganizerItemFetchForExportRequest::setFilter(const QOrganizerItemFilter &filter)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_filter = filter;
}

/*!
    Sets the sort order of the result to \a sorting.
*/
void QOrganizerItemFetchForExportRequest::setSorting(const QList<QOrganizerItemSortOrder> &sorting)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_sorting = sorting;
}

/*!
    Sets the fetch hint which may be used by the backend to optimize item retrieval to \a fetchHint.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
void QOrganizerItemFetchForExportRequest::setFetchHint(const QOrganizerItemFetchHint &fetchHint)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_fetchHint = fetchHint;
}

/*!
    Sets the start period of the request to \a date.

    A default-constructed (invalid) start date time specifies an open start date time (matches anything
    which occurs up until the end date time).
*/
void QOrganizerItemFetchForExportRequest::setStartDate(const QDateTime &date)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_startDate = date;
}

/*!
    Sets the end period of the request to \a date.

    A default-constructed (invalid) end date time specifies an open end date time (matches anything
    which occurs after the start date time).
*/
void QOrganizerItemFetchForExportRequest::setEndDate(const QDateTime &date)
{
    Q_D(QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_endDate = date;
}

/*!
    Returns the filter that will be used to select organizer items to be returned.
*/
QOrganizerItemFilter QOrganizerItemFetchForExportRequest::filter() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_filter;
}

/*!
    Returns the sort ordering that will be used to sort the results of this request.
*/
QList<QOrganizerItemSortOrder> QOrganizerItemFetchForExportRequest::sorting() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_sorting;
}

/*!
    Returns the fetch hint which may be used by the backend to optimize item retrieval.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
QOrganizerItemFetchHint QOrganizerItemFetchForExportRequest::fetchHint() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_fetchHint;
}

/*!
    Returns the date-time which is the lower bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemFetchForExportRequest::startDate() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_startDate;
}

/*!
    Returns the date-time which is the upper bound for the range in which items will be returned.
 */
QDateTime QOrganizerItemFetchForExportRequest::endDate() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_endDate;
}

/*!
    Returns the list of organizer items retrieved by this request.
*/
QList<QOrganizerItem> QOrganizerItemFetchForExportRequest::items() const
{
    Q_D(const QOrganizerItemFetchForExportRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritems;
}

#include "moc_qorganizeritemfetchforexportrequest.cpp"

QT_END_NAMESPACE_ORGANIZER
