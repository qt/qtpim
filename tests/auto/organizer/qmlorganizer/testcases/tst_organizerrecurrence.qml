/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    id: test
    name: "OrganizerRecurrenceTests"

    OrganizerModel {
        id: model
        autoUpdate:true
        startPeriod:'2009-01-01'
        endPeriod:'2014-12-31'
    }

    Event {
        id: testEvent
    }

    Todo {
        id: testTodo
    }

    RecurrenceRule {
        id: testRule
    }

    RecurrenceRule {
        id: testXRule
    }

    SignalSpy {
        id: spy
        signalName: "modelChanged"
        target: model
    }

    function test_recurrenceDates_data() {
        return [
                    {
                        tag: "Event with two recurrence dates",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2012-01-02'), new Date('2012-01-03')],
                            "exceptionDates": [],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-02T14:00:00'), new Date('2012-01-03T14:00:00')]
                    },

                    {
                        tag: "Event with recurrence date before event start date",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2011-01-01')],
                            "exceptionDates": [],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-01T14:00:00')]
                    },

                    {
                        tag: "Event outside model range with occurrences inside model range",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2008-01-01T14:00:00'),
                            "end" : new Date('2008-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2012-01-02'), new Date('2012-01-03')],
                            "exceptionDates": [],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-02T14:00:00'), new Date('2012-01-03T14:00:00')]
                    },

                    {
                        tag: "Event inside model range with occurrences outside model range",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2020-01-02'), new Date('2020-01-03')],
                            "exceptionDates": [],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-01T14:00:00')]
                    },

                    {
                        tag: "Overlapping recurrence dates",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2012-01-02'), new Date('2020-01-02')],
                            "exceptionDates": [],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-02T14:00:00')]
                    }
                ]
    }

    function test_recurrenceDates(data) {
        runTest(data)
    }


    function test_recurrenceRules_data() {
        return [
                    {
                        tag: "Daily recurrence, limit to 3",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 3,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-02T14:00:00'), new Date('2012-01-03T14:00:00')]
                    },

                    {
                        tag: "Daily recurrence, limit to 3, interval of 2 days",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 3,
                            "interval": 2,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-03T14:00:00'), new Date('2012-01-05T14:00:00')]
                    },

                    {
                        tag: "Daily recurrence, limit to date",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": new Date('2012-01-05'),
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-02T14:00:00'), new Date('2012-01-03T14:00:00'),
                            new Date('2012-01-04T14:00:00'), new Date('2012-01-05T14:00:00')]
                    },

                    {
                        tag: "Daily recurrence, limit to 6, Mondays, Wednesdays and Saturdays only",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [new Date('2012-01-04T14:00:00'), new Date('2012-01-09T14:00:00')],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 6,
                            "interval": 1,
                            "daysOfWeek": [Qt.Monday, Qt.Wednesday, Qt.Saturday],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-02T14:00:00'),  new Date('2012-01-07T14:00:00'),
                            new Date('2012-01-11T14:00:00'), new Date('2012-01-14T14:00:00')]
                    },

                    {
                        tag: "Daily recurrence, limit to 4, days of month: 1, 2, 10, 11",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 4,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [1, 2, 10, 11],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-02T14:00:00'), new Date('2012-01-10T14:00:00'), new Date('2012-01-11T14:00:00')]
                    },

                    // Weekly recurrences

                    {
                        tag: "Weekly recurrence, limit by date, biweekly",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Weekly,
                            "limit": new Date('2012-02-05'),
                            "interval": 2,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-15T14:00:00'), new Date('2012-01-29T14:00:00')]
                    },

                    // Monthly recurrences

                    {
                        tag: "Monthly recurrence, limit of 6, February, May, December only",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Monthly,
                            "limit": 6,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [RecurrenceRule.February, RecurrenceRule.May, RecurrenceRule.December],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-02-01T14:00:00'), new Date('2012-05-01T14:00:00'), new Date('2012-12-01T14:00:00'),
                            new Date('2013-02-01T14:00:00'), new Date('2013-05-01T14:00:00'), new Date('2013-12-01T14:00:00')]
                    },

                    {
                        tag: "Monthly recurrence, limit of 6, positions 1, 2, -1",
                        managers: ["jsondb"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Monthly,
                            "limit": 6,
                            "interval": 1,
                            "daysOfWeek": [Qt.Monday, Qt.Tuesday, Qt.Wednesday, Qt.Thursday, Qt.Friday, Qt.Saturday, Qt.Sunday],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [1, 2, -1],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'), new Date('2012-01-02T14:00:00'), new Date('2012-01-31T14:00:00'),
                            new Date('2012-02-01T14:00:00'), new Date('2012-02-02T14:00:00'), new Date('2012-02-29T14:00:00')]
                    },

                    {
                        tag: "Monthly recurrence, limit of 6, position 31",
                        managers: ["jsondb"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Monthly,
                            "limit": 6,
                            "interval": 1,
                            "daysOfWeek": [Qt.Monday, Qt.Tuesday, Qt.Wednesday, Qt.Thursday, Qt.Friday, Qt.Saturday, Qt.Sunday],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [31],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-31T14:00:00'), new Date('2012-03-31T14:00:00'), new Date('2012-05-31T14:00:00'),
                            new Date('2012-07-31T14:00:00'), new Date('2012-08-31T14:00:00'), new Date('2012-10-31T14:00:00')]
                    },

                    // Yearly recurrences

                    {
                        tag: "Yearly recurrence, limit to 4, two exception dates ",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [new Date('2012-01-04T14:00:00'), new Date('2012-01-09T14:00:00')],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Yearly,
                            "limit": 4,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-01T14:00:00'),  new Date('2013-01-01T14:00:00'), new Date('2014-01-01T14:00:00')]
                    },

                    {
                        tag: "Yearly recurrence, limit to 6, May, July, August, December only",
                        managers: ["jsondb"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Yearly,
                            "limit": 6,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [RecurrenceRule.May, RecurrenceRule.July, RecurrenceRule.August, RecurrenceRule.December],
                            "positions": [2, -2],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-07-01T14:00:00'), new Date('2012-08-01T14:00:00'), new Date('2013-07-01T14:00:00'),
                            new Date('2013-08-01T14:00:00'), new Date('2014-07-01T14:00:00'), new Date('2014-08-01T14:00:00')]
                    }
                ]
    }

    function test_recurrenceRules(data) {
        runTest(data)
    }

    function test_exceptionDates_data() {
        return [
                    {
                        tag: "Exception dates, two matching exception dates",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T23:00:00'),
                            "end" : new Date('2012-01-02T01:00:00'),
                            "recurrenceDates": [new Date('2012-01-02'), new Date('2012-01-03'), new Date('2012-01-04')],
                            "exceptionDates": [new Date('2012-01-02'), new Date('2012-01-04')],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-01T23:00:00'), new Date('2012-01-03T23:00:00')]
                    },

                    {
                        tag: "Exception dates, two non-matching exception dates",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [new Date('2012-01-02'), new Date('2012-01-03')],
                            "recurrenceRules": [],
                            "exceptionRules": []
                        },
                        results: [new Date('2012-01-01T14:00:00')]
                    }
                ]
    }

    function test_exceptionDates(data) {
        runTest(data)
    }

    function test_exceptionRules_data() {
        return [
                    {
                        tag: "Daily recurrence with matching daily exceptions",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2012-05-18')],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": [testXRule]
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 3,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },
                        xrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 3,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-05-18T14:00:00')]
                    },

                    {
                        tag: "Daily recurrence, weekly exceptions on Monday and Sunday",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": [testXRule]
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Daily,
                            "limit": 10,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },
                        xrule: {
                            "frequency": RecurrenceRule.Weekly,
                            "limit": 10,
                            "interval": 1,
                            "daysOfWeek": [Qt.Monday, Qt.Sunday],
                            "daysOfMonth": [],
                            "daysOfYear": [],
                            "monthsOfYear": [],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },

                        results: [new Date('2012-01-03T14:00:00'), new Date('2012-01-04T14:00:00'), new Date('2012-01-05T14:00:00'),
                            new Date('2012-01-06T14:00:00'), new Date('2012-01-07T14:00:00'), new Date('2012-01-10T14:00:00')]

                    }]
    }

    function test_exceptionRules(data) {
        runTest(data)
    }

    /* Todo, how this should behave? Currently there will be no occurrences, despite the recurrenceDate
    function test_invalidRecurrence_data() {
        return [
                    {
                        tag: "Invalid recurrence, no matching dates for rule",
                        managers: ["jsondb", "memory"],
                        definitions: {

                            "start" : new Date('2012-01-01T14:00:00'),
                            "end" : new Date('2012-01-01T15:00:00'),
                            "recurrenceDates": [new Date('2012-01-01T14:00:00')],
                            "exceptionDates": [],
                            "recurrenceRules": [testRule],
                            "exceptionRules": []
                        },
                        rrule: {
                            "frequency": RecurrenceRule.Monthly,
                            "limit": 5,
                            "interval": 1,
                            "daysOfWeek": [],
                            "daysOfMonth": [31],
                            "daysOfYear": [],
                            "monthsOfYear": [RecurrenceRule.February, RecurrenceRule.June, RecurrenceRule.April, RecurrenceRule.September, RecurrenceRule.November],
                            "positions": [],
                            "firstDayOfWeek": Qt.Monday
                        },
                        results: [new Date('2012-01-01T14:00:00')]
                    }]
    }

    function test_invalidRecurrence(data) {
        runTest(data);
    }*/

    function test_recurrenceRulesMaxLimit() {
        var managers = ["jsondb", "memory"];
        for (var i in managers) {
            console.log("Testing "+managers[i]+" backend");
            model.manager = managers[i];
            wait(500) // Todo: replace with spy.wait()
            cleanDatabase();

            testRule.frequency = RecurrenceRule.Daily;
            testRule.interval = 1;
            testRule.limit = null;
            testRule.daysOfWeek = [];
            testRule.daysOfMonth = [];
            testRule.daysOfYear = [];
            testRule.monthsOfYear = [];
            testRule.positions = [];

            testEvent.recurrence.recurrenceRules = [testRule];
            model.saveItem(testEvent)
            spy.wait();
            compare(model.itemCount, 50); // Default max limit is 50

            cleanDatabase();
        }
    }

    function test_recurrenceRulesUnion() {
        var managers = ["jsondb", "memory"];
        for (var i in managers) {
            console.log("Testing "+managers[i]+" backend");
            model.manager = managers[i];
            wait(500) // Todo: replace with spy.wait()
            cleanDatabase();

            testRule.frequency = RecurrenceRule.Daily;
            testRule.interval = 3;
            testRule.limit = 3;
            testRule.daysOfWeek = [];
            testRule.daysOfMonth = [];
            testRule.daysOfYear = [];
            testRule.monthsOfYear = [];
            testRule.positions = [];

            // Not used for exception this time...
            testXRule.frequency = RecurrenceRule.Daily;
            testXRule.interval = 2;
            testXRule.limit = 4;
            testXRule.daysOfWeek = [];
            testXRule.daysOfMonth = [];
            testXRule.daysOfYear = [];
            testXRule.monthsOfYear = [];
            testXRule.positions = [];

            testEvent.recurrence.recurrenceDates = [];
            testEvent.recurrence.exceptionDates = [];
            testEvent.recurrence.recurrenceRules = [testRule, testXRule];
            testEvent.recurrence.exceptionRules = [];
            testEvent.startDateTime = new Date('2012-01-01T14:00:00');
            testEvent.endDateTime = new Date('2012-01-01T16:00:00');
            model.saveItem(testEvent)
            spy.wait();

            compareResultDatesToModel([new Date('2012-01-01T14:00:00'),
                                       new Date('2012-01-03T14:00:00'),
                                       new Date('2012-01-04T14:00:00'),
                                       new Date('2012-01-05T14:00:00'),
                                       new Date('2012-01-07T14:00:00'),
                                      ],model);

            cleanDatabase();
        }
    }


    // Helper functions

    function runTest(data) {
        for (var i in data.managers) {
            console.log("Testing "+data.managers[i]+" backend")
            model.manager = data.managers[i]
            wait(500) // Todo: replace with spy.wait()
            cleanDatabase()
            compare(model.itemCount, 0, "Model not empty")
            populateTestItemsFromData(data);
            model.saveItem(testEvent)
            spy.wait()
            compareResultDatesToModel(data.results, model)

            cleanDatabase()
            compare(model.itemCount, 0, "Model not empty")
            populateTestItemsFromData(data);
            model.saveItem(testTodo)
            spy.wait()
            compareResultDatesToModel(data.results, model)

            cleanDatabase()
        }
    }

    function cleanDatabase() {
        var ids = []; //model.itemIds();
        var removeIds = [];
        var i;
        var j;
        var parentId;
        for (i = 0; i < model.itemCount; i++) {
            if (model.items[i].itemType == Type.EventOccurrence || model.items[i].itemType == Type.TodoOccurrence)
                ids.push(model.items[i].parentId)
            else
                ids.push(model.items[i].itemId)
        }
        // remove duplicates

        for (i = 0; i < ids.length; i++) {
            for (j = 0; j < removeIds.length; j++) {
                if (ids[i] == removeIds[j])
                    break;
            }
            if (j == removeIds.length)
                removeIds.push(ids[i])
        }

        spy.clear()
        if (ids.length > 0) {
            model.removeItems(removeIds)
            spy.wait()
        }
        compare(model.itemIds().length, 0)
    }

    function populateTestItemsFromData(data) {

        if (data.rrule !== undefined) {
            testRule.frequency = data.rrule.frequency;
            testRule.limit = data.rrule.limit;
            testRule.interval = data.rrule.interval;
            testRule.daysOfWeek = data.rrule.daysOfWeek;
            testRule.daysOfMonth = data.rrule.daysOfMonth;
            testRule.daysOfYear = data.rrule.daysOfYear;
            testRule.monthsOfYear = data.rrule.monthsOfYear;
            testRule.positions = data.rrule.positions;
            testRule.firstDayOfWeek = data.rrule.firstDayOfWeek;
        }

        if (data.xrule !== undefined) {
            testXRule.frequency = data.xrule.frequency;
            testXRule.limit = data.xrule.limit;
            testXRule.interval = data.xrule.interval;
            testXRule.daysOfWeek = data.xrule.daysOfWeek;
            testXRule.daysOfMonth = data.xrule.daysOfMonth;
            testXRule.daysOfYear = data.xrule.daysOfYear;
            testXRule.monthsOfYear = data.xrule.monthsOfYear;
            testXRule.positions = data.xrule.positions;
            testXRule.firstDayOfWeek = data.xrule.firstDayOfWeek;
        }

        testEvent.startDateTime = new Date(data.definitions.start);
        testTodo.startDateTime = new Date(data.definitions.start);
        testEvent.endDateTime = new Date(data.definitions.end);
        testTodo.dueDateTime = new Date(data.definitions.end);

        testEvent.recurrence.recurrenceDates = data.definitions.recurrenceDates;
        testTodo.recurrence.recurrenceDates = data.definitions.recurrenceDates;
        testEvent.recurrence.recurrenceRules = data.definitions.recurrenceRules;
        testTodo.recurrence.recurrenceRules = data.definitions.recurrenceRules;

        testEvent.recurrence.exceptionDates = data.definitions.exceptionDates;
        testTodo.recurrence.exceptionDates = data.definitions.exceptionDates;
        testEvent.recurrence.exceptionRules = data.definitions.exceptionRules;
        testTodo.recurrence.exceptionRules = data.definitions.exceptionRules;

    }

    function compareResultDatesToModel(results, model) {
        compare(model.itemCount, results.length, "Occurrence count is wrong.");

        for (var i = 0; i < results.length; i++) {
            var itemStart = model.items[i].startDateTime;
            compare(itemStart, results[i], " Occurrence start date is not correct");
        }
    }

}

