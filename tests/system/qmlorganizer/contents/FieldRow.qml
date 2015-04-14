/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

Item {
    property alias label: nameField.text
    property variant value
    property alias newValue: textEdit.text

    height: 50
    width: detailsView.width

    Text {
        id: nameField
        width: parent.width * 0.4
        anchors { left: parent.left; margins: 3 }
        font.bold: true
        color: "white"
    }
    Rectangle {
        id: editorField
        anchors { left: nameField.right; right:  parent.right; margins: 3 }
        height: parent.height - nameField.height;
        color: textEdit.activeFocus ? "white" : "lightgray";
        border { width: 3; color: "white" }
        radius: 5
        opacity: 0.95
        TextEdit {
            id: textEdit
            anchors { fill: parent; margins: 3}
            width: parent.width
            text: value ? value.toString() : ""
            wrapMode: TextEdit.Wrap
        }
    }

    function setValue(initialValue) {
        if (textEdit.text != initialValue) {
            textEdit.text = initialValue;
        }
    }

}
