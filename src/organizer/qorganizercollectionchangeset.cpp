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

#include <qorganizercollectionchangeset.h>
#include "qorganizercollectionchangeset_p.h"
#include <qorganizermanagerengine.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
   \class QOrganizerCollectionChangeSet

   \brief The QOrganizerCollectionChangeSet class provides a simple API to
   simplify the emission of state-change signals from QOrganizerManagerEngine
   implementations.

  \inmodule QtOrganizer



   This class can be utilized by backend implementations to ensure
   correct emission of the \l QOrganizerManagerEngine::dataChanged(),
   \l QOrganizerManagerEngine::itemsAdded(),
   \l QOrganizerManagerEngine::itemsChanged() and
   \l QOrganizerManagerEngine::itemsRemoved().

   \sa QOrganizerManagerEngine
 */

/*!
   Constructs a new change set
 */
QOrganizerCollectionChangeSet::QOrganizerCollectionChangeSet()
    : d(new QOrganizerCollectionChangeSetData)
{
}

/*!
   Constructs a copy of the \a other change set
 */
QOrganizerCollectionChangeSet::QOrganizerCollectionChangeSet(const QOrganizerCollectionChangeSet& other)
    : d(other.d)
{
}

/*!
   Frees the memory used by this change set
 */
QOrganizerCollectionChangeSet::~QOrganizerCollectionChangeSet()
{
}

/*!
   Assigns this change set to be equal to \a other
 */
QOrganizerCollectionChangeSet& QOrganizerCollectionChangeSet::operator=(const QOrganizerCollectionChangeSet& other)
{
    d = other.d;
    return *this;
}

/*!
   Sets the data changed flag to \a dataChanged.  If this is set to true prior to calling \l emitSignals(),
   only the \l QOrganizerManagerEngine::dataChanged() signal will be emitted; otherwise, the appropriate
   finer-grained signals will be emitted.
 */
void QOrganizerCollectionChangeSet::setDataChanged(bool dataChanged)
{
    d->m_dataChanged = dataChanged;
}

/*!
   Returns the value of the data changed flag
 */
bool QOrganizerCollectionChangeSet::dataChanged() const
{
    return d->m_dataChanged;
}

/*!
   Returns the set of ids of collections which have been added to
   the database.
 */
QSet<QOrganizerCollectionId> QOrganizerCollectionChangeSet::addedCollections() const
{
    return d->m_addedCollections;
}

/*!
  Inserts the given collection id \a addedOrganizerCollectionId into the set of ids of collections
  which have been added to the database.
 */
void QOrganizerCollectionChangeSet::insertAddedCollection(const QOrganizerCollectionId& addedOrganizerCollectionId)
{
    d->m_addedCollections.insert(addedOrganizerCollectionId);
    d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(addedOrganizerCollectionId, QOrganizerManager::Add));
}

/*!
  Inserts each of the given collection ids \a addedOrganizerCollectionIds into
  the set of ids of collections which have been added to the database.
 */
void QOrganizerCollectionChangeSet::insertAddedCollections(const QList<QOrganizerCollectionId>& addedOrganizerCollectionIds)
{
    foreach (const QOrganizerCollectionId& id, addedOrganizerCollectionIds) {
        d->m_addedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(id, QOrganizerManager::Add));
    }
}

/*!
  Clears the set of ids of collections which have been added to the database
 */
void QOrganizerCollectionChangeSet::clearAddedCollections()
{
    d->m_addedCollections.clear();
}

/*!
   Returns the set of ids of collections which have been changed in
   the database.
 */
QSet<QOrganizerCollectionId> QOrganizerCollectionChangeSet::changedCollections() const
{
    return d->m_changedCollections;
}

/*!
  Inserts the given collection id \a changedOrganizerCollectionId into the set of ids of collections
  which have been changed to the database.
 */
void QOrganizerCollectionChangeSet::insertChangedCollection(const QOrganizerCollectionId& changedOrganizerCollectionId)
{
    d->m_changedCollections.insert(changedOrganizerCollectionId);
    d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(changedOrganizerCollectionId, QOrganizerManager::Change));
}

/*!
  Inserts each of the given collection ids \a changedOrganizerCollectionIds into the set of ids of collections
  which have been changed to the database.
 */
void QOrganizerCollectionChangeSet::insertChangedCollections(const QList<QOrganizerCollectionId>& changedOrganizerCollectionIds)
{
    foreach (const QOrganizerCollectionId& id, changedOrganizerCollectionIds) {
        d->m_changedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(id, QOrganizerManager::Change));
    }
}

/*!
  Clears the set of ids of collections which have been changed to the database
 */
void QOrganizerCollectionChangeSet::clearChangedCollections()
{
    d->m_changedCollections.clear();
}

/*!
   Returns the set of ids of collections which have been removed from
   the database.
 */
QSet<QOrganizerCollectionId> QOrganizerCollectionChangeSet::removedCollections() const
{
    return d->m_removedCollections;
}

/*!
  Inserts the given collection id \a removedOrganizerCollectionId into the set of ids of collections
  which have been removed to the database.
 */
void QOrganizerCollectionChangeSet::insertRemovedCollection(const QOrganizerCollectionId& removedOrganizerCollectionId)
{
    d->m_removedCollections.insert(removedOrganizerCollectionId);
    d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(removedOrganizerCollectionId, QOrganizerManager::Remove));
}

/*!
  Inserts each of the given collection ids \a removedOrganizerCollectionIds into the set of ids of collections
  which have been removed to the database.
 */
void QOrganizerCollectionChangeSet::insertRemovedCollections(const QList<QOrganizerCollectionId>& removedOrganizerCollectionIds)
{
    foreach (const QOrganizerCollectionId& id, removedOrganizerCollectionIds) {
        d->m_removedCollections.insert(id);
        d->m_modifiedCollections.append(QPair<QOrganizerCollectionId, QOrganizerManager::Operation>(id, QOrganizerManager::Remove));
    }
}

/*!
  Clears the set of ids of collections which have been removed to the database
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
   Clears all flags and sets of ids in this change set
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
   Emits the appropriate signals from the given \a engine given the state of the change set
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

QTORGANIZER_END_NAMESPACE
