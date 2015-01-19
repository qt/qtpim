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
