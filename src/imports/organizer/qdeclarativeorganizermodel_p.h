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
#include <QQmlListProperty>
#include <QQmlParserStatus>

#include <qorganizeritem.h>
#include <qorganizerabstractrequest.h>
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
class QDeclarativeOrganizerModel : public QAbstractListModel, public QQmlParserStatus
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
    Q_PROPERTY(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> sortOrders READ sortOrders NOTIFY sortOrdersChanged)
    Q_PROPERTY(QQmlListProperty<QDeclarativeOrganizerItem> items READ items NOTIFY modelChanged)
    Q_PROPERTY(QQmlListProperty<QDeclarativeOrganizerCollection> collections READ collections NOTIFY collectionsChanged)
    Q_PROPERTY(int storageLocations READ storageLocations WRITE setStorageLocations NOTIFY storageLocationsChanged)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY modelChanged)
    Q_ENUMS(ExportError)
    Q_ENUMS(ImportError)
    Q_ENUMS(StorageLocation)
    Q_INTERFACES(QQmlParserStatus)
public:
    enum {
        OrganizerItemRole = Qt::UserRole + 500
    };

    enum ExportError {
        ExportNoError          = QVersitWriter::NoError,
        ExportUnspecifiedError = QVersitWriter::UnspecifiedError,
        ExportIOError          = QVersitWriter::IOError,
        ExportOutOfMemoryError = QVersitWriter::OutOfMemoryError,
        ExportNotReadyError    = QVersitWriter::NotReadyError
    };

    enum ImportError {
        ImportNoError          = QVersitReader::NoError,
        ImportUnspecifiedError = QVersitReader::UnspecifiedError,
        ImportIOError          = QVersitReader::IOError,
        ImportOutOfMemoryError = QVersitReader::OutOfMemoryError,
        ImportNotReadyError    = QVersitReader::NotReadyError,
        ImportParseError       = QVersitReader::ParseError
    };

    explicit QDeclarativeOrganizerModel(QObject *parent = 0);
    explicit QDeclarativeOrganizerModel(QOrganizerManager* manager, const QDateTime& start, const QDateTime& end, QObject *parent = 0);
    ~QDeclarativeOrganizerModel();

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

    enum StorageLocation {
        UserDataStorage = QOrganizerAbstractRequest::UserDataStorage,
        SystemStorage = QOrganizerAbstractRequest::SystemStorage
    };

    int storageLocations() const;
    void setStorageLocations(int storageLocationsFlag);

    // From QQmlParserStatus
    virtual void classBegin() {}
    virtual void componentComplete();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QDeclarativeOrganizerItemFilter* filter() const;

    QDeclarativeOrganizerItemFetchHint* fetchHint() const;


    QQmlListProperty<QDeclarativeOrganizerItem> items() ;
    QQmlListProperty<QDeclarativeOrganizerItemSortOrder> sortOrders() ;
    QQmlListProperty<QDeclarativeOrganizerCollection> collections();

    Q_INVOKABLE void removeItem(const QString& id);
    Q_INVOKABLE void removeItem(QDeclarativeOrganizerItem *item);
    Q_INVOKABLE void removeItems(const QStringList& ids);
    Q_INVOKABLE void removeItems(const QList<QDeclarativeOrganizerItem> &items);
    Q_INVOKABLE void saveItem(QDeclarativeOrganizerItem* item,
                              QDeclarativeOrganizerModel::StorageLocation storageLocation = UserDataStorage);
    Q_INVOKABLE int fetchItems(const QStringList &itemIds);
    Q_INVOKABLE int fetchItems(const QDateTime &start, const QDateTime &end,
                               QDeclarativeOrganizerItemFilter *filter = new QDeclarativeOrganizerItemFilter(),
                               int maxCount = -1,
                               const QVariantList &sortOrders = QVariantList(),
                               QDeclarativeOrganizerItemFetchHint *fetchHint = new QDeclarativeOrganizerItemFetchHint(),
                               QDeclarativeOrganizerModel::StorageLocation storageLocation = UserDataStorage);
    Q_INVOKABLE void removeCollection(const QString& collectionId);
    Q_INVOKABLE void saveCollection(QDeclarativeOrganizerCollection* collection,
                         QDeclarativeOrganizerModel::StorageLocation storageLocation = UserDataStorage);
    // FIXME : Naming indicates fetch from database
    Q_INVOKABLE void fetchCollections();

    Q_INVOKABLE QList<bool> containsItems(const QDateTime &start, const QDateTime &end, int interval);
    Q_INVOKABLE bool containsItems(const QDateTime &start, const QDateTime &end = QDateTime());
    Q_INVOKABLE QVariantList itemsByTimePeriod(const QDateTime &start = QDateTime(), const QDateTime &end = QDateTime());
    Q_INVOKABLE QDeclarativeOrganizerItem* item(const QString& id);
    Q_INVOKABLE QStringList itemIds(const QDateTime &start = QDateTime(), const QDateTime &end = QDateTime());
    Q_INVOKABLE QDeclarativeOrganizerCollection* defaultCollection();
    Q_INVOKABLE QDeclarativeOrganizerCollection* collection(const QString& collectionId);

    Q_INVOKABLE void importItems(const QUrl& url, const QStringList& profiles = QStringList());
    Q_INVOKABLE void exportItems(const QUrl& url, const QStringList& profiles = QStringList());

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
    void exportCompleted(ExportError error, QUrl url);
    void importCompleted(ImportError error, QUrl url);
    void storageLocationsChanged();

