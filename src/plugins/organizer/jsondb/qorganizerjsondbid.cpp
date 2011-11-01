/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring_p.h"

QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbItemId::QOrganizerJsonDbItemId()
    : QOrganizerItemEngineId()
    , m_itemId(QString())
{
}

QOrganizerJsonDbItemId::QOrganizerJsonDbItemId(const QString &itemId)
    : QOrganizerItemEngineId()
    , m_itemId(itemId)
{
}

QOrganizerJsonDbItemId::QOrganizerJsonDbItemId(const QOrganizerJsonDbItemId &other)
    : QOrganizerItemEngineId(),
      m_itemId(other.m_itemId)
{
}

QOrganizerJsonDbItemId::~QOrganizerJsonDbItemId()
{
}

bool QOrganizerJsonDbItemId::isEqualTo(const QOrganizerItemEngineId *other) const
{
    QString otherItemId = static_cast<const QOrganizerJsonDbItemId *>(other)->m_itemId;
    return (m_itemId == otherItemId);
}

bool QOrganizerJsonDbItemId::isLessThan(const QOrganizerItemEngineId *other) const
{
    QString otherItemId = static_cast<const QOrganizerJsonDbItemId *>(other)->m_itemId;
    return (m_itemId < otherItemId);
}

QString QOrganizerJsonDbItemId::managerUri() const
{
    return QOrganizerJsonDbStr::ManagerUri;
}

QOrganizerItemEngineId *QOrganizerJsonDbItemId::clone() const
{
    return new QOrganizerJsonDbItemId(m_itemId);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &QOrganizerJsonDbItemId::debugStreamOut(QDebug &dbg) const
{
    dbg.nospace() << "QOrganizerJsonDbItemId(" << m_itemId << ")";
    return dbg.maybeSpace();
}
#endif

QString QOrganizerJsonDbItemId::toString() const
{
    return m_itemId;
}

uint QOrganizerJsonDbItemId::hash() const
{
    /*
      TODO

      Provide a hash function for your engine-specific id.
      Note that the hash doesn't strictly need to be unique, since isEqualTo()
      ensures that individual id's in a single hash-bucket can be uniquely
      determined; however a better hash function will result in better performance
      because the ids will be distributed more randomly in a hash table.

      In the example implementation below, we could simply return the id, since the
      id is a quint32.  In more complex id classes, however, you may need to
      qHash() individual data members and combine the results somehow.
     */

    return QT_PREPEND_NAMESPACE(qHash)(m_itemId);
}

void QOrganizerJsonDbItemId::setItemId(QString itemId)
{
    m_itemId = itemId;
}

QOrganizerJsonDbCollectionId::QOrganizerJsonDbCollectionId()
    : QOrganizerCollectionEngineId()
    , m_collectionId(QString())
{
}

QOrganizerJsonDbCollectionId::QOrganizerJsonDbCollectionId(const QString &collectionId)
    : QOrganizerCollectionEngineId()
    , m_collectionId(collectionId)
{
}

QOrganizerJsonDbCollectionId::QOrganizerJsonDbCollectionId(const QOrganizerJsonDbCollectionId &other)
    : QOrganizerCollectionEngineId()
    , m_collectionId(other.m_collectionId)
{
}

QOrganizerJsonDbCollectionId::~QOrganizerJsonDbCollectionId()
{
}

bool QOrganizerJsonDbCollectionId::isEqualTo(const QOrganizerCollectionEngineId *other) const
{
    QString otherCollectionId = static_cast<const QOrganizerJsonDbCollectionId *>(other)->m_collectionId;
    return (m_collectionId == otherCollectionId);
}

bool QOrganizerJsonDbCollectionId::isLessThan(const QOrganizerCollectionEngineId *other) const
{
    QString otherCollectionId = static_cast<const QOrganizerJsonDbCollectionId *>(other)->m_collectionId;
    return (m_collectionId < otherCollectionId);
}

QString QOrganizerJsonDbCollectionId::managerUri() const
{
    return QOrganizerJsonDbStr::ManagerUri;
}

QOrganizerCollectionEngineId *QOrganizerJsonDbCollectionId::clone() const
{
    return new QOrganizerJsonDbCollectionId(m_collectionId);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &QOrganizerJsonDbCollectionId::debugStreamOut(QDebug &dbg) const
{
    dbg.nospace() << "QOrganizerJsonDbCollectionId(" << m_collectionId << ")";
    return dbg.maybeSpace();
}
#endif

QString QOrganizerJsonDbCollectionId::toString() const
{
    return m_collectionId;
}

uint QOrganizerJsonDbCollectionId::hash() const
{
    /*
      TODO

      Provide a hash function for your engine-specific id.
      Note that the hash doesn't strictly need to be unique, since isEqualTo()
      ensures that individual id's in a single hash-bucket can be uniquely
      determined; however a better hash function will result in better performance
      because the ids will be distributed more randomly in a hash table.

      In the example implementation below, we could simply return the id, since the
      id is a quint32.  In more complex id classes, however, you may need to
      qHash() individual data members and combine the results somehow.
     */

    return QT_PREPEND_NAMESPACE(qHash)(m_collectionId);
}

void QOrganizerJsonDbCollectionId::setCollectionId(QString collectionId)
{
    m_collectionId = collectionId;
}

QTORGANIZER_END_NAMESPACE
