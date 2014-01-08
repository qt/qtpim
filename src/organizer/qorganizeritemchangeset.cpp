/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include "qorganizeritemchangeset.h"
#include "qorganizeritemchangeset_p.h"

#include "qorganizermanagerengine.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemChangeSet
    \brief The QOrganizerItemChangeSet class provides a simple API to simplify the emission of
           state-change signals for items from QOrganizerManagerEngine implementations.
    \inmodule QtOrganizer
    \ingroup organizer-backends

    This class should only be used by backend developers.
 */

/*!
    Constructs a new change set.
 */
QOrganizerItemChangeSet::QOrganizerItemChangeSet()
    : d(new QOrganizerItemChangeSetData)
{
}

/*!
    Constructs a copy of the \a other change set.
 */
QOrganizerItemChangeSet::QOrganizerItemChangeSet(const QOrganizerItemChangeSet &other)
    : d(other.d)
{
}

/*!
    Frees the memory used by this change set.
 */
QOrganizerItemChangeSet::~QOrganizerItemChangeSet()
{
}

/*!
    Assigns this change set to be equal to \a other.
 */
QOrganizerItemChangeSet &QOrganizerItemChangeSet::operator=(const QOrganizerItemChangeSet &other)
{
    d = other.d;
    return *this;
}

/*!
    Sets the data changed flag to \a dataChanged.

    If this is set to true prior to calling emitSignals(), only the QOrganizerManagerEngine::dataChanged()
    signal will be emitted; otherwise, the appropriate finer-grained signals will be emitted.
 */
void QOrganizerItemChangeSet::setDataChanged(bool dataChanged)
{
    d->m_dataChanged = dataChanged;
}

/*!
    Returns the value of the data changed flag.
 */
bool QOrganizerItemChangeSet::dataChanged() const
{
    return d->m_dataChanged;
}

/*!
    Returns the set of IDs of organizer items which have been added to the database.
 */
QSet<QOrganizerItemId> QOrganizerItemChangeSet::addedItems() const
{
    return d->m_addedItems;
}

/*!
    Inserts the given \a itemId into the set of ids of organizer items which have been added to the
    database.
 */
void QOrganizerItemChangeSet::insertAddedItem(const QOrganizerItemId &itemId)
{
    d->m_addedItems.insert(itemId);
    d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(itemId, QOrganizerManager::Add));
}

/*!
    Inserts each of the given \a itemIds into the set of IDs of organizer items which have been added
    to the database.
 */
void QOrganizerItemChangeSet::insertAddedItems(const QList<QOrganizerItemId> &itemIds)
{
    foreach (const QOrganizerItemId &id, itemIds) {
        d->m_addedItems.insert(id);
        d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(id, QOrganizerManager::Add));
    }
}

/*!
    Clears the set of IDs of organizer items which have been added to the database.
 */
void QOrganizerItemChangeSet::clearAddedItems()
{
    d->m_addedItems.clear();
}

/*!
    Returns the set of IDs of organizer items which have been changed in the database.
 */
QSet<QOrganizerItemId> QOrganizerItemChangeSet::changedItems() const
{
    return d->m_changedItems;
}

/*!
    Inserts the given \a itemId into the set of IDs of organizer items which have been changed in
    the database.
 */
void QOrganizerItemChangeSet::insertChangedItem(const QOrganizerItemId &itemId)
{
    d->m_changedItems.insert(itemId);
    d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(itemId, QOrganizerManager::Change));
}

/*!
    Inserts each of the given \a itemIds into the set of IDs of organizer items which have been
    changed in the database.
 */
void QOrganizerItemChangeSet::insertChangedItems(const QList<QOrganizerItemId> &itemIds)
{
    foreach (const QOrganizerItemId &id, itemIds) {
        d->m_changedItems.insert(id);
        d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(id, QOrganizerManager::Change));
    }
}

/*!
    Clears the set of IDs of organizer items which have been changed in the database.
 */
void QOrganizerItemChangeSet::clearChangedItems()
{
    d->m_changedItems.clear();
}

/*!
    Returns the set of IDs of organizer items which have been removed from the database.
 */
QSet<QOrganizerItemId> QOrganizerItemChangeSet::removedItems() const
{
    return d->m_removedItems;
}

/*!
    Inserts the given \a itemId into the set of IDs of organizer items which have been removed from
    the database.
 */
void QOrganizerItemChangeSet::insertRemovedItem(const QOrganizerItemId &itemId)
{
    d->m_removedItems.insert(itemId);
    d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(itemId, QOrganizerManager::Remove));
}

/*!
    Inserts each of the given \a itemIds into the set of IDs of organizer items which have been
    removed from the database.
 */
void QOrganizerItemChangeSet::insertRemovedItems(const QList<QOrganizerItemId> &itemIds)
{
    foreach (const QOrganizerItemId &id, itemIds) {
        d->m_removedItems.insert(id);
        d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(id, QOrganizerManager::Remove));
    }
}

/*!
    Clears the set of IDs of organizer items which have been removed from the database.
 */
void QOrganizerItemChangeSet::clearRemovedItems()
{
    d->m_removedItems.clear();
}

/*!
   Returns the list of ids of organizer items which have been added, changed or removed from
   the database. The list includes information about which database operation was done. The ids and
   operations are ordered so that the first operation is first in the list.
 */
QList<QPair<QOrganizerItemId, QOrganizerManager::Operation> > QOrganizerItemChangeSet::modifiedItems() const
{
    return d->m_modifiedItems;
}

/*!
  Clears the list of ids of organizer items which have been added, changed or removed from the database
 */
void QOrganizerItemChangeSet::clearModifiedItems()
{
    d->m_modifiedItems.clear();
}

/*!
    Clears all flags and sets of IDs in this change set.
 */
void QOrganizerItemChangeSet::clearAll()
{
    d->m_dataChanged = false;
    d->m_addedItems.clear();
    d->m_changedItems.clear();
    d->m_removedItems.clear();
    d->m_modifiedItems.clear();
}

/*!
    Emits the appropriate signals from the given \a engine given the state of the change set. Note
    that the flags and sets of IDs are not cleared after signals are emitted.
 */
void QOrganizerItemChangeSet::emitSignals(QOrganizerManagerEngine *engine) const
{
    if (!engine)
        return;

    if (d->m_dataChanged) {
        emit engine->dataChanged();
    } else {
        if (!d->m_addedItems.isEmpty())
            emit engine->itemsAdded(d->m_addedItems.toList());
        if (!d->m_changedItems.isEmpty())
            emit engine->itemsChanged(d->m_changedItems.toList());
        if (!d->m_removedItems.isEmpty())
            emit engine->itemsRemoved(d->m_removedItems.toList());
        if (!d->m_modifiedItems.isEmpty())
            emit engine->itemsModified(d->m_modifiedItems);
    }
}

QT_END_NAMESPACE_ORGANIZER
