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

#include "qcontactcollectionsaverequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
    \class QContactCollectionSaveRequest
    \brief The QContactCollectionSaveRequest class allows a client to asynchronously save collections
           to a backend.
    \inmodule QtContacts
    \ingroup contact-requests
 */

/*!
    Constructs a new collection save request whose parent is the specified \a parent.
*/
QContactCollectionSaveRequest::QContactCollectionSaveRequest(QObject *parent)
    : QContactAbstractRequest(new QContactCollectionSaveRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QContactCollectionSaveRequest::~QContactCollectionSaveRequest()
{
}

/*!
    Sets the collection which will be saved to \a collection.
*/
void QContactCollectionSaveRequest::setCollection(const QContactCollection &collection)
{
    Q_D(QContactCollectionSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collections.clear();
    d->m_collections.append(collection);
}

/*!
    Sets the list of collections which will be saved to \a collections.
*/
void QContactCollectionSaveRequest::setCollections(const QList<QContactCollection> &collections)
{
    Q_D(QContactCollectionSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_collections = collections;
}

/*!
    Returns the collections which will be saved by this request if called prior to calling start(),
    otherwise returns the (possibly updated) collections which have been saved.
*/
QList<QContactCollection> QContactCollectionSaveRequest::collections() const
{
    Q_D(const QContactCollectionSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_collections;
}

/*!
    Returns the map of input definition list indices to errors which occurred.
*/
QMap<int, QContactManager::Error> QContactCollectionSaveRequest::errorMap() const
{
    Q_D(const QContactCollectionSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

QT_END_NAMESPACE_CONTACTS

#include "moc_qcontactcollectionsaverequest.cpp"
