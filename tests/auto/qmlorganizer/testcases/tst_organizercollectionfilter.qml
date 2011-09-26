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

    Collection {
        id: testCollection
        name: 'My collection filter test'
        description: 'collection filter test'
    }

    TestCase {
        name: "Organizer collection filter test"

        function test_collectionFilter_data() {
            return [
                        { tag:  "properties",
                            code: "import QtAddOn.organizer 2.0;"
                                  + "CollectionFilter{\n"
                                  + "    id:collectionfilter\n"
                                  + "}\n"
                        },
                    ]
        }

        function test_collectionFilter(data) {

            var list = utility.test_managerdata();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                var debugFlag = 0;
                console.log("test_collectionFilter test start! :" + managerName);
                var model = Qt.createQmlObject(
                        "import QtAddOn.organizer 2.0;"
                        + "OrganizerModel {"
                        + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                        + "   startPeriod:'2009-01-01';"
                        + "   endPeriod:'2012-12-31';"
                        + "   autoUpdate:true; }"
                        , test);

                var collectionFilter = Qt.createQmlObject(data.code, test);

                var event = Qt.createQmlObject(
                        "import QtAddOn.organizer 2.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer collection filter test event\"; "
                        + "   description: \"organizer collection filter test event\"; "
                        + "   startDateTime: '2010-12-12'; "
                        + "   endDateTime: '2010-12-13'; }"
                        , test);

                var organizerChangedSpy = Qt.createQmlObject(
                        "import QtTest 1.0;"
                        + "SignalSpy {id : organizerChangedSpy;}"
                        , test);

                organizerChangedSpy.target = model;
                organizerChangedSpy.signalName = "collectionsChanged";
                utility.model = model;
                utility.spy = organizerChangedSpy;
                utility.empty_calendar();
                //------Create and save the detail test------//
                var collectionLegnth = model.collections.length;
                model.saveCollection(testCollection)

                //Let's wait for the model to be up-to-date
                var count = 0;
                do {
                    organizerChangedSpy.wait(400);
                    count ++;
                    verify(count <= 10)
                } while (model.collections.length < collectionLegnth + 1)

                //we should have more than default collection now
                utility.debug("New collection length :" + model.collections.length, debugFlag);
                var savedCollection = model.collections[model.collections.length - 1];
                utility.debug("New collection id :" + savedCollection.collectionId, debugFlag);
                event.collectionId = savedCollection.collectionId;
                organizerChangedSpy.signalName = "modelChanged";
                model.saveItem(event);
                count = 0;
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
                utility.debug("New event collection id :" + savedEvent.collectionId, debugFlag);
                compare(savedEvent.collectionId, event.collectionId);

                //Change collection filter id
                collectionFilter.ids = [event.collectionId];
                model.filter = collectionFilter;
                count = 0;
                do {
                    organizerChangedSpy.wait(100);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount < 1)

                model.update();
                compare(model.itemCount, 1)

                //default collection
                //collectionFilter.ids = [model.defaultCollection().collectionId];
                utility.debug("default collection id :" + model.defaultCollection().collectionId, debugFlag);
                var modelCollectionFilter = model.filter;
                organizerChangedSpy.signalName = "collectionsChanged";
                modelCollectionFilter.ids = [model.defaultCollection().collectionId];

                count = 0;
                do {
                    utility.debug("175 line item count" + model.itemCount, debugFlag);
                    organizerChangedSpy.wait();
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount != 0)

                compare(model.itemCount, 0)
                //save event to default collection
                event.collectionId = model.defaultCollection().collectionId;
                model.saveItem(event);
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    utility.debug("191 line item count" + model.itemCount, debugFlag);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount < 1)
                compare(model.itemCount, 1)

                //empty ides:
                modelCollectionFilter.ids = [];
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount == 1)
                compare(model.itemCount, 0)

                //remove collection filter or set empty filter
                model.filter = null;
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    count ++;
                    utility.debug("count after save item :" + model.itemCount, debugFlag);
                    verify(model.itemCount <= 2)
                    verify(count <= 10)
                } while (model.itemCount < 2)

                compare(model.itemCount, 2);

                //reset back filter
                organizerChangedSpy.signalName = "modelChanged";
                collectionFilter = Qt.createQmlObject(data.code, test);
                collectionFilter.ids = [event.collectionId];
                model.filter = collectionFilter;
                count = 0;
                do {
                    utility.debug("228 line item count" + model.itemCount, debugFlag);
                    organizerChangedSpy.wait();
                    count ++;
                    verify(model.itemCount <= 1)
                    verify(count <= 10)
                } while (model.itemCount < 1)

                compare(model.itemCount, 1);

                //set more collection ids
                modelCollectionFilter = model.filter
                modelCollectionFilter.ids = [model.defaultCollection().collectionId, savedCollection.collectionId];
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    count ++;
                    utility.debug("244 count after wait :" + model.itemCount, debugFlag);
                    verify(model.itemCount <= 2)
                    verify(count <= 10)
                } while (model.itemCount < 2)

                compare(model.itemCount, 2);

                //One invalid collection id
                modelCollectionFilter.ids = [model.defaultCollection().collectionId, "12345666666",savedCollection.collectionId];
                count = 0;
                do {
                    organizerChangedSpy.wait(200);
                    count ++;
                    utility.debug("257 count after wait :" + model.itemCount, debugFlag);
                    verify(model.itemCount <= 2)
                    verify(count <= 10)
                } while (model.itemCount < 2)

                compare(model.itemCount, 2);
            }
        }
    }
}
