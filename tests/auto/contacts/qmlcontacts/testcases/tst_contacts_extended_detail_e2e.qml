/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPim module of the Qt Toolkit.
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
    name: "ContactsExtendedDetailE2ETests"

    ContactModel {
        id: model
        manager: getManagerUnderTest()
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
        var fetchedExtendedDetail = saveAndUpdate(contactForBoolean);
        compare(fetchedExtendedDetail.name, "boolean", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myBoolean, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForColor);
        compare(fetchedExtendedDetail.name, "color", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myColor, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForDate);
        compare(fetchedExtendedDetail.name, "date", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myDate, "extendedDetail.data");
    }


    Contact {
        id: contactForDateAsString
        ExtendedDetail {
            name: "date"
            data: new Date(Date.UTC(2000, 0, 2, 12, 13, 14)).toISOString()
        }
    }

    function test_withDateAsString()
    {
        var fetchedExtendedDetail = saveAndUpdate(contactForDateAsString);
        compare(fetchedExtendedDetail.data, "2000-01-02T12:13:14.000Z", "data as string");
        compare(Date.parse(fetchedExtendedDetail.data),
                Date.UTC(2000, 0, 2, 12, 13, 14), "data parsed to Date (in milliseconds)");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForDouble);
        compare(fetchedExtendedDetail.name, "double", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myDouble, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForInt);
        compare(fetchedExtendedDetail.name, "int", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myInt, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForNegativeInt);
        compare(fetchedExtendedDetail.name, "negativeInt", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myNegativeInt, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForReal );
        compare(fetchedExtendedDetail.name, "real", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myReal, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForNegativeReal);
        compare(fetchedExtendedDetail.name, "negativeReal", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myNegativeReal, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForString);
        compare(fetchedExtendedDetail.name, "string", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myString, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForUrl);
        compare(fetchedExtendedDetail.name, "url", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myUrl, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForVariant);
        compare(fetchedExtendedDetail.name, "stringInVariant", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myVariant, "extendedDetail.data Variant");
        compare(fetchedExtendedDetail.data, stringForVariant, "extendedDetail.data String");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForIntList);
        compare(fetchedExtendedDetail.name, "intList", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myIntList, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForStringList);
        compare(fetchedExtendedDetail.name, "stringList", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myStringList, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForVariantList);
        compare(fetchedExtendedDetail.name, "variantList", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myVariantList, "extendedDetail.data");
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
        var fetchedExtendedDetail = saveAndUpdate(contactForEmbeddedVariantLists);
        compare(fetchedExtendedDetail.name, "embeddedVariantLists", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myEmbeddedVariantLists, "extendedDetail.data");

    }


    property variant myDeeplyEmbeddedVariantLists: [["one", "two", [3, 4, [5, 6], 7], 8], 9, "ten", [ "eleven", 12]]

    Contact {
        id: contactForDeeplyEmbeddedVariantLists
        ExtendedDetail {
            name: "deeplyEmbeddedVariantLists"
            data: myDeeplyEmbeddedVariantLists
        }
    }

    function test_withDeeplyEmbeddedVariantLists()
    {
        var fetchedExtendedDetail = saveAndUpdate(contactForDeeplyEmbeddedVariantLists);
        compare(fetchedExtendedDetail.name, "deeplyEmbeddedVariantLists", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myDeeplyEmbeddedVariantLists, "extendedDetail.data");
    }

    property variant myMap: {"key1": 1, "key2": 2, "key3": 3}

    Contact {
        id: contactForMap
        ExtendedDetail {
            name: "mapData"
            data: myMap
        }
    }

    function test_withMap()
    {
        var fetchedExtendedDetail = saveAndUpdate(contactForMap);
        compare(fetchedExtendedDetail.name, "mapData", "extendedDetail.name");
        compare(fetchedExtendedDetail.data, myMap, "extendedDetail.data");
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
        var fetchedContact = saveAndUpdateContact(contactWithThreeExtendedDetails);
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
        var fetchedContact = saveAndUpdateContact(contactWithOneExtendedDetail);
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
        var fetchedExtendedDetails = saveAndUpdateContact(contactWithoutExtendedDetails).extendedDetails;
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
        var fetchedExtendedDetails = saveAndUpdateContact(contactWithTwoExtendedDetails).extendedDetails;
        compare(fetchedExtendedDetails.length, 2, "extendedDetails.length");
        contactWithTwoExtendedDetails.removeDetail(contactWithTwoExtendedDetails.extendedDetails[1])
        compare(contactWithTwoExtendedDetails.extendedDetails.length, 1, "extendedDetails.length");
    }

    // TODO: Test more QML and Javascript types.

    function saveAndUpdate(contact)
    {
        model.saveContact(contact);
        waitForContactsChanged();

        model.update();
        waitForContactsChanged();
        return model.contacts[0].extendedDetail;
    }

    function saveAndUpdateContact(contact)
    {
        model.saveContact(contact);
        waitForContactsChanged();

        model.update();
        waitForContactsChanged();
        return model.contacts[0];
    }

    function compareExtendedDetails(actual, expected)
    {
        compare(actual.name, expected.name, "name")
        compare(actual.data, expected.data, "data")
    }

    // Init & teardown

    function initTestCase() {
        initTestForModel(model);
        waitUntilContactsChanged();
        // The wait is needed so the model is populated
        // (e.g. with garbage left from previous test runs)
        // before cleanup() is called.
        emptyContacts(model);
    }

    function init() {
        initTestForModel(model);
    }

    function cleanup() {
        emptyContacts(model);
    }

}
