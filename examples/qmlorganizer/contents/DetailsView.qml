/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtAddOn.organizer 2.0
import "datetimerollercontents"

Rectangle
{
    id:detailsView
    anchors.fill: parent
    property bool isNewItem
    property variant item
    property variant rule
    property variant audibleReminderdetail
    property variant visualReminderdetail

    onOpacityChanged: {
        // Is this view visible
        if (detailsView.opacity == 1) {
            // Initialize empty item
            if (isNewItem) {
                if (item.type == "Event") {
                    item.startDateTime = new Date (calendar.year, calendar.month, calendar.day, 12);
                    item.endDateTime = new Date (calendar.year, calendar.month, calendar.day, 13);
                } else if (item.type == "Todo") {
                    item.startDateTime = new Date (calendar.year, calendar.month, calendar.day, 12);
                    item.dueDateTime = new Date (calendar.year, calendar.month, calendar.day, 13);
                }
            }
            // Initialize recurrence rule
            emptyRecurrenceRule.frequency = RecurrenceRule.Invalid;
            emptyRecurrenceRule.interval = 1;
            emptyRecurrenceRule.limit = null;

            // Initialize UI
            if (item.type == "Event") {
                customLabelRow.setValue(item.displayLabel);
                customStartDateRow.dateTimeRoller.setDateTime(item.startDateTime);
                customEndDateRow.dateTimeRoller.setDateTime(item.endDateTime);
                customAlldayCheckBox.setValue(item.allDay);
                customDescriptionRow.setValue(item.description);
                customLocationRow.setValue(item.location);
            } else if (item.type == "Todo") {
                todoCustomLabelRow.setValue(item.displayLabel);
                todoCustomDueDateRow.dateTimeRoller.setDateTime(detailsView.item.dueDateTime);
                todoCustomDescriptionRow.setValue(item.description);
                todoCustomPriorityRow.value = detailsView.item.priority;
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
            var audibleDetailList = item.details("audibleReminder");
            if (audibleDetailList != undefined) {
                audibleReminderdetail = item.detail("audibleReminder");
                audibleReminderCheckBox.setValue(true);
            } else {
                audibleReminderdetail = emptyAudibleReminder;
                audibleReminderCheckBox.setValue(false);
            }

            //Visual reminder
            var visualDetailList = item.details("visualReminder");
            if (visualDetailList != undefined) {
                visualReminderdetail = item.detail("visualReminder");
                visualReminderCheckBox.setValue(true);
            } else {
                visualReminderdetail = emptyVisualReminder;
                visualReminderCheckBox.setValue(false);
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

    VisualReminder {
        id: emptyVisualReminder
        dataUrl: "http://www.qt.nokia.com"
        message: "empty message"
    }

    Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.8 }
    color: "#343434";

    Button {
        id: saveButton
        text: "Save & Exit"
        anchors { top: parent.top; left: parent.left }
        width: parent.width / 2
        onClicked: {
            //save item
            saveButton.enabled = false;
            calendar.currentDate = item.startDateTime;
            calendar.organizer.saveItem(item);
            calendar.state = "DayView";
            saveButton.enabled = true;
        }
    }
    Button {
        id: deleteButton
        visible: !isNewItem
        text: "Delete"
        anchors { top: parent.top; left: saveButton.right }
        width: parent.width / 2
        onClicked: {
            calendar.organizer.removeItem(item.itemId);
            calendar.state = "DayView";
        }
    }

    ListView {
        anchors {top: saveButton.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        clip: true
        opacity: 0.8
        model: {
            switch (item.type) {
                    case "Event" :
                        //temporary comment because recurrence is not supported yet
                        // return eventItemModel;
                    case "EventOccurrence":
                         return eventOccurrenceItemModel;
                    case "Todo":
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
           value: rule.frequency
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
               if (!customRecurrenceLimitRow.newValue)
                   rule.limit = null;
           }
       }

       CheckBoxRow {
           id: audibleReminderCheckBox
           label: "Audible reminder"
           onCheckBoxChanged: {
                if (newValue) {
                    audibleReminderdetail = emptyAudibleReminder;
                    //This function will create new detail
                    item.addDetail(audibleReminderdetail);
                    audibleReminderdetail = item.detail("audibleReminder");
                } else {
                    var removeDetail = item.detail("audibleReminder");
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
               text: "Audible reminder"
               verticalAlignment: Text.AlignVCenter
               horizontalAlignment: Text.AlignHCenter
               color: "White";
               font.weight: Font.Bold
           }

           FieldRow {
               id: customAudibleReminderRepCountRow
               anchors.top: audibleReminderDataRowText.bottom
               label: "Repetition count"
               value: audibleReminderdetail.repetitionCount
               onNewValueChanged: {
                   audibleReminderdetail.repetitionCount = newValue;
               }
           }
           FieldRow {
                id: customAudibleReminderRepDelayRow
                anchors.top: customAudibleReminderRepCountRow.bottom
                label: "Repetition delay"
                value: audibleReminderdetail.repetitionDelay
                onNewValueChanged: {
                    audibleReminderdetail.repetitionDelay = newValue;
                }
            }
            FieldRow {
                id: customAudibleReminderSecBeforeStartRow
                anchors.top: customAudibleReminderRepDelayRow.bottom
                label: "Seconds before start"
                value: audibleReminderdetail.secondsBeforeStart
                onNewValueChanged: {
                    audibleReminderdetail.secondsBeforeStart = newValue;
                }
            }
            FieldRow {
                anchors.top: customAudibleReminderSecBeforeStartRow.bottom
                label: "Data url"
                value: audibleReminderdetail.dataUrl
                onNewValueChanged: {
                    audibleReminderdetail.dataUrl = newValue;
                }
            }
        }

        CheckBoxRow {
            id:  visualReminderCheckBox
            label: "Visual reminder"
            onCheckBoxChanged: {
                if (newValue) {
                    visualReminderdetail = emptyVisualReminder;
                    item.addDetail(visualReminderdetail);
                    visualReminderdetail = item.detail("visualReminder");
                } else {
                    var removeDetail = item.detail("visualReminder");
                    item.removeDetail(removeDetail);
                }
            }
        }
        Item {
            height: visible? 260 : 0
            width: detailsView.width
            id:  visualReminderDataRow
            visible:  visualReminderCheckBox.newValue
            Text {
                id:  visualReminderDataRowText
                anchors.top: parent.top
                width: detailsView.width;
                height: 20
                text: "Visual reminder"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "White";
                font.weight: Font.Bold
            }
            FieldRow {
                id: customVisualReminderRepCountRow
                anchors.top:  visualReminderDataRowText.bottom
                label: "Repetition count"
                value:  visualReminderdetail.repetitionCount
                onNewValueChanged: {
                    visualReminderdetail.repetitionCount = newValue;
                }
            }
            FieldRow {
                id: customVisualReminderRepDelayRow
                anchors.top: customVisualReminderRepCountRow.bottom
                label: "Repetition delay"
                value:  visualReminderdetail.repetitionDelay
                onNewValueChanged: {
                     visualReminderdetail.repetitionDelay = newValue;
                }
            }
            FieldRow {
                id: customVisualReminderSecBeforeStartRow
                anchors.top: customVisualReminderRepDelayRow.bottom
                label: "Seconds before start"
                value: visualReminderdetail.secondsBeforeStart
                onNewValueChanged: {
                    visualReminderdetail.secondsBeforeStart = newValue;
                }
            }
            FieldRow {
                id: customVisualReminderDataUrlRow
                anchors.top: customVisualReminderSecBeforeStartRow.bottom
                label: "Data url"
                value: visualReminderdetail.dataUrl
                onNewValueChanged: {
                    visualReminderdetail.dataUrl = newValue;
                }
            }
            FieldRow {
                anchors.top: customVisualReminderDataUrlRow.bottom
                label: "Message"
                value: visualReminderdetail.message
                onNewValueChanged: {
                    visualReminderdetail.message = newValue;
                }
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
                if (item.type == "Todo") {
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
    }
}
