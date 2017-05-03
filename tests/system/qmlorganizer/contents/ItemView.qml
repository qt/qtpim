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

import QtOrganizer 5.0

Rectangle
{
    id:itemView
    property string  itemId
    property OrganizerItem item
    property int startTime
    property int endTime
    onItemIdChanged :{
        if (itemId != "") {
            item = calendar.organizer.item(itemId);
            startTime = item.itemStartTime.getHours() * 60 + item.itemStartTime.getMinutes();
            if (item.itemType == Type.Event)
                endTime = item.itemEndTime.getHours() * 60 + item.itemEndTime.getMinutes();
            else
                endTime = startTime;
            itemLabel.text = item.displayLabel;
            itemDesc.text = item.description;
        }
    }
    radius: 5
    color: Type.Event == item.itemType ? "steelblue" : "green"

    Column {
        spacing: 2
        Text { id: itemLabel; color: "yellow"; wrapMode: Text.Wrap;  font.bold: true; horizontalAlignment: Text.AlignHCenter; style: Text.Raised; verticalAlignment: Text.AlignVCenter; font.pointSize: 12 }
        Text { id: itemDesc; color: "white"; wrapMode: Text.Wrap;  font.pointSize: 10}
    }

    MouseArea {
        anchors.fill: parent
        onClicked : {
            detailsView.itemId = itemId
            detailsView.savedItem = true
            calendar.state = "DetailsView"
        }
    }
}
