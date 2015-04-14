/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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
    \fn QOrganizerItemId::QOrganizerItemId(const QString &managerUri, const QByteArray &localId)
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
    \fn QByteArray QOrganizerItemId::localId() const

    Returns the organizer item's engine specific ID part.

    \sa managerUri()
*/

/*!
    Serializes the organizer item ID to a string. The format of the string will be:
    "qtorganizer:managerName:params:localId", where localId is encoded binary data
    formatted as hexadecimal to ensure it is in a printable form.

    \sa fromString(), toByteArray()
*/
QString QOrganizerItemId::toString() const
{
    if (!isNull()) {
        // Ensure the localId component has a valid string representation by hex encoding
        const QByteArray encodedLocalId(m_localId.toHex());
        return QString::fromUtf8(QOrganizerManagerData::buildIdData(m_managerUri, encodedLocalId));
    }

    return QString();
}

/*!
    Deserializes the given \a idString. Returns a default-constructed (null)
    item ID if the given \a idString is not a valid, serialized item ID.

    \sa toString(), fromByteArray()
*/
QOrganizerItemId QOrganizerItemId::fromString(const QString &idString)
{
    QString managerUri;
    QByteArray localId;

    if (QOrganizerManagerData::parseIdData(idString.toUtf8(), 0, 0, &managerUri, &localId)) {
        // The localId component must be decoded from hex
        return QOrganizerItemId(managerUri, QByteArray::fromHex(localId));
    }

    return QOrganizerItemId();
}

/*!
    Serializes the organizer item ID to a byte array.

    \sa fromByteArray(), toString()
*/
QByteArray QOrganizerItemId::toByteArray() const
{
    if (!isNull())
        return QOrganizerManagerData::buildIdData(m_managerUri, m_localId);

    return QByteArray();
}

/*!
    Deserializes the given \a idData. Returns a default-constructed (null)
    item ID if the given \a idData does not contain a valid, serialized item ID.

    \sa toByteArray(), fromString()
*/
QOrganizerItemId QOrganizerItemId::fromByteArray(const QByteArray &idData)
{
    QString managerUri;
    QByteArray localId;

    if (QOrganizerManagerData::parseIdData(idData, 0, 0, &managerUri, &localId))
        return QOrganizerItemId(managerUri, localId);

    return QOrganizerItemId();
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerItemId
    Outputs \a id to the debug stream \a dbg.
*/
Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerItemId &id)
{
    dbg.nospace() << "QOrganizerItemId(" << qPrintable(id.toString()) << ")";
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
    out << id.toByteArray();
    return out;
}

/*!
    \relates QOrganizerItemId
    Streams \a id in from the data stream \a in.
*/
Q_ORGANIZER_EXPORT QDataStream &operator>>(QDataStream &in, QOrganizerItemId &id)
{
    QByteArray idData;
    in >> idData;
    id = QOrganizerItemId::fromByteArray(idData);
    return in;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE_ORGANIZER
