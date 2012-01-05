/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Organizer Module.
**
** $QT_BEGIN_LICENSE:FDL$
** GNU Free Documentation License
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms
** and conditions contained in a signed written agreement between you
** and Nokia.
**
**
**
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
        if (eventItem.itemId != "qtorganizer:::")
            organizer.saveItem(eventItem.itemId)
        else
            organizer.saveItem(eventItem)
    }
    //![Save Item Function]

    //![Grid Editable Data]
    Column {
        anchors.fill: parent
        spacing: 5
    //![Grid Editable Data]

        //![Create Event Title]
        Row {
            spacing: 2

            Text {
                text: "Event Label:"
            }
            TextEdit {
                id: displayLabel
                text: (eventItem) ? eventItem.displayLabel : ""
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
            width: parent.width / 2
            buttonText: "Cancel"

            onClicked: {
                organizerApplication.state = "EventListView"
            }
        }

        GenericButton {
            width: parent.width / 2
            buttonText: "Save"

            onClicked: {
                saveItem()
                organizerApplication.state = "EventListView"
            }
        }
    }
    //![Cancel and Save Buttons]
}//![Event Editor]

