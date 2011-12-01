
/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the documentation of the Qt Organizer Module.
**
** $QT_BEGIN_LICENSE:FDL$
** GNU Free Documentation License
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms
** and conditions contained in a signed written agreement between you
** and Nokia.
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.0

//![Generic Button]
//![Create Button Rectangle]
Rectangle {
    radius: 5
    height: buttonLabel.height
    color: "black"
//![Create Button Rectangle]

//![Create Button Border]
    border {
        width: 1
        color: "black"
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
