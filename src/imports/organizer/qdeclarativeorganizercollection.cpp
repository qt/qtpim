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

#include "qdeclarativeorganizercollection_p.h"

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype Collection
    \instantiates QDeclarativeOrganizerCollection
    \brief The Collection element represents a collection of items in an organizer manager.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main
 */

/*!
    \internal
 */
QDeclarativeOrganizerCollection::QDeclarativeOrganizerCollection(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty string Collection::collectionId

    This property holds the ID of the collection.
 */
QString QDeclarativeOrganizerCollection::id() const
{
    return d.id().toString();
}

void QDeclarativeOrganizerCollection::setId(const QString &id)
{
    if (d.id().toString() != id) {
        d.setId(QOrganizerCollectionId::fromString(id));
        emit valueChanged();
    }
}

/*!
    \qmlproperty string Collection::name

    This property holds the name meta data of a collection.
 */
QString QDeclarativeOrganizerCollection::name() const
{
    return metaData(QOrganizerCollection::KeyName).toString();
}

void QDeclarativeOrganizerCollection::setName(const QString &name)
{
    setMetaData(QOrganizerCollection::KeyName, name);
}

/*!
    \qmlproperty string Collection::description

    This property holds the description meta data of a collection.
 */
QString QDeclarativeOrganizerCollection::description() const
{
    return metaData(QOrganizerCollection::KeyDescription).toString();
}

void QDeclarativeOrganizerCollection::setDescription(const QString &description)
{
    setMetaData(QOrganizerCollection::KeyDescription, description);
}

/*!
    \qmlproperty color Collection::secondaryColor

    This property holds the secondary color meta data of a collection.
 */
QColor QDeclarativeOrganizerCollection::secondaryColor() const
{
    return metaData(QOrganizerCollection::KeySecondaryColor).value<QColor>();
}

void QDeclarativeOrganizerCollection::setSecondaryColor(const QColor &secondaryColor)
{
    setMetaData(QOrganizerCollection::KeySecondaryColor, secondaryColor);
}

/*!
    \qmlproperty color Collection::color

    This property holds the color meta data of a collection.
 */
QColor QDeclarativeOrganizerCollection::color() const
{
    return metaData(QOrganizerCollection::KeyColor).value<QColor>();
}

void QDeclarativeOrganizerCollection::setColor(const QColor &color)
{
    setMetaData(QOrganizerCollection::KeyColor, color);
}

/*!
    \qmlproperty url Collection::image

    This property holds the image url meta data of a collection.
 */
QUrl QDeclarativeOrganizerCollection::image() const
{
    return QUrl(metaData(QOrganizerCollection::KeyImage).toString());
}

void QDeclarativeOrganizerCollection::setImage(const QUrl &url)
{
    setMetaData(QOrganizerCollection::KeyImage, url);
}

/*!
    \qmlmethod Collection::setMetaData(key, value)

    Sets the meta data of the collection for the given \a key to the given \a value. Possible keys
    include:
    \list
    \li Collection.KeyName
    \li Collection.KeyDescription
    \li Collection.KeyColor
    \li Collection.KeySecondaryColor
    \li Collection.KeyImage
    \li Collection.KeyExtended
    \endlist
 */
void QDeclarativeOrganizerCollection::setMetaData(QOrganizerCollection::MetaDataKey key, const QVariant &value)
{
    if (metaData(key) != value) {
        d.setMetaData(key, value);
        emit valueChanged();
    }
}

/*!
    \qmlmethod var Collection::metaData(key)

    Returns the meta data stored in this collection for the given \a key. Possible keys include:
    \list
    \li Collection.KeyName
    \li Collection.KeyDescription
    \li Collection.KeyColor
    \li Collection.KeyImage
    \li Collection.KeyExtended
    \endlist
 */
QVariant QDeclarativeOrganizerCollection::metaData(QOrganizerCollection::MetaDataKey key) const
{
    return d.metaData(key);
}

/*!
    \qmlmethod Collection::setExtendedMetaData(key, value)

    Sets the value of the extended metadata with the given \a key to \a value.
 */
void QDeclarativeOrganizerCollection::setExtendedMetaData(const QString &key, const QVariant &value)
{
    if (extendedMetaData(key) != value) {
        d.setExtendedMetaData(key, value);
        emit valueChanged();
    }
}

/*!
    \qmlmethod var Collection::extendedMetaData(key)

    Returns the value of extended metadata with the given \a key.
 */
QVariant QDeclarativeOrganizerCollection::extendedMetaData(const QString &key) const
{
    return d.extendedMetaData(key);
}

/*!
    \internal
 */
QOrganizerCollection QDeclarativeOrganizerCollection::collection() const
{
    return d;
}

/*!
    \internal
 */
void QDeclarativeOrganizerCollection::setCollection(const QOrganizerCollection &collection)
{
    d = collection;
}

#include "moc_qdeclarativeorganizercollection_p.cpp"

QT_END_NAMESPACE
