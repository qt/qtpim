/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtTest/QtTest>

#include <qorganizeritems.h>
#include <qorganizermanager.h>
#include <qorganizeritemrequests.h>
#include "../../../jsondbprocess.h"

QTORGANIZER_USE_NAMESPACE

struct ManagerExistingStorageLocationDetails {
    int m_collectionAmountOnUserData;
    int m_collectionAmountOnSystem;
    int m_itemAmountOnUserData;
    int m_itemAmountOnSystem;
};

class tst_QOrganizerStorageLocations : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStorageLocationsForCollections_data() { addManager(); }
    void testStorageLocationsForCollections();

    void testStorageLocationsForItems_data() { addManager(); }
    void testStorageLocationsForItems();

private:
    void addManager();
    QList<QOrganizerCollection> collectionsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations);
    QList<QOrganizerItem> itemsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations);

    QMap<QString, ManagerExistingStorageLocationDetails> m_managerStorageLocationDetails;
    JsonDbProcess m_jsondbProcess;
};

void tst_QOrganizerStorageLocations::addManager()
{
    QTest::addColumn<QString>("managerName");

    QStringList availableManagers = QOrganizerManager::availableManagers();
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    foreach (const QString &manager, availableManagers)
        QTest::newRow(manager.toLatin1().constData()) << manager;
}

QList<QOrganizerCollection> tst_QOrganizerStorageLocations::collectionsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations)
{
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    cfr.setStorageLocations(storageLocations);
    cfr.start();
    cfr.waitForFinished();
    cfr.isFinished();
    return cfr.collections();
}

QList<QOrganizerItem> tst_QOrganizerStorageLocations::itemsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations)
{
    QOrganizerItemFetchRequest ifr;
    ifr.setManager(&organizerManager);
    ifr.setStorageLocations(storageLocations);
    ifr.start();
    ifr.waitForFinished();
    ifr.isFinished();
    return ifr.items();
}

void tst_QOrganizerStorageLocations::initTestCase()
{
    // Start JsonDb daemon if needed
    if (QOrganizerManager::availableManagers().contains("jsondb")) {
        QString partitions_json = QFINDTESTDATA("partitions.json");
        QVERIFY2(!partitions_json.isEmpty(), "partitions.json file is missing");
        QVERIFY2(m_jsondbProcess.start(partitions_json), "Failed to start JsonDb process");
    }

    QStringList availableManagers(QOrganizerManager::availableManagers());
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    // gather information from storagelocations
    foreach (const QString &manager, availableManagers) {
        QOrganizerManager organizerManager(manager);
        ManagerExistingStorageLocationDetails details;
        details.m_collectionAmountOnUserData = collectionsFromStorageLocation(organizerManager, QOrganizerAbstractRequest::UserDataStorage).count();
        details.m_collectionAmountOnSystem = collectionsFromStorageLocation(organizerManager, QOrganizerAbstractRequest::SystemStorage).count();
        details.m_itemAmountOnUserData = itemsFromStorageLocation(organizerManager, QOrganizerAbstractRequest::UserDataStorage).count();
        details.m_itemAmountOnSystem = itemsFromStorageLocation(organizerManager, QOrganizerAbstractRequest::SystemStorage).count();
        m_managerStorageLocationDetails.insert(manager, details);
    }
}

void tst_QOrganizerStorageLocations::cleanupTestCase()
{
    // Kill jsondb-process, if started one
    if (QOrganizerManager::availableManagers().contains("jsondb"))
        m_jsondbProcess.terminate();

    m_managerStorageLocationDetails.clear();
}

