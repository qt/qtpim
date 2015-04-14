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
    name: "detailFilterTests"
    id: detailFilterTests

    property OrganizerModel organizerModel;
    property int spyWaitDelay: 250

    QOrganizerTestUtility {
        id: utility
    }

    // UTILITIES
    function create_testobject(ctorString) {
        var newObject = Qt.createQmlObject(ctorString, detailFilterTests);
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
        return detail;
    }

    function applyFilter(detailToMatch) {
        var filterChangedSpy = utility.create_spy(organizerModel, "filterChanged");
        var modelChangedSpy = utility.create_spy(organizerModel, "modelChanged");
        var filter = create_detailFilter();
        filter.detail = detailToMatch;
        compare(organizerModel.error, "NoError");
        organizerModel.filter = filter;
        filterChangedSpy.wait();
        compare(filterChangedSpy.count, 1);
        modelChangedSpy.wait()
        compare(modelChangedSpy.count, 1);
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
                        + "detail: Description {\n"
                           + "description: 'my value'\n"
                           + "}\n"
                        + "}\n"
                }
        ]
    }
    function test_creation(data) {
        console.log("");//to print out test tags for every data set
        var newDetailFieldFilter = create_testobject(data.code, detailFilterTests);
    }

    // detail API
    function test_detail_data() {
        return [
                    {tag: "empty detail filter",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "Description {\n"
                        + "}\n"
                },
                    {tag: "valid detail",
                     code: "import QtQuick 2.0 \n"
                        + "import QtOrganizer 5.0\n"
                        + "Description {\n"
                           + "description: 'my value'\n"
                           + "}\n"
                }
        ]
    }
    function test_detail(data) {
        var newDetailToMatch = create_testobject(data.code, detailFilterTests);
        var newDetailFilter = create_detailFilter();
        var valueChangedSpy = utility.create_spy(newDetailFilter, "valueChanged");
        var filterChangedSpy = utility.create_spy(newDetailFilter, "filterChanged");
        // change
        newDetailFilter.detail = newDetailToMatch;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(newDetailFilter.detail, newDetailToMatch);
        // change without change
        newDetailFilter.detail = newDetailToMatch;
        compare(valueChangedSpy.count, 1);
        compare(filterChangedSpy.count, 1);
        compare(newDetailFilter.detail, newDetailToMatch);
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
                    {tag: "matching filter, TodoProgress", expectedItemsAmount: 1,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   TodoProgress {\n"
                                            + "     status: TodoProgress.InProgress\n"
                                            + "     percentageComplete: 89\n"
                                            + "   }"
                    },
                    {tag: "matching filter, Description", expectedItemsAmount: 1,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Description {\n"
                                            + "   description: 'for exact filter - Description'\n"
                                            + "   }"
                    },
                    {tag: "matching filter, ExtendedDetail - int value", expectedItemsAmount: 1,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "   name: 'intExtDetail'\n"
                                            + "   data: 123\n"
                                            + "   }",
                        isExtDetail: 1
                    },
                    {tag: "matching filter, ExtendedDetail - string value", expectedItemsAmount: 1,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "   name: 'stringExtDetail'\n"
                                            + "   data: '123'\n"
                                            + "   }",
                        isExtDetail: 1
                    },
                    {tag: "matching filter, ExtendedDetail - date value", expectedItemsAmount: 1,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "   name: 'stringExtDetail'\n"
                                            + "   data: new Date('2010-10-25T15:49:00')\n"
                                            + "   }",
                        isExtDetail: 1
                    },
                    {tag: "matching filter, EventTime", expectedItemsAmount: 5,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   EventTime {\n"
                                            + "      startDateTime:'2011-10-23T15:00:00Z'\n"
                                            + "      endDateTime: '2011-11-23T16:00:00'\n"
                                            + "   }",
                    },
                    {tag: "non-matching filter, Description", expectedItemsAmount: 0,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Description {\n"
                                            + "   description: 'NonMatchingDescription'\n"
                                            + "   }"
                    },
                    {tag: "non-matching filter, less fields", expectedItemsAmount: 0,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   TodoProgress {\n"
                                            + "     status: TodoProgress.InProgress\n"
                                            + "   }"
                    },
                    {tag: "non-matching filter, ExtendedDetail - different string data", expectedItemsAmount: 0,
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "   name: 'stringExtDetail'\n"
                                            + "   data: '123'\n"
                                            + "   }",
                        isExtDetail: 1,
                        nonMatchingCtrStr:  "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "   name: 'stringExtDetail'\n"
                                            + "   data: '456'\n"
                                            + "   }",
                    }
                ]
    }

    function test_filtering(data) {
        //preparations
        var managers = utility.getManagerList();
        var detailToMatch;
        for (var i=0;i<managers.length;i++) {

            var managerToBeTested = managers[i];
            console.log("## Testing plugin: " + managerToBeTested);
            organizerModel = utility.createModel(managerToBeTested);

            addEventsToModel(filterTestItems());
            compare(organizerModel.items.length, filterTestItems().length);

            if (data.filterDetailCtrStr) {
                if (data.isExtDetail) {
                    detailToMatch = addDetailWithoutConvenienceAPI(data.filterDetailCtrStr)
                    if (data.nonMatchingCtrStr)
                        detailToMatch = create_testobject(data.nonMatchingCtrStr)
                } else {
                    detailToMatch = create_testobject(data.filterDetailCtrStr);
                }
            }
            if (data.tag != "no filter") {
                applyFilter(detailToMatch);
            }

            compare(organizerModel.items.length, data.expectedItemsAmount);

            organizerModel.destroy();
        }
    }


    function test_errors_data() {
        return [
            // filtering certain details/definitions are currently not supported
                    {tag: "Unsupported details, Detail with no fields",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   JournalTime {\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, AudibleReminder",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   AudibleReminder {\n"
                                            + "       repetitionCount: 5\n"
                                            + "       repetitionDelay: 90\n"
                                            + "       secondsBeforeStart: 90\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, Comment",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Comment {\n"
                                            + "       comment: 'Item Comment'\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, EmailReminder",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   EmailReminder {\n"
                                            + "       repetitionCount: 5\n"
                                            + "       repetitionDelay: 90\n"
                                            + "       secondsBeforeStart: 90\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, ExtendedDetail with name field only",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "       name: 'property name'\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, ExtendedDetail with data field only",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   ExtendedDetail {\n"
                                            + "       data: 'property data'\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, JournalTime",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   JournalTime {\n"
                                            + "       entryDateTime: '2011-10-23T15:00:00Z'\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, Recurrence",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Recurrence {\n"
                                            + "       recurrenceDates: [new Date('2011-01-01')]\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, Reminder",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Reminder {\n"
                                            + "       repetitionCount: 5\n"
                                            + "       repetitionDelay: 90\n"
                                            + "       secondsBeforeStart: 90\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, Tag",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Tag {\n"
                                            + "       tag: 'Item Tag'\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, Timestamp",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   Timestamp {\n"
                                            + "       created: '2011-10-23T15:00:00Z'\n"
                                            + "   }"
                    },
                    {tag: "Unsupported details, VisualReminder",
                        filterDetailCtrStr: "import QtQuick 2.0\n"
                                            + "import QtOrganizer 5.0 \n"
                                            + "   VisualReminder {\n"
                                            + "       repetitionCount: 5\n"
                                            + "       repetitionDelay: 90\n"
                                            + "       secondsBeforeStart: 90\n"
                                            + "   }"
                    },
        ]
    }

    function test_errors(data) {
        console.log();
        // error codes are backend specific - currently untested
    }
}
