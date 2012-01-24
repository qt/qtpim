/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "qdeclarativeorganizercollection_p.h"

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass Collection QDeclarativeOrganizerCollection
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
    \qmlmethod Collection::setMetaData(string key, var value)

    Sets the meta data of the collection for the given \a key to the given \a value.
 */
void QDeclarativeOrganizerCollection::setMetaData(const QString &key, const QVariant &value)
{
    if (metaData(key) != value) {
        d.setMetaData(key, value);
        emit valueChanged();
    }
}

/*!
    \qmlmethod var Collection::metaData(string key)

    Returns the meta data stored in this collection for the given \a key.
 */
QVariant QDeclarativeOrganizerCollection::metaData(const QString &key) const
{
    return d.metaData(key);
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

QTORGANIZER_END_NAMESPACE
