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

    name: "ContactsFilteringTests"

    IdFilter {
        id: filter
        ids: []
    }

    ContactModel {
        id: model
        manager: "jsondb"
        autoUpdate: true
        filter: filter
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
        waitForContactsChanged();
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
        if (model.filter) {
            model.filter = null;
            waitForContactsChanged();
        }
        emptyContacts(model);
    }

    // Clear filter
    function cleanup() {
        model.filter = null;
        waitForContactsChanged();
        compare (model.contacts.length, 3);
    }

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
        waitForContactsChanged();
        compare (model.contacts.length, 0);
    }

    function test_filterByMultipleNonExistingIds() {
        filter.ids = ["foo", "bar", "baz", "qux"];
        model.filter = filter;
        waitForContactsChanged();
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

    function test_filterByEmptyList() {
        filter.ids = [];
        model.filter = filter;
        waitForContactsChanged();
        compare (model.contacts.length, 3);
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

    function test_filterAndFetchSomeMatchingIds() {
        var contact = model.contacts[0];
        var id = contact.contactId;

        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();

        model.fetchContacts([id]);
        waitForContactsChanged();

        compare(model.contacts[0].contactId, id);
    }

    function test_filterAndFetchSomeNonMatchingIds() {
        var contact = model.contacts[0];
        var id = contact.contactId;
        var idOfAnotherContact = model.contacts[1].contactId;
        verify(id != idOfAnotherContact, "guard: contacts are different");

        filter.ids = [id];
        model.filter = filter;
        waitForContactsChanged();

        model.fetchContacts([idOfAnotherContact]);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "contacts length");
        compare(model.contacts[0].contactId, id, "contact is still present");
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
