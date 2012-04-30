/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

ContactsSavingTestCase {
    name: "ContactsDetailsSavingE2ETests"
    id: contactsDetailsSavingE2ETests

    property Contact contact

    ContactModel {
        id: model
        manager: getManagerUnderTest()
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
        address.contexts = [ContactDetail.ContextHome]
        contact.addDetail(address)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Address)
        compare(detail.street, "Street")
        compare(detail.locality, "Locality")
        compare(detail.region, "Region")
        compare(detail.postcode, "Postcode")
        compare(detail.country, "Country")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], ContactDetail.ContextHome, "contexts")
    }

    Address {
        id: addressSanitizable
    }

    function test_addressCanBeSanitized() {
        addressSanitizable.street = " Street"
        addressSanitizable.locality = "  Locality"
        addressSanitizable.region = "   Region "
        addressSanitizable.postcode = " Postcode   "
        addressSanitizable.country = "     Country    "
        contact.addDetail(addressSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Address)
        if (model.manager == "jsondb") {
            compare(detail.street, "Street")
            compare(detail.locality, "Locality")
            compare(detail.region, "Region")
            compare(detail.postcode, "Postcode")
            compare(detail.country, "Country")
        } else {
            // we do not expect other backends to sanitize the input by default
            compare(detail.street, " Street")
            compare(detail.locality, "  Locality")
            compare(detail.region, "   Region ")
            compare(detail.postcode, " Postcode   ")
            compare(detail.country, "     Country    ")
        }
    }

    Address {
        id: addressNotSanitizable
    }

    function test_addressCanNotBeSanitized() {
        // If any of the address fields is longer than 50 chars it is invalid, thus the contact won't be saved
        addressNotSanitizable.street = " Street"
        addressNotSanitizable.locality = ""
        addressNotSanitizable.region = "   Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee "
        addressNotSanitizable.postcode = " Postcode   "
        addressNotSanitizable.country = "     Country    "
        contact.addDetail(addressNotSanitizable)
        if (model.manager == "jsondb") {
            // jsondb backend will remove blank spaces and other invalid characters
            // resulting this case into an invalid contact object. The model
            // should return here a BadArgument error.
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            var detail = contact.detail(ContactDetail.Address)
            compare(detail.street, " Street")
            compare(detail.locality, "")
            compare(detail.region, "   Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee ")
            compare(detail.postcode, " Postcode   ")
            compare(detail.country, "     Country    ")
        }
    }

    Address {
        id: addresses1
        street: "Street 1"
    }

    Address {
        id: addresses2
        street: "Street 2"
    }

    function test_multipleAddresses() {
        contact.addDetail(addresses1)
        contact.addDetail(addresses2)
        saveAndRefreshContact()
        var details = contact.details(ContactDetail.Address)
        compare(details.length, 2, "details length");
        if (details[0].street === addresses2.street)
            details.reverse(); // assuming detail order was not preserved
        compare(details[0].street, addresses1.street, "street 0")
        compare(details[1].street, addresses2.street, "street 1")
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
        emailaddress.contexts = [ContactDetail.ContextHome]
        contact.addDetail(emailaddress)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Email)
        compare(detail.emailAddress, "test@qt.nokia.com")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], ContactDetail.ContextHome, "contexts")
    }

    EmailAddress {
        id: emailaddressSanitizable
    }

    function test_test_emailAddressCanBeSanitized() {
        emailaddressSanitizable.emailAddress = "    test@qt.nokia.com "
        emailaddressSanitizable.contexts = [ContactDetail.ContextHome]
        contact.addDetail(emailaddressSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Email)
        if (model.manager == "jsondb") {
            compare(detail.emailAddress, "test@qt.nokia.com")
        } else { // we do not expect other backends to sanitize the input by default
            compare(detail.emailAddress, "    test@qt.nokia.com ")
        }
    }

    EmailAddress {
        id: emailaddressNotSanitizable
    }

    function test_emailAddressCanNotBeSanitized() {
        // If email address is longer than 126 chars it is invalid and won't be saved
        var veryLongEmailAddress
        for (var i = 0; i < 127; i++) {
            veryLongEmailAddress = "a" + veryLongEmailAddress
        }
        emailaddressNotSanitizable.emailAddress = veryLongEmailAddress
        emailaddressNotSanitizable.contexts = [ContactDetail.ContextHome]
        contact.addDetail(emailaddressNotSanitizable)
        if (model.manager == "jsondb") {
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            var detail = contact.detail(ContactDetail.Email)
            compare(detail.emailAddress, veryLongEmailAddress)
        }
    }

    EmailAddress {
        id: emailAddresses1
        emailAddress: "a@a"
    }

    EmailAddress {
        id: emailAddresses2
        emailAddress: "b@b"
    }

    function test_multipleEmailAddresses() {
        contact.addDetail(emailAddresses1)
        contact.addDetail(emailAddresses2)
        saveAndRefreshContact()
        var details = contact.details(ContactDetail.Email)

        compare(details.length, 2, "details length");
        if (details[0].emailAddress === emailAddresses2.emailAddress)
            details.reverse(); // assuming detail order was not preserved
        compare(details[0].emailAddress, emailAddresses1.emailAddress, "email address 0");
        compare(details[1].emailAddress, emailAddresses2.emailAddress, "email address 1");
    }

    Favorite {
        id: favorite
    }

    function test_favorite() {
        if (model.manager === "jsondb")
            skip("jsondb does not save favorite correctly");
        favorite.favorite = true;
        favorite.index = 1;
        contact.addDetail(favorite);
        saveAndRefreshContact();
        var detail = contact.detail(ContactDetail.Favorite);
        compare(detail.favorite, true);
        compare(detail.index, 1);
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

    Guid {
        id: guid
    }

    function test_guid() {
        guid.guid = "TestGUID"
        contact.addDetail(guid)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Guid)
        compare(detail.guid, "TestGUID")
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
        name.contexts = [ContactDetail.ContextHome]
        contact.addDetail(name)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Name)
        compare(detail.prefix, "Dr.")
        compare(detail.firstName, "John")
        compare(detail.middleName, "B.")
        compare(detail.lastName, "Doe")
        compare(detail.suffix, "Sr.")
        if (model.manager == 'jsondb')
            expectFail("", "contexts are not supported at the moment")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], ContactDetail.ContextHome, "contexts")
    }

    Name {
        id: nameSanitizable
    }

    function test_nameCanBeSanitized() {
        nameSanitizable.prefix = "    Mr.  "
        nameSanitizable.firstName = " Matti-Tapio "
        nameSanitizable.middleName = "C. "
        nameSanitizable.lastName = " De  Angelis"
        nameSanitizable.suffix = "Sr."
        contact.addDetail(nameSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Name)
        if (model.manager == "jsondb") {
            compare(detail.prefix, "Mr.")
            compare(detail.firstName, "Matti-Tapio")
            compare(detail.middleName, "C.")
            compare(detail.lastName, "De Angelis")
            compare(detail.suffix, "Sr.")
        } else {
            compare(detail.prefix, "    Mr.  ")
            compare(detail.firstName, " Matti-Tapio ")
            compare(detail.middleName, "C. ")
            compare(detail.lastName, "De  Angelis")
            compare(detail.suffix, "Sr.")
        }
    }

    Name {
        id: nameNotSanitizable
    }

    function test_nameCanNotBeSanitized() {
        // If any of the name fields is longer than 50 chars it is invalid, thus the contact won't be saved
        nameNotSanitizable.prefix = ""
        nameNotSanitizable.firstName = "Matti-Tapio"
        nameNotSanitizable.middleName = "C. "
        nameNotSanitizable.lastName = "Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee"
        nameNotSanitizable.suffix = "Sr."
        contact.addDetail(nameNotSanitizable)
        if (model.manager == "jsondb") {
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            var detail = contact.detail(ContactDetail.Name)
            compare(detail.prefix, "")
            compare(detail.firstName, "Matti-Tapio")
            compare(detail.middleName, "C. ")
            compare(detail.lastName, "Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee")
            compare(detail.suffix, "Sr.")
        }
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

    Nickname {
        id: nicknameSanitizable
    }

    function test_nicknameCanBeSanitized() {
        nicknameSanitizable.nickname = "     Dummy     "
        contact.addDetail(nicknameSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.NickName)
        if (model.manager == "jsondb") {
            compare(detail.nickname, "Dummy")
        } else {
            compare(detail.nickname, "     Dummy     ")
        }
    }

    Nickname {
        id: nicknameNotSanitizable
    }

    function test_nicknameCanNotBeSanitized() {
        // If nickname is longer than 50 chars it is invalid and won't be saved
        nicknameNotSanitizable.nickname = "Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee"
        contact.addDetail(nicknameNotSanitizable)
        if (model.manager == "jsondb") {
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            var detail = contact.detail(ContactDetail.NickName)
            compare(detail.nickname, "Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee")
        }
    }

    DisplayLabel {
        id: displaylabel
    }

    function test_displaylabel() {
        displaylabel.label = "Dummy"
        contact.addDetail(displaylabel)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.DisplayLabel)
        compare(detail.label, "Dummy")
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

    Note {
        id: noteSanitizable
    }

    function test_noteCanBeSanitized() {
        noteSanitizable.note = "     Dummy     "
        contact.addDetail(noteSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Note)
        if (model.manager == "jsondb") {
            compare(detail.note, "Dummy")
        } else {
            compare(detail.note, "     Dummy     ")
        }
    }

    Note {
        id: noteNotSanitizable
    }

    function test_noteCanNotBeSanitized() {
        // If the note is longer than 1000 chars it is invalid and won't be saved
        var veryLongNote
        for (var i = 0; i < 10001; i++) {
             veryLongNote = "a" + veryLongNote
        }
        noteNotSanitizable.note = veryLongNote
        contact.addDetail(noteNotSanitizable)
        if (model.manager == "jsondb") {
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            var detail = contact.detail(ContactDetail.Note)
            compare(detail.note, veryLongNote)
        }
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
        organization.contexts = ["Home"]
        contact.addDetail(organization)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Organization)
        compare(detail.name, "Name")
        compare(detail.logoUrl, "http://qt.nokia.com")
        compare(detail.department[0], "Department")
        if (model.manager == 'jsondb')
            expectFail("", "TODO: location is not working as expected at the moment");
        compare(detail.location, "Location")
        if (model.manager == 'jsondb')
            expectFail("", "TODO: role is not working as expected at the moment");
        compare(detail.role, "Role")
        compare(detail.title, "Title")
        compare(detail.assistantName, "Assistant name")

        expectFail("", "TODO: contexts are not working as expected at the moment");
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], "Home", "contexts")
    }

    Organization {
        id: organizationSanitizable
    }

    function test_organizationCanBeSanitized() {
        organizationSanitizable.name = "  Company   Name     "
        contact.addDetail(organizationSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Organization)
        if (model.manager == "jsondb") {
            compare(detail.name, "Company Name")
        } else {
            compare(detail.name, "  Company   Name     ")
        }
    }

    Organization {
        id: organizationNotSanitizable
    }

    function test_organizationCanNotBeSanitized() {
        // If any of the organization fields is longer than 50 chars it is invalid, thus the contact won't be saved
        organizationNotSanitizable.name =  "Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee"
        contact.addDetail(organizationNotSanitizable)
        if (model.manager == "jsondb") {
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            var detail = contact.detail(ContactDetail.Organization)
            compare(detail.name, "Vvvvvvvvvveeeeeeeeeerrrrrrrrrryyyyyyyyyylongnameeeeeeeeee")
        }
    }

    Organization {
        id: organizations1
        name: "A"
    }

    Organization {
        id: organizations2
        name: "B"
    }

    function test_multipleOrganizations() {
        contact.addDetail(organizations1);
        contact.addDetail(organizations2);
        saveAndRefreshContact();
        var details = contact.details(ContactDetail.Organization);
        compare(details.length, 2, "details length");
        if (details[0].name === organizations2.name)
            details.reverse(); // assuming detail order was not preserved
        compare(details[0].name, organizations1.name, "name 0");
        compare(details[1].name, organizations2.name, "name 1");
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
        phonenumber.contexts = [ContactDetail.ContextHome]
        contact.addDetail(phonenumber)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.PhoneNumber)
        compare(detail.number, "1")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], ContactDetail.ContextHome, "contexts")
        if (model.manager == 'jsondb') {
            compare(detail.subTypes.length, 1, "jsondb phone number subtypes length")
            compare(detail.subTypes[0], phonenumber.subTypes[0])
        } else {
            compare(detail.subTypes.length, phonenumber.subTypes.length)
        }
    }

    PhoneNumber {
        id: phonenumberOneSubtype
    }

    function test_phoneNumberOneSubtype() {
        phonenumberOneSubtype.subTypes = [
                    PhoneNumber.Mobile
                ]
        phonenumberOneSubtype.number = "1"
        contact.addDetail(phonenumberOneSubtype)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.PhoneNumber)
        compare(detail.number, "1")
        compare(detail.subTypes.length, phonenumberOneSubtype.subTypes.length)
    }

    PhoneNumber {
        id: phonenumberSanitizable
    }

    function test_phoneNumberCanBeSanitized() {
        phonenumberSanitizable.subTypes = [
                    PhoneNumber.Mobile
                ]
        phonenumberSanitizable.number = "+1     +(234)-56789abcdef*#     "
        contact.addDetail(phonenumberSanitizable)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.PhoneNumber)
        if (model.manager == "jsondb")
            // jsondb backend will remove blank spaces and other invalid characters
            compare(detail.number, "+1(234)-56789abcd*#")
        else // we do not expect other backends to sanitize the input by default
            compare(detail.number, "+1     +(234)-56789abcdef*#     ")
        compare(detail.subTypes.length, phonenumberSanitizable.subTypes.length)
    }

    PhoneNumber {
        id: phonenumberNotSanitizable
    }

    function test_phoneNumberCannotBeSanitized() {
        phonenumberNotSanitizable.subTypes = [
                    PhoneNumber.Mobile
                ]
        phonenumberNotSanitizable.number = "     efghijk     []/>>>>>"
        contact.addDetail(phonenumberNotSanitizable)

        var detail
        if (model.manager == "jsondb") {
            saveContactWithError("BadArgument")
        } else {
            saveAndRefreshContact()
            detail = contact.detail(ContactDetail.PhoneNumber)
            compare(detail.number, "     efghijk     []/>>>>>")
            compare(detail.subTypes.length, phonenumberNotSanitizable.subTypes.length)
        }
    }

    PhoneNumber {
        id: phonenumbers1
        number: "1"
    }

    PhoneNumber {
        id: phonenumbers2
        number: "2"
    }

    function test_multiplePhoneNumbers() {
        contact.addDetail(phonenumbers1);
        contact.addDetail(phonenumbers2);
        saveAndRefreshContact();
        var details = contact.details(ContactDetail.PhoneNumber);
        compare(details.length, 2, "details length");
        if (details[0].number === phonenumbers2.number)
            details.reverse(); // assuming detail order was not preserved
        compare(details[0].number, phonenumbers1.number, "number 0");
        compare(details[1].number, phonenumbers2.number, "number 1");
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

    SyncTarget {
        id: synctarget
    }

    function test_synctarget() {
        synctarget.syncTarget = "TestSyncTarget"
        contact.addDetail(synctarget)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.SyncTarget)
        compare(detail.syncTarget, "TestSyncTarget")
    }

    Url {
        id: url
    }

    function test_url() {
        url.url = "http://qt.nokia.com"
        url.contexts = [ContactDetail.ContextHome]
        contact.addDetail(url)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Url)
        compare(detail.url, "http://qt.nokia.com")
        compare(detail.contexts.length, 1, "contexts length")
        compare(detail.contexts[0], ContactDetail.ContextHome, "contexts")
    }

    Url {
        id: urls1
        url: "http://a"
    }

    Url {
        id: urls2
        url: "http://b"
    }

    function test_multipleUrls() {
        contact.addDetail(urls1);
        contact.addDetail(urls2);
        saveAndRefreshContact();
        var details = contact.details(ContactDetail.Url);
        compare(details.length, 2, "details length");
        if (details[0].url === urls2.url)
            details.reverse(); // assuming detail order was not preserved
        compare(details[0].url, urls1.url, "url 0");
        compare(details[1].url, urls2.url, "url 1");
    }

    Version {
        id: version
    }

    function test_version() {
        version.sequenceNumber = 64
        version.extendedVersion = "Qt rules"
        contact.addDetail(version)
        saveAndRefreshContact()
        var detail = contact.detail(ContactDetail.Version)
        verify(detail.sequenceNumber != undefined)
        if (model.manager == "jsondb") {
            compare(detail.sequenceNumber, 1, "sequenceNumber is always 1 for jsondb")
            compare(detail.extendedVersion.length, 10, "jsondb extended version field is 10 characters long")
        }
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

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

    function saveAndRefreshContact() {
        model.saveContact(contact)
        waitForContactsChanged();
        compare(model.contacts.length, 1, "Unexpected amount of contacts in model after updating contact.")
        contact = model.contacts[0]
    }

    function saveContactWithError(errorMessage) {
        var errorSpy = initTestForTargetListeningToSignal(model, "errorChanged");
        model.saveContact(contact);
        waitForTargetSignal(errorSpy);
        verifyNoContactsChangedReceived();
        compare(model.contacts.length, 0, "Model should be empty after saving an invalid contact")
        compare(model.error, errorMessage, "At this point we should get this error \"" + errorMessage + "\" from the model.")
    }
}
