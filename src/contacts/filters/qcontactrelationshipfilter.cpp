/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactrelationshipfilter.h"
#include "qcontactrelationshipfilter_p.h"
#include "qcontactmanager.h"

QTPIM_BEGIN_NAMESPACE

/*!
  \class QContactRelationshipFilter
  \brief The QContactRelationshipFilter class provides a filter based
  around relationship criteria.


  \inmodule QtContacts
   \since 1.0

  \ingroup contacts-filters

  It may be used to select contacts which are involved in relationships
  which are of a certain type, or which involve certain contacts.

  One common use-case might be to select the contacts which are a member of a particular group.
  This use-case may be met with the following filter:

  \code
  QContactRelationshipFilter groupFilter;                               // select all contacts which are involved
  groupFilter.setRelationshipType(QContactRelationship::HasMember);     // in a group relationship
  groupFilter.setRelatedContactId(groupContact.id());                   // with the group contact
  groupFilter.setRelatedContactRole(QContactRelationship::First); // where the group contact is the first participant
  \endcode

  Another common use-case might be to select the groups which a particular contact is a member of.
  This use-case may be met with the following filter:

  \code
  QContactRelationshipFilter whichGroupsFilter;                                 // select all contacts which are involved
  whichGroupsFilter.setRelationshipType(QContactRelationship::HasMember); // in a group relationship
  whichGroupsFilter.setRelatedContactId(particularContact.id());                // with the particular contact
  whichGroupsFilter.setRelatedContactRole(QContactRelationship::Second);  // where the particular contact is the second participant
  \endcode

 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactRelationshipFilter)

/*!
  \fn QContactRelationshipFilter::QContactRelationshipFilter(const QContactFilter& other)
  Constructs a copy of \a other if possible, else constructs a new QContactRelationshipFilter.
 * \since 1.0
 */

/*!
  Constructs a new relationship filter
 */
QContactRelationshipFilter::QContactRelationshipFilter()
    : QContactFilter(new QContactRelationshipFilterPrivate)
{
}

/*!
  Sets the type of relationship which a contact must have in order to match this filter to \a relationshipType
  \since 1.0
 */
void QContactRelationshipFilter::setRelationshipType(const QString& relationshipType)
{
    Q_D(QContactRelationshipFilter);
    d->m_relationshipType = relationshipType;
}

/*!
  Returns the type of relationship that a contact must have in order to match the filter
  \since 1.0
 */
QString QContactRelationshipFilter::relationshipType() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relationshipType;
}

/*!
  Sets the id of the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter to be \a relatedContactId
  \since 1.0
 */
void QContactRelationshipFilter::setRelatedContactId(const QContactId &relatedContactId)
{
    Q_D(QContactRelationshipFilter);
    d->m_relatedContactId = relatedContactId;
}

/*!
  Returns the id of the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter
  \since 1.0
 */
QContactId QContactRelationshipFilter::relatedContactId() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relatedContactId;
}

/*!
  Sets the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter to be \a relatedContactRole
  \since 1.0
 */
void QContactRelationshipFilter::setRelatedContactRole(QContactRelationship::Role relatedContactRole)
{
    Q_D(QContactRelationshipFilter);
    d->m_relatedContactRole = relatedContactRole;
}

/*!
  Returns the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter
  \since 1.0
 */
QContactRelationship::Role QContactRelationshipFilter::relatedContactRole() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relatedContactRole;
}


QTPIM_END_NAMESPACE