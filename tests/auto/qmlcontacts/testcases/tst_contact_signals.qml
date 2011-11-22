/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
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
    name: "ContactSignalsTests"
    id: contactSignalsTests

    Contact {
        id: contact1
        Address {
            street: "old"
        }
        Anniversary {
            originalDate: "2011-10-26"
        }
        Avatar {
            imageUrl: "http://old"
        }
        Birthday {
            birthday: "2010-10-26"
        }
        EmailAddress {
            emailAddress: "a@a"
        }
        Favorite {
            favorite: false
        }
        Gender {
            gender: Gender.Male
        }
        Location {
            latitude: 0.0
        }
        GlobalPresence {
            state: Presence.Unknown
        }
        Guid {
            guid: "0000"
        }
        Name {
            firstName: "old"
        }
        Nickname {
            nickname: "old"
        }
        Note {
            note: "old"
        }
        OnlineAccount {
            accountUri: "http://old"
        }
        Organization {
            name: "old"
        }
        PhoneNumber {
            number: "1234"
        }
        Presence {
            state: Presence.Unknown
        }
        Ringtone {
            audioRingtoneUrl: "http://old"
        }
        SyncTarget {
            syncTarget: "old"
        }
        Tag {
            tag: "old"
        }
        thumbnail: "http://old"
        type: Type.Contact
        Url {
            url: "http://old"
        }
        Hobby {
            hobby: "old"
        }
        Personid {
            personid: "old"
        }
    }

    function test_contact_address_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.address.street = "new";
        verifySignalReceived();
    }

    function test_contact_address_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.address.street = contact1.address.street;
        verifyNoSignalReceived();
    }

    function test_contact_anniversary_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.anniversary.originalDate = "2011-10-27";
        verifySignalReceived();
    }

    function test_contact_anniversary_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.anniversary.originalDate = contact1.anniversary.originalDate;
        verifyNoSignalReceived();
    }

    function test_contact_avatar_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.avatar.imageUrl = "http://new";
        verifySignalReceived();
    }

    function test_contact_avatar_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.avatar.imageUrl = contact1.avatar.imageUrl;
        verifyNoSignalReceived();
    }

    function test_contact_birthday_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.birthday.birthday = "2010-10-27";
        verifySignalReceived();
    }

    function test_contact_birthday_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.birthday.birthday = contact1.birthday.birthday;
        verifyNoSignalReceived();
    }

    function test_contact_email_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.email.emailAddress = "b@b";
        verifySignalReceived();
    }

    function test_contact_email_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.email.emailAddress = contact1.email.emailAddress;
        verifyNoSignalReceived();
    }

    function test_contact_favorite_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.favorite.favorite = true;
        verifySignalReceived();
    }

    function test_contact_favorite_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.favorite.favorite = contact1.favorite.favorite;
        verifyNoSignalReceived();
    }

    function test_contact_geolocation_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.geolocation.latitude = 1.0;
        verifySignalReceived();
    }

    function test_contact_geolocation_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.geolocation.latitude = contact1.geolocation.latitude;
        verifyNoSignalReceived();
    }

    function test_contact_globalpresence_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.globalPresence.state = Presence.Available;
        verifySignalReceived();
    }

    function test_contact_globalpresence_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.globalPresence.state = contact1.globalPresence.state;
        verifyNoSignalReceived();
    }

    function test_contact_guid_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.guid.guid = "1111";
        verifySignalReceived();
    }

    function test_contact_guid_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.guid.guid = contact1.guid.guid;
        verifyNoSignalReceived();
    }

    function test_contact_hobby_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.hobby.hobby = "new";
        verifySignalReceived();
    }

    function test_contact_hobby_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.hobby.hobby = "new";
        verifyNoSignalReceived();
    }

    function test_contact_name_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.name.firstName = "new";
        verifySignalReceived();
    }

    function test_contact_name_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.name.firstName = contact1.name.firstName;
        verifyNoSignalReceived();
    }

    function test_contact_nickname_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.nickname.nickname = "new";
        verifySignalReceived();
    }

    function test_contact_nickname_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.nickname.nickname = contact1.nickname.nickname;
        verifyNoSignalReceived();
    }

    function test_contact_note_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.note.note = "new";
        verifySignalReceived();
    }

    function test_contact_note_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.note.note = contact1.note.note;
        verifyNoSignalReceived();
    }

    function test_contact_onlineaccount_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.onlineAccount.accountUri = "http://new";
        verifySignalReceived();
    }

    function test_contact_onlineaccount_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.onlineAccount.accountUri = contact1.onlineAccount.accountUri;
        verifyNoSignalReceived();
    }

    function test_contact_organization_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.organization.name = "new";
        verifySignalReceived();
    }

    function test_contact_organization_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.organization.name = contact1.organization.name;
        verifyNoSignalReceived();
    }

    function test_contact_personid_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.personid.personid = "new";
        verifySignalReceived();
    }

    function test_contact_personid_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.personid.personid = contact1.personid.personid;
        verifyNoSignalReceived();
    }

    function test_contact_phonenumber_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.phoneNumber.number = "5678";
        verifySignalReceived();
    }

    function test_contact_phonenumber_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.phoneNumber.number = contact1.phoneNumber.number;
        verifyNoSignalReceived();
    }

    function test_contact_presence_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.presence.state = Presence.Available;
        verifySignalReceived();
    }

    function test_contact_presence_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.presence.state = contact1.presence.state;
        verifyNoSignalReceived();
    }

    function test_contact_ringtone_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.ringtone.audioRingtoneUrl = "http://new";
        verifySignalReceived();
    }

    function test_contact_ringtone_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.ringtone.audioRingtoneUrl = contact1.ringtone.audioRingtoneUrl;
        verifyNoSignalReceived();
    }

    function test_contact_synctarget_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.syncTarget.syncTarget = "new";
        verifySignalReceived();
    }

    function test_contact_synctarget_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.syncTarget.syncTarget = contact1.syncTarget.syncTarget;
        verifyNoSignalReceived();
    }

    function test_contact_tag_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.tag.tag = "new";
        verifySignalReceived();
    }

    function test_contact_tag_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.tag.tag = "new";
        verifyNoSignalReceived();
    }

    function test_contact_thumbnail_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.thumbnail = "http://new";
        verifySignalReceived();
    }

    function test_contact_thumbnail_emits_signal_also_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.thumbnail = contact1.thumbnail;
        verifySignalReceived();
    }

    function test_contact_type_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.type = Type.Group;
        verifySignalReceived();
    }

    function test_contact_type_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.type = contact1.type;
        verifyNoSignalReceived();
    }

    function test_contact_url_change_emits_signal()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.url.url = "http://new";
        verifySignalReceived();
    }

    function test_contact_url_emits_no_signal_when_value_does_not_change()
    {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.url.url = contact1.url.url;
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
