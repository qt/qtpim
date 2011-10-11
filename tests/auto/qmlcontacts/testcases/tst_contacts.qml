/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

        // test data is handled by ContactsTestHelper.qml function addContacts(contacts_data)
        function test_detailFilters_data() {
            return [
                {tag: "Contacts set 1",
                    contacts:[
                       {
                            tag: "Contact 1",
                            type: 'Contact',
                            firstName: "First1",
                            lastName: "Last1",
                            phoneNumber: "1111111"
                        },
                        {   tag: "Contact 2",
                            type: 'Contact',
                            firstName: "First2",
                            lastName: "Last2",
                            phoneNumber: "2222222"
                       },
                    ]
                }
            ]
        }

        function test_contactDetails_data() {
            return [
                {tag: "Contacts organization details",
                    organizations:[
                       {
                            tag: "Contacts organization details",
                            type: 'Contact',
                            firstName:"John",
                            lastName:"Joe",
                            name: "TechSoft",
                            logoUrl:"http://www.example.com",
                            assistantName:"Maria",
                            startDate:"1995-10-01T00:00:00",
                            phoneNumber: "9999",
                            phoneSubType: PhoneNumber.Landline,
                            phoneSubTypeString: "Landline",
                            contexts:"Home"
                        },
                    ]
                }
            ]
        }

        function waitForContactsChanged (expectedCount) {
            contactsChangedSpy.wait();
            compare (contactsChangedSpy.count, expectedCount)
        }

