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

    Collection {
        id: testCollection
        name: 'My collection filter'
        description: 'IntersectionFilter test'
    }
    CollectionFilter {
        id: collectionFilter
    }
    IdFilter {
        id: idFilter
    }

    TestCase {
        name: "Organizer intersection filter test"
        function test_intersectionFilter_data() {
            return [
                        { tag:  "properties",
                            code: "import QtOrganizer 5.0;"
                                  + "IntersectionFilter{\n"
                                  + "    id:intersectionFilter"
                                  + "}\n"
                        },
                    ]
        }

        function test_intersectionFilter(data) {
            var list = utility.getManagerList();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                var debugFlag = 0;
                console.log("test_intersectionFilter test start! :" + managerName);
                var model = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "OrganizerModel {"
                        + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                        + "   startPeriod:'2009-01-01';"
                        + "   endPeriod:'2012-12-31';"
                        + "   autoUpdate:true; }"
                        , test);

                var intersectionFilter = Qt.createQmlObject(data.code, test);

                var event = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer intersection filter test event\"; "
                        + "   description: \"organizer intersection filter test event\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                utility.init(model);
                utility.waitModelChange();
                utility.empty_calendar();
                //------prepare filter data: save event and collection------//
                var collectionLegnth = model.collections.length

                model.saveCollection(testCollection)
                //Let's wait for the model to be up-to-date
                utility.waitModelChange(collectionLegnth + 1, utility.collectionChange);
                compare(model.collections.length, collectionLegnth + 1)

                //we should have more than default collection now
                var savedCollection = model.collections[model.collections.length - 1];
                model.saveItem(event);
                utility.waitModelChange(1);
                compare(model.itemCount, 1)

                //new event with new collection id
                event = Qt.createQmlObject(
                            "import QtOrganizer 5.0;"
                            + "Event { "
                            + "   id:event;"
                            + "   displayLabel: \"organizer intersection filter test event\"; "
                            + "   description: \"organizer intersection filter test event\"; "
                            + "   startDateTime: '2010-12-12'; "
                            + "   endDateTime: '2010-12-13'; }"
                            , test);
                event.collectionId = savedCollection.collectionId;
                model.saveItem(event);
                utility.waitModelChange(2);
                compare(model.itemCount, 2)
                var fetchlist = model.items;
                var idEventId;
                var collectionEventId;
                if (fetchlist[1].collectionId == savedCollection.collectionId) {
                    idEventId = fetchlist[0].itemId;
                    collectionEventId = fetchlist[1].itemId;
                } else if (fetchlist[0].collectionId == savedCollection.collectionId) {
                    idEventId = fetchlist[1].itemId;
                    collectionEventId = fetchlist[0].itemId;
                } else {
                    console.log("Something Wrong!");
                }

                utility.debug("Single filter", debugFlag);
                idFilter.ids = [idEventId];
                //Single filter
                intersectionFilter.filters = [idFilter];
                model.filter = intersectionFilter;
                utility.waitModelChange(1);
                compare(model.itemCount, 1)
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId];
                //Duoble filters
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter", debugFlag);
                utility.waitModelChange(0);
                compare(model.itemCount, 0)

                //Double filter 2
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId, model.defaultCollectionId()];
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter 2", debugFlag);
                utility.waitModelChange(1);
                compare(model.itemCount, 1)

                //Double filter 3
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId, model.defaultCollectionId()];
                idFilter.ids = [idEventId, collectionEventId];
                utility.debug("Duoble filter ~3", debugFlag);
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter 3" + collectionFilter.ids, debugFlag);
                utility.waitModelChange(2);
                compare(model.itemCount, 2)
                utility.debug("Test over!", debugFlag);

                utility.empty_calendar();
            }
        }
    }
}
