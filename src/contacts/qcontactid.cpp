/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactid.h"
#include "qcontactmanager_p.h"
#include "qcontactengineid.h"
#include <QHash>
#include <QDebug>
#include <QDataStream>

QT_BEGIN_NAMESPACE
template<> QTCONTACTS_PREPEND_NAMESPACE(QContactEngineId) *QSharedDataPointer<QTCONTACTS_PREPEND_NAMESPACE(QContactEngineId)>::clone()
{
    return d ? d->clone() : 0;
}
QT_END_NAMESPACE

QTCONTACTS_BEGIN_NAMESPACE

/*!
  \class QContactId

  \inmodule QtContacts

  \brief The QContactId class provides information that uniquely identifies
  a contact in a particular manager.

  It consists of a manager URI which identifies the manager which contains the contact,

  A QContactId is "null" when it is default-constructed and therefore not associated with a contact in any manager.
*/

/*!
 * Constructs a new contact id. The contact is said to be "null",
   it has null engine id and isNull() for it returns true. This is the default constructor

   \sa isNull()
 */
QContactId::QContactId()
        : d(0)
{
}

/*!
 * Cleans up the memory in use by the contact id
 */
QContactId::~QContactId()
{
}

/*! Constructs a new contact id as a copy of \a other */
QContactId::QContactId(const QContactId &other)
        : d(other.d)
{
}

/*!
  Constructs a manager-unique id which wraps the given engine-unique contact id
  \a engineId.  This id takes ownership of the engine-unique contact id and
  will delete it when the id goes out of scope.  Engine implementors must not
  delete the \a engineId or undefined behaviour will occur.
  The created contact id is not null, if the \a engineId is not null.
 */
QContactId::QContactId(QContactEngineId *engineId)
    : d(engineId)
{
}

/*! Assigns the contact id to be equal to \a other
*/
QContactId& QContactId::operator=(const QContactId &other)
{
    d = other.d;
    return *this;
}

/*! Returns true if the \a other contact id has the same manager URI
    and they have equal engine ids. Returns true also, if both are null contact ids.
*/
bool QContactId::operator==(const QContactId &other) const
{
    // if both ids are null then they are equal.
    if (d == 0 && other.d == 0)
        return true;

    if (d && other.d) {
        // ensure they're of the same type (and therefore comparable)
        if (d->managerUri() == other.d->managerUri())
            return d->isEqualTo(other.d);
    }

    return false;
}

/*! Returns true if either the manager URI or id of the contact id is different to that of \a other
*/
bool QContactId::operator!=(const QContactId &other) const
{
    return !(*this == other);
}

/*! Returns true if this id is less than the \a other id.
    This id will be considered less than the \a other id if the
    manager URI of this id is alphabetically less than the manager
    URI of the \a other id.  If both ids have the same manager URI,
    this id will be considered less than the \a other id if the
    engine id of this id is less than the engine id of the \a other id.

    The null contact id is less than any non-null id.

    This operator is provided primarily to allow use of a QContactId
    as a key in a QMap.
 */
bool QContactId::operator<(const QContactId &other) const
{

    // a null id is always less than a non-null id.
    if (d == 0 && other.d != 0)
        return true;

    if (d && other.d) {
        // ensure they're of the same type (and therefore comparable)
        if (d->managerUri() == other.d->managerUri())
            return d->isLessThan(other.d);

        // not the same type?  just compare the manager uri.
        return d->managerUri() < other.d->managerUri();
    }

    return false;
}

/*!
 * Returns the hash value for \a key.
 */
uint qHash(const QContactId &key)
{
    if (key.d)
        return key.d->hash();
    return 0;
}


/*!
Returns true if this is the null (default constructed) id which has a null engine id;
 */
bool QContactId::isNull() const
{
    return d == 0;
}

/*!
  Escapes the parameters for inclusion to URIs
 */
QString QContactId::escapeUriParam(const QString &param)
{
    QString rich;
    const int len = param.length();
    rich.reserve(int(len * 1.1));
    for (int i = 0; i < len; ++i) {
        if (param.at(i) == QLatin1Char(':'))
            rich += QStringLiteral("&#58;");
        else if (param.at(i) == QLatin1Char('='))
            rich += QStringLiteral("&equ;");
        else if (param.at(i) == QLatin1Char('&'))
            rich += QStringLiteral("&amp;");
        else
            rich += param.at(i);
    }
    rich.squeeze();
    return rich;
}

