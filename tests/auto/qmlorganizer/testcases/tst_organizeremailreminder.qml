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
import QtAddOn.organizer 2.0

Rectangle {
    id: test;

    QOrganizerTestUtility {
        id: utility
    }


    TestCase {
        name: "Email Reminder"

        function test_emailReminder_data() {
            return [
                   { tag: " no properties",
                     code: "import QtAddOn.organizer 2.0;"
                        + "EmailReminder {"
                        + "}"
                    },
                    { tag: " 1 properties",
                      code: "import QtAddOn.organizer 2.0;"
                        + "EmailReminder {"
                        + "    repetitionCount: 3; }"
                    },
                    { tag: " 2 properties",
                      code: "import QtAddOn.organizer 2.0;"
                        + "EmailReminder {"
                        + "    repetitionCount: 3; "
                        + "    repetitionDelay: 30;}"
                    },
                    { tag: " 3 properties",
                      code: "import QtAddOn.organizer 2.0;"
                        + "EmailReminder {"
                        + "    repetitionCount: -1; "
                        + "    secondsBeforeStart: 100;"
                        + "    attachments : [\"1attachments\", \"2attachments\", \"3attachments\"];}"
                    },
                    { tag: " 6 properties",
                      code: "import QtAddOn.organizer 2.0;"
                         + "EmailReminder {"
                         + "    repetitionCount: 3; "
                         + "    repetitionDelay: 30; "
                         + "    secondsBeforeStart: 40; "
                         + "    body: \"email body........not a long one\"; "
                         + "    recipients: [\"one@email.com\", \"two@email.com\", \"three@email.com\"];"
                         + "    attachments : [\"1attachments\", \"2attachments\", \"3attachments\"];"
                         + "    subject: \"email subject\";}"
                    },
            ]
        }

        function test_emailReminder(data) {

            var list = utility.test_managerdata();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }

            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                console.log("Email reminder test start! :" + managerName);
                var model = Qt.createQmlObject(
                      "import QtAddOn.organizer 2.0;"
                    + "OrganizerModel {"
                    + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                    + "   startPeriod:'2009-01-01';"
                    + "   endPeriod:'2012-12-31';"
                    + "   autoUpdate:true; }"
                    , test);

                var event = Qt.createQmlObject(
                        "import QtAddOn.organizer 2.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer qml reminder test event\"; "
                        + "   description: \"This is a new reminder event!!!\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                var emailReminderDetail = Qt.createQmlObject(data.code, event);
                var organizerChangedSpy = Qt.createQmlObject(
                          "import QtTest 1.0;"
                          + "SignalSpy {id : organizerChangedSpy;}"
                          , test);
                organizerChangedSpy.target = model;
                organizerChangedSpy.signalName = "modelChanged";
                utility.model = model;
                utility.spy = organizerChangedSpy;
                utility.empty_calendar();
                //------Create and save the detail test------//
                emailReminderDetail.repetitionCount = -1;
                event.addDetail(emailReminderDetail);
                model.saveItem(event);
                //Wait for the model to be up-to-date
                var count = 0;
                do {
                   organizerChangedSpy.wait(100);
                   count ++;
                   verify(model.itemCount <= 1)
                   verify(count <= 10)
                } while (model.itemCount < 1)

                model.update();
                var fetchlist = model.items;
                compare(model.itemCount, 1)
                var savedEvent = fetchlist[0];

                var detail = savedEvent.detail(Detail.EmailReminder);
                verify(savedEvent != undefined)
                verify(detail != undefined)
                utility.compareReminderDetails(emailReminderDetail, detail);

                //------update the detail test------//
                var savedEventDetail = savedEvent.detail(Detail.EmailReminder);
                savedEventDetail.subject = "email subject";
                savedEventDetail.body = "Hello! mail body!!!";
                savedEventDetail.recipients = ["arecipients", "brecipients", "crecipients"];
                savedEventDetail.attachments = ["aattachments", "battachments", "cattachments"];
                savedEventDetail.secondsBeforeStart = 300;
                savedEventDetail.repetitionCount = -1;

                emailReminderDetail.subject = "email subject";
                emailReminderDetail.body = "Hello! mail body!!!";
                emailReminderDetail.recipients = ["arecipients", "brecipients", "crecipients"];
                emailReminderDetail.attachments = ["aattachments", "battachments", "cattachments"];
                emailReminderDetail.secondsBeforeStart = 300;
                emailReminderDetail.repetitionCount = -1;

                savedEvent.setDetail(savedEventDetail);

                model.saveItem(savedEvent);
                organizerChangedSpy.wait();
                model.update();
                //no new event created
                compare(model.itemCount, 1)
                fetchlist = model.items;
                var updatedEvent = fetchlist[0];
                var updatedEventDetail = updatedEvent.detail(Detail.EmailReminder);
                verify(updatedEvent != undefined)
                verify(updatedEventDetail != undefined)
                utility.compareReminderDetails(emailReminderDetail, updatedEventDetail);

                //------remove the detail test------//
                var removeEventDetail = updatedEvent.detail(Detail.EmailReminder);
                updatedEvent.removeDetail(removeEventDetail);
                model.saveItem(updatedEvent);
                organizerChangedSpy.wait();
                model.update();
                //no new event created
                compare(model.itemCount, 1)
                fetchlist = model.items;
                var detailRemovedEvent = fetchlist[0];
                var detailRemovedEventDetailList = detailRemovedEvent.details(Detail.EmailReminder);
                if (detailRemovedEventDetailList.length > 0)
                    utility.outputDetail(detailRemovedEvent.detail(Detail.EmailReminder));
                verify(detailRemovedEventDetailList.length == 0)
                verify(removeEventDetail != undefined)

                organizerChangedSpy.clear();
                organizerChangedSpy.destroy();
                model.destroy();
                event.destroy();
                emailReminderDetail.destroy();
   //           utility.empty_calendar();
            }
        }
    }
}
