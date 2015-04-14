/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
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

#include "qcontactmemorybackend_p.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qpointer.h>
#include <QtCore/qstringbuilder.h>
#include <QtCore/quuid.h>

#include <QtContacts/qcontactidfilter.h>
#include <QtContacts/qcontactrequests.h>
#include <QtContacts/qcontacttimestamp.h>

QT_BEGIN_NAMESPACE_CONTACTS

QContactManagerEngine* QContactMemoryEngineFactory::engine(const QMap<QString, QString> &parameters, QContactManager::Error *error)
{
    Q_UNUSED(error);

    QContactMemoryEngine *ret = QContactMemoryEngine::createMemoryEngine(parameters);
    return ret;
}

QString QContactMemoryEngineFactory::managerName() const
{
    return QString::fromLatin1("memory");
}

/*!
  \class QContactMemoryEngine

  \inmodule QtContacts

  \brief The QContactMemoryEngine class provides an in-memory implementation
  of a contacts backend.

  \internal

  It may be used as a reference implementation, or when persistent storage is not required.

  During construction, it will load the in-memory data associated with the memory store
  identified by the "id" parameter from the given parameters if it exists, or a new,
  anonymous store if it does not.

  Data stored in this engine is only available in the current process.

  This engine supports sharing, so an internal reference count is increased
  whenever a manager uses this backend, and is decreased when the manager
  no longer requires this engine.
 */

/* static data for manager class */
QMap<QString, QContactMemoryEngineData*> QContactMemoryEngine::engineDatas;

/*!
 * Factory function for creating a new in-memory backend, based
 * on the given \a parameters.
 *
 * The same engine will be returned for multiple calls with the
 * same value for the "id" parameter, while one of them is in scope.
 */
QContactMemoryEngine* QContactMemoryEngine::createMemoryEngine(const QMap<QString, QString> &parameters)
{
    bool anonymous = false;
    QString idValue = parameters.value(QStringLiteral("id"));
    if (idValue.isNull() || idValue.isEmpty()) {
        // no store given?  new, anonymous store.
        idValue = QUuid::createUuid().toString();
        anonymous = true;
    }

    QContactMemoryEngineData *data = engineDatas.value(idValue);
    if (data) {
        data->m_refCount.ref();
    } else {
        data = new QContactMemoryEngineData();
        data->m_id = idValue;
        data->m_anonymous = anonymous;
        engineDatas.insert(idValue, data);
    }
    return new QContactMemoryEngine(data);
}

/*!
 * Constructs a new in-memory backend which shares the given \a data with
 * other shared memory engines.
 */
QContactMemoryEngine::QContactMemoryEngine(QContactMemoryEngineData *data)
    : d(data)
{
    qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
    qRegisterMetaType<QList<QContactId> >("QList<QContactId>");
    qRegisterMetaType<QContactId>("QContactId");
    d->m_managerUri = managerUri();
    d->m_sharedEngines.append(this);

    // the default collection always exists.
    if (d->m_idToCollectionHash.isEmpty()) {
        d->m_managerUri = managerUri();
        const QContactCollectionId defaultId = defaultCollectionId();
        QContactCollection defaultCollection;
        defaultCollection.setId(defaultId);
        defaultCollection.setMetaData(QContactCollection::KeyName, QString(QStringLiteral("Default Collection")));
        d->m_idToCollectionHash.insert(defaultId, defaultCollection);
    }
}

/*! Frees any memory used by this engine */
QContactMemoryEngine::~QContactMemoryEngine()
{
    d->m_sharedEngines.removeAll(this);
    if (!d->m_refCount.deref()) {
        engineDatas.remove(d->m_id);
        delete d;
    }
}

/*! \reimp */
QString QContactMemoryEngine::managerName() const
{
    return QStringLiteral("memory");
}

/*! \reimp */
QMap<QString, QString> QContactMemoryEngine::managerParameters() const
{
    QMap<QString, QString> params;
    params.insert(QStringLiteral("id"), d->m_id);
    return params;
}

/*! \reimp
*/
QMap<QString, QString> QContactMemoryEngine::idInterpretationParameters() const
{
    return managerParameters();
}

/*! \reimp */
bool QContactMemoryEngine::setSelfContactId(const QContactId &contactId, QContactManager::Error *error)
{
    if (contactId.isNull() || d->m_contactIds.contains(contactId)) {
        *error = QContactManager::NoError;
        QContactId oldId = d->m_selfContactId;
        d->m_selfContactId = contactId;

        QContactChangeSet changeSet;
        changeSet.setOldAndNewSelfContactId(QPair<QContactId, QContactId>(oldId, contactId));
        d->emitSharedSignals(&changeSet);
        return true;
    }

    *error = QContactManager::DoesNotExistError;
    return false;
}

