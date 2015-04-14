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

#include "qorganizeritemmemorybackend_p.h"

#include <QtOrganizer/qorganizeritemrecurrence.h>
#include <QtOrganizer/qorganizeritems.h>
#include <QtOrganizer/qorganizeritemdetails.h>
#include <QtOrganizer/qorganizeritemfilters.h>
#include <QtOrganizer/qorganizeritemrequests.h>

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qstringbuilder.h>
#include <QtCore/quuid.h>

QT_BEGIN_NAMESPACE_ORGANIZER

QOrganizerManagerEngine* QOrganizerItemMemoryFactory::engine(const QMap<QString, QString>& parameters, QOrganizerManager::Error* error)
{
    Q_UNUSED(error);

    QOrganizerItemMemoryEngine *ret = QOrganizerItemMemoryEngine::createMemoryEngine(parameters);
    return ret;
}

QString QOrganizerItemMemoryFactory::managerName() const
{
    return QString::fromLatin1("memory");
}

/*!
  \class QOrganizerItemMemoryEngine
  \brief The QOrganizerItemMemoryEngine class provides an in-memory implementation
  of an organizer item backend.
  \inmodule QtOrganizer
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

typedef QHash<QString, QOrganizerItemMemoryEngineData *> EngineDatas;
Q_GLOBAL_STATIC(EngineDatas, theEngineDatas);

/*! Constructor of a QOrganizerItemMemoryEngineData object
*/
QOrganizerItemMemoryEngineData::QOrganizerItemMemoryEngineData()
    : QSharedData(),
    m_nextOrganizerItemId(1),
    m_nextOrganizerCollectionId(2)
{

}

/*!
 * Factory function for creating a new in-memory backend, based
 * on the given \a parameters.
 *
 * The same engine will be returned for multiple calls with the
 * same value for the "id" parameter, while one of them is in scope.
 */
QOrganizerItemMemoryEngine* QOrganizerItemMemoryEngine::createMemoryEngine(const QMap<QString, QString>& parameters)
{
    QString idValue = parameters.value(QStringLiteral("id"));

    EngineDatas &engineDatas = *theEngineDatas();
    QOrganizerItemMemoryEngineData* data = engineDatas.value(idValue);
    if (!data) {
        data = new QOrganizerItemMemoryEngineData();
        // no store given?  new, anonymous store.
        if (!idValue.isEmpty()) {
            data->m_id = idValue;
            engineDatas.insert(idValue, data);
        }
    }
    data->ref.ref();
    return new QOrganizerItemMemoryEngine(data);
}

/*!
 * Constructs a new in-memory backend which shares the given \a data with
 * other shared memory engines.
 */
QOrganizerItemMemoryEngine::QOrganizerItemMemoryEngine(QOrganizerItemMemoryEngineData* data)
    : d(data)
{
    d->m_sharedEngines.append(this);

    // the default collection always exists.
    if (d->m_idToCollectionHash.isEmpty()) {
        d->m_managerUri = managerUri();
        const QOrganizerCollectionId defaultId = defaultCollectionId();
        QOrganizerCollection defaultCollection;
        defaultCollection.setId(defaultId);
        defaultCollection.setMetaData(QOrganizerCollection::KeyName, QString(QStringLiteral("Default Collection")));
        d->m_idToCollectionHash.insert(defaultId, defaultCollection);
    }
}

/*! Frees any memory used by this engine
*/
QOrganizerItemMemoryEngine::~QOrganizerItemMemoryEngine()
{
    d->m_sharedEngines.removeAll(this);
    if (!d->ref.deref()) {
        if (!d->m_id.isEmpty()) {
            EngineDatas &engineDatas = *theEngineDatas();
            engineDatas.remove(d->m_id);
        }
        delete d;
    }
}

/*! \reimp
*/
QString QOrganizerItemMemoryEngine::managerName() const
{
    return QStringLiteral("memory");
}

/*! \reimp
*/
QMap<QString, QString> QOrganizerItemMemoryEngine::managerParameters() const
{
    QMap<QString, QString> params;
    params.insert(QStringLiteral("id"), d->m_id);
    return params;
}

/*! \reimp
*/
QMap<QString, QString> QOrganizerItemMemoryEngine::idInterpretationParameters() const
{
    return managerParameters();
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                                        QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    Q_UNUSED(fetchHint)

    QList<QOrganizerItem> items;
    items.reserve(itemIds.size());
    QOrganizerItem tmp;
    for (int i = 0; i < itemIds.size(); ++i) {
        tmp = item(itemIds.at(i));
        items.append(tmp);
        if (tmp.isEmpty())
            errorMap->insert(i, QOrganizerManager::DoesNotExistError);
    }
    *error = errorMap->isEmpty() ? QOrganizerManager::NoError : QOrganizerManager::DoesNotExistError;
    return items;
}

