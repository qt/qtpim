/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
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