/*! \reimp */
QContactId QContactMemoryEngine::selfContactId(QContactManager::Error *error) const
{
    *error = QContactManager::DoesNotExistError;
    if (!d->m_selfContactId.isNull())
        *error = QContactManager::NoError;
    return d->m_selfContactId;
}

/*! \reimp */
QContact QContactMemoryEngine::contact(const QContactId &contactId, const QContactFetchHint &fetchHint, QContactManager::Error *error) const
{
    Q_UNUSED(fetchHint); // no optimizations are possible in the memory backend; ignore the fetch hint.
    int index = d->m_contactIds.indexOf(contactId);
    if (index != -1) {
        // found the contact successfully.
        *error = QContactManager::NoError;
        return d->m_contacts.at(index);
    }

    *error = QContactManager::DoesNotExistError;
    return QContact();
}

/*! \reimp */
QList<QContactId> QContactMemoryEngine::contactIds(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, QContactManager::Error *error) const
{
    /* Special case the fast case */
    if (filter.type() == QContactFilter::DefaultFilter && sortOrders.count() == 0) {
        return d->m_contactIds;
    } else {
        QList<QContact> clist = contacts(filter, sortOrders, QContactFetchHint(), error);

        /* Extract the ids */
        QList<QContactId> ids;
        foreach (const QContact &c, clist)
            ids.append(c.id());

        return ids;
    }
}

/*! \reimp */
QList<QContact> QContactMemoryEngine::contacts(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, const QContactFetchHint &fetchHint, QContactManager::Error *error) const
{
    Q_UNUSED(fetchHint); // no optimizations are possible in the memory backend; ignore the fetch hint.
    Q_UNUSED(error);

    QList<QContact> sorted;

    /* First filter out contacts - check for default filter first */
    if (filter.type() == QContactFilter::DefaultFilter) {
        foreach(const QContact&c, d->m_contacts) {
            QContactManagerEngine::addSorted(&sorted,c, sortOrders);
        }
    } else {
        foreach(const QContact&c, d->m_contacts) {
            if (QContactManagerEngine::testFilter(filter, c))
                QContactManagerEngine::addSorted(&sorted,c, sortOrders);
        }
    }

    return sorted;
}

/*! Saves the given contact \a theContact, storing any error to \a error and
    filling the \a changeSet with ids of changed contacts as required
    Returns true if the operation was successful otherwise false.
*/
bool QContactMemoryEngine::saveContact(QContact *theContact, QContactChangeSet &changeSet, QContactManager::Error *error)
{
    return saveContact(theContact, changeSet, error, QList<QContactDetail::DetailType>());
}

/*! \reimp */
bool QContactMemoryEngine::saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error)
{
    return saveContacts(contacts, errorMap, error, QList<QContactDetail::DetailType>());
}

/*! Removes the contact identified by the given \a contactId, storing any error to \a error and
    filling the \a changeSet with ids of changed contacts and relationships as required.
    Returns true if the operation was successful otherwise false.
*/
bool QContactMemoryEngine::removeContact(const QContactId &contactId, QContactChangeSet &changeSet, QContactManager::Error *error)
{
    int index = d->m_contactIds.indexOf(contactId);

    if (index == -1) {
        *error = QContactManager::DoesNotExistError;
        return false;
    }

    // remove the contact from any relationships it was in.
    QContact thisContact = d->m_contacts.at(index);
    QList<QContactRelationship> allRelationships = relationships(QString(), thisContact.id(), QContactRelationship::Either, error);
    if (*error != QContactManager::NoError && *error != QContactManager::DoesNotExistError) {
        *error = QContactManager::UnspecifiedError; // failed to clean up relationships
        return false;
    }

    // this is meant to be a transaction, so if any of these fail, we're in BIG TROUBLE.
    // a real backend will use DBMS transactions to ensure database integrity.
    removeRelationships(allRelationships, 0, error);

    // having cleaned up the relationships, remove the contact from the lists.
    d->m_contacts.removeAt(index);
    d->m_contactIds.removeAt(index);
    *error = QContactManager::NoError;

    // and if it was the self contact, reset the self contact id
    if (contactId == d->m_selfContactId) {
        d->m_selfContactId = QContactId();
        changeSet.setOldAndNewSelfContactId(QPair<QContactId, QContactId>(contactId, QContactId()));
    }

    changeSet.insertRemovedContact(contactId);
    return true;
}

