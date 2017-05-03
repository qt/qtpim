
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
import QtTest 1.0
import QtOrganizer 5.0


Rectangle {
    id: test;

    QOrganizerTestUtility {
        id: utility
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
        function test_addRemoveEvent() {
            var list = utility.getManagerList();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                console.log(managerName);
                var model = utility.createModel(managerName);

                utility.init(model);
                utility.empty_calendar()

                //------model save event------//
                model.saveItem(newEvent1);
                //make sure event is saved
                utility.waitModelChange(1);
                verify(model.itemCount === 1);

                var ids = model.itemIds();
                verify(ids.length === 1);
                var item = model.item(ids[0]);
                //verify DisplayLabel and description
                verify(item.displayLabel === "NewEvent1");
                verify(item.description === "This is a new created event 1");

                //------modify saved event------//
                item.displayLabel = "EditedEvent1";
                model.saveItem(item);
                //make sure exist event saved/updated and no duplicated items saved
                utility.waitModelChange(1);
                //console.log(model.itemCount);
                verify(model.itemCount === 1);
                var item2 = model.item(ids[0]);
                verify(item2.displayLabel === "EditedEvent1");

                //------delete event------//
                model.removeItem(ids[0]);
                utility.waitModelChange(0);
                verify(model.itemCount === 0);

                //delete invalid items and no crash
                model.removeItem(ids[0]);
                utility.waitModelChange(0);
                verify(model.itemCount === 0);

                //remove a list of event
                model.saveItem(newEvent2);
                utility.waitModelChange(1);
                model.saveItem(newEvent1);
                utility.waitModelChange(1);
                var deletList = model.itemIds();
                model.removeItems(deletList);
                utility.waitModelChange(0);
            }
        }
    }
}
