/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

ContactsSavingTestCase {
    name: "ContactsSortingTests"

    ContactModel {
        id: model
        autoUpdate:true
    }

    SortOrder {
        id: sortOrderByFirstName
        detail: ContactDetail.Name
        field: Name.FirstName
        direction: Qt.AscendingOrder
    }

    Contact {
        id: contactWithFirstName1
        Name {
            firstName: "A"
        }
    }

    Contact {
        id: contactWithFirstName2
        Name {
            firstName: "B"
        }
    }

    function test_sortByFirstName()
    {
        model.sortOrders = [sortOrderByFirstName];
        waitForContactsChanged();

        model.saveContact(contactWithFirstName2);
        waitForContactsChanged();
        model.saveContact(contactWithFirstName1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactWithFirstName1, contactWithFirstName2]);
    }

    SortOrder {
        id: sortOrderByLastName
        detail: ContactDetail.Name
        field: Name.LastName
        direction: Qt.AscendingOrder
    }

    Contact {
        id: contactWithLastName1
        Name {
            lastName: "A"
        }
    }

    Contact {
        id: contactWithLastName2
        Name {
            lastName: "B"
        }
    }

    function test_sortByLastName()
    {
        model.sortOrders = [sortOrderByLastName];
        waitForContactsChanged();

        model.saveContact(contactWithLastName2);
        waitForContactsChanged();
        model.saveContact(contactWithLastName1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactWithLastName1, contactWithLastName2]);
    }

    SortOrder {
        id: sortOrderByEmailAddress
        detail: ContactDetail.Email
        field: EmailAddress.EmailAddress
        direction: Qt.AscendingOrder
    }

    Contact {
        id: contactWithEmailAddress1
        EmailAddress {
            emailAddress: "a@a"
        }
    }

    Contact {
        id: contactWithEmailAddress2
        EmailAddress {
            emailAddress: "b@b"
        }
    }

    function test_sortByEmail()
    {
        model.sortOrders = [sortOrderByEmailAddress];
        waitForContactsChanged();

        model.saveContact(contactWithEmailAddress2);
        waitForContactsChanged();
        model.saveContact(contactWithEmailAddress1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactWithEmailAddress1, contactWithEmailAddress2]);
    }

    Contact {
        id: contactWithFirstAndLastName1
        Name {
            lastName: "A"
            firstName: "E"
        }
    }

    Contact {
        id: contactWithFirstAndLastName2
        Name {
            lastName: "B"
            firstName: "C"
        }
    }

    Contact {
        id: contactWithFirstAndLastName3
        Name {
            lastName: "B"
            firstName: "D"
        }
    }

    function test_sortByLastAndFirstName()
    {
        skip("Backends do not support this at the moment.")
        model.sortOrders = [sortOrderByFirstName, sortOrderByLastName];
        waitForContactsChanged();

        model.saveContact(contactWithFirstAndLastName3);
        waitForContactsChanged();
        model.saveContact(contactWithFirstAndLastName2);
        waitForContactsChanged();
        model.saveContact(contactWithFirstAndLastName1);
        waitForContactsChanged();

        compareContactArrays(model.contacts,
                             [contactWithFirstAndLastName1, contactWithFirstAndLastName2,
                             contactWithFirstAndLastName3]);
    }

    SortOrder {
        id: sortOrderInDefaultOrder
        detail: ContactDetail.Name
        field: Name.FirstName
    }

    Contact {
        id: contactInDefaultOrder1
        Name {
            firstName: "A"
        }
    }

    Contact {
        id: contactInDefaultOrder2
        Name {
            firstName: "B"
        }
    }

    function test_sortInDefaultOrder()
    {
        model.sortOrders = [sortOrderInDefaultOrder];
        waitForContactsChanged();

        model.saveContact(contactInDefaultOrder2);
        waitForContactsChanged();
        model.saveContact(contactInDefaultOrder1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactInDefaultOrder1,
                                              contactInDefaultOrder2]);
    }

    SortOrder {
        id: sortOrderInAscendingOrder
        detail: ContactDetail.Name
        field: Name.FirstName
        direction: Qt.AscendingOrder
    }

    Contact {
        id: contactInAscendingOrder1
        Name {
            firstName: "A"
        }
    }

    Contact {
        id: contactInAscendingOrder2
        Name {
            firstName: "B"
        }
    }

    function test_sortInAscendingOrder()
    {
        model.sortOrders = [sortOrderInAscendingOrder];
        waitForContactsChanged();

        model.saveContact(contactInAscendingOrder2);
        waitForContactsChanged();
        model.saveContact(contactInAscendingOrder1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactInAscendingOrder1,
                                              contactInAscendingOrder2]);
    }

    SortOrder {
        id: sortOrderInDescendingOrder
        detail: ContactDetail.Name
        field: Name.FirstName
        direction: Qt.DescendingOrder
    }

    Contact {
        id: contactInDescendingOrder1
        Name {
            firstName: "B"
        }
    }

    Contact {
        id: contactInDescendingOrder2
        Name {
            firstName: "A"
        }
    }

    function test_sortInDescendingOrder()
    {
        model.sortOrders = [sortOrderInDescendingOrder];
        waitForContactsChanged();

        model.saveContact(contactInDescendingOrder2);
        waitForContactsChanged();
        model.saveContact(contactInDescendingOrder1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactInDescendingOrder1,
                                              contactInDescendingOrder2]);
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
        emptyContacts(model);
    }

    function cleanup() {
        if (model.sortOrders.length > 0) {
            model.sortOrders = [];
            waitForContactsChanged();
        }
        emptyContacts(model);
    }

    function cleanupTestCase() {
        finishTestForModel(model);
    }

    // Helper functions

    function compareContactArrays(actual, expected) {
        compare(actual.length, expected.length, "length");
        for (var i = 0; i < expected.length; i++) {
            compareContacts(actual[i], expected[i], "index " + i);
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
}
