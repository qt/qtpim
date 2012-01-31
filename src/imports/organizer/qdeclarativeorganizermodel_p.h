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

#ifndef QDECLARATIVEORGANIZERMODEL_H
#define QDECLARATIVEORGANIZERMODEL_H

#include <QAbstractListModel>
#include <QDeclarativeListProperty>
#include <QDeclarativeParserStatus>

#include <qorganizeritem.h>
#include "qdeclarativeorganizeritem_p.h"
#include <qversitreader.h>
#include <qversitwriter.h>
#include <qorganizercollectionid.h>
#include "qdeclarativeorganizeritemfetchhint_p.h"
#include "qdeclarativeorganizeritemsortorder_p.h"
#include "qdeclarativeorganizeritemfilter_p.h"
#include "qdeclarativeorganizercollection_p.h"

QTVERSIT_BEGIN_NAMESPACE
class QVersitReader;
class QVersitWriter;
QTVERSIT_END_NAMESPACE

QTVERSIT_USE_NAMESPACE

QTORGANIZER_BEGIN_NAMESPACE

class QDeclarativeOrganizerModelPrivate;
class QDeclarativeOrganizerModel : public QAbstractListModel, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QString manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(QString managerName READ managerName  NOTIFY managerChanged)
    Q_PROPERTY(QStringList availableManagers READ availableManagers)
    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(QDateTime startPeriod READ startPeriod WRITE setStartPeriod NOTIFY startPeriodChanged)
    Q_PROPERTY(QDateTime endPeriod READ endPeriod WRITE setEndPeriod NOTIFY endPeriodChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemFilter* filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemFetchHint* fetchHint READ fetchHint WRITE setFetchHint NOTIFY fetchHintChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerItemSortOrder> sortOrders READ sortOrders NOTIFY sortOrdersChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerItem> items READ items NOTIFY modelChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerCollection> collections READ collections NOTIFY collectionsChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY modelChanged)
    Q_INTERFACES(QDeclarativeParserStatus)
public:
    enum {
        OrganizerItemRole = Qt::UserRole + 500
    };


    explicit QDeclarativeOrganizerModel(QObject *parent = 0);
    explicit QDeclarativeOrganizerModel(QOrganizerManager* manager, const QDateTime& start, const QDateTime& end, QObject *parent = 0);

    QString error() const;
    int itemCount() const;

    QString manager() const;
    void setManager(const QString& managerUri);
    QString managerName() const;
    QStringList availableManagers() const;
    QDateTime startPeriod() const;
    void setStartPeriod(const QDateTime& start);

    QDateTime endPeriod() const;
    void setEndPeriod(const QDateTime& end);

    // From QDeclarativeParserStatus
    virtual void classBegin() {}
    virtual void componentComplete();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QDeclarativeOrganizerItemFilter* filter() const;

    QDeclarativeOrganizerItemFetchHint* fetchHint() const;


    QDeclarativeListProperty<QDeclarativeOrganizerItem> items() ;
    QDeclarativeListProperty<QDeclarativeOrganizerItemSortOrder> sortOrders() ;
    QDeclarativeListProperty<QDeclarativeOrganizerCollection> collections();

    Q_INVOKABLE void removeItem(const QString& id);
    Q_INVOKABLE void removeItems(const QStringList& ids);
    Q_INVOKABLE void saveItem(QDeclarativeOrganizerItem* item);
    Q_INVOKABLE int fetchItems(const QStringList &itemIds);
    Q_INVOKABLE void removeCollection(const QString& collectionId);
    Q_INVOKABLE void saveCollection(QDeclarativeOrganizerCollection* collection);
    Q_INVOKABLE void fetchCollections();

    Q_INVOKABLE QList<bool> containsItems(const QDateTime &start, const QDateTime &end, int interval);
    Q_INVOKABLE bool containsItems(const QDateTime &start, const QDateTime &end = QDateTime());
    Q_INVOKABLE QVariantList itemsByTimePeriod(const QDateTime &start = QDateTime(), const QDateTime &end = QDateTime());
    Q_INVOKABLE QDeclarativeOrganizerItem* item(const QString& id);
    Q_INVOKABLE QStringList itemIds(const QDateTime &start = QDateTime(), const QDateTime &end = QDateTime());
    Q_INVOKABLE QDeclarativeOrganizerCollection* defaultCollection();
    Q_INVOKABLE QDeclarativeOrganizerCollection* collection(const QString& collectionId);
    bool autoUpdate() const;
    void setAutoUpdate(bool autoUpdate);

    void setFilter(QDeclarativeOrganizerItemFilter* filter);
    void setFetchHint(QDeclarativeOrganizerItemFetchHint* fetchHint);


