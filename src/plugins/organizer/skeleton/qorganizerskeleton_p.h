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

#include <QSharedData>
#include <QMap>
#include <QMultiMap>
#include <QList>
#include <QQueue>
#include <QPair>
#include <QSet>
#include <QDateTime>
#include <QString>
#include <QObject>

#include <qorganizeritem.h>
#include <qorganizermanager.h>
#include <qorganizermanagerengine.h>
#include <qorganizermanagerenginefactory.h>
#include <qorganizerabstractrequest.h>
#include <qorganizeritemengineid.h>
#include <qorganizercollectionengineid.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemSkeletonFactory : public QOrganizerManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QOrganizerManagerEngineFactoryInterface" FILE "skeleton.json")

public:
    QOrganizerManagerEngine* engine(const QMap<QString, QString>& parameters, QOrganizerManager::Error*);
    QOrganizerItemEngineId* createItemEngineId(const QMap<QString, QString>& parameters, const QString& idString) const;
    QOrganizerCollectionEngineId* createCollectionEngineId(const QMap<QString, QString>& parameters, const QString& idString) const;
    QString managerName() const;
};

class QOrganizerCollectionSkeletonEngineId : public QOrganizerCollectionEngineId
{
public:
    QOrganizerCollectionSkeletonEngineId();
    QOrganizerCollectionSkeletonEngineId(quint32 collectionId);
    ~QOrganizerCollectionSkeletonEngineId();
    QOrganizerCollectionSkeletonEngineId(const QOrganizerCollectionSkeletonEngineId& other);

    bool isEqualTo(const QOrganizerCollectionEngineId* other) const;
    bool isLessThan(const QOrganizerCollectionEngineId* other) const;

    QString managerUri() const;
    QOrganizerCollectionEngineId* clone() const;

    QString toString() const;

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const;
#endif
    uint hash() const;

    // data members:
    // Your backend can use whatever it likes as an id internally.
    // In this example, we use just a single quint32, but you can
    // use any datatype you need to (filename string, etc).
    quint32 m_collectionId;
};

class QOrganizerItemSkeletonEngineId : public QOrganizerItemEngineId
{
public:
    QOrganizerItemSkeletonEngineId();
    QOrganizerItemSkeletonEngineId(quint32 itemId);
    ~QOrganizerItemSkeletonEngineId();
    QOrganizerItemSkeletonEngineId(const QOrganizerItemSkeletonEngineId& other);

    bool isEqualTo(const QOrganizerItemEngineId* other) const;
    bool isLessThan(const QOrganizerItemEngineId* other) const;

    QString managerUri() const;
    QOrganizerItemEngineId* clone() const;

    QString toString() const;

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const;
#endif
    uint hash() const;

    // data members:
    // Your backend can use whatever it likes as an id internally.
    // In this example, we use just a single quint32, but you can
    // use a pair of ints (one for collectionId, one for itemId)
    // or any other information (uuid string, etc).
    quint32 m_itemId;
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
    int managerVersion() const;

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
    QOrganizerCollection defaultCollection(QOrganizerManager::Error* error);
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

QTORGANIZER_END_NAMESPACE

#endif
