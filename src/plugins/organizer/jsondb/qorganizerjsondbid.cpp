/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qorganizerjsondbid.h"
#include "qorganizerjsondbstring.h"

QTORGANIZER_BEGIN_NAMESPACE

QOrganizerJsonDbItemId::QOrganizerJsonDbItemId()
    : QOrganizerItemEngineId()
    , m_jsonDbUuid(QString())
    , m_storageLocation(QOrganizerJsonDbEngine::UserDataStorage)
{
}

QOrganizerJsonDbItemId::QOrganizerJsonDbItemId(const QString &fullEngineId)
    : QOrganizerItemEngineId()
{
    splitId(fullEngineId, m_jsonDbUuid, m_storageLocation);
}

QOrganizerJsonDbItemId::QOrganizerJsonDbItemId(const QOrganizerJsonDbItemId &other)
    : QOrganizerItemEngineId(),
      m_jsonDbUuid(other.m_jsonDbUuid),
      m_storageLocation(other.m_storageLocation)
{
}

QOrganizerJsonDbItemId::~QOrganizerJsonDbItemId()
{
}

bool QOrganizerJsonDbItemId::isEqualTo(const QOrganizerItemEngineId *other) const
{
    const QOrganizerJsonDbItemId* id = static_cast<const QOrganizerJsonDbItemId *>(other);
    return ((m_jsonDbUuid == id->m_jsonDbUuid) && (m_storageLocation == id->m_storageLocation));
}

bool QOrganizerJsonDbItemId::isLessThan(const QOrganizerItemEngineId *other) const
{
    const QOrganizerJsonDbItemId* id = static_cast<const QOrganizerJsonDbItemId *>(other);
    if (m_storageLocation == id->m_storageLocation)
        return (m_jsonDbUuid < id->m_jsonDbUuid);
    else
        return (m_storageLocation < id->m_storageLocation);
}

QString QOrganizerJsonDbItemId::managerUri() const
{
    return QOrganizerJsonDbStr::jsonDbManagerUri();
}

