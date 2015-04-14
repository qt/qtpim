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
import "contents"

Rectangle {
         id: calendar
         width: 400
         height: 640
         property date currentDate:new Date()
         property int year:currentDate.getFullYear()
         property int month:currentDate.getMonth()
         property int day:currentDate.getDate()
         property int hour:currentDate.getHours()
         property int weekDay:currentDate.getDay()
         property string status:currentDate.toDateString()
         property string preState: "MonthView"
         property alias contentArea : contentArea

        color: "#343434";
        Image { source: "contents/images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 1 }

        state: "MonthView";

        CollectionFilter {
            id: modelCollectionFilter
            ids: {
                var idList = [];
                var list = organizer.collections;
                for (var i = 0; i< list.length; i++) {
                    idList.push(list[i].collectionId);
                }
                return idList;
            }
        }

        DetailFieldFilter {
            id: todoFilter
            detail: Detail.ItemType
            field: Type.FieldType
            value: Type.Todo
        }

        IntersectionFilter {
            id: intersectionFilter
            filters: [modelCollectionFilter]
        }

        SystemPalette { id: activePalette }
        property OrganizerModel organizer:OrganizerModel{
            id: organizer
            manager:"qtorganizer:memory:id=qml"
            startPeriod:'2009-01-01'
            endPeriod:'2012-12-31'
            autoUpdate:true
            filter: intersectionFilter
            Component.onCompleted : {
                if (managerName == "memory")
                    organizer.importItems(Qt.resolvedUrl("contents/test.ics"));
            }
            onItemsFetched: {
                // this is for occurrenceDialog
                console.log("QML --- ITEMS FETCHED" + fetchedItems[0].displayLabel + fetchedItems[0].itemStartTime);
                if (fetchedItems.length == 1) {
                    detailsView.isNewItem = false;
                    detailsView.item = fetchedItems[0];
                    calendar.state = "DetailsView";
                }
            }
        }

        InfoBar {
            id: infoBar;
            anchors {
                left: parent.left; right: parent.right; top: parent.top
            }
            height: 20
        }

        MenuBar {
            id: menuBar;
            anchors {
                left: parent.left; right: parent.right; top: infoBar.bottom
            }
            height: 35; opacity: 0.9
            info: organizer.error + "\nTotal:" + organizer.itemCount
        }

        StatusBar {
            id: statusBar; status:calendar.status; width: parent.width; height: 35; opacity: 0.9; anchors.bottom: calendar.bottom
            onLeftClicked: {
                if (calendar.state == "MonthView") {
                    calendar.currentDate = new Date(calendar.year, calendar.month - 1, calendar.day);
                } else if (calendar.state == "WeekView") {
                    calendar.currentDate = new Date(calendar.year, calendar.month , calendar.day - 7);
                } else if (calendar.state == "DayView" || calendar.state == "TimelineView") {
                    calendar.currentDate = new Date(calendar.year, calendar.month , calendar.day - 1);
                }

            }

            //rightClick
            onRightClicked: {
                if (calendar.state == "MonthView") {
                    calendar.currentDate = new Date(calendar.year, calendar.month + 1, calendar.day);
                } else if (calendar.state == "WeekView") {
                    calendar.currentDate = new Date(calendar.year, calendar.month , calendar.day + 7);
                } else if (calendar.state == "DayView" || calendar.state == "TimelineView") {
                    calendar.currentDate = new Date(calendar.year, calendar.month , calendar.day + 1);
                }
            }

            //add new item clicked
            onAddClicked : {
                calendar.preState = calendar.state;
                if (calendar.state == "SettingsView" && settingsView.state == "CollectionManagerView") {
                    settingsView.collectionManagerView.addCollection();
                } else {
                    calendar.state = "AddNewItemSelectView";
                }
            }
        }

        states: [
            State {name: "MonthView"; PropertyChanges { target: monthView; opacity: 1; }},
            State {name: "TimelineView"; PropertyChanges { target: timelineView; opacity: 1; }},
            State {name: "WeekView"; PropertyChanges { target: weekView; opacity: 1; }},
            State {name: "DayView"; PropertyChanges { target: dayView; opacity: 1; }},
            State {name: "AgenderView"; PropertyChanges { target: agenderView; opacity: 1; }},
            State {
                name: "DetailsView";
                PropertyChanges { target: detailsView; opacity: 1;}
                PropertyChanges { target: statusBar; opacity: 0; }
            },
            State {name: "AddNewItemSelectView"; PropertyChanges { target: addNewItemview; opacity: 0.8; }},
            State {name: "OccurrenceDialogView"; PropertyChanges { target: occurrenceDialog; opacity: 0.8; }},
            State {name: "TodoView"; PropertyChanges { target: todoView; opacity: 1; }},
            State {name: "AttendeeDetailsView"; PropertyChanges { target: attendeeDetailsView; opacity: 1; }},
            State {name: "SettingsView"; PropertyChanges { target: settingsView; opacity: 1; }}
        ]
        transitions: [
            Transition {
                NumberAnimation {
                    properties: "opacity"
                    easing.type: "Linear"
                    duration: 10
                }
            }
        ]

        // some views are based on certain filters
        onStateChanged: {
            if (state == "TodoView") {
                intersectionFilter.filters = [todoFilter, modelCollectionFilter];
            } else if (intersectionFilter.filters.length != 1) {
                // No need to change the filter if filter is the same
                // (Currently changing the filter triggers also a full update.)
                intersectionFilter.filters = [modelCollectionFilter];
            }
        }

        Item {
            id: contentArea;
            anchors.top: menuBar.bottom;
            anchors.left: calendar.left;
            anchors.right: calendar.right;
            anchors.bottom: (statusBar.opacity != 0) ? statusBar.top : statusBar.bottom;

            MonthView {
                id: monthView;
                width: calendar.width;
                height: calendar.height - menuBar.height - statusBar.height;
                opacity: 0;
                month:calendar.month
                year:calendar.year
                anchors.fill: contentArea;
            }
            TimelineView {
                id: timelineView;
                width: calendar.width;
                height: calendar.height - menuBar.height - statusBar.height;
                opacity: 0;
                anchors.fill: contentArea;
            }
            WeekView {
                id: weekView;
                width: calendar.width;
                height: calendar.height - menuBar.height - statusBar.height;
                opacity: 0;
                anchors.fill: contentArea;
            }
            DayView {
                id: dayView;
                width: calendar.width;
                height: calendar.height - menuBar.height - statusBar.height;
                opacity: 0;
                anchors.fill: contentArea;
            }

            AgenderView {
                id: agenderView;
                width: calendar.width;
                height: calendar.height - menuBar.height - statusBar.height;
                opacity: 0;
                anchors.fill: contentArea;
            }
            DetailsView {
                id: detailsView;
                width: calendar.width;
                height: calendar.height - menuBar.height - statusBar.height;
                opacity: 0;
                anchors.fill: contentArea;
            }
            SelectionView {
                id: addNewItemview;
                title: "Select type:"

                model: VisualItemModel {
                    Button {
                        text: "New event"
                        width: addNewItemview.width / 2
                        onClicked: {
                            detailsView.isNewItem = true;
                            detailsView.item = createEmptyItem(Type.Event);
                            detailsView.item.displayLabel = "Event"+(organizer.itemCount+1)
                            calendar.state = "DetailsView";
                        }
                    }
                    Button {
                        text: "New todo-item"
                        width: addNewItemview.width / 2
                        onClicked: {
                            detailsView.isNewItem = true;
                            detailsView.item = createEmptyItem(Type.Todo);
                            detailsView.item.displayLabel = "Todo"+(organizer.itemCount+1)
                            calendar.state = "DetailsView";
                        }
                    }
                    Button {
                        text: "Cancel"
                        width: addNewItemview.width / 2
                        onClicked: {
                            calendar.state = calendar.preState;
                        }
                    }

                }
            }
            SelectionView {
                id: occurrenceDialog;
                //title: "This is a recurring item. Open this instance or whole series?"
                title: "Recurring item"

                model: VisualItemModel {
                    Button {
                        text: "Open this instance"
                        width: addNewItemview.width / 2
                        onClicked: {
                            calendar.state = "DetailsView";
                        }
                    }
                    Button {
                        text: "Open whole series"
                        width: addNewItemview.width / 2
                        onClicked: {
                            var parentDetail = detailsView.item.detail(Detail.Parent);
                            organizer.fetchItems([parentDetail.parentId]);
                        }
                    }
                }
            }
            TodoView {
                id: todoView;
            }
            AttendeeDetailsView {
                id: attendeeDetailsView;
            }
            SettingsView {
                id: settingsView;
            }
        }

        function createEmptyItem(type)
        {
            if (type == Type.Event) {
                return Qt.createQmlObject("import QtOrganizer 5.0; Event { }", organizer);
            } else if (type == Type.Todo) {
                return Qt.createQmlObject("import QtOrganizer 5.0; Todo { }", organizer);
    //        } else if (type == Type.EventOccurrence) {
    //            return Qt.createQmlObject("import QtOrganizer 5.0; EventOccurrence { }", organizer);
    //        } else if (type == Type.TodoOccurrence) {
    //            return Qt.createQmlObject("import QtOrganizer 5.0; TodoOccurrence { }", organizer);
            } else {
                return Qt.createQmlObject("import QtOrganizer 5.0; Event { }", organizer);
            }
        }
}
