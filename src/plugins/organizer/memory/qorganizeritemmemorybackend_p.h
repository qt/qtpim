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

#ifndef QCONTACTMEMORYBACKEND_P_H
#define QCONTACTMEMORYBACKEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtOrganizer/qorganizermanagerengine.h>
#include <QtOrganizer/qorganizermanagerenginefactory.h>
#include <QtOrganizer/qorganizercollectionchangeset.h>
#include <QtOrganizer/qorganizeritemchangeset.h>
#include <QtOrganizer/qorganizerrecurrencerule.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemMemoryFactory : public QOrganizerManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QOrganizerManagerEngineFactoryInterface" FILE "memory.json")

public:
    QOrganizerManagerEngine* engine(const QMap<QString, QString>& parameters, QOrganizerManager::Error*);
    QString managerName() const;
};


class QOrganizerAbstractRequest;
class QOrganizerManagerEngine;
class QOrganizerItemMemoryEngineData : public QSharedData
{
public:
    enum { DefaultCollectionLocalId = 1 }; // default collection has id of 1.

    QOrganizerItemMemoryEngineData();
    ~QOrganizerItemMemoryEngineData()
    {
    }

    QString m_id;                                  // the id parameter value

    QHash<QOrganizerItemId, QOrganizerItem> m_idToItemHash; // hash of id to the item identified by that id
    QMultiHash<QOrganizerItemId, QOrganizerItemId> m_parentIdToChildIdHash; // hash of id to that item's children's ids
    QHash<QOrganizerCollectionId, QOrganizerCollection> m_idToCollectionHash; // hash of id to the collection identified by that id
    QMultiHash<QOrganizerCollectionId, QOrganizerItemId> m_itemsInCollectionsHash; // hash of collection ids to the ids of items the collection contains.
    quint32 m_nextOrganizerItemId; // the localId() portion of a QOrganizerItemId
    quint32 m_nextOrganizerCollectionId; // the localId() portion of a QOrganizerCollectionId
    QString m_managerUri;                        // for faster lookup.

    void emitSharedSignals(QOrganizerCollectionChangeSet *cs)
    {
        foreach (QOrganizerManagerEngine *engine, m_sharedEngines)
            cs->emitSignals(engine);
    }
    void emitSharedSignals(QOrganizerItemChangeSet* cs)
    {
        foreach(QOrganizerManagerEngine* engine, m_sharedEngines)
            cs->emitSignals(engine);
    }

    QList<QOrganizerManagerEngine*> m_sharedEngines;   // The list of engines that share this data
};

class QOrganizerItemMemoryEngine : public QOrganizerManagerEngine
{
    Q_OBJECT

public:
    static QOrganizerItemMemoryEngine *createMemoryEngine(const QMap<QString, QString>& parameters);

    ~QOrganizerItemMemoryEngine();

    /* URI reporting */
    QString managerName() const;
    QMap<QString, QString> managerParameters() const;
    QMap<QString, QString> idInterpretationParameters() const;

    // items
    QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    QList<QOrganizerItem> items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                const QDateTime &endDateTime, int maxCount,
                                const QList<QOrganizerItemSortOrder> &sortOrders,
                                const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    QList<QOrganizerItemId> itemIds(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                    const QDateTime &endDateTime, const QList<QOrganizerItemSortOrder> &sortOrders,
                                    QOrganizerManager::Error *error);

