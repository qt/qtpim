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

// Top-level element should have a default property to allow declaring elements
// inline (jsonDb element below). Component does not, so Item is used here
// although this is not a visual element.
Item {
    id: contactsJsonDbTestHelper

    property alias partition: jsonDb.partition

    function initTestHelper() {
        logDebug("initTestHelper()");
        createSpyForJsonDb();
    }

    function createContactToJsonDb(contact) {
        logDebug("createContactToJsonDb()");
        contact["_type"] = "com.nokia.mt.contacts.Contact";
        jsonDb.createAndSignal(contact);
        jsonDbSpy.wait();
    }

    function removeContactFromJsonDb(contact) {
        logDebug("removeContactFromJsonDb(): remove contact id " + contact.contactId);
        var contactUuid = convertContactIdToJsonDbUuid(contact.contactId);
        removeContactWithUuidFromJsonDb(contactUuid);
    }

    function removeContactWithUuidFromJsonDb(contactUuid) {
        logDebug("removeContactWithUuidFromJsonDb(): contact uuid " + contactUuid);
        var query = '[?_type="com.nokia.mt.contacts.Contact"]' +
                '[?_uuid="' + contactUuid + '"]';
        jsonDb.queryAndSignal(query);
        jsonDbSpy.wait();
        var object = jsonDb.lastResult[0];
        verify(object, "fetched given contact from jsondb");
        jsonDb.removeAndSignal(object);
        jsonDbSpy.wait();
    }

    // updates only the first name at the moment!
    function updateContactInJsonDb(contact, update) {
        var jsonUuid = convertContactIdToJsonDbUuid(contact.contactId);
        updateContactWithUuidInJsonDb(jsonUuid, update);
    }

    // updates only the first name at the moment!
    function updateContactWithUuidInJsonDb(contactUuid, update) {
        logDebug("updateContactWithUuidInJsonDb(): contact uuid " + contactUuid);

        var query = '[?_type="com.nokia.mt.contacts.Contact"]' +
                '[?_uuid="' + contactUuid + '"]';
        jsonDb.queryAndSignal(query);
        jsonDbSpy.wait();
        var object = jsonDb.lastResult[0];

        object.name.firstName = update.name.firstName;
        jsonDb.updateAndSignal(object);
        jsonDbSpy.wait();
    }

    function queryContactsInJsonDb() {
        logDebug("queryContactsInJsonDb()");

        var query = '[?_type="com.nokia.mt.contacts.Contact"]';
        jsonDb.queryAndSignal(query);
        jsonDbSpy.wait();
        return jsonDb.lastResult;
    }

    function emptyContacts() {
        logDebug("emptyContacts()");
        var query = '[?_type="com.nokia.mt.contacts.Contact"]';
        jsonDb.queryAndSignal(query);
        jsonDbSpy.wait();

        var contacts = jsonDb.lastResult;
        for (var i = 0; i < contacts.length; i++) {
            jsonDb.removeAndSignal(contacts[i]);
            jsonDbSpy.wait();
        }
    }

    function convertJsonDbUuidAndStorageLocationToContactId(contactUuid, storageLocation) {
        return 'qtcontacts:jsondb::' + storageLocation + '/' + contactUuid;
    }

    function convertContactIdToJsonDbUuid(contactId) {
        var idParts = contactId.split(':');
        var engineId = lastItemInArray(idParts);
        var engineIdParts = engineId.split('/');
        var jsonDbUuid = lastItemInArray(engineIdParts);
        return jsonDbUuid;
    }

    function lastItemInArray(array) {
        return array[array.length - 1];
    }

    SignalingJsonDb {
        id: jsonDb
    }

    property SignalSpy jsonDbSpy

    function createSpyForJsonDb() {
        logDebug("createSpyForJson()");
        jsonDbSpy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsJsonDbTestHelper);
        jsonDbSpy.target = jsonDb;
        jsonDbSpy.signalName = "operationFinished"
    }

    property bool debug: false

    function logDebug(message) {
        if (debug)
            console.log('ContactsJsonDbTestHelper.' + message);
    }
}

