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
import QtJsonDb 1.0

Partition {
    id: jsonDb

    property alias partition: jsonDb.name

    function createAndSignal(object) {
        logDebug("createAndSignal()");
        create(object, resultCallback);
    }

    function removeAndSignal(object) {
        logDebug("removeAndSignal()");
        remove(object, resultCallback);
    }

    function updateAndSignal(object) {
        logDebug("updateAndSignal()");
        update(object, resultCallback);
    }

    property variant queryObject

    function queryAndSignal(querystr) {
        logDebug("queryAndSignal()");
        queryObject = createQuery(querystr, -1, {}, jsonDb);
        queryObject.finished.connect(queryResultCallback);
        queryObject.start();
    }

    signal operationFinished

    property variant lastResult: {}

    function resultCallback(error, result) {
        if (error) {
            console.log("resultCallback(): error: code " + error.code + ": message: " + error.message);
            return;
        }

        logDebug("resultCallback(): result received: " + JSON.stringify(result));
        lastResult = result.items;
        operationFinished();
    }

    function queryResultCallback() {
        var result = queryObject.takeResults();
        logDebug("queryResultCallback(): result received: " + JSON.stringify(result));
        lastResult = result;
        operationFinished();
    }

    property bool debug: false

    function logDebug(message) {
        if (debug)
            console.log('SignalingJsonDb.' + message);
    }
}

