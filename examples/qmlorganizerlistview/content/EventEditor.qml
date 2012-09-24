/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Organizer Module.
**
** $QT_BEGIN_LICENSE:FDL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Free Documentation License Usage
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.  Please review the following information to ensure
** the GNU Free Documentation License version 1.3 requirements
** will be met: http://www.gnu.org/copyleft/fdl.html.
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

