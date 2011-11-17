/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qorganizeritemdetail.h>
#include <private/qorganizeritemdetail_p.h>
#include <qorganizeritemrecurrence.h>

#include <qdebug.h>
#include <qdatastream.h>

QTORGANIZER_BEGIN_NAMESPACE

/* Initialise our static private data member */
QAtomicInt QOrganizerItemDetailPrivate::lastDetailKey(1);

/*!
    \class QOrganizerItemDetail

    \brief The QOrganizerItemDetail class represents a single, complete detail about an organizer item.
    \inmodule QtOrganizer
    \ingroup organizer-main

    All of the information for an organizer item is stored in one or more QOrganizerItemDetail objects.

    A detail is a group of logically related bits of data - for example, a QOrganizerItemTimestamp is a single
    detail that has multiple fields (timestamp of creation, timestamp of last update, etc). Different organizer
    managers may support different details for different item types, e.g. certain manager may not support the
    timestamp, while others do.

    When a QOrganizerItemDetail has been retrieved in a QOrganizerItem from a QOrganizerManager, it may have certain
    access constraints provided with it, like \l ReadOnly or \l Irremovable.  This might mean that the
    supplied detail is calculated or otherwise not modifiable by the user.
    Also, some details may be marked \l Irremovable.  These are typically things that
    an organizer item has to have - like a QOrganizerItemType.

    In general, QOrganizerItemDetail and the built in subclasses (like \l QOrganizerEventTime) provide
    convenience and standardized access to values. For example, \l QOrganizerEventTime provides a
    convenient API for manipulating a QOrganizerItemDetail to describe the start and end time of an
    event. Subclasses also provide constants for the names of fields (like \l QOrganizerEventTime::FieldStartDateTime).
    Typically the constants for field names start with \c Field, and the constants for predefined values
    of a field start with the name of that field (e.g. \c TypeEvent is a predefined constant for \c FieldType).

    QOrganizerItemDetail objects act like type checked values.  In general, you can assign them
    to and fro and have reasonable behaviour, like the following example.

    \code
    QOrganizerItemDescription description;
    description.setDescription("Some descriptive text");
    // description.value(QOrganizerItemDescription::FieldDescription) == "Some descriptive text";
    // description.definitionName() == QOrganizerItemDescription::DefinitionName

    QOrganizerItemDetail detail = description;
    // detail.value(QOrganizerItemDescription::FieldDescription) == "Some descriptive text";
    // detail.definitionName() == QOrganizerItemDescription::DefinitionName

    QOrganizerItemDescription otherDescription = detail;
    // otherDescription.description() == "Some descriptive text";
    // otherDescription.definitionName() == QOrganizerItemDescription::DefinitionName

    QOrganizerItemDisplayLabel label = detail;
    // label is now a default constructed QOrganizerItemDisplayLabel
    // label.value(QOrganizerItemDescription::FieldDescription) is empty
    // label.definitionName() == QOrganizerItemDisplayLabel::DefinitionName

    QOrganizerItemDisplayLabel otherLabel = description;
    // otherLabel is now a default constructed QOrganizerItemDisplayLabel
    // otherLabel.value(QOrganizerItemDescription::FieldDescription) is empty
    // otherLabel.definitionName() == QOrganizerItemDisplayLabel::DefinitionName
    \endcode

    \sa QOrganizerItem, QOrganizerItemDetailFilter, QOrganizerItemDetailRangeFilter
 */

/*!
    \internal
    \macro Q_DECLARE_CUSTOM_ORGANIZER_DETAIL
    \relates QOrganizerItemDetail

    Macro for simplifying declaring leaf detail classes.

    The first argument is the name of the class, and the second argument is the
    detail definition name.

    If you are creating a leaf detail class for a type of QOrganizerItemDetail,
    you should use this macro when declaring your class to ensure that
    it interoperates with other organizer item functionality.
 */


/*!
    \fn bool QOrganizerItemDetail::operator!=(const QOrganizerItemDetail &other) const

    Returns true if the values or id of this detail is different to those of the \a other detail
 */

/*!
    Constructs a new, empty detail.
 */
QOrganizerItemDetail::QOrganizerItemDetail()
    : d(new QOrganizerItemDetailPrivate)
{
}

/*!
    Constructs a new, empty detail of the definition identified by \a thisDefinitionId.
    The definitionId must be restricted to the Latin 1 character set.
 */
