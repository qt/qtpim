/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERJSONDBENGINE_H
#define QORGANIZERJSONDBENGINE_H

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

#include "qorganizeritem.h"
#include "qorganizermanager.h"
#include "qorganizermanagerengine.h"
#include "qorganizermanagerenginefactory.h"
#include "qorganizeritemdetaildefinition.h"
#include "qorganizerabstractrequest.h"
#include "qorganizeritemchangeset.h"
#include "qorganizeritemengineid.h"
#include "qorganizercollectionengineid.h"
#include "qorganizerjsondbrequestthread.h"


QTPIM_USE_NAMESPACE


class QOrganizerJsonDbEngineData : public QSharedData
{
public:
    QOrganizerJsonDbEngineData()
        : QSharedData()
        , m_requestHandlerThread(0)
    {

    }

    QOrganizerJsonDbEngineData(const QOrganizerJsonDbEngineData& other)
        : QSharedData(other)
    {
        //TODO!!!
    }

    ~QOrganizerJsonDbEngineData()
    {
        if (m_requestHandlerThread)
            delete m_requestHandlerThread;
        m_requestHandlerThread = 0;
    }

    QOrganizerJsonDbRequestThread* m_requestHandlerThread;
};

class QOrganizerJsonDbEngine : public QOrganizerManagerEngine
{
    Q_OBJECT

public:
    static QOrganizerJsonDbEngine *createJsonDbEngine(QOrganizerManager::Error* error);

    ~QOrganizerJsonDbEngine();

    /* URI reporting */
    QString managerName() const;
    QMap<QString, QString> managerParameters() const;
    int managerVersion() const;

    QList<QOrganizerItem> itemOccurrences(const QOrganizerItem& parentItem, const QDateTime& periodStart, const QDateTime& periodEnd, int maxCount, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const;
    QList<QOrganizerItemId> itemIds(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, QOrganizerManager::Error* error) const;
    QList<QOrganizerItem> items(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const;
    QList<QOrganizerItem> itemsForExport(const QDateTime& startDate, const QDateTime& endDate, const QOrganizerItemFilter& filter, const QList<QOrganizerItemSortOrder>& sortOrders, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const;
    QOrganizerItem item(const QOrganizerItemId& itemId, const QOrganizerItemFetchHint& fetchHint, QOrganizerManager::Error* error) const;

    bool saveItems(QList<QOrganizerItem>* items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);
    bool removeItems(const QList<QOrganizerItemId>& itemIds, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error);

    /* Definitions - Accessors and Mutators */
    QMap<QString, QOrganizerItemDetailDefinition> detailDefinitions(const QString& itemType, QOrganizerManager::Error* error) const;
    QOrganizerItemDetailDefinition detailDefinition(const QString& definitionId, const QString& itemType, QOrganizerManager::Error* error) const;
    bool saveDetailDefinition(const QOrganizerItemDetailDefinition& def, const QString& itemType, QOrganizerManager::Error* error);
    bool removeDetailDefinition(const QString& definitionId, const QString& itemType, QOrganizerManager::Error* error);

    /* Collections - every item belongs to exactly one collection */
    QOrganizerCollection defaultCollection(QOrganizerManager::Error* error) const;
    QOrganizerCollection collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error) const;
    QList<QOrganizerCollection> collections(QOrganizerManager::Error* error) const;
    bool saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error);
    bool removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error);

    /* Capabilities reporting */
    bool hasFeature(QOrganizerManager::ManagerFeature feature, const QString& itemType) const;
    bool isFilterSupported(const QOrganizerItemFilter& filter) const;
    QList<int> supportedDataTypes() const;
    QStringList supportedItemTypes() const;

    /* Asynchronous Request Support */
    void requestDestroyed(QOrganizerAbstractRequest* req);
    bool startRequest(QOrganizerAbstractRequest* req);
    bool cancelRequest(QOrganizerAbstractRequest* req);
    bool waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs);

signals:
    void requestReceived(QOrganizerAbstractRequest* req);

private:
    QOrganizerJsonDbEngine();
    bool initEngine(QOrganizerManager::Error* error);
    /*Initialize the default collection*/
    void initCollection(QOrganizerManager::Error* error);

    QOrganizerJsonDbEngineData* d;

    friend class QOrganizerJsonDbEngineFactory;
};

#endif

