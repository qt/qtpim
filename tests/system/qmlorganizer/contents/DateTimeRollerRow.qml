/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
import "datetimerollercontents"

Item {
    property variant value
    property alias label: nameField.text
    property alias dateTimeRoller : dateTimeRoller

    opacity : (enabled) ? 1.0 : 0.6

    signal rollerChanged

    height: 100
    width: detailsView.width

    onValueChanged: {
        dateTimeRoller.setDateTime(value)
    }

    Text {
        id: nameField
        width: parent.width * 0.4
        anchors { left: parent.left; margins: 3 }
        font.bold: true
        color: "white"
    }

    Rectangle {
        id: dateTimeRoller

        width: roller.width
        height: roller.height
        anchors { left: nameField.right; margins: 3}
        color: "white";
        border { width: 3; color: "white" }
        radius: 5

        Grid {
            id: roller
            spacing: 2
            columns: 5
            Title { width: day.width; label: "day" }
            Title { width: month.width; label: "month" }
            Title { width: year.width; label: "year" }
            Title { width: hour.width; label: "hour" }
            Title { width: minute.width; label: "minute" }
            Spinner {
                id: day
                model: 31
                delegate: Text { text: dateTimeRoller.formatDigits(index+1); }
                onCurrentIndexChanged: rollerChanged()
            }
            Spinner {
                id: month
                width: 40
                model: 12
                delegate: Text { text: dateTimeRoller.monthName(index+1); }
                onCurrentIndexChanged: rollerChanged()
            }
            Spinner {
                id: year
                width: 60
                model: 5
                delegate: Text { text: index+2010; }
                onCurrentIndexChanged: rollerChanged()
            }
            Spinner {
                id: hour
                model: 24
                delegate: Text { text: dateTimeRoller.formatDigits(index); }
                onCurrentIndexChanged: rollerChanged()
            }
            Spinner {
                id: minute
                model: 60
                delegate: Text {text: dateTimeRoller.formatDigits(index); }
                onCurrentIndexChanged: rollerChanged()
            }
        }

        // internal formatting
        function formatDigits(value) {return value < 10 ? "0"+value : value;}
        // internal formatting
        function monthName(monthIndex) {
            switch(monthIndex)
            {
            case 1: return "Jan."; break;
            case 2: return "Feb."; break;
            case 3: return "Mar."; break;
            case 4: return "Apr."; break;
            case 5: return "May."; break;
            case 6: return "Jun."; break;
            case 7: return "Jul."; break;
            case 8: return "Aug."; break;
            case 9: return "Sep."; break;
            case 10: return "Oct."; break;
            case 11: return "Nov."; break;
            case 12: return "Dec."; break;
            default: return monthIndex;
            }
        }

        // component API

        // use this to show current date
        function showCurrentDateTime() {
            var date = new Date();
            day.currentIndex = date.getDate()-1;
            month.currentIndex = date.getMonth();
            year.currentIndex = date.getFullYear()-2010;
            //hour.currentIndex = date.getHours();
            //minute.currentIndex = date.getMinutes();
        }

        // use this to read selected datetime
        function selectedDateTime() {
            return new Date(year.currentIndex+2010, month.currentIndex, day.currentIndex+1, hour.currentIndex, minute.currentIndex, 0 ,0);
        }

        // use this to set datetime
        function setDateTime(date) {
            if (date == undefined) {
                showCurrentDateTime();
            } else {
                day.currentIndex = date.getDate()-1;
                month.currentIndex = date.getMonth();
                year.currentIndex = date.getFullYear()-2010;
                hour.currentIndex = date.getHours();
                minute.currentIndex = date.getMinutes();
            }
        }
    }
}