/*!
  Parses the individual components of the given \a idString and fills the \a managerName, \a params and \a engineIdString.
  Returns true if the parts could be parsed successfully, false otherwise.
 */
bool QContactId::parseIdString(const QString &idString, QString* managerName, QMap<QString, QString> *params, QString *engineIdString)
{
    QStringList colonSplit = idString.split(QLatin1Char(':'));

    QString prefix = colonSplit.value(0);
    if (prefix != QStringLiteral("qtcontacts") || colonSplit.size() != 4)
        return false; // invalid serialized string.  we cannot continue.

    QString mgrName = colonSplit.value(1);
    QString paramString = colonSplit.value(2);
    QString engIdString = colonSplit.value(3);

    // Now we have to decode each parameter
    QMap<QString, QString> outParams;
    if (!paramString.isEmpty()) {
        QStringList params = paramString.split(QStringLiteral("&(?!(amp;|equ;))"), QString::KeepEmptyParts);
        // If we have an empty string for paramstring, we get one entry in params,
        // so skip that case.
        for (int i = 0; i < params.count(); i++) {
            /* This should be something like "foo&amp;bar&equ;=grob&amp;" */
            QStringList paramChunk = params.value(i).split(QStringLiteral("="), QString::KeepEmptyParts);

            if (paramChunk.count() != 2)
                return false;

            QString arg = paramChunk.value(0);
            QString param = paramChunk.value(1);

            arg.replace(QStringLiteral("&#58;"), QStringLiteral(":"));
            arg.replace(QStringLiteral("&equ;"), QStringLiteral("="));
            arg.replace(QStringLiteral("&amp;"), QStringLiteral("&"));
            param.replace(QStringLiteral("&#58;"), QStringLiteral(":"));
            param.replace(QStringLiteral("&equ;"), QStringLiteral("="));
            param.replace(QStringLiteral("&amp;"), QStringLiteral("&"));
            if (arg.isEmpty())
                return false;
            outParams.insert(arg, param);
        }
    }

    // and unescape the engine id string.
    engIdString.replace(QStringLiteral("&#58;"), QStringLiteral(":"));
    engIdString.replace(QStringLiteral("&amp;"), QStringLiteral("&"));

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
 * Returns the URI of the manager which contains the contact identified by this id
 */
QString QContactId::managerUri() const
{
    return d ? d->managerUri() : QString();
}

/*!
   Returns the contact id as a string. This string can be converted back to equal contact id
   using fromString.

  \sa fromString()
*/
QString QContactId::toString() const
{
    // rely on engine id to supply the full manager uri
    if (d) {
        QString result(QStringLiteral("%1:%2"));
        QString managerUri = d->managerUri();
        QString escapedEngineId = QContactId::escapeUriParam(d->toString());
        return result.arg(managerUri, escapedEngineId);
    }
    return QString(QStringLiteral("qtcontacts:::"));
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QContactId &id)
{
    dbg.nospace() << "QContactId(";
    if (id.isNull())
        dbg.nospace() << "(null))";
    else
        id.d->debugStreamOut(dbg)  << ")";
    return dbg.maybeSpace();
}
#endif

#ifndef QT_NO_DATASTREAM
QDataStream& operator<<(QDataStream &out, const QContactId &contactId)
{
    out << (contactId.toString());
    return out;
}

QDataStream& operator>>(QDataStream &in, QContactId &id)
{
    QString idString;
    in >> idString;
    id = QContactId::fromString(idString);
    return in;
}
#endif

/*!
  Deserializes the given \a idString.  Returns a default-constructed (null)
  contact id if the given \a idString is not a valid, serialized contact id, or
  if the manager engine from which the id came could not be found.

  \sa toString()
 */
QContactId QContactId::fromString(const QString &idString)
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    if (!QContactId::parseIdString(idString, &managerName, &params, &engineIdString))
        return QContactId(); // invalid idString given.

    QContactEngineId* engineId = QContactManagerData::createEngineContactId(managerName, params, engineIdString);
    return QContactId(engineId);
}

QTCONTACTS_END_NAMESPACE