QList<QOrganizerItemId> QOrganizerItemMemoryEngine::itemIds(const QOrganizerItemFilter &filter,
                                                            const QDateTime &startDateTime,
                                                            const QDateTime &endDateTime,
                                                            const QList<QOrganizerItemSortOrder> &sortOrders,
                                                            QOrganizerManager::Error *error)
{
    if (startDateTime.isNull() && endDateTime.isNull() && filter.type() == QOrganizerItemFilter::DefaultFilter && sortOrders.count() == 0)
        return d->m_idToItemHash.keys();
    else
        return QOrganizerManager::extractIds(itemsForExport(startDateTime, endDateTime, filter, sortOrders, QOrganizerItemFetchHint(), error));
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::internalItemOccurrences(const QOrganizerItem& parentItem, const QDateTime& periodStart, const QDateTime& periodEnd, int maxCount, bool includeExceptions, bool sortItems, QList<QDate> *exceptionDates, QOrganizerManager::Error* error) const
{
    // given the generating item, grab it's QOrganizerItemRecurrence detail (if it exists), and calculate all of the dates within the given period.
    // how would a real backend do this?
    // Also, should this also return the exception instances (ie, return any persistent instances with parent information == parent item?)
    // XXX TODO: in detail validation, ensure that the referenced parent Id exists...

    QDateTime realPeriodStart(periodStart);
    QDateTime realPeriodEnd(periodEnd);
    QDateTime initialDateTime;
    if (parentItem.type() == QOrganizerItemType::TypeEvent) {
        QOrganizerEvent evt = parentItem;
        initialDateTime = evt.startDateTime().isValid() ? evt.startDateTime() : evt.endDateTime();
    } else if (parentItem.type() == QOrganizerItemType::TypeTodo) {
        QOrganizerTodo todo = parentItem;
        initialDateTime = todo.startDateTime().isValid() ? todo.startDateTime() : todo.dueDateTime();
    } else {
        // erm... not a recurring item in our schema...
        return QList<QOrganizerItem>();
    }

    if (realPeriodStart.isValid() && initialDateTime.isValid()) {
        if (initialDateTime > realPeriodStart)
            realPeriodStart = initialDateTime;
    } else if (initialDateTime.isValid()) {
        realPeriodStart = initialDateTime;
    }

    if (!periodEnd.isValid()) {
        // If no endDateTime is given, we'll only generate items that occur within the next 4 years of realPeriodStart.
        realPeriodEnd.setDate(realPeriodStart.date().addDays(1461));
        realPeriodEnd.setTime(realPeriodStart.time());
    }
    if (realPeriodStart > realPeriodEnd) {
        *error = QOrganizerManager::BadArgumentError;
        return QList<QOrganizerItem>();
    }

    QList<QOrganizerItem> retn;
    QList<QOrganizerItem> xoccurrences;
    QOrganizerItemRecurrence recur = parentItem.detail(QOrganizerItemDetail::TypeRecurrence);

    if (includeExceptions) {
        // first, retrieve all persisted instances (exceptions) which occur between the specified datetimes.
        foreach (const QOrganizerItem& item, d->m_idToItemHash) {
            if (item.detail(QOrganizerItemDetail::TypeParent).value<QOrganizerItemId>(QOrganizerItemParent::FieldParentId) == parentItem.id()) {
                QDateTime lowerBound;
                QDateTime upperBound;
                if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
                    QOrganizerEventOccurrence instance = item;
                    lowerBound = instance.startDateTime();
                    upperBound = instance.endDateTime();
                } else {
                    QOrganizerTodoOccurrence instance = item;
                    lowerBound = instance.startDateTime();
                    upperBound = instance.dueDateTime();
                }

                if ((lowerBound.isNull() || lowerBound >= realPeriodStart) && (upperBound.isNull() || upperBound <= realPeriodEnd)) {
                    // this occurrence fulfils the criteria.
                    xoccurrences.append(item);
                }
            }
        }
    }

    // then, generate the required (unchanged) instances from the parentItem.
    // before doing that, we have to find out all of the exception dates.
    QList<QDate> xdates;
    foreach (const QDate& xdate, recur.exceptionDates()) {
        xdates += xdate;
    }
    if (realPeriodStart.isValid()) {
        // Dates are interpreted as local time, but realPeriodStart is UTC
        const QDate localStartDate(realPeriodStart.toLocalTime().date());
        QSet<QOrganizerRecurrenceRule> xrules = recur.exceptionRules();
        foreach (const QOrganizerRecurrenceRule& xrule, xrules) {
            if (xrule.frequency() != QOrganizerRecurrenceRule::Invalid
                    && ((xrule.limitType() != QOrganizerRecurrenceRule::DateLimit) || (xrule.limitDate() >= localStartDate))) {
                // we cannot skip it, since it applies in the given time period.
                QList<QDateTime> xdatetimes = generateDateTimes(initialDateTime, xrule, realPeriodStart, realPeriodEnd, 50); // max count of 50 is arbitrary...
                foreach (const QDateTime& xdatetime, xdatetimes)
                    xdates += xdatetime.toLocalTime().date();
            }
        }
    }
    // now generate a list of rdates (from the recurrenceDates and recurrenceRules)

    // QMap is used for storing dates, because we don't want to have duplicate dates and
    // we want to have dates sorted
    // Only key of the map is relevant (QDateTime), the value (int) is not used
    QMap<QDateTime, int> rdateMap;
    foreach (const QDate& rdate, recur.recurrenceDates()) {
        QDateTime dt(initialDateTime.toLocalTime());
        dt.setDate(rdate);
        rdateMap.insert(dt.toUTC(), 0);
    }

    if (realPeriodStart.isValid()) {
        const QDate localStartDate(realPeriodStart.toLocalTime().date());
        QSet<QOrganizerRecurrenceRule> rrules = recur.recurrenceRules();
        foreach (const QOrganizerRecurrenceRule& rrule, rrules) {
            if (rrule.frequency() != QOrganizerRecurrenceRule::Invalid
                    && ((rrule.limitType() != QOrganizerRecurrenceRule::DateLimit) || (rrule.limitDate() >= localStartDate))) {
                // we cannot skip it, since it applies in the given time period.
                QList<QDateTime> rdatetimes = generateDateTimes(initialDateTime, rrule, realPeriodStart, realPeriodEnd, 50); // max count of 50 is arbitrary...
                foreach (const QDateTime& rdatetime, rdatetimes)
                    rdateMap.insert(rdatetime, 0);
            }
        }
    }
    // now order the contents of retn by date
    QList<QDateTime> rdates = rdateMap.keys();

    if (initialDateTime.isValid() && !recur.recurrenceDates().isEmpty() && qBinaryFind(rdates, initialDateTime) == rdates.constEnd()) {
        rdates.prepend(initialDateTime);
    }

    // now for each rdate which isn't also an xdate
    foreach (const QDateTime& rdate, rdates) {
        if (rdate >= realPeriodStart && rdate <= realPeriodEnd) {
            const QDate localRDate(rdate.toLocalTime().date());
            if (!xdates.contains(localRDate)) {
                // generate the required instance and add it to the return list.
                retn.append(QOrganizerManagerEngine::generateOccurrence(parentItem, rdate));
            } else if (includeExceptions) {
                for (int i = 0; i < xoccurrences.size(); i++) {
                    QOrganizerItemParent parentDetail = xoccurrences[i].detail(QOrganizerItemDetail::TypeParent);
                    if (parentDetail.originalDate() == localRDate)
                        retn.append(xoccurrences[i]);
                }
            } else if (exceptionDates) {
                exceptionDates->append(localRDate);
            }
        }
    }

    if (sortItems) {
        // should we always sort if a maxCount is given?
        QMultiMap<QDateTime, QOrganizerItem> defaultSorted;
        foreach (QOrganizerItem item, retn)
            QOrganizerManagerEngine::addDefaultSorted(&defaultSorted, item);

        retn = defaultSorted.values();
    }

    // and return the first maxCount entries.
    return retn.mid(0, maxCount);
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::itemOccurrences(const QOrganizerItem &parentItem,
                                                                  const QDateTime &startDateTime,
                                                                  const QDateTime &endDateTime, int maxCount,
                                                                  const QOrganizerItemFetchHint &fetchHint,
                                                                  QOrganizerManager::Error *error)
{
    Q_UNUSED(fetchHint);
    return internalItemOccurrences(parentItem, startDateTime, endDateTime, maxCount, true, true, 0, error);
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                                        const QDateTime &endDateTime, int maxCount,
                                                        const QList<QOrganizerItemSortOrder> &sortOrders,
                                                        const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
{
    QList<QOrganizerItem> list;
    if (sortOrders.size() > 0) {
        list = internalItems(startDateTime, endDateTime, filter, sortOrders, fetchHint, error, false);
    } else {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setDetail(QOrganizerItemDetail::TypeEventTime, QOrganizerEventTime::FieldStartDateTime);
        sortOrder.setDirection(Qt::AscendingOrder);

        QList<QOrganizerItemSortOrder> sortOrders;
        sortOrders.append(sortOrder);

        sortOrder.setDetail(QOrganizerItemDetail::TypeTodoTime, QOrganizerTodoTime::FieldStartDateTime);
        sortOrders.append(sortOrder);

        sortOrder.setDetail(QOrganizerItemDetail::TypeTodoTime, QOrganizerTodoTime::FieldStartDateTime);
        sortOrders.append(sortOrder);

        list = internalItems(startDateTime, endDateTime, filter, sortOrders, fetchHint, error, false);
    }

    if (maxCount < 0)
        return list;
    else
        return list.mid(0, maxCount);
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::itemsForExport(const QDateTime &startDateTime,
                                                                 const QDateTime &endDateTime,
                                                                 const QOrganizerItemFilter &filter,
                                                                 const QList<QOrganizerItemSortOrder> &sortOrders,
                                                                 const QOrganizerItemFetchHint &fetchHint,
                                                                 QOrganizerManager::Error *error)
{
    return internalItems(startDateTime, endDateTime, filter, sortOrders, fetchHint, error, true);
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::itemsForExport(const QList<QOrganizerItemId> &ids, const QOrganizerItemFetchHint &fetchHint, QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    QOrganizerItemIdFilter filter;
    filter.setIds(ids);

    QList<QOrganizerItem> unsorted = itemsForExport(QDateTime(), QDateTime(), filter, QOrganizerItemSortOrder(), fetchHint, error);

    // Build an index into the results
    QHash<QOrganizerItemId, int> idMap; // value is index into unsorted
    if (*error == QOrganizerManager::NoError) {
        for (int i = 0; i < unsorted.size(); i++) {
            idMap.insert(unsorted[i].id(), i);
        }
    }

    // Build up the results and errors
    QList<QOrganizerItem> results;
    for (int i = 0; i < ids.count(); i++) {
        QOrganizerItemId id(ids[i]);
        if (!idMap.contains(id)) {
            if (errorMap)
                errorMap->insert(i, QOrganizerManager::DoesNotExistError);
            if (*error == QOrganizerManager::NoError)
                *error = QOrganizerManager::DoesNotExistError;
            results.append(QOrganizerItem());
        } else {
            results.append(unsorted[idMap[id]]);
        }
    }

    return results;
}

QOrganizerItem QOrganizerItemMemoryEngine::item(const QOrganizerItemId& organizeritemId) const
{
    return d->m_idToItemHash.value(organizeritemId);
}

QList<QOrganizerItem> QOrganizerItemMemoryEngine::internalItems(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error, bool forExport) const
{
    Q_UNUSED(fetchHint); // no optimisations are possible in the memory backend; ignore the fetch hint.
    Q_UNUSED(error);

    QList<QOrganizerItem> sorted;
    QSet<QOrganizerItemId> parentsAdded;
    bool isDefFilter = (filter.type() == QOrganizerItemFilter::DefaultFilter);

    foreach(const QOrganizerItem& c, d->m_idToItemHash) {
        if (itemHasReccurence(c)) {
            addItemRecurrences(sorted, c, startDate, endDate, filter, sortOrders, forExport, &parentsAdded);
        } else {
            if ((isDefFilter || QOrganizerManagerEngine::testFilter(filter, c)) && QOrganizerManagerEngine::isItemBetweenDates(c, startDate, endDate)) {
                QOrganizerManagerEngine::addSorted(&sorted,c, sortOrders);
                if (forExport
                        && (c.type() == QOrganizerItemType::TypeEventOccurrence
                        ||  c.type() == QOrganizerItemType::TypeTodoOccurrence)) {
                    QOrganizerItemId parentId(c.detail(QOrganizerItemDetail::TypeParent).value<QOrganizerItemId>(QOrganizerItemParent::FieldParentId));
                    if (!parentsAdded.contains(parentId)) {
                        parentsAdded.insert(parentId);
                        QOrganizerManagerEngine::addSorted(&sorted, item(parentId), sortOrders);
                    }
                }
            }
        }
    }

    return sorted;
}


void QOrganizerItemMemoryEngine::addItemRecurrences(QList<QOrganizerItem>& sorted, const QOrganizerItem& c, const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, bool forExport, QSet<QOrganizerItemId>* parentsAdded) const
{
    QOrganizerManager::Error error = QOrganizerManager::NoError;
    if (forExport && parentsAdded->contains(c.id()))
        return;

    QList<QOrganizerItem> recItems = internalItemOccurrences(c, startDate, endDate, forExport ? 1 : 50, false, false, 0, &error); // XXX TODO: why maxcount of 50?
    if (filter.type() == QOrganizerItemFilter::DefaultFilter) {
        foreach(const QOrganizerItem& oi, recItems) {
            QOrganizerManagerEngine::addSorted(&sorted, forExport ? c : oi, sortOrders);
            if (forExport)
                parentsAdded->insert(c.id());
        }
    } else {
        foreach(const QOrganizerItem& oi, recItems) {
            if (QOrganizerManagerEngine::testFilter(filter, oi)) {
                QOrganizerManagerEngine::addSorted(&sorted, forExport ? c : oi, sortOrders);
                if (forExport)
                    parentsAdded->insert(c.id());
            }
        }
    }
}

/*! Saves the given organizeritem \a theOrganizerItem, storing any error to \a error and
    filling the \a changeSet with ids of changed organizeritems as required */
bool QOrganizerItemMemoryEngine::storeItem(QOrganizerItem* theOrganizerItem, QOrganizerItemChangeSet& changeSet, const QList<QOrganizerItemDetail::DetailType> &detailMask, QOrganizerManager::Error* error)
{
    QOrganizerCollectionId targetCollectionId = theOrganizerItem->collectionId();

    // check that the collection exists (or is null :. default collection):
    if (!targetCollectionId.isNull() && !d->m_idToCollectionHash.contains(targetCollectionId)) {
        *error = QOrganizerManager::InvalidCollectionError;
        return false;
    }

    if (theOrganizerItem->type() == QOrganizerItemType::TypeUndefined) {
        *error = QOrganizerManager::InvalidItemTypeError;
        return false;
    }

    // check to see if this organizer item already exists
    QOrganizerItemId theOrganizerItemId = theOrganizerItem->id();
    QHash<QOrganizerItemId, QOrganizerItem>::const_iterator hashIterator = d->m_idToItemHash.find(theOrganizerItemId);
    if (hashIterator != d->m_idToItemHash.constEnd()) {
        /* We also need to check that there are no modified create only details */
        QOrganizerItem oldOrganizerItem = hashIterator.value();

        if (oldOrganizerItem.type() != theOrganizerItem->type()) {
            *error = QOrganizerManager::AlreadyExistsError;
            return false;
        }

        // check that the old and new collection is the same (ie, not attempting to save to a different collection)
        if (targetCollectionId.isNull()) {
            // it already exists, so save it where it already exists.
            targetCollectionId = d->m_itemsInCollectionsHash.key(theOrganizerItemId);
        } else if (!d->m_itemsInCollectionsHash.values(targetCollectionId).contains(theOrganizerItemId)) {
            // the given collection id was non-null but doesn't already contain this item.  error.
            *error = QOrganizerManager::InvalidCollectionError;
            return false;
        }

        QOrganizerItemTimestamp ts = theOrganizerItem->detail(QOrganizerItemDetail::TypeTimestamp);
        ts.setLastModified(QDateTime::currentDateTime());
        theOrganizerItem->saveDetail(&ts);

        if (!fixOccurrenceReferences(theOrganizerItem, error)) {
            return false;
        }
        // Looks ok, so continue
        d->m_idToItemHash.insert(theOrganizerItemId, *theOrganizerItem); // replacement insert.
        changeSet.insertChangedItem(theOrganizerItemId, detailMask);

        // cross-check if stored exception occurrences are still valid
        if (itemHasReccurence(oldOrganizerItem)) {
            // if we are updating an existing item and the item had recurrence defined, it might
            // have some exception occurrences which don't match the current recurrence.

            // should we also check and remove exception dates if e.g. limit date or limit count has been changed
            // to be earlier (or smaller) than before thus invelidating an exception date..?
            QList<QOrganizerItemId> occurrenceIds = d->m_parentIdToChildIdHash.values(theOrganizerItemId);
            QOrganizerManager::Error occurrenceError = QOrganizerManager::NoError;
            if (!occurrenceIds.isEmpty()) {
                if (itemHasReccurence(*theOrganizerItem)) {
                    // generate occurrences to get the dates when there can be an exception occurrence
                    // if the new item does not have recurrence, all exception occurrences of this item
                    // are removed
                    QList<QDate> exceptionDates;
                    QList<QOrganizerItem> occurrences = internalItemOccurrences(*theOrganizerItem, QDateTime(), QDateTime(), -1, false, false, &exceptionDates, &occurrenceError);
                    foreach (const QOrganizerItemId &occurrenceId, occurrenceIds) {
                        // remove all occurrence ids from the list which have valid exception date
                        QOrganizerItemParent parentDetail = d->m_idToItemHash.value(occurrenceId).detail(QOrganizerItemDetail::TypeParent);
                        if (!parentDetail.isEmpty() && exceptionDates.contains(parentDetail.originalDate()))
                            occurrenceIds.removeOne(occurrenceId);
                    }
                }
                foreach (const QOrganizerItemId &occurrenceId, occurrenceIds)
                    removeItem(occurrenceId, changeSet, &occurrenceError);
            }
        }
    } else {
        // id does not exist; if not zero, fail.
        if (!theOrganizerItemId.isNull()) {
            // the ID is not empty, and it doesn't identify an existing organizer item in our database either.
            *error = QOrganizerManager::DoesNotExistError;
            return false;
        }
        /* New organizer item */
        QOrganizerItemTimestamp ts = theOrganizerItem->detail(QOrganizerItemDetail::TypeTimestamp);
        ts.setLastModified(QDateTime::currentDateTime());
        ts.setCreated(ts.lastModified());
        theOrganizerItem->saveDetail(&ts);

        if (!fixOccurrenceReferences(theOrganizerItem, error)) {
            return false;
        }
        // set the guid if not set
        if (theOrganizerItem->guid().isEmpty())
            theOrganizerItem->setGuid(QUuid::createUuid().toString());

        // if we're saving an exception occurrence, we need to add it's original date as an exdate to the parent.
        QOrganizerItemId parentId;
        if (theOrganizerItem->type() == QOrganizerItemType::TypeEventOccurrence
            || theOrganizerItem->type() == QOrganizerItemType::TypeTodoOccurrence) {
            // update the event or the todo by adding an EX-DATE which corresponds to the original date of the occurrence being saved.
            QOrganizerItemParent origin = theOrganizerItem->detail(QOrganizerItemDetail::TypeParent);
            parentId = origin.parentId();

            // for occurrences, if given a null collection id, save it in the same collection as the parent.
            // otherwise, ensure that the parent is in the same collection.  You cannot save an exception to a different collection than the parent.
            if (targetCollectionId.isNull()) {
                targetCollectionId = d->m_itemsInCollectionsHash.key(parentId);
                if (targetCollectionId.isNull()) {
                    *error = QOrganizerManager::UnspecifiedError; // this should never occur; parent should _always_ be in a collection.
                    return false;
                }
            } else if (!d->m_itemsInCollectionsHash.values(targetCollectionId).contains(parentId)) {
                // nope, the specified collection doesn't contain the parent.  error.
                *error = QOrganizerManager::InvalidCollectionError;
                return false;
            }

            QMap<int, QOrganizerManager::Error> tempErrorMap;
            QOrganizerManager::Error tempError = QOrganizerManager::NoError;
            const QList<QOrganizerItem> candidates = items(QList<QOrganizerItemId>() << parentId, QOrganizerItemFetchHint(), &tempErrorMap, &tempError);
            if (tempError != QOrganizerManager::NoError || candidates.isEmpty()) {
                *error = tempError != QOrganizerManager::NoError ? tempError : QOrganizerManager::UnspecifiedError;
                return false;
            }
            QOrganizerItem parentItem = candidates.first();
            QDate originalDate = origin.originalDate();
            QOrganizerItemRecurrence recurrence = parentItem.detail(QOrganizerItemDetail::TypeRecurrence);
            QSet<QDate> currentExceptionDates = recurrence.exceptionDates();
            if (!currentExceptionDates.contains(originalDate)) {
                currentExceptionDates << originalDate;
                recurrence.setExceptionDates(currentExceptionDates);
                parentItem.saveDetail(&recurrence);
                d->m_idToItemHash.insert(parentId, parentItem); // replacement insert
                changeSet.insertChangedItem(parentId, detailMask); // is this correct?  it's an exception, so change parent?
            }
        }

        // if target collection id is null, set to default id.
        if (targetCollectionId.isNull())
            targetCollectionId = defaultCollectionId();

        // update the organizer item - set its ID
        theOrganizerItemId = itemId(QByteArray(reinterpret_cast<const char *>(&d->m_nextOrganizerItemId), sizeof(quint32)));
        ++(d->m_nextOrganizerItemId);
        theOrganizerItem->setId(theOrganizerItemId);
        // finally, add the organizer item to our internal lists and return
        theOrganizerItem->setCollectionId(targetCollectionId);
        d->m_idToItemHash.insert(theOrganizerItemId, *theOrganizerItem);  // add organizer item to hash
        if (!parentId.isNull()) {
            // if it was an occurrence, we need to add it to the children hash.
            d->m_parentIdToChildIdHash.insert(parentId, theOrganizerItemId);
        }
        d->m_itemsInCollectionsHash.insert(targetCollectionId, theOrganizerItemId);
        changeSet.insertAddedItem(theOrganizerItemId);
    }

    *error = QOrganizerManager::NoError;     // successful.
    return true;
}

/*!
 * For Occurrence type items, ensure the ParentId and the Guid are set consistently.  Returns
 * false and sets \a error on error, returns true otherwise.
 */
bool QOrganizerItemMemoryEngine::fixOccurrenceReferences(QOrganizerItem* theItem, QOrganizerManager::Error* error)
{
    if (theItem->type() == QOrganizerItemType::TypeEventOccurrence
            || theItem->type() == QOrganizerItemType::TypeTodoOccurrence) {
        const QString guid = theItem->guid();
        QOrganizerItemParent instanceOrigin = theItem->detail(QOrganizerItemDetail::TypeParent);
        if (!instanceOrigin.originalDate().isValid()) {
            *error = QOrganizerManager::InvalidOccurrenceError;
            return false;
        }
        QOrganizerItemId parentId = instanceOrigin.parentId();
        if (!guid.isEmpty()) {
            if (!parentId.isNull()) {
                QOrganizerManager::Error tempError;
                QMap<int, QOrganizerManager::Error> tempErrorMap;
                QOrganizerItem parentItem = items(QList<QOrganizerItemId>() << parentId, QOrganizerItemFetchHint(), &tempErrorMap, &tempError).at(0);
                if (guid != parentItem.guid()
                        || !typesAreRelated(theItem->type(), parentItem.type())) {
                    // parentId and guid are both set and inconsistent, or the parent is the wrong
                    // type
                    *error = QOrganizerManager::InvalidOccurrenceError;
                    return false;
                }
            } else {
                // guid set but not parentId
                // find an item with the given guid
                foreach (const QOrganizerItem& item, d->m_idToItemHash) {
                    if (item.guid() == guid) {
                        parentId = item.id();
                        break;
                    }
                }
                if (parentId.isNull()) {
                    // couldn't find an item with the given guid
                    *error = QOrganizerManager::InvalidOccurrenceError;
                    return false;
                }
                QOrganizerManager::Error tempError;
                QMap<int, QOrganizerManager::Error> tempErrorMap;
                QOrganizerItem parentItem = items(QList<QOrganizerItemId>() << parentId, QOrganizerItemFetchHint(), &tempErrorMap, &tempError).at(0);
                if (!typesAreRelated(theItem->type(), parentItem.type())) {
                    // the parent is the wrong type
                    *error = QOrganizerManager::InvalidOccurrenceError;
                    return false;
                }
                // found a matching item - set the parentId of the occurrence
                QOrganizerItemParent origin = theItem->detail(QOrganizerItemDetail::TypeParent);
                origin.setParentId(parentId);
                theItem->saveDetail(&origin);
            }
        } else if (!parentId.isNull()) {
            QOrganizerManager::Error tempError;
            QMap<int, QOrganizerManager::Error> tempErrorMap;
            QOrganizerItem parentItem = items(QList<QOrganizerItemId>() << parentId, QOrganizerItemFetchHint(), &tempErrorMap, &tempError).at(0);
            if (parentItem.guid().isEmpty()
                    || !typesAreRelated(theItem->type(), parentItem.type())) {
                // found the matching item but it has no guid, or it isn't the right type
                *error = QOrganizerManager::InvalidOccurrenceError;
                return false;
            }
            theItem->setGuid(parentItem.guid());
        } else {
            // neither parentId or guid is supplied
            *error = QOrganizerManager::InvalidOccurrenceError;
            return false;
        }
    }
    return true;
}

/*!
 * Returns true if and only if \a occurrenceType is the "Occurrence" version of \a parentType.
 */
bool QOrganizerItemMemoryEngine::typesAreRelated(QOrganizerItemType::ItemType occurrenceType, QOrganizerItemType::ItemType parentType)
{
    return ((parentType == QOrganizerItemType::TypeEvent
                && occurrenceType == QOrganizerItemType::TypeEventOccurrence)
            || (parentType == QOrganizerItemType::TypeTodo
                && occurrenceType == QOrganizerItemType::TypeTodoOccurrence));
}

bool QOrganizerItemMemoryEngine::storeItems(QList<QOrganizerItem>* organizeritems, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                                            QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    Q_ASSERT(errorMap);

    errorMap->clear();

    if (!organizeritems) {
        *error = QOrganizerManager::BadArgumentError;
        return false;
    }

    QOrganizerItemChangeSet changeSet;
    QOrganizerItem current;
    QOrganizerManager::Error operationError = QOrganizerManager::NoError;
    for (int i = 0; i < organizeritems->count(); i++) {
        current = organizeritems->at(i);
        if (!storeItem(&current, changeSet, detailMask, error)) {
            operationError = *error;
            errorMap->insert(i, operationError);
        } else {
            (*organizeritems)[i] = current;
        }
    }

    *error = operationError;
    d->emitSharedSignals(&changeSet);
    // return false if some error occurred
    return (*error == QOrganizerManager::NoError);
}

/*! \reimp
*/
bool QOrganizerItemMemoryEngine::saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                                           QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    // TODO should the default implementation do the right thing, or return false?
    if (detailMask.isEmpty()) {
        // Non partial, just pass it on
        return storeItems(items, detailMask, errorMap, error);
    } else {
        // Partial item save.
        // Basically

        // Need to:
        // 1) fetch existing items
        // 2) strip out details in definitionMask for existing items
        // 3) copy the details from the passed in list for existing items
        // 4) for any new items, copy the masked details to a blank item
        // 5) save the modified ones
        // 6) update the id of any new items
        // 7) transfer any errors from saving to errorMap

        QList<QOrganizerItemId> existingItemIds;

        // Error conditions:
        // 1) bad id passed in (can't fetch)
        // 2) bad fetch (can't save partial update)
        // 3) bad save error
        // all of which needs to be returned in the error map

        QHash<int, int> existingIdMap; // items index to existingItems index

        // Try to figure out which of our arguments are new items
        for (int i = 0; i < items->count(); i++) {
            // See if there's a itemId that's not from this manager
            const QOrganizerItem item = items->at(i);
            if (item.id().managerUri() == managerUri()) {
                if (!item.id().isNull()) {
                    existingIdMap.insert(i, existingItemIds.count());
                    existingItemIds.append(item.id());
                } else {
                    // Strange. it's just a new item
                }
            } else if (!item.id().managerUri().isEmpty() || !item.id().isNull()) {
                // Hmm, error (wrong manager)
                errorMap->insert(i, QOrganizerManager::DoesNotExistError);
            } // else new item
        }

        // Now fetch the existing items
        QMap<int, QOrganizerManager::Error> fetchErrors;
        QOrganizerManager::Error fetchError = QOrganizerManager::NoError;
        QList<QOrganizerItem> existingItems = this->itemsForExport(existingItemIds, QOrganizerItemFetchHint(), &fetchErrors, &fetchError);

        // Prepare the list to save
        QList<QOrganizerItem> itemsToSave;
        QList<int> savedToOriginalMap; // itemsToSave index to items index

        for (int i = 0; i < items->count(); i++) {
            // See if this is an existing item or a new one
            const int fetchedIdx = existingIdMap.value(i, -1);
            QOrganizerItem itemToSave;
            if (fetchedIdx >= 0) {
                // See if we had an error
                if (fetchErrors[fetchedIdx] != QOrganizerManager::NoError) {
                    errorMap->insert(i, fetchErrors[fetchedIdx]);
                    continue;
                }

                // Existing item we should have fetched
                itemToSave = existingItems.at(fetchedIdx);

                // QOrganizerItemData::removeOnly() is not exported, so we can only do this...
                foreach (QOrganizerItemDetail::DetailType mask, detailMask) {
                    QList<QOrganizerItemDetail> details(itemToSave.details(mask));
                    foreach (QOrganizerItemDetail detail, details)
                        itemToSave.removeDetail(&detail);
                }
            } else if (errorMap->contains(i)) {
                // A bad argument.  Leave it out of the itemsToSave list
                continue;
            } else {
                // new item
                itemToSave.setType(items->at(i).type());
            }

            // Now copy in the details from the arguments
            const QOrganizerItem& item = items->at(i);

            // Perhaps this could do this directly rather than through saveDetail
            // but that would duplicate the checks for display label etc
            foreach (QOrganizerItemDetail::DetailType name, detailMask) {
                QList<QOrganizerItemDetail> details = item.details(name);
                foreach (QOrganizerItemDetail detail, details)
                    itemToSave.saveDetail(&detail);
            }
            savedToOriginalMap.append(i);
            itemsToSave.append(itemToSave);
        }

        // Now save them
        QMap<int, QOrganizerManager::Error> saveErrors;
        QOrganizerManager::Error saveError = QOrganizerManager::NoError;
        storeItems(&itemsToSave, detailMask, &saveErrors, &saveError);
        // Now update the passed in arguments, where necessary

        // Update IDs of the items list
        for (int i = 0; i < itemsToSave.count(); i++) {
            (*items)[savedToOriginalMap[i]].setId(itemsToSave[i].id());
        }
        // Populate the errorMap with the errorMap of the attempted save
        QMap<int, QOrganizerManager::Error>::iterator it(saveErrors.begin());
        while (it != saveErrors.end()) {
            if (it.value() != QOrganizerManager::NoError) {
                errorMap->insert(savedToOriginalMap[it.key()], it.value());
            }
            it++;
        }
        return errorMap->isEmpty();
    }
}

/*! Removes the organizer item identified by the given \a organizeritemId, storing any error to \a error and
    filling the \a changeSet with ids of changed organizer items as required
*/
bool QOrganizerItemMemoryEngine::removeItem(const QOrganizerItemId& organizeritemId, QOrganizerItemChangeSet& changeSet, QOrganizerManager::Error* error)
{
    QHash<QOrganizerItemId, QOrganizerItem>::const_iterator hashIterator = d->m_idToItemHash.find(organizeritemId);
    if (hashIterator == d->m_idToItemHash.constEnd()) {
        *error = QOrganizerManager::DoesNotExistError;
        return false;
    }

    // if it is a child item, remove itself from the children hash
    QOrganizerItem thisItem = hashIterator.value();
    QOrganizerItemParent parentDetail = thisItem.detail(QOrganizerItemDetail::TypeParent);
    if (!parentDetail.parentId().isNull()) {
        d->m_parentIdToChildIdHash.remove(parentDetail.parentId(), organizeritemId);
    }

    // if it is a parent item, remove any children.
    QList<QOrganizerItemId> childrenIds = d->m_parentIdToChildIdHash.values(organizeritemId);
    foreach (const QOrganizerItemId& childId, childrenIds) {
        // remove the child occurrence from our lists.
        d->m_idToItemHash.remove(childId);
        d->m_itemsInCollectionsHash.remove(d->m_itemsInCollectionsHash.key(childId), childId);
        changeSet.insertRemovedItem(childId);
    }

    // remove the organizer item from the lists.
    d->m_idToItemHash.remove(organizeritemId);
    d->m_parentIdToChildIdHash.remove(organizeritemId);
    d->m_itemsInCollectionsHash.remove(d->m_itemsInCollectionsHash.key(organizeritemId), organizeritemId);
    *error = QOrganizerManager::NoError;

    changeSet.insertRemovedItem(organizeritemId);
    return true;
}

/*! Removes the organizer item occurrence identified by the given \a organizeritem. Removing a generated occurrence means
    adding a new exception date to parent items exception date list. Stores any error to \a error and
    fills the \a changeSet with ids of changed organizer items as required
*/
bool QOrganizerItemMemoryEngine::removeOccurrence(const QOrganizerItem &organizeritem, QOrganizerItemChangeSet &changeSet, QOrganizerManager::Error *error)
{
    QOrganizerItemParent parentDetail = organizeritem.detail(QOrganizerItemDetail::TypeParent);
    if (parentDetail.parentId().isNull()) {
        *error = QOrganizerManager::InvalidOccurrenceError;
        return false;
    }

    QHash<QOrganizerItemId, QOrganizerItem>::const_iterator hashIterator = d->m_idToItemHash.find(parentDetail.parentId());
    if (hashIterator == d->m_idToItemHash.constEnd()) {
        *error = QOrganizerManager::InvalidOccurrenceError;
        return false;
    } else {
        QOrganizerItem parentItem = hashIterator.value();
        QOrganizerItemRecurrence recurrenceDetail = parentItem.detail(QOrganizerItemDetail::TypeRecurrence);
        QSet<QDate> exceptionDates = recurrenceDetail.exceptionDates();
        exceptionDates.insert(parentDetail.originalDate());
        recurrenceDetail.setExceptionDates(exceptionDates);
        parentItem.saveDetail(&recurrenceDetail);
        d->m_idToItemHash.insert(parentDetail.parentId(), parentItem);
        changeSet.insertChangedItem(parentDetail.parentId(), QList<QOrganizerItemDetail::DetailType>());
    }
    *error = QOrganizerManager::NoError;
    return true;
}

/*! \reimp
*/
bool QOrganizerItemMemoryEngine::removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                                             QOrganizerManager::Error *error)
{
    Q_ASSERT(errorMap);

    if (itemIds.count() == 0) {
        *error = QOrganizerManager::BadArgumentError;
        return false;
    }

    QOrganizerItemChangeSet changeSet;
    QOrganizerItemId current;
    QOrganizerManager::Error operationError = QOrganizerManager::NoError;
    for (int i = 0; i < itemIds.count(); i++) {
        current = itemIds.at(i);
        if (!removeItem(current, changeSet, error)) {
            operationError = *error;
            errorMap->insert(i, operationError);
        }
    }

    *error = operationError;
    d->emitSharedSignals(&changeSet);

    // return false if some errors occurred
    return (*error == QOrganizerManager::NoError);
}

/*! \reimp
*/
bool QOrganizerItemMemoryEngine::removeItems(const QList<QOrganizerItem> *items, QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    Q_ASSERT(errorMap);
    if (items->count() == 0) {
        *error = QOrganizerManager::BadArgumentError;
        return false;
    }

    QOrganizerItemChangeSet changeSet;
    QOrganizerItem current;
    QSet<QOrganizerItemId> removedParentIds;
    QOrganizerManager::Error operationError = QOrganizerManager::NoError;
    for (int i = 0; i < items->count(); i++) {
        current = items->at(i);
        QOrganizerManager::Error tempError = QOrganizerManager::NoError;
        if ((current.type() == QOrganizerItemType::TypeEventOccurrence
             || current.type() == QOrganizerItemType::TypeTodoOccurrence)
                && current.id().isNull()) {
            // this is a generated occurrence, modify parent items exception dates
            QOrganizerItemParent parentDetail = current.detail(QOrganizerItemDetail::TypeParent);
            if (removedParentIds.isEmpty() || !removedParentIds.contains(parentDetail.parentId()))
                removeOccurrence(current, changeSet, &tempError);
        } else {
            removeItem(current.id(), changeSet, &tempError);
            if (tempError == QOrganizerManager::NoError && itemHasReccurence(current))
                removedParentIds.insert(current.id());
        }
        if (tempError != QOrganizerManager::NoError) {
            errorMap->insert(i, tempError);
            operationError = tempError;
        }
    }

    *error = operationError;
    d->emitSharedSignals(&changeSet);

    // return false if some errors occurred
    return (*error == QOrganizerManager::NoError);
}

QOrganizerCollectionId QOrganizerItemMemoryEngine::defaultCollectionId() const
{
    const uint id(QOrganizerItemMemoryEngineData::DefaultCollectionLocalId);
    return collectionId(QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}

QOrganizerCollection QOrganizerItemMemoryEngine::collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    if (d->m_idToCollectionHash.contains(collectionId)) {
        *error = QOrganizerManager::NoError;
        return d->m_idToCollectionHash.value(collectionId);
    }

    *error = QOrganizerManager::DoesNotExistError;
    return QOrganizerCollection();
}

QList<QOrganizerCollection> QOrganizerItemMemoryEngine::collections(QOrganizerManager::Error* error)
{
    Q_ASSERT(!d->m_idToCollectionHash.isEmpty());
    *error = QOrganizerManager::NoError;
    return d->m_idToCollectionHash.values();
}

bool QOrganizerItemMemoryEngine::saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error)
{
    QOrganizerCollectionId collectionId = collection->id();

    QOrganizerCollectionChangeSet cs;
    if (d->m_idToCollectionHash.contains(collectionId)) {
        // this collection already exists.  update our internal list
        // if the collection has been modified.
        if (d->m_idToCollectionHash.value(collectionId) == *collection) {
            *error = QOrganizerManager::NoError;
            return true;
        }

        cs.insertChangedCollection(collectionId);
    } else {
        // this must be a new collection.  check that the id is null.
        if (!collectionId.isNull() && collectionId.managerUri() != d->m_managerUri) {
            // nope, this collection belongs in another manager, or has been deleted.
            *error = QOrganizerManager::DoesNotExistError;
            return false;
        }

        // this is a new collection with a null id; create a new id, add it to our list.
        collectionId = this->collectionId(QByteArray(reinterpret_cast<const char *>(&d->m_nextOrganizerCollectionId), sizeof(quint32)));
        ++(d->m_nextOrganizerCollectionId);
        collection->setId(collectionId);
        cs.insertAddedCollection(collectionId);
    }

    d->m_idToCollectionHash.insert(collectionId, *collection);
    d->emitSharedSignals(&cs);
    *error = QOrganizerManager::NoError;
    return true;
}

bool QOrganizerItemMemoryEngine::removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    if (collectionId == defaultCollectionId()) {
        // attempting to remove the default collection.  this is not allowed in the memory engine.
        *error = QOrganizerManager::PermissionsError;
        return false;
    }

    // try to find the collection to remove it (and the items it contains)
    if (d->m_idToCollectionHash.contains(collectionId)) {
        // found the collection to remove.  remove the items in the collection.
        const QList<QOrganizerItemId> itemsToRemove = d->m_itemsInCollectionsHash.values(collectionId);
        if (!itemsToRemove.isEmpty()) {
            QMap<int, QOrganizerManager::Error> errorMap;
            if (!removeItems(itemsToRemove, &errorMap, error)) {
                // without transaction support, we can't back out.  but the operation should fail.
                return false;
            }
        }

        // now remove the collection from our lists.
        d->m_idToCollectionHash.remove(collectionId);
        d->m_itemsInCollectionsHash.remove(collectionId);
        QOrganizerCollectionChangeSet cs;
        cs.insertRemovedCollection(collectionId);
        d->emitSharedSignals(&cs);
        *error = QOrganizerManager::NoError;
        return true;
    }

    // the collection doesn't exist...
    *error = QOrganizerManager::DoesNotExistError;
    return false;
}

