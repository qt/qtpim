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
    name: "ContactsExtendedDetailsTests"

    TestCase {
        name: "ContactsExtendedDetailsTests::NoExtendedDetail"

        Contact {
            id: contactWithoutExtendedDetail
        }

        Contact {
            id: anotherContactWithoutExtendedDetail
        }

        Contact {
            id: yetAnotherContactWithoutExtendedDetail
        }

        function test_ExtendedDetailOfContactWithoutExtendedDetailIsDefined() {
            verifyIsDefined(contactWithoutExtendedDetail.extendedDetail);
        }

        function test_ExtendedDetailNameOfContactWithoutExtendedDetailIsEmpty() {
            verifyIsUndefined(contactWithoutExtendedDetail.extendedDetail.name);
        }

        function test_ExtendedDetailDataOfContactWithoutExtendedDetailIsEmpty() {
            verifyIsUndefined(contactWithoutExtendedDetail.extendedDetail.data);
        }

        function test_ExtendedDetailsCountOfAnotherContactWithoutExtendedDetailIsZero() {
            // we do not access the ExtendedDetail property of the contact,
            // so in this case the expected length is 0
            compare(anotherContactWithoutExtendedDetail.extendedDetails.length, 0);
        }

        function test_ExtendedDetailsCountOfYetAnotherContactWithoutExtendedDetailIsOne() {
            // we first access the ExtendedDetail property of the contact,
            // so in this case the expected length is 1
            verifyIsDefined(yetAnotherContactWithoutExtendedDetail.extendedDetail);
            compare(yetAnotherContactWithoutExtendedDetail.extendedDetails.length, 1);
        }
    }

    TestCase {
        name: "ContactsExtendedDetailsTests::OneExtendedDetail"

        Contact {
            id: contactWithOneExtendedDetail

            ExtendedDetail {
                name: "First"
                data: 1
            }
        }

        function test_accessExtendedDetailThroughExtendedDetail() {
            compare(contactWithOneExtendedDetail.extendedDetail.name, "First", "name");
            compare(contactWithOneExtendedDetail.extendedDetail.data, 1, "data");
        }

        function test_ExtendedDetailsCount() {
            compare(contactWithOneExtendedDetail.extendedDetails.length, 1, "extendedDetails length");
        }

        function test_accessExtendedDetailThroughExtendedDetails() {
            compare(contactWithOneExtendedDetail.extendedDetails[0].name, "First", "name");
            compare(contactWithOneExtendedDetail.extendedDetails[0].data, 1, "data");
        }

        function test_accessExtendedDetailThroughDetails() {
            compare(contactWithOneExtendedDetail.details(
                        ContactDetail.ExtendedDetail)[0].name, "First", "name");
            compare(contactWithOneExtendedDetail.details(
                        ContactDetail.ExtendedDetail)[0].data, 1, "data");

        }
    }

    TestCase {
        name: "ContactsExtendedDetailsTests::MultipleExtendedDetails"

        Contact {
            id: contactWithExtendedDetails
            ExtendedDetail {
                name: "First"
                data: 1
            }
            ExtendedDetail {
                name: "Second"
                data: 2
            }
        }

        function test_accessExtendedDetailThroughExtendedDetail() {
            compare(contactWithExtendedDetails.extendedDetail.name, "First", "name");
            compare(contactWithExtendedDetails.extendedDetail.data, 1, "name");
        }

        function test_ExtendedDetailCount() {
            compare(contactWithExtendedDetails.extendedDetails.length, 2, "length");
        }

        function test_accessExtendedDetailThroughExtendedDetails() {
            compare(contactWithExtendedDetails.extendedDetails[0].name, "First", "[0].name");
            compare(contactWithExtendedDetails.extendedDetails[0].data, 1, "[0].data");
            compare(contactWithExtendedDetails.extendedDetails[1].name, "Second", "[1].name");
            compare(contactWithExtendedDetails.extendedDetails[1].data, 2, "[1].data");
        }

        function test_accessExtendedDetailThroughDetails() {
            compare(contactWithExtendedDetails.details(
                        ContactDetail.ExtendedDetail)[0].name, "First", "[0].name");
            compare(contactWithExtendedDetails.details(
                        ContactDetail.ExtendedDetail)[0].data, 1, "[0].data");
            compare(contactWithExtendedDetails.details(
                        ContactDetail.ExtendedDetail)[1].name, "Second", "[1].name");
            compare(contactWithExtendedDetails.details(
                        ContactDetail.ExtendedDetail)[1].data, 2, "[1].data");
        }
    }

    TestCase {
        name: "ContactsExtendedDetailsTests::DynamicallyAdded::MultipleExtendedDetails"

        Contact {
            id: contact1
        }

        ExtendedDetail {
            id: extendedDetail1
            name: "First"
            data: 1
        }

        ExtendedDetail {
            id: extendedDetail2
            name: "Second"
            data: 2
        }

        function init() {
            contact1.clearDetails();
            contact1.addDetail(extendedDetail1);
            contact1.addDetail(extendedDetail2);
        }

        function test_accessExtendedDetailThroughExtendedDetail() {
            compare(contact1.extendedDetail.name, "First", "name");
            compare(contact1.extendedDetail.data, 1, "data");
        }

        function test_ExtendedDetailCount() {
            verify(contact1.extendedDetails);
            compare(contact1.extendedDetails.length, 2);
        }

        function test_accessExtendedDetailThroughExtendedDetails() {
            compare(contact1.extendedDetails[0].name, "First", "[0].name");
            compare(contact1.extendedDetails[0].data, 1, "[0].data");
            compare(contact1.extendedDetails[1].name, "Second", "[1].name");
            compare(contact1.extendedDetails[1].data, 2, "[1].data");
        }

        function test_accessExtendedDetailThroughDetails() {
            compare(contact1.details(
                        ContactDetail.ExtendedDetail)[0].name, "First", "[0].name");
            compare(contact1.details(
                        ContactDetail.ExtendedDetail)[0].data, 1, "[0].data");
            compare(contact1.details(
                        ContactDetail.ExtendedDetail)[1].name, "Second", "[1].name");
            compare(contact1.details(
                        ContactDetail.ExtendedDetail)[1].data, 2, "[1].data");
        }

        function cleanup () {
            contact1.clearDetails();
        }
    }

    TestCase {
        name: "ContactsExtendedDetailsTests::ModificationSignaling"

        Contact {
            id: contactWithExtendedDetail
            ExtendedDetail {
                name: "First"
                data: 1
            }
        }

        function test_changeToNameThroughExtendedDetailsEmitsSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithExtendedDetail);
            contactWithExtendedDetail.extendedDetails[0].name = "FirstChanged";
            verifySignalReceived();
        }

        function test_changeToDataThroughExtendedDetailsEmitsSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithExtendedDetail);
            contactWithExtendedDetail.extendedDetails[0].data = 5;
            verifySignalReceived();
        }

        Contact {
            id: contactWithExtendedDetail2
            ExtendedDetail {
                name: "First"
                data: 1
            }
        }

        function test_noChangeToNameThroughExtendedDetailsEmitsNoSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithExtendedDetail2);
            contactWithExtendedDetail2.extendedDetails[0].name = "First";
            verifyNoSignalReceived();
        }

        function test_noChangeToDataThroughExtendedDetailsEmitsNoSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithExtendedDetail2);
            contactWithExtendedDetail2.extendedDetails[0].data = 1;
            verifyNoSignalReceived();
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
