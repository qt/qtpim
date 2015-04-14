/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2015 Canonical Ltd
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactcollectionchangeset.h"
#include "qcontactcollectionchangeset_p.h"

#include "qcontactmanagerengine.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
    \class QContactCollectionChangeSet
    \brief The QContactCollectionChangeSet class provides a simple API to simplify the emission
           of state-change signals for collections from QContactManagerEngine implementations.
    \inmodule QtContacts
    \ingroup contacts-main

    This class should only be used by backend developers.
 */

/*!
    Constructs a new change set.
 */
QContactCollectionChangeSet::QContactCollectionChangeSet()
    : d(new QContactCollectionChangeSetData)
{
}

/*!
    Constructs a copy of the \a other change set.
 */
QContactCollectionChangeSet::QContactCollectionChangeSet(const QContactCollectionChangeSet &other)
    : d(other.d)
{
}

/*!
    Frees the memory used by this change set.
 */
QContactCollectionChangeSet::~QContactCollectionChangeSet()
{
}

/*!
    Assigns this change set to be equal to \a other.
 */
QContactCollectionChangeSet &QContactCollectionChangeSet::operator=(const QContactCollectionChangeSet &other)
{
    d = other.d;
    return *this;
}

/*!
    Sets the data changed flag to \a dataChanged.

    If this is set to true prior to calling emitSignals(), only the QContactManagerEngine::dataChanged()
    signal will be emitted; otherwise, the appropriate finer-grained signals will be emitted.
 */
void QContactCollectionChangeSet::setDataChanged(bool dataChanged)
{
    d->m_dataChanged = dataChanged;
}

/*!
    Returns the value of the data changed flag.
 */
bool QContactCollectionChangeSet::dataChanged() const
{
    return d->m_dataChanged;
}

/*!
    Returns the set of IDs of collections which have been added to the database.
 */
QSet<QContactCollectionId> QContactCollectionChangeSet::addedCollections() const
{
    return d->m_addedCollections;
}

/*!
    Inserts the given \a collectionId into the set of IDs of collections which have been added to
    the database.
 */
void QContactCollectionChangeSet::insertAddedCollection(const QContactCollectionId &collectionId)
{
    d->m_addedCollections.insert(collectionId);
    d->m_modifiedCollections.append(QPair<QContactCollectionId, QContactManager::Operation>(collectionId, QContactManager::Add));
}

/*!
    Inserts each of the given \a collectionIds into the set of IDs of collections which have been
    added to the database.
 */
void QContactCollectionChangeSet::insertAddedCollections(const QList<QContactCollectionId> &collectionIds)
{
    foreach (const QContactCollectionId &id, collectionIds) {
        d->m_addedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QContactCollectionId, QContactManager::Operation>(id, QContactManager::Add));
    }
}

/*!
    Clears the set of IDs of collections which have been added to the database.
 */
void QContactCollectionChangeSet::clearAddedCollections()
{
    d->m_addedCollections.clear();
}

/*!
    Returns the set of IDs of collections which have been changed in the database.
 */
QSet<QContactCollectionId> QContactCollectionChangeSet::changedCollections() const
{
    return d->m_changedCollections;
}

/*!
    Inserts the given \a collectionId into the set of IDs of collections which have been changed in
    the database.
 */
void QContactCollectionChangeSet::insertChangedCollection(const QContactCollectionId &collectionId)
{
    d->m_changedCollections.insert(collectionId);
    d->m_modifiedCollections.append(QPair<QContactCollectionId, QContactManager::Operation>(collectionId, QContactManager::Change));
}

/*!
    Inserts each of the given \a collectionIds into the set of IDs of collections which have been
    changed in the database.
 */
void QContactCollectionChangeSet::insertChangedCollections(const QList<QContactCollectionId> &collectionIds)
{
    foreach (const QContactCollectionId &id, collectionIds) {
        d->m_changedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QContactCollectionId, QContactManager::Operation>(id, QContactManager::Change));
    }
}

/*!
    Clears the set of IDs of collections which have been changed in the database.
 */
void QContactCollectionChangeSet::clearChangedCollections()
{
    d->m_changedCollections.clear();
}

/*!
    Returns the set of IDs of collections which have been removed from the database.
 */
QSet<QContactCollectionId> QContactCollectionChangeSet::removedCollections() const
{
    return d->m_removedCollections;
}

/*!
    Inserts the given \a collectionId into the set of IDs of collections which have been removed from
    the database.
 */
void QContactCollectionChangeSet::insertRemovedCollection(const QContactCollectionId &collectionId)
{
    d->m_removedCollections.insert(collectionId);
    d->m_modifiedCollections.append(QPair<QContactCollectionId, QContactManager::Operation>(collectionId, QContactManager::Remove));
}

/*!
    Inserts each of the given \a collectionIds into the set of IDs of collections which have been
    removed from the database.
 */
void QContactCollectionChangeSet::insertRemovedCollections(const QList<QContactCollectionId> &collectionIds)
{
    foreach (const QContactCollectionId &id, collectionIds) {
        d->m_removedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QContactCollectionId, QContactManager::Operation>(id, QContactManager::Remove));
    }
}

/*!
    Clears the set of ids of collections which have been removed from the database.
 */
void QContactCollectionChangeSet::clearRemovedCollections()
{
    d->m_removedCollections.clear();
}

/*!
    Returns the list of ids of contact collections which have been added, changed or removed from
    the database. The list includes information about which database operation was done. The ids and
    operations are ordered so that the first operation is first in the list.
 */
QList<QPair<QContactCollectionId, QContactManager::Operation> > QContactCollectionChangeSet::modifiedCollections() const
{
    return d->m_modifiedCollections;
}

/*!
    Clears the list of ids of contact collections which have been added, changed or removed from the database
 */
void QContactCollectionChangeSet::clearModifiedCollections()
{
    d->m_modifiedCollections.clear();
}

/*!
    Clears all flags and sets of IDs in this change set.
 */
void QContactCollectionChangeSet::clearAll()
{
    d->m_dataChanged = false;
    d->m_addedCollections.clear();
    d->m_changedCollections.clear();
    d->m_removedCollections.clear();
    d->m_modifiedCollections.clear();
}

/*!
    Emits the appropriate signals from the given \a engine given the state of the change set. Note
    that the flags and sets of IDs are not cleared after signals are emitted.
 */
void QContactCollectionChangeSet::emitSignals(QContactManagerEngine *engine) const
{
    if (!engine)
        return;

    if (d->m_dataChanged) {
        emit engine->dataChanged();
    } else {
        if (!d->m_addedCollections.isEmpty())
            emit engine->collectionsAdded(d->m_addedCollections.toList());
        if (!d->m_changedCollections.isEmpty())
            emit engine->collectionsChanged(d->m_changedCollections.toList());
        if (!d->m_removedCollections.isEmpty())
            emit engine->collectionsRemoved(d->m_removedCollections.toList());
        if (!d->m_modifiedCollections.isEmpty())
            emit engine->collectionsModified(d->m_modifiedCollections);

    }
}

QT_END_NAMESPACE_CONTACTS
