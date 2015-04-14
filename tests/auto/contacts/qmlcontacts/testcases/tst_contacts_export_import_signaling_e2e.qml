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

    name: "ContactsExportImportSignalingTest"

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
        onExportCompleted: {
            exportErrorCode = error;
            exportFileName = url;
            exportResults.append({"error": error, "url": exportFileName});
        }
        onImportCompleted: {
            importErrorCode = error;
            importFileName = url;
            importResults.append({"error": error, "url": importFileName});
        }
    }

    ListModel {
        id: exportResults
    }

    ListModel {
        id: importResults
    }

    Component {
        id: testContact

        Contact {
            Name {
                firstName: "First"
                lastName: "Last"
            }

            PhoneNumber {
                contexts: ContactDetail.ContextOther
                number: "5874983729"
            }
        }
    }

    // Helper properties for signal parameters testing.
    property int exportErrorCode
    property string exportFileName
    property int importErrorCode
    property string importFileName

    property string vcardFileNameBase: 'tst_contacts_export_import_signaling_e2e_'

    function createTestContact() {
        return testContact.createObject()
    }

    function test_successfulExportEmitsSignalProperly() {

        var signalSpy = initTestForTargetListeningToSignal(model, "exportCompleted");

        // Save and export test contact.
        var vcardFilePath = Qt.resolvedUrl(vcardFileNameBase + "export_1.vcard");
        model.saveContact(createTestContact());
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
        model.saveContact(createTestContact());
        waitForContactsChanged();
        model.exportContacts(vcardFilePath, ["Sync"]);

        // Make immediate re-export which overlaps with the previous one and causes error.
        model.exportContacts(vcardFilePath2, ["Sync"]);

        // Wait for both exports completed.
        waitForTargetSignal(signalSpy);
        waitForTargetSignal(signalSpy);

        // It is possible that threads get scheduled so that both exports succeed.
        if ((exportResults.get(0).error === ContactModel.ExportNoError) && (exportResults.get(1).error === ContactModel.ExportNoError)) {
            console.log("Overlapping exports did not overlap. This may be ok but error on overlapping did not get tested!")
        } else {
            // We got overlapping but it is a bit complex to verify since we do not know for sure in which order signals did arrive.
             for (var i = 0; i < exportResults.count; i++) {
                if (exportResults.get(i).error === ContactModel.ExportNoError) {
                    compare(exportResults.get(i).url, vcardFilePath, 'signal finished state filename');
                } else {
                    compare(exportResults.get(i).error, ContactModel.ExportNotReadyError, 'signal not ready state error');
                    compare(exportResults.get(i).url, vcardFilePath2, 'signal not ready state filename');
                }
            }
        }
    }


    function test_successfulImportEmitsSignalProperly() {

        // Save and fetch test contact.
        model.saveContact(createTestContact());
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

        // Check if importComplete contains the correct contactId
        var importedIds = signalSpy2.signalArguments[0][2]
        compare(importedIds[0], model.contacts[model.contacts.length - 1].contactId)
    }


    function test_overlappingImportEmitsSignalWithError() {

        // Save and fetch test contact.
        model.saveContact(createTestContact());
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
        model.importContacts(vcardFilePath2, ["Sync"]);

        // Wait for both imports to finish.
        waitForTargetSignal(signalSpy2);
        waitForTargetSignal(signalSpy2);

        // It is possible that threads get scheduled so that both imports succeed.
        if ((importResults.get(0).error === ContactModel.ImportNoError) && (importResults.get(1).error === ContactModel.ImportNoError)) {
            console.log("Overlapping imports did not overlap. This may be ok but error on overlapping did not get tested!")
        } else {
            // We got overlapping but it is a bit complex to verify since we do not know for sure in which order signals did arrive.
            for (var i = 0; i < importResults.count; i++) {
                if (importResults.get(i).error === ContactModel.ImportNoError) {
                    compare(importResults.get(i).url, vcardFilePath, 'signal finished state filename');
                } else {
                    compare(importResults.get(i).error, ContactModel.ImportNotReadyError, 'signal not ready state error');
                    compare(importResults.get(i).url, vcardFilePath2, 'signal not ready state filename');
                }
            }
        }
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
        waitUntilContactsChanged();

        // Initial values for helper properties.
        exportErrorCode = ContactModel.ExportNoError
        exportFileName =  ""
        importErrorCode = ContactModel.ImportNoError
        importFileName =  ""

    }

    function cleanup() {
        emptyContacts(model);

        exportResults.clear();
        importResults.clear();
    }

    function cleanupTestCase() {
        finishTestForModel(model);
    }

}
