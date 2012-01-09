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

#include <qorganizermanagerengine.h>
#include <qorganizerjournal.h>
#include <qorganizeritemdetails.h>
#include <qorganizeritemfilters.h>
#include <private/qorganizeritemrequests_p.h>

#include <QtCore/qmutex.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerManagerEngine
    \brief The QOrganizerManagerEngine class provides the interface to implement functionalities
           of organizer managers.
    \inmodule QtOrganizer
    \ingroup organizer-backends

    This class should only be used by backend developers. Instances of this class are provided to
    QOrganizerManager by a QOrganizerManagerEngineFactory instance, which is loaded from a plugin.

    The default implementation of this interface provides a backend doing nothing, so that backend
    developers only need to reimplement the functionalities needed.

    More information on writing a organizer engine plugin is available in the \l{Qt Organizer Manager Engines
    documentation.

    \sa QOrganizerManager, QOrganizerManagerEngineFactory
 */

/*!
    \fn QOrganizerManagerEngine::dataChanged()

    This signal should be emitted if the internal state of the plugin changes, and it is unable to
    determine the changes which occurred, or if it considers the changes to be radical enough to
    require clients to reload all data.

    If this signal is emitted, no other signals will be emitted for the associated changes.

    \sa itemsAdded(), itemsChanged(), itemsRemoved()
 */

/*!
    \fn QOrganizerManagerEngine::itemsAdded(const QList<QOrganizerItemId> &itemIds);

    This signal should be emitted at some point once the items identified by \a itemIds have been
    added to the backend.

    This signal should not be emitted if the dataChanged() signal was previously emitted for these
    changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManagerEngine::itemsChanged(const QList<QOrganizerItemId> &itemIds);

    This signal should be emitted at some point once the items identified by \a itemIds have been
    modified in the backend.

    This signal should not be emitted if the dataChanged() signal was previously emitted for these
    changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManagerEngine::itemsRemoved(const QList<QOrganizerItemId> &itemIds);

    This signal should be emitted at some point once the items identified by \a itemIds have been
    removed from the backend.

    This signal should not be emitted if the dataChanged() signal was previously emitted for these
    changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManagerEngine::collectionsAdded(const QList<QOrganizerCollectionId> &collectionIds)

    This signal should be emitted at some point once the collections identified by \a collectionIds
    have been added to the backend.

    This signal should not be emitted if the dataChanged() signal was previously emitted for these
    changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManagerEngine::collectionsChanged(const QList<QOrganizerCollectionId> &collectionIds)

    This signal should be emitted at some point once the collections identified by \a collectionIds
    have been changed in the backend.

    This signal should not be emitted if items in the collections have been added, modified, or
    removed.

    This signal should not be emitted if the dataChanged() signal was previously emitted for these
    changes.

    \sa dataChanged()
 */

/*!
    \fn QOrganizerManagerEngine::collectionsRemoved(const QList<QOrganizerCollectionId> &collectionIds)

    This signal should be emitted at some point once the collections identified by \a collectionIds
    have been removed from the backend.

    This signal should not be emitted if the dataChanged() signal was previously emitted for these
    changes.

    \sa dataChanged()
 */

/*!
    Constructs an empty QOrganizerManagerEngine with the given \a parent.
 */
QOrganizerManagerEngine::QOrganizerManagerEngine(QObject *parent)
    : QObject(parent)
{
}

/*!
    This function should be reimplemented to return the name of this backend. The default implementation
    returns the name "invalid".
*/
QString QOrganizerManagerEngine::managerName() const
{
    return QString(QLatin1String("invalid"));
}

/*!
    This function should be reimplemented to return the parameters used in when constructing this
    backend. The default implementation returns an empty QMap.

    If certain paramters are invalid, or discarded by the backend, they should not be returned.
 */
QMap<QString, QString> QOrganizerManagerEngine::managerParameters() const
{
    return QMap<QString, QString>();
}

/*!
    This function should be reimplemented to return the version of this backend. The default implementation
    returns 0.
 */
int QOrganizerManagerEngine::managerVersion() const
{
    return 0;
}

/*!
    Returns the unique URI of this manager, which is built from the manager name and the parameters
    used to construct it.
 */
QString QOrganizerManagerEngine::managerUri() const
{
    return QOrganizerManager::buildUri(managerName(), managerParameters());
}

/*!
    This function should be reimplemented to support synchronous calls to fetch occurrences of the
    given parent item.

    This function is supposed to return a list of a maximum of \a maxCount organizer item instances
    which are occurrences of the given \a parentItem recurring item, which occur between the given
    \a startDateTime and the given \a endDateTime date, inclusive. Any error which occurs should be
    saved in \a error.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    It's up to the backend to decide how many occurrences are returned if the given \a maxCount is
    negative.

    It's up to the backend to decide if fetch hint is supported.
  */
QList<QOrganizerItem> QOrganizerManagerEngine::itemOccurrences(const QOrganizerItem &parentItem,
                                                               const QDateTime &startDateTime,
                                                               const QDateTime &endDateTime, int maxCount,
                                                               const QOrganizerItemFetchHint &fetchHint,
                                                               QOrganizerManager::Error *error)
{
    Q_UNUSED(parentItem);
    Q_UNUSED(startDateTime);
    Q_UNUSED(endDateTime);
    Q_UNUSED(maxCount);
    Q_UNUSED(fetchHint);

    *error = QOrganizerManager::NotSupportedError;
    return QList<QOrganizerItem>();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch organizer item IDs.

    This function is supposed to return a list of item IDs of persisted organizer items that match
    the given \a filter, sorted according to the given list of \a sortOrders, for any item which
    occurs (or has an occurrence which occurs) in the range specified by the given \a startDateTime
    and \a endDateTime, inclusive. Any error which occurs should be saved in \a error.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    It's up to the backend to decide how filters are supported.
 */
QList<QOrganizerItemId> QOrganizerManagerEngine::itemIds(const QOrganizerItemFilter &filter,
                                                         const QDateTime &startDateTime,
                                                         const QDateTime &endDateTime,
                                                         const QList<QOrganizerItemSortOrder> &sortOrders,
                                                         QOrganizerManager::Error *error)
{
    Q_UNUSED(startDateTime)
    Q_UNUSED(endDateTime)
    Q_UNUSED(filter)
    Q_UNUSED(sortOrders)

    *error = QOrganizerManager::NotSupportedError;
    return QList<QOrganizerItemId>();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch organizer items.

    This function is supposed to return a list of a maximum of \a maxCount organizer items and
    occurrences that match the given \a filter, which occur in the range specified by the given
    \a startDateTime and \a endDateTime, inclusive, and sorted according to the given list of
    \a sortOrders. Any operation error which occurs should be saved in \a error.

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    If no sort order is provided, the list is returned sorted by date.

    It's up to the backend to decide how many items should be returned if \a maxCount is negative.

    It's up to the backend to decide if filter and fetch hint are supported.
 */
QList<QOrganizerItem> QOrganizerManagerEngine::items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                                     const QDateTime &endDateTime, int maxCount,
                                                     const QList<QOrganizerItemSortOrder> &sortOrders,
                                                     const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
{
    Q_UNUSED(filter)
    Q_UNUSED(startDateTime)
    Q_UNUSED(endDateTime)
    Q_UNUSED(maxCount)
    Q_UNUSED(sortOrders)
    Q_UNUSED(fetchHint)

    *error = QOrganizerManager::NotSupportedError;
    return QList<QOrganizerItem>();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch organizer items for
    export.

    This function is supposed to return a list of organizer items that match the given \a filter,
    sorted according to the given list of \a sortOrders, for any item which occurs (or has an
    occurrence which occurs) in the range specified by the given \a startDateTime and \a endDateTime,
    inclusive. Any operation error which occurs should be saved in \a error.

    Note that event occurrences and TODO occurrences should only be returned when they represent an
    exceptional occurrence (i.e. the client has specifically saved the occurrence in the backend).

    A default-constructed (invalid) \a startDateTime specifies an open start date time (matches anything
    which occurs up until the \a endDateTime), and a default-constructed (invalid) \a endDateTime
    specifies an open end date time (matches anything which occurs after the \a startDateTime). If
    both the \a startDateTime and \a endDateTime are invalid, this function will return the IDs of
    all items which match the \a filter criteria.

    It's up to the backend to decide if filter and fetch hint are supported.
 */
QList<QOrganizerItem> QOrganizerManagerEngine::itemsForExport(const QDateTime &startDateTime,
                                                              const QDateTime &endDateTime,
                                                              const QOrganizerItemFilter &filter,
                                                              const QList<QOrganizerItemSortOrder> &sortOrders,
                                                              const QOrganizerItemFetchHint &fetchHint,
                                                              QOrganizerManager::Error *error)
{
    Q_UNUSED(startDateTime)
    Q_UNUSED(endDateTime)
    Q_UNUSED(filter)
    Q_UNUSED(sortOrders)
    Q_UNUSED(fetchHint)

    *error = QOrganizerManager::NotSupportedError;
    return QList<QOrganizerItem>();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch organizer items by
    their IDs.

    The items fetched by the backend should have a one-to-one correspondence to the IDs passed into
    this class.  That is, the nth item in the returned list should have an ID which is equal to the
    nth ID in the list of IDs.  Any invalid ID should correspond to an empty QOrganizerItem.

    It's up to the backend to decide if fetch hint is supported.

    Any operation error which occurs should be saved in \a error. And the per-input errors should be
    stored in \a errorMap.

 */
QList<QOrganizerItem> QOrganizerManagerEngine::items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                                     QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    Q_UNUSED(itemIds)
    Q_UNUSED(fetchHint)
    Q_UNUSED(errorMap)

    *error = QOrganizerManager::NotSupportedError;
    return QList<QOrganizerItem>();
}

/*!
    This function should be reimplemented to return the list of filters supported by this backend.
    The default implementation returns an empty list.
 */
QList<QOrganizerItemFilter::FilterType> QOrganizerManagerEngine::supportedFilters() const
{
    return QList<QOrganizerItemFilter::FilterType>();
}

/*!
    This function should be reimplemented to return the list of details supported by this backend
    for the given \a itemType. The default implementation returns an empty list.
 */
QList<QOrganizerItemDetail::DetailType> QOrganizerManagerEngine::supportedItemDetails(QOrganizerItemType::ItemType itemType) const
{
    Q_UNUSED(itemType)
    return QList<QOrganizerItemDetail::DetailType>();
}

/*!
    This function should be reimplemented to return the list of details supported by this backend
    for the given \a itemType. The default implementation returns an empty list.
 */
QList<QOrganizerItemType::ItemType> QOrganizerManagerEngine::supportedItemTypes() const
{
    return QList<QOrganizerItemType::ItemType>();
}

/*!
    This function should be reimplemented to support synchronous calls to save organizer items.

    This function is supposed to save the given list of \a items to the backend, and returns true on
    success or false otherwise.

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

    Any error which occurs should be saved in \a error, and per-input errors for the operation should
    be stored in \a errorMap.
 */
bool QOrganizerManagerEngine::saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                                        QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    Q_UNUSED(items)
    Q_UNUSED(detailMask)
    Q_UNUSED(errorMap)

    *error = QOrganizerManager::NotSupportedError;
    return false;
}

/*!
    This function should be reimplemented to support synchronous calls to remove organizer items.

    This function is supposed to remove all the items whose ID is contained in the given list of
    \a itemIds, and all the occurrences whose parent ID is containd in the \a itemIds. Returns true
    if all the items and occurrences are successfully removed, or false otherwise.

    Any error which occurs should be saved in \a error, and per-input errors for the operation should
    be stored in \a errorMap.
 */
bool QOrganizerManagerEngine::removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                                          QOrganizerManager::Error *error)
{
    Q_UNUSED(itemIds)
    Q_UNUSED(errorMap)

    *error = QOrganizerManager::NotSupportedError;
    return false;
}

/*!
    This function should be reimplemented to support synchronous calls to fetch the default collection.
    Any errors encountered during this operation should be stored to \a error.
*/
QOrganizerCollection QOrganizerManagerEngine::defaultCollection(QOrganizerManager::Error* error)
{
    *error = QOrganizerManager::NotSupportedError;
    return QOrganizerCollection();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch a collection based
    on its ID. Any errors encountered during this operation should be stored to \a error.
*/
QOrganizerCollection QOrganizerManagerEngine::collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    Q_UNUSED(collectionId);
    *error = QOrganizerManager::NotSupportedError;
    return QOrganizerCollection();
}

/*!
    This function should be reimplemented to support synchronous calls to fetch all the collections
    managed by this backend. Any errors encountered during this operation should be stored to \a error.
 */
QList<QOrganizerCollection> QOrganizerManagerEngine::collections(QOrganizerManager::Error* error)
{
    *error = QOrganizerManager::NotSupportedError;
    return QList<QOrganizerCollection>();
}

/*!
    This function should be reimplemented to support synchronous calls to save a collection.

    This function is supposed to save the given \a collection to the backend, and returns true on
    success or false otherwise. Any errors encountered during this operation should be stored to
    \a error.

    A new collection will be created in the backend store if the collection ID of it is null.
    Otherwise, an existing collection with the same ID will be updated. If the given collection ID
    does not exist in the backend, it will result a QOrganizerManager::DoesNotExistError error.

    Note that upon successful saving, the backend may update the collection, e.g. collection ID for
    newly saved collections.
*/
bool QOrganizerManagerEngine::saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error)
{
    Q_UNUSED(collection);

    *error = QOrganizerManager::NotSupportedError;
    return false;
}

/*!
    This function should be reimplemented to support synchronous calls to remove a collection.

    This function is supposed to remove the collection identified by the given \a collectionId, and
    all items in the collection. Returns true on success, or false otherwise. Any errors encountered
    during this operation should be stored to \a error.

    Note that removing the default collection should not be allowed and should result a
    QOrganizerManager::PermissionsError error.
*/
bool QOrganizerManagerEngine::removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    Q_UNUSED(collectionId);

    *error = QOrganizerManager::NotSupportedError;
    return false;
}

