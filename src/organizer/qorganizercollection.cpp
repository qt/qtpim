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

#include "qorganizercollection.h"
#include "qorganizercollection_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerCollection
    \brief The QOrganizerCollection class represents a collection of items in a manager.
    \inmodule QtOrganizer
    \ingroup organizer-main

    A collection has an ID and optionally some metadata, and contains zero or more items.
    Each different manager will have different requirements before a collection may be saved
    in it.  Some managers do not allow collections to be saved at all, while others may require
    a collection to have some minimal amount of meta data defined in it prior to save.
    For example, most managers require a valid value for the QOrganizerCollection::KeyName
    meta data key to be set prior to save.

    Every QOrganizerItem is contained within a collection when stored in a manager.
    To save an item in a collection, the client should call QOrganizerItem::setCollectionId()
    on the item, passing in the ID of the destination collection as the argument, and then
    save the item in the manager.  To move an item from one collection to another, the client
    must fetch the item from the manager, set the collection ID in the item to the ID of the
    collection to which the client wishes the item to be moved, and then resave the item in the
    manager.  That is, the collection which an item is part of is treated as a property of the
    item.
 */

/*!
    \enum QOrganizerCollection::MetaDataKey

    This enumeration describes the key of the organizer collection metadata.

    \value KeyName         This metadata describes the name of the collection.
    \value KeyDescription  This metadata gives a description of the collection.
    \value KeyColor        This metadata describes the color of the collection.
    \value KeySecondaryColor  This metadata describes the secondary color of the collection.
    \value KeyImage        This metadata describes the image of the collection.
    \value KeyExtended     This is an extened metadata, which is stored as a QVariantMap.
 */

/*!
    Constructs a new collection.
 */
QOrganizerCollection::QOrganizerCollection()
    : d(new QOrganizerCollectionData)
{
}

/*!
    Cleans up any memory in use by the collection.
 */
QOrganizerCollection::~QOrganizerCollection()
{
}

/*!
    Constructs a new copy of the \a other collection.
 */
QOrganizerCollection::QOrganizerCollection(const QOrganizerCollection &other)
    : d(other.d)
{
}

/*!
    Assigns this collection to be equal to the \a other collection.
 */
