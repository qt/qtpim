/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
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
