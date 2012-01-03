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

#include "qorganizermanager.h"

#include "qorganizeritem_p.h"
#include "qorganizeritemfilter.h"
#include "qorganizermanager_p.h"
#include "qorganizeritemfetchhint.h"

#include <QSharedData>
#include <QPair>
#include <QSet>

QTORGANIZER_BEGIN_NAMESPACE

/*!
  \class QOrganizerManager
  \brief The QOrganizerManager class provides an interface which allows clients with access to organizer item information stored in a particular backend.

  \inmodule QtOrganizer
  \ingroup organizer-main

  This class provides an abstraction of a datastore or aggregation of datastores which contains organizer item information.
  It provides methods to retrieve and manipulate organizer item information, collection information and supported schema definitions.
  It also provides metadata and error information reporting.

  The functions provided by QOrganizerManager are purely synchronous; to access the same functionality in an
  asynchronous manner, clients should use the use-case-specific classes derived from QOrganizerAbstractRequest.

  Some functionality provided by QOrganizerManager directly is not accessible using the asynchronous API; see
  the \l{Organizer Synchronous API}{synchronous} and \l{Organizer Asynchronous API}{asynchronous} API
  information from the \l{C++ Organizer}{organizer module} API documentation.
 */

/*!
  \fn QOrganizerManager::dataChanged()
  This signal is emitted by the manager if its internal state changes, and it is unable to determine the changes
  which occurred, or if the manager considers the changes to be radical enough to require clients to reload all data.
  If this signal is emitted, no other signals will be emitted for the associated changes.
 */

/*!
  \fn QOrganizerManager::itemsAdded(const QList<QOrganizerItemId>& itemIds)
  This signal is emitted at some point once the items identified by \a itemIds have been added to a datastore managed by this manager.
  This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QOrganizerManager::itemsChanged(const QList<QOrganizerItemId>& itemIds)
  This signal is emitted at some point once the items identified by \a itemIds have been modified in a datastore managed by this manager.
  This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QOrganizerManager::itemsRemoved(const QList<QOrganizerItemId>& itemIds)
  This signal is emitted at some point once the items identified by \a itemIds have been removed from a datastore managed by this manager.
  This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QOrganizerManager::collectionsAdded(const QList<QOrganizerCollectionId>& collectionIds)
  This signal is emitted at some point once the collections identified by \a collectionIds have been added to a datastore managed by this manager.
  This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QOrganizerManager::collectionsChanged(const QList<QOrganizerCollectionId>& collectionIds)
  This signal is emitted at some point once the metadata for the collections identified by \a collectionIds have been modified in a datastore managed by this manager.
  This signal is not emitted if one of the items in this collection has changed - itemsChanged() will be emitted instead.
  This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.
 */

/*!
  \fn QOrganizerManager::collectionsRemoved(const QList<QOrganizerCollectionId>& collectionIds)
  This signal is emitted at some point once the collections identified by \a collectionIds have been removed from a datastore managed by this manager.
  This signal will not be emitted if the dataChanged() signal was previously emitted for these changes.
 */



#define makestr(x) (#x)
#define makename(x) makestr(x)

/*!
    Returns a list of available manager ids that can be used when constructing
    a QOrganizerManager.  If an empty id is specified to the constructor, the
    first value in this list will be used instead.
  */
QStringList QOrganizerManager::availableManagers()
{
    QStringList ret;
    ret << QLatin1String("invalid");
    QOrganizerManagerData::loadFactories();
    ret.append(QOrganizerManagerData::m_engines.keys());

    // now swizzle the default engine to pole position
#if defined(Q_ORGANIZER_DEFAULT_ENGINE)
    if (ret.removeAll(QLatin1String(makename(Q_ORGANIZER_DEFAULT_ENGINE)))) {
        ret.prepend(QLatin1String(makename(Q_ORGANIZER_DEFAULT_ENGINE)));
    }
#endif

    return ret;
}

/*!
    Splits the given \a uri into the manager name and parameters that it describes, and places the
    information into the memory addressed by \a pManagerName and \a pParams respectively. Returns true
    if \a uri could be split successfully, otherwise returns false.
 */
