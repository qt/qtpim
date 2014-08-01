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

#include "qorganizeritemid.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qorganizermanager_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemId
    \brief The QOrganizerItemId class provides information that uniquely identifies an organizer
           item in a particular manager.
    \inmodule QtOrganizer
    \ingroup organizer-main

    It consists of a manager URI which identifies the manager which contains the organizer item,
    and the engine specific ID of the organizer item in that manager.

    An invalid QOrganizerItemId has an empty manager URI.
*/

/*!
    \fn QOrganizerItemId::QOrganizerItemId()

    Constructs a new, invalid organizer item ID.
*/

// TODO: Document and remove internal once the correct signature has been determined
/*!
    \fn QOrganizerItemId::QOrganizerItemId(const QString &managerUri, const QString &localId)
    \internal

    Constructs an ID from the supplied manager URI \a managerUri and the engine
    specific \a localId string.
*/

/*!
    \fn bool QOrganizerItemId::operator==(const QOrganizerItemId &other) const

    Returns true if this organizer item ID has the same manager URI and
    engine specific ID as \a other. Returns true also, if both IDs are null.
*/

/*!
    \fn bool QOrganizerItemId::operator!=(const QOrganizerItemId &other) const

    Returns true if either the manager URI or engine specific ID of this
    organizer item ID is different to that of \a other.
*/

/*!
    \fn bool operator<(const QOrganizerItemId &id1, const QOrganizerItemId &id2)
    \relates QOrganizerItemId

    Returns true if the organizer item ID \a id1 will be considered less than
    the organizer item ID \a id2 if the manager URI of ID \a id1 is alphabetically
    less than the manager URI of the \a id2 ID. If both IDs have the same
    manager URI, ID \a id1 will be considered less than the ID \a id2
    if the the engine specific ID of \a id1 is less than the engine specific ID of \a id2.

    The invalid, null organizer item ID consists of an empty manager URI and a null engine ID,
    and hence will be less than any valid, non-null organizer item ID.

    This operator is provided primarily to allow use of a QOrganizerItemId as a key in a QMap.
*/

/*!
    \fn uint qHash(const QOrganizerItemId &id)
    \relates QOrganizerItemId

    Returns the hash value for \a id.
*/

/*!
    \fn bool QOrganizerItemId::isValid() const

    Returns true if the manager URI part is not null; returns false otherwise.

    Note that valid ID may be null at the same time, which means new organizer item.

    \sa isNull()
*/

/*!
    \fn bool QOrganizerItemId::isNull() const

    Returns true if the engine specific ID part is a null (default constructed);
    returns false otherwise.

    \sa isValid()
*/

/*!
    \fn QString QOrganizerItemId::managerUri() const

    Returns the URI of the manager which contains the organizer item identified by this ID.

    \sa localId()
*/

/*!
    \fn QString QOrganizerItemId::localId() const

    Returns the organizer item's engine specific ID part.

    \sa managerUri()
*/

/*!
    Serializes the organizer item ID to a string. The format of the string will be:
    "qtorganizer:managerName:constructionParams:engineLocalItemId".

    \sa fromString()
*/
QString QOrganizerItemId::toString() const
{
    if (!isNull() && QOrganizerManagerData::parseIdString(m_managerUri, 0, 0))
        return QOrganizerManagerData::buildIdString(m_managerUri, m_localId);

    return QString();
}

/*!
    Deserializes the given \a idString. Returns a default-constructed (null)
    organizer item ID if the given \a idString is not a valid, serialized organizer item ID.

    \sa toString()
*/
QOrganizerItemId QOrganizerItemId::fromString(const QString &idString)
{
    QString managerUri;
    QString engineIdString;

    if (QOrganizerManagerData::parseIdString(idString, 0, 0, &managerUri, &engineIdString))
        return QOrganizerItemId(managerUri, engineIdString);

    return QOrganizerItemId();
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerItemId
    Outputs \a id to the debug stream \a dbg.
*/
Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerItemId &id)
{
    dbg.nospace() << "QOrganizerItemId(" << id.toString() << ")";
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerItemId
    Streams \a id to the data stream \a out.
*/
Q_ORGANIZER_EXPORT QDataStream &operator<<(QDataStream &out, const QOrganizerItemId &id)
{
    out << id.toString();
    return out;
}

/*!
    \relates QOrganizerItemId
    Streams \a id in from the data stream \a in.
*/
Q_ORGANIZER_EXPORT QDataStream &operator>>(QDataStream &in, QOrganizerItemId &id)
{
    QString idString;
    in >> idString;
    id = QOrganizerItemId::fromString(idString);
    return in;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE_ORGANIZER
