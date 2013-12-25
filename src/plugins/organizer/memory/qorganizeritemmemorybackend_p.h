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

#include <qorganizeritemengineid.h>
#include <qorganizercollectionengineid.h>
#include <qorganizermanagerengine.h>
#include <qorganizermanagerenginefactory.h>
#include <qorganizeritemchangeset.h>
#include <qorganizerrecurrencerule.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemMemoryFactory : public QOrganizerManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QOrganizerManagerEngineFactoryInterface" FILE "memory.json")

public:
    QOrganizerManagerEngine* engine(const QMap<QString, QString>& parameters, QOrganizerManager::Error*);
    QOrganizerItemEngineId* createItemEngineId(const QMap<QString, QString>& parameters, const QString& idString) const;
    QOrganizerCollectionEngineId* createCollectionEngineId(const QMap<QString, QString>& parameters, const QString& idString) const;
    QString managerName() const;
};

class QOrganizerItemMemoryEngineId : public QOrganizerItemEngineId
{
public:
    QOrganizerItemMemoryEngineId();
    QOrganizerItemMemoryEngineId(quint32 collectionId, quint32 itemId, const QString& managerUri);
    ~QOrganizerItemMemoryEngineId();
    QOrganizerItemMemoryEngineId(const QOrganizerItemMemoryEngineId& other);
    QOrganizerItemMemoryEngineId(const QString& idString);

    bool isEqualTo(const QOrganizerItemEngineId* other) const;
    bool isLessThan(const QOrganizerItemEngineId* other) const;

    QString managerUri() const;
    QOrganizerItemEngineId* clone() const;

    QString toString() const;

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const;
#endif
    uint hash() const;

private:
    quint32 m_collectionId;
    quint32 m_itemId;
    QString m_managerUri;
    friend class QOrganizerItemMemoryEngine;
};

class QOrganizerCollectionMemoryEngineId : public QOrganizerCollectionEngineId
{
public:
    QOrganizerCollectionMemoryEngineId();
    QOrganizerCollectionMemoryEngineId(quint32 collectionId, const QString& managerUri);
    ~QOrganizerCollectionMemoryEngineId();
    QOrganizerCollectionMemoryEngineId(const QOrganizerCollectionMemoryEngineId& other);
    QOrganizerCollectionMemoryEngineId(const QString& idString);

    bool isEqualTo(const QOrganizerCollectionEngineId* other) const;
    bool isLessThan(const QOrganizerCollectionEngineId* other) const;

    QString managerUri() const;
    QOrganizerCollectionEngineId* clone() const;

    QString toString() const;

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const;
#endif
    uint hash() const;

private:
    quint32 m_collectionId;
    QString m_managerUri;
    friend class QOrganizerItemMemoryEngine;
};

class QOrganizerAbstractRequest;
class QOrganizerManagerEngine;
class QOrganizerItemMemoryEngineData : public QSharedData
{
public:
    QOrganizerItemMemoryEngineData();
    ~QOrganizerItemMemoryEngineData()
    {
    }

    QString m_id;                                  // the id parameter value

    QHash<QOrganizerItemId, QOrganizerItem> m_idToItemHash; // hash of id to the item identified by that id
    QMultiHash<QOrganizerItemId, QOrganizerItemId> m_parentIdToChildIdHash; // hash of id to that item's children's ids

    QList<QOrganizerCollection> m_organizerCollections;          // list of collections
    QList<QOrganizerCollectionId> m_organizerCollectionIds; // list of collection ids
    QMultiMap<QOrganizerCollectionId, QOrganizerItemId> m_itemsInCollections; // map of collection ids to the ids of items the collection contains.
    quint32 m_nextOrganizerItemId; // the m_itemId portion of a QOrganizerItemMemoryEngineId.
    quint32 m_nextOrganizerCollectionId; // the m_collectionId portion of a QOrganizerCollectionMemoryEngineId.
    bool m_anonymous;                              // Is this backend ever shared?
    QString m_managerUri;                        // for faster lookup.

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
    QOrganizerCollection defaultCollection(QOrganizerManager::Error* error);
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
    virtual bool saveItem(QOrganizerItem* theOrganizerItem, QOrganizerItemChangeSet& changeSet, QOrganizerManager::Error* error);
    virtual bool removeItem(const QOrganizerItemId& organizeritemId, QOrganizerItemChangeSet& changeSet, QOrganizerManager::Error* error);
    virtual bool removeOccurrence(const QOrganizerItem& organizeritem, QOrganizerItemChangeSet& changeSet, QOrganizerManager::Error* error);

private:
    QOrganizerItem item(const QOrganizerItemId& organizeritemId) const;
    bool saveItems(QList<QOrganizerItem>* organizeritems, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    QList<QOrganizerItem> itemsForExport(const QList<QOrganizerItemId> &ids, const QOrganizerItemFetchHint &fetchHint, QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);
    QList<QOrganizerItem> internalItems(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error, bool forExport) const;
    QList<QOrganizerItem> internalItemOccurrences(const QOrganizerItem& parentItem, const QDateTime& periodStart, const QDateTime& periodEnd, int maxCount, bool includeExceptions, bool sortItems, QList<QDate> *exceptionDates, QOrganizerManager::Error* error) const;
    void addItemRecurrences(QList<QOrganizerItem>& sorted, const QOrganizerItem& c, const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, bool forExport, QSet<QOrganizerItemId>* parentsAdded) const;

    bool fixOccurrenceReferences(QOrganizerItem* item, QOrganizerManager::Error* error);
    bool typesAreRelated(QOrganizerItemType::ItemType occurrenceType, QOrganizerItemType::ItemType parentType);

    void performAsynchronousOperation(QOrganizerAbstractRequest* request);

    QOrganizerItemMemoryEngineData* d;
    static QMap<QString, QOrganizerItemMemoryEngineData*> engineDatas;
};

QT_END_NAMESPACE_ORGANIZER

#endif
