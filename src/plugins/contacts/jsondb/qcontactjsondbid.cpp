/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Pim module.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QUuid>
#include "qcontactjsondbid.h"
#include "qcontactjsondbstring.h"

QTCONTACTS_BEGIN_NAMESPACE

QContactJsonDbId::QContactJsonDbId(const QString &engineId)
{
    QStringList splitEngineId = engineId.split("/");
    if (splitEngineId.size() == 2) {
        m_uuid = splitEngineId.last();
        m_storageLocation = QContactAbstractRequest::StorageLocation(
                    splitEngineId.first().toInt());
    }
}

QContactJsonDbId::QContactJsonDbId(const QContactJsonDbId &other)
    : m_uuid(other.m_uuid),
      m_storageLocation(other.m_storageLocation)
{
}

QContactJsonDbId::QContactJsonDbId(const QUuid &uuid, const QContactAbstractRequest::StorageLocation &storageLocation)
    : m_uuid(uuid),
      m_storageLocation(storageLocation)
{
}

QContactJsonDbId::~QContactJsonDbId()
{
}

bool QContactJsonDbId::isEqualTo(const QContactEngineId *other) const
{
    const QContactJsonDbId *otherJsonDbId = static_cast<const QContactJsonDbId *>(other);
    QUuid otherUuid = otherJsonDbId->m_uuid;
    QContactAbstractRequest::StorageLocation otherStorageLocation = otherJsonDbId->m_storageLocation;
    return ((m_uuid == otherUuid) && (m_storageLocation == otherStorageLocation));
}

bool QContactJsonDbId::isLessThan(const QContactEngineId *other) const
{
    const QContactJsonDbId *otherJsonDbId = static_cast<const QContactJsonDbId *>(other);
    QUuid otherUuid = otherJsonDbId->m_uuid;
    QContactAbstractRequest::StorageLocation otherStorageLocation = otherJsonDbId->m_storageLocation;
    return ((m_storageLocation < otherStorageLocation) ||
            ((m_storageLocation == otherStorageLocation) && (m_uuid < otherUuid)));
}

QString QContactJsonDbId::managerUri() const
{
    return QContactJsonDbStr::managerUri();
}

QContactEngineId *QContactJsonDbId::clone() const
{
    return new QContactJsonDbId(m_uuid, m_storageLocation);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &QContactJsonDbId::debugStreamOut(QDebug &dbg) const
{
    dbg.nospace() << "QContactJsonDbId(" << this->toString() << ")";
    return dbg.maybeSpace();
}
#endif

QString QContactJsonDbId::toString() const
{
    QString stringifiedIdFormat("%1/%2");
    return stringifiedIdFormat.arg(QString::number(m_storageLocation)).arg(m_uuid.toString());
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

    return QT_PREPEND_NAMESPACE(qHash)(this->toString());
}

/*!
 * Returns the jsondb uuid of the contact
 */
QUuid QContactJsonDbId::uuid() const
{
    return m_uuid;
}

/*!
 * Returns the storage location where the contact is stored in
 */
QContactAbstractRequest::StorageLocation QContactJsonDbId::storageLocation() const
{
    return m_storageLocation;
}

QTCONTACTS_END_NAMESPACE
