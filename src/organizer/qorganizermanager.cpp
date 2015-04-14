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

#include "qorganizermanager.h"
#include "qorganizermanager_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerManager
    \brief The QOrganizerManager class provides an interface which allows clients with access to
           organizer item and collection information stored in a particular backend.
    \inmodule QtOrganizer
    \ingroup organizer-main

    This class provides synchronous methods to retrieve and manipulate organizer item information,
    collection information, as well as functionality reporting and error information reporting.

    Clients can also use the use-case-specific classes derived from QOrganizerAbstractRequest to
    retrieve and manipulate organizer items and collections in an asynchronous manner. However,
    certain functionality (e.g. backend functionality reporting) can not be accessed using the
    asynchronous APIs.

    See the \l{Qt Organizer Synchronous API}{synchronous} and \l{Qt Organizer Asynchronous API}{asynchronous}
    API information from the \l{Qt Organizer Overview}{organizer module} API documentation for more details.

    When constructing a QOrganizerManager instance, certain parameters can be given to provide more
    control, e.g. to specify the version of the backend it wants to construct. Note that the parameters
    returned when calling managerParameters() are not necessarily the same as the ones passed in,
    since certain parameters might be discarded or added by the backend.
 */

/*!
    \enum QOrganizerManager::Operation

    This enumeration describes the operation that has been done to the item or collection modified.
    \value Add     The item / collection has been added.
    \value Change  The item / collection has been changed.
    \value Remove  The item / collection has been removed.
 */

/*!
    \fn QOrganizerManager::itemsModified(const QList<QPair<QOrganizerItemId, QOrganizerManager::Operation> > &itemIds);

    This signal is emitted at some point once the items have been modified in a datastore managed by
    this manager. In the \a itemIds list, the item ID tells which item has been modified with the
    corresponding operation.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManager::collectionsModified(const QList<QPair<QOrganizerCollectionId, QOrganizerManager::Operation> > &collectionIds);

    This signal is emitted at some point once the collections have been modified in a datastore managed
    by this manager. In the \a collectionIds list, the collection ID tells which item has been modified
    with the corresponding operation.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \macro QTORGANIZER_BACKEND_VERSION
    \relates QOrganizerManager

    This macro tells the parameter is to specify the version the backend that the customer wants to
    create.

    If the specified version is not available, a backend with the given name and a default version
    will be created for this manager.
 */

/*!
    \fn QOrganizerManager::dataChanged()

    This signal is emitted by the manager if its internal state changes, and it is unable to determine
    the changes which occurred, or if the manager considers the changes to be radical enough to require
    clients to reload all data.

    If this signal is emitted, no other signals will be emitted for the associated changes.

    \sa itemsAdded(), itemsChanged(), itemsRemoved()
 */

/*!
    \fn QOrganizerManager::itemsAdded(const QList<QOrganizerItemId> &itemIds)

    This signal is emitted at some point once the items identified by \a itemIds have been added to
    a datastore managed by this manager.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManager::itemsChanged(const QList<QOrganizerItemId> &itemIds)

    This signal is emitted at some point once the items identified by \a itemIds have been modified
    in a datastore managed by this manager.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManager::itemsRemoved(const QList<QOrganizerItemId> &itemIds)

    This signal is emitted at some point once the items identified by \a itemIds have been removed
    from a datastore managed by this manager.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManager::collectionsAdded(const QList<QOrganizerCollectionId> &collectionIds)

    This signal is emitted at some point once the collections identified by \a collectionIds have
    been added to a datastore managed by this manager.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManager::collectionsChanged(const QList<QOrganizerCollectionId> &collectionIds)

    This signal is emitted at some point once the metadata for the collections identified by \a collectionIds
    have been modified in a datastore managed by this manager.

    This signal will not be emitted if items in the collections have been added, modified, or removed.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManager::collectionsRemoved(const QList<QOrganizerCollectionId> &collectionIds)

    This signal is emitted at some point once the collections identified by \a collectionIds have
    been removed from a datastore managed by this manager.

    This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.

    \sa dataChanged()
 */

#define makestr(x) (#x)
#define makename(x) makestr(x)

/*!
    Returns a list of available manager names that can be used to construct a QOrganizerManager.
 */
