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

#include "qorganizercollectionchangeset.h"
#include "qorganizercollectionchangeset_p.h"

#include "qorganizermanagerengine.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerCollectionChangeSet
    \brief The QOrganizerCollectionChangeSet class provides a simple API to simplify the emission
           of state-change signals for collections from QOrganizerManagerEngine implementations.
    \inmodule QtOrganizer
    \ingroup organizer-backends

    This class should only be used by backend developers.
 */

/*!
    Constructs a new change set.
 */
QOrganizerCollectionChangeSet::QOrganizerCollectionChangeSet()
    : d(new QOrganizerCollectionChangeSetData)
{
}

/*!
    Constructs a copy of the \a other change set.
 */
QOrganizerCollectionChangeSet::QOrganizerCollectionChangeSet(const QOrganizerCollectionChangeSet &other)
    : d(other.d)
{
}

/*!
    Frees the memory used by this change set.
 */
QOrganizerCollectionChangeSet::~QOrganizerCollectionChangeSet()
{
}

/*!
    Assigns this change set to be equal to \a other.
 */
QOrganizerCollectionChangeSet &QOrganizerCollectionChangeSet::operator=(const QOrganizerCollectionChangeSet &other)
{
    d = other.d;
    return *this;
}

/*!
    Sets the data changed flag to \a dataChanged.

    If this is set to true prior to calling emitSignals(), only the QOrganizerManagerEngine::dataChanged()
    signal will be emitted; otherwise, the appropriate finer-grained signals will be emitted.
 */
void QOrganizerCollectionChangeSet::setDataChanged(bool dataChanged)
{
    d->m_dataChanged = dataChanged;
}

/*!
    Returns the value of the data changed flag.
 */
bool QOrganizerCollectionChangeSet::dataChanged() const
{
    return d->m_dataChanged;
}

/*!
    Returns the set of IDs of collections which have been added to the database.
 */
QSet<QOrganizerCollectionId> QOrganizerCollectionChangeSet::addedCollections() const
{
    return d->m_addedCollections;
}

/*!
    Inserts the given \a collectionId into the set of IDs of collections which have been added to
    the database.
 */
void QOrganizerCollectionChangeSet::insertAddedCollection(const QOrganizerCollectionId &collectionId)
{
    d->m_addedCollections.insert(collectionId);
    d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(collectionId, QOrganizerManager::Add));
}

/*!
    Inserts each of the given \a collectionIds into the set of IDs of collections which have been
    added to the database.
 */
void QOrganizerCollectionChangeSet::insertAddedCollections(const QList<QOrganizerCollectionId> &collectionIds)
{
    foreach (const QOrganizerCollectionId &id, collectionIds) {
        d->m_addedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(id, QOrganizerManager::Add));
    }
}

/*!
    Clears the set of IDs of collections which have been added to the database.
 */
void QOrganizerCollectionChangeSet::clearAddedCollections()
{
    d->m_addedCollections.clear();
}

/*!
    Returns the set of IDs of collections which have been changed in the database.
 */
QSet<QOrganizerCollectionId> QOrganizerCollectionChangeSet::changedCollections() const
{
    return d->m_changedCollections;
}

/*!
    Inserts the given \a collectionId into the set of IDs of collections which have been changed in
    the database.
 */
void QOrganizerCollectionChangeSet::insertChangedCollection(const QOrganizerCollectionId &collectionId)
{
    d->m_changedCollections.insert(collectionId);
    d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(collectionId, QOrganizerManager::Change));
}

/*!
    Inserts each of the given \a collectionIds into the set of IDs of collections which have been
    changed in the database.
 */
void QOrganizerCollectionChangeSet::insertChangedCollections(const QList<QOrganizerCollectionId> &collectionIds)
{
    foreach (const QOrganizerCollectionId &id, collectionIds) {
        d->m_changedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(id, QOrganizerManager::Change));
    }
}

/*!
    Clears the set of IDs of collections which have been changed in the database.
 */
void QOrganizerCollectionChangeSet::clearChangedCollections()
{
    d->m_changedCollections.clear();
}

/*!
    Returns the set of IDs of collections which have been removed from the database.
 */
QSet<QOrganizerCollectionId> QOrganizerCollectionChangeSet::removedCollections() const
{
    return d->m_removedCollections;
}

/*!
    Inserts the given \a collectionId into the set of IDs of collections which have been removed from
    the database.
 */
void QOrganizerCollectionChangeSet::insertRemovedCollection(const QOrganizerCollectionId &collectionId)
{
    d->m_removedCollections.insert(collectionId);
    d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(collectionId, QOrganizerManager::Remove));
}

/*!
    Inserts each of the given \a collectionIds into the set of IDs of collections which have been
    removed from the database.
 */
void QOrganizerCollectionChangeSet::insertRemovedCollections(const QList<QOrganizerCollectionId> &collectionIds)
{
    foreach (const QOrganizerCollectionId &id, collectionIds) {
        d->m_removedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(id, QOrganizerManager::Remove));
    }
}

/*!
    Clears the set of ids of collections which have been removed from the database.
 */
void QOrganizerCollectionChangeSet::clearRemovedCollections()
{
    d->m_removedCollections.clear();
}

/*!
    Returns the list of ids of organizer collections which have been added, changed or removed from
    the database. The list includes information about which database operation was done. The ids and
    operations are ordered so that the first operation is first in the list.
 */
QList<QPair<QOrganizerCollectionId, QOrganizerManager::Operation> > QOrganizerCollectionChangeSet::modifiedCollections() const
{
    return d->m_modifiedCollections;
}

/*!
    Clears the list of ids of organizer collections which have been added, changed or removed from the database
 */
void QOrganizerCollectionChangeSet::clearModifiedCollections()
{
    d->m_modifiedCollections.clear();
}

/*!
    Clears all flags and sets of IDs in this change set.
 */
void QOrganizerCollectionChangeSet::clearAll()
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
void QOrganizerCollectionChangeSet::emitSignals(QOrganizerManagerEngine *engine) const
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

QT_END_NAMESPACE_ORGANIZER
