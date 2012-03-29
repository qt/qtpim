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

    property int spyWaitDelay: 500

    property string collection1id: ""
    property string collection2id: ""

    QOrganizerTestUtility {
        id: utility
    }

    OrganizerModel{
        id: organizerModel
        manager: "jsondb"
        startPeriod:'2009-01-01'
        endPeriod:'2012-12-31'
        onErrorChanged: {console.log("errorChanged:"+error)}
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

    function initTestCase() {
        // check initial setup
        compare(organizerModel.storageLocations, OrganizerModel.UserDataStorage);
        collectionsChangedSpy.clear();
        var originalLocations = organizerModel.storageLocations;
        organizerModel.storageLocations = OrganizerModel.SystemStorage | OrganizerModel.UserDataStorage;
        collectionsChangedSpy.wait(spyWaitDelay);

        // cleaning collections
        var defaultCollectionId = organizerModel.defaultCollection().collectionId;
        var collectionsDeleted = false;
        var origCollectionCount = organizerModel.collections.length;
        collectionsChangedSpy.clear();
        for (var i=0;i<origCollectionCount;i++) {
            var collectionId = organizerModel.collections[i].collectionId;
            if (collectionId != defaultCollectionId) {
                organizerModel.removeCollection(collectionId);
                collectionsDeleted = true;
            }
        }
        if (collectionsDeleted)
            collectionsChangedSpy.wait();

        collectionsChangedSpy.clear();
        organizerModel.storageLocations = originalLocations;
        collectionsChangedSpy.wait(spyWaitDelay);
    }

    function test_collectionStorageLocationSupport() {
        // Preparations
        compare(organizerModel.storageLocations, OrganizerModel.UserDataStorage);
        modelChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 1);

        // Changing storage location to location without any collections
        collectionsChangedSpy.clear();
        organizerModel.storageLocations = OrganizerModel.SystemStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 0);

        // Save collection 1 and confirm it to be visible on System-storageLocation
        var collection1 = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Collection { name: 'collection 1'  }", storageLocationTests)
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collection1, OrganizerModel.SystemStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 1);
        collection1id = organizerModel.collections[0].collectionId;
        // collection1 should not be visible on User-storageLocation
        organizerModel.storageLocations = OrganizerModel.UserDataStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 1);//default collection should be still there
        verify(organizerModel.collections[0].collectionId != collection1id)

        // Save collection2 and confirm it to be visible on User-storageLocation
        var collection2 = utility.create_testobject("import QtOrganizer 5.0 \n"
            + "  Collection { name: 'collection 2'  }", storageLocationTests)
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collection2, OrganizerModel.UserDataStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        collection2 = organizerModel.collections[organizerModel.collections.length-1];
        collection2id = collection2.collectionId;

        // Modify and save collection2 again
        // -first try to save different storageLocation, storageLocation-parameter should be just ignored
        collection2.name = "changed";
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collection2, OrganizerModel.SystemStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.collections[organizerModel.collections.length-1].name, "changed");
        // -save to same storageLocation
        collection2.name = "changed again";
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collection2, OrganizerModel.UserDataStorage);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.collections[organizerModel.collections.length-1].name, "changed again");
        // -save without storageLocation defined
        collection2.name = "really changed again";
        collectionsChangedSpy.clear();
        organizerModel.saveCollection(collection2);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 2);
        compare(organizerModel.error, "NoError");
        compare(organizerModel.collections[organizerModel.collections.length-1].name, "really changed again");

        // Ensure both collections are visible if following both User- and System-storageLocations
        organizerModel.storageLocations = OrganizerModel.UserDataStorage | OrganizerModel.SystemStorage;
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 3);
    }

    function cleanupTestCase() {
        if (organizerModel.storageLocations != (OrganizerModel.SystemStorage | OrganizerModel.UserDataStorage)) {
            collectionsChangedSpy.clear();
            organizerModel.storageLocations = OrganizerModel.SystemStorage | OrganizerModel.UserDataStorage;
            collectionsChangedSpy.wait(spyWaitDelay);
        }

        collectionsChangedSpy.clear();
        organizerModel.removeCollection(collection1id);
        organizerModel.removeCollection(collection2id);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, 1);
    }
}