QStringList QOrganizerManager::availableManagers()
{
    QStringList ret;
    ret << QStringLiteral("invalid");
    QOrganizerManagerData::loadFactories();
    ret.append(QOrganizerManagerData::m_engines.keys());

    // now swizzle the default engine to pole position
#if defined(Q_ORGANIZER_DEFAULT_ENGINE)
    if (ret.removeAll(QLatin1String(makename(Q_ORGANIZER_DEFAULT_ENGINE))))
        ret.prepend(QLatin1String(makename(Q_ORGANIZER_DEFAULT_ENGINE)));
#endif

    return ret;
}

/*!
    Splits the given \a uri into the manager name and parameters that it describes,
    and places the information into the memory addressed by \a managerName and \a params respectively.
    Returns true if \a uri could be split successfully, otherwise returns false.
 */
bool QOrganizerManager::parseUri(const QString &uri, QString *managerName, QMap<QString, QString> *params)
{
    return QOrganizerManagerData::parseUri(uri, managerName, params, false);
}

/*!
    Returns a URI that describes a manager name, parameters, and version with which to instantiate
    a manager object, from the given \a managerName and \a params.
 */
QString QOrganizerManager::buildUri(const QString &managerName, const QMap<QString, QString> &params)
{
    return QOrganizerManagerData::buildUri(managerName, params);
}

/*!
    Constructs a QOrganizerManager whose name, parameters, and version are specified in the given
    \a uri, and whose parent object is \a parent.
 */
QOrganizerManager *QOrganizerManager::fromUri(const QString &uri, QObject *parent)
{
    if (uri.isEmpty()) {
        return new QOrganizerManager(QString(), QMap<QString, QString>(), parent);
    } else {
        QString id;
        QMap<QString, QString> parameters;
        if (parseUri(uri, &id, &parameters))
            return new QOrganizerManager(id, parameters, parent);
        else
            return new QOrganizerManager(QStringLiteral("invalid"), QMap<QString, QString>(), parent);
    }
}

/*!
    Constructs a QOrganizerManager whose parent object is \a parent.

    The default backend, i.e. the first one returned by the availableManagers() function, for the
    platform will be created.
 */
QOrganizerManager::QOrganizerManager(QObject *parent)
    : QObject(parent), d(new QOrganizerManagerData)
{
    createEngine(QString(), QMap<QString, QString>());
}

/*!
    Constructs a QOrganizerManager whose backend is identified by \a managerName with the given
    \a parameters, and the parent object is \a parent.

    The default backend, i.e. the first one returned by the availableManagers() function, for the
    platform will be created, if the \a managerName is empty. If the backend identified by \a managerName
    does not exist, an invalid backend is created.
 */
QOrganizerManager::QOrganizerManager(const QString &managerName, const QMap<QString, QString> &parameters, QObject *parent)
    : QObject(parent), d(new QOrganizerManagerData)
{
    createEngine(managerName, parameters);
}

void QOrganizerManager::createEngine(const QString &managerName, const QMap<QString, QString> &parameters)
{
    d->createEngine(managerName, parameters);

    connect(d->m_engine, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(d->m_engine, SIGNAL(itemsAdded(QList<QOrganizerItemId>)), this, SIGNAL(itemsAdded(QList<QOrganizerItemId>)));
    connect(d->m_engine, SIGNAL(itemsChanged(QList<QOrganizerItemId>, QList<QOrganizerItemDetail::DetailType>)), this, SIGNAL(itemsChanged(QList<QOrganizerItemId>, QList<QOrganizerItemDetail::DetailType>)));
    connect(d->m_engine, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)), this, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)));
    connect(d->m_engine, SIGNAL(itemsModified(QList<QPair<QOrganizerItemId,QOrganizerManager::Operation> >)), this, SIGNAL(itemsModified(QList<QPair<QOrganizerItemId,QOrganizerManager::Operation> >)));
    connect(d->m_engine, SIGNAL(collectionsAdded(QList<QOrganizerCollectionId>)), this, SIGNAL(collectionsAdded(QList<QOrganizerCollectionId>)));
    connect(d->m_engine, SIGNAL(collectionsChanged(QList<QOrganizerCollectionId>)), this, SIGNAL(collectionsChanged(QList<QOrganizerCollectionId>)));
    connect(d->m_engine, SIGNAL(collectionsRemoved(QList<QOrganizerCollectionId>)), this, SIGNAL(collectionsRemoved(QList<QOrganizerCollectionId>)));
    connect(d->m_engine, SIGNAL(collectionsModified(QList<QPair<QOrganizerCollectionId,QOrganizerManager::Operation> >)), this, SIGNAL(collectionsModified(QList<QPair<QOrganizerCollectionId,QOrganizerManager::Operation> >)));
    connect(d->m_engine, SIGNAL(itemsChanged(QList<QOrganizerItemId>, QList<QOrganizerItemDetail::DetailType>)), this, SLOT(_q_itemsUpdated(QList<QOrganizerItemId>, QList<QOrganizerItemDetail::DetailType>)));
    connect(d->m_engine, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)), this, SLOT(_q_itemsDeleted(QList<QOrganizerItemId>)));
}

