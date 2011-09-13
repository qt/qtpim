/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
import QtAddOn.contacts 2.0
import com.nokia.components 1.0
import com.nokia.components.style 1.0

import "contents"
ApplicationWindow {
    id: screen; width: 320; height: 480
    property bool enabled: !pageStack.busy && !busy // global app flag
    property bool busy: false
    property string viewType : "contactListView"
    property bool showContact: false
    // let the ApplicationWindow push the first page
    initialPage: pageList



    Page {
        id: pageList

        title: "Contacts [" + contactModel.manager + "]: " +contactListView.count

        // TODO These should be the default/universal values
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        // Add a toolbar with a tool button which
        // pops the stack, bringing you back to the
        // previous page

        tools: clistToolbar

        ToolBarLayout {
            id: clistToolbar; objectName: "clistToolbar"
            anchors.bottom: parent.bottom

            ToolIcon {
                objectName: "toolIcon3"
                iconId: "clist.gadd"
                onClicked: {
                    var tempContact = Qt.createQmlObject(
                        "import QtAddOn.contacts 2.0;" +
                        "Contact {}", contactModel);

// NOTE:    Following code is commented out, since with
//          current approach in ContactView.qml (VisualItemModel),
//          no pre-filling of contact details is needed in the tempContact variable.
//
//                    var phone = Qt.createQmlObject("import QtAddOn.contacts 2.0;" +
//                                                   "PhoneNumber {contexts: ['Home']}", contactModel);
//                    var email = Qt.createQmlObject("import QtAddOn.contacts 2.0;" +
//                                                   "EmailAddress {}", contactModel);
//                    var address = Qt.createQmlObject("import QtAddOn.contacts 2.0;" +
//                                                     "Address {locality: \"\";street: \"\";country: \"\";postcode: \"\";}", contactModel);
//                    var name = Qt.createQmlObject("import QtAddOn.contacts 2.0;" +
//                                                  "Name {firstName: \"\";lastName: \"\"}", contactModel);
//                    tempContact.addDetail(name)
//                    tempContact.addDetail(phone)
//                    tempContact.addDetail(email)
//                    tempContact.addDetail(address)

                    pageStack.push(contactEditComponent, {contact: tempContact, addMode: true, mainModel: contactModel});
                }
                enabled: true
                visible: enabled
            }
            ToolIcon {
                objectName: "toolIcon4"
                // TEMP: we need correct toolbar icons from Qt-components when toolkit design stable and available
                //iconSource: "../images/toolbar_options.png"
                iconId: "ctoolbar.gcategories"
                onClicked: {
                    contextManagerSelectMenu.open()
                }
                enabled: true
                visible: enabled
            }
        }

        Rectangle {
            id: background
            anchors.fill: parent;

            color: "#343434";
            Image { source: "contents/images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 1 }
        }

        ContactModel {
            id: contactModel
            Component.onCompleted : {
                for (var i = 0; i < contactModel.availableManagers.length; i++)
                {
                    if (contactModel.availableManagers[i] != "invalid") {
                        console.log("Manager found: " + contactModel.availableManagers[i])
                        managersModel.append({"name": contactModel.availableManagers[i]})
                    }
                }

                if (manager == "memory")
                    contactModel.importContacts(Qt.resolvedUrl("contents/example.vcf"));
            }
            sortOrders: [
                SortOrder {
                    detail: ContactDetail.Name
                    field: Name.FirstName
                    direction: Qt.AscendingOrder
                }
            ]
            filter: phoneNumberFilter
        }

        DetailFilter {
            id: phoneNumberFilter
            detail: ContactDetail.PhoneNumber
            field: PhoneNumber.Number
            value: searchField.text
            matchFlags: Filter.MatchContains
        }

        Component {
            id: contactEditComponent
            ContactView {
                id: contactView


                onBackClicked: pageStack.pop()

                onDismissed: {screen.showContact = false;}
                onDeleted: {contactModel.removeContact(id);}
            }
        }

        TextField {
            id:searchField

            text: ""
            anchors.left:  parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            platformStyle: TextFieldStyle {
                paddingRight: searchButton.width
            }
            onFocusChanged: {
                if ((pageList.state == "List") && (searchField.state == "someText"))
                    searchField.forceActiveFocus()

            }

            states: [
                State {
                    name: "empty"
                    when: searchField.text == ""
                    PropertyChanges {
                        target: contactModel; filter: null
                    }
                },
                State {
                    name: "someText"
                    when: searchField.text != ""
                    PropertyChanges {
                        target: contactModel; filter: phoneNumberFilter
                    }
                }
            ]

            Item {
                id: searchButton
                width: searchImage.width+22
                height: searchImage.height+10
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                Image {
                    id: searchImage
                    anchors.centerIn: parent
                    source: "contents/images/search"+(searchArea.pressed?"-pressed":"")+".png"
                }
                MouseArea {
                    id: searchArea
                    anchors.fill: parent
                    anchors.margins: -8
                    onClicked: console.log("Icon was clicked")
                    hoverEnabled: true
                }
            }

        }
        ContactListView {
            id: contactListView
            width: parent.width
            anchors.top: searchField.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: childrenRect.height
            x: 0
            contacts: contactModel
            onOpenContact: {
                    //screen.showContact = true;
                    pageStack.push(contactEditComponent, {contact: contact});
                     }
        }

        states: [
                State {
                    name: "List";
                    when: pageStack.currentPage == pageList
                    //PropertyChanges { target: searchField; focus: true; }
                },
                State {
                    name: "SingleContact";
                    when: pageStack.currentPage == contactEditComponent
                    PropertyChanges { target: searchField; focus: false}
                }
            ]



        ListModel {
            id: managersModel;
        }

        ContextMenu {
            id: contextManagerSelectMenu
            objectName: "contextManagerSelectMenu"
            titleText: qsTr("Select manager")
            MenuLayout {
                objectName: "contextMenuLayout"
                Repeater {
                    objectName: "selectManagerRepeater"
                    model: managersModel
                    MenuItem {
                        objectName: name + "_menuItem"
                        text: {
                            return name
                        }
                        onClicked: {
                            contactModel.manager = name
                            if (name == "memory")
                                contactModel.importContacts(Qt.resolvedUrl("contents/example.vcf"));
                        }
                    }
                }
            }
        }
    }
}

