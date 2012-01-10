/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtPim module of the Qt Toolkit.
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
import QtContacts 5.0

ContactsSavingTestCase {
    name: "ContactsExtendedDetailE2ETests"

    ContactModel {
        id: modelForSaveAndFetch
        autoUpdate:true
    }


    property bool myBoolean: false

    Contact {
        id: contactForBoolean
        ExtendedDetail {
            name: "boolean"
            data: myBoolean
        }
    }

    function test_withBoolean()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForBoolean);
        compare(fetchedExtendedDetail.name, "boolean", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myBoolean, "extendDetail.data");
    }


    property color myColor: "red"

    Contact {
        id: contactForColor
        ExtendedDetail {
            name: "color"
            data: myColor
        }
    }

    function test_withColor()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForColor);
        compare(fetchedExtendedDetail.name, "color", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myColor, "extendDetail.data");
    }


    property date myDate: "2001-08-14"

    Contact {
        id: contactForDate
        ExtendedDetail {
            name: "date"
            data: myDate
        }
    }

    function test_withDate()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForDate);
        compare(fetchedExtendedDetail.name, "date", "extendDetail.name");
        if (modelForSaveAndFetch.manager == "jsondb") {
            // In jsondb date is a bit tricky as UTC format with trailing Zulu is used.
            // Also backend does not convert it back but returns a string in the zulu format.
            compare(fetchedExtendedDetail.data, "2001-08-14T00:00:00Z", "extendDetail.data");
       } else {
            compare(fetchedExtendedDetail.data, myDate, "extendDetail.data");
       }
    }


    property double myDouble: 4.5

    Contact {
        id: contactForDouble
        ExtendedDetail {
            name: "double"
            data: myDouble
        }
    }

    function test_withDouble()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForDouble);
        compare(fetchedExtendedDetail.name, "double", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myDouble, "extendDetail.data");
    }


    property int myInt: 42

    Contact {
        id: contactForInt
        ExtendedDetail {
            name: "int"
            data: myInt
        }
    }

    function test_withInt()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForInt);
        compare(fetchedExtendedDetail.name, "int", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myInt, "extendDetail.data");
    }


    property int myNegativeInt: -52

    Contact {
        id: contactForNegativeInt
        ExtendedDetail {
            name: "negativeInt"
            data: myNegativeInt
        }
    }

    function test_withNegativeInt()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForNegativeInt);
        compare(fetchedExtendedDetail.name, "negativeInt", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myNegativeInt, "extendDetail.data");
    }


    property real  myReal: 4.4

    Contact {
        id: contactForReal
        ExtendedDetail {
            name: "real"
            data: myReal
        }
    }

    function test_withReal()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForReal );
        compare(fetchedExtendedDetail.name, "real", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myReal, "extendDetail.data");
    }


    property real  myNegativeReal: -4.5

    Contact {
        id: contactForNegativeReal
        ExtendedDetail {
            name: "negativeReal"
            data: myNegativeReal
        }
    }

    function test_withNegReal()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForNegativeReal);
        compare(fetchedExtendedDetail.name, "negativeReal", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myNegativeReal, "extendDetail.data");
    }


    property string myString: "Hello world!"

    Contact {
        id: contactForString
        ExtendedDetail {
            name: "string"
            data: myString
        }
    }

    function test_withString()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForString);
        compare(fetchedExtendedDetail.name, "string", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myString, "extendDetail.data");
    }


    property url myUrl: "http://ovi.com"

    Contact {
        id: contactForUrl
        ExtendedDetail {
            name: "url"
            data: myUrl
        }
    }

    function test_withUrl()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForUrl);
        compare(fetchedExtendedDetail.name, "url", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myUrl, "extendDetail.data");
    }


    property string stringForVariant: "StringForVariant"
    property variant myVariant: stringForVariant

    Contact {
        id: contactForVariant
        ExtendedDetail {
            name: "stringInVariant"
            data: myVariant
        }
    }

    function test_withVariant()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForVariant);
        compare(fetchedExtendedDetail.name, "stringInVariant", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myVariant, "extendDetail.data Variant");
        compare(fetchedExtendedDetail.data, stringForVariant, "extendDetail.data String");
    }


    property variant myIntList: [ 1, 2, 3, 4, 5, 6, 7]

    Contact {
        id: contactForIntList
        ExtendedDetail {
            name: "intList"
            data: myIntList
        }
    }


    function test_withIntList()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForIntList);
        compare(fetchedExtendedDetail.name, "intList", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myIntList, "extendDetail.data");
    }


    property variant myStringList:[ "one", "two", "three", "four"]

    Contact {
        id: contactForStringList
        ExtendedDetail {
            name: "stringList"
            data: myStringList
        }
    }

    function test_withStringList()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForStringList);
        compare(fetchedExtendedDetail.name, "stringList", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myStringList, "extendDetail.data");
    }


    property variant myVariantList: [ "one", "two", 3, 4, 5, 6, 7]

    Contact {
        id: contactForVariantList
        ExtendedDetail {
            name: "variantList"
            data: myVariantList
        }
    }

    function test_withVariantList()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForVariantList);
        compare(fetchedExtendedDetail.name, "variantList", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myVariantList, "extendDetail.data");
    }


    property variant myEmbeddedVariantLists: [["one", "two"], 3, 4, 5, [6, 7], 8, "nine",  ["ten", 11]]

    Contact {
        id: contactForEmbeddedVariantLists
        ExtendedDetail {
            name: "embeddedVariantLists"
            data: myEmbeddedVariantLists
        }
    }

    function test_withEmbeddedVariantsList()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForEmbeddedVariantLists);
        compare(fetchedExtendedDetail.name, "embeddedVariantLists", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myEmbeddedVariantLists, "extendDetail.data");

    }


    property variant myDeeblyEmbeddedVariantLists: [["one", "two", [3, 4, [5, 6], 7], 8], 9, "ten", [ "eleven", 12]]

    Contact {
        id: contactForDeeblyEmbeddedVariantLists
        ExtendedDetail {
            name: "deeblyEmbeddedVariantLists"
            data: myDeeblyEmbeddedVariantLists
        }
    }

    function test_withDeeblyEmbeddedVariantLists()
    {
        var fetchedExtendedDetail = saveAndFetch(contactForDeeblyEmbeddedVariantLists);
        compare(fetchedExtendedDetail.name, "deeblyEmbeddedVariantLists", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myDeeblyEmbeddedVariantLists, "extendDetail.data");
    }

    Contact {
        id: contactForMap
        ExtendedDetail {
            name: "mapData"
        }
    }

    function test_withMap()
    {
        var myMapData = { key1: 1, key2: 2, key3: 3}
        contactForMap.extendedDetail.data = myMapData;
        var fetchedExtendedDetail = saveAndFetch(contactForMap);
        compare(fetchedExtendedDetail.name, "mapData", "extendDetail.name");
        compare(fetchedExtendedDetail.data, myMapData, "extendDetail.data");
    }

    Contact {
        id: contactWithThreeExtendedDetails
        ExtendedDetail {
            name: "First"
            data: 1
        }
        ExtendedDetail {
            name: "Second"
            data: 2
        }
        ExtendedDetail {
            name: "Third"
            data: 3
        }
    }

    function test_extendedDetailsListProperty_multipleDetails()
    {
        var fetchedContact = saveAndFetchContact(contactWithThreeExtendedDetails);
        var fetchedExtendedDetails = fetchedContact.extendedDetails;
        compare(fetchedExtendedDetails.length, 3, "extendedDetails.length");
        compare(fetchedExtendedDetails[0], fetchedContact.extendedDetail, "extendedDetail == extendedDetails[0]");
        compareExtendedDetails(fetchedExtendedDetails[0], contactWithThreeExtendedDetails.extendedDetails[0]);
        compareExtendedDetails(fetchedExtendedDetails[1], contactWithThreeExtendedDetails.extendedDetails[1]);
        compareExtendedDetails(fetchedExtendedDetails[2], contactWithThreeExtendedDetails.extendedDetails[2]);
    }

    Contact {
        id: contactWithOneExtendedDetail
        ExtendedDetail {
            name: "First"
            data: 1
        }
    }

    function test_extendedDetailsListProperty_oneDetail()
    {
        var fetchedContact = saveAndFetchContact(contactWithOneExtendedDetail);
        var fetchedExtendedDetails = fetchedContact.extendedDetails;
        compare(fetchedExtendedDetails.length, 1, "extendedDetails.length");
        compare(fetchedExtendedDetails[0], fetchedContact.extendedDetail, "extendedDetail == extendedDetails[0]");
        compareExtendedDetails(fetchedExtendedDetails[0], contactWithOneExtendedDetail.extendedDetail);
    }

    Contact {
        id: contactWithoutExtendedDetails
    }

    function test_extendedDetailsListProperty_noDetails()
    {
        var fetchedExtendedDetails = saveAndFetchContact(contactWithoutExtendedDetails).extendedDetails;
        compare(fetchedExtendedDetails.length, 0, "extendedDetails.length");
        verify(!fetchedExtendedDetails[0], "extendedDetails[0] is undefined");
    }

    Contact {
        id: contactWithTwoExtendedDetails
        ExtendedDetail {
            name: "First"
            data: 1
        }
        ExtendedDetail {
            name: "Second"
            data: 2
        }
    }

    function test_extendedDetailsListProperty_removing_one_detail()
    {
        var fetchedExtendedDetails = saveAndFetchContact(contactWithTwoExtendedDetails).extendedDetails;
        compare(fetchedExtendedDetails.length, 2, "extendedDetails.length");
        contactWithTwoExtendedDetails.removeDetail(contactWithTwoExtendedDetails.extendedDetails[1])
        compare(contactWithTwoExtendedDetails.extendedDetails.length, 1, "extendedDetails.length");
    }

    // TODO: Test more QML and Javascript types.

    function saveAndFetch(contact)
    {
        modelForSaveAndFetch.saveContact(contact);
        waitForContactsChanged();

        modelForSaveAndFetch.fetchContacts([]);
        waitForContactsChanged();
        return modelForSaveAndFetch.contacts[0].extendedDetail;
    }

    function saveAndFetchContact(contact)
    {
        modelForSaveAndFetch.saveContact(contact);
        waitForContactsChanged();

        modelForSaveAndFetch.fetchContacts([]);
        waitForContactsChanged();
        return modelForSaveAndFetch.contacts[0];
    }

    function compareExtendedDetails(actual, expected)
    {
        compare(actual.name, expected.name, "name")
        compare(actual.data, expected.data, "data")
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(modelForSaveAndFetch);
        waitForContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(modelForSaveAndFetch);
    }

    function init() {
        initTestForModel(modelForSaveAndFetch);
    }

    function cleanup() {
        emptyContacts(modelForSaveAndFetch);
    }

    function cleanupTestCase() {
        modelForSaveAndFetch.autoUpdate = false;
    }
}
