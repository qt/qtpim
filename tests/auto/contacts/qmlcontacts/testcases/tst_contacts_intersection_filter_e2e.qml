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
    name: "ContactsIntersectionFilterTests"

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    IntersectionFilter {
        id: intersectionFilter
    }

    DetailFilter {
        id: detailFilter1
        detail: ContactDetail.Name
        field: Name.FirstName
        value: "1"
        matchFlags: Filter.MatchExactly
    }

    DetailFilter {
        id: detailFilterB
        detail: ContactDetail.Name
        field: Name.LastName
        value: "B"
        matchFlags: Filter.MatchExactly
    }

    IdFilter { // contact id:s are set in initTestCase()
        id: idFilter_1B_2B_3B
    }

    IdFilter { // contact id:s are set in initTestCase()
        id: idFilter_2A_3A_1B_2B
    }

    Contact {
        id: contact1A
        Name {
            firstName: "1"
            lastName: "A"
        }
    }

    Contact {
        id: contact2A
        Name {
            firstName: "2"
            lastName: "A"
        }
    }

    Contact {
        id: contact3A
        Name {
            firstName: "3"
            lastName: "A"
        }
    }

    Contact {
        id: contact1B
        Name {
            firstName: "1"
            lastName: "B"
        }
    }

    Contact {
        id: contact2B
        Name {
            firstName: "2"
            lastName: "B"
        }
    }

    Contact {
        id: contact3B
        Name {
            firstName: "3"
            lastName: "B"
        }
    }

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
        saveContactsToModel();
        setupIdFilter();
    }

    function saveContactsToModel() {
        model.saveContact(contact1A);
        waitForContactsChanged();
        model.saveContact(contact2A);
        waitForContactsChanged();
        model.saveContact(contact3A);
        waitForContactsChanged();
        model.saveContact(contact1B);
        waitForContactsChanged();
        model.saveContact(contact2B);
        waitForContactsChanged();
        model.saveContact(contact3B);
        waitForContactsChanged();
        compare(model.contacts.length, 6);
    }

    function setupIdFilter() {
        idFilter_1B_2B_3B.ids = [findId("1", "B"), // contacts in model aren't guaranteed to be in the saving order, so find is needed
                                 findId("2", "B"),
                                 findId("3", "B")];
        idFilter_2A_3A_1B_2B.ids = [findId("2", "A"),
                                    findId("3", "A"),
                                    findId("1", "B"),
                                    findId("2", "B")];
    }

    function findId(firstName, lastName) {
        for (var i = 0; i < model.contacts.length; i++) {
            if (model.contacts[i].name.firstName == firstName &&
                    model.contacts[i].name.lastName == lastName)
                return model.contacts[i].contactId;
        }
    }

    function test_intersectionFilter_data() {
        return [{
                    tag: "Intersection of two detail filters",
                    filters: [detailFilter1, detailFilterB],
                    intersection: [contact1B]
                },
                {
                    tag: "Intersection of detail filter and id filter",
                    filters: [detailFilterB, idFilter_2A_3A_1B_2B],
                    intersection: [contact1B, contact2B]
                },
                {
                    tag: "Intersection of two id filters",
                    filters: [idFilter_1B_2B_3B, idFilter_2A_3A_1B_2B],
                    intersection: [contact1B, contact2B]
                },
                {
                    tag: "Intersection of two id filters and detail filter",
                    filters: [detailFilter1, idFilter_1B_2B_3B, idFilter_2A_3A_1B_2B],
                    intersection: [contact1B]
                },
                {
                    tag: "Intersection filter with empty filter set (manager: memory)",
                    manager: "memory",
                    filters: [],
                    intersection: []
                },
                {
                    tag: "Intersection filter with a single detail filter",
                    filters: [detailFilter1],
                    intersection: [contact1A, contact1B]
                }
                ];
    }

    function test_intersectionFilter(data) {
        if (data.manager && data.manager !== model.manager)
            skip("Test not applicable to this manager");
        intersectionFilter.filters = data.filters;
        model.filter = intersectionFilter;
        waitForContactsChanged();
        compareContacts(model.contacts, data.intersection)
    }

    function compareContacts(set1, set2) {
        // Compare filtered set to expected set.
        // All contacts in sets are assumed to have unique firstName and lastName detail pairs.
        compare(set1.length, set2.length);
        for (var i = 0; i < set1.length; i++) {
            var match = false;
            for (var j = 0; j < set2.length; j++) {
                if (set1[i].name.firstName == set2[j].name.firstName &&
                    set1[i].name.lastName == set2[j].name.lastName)
                    match = true;
            }
            compare(match, true);
        }
    }

    function cleanup() {
        if (model.filter) {
            model.filter = null;
            waitForContactsChanged();
        }
    }

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

}