bool QOrganizerManager::parseUri(const QString &uri, QString *pManagerName, QMap<QString, QString> *pParams)
{
    // Format: qtorganizer:<managerid>:<key>=<value>&<key>=<value>
    // 1) parameters are currently a qstringlist.. should they be a map?
    // 2) is the uri going to be escaped?  my guess would be "probably not"
    // 3) hence, do we assume that the prefix, managerid and storeid cannot contain `:'
    // 4) similarly, that neither keys nor values can contain `=' or `&'

    QStringList colonSplit = uri.split(QLatin1Char(':'));
    QString prefix = colonSplit.value(0);

    if (prefix != QLatin1String("qtorganizer"))
        return false;

    QString managerName = colonSplit.value(1);

    if (managerName.trimmed().isEmpty())
        return false;

    QString firstParts = prefix + QLatin1Char(':') + managerName + QLatin1Char(':');
    QString paramString = uri.mid(firstParts.length());

    QMap<QString, QString> outParams;

    // Now we have to decode each parameter
    if (!paramString.isEmpty()) {
        QStringList params = paramString.split(QRegExp(QLatin1String("&(?!(amp;|equ;))")), QString::KeepEmptyParts);
        // If we have an empty string for paramstring, we get one entry in params,
        // so skip that case.
        for (int i = 0; i < params.count(); ++i) {
            /* This should be something like "foo&amp;bar&equ;=grob&amp;" */
            QStringList paramChunk = params.value(i).split(QLatin1String("="), QString::KeepEmptyParts);

            if (paramChunk.count() != 2)
                return false;

            QString arg = paramChunk.value(0);
            QString param = paramChunk.value(1);
            arg.replace(QLatin1String("&equ;"), QLatin1String("="));
            arg.replace(QLatin1String("&amp;"), QLatin1String("&"));
            param.replace(QLatin1String("&equ;"), QLatin1String("="));
            param.replace(QLatin1String("&amp;"), QLatin1String("&"));
            if (arg.isEmpty())
                return false;
            outParams.insert(arg, param);
        }
    }

    if (pParams)
        *pParams = outParams;
    if (pManagerName)
        *pManagerName = managerName;
    return true;
}

/*! Returns a URI that completely describes a manager implementation, datastore, and the parameters with which to instantiate the manager, from the given \a managerName, \a params and an optional \a implementationVersion
*/
QString QOrganizerManager::buildUri(const QString& managerName, const QMap<QString, QString>& params, int implementationVersion)
{
    QString ret(QLatin1String("qtorganizer:%1:%2"));
    // we have to escape each param
    QStringList escapedParams;
    QStringList keys = params.keys();
    for (int i=0; i < keys.size(); i++) {
        QString key = keys.at(i);
        QString arg = params.value(key);
        arg = arg.replace(QLatin1Char('&'), QLatin1String("&amp;"));
        arg = arg.replace(QLatin1Char('='), QLatin1String("&equ;"));
        key = key.replace(QLatin1Char('&'), QLatin1String("&amp;"));
        key = key.replace(QLatin1Char('='), QLatin1String("&equ;"));
        key = key + QLatin1Char('=') + arg;
        escapedParams.append(key);
    }

    if (implementationVersion != -1) {
        QString versionString = QString(QLatin1String(QTORGANIZER_IMPLEMENTATION_VERSION_NAME));
        versionString += QString::fromAscii("=");
        versionString += QString::number(implementationVersion);
        escapedParams.append(versionString);
    }

    return ret.arg(managerName, escapedParams.join(QLatin1String("&")));
}

/*!
  Constructs a QOrganizerManager whose implementation, store and parameters are specified in the given \a storeUri,
  and whose parent object is \a parent.
 */
QOrganizerManager* QOrganizerManager::fromUri(const QString& storeUri, QObject* parent)
{
    if (storeUri.isEmpty()) {
        return new QOrganizerManager(QString(), QMap<QString, QString>(), parent);
    } else {
        QString id;
        QMap<QString, QString> parameters;
        if (parseUri(storeUri, &id, &parameters)) {
            return new QOrganizerManager(id, parameters, parent);
        } else {
            // invalid
            return new QOrganizerManager(QLatin1String("invalid"), QMap<QString, QString>(), parent);
        }
    }
}

/*!
  Constructs a QOrganizerManager whose parent QObject is \a parent.
  The default implementation for the platform will be created.
 */