//data driven test as part of QtTest frame work
        function test_detailFilters(data)
        {
            var model = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "ContactModel {id:model;manager:\"jsondb\";autoUpdate:true;}", testHelper);
            var spy = Qt.createQmlObject("import QtTest 1.0;" +
                    "SignalSpy {id: theSpy;signalName: \"contactsChanged\";}", testHelper);
            contactsChangedSpy = spy;
            contactsChangedSpy.target = model;

            testHelper.model = model;
            //Let's wait for the model to be up-to-date
            console.log(">>>>>>>>>>>> ready? " + model.autoUpdate)
            waitForContactsChanged (1)
            testHelper.emptyContactsDb();
            //DB should be now empty
            compare (model.contacts.length, 0)
            testHelper.addContacts(data.contacts)
            compare (model.contacts.length, data.contacts.length)

            //Filter by name
            model.filter = firstNameFilter
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            model.filter = noMatchingFilter
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)

            //Filter by phone number
            detailFilter.detail = ContactDetail.PhoneNumber
            detailFilter.field = PhoneNumber.Number
            detailFilter.value = data.contacts[0].phoneNumber
            detailFilter.matchFlags = Filter.MatchExactly
            model.filter = detailFilter
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            detailFilter.matchFlags = Filter.MatchContains
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            detailFilter.matchFlags = Filter.MatchStartsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            detailFilter.matchFlags = Filter.MatchEndsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            //FixedString and CaseSensitive flags not yet handled
            /*detailFilter.matchFlags = Filter.MatchFixedString
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            detailFilter.matchFlags = Filter.MatchCaseSensitive
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)*/

            detailFilter.value = data.contacts[1].phoneNumber
            detailFilter.matchFlags = Filter.MatchExactly
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            detailFilter.value = "2222"
            detailFilter.matchFlags = Filter.MatchExactly
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)

            detailFilter.matchFlags = Filter.MatchContains
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            detailFilter.matchFlags = Filter.MatchStartsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)

            //test with sequence of match flag changes and very the results
            detailFilter.matchFlags = Filter.MatchExactly
            detailFilter.value = "2222"
            detailFilter.matchFlags = Filter.MatchExactly
            detailFilter.matchFlags = Filter.MatchContains
            detailFilter.matchFlags = Filter.MatchStartsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 1)
            //Following case does not work since the filtering logic
            //does not parse yet special characters properly.
            //The "*" is added to the regexp, and the result is that
            //the filter will return all available contacts from JsonDb
            //
            /*
            detailFilter.value = "*"
            detailFilter.matchFlags = Filter.MatchStartsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            console.log("LENGTH: " + model.contacts.length)
            verify (model.contacts.length == 0)
            */

            detailFilter.value = ""
            detailFilter.matchFlags = Filter.MatchExactly
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)
            detailFilter.matchFlags = Filter.MatchContains
            waitForContactsChanged (contactsChangedSpy.count + 1)
            //MatchContains "" will return all available contacts
            verify (model.contacts.length == data.contacts.length)
            detailFilter.matchFlags = Filter.MatchStartsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            //MatchStartsWith "" will return all available contacts
            verify (model.contacts.length == data.contacts.length)
            detailFilter.matchFlags = Filter.MatchEndsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            //MatchEndsWith "" will return all available contacts
            verify (model.contacts.length == data.contacts.length)

            detailFilter.value = " "
            detailFilter.matchFlags = Filter.MatchExactly
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)

            detailFilter.matchFlags = Filter.MatchContains
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)

            detailFilter.matchFlags = Filter.MatchStartsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)

            detailFilter.matchFlags = Filter.MatchEndsWith
            waitForContactsChanged (contactsChangedSpy.count + 1)
            verify (model.contacts.length == 0)

            //unsetting the filter to clean up all contacts in the model
            model.filter = null;
            waitForContactsChanged (contactsChangedSpy.count + 1)
            testHelper.emptyContactsDb();
        }
        DetailFilter {
           id:firstNameFilter
           detail:ContactDetail.Name
           field:Name.FirstName
           value:"First1"
           matchFlags:Filter.MatchExactly
        }
        DetailFilter {
            id:noMatchingFilter
            detail:ContactDetail.Name
            field:Name.FirstName
            value:"DOESNOTEXIST"
            matchFlags:Filter.MatchExactly
        }
        DetailFilter {
            id:detailFilter
        }

        //data driven test as part of QtTest frame work
        function test_contactDetails(data)
        {
            var model2 = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "ContactModel {id:model2;manager:\"jsondb\";autoUpdate:true;}", testHelper);
            var spy2 = Qt.createQmlObject("import QtTest 1.0;" +
                    "SignalSpy {id: theSpy;signalName: \"contactsChanged\";}", testHelper);
            contactsChangedSpy = spy2;
            contactsChangedSpy.target = model2;
            contactsChangedSpy.clear()
            testHelper.model = model2;
            waitForContactsChanged (1)
            testHelper.emptyContactsDb();
            compare (model2.contacts.length, 0)
            testHelper.addContactDetail(data.organizations)
            compare (model2.contacts[0].organization.name, data.organizations[0].name)
            compare (model2.contacts[0].organization.assistantName, data.organizations[0].assistantName)
            compare (model2.contacts[0].organization.logoUrl, data.organizations[0].logoUrl)
            compare (model2.contacts[0].phoneNumber.number, data.organizations[0].phoneNumber)
            compare (Qt.formatDateTime(model2.contacts[0].organization.startDate,Qt.ISODate), data.organizations[0].startDate)
            compare (model2.contacts[0].phoneNumber.subTypes[0], data.organizations[0].phoneSubTypeString)
            compare (model2.contacts[0].phoneNumber.contexts[0],data.organizations[0].contexts)
            testHelper.emptyContactsDb();
        }

        function test_addAndRemoveDetails()
        {
            //TODO verify this test case when we remove dynamic properties from the Api
            //currently removing details from the contact is not updated in contact model
            //TODO add a test case to remove the same detail N times and remove a detail which doesnt exist etc...
            var model = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "ContactModel {id:model;manager:\"jsondb\";autoUpdate:true;onContactsChanged:{console.log(\"CONTACTS CHANGED!\")}}", testHelper);
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
            compare(testContact.nickname,null)
            compare(testContact.phoneNumber,null)
            compare(testContact.email,null)
            compare(model.contacts.length, 1)
            testHelper.emptyContactsDb();
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
            compare(contact.type, 0)  // Type is Contact and not Group

            // access contactId
            console.log("contactId is: " + JSON.stringify(contact.contactId) )
            compare(contact.contactId, "")

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
            compare(contact4.phoneNumber,null)
        }
    }
}
