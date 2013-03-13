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


#include "qcontactchangeset.h"
#include "qcontactchangeset_p.h"
#include "qcontactmanagerengine.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
   \class QContactChangeSet

  \inmodule QtContacts

   \brief The QContactChangeSet class provides a simple API to
   simplify the emission of state-change signals from
   QContactManagerEngine implementations.

   This class can be utilised by backend implementations to ensure
   correct emission of the \l QContactManagerEngine::dataChanged(), \l
   QContactManagerEngine::contactsAdded(), \l
   QContactManagerEngine::contactsChanged() and \l
   QContactManagerEngine::contactsRemoved().

   \sa QContactManagerEngine
 */

/*!
   Constructs a new change set
 */
QContactChangeSet::QContactChangeSet()
    : d(new QContactChangeSetData)
{
}

/*!
   Constructs a copy of the \a other change set
 */
QContactChangeSet::QContactChangeSet(const QContactChangeSet& other)
    : d(other.d)
{
}

/*!
   Frees the memory used by this change set
 */
QContactChangeSet::~QContactChangeSet()
{
}

/*!
   Assigns this change set to be equal to \a other
 */
QContactChangeSet& QContactChangeSet::operator=(const QContactChangeSet& other)
{
    d = other.d;
    return *this;
}

/*!
   Sets the data changed flag to \a dataChanged.  If this is set to true prior to calling \l emitSignals(),
   only the \l QContactManagerEngine::dataChanged() signal will be emitted; otherwise, the appropriate
   finer-grained signals will be emitted.
 */
void QContactChangeSet::setDataChanged(bool dataChanged)
{
    d->m_dataChanged = dataChanged;
}

/*!
   Returns the value of the data changed flag
 */
bool QContactChangeSet::dataChanged()
{
    return d.constData()->m_dataChanged;
}

/*!
   Returns the set of ids of contacts which have been added to
   the database.
 */
QSet<QContactId> QContactChangeSet::addedContacts() const
{
    return d.constData()->m_addedContacts;
}

/*!
  Inserts the given contact id \a addedContactId into the set of ids of contacts
  which have been added to the database.
 */
void QContactChangeSet::insertAddedContact(QContactId addedContactId)
{
    d->m_addedContacts.insert(addedContactId);
}

/*!
  Inserts each of the given contact ids \a addedContactIds into the set of ids of contacts
  which have been added to the database.
 */
void QContactChangeSet::insertAddedContacts(const QList<QContactId>& addedContactIds)
{
    foreach (const QContactId& id, addedContactIds)
        d->m_addedContacts.insert(id);
}

/*!
  Clears the set of ids of contacts which have been added to the database
 */
void QContactChangeSet::clearAddedContacts()
{
    d->m_addedContacts.clear();
}

/*!
   Returns the set of ids of contacts which have been changed in
   the database.
 */
QSet<QContactId> QContactChangeSet::changedContacts() const
{
    return d.constData()->m_changedContacts;
}

/*!
  Inserts the given contact id \a changedContactId into the set of ids of contacts
  which have been changed to the database.
 */
void QContactChangeSet::insertChangedContact(QContactId changedContactId)
{
    d->m_changedContacts.insert(changedContactId);
}

/*!
  Inserts each of the given contact ids \a changedContactIds into the set of ids of contacts
  which have been changed to the database.
 */
void QContactChangeSet::insertChangedContacts(const QList<QContactId>& changedContactIds)
{
    foreach (const QContactId& id, changedContactIds)
        d->m_changedContacts.insert(id);
}

/*!
  Clears the set of ids of contacts which have been changed to the database
 */
void QContactChangeSet::clearChangedContacts()
{
    d->m_changedContacts.clear();
}

/*!
   Returns the set of ids of contacts which have been removed from
   the database.
 */
QSet<QContactId> QContactChangeSet::removedContacts() const
{
    return d.constData()->m_removedContacts;
}

/*!
  Inserts the given contact id \a removedContactId into the set of ids of contacts
  which have been removed to the database.
 */
void QContactChangeSet::insertRemovedContact(QContactId removedContactId)
{
    d->m_removedContacts.insert(removedContactId);
}

/*!
  Inserts each of the given contact ids \a removedContactIds into the set of ids of contacts
  which have been removed to the database.
 */
void QContactChangeSet::insertRemovedContacts(const QList<QContactId>& removedContactIds)
{
    foreach (const QContactId& id, removedContactIds)
        d->m_removedContacts.insert(id);
}

/*!
  Clears the set of ids of contacts which have been removed to the database
 */
void QContactChangeSet::clearRemovedContacts()
{
    d->m_removedContacts.clear();
}

/*!
   Returns the set of ids of contacts which have been affected
   by the addition of relationships to the database.
 */
QSet<QContactId> QContactChangeSet::addedRelationshipsContacts() const
{
    return d.constData()->m_addedRelationships;
}

