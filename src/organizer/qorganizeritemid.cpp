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

#include "qorganizeritemengineid.h"
#include "qorganizermanager_p.h"

#if !defined(Q_CC_MWERKS)
QT_BEGIN_NAMESPACE
template<> QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemEngineId) *QSharedDataPointer<QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemEngineId)>::clone()
{
    return d ? d->clone() : 0;
}
QT_END_NAMESPACE
#endif

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemId
    \brief The QOrganizerItemId class provides information that uniquely identifies an organizer
           item in a particular manager.
    \inmodule QtOrganizer
    \ingroup organizer-main

    It consists of a manager URI which identifies the manager which contains the organizer item,
    and the engine specific ID of the organizer item in that manager.

    A "null" QOrganizerItemId has an empty manager URI.
 */

/*!
    Constructs a new organizer item ID.
 */
QOrganizerItemId::QOrganizerItemId()
    : d(0)
{
}

/*!
    Constructs a manager-unique ID which wraps the given engine-unique item ID \a engineItemId.
    This item ID takes ownership of the engine ID and will delete it when the item ID goes out
    of scope.  Engine implementors must not delete the \a engineItemId or undefined behaviour
    will occur.
 */
QOrganizerItemId::QOrganizerItemId(QOrganizerItemEngineId *engineItemId)
    : d(engineItemId)
{
}

/*!
    Cleans up the memory in use by the organizer item ID.
 */
QOrganizerItemId::~QOrganizerItemId()
{
}

/*!
    Constructs a new organizer item ID as a copy of \a other.
*/
QOrganizerItemId::QOrganizerItemId(const QOrganizerItemId &other)
    : d(other.d)
{
}

/*!
    Assigns the organizer item ID to be equal to \a other.
*/
QOrganizerItemId &QOrganizerItemId::operator=(const QOrganizerItemId &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if the organizer item ID has the same manager URI and ID as \a other.
*/
bool QOrganizerItemId::operator==(const QOrganizerItemId &other) const
{
    if (d == other.d)
        return true;

    if (d && other.d)
        return d->managerUri() == other.d->managerUri() && d->isEqualTo(other.d);

    return false;
}

/*!
    Returns true if either the manager URI or ID is different to that of \a other.
*/
bool QOrganizerItemId::operator!=(const QOrganizerItemId &other) const
{
    return !(*this == other);
}

/*!
    Returns true if this ID is less than the \a other ID.

    This ID will be considered less than the \a other iID if the manager URI of this ID is
    alphabetically less than the manager URI of the \a other ID.  If both IDs have the same
    manager URI, this ID will be considered less than the \a other ID if the engine ID of
    this ID is less than the engine ID of the \a other ID.

    The invalid, empty ID consists of an empty manager URI and a null engine ID, and hence will
    be less than any non-invalid ID.

    This operator is provided primarily to allow use of a QOrganizerItemId as a key in a QMap.
 */
bool QOrganizerItemId::operator<(const QOrganizerItemId &other) const
{
    if (d == 0 && other.d != 0)
        return true;

    if (d && other.d) {
        if (d->managerUri() == other.d->managerUri())
            return d->isLessThan(other.d);

        return d->managerUri() < other.d->managerUri();
    }

    return false;
}

/*!
    \relates QOrganizerItemId
    Returns the hash value for \a key.
 */
Q_ORGANIZER_EXPORT uint qHash(const QOrganizerItemId &key)
{
    if (key.d)
        return key.d->hash();
    return 0;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerItemId
    Outputs \a id to the debug stream \a dbg.
 */
Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerItemId &id)
{
    dbg.nospace() << "QOrganizerItemId(";
    if (id.isNull())
        dbg.nospace() << "(null))";
    else
        id.d->debugStreamOut(dbg)  << ")";
    return dbg.maybeSpace();
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerItemId
    Streams \a itemId to the data stream \a out.
 */
Q_ORGANIZER_EXPORT QDataStream &operator<<(QDataStream &out, const QOrganizerItemId &itemId)
{
    out << (itemId.toString());
    return out;
}

/*!
    \relates QOrganizerItemId
    Streams \a itemId in from the data stream \a in.
 */
Q_ORGANIZER_EXPORT QDataStream &operator>>(QDataStream &in, QOrganizerItemId &itemId)
{
    QString idString;
    in >> idString;
    itemId = QOrganizerItemId::fromString(idString);
    return in;
}
#endif

/*!
    Returns true if the engine ID part is a null (default constructed) engine ID; otherwise,
    returns false.
 */
bool QOrganizerItemId::isNull() const
{
    return d == 0;
}

/*!
    Returns the URI of the manager which contains the organizer item identified by this ID.
 */
QString QOrganizerItemId::managerUri() const
{
    return d ? d->managerUri() : QString();
}

/*!
    Serializes the organizer item ID to a string. The format of the string will be:
    "qtorganizer:managerName:constructionParams:engineLocalItemId".

    \sa fromString()
*/
QString QOrganizerItemId::toString() const
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    // rely on engine id to supply the full manager uri
    if (d && QOrganizerManagerData::parseIdString(d->managerUri(), &managerName, &params))
        engineIdString = d->toString();

    return QOrganizerManagerData::buildIdString(managerName, params, &engineIdString);
}

/*!
    Deserializes the given \a idString. Returns a default-constructed (null)
    organizer item ID if the given \a idString is not a valid, serialized organizer item ID,
    or if the manager engine from which the ID came could not be found.

    \sa toString()
*/
QOrganizerItemId QOrganizerItemId::fromString(const QString &idString)
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    if (!QOrganizerManagerData::parseIdString(idString, &managerName, &params, &engineIdString))
        return QOrganizerItemId(); // invalid idString given.

    QOrganizerItemEngineId *engineId = QOrganizerManagerData::createEngineItemId(managerName, params, engineIdString);
    return QOrganizerItemId(engineId);
}

QT_END_NAMESPACE_ORGANIZER
