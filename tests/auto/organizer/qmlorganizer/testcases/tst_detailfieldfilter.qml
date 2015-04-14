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
    name: "detailFieldFilterTests"
    id: detailFieldFilterTests

    property OrganizerModel organizerModel;
    property int spyWaitDelay: 250

    QOrganizerTestUtility {
        id: utility
    }

    // UTILITIES
    function create_testobject(ctorString) {
        var newObject = Qt.createQmlObject(ctorString, detailFieldFilterTests);
        verify(newObject != undefined, 'Object creation failed');
        return newObject;
    }

    function addEventsToModel(ctrStrings) {
        for (var i=0;i<ctrStrings.length;i++) {
            var event = create_testobject(ctrStrings[i]);
            organizerModel.saveItem(event);
            utility.waitModelChange();
        }
    }

    function addDetailWithoutConvenienceAPI(constructionString) {
        // not all details have convenience API
        var modelChangedSpy = utility.create_spy(organizerModel, "modelChanged");
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
        var filterChangedSpy = utility.create_spy(organizerModel, "filterChanged");
        var modelChangedSpy = utility.create_spy(organizerModel, "modelChanged");
        var filter = create_detailFieldFilter();
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
        modelChangedSpy.wait(spyWaitDelay)
        compare(modelChangedSpy.count, 1);
    }

    // DETAILFIELDFILTER OWN API

    function create_detailFieldFilter() {
        var ctorString = "import QtQuick 2.0 \n"
            + "import QtOrganizer 5.0\n"
            + "DetailFieldFilter {\n"
            + "}\n"
        var newDetailFieldFilter = Qt.createQmlObject(ctorString, detailFieldFilterTests);
        verify(newDetailFieldFilter != undefined, 'DetailFieldFilter creation failed');
        return newDetailFieldFilter;
    }

    // creation
    function test_creation_data() {
        return [
                    {tag: "empty definition",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "DetailFieldFilter {\n"
                        + "}\n"
                },
                    {tag: "full definition",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "DetailFieldFilter {\n"
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
        var newDetailFieldFilter = create_testobject(data.code, detailFieldFilterTests);
    }

    // detail API
    function test_detail_data() {
        return [
            {tag: "empty field", testValue: -1},
            {tag: "enum detail", testValue: Detail.Priority}
        ]
    }
    function test_detail(data) {
        var detailFieldFilter = create_detailFieldFilter();
        var valueChangedSpy = utility.create_spy(detailFieldFilter, "valueChanged");
        var filterChangedSpy = utility.create_spy(detailFieldFilter, "filterChanged");
        // change
        detailFieldFilter.detail = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFieldFilter.detail, data.testValue);
        // change without change
        detailFieldFilter.detail = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFieldFilter.detail, data.testValue);
    }

    // field API
    function test_field_data() {
        return [
            {tag: "empty field", testValue: -1},
            {tag: "enum field", testValue: Type.FieldType},
        ]
    }
    function test_field(data) {
        var detailFieldFilter = create_detailFieldFilter();
        var valueChangedSpy = utility.create_spy(detailFieldFilter, "valueChanged");
        var filterChangedSpy = utility.create_spy(detailFieldFilter, "filterChanged");
        // change
        detailFieldFilter.field = data.testValue;
        compare(valueChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(filterChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(detailFieldFilter.field, data.testValue);
        // change without change
        detailFieldFilter.field = data.testValue;
        compare(valueChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(filterChangedSpy.count, data.testValue == -1 ? 0 : 1);
        compare(detailFieldFilter.field, data.testValue);
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
        var detailFieldFilter = create_detailFieldFilter();
        var valueChangedSpy = utility.create_spy(detailFieldFilter, "valueChanged");
        var filterChangedSpy = utility.create_spy(detailFieldFilter, "filterChanged");
        // change
        detailFieldFilter.value = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFieldFilter.value, data.testValue);
        // change without change
        detailFieldFilter.value = data.testValue;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(detailFieldFilter.value, data.testValue);
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
        var detailFieldFilter = create_detailFieldFilter();
        var valueChangedSpy = utility.create_spy(detailFieldFilter, "valueChanged");
        var filterChangedSpy = utility.create_spy(detailFieldFilter, "filterChanged");

        var initialKey = detailFieldFilter.matchFlags;
        compare(initialKey, Filter.MatchExactly);
        var initialEqualsTestedKey = data.key == initialKey;

        // change
        detailFieldFilter.matchFlags = data.key
        compare(valueChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(filterChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(detailFieldFilter.matchFlags, data.key);
        // change without change
        detailFieldFilter.matchFlags = data.key;
        compare(valueChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(filterChangedSpy.count, initialEqualsTestedKey ? 0 : 1);
        compare(detailFieldFilter.matchFlags, data.key);
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
        var detailFieldFilter = create_detailFieldFilter();
        var valueChangedSpy = utility.create_spy(detailFieldFilter, "valueChanged");
        var filterChangedSpy = utility.create_spy(detailFieldFilter, "filterChanged");

        var initialKey = detailFieldFilter.matchFlags;
        var combinedKey = data.key1 | data.key2;
        var initialEqualsCombinedKey = initialKey == combinedKey;

        // change - combined key
        detailFieldFilter.matchFlags = combinedKey;
        compare(valueChangedSpy.count, initialEqualsCombinedKey ? 0 : 1);
        compare(filterChangedSpy.count, initialEqualsCombinedKey ? 0 : 1);
        compare(detailFieldFilter.matchFlags, combinedKey);
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
            + "  endDateTime: new Date('2011-10-23T17:42:00')\n" //notice, date-object
            + "  description: 'for exact filter - EventTime.StartDateTime'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime: '2011-09-23T16:42:00'\n" //notice, string to date-object
            + "  endDateTime: new Date('2011-10-23T21:00:00')\n"
            + "  description: 'for exact filter - EventTime.EndDateTime'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime: '2011-10-23T15:00:00'\n"
            + "  endDateTime: '2011-10-23T16:00:00'\n"
            + "  allDay: true\n"
            + "  description: 'for exact filter - EventTime.AllDayEvent'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00'\n"
            + "  endDateTime: '2011-11-23T16:00:00'\n"
            + "  description: 'for exact filter - Description'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  endDateTime: '2011-11-23T16:00:00'\n"
            + "  displayLabel: 'my displayLabel'\n"
            + "  description: 'for exact filter - DisplayLabel'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  endDateTime: '2011-11-23T16:00:00'\n"
            + "  guid: 'my guid'\n"
            + "  description: 'for exact filter - Guid'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  endDateTime: '2011-11-23T16:00:00'\n"
            + "  location: 'my location'\n"
            + "  description: 'for exact filter - Location'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:'2011-10-23T15:00:00Z'\n"
            + "  endDateTime: '2011-11-23T16:00:00'\n"
            + "  priority: Priority.VeryHigh\n"
            + "  description: 'for exact filter - Priority'\n"
            + "   }",
            // todos
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  dueDateTime:'2010-10-23T15:00:00Z'\n"
            + "  status:TodoProgress.InProgress\n"
            + "  percentageComplete:89\n"
            + "  description: 'for exact filter - Type'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime: new Date('2010-10-23T15:48:00')\n"
            + "  percentageComplete:89\n"
            + "  description: 'for exact filter - TodoTime.StartDateTime'\n"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime:new Date('2010-10-22T15:47:00')\n"
            + "  dueDateTime: new Date('2010-10-25T15:49:00')\n"
            + "  description: 'for exact filter - TodoTime.DueDateTime'\n"
            + "   }",
           "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  startDateTime:new Date('2010-10-22T15:50:00')\n"
            + "  description: 'for exact filter - TodoTime.AllDay'\n"
            + "  allDay: true\n"
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
            {tag: "exact filter - EventTime.AllDayEvent", expectedItemsAmount: 1,
                filterDetail: Detail.EventTime, filterField: EventTime.FieldAllDay, filterValue: true},
            {tag: "exact filter - TodoTime.StartDateTime", expectedItemsAmount: 1,
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldStartDateTime, filterValue: new Date("2010-10-23T15:48:00")},
            {tag: "exact filter - TodoTime.DueDateTime", expectedItemsAmount: 1,
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldDueDateTime, filterValue: new Date("2010-10-25T15:49:00")},
            {tag: "exact filter - TodoTime.AllDay", expectedItemsAmount: 1,
                filterDetail: Detail.TodoTime, filterField: TodoTime.FieldAllDay, filterValue: true},
            {tag: "exact filter - TodoProgress.Status", expectedItemsAmount: 1,
                filterDetail: Detail.TodoProgress, filterField: TodoProgress.FieldStatus, filterValue: TodoProgress.InProgress},
            {tag: "exact filter - TodoProgress.PercentageComplete", expectedItemsAmount: 2,
                filterDetail: Detail.TodoProgress, filterField: TodoProgress.FieldPercentageComplete, filterValue: 89},
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
                filterDetail: Detail.ItemType, filterField: Type.FieldType, filterValue: Type.Todo},
            {tag: "exact filter - Tag", expectedItemsAmount: 1,
                filterDetail: Detail.Tag, filterField: Tag.FieldTag, filterValue: "my tag",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Tag {\n"
                + "   tag: 'my tag'\n"
                + "   }"},
            {tag: "exact filter - ExtendedDetail", expectedItemsAmount: 1,
                filterDetail: Detail.ExtendedDetail, filterField: ExtendedDetail.FieldName, filterValue: "myField",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   ExtendedDetail {\n"
                + "   name: 'myField'\n"
                + "   data: 42\n"
                + "   }"},
            {tag: "exact filter - EventRsvp.ParticipationStatus", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldParticipationStatus, filterValue: EventAttendee.StatusAccepted,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        participationStatus: EventAttendee.StatusAccepted\n"
                + "    }"},
            {tag: "exact filter - EventRsvp.ParticipationRole", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldParticipationRole, filterValue: EventAttendee.RoleHost,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        participationRole: EventAttendee.RoleHost\n"
                + "    }"},
            {tag: "exact filter - EventRsvp.ResponseRequirement", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldResponseRequirement, filterValue: EventRsvp.ResponseRequired,
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        responseRequirement: EventRsvp.ResponseRequired\n"
                + "    }"},
            {tag: "exact filter - EventRsvp.ResponseDeadline", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldResponseDeadline, filterValue: new Date("2008-12-28"),
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        responseDeadline: new Date(\"2008-12-28\")\n"
                + "    }"},
            {tag: "exact filter - EventRsvp.ResponseDate", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldResponseDate, filterValue: new Date("2008-12-28"),
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        responseDate: new Date(\"2008-12-28\")\n"
                + "    }"},
            {tag: "exact filter - EventRsvp.OrganizerName", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerName, filterValue: "QtPim",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        organizerName: \"QtPim\"\n"
                + "    }"},
            {tag: "exact filter - EventRsvp.OrganizerEmail", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerEmail, filterValue: "pim@qt-project.org",
                separateDetailCtrStr: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "    EventRsvp {\n"
                + "        organizerEmail: \"pim@qt-project.org\"\n"
                + "    }"}
        ]
    }

    function test_filtering(data) {
        //preparations
        var managers = utility.getManagerList();
        for (var i=0;i<managers.length;i++) {

            var managerToBeTested = managers[i];
            console.log("## Testing plugin: " + managerToBeTested);
            organizerModel = utility.createModel(managerToBeTested);

            addEventsToModel(filterTestItems());
            compare(organizerModel.items.length, filterTestItems().length);

            if (data.separateDetailCtrStr) {
                addDetailWithoutConvenienceAPI(data.separateDetailCtrStr);
            }
            if (data.tag != "no filter") {
                applyFilter(data);
            }

            compare(organizerModel.items.length, data.expectedItemsAmount);

            organizerModel.destroy();
        }
    }

    // FILTERING MATCHFLAG TESTS

    function matchflagTestItems() {
        return [
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:new Date('2010-10-20')\n"
            + "  endDateTime:new Date('2010-10-21')\n"
            + "  description: 'aa bb cc'\n"
            + "  EventRsvp {\n"
            + "    organizerName: 'QtPim'\n"
            + "    organizerEmail: 'pim@qt-project.org'\n"
            + "  }"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "  startDateTime:new Date('2011-10-22')\n"
            + "  endDateTime:new Date('2011-10-23')\n"
            + "  description: 'bB cc dd'\n"
            + "  EventRsvp {\n"
            + "    organizerName: 'random organizer name'\n"
            + "    organizerEmail: 'random organizer email'\n"
            + "  }"
            + "   }",
            "import QtQuick 2.0\n"
            + "import QtOrganizer 5.0 \n"
            + "   Todo {\n"
            + "  dueDateTime:new Date('2010-12-22')\n"
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
                // mem, needs to create again
                separateDetailCtrStrMemory: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my great comment'\n"
                + "   }"},
            {tag: "Filter set, Comment - MatchContains", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "great",
                matchFlags: Filter.MatchContains,
                // mem, needs to create again
                separateDetailCtrStrMemory: "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0 \n"
                + "   Comment {\n"
                + "   comment: 'my great comment'\n"
                + "   }"},
            {tag: "Filter set, Comment - MatchEndsWith", expectedItemsAmount: 1,
                filterDetail: Detail.Comment, filterField: Comment.FieldComment, filterValue: "comment",
                matchFlags: Filter.MatchEndsWith,
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
                matchFlags: Filter.MatchContains, matchFlagsTighter: Filter.MatchContains | Filter.MatchCaseSensitive},
            {tag: "Filter set, EventRsvp.OrganizerName - MatchExactly", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerName, filterValue: "QtPim",
                matchFlags: Filter.MatchExactly},
            {tag: "Filter set, EventRsvp.OrganizerName - MatchStartsWith", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerName, filterValue: "Qt",
                matchFlags: Filter.MatchStartsWith},
            {tag: "Filter set, EventRsvp.OrganizerName - MatchContains", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerName, filterValue: "tP",
                matchFlags: Filter.MatchContains},
            {tag: "Filter set, EventRsvp.OrganizerName - MatchEndsWith", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerName, filterValue: "im",
                matchFlags: Filter.MatchEndsWith},
            {tag: "Filter set, EventRsvp.OrganizerEmail - MatchExactly", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerEmail, filterValue: "pim@qt-project.org",
                matchFlags: Filter.MatchExactly},
            {tag: "Filter set, EventRsvp.OrganizerEmail - MatchStartsWith", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerEmail, filterValue: "pim",
                matchFlags: Filter.MatchStartsWith},
            {tag: "Filter set, EventRsvp.OrganizerEmail - MatchContains", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerEmail, filterValue: "@qt-",
                matchFlags: Filter.MatchContains},
            {tag: "Filter set, EventRsvp.OrganizerEmail - MatchEndsWith", expectedItemsAmount: 1,
                filterDetail: Detail.EventRsvp, filterField: EventRsvp.FieldOrganizerEmail, filterValue: ".org",
                matchFlags: Filter.MatchEndsWith}
        ]
    }

    function test_matchflags(data) {
        //preparations
        var managers = utility.getManagerList();

        for (var i=0;i<managers.length;i++) {

            var managerToBeTested = managers[i];
            console.log("## Testing plugin: " + managerToBeTested);
            organizerModel = utility.createModel(managerToBeTested);

            addEventsToModel(matchflagTestItems());
            compare(organizerModel.items.length, matchflagTestItems().length);

            // for all backends
            if (data.separateDetailCtrStr) {
                addDetailWithoutConvenienceAPI(data.separateDetailCtrStr);
            }
            // for memory backend tests
            if (data.separateDetailCtrStrMemory && managerToBeTested == "memory") {
                addDetailWithoutConvenienceAPI(data.separateDetailCtrStrMemory);
            }

            applyFilter(data);

            compare(organizerModel.items.length, data.expectedItemsAmount);

            if (data.matchFlagsTighter) {
                // testing the case where we have had a filter and then were making the filtering rules tighter
                var filterChangedSpy = utility.create_spy(organizerModel, "filterChanged");
                var modelChangedSpy = utility.create_spy(organizerModel, "modelChanged");
                organizerModel.filter.matchFlags = data.matchFlagsTighter;
                filterChangedSpy.wait();
                modelChangedSpy.wait(spyWaitDelay);
                compare(filterChangedSpy.count, 1);
                compare(modelChangedSpy.count, 1);
                compare(organizerModel.items.length, data.expectedItemsAmount - 1);
            }

            organizerModel.destroy();
        }
    }

    function test_errors_data() {
        return [
            // filtering certain details/definitions are currently not supported
            {tag: "Unsupported details, JournalTime", filterDetail: Detail.JournalTime },
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
            {tag: "Unsupported matchflag, TodoProgress", filterDetail: Detail.TodoProgress, matchFlags: Filter.MatchStartsWith },
            {tag: "Unsupported matchflag, Comment", filterDetail: Detail.Comment, matchFlags: Filter.MatchContains },
            {tag: "Unsupported matchflag, Location - Longitude", filterDetail: Detail.Location,
                filterField: Location.FieldLongitude, matchFlags: Filter.MatchEndsWith },
            {tag: "Unsupported matchflag, Location - Latitude", filterDetail: Detail.Location,
                filterField: Location.FieldLatitude, matchFlags: Filter.MatchEndsWith },
            {tag: "Unsupported matchflag, Priority", filterDetail: Detail.Priority, matchFlags: Filter.MatchStartsWith },
            {tag: "Unsupported matchflag, Type", filterDetail: Detail.ItemType, matchFlags: Filter.MatchFixedString },
            {tag: "Unsupported matchflag, Tag", filterDetail: Detail.Tag, matchFlags: Filter.MatchCaseSensitive },
            {tag: "Unsupported matchflag, ExtendedDetail", filterDetail: Detail.ExtendedDetail, matchFlags: Filter.MatchFixedString },
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
            {tag: "Unsupported string value, TodoProgress - FinishedDateTime",
                filterDetail: Detail.TodoProgress, filterField: TodoProgress.FieldFinishedDateTime, filterValue: "mystringie" },
            {tag: "Unsupported string value, TodoProgress - Status",
                filterDetail: Detail.TodoProgress, filterField: TodoProgress.FieldStatus, filterValue: "mystringie" },
            {tag: "Unsupported string value, TodoProgress - PercentageComplete",
                filterDetail: Detail.TodoProgress, filterField: TodoProgress.FieldPercentageComplete, filterValue: "mystringie" },
            // missing all different Reminders -details/fields (tested already on Unsupported details-part)
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
        // error codes are backend specific - currently untested
    }
}
