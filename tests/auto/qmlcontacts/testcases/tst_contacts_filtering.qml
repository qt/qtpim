/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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

TestCase {

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

    SignalSpy {
        id: contactsChangedSpy
        signalName: "contactsChanged"
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
        contactsChangedSpy.wait()

        cleanupTestCase()

        model.saveContact(contact1)
        contactsChangedSpy.wait()
        model.saveContact(contact2)
        contactsChangedSpy.wait()
        model.saveContact(contact3)
        contactsChangedSpy.wait()
    }

    // Clean database
    function cleanupTestCase() {
        var amt = model.contacts.length;
        for (var i = 0; i < amt; ++i) {
            var id = model.contacts[0].contactId
            model.removeContact(id)
            contactsChangedSpy.wait()
        }
        compare(model.contacts.length, 0)
    }

    // Clear filter
    function cleanup() {
        model.filter = null
        contactsChangedSpy.wait();
        compare (model.contacts.length, 3)
    }

    function test_dynamicIdFilterConstruction() {
        var newFilter = Qt.createQmlObject(
                "import QtContacts 5.0;" +
                    "IdFilter { ids: ['" + model.contacts[0].contactId + "']" +
                "}",
                this);
        model.filter = newFilter;
        contactsChangedSpy.wait();
        compare(model.contacts.length, 1)
    }

    function test_filterById() {
        filter.ids = [model.contacts[0].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 1)

        cleanup()

        filter.ids = [model.contacts[1].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 1)

        cleanup()

        filter.ids = [model.contacts[2].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 1)
    }

    function test_filterByMultipleIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[1].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 2)

        cleanup()

        filter.ids = [model.contacts[0].contactId, model.contacts[2].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 2)

        cleanup()

        filter.ids = [model.contacts[1].contactId, model.contacts[2].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 2)
    }

    function test_filterByNonExistingId() {
        filter.ids = ["foo bar"]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 0)

        cleanup()

        filter.ids = ["foo", "bar", "baz", "qux"]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 0)

        cleanup()

        filter.ids = ["foo bar", model.contacts[0].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 1)
    }

    function test_filterByEmptyList() {
        filter.ids = []
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 3)
    }

    function test_filterByTwoOverlappingIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[0].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 1)
    }


    function test_filterByTwoCouplesOfOverlappingIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[0].contactId, model.contacts[1].contactId, model.contacts[1].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 2)
    }

    function test_filterByAlternatingOverlappingIds() {
        filter.ids = [model.contacts[0].contactId, model.contacts[1].contactId, model.contacts[0].contactId]
        model.filter = filter
        contactsChangedSpy.wait()
        compare (model.contacts.length, 2)
    }

}