/*! \reimp
*/
void QOrganizerItemMemoryEngine::requestDestroyed(QOrganizerAbstractRequest* req)
{
    Q_UNUSED(req);
}

/*! \reimp
*/
bool QOrganizerItemMemoryEngine::startRequest(QOrganizerAbstractRequest* req)
{
    updateRequestState(req, QOrganizerAbstractRequest::ActiveState);
    performAsynchronousOperation(req);

    return true;
}

/*! \reimp
*/
bool QOrganizerItemMemoryEngine::cancelRequest(QOrganizerAbstractRequest* req)
{
    Q_UNUSED(req); // we can't cancel since we complete immediately
    return false;
}

/*! \reimp
*/
bool QOrganizerItemMemoryEngine::waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs)
{
    // in our implementation, we always complete any operation we start.
    Q_UNUSED(msecs);
    Q_UNUSED(req);

    return true;
}

QList<QOrganizerItemDetail::DetailType> QOrganizerItemMemoryEngine::supportedItemDetails(QOrganizerItemType::ItemType itemType) const
{
    QList<QOrganizerItemDetail::DetailType> supportedDetails;
    supportedDetails << QOrganizerItemDetail::TypeItemType
                     << QOrganizerItemDetail::TypeGuid
                     << QOrganizerItemDetail::TypeTimestamp
                     << QOrganizerItemDetail::TypeDisplayLabel
                     << QOrganizerItemDetail::TypeDescription
                     << QOrganizerItemDetail::TypeComment
                     << QOrganizerItemDetail::TypeTag
                     << QOrganizerItemDetail::TypeClassification
                     << QOrganizerItemDetail::TypeExtendedDetail;

    if (itemType == QOrganizerItemType::TypeEvent) {
        supportedDetails << QOrganizerItemDetail::TypeRecurrence
                         << QOrganizerItemDetail::TypeEventTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeLocation
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder
                         << QOrganizerItemDetail::TypeEmailReminder
                         << QOrganizerItemDetail::TypeVisualReminder;
    } else if (itemType == QOrganizerItemType::TypeTodo) {
        supportedDetails << QOrganizerItemDetail::TypeRecurrence
                         << QOrganizerItemDetail::TypeTodoTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeTodoProgress
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder
                         << QOrganizerItemDetail::TypeEmailReminder
                         << QOrganizerItemDetail::TypeVisualReminder;
    } else if (itemType == QOrganizerItemType::TypeEventOccurrence) {
        supportedDetails << QOrganizerItemDetail::TypeParent
                         << QOrganizerItemDetail::TypeEventTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeLocation
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder
                         << QOrganizerItemDetail::TypeEmailReminder
                         << QOrganizerItemDetail::TypeVisualReminder;
    } else if (itemType == QOrganizerItemType::TypeTodoOccurrence) {
        supportedDetails << QOrganizerItemDetail::TypeParent
                         << QOrganizerItemDetail::TypeTodoTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeTodoProgress
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder
                         << QOrganizerItemDetail::TypeEmailReminder
                         << QOrganizerItemDetail::TypeVisualReminder;
    } else if (itemType == QOrganizerItemType::TypeJournal) {
        supportedDetails << QOrganizerItemDetail::TypeJournalTime;
    } else if (itemType == QOrganizerItemType::TypeNote) {
        // nothing ;)
    } else {
        supportedDetails.clear();
    }

    return supportedDetails;
}

