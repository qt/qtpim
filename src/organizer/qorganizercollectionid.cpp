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

#include "qorganizercollectionid.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qorganizermanager_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerCollectionId
    \brief The QOrganizerCollectionId class provides information that uniquely identifies a collection
           in a particular manager.
    \inmodule QtOrganizer
    \ingroup organizer-main

    It consists of a manager URI which identifies the manager which contains the collection,
    and the engine specific ID of the collection in that manager.

    An invalid QOrganizerCollectionId has an empty manager URI.
*/

/*!
    \fn QOrganizerCollectionId::QOrganizerCollectionId()

    Constructs a new, invalid collection ID.
*/

// TODO: Document and remove internal once the correct signature has been determined
/*!
    \fn QOrganizerCollectionId::QOrganizerCollectionId(const QString &managerUri, const QByteArray &localId)
    \internal

    Constructs an ID from the supplied manager URI \a managerUri and the engine
    specific \a localId string.
*/

/*!
    \fn bool QOrganizerCollectionId::operator==(const QOrganizerCollectionId &other) const

    Returns true if this collection ID has the same manager URI and
    engine specific ID as \a other. Returns true also, if both IDs are null.
*/

/*!
    \fn bool QOrganizerCollectionId::operator!=(const QOrganizerCollectionId &other) const

    Returns true if either the manager URI or engine specific ID of this
    collection ID is different to that of \a other.
*/

/*!
    \fn bool operator<(const QOrganizerCollectionId &id1, const QOrganizerCollectionId &id2)
    \relates QOrganizerCollectionId

    Returns true if the collection ID \a id1 will be considered less than
    the collection ID \a id2 if the manager URI of ID \a id1 is alphabetically
    less than the manager URI of the \a id2 ID. If both IDs have the same
    manager URI, ID \a id1 will be considered less than the ID \a id2
    if the the engine specific ID of \a id1 is less than the engine specific ID of \a id2.

    The invalid, null collection ID consists of an empty manager URI and a null engine ID,
    and hence will be less than any valid, non-null collection ID.

    This operator is provided primarily to allow use of a QOrganizerCollectionId as a key in a QMap.
*/

/*!
    \fn uint qHash(const QOrganizerCollectionId &id)
    \relates QOrganizerCollectionId

    Returns the hash value for \a id.
*/

/*!
    \fn bool QOrganizerCollectionId::isValid() const

    Returns true if the manager URI part is not null; returns false otherwise.

    Note that valid ID may be null at the same time, which means new collection.

    \sa isNull()
*/

/*!
    \fn bool QOrganizerCollectionId::isNull() const

    Returns true if the engine specific ID part is a null (default constructed);
    returns false otherwise.

    \sa isValid()
*/

/*!
    \fn QString QOrganizerCollectionId::managerUri() const

    Returns the URI of the manager which contains the collection identified by this ID.

    \sa localId()
*/

/*!
    \fn QByteArray QOrganizerCollectionId::localId() const

    Returns the collection's engine specific ID part.

    \sa managerUri()
*/

/*!
    Serializes the collection ID to a string. The format of the string will be:
    "qtorganizer:managerName:params:localId", where localId is encoded binary data
    formatted as hexadecimal to ensure it is in a printable form.

    \sa fromString(), toByteArray()
*/
QString QOrganizerCollectionId::toString() const
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
    collection ID if the given \a idString is not a valid, serialized collection ID.

    \sa toString(), fromByteArray()
*/
QOrganizerCollectionId QOrganizerCollectionId::fromString(const QString &idString)
{
    QString managerUri;
    QByteArray localId;

    if (QOrganizerManagerData::parseIdData(idString.toUtf8(), 0, 0, &managerUri, &localId)) {
        // The localId component must be decoded from hex
        return QOrganizerCollectionId(managerUri, QByteArray::fromHex(localId));
    }

    return QOrganizerCollectionId();
}

/*!
    Serializes the collection ID to a byte array.

    \sa fromByteArray(), toString()
*/
QByteArray QOrganizerCollectionId::toByteArray() const
{
    if (!isNull())
        return QOrganizerManagerData::buildIdData(m_managerUri, m_localId);

    return QByteArray();
}

/*!
    Deserializes the given \a idData. Returns a default-constructed (null)
    collection ID if the given \a idData does not contain a valid, serialized collection ID.

    \sa toByteArray(), fromString()
*/
QOrganizerCollectionId QOrganizerCollectionId::fromByteArray(const QByteArray &idData)
{
    QString managerUri;
    QByteArray localId;

    if (QOrganizerManagerData::parseIdData(idData, 0, 0, &managerUri, &localId))
        return QOrganizerCollectionId(managerUri, localId);

    return QOrganizerCollectionId();
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerCollectionId
    Outputs \a id to the debug stream \a dbg.
*/
QDebug operator<<(QDebug dbg, const QOrganizerCollectionId &id)
{
    dbg.nospace() << "QOrganizerCollectionId(" << id.toString().toUtf8().constData() << ")";
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerCollectionId
    Streams \a id to the data stream \a out.
*/
QDataStream &operator<<(QDataStream &out, const QOrganizerCollectionId &id)
{
    out << id.toByteArray();
    return out;
}

/*!
    \relates QOrganizerCollectionId
    Streams \a id in from the data stream \a in.
*/
QDataStream &operator>>(QDataStream &in, QOrganizerCollectionId &id)
{
    QByteArray idData;
    in >> idData;
    id = QOrganizerCollectionId::fromByteArray(idData);
    return in;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE_ORGANIZER