/*!
    Frees the memory used by the QOrganizerManager.
 */
QOrganizerManager::~QOrganizerManager()
{
    delete d;
}

/*!
    \enum QOrganizerManager::Error

    This enum specifies an error that occurred during the most recent operation:

    \value NoError The most recent operation was successful
    \value DoesNotExistError The most recent operation failed because the requested organizer item or detail definition does not exist
    \value AlreadyExistsError The most recent operation failed because the specified organizer item or detail definition already exists
    \value InvalidDetailError The most recent operation failed because the specified organizer detail definition already exists
    \value LockedError The most recent operation failed because the datastore specified is currently locked
    \value DetailAccessError The most recent operation failed because a detail was modified or removed and its access method does not allow that
    \value PermissionsError The most recent operation failed because the caller does not have permission to perform the operation
    \value OutOfMemoryError The most recent operation failed due to running out of memory
    \value NotSupportedError The most recent operation failed because the requested operation is not supported in the specified store
    \value BadArgumentError The most recent operation failed because one or more of the parameters to the operation were invalid
    \value UnspecifiedError The most recent operation failed for an undocumented reason
    \value LimitReachedError The most recent operation failed because the limit for that type of object has been reached
    \value InvalidItemTypeError The most recent operation failed because the item given was of an invalid type for the operation
    \value InvalidCollectionError The most recent operation failed because the collection is invalid
    \value InvalidOccurrenceError The most recent operation failed because it was an attempt to save an occurrence without a correct InstanceOrigin detail
    \value TimeoutError The most recent operation failed because it took longer than expected.  It may be possible to try again.

    Backend specific documentation might have more details on exact error cases.
 */

/*!
    Return the error code of the most recent operation.

    \sa errorMap()
 */
QOrganizerManager::Error QOrganizerManager::error() const
{
    return d->m_lastError;
}

/*!
    Returns per-input error codes for the most recent operation.

    This function only returns meaningful information if the most recent operation was a batch
    operation.

    Each key in the map is the index of the element in the input list for which the error (whose
    error code is stored in the value for that key in the map) occurred during the batch operation.

    \sa error(), saveItems(), removeItems()
 */
QMap<int, QOrganizerManager::Error> QOrganizerManager::errorMap() const
{
    return d->m_lastErrorMap;
}

/*!
    Returns a list of a maximum of \a maxCount organizer item instances which are occurrences of
    the given \a parentItem recurring item, which occur between the given \a startDateTime and the
    given \a endDateTime date, inclusive.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items.

    Note that backends will decide how many occurrences are returned if \a maxCount is negative.

    The \a fetchHint allows clients to specify which pieces of information they are interested or
    not interested in, to allow backends to optimise data retrieval if possible. Note that it is
    simply a hint; backends can ignore the \a fetchHint and return the full item. If a client makes
    changes to an organizer item which has been retrieved with a fetch hint, they should save it
    back using a partial save, masked by the same set of detail types in order to avoid information
    loss.
  */
QList<QOrganizerItem> QOrganizerManager::itemOccurrences(const QOrganizerItem &parentItem,const QDateTime &startDateTime,
                                                         const QDateTime &endDateTime, int maxCount,
                                                         const QOrganizerItemFetchHint &fetchHint)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->itemOccurrences(parentItem, startDateTime, endDateTime, maxCount, fetchHint, &h.error);
}

/*!
    Returns a list of item IDs of persisted organizer items that match the given \a filter, sorted
    according to the given list of \a sortOrders, for any item which occurs (or has an occurrence
    which occurs) in the range specified by the given \a startDateTime and \a endDateTime, inclusive.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    Note that certain backends may ignore the given \a filter.
 */
QList<QOrganizerItemId> QOrganizerManager::itemIds(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                   const QOrganizerItemFilter &filter, const QList<QOrganizerItemSortOrder> &sortOrders)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->itemIds(filter, startDateTime, endDateTime, sortOrders, &h.error);
}

