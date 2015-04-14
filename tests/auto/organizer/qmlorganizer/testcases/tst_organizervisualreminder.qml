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
        name: "VisualReminder"
        function test_visualReminder_data() {
            return [
                   { tag: " no properties",
                     code: "import QtOrganizer 5.0;"
                        + "VisualReminder {"
                        + "}"
                    },
                    { tag: " 1 properties",
                      code: "import QtOrganizer 5.0;"
                        + "VisualReminder {"
                        + "    repetitionCount: 3; }"
                    },
                    { tag: " 2 properties",
                      code: "import QtOrganizer 5.0;"
                        + "VisualReminder {"
                        + "    repetitionCount: 3; "
                        + "    repetitionDelay: 30;}"
                    },
                    { tag: " 3 properties",
                      code: "import QtOrganizer 5.0;"
                        + "VisualReminder {"
                        + "    repetitionCount: 0; "
                        + "    secondsBeforeStart: 100;}"
                    },
                    { tag: " 4 properties",
                      code: "import QtOrganizer 5.0;"
                         + "VisualReminder {"
                         + "    repetitionCount: 3; "
                         + "    repetitionDelay: 30; "
                         + "    secondsBeforeStart: 40; "
                         + "    dataUrl: \"www.nokia.com\"  }"
                    },
                    { tag: " 5 properties",
                      code: "import QtOrganizer 5.0;"
                         + "VisualReminder {"
                         + "    repetitionCount: 3; "
                         + "    repetitionDelay: 30; "
                         + "    secondsBeforeStart: 40; "
                         + "    message: \"visual reminder Message\"; "
                         + "    dataUrl: \"www.nokia.com\"  }"
                    },
            ]
        }

        function test_visualReminder(data) {

            var debugFlag = 0;
            var list = utility.getManagerList();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                console.log("VisualReminder test start! :" + managerName);
                var model = Qt.createQmlObject(
                      "import QtOrganizer 5.0;"
                    + "OrganizerModel {"
                    + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                    + "   startPeriod:'2009-01-01';"
                    + "   endPeriod:'2012-12-31';"
                    + "   autoUpdate:true; }"
                    , test);

                var visualReminderEvent = Qt.createQmlObject(
                          "import QtOrganizer 5.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer qml reminder test event\"; "
                        + "   description: \"This is a new reminder event!!!\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                var visualReminderDetail = Qt.createQmlObject(data.code , visualReminderEvent);
                utility.init(model);
                utility.empty_calendar();
                //------Create and save the detail test------//
                utility.debug("Create and save the detail test", debugFlag);
                visualReminderDetail.dataUrl = "http://www.test0.com";
                visualReminderEvent.setDetail(visualReminderDetail);
                model.saveItem(visualReminderEvent);
                //Let's wait for the model to be up-to-date
                utility.waitModelChange(1);
                compare(model.itemCount, 1)
                var fetchVisuallist = model.items;
                var savedVisualEvent = fetchVisuallist[0];
                verify(savedVisualEvent != undefined);
                verify(savedVisualEvent.detail(Detail.VisualReminder) != undefined);
                utility.compareReminderDetails(visualReminderDetail, savedVisualEvent.detail(Detail.VisualReminder));

                //------update the details test------//
                var savedEventDetail = savedVisualEvent.detail(Detail.VisualReminder);
                savedEventDetail.dataUrl = "http://www.test222.com";
                savedEventDetail.message = "visual reminder message";
                savedEventDetail.secondsBeforeStart = 300;
                savedEventDetail.repetitionCount = 0;

                visualReminderDetail.dataUrl = "http://www.test222.com";
                visualReminderDetail.message = "visual reminder message";
                visualReminderDetail.secondsBeforeStart = 300;
                visualReminderDetail.repetitionCount = 0;

                savedVisualEvent.setDetail(savedEventDetail);

                model.saveItem(savedVisualEvent);
                utility.organizerChangedSpy.wait();
                //no new event created
                compare(model.itemCount, 1)
                fetchVisuallist = model.items;
                var updatedEvent = fetchVisuallist[0];
                verify(updatedEvent != undefined);
                verify(updatedEvent.detail(Detail.VisualReminder) != undefined);
                var updatedEventDetail = updatedEvent.detail(Detail.VisualReminder);
                utility.compareReminderDetails(savedEventDetail, updatedEventDetail);
                utility.compareReminderDetails(visualReminderDetail, updatedEventDetail);

                //------remove the details test------//
                var removeEventDetail = updatedEvent.detail(Detail.VisualReminder);
                updatedEvent.removeDetail(removeEventDetail);
                model.saveItem(updatedEvent);
                utility.organizerChangedSpy.wait();
                //no new event created
                compare(model.itemCount, 1)
                fetchVisuallist = model.items;
                var detailRemovedEvent = fetchVisuallist[0];
                var detailRemovedEventDetailList = detailRemovedEvent.details(Detail.VisualReminder);
                if (detailRemovedEventDetailList.length > 0) {
                    var detailRemovedEventDetail = detailRemovedEvent.detail(Detail.VisualReminder);
                    utility.outputEvent(detailRemovedEvent)
                }
                verify(detailRemovedEventDetailList.length == 0);
                verify(removeEventDetail != undefined)
                utility.organizerChangedSpy.clear();
                utility.organizerChangedSpy.destroy();
                model.destroy();
                visualReminderEvent.destroy();
                visualReminderDetail.destroy();
    //            utility.empty_calendar();
            }
        }
    }
}
