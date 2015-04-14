/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2015 Canonical Ltd
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactcollectionfetchrequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
    \class QContactCollectionFetchRequest
    \brief The QContactCollectionFetchRequest class allows a client to asynchronously fetch collections
           from a backend.
    \inmodule QtContacts
    \ingroup contacts-requests

    This request will fetch all the collections stored in the given backend.
 */

/*!
    Constructs a new contact fetch request whose parent is the specified \a parent.
*/
QContactCollectionFetchRequest::QContactCollectionFetchRequest(QObject *parent)
    : QContactAbstractRequest(new QContactCollectionFetchRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QContactCollectionFetchRequest::~QContactCollectionFetchRequest()
{
}

/*!
    Returns the collections retrieved by this request.
*/
QList<QContactCollection> QContactCollectionFetchRequest::collections() const
{
    Q_D(const QContactCollectionFetchRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_collections;
}

#include "moc_qcontactcollectionfetchrequest.cpp"

QT_END_NAMESPACE_CONTACTS