/*! \reimp */
bool QContactMemoryEngine::removeContacts(const QList<QContactId> &contactIds, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error)
{
    if (contactIds.count() == 0) {
        *error = QContactManager::BadArgumentError;
        return false;
    }

    QContactChangeSet changeSet;
    QContactId current;
    QContactManager::Error operationError = QContactManager::NoError;
    for (int i = 0; i < contactIds.count(); i++) {
        current = contactIds.at(i);
        if (!removeContact(current, changeSet, error)) {
            operationError = *error;
            if (errorMap)
                errorMap->insert(i, operationError);
        }
    }

    *error = operationError;
    d->emitSharedSignals(&changeSet);
    // return false if some errors occurred
    return (*error == QContactManager::NoError);
}

/*! \reimp */
QList<QContactRelationship> QContactMemoryEngine::relationships(const QString &relationshipType, const QContactId &participantId, QContactRelationship::Role role, QContactManager::Error *error) const
{
    const QContactId defaultId;
    QList<QContactRelationship> retn;
    for (int i = 0; i < d->m_relationships.size(); i++) {
        QContactRelationship curr = d->m_relationships.at(i);

        // check that the relationship type matches
        if (curr.relationshipType() != relationshipType && !relationshipType.isEmpty())
            continue;

        // if the participantId argument is default constructed, then the relationship matches.
        if (participantId == defaultId) {
            retn.append(curr);
            continue;
        }

        // otherwise, check that the participant exists and plays the required role in the relationship.
        if (role == QContactRelationship::First && curr.first() == participantId) {
            retn.append(curr);
        } else if (role == QContactRelationship::Second && curr.second() == participantId) {
            retn.append(curr);
        } else if (role == QContactRelationship::Either && (curr.first() == participantId || curr.second() == participantId)) {
            retn.append(curr);
        }
    }

    *error = QContactManager::NoError;
    if (retn.isEmpty())
        *error = QContactManager::DoesNotExistError;
    return retn;
}

/*! Saves the given relationship \a relationship, storing any error to \a error and
    filling the \a changeSet with ids of changed contacts and relationships as required
    Returns true if the operation was successful otherwise false.
*/
bool QContactMemoryEngine::saveRelationship(QContactRelationship *relationship, QContactChangeSet &changeSet, QContactManager::Error *error)
{
    // Attempt to validate the relationship.
    // first, check that the source contact exists and is in this manager.
    QString myUri = managerUri();
    int firstContactIndex = d->m_contactIds.indexOf(relationship->first());
    if ((!relationship->first().managerUri().isEmpty() && relationship->first().managerUri() != myUri)
            ||firstContactIndex == -1) {
        *error = QContactManager::InvalidRelationshipError;
        return false;
    }

    // second, check that the second contact exists (if it's local); we cannot check other managers' contacts.
    QContactId dest = relationship->second();
    int secondContactIndex = d->m_contactIds.indexOf(dest);

    if (dest.managerUri().isEmpty() || dest.managerUri() == myUri) {
        // this entry in the destination list is supposedly stored in this manager.
        // check that it exists, and that it isn't the source contact (circular)
        if (secondContactIndex == -1 || dest == relationship->first()) {
            *error = QContactManager::InvalidRelationshipError;
            return false;
        }
    }

    // the relationship is valid.  We need to update the manager URIs in the second contact if it is empty to our URI.
    if (dest.managerUri().isEmpty()) {
        // need to update the URI
        relationship->setSecond(dest);
    }

    // check to see if the relationship already exists in the database.  If so, replace.
    // We do this because we don't want duplicates in our lists / maps of relationships.
    *error = QContactManager::NoError;
    QList<QContactRelationship> allRelationships = d->m_relationships;
    for (int i = 0; i < allRelationships.size(); i++) {
        QContactRelationship curr = allRelationships.at(i);
        if (curr == *relationship) {
            return true;
            // TODO: set error to AlreadyExistsError and return false?
        }
    }

    // no matching relationship; must be new.  append it to lists in our map of relationships where required.
    QList<QContactRelationship> firstRelationships = d->m_orderedRelationships.value(relationship->first());
    QList<QContactRelationship> secondRelationships = d->m_orderedRelationships.value(relationship->second());
    firstRelationships.append(*relationship);
    secondRelationships.append(*relationship);
    d->m_orderedRelationships.insert(relationship->first(), firstRelationships);
    d->m_orderedRelationships.insert(relationship->second(), secondRelationships);
    changeSet.insertAddedRelationshipsContact(relationship->first());
    changeSet.insertAddedRelationshipsContact(relationship->second());

    // update the contacts involved
    QContactManagerEngine::setContactRelationships(&d->m_contacts[firstContactIndex], firstRelationships);
    QContactManagerEngine::setContactRelationships(&d->m_contacts[secondContactIndex], secondRelationships);

    // finally, insert into our list of all relationships, and return.
    d->m_relationships.append(*relationship);
    return true;
}

