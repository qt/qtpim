/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
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
                    "ContactModel {" +
                        "id: model;" +
                        "manager: \"memory\";" +
                        "autoUpdate:true;" +
                    "}",
                    testHelper);
            var tmp = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "Contact { Name {} }",
                    testHelper);
            var spy2 = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                        "id: theSpy;" +
                        "signalName: \"contactsChanged\";" +
                    "}",
                    testHelper);
            contactsChangedSpy = spy2;
            contactsChangedSpy.target = model;
            contactsChangedSpy.clear()
            testHelper.model = model;
            testHelper.emptyContactsDb();
            testContact = tmp;
            var phone = Qt.createQmlObject("import QtContacts 5.0;" +
                                              "PhoneNumber {number: '99999999'}", testContact);
            var nick = Qt.createQmlObject("import QtContacts 5.0;" +
                                             "Nickname {nickname: 'jack'}", testContact);
            var label = Qt.createQmlObject("import QtContacts 5.0;" +
                                             "DisplayLabel {label: 'Alice In Wonderland'}", testContact);
            var mail = Qt.createQmlObject("import QtContacts 5.0;" +
                                             "EmailAddress {emailAddress: 'joe.john@ovi.com'}", testContact);
            testContact.name.firstName = "Joe"
            testContact.name.lastName = "John"
            testContact.addDetail(phone)
            testContact.addDetail(nick)
            testContact.addDetail(label)
            testContact.addDetail(mail)
            contactsChangedSpy.clear()
            model.saveContact(testContact)
            waitForContactsChanged (1)
            testContact = model.contacts[0]
            compare(testContact.phoneNumber.number,"99999999")
            compare(testContact.nickname.nickname,"jack")
            compare(testContact.displayLabel.label,"Alice In Wonderland")
            compare(testContact.email.emailAddress,"joe.john@ovi.com")
            testContact.removeDetail(testContact.detail(ContactDetail.NickName));
            testContact.removeDetail(testContact.detail(ContactDetail.DisplayLabel));
            testContact.removeDetail(testContact.detail(ContactDetail.PhoneNumber));
            testContact.removeDetail(testContact.detail(ContactDetail.Email));
            contactsChangedSpy.clear()
            model.saveContact(testContact)
            waitForContactsChanged (1)
            testContact = model.contacts[0]
            verify(!testContact.detail(ContactDetail.NickName))
            verify(!testContact.detail(ContactDetail.DisplayLabel))
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
            compare(contact.type, Type.Contact)  // Type is Contact and not Group

            // access contactId
            compare(contact.contactId, "")

            // access manager
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
