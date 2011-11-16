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

//TESTED_COMPONENT=src/organizer

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    name: "ModelTests"
    id:modelTests

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
                   + "    filter:DetailFilter{\n"
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
                {tag: "With changelog filter",
                 code: "import QtOrganizer 5.0\n"
                    + "OrganizerModel {\n"
                    + "   id:organizerModelId\n"
                    + "   filter:ChangeLogFilter{\n"
                    + "       id:filter\n"
                    + "       eventType:ChangeLogFilter.EventAdded\n"
                    + "       since:'2010-08-12T13:22:01'\n"
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
                    + "           DetailFilter{\n"
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
        return [
            {tag: "memory backend", managerToBeTested: "memory"},
            {tag: "jsondb backend", managerToBeTested: "jsondb"}
        ]
    }

    function test_organizermodel_error(data) {
        var spyWaitDelay = 200;
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
        organizerModel.removeCollection(organizerModel.defaultCollection().collectionId);
        wait(spyWaitDelay);// how to utilise SignalSpy to check signal is _not_ emitted?
        compare(organizerModel.error, "PermissionsError");
    }

    function test_organizermodel_fetchitems_data() {
        return [
            {tag: "memory backend", managerToBeTested: "memory"},
            {tag: "jsondb backend", managerToBeTested: "jsondb"}
        ]
    }

    function test_organizermodel_fetchitems(data) {
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "  manager: '" + data.managerToBeTested + "'\n"
            + "}\n", modelTests);
        wait(500);

        compare(organizerModel.fetchItems([]), -1)

        var spy = Qt.createQmlObject( "import QtTest 1.0 \nSignalSpy {}", modelTests);
        spy.target = organizerModel;
        spy.signalName = "itemsFetched";

        verify(organizerModel.fetchItems(["invalid-id"]) >= 0)
        spy.wait()
        compare(spy.count, 1)
    }

    function test_organizermodel_containsitems_data() {
        return [
            {tag: "memory backend", managerToBeTested: "memory"},
            {tag: "jsondb backend", managerToBeTested: "jsondb"}
        ]
    }

    function test_organizermodel_containsitems(data) {
        var organizerModel = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "OrganizerModel {\n"
            + "  manager: '" + data.managerToBeTested + "'\n"
            + "  startPeriod: new Date(2011, 12, 8, 14, 0)\n"
            + "  endPeriod: new Date(2011, 12, 8, 16, 0)\n"
            + "}\n", modelTests);
        wait(500);

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

        var event3 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 14, 25, 0)\n"
            + "}\n", modelTests);

        var event4 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  endDateTime: new Date(2011, 12, 8, 14, 45)\n"
            + "}\n", modelTests);

        var event5 = utility.create_testobject("import QtQuick 2.0\n"
            + "import QtOrganizer 5.0\n"
            + "Event {\n"
            + "  startDateTime: new Date(2011, 12, 8, 14, 55)\n"
            + "  endDateTime: new Date(2011, 12, 8, 15, 05)\n"
            + "}\n", modelTests);

        organizerModel.saveItem(event1);
        organizerModel.saveItem(event2);
        organizerModel.saveItem(event3);
        organizerModel.saveItem(event4);
        organizerModel.saveItem(event5);
        wait(2000);
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
}

