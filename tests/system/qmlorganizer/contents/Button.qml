/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Mobility Components.
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

// These buttons are very ugly
Rectangle {
    id: container

    signal clicked
    property string image: ""
    property string text: ""
    property int padding: 2

    color: activePalette.button;
    smooth: true
    border.width: 1;
    border.color: Qt.darker(activePalette.dark);
    radius: 2;


    gradient: Gradient {
        GradientStop { position: 0.0; color: Qt.lighter(activePalette.button) }
        GradientStop { position: 1.0; color : mr.pressed ?  Qt.lighter(activePalette.light) : Qt.darker(activePalette.dark) }
    }
    width: stuff.width + 4 * padding
    height: stuff.height + 2 * padding

    Item {
        id: stuff
        width: childrenRect.width;
        height: childrenRect.height;
        y: padding
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            Image {
                id: imgItem;
                smooth: true
                width: source != "" ? 16 : 0; height: source != "" ? 16 : 0;
                source: container.image;
                opacity: source == "" ? 0 : 1;
            }

            Text {
                horizontalAlignment: Text.AlignHCenter
                id: txtItem; text: container.text; font.pixelSize: 14; color: activePalette.buttonText
            }
        }
    }

    // Don't make mr part of the size determination, since it uses anchors.fill
    MouseArea { id: mr; anchors.fill: parent; onClicked: container.clicked() }

}
