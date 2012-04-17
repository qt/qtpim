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
        waitForContactsChanged();

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
