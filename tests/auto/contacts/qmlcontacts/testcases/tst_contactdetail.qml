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


TestCase {
    name: "ContactDetailsTests"

    Address {
        id: address
    }

    Anniversary {
        id: anniversary
    }

    Avatar {
        id: avatar
    }

    Birthday {
        id: birthday
    }

    ContactDetail {
        id: detailCommon
    }

    ContactDetail {
        id: detailContexts
    }

    DisplayLabel {
        id: displaylabel
    }

    EmailAddress {
        id: emailAddress
    }

    ExtendedDetail {
        id: extendedDetail;
    }

    Family {
        id: family
    }

    Favorite {
        id: favorite
    }

    Gender {
        id: gender
    }

    Location {
        id: location
    }

    GlobalPresence {
        id: globalpresence
    }

    Guid {
        id: guid
    }

    Hobby {
        id: hobby
    }

    Name {
        id: name
    }

    Nickname {
        id: nickname
    }

    Note {
        id: note
    }

    OnlineAccount {
        id: onlineAccount
    }

    Organization {
        id: organization
    }

    PhoneNumber {
        id: phonenumber
    }

    Presence {
        id: presence
    }

    Ringtone {
        id: ringtone
    }

    SyncTarget {
        id: synctarget
    }

    Tag {
        id: tag
    }

    Timestamp {
        id: timestamp
    }

    Type {
        id: type
    }

    Url {
        id: url
    }

    Version {
        id: version
    }

    property url myUrl: "http://nokia.com"
    property variant myEmptyVariant
    property variant myStringVariant: "String data in a variant"
    property variant myListVariant: [ 0, 1, 2, 3, 4, 5, 6, 7, "eight", "nine", "ten"]


    function test_address() {
        compare(address.street, "")
        address.street = "Kauppakatu"
        compare(address.street, "Kauppakatu")

        compare(address.locality, "")
        address.locality = "Tampere"
        compare(address.locality, "Tampere")

        compare(address.region, "")
        address.region = "Pirkanmaa"
        compare(address.region, "Pirkanmaa")

        compare(address.postcode, "")
        address.postcode = "33720"
        compare(address.postcode, "33720")

        compare(address.country, "")
        address.country = "Finland"
        compare(address.country, "Finland")

        compare(address.subTypes, [])
        address.subTypes = [Address.Postal]
        compare(address.subTypes, [Address.Postal])

        compare(address.postOfficeBox, "")
        address.postOfficeBox = "510"
        compare(address.postOfficeBox, "510")
    }


    function test_anniversary() {
        compare(anniversary.calendarId, "")
        anniversary.calendarId = "myCalendarId"
        compare(anniversary.calendarId, "myCalendarId")

        //compare(anniversary.originalDate, "")
        var dateTime = new Date(2001, 5, 21)  // 21 May 2001 14:13:09
        anniversary.originalDate = dateTime
        compare(anniversary.originalDate, dateTime)

        compare(anniversary.event, "")
        anniversary.event = "myEvent"
        compare(anniversary.event, "myEvent")

        //compare(anniversary.subType, "")
        anniversary.subType = Anniversary.Wedding
        compare(anniversary.subType, Anniversary.Wedding)
    }


    function test_avatar() {
        compare(avatar.imageUrl, "")
        avatar.imageUrl = "http://nokia.com"
        compare(avatar.imageUrl.toString(), "http://nokia.com")

        compare(avatar.videoUrl, "")
        avatar.videoUrl = "http://nokia2.com"
        compare(avatar.videoUrl.toString(), "http://nokia2.com")
    }


    function test_birthday() {
        var dateTime = new Date(2001, 5, 21, 14, 13, 09)  // 21 May 2001 14:13:09
        //compare(birthday.birthday, "")
        birthday.birthday = dateTime;
        compare(birthday.birthday, dateTime)
    }


    function test_detail_common_properties() {
        compare(detailCommon.type, ContactDetail.Unknown, "type");
        compare(detailCommon.detailUri, "", "detailUri");
        compare(detailCommon.linkedDetailUris, [], "linkedDetailUris");
        compare(detailCommon.fields, [], "fields");
        compare(detailCommon.readOnly, false, "readOnly");
        compare(detailCommon.removable, true, "removable");
    }

    function test_detail_contexts() {
        compare(detailContexts.contexts, [], "contexts");
        detailContexts.contexts = [ContactDetail.ContextHome];
        compare(detailContexts.contexts, [ContactDetail.ContextHome], "context");
    }

    function test_displayLabel() {
        compare(displaylabel.label, "")
        displaylabel.label = "exampleFullName";
        compare(displaylabel.label, "exampleFullName")
    }

    function test_emailAddress() {
        compare(emailAddress.emailAddress, "")
        emailAddress.emailAddress = "ns@ovi.com";
        compare(emailAddress.emailAddress, "ns@ovi.com")
    }


    function test_extendedDetail() {

        compare(extendedDetail.name, "", "Empty name")
        verify(!extendedDetail.data, "Empty data")

        extendedDetail.name = "MyDetail";
        extendedDetail.data = "MyData";
        compare(extendedDetail.name, "MyDetail", "Valid name (string data)")
        compare(extendedDetail.data, "MyData", "Valid data (string data)")

        extendedDetail.name = "MyNumber";
        extendedDetail.data = 1;
        compare(extendedDetail.name, "MyNumber", "Valid name (int data)")
        compare(extendedDetail.data, 1, "Valid data (int data)")

        extendedDetail.name = "MyVariant";
        extendedDetail.data = myStringVariant;
        compare(extendedDetail.name, "MyVariant", "Valid name (string variant data)")
        compare(extendedDetail.data, myStringVariant, "Valid data (string variant data)")

        extendedDetail.name = "MyListVariant";
        extendedDetail.data = myListVariant;
        compare(extendedDetail.name, "MyListVariant", "Valid name (list variant data)")

        var actualData = extendedDetail.data;
        var expectedData = myListVariant;
        for (var j=0; j < expectedData.length;j++) {
            var actualItem = actualData[j];
            var expectedItem = expectedData[j];
            compare(actualItem, expectedItem, "Valid data (list variant), item #" + j)
        }
    }


    function test_family() {
        compare(family.spouse, "")
        family.spouse = "Kate";
        compare(family.spouse, "Kate")

        compare(family.children, [])
        family.children = ["Mike"];
        compare(family.children, ["Mike"])
    }


    function test_favorite() {
        compare(favorite.favorite, false)
        favorite.favorite = true;
        compare(favorite.favorite, true)

        compare(favorite.index, 0)
        favorite.index = 1;
        compare(favorite.index, 1)
    }


    function test_gender() {
        compare(gender.gender, Gender.Female)
        gender.gender = Gender.Male;
        compare(gender.gender, Gender.Male)
    }


    function test_geolocation() {
        compare(location.label, "")
        location.label = "Finland";
        compare(location.label, "Finland")

        compare(location.latitude, 0.0)
        location.latitude = 15.0;
        compare(location.latitude, 15.0)

        compare(location.longitude, 0.0)
        location.longitude = 20.0;
        compare(location.longitude, 20.0)

        compare(location.accuracy, 0.0)
        location.accuracy = 1.0;
        compare(location.accuracy, 1.0)

        compare(location.altitude, 0.0)
        location.altitude = 100.0;
        compare(location.altitude, 100.0)

        compare(location.altitudeAccuracy, 0.0)
        location.altitudeAccuracy = 2.0;
        compare(location.altitudeAccuracy, 2.0)

        compare(location.heading, 0.0)
        location.heading = 1.0;
        compare(location.heading, 1.0)

        compare(location.speed, 0.0)
        location.speed = 30.0;
        compare(location.speed, 30.0)

        var dateTime = new Date(2001, 5, 21, 14, 13, 09)  // 21 May 2001 14:13:09
        location.timestamp = dateTime;
        compare(location.timestamp, dateTime)
    }


    function test_globalpresence() {
        var dateTime = new Date(2001, 5, 21, 14, 13, 09)  // 21 May 2001 14:13:09
        globalpresence.timestamp = dateTime;
        compare(globalpresence.timestamp, dateTime)

        compare(globalpresence.nickname, "")
        globalpresence.nickname = "superman";
        compare(globalpresence.nickname, "superman")

        compare(globalpresence.state, 0)
        globalpresence.state = Presence.Available;
        compare(globalpresence.state, Presence.Available);

        compare(globalpresence.stateText, "")
        globalpresence.stateText = "away...";
        compare(globalpresence.stateText, "away...")

        compare(globalpresence.imageUrl, "")
        globalpresence.imageUrl = "http://nokia.com"
        compare(globalpresence.imageUrl.toString(), "http://nokia.com")

        compare(globalpresence.customMessage, "")
        globalpresence.customMessage = "Gone fishing";
        compare(globalpresence.customMessage, "Gone fishing")
    }


    function test_guid() {
        compare(guid.guid, "")
        guid.guid = "myGuid";
        compare(guid.guid, "myGuid")
    }


    function test_hobby() {
        compare(hobby.hobby, "")
        hobby.hobby = "biking";
        compare(hobby.hobby, "biking")
    }


    function test_name() {
        compare(name.prefix, "")
        name.prefix = "Mr";
        compare(name.prefix, "Mr")

        compare(name.firstName, "")
        name.firstName = "Paul";
        compare(name.firstName, "Paul")

        compare(name.middleName, "")
        name.middleName = "Ron";
        compare(name.middleName, "Ron")

        compare(name.lastName, "")
        name.lastName = "Walsh";
        compare(name.lastName, "Walsh")

        compare(name.suffix, "")
        name.suffix = "First";
        compare(name.suffix, "First")

    }


    function test_nickname() {
        compare(nickname.nickname, "")
        nickname.nickname = "piggy";
        compare(nickname.nickname, "piggy")
    }

    function test_note() {
        compare(note.note, "")
        note.note = "see you";
        compare(note.note, "see you")
    }

    function test_onlineaccount() {
        compare(onlineAccount.accountUri, "")
        onlineAccount.accountUri = "myUri";
        compare(onlineAccount.accountUri, "myUri")

        compare(onlineAccount.serviceProvider, "")
        onlineAccount.serviceProvider = "myProvider";
        compare(onlineAccount.serviceProvider, "myProvider")

        compare(onlineAccount.capabilities, [])
        onlineAccount.capabilities = ["myCapabilities"];
        compare(onlineAccount.capabilities, ["myCapabilities"])

        compare(onlineAccount.subTypes, [])
        onlineAccount.subTypes = [OnlineAccount.Sip];
        compare(onlineAccount.subTypes, [OnlineAccount.Sip])
    }


    function test_organization() {
        compare(organization.name, "")
        organization.name = "ACME";
        compare(organization.name, "ACME")

        compare(organization.logoUrl, "")
        organization.logoUrl = "http://acme.com"
        compare(organization.logoUrl.toString(), "http://acme.com")

        compare(organization.department, [])
        organization.department = ["R&D"];
        compare(organization.department, ["R&D"])

        compare(organization.location, "")
        organization.location = "NY";
        compare(organization.location, "NY")

        compare(organization.role, "")
        organization.role = "CEO";
        compare(organization.role, "CEO")

        compare(organization.title, "")
        organization.title = "Dr";
        compare(organization.title, "Dr")

        compare(organization.assistantName, "")
        organization.assistantName = "Pamela";
        compare(organization.assistantName, "Pamela")

    }

    function test_phonenumber() {
        compare(phonenumber.number, "")
        phonenumber.number  = "112";
        compare(phonenumber.number , "112")

        compare(phonenumber.subTypes, [])
        phonenumber.subTypes = [PhoneNumber.Landline]
        compare(phonenumber.subTypes, [PhoneNumber.Landline])
    }

    function test_presence() {
        var DateTime = new Date(2001, 5, 21)  // 21 May 2001
        presence.timestamp  = DateTime;
        compare(presence.timestamp , DateTime)

        compare(presence.nickname, "")
        presence.nickname = "pearl";
        compare(presence.nickname, "pearl")

        compare(presence.state, 0)
        presence.state = Presence.Available;
        compare(presence.state, Presence.Available)

        compare(presence.stateText, "")
        presence.stateText = "away...";
        compare(presence.stateText, "away...")

        compare(presence.imageUrl, "")
        presence.imageUrl = "http://nokia.com"
        compare(presence.imageUrl.toString(), "http://nokia.com")

        compare(presence.customMessage, "")
        presence.customMessage = "Gone fishing";
        compare(presence.customMessage, "Gone fishing")
    }


    function test_ringtone() {
        compare(ringtone.audioRingtoneUrl, "")
        ringtone.audioRingtoneUrl = "http://acme.com"
        compare(ringtone.audioRingtoneUrl.toString(), "http://acme.com")

        compare(ringtone.videoRingtoneUrl, "")
        ringtone.videoRingtoneUrl = "http://acme.com"
        compare(ringtone.videoRingtoneUrl.toString(), "http://acme.com")

        compare(ringtone.vibrationRingtoneUrl, "")
        ringtone.vibrationRingtoneUrl = "http://acme.com"
        compare(ringtone.vibrationRingtoneUrl.toString(), "http://acme.com")
    }


    function test_synctarget() {
        compare(synctarget.syncTarget, "")
        synctarget.syncTarget = "mySynctarget";
        compare(synctarget.syncTarget, "mySynctarget")
    }


    function test_tag() {
        compare(tag.tag, "")
        tag.tag = "myTag";
        compare(tag.tag, "myTag")
    }


    function test_timestamp() {
        var modifiedDateTime = new Date(2002, 5, 21)  // 21 May 2001
        timestamp.lastModified = modifiedDateTime;
        compare(timestamp.lastModified, modifiedDateTime)
    }


    function test_type() {
        compare(type.type, Type.Unspecified)
        type.type = Type.Group;
        compare(type.type, Type.Group)
        type.type = Type.Contact;
        compare(type.type, Type.Contact)
        type.type = Type.Facet;
        compare(type.type, Type.Facet)
    }


    function test_url() {
        compare(url.url, "")
        url.url = "http://acme.com"
        compare(url.url.toString(), "http://acme.com")

        url.subType = Url.HomePage
        compare(url.subType, Url.HomePage)
    }

    function test_version() {
        compare(version.sequenceNumber, 0)
        compare(version.value(Version.SequenceNumber), undefined)
        version.sequenceNumber = 64
        compare(version.sequenceNumber, 64)
        compare(version.value(Version.SequenceNumber), 64)

        compare(version.extendedVersion, "")
        compare(version.value(Version.ExtendedVersion), undefined)
        version.extendedVersion = "Qt rules"
        compare(version.extendedVersion, "Qt rules")
        //Temporarily disable this test.  TODO: FIXME!
        //compare(version.value(Version.ExtendedVersion).toString(), "Qt rules")
    }


}

