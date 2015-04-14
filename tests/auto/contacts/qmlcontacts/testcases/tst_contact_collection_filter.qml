/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2015 Canonical Ltd
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
import QtContacts 5.0

ContactsSavingTestCase {
    id: testcase
    name: "Contacts collection filter test"

    property string defaultCollectionId: ""

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    Collection {
        id: testCollection
        name: 'My collection filter test'
        description: 'collection filter test'
    }

    Contact {
        id: contact1;
        Name {
            firstName: "A"
        }
        PhoneNumber {
            number: "1111111111"
        }
    }

    Contact {
        id: contact2;
        Name {
            firstName: "B"
        }
        PhoneNumber {
            number: "2222222222"
        }
    }

    Contact {
        id: contact3;
        Name {
            firstName: "John Joe"
        }
        PhoneNumber {
            number: "3333333333"
        }
    }

    function createCollectionFilter(ids)
    {
        var filter = Qt.createQmlObject(
                    "import QtContacts 5.0;" +
                    "CollectionFilter {}",
                    testcase);

        filter.ids = ids
        return filter;
    }

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();
        compare(model.collections.length, 1);
        defaultCollectionId = model.collections[0].collectionId
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
        model.saveCollection(testCollection)
        waitForCollectionsChanged();
        compare(model.collections.length, 2);
        model.saveContact(contact1);
        waitForContactsChanged();
        compare(contact1.collectionId, defaultCollectionId);
        contact2.collectionId = testCollection.collectionId
        model.saveContact(contact2);
        waitForContactsChanged();
        contact3.collectionId = testCollection.collectionId
        model.saveContact(contact3);
        waitForContactsChanged();
        compare(model.contacts.length, 3);
    }

    function test_collectionFilter(data) {
        var filterDefaultCollection = createCollectionFilter([defaultCollectionId]);
        model.filter = filterDefaultCollection;
        waitForContactsChanged();
        compare(model.contacts.length, 1);

        var filterOnlyNewCollection = createCollectionFilter([testCollection.collectionId]);
        model.filter = filterOnlyNewCollection;
        waitForContactsChanged();
        compare(model.contacts.length, 2);

        var filterNewCollectionAndDefaultCollection = createCollectionFilter([defaultCollectionId, testCollection.collectionId])
        model.filter = filterNewCollectionAndDefaultCollection;
        waitForContactsChanged();
        compare(model.contacts.length, 3);

        var filterEmpty = createCollectionFilter([])
        model.filter = filterEmpty;
        waitForContactsChanged();
        compare(model.contacts.length, 0);

        var filterValidAndInvalidIds = createCollectionFilter([defaultCollectionId, "12345678", testCollection.collectionId])
        model.filter = filterValidAndInvalidIds;
        waitForContactsChanged();
        compare(model.contacts.length, 3);

        var filterWithInvalidId = createCollectionFilter(["12345678"])
        model.filter = filterWithInvalidId;
        waitForContactsChanged();
        compare(model.contacts.length, 0);
    }
}
