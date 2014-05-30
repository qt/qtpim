/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtPim module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

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
        if (model) {
            model.autoUpdate = false;
            model.destroy();
        }
    }

    // Partitions under test

    Loader {
        id: jsonDbPartitions
        source: 'ContactsJsonDbPartitions.qml'
    }

    property var defaultPartition: jsonDbPartitions.item ? jsonDbPartitions.item.userPartition : undefined
    property var testPartition: jsonDbPartitions.item ? jsonDbPartitions.item.systemPartition : undefined

    // Tests

    Contact {
        id: contactInTheDefaultPartition
        Name {
            firstName: "contactInTheDefaultPartition"
        }
    }

    function test_fetchContactWithoutSpecifyingPartition()
    {
        createAndInitModelForPartition(); // does not set partition

        createContactToPartition({name: {firstName: "contactFromDefaultPartition"}}, defaultPartition);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "model has a contact");
        compare(model.contacts[0].name.firstName, "contactFromDefaultPartition", "detail values match");
    }

    function test_fetchContactFromTheDefaultPartitionWhenThereAreContactsInAnotherPartition()
    {
        createAndInitModelForPartition(defaultPartition);

        // create a contact to the other partition (does not change the model)
        listenToContactsChanged();
        createContactToPartition({}, testPartition);
        verifyNoContactsChangedReceived();

        // create a contact to and fetch it from the default partition
        createContactToPartition({name: {firstName: "contactFromDefaultPartition"}}, defaultPartition);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "model has a contact");
        compare(model.contacts[0].name.firstName, "contactFromDefaultPartition", "detail values match");
    }

    function test_fetchContactsToModelFromGivenPartition() {
        createAndInitModelForPartition(testPartition);

        createContactToPartition({name: {firstName: "contactFromGivenPartition"}}, testPartition);
        waitForContactsChanged();

        compare(model.contacts.length, 1, "model has a contact");
        compare(model.contacts[0].name.firstName, "contactFromGivenPartition", "detail values match");
    }

    function test_createNewContactAndSaveItToDefaultPartition() {
        createAndInitModelForPartition(defaultPartition);

        model.saveContact(contactInTheDefaultPartition);
        waitForContactsChanged();

        var contacts = queryContactsInPartition(defaultPartition);
        verify(contacts, "contacts is defined");
        compare(contacts.length, 1, "partition has a contact");
        compare(model.contacts[0].name.firstName, contactInTheDefaultPartition.name.firstName, "detail values match");
    }

    Contact {
        id: contactInTheTargetPartition
        Name {
            firstName: "contactInTheTargetPartition"
        }
    }

    function test_createNewContactAndSaveItToGivenPartition() {
        createAndInitModelForPartition(testPartition);

        model.saveContact(contactInTheTargetPartition, testPartition.storageLocation);
        waitForContactsChanged();

        var contacts = queryContactsInPartition(testPartition);
        verify(contacts, "contacts is defined");
        compare(contacts.length, 1, "partition has a contact");
        compare(model.contacts[0].name.firstName, contactInTheTargetPartition.name.firstName, "detail values match");
    }

    function test_createNewContactAndSaveItToPartitionWhichIsNotInTheModel() {
        createAndInitModelForPartition(defaultPartition);

        model.saveContact(contactInTheTargetPartition, testPartition.storageLocation);
        // the completion of the save does not signal through the model since it does not change
        wait(500);

        var contacts = queryContactsInPartition(testPartition);
        verify(contacts, "contacts is defined");
        compare(contacts.length, 1, "partition has a contact");
        compare(contacts[0].name.firstName, contactInTheTargetPartition.name.firstName, "detail values match");
    }

    function test_removeContactFromGivenPartition() {
        createAndInitModelForPartition(testPartition);

        createContactToPartition({name: {firstName: "contactToBeRemoved"}}, testPartition);
        waitForContactsChanged();
        compare(model.contacts.length, 1, "guard: model has a contact");

        model.removeContact(model.contacts[0].contactId);
        waitForContactsChanged();

        var contacts = queryContactsInPartition(testPartition);
        verify(contacts, "contacts is is defined");
        compare(contacts.length, 0, "partition is empty");
    }

    function test_fetchContactFromGivenPartitionChangeItAndSaveBackToTheSamePartition() {
        createAndInitModelForPartition(testPartition);

        createContactToPartition({name: {firstName: "contactToBeChanged"}}, testPartition);
        waitForContactsChanged();
        compare(model.contacts.length, 1, "guard: model has a contact");

        var contact = model.contacts[0];
        var newFirstName = "contactWithNewFirstName";
        contact.name.firstName = newFirstName;
        model.saveContact(contact);
        waitForContactsChanged();

        var contacts = queryContactsInPartition(testPartition);
        compare(contacts.length, 1, "partition has a contact");
        compare(model.contacts[0].name.firstName, newFirstName, "first name");
    }

    function test_updateContactInAnotherPartitionDoesNotChangeModel() {
        createAndInitModelForPartition(defaultPartition);

        createContactToPartition({name: {firstName: "old"}}, testPartition);
        compare(model.contacts.length, 0, "guard: model does not see the contact");

        var contacts = queryContactsInPartition(testPartition);
        compare(contacts.length, 1, "target partition has a contact");

        listenToContactsChanged();
        updateContactWithUuidInTargetPartition(contacts[0]["_uuid"], {name: {firstName: "new"}}, testPartition);
        verifyNoContactsChangedReceived();

        compare(model.contacts.length, 0, "model has not changed");
    }

    function test_removeContactInAnotherPartitionDoesNotChangeModel() {
        createAndInitModelForPartition(defaultPartition);

        createContactToPartition({}, testPartition);

        var contacts = queryContactsInPartition(testPartition);
        compare(contacts.length, 1, "target partition has a contact");

        listenToContactsChanged();
        removeContactWithUuidFromPartition(contacts[0]["_uuid"], testPartition);
        verifyNoContactsChangedReceived();
    }

    function test_setStorageLocationToGivenPartitionShouldFetchContacts() {
        createAndInitModelForPartition(defaultPartition);

        createContactToPartition({name: {firstName: "contactFromGivenPartition"}}, testPartition);
        // the completion of the save does not signal through the model since it does not change
        wait(500);

        model.storageLocations = testPartition.storageLocation;
        waitForContactsChanged();

        compare(model.contacts.length, 1, "model has a contact");
        compare(model.contacts[0].name.firstName, "contactFromGivenPartition", "detail values match");
    }

    function test_setIncorrectStorageLocationRaisesError() {
        createAndInitModelForPartition(testPartition);

        model.storageLocations = 0;

        compare(model.error, "BadArgument", "error");
    }

    function test_setMultipleStorageLocationsRaisesError() {
        createAndInitModelForPartition(testPartition);

        model.storageLocations = ContactModel.UserDataStorage | ContactModel.SystemStorage;

        compare(model.error, "NotSupported", "error");
    }

    function test_fetchContactsFromThePartitionInTheModel()
    {
        createAndInitModelForPartition(defaultPartition);

        initContactsFetchTestForModel(model);

        createContactToPartition({}, defaultPartition);
        waitForContactsChanged();
        compare(model.contacts.length, 1, "guard: model has a contact");

        var id = model.contacts[0].contactId;
        model.fetchContacts([id]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 1, "contacts length");
        compare(lastContactsFetched[0].contactId, id, "contact id");
    }

    function test_fetchContactsFromPartitionNotInTheModel()
    {
        createAndInitModelForPartition(defaultPartition);

        initContactsFetchTestForModel(model);

        createContactToPartition({}, testPartition);
        // the completion of the save does not signal through the model since it does not change
        wait(500);

        var id = queryContactIdOfTheContactOnPartition(testPartition);
        model.fetchContacts([id]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 1, "contacts length");
        compare(lastContactsFetched[0].contactId, id, "contact id");
    }

    function test_fetchContactsFromMultiplePartitions()
    {
        createAndInitModelForPartition(defaultPartition);

        initContactsFetchTestForModel(model);

        createContactToPartition({}, defaultPartition);
        waitForContactsChanged();
        createContactToPartition({}, testPartition);
        // the completion of the save does not signal through the model since it does not change
        wait(500);

        var id1 = queryContactIdOfTheContactOnPartition(defaultPartition);
        var id2 = queryContactIdOfTheContactOnPartition(testPartition);
        model.fetchContacts([id1, id2]);
        waitForContactsFetched();

        compare(lastContactsFetched.length, 2, "contacts length");
        compare(lastContactsFetched[0].contactId, id1, "contact id 1");
        compare(lastContactsFetched[1].contactId, id2, "contact id 2");
    }

    property SignalSpy contactsFetchedSpy
    property list<Contact> lastContactsFetched

    function onContactsFetched(requestId, fetchedContacts) {
        logDebug("onContactsFetched" + ": fetchedContacts " + JSON.stringify(fetchedContacts));
        lastContactsFetched = fetchedContacts;
    }

    function initContactsFetchTestForModel(model) {
        contactsFetchedSpy = initTestForTargetListeningToSignal(model, "contactsFetched");
        lastContactsFetched = [];
        model.contactsFetched.connect(onContactsFetched);
    }

    function waitForContactsFetched() {
        waitForTargetSignal(contactsFetchedSpy);
    }

    // Init & teardown

    function initTestCase() {
        initJsonDbAccess();
    }

    function init() {
        if (jsonDbTestHelperForDefaultPartition.item == null || jsonDbTestHelperForTestPartition.item == null)
            skip('Could not load jsondb components')

        cleanupContacts();
    }

    function cleanup() {
        destroyModel();
        cleanupContacts();
    }

    function cleanupTestCase() {
        if (jsonDbTestHelperForDefaultPartition.item != null && jsonDbTestHelperForTestPartition.item != null)
            cleanupContacts();
    }

    function cleanupContacts() {
        emptyContactsInPartition(defaultPartition);
        emptyContactsInPartition(testPartition);
    }

    // Helpers

    function createAndInitModelForPartition(partition) {
        if (partition === defaultPartition) {
            createModelForPartition(defaultPartition);
        } else if (partition === testPartition) {
            createModelForPartition(testPartition);
            waitForModelToBeReady(model); // change to another partition
        } else if (!partition) {
            createModelForPartition(); // does not set partition
        } else {
            fail("Failed to initialize partition for the model under test.")
        }
        initTestForModel(model);
    }

    function createModelForPartition(partition) {
        createModel();
        waitForModelToBeReady(model);
        if (partition)
            model.storageLocations = partition.storageLocation;
    }

    function waitForModelToBeReady(model) {
        initTestForModel(model);
        waitUntilContactsChanged();
    }

    function emptyContactsInPartition(partition) {
        partition.testHelper.emptyContacts();
    }

    function createContactToPartition(contact, partition) {
        partition.testHelper.createContactToJsonDb(contact);
    }

    function queryContactsInPartition(partition) {
        return partition.testHelper.queryContactsInJsonDb();
    }

    function updateContactWithUuidInTargetPartition(contactUuid, update, partition) {
        partition.testHelper.updateContactWithUuidInJsonDb(contactUuid, update);
    }

    function removeContactWithUuidFromPartition(contactUuid, partition) {
        partition.testHelper.removeContactWithUuidFromJsonDb(contactUuid);
    }

    function queryContactIdOfTheContactOnPartition(partition) {
        var contacts = queryContactsInPartition(partition);
        compare(contacts.length, 1, "guard: partition has one contact");
        var uuid = contacts[0]["_uuid"];
        return convertJsonDbUuidOnPartitionToContactId(uuid, partition);
    }

    function convertJsonDbUuidOnPartitionToContactId(uuid, partition) {
        return partition.testHelper.convertJsonDbUuidAndStorageLocationToContactId(uuid, partition.storageLocation);
    }

    Loader {
        id: jsonDbTestHelperForDefaultPartition
        source: 'ContactsJsonDbTestHelper.qml'
    }

    Loader {
        id: jsonDbTestHelperForTestPartition
        source: 'ContactsJsonDbTestHelper.qml'
    }

    function initJsonDbAccess() {
        if (jsonDbTestHelperForDefaultPartition.item != null && jsonDbTestHelperForTestPartition.item != null) {
            jsonDbTestHelperForDefaultPartition.item.partition = defaultPartition.name;
            jsonDbTestHelperForDefaultPartition.item.initTestHelper();
            defaultPartition.testHelper = jsonDbTestHelperForDefaultPartition.item;

            jsonDbTestHelperForTestPartition.item.partition = testPartition.name;
            jsonDbTestHelperForTestPartition.item.initTestHelper();
            testPartition.testHelper = jsonDbTestHelperForTestPartition.item;
        }
    }
}
