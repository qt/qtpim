/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtPim module of the Qt Toolkit.
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

//TESTED_COMPONENT=src/organizer

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    name: "StorageLocationTests"
    id: storageLocationTests

    property int spyWaitDelay: 4000

    property string collectionOnSystemId: ""
    property string collectionOnUserDataId: ""
    property string eventOnUserDataId: ""

    QOrganizerTestUtility {
        id: utility
    }

    OrganizerModel{
        id: organizerModel
        manager: "jsondb"
        startPeriod:'2009-01-01'
        endPeriod:'2012-12-31'
//        onErrorChanged: {console.log("errorChanged:"+error)}
    }

    SignalSpy {
        id: modelChangedSpy
        target: organizerModel
        signalName: "modelChanged"
    }

    SignalSpy {
        id: collectionsChangedSpy
        target: organizerModel
        signalName: "collectionsChanged"
    }

    SignalSpy {
        id: errorChangedSpy
        target: organizerModel
        signalName: "errorChanged"
    }

    function initTestCase() {
        // check initial setup
        compare(organizerModel.storageLocations, OrganizerModel.UserDataStorage);
        collectionsChangedSpy.clear();
        organizerModel.storageLocations = OrganizerModel.SystemStorage | OrganizerModel.UserDataStorage;
        collectionsChangedSpy.wait(spyWaitDelay);

        organizerModel.autoUpdate = false;

        // cleaning collections
        var defaultCollectionId = organizerModel.defaultCollection().collectionId;
        var origCollectionCount = organizerModel.collections.length;
        for (var i=0;i<origCollectionCount;i++) {
            var collectionId = organizerModel.collections[i].collectionId;
            if (collectionId != defaultCollectionId) {
                organizerModel.removeCollection(collectionId);
            }
        }
        // cleaning items
        var ids = organizerModel.itemIds();
        if (ids.length > 0) {
            organizerModel.removeItems(ids);
        }

        organizerModel.autoUpdate = true;

        collectionsChangedSpy.clear();
        organizerModel.update()
        collectionsChangedSpy.wait(spyWaitDelay);

        collectionsChangedSpy.clear();
        organizerModel.storageLocations = OrganizerModel.UserDataStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
    }

    function test_collectionStorageLocationSupport() {
        // Preparations
        if (organizerModel.storageLocations != OrganizerModel.UserDataStorage) {
            collectionsChangedSpy.clear();
            organizerModel.storageLocations = OrganizerModel.UserDataStorage;
            collectionsChangedSpy.wait(spyWaitDelay);
        }
        compare(organizerModel.collections.length, 1);

        // Changing storage location to location without any collections
        collectionsChangedSpy.clear();
        organizerModel.storageLocations = OrganizerModel.SystemStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 0);

        // Save collectionOnSystem and confirm it to be visible on System-storageLocation
        var collectionOnSystem = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Collection { name: 'collection 1'  }", storageLocationTests)
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collectionOnSystem, OrganizerModel.SystemStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 1);
        collectionOnSystemId = organizerModel.collections[0].collectionId;
        // collectionOnSystem should not be visible on User-storageLocation
        organizerModel.storageLocations = OrganizerModel.UserDataStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 1);//default collection should be still there
        verify(organizerModel.collections[0].collectionId != collectionOnSystemId)

        // Save collectionOnUserData and confirm it to be visible on User-storageLocation
        var collectionOnUserData = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Collection { name: 'collection 2'  }", storageLocationTests)
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collectionOnUserData, OrganizerModel.UserDataStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        collectionOnUserData = organizerModel.collections[organizerModel.collections.length-1];
        collectionOnUserDataId = collectionOnUserData.collectionId;

        // Modify and save collectionOnUserData again
        // -first try to save different storageLocation, storageLocation-parameter should be just ignored
        collectionOnUserData.name = "changed";
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collectionOnUserData, OrganizerModel.SystemStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.collections[organizerModel.collections.length-1].name, "changed");
        // -save to same storageLocation
        collectionOnUserData.name = "changed again";
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collectionOnUserData, OrganizerModel.UserDataStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.collections[organizerModel.collections.length-1].name, "changed again");
        // -save without storageLocation defined
        collectionOnUserData.name = "really changed again";
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collectionOnUserData);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.collections[organizerModel.collections.length-1].name, "really changed again");

        // Ensure both collections are visible if following both User- and System-storageLocations
        organizerModel.storageLocations = OrganizerModel.UserDataStorage | OrganizerModel.SystemStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 3);
    }

    function test_itemStorageLocationSupport() {

        if (organizerModel.storageLocations != OrganizerModel.UserDataStorage) {
            collectionsChangedSpy.clear();
            organizerModel.storageLocations = OrganizerModel.UserDataStorage;
            collectionsChangedSpy.wait(spyWaitDelay);
        }
        compare(organizerModel.items.length, 0);

        // Save eventOnUserData and confirm it to be visible on UserDataStorage-storageLocation
        var eventOnUserData = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Event { displayLabel: 'item 1' \n startDateTime:new Date('2010-10-22') }", storageLocationTests)
        modelChangedSpy.clear();
        organizerModel.saveItem(eventOnUserData);// should be by default OrganizerModel.UserDataStorage
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 1);
        eventOnUserDataId = organizerModel.items[0].itemId;
        // eventOnUserData should not be visible on SystemStorage-storageLocation
        organizerModel.storageLocations = OrganizerModel.SystemStorage;
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 0);

        // Save eventOnSystem and confirm it to be visible on SystemStorage-storageLocation
        // -we need collection on System-partition for these tests
        //  (we should still have collectionOnSystem there from test_collectionStorageLocationSupport)
        compare(organizerModel.collections.length, 1);
        var collectionOnSystem = organizerModel.collections[0];
        // -first try with default collection, which is in different storageLocation, should fail
        var eventOnSystem = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Todo { displayLabel: 'item 2' \n startDateTime:new Date('2010-10-23') }", storageLocationTests)
        errorChangedSpy.clear();
        organizerModel.saveItem(eventOnSystem, OrganizerModel.SystemStorage);
        errorChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.error, "InvalidCollection");
        compare(organizerModel.items.length, 0);
        // - then try to save with with collectionOnSystem
        eventOnSystem.collectionId = collectionOnSystemId;
        modelChangedSpy.clear();
        organizerModel.saveItem(eventOnSystem, OrganizerModel.SystemStorage);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.items.length, 1);
        eventOnSystem = organizerModel.items[0];

        // Modify and save eventOnSystem again
        // -first try to save different storageLocation, storageLocation-parameter should be just ignored
        eventOnSystem.displayLabel = "changed";
        modelChangedSpy.clear();
        organizerModel.saveItem(eventOnSystem, OrganizerModel.UserDataStorage);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 1);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.items[0].displayLabel, "changed");
        // -save to same storageLocation
        eventOnSystem.displayLabel = "changed again";
        modelChangedSpy.clear();
        organizerModel.saveItem(eventOnSystem, OrganizerModel.SystemStorage);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 1);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.items[0].displayLabel, "changed again");
        // -save without storageLocation defined
        eventOnSystem.displayLabel = "really changed again";
        modelChangedSpy.clear();
        organizerModel.saveItem(eventOnSystem);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 1);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.items[0].displayLabel, "really changed again");

        // Ensure both items are visible if following both User- and System-storageLocations
        modelChangedSpy.clear();
        organizerModel.storageLocations = OrganizerModel.UserDataStorage | OrganizerModel.SystemStorage;
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 2);

        // Check default sorting order is working
        // - save one new event on SystemStorage with date which should come as first one, when sorted out
        var eventOnSystem2 = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Todo { displayLabel: 'item 0' \n startDateTime:new Date('2010-10-21') }", storageLocationTests)
        eventOnSystem2.collectionId = collectionOnSystemId;
        modelChangedSpy.clear();
        organizerModel.saveItem(eventOnSystem2, OrganizerModel.SystemStorage);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 3);
        var date0 = new Date(organizerModel.items[0].startDateTime);
        compare(date0.getDate(), 21)
        var date1 = new Date(organizerModel.items[1].startDateTime);
        compare(date1.getDate(), 22)
        var date2 = new Date(organizerModel.items[2].startDateTime);
        compare(date2.getDate(), 23)
    }

    function cleanupTestCase() {
        if (organizerModel.storageLocations != (OrganizerModel.SystemStorage | OrganizerModel.UserDataStorage)) {
            collectionsChangedSpy.clear();
            organizerModel.storageLocations = OrganizerModel.SystemStorage | OrganizerModel.UserDataStorage;
            collectionsChangedSpy.wait(spyWaitDelay);
        }

        modelChangedSpy.clear();
        organizerModel.removeCollection(collectionOnSystemId);
        organizerModel.removeCollection(collectionOnUserDataId);
        organizerModel.removeItem(eventOnUserDataId);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.items.length, 0);
        compare(organizerModel.collections.length, 1);
    }
}

