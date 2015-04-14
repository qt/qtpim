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

    id: testcase
    name: "ContactsFilteringByDetailE2ETests"

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    Contact {
        id: contact1;
        Name {
            firstName: "A"
        }
        PhoneNumber {
            number: "1111111111"
        }
    }

    Contact {
        id: contact2;
        Name {
            firstName: "B"
        }
        PhoneNumber {
            number: "2222222222"
        }
    }

    Contact {
        id: contact3;
        Name {
            firstName: "John Joe"
        }
        PhoneNumber {
            number: "3333333333"
        }
    }

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
        compare(model.contacts.length, 3);
    }

    function createDetailFilter(detail, field, value, matchFlags) {
        var filter = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "DetailFilter {}",
                    testcase);

        filter.detail = detail
        filter.field = field
        filter.value = value
        filter.matchFlags = matchFlags
        return filter;
    }

    function createPhoneNumberFilter(value, matchFlags) {
        return createDetailFilter(ContactDetail.PhoneNumber, PhoneNumber.Number, value, matchFlags);
    }

    function createFirstNameFilter(value, matchFlags) {
        return createDetailFilter(ContactDetail.Name, Name.FirstName, value, matchFlags);
    }

    function test_filterByDetail_data() {
        return [{
                    tag: "Phone number, match exactly, identical value",
                    filter: createPhoneNumberFilter(contact1.phoneNumber.number, Filter.MatchExactly),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match exactly, no match",
                    filter: createPhoneNumberFilter("1111", Filter.MatchExactly),
                    matches: []
                },
                {
                    tag: "Phone number, match exactly, empty string",
                    filter: createPhoneNumberFilter("", Filter.MatchExactly),
                    matches: []
                },
                {
                    tag: "Phone number, match contains, identical value",
                    filter: createPhoneNumberFilter(contact1.phoneNumber.number, Filter.MatchContains),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match contains, no match",
                    filter: createPhoneNumberFilter(" ", Filter.MatchContains),
                    matches: []
                },
                {
                    tag: "Phone number, match contains, matching substring",
                    filter: createPhoneNumberFilter("1111", Filter.MatchContains),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match contains, empty string",
                    filter: createPhoneNumberFilter("", Filter.MatchContains),
                    matches: [contact1, contact2, contact3]
                },
                {
                    tag: "Phone number, match starts with, identical value",
                    filter: createPhoneNumberFilter(contact1.phoneNumber.number, Filter.MatchStartsWith),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match starts with, no match",
                    filter: createPhoneNumberFilter(" ", Filter.MatchStartsWith),
                    matches: []
                },
                {
                    tag: "Phone number, match starts with, matching substring",
                    filter: createPhoneNumberFilter("1111", Filter.MatchStartsWith),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match starts with, empty string",
                    filter: createPhoneNumberFilter("", Filter.MatchStartsWith),
                    matches: [contact1, contact2, contact3]
                },
                {
                    tag: "Phone number, match ends with, identical value",
                    filter: createPhoneNumberFilter(contact1.phoneNumber.number, Filter.MatchEndsWith),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match ends with, no match",
                    filter: createPhoneNumberFilter(" ", Filter.MatchEndsWith),
                    matches: []
                },
                {
                    tag: "Phone number, match ends with, matching substring",
                    filter: createPhoneNumberFilter("1111", Filter.MatchEndsWith),
                    matches: [contact1]
                },
                {
                    tag: "Phone number, match ends with, empty string",
                    filter: createPhoneNumberFilter("", Filter.MatchEndsWith),
                    matches: [contact1, contact2, contact3]
                },
                {
                    tag: "First name, match exactly, identical value",
                    filter: createFirstNameFilter(contact1.name.firstName, Filter.MatchExactly),
                    matches: [contact1]
                },
                {
                    tag: "First name, match exactly, no match",
                    filter: createFirstNameFilter("C", Filter.MatchExactly),
                    matches: []
                },
                {
                    tag: "First name, match contains , matching substring",
                    filter: createFirstNameFilter("Joe", Filter.MatchContains),
                    matches: [contact3]
                },
                {
                    tag: "First name, match contains , caseInsensitive by default so matches",
                    filter: createFirstNameFilter("joe", Filter.MatchContains),
                    matches: [contact3]
                },
                {
                    tag: "First name, match exactly ,always case sensitive so no matches here",
                    filter: createFirstNameFilter("john joe", Filter.MatchExactly),
                    matches: []
                },
                {
                    tag: "First name, match fixed string , identical value",
                    filter: createFirstNameFilter("John Joe", Filter.MatchFixedString),
                    matches: [contact3]
                },
                {
                    tag: "First name, match fixed string , no match",
                    filter: createFirstNameFilter("John", Filter.MatchFixedString),
                    matches: []
                },
                {
                    tag: "First name, match fixed string, caseInsensitive by default so matches",
                    filter: createFirstNameFilter("john joe", Filter.MatchFixedString),
                    matches: [contact3]
                },
                {
                    tag: "First name, match fixed string and case sensitive, no match",
                    filter: createFirstNameFilter("john joe", Filter.MatchFixedString | Filter.MatchCaseSensitive),
                    matches: []
                },
                {
                    tag: "First name, match starts with, caseInsensitive by default so matches",
                    filter: createFirstNameFilter("john", Filter.MatchStartsWith),
                    matches: [contact3]
                },
                {
                    tag: "First name, match starts with and match caseSensitive, matching substring",
                    filter: createFirstNameFilter("John", Filter.MatchStartsWith | Filter.MatchCaseSensitive),
                    matches: [contact3]
                },
                {
                    tag: "First name, match starts with and match caseSensitive, no match",
                    filter: createFirstNameFilter("john", Filter.MatchStartsWith | Filter.MatchCaseSensitive),
                    matches: []
                },
                {
                    tag: "First name, match ends with and match caseSensitive, matching substring",
                    filter: createFirstNameFilter("Joe", Filter.MatchEndsWith | Filter.MatchCaseSensitive),
                    matches: [contact3]
                },
                {
                    tag: "First name, match ends with and match caseSensitive, no match",
                    filter: createFirstNameFilter("joe", Filter.MatchEndsWith | Filter.MatchCaseSensitive),
                    matches: []
                },
                {
                    tag: "First name, match ends with, caseInsensitive by default so matches",
                    filter: createFirstNameFilter("joe", Filter.MatchEndsWith),
                    matches: [contact3]
                },
                {
                    tag: "First name, match fixedString and match ends with, matching substring",
                    filter: createFirstNameFilter("John Joe", Filter.MatchFixedString | Filter.MatchEndsWith),
                    matches: [contact3]
                },
                {
                    tag: "First name, match fixedString and match startsWith, matching substring",
                    filter: createFirstNameFilter("John Joe", Filter.MatchFixedString | Filter.MatchStartsWith),
                    matches: [contact3]
                },
                {
                    tag: "Last name, match exactly, no contact has a value for last name",
                    filter: createDetailFilter(ContactDetail.Name, Name.LastName, "NoContactHasThisAsLastName", Filter.MatchExactly),
                    matches: []
                },
               ]
    }

    function test_filterByDetail(data) {
        model.filter = data.filter;
        waitForContactsChanged();
        expectFail('First name, match exactly ,always case sensitive so no matches here', 'Not working as expected')
        compare(model.contacts.length, data.matches.length, data.tag);
        // Compare filtered set to expected set
        for (var i = 0; i < model.contacts.length; i++) {
            var match = false;
            for (var j = 0; j < data.matches.length; j++) {
                if (model.contacts[i].name.firstName == data.matches[j].name.firstName &&
                    model.contacts[i].phoneNumber.number == data.matches[j].phoneNumber.number)
                    match = true;
            }
            compare(match, true);
        }
    }

    function test_settingMatchFlagDoesChangeTheContacts() {
        var filter = createPhoneNumberFilter("1111", Filter.MatchExactly);
        model.filter = filter;
        waitForContactsChanged();
        compare(model.contacts.length, 0);
        model.filter.matchFlags = Filter.MatchContains;
        waitForContactsChanged();
        compare(model.contacts.length, 1);
        model.filter.matchFlags = Filter.MatchStartsWith;
        waitForContactsChanged();
        compare(model.contacts.length, 1);
        model.filter.matchFlags = Filter.MatchEndsWith;
        waitForContactsChanged();
        compare(model.contacts.length, 1);
    }

    // Teardown

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
