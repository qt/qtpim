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

//TESTED_COMPONENT=src/organizer

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    name: "ModelTests"
    id:modelTests

    property var signalWaitTime : 300

    QOrganizerTestUtility {
        id: utility
    }

    property Rectangle rect: Rectangle {
        id:myRectangle
    }

    function test_componentCreation_data() {
        return [
                // OrganizerModel
                {tag: "No properties",
                 code: "import QtQuick 2.0\n"
                    + "import QtOrganizer 5.0 \n"
                    + "   OrganizerModel {\n"
                    + "   }"
                },
                {tag: "Only id property",
                 code: "import QtQuick 2.0\n"
                   + "import QtOrganizer 5.0\n"
                   + "    OrganizerModel {\n"
                   + "        id:organizerModelId\n"
                   + "     }\n"
                },
                {tag: "Valuetype properties",
                 code: "import QtQuick 2.0\n"
                   + "import QtOrganizer 5.0\n"
                   + "    OrganizerModel {\n"
                   + "        id:organizerModelId\n"
                   + "        manager:'memory'\n"
                   + "        startPeriod:'2010-08-12T13:22:01'\n"
                   + "        endPeriod:'2010-09-12T13:22:01'\n"
                   + "     }\n"
                },
                {tag: "With filter",
                 code: "import QtOrganizer 5.0\n"
                   + "OrganizerModel {\n"
                   + "    id:organizerModelId\n"
                   + "    filter:DetailFieldFilter{\n"
                   + "        id:filter\n"
                   + "        field:EventTime.FieldStartDateTime\n"
                   + "        value:'2010-08-12T13:22:01'\n"
                   + "    }\n"
                   + "}\n"
                },
                {tag: "With invalid filter",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerModel {\n"
                    + "   id:organizerModelId\n"
                    + "   filter:InvalidFilter{\n"
                    + "       id:filter\n"
                    + "   }\n"
                    + "}\n"
                },
                {tag: "With range filter",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerModel {\n"
                    + "   id:organizerModelId\n"
                    + "   filter:DetailRangeFilter{\n"
                    + "       id:filter\n"
                    + "       field:EventTime.FieldStartDateTime\n"
                    + "       min:'2010-08-12T13:22:01'\n"
                    + "       max:'2010-09-12T13:22:01'\n"
                    + "   }\n"
                    + "}\n"
                },
                {tag: "With collection filter",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerModel {\n"
                    + "   id:organizerModelId\n"
                    + "   filter:CollectionFilter{\n"
                    + "       id:filter\n"
                    + "       ids:['1234', '456', '90']\n"
                    + "   }\n"
                    + "}\n"
                },
                {tag: "With intersection filter",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerModel {\n"
                    + "   id:organizerModelId\n"
                    + "   filter:IntersectionFilter {\n"
                    + "       id:filter\n"
                    + "       filters:[\n"
                    + "           DetailFieldFilter{\n"
                    + "               id:filter1\n"
                    + "               field:EventTime.FieldStartDateTime\n"
                    + "               value:'2010-08-12T13:22:01'\n"
                    + "           },\n"
                    + "           DetailRangeFilter{\n"
                    + "               id:filter2\n"
                    + "               field:EventTime.FieldStartDateTime\n"
                    + "               min:'2010-08-12T13:22:01'\n"
                    + "               max:'2010-09-12T13:22:01'\n"
                    + "           }\n"
                    + "      ]\n"
                    + "   }\n"
                    + "}\n"
                },
                {tag: "With fetchHint",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerModel {\n"
                    + "    id:organizerModelId\n"
                    + "    fetchHint:FetchHint {\n"
                    + "        id:hint\n"
                    + "        optimizationHints:FetchHint.AllRequired\n"
                    + "    }\n"
                    + "}\n"
                },

                // Organizer Items
                {tag: "Base organizer item",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerItem {\n"
                    + "}\n"
                },
                {tag: "Base organizer item: only id",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerItem {\n"
                    + "    id:organizerItem\n"
                    + "}\n"
                },
                {tag: "Base organizer item: Valuetype properties",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerItem {\n"
                    + "    id:organizerItem\n"
                    + "    displayLabel:'test item'\n"
                    + "    description:'item description'\n"
                    + "    guid:'1112232133'\n"
                    + "}\n"
                },

                {tag: "Base organizer item: default property",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerItem {\n"
                    + "    id:organizerItem\n"
                    + "    DisplayLabel {\n"
                    + "        label:'test item'\n"
                    + "    }\n"
                    + "    Description {\n"
                    + "        description:'item description'\n"
                    + "    }\n"
                    + "    Guid{\n"
                    + "        guid:'111223213'\n"
                    + "    }\n"
                    + "}\n"
                },

                //Event
                {tag: "Organizer event",
                 code: "import QtOrganizer 5.0\n"
                    + "Event {\n"
                    + "}\n"
                },
                /*{tag: "Organizer event:Valuetype properties",
                 code: "import QtOrganizer 5.0 \n"
                    + "Event {\n"
                    + "    id:organizerEvent\n"
                    + "    displayLabel:'meeting'\n"
                    + "    startDateTime:'2010-08-12T13:00:00'\n"
                    + "    endDateTime:'2010-08-12T15:00:00'\n"
                    + "    allDay:false\n"
                    + "    location:'office'\n"
                    + "    Location {\n"
                    + "        label:'53 Brandl st'\n"
                    + "        latitude:-27.579570\n"
                    + "        longitude:153.10031\n"
                    + "    }\n"
                    + "    priority:Priority.Low\n"
                    + "    recurrence.recurrenceRules:[\n"
                    + "        RecurrenceRule {\n"
                    + "        }\n"
                    + "    ]\n"
                    + "    recurrence.recurrenceDates:[]\n"
                    + "    recurrence.exceptionDates:[]\n"
                    + "}\n"
                },*/
                /*
                {tag: "",
                 code: ""
                },
                */

        ]
    }

    function test_componentCreation(data)
    {
        try{
            var obj = Qt.createQmlObject(data.code, myRectangle, "dynamicSnippet1");
            verify(obj != undefined, "Unable to load script for " + data.tag);
            obj.destroy();
            console.log("Completed test on '" + data.tag + "'")
        }
        catch (errorObject)  {
            console.log("For code " + data.code + " Error was seen is " +  errorObject );
        }

    }

    function checkDetails(events)
    {
        var testDataEvents = test_addEvent_data()[0].events
        var foundEvent = false
        var testDataEvent = undefined
        for (var index = 0; index < testDataEvents.length; index++){
            testDataEvent = testDataEvents[index]
            foundEvent = false
            console.log("Checking event " + testDataEvent.subject)
            for (var index2 = 0; index2 < events.length; index2++){
                var actualEvent = events[index2];
                if (testDataEvent.subject == actualEvent.description){
                    foundEvent = true
                    compare(actualEvent.type, testDataEvent.type)
                    compare(actualEvent.startDateTime.toString(), testDataEvent.startDateTime.toString())
                    compare(actualEvent.endDateTime.toString(), testDataEvent.endDateTime.toString())
                    if (testDataEvent.repeat != undefined){
                        verify(actualEvent.recurrence != undefined, "Expected recurrance Element to be present for event")
                        verify(actualEvent.recurrence.recurrenceRules.length == 1, "Expected there to be one RecurranceRule")
                        var recurranceRule = actualEvent.recurrence.recurrenceRules[0] // first rule only
                        switch (event.repeat){
                        case RecurrenceRule.Daily:
                        case RecurrenceRule.Weekly:
                        case RecurrenceRule.Monthly:
                        case RecurrenceRule.Yearly:
                            compare(recurranceRule.frequency, RecurrenceRule.Weekly)
                            for (var dayIndex=0; dayIndex < testDataEvent.repeatDays; dayIndex++){
                                verify(recurranceRule.daysOfWeek.indexOf(testDataEvent.repeatDays[dayIndex]) != -1, "Expected event to recurrence on day " + testDataEvent.repeatDays[dayIndex])
                            }
                            compare(recurranceRule.limit.toString(), testDataEvent.limit.toString())
                        break
                        default:
                            if (event.repeat != RecurrenceRule.Invalid)
                                console.log("Unsupported repeat for Event actual:" + actualEvent.reoccurance + " expected:" + testDataEvent.repeat);
                        }
                    }
                    break
                }
            }
            //verify(foundEvent, "Did not find event " + testDataEvent.subject)
        }
        console.log("Done checking Event")
    }

    // test data is handled by addEvent.qml function addEvents()
    function test_addEvent_data() {
        return [
            {tag: "Event set 1",
                events:[
                    {tag: "Event 1",
                     type: Type.Event,
                     subject: "Event 1",
                     description: "starts 2010-12-09 8AM finishes 5PM",
                     startDateTime: new Date(2010, 12, 9, 8, 0),
                     endDateTime: new Date(2010, 12, 9, 17, 0),
                    },
                    {tag: "Event 2",
                     type: Type.Event,
                     subject: "Event 2",
                     description: "starts 2010-12-08 8AM finishes 1PM",
                     startDateTime: new Date(2010, 12, 8, 8, 0),
                     endDateTime: new Date(2010, 12, 8, 13, 0),
                    },
                    {tag: "Event 3",
                     type: Type.Event,
                     subject: "Event 3",
                     description: "starts a month from 2010-12-08 at 11AM finish at 2PM",
                     startDateTime: new Date(2010, 12, 8, 11, 0),
                     endDateTime: new Date(2010, 12, 8, 14, 0),
                    },
                    {tag: "Event 4",
                     type: Type.Event,
                     subject: "Event 4",
                     description: "starts after Event 3 and finishes 4PM",
                     startDateTime: new Date(2010, 12, 8, 14, 0),
                     endDateTime: new Date(2010, 12, 8, 16, 0),
                    },
                    {tag: "Event 5",
                     type: Type.Event,
                     subject: "Event 5",
                     description: "starts after Event 4 and finishes 5PM",
                     startDateTime: new Date(2010, 12, 8, 16, 0),
                     endDateTime: new Date(2010, 12, 8, 17, 0),
                    },
                    {tag: "Event 6",
                     type: Type.Event,
                     subject: "Event 6",
                     description: "starts 2010-12-10 at 11AM finishing 1PM, repeating for 4 weeks",
                     startDateTime: new Date(2010, 12, 10, 11, 0),
                     endDateTime: new Date(2010, 12, 10, 13, 0),
                     repeat: RecurrenceRule.Weekly,
                     repeatDays: [Qt.Friday],
                     repeatCount: 4
                    },
                ]
            }
        ]
    }

    function test_addEvent(data)
    {
        var component = Qt.createComponent("addEvent.qml")
        var obj = component.createObject(top)
        if (obj == undefined)
            console.log("Unable to load component from " + name +  " error is ", component.errorString())
        verify(obj != undefined, 'Unable to load component ' + name)
        obj.addEvents(data.events)
        var items = obj.testEvents()
        checkDetails(items)
        if (items.length == 0)
            console.log("No records added")
        obj.destroy()
        component.destroy()
    }

    function test_organizermodel_error_data() {
        return utility.getManagerListData();
    }

    function test_organizermodel_error(data) {
        var organizerChangedSpy = utility.create_testobject("import QtTest 1.0; SignalSpy {}", modelTests);
        // Create and check that backend for the tests is available
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "  manager: '" + data.managerToBeTested + "'\n"
            + "}\n", modelTests);
        organizerChangedSpy.target = organizerModel;
        organizerChangedSpy.signalName = "modelChanged";
        organizerChangedSpy.wait();
        organizerModel.removeCollection(organizerModel.defaultCollectionId());
        wait(signalWaitTime);// how to utilise SignalSpy to check signal is _not_ emitted?
        compare(organizerModel.error, "PermissionsError");
    }

    function test_organizermodel_fetchitemsbyid_data() {
        return utility.getManagerListData();
    }

    function test_organizermodel_fetchitemsbyid(data) {
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "  manager: '" + data.managerToBeTested + "'\n"
            + "}\n", modelTests);

        compare(organizerModel.fetchItems([]), -1)

        var spy = Qt.createQmlObject( "import QtTest 1.0 \nSignalSpy {}", modelTests);
        spy.target = organizerModel;
        spy.signalName = "itemsFetched";

        verify(organizerModel.fetchItems(["invalid-id"]) >= 0)
        spy.wait()
        compare(spy.count, 1)
    }


   function fetchItemsPopulation_data() {

        var event1 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  displayLabel: \"event1:\"\n"
            + "  id: event1\n"
            + "  startDateTime: new Date(2011, 11, 8, 13, 55)\n"
            + "  endDateTime: new Date(2011, 11, 8, 14, 07)\n"
            + "}\n", modelTests);

        var event2 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  displayLabel: \"event2:\"\n"
            + "  id: event2\n"
            + "  startDateTime: new Date(2011, 11, 8, 14, 11)\n"
            + "  endDateTime: new Date(2011, 11, 8, 14, 15)\n"
            + "}\n", modelTests);

        var event3 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  displayLabel: \"event3:\"\n"
            + "  id: event3\n"
            + "  startDateTime: new Date(2011, 11, 9, 14, 25, 0)\n"
            + "  endDateTime: new Date(2011, 11, 9, 14, 45)\n"
            + "}\n", modelTests);

        var event4 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  displayLabel: \"event4:\"\n"
            + "  id: event4\n"
            + "  startDateTime: new Date(2011, 11, 10, 14, 11)\n"
            + "  endDateTime: new Date(2011, 11, 10, 14, 45)\n"
            + "}\n", modelTests);

        var event5 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  displayLabel: \"event5:\"\n"
            + "  id: event5\n"
            + "  startDateTime: new Date(2011, 12, 11, 14, 55)\n"
            + "  endDateTime: new Date(2011, 12, 11, 15, 05)\n"
            + "}\n", modelTests);

        return [
            {
                tag: "Event set 1",
                events:[
                    event1,
                    event2,
                    event3,
                    event4,
                    event5,
                ]
            }
        ]
    }

    function test_organizermodel_fetchitems_data() {
        return utility.getManagerListData();
    }

    function test_organizermodel_fetchitems(data) {
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "    manager: '" + data.managerToBeTested + "'\n"
            + "    id:fetchItemsTestModel\n"
            + "    autoUpdate:true\n "
            + "    property list<OrganizerItem> testFetchedItems: [OrganizerItem{},OrganizerItem{}]\n"
            + "    onItemsFetched: {\n"
            + "        testFetchedItems = fetchedItems;\n"
            + "    }\n"
            + "}\n", modelTests);

        var modelChangedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
        modelChangedSpy.target = organizerModel
        modelChangedSpy.signalName = "modelChanged"

        var modelItemsFetchedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
        modelItemsFetchedSpy.target = organizerModel
        modelItemsFetchedSpy.signalName = "itemsFetched"

        var startDate = new Date(1977, 12, 9, 8, 0);
        var endDate = new Date(2022, 12, 9, 8, 0);
        organizerModel.startPeriod = startDate;
        organizerModel.endPeriod = endDate;

        // during initialisation only one modelChanged allowed
        modelChangedSpy.wait();
        compare(modelChangedSpy.count, 1)

        //Cleanup
        utility.init(organizerModel)
        utility.empty_calendar();
        compare(organizerModel.items.length, 0);

        // Store some test data....
        var testDataSet = fetchItemsPopulation_data()[0] //Event set 1
        console.log("Test Set:" + testDataSet.tag)
        console.log(" Saving " + testDataSet.events.length + " events...")
        for (var index = 0; index < testDataSet.events.length; index++){
            modelChangedSpy.clear();
            console.log("saving event:" + testDataSet.events[index]);
            organizerModel.saveItem( testDataSet.events[index]);
            modelChangedSpy.wait();
            compare(modelChangedSpy.count, 1)
        }
        console.log("Orgnizer itemcount = " + organizerModel.items.length);
        compare(organizerModel.items.length, testDataSet.events.length);

        // 1. Only fetch events with start/end dates before any stored events
        modelItemsFetchedSpy.clear();
        startDate = new Date(1977, 11, 9, 8, 0);
        endDate = new Date(2010, 11, 9, 8, 0);
        verify(organizerModel.fetchItems( startDate, endDate ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1);
        compare(organizerModel.fetchItems.length,0);

        // 2. Nothing fetched if start+end dates are after any stored events
        modelItemsFetchedSpy.clear();
        startDate = new Date(2012, 11, 9, 8, 0);
        endDate = new Date(2099, 11, 9, 8, 0);
        verify( organizerModel.fetchItems( startDate, endDate ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1);
        compare(organizerModel.fetchItems.length,0);

        // 3. All events fetched
        modelItemsFetchedSpy.clear();
        startDate = new Date(1977, 11, 9, 8, 0);
        endDate = new Date(2099, 11, 9, 8, 0);
        verify( organizerModel.fetchItems( startDate, endDate ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1)
        compare(organizerModel.testFetchedItems.length, 5);

        // 4. Only items within start+end fetched
        modelItemsFetchedSpy.clear();
        startDate = new Date(2011, 11, 8, 14, 10);
        endDate = new Date(2011, 11, 10, 20, 0);
        verify(organizerModel.fetchItems( startDate, endDate ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1)
        compare(organizerModel.testFetchedItems.length, 3);

        // 5. Filtering
        var testFilterDisplayLabel = Qt.createQmlObject("import QtOrganizer 5.0; DetailFieldFilter{}", organizerModel)
        testFilterDisplayLabel.detail = Detail.DisplayLabel
        testFilterDisplayLabel.field = DisplayLabel.FieldLabel
        testFilterDisplayLabel.value = "event2:"
        testFilterDisplayLabel.matchFlags = DetailFilter.MatchContains
        modelItemsFetchedSpy.clear();
        startDate = new Date(1977, 11, 9, 8, 0);
        endDate = new Date(2099, 11, 9, 8, 0);
        verify( organizerModel.fetchItems( startDate, endDate, testFilterDisplayLabel ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1)
        compare(organizerModel.testFetchedItems.length, 1);

        // 6. maxcount is not available in Memory model
        var testDefaultFilter = Qt.createQmlObject("import QtOrganizer 5.0; Filter{}", organizerModel)
        if (data.managerToBeTested != "memory")
        {
            startDate = new Date(1977, 11, 9, 8, 0);
            endDate = new Date(2099, 11, 9, 8, 0);
            modelItemsFetchedSpy.clear();
            verify( organizerModel.fetchItems( startDate, endDate, testDefaultFilter, 2) != -1);
            modelItemsFetchedSpy.wait();
            compare(modelItemsFetchedSpy.count, 1)
            compare(organizerModel.testFetchedItems.length, 2);
        }

         // 7. Sorting
        var sortAscending = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "    SortOrder {\n"
            + "    detail: Detail.DisplayLabel\n"
            + "    field: DisplayLabel.FieldLabel\n" //field: DisplayLabelField.FieldLabel\n"
            + "    id: testAscendingOrder\n"
            + "    direction: Qt.AscendingOrder\n"
            + "}\n", organizerModel);

        var sortDescending = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "    SortOrder {\n"
            + "    detail: Detail.DisplayLabel\n"
            + "    field: DisplayLabel.FieldLabel\n" //field: DisplayLabelField.FieldLabel\n"
            + "    id: testDecendingOrder\n"
            + "    direction: Qt.DescendingOrder\n"
            + "}\n", organizerModel);

        modelItemsFetchedSpy.clear();
        startDate = new Date(2011, 11, 8, 14, 10);
        endDate = new Date(2011, 11, 10, 20, 0);
        modelItemsFetchedSpy.clear();
        verify( organizerModel.fetchItems( startDate, endDate, testDefaultFilter, -1, [sortAscending] ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1);
        compare(organizerModel.testFetchedItems.length, 3);
        var expectedSortOrder = [ "event2:", "event3:", "event4:"]
        compare(expectedSortOrder.length, organizerModel.testFetchedItems.length);
        for (var j = 0; j < organizerModel.testFetchedItems.length; j++) {
            verify(organizerModel.testFetchedItems[j].displayLabel === expectedSortOrder[j], "FetchItems Ascending Sort Order Incorrect.");
        }

        modelItemsFetchedSpy.clear();
        verify( organizerModel.fetchItems( startDate, endDate, testDefaultFilter, -1, [sortDescending] ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1);
        compare(organizerModel.testFetchedItems.length, 3);
        compare(expectedSortOrder.length, organizerModel.testFetchedItems.length);
        for ( j = 0; j < organizerModel.testFetchedItems.length; j++) {
            verify(organizerModel.testFetchedItems[j].displayLabel == expectedSortOrder[2-j], "FetchItems Decending Sort Order Incorrect.");
        }

        // 8. fetch hints
        var fetchhint = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "    FetchHint {\n"
            + "        id:hint\n"
            + "        optimizationHints:FetchHint.AllRequired\n"
            + "}\n", organizerModel);
        modelItemsFetchedSpy.clear();
        verify( organizerModel.fetchItems( startDate, endDate, testFilterDisplayLabel, -1, [sortDescending], fetchhint ) != -1);
        modelItemsFetchedSpy.wait();
        compare(modelItemsFetchedSpy.count, 1)
        compare(organizerModel.testFetchedItems.length, 1);

        // 9. storage locations and maxcount are not available in Memory model
        if (data.managerToBeTested != "memory")
        {
            modelItemsFetchedSpy.clear();
            organizerModel.fetchItems( startDate, endDate, testFilterDisplayLabel, -1, [sortDescending], fetchhint , OrganizerModel.SystemStorage);
            modelItemsFetchedSpy.wait();
            compare(modelItemsFetchedSpy.count, 1)
            compare(organizerModel.testFetchedItems.length, 0);
        }

        //Cleanup
        utility.empty_calendar();
        compare(organizerModel.items.length, 0);
    }

    function test_organizermodel_containsitems_data() {
        return utility.getManagerListData();
    }

    function test_organizermodel_containsitems(data) {
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "  manager: '" + data.managerToBeTested + "'\n"
            + "  startPeriod: new Date(2011, 12, 8, 14, 0)\n"
            + "  endPeriod: new Date(2011, 12, 8, 16, 0)\n"
            + "}\n", modelTests);
        utility.init(organizerModel)
        utility.waitModelChange()
        utility.empty_calendar()

        var event1 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 13, 55)\n"
            + "  endDateTime: new Date(2011, 12, 8, 14, 07)\n"
            + "}\n", modelTests);

        var event2 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 14, 11)\n"
            + "  endDateTime: new Date(2011, 12, 8, 14, 15)\n"
            + "}\n", modelTests);

        var todo3 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Todo {\n"
            + "  startDateTime: new Date(2011, 12, 8, 14, 25, 0)\n"
            + "}\n", modelTests);

        var todo4 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Todo {\n"
            + "  dueDateTime: new Date(2011, 12, 8, 14, 45)\n"
            + "}\n", modelTests);

        var event5 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 14, 55)\n"
            + "  endDateTime: new Date(2011, 12, 8, 15, 05)\n"
            + "}\n", modelTests);

        organizerModel.saveItem(event1);
        utility.waitModelChange()
        organizerModel.saveItem(event2);
        utility.waitModelChange()
        organizerModel.saveItem(todo3);
        utility.waitModelChange()
        organizerModel.saveItem(todo4);
        utility.waitModelChange()
        organizerModel.saveItem(event5);
        utility.waitModelChange()
        compare(organizerModel.items.length, 5);

        var containsItems = organizerModel.containsItems(new Date(2011, 12, 8, 14, 0), new Date(2011, 12, 8, 16, 0), 600);
        compare(containsItems.length, 12);
        compare(containsItems[0], true);
        compare(containsItems[1], true);
        compare(containsItems[2], true);
        compare(containsItems[3], false);
        compare(containsItems[4], true);
        compare(containsItems[5], true);
        compare(containsItems[6], true);
        compare(containsItems[7], false);
        compare(containsItems[8], false);
        compare(containsItems[9], false);
        compare(containsItems[10], false);
        compare(containsItems[11], false);
    }

    function test_organizermodel_containsitems2_data() {
        return utility.getManagerListData();
    }

    function test_organizermodel_containsitems2(data) {
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "  manager: '" + data.managerToBeTested + "'\n"
            + "  startPeriod: new Date(2011, 12, 7)\n"
            + "  endPeriod: new Date(2011, 12, 9)\n"
            + "}\n", modelTests);
        utility.init(organizerModel)
        utility.waitModelChange()
        utility.empty_calendar()

        var event00 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 7, 11)\n"
            + "  endDateTime: new Date(2011, 12, 8, 0, 30)\n"
            + "}\n", modelTests);

        var event0 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Todo {\n"
            + "  startDateTime: new Date(2011, 12, 8, 1)\n"
            + "}\n", modelTests);

        var event1 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 3)\n"
            + "  endDateTime: new Date(2011, 12, 8, 3, 30)\n"
            + "}\n", modelTests);

        var event2 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 5)\n"
            + "  endDateTime: new Date(2011, 12, 8, 6)\n"
            + "}\n", modelTests);


        var event3 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 8)\n"
            + "  endDateTime: new Date(2011, 12, 8, 10)\n"
            + "}\n", modelTests);

        var event4 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 11, 30)\n"
            + "  endDateTime: new Date(2011, 12, 8, 12)\n"
            + "}\n", modelTests);

        var event5 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 13)\n"
            + "  endDateTime: new Date(2011, 12, 8, 13, 30)\n"
            + "}\n", modelTests);

        compare(organizerModel.items.length, 0);
        organizerModel.saveItem(event00);
        utility.waitModelChange()
        organizerModel.saveItem(event0);
        utility.waitModelChange()
        organizerModel.saveItem(event1);
        utility.waitModelChange()
        organizerModel.saveItem(event2);
        utility.waitModelChange()
        organizerModel.saveItem(event3);
        utility.waitModelChange()
        organizerModel.saveItem(event4);
        utility.waitModelChange()
        organizerModel.saveItem(event5);
        utility.waitModelChange()
        compare(organizerModel.items.length, 7);

        var containsItems = organizerModel.containsItems(new Date(2011, 12, 8), new Date(2011, 12, 8, 13), 3600);

        compare(containsItems.length, 13);
        compare(containsItems[0], true);
        compare(containsItems[1], true);
        compare(containsItems[2], false);
        compare(containsItems[3], true);
        compare(containsItems[4], false);
        compare(containsItems[5], true);
        compare(containsItems[6], false);
        compare(containsItems[7], false);
        compare(containsItems[8], true);
        compare(containsItems[9], true);
        compare(containsItems[10], false);
        compare(containsItems[11], true);
        compare(containsItems[12], false);
    }

    function modelChangedSignalTestItems() {
        return [
            // events
            "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "   startDateTime:'2011-10-25'\n"
            + "   endDateTime:'2011-10-26'\n"
            + "   allDay: false\n"
            + "   }",
            "import QtOrganizer 5.0 \n"
            + "   Event {\n"
            + "   startDateTime:'2011-10-26'\n"
            + "   endDateTime:'2011-10-27'\n"
            + "   allDay: true\n"
            + "   }"
        ]
    }

    // to test various usecases for modelChanged-signal
    function test_modelChangedSignal() {
        var managerlist = utility.getManagerList();
        if (managerlist.length < 0) {
            console.log("No manager to test");
            return;
        }
        for (var i = 0; i < managerlist.length; i ++) {

            var filter = Qt.createQmlObject("import QtOrganizer 5.0; DetailFieldFilter{}", modelTests)
            filter.detail = Detail.EventTime
            filter.field = EventTime.FieldAllDay
            filter.value = true

            var model = Qt.createQmlObject(
                    "import QtOrganizer 5.0;"
                    + "OrganizerModel {"
                    + "   manager: \"qtorganizer:" + managerlist[i] + ":id=qml\";"
                    + "   startPeriod:'2009-01-01';"
                    + "   endPeriod:'2012-12-31';"
                    + "   autoUpdate:true; }"
                    , modelTests);
            console.log("## Testing plugin: " + managerlist[i]);
            var modelChangedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
            modelChangedSpy.target = model
            modelChangedSpy.signalName = "modelChanged"

            // during initialisation only one modelChanged allowed
            modelChangedSpy.wait(signalWaitTime);
            compare(modelChangedSpy.count, 1)

            // prepare for rest of cases
            utility.init(model)
            utility.empty_calendar()
            utility.addItemsToModel(modelChangedSignalTestItems(), modelTests)
            compare(model.itemCount, 2)

            // after filterchange only one modelChanged allowed
            modelChangedSpy.clear()
            model.filter = filter
            modelChangedSpy.wait(signalWaitTime);
            compare(modelChangedSpy.count, 1)
            compare(model.itemCount, 1)

            // after manual update only one modelChanged allowed
            model.autoUpdate = false
            modelChangedSpy.clear()
            model.filter = null
            model.update()
            modelChangedSpy.wait(signalWaitTime);
            compare(modelChangedSpy.count, 1)
            compare(model.itemCount, 2)
            utility.empty_calendar()
        }
    }

    function test_updateMethods() {
        var managerlist = utility.getManagerList();
        if (managerlist.length < 0) {
            console.log("No manager to test");
            return;
        }
        for (var i = 0; i < managerlist.length; i ++) {
            var organizerModel = Qt.createQmlObject(
                    "import QtOrganizer 5.0;"
                    + "OrganizerModel {"
                    + "   manager: '" + managerlist[i] + "'\n"
                    + "   startPeriod:'2009-01-01'\n"
                    + "   endPeriod:'2012-12-31'\n"
                    + "}"
                    , modelTests);
            console.log("## Testing plugin: " + managerlist[i]);

            utility.init(organizerModel);
            utility.waitModelChange();
            utility.empty_calendar();

            var event1 = utility.create_testobject(
                "import QtOrganizer 5.0\n"
                + "Event {\n"
                + "  startDateTime: new Date(2011, 12, 7, 11)\n"
                + "  endDateTime: new Date(2011, 12, 8, 0, 30)\n"
                + "}\n", modelTests);
            var event2 = utility.create_testobject(
                "import QtOrganizer 5.0\n"
                + "Event {\n"
                + "  startDateTime: new Date(2011, 13, 7, 11)\n"
                + "  endDateTime: new Date(2011, 13, 8, 0, 30)\n"
                + "}\n", modelTests);

            var collection1 = utility.create_testobject("import QtQuick 2.0 \n"
              + "import QtOrganizer 5.0\n"
              + "Collection {\n"
              + "id: coll1\n"
              + "}\n", modelTests);

            var collection2 = utility.create_testobject("import QtQuick 2.0 \n"
              + "import QtOrganizer 5.0\n"
              + "Collection {\n"
              + "id: coll1\n"
              + "}\n", modelTests);

            var modelChangedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
            modelChangedSpy.target = organizerModel
            modelChangedSpy.signalName = "modelChanged"
            var collectionsChangedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
            collectionsChangedSpy.target = organizerModel
            collectionsChangedSpy.signalName = "collectionsChanged"

            // starting point
            compare(organizerModel.items.length, 0);
            compare(organizerModel.collections.length, 1);

            // autoUpdate is false, so these operations should not cause signal emission
            organizerModel.autoUpdate = false;
            organizerModel.saveItem(event1);
            organizerModel.saveCollection(collection1);
            wait(signalWaitTime)
            compare(modelChangedSpy.count, 0)
            compare(collectionsChangedSpy.count, 0)
            compare(organizerModel.items.length, 0)
            compare(organizerModel.collections.length, 1)

            // updateItems() - should update only items count
            organizerModel.updateItems();
            modelChangedSpy.wait(signalWaitTime)
            compare(modelChangedSpy.count, 1)
            compare(collectionsChangedSpy.count, 0)
            compare(organizerModel.items.length, 1)//+1
            compare(organizerModel.collections.length, 1)// collection change not visible, only default collection visible

            // updateCollections() - should update only collections
            modelChangedSpy.clear();
            collectionsChangedSpy.clear();
            organizerModel.saveItem(event2);
            // - there is saved unseen collection from updateItems()-test
            wait(signalWaitTime)
            compare(modelChangedSpy.count, 0)
            compare(collectionsChangedSpy.count, 0)
            organizerModel.updateCollections();
            collectionsChangedSpy.wait(signalWaitTime)
            compare(modelChangedSpy.count, 0)
            compare(collectionsChangedSpy.count, 1)
            compare(organizerModel.items.length, 1)// event2 not visible, only event1 saved in updateItems()-test visible
            compare(organizerModel.collections.length, 2)//+1

            // update() - should update both
            modelChangedSpy.clear();
            collectionsChangedSpy.clear();
            // - there is saved unseen item from updateCollections()-test
            organizerModel.saveCollection(collection2);
            wait(signalWaitTime)
            compare(modelChangedSpy.count, 0)
            compare(collectionsChangedSpy.count, 0)
            organizerModel.update();
            modelChangedSpy.wait(signalWaitTime)
            compare(modelChangedSpy.count, 1)
            compare(collectionsChangedSpy.count, 1)
            compare(organizerModel.items.length, 2)//+1
            compare(organizerModel.collections.length, 3)//+1

            modelChangedSpy.destroy();
            collectionsChangedSpy.destroy();
            organizerModel.destroy();
        }
    }

    function test_updateMethodsStartWithAutoupdateFalse() {

        var organizerModel = Qt.createQmlObject(
                "import QtOrganizer 5.0;"
                + "OrganizerModel {"
                + "   manager:'memory'\n"
                + "   startPeriod:'2009-01-01'\n"
                + "   endPeriod:'2012-12-31'\n"
                + "   autoUpdate: false\n"
                + "}"
                , modelTests);

        var modelChangedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
        modelChangedSpy.target = organizerModel
        modelChangedSpy.signalName = "modelChanged"
        var collectionsChangedSpy = Qt.createQmlObject("import QtTest 1.0; SignalSpy{}", modelTests)
        collectionsChangedSpy.target = organizerModel
        collectionsChangedSpy.signalName = "collectionsChanged"

        // Add data to this model
        var event1 = utility.create_testobject(
            "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 7, 11)\n"
            + "  endDateTime: new Date(2011, 12, 8, 0, 30)\n"
            + "}\n", modelTests);
        var event2 = utility.create_testobject(
            "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 13, 7, 11)\n"
            + "  endDateTime: new Date(2011, 13, 8, 0, 30)\n"
            + "}\n", modelTests);

        var collection1 = utility.create_testobject("import QtQuick 2.0 \n"
          + "import QtOrganizer 5.0\n"
          + "Collection {\n"
          + "id: coll1\n"
          + "}\n", modelTests);

        var collection2 = utility.create_testobject("import QtQuick 2.0 \n"
          + "import QtOrganizer 5.0\n"
          + "Collection {\n"
          + "id: coll1\n"
          + "}\n", modelTests);

        organizerModel.saveItem(event1);
        organizerModel.saveItem(event2);
        organizerModel.saveCollection(collection1);
        organizerModel.saveCollection(collection2);

        // Now there should be 2 items and 2 collections (+ default collection)
        // in the model. They're just not visible, since autoUpdate is false.
        compare(organizerModel.items.length, 0);
        compare(organizerModel.collections.length, 0);

        // Check we see still only collections after updating collections
        organizerModel.updateCollections();
        collectionsChangedSpy.wait(signalWaitTime)
        compare(organizerModel.items.length, 0);
        compare(organizerModel.collections.length, 3);// 2 + default collection

        // Save there item with other collection than default collection
        var event = utility.create_testobject(
            "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 7, 11)\n"
            + "  endDateTime: new Date(2011, 12, 8, 0, 30)\n"
            + "}\n", modelTests);
        for (var i = 0; i < organizerModel.collections.length; ++i) {
            var collId = organizerModel.collections[i].collectionId;
            if (collId != organizerModel.defaultCollectionId()) {
                event.collectionId = collId;
            }
        }
        organizerModel.saveItem(event);

        // Create collection filter and check that only the item with that collection is visible
        var collectionFilter = Qt.createQmlObject("import QtOrganizer 5.0;CollectionFilter{}", modelTests);
        collectionFilter.ids = [event.collectionId];
        organizerModel.filter = collectionFilter;
        organizerModel.updateItems();
        modelChangedSpy.wait(signalWaitTime)
        compare(organizerModel.items.length, 1);// 1 with that collection

        // Lastly check we get everything if filter is reset
        organizerModel.filter = null;
        organizerModel.updateItems();
        modelChangedSpy.wait(signalWaitTime)
        compare(organizerModel.items.length, 3); // all items

        // cleanup
        utility.init(organizerModel);
        utility.empty_calendar();

        collectionFilter.destroy();
        modelChangedSpy.destroy();
        collectionsChangedSpy.destroy();
        organizerModel.destroy();
    }

    function test_collectionsSaveRemove_data() {
        return utility.getManagerListData();
    }

    function test_collectionsSaveRemove(data) {

        var model = utility.createModel(data.managerToBeTested);

        var view = utility.create_testobject(
                "import QtQuick 2.0\n"
                + "ListView {\n"
                + "  width:100; height: 1000;\n"
                + "  delegate: Text{ text: name }\n"
                + "}\n", modelTests);

        var collections = []
        var events = []

        var modelChangedSpy = utility.create_testobject("import QtTest 1.0; SignalSpy{}", modelTests)
        modelChangedSpy.target = model
        modelChangedSpy.signalName = "modelChanged"
        var collectionsChangedSpy = utility.create_testobject("import QtTest 1.0; SignalSpy{}", modelTests)
        collectionsChangedSpy.target = model
        collectionsChangedSpy.signalName = "collectionsChanged"

        // starting point
        compare(model.items.length, 0);
        compare(model.collections.length, 1);

        // add collections
        var ncoll = 10;
        for (var i=1; i<=ncoll; ++i) {
            var collection1 = utility.create_testobject("import QtQuick 2.0 \n"
                  + "import QtOrganizer 5.0\n"
                  + "Collection {\n"
                  + "id: coll1\n"
                  + "}\n", modelTests);
            collections[i] = collection1
            collection1.name = 'collection ' + i
            model.saveCollection(collection1);
            collectionsChangedSpy.wait(signalWaitTime);
            var event1 = utility.create_testobject(
                    "import QtOrganizer 5.0\n"
                    + "Event {\n"
                    + "  startDateTime: new Date(2011, 12, 7, 11)\n"
                    + "  endDateTime: new Date(2011, 12, 8, 0, 30)\n"
                    + "}\n", modelTests);
            events[i] = event1
            event1.collectionId = model.collections[i].collectionId;
            model.saveItem(event1);
            modelChangedSpy.wait(signalWaitTime);
        }
        compare(model.collections.length, ncoll+1);
        compare(model.itemCount, ncoll);

        view.model = model.collections;
        verify(view.contentHeight > 0, 'view content is empty');
        compare(view.count, ncoll+1);

        view.model = [] // avoid QTBUG-41485
        model.removeCollection(model.collections[ncoll].collectionId);
        collectionsChangedSpy.wait(signalWaitTime);
        view.model = model.collections;
        compare(model.collections.length, ncoll);
        compare(model.itemCount, ncoll-1);
        compare(view.count, ncoll);

        view.destroy()
        model.destroy();
    }

    function test_modelsSharingFilterAndFetchHint() {
        // ensure model won't delete filter and fetchHint it doesn't own
        var view = utility.create_testobject(
                "import QtQuick 2.0\n"
                + "import QtOrganizer 5.0\n"
                + "ListView {\n"
                + "  model:2;\n"
                + "  width:100; height: 1000;\n"
                + "  property var theFilter: DetailRangeFilter {}\n"
                + "  property var theFetchHint: FetchHint {}\n"
                + "  delegate: Item { OrganizerModel { filter: theFilter; fetchHint: theFetchHint } }\n"
                + "}\n", modelTests);
    }
}

