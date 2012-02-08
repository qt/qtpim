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
    id: contactsJsonDbPartitionTestHelper

    function initTestHelper() {
        logDebug("initTestHelper()");
        createSpyForJsonDb();
    }

    function createPartitionToJsonDb(partitionName) {
        logDebug("createPartitionToJsonDb()");

        var query = '[?_type="Partition"]' +
                '[?name="' + partitionName + '"]';
        jsonDb.queryAndSignal(query);
        jsonDbSpy.wait();
        if (jsonDb.lastResult.length > 0)
            return;

        var partition = {
            "_type": "Partition",
            "name": partitionName
        }
        jsonDb.createAndSignal(partition);
        jsonDbSpy.wait();
    }

    function removePartitionFromJsonDb(partitionName) {
        logDebug("removePartitionFromJsonDb()");
        // JsonDb does not allow updates to partitions at the moment
    }

    SignalingJsonDb {
        id: jsonDb
        name: "com.nokia.qtjsondb.System"
    }

    property SignalSpy jsonDbSpy

    function createSpyForJsonDb() {
        logDebug("createSpyForJson()");
        jsonDbSpy = Qt.createQmlObject(
                    "import QtTest 1.0;" +
                    "SignalSpy {" +
                    "}",
                    contactsJsonDbPartitionTestHelper);
        jsonDbSpy.target = jsonDb;
        jsonDbSpy.signalName = "operationFinished"
    }
}