/*! \reimp */
bool QContactMemoryEngine::saveRelationships(QList<QContactRelationship> *relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error)
{
    *error = QContactManager::NoError;
    QContactManager::Error functionError;
    QContactChangeSet changeSet;

    for (int i = 0; i < relationships->size(); i++) {
        QContactRelationship curr = relationships->at(i);
        saveRelationship(&curr, changeSet, &functionError);
        if (functionError != QContactManager::NoError && errorMap)
            errorMap->insert(i, functionError);

        // and replace the current relationship with the updated version.
        relationships->replace(i, curr);

        // also, update the total error if it did not succeed.
        if (functionError != QContactManager::NoError)
            *error = functionError;
    }

    d->emitSharedSignals(&changeSet);
    return (*error == QContactManager::NoError);
}

/*! Removes the given relationship \a relationship, storing any error to \a error and
    filling the \a changeSet with ids of changed contacts and relationships as required
    Returns true if the operation was successful otherwise false.
*/
bool QContactMemoryEngine::removeRelationship(const QContactRelationship &relationship, QContactChangeSet &changeSet, QContactManager::Error *error)
{
    // attempt to remove it from our list of relationships.
    if (!d->m_relationships.removeOne(relationship)) {
        *error = QContactManager::DoesNotExistError;
        return false;
    }

    // if that worked, then we need to remove it from the two locations in our map, also.
    QList<QContactRelationship> firstRelationships = d->m_orderedRelationships.value(relationship.first());
    QList<QContactRelationship> secondRelationships = d->m_orderedRelationships.value(relationship.second());
    firstRelationships.removeOne(relationship);
    secondRelationships.removeOne(relationship);
    d->m_orderedRelationships.insert(relationship.first(), firstRelationships);
    d->m_orderedRelationships.insert(relationship.second(), secondRelationships);

    // Update the contacts as well
    int firstContactIndex = d->m_contactIds.indexOf(relationship.first());
    int secondContactIndex = relationship.second().managerUri() == managerUri() ? d->m_contactIds.indexOf(relationship.second()) : -1;
    if (firstContactIndex != -1)
        QContactMemoryEngine::setContactRelationships(&d->m_contacts[firstContactIndex], firstRelationships);
    if (secondContactIndex != -1)
        QContactMemoryEngine::setContactRelationships(&d->m_contacts[secondContactIndex], secondRelationships);

    // set our changes, and return.
    changeSet.insertRemovedRelationshipsContact(relationship.first());
    changeSet.insertRemovedRelationshipsContact(relationship.second());
    *error = QContactManager::NoError;
    return true;
}

/*! \reimp */
bool QContactMemoryEngine::removeRelationships(const QList<QContactRelationship> &relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error)
{
    QContactManager::Error functionError;
    QContactChangeSet cs;
    for (int i = 0; i < relationships.size(); i++) {
        removeRelationship(relationships.at(i), cs, &functionError);

        // update the total error if it did not succeed.
        if (functionError != QContactManager::NoError) {
            if (errorMap)
                errorMap->insert(i, functionError);
            *error = functionError;
        }
    }

    d->emitSharedSignals(&cs);
    return (*error == QContactManager::NoError);
}

QContactCollectionId QContactMemoryEngine::defaultCollectionId() const
{
    static const QByteArray id("Personal");
    return collectionId(id);
}

QContactCollection QContactMemoryEngine::collection(const QContactCollectionId &collectionId, QContactManager::Error *error)
{
    if (d->m_idToCollectionHash.contains(collectionId)) {
        *error = QContactManager::NoError;
        return d->m_idToCollectionHash.value(collectionId);
    }

    *error = QContactManager::DoesNotExistError;
    return QContactCollection();
}

QList<QContactCollection> QContactMemoryEngine::collections(QContactManager::Error *error)
{
    Q_ASSERT(!d->m_idToCollectionHash.isEmpty());
    *error = QContactManager::NoError;
    return d->m_idToCollectionHash.values();
}

