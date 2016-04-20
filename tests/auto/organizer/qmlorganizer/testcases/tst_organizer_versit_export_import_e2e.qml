/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPim Addon Module.
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
import QtOrganizer 5.0

TestCase {
    id: test
    name: "OrganizerVersitExportImportTests"

    property int spyWaitDelay: 250

    // Helper properties for signal parameters testing.
    property int exportErrorCode
    property string exportFileName
    property int importErrorCode
    property string importFileName

    QOrganizerTestUtility {
        id: utility
    }

    OrganizerModel {
        id: organizerModel
        startPeriod: '2009-01-01'
        endPeriod: '2012-12-31'
        autoUpdate: true
        onExportCompleted: {
            exportErrorCode = error
            exportFileName = url
        }
        onImportCompleted: {
            importErrorCode = error
            importFileName = url
        }
    }

    SignalSpy {
        id: modelChangedSpy
        target: organizerModel
        signalName: "modelChanged"
    }

    Event {
        id: testEvent
        startDateTime: new Date("1991-08-25T20:57:08Z")
        endDateTime: new Date("1995-05-20 11:22:33 GMT+0200")
        displayLabel: "Versit event test"
    }

    MegaEvent {
        id: megaEvent
    }

    function initTestCase() {
    }

    function init() {
        empty_calendar()
    }

    function cleanup() {
        empty_calendar()
    }

    // UTILITIES

    // There is currently some problem with static
    // SignalSpy and changing the target (QTBUG-21083).
    // As a workaround recreating the spy dynamically.
    function create_spy(targetObj, signalName) {
        var spy = Qt.createQmlObject("import QtTest 1.0 \nSignalSpy {}", test)
        spy.target = targetObj
        spy.signalName = signalName
        return spy
    }

    function create_testobject(ctorString) {
        var newObject = Qt.createQmlObject(ctorString, organizerModel)
        verify(newObject != undefined, 'Object creation failed')
        return newObject
    }

    function empty_calendar() {
        var ids = organizerModel.itemIds()
        if (ids.length > 0) {
            organizerModel.removeItems(ids)
        }
        organizerModel.update()

        verify(organizerModel.itemCount === 0, 'empty_calendar failed')
    }

    function saveTestEvent() {
        modelChangedSpy.clear()
        var detailEvent = create_testobject(
                    "import QtQuick 2.0\n" + "import QtOrganizer 5.0 \n" + "   Event {\n"
                    + "  displayLabel:\"Event 1\"\n" + "  description:\"Event 1 description\"\n"
                    + "  startDateTime:new Date('2011-10-25')\n"
                    + "  endDateTime:new Date('2011-10-26')\n" + "   }")
        organizerModel.saveItem(detailEvent)
        modelChangedSpy.wait()
        verify(organizerModel.itemCount === 1, 'Save item check failed.')
    }

    function test_organizerImportExportSignaling_data() {
        return utility.getManagerListData();
    }

    function test_organizerImportExportSignaling(data) {

        organizerModel.manager = data.managerToBeTested

        // Save test Event.
        saveTestEvent()

        // Export even to vcal file.
        var exportModelChangedSpy = create_spy(organizerModel,
                                               "exportCompleted")
        var icalFilePath = Qt.resolvedUrl(
                    "tst_organizer_versit_export_import_e2e.ical")
        organizerModel.exportItems(icalFilePath, ["Sync"])
        exportModelChangedSpy.wait()

        // Check if export enters finished state properly.
        compare(exportErrorCode, OrganizerModel.ExportNoError,
                'exportCompleted signal gave error during export.')
        compare(exportFileName, icalFilePath,
                'exportCompleted signal reported incorrect filename compared to the given name.')
        compare(organizerModel.error, "NoError", "exportError found")

        // Empty all items and import just created icalFilePath file.
        empty_calendar()
        var importModelChangedSpy = create_spy(organizerModel,
                                               "importCompleted")
        organizerModel.importItems(icalFilePath, ["Sync"])
        importModelChangedSpy.wait()

        compare(importErrorCode, OrganizerModel.ImportNoError,
                'importCompleted signal gave error during import.')

        compare(importFileName, icalFilePath,
                'importCompleted signal reported incorrect filename compared to the given name.')
        compare(organizerModel.error, "NoError", "importError found")

        // Check that imported items have correct data.
        compare(organizerModel.itemCount, 1,
                "Versit Import Count Check Failed.")
        var importedEvent = organizerModel.items[0]
        compare(importedEvent.description, "Event 1 description",
                "imported description different from the exported description.")
        compare(importedEvent.displayLabel, "Event 1",
                "imported event label different to exported label.")

        // Check if importComplete contains the correct itemId
        var importedIds = importModelChangedSpy.signalArguments[0][2]
        compare(importedIds[0], importedEvent.itemId)
    }

    function test_overlappingExportEmitsSignalWithError_data() {
        return utility.getManagerListData();
    }

    function test_overlappingExportEmitsSignalWithError(data) {

        organizerModel.manager = data.managerToBeTested

        // Export items to two ical files which we will use for testing purposes....
        var icalFilePath1 = Qt.resolvedUrl("export_3.ical")
        var icalFilePath2 = Qt.resolvedUrl("export_4.ical")

        // Save test Event.
        saveTestEvent()

        // Make immediate re-export which overlaps with the previous one and causes error.
        // We should get immediate signal with error showing the not ready yet.
        var exportModelChangedSpy = create_spy(organizerModel,
                                               "exportCompleted")
        organizerModel.exportItems(icalFilePath1, ["Sync"])
        organizerModel.exportItems(icalFilePath2, ["Sync"])
        exportModelChangedSpy.wait()
        compare(exportErrorCode, OrganizerModel.ExportNotReadyError,
                'Overlapping export should have given an error.')
        compare(exportFileName, icalFilePath2,
                'Overlapping export incorrect file path.')

        // Check if the first export enters finished state properly.
        exportModelChangedSpy.wait()
        compare(exportErrorCode, OrganizerModel.ExportNoError,
                'Exported item gave an unexpected error.')
        compare(exportFileName, icalFilePath1,
                'Exported item was different than expected.')
    }

    function test_overlappingImportEmitsSignalWithError_data() {
        return utility.getManagerListData();
    }

    function test_overlappingImportEmitsSignalWithError(data) {

        organizerModel.manager = data.managerToBeTested

        // Save test Event.
        saveTestEvent()

        // Export items to two ical files which we will use for testing purposes....
        var icalFilePath1 = Qt.resolvedUrl("import_1.ical")
        var icalFilePath2 = Qt.resolvedUrl("import_2.ical")

        var exportModelChangedSpy = create_spy(organizerModel,
                                               "exportCompleted")
        organizerModel.exportItems(icalFilePath1, ["Sync"])
        exportModelChangedSpy.wait()
        organizerModel.exportItems(icalFilePath2, ["Sync"])
        exportModelChangedSpy.wait()

        // Import Events form ical file just created.
        modelChangedSpy.clear()
        var importModelChangedSpy = create_spy(organizerModel,
                                               "importCompleted")
        organizerModel.importItems(icalFilePath1, ["Sync"])

        // Make immediate second import request which should fail as reader is busy.
        organizerModel.importItems(icalFilePath2, ["Sync"])

        // Check that import enters finished state properly.
        importModelChangedSpy.wait()
        compare(importErrorCode, OrganizerModel.ImportNotReadyError,
                'Overlapping import error state incorrect.')
        compare(importFileName, icalFilePath2,
                'Overlapping import file path incorrect.')

        // Check that the first import enters finished state properly.
        importModelChangedSpy.wait()// import finished
        modelChangedSpy.wait()// imported items also now in model

        compare(importFileName, icalFilePath1, 'Imported item failed filename.')
        compare(importErrorCode, OrganizerModel.ImportNoError,
                'Imported Item failed.')
    }

    // Memory model for detail exporting test
    OrganizerModel {
        id: memoryModel
        manager: "memory"
        startPeriod: '2009-01-01'
        endPeriod: '2012-12-31'
        autoUpdate: true
    }

    SignalSpy {
        id: memoryModelSpy
        target: memoryModel
        signalName: "modelChanged"
    }

    function test_organizerImportExportItemDetails() {
        memoryModelSpy.wait() // Signal from model initialization

        memoryModel.saveItem(megaEvent)
        memoryModelSpy.wait()

        var exportModelChangedSpy = create_spy(memoryModel, "exportCompleted")
        memoryModel.exportItems(Qt.resolvedUrl("tst_organizer_versit_export_import_e2e_megaevent.ical"), ["Sync"])
        exportModelChangedSpy.wait()

        memoryModel.removeItem(memoryModel.items[0])
        memoryModelSpy.wait()

        memoryModel.importItems(Qt.resolvedUrl("tst_organizer_versit_export_import_e2e_megaevent.ical"), ["Sync"])
        memoryModelSpy.wait()

        compare(memoryModel.itemCount, 1)

        var importedEvent = memoryModel.items[0]

        // Verify imported details. Todo: verify more details when more details are supported
        compare(importedEvent.startDateTime, megaEvent.startDateTime)
        compare(importedEvent.endDateTime, megaEvent.endDateTime)
        compare(importedEvent.displayLabel, megaEvent.displayLabel)
        compare(importedEvent.priority, megaEvent.priority)
        compare(importedEvent.description, megaEvent.description)
        compare(importedEvent.details(Detail.ExtendedDetail).length, 4)
        compare(importedEvent.details(Detail.ExtendedDetail)[0].name, megaEvent.details(Detail.ExtendedDetail)[0].name)
        compare(importedEvent.details(Detail.ExtendedDetail)[0].data, megaEvent.details(Detail.ExtendedDetail)[0].data)
        compare(importedEvent.details(Detail.ExtendedDetail)[1].name, megaEvent.details(Detail.ExtendedDetail)[1].name)
        compare(importedEvent.details(Detail.ExtendedDetail)[1].data, megaEvent.details(Detail.ExtendedDetail)[1].data)
        compare(importedEvent.details(Detail.ExtendedDetail)[2].name, megaEvent.details(Detail.ExtendedDetail)[2].name)
        compare(importedEvent.details(Detail.ExtendedDetail)[2].data, megaEvent.details(Detail.ExtendedDetail)[2].data)
        compare(importedEvent.details(Detail.ExtendedDetail)[3].name, megaEvent.details(Detail.ExtendedDetail)[3].name)
        compare(importedEvent.details(Detail.ExtendedDetail)[3].data, megaEvent.details(Detail.ExtendedDetail)[3].data)
        compare(importedEvent.detail(Detail.Comment).comment, megaEvent.detail(Detail.Comment).comment)
        compare(importedEvent.detail(Detail.AudibleReminder).repetitionCount, megaEvent.detail(Detail.AudibleReminder).repetitionCount)
        compare(importedEvent.detail(Detail.AudibleReminder).repetitionDelay, megaEvent.detail(Detail.AudibleReminder).repetitionDelay)
        compare(importedEvent.detail(Detail.AudibleReminder).secondsBeforeStart, megaEvent.detail(Detail.AudibleReminder).secondsBeforeStart)
        compare(importedEvent.detail(Detail.AudibleReminder).dataUrl, megaEvent.detail(Detail.AudibleReminder).dataUrl)
        compare(importedEvent.detail(Detail.EmailReminder).repetitionCount, megaEvent.detail(Detail.EmailReminder).repetitionCount)
        compare(importedEvent.detail(Detail.EmailReminder).repetitionDelay, megaEvent.detail(Detail.EmailReminder).repetitionDelay)
        compare(importedEvent.detail(Detail.EmailReminder).secondsBeforeStart, megaEvent.detail(Detail.EmailReminder).secondsBeforeStart)
        compare(importedEvent.detail(Detail.EmailReminder).body, megaEvent.detail(Detail.EmailReminder).body)
        compare(importedEvent.detail(Detail.EmailReminder).subject, megaEvent.detail(Detail.EmailReminder).subject)
        compare(importedEvent.detail(Detail.EmailReminder).recipients, megaEvent.detail(Detail.EmailReminder).recipients)
        compare(importedEvent.detail(Detail.EmailReminder).attachments, megaEvent.detail(Detail.EmailReminder).attachments)
        compare(importedEvent.detail(Detail.VisualReminder).repetitionCount, megaEvent.detail(Detail.VisualReminder).repetitionCount)
        compare(importedEvent.detail(Detail.VisualReminder).repetitionDelay, megaEvent.detail(Detail.VisualReminder).repetitionDelay)
        compare(importedEvent.detail(Detail.VisualReminder).secondsBeforeStart, megaEvent.detail(Detail.VisualReminder).secondsBeforeStart)
        compare(importedEvent.detail(Detail.VisualReminder).message, megaEvent.detail(Detail.VisualReminder).message)
        compare(importedEvent.detail(Detail.VisualReminder).dataUrl, megaEvent.detail(Detail.VisualReminder).dataUrl)

        expectFail("", "During import the timestamp information gets overwritten with current datetime")
        compare(importedEvent.detail(Detail.Timestamp).created, megaEvent.detail(Detail.Timestamp).created)
        expectFail("", "During import the timestamp information gets overwritten with current datetime")
        compare(importedEvent.detail(Detail.Timestamp).lastModified, megaEvent.detail(Detail.Timestamp).lastModified)
        compare(importedEvent.detail(Detail.Version).version, megaEvent.detail(Detail.Version).version)
        compare(importedEvent.detail(Detail.Version).extendedVersion, megaEvent.detail(Detail.Version).extendedVersion)

        memoryModel.removeItem(memoryModel.items[0])
        memoryModelSpy.wait()
    }

}
