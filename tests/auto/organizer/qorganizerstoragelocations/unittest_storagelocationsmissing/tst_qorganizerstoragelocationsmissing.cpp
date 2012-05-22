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

#include <qorganizermanager.h>
#include <qorganizeritems.h>
#include <qorganizeritemrequests.h>
#include "../../../jsondbprocess.h"

QTORGANIZER_USE_NAMESPACE

class tst_QOrganizerStorageLocationsMissing : public QObject
{
    Q_OBJECT

private slots:
    void testAllStorageLocationsMissing_data() { addManager(); }
    void testAllStorageLocationsMissing();

    void testUserDataStorageLocationMissing_data() { addManager(); }
    void testUserDataStorageLocationMissing();

    void testSystemStorageLocationMissing_data() { addManager(); }
    void testSystemStorageLocationMissing();

private:
    void addManager();
    QList<QOrganizerCollection> collectionsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations);
    QList<QOrganizerItem> itemsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations);

    JsonDbProcess m_jsondbProcess;
};

void tst_QOrganizerStorageLocationsMissing::addManager()
{
    QTest::addColumn<QString>("managerName");

    QStringList availableManagers = QOrganizerManager::availableManagers();
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    foreach (const QString &manager, availableManagers)
        QTest::newRow(manager.toLatin1().constData()) << manager;
}

QList<QOrganizerCollection> tst_QOrganizerStorageLocationsMissing::collectionsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations)
{
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    cfr.setStorageLocations(storageLocations);
    cfr.start();
    cfr.waitForFinished();
    cfr.isFinished();
    return cfr.collections();
}

QList<QOrganizerItem> tst_QOrganizerStorageLocationsMissing::itemsFromStorageLocation(QOrganizerManager &organizerManager, QOrganizerAbstractRequest::StorageLocations storageLocations)
{
    QOrganizerItemFetchRequest ifr;
    ifr.setManager(&organizerManager);
    ifr.setStorageLocations(storageLocations);
    ifr.start();
    ifr.waitForFinished();
    ifr.isFinished();
    return ifr.items();
}

void tst_QOrganizerStorageLocationsMissing::testAllStorageLocationsMissing()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QString partitions_json = QFINDTESTDATA("./partitionfiles/nopartitions/partitions.json");
    QVERIFY2(!partitions_json.isEmpty(), "partitions.json file is missing");
    QVERIFY2(m_jsondbProcess.start(partitions_json), "Failed to start JsonDb process");

    qDebug()<<"NOTE: Jsondb backend gives 5 lines of warnings when not able to access any storage locations!";

    QOrganizerManager organizerManager(managerName);

    // try some operatios, they should return with MissingPlatformRequirementsError
    qDebug()<<"NOTE: Jsondb backend warns when there is no UserDataStorage available!";
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(cfr.error(), QOrganizerManager::MissingPlatformRequirementsError);
    qDebug()<<"NOTE: Jsondb backend warns when there is no UserDataStorage available!";
    QOrganizerItemFetchRequest ifr;
    ifr.setManager(&organizerManager);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QCOMPARE(ifr.error(), QOrganizerManager::MissingPlatformRequirementsError);
    QOrganizerCollectionRemoveRequest crr;
    crr.setManager(&organizerManager);
    qDebug()<<"NOTE: Jsondb backend warns when there is no UserDataStorage available!";
    QOrganizerCollectionId dummyCollIdOnUser = QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::1/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}"));
    crr.setCollectionId(dummyCollIdOnUser);
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::MissingPlatformRequirementsError);

    m_jsondbProcess.terminate();
}

