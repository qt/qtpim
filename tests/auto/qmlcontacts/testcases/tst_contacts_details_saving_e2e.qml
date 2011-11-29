/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtPim module of the Qt Toolkit.
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

TestCase {
    name: "ContactsDetailsSavingE2ETests"
    id: contactsDetailsSavingE2ETests

    property Contact contact

    ContactModel {
        id: model
        autoUpdate: true
    }

    // Tests

    Address {
        id: address
    }

    function test_address() {
        address.street = "Street"
        address.locality = "Locality"
        address.region = "Region"
        address.postcode = "Postcode"
        address.country = "Country"
        address.contexts = ["Home"]
        contact.addDetail(address)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Address)
        compare(detail.street, "Street")
        compare(detail.locality, "Locality")
        compare(detail.region, "Region")
        compare(detail.postcode, "Postcode")
        compare(detail.country, "Country")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
    }

    Avatar {
        id: avatar
    }

    function test_avatar() {
        avatar.imageUrl = "http://qt.nokia.com"
        contact.addDetail(avatar)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Avatar)
        compare(detail.imageUrl, "http://qt.nokia.com")
    }

    Birthday {
        id: birthday
    }

    function test_birthday() {
        birthday.birthday = new Date(2012, 12, 01)
        contact.addDetail(birthday)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Birthday)
        compare(new Date(detail.birthday), new Date(2012, 12, 01))
    }

    EmailAddress {
        id: emailaddress
    }

    function test_emailAddress() {
        emailaddress.emailAddress = "test@qt.nokia.com"
        emailaddress.contexts = ["Home"]
        contact.addDetail(emailaddress)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Email)
        compare(detail.emailAddress, "test@qt.nokia.com")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
    }

    Gender {
        id: gender
    }

    function test_gender() {
        gender.gender = Gender.Male
        contact.addDetail(gender)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Gender)
        compare(detail.gender, Gender.Male)
    }

    Name {
        id: name
    }

    function test_name() {
        name.prefix = "Dr."
        name.firstName = "John"
        name.middleName = "B."
        name.lastName = "Doe"
        name.suffix = "Sr."
        name.contexts = ["Home"]
        contact.addDetail(name)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Name)
        compare(detail.prefix, "Dr.")
        compare(detail.firstName, "John")
        compare(detail.middleName, "B.")
        compare(detail.lastName, "Doe")
        compare(detail.suffix, "Sr.")
        expectFail("", "contexts are not supported at the moment")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
    }

    Nickname {
        id: nickname
    }

    function test_nickname() {
        nickname.nickname = "Dummy"
        contact.addDetail(nickname)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.NickName)
        compare(detail.nickname, "Dummy")
    }

    Note {
        id: note
    }

    function test_note() {
        note.note = "Dummy"
        contact.addDetail(note)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Note)
        compare(detail.note, "Dummy")
    }

    OnlineAccount {
        id: onlineaccount
    }

    function test_onlineAccount() {
        onlineaccount.accountUri = "Account URI";
        contact.addDetail(onlineaccount);
        expectFail("", "Saving of Account URI is not supported at the moment");
        verify(false);
        saveAndRefreshContact();
        var detail = contact.detail(ContactDetail.OnlineAccount);
        expectFail("", "This is not supported at the moment");
        verify(detail);
        compare(detail.accountUri, "Account URI");
    }

    Organization {
        id: organization
    }

    function test_organization() {
        organization.name = "Name"
        organization.logoUrl = "http://qt.nokia.com"
        organization.department = "Department"
        organization.location = "Location"
        organization.role = "Role"
        organization.title = "Title"
        organization.assistantName = "Assistant name"
        organization.startDate = new Date(2012, 12, 01)
        organization.endDate = new Date(2012, 12, 02)
        organization.contexts = ["Home"]
        contact.addDetail(organization)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Organization)
        compare(detail.name, "Name")
        compare(detail.logoUrl, "http://qt.nokia.com")
        expectFail("", "TODO: department is not working as expected at the moment");
        compare(detail.department[0], "Department")
        expectFail("", "TODO: location is not working as expected at the moment");
        compare(detail.location, "Location")
        expectFail("", "TODO: role is not working as expected at the moment");
        compare(detail.role, "Role")
        compare(detail.title, "Title")
        compare(detail.assistantName, "Assistant name")
        expectFail("", "TODO:  startDate is not working as expected at the moment");
        compare(new Date(detail.startDate), new Date(2012, 12, 01))
        expectFail("", "TODO: endDate is not working as expected at the moment");
        compare(detail.endDate, new Date(2012, 12, 03))
        compare(new Date(detail.endDate), new Date(2012, 12, 02))
        expectFail("", "TODO: contexts are not working as expected at the moment");
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
    }

    Personid {
        id: personid
    }

    function test_personId() {
        personid.personid = "Person id"
        contact.addDetail(personid)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.PersonId)
        compare(detail.personid, "Person id")
    }

    PhoneNumber {
        id: phonenumber
    }

    function test_phoneNumber() {
        phonenumber.subTypes = [
                    PhoneNumber.Landline,
                    PhoneNumber.Mobile,
                    PhoneNumber.Fax,
                    PhoneNumber.Video,
                ]
        phonenumber.number = "1"
        phonenumber.contexts = ["Home"]
        contact.addDetail(phonenumber)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.PhoneNumber)
        compare(detail.number, "1")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
        expectFail("", "TODO: This is not working as expected at the moment")
        compare(detail.subTypes.length, phonenumber.subTypes.length)
    }

    function test_phoneNumberOneSubtype() {
        phonenumber.subTypes = [
                    PhoneNumber.Mobile
                ]
        phonenumber.number = "1"
        contact.addDetail(phonenumber)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.PhoneNumber)
        compare(detail.number, "1")
        compare(detail.subTypes.length, phonenumber.subTypes.length)
    }

    Ringtone {
        id: ringtone
    }

    function test_ringtone() {
        ringtone.audioRingtoneUrl = "http://qt.nokia.com"
        contact.addDetail(ringtone)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Ringtone)
        compare(detail.audioRingtoneUrl, "http://qt.nokia.com")
    }

    // Thumbnail is not supported at the moment
    Thumbnail {
        id: thumbnail
    }

    function test_thumbnail() {
        expectFail("", "This is not supported at the moment");
        verify(false);
    }

    Url {
        id: url
    }

    function test_url() {
        url.url = "http://qt.nokia.com"
        url.contexts = ["Home"]
        contact.addDetail(url)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Url)
        compare(detail.url, "http://qt.nokia.com")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitForContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
    }

    function init() {
        initTestForModel(model);
        contact = Qt.createQmlObject(
                  "import QtContacts 5.0;" +
                  "Contact {}", contactsDetailsSavingE2ETests)
    }

    function cleanup() {
        emptyContacts(model);
    }

    function saveAndRefreshContact() {
        model.saveContact(contact)
        waitForContactsChanged();
        compare(model.contacts.length, 1, "Unexpected amount of contacts in model after updating contact.")
        contact = model.contacts[0]
    }

    property SignalSpy spy

    function initTestForModel(model) {
        spy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsDetailsSavingE2ETests);
        spy.target = model;
        spy.signalName = "contactsChanged"
    }

    function waitForContactsChanged() {
        spy.wait();
    }

    function emptyContacts(model) {
        var count = model.contacts.length
        for (var i = 0; i < count; i++) {
            var id = model.contacts[0].contactId
            model.removeContact(id)
            spy.wait()
        }
        compare(model.contacts.length, 0, "model is empty")
    }
}
