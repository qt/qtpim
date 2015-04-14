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

#include "qcontactrelationshipsaverequest.h"

#include "qcontactrequests_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactRelationshipSaveRequest
  \brief The QContactRelationshipSaveRequest class allows a client to asynchronously
    request that certain groups be saved to a contacts store.

  For a QContactRelationshipSaveRequest, the resultsAvailable() signal will be emitted when
  either the individual item errors (which may be retrieved by calling errorMap()), or the resultant
  relationships (which may be retrieved by calling relationships()), are updated, as well as if
  the overall operation error (which may be retrieved by calling error()) is updated.

  Please see the class documentation of QContactAbstractRequest for more information about
  the usage of request classes and ownership semantics.


  \inmodule QtContacts

  \ingroup contacts-requests
 */

/*! Constructs a new relationship save request whose parent is the specified \a parent */
QContactRelationshipSaveRequest::QContactRelationshipSaveRequest(QObject* parent)
    : QContactAbstractRequest(new QContactRelationshipSaveRequestPrivate, parent)
{
}

/*! Frees any memory used by this request */
QContactRelationshipSaveRequest::~QContactRelationshipSaveRequest()
{
}

/*!
  Sets the relationship to save to be \a contactRelationship.
  Equivalent to calling:
  \code
      setRelationships(QList<QContactRelationship>() << contactRelationships);
  \endcode
 */
void QContactRelationshipSaveRequest::setRelationship(const QContactRelationship& contactRelationship)
{
    Q_D(QContactRelationshipSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_relationships.clear();
    d->m_relationships.append(contactRelationship);
}

/*! Sets the relationships to save to be \a contactRelationships
*/
void QContactRelationshipSaveRequest::setRelationships(const QList<QContactRelationship>& contactRelationships)
{
    Q_D(QContactRelationshipSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_relationships = contactRelationships;
}

/*! Returns the list of relationships that will be saved if called prior to calling \c start(),
    otherwise returns the list of relationships as they were saved in the contacts store
*/
QList<QContactRelationship> QContactRelationshipSaveRequest::relationships() const
{
    Q_D(const QContactRelationshipSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_relationships;
}

/*! Returns the map of input relationship list indices to errors which occurred
*/
QMap<int, QContactManager::Error> QContactRelationshipSaveRequest::errorMap() const
{
    Q_D(const QContactRelationshipSaveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qcontactrelationshipsaverequest.cpp"

QT_END_NAMESPACE_CONTACTS
