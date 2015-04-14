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