QOrganizerManager::QOrganizerManager(QObject* parent)
    : QObject(parent),
    d(new QOrganizerManagerData)
{
    createEngine(QString(), QMap<QString, QString>());
}

/*!
  Constructs a QOrganizerManager whose implementation is identified by \a managerName with the given \a parameters.

  The \a parent QObject will be used as the parent of this QOrganizerManager.

  If an empty \a managerName is specified, the default implementation for the platform will
  be used.
 */
QOrganizerManager::QOrganizerManager(const QString& managerName, const QMap<QString, QString>& parameters, QObject* parent)
    : QObject(parent),
    d(new QOrganizerManagerData)
{
    createEngine(managerName, parameters);
}

void QOrganizerManager::createEngine(const QString& managerName, const QMap<QString, QString>& parameters)
{
    d->createEngine(managerName, parameters);
    connect(d->m_engine, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
    connect(d->m_engine, SIGNAL(itemsAdded(QList<QOrganizerItemId>)), this, SIGNAL(itemsAdded(QList<QOrganizerItemId>)));
    connect(d->m_engine, SIGNAL(itemsChanged(QList<QOrganizerItemId>)), this, SIGNAL(itemsChanged(QList<QOrganizerItemId>)));
    connect(d->m_engine, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)), this, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)));
    connect(d->m_engine, SIGNAL(collectionsAdded(QList<QOrganizerCollectionId>)), this, SIGNAL(collectionsAdded(QList<QOrganizerCollectionId>)));
    connect(d->m_engine, SIGNAL(collectionsChanged(QList<QOrganizerCollectionId>)), this, SIGNAL(collectionsChanged(QList<QOrganizerCollectionId>)));
    connect(d->m_engine, SIGNAL(collectionsRemoved(QList<QOrganizerCollectionId>)), this, SIGNAL(collectionsRemoved(QList<QOrganizerCollectionId>)));

    connect(d->m_engine, SIGNAL(itemsChanged(QList<QOrganizerItemId>)),
            this, SLOT(_q_itemsUpdated(QList<QOrganizerItemId>)));
    connect(d->m_engine, SIGNAL(itemsRemoved(QList<QOrganizerItemId>)),
            this, SLOT(_q_itemsDeleted(QList<QOrganizerItemId>)));

}

/*!
  Constructs a QOrganizerManager whose backend has the name \a managerName and version \a implementationVersion, where the manager
  is constructed with the provided \a parameters.

  The \a parent QObject will be used as the parent of this QOrganizerManager.

  If an empty \a managerName is specified, the default implementation for the platform will be instantiated.
  If the specified implementation version is not available, the manager with the name \a managerName with the default implementation version is instantiated.
 */
QOrganizerManager::QOrganizerManager(const QString& managerName, int implementationVersion, const QMap<QString, QString>& parameters, QObject* parent)
    : QObject(parent),
    d(new QOrganizerManagerData)
{
    QMap<QString, QString> params = parameters;
    params[QString(QLatin1String(QTORGANIZER_IMPLEMENTATION_VERSION_NAME))] = QString::number(implementationVersion);
    createEngine(managerName, params);
}

/*! Frees the memory used by the QOrganizerManager */
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
  \value VersionMismatchError The most recent operation failed because the backend of the manager is not of the required version
  \value LimitReachedError The most recent operation failed because the limit for that type of object has been reached
  \value InvalidItemTypeError The most recent operation failed because the item given was of an invalid type for the operation
  \value InvalidCollectionError The most recent operation failed because the collection is invalid
  \value InvalidOccurrenceError The most recent operation failed because it was an attempt to save an occurrence without a correct InstanceOrigin detail
  \value TimeoutError The most recent operation failed because it took longer than expected.  It may be possible to try again.
 */

/*! Return the error code of the most recent operation
*/
QOrganizerManager::Error QOrganizerManager::error() const
{
    return d->m_lastError;
}

/*!
  Returns per-input error codes for the most recent operation.
  This function only returns meaningful information if the most
  recent operation was a batch operation.
  Each key in the map is the index of the element in the input list
  for which the error (whose error code is stored in the value for
  that key in the map) occurred during the batch operation.
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
    all items which match the \a filter criteria.

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
QList<QOrganizerItemId> QOrganizerManager::itemIds(const QOrganizerItemFilter &filter,
                                                   const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                   const QList<QOrganizerItemSortOrder> &sortOrders)
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->itemIds(filter, startDateTime, endDateTime, sortOrders, &h.error);
}

/*!
    \obsolete
 */
