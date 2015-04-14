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

TestCase {
    name: "ContactSignalsTests"
    id: contactSignalsTests

    Contact {
        id: contactWithAddress1
        Address {
            street: "old"
        }
    }

    function test_contact_address_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithAddress1);
        contactWithAddress1.address.street = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithAddress2
        Address {
            street: "old"
        }
    }

    function test_contact_address_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithAddress2);
        contactWithAddress2.address.street = contactWithAddress2.address.street;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithAnniversary1
        Anniversary {
            originalDate: "2011-10-26"
        }
    }

    function test_contact_anniversary_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithAnniversary1);
        contactWithAnniversary1.anniversary.originalDate = "2011-10-27";
        verifySignalReceived();
    }

    Contact {
        id: contactWithAnniversary2
        Anniversary {
            originalDate: "2011-10-26"
        }
    }

    function test_contact_anniversary_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithAnniversary2);
        contactWithAnniversary2.anniversary.originalDate = contactWithAnniversary2.anniversary.originalDate;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithAvatar1
        Avatar {
            imageUrl: "http://old"
        }
    }

    function test_contact_avatar_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithAvatar1);
        contactWithAvatar1.avatar.imageUrl = "http://new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithAvatar2
        Avatar {
            imageUrl: "http://old"
        }
    }

    function test_contact_avatar_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithAvatar2);
        contactWithAvatar2.avatar.imageUrl = contactWithAvatar2.avatar.imageUrl;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithBirthday1
        Birthday {
            birthday: "2010-10-26"
        }
    }

    function test_contact_birthday_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithBirthday1);
        contactWithBirthday1.birthday.birthday = "2010-10-27";
        verifySignalReceived();
    }

    Contact {
        id: contactWithBirthday2
        Birthday {
            birthday: "2010-10-26"
        }
    }

    function test_contact_birthday_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithBirthday2);
        contactWithBirthday2.birthday.birthday = contactWithBirthday2.birthday.birthday;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithEmailAddress1
        EmailAddress {
            emailAddress: "a@a"
        }
    }

    function test_contact_email_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithEmailAddress1);
        contactWithEmailAddress1.email.emailAddress = "b@b";
        verifySignalReceived();
    }

    Contact {
        id: contactWithEmailAddress2
        EmailAddress {
            emailAddress: "a@a"
        }
    }

    function test_contact_email_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithEmailAddress2);
        contactWithEmailAddress2.email.emailAddress = contactWithEmailAddress2.email.emailAddress;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithExtendedDetail1
        ExtendedDetail {
            name: "extended1"
            data: "originaldata1"
        }
    }

    function test_contact_extended_detail_change_name_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithExtendedDetail1);
        contactWithExtendedDetail1.extendedDetail.name = "new name";
        verifySignalReceived();
    }

    function test_contact_extended_detail_change_data_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithExtendedDetail1);
        contactWithExtendedDetail1.extendedDetail.data = "new data";
        verifySignalReceived();
    }

    function test_contact_extended_detail_emits_no_signal_when_name_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithExtendedDetail1);
        contactWithExtendedDetail1.extendedDetail.name = contactWithExtendedDetail1.extendedDetail.name;
        verifyNoSignalReceived();
    }

    function test_contact_extended_detail_emits_no_signal_when_data_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithExtendedDetail1);
        contactWithExtendedDetail1.extendedDetail.data = contactWithExtendedDetail1.extendedDetail.data;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithFavorite1
        Favorite {
            favorite: false
        }
    }

    function test_contact_favorite_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithFavorite1);
        contactWithFavorite1.favorite.favorite = true;
        verifySignalReceived();
    }

    Contact {
        id: contactWithFavorite2
        Favorite {
            favorite: false
        }
    }

    function test_contact_favorite_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithFavorite2);
        contactWithFavorite2.favorite.favorite = contactWithFavorite2.favorite.favorite;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithGender1
        Gender {
            gender: Gender.Male
        }
    }

    function test_contact_gender_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithGender1);
        contactWithGender1.gender.gender = Gender.Female;
        verifySignalReceived();
    }

    Contact {
        id: contactWithGender2
        Gender {
            gender: Gender.Male
        }
    }

    function test_contact_gender_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithGender2);
        contactWithGender2.gender.gender = contactWithGender2.gender.gender;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithLocation1
        Location {
            latitude: 0.0
        }
    }

    function test_contact_geolocation_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithLocation1);
        contactWithLocation1.geolocation.latitude = 1.0;
        verifySignalReceived();
    }

    Contact {
        id: contactWithLocation2
        Location {
            latitude: 0.0
        }
    }

    function test_contact_geolocation_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithLocation2);
        contactWithLocation2.geolocation.latitude = contactWithLocation2.geolocation.latitude;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithGlobalPresence1
        GlobalPresence {
            state: Presence.Unknown
        }
    }

    function test_contact_globalpresence_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithGlobalPresence1);
        contactWithGlobalPresence1.globalPresence.state = Presence.Available;
        verifySignalReceived();
    }

    Contact {
        id: contactWithGlobalPresence2
        GlobalPresence {
            state: Presence.Unknown
        }
    }

    function test_contact_globalpresence_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithGlobalPresence2);
        contactWithGlobalPresence2.globalPresence.state = contactWithGlobalPresence2.globalPresence.state;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithGuid1
        Guid {
            guid: "0000"
        }
    }

    function test_contact_guid_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithGuid1);
        contactWithGuid1.guid.guid = "1111";
        verifySignalReceived();
    }

    Contact {
        id: contactWithGuid2
        Guid {
            guid: "0000"
        }
    }

    function test_contact_guid_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithGuid2);
        contactWithGuid2.guid.guid = contactWithGuid2.guid.guid;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithHobby1
        Hobby {
            hobby: "old"
        }
    }

    function test_contact_hobby_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithHobby1);
        contactWithHobby1.hobby.hobby = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithHobby2
        Hobby {
            hobby: "old"
        }
    }

    function test_contact_hobby_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithHobby2);
        contactWithHobby2.hobby.hobby = contactWithHobby2.hobby.hobby;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithName1
        Name {
            firstName: "old"
        }
    }

    function test_contact_name_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithName1);
        contactWithName1.name.firstName = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithName2
        Name {
            firstName: "old"
        }
    }

    function test_contact_name_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithName2);
        contactWithName2.name.firstName = contactWithName2.name.firstName;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithNickname1
        Nickname {
            nickname: "old"
        }
    }

    function test_contact_nickname_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithNickname1);
        contactWithNickname1.nickname.nickname = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithNickname2
        Nickname {
            nickname: "old"
        }
    }

    function test_contact_nickname_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithNickname2);
        contactWithNickname2.nickname.nickname = contactWithNickname2.nickname.nickname;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithDisplayLabel1
        DisplayLabel {
            label: "old"
        }
    }

    function test_contact_displaylabel_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithDisplayLabel1);
        contactWithDisplayLabel1.displayLabel.label = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithDisplayLabel2
        DisplayLabel {
            label: "old"
        }
    }

    function test_contact_displaylabel_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithDisplayLabel2);
        contactWithDisplayLabel2.displayLabel.label = contactWithDisplayLabel2.displayLabel.label;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithNote1
        Note {
            note: "old"
        }
    }

    function test_contact_note_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithNote1);
        contactWithNote1.note.note = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithNote2
        Note {
            note: "old"
        }
    }

    function test_contact_note_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithNote2);
        contactWithNote2.note.note = contactWithNote2.note.note;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithOnlineAccount1
        OnlineAccount {
            accountUri: "http://old"
        }
    }

    function test_contact_onlineaccount_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithOnlineAccount1);
        contactWithOnlineAccount1.onlineAccount.accountUri = "http://new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithOnlineAccount2
        OnlineAccount {
            accountUri: "http://old"
        }
    }

    function test_contact_onlineaccount_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithOnlineAccount2);
        contactWithOnlineAccount2.onlineAccount.accountUri = contactWithOnlineAccount2.onlineAccount.accountUri;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithOrganization1
        Organization {
            name: "old"
        }
    }

    function test_contact_organization_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithOrganization1);
        contactWithOrganization1.organization.name = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithOrganization2
        Organization {
            name: "old"
        }
    }

    function test_contact_organization_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithOrganization2);
        contactWithOrganization2.organization.name = contactWithOrganization2.organization.name;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithPhoneNumber1
        PhoneNumber {
            number: "1234"
        }
    }

    function test_contact_phonenumber_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithPhoneNumber1);
        contactWithPhoneNumber1.phoneNumber.number = "5678";
        verifySignalReceived();
    }

    Contact {
        id: contactWithPhoneNumber2
        PhoneNumber {
            number: "1234"
        }
    }

    function test_contact_phonenumber_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithPhoneNumber2);
        contactWithPhoneNumber2.phoneNumber.number = contactWithPhoneNumber2.phoneNumber.number;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithPresence1
        Presence {
            state: Presence.Unknown
        }
    }

    function test_contact_presence_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithPresence1);
        contactWithPresence1.presence.state = Presence.Available;
        verifySignalReceived();
    }

    Contact {
        id: contactWithPresence2
        Presence {
            state: Presence.Unknown
        }
    }

    function test_contact_presence_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithPresence2);
        contactWithPresence2.presence.state = contactWithPresence2.presence.state;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithRingtone1
        Ringtone {
            audioRingtoneUrl: "http://old"
        }
    }

    function test_contact_ringtone_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithRingtone1);
        contactWithRingtone1.ringtone.audioRingtoneUrl = "http://new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithRingtone2
        Ringtone {
            audioRingtoneUrl: "http://old"
        }
    }

    function test_contact_ringtone_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithRingtone2);
        contactWithRingtone2.ringtone.audioRingtoneUrl = contactWithRingtone2.ringtone.audioRingtoneUrl;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithSyncTarget1
        SyncTarget {
            syncTarget: "old"
        }
    }

    function test_contact_synctarget_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithSyncTarget1);
        contactWithSyncTarget1.syncTarget.syncTarget = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithSyncTarget2
        SyncTarget {
            syncTarget: "old"
        }
    }

    function test_contact_synctarget_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithSyncTarget2);
        contactWithSyncTarget2.syncTarget.syncTarget = contactWithSyncTarget2.syncTarget.syncTarget;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithTag1
        Tag {
            tag: "old"
        }
    }

    function test_contact_tag_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithTag1);
        contactWithTag1.tag.tag = "new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithTag2
        Tag {
            tag: "old"
        }
    }

    function test_contact_tag_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithTag2);
        contactWithTag2.tag.tag = contactWithTag2.tag.tag;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithTimestamp1
        Timestamp {
            created: new Date(2012, 1, 1)
        }
    }

    function test_contact_timestamp_change_emits_signal() {
        listenToSignalFromObject("contactChanged", contactWithTimestamp1);
        contactWithTimestamp1.timestamp.created = new Date(2012, 1, 2);
        verifySignalReceived();
    }

    Contact {
        id: contactWithTimestamp2
        Timestamp {
            created: new Date(2012, 1, 1)
        }
    }

    function test_contact_timestamp_emits_no_signal_when_value_does_not_change() {
        listenToSignalFromObject("contactChanged", contactWithTimestamp2);
        contactWithTimestamp2.timestamp.created = contactWithTimestamp2.timestamp.created;
        verifyNoSignalReceived();
    }

    Contact {
        id: contactWithUrl1
        Url {
            url: "http://old"
        }
    }

    function test_contact_url_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contactWithUrl1);
        contactWithUrl1.url.url = "http://new";
        verifySignalReceived();
    }

    Contact {
        id: contactWithUrl2
        Url {
            url: "http://old"
        }
    }

    function test_contact_url_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contactWithUrl2);
        contactWithUrl2.url.url = contactWithUrl2.url.url;
        verifyNoSignalReceived();
    }

    property SignalSpy spy

    function init() {
        spy = Qt.createQmlObject("import QtTest 1.0;" +
                                 "SignalSpy {}",
                                 contactSignalsTests);
    }

    function listenToSignalFromObject(signalName, object) {
        spy.target = object;
        spy.signalName = signalName;
        spy.clear();
    }

    function verifySignalReceived() {
        spy.wait();
    }

    function verifyNoSignalReceived() {
        verify(spy.count == 0, "no signal was received");
    }
}
