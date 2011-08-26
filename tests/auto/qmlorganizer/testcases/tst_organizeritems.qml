
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
import QtMobility.organizer 1.1


Rectangle {

    OrganizerModel {
        id:model
        manager:"qtorganizer:jsondb:id=qml"
        startPeriod:'2009-01-01'
        endPeriod:'2012-12-31'
        autoUpdate:false
        Component.onCompleted: {
            model.update ();
            model.autoUpdate = true;
            console.log("model is created!");
        }
    }

    Event { id: event }

    Event {
        id:newEvent1
        displayLabel: "NewEvent1"
        description: "This is a new created event 1"
        startDateTime: '2010-12-12'
        endDateTime: '2010-12-13'
    }

    Event {
        id:newEvent2
        displayLabel: "NewEvent2"
        description: "This is a new created event 2"
        startDateTime: '2010-12-13'
        endDateTime: '2010-12-14'
    }


    TestCase {
        name: "OrganizerItemTests"
        when: model.autoUpdate
        //long enough for emulator, May need longer time on real HW
        property int wait_time: 100

        //Empty calendar data
        function empty_calendar () {
            //wait for the model item update, May need longer time on real HW
            wait(wait_time);
            console.log(model.itemCount);

            var ids = model.itemIds();
            //why model.removeItems (ids); does not work?
            //remove all the exist items
            for (var i = 0; i < model.itemCount; i++)
                model.removeItem (ids[i]);

            wait(wait_time);
            verify(model.itemCount === 0);
        }

        function test_event() {
            console.log("Test start!");
            empty_calendar ();
            //event property test
            verify(!event.modified);
            verify(event.type == "Event");
            event.startDateTime = new Date();
            verify(event.startDateTime, new Date());
            event.endDateTime = new Date();
            verify(event.endDateTime, new Date());
            verify(!event.allDay);
            event.allDay = true;
            verify(event.allDay);

            //save event test
            model.saveItem(newEvent1) ;
            //make sure event is saved
            wait(wait_time);
            verify(model.itemCount === 1);
            var ids = model.itemIds();
            verify(ids.length === 1);
            var item = model.item(ids[0]);
            //verify DisplayLabel and description
            verify(item.displayLabel === "NewEvent1");
            verify(item.description === "This is a new created event 1");

//            //modify event test
//            item.displayLabel = "EditedEvent1";
//            model.saveItem(item);
//            //make sure exist event saved/updated and no duplicated items saved
//            wait(wait_time);
//            console.log(model.itemCount);
//            // verify(model.itemCount === 1);
//            var item2 = model.item(ids[0]);
//            console.log(item2.displayLabel);
//            //verify(item2.displayLabel === "EditedEvent1");

            //delete test
            console.log(model.itemCount);
            console.log(ids[0]);
            model.removeItem (ids[0]);
            wait(100);
            console.log(model.itemCount);
            verify(model.itemCount === 0);

            //delete invalid items and no crash
            model.removeItem (ids[0]);
            wait(100);
            verify(model.itemCount === 0);

            //delete invalid items and no crash
            console.log();
            model.removeItem (ids[0]);
            wait(100);
            verify(model.itemCount === 0);
        }

    }
}