/*!
    Returns a list of a maximum of \a maxCount organizer items and occurrences that match the given
    \a filter, which occur in the range specified by the given \a startDateTime and \a endDateTime,
    inclusive, and sorted according to the given list of \a sortOrders.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    If no sort order is provided, the list is returned sorted by date.

    Note that certain backends may ignore the given \a filter.

    Note that backends will decide how many items are returned if \a maxCount is negative.

    The \a fetchHint allows clients to specify which pieces of information they are interested or
    not interested in, to allow backends to optimise data retrieval if possible. Note that it is
    simply a hint; backends can ignore the \a fetchHint and return the full item. If a client makes
    changes to an organizer item which has been retrieved with a fetch hint, they should save it
    back using a partial save, masked by the same set of detail types in order to avoid information
    loss.
 */
QList<QOrganizerItem> QOrganizerManager::items(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                               const QOrganizerItemFilter &filter, int maxCount,
                                               const QList<QOrganizerItemSortOrder> &sortOrders,
                                               const QOrganizerItemFetchHint &fetchHint)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->items(filter, startDateTime, endDateTime, maxCount, sortOrders, fetchHint, &h.error);
}

/*!
    Returns a list of organizer items that match the given \a filter, sorted according to the given
    list of \a sortOrders, for any item which occurs (or has an occurrence which occurs) in the range
    specified by the given \a startDateTime and \a endDateTime, inclusive.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    This function will only return parent items and persisted exceptions which match the specified
    criteria; not generated occurrences.

    Note that certain backends may ignore the given \a filter.

    The \a fetchHint allows clients to specify which pieces of information they are interested or
    not interested in, to allow backends to optimise data retrieval if possible. Note that it is
    simply a hint; backends can ignore the \a fetchHint and return the full item. If a client makes
    changes to an organizer item which has been retrieved with a fetch hint, they should save it
    back using a partial save, masked by the same set of detail types in order to avoid information
    loss.
 */
QList<QOrganizerItem> QOrganizerManager::itemsForExport(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                        const QOrganizerItemFilter &filter,
                                                        const QList<QOrganizerItemSortOrder> &sortOrders,
                                                        const QOrganizerItemFetchHint &fetchHint)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->itemsForExport(startDateTime, endDateTime, filter, sortOrders, fetchHint, &h.error);
}

/*!
    Returns the organizer items in the database identified by \a itemIds.

    The items fetched by the backend should have a one-to-one correspondence to the IDs passed into
    this class.  That is, the nth item in the returned list should have an ID which is equal to the
    nth ID in the list of IDs.  Any invalid ID should correspond to an empty QOrganizerItem.

    The \a fetchHint allows clients to specify which pieces of information they are interested or
    not interested in, to allow backends to optimise data retrieval if possible. Note that it is
    simply a hint; backends can ignore the \a fetchHint and return the full item. If a client makes
    changes to an organizer item which has been retrieved with a fetch hint, they should save it
    back using a partial save, masked by the same set of detail types in order to avoid information
    loss.

    Calling \l errorMap() will return the per-input errors for the fetch. The \l QOrganizerManager::error()
    function will only return \c QOrganizerManager::NoError if all organizer items are fetched
    successfully.
 */
QList<QOrganizerItem> QOrganizerManager::items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->items(itemIds, fetchHint, &h.errorMap, &h.error);
}

/*!
    Returns the organizer item in the database identified by \a itemId, or an empty item if the
    specified item does not exist.

    The \a fetchHint allows clients to specify which pieces of information they are interested or
    not interested in, to allow backends to optimise data retrieval if possible. Note that it is
    simply a hint; backends can ignore the \a fetchHint and return the full item. If a client makes
    changes to an organizer item which has been retrieved with a fetch hint, they should save it
    back using a partial save, masked by the same set of detail types in order to avoid information
    loss.
 */
QOrganizerItem QOrganizerManager::item(const QOrganizerItemId& itemId, const QOrganizerItemFetchHint& fetchHint)
{
    QList<QOrganizerItem> tmp = items(QList<QOrganizerItemId>() << itemId, fetchHint);
    if (tmp.size() > 0)
        return tmp.at(0);
    return QOrganizerItem();
}

