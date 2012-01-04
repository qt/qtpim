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

#ifndef QORGANIZERMANAGERENGINE_H
#define QORGANIZERMANAGERENGINE_H

#include <qorganizermanager.h>
#include <qorganizerabstractrequest.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerCollectionFetchRequest;
class QOrganizerCollectionRemoveRequest;
class QOrganizerCollectionSaveRequest;
class QOrganizerItemIdFetchRequest;
class QOrganizerItemFetchByIdRequest;
class QOrganizerItemFetchRequest;
class QOrganizerItemOccurrenceFetchRequest;
class QOrganizerItemRemoveRequest;
class QOrganizerItemSaveRequest;
class QOrganizerItemFetchForExportRequest;

class Q_ORGANIZER_EXPORT QOrganizerManagerEngine : public QObject
{
    Q_OBJECT

public:
    QOrganizerManagerEngine(QObject *parent = 0);

    virtual QString managerName() const;
    virtual QMap<QString, QString> managerParameters() const;
    virtual int managerVersion() const;
    QString managerUri() const;

    // items
    virtual QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                        QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    virtual QList<QOrganizerItem> items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                        const QDateTime &endDateTime, int maxCount,
                                        const QList<QOrganizerItemSortOrder> &sortOrders,
                                        const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    virtual QList<QOrganizerItemId> itemIds(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                            const QDateTime &endDateTime, const QList<QOrganizerItemSortOrder> &sortOrders,
                                            QOrganizerManager::Error *error);

    virtual QList<QOrganizerItem> itemOccurrences(const QOrganizerItem &parentItem, const QDateTime &startDateTime,
                                                  const QDateTime &endDateTime, int maxCount,
                                                  const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    virtual QList<QOrganizerItem> itemsForExport(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                 const QOrganizerItemFilter &filter,
                                                 const QList<QOrganizerItemSortOrder> &sortOrders,
                                                 const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error);

    virtual bool saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                           QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error);

    virtual bool removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                             QOrganizerManager::Error *error);

    // collections
    virtual QOrganizerCollection defaultCollection(QOrganizerManager::Error *error);
    virtual QOrganizerCollection collection(const QOrganizerCollectionId &collectionId, QOrganizerManager::Error *error);
    virtual QList<QOrganizerCollection> collections(QOrganizerManager::Error *error);
    virtual bool saveCollection(QOrganizerCollection *collection, QOrganizerManager::Error *error);
    virtual bool removeCollection(const QOrganizerCollectionId &collectionId, QOrganizerManager::Error *error);

    // asynchronous request support
    virtual void requestDestroyed(QOrganizerAbstractRequest *request);
    virtual bool startRequest(QOrganizerAbstractRequest *request);
    virtual bool cancelRequest(QOrganizerAbstractRequest *request);
    virtual bool waitForRequestFinished(QOrganizerAbstractRequest *request, int msecs);

    static void updateRequestState(QOrganizerAbstractRequest *request, QOrganizerAbstractRequest::State state);

    static void updateItemOccurrenceFetchRequest(QOrganizerItemOccurrenceFetchRequest *request, const QList<QOrganizerItem> &result,
                                                 QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);

    static void updateItemIdFetchRequest(QOrganizerItemIdFetchRequest *request, const QList<QOrganizerItemId> &result,
                                         QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);

    static void updateItemFetchByIdRequest(QOrganizerItemFetchByIdRequest *request, const QList<QOrganizerItem> &result,
                                           QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error> &errorMap,
                                           QOrganizerAbstractRequest::State);

    static void updateItemFetchRequest(QOrganizerItemFetchRequest *request, const QList<QOrganizerItem> &result,
                                       QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);

    static void updateItemFetchForExportRequest(QOrganizerItemFetchForExportRequest *request, const QList<QOrganizerItem> &result,
                                                QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);

    static void updateItemRemoveRequest(QOrganizerItemRemoveRequest *request, QOrganizerManager::Error error,
                                        const QMap<int, QOrganizerManager::Error> &errorMap, QOrganizerAbstractRequest::State newState);

    static void updateItemSaveRequest(QOrganizerItemSaveRequest *request, const QList<QOrganizerItem> &result, QOrganizerManager::Error error,
                                      const QMap<int, QOrganizerManager::Error> &errorMap, QOrganizerAbstractRequest::State newState);

    static void updateCollectionFetchRequest(QOrganizerCollectionFetchRequest *request, const QList<QOrganizerCollection> &result,
                                             QOrganizerManager::Error error, QOrganizerAbstractRequest::State newState);

    static void updateCollectionRemoveRequest(QOrganizerCollectionRemoveRequest *request, QOrganizerManager::Error error,
                                              const QMap<int, QOrganizerManager::Error> &errorMap, QOrganizerAbstractRequest::State newState);

    static void updateCollectionSaveRequest(QOrganizerCollectionSaveRequest *request, const QList<QOrganizerCollection> &result,
                                            QOrganizerManager::Error error, const QMap<int, QOrganizerManager::Error> &errorMap,
                                            QOrganizerAbstractRequest::State newState);

    // functionality reporting
    virtual QList<QOrganizerItemFilter::FilterType> supportedFilters() const;
    virtual QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const;
    virtual QList<QOrganizerItemType::ItemType> supportedItemTypes() const;

    // helper
    static const QOrganizerItemEngineId *engineItemId(const QOrganizerItemId &itemId);
    static const QOrganizerCollectionEngineId *engineCollectionId(const QOrganizerCollectionId &collectionId);

    static int addSorted(QList<QOrganizerItem> *sorted, const QOrganizerItem &toAdd, const QList<QOrganizerItemSortOrder> &sortOrders);
    static int compareItem(const QOrganizerItem &a, const QOrganizerItem &b, const QList<QOrganizerItemSortOrder> &sortOrders);
    static int compareVariant(const QVariant &a, const QVariant &b, Qt::CaseSensitivity sensitivity);
    static bool isItemBetweenDates(const QOrganizerItem &item, const QDateTime &startPeriod, const QDateTime &endPeriod);
    static bool itemLessThan(const QOrganizerItem &a, const QOrganizerItem &b);
    static bool testFilter(const QOrganizerItemFilter &filter, const QOrganizerItem &item);
    static QOrganizerItemFilter canonicalizedFilter(const QOrganizerItemFilter &filter);

Q_SIGNALS:
    void dataChanged();
    void itemsAdded(const QList<QOrganizerItemId> &itemIds);
    void itemsChanged(const QList<QOrganizerItemId> &itemIds);
    void itemsRemoved(const QList<QOrganizerItemId> &itemIds);
    void collectionsAdded(const QList<QOrganizerCollectionId> &collectionIds);
    void collectionsChanged(const QList<QOrganizerCollectionId> &collectionIds);
    void collectionsRemoved(const QList<QOrganizerCollectionId> &collectionIds);

private:
    Q_DISABLE_COPY(QOrganizerManagerEngine)
    friend class QOrganizerItemChangeSet;
    friend class QOrganizerCollectionChangeSet;
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERMANAGERENGINE_H
