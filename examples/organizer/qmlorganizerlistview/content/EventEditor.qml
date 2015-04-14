/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Organizer Module.
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/
//![Event Editor]
//![Event Editor Setup]
import QtQuick 2.0

Rectangle {
    property variant eventItem
    //![Event Editor Setup]

    //![Save Item Function]
    function saveItem() {
        eventItem.displayLabel = displayLabel.text
        eventItem.startDateTime = new Date(startEvent.year + "," + startEvent.month
                                           + "," + startEvent.date)
        eventItem.endDateTime = new Date(endEvent.year + "," + endEvent.month
                                         + "," + endEvent.date)
        organizer.saveItem(eventItem)
    }
    //![Save Item Function]

    //![Grid Editable Data]
    Column {
        anchors.fill: parent
        spacing: 5
    //![Grid Editable Data]

        //![Create Event Title]

        Column {
            spacing: 2

            Text {
                id: editorTitleLabel
                text: "Event Label:"
            }
            Rectangle {
                id: organizerTitle
                border.color: "black"
                border.width: 2
                radius: 5
                width: displayLabel.width
                height: displayLabel.height
                TextEdit {
                    id: displayLabel
                    text: (eventItem) ? eventItem.displayLabel : ""
                }
            }
        }
        //![Create Event Title]

        //![Start Title]
        Text {
            text: "Start:"
        }

        //![Start Title]

        //![Events Start Date]
        EventDateTime {
            id: startEvent
            date: Qt.formatDate((eventItem) ? eventItem.startDateTime : "",
                                              "dd")
            month: Qt.formatDate((eventItem) ? eventItem.startDateTime : "",
                                               "MM")
            year: Qt.formatDate((eventItem) ? eventItem.startDateTime : "",
                                              "yyyy")
        }
        //![Events Start Date]

        //![End Title]
        Text {
            text: "End:"
        }
        //![End Title]

        //![Events End Date]
        EventDateTime {
            id:endEvent
            date: Qt.formatDate((eventItem) ? eventItem.endDateTime : "", "dd")
            month: Qt.formatDate((eventItem) ? eventItem.endDateTime : "", "MM")
            year: Qt.formatDate((eventItem) ? eventItem.endDateTime : "",
                                              "yyyy")
        }
    }
    //![Events End Date]

    //![Cancel and Save Buttons]
    Row {
        spacing: 2
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        GenericButton {
            width: parent.width / 3
            buttonText: "Cancel"

            onClicked: {
                organizerApplication.state = "EventListView"
            }
        }

        GenericButton {
            width: parent.width / 3
            buttonText: "Delete"

            onClicked: {
                organizerApplication.state = "EventListView"
                organizer.removeItem(eventItem)
            }
        }

        GenericButton {
            width: parent.width / 3
            buttonText: "Save"

            onClicked: {
                saveItem()
                organizerApplication.state = "EventListView"
            }
        }
    }
    //![Cancel and Save Buttons]
}//![Event Editor]

