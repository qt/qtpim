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

ContactsSavingTestCase {

    name: "ContactsFilteringTests"

    IdFilter {
        id: filter
        ids: []
    }

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    SignalSpy {
        id: contactsFetchedSpy
        signalName: "contactsFetched"
        target: model
    }

    Contact {
        id: contact1;
    }

    Contact {
        id: contact2;
    }

    Contact {
        id: contact3;
    }

    // Clean and populate the database with test contacts
    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);

        model.saveContact(contact1);
        waitForContactsChanged();
        model.saveContact(contact2);
        waitForContactsChanged();
        model.saveContact(contact3);
        waitForContactsChanged();
    }

    // Clean database
    function cleanupTestCase() {
        clearFilter();
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Clear filter
    function cleanup() {
        clearFilter();
        compare (model.contacts.length, 3);
    }

    function clearFilter() {
        if (model.filter) {
            model.filter = null;
            waitForContactsChanged();
        }
    }

    // Tests
    function test_dynamicIdFilterConstruction() {
        var newFilter = Qt.createQmlObject(
                "import QtContacts 5.0;" +
                    "IdFilter { ids: ['" + model.contacts[0].contactId + "']" +
                "}",
                this);
        model.filter = newFilter;
        waitForContactsChanged();
        compare(model.contacts.length, 1);
    }

    function test_filterById() {
        var id = model.contacts[0].contactId;
        filterById(id);
    }

    function filterById(id) {
        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 1);
        compare(model.contacts[0].contactId, id);
    }

    function test_filterByIdOfContactInTheMiddle() {
        var id = model.contacts[1].contactId;
        filterById(id);
    }

    function test_filterByIdOfContactAtTheEnd() {
        var id = model.contacts[2].contactId;
        filterById(id);
    }

    function test_filterByMultipleIds() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;
        filterByMultipleIds(id1, id2);
    }

    function filterByMultipleIds(id1, id2) {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[1].contactId;
        filter.ids = [id1, id2];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 2);
        compare(model.contacts[0].contactId, id1);
        compare(model.contacts[1].contactId, id2);
    }

    function test_filterByMultipleNonConsequtiveIds() {
        var id1 = model.contacts[0].contactId;
        var id2 = model.contacts[2].contactId;
        filterByMultipleIds(id1, id2);
    }

    function test_filterByMultipleIdsOfContactsAtTheEnd() {
        var id1 = model.contacts[1].contactId;
        var id2 = model.contacts[2].contactId;
        filterByMultipleIds(id1, id2);
    }

    function test_filterByNonExistingId() {
        filter.ids = ["foo bar"];
        model.filter = filter;
        waitForContactsChanged();;
        compare (model.contacts.length, 0);
    }

    function test_filterByMultipleNonExistingIds() {
        filter.ids = ["foo", "bar", "baz", "qux"];
        model.filter = filter;
        waitForContactsChanged();;
        compare (model.contacts.length, 0);
    }

    function test_filterByMixedExistingAndNonExistingIds() {
        var id = model.contacts[0].contactId;
        filter.ids = ["foo bar", id];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 1);
        compare(model.contacts[0].contactId, id);
    }

    function test_filterByEmptyIdList() {
        filter.ids = [];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 0);
    }

    function test_filterByTwoOverlappingIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[0].contactId];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 1);
    }


    function test_filterByTwoCouplesOfOverlappingIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[0].contactId, model.contacts[1].contactId, model.contacts[1].contactId];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 2);
    }

    function test_filterByAlternatingOverlappingIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[1].contactId, model.contacts[0].contactId];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 2);
    }

    function test_filterMatchingContactLeavesItStillValid() {
        var contact = model.contacts[0];
        var id = contact.contactId;

        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();

        verify(contact, "contact is defined");
        verify(contact.contactId, "contact id is defined");
        verify(contact === model.contacts[0],
               "still contains the contact")
    }

    function test_filterOutContactLeavesItStillValid() {
        var contact = model.contacts[0];
        var idOfAnotherContact = model.contacts[1].contactId;

        filter.ids = [idOfAnotherContact];
        model.filter = filter;
        waitForContactsChanged();

        verify(contact, "contact is defined");
        verify(contact.contactId, "contact id is defined");
    }

    function test_expandFilterLeavesContactsStillValid() {
        var contact = model.contacts[0];
        var id = contact.contactId;
        var idOfAnotherContact = model.contacts[1].contactId;

        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();

        contact = model.contacts[0];

        filter.ids = [id, idOfAnotherContact];
        model.filter = filter;
        waitForContactsChanged();

        verify(contact, "contact is defined");
        verify(contact.contactId, "contact id is defined");
        verify(contact === model.contacts[0] || contact === model.contacts[1],
               "still contains the contact")
    }

    IdFilter {
        id: oldFilter
        ids: []
    }

    IdFilter {
        id: newFilter
        ids: []
    }

    function test_afterSettingANewFilterChangingTheOldFilterDoesNotChangeModel() {
        var id = model.contacts[0].contactId;
        oldFilter.ids = [id];
        model.filter = oldFilter;
        waitForContactsChanged();

        newFilter.ids = [id];
        model.filter = newFilter;
        waitForContactsChanged();

        listenToContactsChanged();
        oldFilter.ids = []; // change the old filter

        verifyNoContactsChangedReceived();
    }
}
