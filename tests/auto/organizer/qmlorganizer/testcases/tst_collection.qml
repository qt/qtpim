/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
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

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    name: "CollectionTests"
    id: collectionTests

    property int spyWaitDelay: 700
    property int noSpyWaitDelay: 250

    // UTILITIES
    QOrganizerTestUtility {
        id: utility
    }

    function empty_calendar(organizerModel) {

        organizerModel.autoUpdate = false;

        var ids = organizerModel.itemIds();
        if (ids.length > 0) {
            organizerModel.removeItems(ids);
        }
        for (var i = 0; i < organizerModel.collections.length; ++i) {
            var collId = organizerModel.collections[i].collectionId;
            if (collId != organizerModel.defaultCollectionId()) {
                organizerModel.removeCollection(collId);
            }
        }
        organizerModel.autoUpdate = true;

        organizerModel.update();
    }

    // COLLECTION ELEMENT API

    function create_collection() {
        console.log("");//to print out test tags for every data set
        var ctorString = "import QtQuick 2.0 \n"
            + "import QtOrganizer 5.0\n"
            + "Collection {\n"
            + "}\n"
        var newCollection = Qt.createQmlObject(ctorString, collectionTests);
        verify(newCollection != undefined, 'Collection creation failed');
        return newCollection;
    }

    //    SignalSpy {
    //        id: spy
    //        signalName: "valueChanged"
    //    }

    // There is currently some problem with static
    // SignalSpy and changing the target (QTBUG-21083).
    // As a workaround recreating the spy dynamicly.
    function create_spy(targetObj, signalName) {
        var spy = Qt.createQmlObject( "import QtTest 1.0 \nSignalSpy {}", collectionTests);
        spy.target = targetObj;
        spy.signalName = signalName;
        return spy;
    }

    // creation
    function test_creation_data() {
        return [
                    {tag: "empty definition",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "Collection {\n"
                        + "}\n"
                },
                    {tag: "full definition",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "Collection {\n"
                        + "id: testCollection\n"
                        + "collectionId: 'my collection id (setting might not be supported by the backend)'\n"
                        + "name: 'My test collection'\n"
                        + "description: 'Here we are testing our awesome collection!'\n"
                        + "color: 'green'\n"
                        + "image: 'my/url/somewhere'\n"
                        + "}\n"
                }
        ]
    }
    function test_creation(data) {
        console.log("");//to print out test tags for every data set
        var newCollection = Qt.createQmlObject(data.code, collectionTests);
        verify(newCollection != undefined, 'Collection creation failed');
        //verify(newCollection.parent == collectionTests);
    }

    // name API
    function test_name_data() {
        return [
            {tag: "empty name", testValue: ""},
            {tag: "string name", testValue: "my name"},
            {tag: "long string name", testValue: "here goes the long string name but what to have here? Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed posuere interdum sem. Quisque ligula eros ullamcorper quis, lacinia quis facilisis sed sapien. Mauris varius diam vitae arcu. Sed arcu lectus auctor vitae, consectetuer et venenatis eget velit. Sed augue orci, lacinia eu tincidunt et eleifend nec lacus. Donec ultricies nisl ut felis, suspendisse potenti. Lorem ipsum ligula ut hendrerit mollis, ipsum erat vehicula risus, eu suscipit sem libero nec erat. Aliquam erat volutpat. Sed congue augue vitae neque. Nulla consectetuer porttitor pede. Fusce purus morbi tortor magna condimentum vel, placerat id blandit sit amet tortor. Mauris sed libero. Suspendisse facilisis nulla in lacinia laoreet, lorem velit accumsan velit vel mattis libero nisl et sem. Proin interdum maecenas massa turpis sagittis in, interdum non lobortis vitae massa. Quisque purus lectus, posuere eget imperdiet nec sodales id arcu. Vestibulum elit pede dictum eu, viverra non tincidunt eu ligula. Nam molestie nec tortor. Donec placerat leo sit amet velit. Vestibulum id justo ut vitae massa. Proin in dolor mauris consequat aliquam. Donec ipsum, vestibulum ullamcorper venenatis augue. Aliquam tempus nisi in auctor vulputate, erat felis pellentesque augue nec, pellentesque lectus justo nec erat. Aliquam et nisl. Quisque sit amet dolor in justo pretium condimentum. Vivamus placerat lacus vel vehicula scelerisque, dui enim adipiscing lacus sit amet sagittis, libero enim vitae mi. In neque magna posuere, euismod ac tincidunt tempor est. Ut suscipit nisi eu purus. Proin ut pede mauris eget ipsum. Integer vel quam nunc commodo consequat. Integer ac eros eu tellus dignissim viverra. Maecenas erat aliquam erat volutpat. Ut venenatis ipsum quis turpis. Integer cursus scelerisque lorem. Sed nec mauris id quam blandit consequat. Cras nibh mi hendrerit vitae, dapibus et aliquam et magna. Nulla vitae elit. Mauris consectetuer odio vitae augue."}
        ]
    }
    function test_name(data) {
        var newCollection = create_collection();
        var spy = create_spy(newCollection, "valueChanged");
        // change
        newCollection.name = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.name, data.testValue);
        // change without change
        newCollection.name = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.name, data.testValue);
    }

    // description API
    function test_description_data() {
        return [
            {tag: "empty desc", testValue: ""},
            {tag: "string desc", testValue: "here goes the string description"},
            {tag: "long string desc", testValue: "here goes the long string description but what to have here? Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed posuere interdum sem. Quisque ligula eros ullamcorper quis, lacinia quis facilisis sed sapien. Mauris varius diam vitae arcu. Sed arcu lectus auctor vitae, consectetuer et venenatis eget velit. Sed augue orci, lacinia eu tincidunt et eleifend nec lacus. Donec ultricies nisl ut felis, suspendisse potenti. Lorem ipsum ligula ut hendrerit mollis, ipsum erat vehicula risus, eu suscipit sem libero nec erat. Aliquam erat volutpat. Sed congue augue vitae neque. Nulla consectetuer porttitor pede. Fusce purus morbi tortor magna condimentum vel, placerat id blandit sit amet tortor. Mauris sed libero. Suspendisse facilisis nulla in lacinia laoreet, lorem velit accumsan velit vel mattis libero nisl et sem. Proin interdum maecenas massa turpis sagittis in, interdum non lobortis vitae massa. Quisque purus lectus, posuere eget imperdiet nec sodales id arcu. Vestibulum elit pede dictum eu, viverra non tincidunt eu ligula. Nam molestie nec tortor. Donec placerat leo sit amet velit. Vestibulum id justo ut vitae massa. Proin in dolor mauris consequat aliquam. Donec ipsum, vestibulum ullamcorper venenatis augue. Aliquam tempus nisi in auctor vulputate, erat felis pellentesque augue nec, pellentesque lectus justo nec erat. Aliquam et nisl. Quisque sit amet dolor in justo pretium condimentum. Vivamus placerat lacus vel vehicula scelerisque, dui enim adipiscing lacus sit amet sagittis, libero enim vitae mi. In neque magna posuere, euismod ac tincidunt tempor est. Ut suscipit nisi eu purus. Proin ut pede mauris eget ipsum. Integer vel quam nunc commodo consequat. Integer ac eros eu tellus dignissim viverra. Maecenas erat aliquam erat volutpat. Ut venenatis ipsum quis turpis. Integer cursus scelerisque lorem. Sed nec mauris id quam blandit consequat. Cras nibh mi hendrerit vitae, dapibus et aliquam et magna. Nulla vitae elit. Mauris consectetuer odio vitae augue."}
            ]
    }
    function test_description(data) {
        var newCollection = create_collection();
        var spy = create_spy(newCollection, "valueChanged");
        // change
        newCollection.description = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.description, data.testValue);
        // change without change
        newCollection.description = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.description, data.testValue);
    }

    // color API
    function test_color_data() {
        return [
            {tag: "black", testValue: "black", colorCode: "#000000"},
            {tag: "red", testValue: "red", colorCode: "#ff0000"}
            ]
    }
    function test_color(data) {
        var newCollection = create_collection();
        var spy = create_spy(newCollection, "valueChanged");
        // change
        newCollection.color = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.color.toString(), data.colorCode.toString());
        // change without change
        newCollection.color = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.color.toString(), data.colorCode.toString());
    }

    // image API
    function test_image_data() {
        return [
            {tag: "empty path", testValue: ""},
            {tag: "string path", testValue: "/my/path/"}
            ]
    }
    function test_image(data) {
        var newCollection = create_collection();
        var spy = create_spy(newCollection, "valueChanged");
        // change
        newCollection.image = data.testValue;
        compare(spy.count, 1);
        var pathString = newCollection.image.toString();
        verify(pathString.search(data.testValue) != -1);
        // change without change
        newCollection.image = data.testValue;
        compare(spy.count, 1);
        compare(newCollection.image.toString(), pathString);
    }

    // metadata API
    function test_metadata_data() {
        return [
            {tag: "string key", testKey: "myKey", testValue: "my awesome key"}
            ]
    }
    function test_metadata(data) {
        var newCollection = create_collection();
        var spy = create_spy(newCollection, "valueChanged");
        // change
        newCollection.setExtendedMetaData(data.testKey, data.testValue);
        compare(spy.count, 1);
        compare(newCollection.extendedMetaData(data.testKey).toString(), data.testValue);
        // change without change
        newCollection.setExtendedMetaData(data.testKey, data.testValue);
        compare(spy.count, 1);
        compare(newCollection.extendedMetaData(data.testKey).toString(), data.testValue);
    }

    // ITEM COLLECTION API

    function backend_plugin_available(model, pluginName) {
        var managerAvailable = false;
        for (var i=0;i<model.availableManagers.length;i++) {
            if (pluginName==model.availableManagers[i]) {
                managerAvailable = true;
                break;
            }
        }
        return managerAvailable;
    }
    function create_testobject(ctorString) {
        var newObject = Qt.createQmlObject(ctorString, collectionTests);
        verify(newObject != undefined, 'Object creation failed');
        return newObject;
    }
    function test_item_api_data() {
        return utility.getManagerListData();
    }
    function test_item_api(data) {
        console.log("");//to print out test tags for every data set
        var modelChangedSpy = create_testobject("import QtTest 1.0; SignalSpy {}");
        // Create and check that backend for the tests is available
        var organizerModel = create_testobject("import QtQuick 2.0\n"
          + "import QtOrganizer 5.0\n"
          + "OrganizerModel {\n"
          + "  manager: '" + data.managerToBeTested + "'\n"
          + "  startPeriod:'2009-01-01'\n"
          + "  endPeriod:'2012-12-31'\n"
          + "}\n");
        modelChangedSpy.target = organizerModel;
        modelChangedSpy.signalName = "modelChanged";
        modelChangedSpy.wait(spyWaitDelay);
        verify(backend_plugin_available(organizerModel, data.managerToBeTested),
               "Cannot run tests for " + data.managerToBeTested + " backend. No plugin available!");
        empty_calendar(organizerModel);

        // There is currently some problem with static
        // SignalSpy and changing the target (QTBUG-21083).
        // As a workaround recreating the spy dynamicly.
        var spy = create_testobject( "import QtTest 1.0 \nSignalSpy {}");

        var event = create_testobject("import QtTest 1.0\nimport QtOrganizer 5.0\n"
          + "Event{\n"
          + "  startDateTime: '2010-12-12'\n"
          + "  endDateTime: '2010-12-13'\n"
          + "}\n");

        // preparations for collection and event
        var collection = create_testobject("import QtQuick 2.0\nimport QtOrganizer 5.0\n"
          + "Collection {name: 'myTestCollection';}\n");
        var collectionsChangedSpy = create_testobject("import QtTest 1.0 \nSignalSpy {}");
        collectionsChangedSpy.target = organizerModel;
        collectionsChangedSpy.signalName = "collectionsChanged";
        organizerModel.saveCollection(collection);
        collectionsChangedSpy.wait(spyWaitDelay);
        var savedCollection = organizerModel.collections[organizerModel.collections.length - 1];

        // check that after first save event has default collection id
        var spySettingCollectionId = create_testobject("import QtTest 1.0 \nSignalSpy {}");
        spySettingCollectionId.target = event;
        spySettingCollectionId.signalName = "itemChanged";
        event.collectionId = "qtorganizer:dummy:non:existing"; //since this collection id is invalid, it will not be really set there
        event.collectionId = ""; //since this collection id is invalid, it will not be really set there
        compare(spySettingCollectionId.count, 0);
        organizerModel.saveItem(event);
        modelChangedSpy.wait(spyWaitDelay);
        var savedEvent = organizerModel.items[organizerModel.items.length - 1];
        compare(savedEvent.collectionId, organizerModel.defaultCollectionId());//savedEvent sometimes undefined!?!?!?
        spySettingCollectionId.target = savedEvent;
        spySettingCollectionId.wait(spyWaitDelay);
        compare(spySettingCollectionId.count, 1);

        // set different collection
        verify(savedCollection.collectionId != organizerModel.defaultCollectionId())
        savedEvent.collectionId = savedCollection.collectionId;
        spySettingCollectionId.wait(spyWaitDelay);
        compare(spySettingCollectionId.count, 2);
        // set same collection again, shouldn't cause signal emission
        savedEvent.collectionId = savedCollection.collectionId;
        compare(spySettingCollectionId.count, 2);

        // check the changed collection is saved
        var errorsChangedSpy = create_testobject("import QtTest 1.0 \nSignalSpy {}");
        errorsChangedSpy.target = organizerModel;
        errorsChangedSpy.signalName = "errorChanged";
        organizerModel.saveItem(savedEvent);

        if (data.managerToBeTested == "memory") {
            // memory backend does not support changing collection of an item, collection id does not change
            errorsChangedSpy.wait(spyWaitDelay);
            savedEvent = organizerModel.items[organizerModel.items.length - 1];
            compare(organizerModel.error, "InvalidCollection");
            expectFailContinue("memory backend", "Model is updated even in error case.")
            compare(savedEvent.collectionId, organizerModel.defaultCollectionId());
        }

        // cleanup
        empty_calendar(organizerModel);
    }

    // MODEL COLLECTION API

    function test_model_api_data() {
        return utility.getManagerListData();
    }
    function test_model_api(data) {
        console.log("");//to print out test tags for every data set
        // Create and check that backend for the tests is available
        var modelChangedSpy = create_testobject("import QtTest 1.0; SignalSpy {}");
        var organizerModel = create_testobject("import QtQuick 2.0\n"
          + "import QtOrganizer 5.0\n"
          + "OrganizerModel {\n"
          + "  manager: '" + data.managerToBeTested + "'\n"
          + "  startPeriod:'2009-01-01'\n"
          + "  endPeriod:'2012-12-31'\n"
          + "}\n");
        modelChangedSpy.target = organizerModel;
        modelChangedSpy.signalName = "modelChanged";
        modelChangedSpy.wait();
        verify(backend_plugin_available(organizerModel, data.managerToBeTested),
               "Cannot run tests for " + data.managerToBeTested + " backend. No plugin available!");
        empty_calendar(organizerModel);

        // There is currently some problem with static
        // SignalSpy and changing the target (QTBUG-21083).
        // As a workaround recreating the spy dynamicly.
        var collectionsChangedSpy = create_testobject( "import QtTest 1.0 \nSignalSpy {}");
        collectionsChangedSpy.target = organizerModel;
        collectionsChangedSpy.signalName = "collectionsChanged"
        collectionsChangedSpy.clear();

        var originalAmountOfCollections = organizerModel.collections.length;

        // default collection exists always
        var defCollection = organizerModel.collection(organizerModel.defaultCollectionId());
        verify(defCollection);

        // add/save collection
        // - autoupdate disabled
        organizerModel.autoUpdate = false
        var coll1 = create_testobject("import QtQuick 2.0 \n"
          + "import QtOrganizer 5.0\n"
          + "Collection {\n"
          + "id: coll1\n"
          + "}\n");
        organizerModel.saveCollection(coll1);
        compare(organizerModel.collections.length, originalAmountOfCollections);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(collectionsChangedSpy.count, 1);
        organizerModel.fetchCollections();
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, originalAmountOfCollections + 1);
        compare(collectionsChangedSpy.count, 2);
        // -autoupdate enabled
        organizerModel.autoUpdate = true
        var coll2 = create_testobject("import QtQuick 2.0 \n"
          + "import QtOrganizer 5.0\n"
          + "Collection{}\n");
        coll2.name = "My collection";
        coll2.description = "My description";
        coll2.color = "red";
        coll2.image = "/test/path/"
        coll2.setExtendedMetaData("MyKey", "My values");
        organizerModel.saveCollection(coll2);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, originalAmountOfCollections + 2);
        compare(collectionsChangedSpy.count, 3);
        // -check collection details were properly saved
        var amountBeforeSavingAgain = organizerModel.collections.length;
        var modifiableCollection = organizerModel.collections[amountBeforeSavingAgain - 1];
        compare(modifiableCollection.name, coll2.name);
        compare(modifiableCollection.description, coll2.description);
        compare(modifiableCollection.color.toString(), coll2.color.toString());
        compare(modifiableCollection.image.toString(), coll2.image.toString());
        compare(modifiableCollection.extendedMetaData("MyKey").toString(), coll2.extendedMetaData("MyKey").toString());
        // -modify collection content
        modifiableCollection.name = "My brilliant collection";
        modifiableCollection.description = "My awesome description";
        modifiableCollection.color = "#000042";
        modifiableCollection.image = "/my/longer/test/path/";
        modifiableCollection.setExtendedMetaData("MyKey", "My just unbelievable values");
        organizerModel.saveCollection(modifiableCollection);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, amountBeforeSavingAgain);
        compare(collectionsChangedSpy.count, 4);
        var resavedCollection = organizerModel.collections[organizerModel.collections.length - 1];
        compare(resavedCollection.name, modifiableCollection.name);
        compare(resavedCollection.description, modifiableCollection.description);
        compare(resavedCollection.color.toString(), modifiableCollection.color.toString());
        compare(resavedCollection.image.toString(), modifiableCollection.image.toString());
        compare(resavedCollection.extendedMetaData("MyKey").toString(), modifiableCollection.extendedMetaData("MyKey").toString());
        // -save new collection with empty content data
        var coll3 = create_testobject("import QtQuick 2.0 \n"
          + "import QtOrganizer 5.0\n"
          + "Collection {\n"
          + "}\n");
        coll3.name = "My empty collection";
        organizerModel.saveCollection(coll3);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, amountBeforeSavingAgain+1);
        compare(collectionsChangedSpy.count, 5);
        var savedEmptyCollection = organizerModel.collections[organizerModel.collections.length - 1];
        compare(savedEmptyCollection.name, coll3.name);
        compare(savedEmptyCollection.description, coll3.description);
        compare(savedEmptyCollection.color.toString(), coll3.color.toString());
        compare(savedEmptyCollection.image.toString(), coll3.image.toString());

        // fetching existing and non-existing collection
        var existingCollection = organizerModel.collection(organizerModel.collections[organizerModel.collections.length - 1].collectionId);
        verify(existingCollection);
        var nonexistingCollection = organizerModel.collection("gone fishing");
        verify(!nonexistingCollection);

        // delete
        var amountBeforeDeletions = organizerModel.collections.length;
        // - remove normal collection
        organizerModel.removeCollection(organizerModel.collections[organizerModel.collections.length - 1].collectionId);
        collectionsChangedSpy.wait(spyWaitDelay);
        compare(organizerModel.collections.length, amountBeforeDeletions - 1);
        compare(collectionsChangedSpy.count, 6);
        // - remove collection with items
        var toBeDeletedCollection = organizerModel.collections[organizerModel.collections.length - 1];
        var event = create_testobject("import QtTest 1.0\nimport QtOrganizer 5.0\n"
          + "Event{\n"
          + "  startDateTime: '2010-12-12'\n"
          + "  endDateTime: '2010-12-13'\n"
          + "}\n");
        event.collectionId = toBeDeletedCollection.collectionId;
        modelChangedSpy.clear();
        organizerModel.saveItem(event);
        modelChangedSpy.wait(spyWaitDelay);
        var eventItemId = organizerModel.items[organizerModel.items.length - 1].itemId;
        verify(organizerModel.item(eventItemId));
        organizerModel.removeCollection(toBeDeletedCollection.collectionId);
        collectionsChangedSpy.wait(spyWaitDelay);
        modelChangedSpy.wait(spyWaitDelay);
        wait(noSpyWaitDelay);//waiting for asyncronous operations to finish on backend side
        verify(!organizerModel.item(eventItemId));
        compare(collectionsChangedSpy.count, 7);
        // - remove non-existing
        organizerModel.removeCollection("Missing in action");
        wait(noSpyWaitDelay);// how to utilise SignalSpy to check signal is _not_ emitted?
        compare(organizerModel.collections.length, amountBeforeDeletions - 2);
        compare(collectionsChangedSpy.count, 7);
        // - remove default collection
        organizerModel.removeCollection(organizerModel.defaultCollectionId());
        wait(noSpyWaitDelay);// how to utilise SignalSpy to check signal is _not_ emitted?
        compare(organizerModel.collections.length, amountBeforeDeletions - 2);
        compare(collectionsChangedSpy.count, 7);

        // after all the modifications to collections, default should still be the same
        compare(defCollection.collectionId, organizerModel.defaultCollectionId());

        empty_calendar(organizerModel);
    }
}

