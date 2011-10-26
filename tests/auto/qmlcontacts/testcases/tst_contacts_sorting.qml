/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the FOO module of the Qt Toolkit.
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
import QtContacts 2.0

TestCase {
    name: "ContactsSortingTests"

    property SignalSpy contactsChangedSpy

    Contact {
        id: contact1
        Name {
            firstName: "A"
        }
    }

    Contact {
        id: contact2
        Name {
            firstName: "B"
        }
    }

    function test_sortByFirstName()
    {
        var model = Qt.createQmlObject(
                "import QtContacts 2.0;" +
                "ContactModel {" +
                "id:model;" +
                "manager:\"jsondb\";" +
                "autoUpdate:true;" +
                "sortOrders: [" +
                "    SortOrder {" +
                "        detail: ContactDetail.Name;" +
                "        field: Name.FirstName;" +
                "        direction: Qt.AscendingOrder;" +
                "    }" +
                "]" +
                "}",
                testHelper);
        testHelper.model = model;

        contactsChangedSpy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "   id: theSpy;" +
                    "   signalName: \"contactsChanged\";" +
                    "}",
                    testHelper);
        contactsChangedSpy.target = model;
        contactsChangedSpy.clear();

        testHelper.emptyContactsDb();

        model.saveContact(contact2);
        waitForContactsChanged();
        model.saveContact(contact1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contact1, contact2]);

        testHelper.emptyContactsDb();
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
        var model = Qt.createQmlObject(
                "import QtContacts 2.0;" +
                "ContactModel {" +
                "id:model;" +
                "manager:\"jsondb\";" +
                "autoUpdate:true;" +
                "sortOrders: [" +
                "    SortOrder {" +
                "        detail: ContactDetail.Name;" +
                "        field: Name.LastName;" +
                "        direction: Qt.AscendingOrder;" +
                "    }" +
                "]" +
                "}",
                testHelper);
        testHelper.model = model;

        contactsChangedSpy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {id: theSpy;signalName: \"contactsChanged\";}",
                    testHelper);
        contactsChangedSpy.target = model;
        contactsChangedSpy.clear();

        testHelper.emptyContactsDb();

        model.saveContact(contactWithLastName2);
        waitForContactsChanged();
        model.saveContact(contactWithLastName1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactWithLastName1, contactWithLastName2]);

        testHelper.emptyContactsDb();
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
        var model = Qt.createQmlObject(
                "import QtContacts 2.0;" +
                "ContactModel {" +
                "   id:model;" +
                "   manager:\"jsondb\";" +
                "   autoUpdate:true;" +
                "   sortOrders: [" +
                "       SortOrder {" +
                "           detail: ContactDetail.Email;" +
                "           field: EmailAddress.EmailAddress;" +
                "           direction: Qt.AscendingOrder;" +
                "       }" +
                "   ]" +
                "}",
                testHelper);
        testHelper.model = model;

        contactsChangedSpy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "   id: theSpy;" +
                    "   signalName: \"contactsChanged\";" +
                    "}",
                    testHelper);
        contactsChangedSpy.target = model;
        contactsChangedSpy.clear();

        testHelper.emptyContactsDb();

        model.saveContact(contactWithEmailAddress2);
        waitForContactsChanged();
        model.saveContact(contactWithEmailAddress1);
        waitForContactsChanged();

        compareContactArrays(model.contacts, [contactWithEmailAddress1, contactWithEmailAddress2]);

        testHelper.emptyContactsDb();
    }

    function compareContactArrays(actual, expected) {
        compare(actual.length, expected.length, "length");
        for (var i = 0; i < expected.length; i++) {
            compareContacts(actual[i], expected[i]);
        }
    }

    function compareContacts(actual, expected) {
        if (expected.name) {
            compare(actual.name.firstName, expected.name.firstName, 'name.firstName');
            compare(actual.name.lastName, expected.name.lastName, 'name.lastName');
        }
        if (expected.email) {
            compare(actual.email.emailAddress, expected.email.emailAddress,
                    'email.emailAddress');
        }
    }

    function waitForContactsChanged() {
        contactsChangedSpy.wait();
    }

    property Component component
    property ContactsTestHelper testHelper

    function init() {
        component = Qt.createComponent("ContactsTestHelper.qml");
        testHelper = component.createObject(top);
        if (testHelper == undefined)
            console.log("Unable to load component from " + name +  " error is ", component.errorString())
        verify(testHelper != undefined, 'Unable to load component ' + name);
    }

    function cleanup() {
        testHelper.destroy();
        component.destroy();
    }
}
