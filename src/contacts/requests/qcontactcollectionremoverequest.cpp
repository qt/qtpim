/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Copyright (C) 2017 Canonical Ltd
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactcollectionremoverequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
    \class QContactCollectionRemoveRequest
    \brief The QContactCollectionRemoveRequest class allows a client to asynchronously remove
           collections from a backend.
    \inmodule QtContacts
    \ingroup contact-requests
 */

/*!
    Constructs a new collection remove request whose parent is the specified \a parent.
*/
QContactCollectionRemoveRequest::QContactCollectionRemoveRequest(QObject *parent)
    : QContactAbstractRequest(new QContactCollectionRemoveRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QContactCollectionRemoveRequest::~QContactCollectionRemoveRequest()
{
}

/*!
    Sets the ID of collection which will be removed by this request to  \a collectionId.
*/
void QContactCollectionRemoveRequest::setCollectionId(const QContactCollectionId &collectionId)
{
    Q_D(QContactCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collectionIds.clear();
    d->m_collectionIds.append(collectionId);
}

/*!
    Sets the list of IDs of collections which will be removed by this request to \a collectionIds.
*/
void QContactCollectionRemoveRequest::setCollectionIds(const QList<QContactCollectionId> &collectionIds)
{
    Q_D(QContactCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collectionIds = collectionIds;
}

/*!
    Returns the list of IDs of collections which will be removed by this request.
*/
QList<QContactCollectionId> QContactCollectionRemoveRequest::collectionIds() const
{
    Q_D(const QContactCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_collectionIds;
}

/*!
    Returns any errors which occurred during the request.
*/
QMap<int, QContactManager::Error> QContactCollectionRemoveRequest::errorMap() const
{
    Q_D(const QContactCollectionRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qcontactcollectionremoverequest.cpp"

QT_END_NAMESPACE_CONTACTS
