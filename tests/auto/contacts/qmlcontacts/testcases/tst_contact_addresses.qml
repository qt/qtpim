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
    name: "ContactsAddressesTests"

    TestCase {
        name: "ContactsAddressesTests::NoAddress"

        Contact {
            id: contactWithoutAddress
        }

        Contact {
            id: anotherContactWithoutAddress
        }

        Contact {
            id: yetAnotherContactWithoutAddress
        }

        function test_addressOfContactWithoutAddressIsDefined() {
            verifyIsDefined(contactWithoutAddress.address);
        }

        function test_addressesCountOfAnotherContactWithoutAddressIsZero() {
            // we do not access the address property of the contact,
            // so in this case the expected length is 0
            compare(anotherContactWithoutAddress.addresses.length, 0);
        }

        function test_addressesCountOfYetAnotherContactWithoutAddressIsOne() {
            // we first access the address property of the contact,
            // so in this case the expected length is 1
            verifyIsDefined(yetAnotherContactWithoutAddress.address);
            compare(yetAnotherContactWithoutAddress.addresses.length, 1);
        }

        function test_addressOfContactWithoutAddressIsEmpty() {
            verifyIsUndefined(contactWithoutAddress.address.city);
            verifyIsUndefined(contactWithoutAddress.address.country);
            verifyIsUndefined(contactWithoutAddress.address.countryCode);
            verifyIsUndefined(contactWithoutAddress.address.county);
            verifyIsUndefined(contactWithoutAddress.address.district);
            verifyIsUndefined(contactWithoutAddress.address.postcode);
            verifyIsUndefined(contactWithoutAddress.address.state);
            verifyIsUndefined(contactWithoutAddress.address.street);
        }

        function test_addressSetStreetOfContactWithoutAddress() {
            contactWithoutAddress.address.street = "NoAddressStreet";
            compare(contactWithoutAddress.address.street, "NoAddressStreet");
        }
    }

    TestCase {
        name: "ContactsAddressesTests::OneAddress"

        Contact {
            id: contactWithOneAddress

            Address {
                street: "myAddress"
            }
        }

        function test_accessAddressThroughAddress() {
            compare(contactWithOneAddress.address.street, "myAddress");
        }

        function test_addressCount() {
            compare(contactWithOneAddress.addresses.length, 1);
        }

        function test_accessAddressThroughAddresses() {
            compare(contactWithOneAddress.addresses[0].street, "myAddress");
        }

        function test_accessAddressThroughDetails() {
            compare(contactWithOneAddress.details(
                        ContactDetail.Address)[0].street, "myAddress");
        }
    }

    TestCase {
        name: "ContactsAddressesTests::MultipleAddresses"

        Contact {
            id: contactWithAddresses

            Address {
                street: "myAddress1"
            }

            Address {
                street: "myAddress2"
            }
        }

        function test_accessAddressThroughAddress() {
            compare(contactWithAddresses.address.street, "myAddress1");
        }

        function test_addressCount() {
            compare(contactWithAddresses.addresses.length, 2);
        }

        function test_accessAddressThroughAddresses() {
            compare(contactWithAddresses.addresses[0].street, "myAddress1");
            compare(contactWithAddresses.addresses[1].street, "myAddress2");
        }

        function test_accessAddressThroughDetails() {
            compare(contactWithAddresses.details(
                        ContactDetail.Address)[0].street, "myAddress1");
            compare(contactWithAddresses.details(
                        ContactDetail.Address)[1].street, "myAddress2");
        }
    }

    TestCase {
        name: "ContactsAddressesTests::DynamicallyAdded::MultipleAddresses"

        Contact {
            id: contact1
        }

        Address {
            id: address1
            street: "myAddress1"
        }

        Address {
            id: address2
            street: "myAddress2"
        }

        function init() {
            contact1.clearDetails();
            contact1.addDetail(address1);
            contact1.addDetail(address2);
        }

        function test_accessAddressThroughAddress() {
            compare(contact1.address.street, "myAddress1");
        }

        function test_addressCount() {
            verify(contact1.addresses);
            compare(contact1.addresses.length, 2);
        }

        function test_accessAddressThroughAddresses() {
            compare(contact1.addresses[0].street, "myAddress1");
            compare(contact1.addresses[1].street, "myAddress2");
        }

        function test_accessAddressThroughDetails() {
            compare(contact1.details(
                        ContactDetail.Address)[0].street, "myAddress1");
            compare(contact1.details(
                        ContactDetail.Address)[1].street, "myAddress2");
        }

        function cleanup () {
            contact1.clearDetails();
        }
    }

    TestCase {
        name: "ContactsAddressesTests::ModificationSignaling"

        Contact {
            id: contactWithAddress
            Address {
                street: "old"
            }
        }

        function test_changeToDetailThroughAddressesEmitsSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithAddress);
            contactWithAddress.addresses[0].street = "new";
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