QOrganizerCollection &QOrganizerCollection::operator=(const QOrganizerCollection &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns true if the collection is the same as that of the \a other collection, false if either
    the ID or any of the stored metadata are not the same.
 */
bool QOrganizerCollection::operator==(const QOrganizerCollection &other) const
{
    if (d == other.d)
        return true;

    if (d->m_id != other.d->m_id
        || d->m_metaData.size() != other.d->m_metaData.size()) {
        return false;
    }

    QMap<QOrganizerCollection::MetaDataKey, QVariant>::const_iterator i = d->m_metaData.constBegin();
    while (i != d->m_metaData.constEnd()) {
        if (i.value() != other.d->m_metaData.value(i.key()))
            return false;
        ++i;
    }

    return true;
}

/*!
    \fn QOrganizerCollection::operator!=(const QOrganizerCollection &other) const

    Returns true if the collection is not the same as the \a other collection.
 */

/*!
    Returns the ID of the collection.
 */
QOrganizerCollectionId QOrganizerCollection::id() const
{
    return d->m_id;
}

/*!
    Sets the ID of the collection to \a id.

    If the ID is set to a null (default-constructed) ID, saving the collection will cause the manager
    to save the collection as a new collection.
 */
void QOrganizerCollection::setId(const QOrganizerCollectionId &id)
{
    d->m_id = id;
}

/*!
    Sets the metadata of the collection to be \a metaData.
 */
void QOrganizerCollection::setMetaData(const QMap<QOrganizerCollection::MetaDataKey, QVariant> &metaData)
{
    d->m_metaData = metaData;
}

/*!
    Returns the meta data of the collection.
 */
QMap<QOrganizerCollection::MetaDataKey, QVariant> QOrganizerCollection::metaData() const
{
    return d->m_metaData;
}

/*!
    Sets the meta data of the collection for the given \a key to the given \a value.
 */
void QOrganizerCollection::setMetaData(MetaDataKey key, const QVariant &value)
{
    d->m_metaData.insert(key, value);
}

/*!
    Sets the value of the extended metadata with the given \a key to \a value.
 */
void QOrganizerCollection::setExtendedMetaData(const QString &key, const QVariant &value)
{
    QVariantMap variantMap = d->m_metaData.value(QOrganizerCollection::KeyExtended).toMap();
    variantMap.insert(key, value);
    d->m_metaData.insert(QOrganizerCollection::KeyExtended, variantMap);
}

/*!
    Returns the value of extended metadata with the given \a key.
 */
QVariant QOrganizerCollection::extendedMetaData(const QString &key) const
{
    return d->m_metaData.value(QOrganizerCollection::KeyExtended).toMap().value(key);
}

/*!
    Returns the meta data of the collection for the given \a key.
 */
QVariant QOrganizerCollection::metaData(MetaDataKey key) const
{
    return d->m_metaData.value(key);
}

/*!
    \relates QOrganizerCollection
    Returns the hash value for \a key.
 */
Q_ORGANIZER_EXPORT uint qHash(const QOrganizerCollection &key)
{
    uint hash = qHash(key.id());
    QMap<QOrganizerCollection::MetaDataKey, QVariant>::const_iterator i = key.d->m_metaData.constBegin();
    while (i != key.d->m_metaData.constEnd()) {
        if (i.key() == QOrganizerCollection::KeyExtended) {
            QVariantMap variantMap = i.value().toMap();
            QVariantMap::const_iterator j = variantMap.constBegin();
            while (j != variantMap.constEnd()) {
                hash += QT_PREPEND_NAMESPACE(qHash)(j.key()) + QT_PREPEND_NAMESPACE(qHash)(j.value().toString());
                ++j;
            }
        } else {
            hash += QT_PREPEND_NAMESPACE(qHash)(i.key()) + QT_PREPEND_NAMESPACE(qHash)(i.value().toString());
        }
        ++i;
    }
    return hash;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerCollection
    Streams the \a collection to the given debug stream \a dbg, and returns the stream.
 */
QDebug operator<<(QDebug dbg, const QOrganizerCollection& collection)
{
    dbg.nospace() << "QOrganizerCollection(id=" << collection.id();

    QMap<QOrganizerCollection::MetaDataKey, QVariant> metaData = collection.metaData();
    QMap<QOrganizerCollection::MetaDataKey, QVariant>::const_iterator i = metaData.constBegin();
    while (i != metaData.constEnd()) {
        dbg.nospace() << ", " << i.key() << '=' << i.value();
        ++i;
    }
    dbg.nospace() << ')';
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerCollection
    Writes \a collection to the stream \a out.
 */
QDataStream &operator<<(QDataStream &out, const QOrganizerCollection &collection)
{
    quint8 formatVersion = 1;
    return out << formatVersion
               << collection.id().toString()
               << collection.metaData();
}

/*!
    \relates QOrganizerCollection
    Reads an organizer collection from stream \a in into \a collection.
 */
QDataStream &operator>>(QDataStream &in, QOrganizerCollection &collection)
{
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        QString idString;
        QMap<int, QVariant> values;
        in >> idString >> values;

        collection = QOrganizerCollection();
        collection.setId(QOrganizerCollectionId::fromString(idString));

        QMap<int, QVariant>::const_iterator i = values.constBegin();
        while (i != values.constEnd()) {
            collection.setMetaData(static_cast<QOrganizerCollection::MetaDataKey>(i.key()), i.value());
            ++i;
        }
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif // QT_NO_DATASTREAM

QT_END_NAMESPACE_ORGANIZER
