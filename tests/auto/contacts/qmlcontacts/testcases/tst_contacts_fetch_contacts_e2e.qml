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
    name: "ContactsFetchContactsE2ETests"
    id: contactsFetchContactsE2ETests

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true

        onContactsFetched: {
            lastTransactionId = requestId;
            lastContactsFetched = fetchedContacts;
        }
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

    property string nonExistingId

    property int lastTransactionId
    property list<Contact> lastContactsFetched

    // Tests

    function test_fetchExistingContact() {
        var id = model.contacts[0].contactId;

        var trid = model.fetchContacts([id]);
        waitForContactsFetched();

        compare(lastTransactionId, trid, "transaction id");
        compare(lastContactsFetched.length, 1, "contacts length");
        compare(lastContactsFetched[0].contactId, model.contacts[0].contactId, "fetched contact id");
    }

    function test_fetchMultipleExistingContacts() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;

        model.fetchContacts([id1, id2]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 2, "contacts length");
        compare(lastContactsFetched[0].contactId, id1, "fetched contact id 1");
        compare(lastContactsFetched[1].contactId, id2, "fetched contact id 2");
    }

    function test_fetchNonExistingContact() {
        var trid = model.fetchContacts([nonExistingId]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 0, "contacts length");
    }

    function test_fetchBothExistingAndNonExistingContacts() {
        var id1 = model.contacts[0].contactId;
        var id2 = nonExistingId

        model.fetchContacts([id1, id2]);
        waitForContactsFetched();

        expectFail("", "implementation returns an empty list of contacts")
        compare(lastContactsFetched.length, 1, "contacts length");
        compare(lastContactsFetched[0].contactId, id1, "fetched contact id 1");
    }

    function test_fetchDuplicateContactIds() {
        var id = model.contacts[0].contactId;
        var originalLength = model.contacts.length;

        var trid = model.fetchContacts([id,id]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 2, "contacts length");
        compare(lastContactsFetched[0].contactId, model.contacts[0].contactId, "fetched contact id 0");
        compare(lastContactsFetched[1].contactId, model.contacts[0].contactId, "fetched contact id 1");
        compare(model.contacts.length, originalLength, "model length");
    }

    function test_fetchEmptyListOfContactIds() {
        listenToContactsFetched();
        var trid = model.fetchContacts([]);
        verifyNoContactsFetchedReceived();
        compare(trid, -1, "transaction id");
    }

    Contact {
        id: contactWhichHasNullId
    }

    function test_fetchNullContactId() {
        var id = contactWhichHasNullId.contactId;

        var trid = model.fetchContacts([id]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 0, "contacts length");
    }

    function test_fetchInvalidContactId() {
        var id = "invalidContactId";

        var trid = model.fetchContacts([id]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 0, "contacts length");
    }

    IdFilter {
        id: filter
        ids: []
    }

    function test_filterAndFetchContactMatchingTheFilter() {
        var contact = model.contacts[0];
        var id = contact.contactId;

        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();

        model.fetchContacts([id]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 1, "contacts length");
        compare(lastContactsFetched[0].contactId, id, "fetched contact id");
    }

    function test_filterAndFetchContactWhichDoesNotMatchTheFilter() {
        var id = model.contacts[0].contactId;
        var idOfAnotherContact = model.contacts[1].contactId;

        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();

        model.fetchContacts([idOfAnotherContact]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 1, "contacts length");
        compare(lastContactsFetched[0].contactId, idOfAnotherContact, "fetched contact id");
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);

        addTestContactsToModel();
    }

    Contact {
        id: contactFirstSavedAndThenRemoved
        Name {
            firstName: "contactFirstSavedAndThenRemoved"
        }
    }

    function addTestContactsToModel() {
        model.saveContact(contact1);
        waitForContactsChanged();
        model.saveContact(contact2);
        waitForContactsChanged();
        model.saveContact(contact3);
        waitForContactsChanged();

        model.saveContact(contactFirstSavedAndThenRemoved);
        waitForContactsChanged();
        for (var i = 0; i < model.contacts.length; i++) {
            if (model.contacts[i].name.firstName == contactFirstSavedAndThenRemoved.name.firstName) {
                nonExistingId = model.contacts[i].contactId;
                break;
            }
        }
        model.removeContact(nonExistingId);
        waitForContactsChanged();
    }

    function init() {
        initTestForModel(model);
        initContactsFetchSpy();
    }

    function cleanup() {
        clearFilter();
    }

    function cleanupTestCase() {
        initTestForModel(model);
        clearFilter();
        emptyContacts(model);
        finishTestForModel(model);
    }

    function clearFilter() {
        if (model.filter) {
            model.filter = null;
            waitForContactsChanged();
        }
    }

    property SignalSpy contactsFetchedSpy

    function initContactsFetchSpy() {
        contactsFetchedSpy = initTestForTargetListeningToSignal(model, "contactsFetched");

        lastTransactionId = -99; // different from -1 (used as an error code in fetchContacts)
        lastContactsFetched = [];
    }

    function waitForContactsFetched() {
        waitForTargetSignal(contactsFetchedSpy);
    }

    function listenToContactsFetched() {
        listenToTargetSignal(contactsFetchedSpy);
    }

    function verifyNoContactsFetchedReceived() {
        verifyNoTargetSignalReceived(contactsFetchedSpy);
    }

    // Helpers

    // These will be later found from the base test case element
    function initTestForTargetListeningToSignal(target, signalName) {
        logDebug("initTestForTargetListeningToSignal");
        var spy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsFetchContactsE2ETests);
        spy.target = target;
        spy.signalName = signalName;
        return spy;
    }

    function waitForTargetSignal(spy) {
        logDebug("waitForTargetSignal");
        spy.wait();
    }

    function listenToTargetSignal(spy) {
        logDebug("listenToTargetSignal");
        spy.clear();
    }

    function verifyNoTargetSignalReceived(spy) {
        logDebug("verifyNoTargetSignalReceived");
        wait(500);
        compare(spy.count, 0, "no target signal received");
    }

}
