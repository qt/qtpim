/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#include "qdeclarativecontactrelationship_p.h"

#include <QtQml/qqmlinfo.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
   \qmltype Relationship
    \instantiates QDeclarativeContactRelationship
   \brief The Relationship element describes a one-to-one relationship
  between a locally-stored contact and another (possibly remote) contact.

   \ingroup qml-contacts-main
   \inqmlmodule QtContacts
   This element is part of the \b{QtContacts} module.

   \sa QContactRelationship
   \sa RelationshipModel
 */

QDeclarativeContactRelationship::QDeclarativeContactRelationship(QObject* parent)
    :QObject(parent)
{

}


/*!
  \qmlproperty int Relationship::first

  This property holds the locally-stored contact which has a relationship of the given type with the second contact.
  */
QString QDeclarativeContactRelationship::first() const
{
    const QContactId contactId(m_relationship.first());
    if (!contactId.isNull())
        return contactId.toString();
    return QString();
}

/*!
  \qmlproperty int Relationship::second

  This property holds the contact with which the first contact has a relationship of the given type.
  */
QString QDeclarativeContactRelationship::second() const
{
    const QContactId contactId(m_relationship.second());
    if (!contactId.isNull())
        return contactId.toString();
    return QString();
}

/*!
  \qmlproperty string Relationship::type

  This property holds the type of relationship which the source contact has with the destination contacts.
  The value for this property can be one of:
  \list
  \li HasMember
  \li Aggregates
  \li IsSameAs
  \li HasAssistant
  \li HasManager
  \li HasSpouse
  \endlist
  or any other customized relationship type string.
  */

QVariant QDeclarativeContactRelationship::relationshipType() const
{
    return m_relationship.relationshipType();
}

void QDeclarativeContactRelationship::setFirst(const QString& firstContactId)
{
    m_relationship.setFirst(QContactId::fromString(firstContactId));
}

void QDeclarativeContactRelationship::setSecond(const QString& secondContactId)
{
    m_relationship.setSecond(QContactId::fromString(secondContactId));
}

void QDeclarativeContactRelationship::setRelationshipType(const QVariant& relationshipType)
{
    if (relationshipType.type() == QVariant::Int) {
        switch (relationshipType.toInt()) {
        case QDeclarativeContactRelationship::HasMember:
            m_relationship.setRelationshipType(QContactRelationship::HasMember());
            break;
        case QDeclarativeContactRelationship::Aggregates:
            m_relationship.setRelationshipType(QContactRelationship::Aggregates());
            break;
        case QDeclarativeContactRelationship::IsSameAs:
            m_relationship.setRelationshipType(QContactRelationship::IsSameAs());
            break;
        case QDeclarativeContactRelationship::HasAssistant:
            m_relationship.setRelationshipType(QContactRelationship::HasAssistant());
            break;
        case QDeclarativeContactRelationship::HasManager:
            m_relationship.setRelationshipType(QContactRelationship::HasManager());
            break;
        case QDeclarativeContactRelationship::HasSpouse:
            m_relationship.setRelationshipType(QContactRelationship::HasSpouse());
            break;
        default:
            //unknown type
            qmlInfo(this) << tr("unknown relationship type:") << relationshipType;
            break;
        }
    } else {
        m_relationship.setRelationshipType(relationshipType.toString());
    }

}


QContactRelationship QDeclarativeContactRelationship::relationship() const
{
    return m_relationship;
}
void QDeclarativeContactRelationship::setRelationship(const QContactRelationship& relationship)
{
    m_relationship = relationship;
    emit valueChanged();
}

#include "moc_qdeclarativecontactrelationship_p.cpp"

QT_END_NAMESPACE
