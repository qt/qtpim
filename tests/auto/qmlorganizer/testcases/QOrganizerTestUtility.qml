/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtOrganizer 2.0

TestCase {
    id: testUtility
    property OrganizerModel model
    property SignalSpy spy

    function debug(string, flag) {
        if (flag == 1)
            console.log(string);
    }

    function create_testobject(ctorString, parent) {
        var newObject = Qt.createQmlObject(ctorString, parent);
        verify(newObject != undefined, 'Object creation failed');
        return newObject;
    }

    function waiting_model_signal(expect_count) {
        var count = 0;
        if (model.itemCount < expect_count) {
            do {
                spy.wait(200);
                count ++;
                verify(model.itemCount <= expect_count)
                verify(count <= 10)
            } while (model.itemCount < expect_count)
        } else if (model.itemCount > expect_count) {
            do {
                spy.wait(500);
                count ++;
                verify(model.itemCount >= expect_count)
                verify(count <= 10)
            } while (model.itemCount > expect_count)
        }
    }

    function test_managerdata() {
        var model = Qt.createQmlObject(
                "import QtOrganizer 2.0; OrganizerModel {}"
                , testUtility);
        wait (100);
        var managerlist = model.availableManagers;
        var idx = managerlist.indexOf("invalid"); // Find the index
        if (idx != -1)
            managerlist.splice(idx, 1); // Remove it if really found!
        //console.log ("Manger list:  " + managerlist);
        return managerlist;
    }

    //Empty calendar data
    function empty_calendar(log) {
        //waiting for model update and remove request
        wait(500);

        var ids = model.itemIds();
        if (log != undefined) {
            console.log("items count :" + model.itemCount);
            console.log("items  :" + ids);
        }

        if (ids.length > 0) {
            model.removeItems(ids);
            if (spy != undefined)
                spy.wait();
            else
                wait(100);
        }
        model.update();

        if (model.itemCount >0) {
            if (log != undefined)
                console.log("Not empty database! " + model.itemCount);
            empty_calendar(log);
        }
    }

    function outputDetail(eventDetail) {
        console.log("=========================================");
        if (eventDetail == undefined) {
            console.log("detail is undefined");
            return;
        }
        console.log(eventDetail);
        console.log("secondsBeforeStart: " + eventDetail.secondsBeforeStart);
        console.log("repetitionCount:    " + eventDetail.repetitionCount);
        console.log("repetitionDelay:    " + eventDetail.repetitionDelay);

        if (eventDetail.reminderType == 2) {
            //audibleReminder type
            console.log("dataUrl:    " + eventDetail.dataUrl);
        } else if (eventDetail.reminderType == 1) {
            //VisualReminder type
            console.log("message:    " + eventDetail.message);
            console.log("dataUrl:    " + eventDetail.dataUrl);

        } else if (eventDetail.reminderType == 3) {
            //EmailReminder type
            console.log("subject:    " + eventDetail.subject);
            console.log("body:    " + eventDetail.body);
            //recipients list
            var recipients = eventDetail.recipients;
            console.log("recipients: " + recipients);
            //attachments list
            var attachments = eventDetail.attachments;
            console.log("recipients: " + attachments);
        }
    }

    function outputEvent(event) {
        console.log("--------------------------------------------");
        console.log("organizer item id: " + event.itemId);
        console.log("audibleReminder:   ");
        //AudibleReminder
        var eventDetail = event.detail(Detail.AudibleReminder);
        outputDetail(eventDetail);

        eventDetail = event.detail(Detail.EmailReminder);
        outputDetail(eventDetail);
        eventDetail = event.detail(Detail.VisualReminder);
        outputDetail(eventDetail);
    }

    function compareReminderDetails(detail1, detail2, log) {
        if (log != undefined) {
            outputDetail(detail1);
            outputDetail(detail2);
        }
        //Reminder common compare
        if (detail1 == undefined) {
            verify(detail2 == undefined);
            return;
        } else {
            verify(detail2 != undefined);
        }
        verify(detail1.reminderType == detail2.reminderType);
        var detail1RepCount = detail1.repetitionCount;
        var detail2RepCount = detail2.repetitionCount;
        compare(detail1RepCount, detail2RepCount);

        var detail1RepDelay = detail1.repetitionDelay;
        var detail2RepDelay = detail2.repetitionDelay;
        compare(detail1RepDelay, detail2RepDelay);

        var detail2SecBeforeStart = detail2.secondsBeforeStart;
        var detail1SecBeforeStart = detail1.secondsBeforeStart;
        compare(detail1SecBeforeStart, detail2SecBeforeStart);


        if (detail1.reminderType == 2) {
            //audibleReminder type
            var detail1DataUrl = detail1.dataUrl;
            var detail2DataUrl = detail2.dataUrl;
            compare(detail1DataUrl, detail2DataUrl);
        } else if (detail1.reminderType == 1) {
            //VisualReminder type
            var detail1Message = detail1.message;
            var detail2Message = detail2.message;
            compare(detail1Message, detail2Message);
            var detail1DataUrl = detail1.dataUrl;
            var detail2DataUrl = detail2.dataUrl;
            compare(detail1DataUrl, detail2DataUrl);
        } else if (detail1.reminderType == 3) {
            //EmailReminder type
            var detail1Subject = detail1.subject;
            var detail2Subject = detail2.subject;
            compare(detail1Subject, detail2Subject);
            var detail1Body = detail1.body;
            var detail2Body = detail2.body;
            compare(detail1Body, detail2Body);
            var detail1Recipients = detail1.recipients;
            var detail2Recipients = detail2.recipients;
            verify(compareList(detail1Recipients, detail2Recipients));
            var detail1Attachments = detail1.attachments;
            var detail2Attachments = detail2.attachments;
            verify(compareList(detail1Attachments, detail2Attachments))
        }
    }

    function compareEvent(event1, event2, log) {
        if (log != undefined) {
            outputEvent(event1);
            outputEvent(event2);
            console.log("compareEvent");
        }
        //Reminder compare
        compareReminderDetails(event1.detail(Detail.AudibleReminder), event2.detail(Detail.AudibleReminder));
        compareReminderDetails(event1.detail(Detail.VisualReminder), event2.detail(Detail.VisualReminder));
        compareReminderDetails(event1.detail(Detail.EmailReminder), event2.detail(Detail.EmailReminder));
    }

    function compareList(a, b) {
        if (a == b) {
            return true;
        }
        if (a.length != b.length)
            return false;

        for (var key in a) {
            if (a[key] != b[key]) {
                console.log("Failed! List values are not the same :" + a[key] + " is not equal to" + b[key]);
                return false;
            }
        }
        return true;
    }
}
