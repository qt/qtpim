/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtPim module of the Qt Toolkit.
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

ContactsSavingTestCase {
    name: "ContactsModelToModelNotificationE2ETests"
    id: contactsModelToModelNotificationE2ETests

    ContactModel {
        id: modifiedModel
        manager: "jsondb"
        autoUpdate:true
    }

    ContactModel {
        id: notifiedModel
        manager: "jsondb"
        autoUpdate:true
    }

    ContactsSavingTestCase {
        name: "ContactsModelToModelNotificationE2ETests::CreateContact"

        // All test functions pass if run individually.
        // When run in batch the first test function passes, others fail.
        Contact {
            id: contact
        }

        function test_createContactShouldUpdateOtherModels()
        {
            initTestForModel(notifiedModel);

            modifiedModel.saveContact(contact);

            expectFail("", "model does not receive a signal");
            waitForContactsChanged();
            compare(notifiedModel.contacts.length, 1,
                    "contacts updated in the notified model");
        }

        Contact {
            id: contactWithDetailToBeVerified
            Name {
                firstName: "contactWithDetailToBeVerified"
            }
        }

        function test_createContactPassesDetailsToOtherModels()
        {
            initTestForModel(notifiedModel);

            var spyModifiedModel = createSpyForModel(modifiedModel);
            modifiedModel.saveContact(contactWithDetailToBeVerified);
            spyModifiedModel.wait();
            compareContactArrays(modifiedModel.contacts, [contactWithDetailToBeVerified],
                                 "contacts updated in the modified model");

            // expectFail("", "model does not receive a signal");
            // This works as it is run as the first test function
            waitForContactsChanged();
            compareContactArrays(notifiedModel.contacts, [contactWithDetailToBeVerified],
                                 "contacts updated in the notified model");
        }

        Contact {
            id: firstOfMultipleContacts
        }

        Contact {
            id: secondOfMultipleContacts
        }

        function test_createMultipleContactsShouldUpdateOtherModels()
        {
            initTestForModel(notifiedModel);

            modifiedModel.saveContact(firstOfMultipleContacts);
            modifiedModel.saveContact(secondOfMultipleContacts);

            expectFail("", "model does not receive a signal");
            waitForContactsChanged();
            if (notifiedModel.length < 2) {
                // model not yet notified of both contacts
                waitForContactsChanged();
            }
            compare(notifiedModel.contacts.length, 2,
                    "contacts updated in the notified model");
        }

        function cleanup() {
            initTestForModel(modifiedModel);
            emptyContacts(modifiedModel);
        }
    }

    function compareContactArrays(actual, expected, message) {
        compare(actual.length, expected.length, message + ": length");
        for (var i = 0; i < expected.length; i++) {
            compareContacts(actual[i], expected[i], message);
        }
    }

    function compareContacts(actual, expected, message) {
        if (expected.name) {
            compare(actual.name.firstName, expected.name.firstName, message + ': name.firstName');
            compare(actual.name.lastName, expected.name.lastName, message + ': name.lastName');
        }
        if (expected.email) {
            compare(actual.email.emailAddress, expected.email.emailAddress,
                    message + ': email.emailAddress');
        }
    }

    function initTestCase() {
        initTestForModel(modifiedModel);
        emptyContacts(modifiedModel);
    }

    function cleanupTestCase() {
        initTestForModel(modifiedModel);
        emptyContacts(modifiedModel);
    }

    function createSpyForModel(model) {
        var spy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsModelToModelNotificationE2ETests);
        spy.target = model;
        spy.signalName = "contactsChanged";
        return spy;
    }
}
