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

#include "qcontactrelationshipfilter.h"
#include "qcontactrelationshipfilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactRelationshipFilter
  \brief The QContactRelationshipFilter class provides a filter based
  around relationship criteria.


  \inmodule QtContacts

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

  Relationships are also used to define composition of facets into contacts.  To find the
  facets from which a contact is composed:

  \code
  QContactRelationshipFilter whichFacetsFilter;                            // select all facets which are involved
  whichFacetsFilter.setRelationshipType(QContactRelationship::Aggregates); // in an aggregation relationship
  whichFacetsFilter.setRelatedContact(particularContact());                // with the particular contact
  whichFacetsFilter.setRelatedContactRole(QContactRelationship::First);    // where the particular contact is the first participant
  \endcode

 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactRelationshipFilter)

/*!
  \fn QContactRelationshipFilter::QContactRelationshipFilter(const QContactFilter& other)
  Constructs a copy of \a other if possible, else constructs a new QContactRelationshipFilter.
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
 */
void QContactRelationshipFilter::setRelationshipType(const QString& relationshipType)
{
    Q_D(QContactRelationshipFilter);
    d->m_relationshipType = relationshipType;
}

/*!
  Returns the type of relationship that a contact must have in order to match the filter
 */
QString QContactRelationshipFilter::relationshipType() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relationshipType;
}

/*!
  Sets the ID of the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter, to be \a relatedContactId
 */
void QContactRelationshipFilter::setRelatedContactId(const QContactId &relatedContactId)
{
    Q_D(QContactRelationshipFilter);
    d->m_relatedContactId = relatedContactId;
}

/*!
  Returns the ID of the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter
 */
QContactId QContactRelationshipFilter::relatedContactId() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relatedContactId;
}

/*!
  Sets the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter to be \a relatedContactRole
 */
void QContactRelationshipFilter::setRelatedContactRole(QContactRelationship::Role relatedContactRole)
{
    Q_D(QContactRelationshipFilter);
    d->m_relatedContactRole = relatedContactRole;
}

/*!
  Returns the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter
 */
QContactRelationship::Role QContactRelationshipFilter::relatedContactRole() const
{
    Q_D(const QContactRelationshipFilter);
    return d->m_relatedContactRole;
}


QT_END_NAMESPACE_CONTACTS
