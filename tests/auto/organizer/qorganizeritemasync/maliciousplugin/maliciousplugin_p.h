/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#ifndef QORGANIZERMALICIOUSPLUGIN_P_H
#define QORGANIZERMALICIOUSPLUGIN_P_H

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

#include <QObject>

#include <QtOrganizer/qorganizermanager.h>
#include <QtOrganizer/qorganizermanagerengine.h>
#include <QtOrganizer/qorganizermanagerenginefactory.h>

QT_FORWARD_DECLARE_CLASS(QThread)

QTORGANIZER_USE_NAMESPACE

class MaliciousThreadObject;
class MaliciousAsyncManagerEngine : public QOrganizerManagerEngine
{
    Q_OBJECT

public:
    MaliciousAsyncManagerEngine();
    ~MaliciousAsyncManagerEngine();

    QString managerName() const;
    bool startRequest(QOrganizerAbstractRequest *req);
    bool cancelRequest(QOrganizerAbstractRequest *req);

    QMap<QString, QString> managerParameters() const { return QMap<QString, QString>(); }
    int managerVersion() const { return 0; }

    // items
    QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                           QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::items(itemIds, fetchHint, errorMap, error);
    }

    QList<QOrganizerItem> items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                           const QDateTime &endDateTime, int maxCount,
                                           const QList<QOrganizerItemSortOrder> &sortOrders,
                                           const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::items(filter, startDateTime, endDateTime, maxCount, sortOrders, fetchHint, error);
    }

    QList<QOrganizerItemId> itemIds(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                               const QDateTime &endDateTime, const QList<QOrganizerItemSortOrder> &sortOrders,
                                               QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::itemIds(filter, startDateTime, endDateTime, sortOrders, error);
    }

    QList<QOrganizerItem> itemOccurrences(const QOrganizerItem &parentItem, const QDateTime &startDateTime,
                                                     const QDateTime &endDateTime, int maxCount,
                                                     const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::itemOccurrences(parentItem, startDateTime, endDateTime, maxCount, fetchHint, error);
    }

    QList<QOrganizerItem> itemsForExport(const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                    const QOrganizerItemFilter &filter,
                                                    const QList<QOrganizerItemSortOrder> &sortOrders,
                                                    const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::itemsForExport(startDateTime, endDateTime, filter, sortOrders, fetchHint, error);
    }

    bool saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                              QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::saveItems(items, detailMask, errorMap, error);
    }

    bool removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::removeItems(itemIds, errorMap, error);
    }

    bool removeItems(const QList<QOrganizerItem> *items, QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::removeItems(items, errorMap, error);
    }

    // collections
    QOrganizerCollectionId defaultCollectionId() const
    {
        return QOrganizerManagerEngine::defaultCollectionId();
    }

    QOrganizerCollection collection(const QOrganizerCollectionId &collectionId, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::collection(collectionId, error);
    }

    QList<QOrganizerCollection> collections(QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::collections(error);
    }

    bool saveCollection(QOrganizerCollection *collection, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::saveCollection(collection, error);
    }

    bool removeCollection(const QOrganizerCollectionId &collectionId, QOrganizerManager::Error *error)
    {
        return QOrganizerManagerEngine::removeCollection(collectionId, error);
    }

    /* Asynchronous Request Support */
    void requestDestroyed(QOrganizerAbstractRequest *req);
    bool waitForRequestFinished(QOrganizerAbstractRequest *req, int msecs) { return QOrganizerManagerEngine::waitForRequestFinished(req, msecs); }

    QList<QOrganizerItemFilter::FilterType> supportedFilters() const
    {
        return QOrganizerManagerEngine::supportedFilters();
    }

    QList<QOrganizerItemType::ItemType> supportedItemTypes() const
    {
        return QOrganizerManagerEngine::supportedItemTypes();
    }
    QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const
    {
        return QOrganizerManagerEngine::supportedItemDetails(itemType);
    }

signals:
    void doStartRequest(QOrganizerAbstractRequest *req);
    void doFinishRequest(QOrganizerAbstractRequest *req);

private:
    QThread *thread;
    MaliciousThreadObject *threadObject;
};


class MaliciousEngineFactory : public QOrganizerManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QOrganizerManagerEngineFactoryInterface" FILE "malicious.json")

public:
    QString managerName() const;
    QOrganizerManagerEngine *engine(const QMap<QString, QString> &parameters, QOrganizerManager::Error *error);

private:
    MaliciousAsyncManagerEngine mame;
};

#endif // QORGANIZERMALICIOUSPLUGIN_P_H
