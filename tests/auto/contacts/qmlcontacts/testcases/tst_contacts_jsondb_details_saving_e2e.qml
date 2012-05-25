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
    name: "ContactsJsonDbDetailsSavingE2ETests"
    id: contactsJsonDbDetailsSavingE2ETests

    ContactModel {
        id: model
        manager: "jsondb"
        autoUpdate: true
    }

    // Tests

    Contact {
        id: contactWithBirthday
        Birthday {
            birthday: new Date(2012,0,2)
        }
    }

    function test_saveBirthday()
    {
        initTestForModel(model);

        model.saveContact(contactWithBirthday);
        waitForContactsChanged();

        var contacts = queryContactsInJsonDb();
        verify(contacts, "contacts is defined");
        compare(contacts.length, 1, "contact present");
        compare(contacts[0].details.birthday, "2012-01-02", "birthday matches");
    }

    // Init & teardown

    function initTestCase() {
        waitForModelToBeReady(model);
        cleanupContacts();
        initJsonDbAccess();
    }

    function init() {
        cleanupContacts();
    }

    function cleanup() {
        cleanupContacts();
    }

    function cleanupTestCase() {
        cleanupContacts();
    }

    function waitForModelToBeReady(model) {
        initTestForModel(model);
        waitForContactsChanged();
    }

    function cleanupContacts() {
        emptyContacts(model);
    }

    // Helpers

    ContactsJsonDbTestHelper {
        id: jsonDbTestHelper
    }

    function initJsonDbAccess() {
        jsonDbTestHelper.initTestHelper();
    }

    function queryContactsInJsonDb() {
        return jsonDbTestHelper.queryContactsInJsonDb();
    }
}