QOrganizerItemDetail::QOrganizerItemDetail(const QString &thisDefinitionId)
    : d(new QOrganizerItemDetailPrivate)
{
    d->m_definitionName = thisDefinitionId;
}

/*!
    Constructs a detail that is a copy of \a other.
 */
QOrganizerItemDetail::QOrganizerItemDetail(const QOrganizerItemDetail &other)
    : d(other.d)
{
}

/*!
    \internal

    Constructs a detail that is a copy of \a other if \a other is of the expected definition
    identified by \a expectedDefinitionId, else constructs a new, empty detail of the
    definition identified by the \a expectedDefinitionId.
*/
QOrganizerItemDetail::QOrganizerItemDetail(const QOrganizerItemDetail &other, const QString &expectedDefinitionId)
{
    if (other.d->m_definitionName == expectedDefinitionId) {
        d = other.d;
    } else {
        d = new QOrganizerItemDetailPrivate;
        d->m_definitionName = expectedDefinitionId;
    }
}

/*!
    Assigns this detail to \a other.
 */
QOrganizerItemDetail &QOrganizerItemDetail::operator=(const QOrganizerItemDetail &other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*!
    \internal

    Assigns this detail to \a other if the definition of \a other is that identified
    by the given \a expectedDefinitionId, else assigns this detail to be a new, empty
    detail of the definition identified by the given \a expectedDefinitionId.
 */
QOrganizerItemDetail &QOrganizerItemDetail::assign(const QOrganizerItemDetail &other, const QString &expectedDefinitionId)
{
    if (this != &other) {
        if (other.d->m_definitionName == expectedDefinitionId) {
            d = other.d;
        } else {
            d = new QOrganizerItemDetailPrivate;
            d->m_definitionName = expectedDefinitionId;
        }
    }
    return *this;
}

/*!
    Frees the memory used by this detail.
 */
QOrganizerItemDetail::~QOrganizerItemDetail()
{
}

/*!
    Returns the (unique) name of the definition which defines the semantics and structure of this detail.
    The actual QOrganizerItemDetailDefinition should be retrieved from the relevant QOrganizerManager using this name.
 */
QString QOrganizerItemDetail::definitionName() const
{
    return d.constData()->m_definitionName;
}

/*!
    Compares this detail to \a other.  Returns true if the definition, access constraints and values of \a other are equal to those of this detail.
    The keys of each detail are not considered during the comparison, in order to allow details from different organizer items to
    be compared according to their values.
 */
bool QOrganizerItemDetail::operator==(const QOrganizerItemDetail &other) const
{
    if (!(d.constData()->m_definitionName == other.d.constData()->m_definitionName))
        return false;

    if (d.constData()->m_access != other.d.constData()->m_access)
        return false;

    // QVariant doesn't support == on QOrganizerItemRecurrence - do it manually
    if (d.constData()->m_definitionName == QOrganizerItemRecurrence::DefinitionName)
        return static_cast<QOrganizerItemRecurrence>(*this) == static_cast<QOrganizerItemRecurrence>(other);

    if (d.constData()->m_values != other.d.constData()->m_values)
        return false;

    return true;
}

bool compareOrganizerItemDetail(const QOrganizerItemDetail &one, const QOrganizerItemDetail &other)
{
    return (one.definitionName() < other.definitionName());
}

/*!
    Returns the hash value for \a key.
 */
uint qHash(const QOrganizerItemDetail &key)
{
    const QOrganizerItemDetailPrivate *dptr= QOrganizerItemDetailPrivate::detailPrivate(key);
    uint hash = qHash(dptr->m_definitionName) + QT_PREPEND_NAMESPACE(qHash)(dptr->m_access);
    QHash<QString, QVariant>::const_iterator it = dptr->m_values.constBegin();
    while (it != dptr->m_values.constEnd()) {
        hash += qHash(it.key()) + QT_PREPEND_NAMESPACE(qHash)(it.value().toString());
        ++it;
    }
    return hash;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QOrganizerItemDetail &detail)
{
    dbg.nospace() << "QOrganizerItemDetail(name=" << detail.definitionName() << ", key=" << detail.key();
    QVariantMap fields = detail.values();
    QVariantMap::const_iterator it;
    for (it = fields.constBegin(); it != fields.constEnd(); ++it)
        dbg.nospace() << ", " << it.key() << '=' << it.value();
    dbg.nospace() << ')';
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    Writes \a detail to the stream \a out.
 */
QDataStream &operator<<(QDataStream &out, const QOrganizerItemDetail &detail)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QOrganizerItemDetail
    return out << formatVersion
               << detail.definitionName()
               << static_cast<quint32>(detail.accessConstraints())
               << detail.values();
}

/*!
    Reads aan organizer item detail from stream \a in into \a detail.
 */
QDataStream &operator>>(QDataStream &in, QOrganizerItemDetail &detail)
{
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        QString definitionName;
        quint32 accessConstraintsInt;
        QVariantMap values;
        in >> definitionName >> accessConstraintsInt >> values;

        detail = QOrganizerItemDetail(definitionName);
        QOrganizerItemDetail::AccessConstraints accessConstraints(accessConstraintsInt);
        detail.d->m_access = accessConstraints;

        QMapIterator<QString, QVariant> it(values);
        while (it.hasNext()) {
            it.next();
            detail.setValue(it.key(), it.value());
        }
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif // QT_NO_DATASTREAM

/*!
    Returns true if no values are contained in this detail.  Note that context is stored as a value; hence, if a context is set, this function will return false.
 */
bool QOrganizerItemDetail::isEmpty() const
{
    return (d.constData()->m_values.isEmpty());
}

/*!
    Returns the key of this detail.
 */
int QOrganizerItemDetail::key() const
{
    return d->m_id;
}

/*!
    Causes the implicitly-shared detail to be detached from any other copies, and generates a new key for it.
    This ensures that calling QOrganizerItem::saveDetail() will result in a new detail being saved, rather than
    another detail being updated.
 */
void QOrganizerItemDetail::resetKey()
{
    d->m_id = QOrganizerItemDetailPrivate::lastDetailKey.fetchAndAddOrdered(1);
}

/*!
    Returns the value stored in this detail for the given \a key as a QVariant, or an invalid QVariant if no value for the given \a key exists
 */
QVariant QOrganizerItemDetail::value(const QString &key) const
{
    return d.constData()->m_values.value(key);
}

/*!
    Returns true if this detail has a field with the given \a key, or false otherwise.
 */
bool QOrganizerItemDetail::hasValue(const QString &key) const
{
    return d.constData()->m_values.contains(key);
}

/*!
    Inserts \a value into the detail for the given \a key if \a value is valid.  If \a value is invalid,
    removes the field with the given \a key from the detail.  Returns true if the given \a value was set
    for the \a key (if the \a value was valid), or if the given \a key was removed from detail (if the
    \a value was invalid), and returns false if the key was unable to be removed (and the \a value was invalid).
 */
bool QOrganizerItemDetail::setValue(const QString &key, const QVariant &value)
{
    if (!value.isValid())
        return removeValue(key);

    d->m_values.insert(key, value);
    return true;
}

/*!
    Removes the value stored in this detail for the given \a key.  Returns true if a value was stored
    for the given \a key and the operation succeeded, and false otherwise.
 */
bool QOrganizerItemDetail::removeValue(const QString &key)
{
    return d->m_values.remove(key);
}

/*!
    Returns the values stored in this detail as a map from value key to value.
 */
QVariantMap QOrganizerItemDetail::values() const
{
    QVariantMap ret;
    QHash<QString, QVariant>::const_iterator it = d.constData()->m_values.constBegin();
    while (it != d.constData()->m_values.constEnd()) {
        ret.insert(it.key(), it.value());
        ++it;
    }

    return ret;
}

/*!
    \fn T QOrganizerItemDetail::value(const QString &key) const
    Returns the value of the template type associated with the given \a key.
 */

/*!
    \enum QOrganizerItemDetail::AccessConstraint

    This enum defines the access constraints for a detail.  This information is typically provided by
    the manager when an organizer item is retrieved.

    \value NoConstraint Users can read, write, and otherwise modify this detail in any manner.
    \value ReadOnly Users cannot write or modify values in this detail.
    \value Irremovable Users cannot remove this detail from an organizer item.
 */

/*!
    Returns the access constraints associated with the detail.

    Some details may not be written to, while other details may
    not be removed from an organizer item.

    \sa QOrganizerItemDetail::AccessConstraints
 */
QOrganizerItemDetail::AccessConstraints QOrganizerItemDetail::accessConstraints() const
{
    return d.constData()->m_access;
}

QTORGANIZER_END_NAMESPACE
