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
    name: "ContactsRemoveContactsE2ETests"
    id: contactsRemoveContactsE2ETests

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    property Contact firstContact
    property Contact secondContact

    function init() {
        initTestForModel(model);
        emptyContacts(model);

        firstContact = createEmptyContact();
        model.saveContact(firstContact);
        waitForContactsChanged();

        secondContact = createEmptyContact();
        model.saveContact(secondContact);
        waitForContactsChanged();
    }

    function cleanup() {
        emptyContacts(model);
    }

    // Tests

    function test_removeEmptyList() {
        model.removeContacts([]);
        wait(500);

        compare(model.contacts.length, 2, "contacts.length");
    }

    function test_removeInvalidId() {
        model.removeContacts(["invalid"]);
        wait(500);

        compare(model.contacts.length, 2, "contacts.length");
    }

    function test_removeOneContact() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;

        model.removeContacts([id1]);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "contacts.length");
        compare(model.contacts[0].contactId, id2, "id of the second contact");
    }

    function test_removeMultipleContacts() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;

        model.removeContacts([id1, id2]);
        waitForContactsChanged();
        if (model.contacts.length > 0)
            waitForContactsChanged();


        compare(model.contacts.length, 0, "contacts is empty");
    }

    function test_saveRemovedContactFails() {
        var id1 = model.contacts[0].contactId;
        var contactToFirstRemoveAndThenSave = model.contacts[0];

        model.removeContacts([id1]);
        waitForContactsChanged();

        var errorSpy = initTestForTargetListeningToSignal(model, "errorChanged");
        model.saveContact(contactToFirstRemoveAndThenSave);
        waitForTargetSignal(errorSpy);

        compare(model.error, "DoesNotExist", "model.error");
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

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Helpers

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }
}
