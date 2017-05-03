/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtPim module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

TestCase {
    name: "ContactsSavingTestCase"
    id: contactsSavingTestCase

    property SignalSpy spy
    property SignalSpy collectionSpy
    property bool debug: false

    ContactsTestConfiguration {
        id: configuration
    }

    function getManagerUnderTest() {
        var managerName = configuration.getManagerUnderTest();
        logDebug("ContactsSavingTestCase: Testing against '" + managerName + "' manager backend.");
        return managerName;
    }

    function initTestForModel(model) {
        logDebug("initTestForModel");
        spy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsSavingTestCase);
        spy.target = model;
        spy.signalName = "contactsChanged";

        collectionSpy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsSavingTestCase);
        collectionSpy.target = model
        collectionSpy.signalName = "collectionsChanged"

        return spy;
    }

    function listenToContactsChanged() {
        logDebug("listenForContactsChanged");
        spy.clear();
    }

    // Verify that the contactsChanged signal is emitted
    function waitForContactsChanged() {
        logDebug("waitForContactsChanged");
        spy.wait();
    }

    // Verify that the collectionsChanged signal is emitted
    function waitForCollectionsChanged() {
        logDebug("waitForCollectionsChanged");
        collectionSpy.wait();
    }


    // Wait until duration has elapsed, or the contactsChanged signal is emitted
    function waitUntilContactsChanged(duration) {
        logDebug("waitUntilContactsChanged");
        if (spy.count == 0) {
            if (duration == undefined)
                duration = 500;
            while (duration > 0) {
                wait(50);
                if (spy.count) {
                    spy.clear();
                    return;
                }
                duration -= 50;
            }
        } else {
            spy.clear();
        }
    }

    function verifyNoContactsChangedReceived() {
        logDebug("verifyNoContactsChangedReceived");
        wait(500);
        compare(spy.count, 0, "no contacts changed signal received");
    }

    function initTestForTargetListeningToSignal(target, signalName)
    {
        logDebug("initTestForTargetListeningToSignal");
        var spy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsSavingTestCase);
        spy.target = target;
        spy.signalName = signalName;
        return spy;
    }

    function waitForTargetSignal(spy) {
        logDebug("waitForTargetSignal");
        spy.wait();
    }

    function emptyContacts(model) {
        logDebug("emptyContacts");
        if (!model.autoUpdate)
            model.update();
        var count = model.contacts.length;
        if (count == 0)
            return;
        for (var i = 0; i < count; i++) {
            var id = model.contacts[0].contactId;
            model.removeContact(id);
            if (!model.autoUpdate)
                model.update()
            spy.wait();
            spy.clear();
        }
        compare(model.contacts.length, 0, "model is empty");
    }

    function finishTestForModel(model) {
        logDebug("finishTestForModel");
        model.autoUpdate = false;
    }

    function logDebug(message) {
        if (debug)
            console.log(message);
    }

    Component {
        id: emptyContactComponent
        Contact {}
    }

    function createEmptyContact()
    {
        return emptyContactComponent.createObject()
    }
}
