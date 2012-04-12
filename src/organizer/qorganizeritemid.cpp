/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include <qorganizeritemid.h>
#include <qorganizeritemengineid.h>
#include <private/qorganizermanager_p.h>

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#include <QtCore/qstringlist.h>

#if !defined(Q_CC_MWERKS)
QT_BEGIN_NAMESPACE
template<> QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemEngineId) *QSharedDataPointer<QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemEngineId)>::clone()
{
    return d ? d->clone() : 0;
}
QT_END_NAMESPACE
#endif

QTORGANIZER_BEGIN_NAMESPACE

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
    if (d == 0 && other.d == 0)
        return true;

    if (d && other.d) {
        if (d->managerUri() == other.d->managerUri())
            return d->isEqualTo(other.d);
    }
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
  \internal

  Escape the id string
 */
QString escapeIdString(const QString &string)
{
    QString rich;
    const int len = string.length();
    rich.reserve(int(len * 1.5));
    for (int i = 0; i < len; ++i) {
        if (string.at(i) == QLatin1Char(':'))
            rich += QLatin1String("&#58;");
        else if (string.at(i) == QLatin1Char('='))
            rich += QLatin1String("&equ;");
        else if (string.at(i) == QLatin1Char('&'))
            rich += QLatin1String("&amp;");
        else
            rich += string.at(i);
    }
    rich.squeeze();
    return rich;
}

/*!
    \internal

    Builds a string from the given \a managerName, \a params and \a engineIdString.
 */
inline QString buildIdString(const QString &managerName, const QMap<QString, QString> &params, const QString &engineIdString)
{
    // the constructed id string will be of the form: "qtcontacts:managerName:param1=value1&param2=value2:
    QString ret(QStringLiteral("qtorganizer:%1:%2:%3"));

    // we have to escape each param
    QStringList escapedParams;
    QString arg;
    foreach (const QString &key, params.keys()) {
        arg = params.value(key);
        escapedParams.append(escapeIdString(key) + QLatin1Char('=') + escapeIdString(arg));
    }

    // and we escape the engine id string.
    QString escapedEngineId = escapeIdString(engineIdString);

    return ret.arg(managerName, escapedParams.join(QStringLiteral("&")), escapedEngineId);
}

/*!
    \internal

    Parses the individual components of the given \a idString and fills the \a managerName, \a params
    and \a engineIdString. Returns true if the parts could be parsed successfully, false otherwise.
 */
inline bool parseIdString(const QString &idString, QString *managerName, QMap<QString, QString> *params, QString *engineIdString)
{
    QStringList colonSplit = idString.split(QLatin1Char(':'));

    QString prefix = colonSplit.value(0);
    if (prefix != QLatin1String("qtorganizer") || colonSplit.size() != 4)
        return false; // invalid serialized string.  we cannot continue.

    QString mgrName = colonSplit.value(1);
    QString paramString = colonSplit.value(2);
    QString engIdString = colonSplit.value(3);

    // Now we have to decode each parameter
    QMap<QString, QString> outParams;
    if (!paramString.isEmpty()) {
        QStringList params = paramString.split(QRegExp(QLatin1String("&(?!(amp;|equ;))")), QString::KeepEmptyParts);
        // If we have an empty string for paramstring, we get one entry in params,
        // so skip that case.
        for(int i = 0; i < params.count(); i++) {
            /* This should be something like "foo&amp;bar&equ;=grob&amp;" */
            QStringList paramChunk = params.value(i).split(QLatin1String("="), QString::KeepEmptyParts);

            if (paramChunk.count() != 2)
                return false;

            QString arg = paramChunk.value(0);
            QString param = paramChunk.value(1);

            arg.replace(QLatin1String("&#58;"), QLatin1String(":"));
            arg.replace(QLatin1String("&equ;"), QLatin1String("="));
            arg.replace(QLatin1String("&amp;"), QLatin1String("&"));
            param.replace(QLatin1String("&#58;"), QLatin1String(":"));
            param.replace(QLatin1String("&equ;"), QLatin1String("="));
            param.replace(QLatin1String("&amp;"), QLatin1String("&"));
            if (arg.isEmpty())
                return false;
            outParams.insert(arg, param);
        }
    }

    // and unescape the engine id string.
    engIdString.replace(QLatin1String("&#58;"), QLatin1String(":"));
    engIdString.replace(QLatin1String("&amp;"), QLatin1String("&"));

    // now fill the return values.
    if (managerName)
        *managerName = mgrName;
    if (params)
        *params = outParams;
    if (engineIdString)
        *engineIdString = engIdString;

    // and return.
    return true;
}

/*!
    Serializes the ID to a string.  The format of the string will be:
    "qtorganizer:managerName:constructionParams:serializedEngineLocalItemId"
 */
QString QOrganizerItemId::toString() const
{
    QString mgrName;
    QMap<QString, QString> params;
    QString engineId;

    if (d) {
        QOrganizerManager::parseUri(d->managerUri(), &mgrName, &params);
        engineId = d->toString();
    }

    // having extracted the params the name, we now need to build a new string.
    return buildIdString(mgrName, params, engineId);
}

/*!
    Deserializes the given \a idString.  Returns a default-constructed (null) item ID if the given
    \a idString is not a valid, serialized item ID, or if the manager engine from which the ID came
    could not be found.
 */
QOrganizerItemId QOrganizerItemId::fromString(const QString &idString)
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    if (!parseIdString(idString, &managerName, &params, &engineIdString))
        return QOrganizerItemId(); // invalid idString given.

    QOrganizerItemEngineId *engineId = QOrganizerManagerData::createEngineItemId(managerName, params, engineIdString);
    return QOrganizerItemId(engineId);
}

QTORGANIZER_END_NAMESPACE
