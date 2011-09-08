/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
            text: value
            wrapMode: TextEdit.Wrap
        }
    }

    function setValue(initialValue) {
        if (textEdit.text != initialValue) {
            textEdit.text = initialValue;
        }
    }

}