bool QContactMemoryEngine::saveCollection(QContactCollection *collection, QContactManager::Error *error)
{
    QContactCollectionId collectionId = collection->id();

    QContactCollectionChangeSet cs;
    if (d->m_idToCollectionHash.contains(collectionId)) {
        // this collection already exists.  update our internal list
        // if the collection has been modified.
        if (d->m_idToCollectionHash.value(collectionId) == *collection) {
            *error = QContactManager::NoError;
            return true;
        }

        cs.insertChangedCollection(collectionId);
    } else {
        // this must be a new collection.  check that the id is null.
        if (!collectionId.isNull() && collectionId.managerUri() != d->m_managerUri) {
            // nope, this collection belongs in another manager, or has been deleted.
            *error = QContactManager::DoesNotExistError;
            return false;
        }

        // this is a new collection with a null id; create a new id, add it to our list.
        QUuid id = QUuid::createUuid();
        collectionId = this->collectionId(id.toByteArray());
        collection->setId(collectionId);
        cs.insertAddedCollection(collectionId);
    }

    d->m_idToCollectionHash.insert(collectionId, *collection);
    d->emitSharedSignals(&cs);
    *error = QContactManager::NoError;
    return true;
}

bool QContactMemoryEngine::removeCollection(const QContactCollectionId &collectionId, QContactManager::Error *error)
{
    if (collectionId == defaultCollectionId()) {
        // attempting to remove the default collection.  this is not allowed in the memory engine.
        *error = QContactManager::PermissionsError;
        return false;
    }

    // try to find the collection to remove it (and the items it contains)
    if (d->m_idToCollectionHash.contains(collectionId)) {
        // found the collection to remove.  remove the items in the collection.
        const QList<QContactId> contactsToRemove = d->m_contactsInCollections.values(collectionId);
        if (!contactsToRemove.isEmpty()) {
            QMap<int, QContactManager::Error> errorMap;
            if (!removeContacts(contactsToRemove, &errorMap, error)) {
                // without transaction support, we can't back out.  but the operation should fail.
                return false;
            }
        }

        // now remove the collection from our lists.
        d->m_idToCollectionHash.remove(collectionId);
        d->m_contactsInCollections.remove(collectionId);
        QContactCollectionChangeSet cs;
        cs.insertRemovedCollection(collectionId);
        d->emitSharedSignals(&cs);
        *error = QContactManager::NoError;
        return true;
    }

    // the collection doesn't exist...
    *error = QContactManager::DoesNotExistError;
    return false;
}

/*! \reimp */
void QContactMemoryEngine::requestDestroyed(QContactAbstractRequest *req)
{
    Q_UNUSED(req);
}

/*! \reimp */
bool QContactMemoryEngine::startRequest(QContactAbstractRequest *req)
{
    updateRequestState(req, QContactAbstractRequest::ActiveState);
    performAsynchronousOperation(req);

    return true;
}

bool QContactMemoryEngine::cancelRequest(QContactAbstractRequest *req)
{
    Q_UNUSED(req); // we can't cancel since we complete immediately
    return false;
}

/*! \reimp */
bool QContactMemoryEngine::waitForRequestFinished(QContactAbstractRequest *req, int msecs)
{
    // in our implementation, we always complete any operation we start.
    Q_UNUSED(msecs);
    Q_UNUSED(req);

    return true;
}

/*!
 * This slot is called some time after an asynchronous request is started.
 * It performs the required operation, sets the result and returns.
 */
