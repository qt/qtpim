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

//![Generic Button]
//![Create Button Rectangle]
Rectangle {
    //radius: 5
    height: buttonLabel.height
    color: "black"
//![Create Button Rectangle]

//![Create Button Border]
    border {
        width: 1
        color: "darkred"
    }
//![Create Button Border]

//![Button Clicked]
    signal clicked
    property alias buttonText: buttonLabel.text
//![Button Clicked]

//![Button Text]
    Text {
        id: buttonLabel
        font.pixelSize: 12
        anchors.centerIn: parent
        color: "white"
    }
//![Button Text]

//![Setup Button Mouse Area]
    MouseArea {
        anchors.fill: parent
        onPressed: {
            parent.color = "white"
        }
        onReleased: {
            parent.color = "black"
            parent.clicked()
        }
    }
//![Setup Button Mouse Area]

}
//![Generic Button]
