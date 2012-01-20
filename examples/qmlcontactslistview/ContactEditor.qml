/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
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
import QtQuick 2.0

Rectangle {
    width: parent.width
    color: "pink"
    property variant contact // contact that is shown in editView

    // ![Widgets for manipulating contact details]
    Column {
        spacing: 8
        anchors {
            fill: parent
            leftMargin: 10
            topMargin: 10
        }
        DetailEditWidget { id: nameField; label: "Name "; value: contact ? contact.name.firstName : " "}
        DetailEditWidget { id: emailField; label: "Email Address "; value: contact ? contact.email.emailAddress : " "}
        DetailEditWidget { id: phoneField; label: "Phone Number "; value: contact ? contact.phoneNumber.number : " "}
    }
    // ![Widgets for manipulating contact details]

    function deleteContact() {
        contactsModel.removeContact(contactEditor.contact.contactId)
        statusBar.updateMsg("contact successfully deleted")
     }

    function updateContact() {
        // read in values from the input fields
        var values = [nameField.value, emailField.value, phoneField.value]
        if (!contact) { // create new contact
            var newContact = Qt.createQmlObject("import QtContacts 5.0; Contact{ }", contactEditor)
            setDetailValues(newContact, values)
            newContact.save()
            contactsModel.saveContact(newContact)
            statusBar.updateMsg("new contact successfully created")
        } else { // update existing contact
            if (contact.modified) {
                setDetailValues(contact, values)
                contact.save()
                statusBar.updateMsg("contact successfully updated")
            } else {
                statusBar.updateMsg("nothing to update, contact already is up-to-date")
            }
        }
    }

    function setDetailValues(c, values) {
        c.name.firstName = values[0]
        c.email.emailAddress = values[1]
        c.phoneNumber.number = values[2]
    }

    function cancel() {
        contact = ""
    }

    function resetToDefaults() {
        nameField.inputFocus = false
        emailField.inputFocus = false
        phoneField.inputFocus = false
        nameField.color = "black"
        emailField.color = "black"
        phoneField.color = "black"
    }
}

