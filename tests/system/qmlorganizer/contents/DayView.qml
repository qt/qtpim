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

Flickable
{
     Rectangle {
         id : dayView

         anchors.fill : parent
         opacity : parent.opacity
         color: "#9eaf30"
         gradient: Gradient {
             GradientStop {
                 position: 0.00;
                 color: "#9eaf30";
             }
             GradientStop {
                 position: 0.89;
                 color: "#ffffff";
             }
         }

         ListView {
             id : hourList
             model : hourModel
             anchors.fill: parent
             clip: true
             focus: true
             opacity : parent.opacity

             delegate : hourDelegate
             highlight: hourHighlight
             preferredHighlightBegin: hourList.height * 0.5
             preferredHighlightEnd: preferredHighlightBegin
             highlightFollowsCurrentItem : true
             highlightMoveSpeed : 2000
             keyNavigationWraps : true

             onCurrentIndexChanged : {
                 if (timelineView.opacity > 0) {
                     calendar.currentDate = new Date(timelineView.year, timelineView.month, currentIndex + 1);
                     monthList.currentIndex = timelineView.month;
                     currentIndex = timelineView.day - 1;
                 }
             }

         }
         Component {
             id: hourHighlight
             Rectangle {
                 width: hourList.width;
                 height: hourList.height /7 ;
                 color: "lightsteelblue" ;radius: 5
             }
         }

         Component {
             id: hourDelegate


             Item {
                 width : hourList.width
                 height : childrenRect.height
                 property int rowIndex : index
                 id:hourDelegateInstanceItem

                 Column {
                     // Draw a line under the previous Hour list tiem
                     Rectangle {
                         height : 1
                         width : hourList.width
                         color : "black"
                     }

                     Text {
                        // text: hour
                         text: index + ":00"
                     }


                     // List all, if any, of the events within this hour.
                     Repeater {

                         focus: true

                         // Simple fetch ALL events on this day...and we will filter them bu hour.
                         model: calendar.organizer.items? calendar.organizer.itemsByTimePeriod(new Date(calendar.year, calendar.month, calendar.day)
                                                                                     , new Date(calendar.year, calendar.month, calendar.day+1))
                                                        : 0

                         Row {
                             spacing:  4
                             Text {
                                 id: itemText
                                 clip: true
                                 focus: true

                                 // Only display a link when the event starts within this hour......
                                 text: (hourDelegateInstanceItem.rowIndex == Qt.formatTime(modelData.itemStartTime, "hh")) ? "<a href=\"#\">" + modelData.displayLabel + "</a>":""
                                 onLinkActivated: {
                                     detailsView.isNewItem = false;
                                     detailsView.item = modelData;
                                     if (detailsView.item.itemType == Type.EventOccurrence || detailsView.item.itemType == Type.TodoOccurrence)
                                         calendar.state = "OccurrenceDialogView";
                                     else
                                         calendar.state = "DetailsView";
                                 }
                             }
                             Rectangle {
                                 width: 15; height: 15
                                 anchors { verticalCenter: parent.verticalCenter }
                                 border { color: "black"; width: 1; }
                                 visible: (hourDelegateInstanceItem.rowIndex == Qt.formatTime(modelData.itemStartTime, "hh")) ? true : false
                                 color: calendar.organizer.collection(modelData.collectionId)? calendar.organizer.collection(modelData.collectionId).color : "red"
                             }
                         }
                     }
                 }
             }

         }

         ListModel {
             id : hourModel
             ListElement {hour : "0:00"}
             ListElement {hour : "1:00"}
             ListElement {hour : "2:00"}
             ListElement {hour : "3:00"}
             ListElement {hour : "4:00"}
             ListElement {hour : "5:00"}
             ListElement {hour : "6:00"}
             ListElement {hour : "7:00"}
             ListElement {hour : "8:00"}
             ListElement {hour : "9:00"}
             ListElement {hour : "10:00"}
             ListElement {hour : "11:00"}
             ListElement {hour : "12:00"}
             ListElement {hour : "13:00"}
             ListElement {hour : "14:00"}
             ListElement {hour : "15:00"}
             ListElement {hour : "16:00"}
             ListElement {hour : "17:00"}
             ListElement {hour : "18:00"}
             ListElement {hour : "19:00"}
             ListElement {hour : "20:00"}
             ListElement {hour : "21:00"}
             ListElement {hour : "22:00"}
             ListElement {hour : "23:00"}
         }
     }
}
