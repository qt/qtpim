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

#include "qorganizeritemremovebyidrequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
   \class QOrganizerItemRemoveByIdRequest
   \brief The QOrganizerItemRemoveByIdRequest class allows a client to asynchronously
   request that certain organizer items be removed from a organizer items store.
   \inmodule QtOrganizer

   For a QOrganizerItemRemoveByIdRequest, the resultsUpdated() signal will be emitted when
   the individual item errors (which may be retrieved by calling errorMap()) are updated, or if the overall
   operation error (which may be retrieved by calling error()) is updated.

   \ingroup organizer-requests
 */

/*!
    Constructs a new organizer item remove request whose parent is the specified \a parent
 */
QOrganizerItemRemoveByIdRequest::QOrganizerItemRemoveByIdRequest(QObject* parent)
    : QOrganizerAbstractRequest(new QOrganizerItemRemoveByIdRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request
*/
QOrganizerItemRemoveByIdRequest::~QOrganizerItemRemoveByIdRequest()
{
}

/*!
   Sets the id of the organizer item which will be removed to \a organizeritemId.
   Equivalent to calling:
   \code
       setOrganizerItemIds(QList<QOrganizerItemId>() << organizeritemIds);
   \endcode
 */
void QOrganizerItemRemoveByIdRequest::setItemId(const QOrganizerItemId& organizeritemId)
{
    Q_D(QOrganizerItemRemoveByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_organizeritemIds.clear();
    d->m_organizeritemIds.append(organizeritemId);
}

/*!
    Sets the list of ids of organizer items which will be removed to \a organizeritemIds
 */
void QOrganizerItemRemoveByIdRequest::setItemIds(const QList<QOrganizerItemId>& organizeritemIds)
{
    Q_D(QOrganizerItemRemoveByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_organizeritemIds = organizeritemIds;
}

/*!
    Returns the list of ids of organizer items which will be removed
 */
QList<QOrganizerItemId> QOrganizerItemRemoveByIdRequest::itemIds() const
{
    Q_D(const QOrganizerItemRemoveByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritemIds;
}

/*!
    Returns the map of input organizer item list indices to errors which occurred
 */
QMap<int, QOrganizerManager::Error> QOrganizerItemRemoveByIdRequest::errorMap() const
{
    Q_D(const QOrganizerItemRemoveByIdRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qorganizeritemremovebyidrequest.cpp"

QT_END_NAMESPACE_ORGANIZER
