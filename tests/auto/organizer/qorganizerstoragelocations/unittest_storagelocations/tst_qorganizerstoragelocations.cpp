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
#include "../../../jsondbprocess.h"

QTORGANIZER_USE_NAMESPACE

class tst_QOrganizerStorageLocations : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStorageLocationsForCollections_data() { addManager(); }
    void testStorageLocationsForCollections();

private:
    void addManager();
    QList<QOrganizerCollection> collectionsFromAllStorageLocations(QOrganizerManager &organizerManager);

    // <manager, items> pair for existing items
    QMap<QString, QList<QOrganizerItem> > existingItems;
    QMap<QString, QList<QOrganizerCollection> > existingCollections;

    JsonDbProcess jsondbProcess;
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

QList<QOrganizerCollection> tst_QOrganizerStorageLocations::collectionsFromAllStorageLocations(QOrganizerManager &organizerManager)
{
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    cfr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage | QOrganizerAbstractRequest::SystemStorage);
    cfr.start();
    cfr.waitForFinished();
    cfr.isFinished();
    return cfr.collections();
}

void tst_QOrganizerStorageLocations::initTestCase()
{
    // Start JsonDb daemon if needed
    if (QOrganizerManager::availableManagers().contains("jsondb"))
        QVERIFY2(jsondbProcess.start(), "Failed to start JsonDb process");

    // back-up all existing items
    QStringList availableManagers(QOrganizerManager::availableManagers());
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    foreach (const QString &manager, availableManagers) {
        QOrganizerManager organizerManager(manager);
        // items
        QList<QOrganizerItem> items(organizerManager.items());
        if (items.size() > 0) {
            existingItems.insert(manager, items);
            QList<QOrganizerItemId> itemIds(organizerManager.itemIds());
            organizerManager.removeItems(itemIds);
        }
        // collections
        QList<QOrganizerCollection> collections(collectionsFromAllStorageLocations(organizerManager));
        QVERIFY(collections.count() >= 1);//there should always be atleast default collection
        existingCollections.insert(manager, collections);
        foreach (const QOrganizerCollection &collection, collections) {
            if (collection.id() != organizerManager.defaultCollection().id())
                organizerManager.removeCollection(collection.id());
        }
    }
}

void tst_QOrganizerStorageLocations::cleanupTestCase()
{
    // remove all items generated when running tests
    QStringList availableManagers(QOrganizerManager::availableManagers());
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    foreach (const QString &manager, availableManagers) {
        QOrganizerManager organizerManager(manager);
        // items
        organizerManager.removeItems(organizerManager.itemIds());
        // collections
        QList<QOrganizerCollection> collections(collectionsFromAllStorageLocations(organizerManager));
        QVERIFY(collections.count() >= 1);//there should always be atleast default collection
        foreach (const QOrganizerCollection &collection, collections) {
            if (collection.id() != organizerManager.defaultCollection().id())
                organizerManager.removeCollection(collection.id());
        }
    }

    // restore all existing items
    QMap<QString, QList<QOrganizerItem> >::iterator i = existingItems.begin();
    while (i != existingItems.end()) {
        QOrganizerManager organizerManager(i.key());
        organizerManager.saveItems(&(i.value()));
        ++i;
    }

    // collections
    QMap<QString, QList<QOrganizerCollection> >::iterator c = existingCollections.begin();
    while (c != existingCollections.end()) {
        QOrganizerManager organizerManager(c.key());
        foreach (const QOrganizerCollection &collection, c.value()) {
            if (collection.id() != organizerManager.defaultCollection().id())
                organizerManager.saveCollection(const_cast<QOrganizerCollection *>(&collection));
        }
        ++c;
    }

    // Kill jsondb-process, if started one
    if (QOrganizerManager::availableManagers().contains("jsondb"))
        jsondbProcess.terminate();
}

void tst_QOrganizerStorageLocations::testStorageLocationsForCollections()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QOrganizerManager organizerManager(managerName);

    QList<QOrganizerCollection> savedCollectionsUserData;//plus default collection
    QList<QOrganizerCollection> savedCollectionsSystemData;
    savedCollectionsUserData<<organizerManager.defaultCollection();

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
    QCOMPARE(savedCollectionsUserData.count(), cfr.collections().count());

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
    QCOMPARE(savedCollectionsUserData.count(), cfr.collections().count());// should be the same as in last fetch
    // - should be visible (1 and only 1) if fetching only from SystemStorage
    cfr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(1, cfr.collections().count());

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
    QCOMPARE(1, cfr.collections().count());
    QCOMPARE(savedCollectionsSystemData, cfr.collections());

    // All the collections should be visible when fetching from multiple storagelocations
    cfr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage | QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(savedCollectionsUserData.count() + savedCollectionsSystemData.count(), cfr.collections().count());// all

    // Remove collections from multiple storageLocations with one request
    QList<QOrganizerCollectionId> collIdListFromDifferentPartitions;
    foreach (const QOrganizerCollection &collection, cfr.collections()) {
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
    QCOMPARE(1, cfr.collections().count());
    QCOMPARE(cfr.collections().at(0).id(), organizerManager.defaultCollection().id());
}

QTEST_MAIN(tst_QOrganizerStorageLocations)
#include "tst_qorganizerstoragelocations.moc"