QOrganizerItemEngineId *QOrganizerJsonDbItemId::clone() const
{
    QOrganizerJsonDbItemId *newId = new QOrganizerJsonDbItemId();
    newId->setJsonDbUuid(m_jsonDbUuid);
    newId->setStorageLocation(m_storageLocation);
    return newId;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &QOrganizerJsonDbItemId::debugStreamOut(QDebug &dbg) const
{
    dbg.nospace() << "QOrganizerJsonDbItemId(" << toString() << ")";
    return dbg.maybeSpace();
}
#endif

QString QOrganizerJsonDbItemId::toString() const
{
    return QString("%1/%2").arg(m_storageLocation).arg(m_jsonDbUuid);
}

uint QOrganizerJsonDbItemId::hash() const
{
    return QT_PREPEND_NAMESPACE(qHash)(this->toString());
}

void QOrganizerJsonDbItemId::setFullEngineId(const QString &fullEngineId)
{
    splitId(fullEngineId, m_jsonDbUuid, m_storageLocation);
}

QString QOrganizerJsonDbItemId::jsondbUuid() const
{
    return m_jsonDbUuid;
}

void QOrganizerJsonDbItemId::setJsonDbUuid(const QString &jsonDbUuid)
{
    m_jsonDbUuid = jsonDbUuid;
}

QOrganizerJsonDbEngine::StorageLocation QOrganizerJsonDbItemId::storageLocation() const
{
    return m_storageLocation;
}

void QOrganizerJsonDbItemId::setStorageLocation(QOrganizerJsonDbEngine::StorageLocation storageLocation)
{
    m_storageLocation = storageLocation;
}

void QOrganizerJsonDbItemId::splitId(const QString &fullId, QString &jsondbUuid, QOrganizerJsonDbEngine::StorageLocation &storageLocation)
{
    // separate engine id part, if full id given
    QString engineId = fullId.contains(":") ? fullId.mid(fullId.lastIndexOf(":")+1) : fullId;
    // separate storagelocation and collection id from each other
    const QStringList splittedEngineId = engineId.split(QStringLiteral("/"));
    storageLocation = (QOrganizerJsonDbEngine::StorageLocation)splittedEngineId.first().toInt();
    jsondbUuid = splittedEngineId.last();
}

QOrganizerJsonDbCollectionId::QOrganizerJsonDbCollectionId()
    : QOrganizerCollectionEngineId()
    , m_jsonDbUuid(QString())
    , m_storageLocation(QOrganizerJsonDbEngine::UserDataStorage)
{
}

QOrganizerJsonDbCollectionId::QOrganizerJsonDbCollectionId(const QString &fullEngineId)
    : QOrganizerCollectionEngineId()
{
    splitId(fullEngineId, m_jsonDbUuid, m_storageLocation);
}

QOrganizerJsonDbCollectionId::QOrganizerJsonDbCollectionId(const QOrganizerJsonDbCollectionId &other)
    : QOrganizerCollectionEngineId()
    , m_jsonDbUuid(other.m_jsonDbUuid)
    , m_storageLocation(other.m_storageLocation)
{
}

QOrganizerJsonDbCollectionId::~QOrganizerJsonDbCollectionId()
{
}

bool QOrganizerJsonDbCollectionId::isEqualTo(const QOrganizerCollectionEngineId *other) const
{
    const QOrganizerJsonDbCollectionId* collId = static_cast<const QOrganizerJsonDbCollectionId *>(other);
    return ((m_jsonDbUuid == collId->m_jsonDbUuid) && (m_storageLocation == collId->m_storageLocation));
}

bool QOrganizerJsonDbCollectionId::isLessThan(const QOrganizerCollectionEngineId *other) const
{
    const QOrganizerJsonDbCollectionId* collId = static_cast<const QOrganizerJsonDbCollectionId *>(other);
    if (m_storageLocation == collId->m_storageLocation)
        return (m_jsonDbUuid < collId->m_jsonDbUuid);
    else
        return (m_storageLocation < collId->m_storageLocation);
}

QString QOrganizerJsonDbCollectionId::managerUri() const
{
    return QOrganizerJsonDbStr::jsonDbManagerUri();
}

QOrganizerCollectionEngineId *QOrganizerJsonDbCollectionId::clone() const
{
    QOrganizerJsonDbCollectionId *newId = new QOrganizerJsonDbCollectionId();
    newId->setJsonDbUuid(m_jsonDbUuid);
    newId->setStorageLocation(m_storageLocation);
    return newId;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug &QOrganizerJsonDbCollectionId::debugStreamOut(QDebug &dbg) const
{
    dbg.nospace() << "QOrganizerJsonDbCollectionId(" << toString() << ")";
    return dbg.maybeSpace();
}
#endif

QString QOrganizerJsonDbCollectionId::toString() const
{
    return QString("%1/%2").arg(m_storageLocation).arg(m_jsonDbUuid);
}

uint QOrganizerJsonDbCollectionId::hash() const
{
    return QT_PREPEND_NAMESPACE(qHash)(toString());
}

void QOrganizerJsonDbCollectionId::setFullEngineId(const QString &fullEngineId)
{
    splitId(fullEngineId, m_jsonDbUuid, m_storageLocation);
}

QString QOrganizerJsonDbCollectionId::jsondbUuid() const
{
    return m_jsonDbUuid;
}

void QOrganizerJsonDbCollectionId::setJsonDbUuid(const QString &jsonDbUuid)
{
    m_jsonDbUuid = jsonDbUuid;
}

QOrganizerJsonDbEngine::StorageLocation QOrganizerJsonDbCollectionId::storageLocation() const
{
    return m_storageLocation;
}

void QOrganizerJsonDbCollectionId::setStorageLocation(QOrganizerJsonDbEngine::StorageLocation storageLocation)
{
    m_storageLocation = storageLocation;
}

void QOrganizerJsonDbCollectionId::splitId(const QString &fullId, QString &jsondbUuid, QOrganizerJsonDbEngine::StorageLocation &storageLocation)
{
    // separate engine id part, if full id given
    QString engineId = fullId.contains(":") ? fullId.mid(fullId.lastIndexOf(":")+1) : fullId;
    // separate storagelocation and collection id from each other
    const QStringList splittedEngineId = engineId.split(QStringLiteral("/"));
    storageLocation = QOrganizerJsonDbEngine::StorageLocation(splittedEngineId.first().toInt());
    jsondbUuid = splittedEngineId.last();
}

QTORGANIZER_END_NAMESPACE
