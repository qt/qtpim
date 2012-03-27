/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
        description: 'unionFilter test'
    }
    CollectionFilter {
        id: collectionFilter
    }
    IdFilter {
        id: idFilter
    }

    TestCase {
        name: "Organizer union filter test"
        function test_unionFilter_data() {
            return [
                        { tag:  "properties",
                            code: "import QtOrganizer 5.0;"
                                  + "UnionFilter{\n"
                                  + "    id:unionFilter"
                                  + "}\n"
                        },
                    ]
        }

        function test_unionFilter(data) {
            var list = utility.getManagerList();
            if (list.length < 0) {
                console.log("No manager to test");
                return;
            }
            //Test all manager backends
            for (var i = 0; i < list.length; i ++) {
                var managerName = list[i];
                var debugFlag = 0;
                console.log("test_unionFilter test start! :" + managerName);
                var model = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "OrganizerModel {"
                        + "   manager: \"qtorganizer:" + managerName + ":id=qml\";"
                        + "   startPeriod:'2009-01-01';"
                        + "   endPeriod:'2012-12-31';"
                        + "   autoUpdate:true; }"
                        , test);

                var unionFilter = Qt.createQmlObject(data.code, test);

                var event = Qt.createQmlObject(
                        "import QtOrganizer 5.0;"
                        + "Event { "
                        + "   id:event;"
                        + "   displayLabel: \"organizer union filter test event\"; "
                        + "   description: \"organizer union filter test event\"; "
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

                //we should have more than default collection now
                var savedCollection = model.collections[model.collections.length - 1];
                model.saveItem(event);
                utility.waitModelChange(1);
                compare(model.itemCount, 1)

                //event with new collection id
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
                unionFilter.filters = [idFilter];
                model.filter = unionFilter;
                utility.waitModelChange(1);

                compare(model.itemCount, 1)
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId];
                //Duoble filters
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter", debugFlag);
                utility.waitModelChange(2);
                compare(model.itemCount, 2)

                //Double filter 2
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId, model.defaultCollection().collectionId];
                model.filter.filters = [idFilter, collectionFilter]
                utility.debug("Duoble filter 2", debugFlag);
                utility.waitModelChange(2);
                compare(model.itemCount, 2)

                //Double filter 3
                //Change collection filter id
                collectionFilter.ids = [savedCollection.collectionId, model.defaultCollection().collectionId];
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
