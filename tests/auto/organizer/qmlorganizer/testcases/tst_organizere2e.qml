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
    name: "OrganizerE2ETests"

    property OrganizerModel model: {}
    property SignalSpy spy: {}

    function initTestCase() {
        model = Qt.createQmlObject(
              "import QtOrganizer 5.0;"
            + "OrganizerModel {"
            + "   startPeriod:'2009-01-01';"
            + "   endPeriod:'2012-12-31'; }"
            , test);

        spy = Qt.createQmlObject( "import QtTest 1.0 \nSignalSpy {}", test);
        spy.target = model;
        spy.signalName = "modelChanged";
    }

    function cleanup() {
        model.destroy()
        spy.destroy()
    }

    function test_megaitems_data() {
        return [{
                managers: ["memory", "jsondb"],
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
            wait(500) // Todo: replace with spy.wait()
            cleanDatabase()
            for (var j in qmlItems) {
                model.saveItem(qmlItems[j])
                spy.wait()
            }
            compare(model.itemCount, qmlItems.length, "Items were not successfully saved.")
            compareViewToModel(qmlItems, model)
            cleanDatabase()
        }
    }



    // Helper functions

    function cleanDatabase() {

        var ids = model.itemIds()

        spy.clear()
        if (ids.length > 0) {
            model.removeItems(ids)
            spy.wait()
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
