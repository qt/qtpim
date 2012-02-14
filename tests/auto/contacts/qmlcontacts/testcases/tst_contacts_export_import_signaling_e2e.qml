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

    name: "ContactsVersitExportSignalingTest"

    ContactModel {
        id: model
        autoUpdate: true
        onExportCompleted: {
            exportErrorCode = error;
            exportFileName = url;
        }
        onImportCompleted: {
            importErrorCode = error;
            importFileName = url;
        }
    }

    Contact {
        id: testContact
        Name {
            firstName: "First"
            lastName: "Last"
        }

        PhoneNumber {
            contexts: 'Other'
            number: "5874983729"
        }
    }

    // Helper properties for signal parameters testing.
    property int exportErrorCode
    property string exportFileName
    property int importErrorCode
    property string importFileName

    // Base for vcard file names.
    property string vcardFileNameBase: 'tst_contacts_export_import_signaling_e2e_'

    function test_successfulExportEmitsSignalProperly() {

        var signalSpy = initTestForTargetListeningToSignal(model, "exportCompleted");

        // Save and export test contact.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "export_1.vcard");
        model.saveContact(testContact);
        waitForContactsChanged();
        model.exportContacts(vcardFilePath, ["Sync"]);

        // Check if export enters finished state properly.
        waitForTargetSignal(signalSpy);
        compare(exportErrorCode, ContactModel.ExportNoError, 'signal finished state error');
        compare(exportFileName, vcardFilePath, 'signal finished state filename');
    }

    function test_overlappingExportEmitsSignalWithError() {

        var signalSpy = initTestForTargetListeningToSignal(model, "exportCompleted");

        // Save and export test contact.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "export_2.vcard");
        var vcardFilePath2 = Qt.resolvedUrl(vcardFileNameBase + "export_3.vcard");
        model.saveContact(testContact);
        waitForContactsChanged();
        model.exportContacts(vcardFilePath, ["Sync"]);

        // Make immediate re-export which overlaps with the previous one and causes error.
        // We should get immediate singal with error showing the not ready yet.
        model.exportContacts(vcardFilePath2, ["Sync"]);
        waitForTargetSignal(signalSpy);
        compare(exportErrorCode, ContactModel.ExportNotReadyError, 'signal notready state error');
        compare(exportFileName, vcardFilePath2, 'signal notready state filename');

        // Check if the first export enters finished state properly.
        waitForTargetSignal(signalSpy);
        compare(exportErrorCode, ContactModel.ExportNoError, 'signal finished state error');
        compare(exportFileName, vcardFilePath, 'signal finished state filename');
    }


    function test_successfulImportEmitsSingalProperly() {

        // Save and fetch test contact.
        model.saveContact(testContact);
        waitForContactsChanged();

        // Export contacts to vcard file.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "import_1.vcard");
        var signalSpy1 = initTestForTargetListeningToSignal(model, "exportCompleted");
        model.exportContacts(vcardFilePath, ["Sync"]);
        waitForTargetSignal(signalSpy1);

        // Import contacts form vcard file just created.
        var signalSpy2 = initTestForTargetListeningToSignal(model, "importCompleted");
        listenToContactsChanged();
        model.importContacts(vcardFilePath, ["Sync"]);
        waitForContactsChanged();

        // Check that import enters finished state properly.
        waitForTargetSignal(signalSpy2);
        compare(importErrorCode, ContactModel.ImportNoError, 'signal finished state error');
        compare(importFileName, vcardFilePath, 'signal finished state filename');
    }


    function test_overlappingImportEmitsSingalWithError() {

        // Save and fetch test contact.
        model.saveContact(testContact);
        waitForContactsChanged();

        // Export contacts to two vcard files.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "import_2.vcard");
        var vcardFilePath2 = Qt.resolvedUrl(vcardFileNameBase + "import_3.vcard");
        var signalSpy1 = initTestForTargetListeningToSignal(model, "exportCompleted");
        model.exportContacts(vcardFilePath, ["Sync"]);
        waitForTargetSignal(signalSpy1);
        model.exportContacts(vcardFilePath2, ["Sync"]);
        waitForTargetSignal(signalSpy1);

        // Import contacts form vcard file just created.
        var signalSpy2 = initTestForTargetListeningToSignal(model, "importCompleted");
        listenToContactsChanged();
        model.importContacts(vcardFilePath, ["Sync"]);

        // Make immediate second import request which should fail as reader is busy.
        // Check that import enters finished state properly.
        model.importContacts(vcardFilePath2, ["Sync"]);
        waitForTargetSignal(signalSpy2);
        compare(importErrorCode, ContactModel.ImportNotReadyError, 'signal finished state error');
        compare(importFileName, vcardFilePath2, 'signal finished state filename');

        waitForContactsChanged();

        // Check that the first import enters finished state properly.
        waitForTargetSignal(signalSpy2);
        compare(importFileName, vcardFilePath, 'signal finished state filename');
        compare(importErrorCode, ContactModel.ImportNoError, 'signal finished state error');
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

        // Initial values for helper properties.
        exportErrorCode = ContactModel.ExportNoError
        exportFileName =  ""
        importErrorCode = ContactModel.ImportNoError
        importFileName =  ""
    }

    function cleanup() {
        emptyContacts(model);
    }

    function cleanupTestCase() {
        finishTestForModel(model);
    }

}
