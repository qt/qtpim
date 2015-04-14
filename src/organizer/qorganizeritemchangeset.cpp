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
   \typedef QOrganizerItemChangeSet::ItemChangeList

   This type describes a set of item changes, where each item whose
   ID is listed in \c second is subject to a change that affects some or
   all of the detail types listed in \c first.

   Note that change grouping is of items affected by equivalent changes,
   rather than of multiple changes affecting the same item.

   \code
        foreach (QOrganizerItemChangeSet::ItemChangeList changeList, set.changedItems()) {
            if (changeList.first.contains(QOrganizerItemDetail::TypeDescription)) {
                qDebug() << "Item IDs with description changes:" << changeList.second;
            }
        }
   \endcode

   \sa QOrganizerItemChangeSet::changedItems()
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
    Returns a list of ItemChangeLists describing which items have been
    changed in the database, and what details of those items have changed.

 */
QList<QOrganizerItemChangeSet::ItemChangeList> QOrganizerItemChangeSet::changedItems() const
{
    return d->m_changedItems;
}

/*!
    Inserts the given item ID \a itemId into the set of IDs of organizer items which have been changed in
    the database, with the changes described by \a typesChanged.
 */
void QOrganizerItemChangeSet::insertChangedItem(const QOrganizerItemId &itemId, const QList<QOrganizerItemDetail::DetailType> &typesChanged)
{
    insertChangedItems(QList<QOrganizerItemId>() << itemId, typesChanged);
}

/*!
    Inserts each of the itemIDs listed in \a itemIds into the set of IDs of organizer items which have been
    changed in the database, with the changes described by \a typesChanged.
 */
void QOrganizerItemChangeSet::insertChangedItems(const QList<QOrganizerItemId> &changedItemIds, const QList<QOrganizerItemDetail::DetailType> &typesChanged)
{
    // Sort and de-duplicate the IDs and change types
    QList<QOrganizerItemId> itemIds(changedItemIds);
    std::sort(itemIds.begin(), itemIds.end());
    {
        QList<QOrganizerItemId>::iterator iit = std::unique(itemIds.begin(), itemIds.end());
        while (iit != itemIds.end()) {
            iit = itemIds.erase(iit);
        }
    }

    QList<QOrganizerItemDetail::DetailType> changeSet(typesChanged);
    std::sort(changeSet.begin(), changeSet.end());
    {
        QList<QOrganizerItemDetail::DetailType>::iterator cit = std::unique(changeSet.begin(), changeSet.end());
        while (cit != changeSet.end()) {
            cit = changeSet.erase(cit);
        }
    }

    // Add these items to the list of items that match this change set
    QList<ItemChangeList>::iterator it = d->m_changedItems.begin(), end = d->m_changedItems.end();
    for ( ; it != end; ++it) {
        if ((*it).first == changeSet) {
            break;
        }
    }
    if (it == end) {
        // Add a list for this set of changes
        d->m_changedItems.append(qMakePair(changeSet, itemIds));
    } else {
        QList<QOrganizerItemId> &changedIds((*it).second);
        QList<QOrganizerItemId>::iterator iit = changedIds.begin();

        foreach (const QOrganizerItemId &itemId, itemIds) {
            // Add this ID if not yet present
            iit = std::lower_bound(iit, changedIds.end(), itemId);
            if (iit == changedIds.end() || *iit != itemId) {
                iit = changedIds.insert(iit, itemId);
            }
        }
    }

    // These items are all now modified
    foreach (const QOrganizerItemId &id, itemIds)
        d->m_modifiedItems.append(QPair<QOrganizerItemId, QOrganizerManager::Operation>(id, QOrganizerManager::Change));
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
        if (!d->m_changedItems.isEmpty()) {
            QList<ItemChangeList>::const_iterator it = d->m_changedItems.constBegin(), end = d->m_changedItems.constEnd();
            for ( ; it != end; ++it)
                emit engine->itemsChanged((*it).second, (*it).first);
        }
        if (!d->m_removedItems.isEmpty())
            emit engine->itemsRemoved(d->m_removedItems.toList());
        if (!d->m_modifiedItems.isEmpty())
            emit engine->itemsModified(d->m_modifiedItems);
    }
}

QT_END_NAMESPACE_ORGANIZER