public slots:
    void update();
    void updateItems();
    void updateCollections();
    void cancelUpdate();

private slots:
    void doUpdate();
    void doUpdateItems();
    void fetchAgain();
    void requestUpdated();

    // handle request from saveItem(), removeItem(), saveCollection(), and removeCollection()
    void onRequestStateChanged(QOrganizerAbstractRequest::State newState);

    // handle fetch request from fetchItems()
    void onFetchItemsRequestStateChanged(QOrganizerAbstractRequest::State state);

    // handle signals from organizer manager
    void onItemsModified(const QList<QPair<QOrganizerItemId, QOrganizerManager::Operation> > &itemIds);

    // handle fetch request from onItemsModified()
    void onItemsModifiedFetchRequestStateChanged(QOrganizerAbstractRequest::State state);

    void collectionsFetched();

    void startImport(QVersitReader::State state);
    void itemsExported(QVersitWriter::State state);


private:
    void removeItemsFromModel(const QList<QString>& ids);
    bool itemHasRecurrence(const QOrganizerItem& oi) const;
    QDeclarativeOrganizerItem* createItem(const QOrganizerItem& item);
    void checkError(const QOrganizerAbstractRequest *request);

    static void item_append(QQmlListProperty<QDeclarativeOrganizerItem> *p, QDeclarativeOrganizerItem *item);
    static int  item_count(QQmlListProperty<QDeclarativeOrganizerItem> *p);
    static QDeclarativeOrganizerItem * item_at(QQmlListProperty<QDeclarativeOrganizerItem> *p, int idx);

    static void sortOrder_append(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p, QDeclarativeOrganizerItemSortOrder *sortOrder);
    static int  sortOrder_count(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p);
    static QDeclarativeOrganizerItemSortOrder * sortOrder_at(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p, int idx);
    static void  sortOrder_clear(QQmlListProperty<QDeclarativeOrganizerItemSortOrder> *p);

    static void collection_append(QQmlListProperty<QDeclarativeOrganizerCollection> *p, QDeclarativeOrganizerCollection *collection);
    static int collection_count(QQmlListProperty<QDeclarativeOrganizerCollection> *p);
    static QDeclarativeOrganizerCollection* collection_at(QQmlListProperty<QDeclarativeOrganizerCollection> *p, int idx);

    QScopedPointer<QDeclarativeOrganizerModelPrivate> d_ptr;
    Q_DECLARE_PRIVATE(QDeclarativeOrganizerModel)
};

QTORGANIZER_END_NAMESPACE

#endif // QDECLARATIVEORGANIZERMODEL_H
