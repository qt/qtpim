/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
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

OrganizerModel {
    id:myModel;

    property Event myEvent : Event {id:myEvent; startDateTime: new Date(); endDateTime: new Date() }
    property RecurrenceRule myReccuranceRule: RecurrenceRule {
                                                    id: myReccuranceRule
                                                    frequency: RecurrenceRule.Monthly
                                                    daysOfWeek:[Qt.Friday]
                                                    limit: 2
                                                }
    property bool testComplete : false

    function addEvents(testEvents)
    {
        for (var index=0; index < testEvents.length; index++){
            var event = testEvents[index]
            console.log("Creating event " + event.subject)
            myModel.myEvent.description = event.subject
            myModel.myEvent.startDateTime = event.startDateTime
            myModel.myEvent.endDateTime = event.endDateTime

            // only support testing simple RecurrenceRules ATM
            if (event.repeat){
                myReccuranceRule.frequency = event.repeat
                myReccuranceRule.limit = event.repeatCount
                myReccuranceRule.daysOfWeek = event.repeatDays
                //myModel.myEvent.recurrence.recurrenceRules = [myReccuranceRule]
            }

            // TODO verify that the OrganizerModel::itemsSaved() signal is fired
            saveItem(myModel.myEvent)
        }
        update()
        myModel.testComplete = (items.length == testEvents.length)
    }

    function testEvents()
    {
        return myModel.items
    }
}

