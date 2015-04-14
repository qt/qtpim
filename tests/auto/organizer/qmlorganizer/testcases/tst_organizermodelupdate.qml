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


// TODO: ideas for additional tests:
// change existing normal event currently not in model to be a recurring event
// put autoupdate off, add an item (no changes to model), put it on --> model should be updated
// put autoupdate off, change start and end dates (no changes to model) and put it back on --> model updated
// put autoupdate off, change start and end dates and filter (e.g. collection) (no changes) and put it back on --> model updated
// put autoupdate off, add item in the range of new start and end times, change start and end dates (no changes to model) and put it back on --> model updated
// put autoupdate off, change start and end dates many times (no changes to model) and put it back on --> model updated
// add sortorder and remove it, check the model

TestCase {
    id: test
    name: "OrganizerRecurrenceTests"

    property int spyWaitDelay: 700

    property variant fetchedItem

    QOrganizerTestUtility {
        id: utility
    }

    OrganizerModel {
        id: model
        onItemsFetched : {
            test.fetchedItem = fetchedItems[0]
        }
    }

    SignalSpy {
        id: spyManagerChanged
        signalName: "managerChanged"
        target: model
    }

    SignalSpy {
        id: modelChangedSpy
        signalName: "modelChanged"
        target: model
    }

    SignalSpy {
        id: fetchSpy
        signalName: "itemsFetched"
        target: model
    }

    function cleanup() {
        model.manager = ""
    }

    function localDateTime(tsSpec) {
        // Parse a ISO8601 time spec and return it as local time; if passed to the Date
        // ctor, it will be interpreted as UTC
        var dt = tsSpec.split('T')
        if (dt.length == 2) {
            var d = dt[0].split('-')
            if (d.length == 3) {
                // If there is UTC offset information, we can't treat it as local time
                var t = dt[1].split(/[\:\.\+\-]/)
                if (t.length == 4) {
                    return new Date(d[0], d[1] - 1, d[2], t[0], t[1], t[2], t[3])
                } else if (t.length == 3) {
                    return new Date(d[0], d[1] - 1, d[2], t[0], t[1], t[2])
                }
            }
        } else if (dt.length == 1) {
            // Only parse if this is exactly a date
            if (dt[0].length <= 10) {
                var d = dt[0].split('-')
                if (d.length == 3) {
                    return new Date(d[0], d[1] - 1, d[2])
                }
            }
        }

        return new Date(tsSpec)
    }

    function localDate(dSpec) {
        // Despite the name, return this date as UTC - on conversion to a QDate, it will
        // be converted from UTC value to a date portion, so from a positive UTC offset, it
        // will be converted to the prior date.
        // The date will always be interpreted as local time by the recurrence rule.
        return new Date(dSpec)
    }

    function test_changeTimePeriod_data() {
        return [{
                managers: utility.getManagerList(),
                definitions: [
                {
                    event :
                    {
                        "displayLabel" : "event0",
                        "start" : localDateTime('2011-10-25T15:00:00'),
                        "end" : localDateTime('2011-10-10T16:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: false
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent1",
                        "start" : localDateTime('2012-01-01T14:00:00'),
                        "end" : localDateTime('2012-01-01T15:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Daily,
                        "limit": localDate('2012-01-03'),
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event1",
                        "start" : localDateTime('2012-01-02T15:00:00'),
                        "end" : localDateTime('2012-01-02T16:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event2",
                        "start" : localDateTime('2012-01-03T11:00:00'),
                        "end" : localDateTime('2012-01-03T18:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent2",
                        "start" : localDateTime('2011-01-01T16:00:00'),
                        "end" : localDateTime('2011-01-01T17:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 15,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event3",
                        "start" : localDateTime('2012-05-15T09:00:00'),
                        "end" : localDateTime('2012-05-15T10:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: false
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent3",
                        "start" : localDateTime('2012-06-03T20:00:00'),
                        "end" : localDateTime('2012-06-03T21:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 4,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: false
                }


                ],
                results: [
                          {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                          {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                          {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                          {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                          {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                          {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                          {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                          {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                          {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                ],
                timePeriods: [
                {
                    start: localDateTime('2012-01-03T00:00:00'),
                    autoUpdate: true,
                    results: [
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                    ]
                },
                {
                    end: localDateTime('2012-07-25T15:00:00'),
                    autoUpdate: true,
                    results: [
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event3", start: localDateTime('2012-05-15T09:00:00')},
                        {label: "recevent3", start: localDateTime('2012-06-03T20:00:00')},
                        {label: "recevent3", start: localDateTime('2012-07-03T20:00:00')}
                    ]
                },
                {
                    start: localDateTime('2011-09-25T15:00:00'),
                    end: localDateTime('2012-12-25T15:00:00'),
                    autoUpdate: false,
                    results: [
                        {label: "recevent2", start: localDateTime('2011-10-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event3", start: localDateTime('2012-05-15T09:00:00')},
                        {label: "recevent3", start: localDateTime('2012-06-03T20:00:00')},
                        {label: "recevent3", start: localDateTime('2012-07-03T20:00:00')},
                        {label: "recevent3", start: localDateTime('2012-08-03T20:00:00')},
                        {label: "recevent3", start: localDateTime('2012-09-03T20:00:00')}
                    ]
                }
            ]
    }]
    }

    // initialize db with normal and recurring items, some are in mode time period some not
    // change time period and check correct items are in model
    function test_changeTimePeriod(data) {
        var j = 0;
        for (var i in data.managers) {
            console.log("Testing "+data.managers[i]+" backend")
            model.manager = data.managers[i];
            model.autoUpdate = true;
            model.startPeriod = localDate('2011-12-01');
            model.endPeriod = localDate('2012-04-30');
            spyManagerChanged.wait(spyWaitDelay)
            cleanDatabase();
            compare(model.itemCount, 0, "Model not empty")
            for (j = 0; j < data.definitions.length; j++) {
                var testEvent = createTestItemFromData(data.definitions[j]);
                model.saveItem(testEvent);
                if (data.definitions[j].affectsModel)
                    modelChangedSpy.wait(spyWaitDelay)
            }
            compareResultDatesToModel(data.results, model);

            skip("TODO Currently fails");
            for (j = 0; j < data.timePeriods.length; j++) {
                model.autoUpdate = data.timePeriods[j].autoUpdate;
                if (data.timePeriods[j].start !== undefined) {
                    model.startPeriod = data.timePeriods[j].start;
                    if (model.autoUpdate)
                        modelChangedSpy.wait(spyWaitDelay);
                }
                if (data.timePeriods[j].end !== undefined) {
                    model.endPeriod = data.timePeriods[j].end;
                }
                if (!model.autoUpdate) {
                    model.update();
                }
                modelChangedSpy.wait(spyWaitDelay);
                compareResultDatesToModel(data.timePeriods[j].results, model);
            }
            model.autoUpdate = true;
            cleanDatabase();
            compare(model.itemCount, 0, "Model not empty")
        }
    }



    function test_modifyParentItems_data() {
        return [{
                managers: utility.getManagerList(),
                definitions: [
                {
                    event :
                    {
                        "displayLabel" : "event0",
                        "start" : localDateTime('2011-10-25T15:00:00'),
                        "end" : localDateTime('2011-10-10T16:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: false
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent1",
                        "start" : localDateTime('2012-01-01T14:00:00'),
                        "end" : localDateTime('2012-01-01T15:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Daily,
                        "limit": localDate('2012-01-03'),
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event1",
                        "start" : localDateTime('2012-01-02T15:00:00'),
                        "end" : localDateTime('2012-01-02T16:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event2",
                        "start" : localDateTime('2012-01-03T11:00:00'),
                        "end" : localDateTime('2012-01-03T18:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent2",
                        "start" : localDateTime('2011-01-01T16:00:00'),
                        "end" : localDateTime('2011-01-01T17:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 15,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event3",
                        "start" : localDateTime('2012-05-15T09:00:00'),
                        "end" : localDateTime('2012-05-15T10:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: false
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent3",
                        "start" : localDateTime('2012-06-03T20:00:00'),
                        "end" : localDateTime('2012-06-03T21:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 4,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: false
                }


                ],
                results: [
                          {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                          {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                          {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                          {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                          {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                          {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                          {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                          {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                          {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                ],
                modifications: {
                addRule:
                {
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
                    results: [
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-03T15:00:00')},
                        {label: "event1", start: localDateTime('2012-01-04T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                    ]
                },
                modifyRule1:
                {
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 3,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    results: [
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "event1", start: localDateTime('2012-02-02T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event1", start: localDateTime('2012-03-02T15:00:00')}
                    ]
                },
                modifyRule2:
                {
                    rrule: {
                        "frequency": RecurrenceRule.Weekly,
                        "limit": 3,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    results: [
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-09T15:00:00')},
                        {label: "event1", start: localDateTime('2012-01-16T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                    ]
                },
                removeRule:
                {
                    results: [
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                    ]
                }
            }
    }]
    }

    // initialize model and modify one normal item:
    // first add a daily recurrence rule then modify recurrence rule from daily to monthly,
    // then to weekly and finally remove rule
    function test_modifyParentItems(data) {
        var j = 0;
        for (var i in data.managers) {
            console.log("Testing "+data.managers[i]+" backend")
            model.manager = data.managers[i];
            model.startPeriod = localDate('2011-12-01');
            model.endPeriod = localDate('2012-04-30');
            model.autoUpdate = true;
            spyManagerChanged.wait(spyWaitDelay)
            cleanDatabase();
            compare(model.itemCount, 0, "Model not empty")
            for (j = 0; j < data.definitions.length; j++) {
                var testEvent = createTestItemFromData(data.definitions[j]);
                model.saveItem(testEvent);
                if (data.definitions[j].affectsModel)
                    modelChangedSpy.wait(spyWaitDelay)
            }
            compareResultDatesToModel(data.results, model);

            // addRule, start modifying event1 by adding daily recurrence rule

            // need to fetch the event we are going to modify separately, because as soon
            // as we make it a recurring event, it will be deleted from the model
            fetchSpy.clear();
            model.fetchItems([model.items[4].itemId]);
            fetchSpy.wait(spyWaitDelay);
            var testItem = test.fetchedItem;
            var newRule = createTestRuleFromData(data.modifications.addRule);
            testItem.recurrence.recurrenceRules = [newRule];
            model.saveItem(testItem);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.addRule.results, model);

            // modifyRule1, change rule to monthly
            var modRule1 = createTestRuleFromData(data.modifications.modifyRule1);
            testItem.recurrence.recurrenceRules = [modRule1];
            model.saveItem(testItem);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.modifyRule1.results, model);

            // modifyRule2, change rule to weekly
            var modRule2 = createTestRuleFromData(data.modifications.modifyRule2);
            testItem.recurrence.recurrenceRules = [modRule2];
            model.saveItem(testItem);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.modifyRule2.results, model);

            // removeRule
            testItem.recurrence.recurrenceRules = [];
            model.saveItem(testItem);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.removeRule.results, model);

            // clean db
            model.startPeriod = localDate('2011-01-01');
            model.endPeriod = localDate('2012-08-30');
            modelChangedSpy.wait(spyWaitDelay);
            cleanDatabase();
            compare(model.itemCount, 0, "Model not empty")
        }
    }

    function test_exceptionOccurrences_data() {
        return [{
                managers: utility.getManagerList(),
                definitions: [
                {
                    event :
                    {
                        "displayLabel" : "event0",
                        "start" : localDateTime('2011-10-25T15:00:00'),
                        "end" : localDateTime('2011-10-10T16:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent1",
                        "start" : localDateTime('2012-01-01T14:00:00'),
                        "end" : localDateTime('2012-01-01T15:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Daily,
                        "limit": localDate('2012-01-03'),
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event1",
                        "start" : localDateTime('2012-01-02T15:00:00'),
                        "end" : localDateTime('2012-01-02T16:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event2",
                        "start" : localDateTime('2012-01-03T11:00:00'),
                        "end" : localDateTime('2012-01-03T18:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent2",
                        "start" : localDateTime('2011-01-01T16:00:00'),
                        "end" : localDateTime('2011-01-01T17:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 15,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: true
                },
                {
                    event :
                    {
                        "displayLabel" : "event3",
                        "start" : localDateTime('2012-05-15T09:00:00'),
                        "end" : localDateTime('2012-05-15T10:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    affectsModel: false
                },
                {
                    event :
                    {
                        "displayLabel" : "recevent3",
                        "start" : localDateTime('2012-06-03T20:00:00'),
                        "end" : localDateTime('2012-06-03T21:00:00'),
                        "recurrenceDates": [],
                        "exceptionDates": []
                    },
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": 4,
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    affectsModel: false
                }


                ],
                results: [
                    {label: "recevent2", start: localDateTime('2011-10-01T16:00:00')},
                    {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                    {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                    {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                    {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                    {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                    {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                    {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                    {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                    {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                    {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                    {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')}
                ],
                modifications: {
                addRule:
                {
                    rrule: {
                        "frequency": RecurrenceRule.Monthly,
                        "limit": localDate('2012-05-31'),
                        "interval": 1,
                        "daysOfWeek": [],
                        "daysOfMonth": [],
                        "daysOfYear": [],
                        "monthsOfYear": [],
                        "positions": [],
                        "firstDayOfWeek": Qt.Monday
                    },
                    results: [
                        {label: "recevent2", start: localDateTime('2011-10-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-11-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-12-25T15:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "event0", start: localDateTime('2012-01-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-02-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-03-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-04-25T15:00:00')}
                    ]
                },
                addException:
                {
                    results: [
                        {label: "recevent2", start: localDateTime('2011-10-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-11-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-12-25T15:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-02-02T15:00:00')},
                        {label: "event0", start: localDateTime('2012-02-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-03-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-04-25T15:00:00')}
                    ]
                },
                removeException:
                {
                    results: [
                        {label: "recevent2", start: localDateTime('2011-10-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-11-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-12-25T15:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-02-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-03-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-04-25T15:00:00')}
                    ]
                },
                addException2:
                {
                    results: [
                        {label: "recevent2", start: localDateTime('2011-10-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-11-25T15:00:00')},
                        {label: "modifiedrecevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-12-25T15:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-02-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-03-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-04-25T15:00:00')}
                    ]
                },
                removeGeneratedOccurrence:
                {
                    results: [
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2011-11-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-11-25T15:00:00')},
                        {label: "modifiedrecevent2", start: localDateTime('2011-12-01T16:00:00')},
                        {label: "event0", start: localDateTime('2011-12-25T15:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-01-01T16:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "recevent2", start: localDateTime('2012-02-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-02-25T15:00:00')},
                        {label: "recevent2", start: localDateTime('2012-03-01T16:00:00')},
                        {label: "event0", start: localDateTime('2012-03-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-04-25T15:00:00')}
                    ]
                },
                removeParent:
                {
                    results: [
                        {label: "event0", start: localDateTime('2011-10-25T15:00:00')},
                        {label: "event0", start: localDateTime('2011-11-25T15:00:00')},
                        {label: "event0", start: localDateTime('2011-12-25T15:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')},
                        {label: "event0", start: localDateTime('2012-02-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-03-25T15:00:00')},
                        {label: "event0", start: localDateTime('2012-04-25T15:00:00')}
                    ]
                },
                removeParent2:
                {
                    results: [
                        {label: "recevent1", start: localDateTime('2012-01-01T14:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-02T14:00:00')},
                        {label: "event1", start: localDateTime('2012-01-02T15:00:00')},
                        {label: "event2", start: localDateTime('2012-01-03T11:00:00')},
                        {label: "recevent1", start: localDateTime('2012-01-03T14:00:00')}
                    ]
                }
            }
    }]
    }

    // initialize model (store one normal event to variable and change model timePeriod so that stored event is not part of the model anymore)
    // modify stored item by adding a daily recurrence rule
    // then add exception, modify event time to move occurrence to different index
    // then remove exception occurrence
    // create another exception occurrence with different displayLabel
    // remove one generated occurrence
    // remove whole parent item

    function test_exceptionOccurrences(data) {
        var j = 0;
        for (var i in data.managers) {
            console.log("Testing "+data.managers[i]+" backend")
            model.manager = data.managers[i];
            model.startPeriod = localDate('2011-10-01');
            model.endPeriod = localDate('2012-04-30');
            model.autoUpdate = true;
            spyManagerChanged.wait(spyWaitDelay)
            cleanDatabase();
            compare(model.itemCount, 0, "Model not empty")
            for (j = 0; j < data.definitions.length; j++) {
                var testItem = createTestItemFromData(data.definitions[j]);
                model.saveItem(testItem);
                if (data.definitions[j].affectsModel)
                    modelChangedSpy.wait(spyWaitDelay)
            }
            compareResultDatesToModel(data.results, model);

            var testEvent = model.items[1];
            var testEventId = testEvent.itemId;

            // addRule, start modifying event0 by adding monthly recurrence rule
            var newRule = createTestRuleFromData(data.modifications.addRule);
            testEvent.recurrence.recurrenceRules = [newRule];
            model.saveItem(testEvent);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.addRule.results, model);

            // addException, modify generated occurrence and save it
            var xoccurrence = model.items[12];
            xoccurrence.startDateTime = localDateTime('2012-02-02T15:00:00');
            xoccurrence.endDateTime = localDateTime('2012-02-02T16:00:00');
            model.saveItem(xoccurrence);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.addException.results, model);

            // removeException
            var xoccurrenceRemove = model.items[13];
            model.removeItem(xoccurrenceRemove);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.removeException.results, model);

            // addException2, change displayLabel
            var xoccurrence2 = model.items[4];
            xoccurrence2.displayLabel = "modifiedrecevent2";
            model.saveItem(xoccurrence2);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.addException2.results, model);

            // removeGeneratedOccurrence
            var occurrenceRemove = model.items[0];
            model.removeItem(occurrenceRemove);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.removeGeneratedOccurrence.results, model);

            // removeParent
            // handle an occurrence of recevent2
            var parentsOccurrence = model.items[1];
            //find out parent id
            var parentId = parentsOccurrence.parentId;
            model.removeItem(parentId);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.removeParent.results, model);

            // removeParent2
            // handle an occurrence of event0
            var parentsOccurrence2 = model.items[0];
            //find out parent id
            var parentId2 = parentsOccurrence2.parentId;
            model.removeItem(parentId2);
            modelChangedSpy.wait(spyWaitDelay);
            compareResultDatesToModel(data.modifications.removeParent2.results, model);

            // clean db
            model.startPeriod = localDate('2011-01-01');
            model.endPeriod = localDate('2012-08-30');
            modelChangedSpy.wait(spyWaitDelay);
            cleanDatabase();
            compare(model.itemCount, 0, "Model not empty")
        }
    }




    // Helper functions

    function cleanDatabase() {
        var ids = [];
        var removeIds = [];
        var i;
        var j;
        var parentId;
        for (i = 0; i < model.itemCount; i++) {
            if (model.items[i].itemType == Type.EventOccurrence || model.items[i].itemType == Type.TodoOccurrence)
                ids.push(model.items[i].parentId)
            if (model.items[i].itemId !== "qtorganizer:::")
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

        modelChangedSpy.clear()
        if (removeIds.length > 0) {
            model.removeItems(removeIds)
            modelChangedSpy.wait(spyWaitDelay)
        }
        compare(model.items.length, 0)
    }

    function createTestItemFromData(data) {
        var testEvent = Qt.createQmlObject("import QtOrganizer 5.0; Event { }", test);
        var testRule = Qt.createQmlObject("import QtOrganizer 5.0; RecurrenceRule { }", test);
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
            testEvent.recurrence.recurrenceRules = [testRule];

        }
        testEvent.displayLabel = data.event.displayLabel;
        testEvent.startDateTime = new Date(data.event.start);
        testEvent.endDateTime = new Date(data.event.end);
        testEvent.recurrence.recurrenceDates = data.event.recurrenceDates;
        testEvent.recurrence.exceptionDates = data.event.exceptionDates;
        return testEvent;
    }

    function createTestRuleFromData(data) {
        var testRule = Qt.createQmlObject("import QtOrganizer 5.0; RecurrenceRule { }", test);
        testRule.frequency = data.rrule.frequency;
        testRule.limit = data.rrule.limit;
        testRule.interval = data.rrule.interval;
        testRule.daysOfWeek = data.rrule.daysOfWeek;
        testRule.daysOfMonth = data.rrule.daysOfMonth;
        testRule.daysOfYear = data.rrule.daysOfYear;
        testRule.monthsOfYear = data.rrule.monthsOfYear;
        testRule.positions = data.rrule.positions;
        testRule.firstDayOfWeek = data.rrule.firstDayOfWeek;
        return testRule;
    }

    function compareResultDatesToModel(results, model) {
        compare(model.itemCount, results.length, "Item count is wrong.")
        for (var i = 0; i < results.length; i++) {
            var itemDisplayLabel = model.items[i].displayLabel;
            var itemStart = model.items[i].startDateTime;
            compare(itemDisplayLabel, results[i].label, "Item displayLabel is not correct at " + i);
            compare(itemStart, results[i].start, "Item start date is not correct at " + i)
        }
    }

}

