/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include "qcontactrelationshipremoverequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactRelationshipRemoveRequest

  \brief The QContactRelationshipRemoveRequest class allows a client
  to asynchronously request that certain relationships be removed from
  a contacts store.

  For a QContactRelationshipRemoveRequest, the resultsUpdated() signal will be emitted when
  the individual item errors (which may be retrieved by calling errorMap()) are updated, or if the overall
  operation error (which may be retrieved by calling error()) is updated.

  Please see the class documentation of QContactAbstractRequest for more information about
  the usage of request classes and ownership semantics.


  \inmodule QtContacts

  \ingroup contacts-requests
 */

/*! Constructs a new relationship remove request whose parent is the specified \a parent */
QContactRelationshipRemoveRequest::QContactRelationshipRemoveRequest(QObject* parent)
    : QContactAbstractRequest(new QContactRelationshipRemoveRequestPrivate, parent)
{
}


/*! Frees any memory used by this request */
QContactRelationshipRemoveRequest::~QContactRelationshipRemoveRequest()
{
}

/*!
  Sets the relationship which will be removed to \a relationship.
  Equivalent to calling:
  \code
      setRelationships(QList<QContactRelationship>() << relationship);
  \endcode
 */
void QContactRelationshipRemoveRequest::setRelationship(const QContactRelationship& relationship)
{
    Q_D(QContactRelationshipRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_relationships.clear();
    d->m_relationships.append(relationship);
}

/*! Sets the list of relationships which will be removed to \a relationships
*/
void QContactRelationshipRemoveRequest::setRelationships(const QList<QContactRelationship>& relationships)
{
    Q_D(QContactRelationshipRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_relationships = relationships;
}

/*! Returns the list of relationships which will be removed
*/
QList<QContactRelationship> QContactRelationshipRemoveRequest::relationships() const
{
    Q_D(const QContactRelationshipRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_relationships;
}

/*! Returns the map of input contact list indices to errors which occurred
*/
QMap<int, QContactManager::Error> QContactRelationshipRemoveRequest::errorMap() const
{
    Q_D(const QContactRelationshipRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qcontactrelationshipremoverequest.cpp"

QT_END_NAMESPACE_CONTACTS