void QContactMemoryEngine::performAsynchronousOperation(QContactAbstractRequest *currentRequest)
{
    // store up changes, and emit signals once at the end of the (possibly batch) operation.
    QContactChangeSet changeSet;

    // Now perform the active request and emit required signals.
    Q_ASSERT(currentRequest->state() == QContactAbstractRequest::ActiveState);
    switch (currentRequest->type()) {
        case QContactAbstractRequest::ContactFetchRequest:
        {
            QContactFetchRequest *r = static_cast<QContactFetchRequest*>(currentRequest);
            QContactFilter filter = r->filter();
            QList<QContactSortOrder> sorting = r->sorting();
            QContactFetchHint fetchHint = r->fetchHint();

            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContact> requestedContacts = contacts(filter, sorting, fetchHint, &operationError);

            // update the request with the results.
            if (!requestedContacts.isEmpty() || operationError != QContactManager::NoError)
                updateContactFetchRequest(r, requestedContacts, operationError, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::ContactFetchByIdRequest:
        {
            QContactFetchByIdRequest *r = static_cast<QContactFetchByIdRequest*>(currentRequest);
            QContactIdFilter idFilter;
            idFilter.setIds(r->contactIds());
            QList<QContactSortOrder> sorting;
            QContactFetchHint fetchHint = r->fetchHint();
            QContactManager::Error error = QContactManager::NoError;
            QList<QContact> requestedContacts = contacts(idFilter, sorting, fetchHint, &error);
            // Build an index into the results
            QHash<QContactId, int> idMap; // value is index into unsorted
            if (error == QContactManager::NoError) {
                for (int i = 0; i < requestedContacts.size(); i++) {
                    idMap.insert(requestedContacts[i].id(), i);
                }
            }
            // Find the order in which the results should be presented
            // Build up the results and errors
            QList<QContact> results;
            QMap<int, QContactManager::Error> errorMap;
            int index = 0;
            foreach (const QContactId &id, r->contactIds()) {
                if (!idMap.contains(id)) {
                    errorMap.insert(index, QContactManager::DoesNotExistError);
                    error = QContactManager::DoesNotExistError;
                    results.append(QContact());
                } else {
                    results.append(requestedContacts[idMap[id]]);
                }
                index++;
            }

            // update the request with the results.
            if (!requestedContacts.isEmpty() || error != QContactManager::NoError)
                QContactManagerEngine::updateContactFetchByIdRequest(r, results, error, errorMap, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::ContactIdFetchRequest:
        {
            QContactIdFetchRequest *r = static_cast<QContactIdFetchRequest*>(currentRequest);
            QContactFilter filter = r->filter();
            QList<QContactSortOrder> sorting = r->sorting();

            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContactId> requestedContactIds = contactIds(filter, sorting, &operationError);

            if (!requestedContactIds.isEmpty() || operationError != QContactManager::NoError)
                updateContactIdFetchRequest(r, requestedContactIds, operationError, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::ContactSaveRequest:
        {
            QContactSaveRequest *r = static_cast<QContactSaveRequest*>(currentRequest);
            QList<QContact> contacts = r->contacts();

            QContactManager::Error operationError = QContactManager::NoError;
            QMap<int, QContactManager::Error> errorMap;
            saveContacts(&contacts, &errorMap, &operationError, r->typeMask());

            updateContactSaveRequest(r, contacts, operationError, errorMap, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::ContactRemoveRequest:
        {
            // this implementation provides scant information to the user
            // the operation either succeeds (all contacts matching the filter were removed)
            // or it fails (one or more contacts matching the filter could not be removed)
            // if a failure occurred, the request error will be set to the most recent
            // error that occurred during the remove operation.
            QContactRemoveRequest *r = static_cast<QContactRemoveRequest*>(currentRequest);
            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContactId> contactsToRemove = r->contactIds();
            QMap<int, QContactManager::Error> errorMap;

            for (int i = 0; i < contactsToRemove.size(); i++) {
                QContactManager::Error tempError;
                removeContact(contactsToRemove.at(i), changeSet, &tempError);

                if (tempError != QContactManager::NoError) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
            }

            if (!errorMap.isEmpty() || operationError != QContactManager::NoError)
                updateContactRemoveRequest(r, operationError, errorMap, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::RelationshipFetchRequest:
        {
            QContactRelationshipFetchRequest *r = static_cast<QContactRelationshipFetchRequest*>(currentRequest);
            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContactManager::Error> operationErrors;
            QList<QContactRelationship> allRelationships = relationships(QString(), QContactId(), QContactRelationship::Either, &operationError);
            QList<QContactRelationship> requestedRelationships;

            // select the requested relationships.
            for (int i = 0; i < allRelationships.size(); i++) {
                QContactRelationship currRel = allRelationships.at(i);
                if (r->first() != QContactId() && r->first() != currRel.first())
                    continue;
                if (r->second() != QContactId() && r->second() != currRel.second())
                    continue;
                if (!r->relationshipType().isEmpty() && r->relationshipType() != currRel.relationshipType())
                    continue;
                requestedRelationships.append(currRel);
            }

            // update the request with the results.
            if (!requestedRelationships.isEmpty() || operationError != QContactManager::NoError)
                updateRelationshipFetchRequest(r, requestedRelationships, operationError, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::RelationshipRemoveRequest:
        {
            QContactRelationshipRemoveRequest *r = static_cast<QContactRelationshipRemoveRequest*>(currentRequest);
            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContactRelationship> relationshipsToRemove = r->relationships();
            QMap<int, QContactManager::Error> errorMap;

            removeRelationships(r->relationships(), &errorMap, &operationError);

            if (!errorMap.isEmpty() || operationError != QContactManager::NoError)
                updateRelationshipRemoveRequest(r, operationError, errorMap, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::RelationshipSaveRequest:
        {
            QContactRelationshipSaveRequest *r = static_cast<QContactRelationshipSaveRequest*>(currentRequest);
            QContactManager::Error operationError = QContactManager::NoError;
            QMap<int, QContactManager::Error> errorMap;
            QList<QContactRelationship> requestRelationships = r->relationships();

            saveRelationships(&requestRelationships, &errorMap, &operationError);

            // update the request with the results.
            updateRelationshipSaveRequest(r, requestRelationships, operationError, errorMap, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::CollectionFetchRequest:
        {
            QContactCollectionFetchRequest* r = static_cast<QContactCollectionFetchRequest*>(currentRequest);
            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContactCollection> requestedContactCollections = collections(&operationError);

            // update the request with the results.
            updateCollectionFetchRequest(r, requestedContactCollections, operationError, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::CollectionSaveRequest:
        {
            QContactCollectionSaveRequest* r = static_cast<QContactCollectionSaveRequest*>(currentRequest);
            QList<QContactCollection> collections = r->collections();
            QList<QContactCollection> retn;

            QContactManager::Error operationError = QContactManager::NoError;
            QMap<int, QContactManager::Error> errorMap;
            for (int i = 0; i < collections.size(); ++i) {
                QContactManager::Error tempError = QContactManager::NoError;
                QContactCollection curr = collections.at(i);
                if (!saveCollection(&curr, &tempError)) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
                retn.append(curr);
            }

            updateCollectionSaveRequest(r, retn, operationError, errorMap, QContactAbstractRequest::FinishedState);
        }
        break;

        case QContactAbstractRequest::CollectionRemoveRequest:
        {
            // removes the collections identified in the list of ids.
            QContactCollectionRemoveRequest* r = static_cast<QContactCollectionRemoveRequest*>(currentRequest);
            QContactManager::Error operationError = QContactManager::NoError;
            QList<QContactCollectionId> collectionsToRemove = r->collectionIds();
            QMap<int, QContactManager::Error> errorMap;

            for (int i = 0; i < collectionsToRemove.size(); i++) {
                QContactManager::Error tempError = QContactManager::NoError;
                removeCollection(collectionsToRemove.at(i), &tempError);

                if (tempError != QContactManager::NoError) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
            }

            if (!errorMap.isEmpty() || operationError != QContactManager::NoError)
                updateCollectionRemoveRequest(r, operationError, errorMap, QContactAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QContactAbstractRequest::FinishedState);
        }
        break;


        default: // unknown request type.
        break;
    }

    // now emit any signals we have to emit
    d->emitSharedSignals(&changeSet);
}

void QContactMemoryEngine::partiallySyncDetails(QContact *to, const QContact &from, const QList<QContactDetail::DetailType> &mask)
{
    // these details in old contact
    QList<QContactDetail> fromDetails;
    // these details in new contact
    QList<QContactDetail> toDetails;
    // Collect details that match mask
    foreach (QContactDetail::DetailType type, mask) {
        fromDetails.append(from.details(type));
        toDetails.append(to->details(type));
    }
    // check details to remove
    foreach (QContactDetail detail, toDetails) {
        if (!fromDetails.contains(detail))
            to->removeDetail(&detail);
    }
    // check details to save
    foreach (QContactDetail detail, fromDetails) {
        if (!toDetails.contains(detail))
            to->saveDetail(&detail);
    }
}

/*!
 * \reimp
 */
bool QContactMemoryEngine::isRelationshipTypeSupported(const QString& relationshipType, QContactType::TypeValues contactType) const
{
    // the memory backend supports arbitrary relationship types
    // but some relationship types don't make sense for groups or facets.
    if (contactType == QContactType::TypeGroup || contactType == QContactType::TypeFacet) {
        if (relationshipType == QContactRelationship::HasSpouse() || relationshipType == QContactRelationship::HasAssistant()) {
            return false;
        }

        if (contactType == QContactType::TypeGroup) {
            if (relationshipType == QContactRelationship::Aggregates())
                return false;
        } else {
            if (relationshipType == QContactRelationship::HasMember())
                return false;
        }
    }

    // all other relationship types for all contact types are supported.
    return true;
}

/*!
 * \reimp
 */
QList<QVariant::Type> QContactMemoryEngine::supportedDataTypes() const
{
    QList<QVariant::Type> st;
    st.append(QVariant::String);
    st.append(QVariant::Date);
    st.append(QVariant::DateTime);
    st.append(QVariant::Time);
    st.append(QVariant::Bool);
    st.append(QVariant::Char);
    st.append(QVariant::Int);
    st.append(QVariant::UInt);
    st.append(QVariant::LongLong);
    st.append(QVariant::ULongLong);
    st.append(QVariant::Double);

    return st;
}

/*!
 * The function returns true if the backend natively supports the given filter \a filter, otherwise false.
 */
bool QContactMemoryEngine::isFilterSupported(const QContactFilter &filter) const
{
    Q_UNUSED(filter);
    // Until we add hashes for common stuff, fall back to slow code
    return false;
}

bool QContactMemoryEngine::saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap,
                                        QContactManager::Error *error, const QList<QContactDetail::DetailType> &mask)
{
    if (!contacts) {
        *error = QContactManager::BadArgumentError;
        return false;
    }

    QContactChangeSet changeSet;
    QContact current;
    QContactManager::Error operationError = QContactManager::NoError;
    for (int i = 0; i < contacts->count(); i++) {
        current = contacts->at(i);
        if (!saveContact(&current, changeSet, error, mask)) {
            operationError = *error;
            if (errorMap)
                errorMap->insert(i, operationError);
        } else {
            (*contacts)[i] = current;
        }
    }

    *error = operationError;
    d->emitSharedSignals(&changeSet);
    // return false if some error occurred
    return (*error == QContactManager::NoError);
}

bool QContactMemoryEngine::saveContact(QContact *theContact, QContactChangeSet &changeSet,
                                       QContactManager::Error *error, const QList<QContactDetail::DetailType> &mask)
{
    // ensure that the contact's details conform to their definitions
    if (!validateContact(*theContact, error)) {
        return false;
    }

    QContactId id(theContact->id());
    if (!id.managerUri().isEmpty() && id.managerUri() != managerUri()) {
        // the contact doesn't belong to this manager
        *error = QContactManager::DoesNotExistError;
        return false;
    }

    // check to see if this contact already exists
    int index = d->m_contactIds.indexOf(id);
    if (index != -1) {
        /* We also need to check that there are no modified create only details */
        QContact oldContact = d->m_contacts.at(index);

        if (oldContact.type() != theContact->type()) {
            *error = QContactManager::AlreadyExistsError;
            return false;
        }

        // check if this is partial save
        if (!mask.isEmpty()) {
            QContact tempContact = oldContact;
            partiallySyncDetails(&tempContact, *theContact, mask);
            *theContact = tempContact;
        }

        QContactTimestamp ts = theContact->detail(QContactTimestamp::Type);
        ts.setLastModified(QDateTime::currentDateTime());
        QContactManagerEngine::setDetailAccessConstraints(&ts, QContactDetail::ReadOnly | QContactDetail::Irremovable);
        theContact->saveDetail(&ts);

        // Looks ok, so continue
        d->m_contacts.replace(index, *theContact);
        changeSet.insertChangedContact(theContact->id(), mask);
    } else {
        // id does not exist; if not zero, fail.
        QContactId newId;
        if (theContact->id() != QContactId() && theContact->id() != newId) {
            // the ID is not empty, and it doesn't identify an existing contact in our database either.
            *error = QContactManager::DoesNotExistError;
            return false;
        }

        // check the contact collection
        QContactCollectionId collectionId = theContact->collectionId();
        // if is null use default collection
        if (collectionId.isNull()) {
            collectionId = this->defaultCollectionId();
            theContact->setCollectionId(collectionId);
        } else {
            // check if the collection exists
            QContactCollection collection = this->collection(collectionId, error);
            if (collection.id().isNull()) {
                return false;
            }
        }

        // check if this is partial save
        if (!mask.isEmpty()) {
            QContact tempContact;
            partiallySyncDetails(&tempContact, *theContact, mask);
            *theContact = tempContact;
        }

        /* New contact */
        QContactTimestamp ts = theContact->detail(QContactTimestamp::Type);
        ts.setLastModified(QDateTime::currentDateTime());
        ts.setCreated(ts.lastModified());
        setDetailAccessConstraints(&ts, QContactDetail::ReadOnly | QContactDetail::Irremovable);
        theContact->saveDetail(&ts);

        // update the contact item - set its ID
        QContactId newContactId = contactId(QByteArray(reinterpret_cast<const char *>(&d->m_nextContactId), sizeof(quint32)));
        ++(d->m_nextContactId);
        theContact->setId(newContactId);

        // finally, add the contact to our internal lists and return
        d->m_contacts.append(*theContact);                   // add contact to list
        d->m_contactIds.append(theContact->id());  // track the contact id.
        d->m_contactsInCollections.insert(collectionId, newContactId); // link contact to collection

        changeSet.insertAddedContact(theContact->id());
    }

    *error = QContactManager::NoError;     // successful.
    return true;
}

#include "moc_qcontactmemorybackend_p.cpp"

QT_END_NAMESPACE_CONTACTS
