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

#include "qorganizeritemremoverequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemRemoveRequest
    \brief The QOrganizerItemRemoveRequest class allows a client to asynchronously request that certain
           organizer items be removed from a backend.
    \inmodule QtOrganizer
    \ingroup organizer-requests

    This request will remove the items and all the occurrences (both generated and persisted) of the
    given items.
 */

/*!
    Constructs a new organizer item remove request whose parent is the specified \a parent.
*/
QOrganizerItemRemoveRequest::QOrganizerItemRemoveRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemRemoveRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerItemRemoveRequest::~QOrganizerItemRemoveRequest()
{
}

/*!
  Sets the organizer item which will be removed to \a item.
  Equivalent to calling:
  \code
      setOrganizerItems(QList<QOrganizerItem>() << item);
  \endcode
 */
void QOrganizerItemRemoveRequest::setItem(const QOrganizerItem &item)
{
    Q_D(QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_organizeritems.clear();
    d->m_organizeritems.append(item);
}

/*! Sets the organizer items which will be removed to \a items
*/
void QOrganizerItemRemoveRequest::setItems(const QList<QOrganizerItem> &items)
{
    Q_D(QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_organizeritems = items;
}

/*!
    Returns the list of IDs of organizer items which will be removed.
*/
QList<QOrganizerItem> QOrganizerItemRemoveRequest::items() const
{
    Q_D(const QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritems;
}

/*!
    Returns the map of input organizer item list indices to errors which occurred.
*/
QMap<int, QOrganizerManager::Error> QOrganizerItemRemoveRequest::errorMap() const
{
    Q_D(const QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qorganizeritemremoverequest.cpp"

QT_END_NAMESPACE_ORGANIZER
