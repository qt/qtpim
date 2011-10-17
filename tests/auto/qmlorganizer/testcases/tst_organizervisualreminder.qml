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
                     code: "import QtOrganizer 2.0;"
                        + "VisualReminder {"
                        + "}"
                    },
                    { tag: " 1 properties",
                      code: "import QtOrganizer 2.0;"
                        + "VisualReminder {"
                        + "    repetitionCount: 3; }"
                    },
                    { tag: " 2 properties",
                      code: "import QtOrganizer 2.0;"
                        + "VisualReminder {"
                        + "    repetitionCount: 3; "
                        + "    repetitionDelay: 30;}"
                    },
                    { tag: " 3 properties",
                      code: "import QtOrganizer 2.0;"
                        + "VisualReminder {"
                        + "    repetitionCount: -1; "
                        + "    secondsBeforeStart: 100;}"
                    },
                    { tag: " 4 properties",
                      code: "import QtOrganizer 2.0;"
                         + "VisualReminder {"
                         + "    repetitionCount: 3; "
                         + "    repetitionDelay: 30; "
                         + "    secondsBeforeStart: 40; "
                         + "    dataUrl: \"www.nokia.com\"  }"
                    },
                    { tag: " 5 properties",
                      code: "import QtOrganizer 2.0;"
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

            var list = utility.test_managerdata();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                console.log("VisualReminder test start! :" + managerName);
                var model = Qt.createQmlObject(
                      "import QtOrganizer 2.0;"
                    + "OrganizerModel {"
                    + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                    + "   startPeriod:'2009-01-01';"
                    + "   endPeriod:'2012-12-31';"
                    + "   autoUpdate:true; }"
                    , test);

                var visualReminderEvent = Qt.createQmlObject(
                          "import QtOrganizer 2.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer qml reminder test event\"; "
                        + "   description: \"This is a new reminder event!!!\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                var visualReminderDetail = Qt.createQmlObject(data.code , visualReminderEvent);
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
                visualReminderDetail.dataUrl = "http://www.test0.com";
                visualReminderEvent.addDetail(visualReminderDetail);
                model.saveItem(visualReminderEvent);
                //Let's wait for the model to be up-to-date
                var count = 0;
                do {
                   organizerChangedSpy.wait(100);
                   count ++;
                   verify(model.itemCount <= 1)
                   verify(count <= 10)
                } while (model.itemCount < 1)

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
                savedEventDetail.repetitionCount = -1;

                visualReminderDetail.dataUrl = "http://www.test222.com";
                visualReminderDetail.message = "visual reminder message";
                visualReminderDetail.secondsBeforeStart = 300;
                visualReminderDetail.repetitionCount = -1;

                savedVisualEvent.setDetail(savedEventDetail);

                model.saveItem(savedVisualEvent);
                organizerChangedSpy.wait();
                model.update();
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
                organizerChangedSpy.wait();
                model.update();
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
                organizerChangedSpy.clear();
                organizerChangedSpy.destroy();
                model.destroy();
                visualReminderEvent.destroy();
                visualReminderDetail.destroy();
    //            utility.empty_calendar();
            }
        }
    }
}
