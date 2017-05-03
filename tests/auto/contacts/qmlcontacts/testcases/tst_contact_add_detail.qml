/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtPim module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

TestCase {
    name: "ContactAddDetailTests"
    id: contactAddDetailTests

    Contact {
        id: contact0
    }

    function test_contact_add_null_detail_emits_no_signal() {
        listenToSignalFromObject("contactChanged", contact0);
        contact0.addDetail(null);
        verifyNoSignalReceived();
    }

    Contact {
        id: contact1
    }

    Name {
        id: contact1Name
    }

    function test_contact_add_detail_emits_signal() {
        listenToSignalFromObject("contactChanged", contact1);
        contact1.addDetail(contact1Name);
        verifySignalReceived();
    }

    Contact {
        id: contact2
        Name {
            id: contact2Name
        }
    }

    function test_contact_add_the_same_detail_emits_no_signal() {
        listenToSignalFromObject("contactChanged", contact2);
        contact2.addDetail(contact2Name);
        verifyNoSignalReceived();
    }

    Contact {
        id: contact3
        PhoneNumber {
            id: contact3PhoneNumber1
        }
    }

    PhoneNumber {
        id: contact3PhoneNumber2
    }

    function test_contact_add_second_detail_of_the_same_type_emits_signal() {
        listenToSignalFromObject("contactChanged", contact3);
        contact3.addDetail(contact3PhoneNumber2);
        verifySignalReceived();
    }

    Contact {
        id: contact4
        Name {
            id: contact4Name
        }
    }

    PhoneNumber {
        id: contact4PhoneNumber
    }

    function test_contact_add_second_detail_of_the_different_type_emits_signal() {
        listenToSignalFromObject("contactChanged", contact4);
        contact4.addDetail(contact4PhoneNumber);
        verifySignalReceived();
    }

    Contact {
        id: contact5
    }

    Name {
        id: contact5Name
        firstName: "old"
    }

    function test_contact_modify_added_detail_emits_signal() {
        contact5.addDetail(contact5Name);
        var detail = contact5.detail(ContactDetail.Name);
        listenToSignalFromObject("contactChanged", contact5);
        detail.firstName = "new";
        verifySignalReceived();
    }

    Contact {
        id: contact6
    }

    Name {
        id: contact6Name
        firstName: "old"
    }

    function test_contact_modify_detail_already_added_modifies_contact() {
        contact6.addDetail(contact6Name);
        contact6Name.firstName = "new";
        var detail = contact6.detail(ContactDetail.Name);
        expectFail("", "implementation takes a copy of the detail");
        compare(detail.firstName, "new");
    }

    Contact {
        id: contact7
    }

    Name {
        id: contact7Name
        firstName: "old"
    }

    function test_contact_modify_detail_already_added_emits_signal() {
        contact7.addDetail(contact7Name);
        listenToSignalFromObject("contactChanged", contact7);
        contact7Name.firstName = "new";
        expectFail("", "implementation takes a copy of the detail");
        verifySignalReceived();
    }

    Contact {
        id: contact8_1
    }

    Contact {
        id: contact8_2
    }

    Name {
        id: contact8Name
        firstName: "old"
    }

    function test_contact_modify_detail_added_to_two_contacts_modifies_both() {
        contact8_1.addDetail(contact8Name);
        contact8_2.addDetail(contact8Name);

        var detail = contact8_1.detail(ContactDetail.Name);
        detail.firstName = "new";

        detail = contact8_2.detail(ContactDetail.Name);
        expectFail("", "does not propagate to the second contact");
        compare(detail.firstName, "new");
        compare(contact8_2.name.firstName, "new");
    }

    property SignalSpy spy

    function init() {
        spy = Qt.createQmlObject("import QtTest 1.0;" +
                                 "SignalSpy {}",
                                 contactAddDetailTests);
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
