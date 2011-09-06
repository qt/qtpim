import QtQuick 2.0
import QtTest 1.0
import QtAddOn.organizer 2.0

TestCase {
    id: testUtility
    property OrganizerModel model
    property SignalSpy spy

    function test_managerdata() {
        var model = Qt.createQmlObject(
                "import QtAddOn.organizer 2.0; OrganizerModel {}"
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
        model.update ();

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
        var eventDetail = event.detail("audibleReminder");
        outputDetail(eventDetail);

        eventDetail = event.detail("emailReminder");
        outputDetail(eventDetail);
        eventDetail = event.detail("visualReminder");
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
        compareReminderDetails(event1.detail("audibleReminder"), event2.detail("audibleReminder"));
        compareReminderDetails(event1.detail("visualReminder"), event2.detail("visualReminder"));
        compareReminderDetails(event1.detail("emailReminder"), event2.detail("emailReminder"));
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
