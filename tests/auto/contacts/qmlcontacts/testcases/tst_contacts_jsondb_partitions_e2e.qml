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
import QtJsonDb 1.0 as JsonDb

ContactsSavingTestCase {
    name: "ContactsJsonDbPartitionsE2ETests"
    id: contactsJsonDbPartitionsE2ETests

    property ContactModel model

    function createModel() {
        model = Qt.createQmlObject(
                    'import QtContacts 5.0;' +
                    'ContactModel {' +
                        'manager: "jsondb";' +
                        'autoUpdate: true;' +
                    '}',
                    contactsJsonDbPartitionsE2ETests);
    }

    function destroyModel() {
        model.autoUpdate = false;
        model.destroy();
    }

    property string testPartition: "qtpim.contacts.jsondbPartitionTests"

    // Tests

    Contact {
        id: contactInTheDefaultPartition
        Name {
            firstName: "A"
        }
    }

    function test_fetchContactFromTheDefaultPartitionWhenThereAreContactsInAnotherPartition()
    {
        initTestForModel(model);

        createPartition(testPartition);
        setTargetPartition(testPartition);
        emptyContactsInTargetPartition();

        // create a contact to the other partition does not affect the model
        listenToContactsChanged();
        createContactToTargetPartition({});
        verifyNoContactsChangedReceived();

        // create a contact to and fetch it from the default partition
        model.saveContact(contactInTheDefaultPartition);
        waitForContactsChanged();
        compare(model.contacts.length, 1, "model is not empty");
        compare(model.contacts[0].name.firstName, contactInTheDefaultPartition.name.firstName, "first name");

        emptyContactsInTargetPartition();
        removePartition(testPartition);
    }

    // Init & teardown

    function initTestCase() {
        cleanupContacts();
    }

    function cleanupTestCase() {
        cleanupContacts();
    }

    function init() {
        initJsonDbAccess();
        createModel();
        waitForModelToBeReady(model);
    }

    function waitForModelToBeReady(model) {
        initTestForModel(model);
        waitForContactsChanged();
    }

    function cleanup() {
        cleanupContacts();
        destroyModel();
    }

    function cleanupContacts() {
        var modelForCleanup = Qt.createQmlObject(
                    'import QtContacts 5.0;' +
                    'ContactModel {' +
                        'manager: "jsondb";' +
                        'autoUpdate: true;' +
                    '}',
                    contactsJsonDbPartitionsE2ETests);
        waitForModelToBeReady(modelForCleanup);

        emptyContacts(modelForCleanup);

        modelForCleanup.autoUpdate = false;
        modelForCleanup.destroy();
    }

    // Helpers

    function createPartition(partition) {
        jsonDbPartitionTestHelper.createPartitionToJsonDb(partition);
    }

    function setTargetPartition(partition) {
        jsonDbTestHelper.partition = partition;
    }

    function emptyContactsInTargetPartition() {
        jsonDbTestHelper.emptyContacts();
    }

    function createContactToTargetPartition(contact) {
        jsonDbTestHelper.createContactToJsonDb(contact);
    }

    function removePartition(partition) {
        jsonDbPartitionTestHelper.removePartitionFromJsonDb(partition);
    }

    ContactsJsonDbTestHelper {
        id: jsonDbTestHelper
        partition: testPartition
    }

    ContactsJsonDbPartitionTestHelper {
        id: jsonDbPartitionTestHelper
    }

    function initJsonDbAccess() {
        jsonDbTestHelper.initTestHelper();
        jsonDbPartitionTestHelper.initTestHelper();
    }
}
