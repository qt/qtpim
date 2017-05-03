/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
import "datetimerollercontents"

Item {
    property variant value
    property variant valueSet
    property alias label: nameField.text
    property alias valueRoller : valueRoller
    property alias currentIndex : valueSpinner.currentIndex
    property alias spinnerDelegate : valueSpinner.delegate

    signal rollerChanged

    height: 100
    width: detailsView.width

    onValueChanged: {
        valueSpinner.currentIndex = value
    }

    Text {
        id: nameField
        width: parent.width * 0.4
        anchors { left: parent.left; margins: 3 }
        font.bold: true
        color: "white"
    }

    Rectangle {
        id: valueRoller

        width: roller.width
        height: roller.height
        anchors { left: nameField.right; margins: 3}
        color: "white";
        border { width: 3; color: "white" }
        radius: 5

        Grid {
            id: roller
            spacing: 2
            columns: 1
            Spinner {
                id: valueSpinner
                width: 150
                model: valueSet.length //10
                delegate: Text { text: valueRoller.valueName(index); }
                onCurrentIndexChanged: rollerChanged()
            }
        }

        function valueName(valueIndex) {
            if (valueSet.length > valueIndex)
                return valueSet[valueIndex];
            else
                return valueIndex;
        }

        // component API

        // use this to read selected datetime
        function selectedValue() {
            return valueSpinner.currentIndex;
        }
    }
}
