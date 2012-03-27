/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Mobility Components.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

Rectangle {

property bool start: false
property SignalSpy contactsChangedSpy
property ContactModel contactModel
property Contact testContact

    TestCase {
        name: "ContactsTests"

        function waitForContactsChanged (expectedCount) {
            contactsChangedSpy.wait();
            compare (contactsChangedSpy.count, expectedCount)
        }

        function test_addAndRemoveDetails()
        {
            //TODO verify this test case when we remove dynamic properties from the Api
            //currently removing details from the contact is not updated in contact model
            //TODO add a test case to remove the same detail N times and remove a detail which doesnt exist etc...
            var model = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "ContactModel {id:model;autoUpdate:true;onContactsChanged:{console.log(\"CONTACTS CHANGED!\")}}", testHelper);
            var tmp = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "Contact {Name{}}", testHelper);
            var spy2 = Qt.createQmlObject("import QtTest 1.0;" +"SignalSpy {id: theSpy;signalName: \"contactsChanged\";}", testHelper);
            contactsChangedSpy = spy2;
            contactsChangedSpy.target = model;
            contactsChangedSpy.clear()
            testHelper.model = model;
            waitForContactsChanged (contactsChangedSpy.count + 1)
            testHelper.emptyContactsDb();
            testContact = tmp;
            var phone = Qt.createQmlObject("import QtContacts 5.0;" +
                                              "PhoneNumber {number: '99999999'}", testContact);
            var nick = Qt.createQmlObject("import QtContacts 5.0;" +
                                             "Nickname {nickname: 'jack'}", testContact);
            var mail = Qt.createQmlObject("import QtContacts 5.0;" +
                                             "EmailAddress {emailAddress: 'joe.john@ovi.com'}", testContact);
            testContact.name.firstName = "Joe"
            testContact.name.lastName = "John"
            testContact.addDetail(phone)
            testContact.addDetail(nick)
            testContact.addDetail(mail)
            model.saveContact(testContact)
            waitForContactsChanged (contactsChangedSpy.count + 1)
            testContact = model.contacts[0]
            compare(testContact.phoneNumber.number,"99999999")
            compare(testContact.nickname.nickname,"jack")
            compare(testContact.email.emailAddress,"joe.john@ovi.com")
            testContact.removeDetail(testContact.detail(ContactDetail.NickName));
            testContact.removeDetail(testContact.detail(ContactDetail.PhoneNumber));
            testContact.removeDetail(testContact.detail(ContactDetail.Email));
            model.saveContact(testContact)
            waitForContactsChanged (contactsChangedSpy.count + 1)
            testContact = model.contacts[0]
            verify(!testContact.detail(ContactDetail.NickName))
            verify(!testContact.detail(ContactDetail.PhoneNumber))
            verify(!testContact.detail(ContactDetail.Email))
            compare(model.contacts.length, 1)
            testHelper.emptyContactsDb();
            model.autoUpdate = false;
        }

        property Component component
        property ContactsTestHelper testHelper

        function init() {
            component = Qt.createComponent("ContactsTestHelper.qml");
            testHelper = component.createObject(top);
            if (testHelper == undefined)
                console.log("Unable to load component from " + name +  " error is ", component.errorString())
            verify(testHelper != undefined, 'Unable to load component ' + name);
        }

        function cleanup() {
            testHelper.destroy();
            component.destroy();
          }


        Contact {
            id: contact
        }

        function test_contact() {
            // empty Contact
            compare(contact.modified, false)

            // access Type
            console.log("type is: " + contact.type)
            compare(contact.type, Type.Contact)  // Type is Contact and not Group

            // access contactId
            console.log("contactId is: " + JSON.stringify(contact.contactId) )
            compare(contact.contactId, "qtcontacts:::")

            // access manager
            console.log("manager is: " + JSON.stringify(contact.manager))
            compare(contact.manager, "")
        }

        Contact {
            id: contact2
        }

        PhoneNumber {
            id: phoneNumber
            number: "99999999"
        }

        function test_contactAddDetail() {
            // add new Detail
            contact2.addDetail(phoneNumber)
            compare(contact2.phoneNumber.number, "99999999")
        }

        Contact {
            id: contact3
            PhoneNumber {
                id: phoneNumber3
                number: "99999999"
            }
        }

        function test_contactUpdateDetail() {
            // update existing Detail
            contact3.phoneNumber.number = "88888"
            compare(contact3.phoneNumber.number, "88888")
        }

        Contact {
            id: contact4
            PhoneNumber {
                id: phoneNumber4
                number: "99999999"
            }
        }
        function test_contactRemoveDetail() {
            // delete one existing Detail
            contact4.removeDetail(phoneNumber4);
            verify(!contact4.phoneNumber.number)
        }
    }
}
