/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Pim Module.
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
//![QML Organizer List View Example]
//![Create Organizer Rectangle]
import QtQuick 2.0
import QtOrganizer 5.0
import "content"

Rectangle {
    id: organizerApplication
    width: 400
    height: 600
    state: "EventListView"
//![Create Organizer Rectangle]

//![Create Empty Item Function]
    function createEmptyItem() {
        var newEvent = Qt.createQmlObject("import QtOrganizer 5.0; Event { }",
                                          organizer)
        newEvent.displayLabel = "NEW Event"
        newEvent.startDateTime = new Date()
        newEvent.endDateTime = new Date()
        return newEvent
    }
//![Create Empty Item Function]

//![ListView Mouse Position  Function]
    function setListViewMouseItem(mouseX, mouseY) {
        var indexedItem = eventView.indexAt(mouseX, mouseY)
        if (indexedItem != -1)
            eventView.currentIndex = indexedItem
    }
//![ListView Mouse Position  Function]

//![Create a title]
    Rectangle {
        id: organizerTitle
        border.color: "black"
        border.width: 4
        radius: 5
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: organizerTitleLabel.height

        Text {
            id: organizerTitleLabel

            height: font.pixelSize + 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Organizer Events"
            font.pixelSize: style.fontPixelSize * 1.4
        }
    }
//![Create a title]

//![Create calendar Window Rectangle]
    Rectangle {
        id: organizerRectangle
        anchors {
            top: organizerTitle.bottom
            left: parent.left
            right: parent.right
        }
        height: organizerApplication.height - organizerTitle.height - eventButtons.height
        color: "white"
//![Create calendar Window Rectangle]

//![ListView Highlight Component]
        Component {
            id: eventHighlight
            Rectangle {
                height: eventView.currentItem.height
                width: organizerRectangle.width

                color: "lightsteelblue"
                radius: 5
                border {
                    color: "black"
                    width: 1
                }

                y: eventView.currentItem.y
                Behavior on y {
                    SpringAnimation {
                        spring: 3
                        damping: 0.2
                    }
                }
            }
        }
//![ListView Highlight Component]

//![ListView]
        ListView {
            id: eventView
            anchors.fill: parent
            clip: true
            property bool readOnlyEvent: true

            delegate: eventDelegate
            model: organizer
            focus: true
            highlight: eventHighlight

            MouseArea {
                anchors.fill: parent
                id: listViewMouseArea
                onClicked: {
                    setListViewMouseItem(mouseX, mouseY)
                    organizerApplication.state = "EventEditorView"
                    eventEditor.eventItem = organizer.items[eventView.currentIndex]
                }
            }
        }
    }
//![ListView]

//![Organizer Model]
    OrganizerModel {
        id: organizer
        startPeriod: new Date("1970-01-01")
        endPeriod: new Date("2012-12-31")
        //manager:"jsondb"
        manager: "memory"

        Component.onCompleted: {
            if (managerName == "memory") {
                organizer.importItems(Qt.resolvedUrl(
                                          "content/organizer_ical_test.ics"))
            }
        }
    }
//![Organizer Model]

    Component {
        id: eventDelegate

//![Organizer ListView Delegate]
        Column {
            id: eventDelegateWrapper
            width: organizerRectangle.width
            height: ListView.isCurrentItem ? (displayLabel.height
                                              + startTime.height + endTime.height)
                                             * 1.6 : (displayLabel.height + startTime.height
                                                      + endTime.height) * 1.2

            Grid {
                columns: 2
                spacing: 3

                Text {
                    font.pixelSize: 14
                    text: "Event: "
                }
                TextEdit {
                    id: displayLabel
                    font.pixelSize: 14
                    text: (model.item) ? model.item.displayLabel : ""
                    readOnly: true
                }
                Text {
                    font.pixelSize: 12
                    text: "Start: "
                }
                TextEdit {
                    id: startTime
                    font.pixelSize: 12
                    text: (model.item) ? Qt.formatDate(
                                             model.item.startDateTime) : ""
                    readOnly: true
                }
                Text {
                    font.pixelSize: 12
                    text: "End: "
                }
                TextEdit {
                    id: endTime
                    font.pixelSize: 12
                    text: (model.item) ? Qt.formatDate(
                                             model.item.endDateTime) : ""
                    readOnly: true
                }
            }
        }
    }
//![Organizer ListView Delegate]

//![Create and Delete Event Buttons]
    Row {
        spacing: 2
        id: eventButtons
        anchors {
            top: organizerRectangle.bottom
            left: parent.left
            right: parent.right
        }

        GenericButton {
            width: parent.width / 2
            buttonText: "Add New Event"

            onClicked: {
                organizerApplication.state = "EventEditorView"
                eventEditor.eventItem = organizerApplication.createEmptyItem()
            }
        }
    }
//![Create and Delete Event Buttons]

//![Event Editor]
    EventEditor {
        id: eventEditor
        anchors.fill: parent
        color: "white"
        opacity: 0
    }
//![Event Editor]

//![States]
    states: [
        State {
            name: "EventListView"
            PropertyChanges {
                target: eventEditor
                opacity: 0
            }
        },
        State {
            name: "EventEditorView"
            PropertyChanges {
                target: eventEditor
                opacity: 1
            }
        }
    ]
//![States]

}
//![QML Organizer List View Example]
