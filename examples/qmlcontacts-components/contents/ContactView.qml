/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.0
import QtContacts 5.0
import com.nokia.components 1.0
import com.nokia.components.style 1.0

Page {
    id: contactEditPage; objectName: "ContactEditPage"

    title: addMode ? qsTr("add")  : qsTr("edit")

    signal backClicked

    property bool addMode: false

    property Contact contact
    property ContactModel mainModel

    signal dismissed
    signal deleted(int id)

    tools: toolbarLayout

    Rectangle {
        id: contactEditPageBackground
        color: "black"
        anchors.fill:parent
    }

    ListView {
        id: contactEditListView; objectName: "contactEditListView"
        anchors.fill: parent
        model: contactCardModel
//        model: contact.details
//        section.property: "definitionName"
//        section.criteria: ViewSection.FullString
//        section.delegate: detailHeading
//        delegate:
//            FieldEdit {
//                id: cardDelegate
//                field: fieldNames[0]
//                detailDefinitionName: definitionName
//                detail: modelData
//                text: value(field)
//                width: contactEditListView.width
//                onValueChanged: {
//                    setValue(field, text)
//                }
//            }
    }
//   Component {
//        id: detailHeading
//        Rectangle {
//            width: contactEditListView.width
//            height: childrenRect.height
//            color: "#d3c9c9"

//            Text {
//                text: section
//                font.bold: true
//                color: "black"
//            }
//        }
//    }

    VisualItemModel {
        id: contactCardModel
        FieldEdit {
            id: firstNameEdit
            field: "firstName"
            detailDefinitionName: "Name"
            text: contact.name.firstName
            width: contactEditListView.width
            onValueChanged: {
                contact.name.firstName = newValue;
            }
        }
        FieldEdit {
            id: lastNameEdit
            field: "lastName"
            detailDefinitionName: "Name"
            text: contact.name.lastName
            width: contactEditListView.width
            onValueChanged: {
                contact.name.lastName = newValue;
            }
        }
        FieldEdit {
            id: emailEdit
            field: "emailAddress"
            detailDefinitionName: "EMail"
            text: contact.email.emailAddress
            width: contactEditListView.width
            onValueChanged: {
                contact.email.emailAddress = newValue;
            }
        }
        FieldEdit {
            id: phoneEdit
            field: "number"
            detailDefinitionName: "Phone Number"
            text: contact.phoneNumber.number
            width: contactEditListView.width
            onValueChanged: {
                contact.phoneNumber.number = newValue;
            }
        }
        FieldEdit {
            id: streetEdit
            field: "street"
            detailDefinitionName: "Address"
            text: contact.address.street
            width: contactEditListView.width
            onValueChanged: {
                contact.address.street = newValue;
            }
        }
        FieldEdit {
            id: postCodeEdit
            field: "postcode"
            detailDefinitionName: "Address"
            text: contact.address.postcode
            width: contactEditListView.width
            onValueChanged: {
                contact.address.postcode = newValue;
            }
        }
        FieldEdit {
            id: cityEdit
            field: "locality"
            detailDefinitionName: "Address"
            text: contact.address.locality
            width: contactEditListView.width
            onValueChanged: {
                contact.address.locality = newValue;
            }
        }
        FieldEdit {
            id: countryEdit
            field: "country"
            detailDefinitionName: "Address"
            text: contact.address.country
            width: contactEditListView.width
            onValueChanged: {
                contact.address.country = newValue;
            }
        }
        FieldEdit {
            id: urlEdit
            field: "url"
            detailDefinitionName: "Url"
            text: contact.url.url
            width: contactEditListView.width
            onValueChanged: {
                contact.url.url = newValue;
            }
        }
        FieldEdit {
            id: genderEdit
            field: "gender"
            detailDefinitionName: "Gender"
            text: contact.gender.gender
            width: contactEditListView.width
            onValueChanged: {
                contact.gender.gender = newValue;
            }
        }
        FieldEdit {
            id: nicknameEdit
            field: "nickname"
            detailDefinitionName: "Nickname"
            text: contact.nickname.nickname
            width: contactEditListView.width
            onValueChanged: {
                contact.nickname.nickname = newValue;
            }
        }
        FieldEdit {
            id: birthdayEdit
            field: "birthday"
            detailDefinitionName: "Birthday"
            text: contact.birthday.birthday
            width: contactEditListView.width
            onValueChanged: {
                contact.birthday.birthday = newValue;
                console.log("<<<<<<<<<<<<< BIRTHDAY: " + contact.birthday.birthday)
            }
        }
        FieldEdit {
            id: noteEdit
            field: "note"
            detailDefinitionName: "Note"
            text: contact.note.note
            width: contactEditListView.width
            onValueChanged: {
                contact.note.note = newValue;
            }
        }
        FieldEdit {
            id: avatarEdit
            field: "imageUrl"
            detailDefinitionName: "Avatar"
            text: contact.avatar.imageUrl
            width: contactEditListView.width
            onValueChanged: {
                contact.avatar.imageUrl = newValue;
            }
        }
        FieldEdit {
            id: ringtoneEdit
            field: "audioRingtoneUrl"
            detailDefinitionName: "Ringtone"
            text: contact.ringtone.audioRingtoneUrl
            width: contactEditListView.width
            onValueChanged: {
                contact.ringtone.audioRingtoneUrl = newValue;
            }
        }
        FieldEdit {
            id: personIdEdit
            field: "personid"
            detailDefinitionName: "Personid"
            text: contact.personid.personid
            width: contactEditListView.width
            onValueChanged: {
                contact.personid.personid = newValue;
            }
        }
        FieldEdit {
            id: assistantnameEdit
            field: "assistantName"
            detailDefinitionName: "Organization"
            text: contact.organization.assistantName
            width: contactEditListView.width
            onValueChanged: {
                contact.organization.assistantName = newValue;
            }
        }
        FieldEdit {
            id: startdateEdit
            field: "startDate"
            detailDefinitionName: "Organization"
            text: contact.organization.startDate
            width: contactEditListView.width
            onValueChanged: {
                contact.organization.startDate = newValue;
            }
        }
    }

    ToolBarLayout {
        id: toolbarLayout; objectName: "toolbarLayout"
        ToolIcon {
            objectName: "toolIcon1"
            iconId: "ctoolbar.gback"
            onClicked: contactEditPage.backClicked()
        }
        ToolButtonRow {
            id:button
            objectName: "toolButtonRow"
            ToolButton {
                objectName: "saveButton"
                text:"Save"
                onClicked: {
                    if (addMode) {
                        mainModel.saveContact(contact);
                    } else {
                        contact.save();
                    }
                    contactEditPage.backClicked()
                }
            }
        }
        ToolIcon {
            objectName: "toolIcon2"
            iconId: "ctoolbar.gcategories"
            onClicked: contextMenu.open()
        }
    }

    ContextMenu {
        id: contextMenu
        objectName: "contextMenu"
        titleText: qsTr("Options")
        MenuLayout {
            objectName: "contextMenuLayout"
            MenuItem {
                objectName: "deleteContactMenuItem"
                text: qsTr("Delete contact")
                visible: !addMode
                onClicked:  {
                    if (contact.contactId != "")
                        contactModel.removeContact(contact.contactId)
                    contactEditPage.backClicked()
                }
            }
            MenuItem {
                objectName: "cancelContactMenuItem"
                text: qsTr("Cancel editing")
                onClicked:  {
                    contactEditPage.backClicked()
                }
            }
        }
    }
}