/*!
  Given an input \a filter, returns the canonical version of the filter.

  Some of the following transformations may be applied:
  \list
   \o Any QOrganizerItemInvalidFilters contained in a union filter will be removed
   \o Any default QOrganizerItemFilters contained in an intersection filter will be removed
   \o Any QOrganizerItemIntersectionFilters with a QOrganizerItemInvalidFilter contained will be
     replaced with a QOrganizerItemInvalidFilter
   \o Any QOrganizerItemUnionFilters with a default QOrganizerItemFilter contained will be replaced
     with a default QOrganizerItemFilter
   \o An empty QOrganizerItemIntersectionFilter will be replaced with a QOrganizerItemDefaultFilter
   \o An empty QOrganizerItemUnionFilter will be replaced with a QOrganizerItemInvalidFilter
   \o An empty QOrganizerItemIdFilter will be replaced with a QOrganizerItemInvalidFilter
   \o An intersection or union filter with a single entry will be replaced by that entry
   \o A QOrganizerItemDetailFilter or QOrganizerItemDetailRangeFilter with no definition name will be replaced with a QOrganizerItemInvalidFilter
   \o A QOrganizerItemDetailRangeFilter with no range specified will be converted to a QOrganizerItemDetailFilter
  \endlist
*/
QOrganizerItemFilter QOrganizerManagerEngine::canonicalizedFilter(const QOrganizerItemFilter &filter)
{
    switch (filter.type()) {
        case QOrganizerItemFilter::IntersectionFilter:
        {
            QOrganizerItemIntersectionFilter f(filter);
            QList<QOrganizerItemFilter> filters = f.filters();
            QList<QOrganizerItemFilter>::iterator it = filters.begin();

            // XXX in theory we can remove duplicates in a set filter
            while (it != filters.end()) {
                QOrganizerItemFilter canon = canonicalizedFilter(*it);
                if (canon.type() == QOrganizerItemFilter::DefaultFilter) {
                    it = filters.erase(it);
                } else if (canon.type() == QOrganizerItemFilter::InvalidFilter) {
                    return QOrganizerItemInvalidFilter();
                } else {
                    *it = canon;
                    ++it;
                }
            }

            if (filters.count() == 0)
                return QOrganizerItemFilter();
            if (filters.count() == 1)
                return filters.first();

            f.setFilters(filters);
            return f;
        }
        // unreachable

        case QOrganizerItemFilter::UnionFilter:
        {
            QOrganizerItemUnionFilter f(filter);
            QList<QOrganizerItemFilter> filters = f.filters();
            QList<QOrganizerItemFilter>::iterator it = filters.begin();

            // XXX in theory we can remove duplicates in a set filter
            while (it != filters.end()) {
                QOrganizerItemFilter canon = canonicalizedFilter(*it);
                if (canon.type() == QOrganizerItemFilter::InvalidFilter) {
                    it = filters.erase(it);
                } else if (canon.type() == QOrganizerItemFilter::DefaultFilter) {
                    return QOrganizerItemFilter();
                } else {
                    *it = canon;
                    ++it;
                }
            }

            if (filters.count() == 0)
                return QOrganizerItemInvalidFilter();
            if (filters.count() == 1)
                return filters.first();

            f.setFilters(filters);
            return f;
        }
        // unreachable

        case QOrganizerItemFilter::IdFilter:
        {
            QOrganizerItemIdFilter f(filter);
            if (f.ids().count() == 0)
                return QOrganizerItemInvalidFilter();
        }
        break; // fall through to return at end

        case QOrganizerItemFilter::DetailRangeFilter:
        {
            QOrganizerItemDetailRangeFilter f(filter);
            if (f.detailType() == QOrganizerItemDetail::TypeUndefined)
                return QOrganizerItemInvalidFilter();
            if (f.minValue() == f.maxValue()
                && f.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeLower | QOrganizerItemDetailRangeFilter::ExcludeUpper))
                return QOrganizerItemInvalidFilter();
            if ((f.minValue().isNull() && f.maxValue().isNull()) || (f.minValue() == f.maxValue())) {
                QOrganizerItemDetailFilter df;
                df.setDetail(f.detailType(), f.detailField());
                df.setMatchFlags(f.matchFlags());
                df.setValue(f.minValue());
                return df;
            }
        }
        break; // fall through to return at end

        case QOrganizerItemFilter::DetailFilter:
        {
            QOrganizerItemDetailFilter f(filter);
            if (f.detailType() == QOrganizerItemDetail::TypeUndefined)
                return QOrganizerItemInvalidFilter();
        }
        break; // fall through to return at end

        default:
            break; // fall through to return at end
    }
    return filter;
}

