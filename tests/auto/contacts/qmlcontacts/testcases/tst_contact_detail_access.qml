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
    name: "ContactDetailAccessTests"
    id: contactDetailAccessTests

    TestCase {
        name: "ContactDetailAccessTests::ContactWithoutDetails"

        Contact {
            id: contact10
        }

        function test_detailReturnsUndefined() {
            verifyIsUndefined(contact10.detail(ContactDetail.PhoneNumber));
        }

        function test_detailsReturnsEmptyList() {
            compare(contact10.details(ContactDetail.PhoneNumber).length, 0);
        }
    }

    TestCase {
        name: "ContactDetailAccessTests::ContactWithOneDetail"

        Contact {
            id: contact20
            PhoneNumber {
                id: contact20PhoneNumber
                number: "99999999"
            }
        }

        function test_detailReturnsOneDetail() {
            var detail = contact20.detail(ContactDetail.PhoneNumber);
            compare(detail.number, contact20PhoneNumber.number);
        }

        function test_detailsReturnsListOfLengthOne() {
            var details = contact20.details(ContactDetail.PhoneNumber);
            compare(details.length, 1);
        }

        function test_detailsReturnsListContainingTheDetail() {
            var details = contact20.details(ContactDetail.PhoneNumber);
            compare(details[0].number, contact20PhoneNumber.number);
        }
    }

    TestCase {
        name: "ContactDetailAccessTests::ContactWithDetailsOfDifferentType"

        Contact {
            id: contact30
            Name {
                id: contact30Name
                firstName: "A"
            }

            PhoneNumber {
                id: contact30PhoneNumber
                number: "99999999"
            }
        }

        function test_detailReturnsOneDetail() {
            var detail = contact30.detail(ContactDetail.Name);
            compare(detail.firstName, contact30Name.firstName);
        }

        function test_detailReturnsTheOtherDetail() {
            var detail = contact30.detail(ContactDetail.PhoneNumber);
            compare(detail.number, contact30PhoneNumber.number);
        }

        function test_detailsReturnsListOfCorrentLengthForOneDetail() {
            var details = contact30.details(ContactDetail.Name);
            compare(details.length, 1);
        }

        function test_detailsReturnsListOfCorrentLengthForTheOtherDetail() {
            var details = contact30.details(ContactDetail.PhoneNumber);
            compare(details.length, 1);
        }

        function test_detailsReturnsListContainingTheCorrectDetailForOne() {
            var details = contact30.details(ContactDetail.Name);
            compare(details[0].firstName, contact30Name.firstName);
        }

        function test_detailsReturnsListContainingTheCorrectDetailForTheOther() {
            var details = contact30.details(ContactDetail.PhoneNumber);
            compare(details[0].number, contact30PhoneNumber.number);
        }
    }

    TestCase {
        name: "ContactDetailAccessTests::ContactWithMultipleDetails"

        Contact {
            id: contact40
            PhoneNumber {
                id: contact40PhoneNumber1
                number: "11111111"
            }
            PhoneNumber {
                id: contact40PhoneNumber2
                number: "22222222"
            }
        }

        function test_detailReturnsTheFirstDetail() {
            var detail = contact40.detail(ContactDetail.PhoneNumber);
            compare(detail.number, contact40PhoneNumber1.number);
        }

        function test_detailsReturnsListOfCorrentLength() {
            var details = contact40.details(ContactDetail.PhoneNumber);
            compare(details.length, 2);
        }

        function test_detailsReturnsListContainingTheFirstDetail() {
            var details = contact40.details(ContactDetail.PhoneNumber);
            compare(details[0].number, contact40PhoneNumber1.number);
        }

        function test_detailsReturnsListContainingTheSecondDetail() {
            var details = contact40.details(ContactDetail.PhoneNumber);
            compare(details[1].number, contact40PhoneNumber2.number);
        }
    }

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + "is undefined");
    }
}