void tst_QOrganizerStorageLocations::testStorageLocationsForCollections()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QOrganizerManager organizerManager(managerName);
    ManagerExistingStorageLocationDetails existingDetails = m_managerStorageLocationDetails.value(managerName);

    QList<QOrganizerCollection> savedCollectionsUserData;
    QList<QOrganizerCollection> savedCollectionsSystemData;

    // Save 2 collections on default storagelocation
    // - storagelocation not set, should go to UserDataStorage
    QOrganizerCollectionSaveRequest csr;
    csr.setManager(&organizerManager);
    QOrganizerCollection testCollection;
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection on default");
    csr.setCollection(testCollection);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    savedCollectionsUserData<<csr.collections();
    // - storagelocation explicitly set to UserDataStorage
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection on UserDataStorage");
    csr.setCollection(testCollection);
    csr.setStorageLocation(QOrganizerAbstractRequest::UserDataStorage);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    savedCollectionsUserData<<csr.collections();
    // - fetch from default storageLocation, should find both above (+ default collection)
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(savedCollectionsUserData.count(), cfr.collections().count()-existingDetails.m_collectionAmountOnUserData);

    // Save 1 collection to different storageLocation
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection on SystemStorage");
    csr.setCollection(testCollection);
    csr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    // - should not be visible with fetch from default storageLocation
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(savedCollectionsUserData.count(), cfr.collections().count()-existingDetails.m_collectionAmountOnUserData);// should be the same as in last fetch
    // - should be visible (1 and only 1) if fetching only from SystemStorage
    cfr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(1, cfr.collections().count()-existingDetails.m_collectionAmountOnSystem);

    // Modify collection and resave
    testCollection = cfr.collections().at(0);
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection saved again on SystemStorage");
    csr.setCollection(testCollection);
    csr.setStorageLocation(QOrganizerAbstractRequest::UserDataStorage);//this should be ignored when saving already saved collection
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    savedCollectionsSystemData<<csr.collections();
    // - should be visible (still 1 and only 1) if fetching only from SystemStorage
    cfr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(1, cfr.collections().count()-existingDetails.m_collectionAmountOnSystem);
    bool exactMatch = false;
    foreach (const QOrganizerCollection &collection, cfr.collections()) {
        if (collection == savedCollectionsSystemData.at(0))// 0 = only one
            exactMatch = true;
    }
    QVERIFY(exactMatch);

    // All the collections should be visible when fetching from multiple storagelocations
    cfr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage | QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(savedCollectionsUserData.count() + savedCollectionsSystemData.count(),
             cfr.collections().count()-existingDetails.m_collectionAmountOnSystem-existingDetails.m_collectionAmountOnUserData);// all

    // Remove collections from multiple storageLocations with one request
    QList<QOrganizerCollectionId> collIdListFromDifferentPartitions;
    foreach (const QOrganizerCollection &collection, savedCollectionsSystemData) {
        collIdListFromDifferentPartitions<<collection.id();
    }
    foreach (const QOrganizerCollection &collection, savedCollectionsUserData) {
        if (collection.id() != organizerManager.defaultCollection().id())
            collIdListFromDifferentPartitions<<collection.id();
    }
    QOrganizerCollectionRemoveRequest crr;
    crr.setManager(&organizerManager);
    crr.setCollectionIds(collIdListFromDifferentPartitions);
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(existingDetails.m_collectionAmountOnSystem+existingDetails.m_collectionAmountOnUserData, cfr.collections().count());
}

