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

QT_END_NAMESPACE_CONTACTS

#include "moc_qcontactcollectionfetchrequest.cpp"
