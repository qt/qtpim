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
                     code: "import QtOrganizer 5.0;"
                        + "EmailReminder {"
                        + "}"
                    },
                    { tag: " 1 properties",
                      code: "import QtOrganizer 5.0;"
                        + "EmailReminder {"
                        + "    repetitionCount: 3; }"
                    },
                    { tag: " 2 properties",
                      code: "import QtOrganizer 5.0;"
                        + "EmailReminder {"
                        + "    repetitionCount: 3; "
                        + "    repetitionDelay: 30;}"
                    },
                    { tag: " 3 properties",
                      code: "import QtOrganizer 5.0;"
                        + "EmailReminder {"
                        + "    repetitionCount: -1; "
                        + "    secondsBeforeStart: 100;"
                        + "    attachments : [\"1attachments\", \"2attachments\", \"3attachments\"];}"
                    },
                    { tag: " 6 properties",
                      code: "import QtOrganizer 5.0;"
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

            var list = utility.getManagerList();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }

            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                console.log("Email reminder test start! :" + managerName);
                var model = Qt.createQmlObject(
                      "import QtOrganizer 5.0;"
                    + "OrganizerModel {"
                    + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                    + "   startPeriod:'2009-01-01';"
                    + "   endPeriod:'2012-12-31';"
                    + "   autoUpdate:true; }"
                    , test);

                var event = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer qml reminder test event\"; "
                        + "   description: \"This is a new reminder event!!!\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                var emailReminderDetail = Qt.createQmlObject(data.code, event);
                utility.init(model);
                utility.empty_calendar();
                //------Create and save the detail test------//
                emailReminderDetail.repetitionCount = -1;
                event.setDetail(emailReminderDetail);
                model.saveItem(event);
                //Wait for the model to be up-to-date
                utility.waitModelChange(1);
                compare(model.itemCount, 1)
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
                utility.organizerChangedSpy.wait();
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
                utility.organizerChangedSpy.wait();
                //no new event created
                compare(model.itemCount, 1)
                fetchlist = model.items;
                var detailRemovedEvent = fetchlist[0];
                var detailRemovedEventDetailList = detailRemovedEvent.details(Detail.EmailReminder);
                if (detailRemovedEventDetailList.length > 0)
                    utility.outputDetail(detailRemovedEvent.detail(Detail.EmailReminder));
                verify(detailRemovedEventDetailList.length == 0)
                verify(removeEventDetail != undefined)

                utility.organizerChangedSpy.clear();
                utility.organizerChangedSpy.destroy();
                model.destroy();
                event.destroy();
                emailReminderDetail.destroy();
   //           utility.empty_calendar();
            }
        }
    }
}