    QList<QOrganizerItem> itemOccurrences(const QOrganizerItem &parentItem, const QDateTime &startDateTime,
                                          const QDateTime &endDateTime, int maxCount,
                                          const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    QList<QOrganizerItem> itemsForExport(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                         const QOrganizerItemFilter &filter,
                                         const QList<QOrganizerItemSortOrder> &sortOrders,
                                         const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    bool saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                   QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    bool removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                     QOrganizerManager::Error *error);

    bool removeItems(const QList<QOrganizerItem> *items, QMap<int, QOrganizerManager::Error>* errorMap,
                     QOrganizerManager::Error* error);

    // collections
    QOrganizerCollectionId defaultCollectionId() const;
    QOrganizerCollection collection(const QOrganizerCollectionId &collectionId, QOrganizerManager::Error *error);
    QList<QOrganizerCollection> collections(QOrganizerManager::Error* error);
    bool saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error);
    bool removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);

    /* Asynchronous Request Support */
    virtual void requestDestroyed(QOrganizerAbstractRequest* req);
    virtual bool startRequest(QOrganizerAbstractRequest* req);
    virtual bool cancelRequest(QOrganizerAbstractRequest* req);
    virtual bool waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs);

    /* Capabilities reporting */
    /*! \reimp */
    virtual QList<QOrganizerItemFilter::FilterType> supportedFilters() const
    {
        QList<QOrganizerItemFilter::FilterType> supported;

        supported << QOrganizerItemFilter::InvalidFilter
                  << QOrganizerItemFilter::DetailFilter
                  << QOrganizerItemFilter::DetailFieldFilter
                  << QOrganizerItemFilter::DetailRangeFilter
                  << QOrganizerItemFilter::IntersectionFilter
                  << QOrganizerItemFilter::UnionFilter
                  << QOrganizerItemFilter::IdFilter
                  << QOrganizerItemFilter::CollectionFilter
                  << QOrganizerItemFilter::DefaultFilter;

        return supported;
    }
    /*! \reimp */
    virtual QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const;

    /*! \reimp */
    virtual QList<QOrganizerItemType::ItemType> supportedItemTypes() const
    {
        return QList<QOrganizerItemType::ItemType>() << QOrganizerItemType::TypeEvent
                             << QOrganizerItemType::TypeEventOccurrence
                             << QOrganizerItemType::TypeJournal
                             << QOrganizerItemType::TypeNote
                             << QOrganizerItemType::TypeTodo
                             << QOrganizerItemType::TypeTodoOccurrence;
    }

protected:
    QOrganizerItemMemoryEngine(QOrganizerItemMemoryEngineData* data);

protected:
    /* Implement "signal coalescing" for batch functions via change set */
    virtual bool storeItem(QOrganizerItem* theOrganizerItem, QOrganizerItemChangeSet& changeSet, const QList<QOrganizerItemDetail::DetailType> &detailMask, QOrganizerManager::Error* error);
    virtual bool removeItem(const QOrganizerItemId& organizeritemId, QOrganizerItemChangeSet& changeSet, QOrganizerManager::Error* error);
    virtual bool removeOccurrence(const QOrganizerItem& organizeritem, QOrganizerItemChangeSet& changeSet, QOrganizerManager::Error* error);

private:
    QOrganizerItem item(const QOrganizerItemId& organizeritemId) const;
    bool storeItems(QList<QOrganizerItem>* organizeritems, const QList<QOrganizerItemDetail::DetailType> &detailMask, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    QList<QOrganizerItem> itemsForExport(const QList<QOrganizerItemId> &ids, const QOrganizerItemFetchHint &fetchHint, QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);
    QList<QOrganizerItem> internalItems(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error, bool forExport) const;
    QList<QOrganizerItem> internalItemOccurrences(const QOrganizerItem& parentItem, const QDateTime& periodStart, const QDateTime& periodEnd, int maxCount, bool includeExceptions, bool sortItems, QList<QDate> *exceptionDates, QOrganizerManager::Error* error) const;
    void addItemRecurrences(QList<QOrganizerItem>& sorted, const QOrganizerItem& c, const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, bool forExport, QSet<QOrganizerItemId>* parentsAdded) const;

    bool fixOccurrenceReferences(QOrganizerItem* item, QOrganizerManager::Error* error);
    bool typesAreRelated(QOrganizerItemType::ItemType occurrenceType, QOrganizerItemType::ItemType parentType);

    void performAsynchronousOperation(QOrganizerAbstractRequest* request);

    QOrganizerItemMemoryEngineData* d;
};

QT_END_NAMESPACE_ORGANIZER

#endif // QCONTACTMEMORYBACKEND_P_H
