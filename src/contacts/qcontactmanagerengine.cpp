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

#include "qcontactmanagerengine.h"

#include <QtCore/qmutex.h>
#include <QtCore/qpointer.h>
#include <QtCore/qset.h>

#include "qcontact_p.h"
#include "qcontactdetail_p.h"
#include "qcontactdetails.h"
#include "qcontactfilters.h"
#include "qcontactabstractrequest_p.h"
#include "qcontactaction.h"
#include "qcontactactiondescriptor.h"
#include "qcontactactionmanager_p.h"
#include "qcontactrequests_p.h"
#include "qcontactsortorder.h"

QT_BEGIN_NAMESPACE_CONTACTS

static bool validateActionFilter(const QContactFilter& filter);

/*!
  \class QContactManagerEngine
  \brief The QContactManagerEngine class provides the interface for
  implementations of the contact manager backend functionality.
  \inmodule QtContacts

  \ingroup contacts-backends

  Instances of this class are usually provided by a
  \l QContactManagerEngineFactory, which is loaded from a plugin.

  The default implementation of this interface provides a basic
  level of functionality for some functions so that specific engines
  can simply implement the functionality that is supported by
  the specific contacts engine that is being adapted.

  More information on writing a contacts engine plugin is available in
  the \l{Qt Contacts Manager Engines} documentation.

  \sa QContactManager, QContactManagerEngineFactory
 */

/*!
  \fn QContactManagerEngine::QContactManagerEngine()

  A default, empty constructor.
 */

/*!
  \fn QContactManagerEngine::dataChanged()

  This signal is emitted some time after changes occur to the data managed by this
  engine, and the engine is unable to determine which changes occurred, or if the
  engine considers the changes to be radical enough to require clients to reload all data.

  If this signal is emitted, no other signals may be emitted for the associated changes.

  As it is possible that other processes (or other devices) may have caused the
  changes, the timing can not be determined.

  \sa contactsAdded(), contactsChanged(), contactsRemoved()
 */

/*!
  \fn QContactManagerEngine::contactsAdded(const QList<QContactId>& contactIds);

  This signal is emitted some time after a set of contacts has been added to
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have added the contacts, the timing cannot be determined.

  The list of ids of contacts added is given by \a contactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::contactsChanged(const QList<QContactId>& contactIds, const QList<QContactDetail::DetailType> &typesChanged);

  This signal is emitted some time after a set of contacts has been modified in
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have modified the contacts, the timing cannot be determined.

  The list of ids of changed contacts is given by \a contactIds.  There may be one or more
  ids in the list.

  The set of contact detail types modified in the reported changes is a subset of those
  listed in \a typesChanged, unless \a typesChanged is empty, in which case no limitation
  on the reported changes may be assumed.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::contactsRemoved(const QList<QContactId>& contactIds);

  This signal is emitted some time after a set of contacts has been removed from
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have removed the contacts, the timing cannot be determined.

  The list of ids of removed contacts is given by \a contactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::relationshipsAdded(const QList<QContactId>& affectedContactIds);

  This signal is emitted some time after a set of contacts has been added to
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have added the contacts, the timing cannot be determined.

  The list of ids of affected contacts is given by \a affectedContactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::relationshipsRemoved(const QList<QContactId>& affectedContactIds);

  This signal is emitted some time after a set of relationships has been removed from
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have removed the relationships, the timing cannot be determined.

  The list of ids of affected contacts is given by \a affectedContactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::selfContactIdChanged(const QContactId& oldId, const QContactId& newId)

  This signal is emitted at some point after the id of the self-contact is changed from \a oldId to \a newId in the manager.
  If the \a newId is the invalid, null id, then the self contact was deleted or no self contact exists.
  This signal must not be emitted if the dataChanged() signal was previously emitted for this change.
  As it is possible that other processes (or other devices) may
  have removed or changed the self contact, the timing cannot be determined.

  \sa dataChanged()
 */

/*! Returns the manager name for this QContactManagerEngine
*/
QString QContactManagerEngine::managerName() const
{
    return QString(QStringLiteral("base"));
}

/*!
  Returns the parameters with which this engine was constructed.  Note that
  the engine may have discarded unused or invalid parameters at the time of
  construction, and these will not be returned.
 */
QMap<QString, QString> QContactManagerEngine::managerParameters() const
{
    return QMap<QString, QString>(); // default implementation requires no parameters.
}

/*!
  Returns the subset of the manager parameters that are relevant when interpreting
  contact ID values. Since contact ID comparison involves equivalence of
  managerUri values, parameters that do not differentiate contact IDs should not
  be returned by this function.

  For example, a manager engine may support 'user' and 'cachesize' parameters,
  where the former distinguishes between separate user domains, and the latter
  is for performance tuning. The 'user' parameter will be relevant to the interpretation
  of contact IDs and thus should be returned by this function, whereas 'cachesize'
  is not relevant and should be omitted.

  \sa managerUri(), managerParamaters()
 */
QMap<QString, QString> QContactManagerEngine::idInterpretationParameters() const
{
    return QMap<QString, QString>(); // default implementation returns no parameters.
}

/*!
  \fn QString QContactManagerEngine::managerUri() const

  Returns the unique URI of this manager, which is built from the manager name and the
  ID interpretation parameters used to construct it.

  \sa idInterpretationParameters()
 */

/*!
    \fn QContactId QContactManagerEngine::contactId(const QString &localId) const

    Returns the contact ID for this managerUri() and the given
    engine specific ID part \a localId.
*/

/*!
  Returns a list of contact ids that match the given \a filter, sorted according to the given list of \a sortOrders.
  Depending on the backend, this filtering operation may involve retrieving all the contacts.
  Any error which occurs will be saved in \a error.
 */
QList<QContactId> QContactManagerEngine::contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const
{
    Q_UNUSED(filter);
    Q_UNUSED(sortOrders);

    *error = QContactManager::NotSupportedError;
    return QList<QContactId>();
}

/*!
  Returns the list of contacts which match the given \a filter stored in the manager sorted according to the given list of \a sortOrders.

  Any operation error which occurs will be saved in \a error.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contacts will be returned.

  If a non-default fetch hint is supplied, and the client wishes to make changes to the contacts,
  they should ensure that only a detail type hint is supplied and that when saving it back, a
  type mask should be used which corresponds to the detail type hint.  This is to ensure
  that no data is lost by overwriting an existing contact with a restricted version of it.

  \sa QContactFetchHint
 */
QList<QContact> QContactManagerEngine::contacts(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    Q_UNUSED(filter);
    Q_UNUSED(sortOrders);
    Q_UNUSED(fetchHint);
    *error = QContactManager::NotSupportedError;
    return QList<QContact>();
}

/*!
  Returns the contact in the database identified by \a contactId.

  If the contact does not exist, an empty, default constructed QContact will be returned,
  and the \a error will be set to  \c QContactManager::DoesNotExistError.

  Any operation error which occurs will be saved in \a error.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contact will be returned.  If a client makes changes to an
  contact which has been retrieved with a fetch hint, they should save it back using a partial save,
  masked by the same set of detail names in order to avoid information loss.

  \sa QContactFetchHint
 */
QContact QContactManagerEngine::contact(const QContactId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    Q_UNUSED(contactId);
    Q_UNUSED(fetchHint);
    *error = QContactManager::NotSupportedError;
    return QContact();
}

/*!
  Sets the id of the "self" contact to the given \a contactId.
  Returns true if the "self" contact id was set successfully.
  If the given \a contactId does not identify a contact
  stored in this manager, the \a error will be set to
  \c QContactManager::DoesNotExistError and the function will
  return false; if the backend does not support the
  concept of a "self" contact, the \a error will be set to
  \c QContactManager::NotSupportedError and the function will
  return false.
 */