signals:
    void managerChanged();
    void availableManagersChanged();
    void filterChanged();
    void fetchHintChanged();
    void modelChanged();
    void sortOrdersChanged();
    void errorChanged();
    void startPeriodChanged();
    void endPeriodChanged();
    void autoUpdateChanged();
    void collectionsChanged();
    void itemsFetched(int requestId, const QVariantList &fetchedItems);

public slots:
    void update();
    void cancelUpdate();
    void exportItems(const QUrl& url, const QStringList& profiles=QStringList());
    void importItems(const QUrl& url, const QStringList& profiles=QStringList());

private slots:
    void fetchAgain();
    void requestUpdated();
    void doUpdate();

    // handle request from saveItem(), removeItem(), saveCollection(), and removeCollection()
    void onRequestStateChanged(QOrganizerAbstractRequest::State newState);

    // handle signals from organizer manager
    void onItemsAdded(const QList<QOrganizerItemId> &itemIds);
    void onItemsChanged(const QList<QOrganizerItemId> &itemIds);
    void onItemsRemoved(const QList<QOrganizerItemId> &itemIds);

    void startImport(QVersitReader::State state);
    void itemsExported(QVersitWriter::State state);

    void collectionsFetched();

    // handle fetch request from onItemsAdded()
    void onItemsAddedFetchRequestStateChanged(QOrganizerAbstractRequest::State state);

    // handle fetch request from onItemsChanged()
    void onItemsChangedFetchRequestStateChanged(QOrganizerAbstractRequest::State state);

    // handle fetch request from fetchItems()
    void onFetchItemsRequestStateChanged(QOrganizerAbstractRequest::State state);

private:
    void clearItems();
    int itemIndex(const QDeclarativeOrganizerItem* item);
    void addSorted(QDeclarativeOrganizerItem* item);
    void removeItemsFromModel(const QList<QString>& ids);
    bool itemHasRecurrence(const QOrganizerItem& oi) const;
    void fetchOccurrences(const QOrganizerItem& item);
    QDeclarativeOrganizerItem* createItem(const QOrganizerItem& item);
    void checkError(const QOrganizerAbstractRequest *request);

    static void item_append(QDeclarativeListProperty<QDeclarativeOrganizerItem> *p, QDeclarativeOrganizerItem *item);
    static int  item_count(QDeclarativeListProperty<QDeclarativeOrganizerItem> *p);
    static QDeclarativeOrganizerItem * item_at(QDeclarativeListProperty<QDeclarativeOrganizerItem> *p, int idx);

    static void sortOrder_append(QDeclarativeListProperty<QDeclarativeOrganizerItemSortOrder> *p, QDeclarativeOrganizerItemSortOrder *sortOrder);
    static int  sortOrder_count(QDeclarativeListProperty<QDeclarativeOrganizerItemSortOrder> *p);
    static QDeclarativeOrganizerItemSortOrder * sortOrder_at(QDeclarativeListProperty<QDeclarativeOrganizerItemSortOrder> *p, int idx);
    static void  sortOrder_clear(QDeclarativeListProperty<QDeclarativeOrganizerItemSortOrder> *p);

    static void collection_append(QDeclarativeListProperty<QDeclarativeOrganizerCollection> *p, QDeclarativeOrganizerCollection *collection);
    static int collection_count(QDeclarativeListProperty<QDeclarativeOrganizerCollection> *p);
    static QDeclarativeOrganizerCollection* collection_at(QDeclarativeListProperty<QDeclarativeOrganizerCollection> *p, int idx);

    QDeclarativeOrganizerModelPrivate* d;
};

QTORGANIZER_END_NAMESPACE

#endif // QDECLARATIVEORGANIZERMODEL_H
