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
            var list = utility.test_managerdata();
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

                var organizerChangedSpy = Qt.createQmlObject(
                        "import QtTest 1.0;"
                        + "SignalSpy {id : organizerChangedSpy;}"
                        , test);

                organizerChangedSpy.target = model
                organizerChangedSpy.signalName = "collectionsChanged"
                utility.model = model
                utility.spy = organizerChangedSpy
                utility.empty_calendar()
                //------prepare filter data: save event and collection------//
                var collectionLegnth = model.collections.length
                model.saveCollection(testCollection)

                //Let's wait for the model to be up-to-date
                var count = 0;
                do {
                    organizerChangedSpy.wait(400)
                    count ++
                    verify(count <= 10)
                } while (model.collections.length < collectionLegnth + 1)

                //we should have more than default collection now
                var savedCollection = model.collections[model.collections.length - 1];
                organizerChangedSpy.signalName = "modelChanged";
                model.saveItem(event);
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount < 1)

                model.update();
                compare(model.itemCount, 1)

                //event with new collection id
                event.collectionId = savedCollection.collectionId;
                model.saveItem(event);
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    count ++;
                    verify(model.itemCount <= 2)
                    verify(count <= 10)
                } while (model.itemCount < 2)

                model.update();
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
                count = 0;
                do {
                    organizerChangedSpy.wait(100);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount < 1)

                model.update();
                compare(model.itemCount, 1)
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId];
                //Duoble filters
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter", debugFlag);
                count = 0;
                do {
                    organizerChangedSpy.wait(100);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount != 0)

                model.update();
                compare(model.itemCount, 0)

                //Double filter 2
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId, model.defaultCollection().collectionId];
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter 2", debugFlag);
                count = 0;
                do {
                    organizerChangedSpy.wait(100);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount < 1)

                model.update();
                compare(model.itemCount, 1)

                //Double filter 3
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId, model.defaultCollection().collectionId];
                idFilter.ids = [idEventId, collectionEventId];
                utility.debug("Duoble filter ~3", debugFlag);
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter 3" + collectionFilter.ids, debugFlag);
                count = 0;
                do {
                    organizerChangedSpy.wait(100);
                    count ++;
                    verify(model.itemCount <= 2)
                    verify(count <= 10)
                } while (model.itemCount < 2)
                model.update();
                compare(model.itemCount, 2)
                utility.debug("Test over!", debugFlag);
            }
        }
    }
}