/*!
  Compares \a first against \a second.  If the types are
  strings (QVariant::String), the \a sensitivity argument controls
  case sensitivity when comparing.

  Returns:
  <0 if \a first is less than \a second
   0 if \a first is equal to \a second
  >0 if \a first is greater than \a second.

  The results are undefined if the variants are different types, or
  cannot be compared.
 */
int QOrganizerManagerEngine::compareVariant(const QVariant& first, const QVariant& second, Qt::CaseSensitivity sensitivity)
{
    switch(first.type()) {
        case QVariant::Int:
            return first.toInt() - second.toInt();

        case QVariant::LongLong:
            return first.toLongLong() - second.toLongLong();

        case QVariant::Bool:
        case QVariant::Char:
        case QVariant::UInt:
            return first.toUInt() - second.toUInt();

        case QVariant::ULongLong:
            return first.toULongLong() - second.toULongLong();

       case QVariant::String:
            return first.toString().compare(second.toString(), sensitivity);

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
            return first.toDate().toJulianDay() - second.toDate().toJulianDay();

        case QVariant::Time:
            {
                const QTime a = first.toTime();
                const QTime b = second.toTime();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        default:
            return 0;
    }
}

/*!
  Returns true if the supplied item \a item matches the supplied filter \a filter.

  This function will test each condition in the filter, possibly recursing.
 */
bool QOrganizerManagerEngine::testFilter(const QOrganizerItemFilter &filter, const QOrganizerItem &item)
{
    switch(filter.type()) {
        case QOrganizerItemFilter::InvalidFilter:
            return false;

        case QOrganizerItemFilter::DefaultFilter:
            return true;

        case QOrganizerItemFilter::IdFilter:
            {
                const QOrganizerItemIdFilter idf(filter);
                if (idf.ids().contains(item.id()))
                    return true;
            }
            // Fall through to end
            break;

        case QOrganizerItemFilter::DetailFilter:
            {
                const QOrganizerItemDetailFilter cdf(filter);
                if (cdf.detailType() == QOrganizerItemDetail::TypeUndefined)
                    return false;

                /* See if this organizer item has one of these details in it */
                const QList<QOrganizerItemDetail>& details = item.details(cdf.detailType());

                if (details.count() == 0)
                    return false; /* can't match */

                /* See if we need to check the values */
                if (cdf.detailField() == -1)
                    return true;  /* just testing for the presence of a detail of the specified definition */

                /* Now figure out what tests we are doing */
                const bool valueTest = cdf.value().isValid();
                const bool presenceTest = !valueTest;

                /* See if we need to test any values at all */
                if (presenceTest) {
                    for(int j=0; j < details.count(); j++) {
                        const QOrganizerItemDetail& detail = details.at(j);

                        /* Check that the field is present and has a non-empty value */
                        if (detail.values().contains(cdf.detailField()) && !detail.value(cdf.detailField()).isNull())
                            return true;
                    }
                    return false;
                }

                /* Case sensitivity, for those parts that use it */
                Qt::CaseSensitivity cs = (cdf.matchFlags() & QOrganizerItemFilter::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;

                /* See what flags are requested, since we're looking at a value */
                if (cdf.matchFlags() & (QOrganizerItemFilter::MatchEndsWith | QOrganizerItemFilter::MatchStartsWith | QOrganizerItemFilter::MatchContains | QOrganizerItemFilter::MatchFixedString)) {
                    /* We're strictly doing string comparisons here */
                    bool matchStarts = (cdf.matchFlags() & 7) == QOrganizerItemFilter::MatchStartsWith;
                    bool matchEnds = (cdf.matchFlags() & 7) == QOrganizerItemFilter::MatchEndsWith;
                    bool matchContains = (cdf.matchFlags() & 7) == QOrganizerItemFilter::MatchContains;

                    /* Value equality test */
                    for(int j=0; j < details.count(); j++) {
                        const QOrganizerItemDetail& detail = details.at(j);
                        const QString& var = detail.value(cdf.detailField()).toString();
                        const QString& needle = cdf.value().toString();
                        if (matchStarts && var.startsWith(needle, cs))
                            return true;
                        if (matchEnds && var.endsWith(needle, cs))
                            return true;
                        if (matchContains && var.contains(needle, cs))
                            return true;
                        if (QString::compare(var, needle, cs) == 0)
                            return true;
                    }
                    return false;
                } else {
                    /* Nope, testing the values as a variant */
                    /* Value equality test */
                    for(int j = 0; j < details.count(); j++) {
                        const QOrganizerItemDetail& detail = details.at(j);
                        const QVariant& var = detail.value(cdf.detailField());
                        if (!var.isNull() && compareVariant(var, cdf.value(), cs) == 0)
                            return true;
                    }
                }
            }
            break;

        case QOrganizerItemFilter::DetailRangeFilter:
            {
                const QOrganizerItemDetailRangeFilter cdf(filter);
                if (cdf.detailType() == QOrganizerItemDetail::TypeUndefined)
                    return false; /* we do not know which field to check */

                /* See if this organizer item has one of these details in it */
                const QList<QOrganizerItemDetail>& details = item.details(cdf.detailType());

                if (details.count() == 0)
                    return false; /* can't match */

                /* Check for a detail presence test */
                if (cdf.detailField() == -1)
                    return true;

                /* See if this is a field presence test */
                if (!cdf.minValue().isValid() && !cdf.maxValue().isValid()) {
                    for(int j=0; j < details.count(); j++) {
                        const QOrganizerItemDetail& detail = details.at(j);
                        if (detail.values().contains(cdf.detailField()))
                            return true;
                    }
                    return false;
                }

                /* open or closed interval testing support */
                const int minComp = cdf.rangeFlags() & QOrganizerItemDetailRangeFilter::ExcludeLower ? 1 : 0;
                const int maxComp = cdf.rangeFlags() & QOrganizerItemDetailRangeFilter::IncludeUpper ? 1 : 0;

                const bool testMin = cdf.minValue().isValid();
                const bool testMax = cdf.maxValue().isValid();

                /* At this point we know that at least of testMin & testMax is true */

                /* Case sensitivity, for those parts that use it */
                Qt::CaseSensitivity cs = (cdf.matchFlags() & QOrganizerItemFilter::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;

                /* See what flags are requested, since we're looking at a value */
                if (cdf.matchFlags() & (QOrganizerItemFilter::MatchEndsWith | QOrganizerItemFilter::MatchStartsWith | QOrganizerItemFilter::MatchContains | QOrganizerItemFilter::MatchFixedString)) {
                    /* We're strictly doing string comparisons here */
                    //bool matchStarts = (cdf.matchFlags() & 7) == QOrganizerItemFilter::MatchStartsWith;
                    bool matchEnds = (cdf.matchFlags() & 7) == QOrganizerItemFilter::MatchEndsWith;
                    bool matchContains = (cdf.matchFlags() & 7) == QOrganizerItemFilter::MatchContains;

                    /* Min/Max and contains do not make sense */
                    if (matchContains)
                        return false;

                    QString minVal = cdf.minValue().toString();
                    QString maxVal = cdf.maxValue().toString();

                    /* Starts with is the normal compare case, endsWith is a bit trickier */
                    for(int j=0; j < details.count(); j++) {
                        const QOrganizerItemDetail& detail = details.at(j);
                        const QString& var = detail.value(cdf.detailField()).toString();
                        if (!matchEnds) {
                            // MatchStarts or MatchFixedString
                            if (testMin && QString::compare(var, minVal, cs) < minComp)
                                continue;
                            if (testMax && QString::compare(var, maxVal, cs) >= maxComp)
                                continue;
                            return true;
                        } else {
                            /* Have to test the length of min & max */
                            // using refs means the parameter order is backwards, so negate the result of compare
                            if (testMin && -QString::compare(minVal, var.rightRef(minVal.length()), cs) < minComp)
                                continue;
                            if (testMax && -QString::compare(maxVal, var.rightRef(maxVal.length()), cs) >= maxComp)
                                continue;
                            return true;
                        }
                    }
                    // Fall through to end
                } else {
                    /* Nope, testing the values as a variant */
                    for(int j=0; j < details.count(); j++) {
                        const QOrganizerItemDetail& detail = details.at(j);
                        const QVariant& var = detail.value(cdf.detailField());

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

        case QOrganizerItemFilter::IntersectionFilter:
            {
                /* XXX In theory we could reorder the terms to put the native tests first */
                const QOrganizerItemIntersectionFilter bf(filter);
                const QList<QOrganizerItemFilter>& terms = bf.filters();
                if (terms.count() > 0) {
                    for(int j = 0; j < terms.count(); j++) {
                        if (!testFilter(terms.at(j), item)) {
                            return false;
                        }
                    }
                    return true;
                }
                // Fall through to end
            }
            break;

        case QOrganizerItemFilter::UnionFilter:
            {
                /* XXX In theory we could reorder the terms to put the native tests first */
                const QOrganizerItemUnionFilter bf(filter);
                const QList<QOrganizerItemFilter>& terms = bf.filters();
                if (terms.count() > 0) {
                    for(int j = 0; j < terms.count(); j++) {
                        if (testFilter(terms.at(j), item)) {
                            return true;
                        }
                    }
                    return false;
                }
                // Fall through to end
            }
            break;

    case QOrganizerItemFilter::CollectionFilter:
            {
                const QOrganizerItemCollectionFilter cf(filter);
                const QSet<QOrganizerCollectionId>& ids = cf.collectionIds();
                if (ids.contains(item.collectionId()))
                    return true;
                return false;
            }
    }
    return false;
}

/*!
  Returns true if the given \a item (or an occurrence of the item) occurs within the range
  specified by the \a startPeriod and the \a endPeriod, inclusive.
  A default-constructed \a startPeriod signifies that the lower bound of the range is
  infinitely small (i.e., will match anything up to the \a endPeriod) and a default-constructed
  \a endPeriod signifies that the upper bound of the range is infinitely large
  (i.e., will match anything which occurs after the \a startPeriod).
 */
bool QOrganizerManagerEngine::isItemBetweenDates(const QOrganizerItem& item, const QDateTime& startPeriod, const QDateTime& endPeriod)
{
    if (startPeriod.isNull() && endPeriod.isNull())
        return true;

    QDateTime itemDateStart;
    QDateTime itemDateEnd;

    if (item.type() == QOrganizerItemType::TypeEvent || item.type() == QOrganizerItemType::TypeEventOccurrence) {
        QOrganizerEventTime etr = item.detail<QOrganizerEventTime>();
        itemDateStart = etr.startDateTime();
        itemDateEnd = etr.endDateTime();
    } else if (item.type() == QOrganizerItemType::TypeTodo || item.type() == QOrganizerItemType::TypeTodoOccurrence) {
        QOrganizerTodoTime ttr = item.detail<QOrganizerTodoTime>();
        itemDateStart = ttr.startDateTime();
        itemDateEnd = ttr.dueDateTime();
    } else if (item.type() == QOrganizerItemType::TypeJournal) {
        QOrganizerJournal journal = item;
        itemDateStart = itemDateEnd = journal.dateTime();
    } else if (item.type() == QOrganizerItemType::TypeNote) {
        //for note, there is no such start/end datetime so we always return false
        return false;
    }

    // if period start date is not given, check that item is starting or ending before period end
    if (startPeriod.isNull()) // endPeriod must be non-null because of initial test
        return (!itemDateStart.isNull() && itemDateStart <= endPeriod) ||
               (!itemDateEnd.isNull() && itemDateEnd <= endPeriod);

    // if period end date is not given, check that item is starting or ending after the period start
    if (endPeriod.isNull())   // startPeriod must be non-null because of initial test
        return (!itemDateEnd.isNull() && itemDateEnd >= startPeriod) ||
               (!itemDateStart.isNull() && itemDateStart >= startPeriod);

    // Both startPeriod and endPeriod are not null
    // check if item start date is between the period start and end date
    if (!itemDateStart.isNull() && itemDateStart >= startPeriod && itemDateStart <= endPeriod)
        return true;

    // check if item end date is between the period start and end date
    if (!itemDateEnd.isNull() && itemDateEnd >= startPeriod && itemDateEnd <= endPeriod)
        return true;

    // check if item interval is including the period interval
    if (!itemDateStart.isNull() && !itemDateEnd.isNull() && itemDateStart <= startPeriod && itemDateEnd >= endPeriod)
        return true;

    return false;
}

/*!
    \internal

    Returns the date associated with \a item that can be used for the purpose of date-sorting the item.
 */
QDateTime getDateForSorting(const QOrganizerItem& item)
{
    QDateTime retn;
    {
        QOrganizerEventTime detail = item.detail<QOrganizerEventTime>();
        if (!detail.isEmpty()) {
            retn = detail.startDateTime();
            if (!retn.isValid())
                retn = detail.endDateTime();
            if (retn.isValid() && detail.isAllDay()) {
                // set it to a millisecond before the given date to have it sorted correctly
                retn.setTime(QTime(23, 59, 59, 999));
                retn.addDays(-1);
            }
            return retn;
        }
    }
    {
        QOrganizerTodoTime detail = item.detail<QOrganizerTodoTime>();
        if (!detail.isEmpty()) {
            retn = detail.startDateTime();
            if (!retn.isValid())
                retn = detail.dueDateTime();
            if (retn.isValid() && detail.isAllDay()) {
                // set it to a millisecond before the given date to have it sorted correctly
                retn.setTime(QTime(23, 59, 59, 999));
                retn.addDays(-1);
            }
            return retn;
        }
    }

    // If it's a note, this will just return null, as expected
    return item.detail<QOrganizerJournalTime>().entryDateTime();
}

/*!
    Returns true if and only if \a a is temporally less than \a b.  Items with an earlier date are
    temporally less than items with a later date, or items with no date.  All day items are
    temporally less than non-all day items on the same date.  For events and todos, the
    start date is used, or if null, the end date is used.  This function defines a total ordering
    suitable for use in a sort function.
 */
bool QOrganizerManagerEngine::itemLessThan(const QOrganizerItem& a, const QOrganizerItem& b)
{
    QDateTime date1 = getDateForSorting(a);
    if (!date1.isValid()) {
        return false;
    } else {
        QDateTime date2 = getDateForSorting(b);
        if (!date2.isValid())
            return true;
        else
            return date1 < date2;
    }
}

/*!
  Compares two organizer items (\a a and \a b) using the given list of \a sortOrders.  Returns a negative number if \a a should appear
  before \a b according to the sort order, a positive number if \a a should appear after \a b according to the sort order,
  and zero if the two are unable to be sorted.
 */
int QOrganizerManagerEngine::compareItem(const QOrganizerItem& a, const QOrganizerItem& b, const QList<QOrganizerItemSortOrder>& sortOrders)
{
    QList<QOrganizerItemSortOrder> copy = sortOrders;
    while (copy.size()) {
        // retrieve the next sort order in the list
        QOrganizerItemSortOrder sortOrder = copy.takeFirst();
        if (!sortOrder.isValid())
            break;

        // obtain the values which this sort order concerns
        const QVariant& aVal = a.detail(sortOrder.detailType()).value(sortOrder.detailField());
        const QVariant& bVal = b.detail(sortOrder.detailType()).value(sortOrder.detailField());

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
            return (sortOrder.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst ? -1 : 1);
        if (bIsNull)
            return (sortOrder.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst ? 1 : -1);

        // real comparison
        int comparison = compareVariant(aVal, bVal, sortOrder.caseSensitivity()) * (sortOrder.direction() == Qt::AscendingOrder ? 1 : -1);
        if (comparison == 0)
            continue;
        return comparison;
    }

    return 0; // or according to id? return (a.id() < b.id() ? -1 : 1);
}


/*!
    Insert \a toAdd to the \a sorted list, according to the provided \a sortOrders. The index where \a toAdd is inserted
    is returned.

    The first one in the \a sortOrders list has the highest priority.
 */
int QOrganizerManagerEngine::addSorted(QList<QOrganizerItem> *sorted, const QOrganizerItem &toAdd, const QList<QOrganizerItemSortOrder> &sortOrders)
{
    if (sortOrders.count() > 0) {
        for (int i = 0; i < sorted->size(); ++i) {
            // check to see if the new item should be inserted here
            int comparison = compareItem(sorted->at(i), toAdd, sortOrders);
            if (comparison > 0) {
                sorted->insert(i, toAdd);
                return i;
            }
        }
    }

    // hasn't been inserted yet?  append to the list.
    sorted->append(toAdd);
    return sorted->size() - 1;
}

/*!
    Returns the engine ID from the given item \a id.

    The caller does not take ownership of the pointer, and should not delete returned id or undefined
    behavior may occur.
 */
const QOrganizerItemEngineId *QOrganizerManagerEngine::engineItemId(const QOrganizerItemId &id)
{
    return id.d.data();
}

/*!
    Returns the engine ID from the given collection \a id.

    The caller does not take ownership of the pointer, and should not delete returned id or undefined
    behavior may occur.
 */
const QOrganizerCollectionEngineId* QOrganizerManagerEngine::engineCollectionId(const QOrganizerCollectionId& id)
{
    return id.d.data();
}

/*!
    This function is called when the given \a request has been destroyed by the client.

    When this function is called, it means for the backend:
    \list
    \o The client doesn't care about the request any more. The engine can still complete it, but
       completion is not required.
    \o It can't reliably access any properties of the request pointer any more. The pointer will
       be invalid once this function returns.
    \endlist

    Note that since the \a request may run in another thread, this function should be blocked until
    the worker thread gets fully notified.
 */
void QOrganizerManagerEngine::requestDestroyed(QOrganizerAbstractRequest *request)
{
    Q_UNUSED(request);
}

/*!
    This function is called when the client tries to start the given asynchronous \a request. Returns
    true if the request is started successfully, or false otherwise.

    Note that the request is supposed to run in an asynchronous manner that this function should
    return as soon as possible. Therefore, it the operation would last sometime, a worker thread
    should be used to queue and process the request. In such cases, backend should be aware that
    the request may be deleted by the client, and requestDestroyed() function will be called.
 */
bool QOrganizerManagerEngine::startRequest(QOrganizerAbstractRequest* request)
{
    Q_UNUSED(request);
    return false;
}

/*!
    This function is called when the client tries to cancel the given asynchronous \a request. Returns
    true if the request is calcelled successfully, or false otherwise.
 */
bool QOrganizerManagerEngine::cancelRequest(QOrganizerAbstractRequest* request)
{
    Q_UNUSED(request);
    return false;
}

/*!
    This function is called when the client wants to be blocked until the given \a request is completed,
    or until \a msecs milliseconds have passed. Returns true when the request is completed, or false
    otherwise.
 */
bool QOrganizerManagerEngine::waitForRequestFinished(QOrganizerAbstractRequest* request, int msecs)
{
    Q_UNUSED(request);
    Q_UNUSED(msecs);
    return false;
}

/*!
  Updates the given asynchronous request \a req by setting the new \a state
  of the request.  If the new state is different, the stateChanged() signal
  will be emitted by the request.
 */
void QOrganizerManagerEngine::updateRequestState(QOrganizerAbstractRequest* req, QOrganizerAbstractRequest::State state)
{
    if (req) {
        QMutexLocker ml(&req->d_ptr->m_mutex);
        if (req->d_ptr->m_state != state) {
            req->d_ptr->m_state = state;
            ml.unlock();
            emit req->stateChanged(state);
        }
    }
}

/*!
  Updates the given QOrganizerItemOccurrenceFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemOccurrenceFetchRequest(QOrganizerItemOccurrenceFetchRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemOccurrenceFetchRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemOccurrenceFetchRequestPrivate* rd = static_cast<QOrganizerItemOccurrenceFetchRequestPrivate*>(req->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_organizeritems = result;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerItemIdFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemIdFetchRequest(QOrganizerItemIdFetchRequest* req, const QList<QOrganizerItemId>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemIdFetchRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemIdFetchRequestPrivate* rd = static_cast<QOrganizerItemIdFetchRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_ids = result;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
    Updates the given QOrganizerItemFetchByIdRequest \a req with the latest results \a result, and
    operation error \a error, and map of input index to individual error \a errorMap. In addition,
    the state of the request will be changed to \a newState.

    It then causes the request to emit its resultsAvailable() signal to notify clients of the request
    progress.

    If the new request state is different from the previous state, the stateChanged() signal will
    also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemFetchByIdRequest(QOrganizerItemFetchByIdRequest *req, const QList<QOrganizerItem> &result, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error> &errorMap, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemFetchByIdRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemFetchByIdRequestPrivate *rd = static_cast<QOrganizerItemFetchByIdRequestPrivate *>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_items = result;
        rd->m_errors = errorMap;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerItemFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemFetchRequest(QOrganizerItemFetchRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemFetchRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemFetchRequestPrivate* rd = static_cast<QOrganizerItemFetchRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_organizeritems = result;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerItemFetchForExportRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemFetchForExportRequest(QOrganizerItemFetchForExportRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemFetchForExportRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemFetchForExportRequestPrivate* rd = static_cast<QOrganizerItemFetchForExportRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_organizeritems = result;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerItemRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemRemoveRequest(QOrganizerItemRemoveRequest* req, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemRemoveRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemRemoveRequestPrivate* rd = static_cast<QOrganizerItemRemoveRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_errors = errorMap;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerItemSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateItemSaveRequest(QOrganizerItemSaveRequest* req, const QList<QOrganizerItem>& result, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerItemSaveRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerItemSaveRequestPrivate* rd = static_cast<QOrganizerItemSaveRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_organizeritems = result;
        rd->m_errors = errorMap;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerCollectionFetchRequest \a req with the latest results \a result and an operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.
  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateCollectionFetchRequest(QOrganizerCollectionFetchRequest* req, const QList<QOrganizerCollection>& result, QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerCollectionFetchRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerCollectionFetchRequestPrivate* rd = static_cast<QOrganizerCollectionFetchRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_collections = result;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerCollectionRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.
  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateCollectionRemoveRequest(QOrganizerCollectionRemoveRequest* req, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerCollectionRemoveRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerCollectionRemoveRequestPrivate* rd = static_cast<QOrganizerCollectionRemoveRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_errors = errorMap;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

/*!
  Updates the given QOrganizerCollectionSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.
  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QOrganizerManagerEngine::updateCollectionSaveRequest(QOrganizerCollectionSaveRequest* req, const QList<QOrganizerCollection>& result, QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error>& errorMap, QOrganizerAbstractRequest::State newState)
{
    if (req) {
        QWeakPointer<QOrganizerCollectionSaveRequest> ireq(req); // Take this in case the first emit deletes us
        QOrganizerCollectionSaveRequestPrivate* rd = static_cast<QOrganizerCollectionSaveRequestPrivate*>(ireq.data()->d_ptr);
        QMutexLocker ml(&rd->m_mutex);
        bool emitState = rd->m_state != newState;
        rd->m_collections = result;
        rd->m_errors = errorMap;
        rd->m_error = error;
        rd->m_state = newState;
        ml.unlock();
        emit ireq.data()->resultsAvailable();
        if (emitState && ireq)
            emit ireq.data()->stateChanged(newState);
    }
}

#include "moc_qorganizermanagerengine.cpp"

QTORGANIZER_END_NAMESPACE
