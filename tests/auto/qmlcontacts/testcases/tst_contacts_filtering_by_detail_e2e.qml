/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

ContactsSavingTestCase {
    name: "ContactsFilteringByDetailE2ETests"

    ContactModel {
        id: model
        manager: "jsondb"
        autoUpdate: true
    }

    DetailFilter {
        id: filterByFirstName
        detail: ContactDetail.Name
        field: Name.FirstName
        matchFlags: Filter.MatchExactly
    }

    Contact {
        id: contact1;
        Name {
            firstName: "A"
        }
    }

    Contact {
        id: contact2;
        Name {
            firstName: "B"
        }
    }

    function test_filterByDetail() {
        model.saveContact(contact1);
        waitForContactsChanged();
        model.saveContact(contact2);
        waitForContactsChanged();

        filterByFirstName.value = "A";
        model.filter = filterByFirstName;
        waitForContactsChanged();

        compare(model.contacts.length, 1);
        compare(model.contacts[0].name.firstName, "A");
    }

    Contact {
        id: contactNotMatching;
        Name {
            firstName: "A"
        }
    }

    function test_filterByDetailValueWhichDoesNotMatchAny() {
        model.saveContact(contactNotMatching);
        waitForContactsChanged();

        filterByFirstName.value = "B";
        model.filter = filterByFirstName;
        waitForContactsChanged();

        compare(model.contacts.length, 0);
    }

    Contact {
        id: contactWithoutFirstName;
    }

    function test_filterByDetailWhenContactDoesNotHaveValueForThatDetail() {
        model.saveContact(contactWithoutFirstName);
        waitForContactsChanged();

        filterByFirstName.value = "A";
        model.filter = filterByFirstName;
        waitForContactsChanged();

        compare(model.contacts.length, 0);
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitForContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        cleanupModel();
    }

    function init() {
        initTestForModel(model);
        cleanupModel();
    }

    function cleanup() {
        cleanupModel();
    }

    function cleanupTestCase() {
        cleanupModel();
    }

    function cleanupModel() {
        if (model.filter) {
            model.filter = null;
            waitForContactsChanged();
        }
        emptyContacts(model);
    }
}