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

    name: "ContactsExportImportE2ETests"

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    Contact {
        id: testContact
        Name {
            firstName: "First"
            lastName: "Last"
        }

        PhoneNumber {
            contexts: ContactDetail.ContextOther
            number: "5874983729"
        }
    }

    // Base for vcard file names.
    property string vcardFileNameBase: 'tst_contacts_export_import_e2e_'

    function test_contactExportImportWithTinyContactExportSignaling() {

        var signalSpy = initTestForTargetListeningToSignal(model, "exportCompleted");

        // Save and fetch test contact.
        model.saveContact(testContact);
        waitForContactsChanged();

        // Export contacts to vcard file.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "1.vcard");
        model.exportContacts(vcardFilePath, ["Sync"]);
        waitForTargetSignal(signalSpy);

        // Empty all contacts and import just created vcard file.
        emptyContacts(model);
        listenToContactsChanged();
        model.importContacts(vcardFilePath, ["Sync"]);
        waitForContactsChanged();
        compare (model.error, "NoError", "importError");

        // Check that imported contact has correct data.
        compare (model.contacts.length, 1, "import count");
        var importedContact = model.contacts[0];
        compare(importedContact.name.firstName,"First", "imported contact first name");
        compare(importedContact.name.lastName,"Last", "imported contact last name");
    }

    function test_exportListOfDeclarativeContacts() {
        var signalSpy = initTestForTargetListeningToSignal(model, "exportCompleted");

        // Export contacts to vcard file.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "contactsList.vcard");
        var temp = new Array(1);
        temp[0] = testContact;
        model.exportContacts(vcardFilePath, ["Sync"], temp);
        waitForTargetSignal(signalSpy);

        // Empty all contacts and import just created vcard file.
        emptyContacts(model);
        listenToContactsChanged();
        model.importContacts(vcardFilePath, ["Sync"]);
        waitForContactsChanged();
        compare (model.error, "NoError", "importError");

        // Check that imported contact has correct data.
        compare (model.contacts.length, 1, "import count");
        var importedContact = model.contacts[0];
        compare(importedContact.name.firstName,"First", "imported contact first name");
        compare(importedContact.name.lastName,"Last", "imported contact last name");
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();

        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
    }

    function init() {
        initTestForModel(model);
    }

    function cleanup() {
        emptyContacts(model);
    }

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

}
