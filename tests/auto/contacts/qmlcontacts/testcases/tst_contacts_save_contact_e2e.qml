/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtPim module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
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

    Contact {
        id: contactToSaveMultipleTimes
    }

    function test_saveTheSameContactMultipleTimes() {
        model.saveContact(contactToSaveMultipleTimes);
        waitForContactsChanged();

        model.saveContact(contactToSaveMultipleTimes);
        waitForContactsChanged();

        compare(model.contacts.length, 2, "contacts.length");
        verify(model.contacts[0].contactId !== model.contacts[1].contactId, "contact ids are different");
    }

    Contact {
        id: contactToSaveMultipleTimesWithoutWaitingForTheModel
    }

    function test_saveTheSameContactMultipleTimesWithoutWaitingForTheModel() {
        model.saveContact(contactToSaveMultipleTimes);

        model.saveContact(contactToSaveMultipleTimes);

        waitForContactsChanged();
        if (model.contacts.length < 2)
            waitForContactsChanged();

        compare(model.contacts.length, 2, "contacts.length");
        verify(model.contacts[0].contactId !== model.contacts[1].contactId, "contact ids are different");
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitForContactsChanged();
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
