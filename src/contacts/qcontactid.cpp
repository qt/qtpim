/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactid.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qcontactengineid.h"
#include "qcontactmanager_p.h"

QT_BEGIN_NAMESPACE
template<> QTCONTACTS_PREPEND_NAMESPACE(QContactEngineId) *QSharedDataPointer<QTCONTACTS_PREPEND_NAMESPACE(QContactEngineId)>::clone()
{
    return d ? d->clone() : 0;
}
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE_CONTACTS

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
 * Returns the URI of the manager which contains the contact identified by this id
 */
QString QContactId::managerUri() const
{
    return d ? d->managerUri() : QString();
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
    Serializes the contact ID to a string. The format of the string will be:
    "qtcontacts:managerName:constructionParams:engineLocalItemId".

    \sa fromString()
*/
QString QContactId::toString() const
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    // rely on engine id to supply the full manager uri
    if (d && QContactManagerData::parseIdString(d->managerUri(), &managerName, &params))
        engineIdString = d->toString();

    return QContactManagerData::buildIdString(managerName, params, &engineIdString);
}

/*!
    Deserializes the given \a idString. Returns a default-constructed (null)
    contact ID if the given \a idString is not a valid, serialized contact ID,
    or if the manager engine from which the ID came could not be found.

    \sa toString()
*/
QContactId QContactId::fromString(const QString &idString)
{
    QString managerName;
    QMap<QString, QString> params;
    QString engineIdString;

    if (!QContactManagerData::parseIdString(idString, &managerName, &params, &engineIdString))
        return QContactId(); // invalid idString given.

    QContactEngineId* engineId = QContactManagerData::createEngineContactId(managerName, params, engineIdString);
    return QContactId(engineId);
}

QT_END_NAMESPACE_CONTACTS