QList<QOrganizerItemId> QOrganizerManager::itemIds(const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders)
{
    return itemIds(filter, QDateTime(), QDateTime(), sortOrders);
}

/*!
    \obsolete
 */
QList<QOrganizerItem> QOrganizerManager::items(const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint)
{
    return items(filter, QDateTime(), QDateTime(), -1, sortOrders, fetchHint);
}

/*!
    \obsolete
 */
QList<QOrganizerItem> QOrganizerManager::items(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint)
{
    return items(filter, startDate, endDate, -1, sortOrders, fetchHint);
}

/*!
    \obsolete
 */
QList<QOrganizerItem> QOrganizerManager::items(const QDateTime& startDate, const QDateTime& endDate, int maxCount, const QOrganizerItemFilter& filter, const QOrganizerItemFetchHint& fetchHint)
{
    return items(filter, startDate, endDate, maxCount, QList<QOrganizerItemSortOrder>(), fetchHint);
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

    Note that backends will decide how many occurrences are returned if \a maxCount is negative.

    The \a fetchHint allows clients to specify which pieces of information they are interested or
    not interested in, to allow backends to optimise data retrieval if possible. Note that it is
    simply a hint; backends can ignore the \a fetchHint and return the full item. If a client makes
    changes to an organizer item which has been retrieved with a fetch hint, they should save it
    back using a partial save, masked by the same set of detail types in order to avoid information
    loss.
 */
QList<QOrganizerItem> QOrganizerManager::items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                               const QDateTime &endDateTime, int maxCount,
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

    \sa saveItems
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
    Removes all the item whose ID is \a itemIds, and all its occurrences. Returns true if the item
    and occurrences are successfully removed, or false otherwise.

    \sa removeItems
 */
bool QOrganizerManager::removeItem(const QOrganizerItemId& itemId)
{
    return removeItems(QList<QOrganizerItemId>() << itemId);
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

    \sa saveItem
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

    \sa removeItem
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
    Returns the default collection managed by this manager. There is always only one default collection
    for each backend.
 */
QOrganizerCollection QOrganizerManager::defaultCollection()
{
    QOrganizerManagerSyncOpErrorHolder h(this);
    return d->m_engine->defaultCollection(&h.error);
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
  from the manager if the given \a collectionId exists.
  Returns true on success, false on failure.

  Attempting to remove the default collection will fail and calling \l error() will return
  QOrganizerManager::PermissionsError.
 */
bool QOrganizerManager::removeCollection(const QOrganizerCollectionId& collectionId)
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
  Returns the engine backend implementation version number
 */
int QOrganizerManager::managerVersion() const
{
    return d->m_engine->managerVersion();
}

/*! Returns the manager name for this QOrganizerManager
*/
QString QOrganizerManager::managerName() const
{
    return d->m_engine->managerName();
}

/*! Return the parameters relevant to the creation of this QOrganizerManager
*/
QMap<QString, QString> QOrganizerManager::managerParameters() const
{
    QMap<QString, QString> params = d->m_engine->managerParameters();

    params.remove(QString::fromAscii(QTORGANIZER_VERSION_NAME));
    params.remove(QString::fromAscii(QTORGANIZER_IMPLEMENTATION_VERSION_NAME));
    return params;
}

/*!
  Return the uri describing this QOrganizerManager, consisting of the manager name and any parameters.
 */
QString QOrganizerManager::managerUri() const
{
    return d->m_engine->managerUri();
}

/*! Return a list of QOrganizerItemId extracted from the \a items
*/
QList<QOrganizerItemId> QOrganizerManager::extractIds(const QList<QOrganizerItem>& items)
{
    QList<QOrganizerItemId> ids;
#if QT_VERSION > 0x040700
    ids.reserve(items.count());
#endif

    foreach(const QOrganizerItem& item, items)
        ids.append(item.id());
    return ids;
}

#include "moc_qorganizermanager.cpp"

QTORGANIZER_END_NAMESPACE
