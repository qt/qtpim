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
import "datetimerollercontents"

Rectangle
{
    id:detailsView
    anchors.fill: parent
    property bool isNewItem
    property variant item
    property variant rule
    property variant audibleReminderdetail

    onOpacityChanged: {
        // Is this view visible
        if (detailsView.opacity == 1) {
            // Initialize empty item
            if (isNewItem) {
                if (item.itemType == Type.Event || item.itemType == Type.EventOccurrence) {
                    item.startDateTime = new Date (calendar.year, calendar.month, calendar.day, 12);
                    item.endDateTime = new Date (calendar.year, calendar.month, calendar.day, 13);
                } else if (item.itemType == Type.Todo || item.itemType == Type.TodoOccurrence) {
                    item.startDateTime = new Date (calendar.year, calendar.month, calendar.day, 12);
                    item.dueDateTime = new Date (calendar.year, calendar.month, calendar.day, 13);
                }
            }
            // Initialize recurrence rule
            emptyRecurrenceRule.frequency = RecurrenceRule.Invalid;
            emptyRecurrenceRule.interval = 1;
            emptyRecurrenceRule.limit = null;

            // Initialize UI
            if (item.itemType == Type.Event || item.itemType == Type.EventOccurrence) {
                customLabelRow.setValue(item.displayLabel);
                customStartDateRow.dateTimeRoller.setDateTime(item.startDateTime);
                customEndDateRow.dateTimeRoller.setDateTime(item.endDateTime);
                customAlldayCheckBox.setValue(item.allDay);
                customDescriptionRow.setValue(item.description);
                customLocationRow.setValue(item.location);
                customCollectionRow.value = customCollectionRow.findCollectionArrayIndex(item.collectionId);
                customRsvpParticipationStatusRow.value = item.details(Detail.EventRsvp).length ?
                    item.detail(Detail.EventRsvp).participationStatus :
                    EventAttendee.StatusUnknown;
            } else if (item.itemType == Type.Todo || item.itemType == Type.TodoOccurrence) {
                todoCustomLabelRow.setValue(item.displayLabel);
                todoCustomDueDateRow.dateTimeRoller.setDateTime(detailsView.item.dueDateTime);
                todoCustomDescriptionRow.setValue(item.description);
                todoCustomPriorityRow.value = detailsView.item.priority;
                todoCollectionRow.value = todoCollectionRow.findCollectionArrayIndex(item.collectionId);
            }

            if (item.recurrence != undefined && item.recurrence.recurrenceRules.length > 0) {
                // Existing rule
                rule = item.recurrence.recurrenceRules[0];
                customRecurrenceRow.setValue(true);
            } else {
                // New rule
                rule = emptyRecurrenceRule;
                customRecurrenceRow.setValue(false);
            }
//            customInterval.setValue(rule.interval != 1);
            customRecurrenceLimitRow.setValue(rule != undefined && rule.limit != null);
            customLimitDateRow.dateTimeRoller.setDateTime((rule.limit == null || typeof rule.limit == "number")? null : rule.limit);

            //Audible reminder
            var audibleDetailList = item.details(Detail.AudibleReminder);
            if (audibleDetailList.length > 0) {
                audibleReminderdetail = item.detail(Detail.AudibleReminder);
                audibleReminderCheckBox.setValue(true);
            } else {
                audibleReminderdetail = emptyAudibleReminder;
                audibleReminderCheckBox.setValue(false);
            }
        }
    }

    RecurrenceRule {
        id: emptyRecurrenceRule
    }

    AudibleReminder {
        id: emptyAudibleReminder
        dataUrl: "http://www.qt.nokia.com"
    }

    Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.8 }
    color: "#343434";

    Rectangle {
        id: buttonRow
        anchors { bottom: parent.bottom; left: parent.left; right: parent.right;  }
        height: saveButton.height
        color:"#343434"
        Button {
            id: saveButton
            text: "Save & Exit"
            //anchors { top: parent.top; left: parent.left }
            anchors { top: parent.top; left: parent.left }
            width: parent.width / 2
            onClicked: {
                //save item
                saveButton.enabled = false;
                calendar.currentDate = item.startDateTime;
                if (item.detail(Detail.AudibleReminder)) {
                    item.setDetail(audibleReminderdetail);
                }
                var isEventType = (item.itemType == Type.Event || item.itemType == Type.EventOccurrence);
                calendar.organizer.saveItem(item);
                calendar.state = "DayView";
                saveButton.enabled = true;
                //"item" will be removed after saved without any signal notify.
                item = null;
            }
        }
        Button {
            id: deleteButton
            visible: !isNewItem
            text: "Delete"
            anchors { top: saveButton.top; left: saveButton.right }
            //anchors { top: parent.top; left: saveButton.right }
            width: parent.width / 2
            onClicked: {
                calendar.organizer.removeItem(item);
                calendar.state = "DayView";
            }
        }
    }

    ListView {
        anchors {top: parent.top; bottom: buttonRow.top; left: parent.left; right: parent.right }
        clip: true
        opacity: 0.8
        model: {
            if (item == undefined)
                return null;
            switch (item.itemType) {
                    case Type.Event:
                        //temporary comment because recurrence is not supported yet
                        // return eventItemModel;
                    case Type.EventOccurrence:
                         return eventOccurrenceItemModel;
                    case Type.Todo:
                         return todoItemModel;
                    default:
                         break;
            }
            return null;
        }
    }

    //event occurrence
    VisualItemModel {
        id:eventOccurrenceItemModel

        Text {
            width: detailsView.width - 6;
            height: 30
            text: "Event"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "White";
            font.weight: Font.Bold
        }
        FieldRow {
            id: customLabelRow
            label: "Title"
            onNewValueChanged: {
                item.displayLabel = customLabelRow.newValue;
            }
        }
        DateTimeRollerRow {
            id: customStartDateRow
            label: "Start Time"
            onRollerChanged: {
                detailsView.item.startDateTime = customStartDateRow.dateTimeRoller.selectedDateTime();
            }
        }
        DateTimeRollerRow {
            id: customEndDateRow
            label: "End Time"
            onRollerChanged: {
                item.endDateTime = customEndDateRow.dateTimeRoller.selectedDateTime();
            }
        }
        CheckBoxRow {
            id: customAlldayCheckBox
            label: "All day"
            onCheckBoxChanged: {
                item.allDay = customAlldayCheckBox.newValue
            }
        }
       FieldRow {
            id: customDescriptionRow
            label: "Description"
            height: 80
            onNewValueChanged: {
                item.description = customDescriptionRow.newValue;
            }
        }
       FieldRow {
            id: customLocationRow
            label: "Location"
            onNewValueChanged: {
                item.location = customLocationRow.newValue;
            }
        }

       CheckBoxRow {
           id: customRecurrenceRow
           label: "Repeated"
           onCheckBoxChanged: {
               if (customRecurrenceRow.newValue)
                   item.recurrence.recurrenceRules = [rule];
               else
                   item.recurrence.recurrenceRules = [];
           }
       }

//       CheckBoxRow {
//           id: customInterval
//           label: "Modify Interval"
//           visible: customRecurrenceRow.newValue
//           onCheckBoxChanged: {
//               if (customInterval.newValue)
//                   rule.interval = 15;
//               else
//                   rule.interval = 1;
//           }
//       }

       RollerRow {
           id: customFrequencyRow
           label: "Frequency"
           height: visible? 100 :0
           visible: customRecurrenceRow.newValue
           value: rule? rule.frequency : 0
           valueSet: ["Invalid", "Daily", "Weekly", "Monthly", "Yearly"]
           onRollerChanged: {
               rule.frequency = customFrequencyRow.valueRoller.selectedValue();
           }
       }

       CheckBoxRow {
           id: customRecurrenceLimitRow
           label: "Repeat limit"
           visible: customRecurrenceRow.newValue
           height: visible? 60 :0
       }

       DateTimeRollerRow {
           id: customLimitDateRow
           label: "Repeat until"
           visible: customRecurrenceRow.newValue && customRecurrenceLimitRow.newValue
           height: visible? 100 :0
           onRollerChanged: {
               if (customLimitDateRow.visible)
                   rule.limit = customLimitDateRow.dateTimeRoller.selectedDateTime();
           }
           onVisibleChanged: {
               if (!customRecurrenceLimitRow.newValue && rule)
                   rule.limit = null;
           }
       }

       CheckBoxRow {
           id: audibleReminderCheckBox
           label: "Reminder"
           onCheckBoxChanged: {
                if (newValue) {
                    audibleReminderdetail = emptyAudibleReminder;
                    //This function will create new detail
                    item.addDetail(audibleReminderdetail);
                } else {
                    var removeDetail = item.detail(Detail.AudibleReminder);
                    item.removeDetail(removeDetail);
                }
           }
       }
       Item {
           height: visible? 220 :0
           width: detailsView.width
           id: audibleReminderDataRow
           visible: audibleReminderCheckBox.newValue
           Text {
               id: audibleReminderDataRowText
               anchors.top: parent.top
               width: detailsView.width;
               height: 20
               text: "Reminder"
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
               color: "White";
               font.weight: Font.Bold
           }

           FieldRow {
               id: customAudibleReminderRepCountRow
               anchors.top: audibleReminderDataRowText.bottom
               label: "Repetition count"
               value: audibleReminderdetail? audibleReminderdetail.repetitionCount : 0
               onNewValueChanged: {
                   audibleReminderdetail.repetitionCount = newValue;
               }
           }
           FieldRow {
                id: customAudibleReminderRepDelayRow
                anchors.top: customAudibleReminderRepCountRow.bottom
                label: "Repetition delay"
                value: audibleReminderdetail? audibleReminderdetail.repetitionDelay : 0
                onNewValueChanged: {
                    audibleReminderdetail.repetitionDelay = newValue;
                }
            }
            FieldRow {
                id: customAudibleReminderSecBeforeStartRow
                anchors.top: customAudibleReminderRepDelayRow.bottom
                label: "Seconds before start"
                value: audibleReminderdetail? audibleReminderdetail.secondsBeforeStart : 0
                onNewValueChanged: {
                    audibleReminderdetail.secondsBeforeStart = newValue;
                }
            }
            FieldRow {
                anchors.top: customAudibleReminderSecBeforeStartRow.bottom
                label: "Data url"
                value: audibleReminderdetail? audibleReminderdetail.dataUrl : ""
                onNewValueChanged: {
                    audibleReminderdetail.dataUrl = newValue;
                }
            }
        }

        CollectionRoller {
            id: customCollectionRow
            onCurrentIndexChanged: {
                item.collectionId = organizer.collections[currentIndex].collectionId;
            }
        }

        RollerRow {
            id: customRsvpParticipationStatusRow
            valueRoller.clip: true // clipping to roller-component..
            label: "Partic. status"
            valueSet: ["Unknown", "Accepted", "Declined", "Tentative", "Delegated", "InProcess", "Completed"]
            onCurrentIndexChanged: {
                // rsvp-detail included only if modified on the view
                if (EventAttendee.StatusUnknown != customRsvpParticipationStatusRow.currentIndex) {
                    var rsvpDetail;
                    if (item.details(Detail.EventRsvp).length) {
                        rsvpDetail = item.detail(Detail.EventRsvp);
                    } else {
                        rsvpDetail = Qt.createQmlObject("import QtOrganizer 5.0; EventRsvp{}", organizer);
                    }
                    rsvpDetail.participationStatus = customRsvpParticipationStatusRow.currentIndex;
                    item.setDetail(rsvpDetail);
                } else if (item.details(Detail.EventRsvp).length) {
                    item.removeDetail(item.detail(Detail.EventRsvp));
                }
            }
        }

        Column {
            id : tagColumn
            Repeater {
                model: item? (item.itemDetails? item.details(Detail.Tag) : 0) : 0
                Rectangle {
                    width: detailsView.width;
                    height: childrenRect.height
                    Text {
                        width: detailsView.width
                        text: "Tag " + index + " : " + modelData.tag;
                    }
                    Button {
                        text: "remove"
                        width: detailsView.width / 6
                        anchors.right: parent.right
                        onClicked: {
                            item.removeDetail(modelData);
                        }
                    }
                }
            }
        }
        FieldRow {
            id: tagRow
            anchors { top: tagColumn.bottom; margins: height / 3 }
            label: "New Tag"
            value: "NewTag"
        }
        Row {
            anchors.horizontalCenter: tagRow.horizontalCenter
            Button {
                text: "Add tag"
                width: detailsView.width / 4
                onClicked: {
                    if (tagRow.newValue) {
                        var tag = Qt.createQmlObject("import QtOrganizer 5.0;Tag {}", organizer);
                        tag.tag = tagRow.newValue;
                        item.setDetail(tag);
                    }
                }
            }
            Button {
                text: "Birthday"
                width: detailsView.width / 4
                onClicked: {
                    var tagList = item.details(Detail.Tag)
                    var found = false;
                    for (var i=0; i< tagList.length; i++) {
                        if (tagList[i].tag == "ANNIVERSARY")
                             found = true;
                    }
                    if (!found) {
                        var tag = Qt.createQmlObject("import QtOrganizer 5.0;Tag {}", organizer);
                        tag.tag = "ANNIVERSARY";
                        item.setDetail(tag);
                    }
                }
            }
        }
        Column {
            id: attendeeColumn
            Repeater {
                model: item? item.attendees : 0
                Rectangle {
                    width: detailsView.width;
                    height: childrenRect.height
                    Text {
                        width: detailsView.width
                        text: "attendee " + index + " : " + name + "," + emailAddress;
                    }
                    Button {
                        text: "remove"
                        width: detailsView.width / 6
                        height: parent
                        anchors.right: parent.right
                        onClicked: {
                            item.removeDetail(modelData);
                        }
                    }
                }
            }
        }
        Button {
            text: "Add Attendee"
            width: detailsView.width / 2
            anchors {
                horizontalCenter: tagRow.horizontalCenter;
                top: attendeeColumn.bottom;
                margins: height / 3
            }
            onClicked: {
                attendeeDetailsView.item = item;
                calendar.state = "AttendeeDetailsView";
            }
        }
    }

    //todo
    VisualItemModel {
        id:todoItemModel
        Text {
            width: detailsView.width - 6;
            height: 30
            text: "ToDo"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "White";
            font.weight: Font.Bold
        }
        FieldRow {
            id: todoCustomLabelRow
            label: "Title"
            onNewValueChanged: {
                item.displayLabel = todoCustomLabelRow.newValue;
            }
        }
        DateTimeRollerRow {
            id: todoCustomDueDateRow
            label: "Due Time"
            onRollerChanged: {
                if (item.itemType == Type.Todo) {
                    var startDate = todoCustomDueDateRow.dateTimeRoller.selectedDateTime();
                    startDate.setHours(startDate.getHours()-1);
                    item.startDateTime = startDate;
                    item.dueDateTime = todoCustomDueDateRow.dateTimeRoller.selectedDateTime();
                }
            }
        }
       FieldRow {
            id: todoCustomDescriptionRow
            label: "Description"
            height: 150
            onNewValueChanged: {
                item.description = todoCustomDescriptionRow.newValue;
            }
       }
       RollerRow {
           id: todoCustomPriorityRow
           label: "Priority"
           valueSet: ["Unknown", "Highest", "ExtremelyHigh", "VeryHigh", "High", "Medium", "Low", "VeryLow", "ExtremelyLow", "Lowest"]
           onRollerChanged: {
               item.priority = todoCustomPriorityRow.valueRoller.selectedValue();
           }
       }
       CollectionRoller {
           id: todoCollectionRow
           onCurrentIndexChanged: {
               item.collectionId = organizer.collections[currentIndex].collectionId;
           }
       }
    }
}
