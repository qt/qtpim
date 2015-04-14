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
import QtOrganizer 5.0

Rectangle
{
    id:weekView

    anchors.fill: parent
    ListView {
        id : dayList
        anchors.fill: parent
        clip: true
        focus: true
        opacity : parent.opacity
        preferredHighlightBegin: dayList.height * 0.5
        preferredHighlightEnd: preferredHighlightBegin
        highlightFollowsCurrentItem : true
        highlightMoveSpeed : 2000
        keyNavigationWraps : true
        Component.onCompleted : positionViewAtIndex(currentIndex, ListView.Beginning)
        onOpacityChanged: {
                currentIndex = calendar.weekDay;
         }

        model : ListModel {
                ListElement {day : "Sunday"}
                ListElement {day : "Monday"}
                ListElement {day : "Tuesday"}
                ListElement {day : "Wednesday"}
                ListElement {day : "Thursday"}
                ListElement {day : "Friday"}
                ListElement {day : "Saturday"}
         }

        delegate:  dayDelegate

        Component {
            id: dayDelegate
            Item {
                width : dayList.width
                height : childrenRect.height
                Column {
                    Rectangle {
                        height : 1
                        width : dayList.width
                        color : "black"
                    }
                    Text {
                        text: day
                    }
                    Repeater {
                        focus: true
                        model: calendar.organizer.items? calendar.organizer.itemsByTimePeriod(new Date(calendar.year,calendar.month, index - calendar.weekDay + calendar.day),
                                                         new Date(calendar.year,calendar.month, index - calendar.weekDay + calendar.day + 1))
                                                       : 0
                        Text {
                            clip: true
                            focus: true
                            text: "a <a href=\"#\">" + modelData.displayLabel + "</a>"
                            onLinkActivated: {
                                detailsView.isNewItem = false;
                                detailsView.item = modelData;
                                if (detailsView.item.itemType == Type.EventOccurrence || detailsView.item.itemType == Type.TodoOccurrence)
                                    calendar.state = "OccurrenceDialogView";
                                else
                                    calendar.state = "DetailsView";
                            }
                        }
                    }
                }
            }
        }


        highlight:  Component {
            Rectangle {
                width: dayList.width
                height: dayList.height /7
                color: "lightsteelblue"
                radius: 5
            }
        }

    }
}

