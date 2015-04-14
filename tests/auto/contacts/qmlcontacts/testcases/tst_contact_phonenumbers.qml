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
    name: "ContactsPhoneNumbersTests"

    TestCase {
        name: "ContactsPhoneNumbersTests::NoPhoneNumber"

        Contact {
            id: contactWithoutPhoneNumber
        }

        Contact {
            id: anotherContactWithoutPhoneNumber
        }

        Contact {
            id: yetAnotherContactWithoutPhoneNumber
        }

        function test_phoneNumberOfContactWithoutPhoneNumberIsDefined() {
            verifyIsDefined(contactWithoutPhoneNumber.phoneNumber);
        }

        function test_phoneNumberOfContactWithoutPhoneNumberIsEmpty() {
            verifyIsUndefined(contactWithoutPhoneNumber.phoneNumber.number);
            compare(contactWithoutPhoneNumber.phoneNumber.subTypes.length, 0);
        }

        function test_phoneNumbersCountOfAnotherContactWithoutPhoneNumberIsZero() {
            // we do not access the phoneNumber property of the contact,
            // so in this case the expected length is 0
            compare(anotherContactWithoutPhoneNumber.phoneNumbers.length, 0);
        }

        function test_phoneNumbersCountOfYetAnotherContactWithoutPhoneNumberIsOne() {
            // we first access the phoneNumber property of the contact,
            // so in this case the expected length is 1
            verifyIsDefined(yetAnotherContactWithoutPhoneNumber.phoneNumber);
            compare(yetAnotherContactWithoutPhoneNumber.phoneNumbers.length, 1);
        }
    }

    TestCase {
        name: "ContactsPhoneNumbersTests::OnePhoneNumber"

        Contact {
            id: contactWithOnePhoneNumber

            PhoneNumber {
                number: "1111111111"
                subTypes:[PhoneNumber.Mobile]
            }
        }

        function test_accessPhoneNumberThroughPhoneNumber() {
            compare(contactWithOnePhoneNumber.phoneNumber.number, "1111111111");
        }

        function test_phoneNumberCount() {
            compare(contactWithOnePhoneNumber.phoneNumbers.length, 1);
        }

        function test_accessPhoneNumberThroughPhoneNumbers() {
            compare(contactWithOnePhoneNumber.phoneNumbers[0].number, "1111111111");
        }

        function test_accessPhoneNumberThroughDetails() {
            compare(contactWithOnePhoneNumber.details(
                        ContactDetail.PhoneNumber)[0].number, "1111111111");
        }
    }

    TestCase {
        name: "ContactsPhoneNumbersTests::MultiplePhoneNumbers"

        Contact {
            id: contactWithPhoneNumbers

            PhoneNumber {
                number: "1111111111"
                subTypes:[PhoneNumber.Mobile]
            }

            PhoneNumber {
                number: "2222222222"
                subTypes:[PhoneNumber.Fax]
            }
        }

        function test_accessPhoneNumberThroughPhoneNumber() {
            compare(contactWithPhoneNumbers.phoneNumber.number, "1111111111");
        }

        function test_phoneNumberCount() {
            compare(contactWithPhoneNumbers.phoneNumbers.length, 2);
        }

        function test_accessPhoneNumberThroughPhoneNumbers() {
            compare(contactWithPhoneNumbers.phoneNumbers[0].number, "1111111111");
            compare(contactWithPhoneNumbers.phoneNumbers[1].number, "2222222222");
        }

        function test_accessPhoneNumberThroughDetails() {
            compare(contactWithPhoneNumbers.details(
                        ContactDetail.PhoneNumber)[0].number, "1111111111");
        }
    }

    TestCase {
        name: "ContactsPhoneNumbersTests::DynamicallyAdded::MultiplePhoneNumbers"
        Contact {
            id: contact1
        }

        PhoneNumber {
            id: phoneNumber1
            number: "1111111111"
            subTypes:[PhoneNumber.Mobile]
        }

        PhoneNumber {
            id: phoneNumber2
            number: "2222222222"
            subTypes:[PhoneNumber.Fax]
        }

        function init() {
            contact1.clearDetails();
            contact1.addDetail(phoneNumber1);
            contact1.addDetail(phoneNumber2);
        }

        function test_accessPhoneNumberThroughPhoneNumber() {
            compare(contact1.phoneNumber.number, "1111111111");
        }

        function test_phoneNumberCount() {
            verify(contact1.phoneNumbers);
            compare(contact1.phoneNumbers.length, 2);
        }

        function test_accessPhoneNumberThroughPhoneNumbers() {
            compare(contact1.phoneNumbers[0].number, "1111111111");
            compare(contact1.phoneNumbers[1].number, "2222222222");
        }

        function test_accessPhoneNumberThroughDetails() {
            compare(contact1.details(
                        ContactDetail.PhoneNumber)[0].number, "1111111111");
        }

        function cleanup () {
            contact1.clearDetails();
        }
    }

    TestCase {
        name: "ContactsPhoneNumbersTests::ModificationSignaling"

        Contact {
            id: contactWithPhoneNumber
            PhoneNumber {
                number: "1234"
            }
        }

        function test_changeToDetailThroughPhoneNumbersEmitsSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithPhoneNumber);
            contactWithPhoneNumber.phoneNumbers[0].number = "5678";
            verifySignalReceived();
        }

        function init() {
            spy = Qt.createQmlObject("import QtTest 1.0;" +
                                     "SignalSpy {}",
                                     this);
        }
    }

    property SignalSpy spy

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

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }

    function verifyIsDefined(object) {
        verify(object, "Object " + object + " is defined");
    }
}

