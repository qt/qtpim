/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qorganizeritemfetchbyidrequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemFetchByIdRequest
    \brief The QOrganizerItemFetchByIdRequest class allows a client to asynchronously
           fetch items from a backend, given a list of item IDs.
    \inmodule QtOrganizer
    \ingroup organizer-requests

    The items fetched by the backend should have a one-to-one correspondence to the IDs passed into
    this class.  That is, the nth item in the returned list should have an ID which is equal to the
    nth ID in the list of IDs.  Any invalid ID should correspond to an empty QOrganizerItem.
 */

/*!
    Constructs a new item fetch by ID request whose parent is the specified \a parent.
*/
QOrganizerItemFetchByIdRequest::QOrganizerItemFetchByIdRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemFetchByIdRequestPrivate, parent)
{
}

/*!
    Frees any memory used by this request.
*/
QOrganizerItemFetchByIdRequest::~QOrganizerItemFetchByIdRequest()
{
}

/*!
    Sets the list of IDs of the items that the backend should retrieve to \a ids.
 */
void QOrganizerItemFetchByIdRequest::setIds(const QList<QOrganizerItemId> &ids)
{
    Q_D(QOrganizerItemFetchByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_ids = ids;
}

/*!
    Sets the fetch hint which may be used by the backend to optimize item retrieval to \a fetchHint.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
void QOrganizerItemFetchByIdRequest::setFetchHint(const QOrganizerItemFetchHint &fetchHint)
{
    Q_D(QOrganizerItemFetchByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_fetchHint = fetchHint;
}

/*!
    Returns the list of IDs of the items that the backend should retrieve.
 */
QList<QOrganizerItemId> QOrganizerItemFetchByIdRequest::ids() const
{
    Q_D(const QOrganizerItemFetchByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_ids;
}

/*!
    Returns the fetch hint which may be used by the backend to optimize item retrieval.

    A client should not make changes to a item which has been retrieved using a fetch hint other than
    the default fetch hint.  Doing so will result in information loss when saving the item back to
    the manager (as the "new" restricted item will replace the previously saved item in the backend).
 */
QOrganizerItemFetchHint QOrganizerItemFetchByIdRequest::fetchHint() const
{
    Q_D(const QOrganizerItemFetchByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_fetchHint;
}

/*!
    Returns the list of items retrieved by this request.
*/
QList<QOrganizerItem> QOrganizerItemFetchByIdRequest::items() const
{
    Q_D(const QOrganizerItemFetchByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_items;
}

/*!
    Returns the map of input definition list indices to errors which occurred.
*/
QMap<int, QOrganizerManager::Error> QOrganizerItemFetchByIdRequest::errorMap() const
{
    Q_D(const QOrganizerItemFetchByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qorganizeritemfetchbyidrequest.cpp"

QT_END_NAMESPACE_ORGANIZER