/*!
    Saves the given \a item to the backend, and returns true on success or false otherwise.

    A new organizer item will be created in the backend store if the item ID of it is null. Otherwise,
    an existing item with the same ID will be updated. If the given item ID does not exist in the
    backend, it will result a QOrganizerManager::DoesNotExistError error.

    If the collection ID of the item is null, it will be saved to the default collection. If the given
    collection ID doesn't exist, the saving will fail.

    If the \a detailMask is empty, only the details currently existing in the item will be saved.
    Otherwise, only details masked by the \a detailMask will be saved or updated, others are kept
    untouched. It's useful to avoid information loss if the items are retrieved with a fetch hint.

    Note that upon successful saving, the backend may update the item, e.g. item ID for newly saved
    items, GUID, timestamp, version, etc.

    \sa saveItems()
 */
bool QOrganizerManager::saveItem(QOrganizerItem *item, const QList<QOrganizerItemDetail::DetailType> &detailMask)
{
    QList<QOrganizerItem> items;
    items.append(*item);
    bool rtn = saveItems(&items, detailMask);
    *item = items.at(0);
    return rtn;
}

/*!
    Removes all the item whose ID is \a itemId, and all its occurrences. Returns true if the item
    and occurrences are successfully removed, or false otherwise.

    \sa removeItems()
 */
bool QOrganizerManager::removeItem(const QOrganizerItemId& itemId)
{
    return removeItems(QList<QOrganizerItemId>() << itemId);
}

/*!
    Remove the organizer \a item from the database. If \a item is a generated occurrence,
    the start date of the occurrence is added to its parent item's exception date list.
    Returns true if the organizer item was removed successfully, otherwise returns false.

    \sa removeItems()
 */
bool QOrganizerManager::removeItem(const QOrganizerItem *item)
{
    QList<QOrganizerItem> list;
    list.append(*item);
    return removeItems(&list);
}

/*!
    Saves the given list of \a items to the backend, and returns true on success or false otherwise.

    A new organizer item will be created in the backend store if the item ID of it is null. Otherwise,
    an existing item with the same ID will be updated. If the given item ID does not exist in the
    backend, it will result a QOrganizerManager::DoesNotExistError error.

    If the collection ID of the item is null, it will be saved to the default collection. If the given
    collection ID doesn't exist, the saving will fail.

    If the \a detailMask is empty, only the details currently existing in the item will be saved.
    Otherwise, only details masked by the \a detailMask will be saved or updated, others are kept
    untouched. It's useful to avoid information loss if the items are retrieved with a fetch hint.

    Note that upon successful saving, the backend may update the item, e.g. item ID for newly saved
    items, GUID, timestamp, version, etc.

    Calling errorMap() will return the per-input errors for the operation. The error() function will
    only return QOrganizerManager::NoError if all organizer items were saved successfully.

    \sa saveItem()
 */
bool QOrganizerManager::saveItems(QList<QOrganizerItem>* items, const QList<QOrganizerItemDetail::DetailType> &detailMask)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    if (!items) {
        h.error = QOrganizerManager::BadArgumentError;
        return false;
    }
    return d->m_engine->saveItems(items, detailMask, &h.errorMap, &h.error);
}

/*!
    Removes all the items whose ID is contained in the given list of \a itemIds, and all the occurrences
    whose parent ID is containd in the \a itemIds. Returns true if all the items and occurrences
    are successfully removed, or false otherwise.

    Calling errorMap() will return the per-input errors for the operation. The error() function will
    only return QOrganizerManager::NoError if all organizer items were saved successfully.

    \sa removeItem()
 */
bool QOrganizerManager::removeItems(const QList<QOrganizerItemId> &itemIds)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    if (itemIds.isEmpty()) {
        h.error = QOrganizerManager::BadArgumentError;
        return false;
    }
    return d->m_engine->removeItems(itemIds, &h.errorMap, &h.error);
}

/*!
   Removes every organizer item which is contained in the list of organizer items
   \a items. If the list contains generated occurrences the start date of the occurrence
   is added to its parent item's exception date list. Returns true if all organizer items
   were removed successfully, otherwise false.

   Calling \l errorMap() will return the per-input errors for the latest batch function.
   The \l QOrganizerManager::error() function will only return \c QOrganizerManager::NoError
   if all organizer items were removed successfully.

   If the given list of organizer \a items is empty, the function will return false
   and calling error() will return \c QOrganizerManager::BadArgumentError.  If the list is non-empty
   and contains items which are not a valid organizer item in the manager, the function will
   remove any valid organizer items in the \a items list, insert \c QOrganizerManager::DoesNotExist
   entries into the error map for the indices of invalid id in the \a items list, return false,
   and set the overall operation error to \c QOrganizerManager::DoesNotExistError.

   \sa QOrganizerManager::removeItem()
 */

