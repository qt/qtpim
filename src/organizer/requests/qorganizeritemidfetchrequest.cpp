/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include <qorganizeritemidfetchrequest.h>
#include <private/qorganizeritemrequests_p.h>

QTORGANIZER_BEGIN_NAMESPACE

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
    QOrganizerAbstractRequestPrivate::notifyEngine(this);
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
    Sets storage locations where the items are fetched from. \a storageLocations is a flag,
    so you can define multiple locations in it.

    \sa QOrganizerAbstractRequest::StorageLocation
    \sa QOrganizerItemIdFetchRequest::storageLocations()
*/
void QOrganizerItemIdFetchRequest::setStorageLocations(QOrganizerAbstractRequest::StorageLocations storageLocations)
{
    Q_D(QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_storageLocations = storageLocations;
}

/*!
    Storage locations where the items are fetched from.

    \sa QOrganizerAbstractRequest::StorageLocation
    \sa QOrganizerItemIdFetchRequest::setStorageLocations()
*/
QOrganizerAbstractRequest::StorageLocations QOrganizerItemIdFetchRequest::storageLocations() const
{
    Q_D(const QOrganizerItemIdFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_storageLocations;
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

QTORGANIZER_END_NAMESPACE
