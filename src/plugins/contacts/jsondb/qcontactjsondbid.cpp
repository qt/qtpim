/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the documentation of the Qt Pim Module.
**
** $QT_BEGIN_LICENSE:FDL$
** GNU Free Documentation License
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms
** and conditions contained in a signed written agreement between you
** and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactjsondbid.h"
#include "qcontactjsondbstring.h"

QTCONTACTS_BEGIN_NAMESPACE

QContactJsonDbId::QContactJsonDbId()
    : QContactEngineId()
    , m_contactId(QString())
{
}

QContactJsonDbId::QContactJsonDbId(const QString &contactId)
    : QContactEngineId()
    , m_contactId(contactId)
{
}

QContactJsonDbId::QContactJsonDbId(const QContactJsonDbId &other)
    : QContactEngineId(),
      m_contactId(other.m_contactId)
{
}

QContactJsonDbId::~QContactJsonDbId()
{
}

bool QContactJsonDbId::isEqualTo(const QContactEngineId *other) const
{
    QString otherContactId = static_cast<const QContactJsonDbId *>(other)->m_contactId;
    return (m_contactId == otherContactId);
}

bool QContactJsonDbId::isLessThan(const QContactEngineId *other) const
{
    QString otherContactId = static_cast<const QContactJsonDbId *>(other)->m_contactId;
    return (m_contactId < otherContactId);
}

QString QContactJsonDbId::managerUri() const
{
    return QContactJsonDbStr::managerUri();
}

QContactEngineId *QContactJsonDbId::clone() const
{
    return new QContactJsonDbId(m_contactId);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &QContactJsonDbId::debugStreamOut(QDebug &dbg) const
{
    dbg.nospace() << "QContactJsonDbId(" << m_contactId << ")";
    return dbg.maybeSpace();
}
#endif

QString QContactJsonDbId::toString() const
{
    return m_contactId;
}

uint QContactJsonDbId::hash() const
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

    return QT_PREPEND_NAMESPACE(qHash)(m_contactId);
}

void QContactJsonDbId::setContactId(const QString &contactId)
{
    m_contactId = contactId;
}

QTCONTACTS_END_NAMESPACE
