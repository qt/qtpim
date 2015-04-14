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
        DetailEditWidget {
            id: nameField
            label: "Name "
            value: contact ? contact.name.firstName : " "
        }
        DetailEditWidget {
            id: emailField
            label: "Email Address "
            value: contact ? contact.email.emailAddress : " "
            showPreferredField: true
            isPreferred: contact ? contact.isPreferredDetail("MESSAGE", contact.email) : false
        }
        DetailEditWidget {
            id: phoneField
            label: "Phone Number "
            value: contact ? contact.phoneNumber.number : " "
            showPreferredField: true
            isPreferred: contact ? contact.isPreferredDetail("CALL", contact.phoneNumber) : false
        }
    }
    // ![Widgets for manipulating contact details]

    function deleteContact() {
        contactsModel.removeContact(contactEditor.contact.contactId)
        statusBar.updateMsg("contact successfully deleted")
     }

    function updateContact() {
        // read in values from the input fields
        var values = [nameField.value,
                      emailField.value, emailField.requestPreferred || emailField.isPreferred,
                      phoneField.value, phoneField.requestPreferred || phoneField.isPreferred]
        if (!contact) { // create new contact
            var newContact = Qt.createQmlObject("import QtContacts 5.0; Contact{ }", contactEditor)
            setDetailValues(newContact, values)
            newContact.save()
            contactsModel.saveContact(newContact)
            statusBar.updateMsg("new contact successfully created")
        } else { // update existing contact
            setDetailValues(contact, values)
            if (contact.modified) {
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
        c.phoneNumber.number = values[3]
        if (values[2]) {
            c.setPreferredDetail("MESSAGE", c.email)
        }

        if (values[4]) {
            c.setPreferredDetail("CALL", c.phoneNumber)
        }
    }

    function cancel() {
        contact = ""
    }

    function resetToDefaults() {
        nameField.inputFocus = false
        emailField.inputFocus = false
        phoneField.inputFocus = false
        emailField.requestPreferred = false
        phoneField.requestPreferred = false
        nameField.color = "black"
        emailField.color = "black"
        phoneField.color = "black"
    }
}

