/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Pim Module.
**
** $QT_BEGIN_LICENSE:FDL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Free Documentation License Usage
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.  Please review the following information to ensure
** the GNU Free Documentation License version 1.3 requirements
** will be met: http://www.gnu.org/copyleft/fdl.html.
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.0
// ![Custom DetailsEditWidget is used for entering new or modifying existing contact detlais]
Item {
    width: parent.width - 30
    height: 20

    property alias label: label.text
    property alias value: valueText.text
    property alias color: valueText.color
    property alias inputFocus: valueText.focus
    property bool isPreferred: false
    property bool requestPreferred: false
    property bool showPreferredField: false

    property string old

    Text {
        id: label
        font {
            family: "Helvetica"
            pixelSize: 15
            bold: true
            italic: true
        }
    }

    Rectangle {
        id: inputField
        width: 180
        height: 20
        anchors.left: label.right
        anchors.leftMargin: 6
        color: "white"
        border {
            width: 2
            color: "darkgreen"
        }

        TextInput {
            id: valueText
            anchors.fill: parent
            anchors.leftMargin: 5
            font {
                pixelSize: 16
                family: "Helvetica"
            }
            onFocusChanged: {
                if (focus === true) { // when entering text field
                    old = valueText.text
                } else { // when exiting text field
                    if (valueText.text !== old) {
                        valueText.color = "red"
                    }
                }
            }
        }
    }

    Text {
        id: labelFav

        anchors.left: inputField.right
        anchors.leftMargin: 5

        font {
            family: "Helvetica"
            pixelSize: 15
            bold: true
            italic: true
        }
        text: "favorite:"
        visible: showPreferredField
    }

    Rectangle {
        anchors.left: labelFav.right
        anchors.leftMargin: 5
        anchors.verticalCenter: labelFav.verticalCenter
        width: 10
        height: 10
        color: isPreferred || requestPreferred ? "black" : "white"
        visible: showPreferredField

        MouseArea {
            anchors.fill: parent
            onClicked: requestPreferred = !requestPreferred
        }
    }
}
// ![Custom DetailsEditWidget is used for entering new or modifying existing contact detlais]
