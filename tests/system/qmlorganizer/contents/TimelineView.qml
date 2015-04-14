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
import "timeline.js" as Timeline

Rectangle {
    id : timelineView
    anchors.fill : parent
    opacity : parent.opacity
    property int year:calendar.year
    property int month:calendar.month
    property int day:calendar.day
    Connections {
        target: calendar
        onCurrentDateChanged : {
            dayList.currentIndex = day - 1;
            monthList.currentIndex = month;
            yearList.currentIndex = year - yearModel.start;
        }
    }

    //Day view
    Rectangle {
        id: dayView
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
        anchors.right : parent.right
        anchors.left : monthView.right
        anchors.top : parent.top
        anchors.bottom : parent.bottom
        opacity : parent.opacity

        ListView {
            id : dayList
            model : dayModel
            anchors.fill: parent
            clip: true
            focus: true
            opacity : parent.opacity

            delegate : dayDelegate
            highlight: dayHighlight
            preferredHighlightBegin: dayList.height * 0.5
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
            id: dayHighlight
            Rectangle {
                width: dayList.width;
                height: dayList.height /7 ;
                color: "lightsteelblue" ;radius: 5
            }
        }

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
                        model:calendar.organizer.items? calendar.organizer.itemsByTimePeriod(new Date(timelineView.year,timelineView.month, index + 1),
                                                                                      new Date(timelineView.year,timelineView.month, index + 2))
                                                      : 0

                        Text {
                            clip: true
                            focus: true
                            text: "<a href=\"#\">" + modelData.displayLabel + "</a>"
                            onLinkActivated: {
                                detailsView.isNewItem = false
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

        ListModel {
            id : dayModel
            ListElement {day : "1"}
            ListElement {day : "2"}
            ListElement {day : "3"}
            ListElement {day : "4"}
            ListElement {day : "5"}
            ListElement {day : "6"}
            ListElement {day : "7"}
            ListElement {day : "8"}
            ListElement {day : "9"}
            ListElement {day : "10"}
            ListElement {day : "11"}
            ListElement {day : "12"}
            ListElement {day : "13"}
            ListElement {day : "14"}
            ListElement {day : "15"}
            ListElement {day : "16"}
            ListElement {day : "17"}
            ListElement {day : "18"}
            ListElement {day : "19"}
            ListElement {day : "20"}
            ListElement {day : "21"}
            ListElement {day : "22"}
            ListElement {day : "23"}
            ListElement {day : "24"}
            ListElement {day : "25"}
            ListElement {day : "26"}
            ListElement {day : "27"}
            ListElement {day : "28"}
            ListElement {day : "29"}
            ListElement {day : "30"}
            ListElement {day : "31"}
        }
    }

    //Month view
    Rectangle {
        id:monthView
        color : "lightgray"
        width : 30
        anchors.left : yearView.right
        anchors.top : parent.top
        anchors.bottom : parent.bottom

        ListView {
            id : monthList
            model : monthModel
            anchors.fill: parent
            clip: true
            delegate : monthDelegate
            highlight: monthHighlight
            preferredHighlightBegin: monthList.height * 0.5
            preferredHighlightEnd: preferredHighlightBegin
            highlightFollowsCurrentItem : true
            highlightMoveSpeed : 1000
            Component.onCompleted : {
               var now = new Date();
               var month = now.getMonth();
               monthList.currentIndex = month;
               var  d = Date.parse("Feb 31, 2010");
            }
            onCurrentIndexChanged : {
                if (timelineView.opacity > 0) {
                    calendar.currentDate = new Date(timelineView.year, currentIndex, timelineView.day);
                    currentIndex = timelineView.month;
                    dayList.currentIndex = timelineView.day - 1;
                }
            }
        }

        Component {
            id: monthHighlight
            Rectangle {
                width: monthList.width; height: monthList.height / 12 ; color: "lightsteelblue" ;radius: 5
            }
        }

        Component {
            id: monthDelegate
            Item {
                width : monthList.width
                height : monthList.height / 12
                Column {
                    Rectangle {
                        height : 1
                        width : monthList.width
                        color : "black"
                    }
                    Text {
                        text: month
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked : monthList.currentIndex = index
                }
            }
        }

        ListModel {
            id : monthModel
            ListElement {month : "Jan"}
            ListElement {month : "Feb"}
            ListElement {month : "Mar"}
            ListElement {month : "Apr"}
            ListElement {month : "May"}
            ListElement {month : "Jun"}
            ListElement {month : "Jul"}
            ListElement {month : "Aug"}
            ListElement {month : "Sep"}
            ListElement {month : "Oct"}
            ListElement {month : "Nov"}
            ListElement {month : "Dec"}
        }
    }

    //Year view
    Rectangle {
        id:yearView
        color : "gray"
        anchors.top : parent.top
        anchors.bottom : parent.bottom
        anchors.left : parent.left
        width : 50


        Component {
            id: yearHighlight
            Rectangle {
                width: yearList.width; height: yearList.height / 10; color: "lightsteelblue" ;radius: 5
            }
        }

        Component {
            id: yearDelegate

            Item {
                width : yearList.width
                height : yearList.height / 10
                Column {
                    Rectangle {
                        height : 1
                        width : yearList.width
                        color : "black"
                    }
                    Text {
                        text: year
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked : yearList.currentIndex = index
                }
            }
        }


        ListView {
            id : yearList
            model : yearModel
            delegate : yearDelegate
            anchors.fill: parent
            clip: true
            highlight: yearHighlight

            preferredHighlightBegin: yearList.height * 0.5
            preferredHighlightEnd: preferredHighlightBegin
            highlightRangeMode: "StrictlyEnforceRange"
            highlightFollowsCurrentItem : true

            Component.onCompleted: Timeline.extendYearModel(true);
            onCurrentIndexChanged: {
                Timeline.extendYearModel(false);
                if (timelineView.opacity > 0) {
                    calendar.currentDate = new Date(yearModel.start + currentIndex,  timelineView.month, timelineView.day);
                    monthList.currentIndex = timelineView.month;
                    dayList.currentIndex = timelineView.day - 1;
                }
            }

        ListModel {
            id : yearModel
            property int start : 0;
            property int end : 0;

            ListElement {
                year : 0;
            }
        }
    }
    }
}
