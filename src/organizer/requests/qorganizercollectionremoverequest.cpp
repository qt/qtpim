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

#include "qorganizercollectionremoverequest.h"

#include "qorganizeritemrequests_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerCollectionRemoveRequest
    \brief The QOrganizerCollectionRemoveRequest class allows a client to asynchronously remove
           collections from a backend.
    \inmodule QtOrganizer
    \ingroup organizeritems-requests
 */

/*!
    Constructs a new collection remove request whose parent is the specified \a parent.
*/
QOrganizerCollectionRemoveRequest::QOrganizerCollectionRemoveRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerCollectionRemoveRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerCollectionRemoveRequest::~QOrganizerCollectionRemoveRequest()
{
}

/*!
    Sets the ID of collection which will be removed by this request to  \a collectionId.
*/
void QOrganizerCollectionRemoveRequest::setCollectionId(const QOrganizerCollectionId &collectionId)
{
    Q_D(QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collectionIds.clear();
    d->m_collectionIds.append(collectionId);
}

/*!
    Sets the list of IDs of collections which will be removed by this request to \a collectionIds.
*/
void QOrganizerCollectionRemoveRequest::setCollectionIds(const QList<QOrganizerCollectionId> &collectionIds)
{
    Q_D(QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collectionIds = collectionIds;
}

/*!
    Returns the list of IDs of collections which will be removed by this request.
*/
QList<QOrganizerCollectionId> QOrganizerCollectionRemoveRequest::collectionIds() const
{
    Q_D(const QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_collectionIds;
}

/*!
    Returns any errors which occurred during the request.
*/
QMap<int, QOrganizerManager::Error> QOrganizerCollectionRemoveRequest::errorMap() const
{
    Q_D(const QOrganizerCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qorganizercollectionremoverequest.cpp"

QT_END_NAMESPACE_ORGANIZER
