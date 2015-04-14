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

#ifndef QORGANIZERSKELETON_P_H
#define QORGANIZERSKELETON_P_H

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

#include <QtCore/qdatetime.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qobject.h>
#include <QtCore/qpair.h>
#include <QtCore/qqueue.h>
#include <QtCore/qset.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

#include <QtOrganizer/qorganizeritem.h>
#include <QtOrganizer/qorganizermanager.h>
#include <QtOrganizer/qorganizermanagerengine.h>
#include <QtOrganizer/qorganizermanagerenginefactory.h>
#include <QtOrganizer/qorganizerabstractrequest.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemSkeletonFactory : public QOrganizerManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QOrganizerManagerEngineFactoryInterface" FILE "skeleton.json")

public:
    QOrganizerManagerEngine* engine(const QMap<QString, QString>& parameters, QOrganizerManager::Error*);
    QString managerName() const;
};


class QOrganizerItemSkeletonEngineData : public QSharedData
{
public:
    QOrganizerItemSkeletonEngineData()
        : QSharedData()
    {
    }

    QOrganizerItemSkeletonEngineData(const QOrganizerItemSkeletonEngineData& other)
        : QSharedData(other)
    {
    }

    ~QOrganizerItemSkeletonEngineData()
    {
    }
};

class QOrganizerItemSkeletonEngine : public QOrganizerManagerEngine
{
    Q_OBJECT

public:
    static QOrganizerItemSkeletonEngine *createSkeletonEngine(const QMap<QString, QString>& parameters);

    ~QOrganizerItemSkeletonEngine();

    /* URI reporting */
    QString managerName() const;
    QMap<QString, QString> managerParameters() const;

    // items
    QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error* error);

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

    // collections
    QOrganizerCollectionId defaultCollectionId() const;
    QOrganizerCollection collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);
    QList<QOrganizerCollection> collections(QOrganizerManager::Error* error);
    bool saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error);
    bool removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);

    /* Capabilities reporting */
    QList<QOrganizerItemFilter::FilterType> supportedFilters() const;
    QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const;
    QList<QOrganizerItemType::ItemType> supportedItemTypes() const;

    /* Asynchronous Request Support */
    void requestDestroyed(QOrganizerAbstractRequest* req);
    bool startRequest(QOrganizerAbstractRequest* req);
    bool cancelRequest(QOrganizerAbstractRequest* req);
    bool waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs);

private:
    QOrganizerItemSkeletonEngineData* d;

    friend class QOrganizerItemSkeletonFactory;
};

QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERSKELETON_P_H
