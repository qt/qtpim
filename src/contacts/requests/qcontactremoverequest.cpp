/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
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

#include "qcontactremoverequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactRemoveRequest
  \brief The QContactRemoveRequest class allows a client to asynchronously
    request that certain contacts be removed from a contacts store.

  For a QContactRemoveRequest, the resultsUpdated() signal will be emitted when
  the individual item errors (which may be retrieved by calling errorMap()) are updated, or if the overall
  operation error (which may be retrieved by calling error()) is updated.

  Please see the class documentation of QContactAbstractRequest for more information about
  the usage of request classes and ownership semantics.


  \inmodule QtContacts

  \ingroup contacts-requests
 */

/*! Constructs a new contact remove request whose parent is the specified \a parent */
QContactRemoveRequest::QContactRemoveRequest(QObject* parent)
    : QContactAbstractRequest(new QContactRemoveRequestPrivate, parent)
{
}

/*! Frees any memory used by this request */
QContactRemoveRequest::~QContactRemoveRequest()
{
}

/*!
  Sets the id of the contact which will be removed to \a contactId.
  Equivalent to calling:
  \code
      setContactIds(QList<QContactId>() << contactIds);
  \endcode
 */
void QContactRemoveRequest::setContactId(const QContactId& contactId)
{
    Q_D(QContactRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_contactIds.clear();
    d->m_contactIds.append(contactId);
}

/*! Sets the list of ids of contacts which will be removed to \a contactIds
*/
void QContactRemoveRequest::setContactIds(const QList<QContactId>& contactIds)
{
    Q_D(QContactRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_contactIds = contactIds;
}

/*! Returns the list of ids of contacts which will be removed
*/
QList<QContactId> QContactRemoveRequest::contactIds() const
{
    Q_D(const QContactRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_contactIds;
}

/*! Returns the map of input contact list indices to errors which occurred
*/
QMap<int, QContactManager::Error> QContactRemoveRequest::errorMap() const
{
    Q_D(const QContactRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qcontactremoverequest.cpp"

QT_END_NAMESPACE_CONTACTS