void tst_QOrganizerStorageLocations::testStorageLocationsForItems()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QOrganizerManager organizerManager(managerName);
    ManagerExistingStorageLocationDetails existingDetails = m_managerStorageLocationDetails.value(managerName);

    QList<QOrganizerItem> savedItemsUserData;
    QList<QOrganizerItem> savedItemsSystem;

    // Save 1 Event and 1 Todo on default storagelocation
    // - storagelocation not set, should go to UserDataStorage
    QOrganizerItemSaveRequest isr;
    isr.setManager(&organizerManager);
    QOrganizerEvent testEvent;
    testEvent.setDisplayLabel("Event on default storage");
    isr.setItem(testEvent);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QVERIFY(!isr.error());
    savedItemsUserData<<isr.items();
    // - storagelocation explicitly set to UserDataStorage
    QOrganizerTodo testTodo;
    testTodo.setDisplayLabel("Todo on UserData");
    isr.setItem(testTodo);
    isr.setStorageLocation(QOrganizerAbstractRequest::UserDataStorage);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QVERIFY(!isr.error());
    savedItemsUserData<<isr.items();
    // - fetch from default storageLocation, should find both above
    QOrganizerItemFetchRequest ifr;
    ifr.setManager(&organizerManager);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QVERIFY(!ifr.error());
    QCOMPARE(savedItemsUserData.count(), ifr.items().count()-existingDetails.m_itemAmountOnUserData);

    // Save 1 item to different storageLocation
    // - try to save to new event to different storage than the collection is, should fail
    testEvent.setDisplayLabel("Event on SystemStorage");
    isr.setItem(testEvent);
    isr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QCOMPARE(isr.error(), QOrganizerManager::InvalidCollectionError);
    // -for that we need also collection on that storageLocation
    QOrganizerCollectionSaveRequest csr;
    csr.setManager(&organizerManager);
    csr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);
    QOrganizerCollection collectionOnSystem;
    collectionOnSystem.setMetaData(QOrganizerCollection::KeyName, "Collection on System");
    csr.setCollection(collectionOnSystem);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(!csr.error());
    collectionOnSystem = csr.collections().at(0);
    // - now we can save event there
    testEvent.setDisplayLabel("Event on SystemStorage");
    testEvent.setCollectionId(collectionOnSystem.id());
    isr.setItem(testEvent);
    isr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QVERIFY(!isr.error());
    // - should not be visible with fetch from default storageLocation
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QCOMPARE(savedItemsUserData.count(), ifr.items().count()-existingDetails.m_itemAmountOnUserData);// should be the same as in last fetch
    // - should be visible (1 and only 1) if fetching only from SystemStorage
    ifr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QVERIFY(!ifr.error());
    QCOMPARE(ifr.items().count()-existingDetails.m_itemAmountOnSystem, 1);

    // Modify item and resave
    // - resave with collection in different location, should fail
    testEvent = isr.items().at(0);
    testEvent.setDisplayLabel("Event saved again on SystemStorage, with default collection.");
    testEvent.setCollectionId(organizerManager.defaultCollection().id());
    isr.setItem(testEvent);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QCOMPARE(isr.error(), QOrganizerManager::InvalidCollectionError);
    // - resave with correct collection (collection on same storage)
    testEvent.setCollectionId(collectionOnSystem.id());
    testEvent.setDisplayLabel("Event saved again on SystemStorage");
    isr.setItem(testEvent);
    isr.setStorageLocation(QOrganizerAbstractRequest::UserDataStorage);//this should be ignored when saving already saved collection
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QVERIFY(!isr.error());
    savedItemsSystem<<isr.items();
    // - should be visible (still 1 and only 1) if fetching only from SystemStorage
    ifr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QVERIFY(!ifr.error());
    QCOMPARE(1, ifr.items().count()-existingDetails.m_itemAmountOnSystem);
    bool exactMatch = false;
    foreach (const QOrganizerItem &item, ifr.items()) {
        if (item == savedItemsSystem.at(0))// 0 = only one
            exactMatch = true;
    }
    QVERIFY(exactMatch);

    // All the items should be visible when fetching from multiple storagelocations
    // - QOrganizerItemFetchRequest - all locations
    ifr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage | QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QCOMPARE(savedItemsUserData.count() + savedItemsSystem.count(), ifr.items().count()-existingDetails.m_itemAmountOnUserData-existingDetails.m_itemAmountOnSystem);// all

    // Try out other fetch request types
    // QOrganizerItemFetchByIdRequest - uses storagelocations saved on the item ids
    // - savedItemsUserData
    QOrganizerItemFetchByIdRequest ifbidr;
    ifbidr.setManager(&organizerManager);
    QList<QOrganizerItemId> ifbidr_idlist;
    foreach (const QOrganizerItem &item, savedItemsUserData)
        ifbidr_idlist<<item.id();
    ifbidr.setIds(ifbidr_idlist);
    QVERIFY(ifbidr.start());
    QVERIFY(ifbidr.waitForFinished());
    QVERIFY(ifbidr.isFinished());
    QVERIFY(!ifbidr.error());
    QCOMPARE(savedItemsUserData.count(), ifbidr.items().count());
    // - savedItemsUserData & savedItemsSystem
    foreach (const QOrganizerItem &item, savedItemsSystem)
        ifbidr_idlist<<item.id();
    ifbidr.setIds(ifbidr_idlist);
    QVERIFY(ifbidr.start());
    QVERIFY(ifbidr.waitForFinished());
    QVERIFY(ifbidr.isFinished());
    QVERIFY(!ifbidr.error());
    QCOMPARE(savedItemsUserData.count() + savedItemsSystem.count(), ifbidr.items().count());// all
    // QOrganizerItemIdFetchRequest
    // - default storage location
    QOrganizerItemIdFetchRequest iidfr;
    iidfr.setManager(&organizerManager);
    QVERIFY(iidfr.start());
    QVERIFY(iidfr.waitForFinished());
    QVERIFY(iidfr.isFinished());
    QVERIFY(!iidfr.error());
    QCOMPARE(savedItemsUserData.count(), iidfr.itemIds().count()-existingDetails.m_itemAmountOnUserData);
    // QOrganizerItemIdFetchRequest
    // - all locations
    iidfr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage | QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(iidfr.start());
    QVERIFY(iidfr.waitForFinished());
    QVERIFY(iidfr.isFinished());
    QVERIFY(!iidfr.error());
    QCOMPARE(savedItemsUserData.count() + savedItemsSystem.count(), iidfr.itemIds().count()-existingDetails.m_itemAmountOnUserData-existingDetails.m_itemAmountOnSystem);
    // QOrganizerItemFetchForExportRequest
    // - default storage location
    QOrganizerItemFetchForExportRequest iffer;
    iffer.setManager(&organizerManager);
    QVERIFY(iffer.start());
    QVERIFY(iffer.waitForFinished());
    QVERIFY(iffer.isFinished());
    QVERIFY(!iffer.error());
    QCOMPARE(savedItemsUserData.count(), iffer.items().count()-existingDetails.m_itemAmountOnUserData);
    // - all locations
    iffer.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage | QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(iffer.start());
    QVERIFY(iffer.waitForFinished());
    QVERIFY(iffer.isFinished());
    QVERIFY(!iffer.error());
    QCOMPARE(savedItemsUserData.count() + savedItemsSystem.count(), iffer.items().count()-existingDetails.m_itemAmountOnUserData-existingDetails.m_itemAmountOnSystem);

    // Remove items from multiple storageLocations with one request
    QList<QOrganizerItemId> itemIdListFromDifferentPartitions;
    foreach (const QOrganizerItem &item, savedItemsUserData)
        itemIdListFromDifferentPartitions<<item.id();
    foreach (const QOrganizerItem &item, savedItemsSystem)
        itemIdListFromDifferentPartitions<<item.id();

    QOrganizerItemRemoveByIdRequest irbir;
    irbir.setManager(&organizerManager);
    irbir.setItemIds(itemIdListFromDifferentPartitions);
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QCOMPARE(ifr.items().count()-existingDetails.m_itemAmountOnUserData-existingDetails.m_itemAmountOnSystem, 0);

    QOrganizerCollectionRemoveRequest crr;
    crr.setManager(&organizerManager);
    crr.setCollectionIds(QList<QOrganizerCollectionId>()<<collectionOnSystem.id());
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QVERIFY(!crr.error());
}

QTEST_MAIN(tst_QOrganizerStorageLocations)
#include "tst_qorganizerstoragelocations.moc"
