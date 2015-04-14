/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPim module of the Qt Toolkit.
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

ContactsSavingTestCase {

    name: "ContactsExportImportDetailsE2ETests"

    ContactModel {
        id: model
        manager: "memory"
        autoUpdate: true
    }

    Contact {
        id: megaContact

        Address {
            street: "Street"
            locality: "Locality"
            region: "Region"
            postcode: "12345"
            country: "Country"
            contexts: [ContactDetail.ContextHome]
        }

        Address {
            street: "Street2"
            locality: "Locality2"
            region: "Region2"
            postcode: "123452"
            country: "Country2"
            contexts: [ContactDetail.ContextHome,
                ContactDetail.ContextWork]
        }

        Anniversary {
            calendarId: "Calendar id"
            originalDate: new Date("2012-01-01")
            event: "Event"
            subType: Anniversary.Employment
        }

        Avatar {
            imageUrl: "http://nokia.com/imageurl"
        }

        Birthday {
            birthday: new Date(2000, 1, 1, 12, 0, 0)
        }

        /* Display label is currently read-only
        DisplayLabel {
            label: "Display label"
        }
        */

        EmailAddress {
            emailAddress: "emailaddress@ovi.com.invalid"
            contexts: [ContactDetail.ContextWork]
        }

        EmailAddress {
            emailAddress: "emailaddress2@ovi.com.invalid"
            contexts: [ContactDetail.ContextHome,
                ContactDetail.ContextWork]
        }

        ExtendedDetail {
            name: "Extended detail: string data"
            data: "string 1"
        }

        ExtendedDetail {
            name: "Extended detail: number data"
            data: 2
        }

        ExtendedDetail {
            name: "Extended detail: date data"
            data: new Date(2000, 0, 2, 12, 13, 14)
        }

        ExtendedDetail {
            name: "Extended detail: array data"
            data: ["string 1", "string 2"]
        }

        ExtendedDetail {
            name: "Extended detail: object data"
            data: {"key 1": "string 1", "key 2": "string 2"}
        }

        Family {
            spouse: "Spouse"
            children: ["Kid A", "Kid B"]
        }

        Favorite {
            favorite: true
            index: 1
        }

        Gender {
            gender: Gender.Male
        }

        Guid {
            guid: "Guid"
        }

        GlobalPresence {
            timestamp: new Date(2012, 1, 1, 12, 0, 0)
            nickname: "Nickname"
            stateText: "State text"
            imageUrl: "http://qt.nokia.com/url"
            customMessage: "Custom message"
            state: Presence.Away
        }

        Hobby {
            hobby: "Hobby"
        }

        Location {
            latitude: 1.1
            longitude: 1.2
            label: "Location"
            accuracy: 1
            altitude: 1
            altitudeAccuracy: 1
            heading: 1
            speed: 1
            timestamp: new Date(2012, 1, 1, 12, 0, 0)
        }

        Name {
            prefix: "Prefix"
            firstName: "First"
            middleName: "Middle"
            lastName: "Last"
            suffix: "Suffix"
        }

        Nickname {
            nickname: "Nick"
        }

        Note {
            note: "Note"
        }

        OnlineAccount {
            accountUri: "http://qt.nokia.com/url"
            serviceProvider: "Service provider"
            capabilities: ["Capability"]
            subTypes: [OnlineAccount.Sip]
        }

        Organization {
            name: "Organization"
            logoUrl: "http://qt.nokia.com/logourl"
            department: "Department"
            location: "Location"
            title: "Title"
            role: "Role"
            assistantName: "Assistant"
        }

        Organization {
            name: "Organization2"
            logoUrl: "http://qt.nokia.com/logourl2"
            department: "Department2"
            location: "Location2"
            title: "Title2"
            role: "Role2"
            assistantName: "Assistant2"
        }

        PhoneNumber {
            subTypes: [
                PhoneNumber.Voice,
                PhoneNumber.Mobile,
                PhoneNumber.Fax,
                PhoneNumber.Video,
            ]
            number: "12345"
            contexts: [ContactDetail.ContextHome]
        }

        PhoneNumber {
            subTypes: [
                PhoneNumber.Voice,
                PhoneNumber.Mobile,
                PhoneNumber.Fax,
                PhoneNumber.Video,
            ]
            number: "54321"
            contexts: [ContactDetail.ContextHome,
                ContactDetail.ContextWork]
        }

        Presence {
            timestamp: new Date()
            nickname: "Nickname"
            stateText: "State text"
            imageUrl: "http://qt.nokia.com/url"
            customMessage: "Custom message"
            state: Presence.Away
        }

        Ringtone {
            audioRingtoneUrl: "http://qt.nokia.com/audioringtoneurl"
        }

        Tag {
            tag: "Tag"
        }

        Timestamp {
            created: new Date(2012, 1, 1, 12, 0, 0)
            lastModified: new Date(2012, 1, 1, 12, 0, 0)
        }

        Url {
            url: "http://qt.nokia.com/url"
            contexts: [ContactDetail.ContextWork]
        }

        Url {
            url: "http://qt.nokia.com/url2"
            contexts: [ContactDetail.ContextHome,
                ContactDetail.ContextWork]
        }

        Version {
            sequenceNumber: 1
            extendedVersion: "Extended version"
        }

    }

    // Base for vcard file names.
    property string vcardFileNameBase: 'tst_contacts_export_import_details_e2e'
    property Contact importedContact

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();

        var signalSpy = initTestForTargetListeningToSignal(model, "exportCompleted");

        // Save and fetch test contact.
        model.saveContact(megaContact);
        waitForContactsChanged();

        // Export contacts to vcard file.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + ".vcard");
        model.exportContacts(vcardFilePath, ["Sync"]);
        waitForTargetSignal(signalSpy);

        // Empty all contacts and import just created vcard file.
        emptyContacts(model);
        listenToContactsChanged();
        model.importContacts(vcardFilePath, ["Sync"]);
        waitForContactsChanged();
        compare (model.error, "NoError", "importError");

        // Check that imported contact has correct data.
        compare (model.contacts.length, 1, "import count");
        importedContact = model.contacts[0];
    }

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Tests

    function test_exportImportAddress() {
        compare(importedContact.addresses.length, 2, "Error importing/exporting multiple addresses");

        var importedAddresses = importedContact.details(ContactDetail.Address);

        if (importedAddresses[0].street === megaContact.addresses[1].street)
            importedAddresses.reverse();

        for (var i = 0; i < 2; i++) {
            compare(importedAddresses[i].street, megaContact.addresses[i].street, "street");
            compare(importedAddresses[i].locality, megaContact.addresses[i].locality, "locality");
            compare(importedAddresses[i].region, megaContact.addresses[i].region, "region");
            compare(importedAddresses[i].postcode, megaContact.addresses[i].postcode, "postcode");
            compare(importedAddresses[i].country, megaContact.addresses[i].country, "country");
            verify(importedAddresses[i].contexts.length, "Contexts not imported/exported");
            compareSets(importedAddresses[i].contexts, megaContact.addresses[i].contexts);
        }
    }

    function test_exportImportAnniversary() {
        compare(importedContact.anniversary.originalDate, megaContact.anniversary.originalDate, "anniversary");
        expectFail("", "Anniversary calendar id is not exported at the moment.")
        compare(importedContact.anniversary.calendarId, megaContact.anniversary.calendarId, "calendar id");
        expectFail("", "Anniversary event is not exported at the moment.")
        compare(importedContact.anniversary.event, megaContact.anniversary.event, "event");
        expectFail("", "Anniversary subtype is not exported at the moment.")
        compare(importedContact.anniversary.subType, megaContact.anniversary.subType, "subtypes");
    }

    function test_exportImportAvatar() {
        compare(importedContact.avatar.imageUrl, megaContact.avatar.imageUrl, "image url");
    }

    function test_exportImportBirthday() {
        compare(importedContact.birthday.birthday, megaContact.birthday.birthday, "birthday");
    }

    function test_exportImportEmail() {
        compare(importedContact.emails.length, 2, "Error importing/exporting multiple email addresses");

        var importedEmails = importedContact.details(ContactDetail.Email);

        if (importedEmails[0].emailAddress === megaContact.emails[1].emailAddress)
            importedEmails.reverse();

        for (var i = 0; i < 2; i++) {
            compare(importedEmails[i].emailAddress, megaContact.emails[i].emailAddress, "email address");
            compareSets(importedEmails[i].contexts, megaContact.emails[i].contexts);
        }
    }

    function test_exportImportExtendedDetail() {
        compare(importedContact.extendedDetails.length, 5, "Error importing/exporting multiple extended details");

        var importedExtendedDetails = importedContact.details(ContactDetail.ExtendedDetail);

        for (var i = 0; i < 4; i++) {
            compare(importedExtendedDetails[i].name, megaContact.extendedDetails[i].name, "extended detail name");
            if (importedExtendedDetails[i].name === "Extended detail: date data") {
                // A date is exported as a string in local time without timezone specified.
                // QML/V8 javascript Date.parse cannot be used to parse it,
                // since it interprets a date string without timezone as using UTC.
                compare(importedExtendedDetails[i].data,
                        Qt.formatDateTime(megaContact.extendedDetails[i].data, 'yyyy-MM-ddThh:mm:ss'), "extended detail data");
            } else {
                compare(importedExtendedDetails[i].data, megaContact.extendedDetails[i].data, "extended detail data");
            }
        }
    }

    function test_exportImportFamily() {
        compare(importedContact.family.spouse, megaContact.family.spouse, "spouse");
        compareSets(importedContact.family.children, megaContact.family.children, "children");
    }

    function test_exportImportFavorite() {
        compare(importedContact.favorite.favorite, megaContact.favorite.favorite, "favorite");
        compare(importedContact.favorite.index, megaContact.favorite.index, "index");
    }

    function test_exportImportGender() {
        compare(importedContact.gender.gender, megaContact.gender.gender, "gender");
    }

    function test_exportImportGlobalPresence() {
        expectFail("", "Global presence is not exported at the moment.");
        compare(importedContact.globalPresence.timestamp, megaContact.globalPresence.timestamp, "timestamp");
        expectFail("", "Global presence is not exported at the moment.");
        compare(importedContact.globalPresence.nickname, megaContact.globalPresence.nickname, "nickname");
        expectFail("", "Global presence is not exported at the moment.");
        compare(importedContact.globalPresence.stateText, megaContact.globalPresence.stateText, "state text");
        expectFail("", "Global presence is not exported at the moment.");
        compare(importedContact.globalPresence.imageUrl, megaContact.globalPresence.imageUrl, "image url");
        expectFail("", "Global presence is not exported at the moment.");
        compare(importedContact.globalPresence.customMessage, megaContact.globalPresence.customMessage, "custom message");
        expectFail("", "Global presence is not exported at the moment.");
        compare(importedContact.globalPresence.state, megaContact.globalPresence.state, "state");
    }

    function test_exportImportGuid() {
        compare(importedContact.guid.guid, megaContact.guid.guid, "guid");
    }

    function test_exportImportHobby() {
        expectFail("", "Hobby is not exported at the moment.");
        compare(importedContact.hobby.hobby, megaContact.hobby.hobby, "hobby");
    }

    function test_exportImportLocation() {
        compare(importedContact.geolocation.latitude, megaContact.geolocation.latitude, "latitude");
        compare(importedContact.geolocation.longitude, megaContact.geolocation.longitude, "longitude");
        expectFail("", "Location label is not exported at the moment.");
        compare(importedContact.geolocation.label, megaContact.geolocation.label, "label");
        expectFail("", "Location accuracy is not exported at the moment.");
        compare(importedContact.geolocation.accuracy, megaContact.geolocation.accuracy, "accuracy");
        expectFail("", "Location altitude is not exported at the moment.");
        compare(importedContact.geolocation.altitude, megaContact.geolocation.altitude, "altitude");
        expectFail("", "Location altitude accuracy is not exported at the moment.");
        compare(importedContact.geolocation.altitudeAccuracy, megaContact.geolocation.altitudeAccuracy, "altitudeAccuracy");
        expectFail("", "Location heading is not exported at the moment.");
        compare(importedContact.geolocation.heading, megaContact.geolocation.heading, "heading");
        expectFail("", "Location speed is not exported at the moment.");
        compare(importedContact.geolocation.speed, megaContact.geolocation.speed, "speed");
        expectFail("", "Location timestamp is not exported at the moment.");
        compare(importedContact.geolocation.timestamp, megaContact.geolocation.timestamp, "timestamp");
    }

    function test_exportImportName() {
        compare(importedContact.name.firstName, megaContact.name.firstName, "first name");
        compare(importedContact.name.lastName, megaContact.name.lastName, "last name");
        compare(importedContact.name.middleName, megaContact.name.middleName, "middle name");
        compare(importedContact.name.prefix, megaContact.name.prefix, "prefix");
        compare(importedContact.name.suffix, megaContact.name.suffix, "suffix");
    }

    function test_exportImportNickname() {
        compare(importedContact.nickname.nickname, megaContact.nickname.nickname, "nickname");
    }

    function test_exportImportNote() {
        compare(importedContact.note.note, megaContact.note.note, "note");
    }

    function test_exportImportOnlineAccount() {
        compare(importedContact.onlineAccount.accountUri, megaContact.onlineAccount.accountUri, "account uri");
        expectFail("", "Service provider is not exported at the moment.");
        compare(importedContact.onlineAccount.serviceProvider, megaContact.onlineAccount.serviceProvider, "service provider");
        expectFail("", "Capabilities are not exported at the moment.");
        compare(importedContact.onlineAccount.capabilities[0], megaContact.onlineAccount.capabilities[0], "capabilities");
        expectFail("", "Subtypes are not exported at the moment.");
        compare(importedContact.onlineAccount.subTypes[0], megaContact.onlineAccount.subTypes[0], "subtypes");
    }

    function test_exportImportOrganization() {
        compare(importedContact.organizations.length, 2, "Error importing/exporting multiple organizations");

        var importedOrganizations = importedContact.details(ContactDetail.Organization);

        if (importedOrganizations[0].name === megaContact.organizations[1].name)
            importedOrganizations.reverse();

        for (var i = 0; i < 2; i++) {
            compare(importedOrganizations[i].name, megaContact.organizations[i].name, "name");
            compare(importedOrganizations[i].logoUrl, megaContact.organizations[i].logoUrl, "logo url");
            compare(importedOrganizations[i].department, megaContact.organizations[i].department, "department");
            compare(importedOrganizations[i].title, megaContact.organizations[i].title, "title");
            compare(importedOrganizations[i].role, megaContact.organizations[i].role, "role");
            compare(importedOrganizations[i].assistantName, megaContact.organizations[i].assistantName, "assistant name");
            expectFail("", "Organization location is not exported at the moment.");
            compare(importedOrganizations[i].location, megaContact.organizations[i].location, "location");
        }

    }

    function test_exportImportPhoneNumber() {
        compare(importedContact.phoneNumbers.length, 2, "Error importing/exporting multiple phone numbers");

        var importedPhoneNumbers = importedContact.details(ContactDetail.PhoneNumber);

        if (importedPhoneNumbers[0].number === megaContact.phoneNumbers[1].number)
            importedPhoneNumbers.reverse();

        for (var i = 0; i < 2; i++) {
            compare(importedPhoneNumbers[i].number, megaContact.phoneNumbers[i].number, "phone number");
            compareSets(importedPhoneNumbers[i].contexts, megaContact.phoneNumbers[i].contexts);
            compareSets(importedPhoneNumbers[i].subTypes, megaContact.phoneNumbers[i].subTypes);
        }
    }

    function test_exportImportPresence() {
        expectFail("", "Presence is not exported at the moment.");
        compare(importedContact.presence.timestamp, megaContact.presence.timestamp, "timestamp");
        expectFail("", "Presence is not exported at the moment.");
        compare(importedContact.presence.nickname, megaContact.presence.nickname, "nickname");
        expectFail("", "Presence is not exported at the moment.");
        compare(importedContact.presence.stateText, megaContact.presence.stateText, "state text");
        expectFail("", "Presence is not exported at the moment.");
        compare(importedContact.presence.imageUrl, megaContact.presence.imageUrl, "image url");
        expectFail("", "Presence is not exported at the moment.");
        compare(importedContact.presence.customMessage, megaContact.presence.customMessage, "custom message");
        expectFail("", "Presence is not exported at the moment.");
        compare(importedContact.presence.state, megaContact.presence.state, "state");
    }

    function test_exportImportRingtone() {
        compare(importedContact.ringtone.audioRingtoneUrl, megaContact.ringtone.audioRingtoneUrl);
        compare(importedContact.ringtone.audioRingtoneUrl, "http://qt.nokia.com/audioringtoneurl");
    }

    function test_exportImportUrl() {
        compare(importedContact.urls.length, 2, "Error importing/exporting multiple urls");

        var importedUrls = importedContact.details(ContactDetail.Url);

        if (importedUrls[0].url === megaContact.urls[1].url)
            importedUrls.reverse();

        for (var i = 0; i < 2; i++) {
            compare(importedUrls[i].url, megaContact.urls[i].url, "url");
            compareSets(importedUrls[i].contexts, megaContact.urls[i].contexts);
        }
    }

    function test_exportImportTag() {
        compare(importedContact.tag.tag, megaContact.tag.tag, "tag");
    }

    function test_exportImportTimestamp() {
        // No convenience accessor exists for timestamp property!
        var importedTimestamp = importedContact.detail(ContactDetail.Timestamp);
        var originalTimestamp = megaContact.detail(ContactDetail.Timestamp);
        expectFail("", "Timestamps are not exported at the moment.");
        compare(importedTimestamp.created, originalTimestamp.created, "timestamp");
        expectFail("", "Timestamps are not exported at the moment");
        compare(importedTimestamp.lastModified, originalTimestamp.lastModified, "timestamp");
    }

    function test_exportImportVersion() {
        compare(importedContact.version.sequenceNumber, megaContact.version.sequenceNumber, "sequence number");
        compare(importedContact.version.extendedVersion, megaContact.version.extendedVersion, "extended version");
    }

    function compareSets(a, b) {
        compare(a.length, b.length, "Different amount of items in compared sets");
        for (var i = 0; i < a.length; i++) {
            var matches = 0;
            var matchesNeeded = 0;
            for (var j = 0; j < a.length; j++) {
                if (a[i] === a[j])
                    matchesNeeded++;
            }
            for (var j = 0; j < b.length; j++) {
                if (a[i] === b[j])
                    matches++;
            }
            compare(matches, matchesNeeded, "Items in sets don't match");
        }
    }

}
