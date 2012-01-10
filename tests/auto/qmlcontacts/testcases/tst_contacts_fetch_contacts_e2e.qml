/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
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
    name: "ContactsFetchContactsE2ETests"

    ContactModel {
        id: model
        autoUpdate: true
    }

    Contact {
        id: contact1
    }

    Contact {
        id: contact2
    }

    Contact {
        id: contact3
    }

    // Tests

    function test_fetchOnlySomeIdsLeavesContactsUnchanged() {
        var id = model.contacts[0].contactId;

        model.fetchContacts([id]);
        waitForContactsChanged();

        compare(model.contacts.length, 3);
    }

    function test_fetchOnlySomeIdsLeavesOriginalContactStillValid() {
        var contact = model.contacts[0];
        var id = contact.contactId;

        model.fetchContacts([id]);
        waitForContactsChanged();

        verify(contact, "contact is defined");
        verify(contact.contactId, "contact id is defined");
    }

    function test_fetchOnlySomeIdsLeavesOriginalContactInTheModel() {
        var contact = model.contacts[0];
        var id = contact.contactId;

        model.fetchContacts([id]);
        waitForContactsChanged();

        verify(contact === model.contacts[0], "still contains the contact");
    }

    function test_fetchInvalidIdDoesNotChangeContacts() {
        var contact1 = model.contacts[0];
        var contact2 = model.contacts[1];
        var contact3 = model.contacts[2];

        model.fetchContacts(["invalid id"]);
        wait(500); // signal is not necessarily emitted

        compare(model.contacts.length, 3);
        compare(model.contacts[0], contact1, "contact1");
        compare(model.contacts[1], contact2, "contact2");
        compare(model.contacts[2], contact3, "contact3");
    }

    function test_fetchEmptyWillFetchAllContacts() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;
        var id3 = model.contacts[2].contactId;

        model.fetchContacts([]);
        waitForContactsChanged();

        compare(model.contacts.length, 3);
        compare(model.contacts[0].contactId, id1, "id1");
        compare(model.contacts[1].contactId, id2, "id2");
        compare(model.contacts[2].contactId, id3, "id3");
    }

    function test_fetchEmptyLeavesOriginalContactStillValid() {
        var contact = model.contacts[0];

        model.fetchContacts([]);
        waitForContactsChanged();

        verifyIsDefined(contact);
        verifyIsDefined(contact.contactId);
    }

    function test_fetchEmptyLeavesOriginalContactInTheModel() {
        var contact = model.contacts[0];

        model.fetchContacts([]);
        waitForContactsChanged();

        verify(contact === model.contacts[0], "still contains the contact")
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitForContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);

        addTestContactsToModel();
    }

    function addTestContactsToModel() {
        model.saveContact(contact1);
        waitForContactsChanged();
        model.saveContact(contact2);
        waitForContactsChanged();
        model.saveContact(contact3);
        waitForContactsChanged();
    }

    function init() {
        initTestForModel(model);
    }

    function cleanupTestCase() {
        initTestForModel(model);
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Helpers

    function verifyIsDefined(object) {
        verify(object, "Object " + object + " is defined");
    }

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }
}
