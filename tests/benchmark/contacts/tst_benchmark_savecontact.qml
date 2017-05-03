/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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


ContactsBenchmarkTestCase {

    name: "ContactsSaveBenchmarkTests"
    id: contactsSaveBenchmarkTests
    property SignalSpy contactsChangedSpy
    property variant contactsList

    ContactModel {
        id: model
        manager:"memory";
        autoUpdate: true
    }

    SignalSpy {
        id: spy
        signalName: "contactsChanged"
        target: model
    }

    function initTestCase() {
        createContacts(100);
    }

    function cleanup() {
        var amt = model.contacts.length
        for (var i = 0; i < amt; i++) {
            var id = model.contacts[0].contactId
            model.removeContact(id)
            spy.wait()
        }
        compare(model.contacts.length, 0, "Failed to clean model")
    }

    function benchmark_saveContacts() {
        for (var it=0 ; it<contactsList.length;it++) {
            model.saveContact(contactsList[it]);
            spy.wait()
        }
    }
}

