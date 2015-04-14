/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Pim Module.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
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
