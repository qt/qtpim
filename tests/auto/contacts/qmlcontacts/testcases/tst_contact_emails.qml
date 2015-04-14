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
    name: "ContactsEmailsTests"

    TestCase {
        name: "ContactsEmailsTests::NoEmail"

        Contact {
            id: contactWithoutEmail
        }

        Contact {
            id: anotherContactWithoutEmail
        }

        Contact {
            id: yetAnotherContactWithoutEmail
        }

        function test_emailOfContactWithoutEmailIsDefined() {
            verifyIsDefined(contactWithoutEmail.email);
        }

        function test_emailOfContactWithoutEmailIsEmpty() {
            verifyIsUndefined(contactWithoutEmail.email.emailAddress);
        }

        function test_emailsCountOfAnotherContactWithoutEmailIsZero() {
            // we do not access the email property of the contact,
            // so in this case the expected length is 0
            compare(anotherContactWithoutEmail.emails.length, 0);
        }

        function test_emailsCountOfYetAnotherContactWithoutEmailIsOne() {
            // we first access the email property of the contact,
            // so in this case the expected length is 1
            verifyIsDefined(yetAnotherContactWithoutEmail.email);
            compare(yetAnotherContactWithoutEmail.emails.length, 1);
        }
    }

    TestCase {
        name: "ContactsEmailsTests::OneEmail"

        Contact {
            id: contactWithOneEmail

            EmailAddress {
                emailAddress: "a@ovi.com"
            }
        }

        function test_accessEmailThroughEmail() {
            compare(contactWithOneEmail.email.emailAddress, "a@ovi.com");
        }

        function test_emailCount() {
            compare(contactWithOneEmail.emails.length, 1);
        }

        function test_accessEmailThroughEmails() {
            compare(contactWithOneEmail.emails[0].emailAddress, "a@ovi.com");
        }

        function test_accessEmailThroughDetails() {
            compare(contactWithOneEmail.details(
                        ContactDetail.Email)[0].emailAddress, "a@ovi.com");
        }
    }

    TestCase {
        name: "ContactsEmailsTests::MultipleEmails"

        Contact {
            id: contactWithEmails

            EmailAddress {
                emailAddress: "a@ovi.com"
            }

            EmailAddress {
                emailAddress: "b@ovi.com"
            }
        }

        function test_accessEmailThroughEmail() {
            compare(contactWithEmails.email.emailAddress, "a@ovi.com");
        }

        function test_emailCount() {
            compare(contactWithEmails.emails.length, 2);
        }

        function test_accessEmailThroughEmails() {
            compare(contactWithEmails.emails[0].emailAddress, "a@ovi.com");
            compare(contactWithEmails.emails[1].emailAddress, "b@ovi.com");
        }

        function test_accessEmailThroughDetails() {
            compare(contactWithEmails.details(
                        ContactDetail.Email)[0].emailAddress, "a@ovi.com");
            compare(contactWithEmails.details(
                        ContactDetail.Email)[1].emailAddress, "b@ovi.com");
        }
    }

    TestCase {
        name: "ContactsEmailsTests::DynamicallyAdded::MultipleEmails"

        Contact {
            id: contact1
        }

        EmailAddress {
            id: emailAddress1
            emailAddress: "a@ovi.com"
        }

        EmailAddress {
            id: emailAddress2
            emailAddress: "b@ovi.com"
        }

        function init() {
            contact1.clearDetails();
            contact1.addDetail(emailAddress1);
            contact1.addDetail(emailAddress2);
        }

        function test_accessEmailThroughEmail() {
            compare(contact1.email.emailAddress, "a@ovi.com");
        }

        function test_emailCount() {
            verify(contact1.emails);
            compare(contact1.emails.length, 2);
        }

        function test_accessEmailThroughEmails() {
            compare(contact1.emails[0].emailAddress, "a@ovi.com");
            compare(contact1.emails[1].emailAddress, "b@ovi.com");
        }

        function test_accessEmailThroughDetails() {
            compare(contact1.details(
                        ContactDetail.Email)[0].emailAddress, "a@ovi.com");
            compare(contact1.details(
                        ContactDetail.Email)[1].emailAddress, "b@ovi.com");
        }

        function cleanup () {
            contact1.clearDetails();
        }
    }

    TestCase {
        name: "ContactsEmailsTests::ModificationSignaling"

        Contact {
            id: contactWithEmailAddress
            EmailAddress {
                emailAddress: "a@a"
            }
        }

        function test_changeToDetailThroughEmailsEmitsSignal()
        {
            listenToSignalFromObject("contactChanged", contactWithEmailAddress);
            contactWithEmailAddress.emails[0].emailAddress = "b@b";
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