bool QOrganizerManager::removeItems(const QList<QOrganizerItem> *items)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    if (items->isEmpty()) {
        h.error = QOrganizerManager::BadArgumentError;
        return false;
    }

    return d->m_engine->removeItems(items, &h.errorMap, &h.error);
}

/*!
    Returns the id of a default collection managed by this manager.
    There is always only one default collection for each backend.
 */
QOrganizerCollectionId QOrganizerManager::defaultCollectionId() const
{
    return d->m_engine->defaultCollectionId();
}

/*!
  Returns the collection identified by the given \a collectionId which is managed by this manager.
 */
QOrganizerCollection QOrganizerManager::collection(const QOrganizerCollectionId& collectionId)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->collection(collectionId, &h.error);
}

/*!
  Returns a list of all of the collections managed by this manager.
 */
QList<QOrganizerCollection> QOrganizerManager::collections()
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->collections(&h.error);
}

/*!
    Saves the given \a collection to the backend, and returns true on success or false otherwise.

    Note that certain backends may not allow modification nor adding new collections, in such cases
    the operation will fail and result a QOrganizerManager::PermissionsError error.

    A new collection will be created in the backend store if the collection ID of it is null.
    Otherwise, an existing collection with the same ID will be updated. If the given collection ID
    does not exist in the backend, it will result a QOrganizerManager::DoesNotExistError error.

    Note that upon successful saving, the backend may update the collection, e.g. collection ID for
    newly saved collections.
 */
bool QOrganizerManager::saveCollection(QOrganizerCollection* collection)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    if (collection) {
        return d->m_engine->saveCollection(collection, &h.error);
    } else {
        h.error = QOrganizerManager::BadArgumentError;
        return false;
    }
}

/*!
    Removes the collection identified by the given \a collectionId (and all items in the collection)
    from the manager. Returns true on success, false on failure.

    If the given \a collectionId does not exist, the operation will fail and QOrganizerManager::DoesNotExistError
    will be returned when calling error().

    If the given \a collectionId refers to the default collection, the operation will fail and
    QOrganizerManager::PermissionsError will be returned when calling error().
 */
bool QOrganizerManager::removeCollection(const QOrganizerCollectionId &collectionId)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->removeCollection(collectionId, &h.error);
}

/*!
    Returns the list of supported filters by this manager.
 */
QList<QOrganizerItemFilter::FilterType> QOrganizerManager::supportedFilters() const
{
    return d->m_engine->supportedFilters();
}

/*!
    Returns the list of details that are supported by this manager for the given \a itemType.
 */
QList<QOrganizerItemDetail::DetailType> QOrganizerManager::supportedItemDetails(QOrganizerItemType::ItemType itemType) const
{
    return d->m_engine->supportedItemDetails(itemType);
}

/*!
    Returns the list of organizer item types supported by this manager.
 */
QList<QOrganizerItemType::ItemType> QOrganizerManager::supportedItemTypes() const
{
    return d->m_engine->supportedItemTypes();
}

/*!
    Returns the manager name for the backend.
*/
QString QOrganizerManager::managerName() const
{
    return d->m_engine->managerName();
}

/*!
    Return the parameters used by the backend.

    Note that if certain paramters are invalid, or discarded by the backend, they will not be returned.
    Also, the backend might add certain parameters when being constructed.
*/
QMap<QString, QString> QOrganizerManager::managerParameters() const
{
    return d->m_engine->managerParameters();
}

/*!
    Returns the version of the backend.

    The version is stored as part of the parameters, with the key QTORGANIZER_BACKEND_VERSION.
 */
int QOrganizerManager::managerVersion() const
{
    return managerParameters().value(QTORGANIZER_BACKEND_VERSION).toInt();
}

/*!
    Return the URI describing the backend.
 */
QString QOrganizerManager::managerUri() const
{
    return d->m_engine->managerUri();
}

/*!
    Return a list of QOrganizerItemId extracted from the \a items.
*/
QList<QOrganizerItemId> QOrganizerManager::extractIds(const QList<QOrganizerItem> &items)
{
    QList<QOrganizerItemId> ids;
    ids.reserve(items.count());
    foreach (const QOrganizerItem &item, items)
        ids.append(item.id());
    return ids;
}

#include "moc_qorganizermanager.cpp"

QT_END_NAMESPACE_ORGANIZER