/*!
 * This slot is called some time after an asynchronous request is started.
 * It performs the required operation, sets the result and returns.
 */
void QOrganizerItemMemoryEngine::performAsynchronousOperation(QOrganizerAbstractRequest *currentRequest)
{
    // store up changes, and emit signals once at the end of the (possibly batch) operation.
    QOrganizerItemChangeSet changeSet;

    // Now perform the active request and emit required signals.
    Q_ASSERT(currentRequest->state() == QOrganizerAbstractRequest::ActiveState);
    switch (currentRequest->type()) {
        case QOrganizerAbstractRequest::ItemFetchRequest:
        {
            QOrganizerItemFetchRequest* r = static_cast<QOrganizerItemFetchRequest*>(currentRequest);
            QOrganizerItemFilter filter = r->filter();
            QList<QOrganizerItemSortOrder> sorting = r->sorting();
            QOrganizerItemFetchHint fetchHint = r->fetchHint();
            QDateTime startDate = r->startDate();
            QDateTime endDate = r->endDate();

            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerItem> requestedOrganizerItems = items(filter, startDate, endDate, -1, sorting, fetchHint, &operationError);

            // update the request with the results.
            if (!requestedOrganizerItems.isEmpty() || operationError != QOrganizerManager::NoError)
                updateItemFetchRequest(r, requestedOrganizerItems, operationError, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;

    case QOrganizerAbstractRequest::ItemFetchByIdRequest: {
        QOrganizerItemFetchByIdRequest* r = static_cast<QOrganizerItemFetchByIdRequest*>(currentRequest);
        // fetch hint cannot be used in memory backend

        QOrganizerManager::Error operationError = QOrganizerManager::NoError;
        QMap<int, QOrganizerManager::Error> errorMap;

        QList<QOrganizerItem> requestedOrganizerItems;

        for (int i = 0; i < r->ids().size(); i++) {
            QOrganizerItem item = d->m_idToItemHash.value(r->ids().at(i), QOrganizerItem());
            requestedOrganizerItems.append(item);
            if (item.isEmpty())
                errorMap.insert(i, QOrganizerManager::DoesNotExistError);
        }

        // update the request with the results.
        if (!requestedOrganizerItems.isEmpty() || operationError != QOrganizerManager::NoError || !errorMap.isEmpty())
            QOrganizerManagerEngine::updateItemFetchByIdRequest(r, requestedOrganizerItems, operationError, errorMap, QOrganizerAbstractRequest::FinishedState);
        else
            updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
    }
    break;

        case QOrganizerAbstractRequest::ItemFetchForExportRequest:
        {
            QOrganizerItemFetchForExportRequest* r = static_cast<QOrganizerItemFetchForExportRequest*>(currentRequest);
            QOrganizerItemFilter filter = r->filter();
            QList<QOrganizerItemSortOrder> sorting = r->sorting();
            QOrganizerItemFetchHint fetchHint = r->fetchHint();
            QDateTime startDate = r->startDate();
            QDateTime endDate = r->endDate();

            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerItem> requestedOrganizerItems = itemsForExport(startDate, endDate, filter, sorting, fetchHint, &operationError);

            // update the request with the results.
            if (!requestedOrganizerItems.isEmpty() || operationError != QOrganizerManager::NoError)
                updateItemFetchForExportRequest(r, requestedOrganizerItems, operationError, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::ItemOccurrenceFetchRequest:
        {
            QOrganizerItemOccurrenceFetchRequest* r = static_cast<QOrganizerItemOccurrenceFetchRequest*>(currentRequest);
            QOrganizerItem parentItem(r->parentItem());
            QDateTime startDate(r->startDate());
            QDateTime endDate(r->endDate());
            int countLimit = r->maxOccurrences();
            QOrganizerItemFetchHint fetchHint = r->fetchHint();

            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerItem> requestedOrganizerItems = itemOccurrences(parentItem, startDate, endDate, countLimit, fetchHint, &operationError);

            // update the request with the results.
            if (!requestedOrganizerItems.isEmpty() || operationError != QOrganizerManager::NoError)
                updateItemOccurrenceFetchRequest(r, requestedOrganizerItems, operationError, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;


        case QOrganizerAbstractRequest::ItemIdFetchRequest:
        {
            QOrganizerItemIdFetchRequest* r = static_cast<QOrganizerItemIdFetchRequest*>(currentRequest);
            QOrganizerItemFilter filter = r->filter();
            QList<QOrganizerItemSortOrder> sorting = r->sorting();
            QDateTime startDate = r->startDate();
            QDateTime endDate = r->endDate();

            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerItemId> requestedOrganizerItemIds = itemIds(filter, startDate, endDate, sorting, &operationError);

            if (!requestedOrganizerItemIds.isEmpty() || operationError != QOrganizerManager::NoError)
                updateItemIdFetchRequest(r, requestedOrganizerItemIds, operationError, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::ItemSaveRequest:
        {
            QOrganizerItemSaveRequest* r = static_cast<QOrganizerItemSaveRequest*>(currentRequest);
            QList<QOrganizerItem> organizeritems = r->items();

            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QMap<int, QOrganizerManager::Error> errorMap;
            saveItems(&organizeritems, r->detailMask(), &errorMap, &operationError);

            updateItemSaveRequest(r, organizeritems, operationError, errorMap, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::ItemRemoveRequest:
        {
            QOrganizerItemRemoveRequest* r = static_cast<QOrganizerItemRemoveRequest*>(currentRequest);
            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerItem> organizeritemsToRemove = r->items();
            QSet<QOrganizerItemId> removedParentIds;
            QMap<int, QOrganizerManager::Error> errorMap;

            for (int i = 0; i < organizeritemsToRemove.size(); i++) {
                QOrganizerItem item = organizeritemsToRemove[i];
                QOrganizerManager::Error tempError = QOrganizerManager::NoError;
                if ((item.type() == QOrganizerItemType::TypeEventOccurrence
                     || item.type() == QOrganizerItemType::TypeTodoOccurrence)
                        && item.id().isNull()) {
                    // this is a generated occurrence, modify parent items exception dates
                    QOrganizerItemParent parentDetail = item.detail(QOrganizerItemDetail::TypeParent);
                    if (removedParentIds.isEmpty() || !removedParentIds.contains(parentDetail.parentId()))
                        removeOccurrence(item, changeSet, &tempError);
                } else {
                    removeItem(item.id(), changeSet, &tempError);
                    if (tempError == QOrganizerManager::NoError && itemHasReccurence(item))
                        removedParentIds.insert(item.id());
                }
                if (tempError != QOrganizerManager::NoError) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
            }
            if (!errorMap.isEmpty() || operationError != QOrganizerManager::NoError)
                updateItemRemoveRequest(r, operationError, errorMap, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::ItemRemoveByIdRequest:
        {
            QOrganizerItemRemoveByIdRequest* r = static_cast<QOrganizerItemRemoveByIdRequest*>(currentRequest);
            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerItemId> organizeritemsToRemove = r->itemIds();
            QMap<int, QOrganizerManager::Error> errorMap;

            for (int i = 0; i < organizeritemsToRemove.size(); i++) {
                QOrganizerManager::Error tempError = QOrganizerManager::NoError;
                removeItem(organizeritemsToRemove.at(i), changeSet, &tempError);

                if (tempError != QOrganizerManager::NoError) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
            }

            if (!errorMap.isEmpty() || operationError != QOrganizerManager::NoError)
                updateItemRemoveByIdRequest(r, operationError, errorMap, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::CollectionFetchRequest:
        {
            QOrganizerCollectionFetchRequest* r = static_cast<QOrganizerCollectionFetchRequest*>(currentRequest);
            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerCollection> requestedOrganizerCollections = collections(&operationError);

            // update the request with the results.
            updateCollectionFetchRequest(r, requestedOrganizerCollections, operationError, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::CollectionSaveRequest:
        {
            QOrganizerCollectionSaveRequest* r = static_cast<QOrganizerCollectionSaveRequest*>(currentRequest);
            QList<QOrganizerCollection> collections = r->collections();
            QList<QOrganizerCollection> retn;

            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QMap<int, QOrganizerManager::Error> errorMap;
            for (int i = 0; i < collections.size(); ++i) {
                QOrganizerManager::Error tempError = QOrganizerManager::NoError;
                QOrganizerCollection curr = collections.at(i);
                if (!saveCollection(&curr, &tempError)) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
                retn.append(curr);
            }

            updateCollectionSaveRequest(r, retn, operationError, errorMap, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        case QOrganizerAbstractRequest::CollectionRemoveRequest:
        {
            // removes the collections identified in the list of ids.
            QOrganizerCollectionRemoveRequest* r = static_cast<QOrganizerCollectionRemoveRequest*>(currentRequest);
            QOrganizerManager::Error operationError = QOrganizerManager::NoError;
            QList<QOrganizerCollectionId> collectionsToRemove = r->collectionIds();
            QMap<int, QOrganizerManager::Error> errorMap;

            for (int i = 0; i < collectionsToRemove.size(); i++) {
                QOrganizerManager::Error tempError = QOrganizerManager::NoError;
                removeCollection(collectionsToRemove.at(i), &tempError);

                if (tempError != QOrganizerManager::NoError) {
                    errorMap.insert(i, tempError);
                    operationError = tempError;
                }
            }

            if (!errorMap.isEmpty() || operationError != QOrganizerManager::NoError)
                updateCollectionRemoveRequest(r, operationError, errorMap, QOrganizerAbstractRequest::FinishedState);
            else
                updateRequestState(currentRequest, QOrganizerAbstractRequest::FinishedState);
        }
        break;

        default: // unknown request type.
        break;
    }

    // now emit any signals we have to emit
    d->emitSharedSignals(&changeSet);
}

#include "moc_qorganizeritemmemorybackend_p.cpp"

QT_END_NAMESPACE_ORGANIZER