void tst_QOrganizerStorageLocationsMissing::testUserDataStorageLocationMissing()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QString partitions_json = QFINDTESTDATA("./partitionfiles/onlysystempartition/partitions.json");
    QVERIFY2(!partitions_json.isEmpty(), "partitions.json file is missing");
    QVERIFY2(m_jsondbProcess.start(partitions_json), "Failed to start JsonDb process");

    qDebug()<<"NOTE: Jsondb backend gives 3 lines of warnings when not able to access UserDataStorage!";

    QOrganizerManager organizerManager(managerName);

    // try some operatios, they should return with MissingPlatformRequirementsError
    qDebug()<<"NOTE: Jsondb backend warns when there is no UserDataStorage available!";
    QOrganizerCollectionSaveRequest csr;
    csr.setManager(&organizerManager);
    QOrganizerCollection testCollection;
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection on default");
    csr.setCollection(testCollection);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QCOMPARE(csr.error(), QOrganizerManager::MissingPlatformRequirementsError);
    qDebug()<<"NOTE: Jsondb backend warns when there is no UserDataStorage available!";
    QOrganizerItemSaveRequest isr;
    isr.setManager(&organizerManager);
    QOrganizerEvent testEvent;
    testEvent.setDisplayLabel("Event on default storage");
    isr.setItem(testEvent);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QCOMPARE(isr.error(), QOrganizerManager::MissingPlatformRequirementsError);
    qDebug()<<"NOTE: Jsondb backend warns when there is no UserDataStorage available!";
    QOrganizerItemRemoveRequest irr;
    irr.setManager(&organizerManager);
    QOrganizerItemId dummyIdOnUser = QOrganizerItemId::fromString(QString("qtorganizer:jsondb::1/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}"));
    testEvent.setId(dummyIdOnUser);
    irr.setItem(testEvent);
    QVERIFY(irr.start());
    QVERIFY(irr.waitForFinished());
    QVERIFY(irr.isFinished());
    QCOMPARE(irr.error(), QOrganizerManager::MissingPlatformRequirementsError);

    m_jsondbProcess.terminate();
}

