/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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
import QtTest 1.0
import QtOrganizer 5.0

TestCase {
    id: testUtility
    property var signalWaitTime : 5000
    property var itemChange: 0
    property var collectionChange: 1
    property SignalSpy organizerChangedSpy
    property SignalSpy organizerCollectionChangedSpy
    property bool modelChanged: false

    //---------internal use---------//
    property OrganizerModel __model

    function init(model) {
        __model = model;
        organizerChangedSpy = Qt.createQmlObject(
                "import QtTest 1.0;"
                + "SignalSpy {}"
                , testUtility);
        organizerChangedSpy.target = model
        organizerChangedSpy.signalName = "modelChanged"
        organizerCollectionChangedSpy = Qt.createQmlObject(
                "import QtTest 1.0;"
                + "SignalSpy {}"
                , testUtility);
        organizerCollectionChangedSpy.target = model;
        organizerCollectionChangedSpy.signalName = "collectionsChanged";

        __model.modelChanged.connect(modelChangedSignalHandler);
    }

    function debug(string, flag) {
        if (flag == 1)
            console.log(string);
    }

    function modelChangedSignalHandler() {
        modelChanged = true;
    }

    function clearModelChanged() {
        modelChanged = false;
        organizerChangedSpy.clear()
    }

    function waitModelChanged(waitTimeCounter) {
        if (modelChanged)
            return;

        var counter = 0;
        modelChanged = false;
        while (counter < waitTimeCounter) {
            wait(1);
            counter++;
            if (modelChanged)
                return;
        }
    }

    function create_testobject(ctorString, parent) {
        var newObject = Qt.createQmlObject(ctorString, parent);
        verify(newObject != undefined, 'Object creation failed');
        return newObject;
    }
    //default is item change
    function waitModelChange(expect_count, waitModelSignalType) {
        var count = 0;
        if (Number(waitModelSignalType) == itemChange || waitModelSignalType === undefined ) {
            if (expect_count === undefined) {
                organizerChangedSpy.wait(signalWaitTime);
            } else if (__model.itemCount < expect_count) {
                do {
                    organizerChangedSpy.wait(signalWaitTime);
                    count ++;
                    verify(__model.itemCount <= expect_count)
                    verify(count <= 10)
                } while (__model.itemCount < expect_count)
            } else if (__model.itemCount > expect_count) {
                do {
                    organizerChangedSpy.wait(signalWaitTime);
                    count ++;
                    verify(__model.itemCount >= expect_count)
                    verify(count <= 10)
                } while (__model.itemCount > expect_count)
            }
        } else if (Number(waitModelSignalType) == collectionChange) {
            if (__model.collections.length < expect_count) {
                do {
                    organizerCollectionChangedSpy.wait(signalWaitTime);
                    count ++;
                    verify(__model.collections.length <= expect_count)
                    verify(count <= 10)
                } while (__model.collections.length < expect_count)
            } else if (__model.collections.length > expect_count) {
                do {
                    organizerCollectionChangedSpy.wait(signalWaitTime);
                    count ++;
                    verify(__model.collections.length >= expect_count)
                    verify(count <= 10)
                } while (__model.collections.length > expect_count)
            }
        }
    }

    function getManagerList() {

        var testManagers = ["memory"];

        var model = Qt.createQmlObject(
                "import QtOrganizer 5.0; OrganizerModel {}"
                , testUtility);

        var managerlist = [];
        for (var i in testManagers) {
            if (model.availableManagers.indexOf(testManagers[i]) != -1) {
                managerlist.push(testManagers[i]);
            }
        }

        model.autoUpdate = false;
        model.destroy();

        return managerlist;
    }

    function getManagerListData() {
        return utility.getManagerList().map(function(x) {
            return {tag: x+" backend", managerToBeTested: x}
        })
    }

    function createModel(managerName) {
        var model = Qt.createQmlObject(
              "import QtOrganizer 5.0;"
            + "OrganizerModel {"
            + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
            + "   startPeriod:'2009-01-01';"
            + "   endPeriod:'2012-12-31';"
            + "   autoUpdate:true; }"
            , testUtility);
        init(model);
        waitModelChange();
        empty_calendar();
        return model;
    }

    function create_spy(targetObj, signalName) {
        var spy = Qt.createQmlObject( "import QtTest 1.0 \nSignalSpy {}", testUtility);
        spy.target = targetObj;
        spy.signalName = signalName;
        return spy;
    }

    function empty_calendar(log) {
        var ids = [];
        var removeIds = [];
        var i;
        var j;
        for (i = 0; i < __model.itemCount; i++) {
            if (__model.items[i].itemType == Type.EventOccurrence || __model.items[i].itemType == Type.TodoOccurrence)
                ids.push(__model.items[i].parentId)
            if (__model.items[i].itemId !== "qtorganizer:::")
                ids.push(__model.items[i].itemId)
        }
        // remove duplicates

        for (i = 0; i < ids.length; i++) {
            for (j = 0; j < removeIds.length; j++) {
                if (ids[i] == removeIds[j])
                    break;
            }
            if (j == removeIds.length)
                removeIds.push(ids[i])
        }
        if (log != undefined) {
            console.log("items count :" + __model.itemCount);
            console.log("items  :" + removeIds);
        }

        if (removeIds.length > 0) {
            clearModelChanged()
            __model.removeItems(removeIds)
            if (!__model.autoUpdate)
                __model.update()
            waitModelChanged(500);
        }
        compare(__model.items.length, 0)
        empty_calendar_collections(log);
    }

    function empty_calendar_collections(log) {
        if (__model.collections.length > 1) {
            var setAutoUpdate = __model.autoUpdate;
            __model.autoUpdate = false;
            for (var i = 0; i < __model.collections.length; ++i) {
                var collId = __model.collections[i].collectionId;
                if (collId != __model.defaultCollectionId()) {
                    __model.removeCollection(collId);
                }
            }
            __model.autoUpdate = setAutoUpdate;
            __model.update();
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

    function addItemsToModel(ctrStrings, parent) {
        for (var i=0;i<ctrStrings.length;i++) {
            var item = create_testobject(ctrStrings[i], parent);
            __model.saveItem(item);
            organizerChangedSpy.wait();
        }
    }

    function toUTCMidnight(date) {
        return new Date(Date.UTC(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate()));
    }
}
