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

ContactsSavingTestCase {
    name: "ContactsSaveContactE2ETests"
    id: contactsSaveContactE2ETests

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    // Tests

    Contact {
        id: singleContact
    }

    function test_saveContact() {
        model.saveContact(singleContact);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "contacts.length");
    }

    Contact {
        id: contactWithDetail
        Name {
            firstName: "Joe"
        }
    }

    function test_saveContactSavesDetails() {
        model.saveContact(contactWithDetail);
        waitForContactsChanged();

        compare(model.contacts[0].name.firstName, contactWithDetail.name.firstName, "contact name");
    }

    function test_saveDynamicallyCreatedContact() {
        var contact = Qt.createQmlObject(
                    'import QtContacts 5.0;' +
                    'Contact {' +
                    '  Name {' +
                    '    firstName: "A"' +
                    '  }' +
                    '}',
                    contactsSaveContactE2ETests);
        model.saveContact(contact);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "contacts.length");
        compare(model.contacts[0].name.firstName, contact.name.firstName, 'contact name');
    }

    Contact {
        id: firstOfMultipleContacts
        Name {
            firstName: "A"
        }
    }

    Contact {
        id: secondOfMultipleContacts
        Name {
            firstName: "B"
        }
    }

    function test_saveMultipleContacts() {
        model.saveContact(firstOfMultipleContacts);
        waitForContactsChanged();

        model.saveContact(secondOfMultipleContacts);
        waitForContactsChanged();

        compare(model.contacts.length, 2, "contacts.length");
        compare(model.contacts[0].name.firstName, firstOfMultipleContacts.name.firstName, 'contacts[0].name.firstName');
        compare(model.contacts[1].name.firstName, secondOfMultipleContacts.name.firstName, 'contacts[1].name.firstName');
    }

    function test_saveTheSameContactMultipleTimes() {
        var contactToSaveMultipleTimes = createEmptyContact();

        model.saveContact(contactToSaveMultipleTimes);
        waitForContactsChanged();

        model.saveContact(contactToSaveMultipleTimes);
        waitUntilContactsChanged();

        // Saving the same contact multiple times yields an insertion and then an update
        compare(model.contacts.length, 1, "contacts.length");
    }

    function test_saveTheSameContactMultipleTimesWithoutWaitingForTheModel() {
        var contactToSaveMultipleTimes = createEmptyContact();

        model.saveContact(contactToSaveMultipleTimes);

        model.saveContact(contactToSaveMultipleTimes);

        waitForContactsChanged();

        // Saving the same contact multiple times yields an insertion and then an update
        compare(model.contacts.length, 1, "contacts.length");
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
    }

    function init() {
        initTestForModel(model);
    }

    function cleanup() {
        emptyContacts(model);
    }

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Helpers

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }
}
