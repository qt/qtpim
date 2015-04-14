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
    id: test
    name: "OrganizerE2ETests"

    property int spyWaitDelay: 250

    QOrganizerTestUtility {
        id: utility
    }

    OrganizerModel {
        id: model
        startPeriod:'2009-01-01';
        endPeriod:'2012-12-31';
    }

    SignalSpy {
        id: spyManagerChanged
        signalName: "managerChanged"
        target: model
    }

    SignalSpy {
        id: spyModelChanged
        signalName: "modelChanged"
        target: model
    }

    function cleanup() {
        model.manager = ""
    }

    function test_megaitems_data() {
        return [{
            managers: utility.getManagerList(),
            definitions: {

                "Event": {
                    "Type": {},
                    "Guid": {},
                    "DisplayLabel": {
                        "label": "'dummy'"
                    },
                    "Description": {
                        "description": "'dummy'"
                    },
                    "Comment": {
                        "comment": "'dummy'"
                    },
                    "Tag": {
                        "tag": "'dummy'"
                    },
                    "ExtendedDetail": {
                        "name": "'dummy'",
                        "data": "'dummy'"
                    },
                    "EventTime": {
                        "startDateTime": "'2012-01-01'",
                        "endDateTime": "'2012-01-01'"
                    },
                    "Priority": {
                        "priority": "Priority.Medium"
                    },
                    "Location": {
                        "label": "'dummy'",
                        "longitude": "1.2",
                        "latitude": "3.4",
                    },
                    "AudibleReminder": {
                        "dataUrl": "'url'"
                    },
                    "EventAttendee": {
                        "name": "'dummy'",
                        "emailAddress": "'new.attendee@qt.com'",
                        "attendeeId": "'123444455555'",
                        "participationStatus": "EventAttendee.StatusAccepted",
                        "participationRole": "EventAttendee.RoleRequiredParticipant"
                    },
                    "EventRsvp": {
                        "organizerName": "'Oswald Organizer'",
                        "organizerEmail": "'ossy@waldo.com'",
                        "participationStatus": "EventAttendee.StatusDelegated",
                        "participationRole": "EventAttendee.RoleChairperson",
                        "responseRequirement": "EventRsvp.ResponseRequired",
                        "responseDeadline": "'2012-10-10'",
                        "responseDate": "'2012-01-01'"
                    }
                },

                "Todo": {
                    "Type": {},
                    "Guid": {},
                    "DisplayLabel": {
                        "label": "'dummy'"
                    },
                    "Description": {
                        "description": "'dummy'"
                    },
                    "Comment": {
                        "comment": "'dummy'"
                    },
                    "Tag": {
                        "tag": "'dummy'"
                    },
                    "ExtendedDetail": {
                        "name": "'dummy'",
                        "data": "'dummy'"
                    },
                    "TodoTime": {
                        "startDateTime": "'2012-01-01'",
                        "dueDateTime": "'2012-01-02'"
                    },
                    "Priority": {
                        "priority": "Priority.Medium"
                    },
                    "AudibleReminder": {
                        "dataUrl": "'url'"
                    }
                }
            }
        }]
    }

    function test_megaitems(data) {
        var qmlItems = createQMLItemsFromHash(data.definitions)
        for (var i in data.managers) {
            console.log("Testing "+data.managers[i]+" backend")

            model.manager = data.managers[i]
            spyManagerChanged.wait(spyWaitDelay)
            cleanDatabase()
            for (var j in qmlItems) {
                model.saveItem(qmlItems[j])
                spyModelChanged.wait(spyWaitDelay)
            }
            compare(model.itemCount, qmlItems.length, "Items were not successfully saved.")
            compareViewToModel(qmlItems, model)
            cleanDatabase()
        }
    }



    // Helper functions

    function cleanDatabase() {

        var ids = model.itemIds()

        spyModelChanged.clear()
        if (ids.length > 0) {
            model.removeItems(ids)
            spyModelChanged.wait(spyWaitDelay)
        }

        compare(model.itemIds().length, 0)
    }

    function createQMLItemsFromHash(data) {

        var qmlItems = []

        for (var itemDef in data) {

            var item = Qt.createQmlObject(
                    "import QtOrganizer 5.0;" +
                    itemDef + "{id: item" + itemDef.toLowerCase() + "}",
                    test)

            for (var detailDef in data[itemDef]) {

                // Don't override the type
                if (detailDef == "Type")
                    continue

                var s = "import QtOrganizer 5.0;" +
                    detailDef + "{" +
                    "id: detail" + detailDef.toLowerCase() + "; "

                for (var fieldDef in data[itemDef][detailDef]) {
                    s += fieldDef + ": " + data[itemDef][detailDef][fieldDef] + "; "
                }
                s += "}"

                var detail = Qt.createQmlObject(s, test)
                var success = item.setDetail(detail)

            }

            qmlItems.push(item)

        }

        return qmlItems
    }

    function compareViewToModel(qmlItems, model) {

        for (var i = 0; i < qmlItems.length; i++) {
            var match = false
            for (var j = 0; j < model.itemCount; j++) {

                if (qmlItems[i].itemType == model.items[j].itemType) {

                    for (var k = 0; k < qmlItems[i].itemDetails.length; k++) {

                        match = false
                        for (var l = 0; l < model.items[j].itemDetails.length; l++) {
                            if (qmlItems[i].itemDetails[k].type == model.items[j].itemDetails[l].type) {
                                match = true;
                                var originalDetail = qmlItems[i].itemDetails[k];
                                var modelDetail = model.items[j].itemDetails[l];
                                var fieldType = originalDetail.type;
                                // 10 is big enough number to go through all the fields of invidual detail
                                for (var m = 0; m < 10; m++) {
                                    fieldType++;
                                    var originalFieldValue = originalDetail.value(fieldType);
                                    var modelFieldValue = modelDetail.value(fieldType);
                                    if (originalFieldValue == undefined) {
                                        if (fieldType == Guid.FieldGuid) {
                                            // exceptions, where model is updating the fields so they differ from original test set
                                            continue;
                                        }
                                        verify(modelFieldValue == undefined)
                                        break;// all fields for this detail have been checked
                                    }
                                    compare(modelFieldValue, originalFieldValue)
                                }

                                break;
                            }
                        }
                        compare(match, true)
                    }

                    break;
                }
            }
            compare(match, true)
        }
    }

}
