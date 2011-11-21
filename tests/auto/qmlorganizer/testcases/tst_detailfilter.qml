
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    name: "DetailFilterTests"
    id: detailFilterTests

    property bool filteringDataCreated: false
    property bool matchflagsDataCreated: false
    property int spyWaitDelay: 250

    OrganizerModel{
        id: organizerModel
        startPeriod:'2009-01-01'
        endPeriod:'2012-12-31'
    }

    // UTILITIES

    // There is currently some problem with static
    // SignalSpy and changing the target (QTBUG-21083).
    // As a workaround recreating the spy dynamicly.
    function create_spy(targetObj, signalName) {
        var spy = Qt.createQmlObject( "import QtTest 1.0 \nSignalSpy {}", detailFilterTests);
        spy.target = targetObj;
        spy.signalName = signalName;
        return spy;
    }

    function managerList() {
        var managerlist = organizerModel.availableManagers;
        var idx = managerlist.indexOf("invalid"); // Find the index
        if (idx != -1)
            managerlist.splice(idx, 1); // Remove it if really found!
        return managerlist;
    }

    function empty_calendar() {
        var ids = organizerModel.itemIds();
        if (ids.length > 0) {
            organizerModel.removeItems(ids);
            wait(200);
        }
        organizerModel.update();
    }

    function create_testobject(ctorString) {
        var newObject = Qt.createQmlObject(ctorString, detailFilterTests);
        verify(newObject != undefined, 'Object creation failed');
        return newObject;
    }

    function addEventsToModel(ctrStrings) {
        var modelChangedSpy = create_spy(organizerModel, "modelChanged");
        for (var i=0;i<ctrStrings.length;i++) {
            var event = create_testobject(ctrStrings[i]);
            organizerModel.saveItem(event);
            modelChangedSpy.wait();
        }
        wait(200);
    }

    function addDetailWithoutConvenienceAPI(constructionString) {
        // not all details have convenience API
        var modelChangedSpy = create_spy(organizerModel, "modelChanged");
        var detail = create_testobject(constructionString);
        var detailEvent = create_testobject("import QtQuick 2.0\n"
        + "import QtOrganizer 5.0 \n"
        + "   Event {\n"
        + "  startDateTime:new Date('2011-10-25')\n"
        + "  endDateTime:new Date('2011-10-26')\n"
        + "   }");
        detailEvent.setDetail(detail);
        organizerModel.saveItem(detailEvent);
        modelChangedSpy.wait();
    }

    function applyFilter(data) {
        var filterChangedSpy = create_spy(organizerModel, "filterChanged");
        var filter = create_detailFilter();
        filter.detail = data.filterDetail;
        if (data.filterField != undefined)
            filter.field = data.filterField;
        if (data.filterValue != undefined)
            filter.value = data.filterValue;
        if (data.matchFlags != undefined)
            filter.matchFlags = data.matchFlags
        compare(organizerModel.error, "NoError");
        organizerModel.filter = filter;
        filterChangedSpy.wait();
        compare(filterChangedSpy.count, 1);
    }

    // DETAILFILTER OWN API

    function create_detailFilter() {
        var ctorString = "import QtQuick 2.0 \n"
            + "import QtOrganizer 5.0\n"
            + "DetailFilter {\n"
            + "}\n"
        var newDetailFilter = Qt.createQmlObject(ctorString, detailFilterTests);
        verify(newDetailFilter != undefined, 'DetailFilter creation failed');
        return newDetailFilter;
    }

    // creation
    function test_creation_data() {
        return [
                    {tag: "empty definition",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "DetailFilter {\n"
                        + "}\n"
                },
                    {tag: "full definition",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "DetailFilter {\n"
                        + "id: testDetailFilter\n"
                        + "detail:Detail.Description\n"
                        + "field:Description.FieldDescription\n"
                        + "value: 'my value'\n"
                        + "matchFlags: Filter.MatchContains\n"
                        + "}\n"
                }
        ]
    }
    function test_creation(data) {
        console.log("");//to print out test tags for every data set
        var newDetailFilter = create_testobject(data.code, detailFilterTests);
    }

    // detail API
    function test_detail_data() {
        return [
            {tag: "empty field", testValue: -1},
            {tag: "enum detail", testValue: Detail.Priority}
        ]
    }
    function test_detail(data) {
        var detailFilter = create_detailFilter();
        var valueChangedSpy = create_spy(detailFilter, "valueChanged");
        var filterChangedSpy = create_spy(detailFilter, "filterChanged");
        // change
        detailFilter.detail = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFilter.detail, data.testValue);
        // change without change
        detailFilter.detail = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFilter.detail, data.testValue);
    }

    // field API
    function test_field_data() {
        return [
            {tag: "empty field", testValue: -1},
            {tag: "enum field", testValue: Type.FieldType},
        ]
    }
    function test_field(data) {
        var detailFilter = create_detailFilter();
        var valueChangedSpy = create_spy(detailFilter, "valueChanged");
        var filterChangedSpy = create_spy(detailFilter, "filterChanged");
        // change
        detailFilter.field = data.testValue;
        compare(valueChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(filterChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(detailFilter.field, data.testValue);
        // change without change
        detailFilter.field = data.testValue;
        compare(valueChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(filterChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(detailFilter.field, data.testValue);
    }


    // value API
    function test_value_data() {
        return [
            {tag: "empty value", testValue: ""},
            {tag: "string value", testValue: "something"},
            {tag: "numeric detail", testValue: 42},
            // do not fully trust to date test, since java layer is doing also some conversions
            {tag: "date value", testValue: new Date("2010-10-23T15:48:00")}
        ]
    }
    function test_value(data) {
        var detailFilter = create_detailFilter();
        var valueChangedSpy = create_spy(detailFilter, "valueChanged");
        var filterChangedSpy = create_spy(detailFilter, "filterChanged");
        // change
        detailFilter.value = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFilter.value, data.testValue);
        // change without change
        detailFilter.value = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFilter.value, data.testValue);
    }

    // matchflags API - single key
    function test_matchflags_singlekey_data() {
        return [
            {tag: "MatchExactly", key: Filter.MatchExactly},
            {tag: "MatchContains", key: Filter.MatchContains},
            {tag: "MatchStartsWith", key: Filter.MatchStartsWith},
            {tag: "MatchEndsWith", key: Filter.MatchEndsWith},
            {tag: "MatchFixedString", key: Filter.MatchFixedString},
            {tag: "MatchCaseSensitive", key: Filter.MatchCaseSensitive}
        ]
    }
    function test_matchflags_singlekey(data) {
        var detailFilter = create_detailFilter();
        var valueChangedSpy = create_spy(detailFilter, "valueChanged");
        var filterChangedSpy = create_spy(detailFilter, "filterChanged");

        var initialKey = detailFilter.matchFlags;
        compare(initialKey, Filter.MatchExactly);
        var initialEqualsTestedKey = data.key == initialKey;

        // change
        detailFilter.matchFlags = data.key
        compare(valueChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(filterChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(detailFilter.matchFlags, data.key);
        // change without change
        detailFilter.matchFlags = data.key;
        compare(valueChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(filterChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(detailFilter.matchFlags, data.key);
    }

    // matchflags API - combined key
    function test_matchflags_combinedkey_data() {
        return [
            {tag: "same keys - MatchExactly", key1: Filter.MatchExactly, key2: Filter.MatchExactly},
            {tag: "same keys - MatchContains", key1: Filter.MatchContains, key2: Filter.MatchContains},
            {tag: "different keys - MatchExactly & MatchEndsWith", key1: Filter.MatchExactly, key2: Filter.MatchEndsWith},
            {tag: "different keys - MatchCaseSensitive & MatchContains", key1: Filter.MatchCaseSensitive, key2: Filter.MatchContains}
        ]
    }
    function test_matchflags_combinedkey(data) {
        var detailFilter = create_detailFilter();
        var valueChangedSpy = create_spy(detailFilter, "valueChanged");
        var filterChangedSpy = create_spy(detailFilter, "filterChanged");

        var initialKey = detailFilter.matchFlags;
        var combinedKey = data.key1 | data.key2;
        var initialEqualsCombinedKey = initialKey == combinedKey;

        // change - combined key
        detailFilter.matchFlags = combinedKey;
        compare(valueChangedSpy.count, initialEqualsCombinedKey ? 0 : 1);
        compare(filterChangedSpy.count, initialEqualsCombinedKey ? 0 : 1);
        compare(detailFilter.matchFlags, combinedKey);
    }

    // FILTERING TESTS

    function filterTestItems() {
        return [
            // events
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-25'\n"
            + "  endDateTime:'2011-10-26'\n"
            + "  description: 'just basic event'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime: new Date('2011-10-23T16:42:00')\n" //notice, date-object
            + "  description: 'for exact filter - EventTime.StartDateTime'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime: '2011-09-23T16:42:00'\n" //notice, string to date-object
            + "  endDateTime: '2011-10-23T21:00:00'\n" //notice, string to date-object
            + "  description: 'for exact filter - EventTime.EndDateTime'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime: '2011-10-23T15:00:00'\n"
            + "  allDay: true\n"
            + "  description: 'for exact filter - EventTime.AllDayEvent'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n" //notice, string to date-object with Z
            + "  description: 'for exact filter - Description'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  displayLabel: 'my displayLabel'\n"
            + "  description: 'for exact filter - DisplayLabel'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  guid: 'my guid'\n"
            + "  description: 'for exact filter - Guid'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  location: 'my location'\n"
            + "  description: 'for exact filter - Location'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  priority: Priority.VeryHigh\n"
            + "  description: 'for exact filter - Priority'\n"
            + "   }",
            // todos
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime:'2010-10-23T15:00:00Z'\n"
            + "  description: 'for exact filter - Type'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime: new Date('2010-10-23T15:48:00')\n"
            + "  description: 'for exact filter - TodoTime.StartDateTime'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime:new Date('2010-10-22T15:47:00')\n"
            + "  dueDateTime: new Date('2010-10-25T15:49:00Z')\n"//notice, date-object with Z
            + "  description: 'for exact filter - TodoTime.DueDateTime'\n"
            + "   }",
           "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime:new Date('2010-10-22T15:50:00')\n"
            + "  description: 'for exact filter - TodoTime.AllDay'\n"
            + "  isAllDay: true\n"
            + "   }",
        ]
    }

    function test_filtering_data() {
        return [
            {tag: "no filter", expectedItemsAmount: filterTestItems().length },
            {tag: "exact filter - EventTime.StartDateTime", expectedItemsAmount: 1,
                filterDetail: Detail.EventTime, filterField: EventTime.FieldStartDateTime, filterValue: new Date("2011-10-23T16:42:00")},//notice, date-object
            {tag: "exact filter - EventTime.EndDateTime", expectedItemsAmount: 1,
                filterDetail: Detail.EventTime, filterField: EventTime.FieldEndDateTime, filterValue: new Date("2011-10-23T21:00:00")},
            {tag: "exact filter - EventTime.AllDayEvent", expectedItemsAmount: 1, expectedItemsAmount_Jsondb: 2,
                filterDetail: Detail.EventTime, filterField: EventTime.FieldAllDay, filterValue: true},
            {tag: "exact filter - TodoTime.StartDateTime", expectedItemsAmount: 1,
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldStartDateTime, filterValue: new Date("2010-10-23T15:48:00")},
            {tag: "exact filter - TodoTime.DueDateTime", expectedItemsAmount: 1,
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldDueDateTime, filterValue: new Date("2010-10-25T15:49:00Z")},//notice, date-object with Z
            {tag: "exact filter - TodoTime.AllDay", expectedItemsAmount: 1, expectedItemsAmount_Jsondb: 2,
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldAllDay, filterValue: true},
            {tag: "exact filter - Comment", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "my comment",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my comment'\n"
                + "   }"},
            {tag: "exact filter - Description", expectedItemsAmount: 1,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "for exact filter - Description"},
            {tag: "exact filter - DisplayLabel", expectedItemsAmount: 1,
                filterDetail: Detail.DisplayLabel, filterField: DisplayLabel.FieldLabel, filterValue: "my displayLabel"},
            {tag: "exact filter - Guid", expectedItemsAmount: 1,
                filterDetail: Detail.Guid, filterField: Guid.FieldGuid, filterValue: "my guid"},
            {tag: "exact filter - Location.Label", expectedItemsAmount: 1,
                filterDetail: Detail.Location, filterField: Location.FieldLabel, filterValue: "my location"},
            {tag: "exact filter - Location.Longitude", expectedItemsAmount: 1,
                filterDetail: Detail.Location, filterField: Location.FieldLongitude, filterValue: 12345.12345,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Location {\n"
                + "   label: 'my place'\n"
                + "   longitude: 12345.12345\n"
                + "   }"},
            {tag: "exact filter - Location.Latitude", expectedItemsAmount: 1,
                filterDetail: Detail.Location, filterField: Location.FieldLatitude, filterValue: 54321.54321,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Location {\n"
                + "   label: 'my 2nd place'\n"
                + "   latitude: 54321.54321\n"
                + "   }"},
            {tag: "exact filter - Priority", expectedItemsAmount: 1,
                filterDetail: Detail.Priority, filterField: Priority.FieldPriority, filterValue: Priority.VeryHigh},
            {tag: "exact filter - Type enum", expectedItemsAmount: 4,//amount of todos
                filterDetail: Detail.Type, filterField: Type.FieldType, filterValue: Type.Todo},
            {tag: "exact filter - Type string", expectedItemsAmount: 4,//amount of todos
                filterDetail: Detail.Type, filterField: Type.FieldType, filterValue: "Todo"},
            {tag: "exact filter - Tag", expectedItemsAmount: 1,
                filterDetail: Detail.Tag, filterField: Tag.FieldTag, filterValue: "my tag",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Tag {\n"
                + "   tag: 'my tag'\n"
                + "   }"},
            {tag: "exact filter - ExtendedDetail", expectedItemsAmount: 1,
                filterDetail: Detail.Customized, filterField: ExtendedDetail.FieldName, filterValue: "myField",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   ExtendedDetail {\n"
                + "   name: 'myField'\n"
                + "   data: 42\n"
                + "   }"}
        ]
    }

    function test_filtering(data) {
        //preparations
        var managers = managerList();
        for (var i=0;i<managers.length;i++) {

            var managerToBeTested = managers[i];
            console.log("## Testing plugin: " + managerToBeTested);
            organizerModel.manager = managerToBeTested;
            organizerModel.filter = null;
            wait(100);//needed so that OrganizerModel is initialised properly

            if (!detailFilterTests.filteringDataCreated || managerToBeTested == "memory") {
                empty_calendar();
                addEventsToModel(filterTestItems());
                compare(organizerModel.items.length, filterTestItems().length);
                detailFilterTests.filteringDataCreated = true;
            }

            if (data.separateDetailCtrStr) {
                addDetailWithoutConvenienceAPI(data.separateDetailCtrStr);
            }
            if (data.tag != "no filter") {
                applyFilter(data);
                wait(spyWaitDelay);

                //console.log("organizerModel.filter: "+ organizerModel.filter.detail + "/" + organizerModel.filter.field + "/" + organizerModel.filter.value)
                //console.log("filtered amount: " + organizerModel.items.length);
            }

            if (managerToBeTested == "jsondb" ) {
                // in some cases the stored details are utilising the same field on the backend, which means the amount of matches to be different
                compare(organizerModel.items.length, data.expectedItemsAmount_Jsondb ? data.expectedItemsAmount_Jsondb : data.expectedItemsAmount);
            } else {
                compare(organizerModel.items.length, data.expectedItemsAmount);
            }
        }
    }

    // FILTERING MATCHFLAG TESTS

    function matchflagTestItems() {
        return [
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:new Date('2010-10-20')\n"
            + "  description: 'aa bb cc'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:new Date('2011-10-22')\n"
            + "  description: 'bB cc dd'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime:new Date('2010-12-22')\n"
            + "  description: 'cc dd ee'\n"
            + "   }"
        ]
    }

    function test_matchflags_data() {
        return [
            {tag: "Filter set, Description - MatchExactly", expectedItemsAmount: 1,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "aa bb cc",
                matchFlags: Filter.MatchExactly},
            {tag: "Filter set, Description - MatchFixedString", expectedItemsAmount: 0,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "bb",
                matchFlags: Filter.MatchEndsWith | Filter.MatchFixedString},
            {tag: "Filter set, Description - MatchContains case insensitive", expectedItemsAmount: 2,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "bb",
                matchFlags: Filter.MatchContains},
            {tag: "Filter set, Description - MatchContains case sensitive", expectedItemsAmount: 1,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "bb",
                matchFlags: Filter.MatchContains | Filter.MatchCaseSensitive},
            {tag: "Filter set, Description - MatchStartsWith", expectedItemsAmount: 1,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "bb",
                matchFlags: Filter.MatchStartsWith},
            {tag: "Filter set, Description - MatchEndsWith", expectedItemsAmount: 1,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "ee",
                matchFlags: Filter.MatchEndsWith},
            {tag: "Filter set, Comment - MatchExactly", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "my great comment",
                matchFlags: Filter.MatchExactly,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my great comment'\n"
                + "   }"},
            {tag: "Filter set, Comment - MatchStartsWith", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "my",
                matchFlags: Filter.MatchStartsWith,
                // jsondb, depends on detail created in previous data set
                // mem, needs to create again
                separateDetailCtrStrMemory: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my great comment'\n"
                + "   }"},
            {tag: "Filter set, Comment - MatchContains", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "great",
                matchFlags: Filter.MatchContains,
                // jsondb, depends on detail created in previous data set
                // mem, needs to create again
                separateDetailCtrStrMemory: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my great comment'\n"
                + "   }"},
            {tag: "Filter set, Comment - MatchEndsWith", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "comment",
                matchFlags: Filter.MatchEndsWith,
                // jsondb, depends on detail created in previous data set
                // mem, needs to create again
                separateDetailCtrStrMemory: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my great comment'\n"
                + "   }"},
            {tag: "Filter set, Tag - MatchContains", expectedItemsAmount: 1,
                filterDetail: Detail.Tag, filterField: Tag.FieldTag, filterValue: "excellent",
                matchFlags: Filter.MatchContains,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Tag {\n"
                + "   tag: 'my excellent tag'\n"
                + "   }"},
            {tag: "Filter set, Description - tighten filter", expectedItemsAmount: 2,
                filterDetail: Detail.Description, filterField: Description.FieldDescription, filterValue: "bb",
                matchFlags: Filter.MatchContains, matchFlagsTighter: Filter.MatchContains | Filter.MatchCaseSensitive}
        ]
    }

    function test_matchflags(data) {
        //preparations
        var managers = managerList();
        for (var i=0;i<managers.length;i++) {

            var managerToBeTested = managers[i];
            console.log("## Testing plugin: " + managerToBeTested);
            organizerModel.manager = managerToBeTested;
            organizerModel.filter = null;
            wait(100);//needed so that OrganizerModel is initialised properly

            if (!detailFilterTests.matchflagsDataCreated || managerToBeTested == "memory") {
                empty_calendar();
                addEventsToModel(matchflagTestItems());
                compare(organizerModel.items.length, matchflagTestItems().length);
                detailFilterTests.matchflagsDataCreated = true;
            }

            // for all backends
            if (data.separateDetailCtrStr) {
                addDetailWithoutConvenienceAPI(data.separateDetailCtrStr);
            }
            // for memory backend tests
            if (data.separateDetailCtrStrMemory && managerToBeTested == "memory") {
                addDetailWithoutConvenienceAPI(data.separateDetailCtrStrMemory);
            }

            applyFilter(data);
            wait(spyWaitDelay);

            //console.log("organizerModel.filter: "+ organizerModel.filter.detail + "/" + organizerModel.filter.field + "/" + organizerModel.filter.value + "/" + organizerModel.filter.matchFlags)
            //console.log("filtered amount: " + organizerModel.items.length);
            expectFail("Filter set, Comment - MatchStartsWith", "\nNot working before indexing starts to work");
            expectFail("Filter set, Comment - MatchContains", "\nNot working before indexing starts to work");
            expectFail("Filter set, Comment - MatchEndsWith", "\nNot working before indexing starts to work");
            expectFail("Filter set, Tag - MatchContains", "\nNot working before indexing starts to work");
            compare(organizerModel.items.length, data.expectedItemsAmount);

            if (data.matchFlagsTighter) {
                // testing the case where we have had a filter and then were making the filtering rules tighter
                var filterChangedSpy = create_spy(organizerModel, "filterChanged");
                organizerModel.filter.matchFlags = data.matchFlagsTighter;
                filterChangedSpy.wait();
                wait(spyWaitDelay);
                compare(organizerModel.items.length, data.expectedItemsAmount - 1);
            }
        }
    }

    function test_errors_data() {
        return [
            // filtering certain details/definitions are currently not supported
            {tag: "Unsupported details, JournalTime", filterDetail: Detail.JournalTime },
            {tag: "Unsupported details, TodoProgress", filterDetail: Detail.TodoProgress },
            {tag: "Unsupported details, Reminder", filterDetail: Detail.Reminder },
            {tag: "Unsupported details, AudibleReminder", filterDetail: Detail.AudibleReminder },
            {tag: "Unsupported details, VisualReminder", filterDetail: Detail.VisualReminder },
            {tag: "Unsupported details, EmailReminder", filterDetail: Detail.EmailReminder },
            {tag: "Unsupported details, Parent", filterDetail: Detail.Parent },
            {tag: "Unsupported details, Recurrence", filterDetail: Detail.Recurrence },
            {tag: "Unsupported details, Timestamp", filterDetail: Detail.Timestamp },
            // filtering matchflags are not supported for all the types
            {tag: "Unsupported matchflag, EventTime", filterDetail: Detail.EventTime, matchFlags: Filter.MatchContains },
            {tag: "Unsupported matchflag, TodoTime", filterDetail: Detail.TodoTime, matchFlags: Filter.MatchStartsWith },
            {tag: "Unsupported matchflag, Comment", filterDetail: Detail.Comment, matchFlags: Filter.MatchContains },
            {tag: "Unsupported matchflag, Location - Longitude", filterDetail: Detail.Location,
                filterField: Location.FieldLongitude, matchFlags: Filter.MatchEndsWith },
            {tag: "Unsupported matchflag, Location - Latitude", filterDetail: Detail.Location,
                filterField: Location.FieldLatitude, matchFlags: Filter.MatchEndsWith },
            {tag: "Unsupported matchflag, Priority", filterDetail: Detail.Priority, matchFlags: Filter.MatchStartsWith },
            {tag: "Unsupported matchflag, Type", filterDetail: Detail.Type, matchFlags: Filter.MatchFixedString },
            {tag: "Unsupported matchflag, Tag", filterDetail: Detail.Tag, matchFlags: Filter.MatchCaseSensitive },
            {tag: "Unsupported matchflag, ExtendedDetail", filterDetail: Detail.Customized, matchFlags: Filter.MatchFixedString },
            // filtering with QString needs extra attention, not allowed for all the types
            {tag: "Unsupported string value, EventTime - StartDateTime",
                filterDetail: Detail.EventTime, filterField: EventTime.FieldStartDateTime, filterValue: "mystringie" },
            {tag: "Unsupported string value, EventTime - EndDateTime",
                filterDetail: Detail.EventTime, filterField: EventTime.FieldEndDateTime, filterValue: "mystringie" },
            {tag: "Unsupported string value, EventTime - AllDay",
                filterDetail: Detail.EventTime, filterField: EventTime.FieldAllDay, filterValue: "mystringie" },
            {tag: "Unsupported string value, JournalTime - EntryDateTime",
                filterDetail: Detail.JournalTime, filterField: Journal.FieldEntryDateTime, filterValue: "mystringie" },
            {tag: "Unsupported string value, TodoTime - StartDateTime",
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldStartDateTime, filterValue: "mystringie" },
            {tag: "Unsupported string value, TodoTime - DueDateTime",
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldDueDateTime, filterValue: "mystringie" },
            {tag: "Unsupported string value, TodoTime - AllDay",
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldAllDay, filterValue: "mystringie" },
            // missing TodoProgress-, all different Reminders -details/fields (tested already on Unsupported details-part)
            {tag: "Unsupported string value, Location - Longitude",
                filterDetail: Detail.Location, filterField: Location.FieldLongitude, filterValue: "mystringie" },
            {tag: "Unsupported string value, Location - Latitude",
                filterDetail: Detail.Location, filterField: Location.FieldLatitude, filterValue: "mystringie" },
            // missing Parent-detail/field (tested already on Unsupported details-part)
            {tag: "Unsupported string value, Priority",
                filterDetail: Detail.Priority, filterField: Priority.FieldPriority, filterValue: "mystringie" },
            // missing Recurrence-, TimeStamp-details/fields (tested already on Unsupported details-part)
        ]
    }

    function test_errors(data) {
        console.log();
        //preparations
        // error codes are backend specific, these are tested only for jsondb
        organizerModel.manager = "jsondb";
        organizerModel.filter = null;
        wait(100);//needed so that OrganizerModel is initialised properly

        var errorChangedSpy = create_spy(organizerModel, "errorChanged");
        applyFilter(data);
        errorChangedSpy.wait();
        wait(50);//why needed?

        compare(organizerModel.error, "BadArgument");
    }

    function cleanupTestCase() {
        empty_calendar();
        console.log("cleaned jsondb up!");
    }
}