bool QContactManagerEngine::setSelfContactId(const QContactId& contactId, QContactManager::Error* error)
{
    Q_UNUSED(contactId);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Returns the id of the "self" contact which has previously been set.
  If no "self" contact has been set, or if the self contact was removed
  from the manager after being set, or if the backend does not support
  the concept of a "self" contact, the null id will be returned
  and the \a error will be set to \c QContactManager::DoesNotExistError.
 */
QContactId QContactManagerEngine::selfContactId(QContactManager::Error* error) const
{
    *error = QContactManager::DoesNotExistError;
    return QContactId();
}

/*!
  Returns a list of relationships of the given \a relationshipType in which the contact identified by \a participantId participates in the given \a role.
  If \a participantId is default-constructed, \a role is ignored and all relationships of the given \a relationshipType are returned.
  If \a relationshipType is empty, relationships of any type are returned.
  If no relationships of the given \a relationshipType in which the contact identified by \a participantId is involved in the given \a role exists,
  \a error is set to QContactManager::DoesNotExistError.
 */
QList<QContactRelationship> QContactManagerEngine::relationships(const QString& relationshipType, const QContactId& participantId, QContactRelationship::Role role, QContactManager::Error* error) const
{
    Q_UNUSED(relationshipType);
    Q_UNUSED(participantId);
    Q_UNUSED(role);

    *error = QContactManager::NotSupportedError;
    return QList<QContactRelationship>();
}

/*!
  Saves the given \a relationships in the database and returns true if the operation was successful.
  For any relationship which was unable to be saved, an entry into the \a errorMap will be created,
  with the key being the index into the input relationships list, and the value being the error which
  occurred for that index.

  The supplied \a errorMap parameter may be null, if the client does not desire detailed error information.
  If supplied, it will be empty upon entry to this function.

  The overall operation error will be saved in \a error.
 */
bool QContactManagerEngine::saveRelationships(QList<QContactRelationship>* relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(relationships);
    Q_UNUSED(errorMap);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Saves the given \a relationship in the database.  If the relationship already exists in the database, this function will
  return \c false and the \a error will be set to \c QContactManager::AlreadyExistsError.
  If the relationship is saved successfully, this function will return \c true and \a error will be set
  to \c QContactManager::NoError.  Note that relationships cannot be updated directly using this function; in order
  to update a relationship, you must remove the old relationship, make the required modifications, and then save it.

  The given relationship is invalid if it is circular (the first contact is the second contact), or
  if it references a non-existent local contact (either the first or second contact).  If the given \a relationship is invalid,
  the function will return \c false and the \a error will be set to \c QContactManager::InvalidRelationshipError.

  The default implementation of this function converts the argument into a call to saveRelationships.
 */
bool QContactManagerEngine::saveRelationship(QContactRelationship *relationship, QContactManager::Error *error)
{
    // Convert to a list op
    if (relationship) {
        QList<QContactRelationship> list;
        list.append(*relationship);

        QMap<int, QContactManager::Error> errors;
        bool ret = saveRelationships(&list, &errors, error);

        if (errors.count() > 0)
            *error = errors.begin().value();

        *relationship = list.value(0);
        return ret;
    } else {
        *error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Removes the given \a relationship from the manager.  If the relationship exists in the manager, the relationship
  will be removed, the \a error will be set to \c QContactManager::NoError and this function will return true.  If no such
  relationship exists in the manager, the \a error will be set to \c QContactManager::DoesNotExistError and this function
  will return false.

  The default implementation of this function converts the argument into a call to removeRelationships
 */
bool QContactManagerEngine::removeRelationship(const QContactRelationship& relationship, QContactManager::Error* error)
{
    // Convert to a list op
    QList<QContactRelationship> list;
    list.append(relationship);

    QMap<int, QContactManager::Error> errors;
    bool ret = removeRelationships(list, &errors, error);

    if (errors.count() > 0)
        *error = errors.begin().value();

    return ret;
}


/*!
  Removes the given \a relationships from the database and returns true if the operation was successful.
  For any relationship which was unable to be removed, an entry into the \a errorMap will be created,
  with the key being the index into the input relationships list, and the value being the error which
  occurred for that index.

  The supplied \a errorMap parameter may be null, if the client does not desire detailed error information.
  If supplied, it will be empty upon entry to this function.

  The overall operation error will be saved in \a error.
 */
bool QContactManagerEngine::removeRelationships(const QList<QContactRelationship>& relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(relationships);
    Q_UNUSED(errorMap);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
    This function should be reimplemented to support synchronous calls to fetch the default collection id.
*/
QContactCollectionId QContactManagerEngine::defaultCollectionId() const
{
    return QContactCollectionId();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch a collection based
    on its ID. Any errors encountered during this operation should be stored to \a error. If the
    given \a collectionId does not specify a valid collection, \a error will be set to
    \c QContactManager::DoesNotExistError.

*/
QContactCollection QContactManagerEngine::collection(const QContactCollectionId& collectionId, QContactManager::Error* error)
{
    Q_UNUSED(collectionId);
    *error = QContactManager::NotSupportedError;
    return QContactCollection();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch all the collections
    managed by this backend. Any errors encountered during this operation should be stored to \a error.
 */
QList<QContactCollection> QContactManagerEngine::collections(QContactManager::Error* error)
{
    *error = QContactManager::NotSupportedError;
    return QList<QContactCollection>();
}

/*!
    This function should be reimplemented to support synchronous calls to save a collection.

    This function is supposed to save the given \a collection to the backend, and returns true on
    success or false otherwise. Any errors encountered during this operation should be stored to
    \a error.

    A new collection will be created in the backend store if the collection ID of it is null.
    Otherwise, an existing collection with the same ID will be updated. If the given collection ID
    does not exist in the backend, it will result a QContactManager::DoesNotExistError error.

    Note that upon successful saving, the backend may update the collection, e.g. collection ID for
    newly saved collections.
*/
bool QContactManagerEngine::saveCollection(QContactCollection* collection, QContactManager::Error* error)
{
    Q_UNUSED(collection);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
    This function should be reimplemented to support synchronous calls to remove a collection.

    This function is supposed to remove the collection identified by the given \a collectionId, and
    all items in the collection. Returns true on success, or false otherwise. Any errors encountered
    during this operation should be stored to \a error.

    Note that removing the default collection should not be allowed and should result a
    QContactManager::PermissionsError error.
*/
bool QContactManagerEngine::removeCollection(const QContactCollectionId& collectionId, QContactManager::Error* error)
{
    Q_UNUSED(collectionId);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Given an input \a filter, returns the canonical version of the filter.

  Some of the following transformations may be applied:
  \list
   \li Any QContactActionFilters are transformed into the corresponding
     QContactFilters returned by matching actions
   \li Any QContactInvalidFilters contained in a union filter will be removed
   \li Any default QContactFilters contained in an intersection filter will be removed
   \li Any QContactIntersectionFilters with a QContactInvalidFilter contained will be
     replaced with a QContactInvalidFilter
   \li Any QContactUnionFilters with a default QContactFilter contained will be replaced
     with a default QContactFilter
   \li An empty QContactIntersectionFilter will be replaced with a QContactDefaultFilter
   \li An empty QContactUnionFilter will be replaced with a QContactInvalidFilter
   \li An empty QContactIdFilter will be replaced with a QContactInvalidFilter
   \li An intersection or union filter with a single entry will be replaced by that entry
   \li A QContactDetailFilter or QContactDetailRangeFilter with no detail type will be replaced with a QContactInvalidFilter
   \li A QContactDetailRangeFilter with no range specified will be converted to a QContactDetailFilter
  \endlist
*/
QContactFilter QContactManagerEngine::canonicalizedFilter(const QContactFilter &filter)
{
    switch(filter.type()) {
        case QContactFilter::ActionFilter:
        {
            // Find any matching actions, and do a union filter on their filter objects
            QContactActionFilter af(filter);
            QList<QContactActionDescriptor> descriptors = QContactActionManager::instance()->actionDescriptors(af.actionName());

            QList<QContactFilter> filters;
            for (int j = 0; j < descriptors.count(); j++) {
                // Action filters are not allowed to return action filters, at all
                // it's too annoying to check for recursion
                QContactFilter d = descriptors.at(j).contactFilter();
                if (!validateActionFilter(d))
                    continue;

                filters.append(d);
            }

            if (filters.count() == 0)
                return QContactInvalidFilter();
            if (filters.count() == 1)
                return filters.first();

            QContactUnionFilter f;
            f.setFilters(filters);
            return canonicalizedFilter(f);
        }
        // unreachable

        case QContactFilter::IntersectionFilter:
        {
            QContactIntersectionFilter f(filter);
            QList<QContactFilter> filters = f.filters();
            QList<QContactFilter>::iterator it = filters.begin();

            // XXX in theory we can remove duplicates in a set filter
            while (it != filters.end()) {
                QContactFilter canon = canonicalizedFilter(*it);
                if (canon.type() == QContactFilter::DefaultFilter) {
                    it = filters.erase(it);
                } else if (canon.type() == QContactFilter::InvalidFilter) {
                    return QContactInvalidFilter();
                } else {
                    *it = canon;
                    ++it;
                }
            }

            if (filters.count() == 0)
                return QContactFilter();
            if (filters.count() == 1)
                return filters.first();

            f.setFilters(filters);
            return f;
        }
        // unreachable

        case QContactFilter::UnionFilter:
        {
            QContactUnionFilter f(filter);
            QList<QContactFilter> filters = f.filters();
            QList<QContactFilter>::iterator it = filters.begin();

            // XXX in theory we can remove duplicates in a set filter
            while (it != filters.end()) {
                QContactFilter canon = canonicalizedFilter(*it);
                if (canon.type() == QContactFilter::InvalidFilter) {
                    it = filters.erase(it);
                } else if (canon.type() == QContactFilter::DefaultFilter) {
                    return QContactFilter();
                } else {
                    *it = canon;
                    ++it;
                }
            }

            if (filters.count() == 0)
                return QContactInvalidFilter();
            if (filters.count() == 1)
                return filters.first();

            f.setFilters(filters);
            return f;
        }
        // unreachable

        case QContactFilter::IdFilter:
        {
            QContactIdFilter f(filter);
            if (f.ids().count() == 0)
                return QContactInvalidFilter();
        }
        break; // fall through to return at end

        case QContactFilter::ContactDetailRangeFilter:
        {
            QContactDetailRangeFilter f(filter);
            if (f.detailType() == QContactDetail::TypeUndefined)
                return QContactInvalidFilter();
            if (f.minValue() == f.maxValue()
                && f.rangeFlags() == (QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper))
                return QContactInvalidFilter();
            if ((f.minValue().isNull() && f.maxValue().isNull()) || (f.minValue() == f.maxValue())) {
                QContactDetailFilter df;
                df.setDetailType(f.detailType(), f.detailField());
                df.setMatchFlags(f.matchFlags());
                df.setValue(f.minValue());
                return df;
            }
        }
        break; // fall through to return at end

        case QContactFilter::ContactDetailFilter:
        {
            QContactDetailFilter f(filter);
            if (f.detailType() == QContactDetail::TypeUndefined)
                return QContactInvalidFilter();
        }
        break; // fall through to return at end

        default:
            break; // fall through to return at end
    }
    return filter;
}


/*!
  Returns a whether the supplied \a filter can be implemented
  natively by this engine.  If not, the base class implementation
  will emulate the functionality.
 */
bool QContactManagerEngine::isFilterSupported(const QContactFilter& filter) const
{
    Q_UNUSED(filter);

    return false;
}

/*!
  Returns the list of data types supported by this engine.
 */
QList<QVariant::Type> QContactManagerEngine::supportedDataTypes() const
{
    return QList<QVariant::Type>();
}

/*!
  Returns true if the manager supports the relationship type specified in \a relationshipType for
  contacts whose type is the given \a contactType.

  Note that some managers may support the relationship type for a contact in a limited manner
  (for example, only as the first contact in the relationship, or only as the second contact
  in the relationship).  In this case, it will still return true.  It will only return false
  if the relationship is entirely unsupported for the given type of contact.
 */
bool QContactManagerEngine::isRelationshipTypeSupported(const QString& relationshipType, QContactType::TypeValues contactType) const
{
    Q_UNUSED(relationshipType);
    Q_UNUSED(contactType);

    return false;
}

/*!
  Returns the list of contact types which are supported by this engine.
  This is a convenience function, equivalent to retrieving the allowable values
  for the \c QContactType::FieldType field of the QContactType detail
  which is valid in this engine.
 */
QList<QContactType::TypeValues> QContactManagerEngine::supportedContactTypes() const
{
    QList<QContactType::TypeValues> list;
    // By default, TypeFacet is not supported
    list << QContactType::TypeContact
         << QContactType::TypeGroup;
    return list;
}

/*!
  \fn int QContactManagerEngine::managerVersion() const

  Returns the engine backend implementation version number
 */

/*!
  Returns the list of contact detail types which are supported by this engine.
 */
QList<QContactDetail::DetailType> QContactManagerEngine::supportedContactDetailTypes() const
{
    QList<QContactDetail::DetailType> supportedDetails;
    supportedDetails << QContactAddress::Type
                     << QContactAnniversary::Type
                     << QContactAvatar::Type
                     << QContactBirthday::Type
                     << QContactDisplayLabel::Type
                     << QContactEmailAddress::Type
                     << QContactExtendedDetail::Type
                     << QContactFamily::Type
                     << QContactFavorite::Type
                     << QContactGender::Type
                     << QContactGeoLocation::Type
                     << QContactGlobalPresence::Type
                     << QContactGuid::Type
                     << QContactHobby::Type
                     << QContactName::Type
                     << QContactNickname::Type
                     << QContactNote::Type
                     << QContactOnlineAccount::Type
                     << QContactOrganization::Type
                     << QContactPhoneNumber::Type
                     << QContactPresence::Type
                     << QContactRingtone::Type
                     << QContactSyncTarget::Type
                     << QContactTag::Type
                     << QContactTimestamp::Type
                     << QContactType::Type
                     << QContactUrl::Type
                     << QContactVersion::Type;
    return supportedDetails;
}

/*!
  Checks that the given contact \a contact does not have a type which
  is not supported. It also checks if the details of the given
  \a contact are valid or not.
  Note that this function is unable to ensure that all the details of
  \a contact are supported by a certain back-end. It also cannot
  check that the access constraints (such as CreateOnly and ReadOnly)
  are observed; backend specific code must be written if you wish to
  enforce these constraints.

  Returns true if the \a contact has a valid type, otherwise returns
  false.

  Any errors encountered during this operation should be stored to
  \a error.
 */
bool QContactManagerEngine::validateContact(const QContact &contact, QContactManager::Error *error) const
{
    if (!supportedContactTypes().contains(contact.type())) {
        *error = QContactManager::InvalidContactTypeError;
        return false;
    }
    if ( (!contact.id().isNull()) && (contact.id().managerUri() != this->managerUri())) {
        *error = QContactManager::DoesNotExistError;
        return false;
    }

    QList<QContactDetail> contactDetailList = contact.details();
    for (int i=0; i<contactDetailList.count(); i++)
    {
        QContactDetail currentDetail = contactDetailList.value(i);
        if (!supportedContactDetailTypes().contains(currentDetail.type()))
        {
            *error = QContactManager::InvalidDetailError;
            return false;
        }
    }

    *error = QContactManager::NoError;
    return true;
}

/*!
  Sets the access constraints of \a detail to the supplied \a constraints.

  This function is provided to allow engine implementations to report the
  access constraints of retrieved details, without generally allowing the
  access constraints to be modified after retrieval.

  Application code should not call this function, since validation of the
  detail will happen in the engine in any case.
 */
void QContactManagerEngine::setDetailAccessConstraints(QContactDetail *detail, QContactDetail::AccessConstraints constraints)
{
    if (detail) {
        QContactDetailPrivate::setAccessConstraints(detail, constraints);
    }
}

/*!
  Sets the provenance of \a detail to the supplied \a provenance.

  This function is provided to allow engine implementations to report the
  provenance of retrieved details, without generally allowing the
  provenance metadata to be modified after retrieval.

  The provenance of a detail in an aggregate Contact should include the
  id of the Facet contact and the detail id of the particular detail in
  that Facet contact from which the aggregate Contact's detail was promoted.

  Application code should not call this function, since validation of the
  detail will happen in the engine in any case.
 */
void QContactManagerEngine::setDetailProvenance(QContactDetail *detail, const QString &provenance)
{
    if (detail) {
        QContactDetailPrivate::setProvenance(detail, provenance);
    }
}


/*!
  Adds the given \a contact to the database if \a contact has a
  null id, otherwise updates the contact in
  the database which has the same id to be the given \a contact.
  If the id is not null but does not identify any contact stored in the
  manager, the function will return false and \a error will be set to
  \c QContactManager::DoesNotExistError.

  Returns true if the save operation completed successfully, otherwise
  returns false.  Any error which occurs will be saved in \a error.

  The default implementation will convert this into a call to saveContacts.

  \sa managerUri()
 */
bool QContactManagerEngine::saveContact(QContact* contact, QContactManager::Error* error)
{
    // Convert to a list op
    if (contact) {
        QList<QContact> list;
        list.append(*contact);

        QMap<int, QContactManager::Error> errors;
        bool ret = saveContacts(&list, &errors, error);

        if (errors.count() > 0)
            *error = errors.begin().value();

        *contact = list.value(0);
        return ret;
    } else {
        *error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Remove the contact identified by \a contactId from the database,
  and also removes any relationships in which the contact was involved.
  After the contact has been removed it can not be updated or re-created
  with the same contact id anymore.
  Returns true if the contact was removed successfully, otherwise
  returns false.

  Any error which occurs will be saved in \a error.

  The default implementation will convert this into a call to removeContacts.
 */
bool QContactManagerEngine::removeContact(const QContactId& contactId, QContactManager::Error* error)
{
    // Convert to a list op
    QList<QContactId> list;
    list.append(contactId);

    QMap<int, QContactManager::Error> errors;
    bool ret = removeContacts(list, &errors, error);

    if (errors.count() > 0)
        *error = errors.begin().value();

    return ret;
}

/*!
  Adds the list of contacts given by \a contacts list to the database.
  Returns true if the contacts were saved successfully, otherwise false.

  The manager might populate \a errorMap (the map of indices of the \a contacts list to
  the error which occurred when saving the contact at that index) for
  every index for which the contact could not be saved, if it is able.

  The supplied \a errorMap parameter may be null, if the client does not desire detailed error information.
  If supplied, it will be empty upon entry to this function.

  The \l QContactManager::error() function will only return \c QContactManager::NoError
  if all contacts were saved successfully.

  For each newly saved contact that was successful, the id of the contact
  in the \a contacts list will be updated with the new value.  If a failure occurs
  when saving a new contact, the id will be cleared.

  Any errors encountered during this operation should be stored to
  \a error.

  \sa QContactManager::saveContact()
 */
bool QContactManagerEngine::saveContacts(QList<QContact>* contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(contacts);
    Q_UNUSED(errorMap);
    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Remove every contact whose id is contained in the list of contacts ids
  \a contactIds.  Returns true if all contacts were removed successfully,
  otherwise false.

  Any contact that was removed successfully will have the relationships
  in which it was involved removed also.

  The manager might populate \a errorMap (the map of indices of the \a contactIds list to
  the error which occurred when saving the contact at that index) for every
  index for which the contact could not be removed, if it is able.

  The supplied \a errorMap parameter may be null, if the client does not desire detailed error information.
  If supplied, it will be empty upon entry to this function.

  The \l QContactManager::error() function will
  only return \c QContactManager::NoError if all contacts were removed
  successfully.

  If the list contains ids which do not identify a valid contact in the manager, the function will
  remove any contacts which are identified by ids in the \a contactIds list, insert
  \c QContactManager::DoesNotExist entries into the \a errorMap for the indices of invalid ids
  in the \a contactIds list, return false, and set the overall operation error to
  \c QContactManager::DoesNotExistError.

  Any errors encountered during this operation should be stored to
  \a error.

  \sa QContactManager::removeContact()
 */
bool QContactManagerEngine::removeContacts(const QList<QContactId>& contactIds, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(contactIds);
    Q_UNUSED(errorMap);
    *error = QContactManager::NotSupportedError;
    return false;
}

/* This implements the string comparison behaviour required for compareVariant, amongst others */
static inline int compareStrings(const QString& left, const QString& right, Qt::CaseSensitivity sensitivity)
{
    if (sensitivity == Qt::CaseSensitive) {
        return left.localeAwareCompare(right);
    } else {
        return left.toCaseFolded().localeAwareCompare(right.toCaseFolded());
    }
/*
    // manual implementation of string comparison.
    // should not be necessary / used, as locale aware compare should be sensible.
    // this code exists here for testing / result comparison purposes.
    int retn = -50;
    for (int i = 0; i < left.size(); ++i) {
        if (i >= right.size()) { retn = 1; break; } // right is a substring of left
        const QChar &lc(left[i]);
        const QChar &rc(right[i]);
        const QChar lowerLC = lc.toLower();
        const QChar lowerRC = rc.toLower();
#if 0
        // upper first (ascii-collation)
        if (lc == rc) continue; // characters are identical.
        if (sensitivity == Qt::CaseInsensitive && lowerLC == lowerRC) continue; // lowercase characters are identical.
        if (lc.isLower() && rc.isUpper()) { retn = 1; break; } // left is greater than right
        if (lc.isUpper() && rc.isLower()) { retn = -1; break; } // left is less than right
        retn = (lc < rc ? -1 : 1); break; // both lower, or both upper.  return relative less-than-ism.
#elif 0
        // lower-first
        if (lc == rc) continue; // characters are identical.
        if (sensitivity == Qt::CaseInsensitive && lowerLC == lowerRC) continue; // lowercase characters are identical.
        if (lc.isLower() && rc.isUpper()) { retn = -1; break; } // left is less than right
        if (lc.isUpper() && rc.isLower()) { retn = 1; break; } // left is greater than right
        retn = (lc < rc ? -1 : 1); break; // both lower, or both upper.  return relative less-than-ism.
#elif 0
        // interleaved-upper-first
        if (lc == rc) continue; // characters are identical.
        if (sensitivity == Qt::CaseInsensitive && lowerLC == lowerRC) continue; // lowercase characters are identical.
        if (lowerLC == lowerRC) {
            // we know that lc.isLower() != rc.isLower() otherwise the original lc==rc check would have been true.
            if (lc.isLower()) { retn = 1; break; } // same letter, but left is lowercase :. greater than right.
            else { retn = -1; break; } // same letter, but left is uppercase :. less than right
        } else if (lowerLC < lowerRC) {
            retn = -1; break;
        } else {
            retn = 1; break;
        }
#elif 0
        // interleaved-lower-first
        if (lc == rc) continue; // characters are identical.
        if (sensitivity == Qt::CaseInsensitive && lowerLC == lowerRC) continue; // lowercase characters are identical.
        if (lowerLC == lowerRC) {
            // we know that lc.isLower() != rc.isLower() otherwise the original lc==rc check would have been true.
            if (lc.isLower()) { retn = -1; break; } // same letter, but left is lowercase :. less than right.
            else { retn = 1; break; }// same letter, but left is uppercase :. greater than right
        } else if (lowerLC < lowerRC) {
            retn = -1; break;
        } else {
            retn = 1; break;
        }
#elif 0
        // interleaved-upper-first with first-pass case-insensitive comparison
        if (i == 0) {
            bool firstPassResult = false;
            for (int j = 0; j < qMin(left.size(), right.size()); ++j) {
                 QChar firstpassLLC = left[j].toLower();
                 QChar firstpassLRC = right[j].toLower();
                 if (firstpassLLC < firstpassLRC) { retn = -1; firstPassResult = true; break; }     // e.g. x < Y
                 else if (firstpassLLC > firstpassLRC) { retn = 1; firstPassResult = true; break; } // e.g. x > P
                 else { continue; } // e.g. x == X
            }
            if (firstPassResult) {
                break; // case-insensitive first pass already found result
            } else if (left.size() < right.size()) {
                retn = -1; break; // no difference in case-insensitive comparison, but left is a (case-insensitive) substring of right.
            } else if (left.size() > right.size()) {
                retn = 1; break;  // no difference in case-insensitive comparison, but right is a (case-insensitive) substring of left.
            } else {
            }
        }
        // if we get here, the strings are the same length and differ
        // only by case.  We use the upper-first semantic to resolve.
        if (lc == rc) continue; // characters are identical.
        if (sensitivity == Qt::CaseInsensitive && lowerLC == lowerRC) continue; // lowercase characters are identical.
        if (lc.isUpper() && rc.isLower()) {
            retn = -1; break;
        } else if (lc.isLower() && rc.isUpper()) {
            retn = 1; break;
        } else {
            // characters are equal in case, doesn't help us resolve ordering.
        }
#elif 0
        // interleaved-lower-first with first-pass case-insensitive comparison
        if (i == 0) {
            bool firstPassResult = false;
            for (int j = 0; j < qMin(left.size(), right.size()); ++j) {
                 QChar firstpassLLC = left[j].toLower();
                 QChar firstpassLRC = right[j].toLower();
                 if (firstpassLLC < firstpassLRC) { retn = -1; firstPassResult = true; break; }     // e.g. x < Y
                 else if (firstpassLLC > firstpassLRC) { retn = 1; firstPassResult = true; break; } // e.g. x > P
                 else { continue; } // e.g. x == X
            }
            if (firstPassResult) {
                break; // case-insensitive first pass already found result
            } else if (left.size() < right.size()) {
                retn = -1; break; // no difference in case-insensitive comparison, but left is a (case-insensitive) substring of right.
            } else if (left.size() > right.size()) {
                retn = 1; break;  // no difference in case-insensitive comparison, but right is a (case-insensitive) substring of left.
            } else {
            }
        }
        // if we get here, the strings are the same length and differ
        // only by case.  We use the lower-first semantic to resolve.
        if (lc == rc) continue; // characters are identical.
        if (sensitivity == Qt::CaseInsensitive && lowerLC == lowerRC) continue; // lowercase characters are identical.
        if (lc.isUpper() && rc.isLower()) {
            retn = 1; break;
        } else if (lc.isLower() && rc.isUpper()) {
            retn = -1; break;
        } else {
            // characters are equal in case, doesn't help us resolve ordering.
        }
#endif
    }
    if (retn == -50) {
        if (left.size() == right.size()) {
            retn = 0; // strings are the same
        } else {
            retn = -1; // left is a substr of right, therefore less.
        }
    }
    return retn;
*/
}

/*!
  Compares \a first against \a second.  If the types are
  strings (QVariant::String), the \a sensitivity argument controls
  case sensitivity when comparing.  Also, when comparing strings,
  a locale aware comparison is used, and if the sensitivity is
  CaseSensitive, strings that are identical under a case insensitive
  sort are then sorted case sensitively within that context.

  For example:

  aaron
  Bob
  Aaron
  aAron
  Carol

  would sort as:

  aaron
  aAron
  Aaron
  Bob
  Carol

  Returns:
  <0 if \a first is less than \a second
   0 if \a first is equal to \a second
  >0 if \a first is greater than \a second.

  The results are undefined if the variants are different types, or
  cannot be compared.
 */
int QContactManagerEngine::compareVariant(const QVariant& first, const QVariant& second, Qt::CaseSensitivity sensitivity)
{
    switch(first.type()) {
        case QVariant::Int:
            {
                const int a = first.toInt();
                const int b = second.toInt();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::LongLong:
            {
                const qlonglong a = first.toLongLong();
                const qlonglong b = second.toLongLong();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::Bool:
        case QVariant::UInt:
            {
                const uint a = first.toUInt();
                const uint b = second.toUInt();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::ULongLong:
            {
                const qulonglong a = first.toULongLong();
                const qulonglong b = second.toULongLong();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::Char: // Needs to do proper string comparison
        case QVariant::String:
            return compareStrings(first.toString(), second.toString(), sensitivity);

        case QVariant::Double:
            {
                const double a = first.toDouble();
                const double b = second.toDouble();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::DateTime:
            {
                const QDateTime a = first.toDateTime();
                const QDateTime b = second.toDateTime();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::Date:
            {
                const QDate a = first.toDate();
                const QDate b = second.toDate();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::Time:
            {
                const QTime a = first.toTime();
                const QTime b = second.toTime();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::StringList:
            {
                // We don't actually sort on these, I hope
                // {} < {"aa"} < {"aa","bb"} < {"aa", "cc"} < {"bb"}

                int i;
                const QStringList a = first.toStringList();
                const QStringList b = second.toStringList();
                for (i = 0; i < a.size(); i++) {
                    if (b.size() <= i)
                        return 1; // first is longer
                    int memberComp = compareStrings(a.at(i), b.at(i), sensitivity);
                    if (memberComp != 0)
                        return memberComp;
                    // this element is the same, so loop again
                }

                // Either a.size() < b.size() and they are equal all
                // the way, or a == b
                if (a.size() < b.size())
                    return -1; // a is less than b;
                return 0; // they are equal
            }

        default:
            return 0;
    }
}

/*!
  Returns true if the supplied contact \a contact matches the supplied filter \a filter.

  This function will test each condition in the filter, possibly recursing.
 */
bool QContactManagerEngine::testFilter(const QContactFilter &filter, const QContact &contact)
{
    switch(filter.type()) {
        case QContactFilter::InvalidFilter:
            return false;

        case QContactFilter::DefaultFilter:
            return true;

        case QContactFilter::IdFilter:
            {
                const QContactIdFilter idf(filter);
                if (idf.ids().contains(contact.id()))
                    return true;
            }
            // Fall through to end
            break;

        case QContactFilter::ContactDetailFilter:
            {
                const QContactDetailFilter cdf(filter);
                if (cdf.detailType() == QContactDetail::TypeUndefined)
                    return false;

                /* See if this contact has one of these details in it */
                const QList<QContactDetail>& details = contact.details(cdf.detailType());

                if (details.count() == 0)
                    return false; /* can't match */

                /* See if we need to check the values */
                if (cdf.detailField() == -1)
                    return true;  /* just testing for the presence of a detail of the specified type */

                /* Now figure out what tests we are doing */
                const bool valueTest = cdf.value().isValid();
                const bool presenceTest = !valueTest;

                /* See if we need to test any values at all */
                if (presenceTest) {
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);

                        /* Check that the field is present and has a non-empty value */
                        if (detail.values().contains(cdf.detailField()) && !detail.value(cdf.detailField()).isNull())
                            return true;
                    }
                    return false;
                }

                /* Case sensitivity, for those parts that use it */
                Qt::CaseSensitivity cs = (cdf.matchFlags() & QContactFilter::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;

                /* See what flags are requested, since we're looking at a value */
                if (cdf.matchFlags() & QContactFilter::MatchPhoneNumber) {
                    /* Doing phone number filtering.  We hand roll an implementation here, backends will obviously want to override this. */
                    QString input = cdf.value().toString();

                    /* preprocess the input - ignore any non-digits (doesn't perform ITU-T collation */
                    QString preprocessedInput;
                    for (int i = 0; i < input.size(); i++) {
                        QChar current = input.at(i).toLower();
                        // XXX NOTE: we ignore characters like '+', 'p', 'w', '*' and '#' which may be important.
                        if (current.isDigit()) {
                            preprocessedInput.append(current);
                        }
                    }

                    /* Look at every detail in the set of details and compare */
                    for (int j = 0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& valueString = detail.value(cdf.detailField()).toString();
                        QString preprocessedValueString;
                        for (int i = 0; i < valueString.size(); i++) {
                            QChar current = valueString.at(i).toLower();
                            // note: we ignore characters like '+', 'p', 'w', '*' and '#' which may be important.
                            if (current.isDigit()) {
                                preprocessedValueString.append(current);
                            }
                        }

                        // if the matchflags input don't require a particular criteria to pass, we assume that it has passed.
                        // the "default" match strategy is an "endsWith" strategy.
                        bool me = (cdf.matchFlags() & 7) == QContactFilter::MatchExactly;
                        bool mc = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;
                        bool msw = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                        bool mew = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;

                        bool mer = (me ? preprocessedValueString == preprocessedInput : true);
                        bool mcr = (mc ? preprocessedValueString.contains(preprocessedInput) : true);
                        bool mswr = (msw ? preprocessedValueString.startsWith(preprocessedInput) : true);
                        bool mewr = (mew ? preprocessedValueString.endsWith(preprocessedInput) : true);
                        if (mewr && mswr && mcr && mer) {
                            return true; // this detail meets all of the criteria which were required, and hence must match.
                        }

                        // fallback case: default MatchPhoneNumber compares the rightmost 7 digits, ignoring other matchflags.
                        if (preprocessedValueString.right(7) == preprocessedInput.right(7)) {
                            return true;
                        }
                    }
                } else if (cdf.matchFlags() & QContactFilter::MatchKeypadCollation) {
                    // XXX TODO: not sure about the filtering semantics for MatchKeypadCollation.
                    QString input = cdf.value().toString();

                    /* Look at every detail in the set of details and compare */
                    for (int j = 0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& valueString = detail.value(cdf.detailField()).toString().toLower();

                        // preprocess the valueString
                        QString preprocessedValue;
                        for (int i = 0; i < valueString.size(); i++) {
                            // we use ITU-T keypad collation by default.
                            QChar currentValueChar = valueString.at(i);
                            if (currentValueChar == QLatin1Char('a') || currentValueChar == QLatin1Char('b') || currentValueChar == QLatin1Char('c'))
                                preprocessedValue.append(QLatin1Char('2'));
                            else if (currentValueChar == QLatin1Char('d') || currentValueChar == QLatin1Char('e') || currentValueChar == QLatin1Char('f'))
                                preprocessedValue.append(QLatin1Char('3'));
                            else if (currentValueChar == QLatin1Char('g') || currentValueChar == QLatin1Char('h') || currentValueChar == QLatin1Char('i'))
                                preprocessedValue.append(QLatin1Char('4'));
                            else if (currentValueChar == QLatin1Char('j') || currentValueChar == QLatin1Char('k') || currentValueChar == QLatin1Char('l'))
                                preprocessedValue.append(QLatin1Char('5'));
                            else if (currentValueChar == QLatin1Char('m') || currentValueChar == QLatin1Char('n') || currentValueChar == QLatin1Char('o'))
                                preprocessedValue.append(QLatin1Char('6'));
                            else if (currentValueChar == QLatin1Char('p') || currentValueChar == QLatin1Char('q') || currentValueChar == QLatin1Char('r') || currentValueChar == QLatin1Char('s'))
                                preprocessedValue.append(QLatin1Char('7'));
                            else if (currentValueChar == QLatin1Char('t') || currentValueChar == QLatin1Char('u') || currentValueChar == QLatin1Char('v'))
                                preprocessedValue.append(QLatin1Char('8'));
                            else if (currentValueChar == QLatin1Char('w') || currentValueChar == QLatin1Char('x') || currentValueChar == QLatin1Char('y') || currentValueChar == QLatin1Char('z'))
                                preprocessedValue.append(QLatin1Char('9'));
                            else
                                preprocessedValue.append(currentValueChar);
                        }

                        bool me = (cdf.matchFlags() & 7) == QContactFilter::MatchExactly;
                        bool mc = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;
                        bool msw = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                        bool mew = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;

                        bool mer = (me ? preprocessedValue == input : true);
                        bool mcr = (mc ? preprocessedValue.contains(input) : true);
                        bool mswr = (msw ? preprocessedValue.startsWith(input) : true);
                        bool mewr = (mew ? preprocessedValue.endsWith(input) : true);
                        if (mewr && mswr && mcr && mer) {
                            return true; // this detail meets all of the criteria which were required, and hence must match.
                        }
                    }
                } else if (cdf.matchFlags() & (QContactFilter::MatchEndsWith | QContactFilter::MatchStartsWith | QContactFilter::MatchContains | QContactFilter::MatchFixedString)) {
                    /* We're strictly doing string comparisons here */
                    bool matchStarts = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                    bool matchEnds = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;
                    bool matchContains = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;

                    /* Value equality test */
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& var = detail.value(cdf.detailField()).toString();
                        const QString& needle = cdf.value().toString();
                        if (matchStarts && var.startsWith(needle, cs))
                            return true;
                        if (matchEnds && var.endsWith(needle, cs))
                            return true;
                        if (matchContains && var.contains(needle, cs))
                            return true;
                        if (compareStrings(var, needle, cs) == 0)
                            return true;
                    }
                    return false;
                } else {
                    /* Nope, testing the values as a variant */
                    /* Value equality test */
                    for(int j = 0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QVariant& var = detail.value(cdf.detailField());
                        if (!var.isNull() && compareVariant(var, cdf.value(), cs) == 0)
                            return true;
                    }
                }
            }
            break;

        case QContactFilter::ContactDetailRangeFilter:
            {
                /* The only supported flags are: MatchExactly, MatchFixedString, MatchCaseSensitive */

                const QContactDetailRangeFilter cdf(filter);
                if (cdf.detailType() == QContactDetail::TypeUndefined)
                    return false; /* we do not know which field to check */

                /* See if this contact has one of these details in it */
                const QList<QContactDetail>& details = contact.details(cdf.detailType());

                if (details.count() == 0)
                    return false; /* can't match */

                /* Check for a detail presence test */
                if (cdf.detailField() == -1)
                    return true;

                /* See if this is a field presence test */
                if (!cdf.minValue().isValid() && !cdf.maxValue().isValid()) {
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        if (detail.values().contains(cdf.detailField()))
                            return true;
                    }
                    return false;
                }

                /* open or closed interval testing support */
                const int minComp = cdf.rangeFlags() & QContactDetailRangeFilter::ExcludeLower ? 1 : 0;
                const int maxComp = cdf.rangeFlags() & QContactDetailRangeFilter::IncludeUpper ? 1 : 0;

                /* Case sensitivity, for those parts that use it */
                Qt::CaseSensitivity cs = (cdf.matchFlags() & QContactFilter::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;

                /* See what flags are requested, since we're looking at a value */
                if (cdf.matchFlags() & QContactFilter::MatchFixedString) {
                    /* We're strictly doing string comparisons here */
                    QString minVal = cdf.minValue().toString();
                    QString maxVal = cdf.maxValue().toString();

                    const bool testMin = !minVal.isEmpty();
                    const bool testMax = !maxVal.isEmpty();

                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);

                        // The detail has to have a field of this type in order to be compared.
                        if (!detail.value(cdf.detailField()).isValid())
                            continue;
                        const QString& var = detail.value(cdf.detailField()).toString();
                        if (testMin && compareStrings(var, minVal, cs) < minComp)
                            continue;
                        if (testMax && compareStrings(var, maxVal, cs) >= maxComp)
                            continue;
                        return true;
                    }
                    // Fall through to end
                } else {
                    const bool testMin = cdf.minValue().isValid();
                    const bool testMax = cdf.maxValue().isValid();

                    /* Nope, testing the values as a variant */
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QVariant& var = detail.value(cdf.detailField());

                        // The detail has to have a field of this type in order to be compared.
                        if (!var.isValid())
                            continue;

                        if (testMin && compareVariant(var, cdf.minValue(), cs) < minComp)
                            continue;
                        if (testMax && compareVariant(var, cdf.maxValue(), cs) >= maxComp)
                            continue;
                        return true;
                    }
                    // Fall through to end
                }
            }
            break;

        case QContactFilter::RelationshipFilter:
            {
                // matches any contact that plays the specified role in a relationship
                // of the specified type with the specified other participant.
                const QContactRelationshipFilter rf(filter);

                // first, retrieve contact IDs
                QContactId relatedId = rf.relatedContactId();

                QContactId contactId = contact.id();
                if (relatedId == contactId) {
                    return false;
                }

                // get the relationships in which this contact is involved.
                QList<QContactRelationship> allRelationships;
                allRelationships = contact.relationships();

                // now check to see if we have a match.
                foreach (const QContactRelationship& rel, allRelationships) {
                    // perform the matching.
                    if (rf.relatedContactRole() == QContactRelationship::Second) { // this is the role of the related contact; ie, to match, contact must be the first in the relationship.
                        if ((rf.relationshipType().isEmpty() || rel.relationshipType() == rf.relationshipType())
                                && (rel.first() == contactId) && (relatedId.isNull() || relatedId == rel.second())) {
                            return true;
                        }
                    } else if (rf.relatedContactRole() == QContactRelationship::First) { // this is the role of the related contact; ie, to match, contact must be the second in the relationship.
                        if ((rf.relationshipType().isEmpty() || rel.relationshipType() == rf.relationshipType())
                                && (rel.second() == contactId) && (relatedId.isNull() || relatedId == rel.first())) {
                            return true;
                        }
                    } else { // QContactRelationship::Either
                        if ((rf.relationshipType().isEmpty() || rel.relationshipType() == rf.relationshipType())
                                && (relatedId.isNull() || (relatedId == rel.first() || relatedId == rel.second()))) {
                            return true;
                        }
                    }
                }

                // if not found by now, it doesn't match the filter.
                return false;
            }
            //break; // unreachable.

        case QContactFilter::ChangeLogFilter:
            {
                QContactChangeLogFilter ccf(filter);

                // See what we can do...
                QContactTimestamp ts = contact.detail(QContactTimestamp::Type);

                // See if timestamps are even supported
                if (ts.isEmpty())
                    break;

                if (ccf.eventType() == QContactChangeLogFilter::EventAdded)
                    return ccf.since() <= ts.created();
                if (ccf.eventType() == QContactChangeLogFilter::EventChanged)
                    return ccf.since() <= ts.lastModified();

                // You can't emulate a removed..
                // Fall through to end
            }
            break;

        case QContactFilter::ActionFilter:
            {
                // Find any matching actions, and do a union filter on their filter objects
                QContactActionFilter af(filter);
                QList<QContactActionDescriptor> descriptors = QContactActionManager::instance()->actionDescriptors(af.actionName());

                // There's a small wrinkle if there's a value specified in the action filter
                // we have to adjust any contained QContactDetailFilters to have that value
                // or test if a QContactDetailRangeFilter contains this value already
                for (int j = 0; j < descriptors.count(); j++) {
                    // Action filters are not allowed to return action filters, at all
                    // it's too annoying to check for recursion
                    QContactFilter d = descriptors.at(j).contactFilter();
                    if (!validateActionFilter(d))
                        return false;

                    // Check for values etc...
                    if (testFilter(d, contact))
                        return true;
                }
                // Fall through to end
            }
            break;

        case QContactFilter::IntersectionFilter:
            {
                /* XXX In theory we could reorder the terms to put the native tests first */
                const QContactIntersectionFilter bf(filter);
                const QList<QContactFilter>& terms = bf.filters();
                if (terms.count() > 0) {
                    for(int j = 0; j < terms.count(); j++) {
                        if (!testFilter(terms.at(j), contact)) {
                            return false;
                        }
                    }
                    return true;
                }
                // Fall through to end
            }
            break;

        case QContactFilter::UnionFilter:
            {
                /* XXX In theory we could reorder the terms to put the native tests first */
                const QContactUnionFilter bf(filter);
                const QList<QContactFilter>& terms = bf.filters();
                if (terms.count() > 0) {
                    for(int j = 0; j < terms.count(); j++) {
                        if (testFilter(terms.at(j), contact)) {
                            return true;
                        }
                    }
                    return false;
                }
                // Fall through to end
            }
            break;

        case QContactFilter::CollectionFilter:
            {
                const QContactCollectionFilter cf(filter);
                const QSet<QContactCollectionId>& ids = cf.collectionIds();
                if (ids.contains(contact.collectionId()))
                    return true;
                return false;
            }
    }
    return false;
}

/*!
  Given a QContactFilter \a filter retrieved from a QContactAction,
  check that it is valid and cannot cause infinite recursion.

  In particular, a filter from a QContactAction cannot contain
  any instances of a QContactActionFilter.

  Returns true if \a filter seems ok, or false otherwise.
 */

bool validateActionFilter(const QContactFilter& filter)
{
    QList<QContactFilter> toVerify;
    toVerify << filter;

    while(toVerify.count() > 0) {
        QContactFilter f = toVerify.takeFirst();
        if (f.type() == QContactFilter::ActionFilter)
            return false;
        if (f.type() == QContactFilter::IntersectionFilter)
            toVerify.append(QContactIntersectionFilter(f).filters());
        if (f.type() == QContactFilter::UnionFilter)
            toVerify.append(QContactUnionFilter(f).filters());
    }

    return true;
}

/*!
  Sets the cached relationships in the given \a contact to \a relationships
 */
void QContactManagerEngine::setContactRelationships(QContact* contact, const QList<QContactRelationship>& relationships)
{
    contact->d->m_relationshipsCache = relationships;
}

/*!
  Compares two contacts (\a a and \a b) using the given list of \a sortOrders.  Returns a negative number if \a a should appear
  before \a b according to the sort order, a positive number if \a a should appear after \a b according to the sort order,
  and zero if the two are unable to be sorted.
 */
int QContactManagerEngine::compareContact(const QContact& a, const QContact& b, const QList<QContactSortOrder>& sortOrders)
{
    foreach(const QContactSortOrder& sortOrder, sortOrders) {
        if (!sortOrder.isValid())
            break;

        const QContactDetail::DetailType detailType = sortOrder.detailType();
        const int detailField = sortOrder.detailField();

        const QList<QContactDetail> aDetails = a.details(detailType);
        const QList<QContactDetail> bDetails = b.details(detailType);
        if (aDetails.isEmpty() && bDetails.isEmpty())
            continue; // use next sort criteria.

        // See if we need to check the values
        if (detailField == -1) {
            // just testing for the presence of a detail of the specified definition
            if (aDetails.size() == bDetails.size())
                continue; // use next sort criteria.
            if (aDetails.isEmpty())
                return sortOrder.blankPolicy() == QContactSortOrder::BlanksFirst ? -1 : 1;
            if (bDetails.isEmpty())
                return sortOrder.blankPolicy() == QContactSortOrder::BlanksFirst ? 1 : -1;
            return 0;
        }

        // obtain the values which this sort order concerns
        const QVariant aVal = !aDetails.isEmpty() ? aDetails.first().value(detailField) : QVariant();
        const QVariant bVal = !bDetails.isEmpty() ? bDetails.first().value(detailField) : QVariant();

        bool aIsNull = false;
        bool bIsNull = false;

        // treat empty strings as null qvariants.
        if ((aVal.type() == QVariant::String && aVal.toString().isEmpty()) || aVal.isNull()) {
            aIsNull = true;
        }
        if ((bVal.type() == QVariant::String && bVal.toString().isEmpty()) || bVal.isNull()) {
            bIsNull = true;
        }

        // early exit error checking
        if (aIsNull && bIsNull)
            continue; // use next sort criteria.
        if (aIsNull)
            return (sortOrder.blankPolicy() == QContactSortOrder::BlanksFirst ? -1 : 1);
        if (bIsNull)
            return (sortOrder.blankPolicy() == QContactSortOrder::BlanksFirst ? 1 : -1);

        // real comparison
        int comparison = compareVariant(aVal, bVal, sortOrder.caseSensitivity()) * (sortOrder.direction() == Qt::AscendingOrder ? 1 : -1);
        if (comparison == 0)
            continue;
        return comparison;
    }

    return 0; // or according to id? return (a.id() < b.id() ? -1 : 1);
}

/* A functor that returns true iff a is less than b, according to the sortOrders passed in to the
 * ctor.  The sortOrders pointer passed in must remain valid for the lifetime of the functor. */
class ContactLessThan {
    public:
        ContactLessThan(const QList<QContactSortOrder>* sortOrders) : mSortOrders(sortOrders) {}
        bool operator()(const QContact& a, const QContact& b) const
        {
            return QContactManagerEngine::compareContact(a, b, *mSortOrders) < 0;
        }
    private:
        const QList<QContactSortOrder>* mSortOrders;
};

/*!
  Performs insertion sort of the contact \a toAdd into the \a sorted list, according to the provided \a sortOrders list.
  The first QContactSortOrder in the list has the highest priority: if the contact \a toAdd is deemed equal to another
  in the \a sorted list according to the first QContactSortOrder, the second QContactSortOrder in the list is used (and
  so on until either the contact is inserted or there are no more sort order objects in the list).

  If a contact is equal to another contact according to all sort orders, it is inserted after the previously-added contact.
 */
void QContactManagerEngine::addSorted(QList<QContact>* sorted, const QContact& toAdd, const QList<QContactSortOrder>& sortOrders)
{
    if (sortOrders.count() > 0) {
        ContactLessThan lessThan(&sortOrders);
        QList<QContact>::iterator it(std::upper_bound(sorted->begin(), sorted->end(), toAdd, lessThan));
        sorted->insert(it, toAdd);
    } else {
        // no sort order? just add it to the end
        sorted->append(toAdd);
    }
}

/*! Sorts the given list of contacts \a cs according to the provided \a sortOrders
*/
QList<QContactId> QContactManagerEngine::sortContacts(const QList<QContact>& cs, const QList<QContactSortOrder>& sortOrders)
{
    QList<QContactId> sortedIds;
    QList<QContact> sortedContacts = cs;
    if (!sortOrders.isEmpty()) {
        ContactLessThan lessThan(&sortOrders);
        std::stable_sort(sortedContacts.begin(), sortedContacts.end(), lessThan);
    }

    foreach(const QContact& c, sortedContacts) {
        sortedIds.append(c.id());
    }
    return sortedIds;
}

/*!
  Notifies the manager engine that the given request \a req is in the process of being destroyed.

  The request pointer \a req is still valid during this function call, but before returning
  from this call the engine should ensure that it no longer holds any references
  to the \a req pointer (for example, in a queue in another thread) because directly
  following this call the request will be deleted and this pointer will become invalid.
  In a multithreaded engine, this may mean blocking the calling thread while other
  threads clean up.

  If a request is still in progress at this point, it is undefined what will
  happen to the operation requested, but in general it should either be
  fully completed or fully aborted.  In any case, the client has signalled that
  they do not care about the outcome (by deleting the request).
 */
void QContactManagerEngine::requestDestroyed(QContactAbstractRequest* req)
{
    Q_UNUSED(req);
}

/*!
  Asks the manager engine to begin the given request \a req which
  is currently in a (re)startable state.
  Returns true if the request was started successfully, else returns false.

  \sa QContactAbstractRequest::start()
 */
bool QContactManagerEngine::startRequest(QContactAbstractRequest* req)
{
    Q_UNUSED(req);
    return false;
}

/*!
  Asks the manager engine to cancel the given request \a req which was
  previously started and is currently in a cancellable state.
  Returns true if cancellation of the request was started successfully,
  otherwise returns false.

  \sa startRequest(), QContactAbstractRequest::cancel()
 */
bool QContactManagerEngine::cancelRequest(QContactAbstractRequest* req)
{
    Q_UNUSED(req);
    return false;
}

/*!
  Blocks until the manager engine has completed the given request \a req
  which was previously started, or until \a msecs milliseconds have passed.
  Returns true if the request was completed, and false if the request was not in the
  \c QContactAbstractRequest::Active state or no progress could be reported.

  \sa startRequest()
 */
bool QContactManagerEngine::waitForRequestFinished(QContactAbstractRequest* req, int msecs)
{
    Q_UNUSED(req);
    Q_UNUSED(msecs);
    return false;
}

/*!
  Updates the given asynchronous request \a req by setting the new \a state
  of the request.  If the new state is different, the stateChanged() signal
  will be emitted by the request.
 */
void QContactManagerEngine::updateRequestState(QContactAbstractRequest* req, QContactAbstractRequest::State state)
{
    Q_ASSERT(req);
    QMutexLocker ml(&req->d_ptr->m_mutex);
    bool emitState = req->d_ptr->m_state != state;
    req->d_ptr->m_state = state;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, state));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}


/*!
  Updates the given QContactIdFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactIdFetchRequest(QContactIdFetchRequest* req, const QList<QContactId>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactIdFetchRequestPrivate* rd = static_cast<QContactIdFetchRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_ids = result;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactFetchRequest(QContactFetchRequest* req, const QList<QContact>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactFetchRequestPrivate* rd = static_cast<QContactFetchRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_contacts = result;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactRemoveRequest(QContactRemoveRequest* req, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactRemoveRequestPrivate* rd = static_cast<QContactRemoveRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactSaveRequest(QContactSaveRequest* req, const QList<QContact>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactSaveRequestPrivate* rd = static_cast<QContactSaveRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_contacts = result;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactRelationshipSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateRelationshipSaveRequest(QContactRelationshipSaveRequest* req, const QList<QContactRelationship>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactRelationshipSaveRequestPrivate* rd = static_cast<QContactRelationshipSaveRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_relationships = result;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactRelationshipRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateRelationshipRemoveRequest(QContactRelationshipRemoveRequest* req, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactRelationshipRemoveRequestPrivate* rd = static_cast<QContactRelationshipRemoveRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactRelationshipFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateRelationshipFetchRequest(QContactRelationshipFetchRequest* req, const QList<QContactRelationship>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactRelationshipFetchRequestPrivate* rd = static_cast<QContactRelationshipFetchRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_relationships = result;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactCollectionFetchRequest \a req with the latest results \a result and an operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.
  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateCollectionFetchRequest(QContactCollectionFetchRequest* req, const QList<QContactCollection>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactCollectionFetchRequestPrivate* rd = static_cast<QContactCollectionFetchRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_collections = result;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactCollectionRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.
  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateCollectionRemoveRequest(QContactCollectionRemoveRequest* req, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactCollectionRemoveRequestPrivate* rd = static_cast<QContactCollectionRemoveRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

/*!
  Updates the given QContactCollectionSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.
  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateCollectionSaveRequest(QContactCollectionSaveRequest* req, const QList<QContactCollection>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactCollectionSaveRequestPrivate* rd = static_cast<QContactCollectionSaveRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_collections = result;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}


/*!
  For each contact in \a contacts, either add it to the database or update an existing one.

  This function accepts a \a typeMask, which specifies which details of the contacts should be
  updated.  Details with types not included in the typeMask will not be updated
  or added.

  The manager should populate \a errorMap (the map of indices of the \a contacts list to the error
  which occurred when saving the contact at that index) for every index for which the contact could
  not be saved, if it is able.

  The supplied \a errorMap parameter may be null, if the client does not desire detailed error information.
  If supplied, it will be empty upon entry to this function.

  The \l QContactManager::error() function will only return \c QContactManager::NoError if all
  contacts were saved successfully.

  For each newly saved contact that was successful, the id of the contact in the \a contacts list
  will be updated with the new value.  If a failure occurs when saving a new contact, the id will be
  cleared.

  Any errors encountered during this operation should be stored to \a error.
 */
bool QContactManagerEngine::saveContacts(QList<QContact> *contacts, const QList<QContactDetail::DetailType> &typeMask, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error)
{
    // TODO should the default implementation do the right thing, or return false?
    if (typeMask.isEmpty()) {
        // Non partial, just pass it on
        return saveContacts(contacts, errorMap, error);
    } else {
        // Partial contact save.
        // Basically

        // Need to:
        // 1) fetch existing contacts
        // 2) strip out details in typeMask for existing contacts
        // 3) copy the details from the passed in list for existing contacts
        // 4) for any new contacts, copy the masked details to a blank contact
        // 5) save the modified ones
        // 6) update the id of any new contacts
        // 7) transfer any errors from saving to errorMap

        QList<QContactId> existingContactIds;

        // Error conditions:
        // 1) bad id passed in (can't fetch)
        // 2) bad fetch (can't save partial update)
        // 3) bad save error
        // all of which needs to be returned in the error map

        QHash<int, int> existingIdMap; // contacts index to existingContacts index

        // Try to figure out which of our arguments are new contacts
        for(int i = 0; i < contacts->count(); i++) {
            // See if there's a contactId that's not from this manager
            const QContact c = contacts->at(i);
            if (c.id().managerUri() == managerUri()) {
                existingIdMap.insert(i, existingContactIds.count());
                existingContactIds.append(c.id());
            } else if (!c.id().isNull()) {
                // Hmm, error (wrong manager)
                errorMap->insert(i, QContactManager::DoesNotExistError);
            } // else new contact
        }

        // Now fetch the existing contacts
        QMap<int, QContactManager::Error> fetchErrors;
        QContactManager::Error fetchError = QContactManager::NoError;
        QList<QContact> existingContacts = this->contacts(existingContactIds, QContactFetchHint(),
                                                          &fetchErrors, &fetchError);

        // Prepare the list to save
        QList<QContact> contactsToSave;
        QList<int> savedToOriginalMap; // contactsToSave index to contacts index
        QSet<QContactDetail::DetailType> mask = typeMask.toSet();

        for (int i = 0; i < contacts->count(); i++) {
            // See if this is an existing contact or a new one
            const int fetchedIdx = existingIdMap.value(i, -1);
            QContact contactToSave;
            if (fetchedIdx >= 0) {
                // See if we had an error
                if (fetchErrors[fetchedIdx] != QContactManager::NoError) {
                    errorMap->insert(i, fetchErrors[fetchedIdx]);
                    continue;
                }

                // Existing contact we should have fetched
                contactToSave = existingContacts.at(fetchedIdx);

                QSharedDataPointer<QContactData>& cd = QContactData::contactData(contactToSave);
                cd->removeOnly(mask);
            } else if (errorMap->contains(i)) {
                // A bad argument.  Leave it out of the contactsToSave list
                continue;
            } // else new contact

            // Now copy in the details from the arguments
            const QContact& c = contacts->at(i);

            foreach (QContactDetail::DetailType type, mask) {
                QList<QContactDetail> details = c.details(type);
                foreach(QContactDetail detail, details) {
                    contactToSave.saveDetail(&detail);
                }
            }

            savedToOriginalMap.append(i);
            contactsToSave.append(contactToSave);
        }

        // Now save them
        QMap<int, QContactManager::Error> saveErrors;
        QContactManager::Error saveError = QContactManager::NoError;
        saveContacts(&contactsToSave, &saveErrors, &saveError);

        // Now update the passed in arguments, where necessary

        // Update IDs of the contacts list
        for (int i = 0; i < contactsToSave.count(); i++) {
            (*contacts)[savedToOriginalMap[i]].setId(contactsToSave[i].id());
        }
        // Populate the errorMap with the errorMap of the attempted save
        QMap<int, QContactManager::Error>::iterator it(saveErrors.begin());
        while (it != saveErrors.end()) {
            if (it.value() != QContactManager::NoError) {
                errorMap->insert(savedToOriginalMap[it.key()], it.value());
            }
            it++;
        }

        return errorMap->isEmpty();
    }
}

/*!
  Returns the list of contacts with the ids given by \a contactIds.  There is a one-to-one
  correspondence between the returned contacts and the supplied \a contactIds.

  If there is an invalid id in \a contactIds, then an empty QContact will take its place in the
  returned list and an entry will be inserted into \a errorMap.

  The overall operation error will be saved in \a error.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details, relationships and
  action preferences in the matching contacts will be returned.

  If a non-default fetch hint is supplied, and the client wishes to make changes to the contacts,
  they should ensure that only a detail type hint is supplied and that when saving it back, a
  type mask should be used which corresponds to the detail type hint.  This is to ensure
  that no data is lost by overwriting an existing contact with a restricted version of it.

  \sa QContactFetchHint
 */
QList<QContact> QContactManagerEngine::contacts(const QList<QContactId> &contactIds, const QContactFetchHint &fetchHint, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error) const
{
    QContactIdFilter lif;
    lif.setIds(contactIds);

    QList<QContact> unsorted = contacts(lif, QContactSortOrder(), fetchHint, error);

    // Build an index into the results
    QHash<QContactId, int> idMap; // value is index into unsorted
    if (*error == QContactManager::NoError) {
        for (int i = 0; i < unsorted.size(); i++) {
            idMap.insert(unsorted[i].id(), i);
        }
    }

    // Build up the results and errors
    QList<QContact> results;
    for (int i = 0; i < contactIds.count(); i++) {
        QContactId id(contactIds[i]);
        if (!idMap.contains(id)) {
            if (errorMap)
                errorMap->insert(i, QContactManager::DoesNotExistError);
            if (*error == QContactManager::NoError)
                *error = QContactManager::DoesNotExistError;
            results.append(QContact());
        } else {
            results.append(unsorted[idMap[id]]);
        }
    }

    return results;
}

/*!
  Updates the given QContactFetchByIdRequest \a req with the latest results \a result, and operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactFetchByIdRequest(QContactFetchByIdRequest *req, const QList<QContact> &result, QContactManager::Error error,
                                                          const QMap<int, QContactManager::Error> &errorMap, QContactAbstractRequest::State newState)
{
    Q_ASSERT(req);
    QContactFetchByIdRequestPrivate* rd = static_cast<QContactFetchByIdRequestPrivate*>(req->d_ptr);
    QMutexLocker ml(&rd->m_mutex);
    bool emitState = rd->m_state != newState;
    rd->m_contacts = result;
    rd->m_errors = errorMap;
    rd->m_error = error;
    rd->m_state = newState;
    ml.unlock();
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    QPointer<QContactAbstractRequest> guard(req);
#endif
    Qt::ConnectionType connectionType = Qt::DirectConnection;
#ifdef QT_NO_THREAD
    if (req->thread() != QThread::currentThread())
        connectionType = Qt::BlockingQueuedConnection;
#endif
    QMetaObject::invokeMethod(req, "resultsAvailable", connectionType);
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
    if (emitState)
        QMetaObject::invokeMethod(req, "stateChanged", connectionType, Q_ARG(QContactAbstractRequest::State, newState));
#if !defined(QT_NO_DEBUG) || defined(QT_FORCE_ASSERTS)
    Q_ASSERT(guard);
#endif
}

#include "moc_qcontactmanagerengine.cpp"

QT_END_NAMESPACE_CONTACTS