/*!
  Inserts the given contact id \a affectedContactId into the set of ids of contacts
  which have been affected by the addition of a relationship to the database.
 */
void QContactChangeSet::insertAddedRelationshipsContact(QContactId affectedContactId)
{
    d->m_addedRelationships.insert(affectedContactId);
}

/*!
  Inserts each of the given contact ids \a affectedContactIds into the set of ids of contacts
  which have been affected by the addition of a relationship to the database.
 */
void QContactChangeSet::insertAddedRelationshipsContacts(const QList<QContactId>& affectedContactIds)
{
    foreach (const QContactId& id, affectedContactIds)
        d->m_addedRelationships.insert(id);
}

/*!
  Clears the set of ids of contacts which have been affected by the addition of a relationship to the database.
 */
void QContactChangeSet::clearAddedRelationshipsContacts()
{
    d->m_addedRelationships.clear();
}

/*!
   Returns the set of ids of contacts which have been affected
   by the removal of relationships from the database.
 */
QSet<QContactId> QContactChangeSet::removedRelationshipsContacts() const
{
    return d.constData()->m_removedRelationships;
}

/*!
  Inserts the given contact id \a affectedContactId into the set of ids of contacts
  which have been affected by the removal of a relationship to the database.
 */
void QContactChangeSet::insertRemovedRelationshipsContact(QContactId affectedContactId)
{
    d->m_removedRelationships.insert(affectedContactId);
}

/*!
  Inserts each of the given contact ids \a affectedContactIds into the set of ids of contacts
  which have been affected by the removal of a relationship to the database.
 */
void QContactChangeSet::insertRemovedRelationshipsContacts(const QList<QContactId>& affectedContactIds)
{
    foreach (const QContactId& id, affectedContactIds)
        d->m_removedRelationships.insert(id);
}

/*!
  Clears the set of ids of contacts which have been affected by the removal of a relationship to the database.
 */
void QContactChangeSet::clearRemovedRelationshipsContacts()
{
    d->m_removedRelationships.clear();
}

/*!
  Sets the pair of ids which represent the old and new self contact ids
  to the given pair of ids \a oldAndNewContactId.
  The first id in the pair is the old self contact id, while the second
  id in the pair is the new self contact id.  If the new id is different
  to the old id at the point in time when emitSignals() is called,
  the QContactManagerEngine::selfContactIdChanged signal will be emitted.
 */
void QContactChangeSet::setOldAndNewSelfContactId(const QPair<QContactId, QContactId> &oldAndNewContactId)
{
    d->m_oldAndNewSelfContactId = oldAndNewContactId;
}

/*!
   Returns the pair of ids which represents the
   old and new self contact ids.  The first id in the pair is the
   old self contact id, while the second id in the pair is the
   new self contact id.  If the new id is different to the old id
   at the point in time when emitSignals() is called,
   the QContactManagerEngine::selfContactIdChanged() signal will be emitted.
 */
QPair<QContactId, QContactId> QContactChangeSet::oldAndNewSelfContactId() const
{
    return d.constData()->m_oldAndNewSelfContactId;
}

/*!
   Clears all flags and sets of ids in this change set
 */
void QContactChangeSet::clearAll()
{
    d->m_dataChanged = false;
    d->m_addedContacts.clear();
    d->m_changedContacts.clear();
    d->m_removedContacts.clear();
    d->m_addedRelationships.clear();
    d->m_removedRelationships.clear();
    d->m_oldAndNewSelfContactId = QPair<QContactId, QContactId>();
}

/*!
   Emits the appropriate signals from the given \a engine given the state of the change set
 */
void QContactChangeSet::emitSignals(QContactManagerEngine *engine)
{
    if (!engine)
        return;

    if (d.constData()->m_dataChanged) {
        emit engine->dataChanged();
    } else {
        if (!d.constData()->m_addedContacts.isEmpty())
            emit engine->contactsAdded(d.constData()->m_addedContacts.toList());
        if (!d.constData()->m_changedContacts.isEmpty())
            emit engine->contactsChanged(d.constData()->m_changedContacts.toList());
        if (!d.constData()->m_removedContacts.isEmpty())
            emit engine->contactsRemoved(d.constData()->m_removedContacts.toList());
        if (!d.constData()->m_addedRelationships.isEmpty())
            emit engine->relationshipsAdded(d.constData()->m_addedRelationships.toList());
        if (!d.constData()->m_removedRelationships.isEmpty())
            emit engine->relationshipsRemoved(d.constData()->m_removedRelationships.toList());
        if (d.constData()->m_oldAndNewSelfContactId.first != d.constData()->m_oldAndNewSelfContactId.second)
            emit engine->selfContactIdChanged(d.constData()->m_oldAndNewSelfContactId.first, d.constData()->m_oldAndNewSelfContactId.second);
    }
}


QT_END_NAMESPACE_CONTACTS
