/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Mobility Components.
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
import QtOrganizer 5.0

Rectangle {
    id: attendeeDetailsView
    anchors.top: menuBar.bottom
    opacity: 0
    width: calendar.width;
    height: calendar.height - menuBar.height
    property variant item

    onItemChanged: {
        attendeeEditNameRow.value = "name";
        attendeeEditEmailRow.value = "name@qt.nokia.com";
        attendeeEditAttendeeIdRow.value = "contact id";
        attendeeEditRoleRow.value = 4;
    }

    Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.8 }
    color: "#343434";

    Column {
        Repeater {
            model: item? item.attendees : 0
            Rectangle {
                width: attendeeDetailsView.width;
                height: 25
                Text {
                    id : nameText;
                    width: parent.width
                    text: "attendee " + index + " : " + name + "," + emailAddress;
                }
                Button {
                    text: "remove"
                    width: parent.width / 6
                    height: 30
                    anchors.right: nameText.right
                    onClicked: {
                        item.removeDetail(modelData);
                    }
                }
            }
        }

        FieldRow {
            id: attendeeEditNameRow
            label: "Attendee name"
            value: "name"
        }
        FieldRow {
            id: attendeeEditEmailRow
            label: "Attendee email"
            value: "name@qt.nokia.com"
        }
        FieldRow {
            id: attendeeEditAttendeeIdRow
            label: "Attendee attendeeId"
            value: ""
        }
        RollerRow {
            id: attendeeEditRoleRow
            label: "Attendee role"
            height: 80
            valueSet: ["Unknown", "Organizer", "Chairperson", "Host", "RequiredParticipant", "OptionalParticipant", "NonParticipant"]
        }
        RollerRow {
            id: attendeeEditStatusRow
            label: "Attendee Status"
            height: 80
            valueSet: ["Unknown", "StatusAccepted", "StatusDeclined", "StatusTentative", "StatusDelegated", "StatusInProcess", "StatusCompleted"]
        }
        Row {
            Button {
                id: addAttendeeButton
                text: "Add New"
                width: attendeeDetailsView.width / 2
                onClicked: {
                    if (attendeeEditNameRow.newValue) {
                        var eventAttendee = Qt.createQmlObject("import QtOrganizer 5.0;EventAttendee {}", organizer);
                        eventAttendee.name = attendeeEditNameRow.newValue;
                        if (attendeeEditEmailRow.newValue)
                            eventAttendee.emailAddress = attendeeEditEmailRow.newValue;
                        if (attendeeEditAttendeeIdRow.newValue)
                            eventAttendee.attendeeId = attendeeEditAttendeeIdRow.newValue;
                        eventAttendee.participationRole = attendeeEditRoleRow.valueRoller.selectedValue()
                        eventAttendee.participationStatus = attendeeEditStatusRow.valueRoller.selectedValue()
                        item.setDetail(eventAttendee);
                    }
                }
            }
            Button {
                id: closeButton
                text: "Close"
                width: attendeeDetailsView.width / 2
                onClicked: {
                    calendar.state = "DetailsView";
                }
            }
        }
    }
}
