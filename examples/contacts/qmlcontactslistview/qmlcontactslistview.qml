/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Pim Module.
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
import QtQuick 2.0
import QtContacts 5.0

Rectangle {
    id: contactsApplication
    width: 400
    height: 600
    state: "listView"

    Rectangle {
        id: browsingArea
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: contactsApplication.height - btnArea.height - statusBar.height
        color: "lightblue"

        ContactModel {
            id: contactsModel
            manager: "memory"
            Component.onCompleted: {
                    contactsModel.importContacts(Qt.resolvedUrl("example.vcf"))
            }
            sortOrders: [
                SortOrder {
                    detail: ContactDetail.Name
                    field: Name.FirstName
                    direction: Qt.AscendingOrder
                }
            ]
        }

        // ![Contact delegate]
        Component {
            id: contactDelegate

            Rectangle {
                id: delRect
                width: parent.width
                height: 60
                border.width: 1
                border.color: "darkgreen"
                color: delRect.ListView.isCurrentItem ? "#F5678A" : browsingArea.color

                Column {
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 6
                    }
                    Text {
                        text: contact.name.firstName
                        font.bold: true
                    }
                    Text { text: contact.phoneNumber.number }
                }

                Keys.onReturnPressed: {
                    contactEditor.contact = contact
                    goToEditView()
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        contactsView.currentIndex = index
                    }
                }
            }
        }
        // ![Contact delegate]

        // ![ListView for showing the contacts]
        ListView {
            id: contactsView
            anchors.fill: browsingArea
            width: browsingArea.width
            height: browsingArea.height
            model: contactsModel
            focus: true
            clip: true
            delegate: contactDelegate
        }
        // ![ListView for showing the contacts]
    }

    // ![Button area]
    Rectangle {
        id: btnArea
        width: parent.width
        height: 20
        color: "#C7BFBF"
        anchors {
            bottom: statusBar.top
            right: parent.right
            left: parent.left
        }
        Row {
            id: btnRow
            anchors.centerIn: parent
            spacing: 10

            // buttons for 'listView' state
            GenericButton {
                id: btnNew
                width: 160
                buttonText: "Add New Contact"
                visible: true
                onClicked: {
                    contactEditor.contact = 0
                    goToEditView()
                }
            }
            GenericButton {
                id: btnEdit
                width: 160
                buttonText: "Edit Contact"
                visible: true
                onClicked: {
                    contactEditor.contact = contactsView.model.contacts[contactsView.currentIndex]
                    goToEditView()
                }
            }
            // buttons for 'editView' state
            GenericButton {
                id: btnCancel
                width: 120
                buttonText: "Cancel"
                visible: false
                onClicked: {
                    contactEditor.cancel()
                    goToListView()
                }
            }

            GenericButton {
                id: btnDelete
                width: 120
                buttonText: "Delete"
                visible: false
                onClicked: {
                    contactEditor.deleteContact()
                    goToListView()
                }
            }

            GenericButton {
                id: btnSave
                width: 120
                buttonText: "Save"
                visible: false
                onClicked: {
                    contactEditor.updateContact()
                    goToListView()
                }
            }
        }
    }
    // ![Button area]

    // ![Status bar area]
    Rectangle {
        id: statusBar
        anchors {
            bottom: parent.bottom
            right: parent.right
            left: parent.left
        }
        width: parent.width
        height: 24
        color: "gainsboro"

        Text {
            id: barText
            anchors.centerIn: parent
            text: " "
        }

        Timer {
            id: barTimer
            interval: 2000
            running: false
            onTriggered: {
                barText.text = " "
            }
        }

        function updateMsg(msg) {
            barText.text = msg
            barTimer.restart()
        }
    }
    // ![Status bar area]

    // ![Custom contact editor]
    ContactEditor {
        id: contactEditor
        height: parent.height - statusBar.height - btnArea.height
        z: -1
    }
    // ![Custom contact editor]

    // ![Applications state changes]
    states: [
        State {
            name: "listView"
            PropertyChanges {
                target: contactEditor
                z: -1
            }
            PropertyChanges {
                target: contactsView
                focus: true
            }
        },
        State {
            name: "editView"
            PropertyChanges {
                target: contactEditor
                z: 1
            }
            PropertyChanges { target: btnNew; visible: false }
            PropertyChanges { target: btnEdit; visible: false }
            PropertyChanges { target: btnCancel; visible: true }
            PropertyChanges { target: btnDelete; visible: true }
            PropertyChanges { target: btnSave; visible: true }
            StateChangeScript { script: contactEditor.resetToDefaults() }
        }
    ]
    // ![Applications state changes]

    function goToListView() {
        contactEditor.contact = ""
        contactsApplication.state = "listView"
    }

    function goToEditView() {
        contactsApplication.state = "editView"
    }
}