void tst_QOrganizerStorageLocationsMissing::testSystemStorageLocationMissing()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QString partitions_json = QFINDTESTDATA("./partitionfiles/onlyuserpartition/partitions.json");
    QVERIFY2(!partitions_json.isEmpty(), "partitions.json file is missing");
    QVERIFY2(m_jsondbProcess.start(partitions_json), "Failed to start JsonDb process");

    qDebug()<<"NOTE: Jsondb backend gives couple of warnings when not able to register for notifications from system!";

    QOrganizerManager organizerManager(managerName);
    const int existingItemCountOnUser = itemsFromStorageLocation(organizerManager, QOrganizerAbstractRequest::UserDataStorage).count();
    const int existingCollectionCountOnUser = collectionsFromStorageLocation(organizerManager, QOrganizerAbstractRequest::UserDataStorage).count();

    QList<QOrganizerItem> savedItemsUserData;
    QList<QOrganizerCollection> savedCollectionsUserData;

    // Collection related requests

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
    // - storagelocation explicitly set to SystemStorage - should fail
    csr.setCollection(testCollection);
    csr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QCOMPARE(csr.error(), QOrganizerManager::InvalidStorageLocationError);
    // - storagelocation explicitly set to UserDataStorage
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection on UserDataStorage");
    csr.setCollection(testCollection);
    csr.setStorageLocation(QOrganizerAbstractRequest::UserDataStorage);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    savedCollectionsUserData<<csr.collections();
    // Modify collection and update it
    testCollection = csr.collections().at(0);
    testCollection.setMetaData(QOrganizerCollection::KeyName, "Collection saved again on UserDataStorage");
    QVERIFY(csr.collections().at(0) != testCollection);
    csr.setCollection(testCollection);
    csr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);//this should be ignored when saving already saved collection
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(!csr.error());
    QVERIFY(csr.collections().at(0) == testCollection);
    // Fetch from default storageLocation, should find both above (+ default collection)
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(savedCollectionsUserData.count(), cfr.collections().count()-existingCollectionCountOnUser);
    // Fetch from system - should fail
    cfr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(cfr.error(), QOrganizerManager::InvalidStorageLocationError);
    // Check collection removal from non-accessible storage location
    QOrganizerCollectionRemoveRequest crr;
    crr.setManager(&organizerManager);
    crr.setCollectionId(QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::2/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::InvalidStorageLocationError);
    // -invalid storage location defined
    crr.setCollectionId(QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::4/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::InvalidStorageLocationError);
    crr.setCollectionId(QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::0/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::InvalidStorageLocationError);
    crr.setCollectionId(QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::-1/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::InvalidStorageLocationError);
    crr.setCollectionId(QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::2//{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::InvalidStorageLocationError);
    crr.setCollectionId(QOrganizerCollectionId::fromString(QString("qtorganizer:jsondb::-3*abg/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QCOMPARE(crr.error(), QOrganizerManager::InvalidStorageLocationError);

    // Item related requests

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
    // - storagelocation explicitly set to SystemStorage - should fail
    QOrganizerTodo testTodo;
    isr.setItem(testTodo);
    isr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QCOMPARE(isr.error(), QOrganizerManager::InvalidStorageLocationError);
    // - storagelocation explicitly set to UserDataStorage
    testTodo.setDisplayLabel("Todo on UserData");
    isr.setItem(testTodo);
    isr.setStorageLocation(QOrganizerAbstractRequest::UserDataStorage);
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QVERIFY(!isr.error());
    savedItemsUserData<<isr.items();
    // Modify event and update it
    testTodo = isr.items().at(0);
    testTodo.setDisplayLabel("Event saved again on UserDataStorage");
    QVERIFY(isr.items().at(0).displayLabel() != testTodo.displayLabel());
    isr.setItem(testTodo);
    isr.setStorageLocation(QOrganizerAbstractRequest::SystemStorage);//this should be ignored when saving already saved collection
    QVERIFY(isr.start());
    QVERIFY(isr.waitForFinished());
    QVERIFY(isr.isFinished());
    QVERIFY(!isr.error());
    QVERIFY(isr.items().at(0).displayLabel() == testTodo.displayLabel());
    // Fetch from default storageLocation, should find both above
    QOrganizerItemFetchRequest ifr;
    ifr.setManager(&organizerManager);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QVERIFY(!ifr.error());
    QCOMPARE(savedItemsUserData.count(), ifr.items().count()-existingItemCountOnUser);
    // Fetch from system - should fail
    ifr.setStorageLocations(QOrganizerAbstractRequest::SystemStorage);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QCOMPARE(ifr.error(), QOrganizerManager::InvalidStorageLocationError);
    // Check removing itemid from non-accessible storage location
    QOrganizerItemRemoveByIdRequest irbir;
    irbir.setManager(&organizerManager);
    QOrganizerItemId dummyIdOnSystem = QOrganizerItemId::fromString(QString("qtorganizer:jsondb::2/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}"));
    irbir.setItemId(dummyIdOnSystem);
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QCOMPARE(irbir.error(), QOrganizerManager::InvalidStorageLocationError);
    // -invalid storage location defined
    irbir.setItemId(QOrganizerItemId::fromString(QString("qtorganizer:jsondb::4/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QCOMPARE(irbir.error(), QOrganizerManager::InvalidStorageLocationError);
    irbir.setItemId(QOrganizerItemId::fromString(QString("qtorganizer:jsondb::0/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QCOMPARE(irbir.error(), QOrganizerManager::InvalidStorageLocationError);
    irbir.setItemId(QOrganizerItemId::fromString(QString("qtorganizer:jsondb::-1/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QCOMPARE(irbir.error(), QOrganizerManager::InvalidStorageLocationError);
    irbir.setItemId(QOrganizerItemId::fromString(QString("qtorganizer:jsondb::2//{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QCOMPARE(irbir.error(), QOrganizerManager::InvalidStorageLocationError);
    irbir.setItemId(QOrganizerItemId::fromString(QString("qtorganizer:jsondb::qw-e*34/{05c5b1f5-6988-4401-b7bd-0edaf7733f88}")));
    QVERIFY(irbir.start());
    QVERIFY(irbir.waitForFinished());
    QVERIFY(irbir.isFinished());
    QCOMPARE(irbir.error(), QOrganizerManager::InvalidStorageLocationError);
    // Check removing item from non-accessible storage location
    QOrganizerItemRemoveRequest irr;
    irr.setManager(&organizerManager);
    testEvent.setId(dummyIdOnSystem);
    irr.setItem(testEvent);
    QVERIFY(irr.start());
    QVERIFY(irr.waitForFinished());
    QVERIFY(irr.isFinished());
    QCOMPARE(irr.error(), QOrganizerManager::InvalidStorageLocationError);

    // cleaning up
    // -removal of created items
    irr.setItems(savedItemsUserData);
    QVERIFY(irr.start());
    QVERIFY(irr.waitForFinished());
    QVERIFY(irr.isFinished());
    QVERIFY(!irr.error());
    ifr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QVERIFY(!ifr.error());
    QCOMPARE(existingItemCountOnUser, ifr.items().count());
    // -removal of created collections
    QList<QOrganizerCollectionId> collIds;
    foreach (const QOrganizerCollection &collection, savedCollectionsUserData) {
        if (collection.id() != organizerManager.defaultCollection().id())
            collIds<<collection.id();
    }
    crr.setCollectionIds(collIds);
    QVERIFY(crr.start());
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());
    QVERIFY(!crr.error());
    cfr.setStorageLocations(QOrganizerAbstractRequest::UserDataStorage);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QVERIFY(!cfr.error());
    QCOMPARE(existingCollectionCountOnUser, cfr.collections().count());

    m_jsondbProcess.terminate();
}

QTEST_MAIN(tst_QOrganizerStorageLocationsMissing)
#include "tst_qorganizerstoragelocationsmissing.moc"
