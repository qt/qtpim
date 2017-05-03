/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Mobility Components.
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

Item {
    id: statusbar
    property string status
    signal leftClicked
    signal rightClicked
    signal addClicked
    signal managerChangeClicked

    BorderImage { source: "images/titlebar.sci"; width: parent.width; height: parent.height + 14; y: -7 }

    Button {//add new item button
        id: addButton
        text: "+"
        anchors.left: parent.left; anchors.leftMargin: 2; y: 3; width: 30; height: 32
        onClicked: statusbar.addClicked()
    }

    Button {
        id: leftButton
        text: "<"
        anchors.left: addButton.right; anchors.leftMargin: 2; y: 3; width: 30; height: 32
        onClicked: statusbar.leftClicked()
    }
    Text {
        id:statusText
        color: "#ecc70a"
        text:status
        font.family: "Monospace"
        font.bold: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 6
        anchors.left: leftButton.right
        anchors.leftMargin: 2
        anchors.right: rightButton.left
        anchors.rightMargin: 2
        y: 5

    }

    Button {
        id: rightButton
        text: ">"
        anchors.right: todayButton.left; anchors.rightMargin: 2; y: 3; width: 30; height: 32
        onClicked: statusbar.rightClicked()
    }

    Button {
        //change the current Date to Today
        id: todayButton;
        anchors.right: parent.right; anchors.leftMargin: 2; y: 3; width: 50; height: 32
        text: "Today";
            onClicked: {
                var tempdate = new Date();
                calendar.currentDate = new Date(tempdate.getFullYear(), tempdate.getMonth(), tempdate.getDate());
            }
    }


}
