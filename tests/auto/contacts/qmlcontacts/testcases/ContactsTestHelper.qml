/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Mobility Components.
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

Item {
    id:myModel;
    property ContactModel model;

    property Contact myContact : Contact {
        id:myContact;
        Name {
            firstName: "";
        }
        PhoneNumber {
            number: ""
        }
    }

    property Contact mycontact2 : Contact {
        id:mycontact2
        Name {
            firstName: "";
        }
        PhoneNumber {
            number: ""
            contexts:["Home"]
            subTypes:[PhoneNumber.Mobile]
        }
        Organization {
            name: ""
            assistantName:""
        }
    }

    function addContacts(contactsData)
    {
        console.log("contactsTestHelper.addContacts: " + contactsData.length + " contacts");
        for (var index=0; index < contactsData.length; index++){
            var contactData = contactsData[index]
            myModel.myContact.name.firstName = contactData.firstName;
            myModel.myContact.name.lastName = contactData.lastName;
            myModel.myContact.phoneNumber.number = contactData.phoneNumber;
            model.saveContact (myModel.myContact)
            contactsChangedSpy.wait();
        }
    }

    function addContactDetail(contactDetails)
    {
        myModel.mycontact2.name.firstName = contactDetails[0].firstName;
        myModel.mycontact2.name.lastName = contactDetails[0].lastName;
        myModel.mycontact2.organization.name = contactDetails[0].name;
        myModel.mycontact2.organization.assistantName = contactDetails[0].assistantName;
        myModel.mycontact2.organization.startDate = contactDetails[0].startDate;
        myModel.mycontact2.organization.logoUrl = contactDetails[0].logoUrl;
        myModel.mycontact2.phoneNumber.number = contactDetails[0].phoneNumber;
        myModel.mycontact2.phoneNumber.subTypes = [contactDetails[0].phoneSubType];

        myModel.mycontact2.phoneNumber.contexts = [contactDetails[0].contexts];
        model.saveContact(myModel.mycontact2)
        contactsChangedSpy.wait();
    }

    function testContacts()
    {
        return model.contacts
    }

    function emptyContactsDb () {
        console.log("contactsTestHelper.emptyContactsDb");
        var id;
        var temp = model.contacts.length;
        for (var i = 0; i < temp; i++) {
            id = model.contacts[0].contactId;
            model.removeContact(id);
            console.log("contactsTestHelper.emptyContactsDb: remove " + id);
            contactsChangedSpy.wait();
        }
        if (model.contacts.length > 0) {
            console.log("contactsTestHelper.emptyContactsDb: Failed to empty Db");
        }
    }
}

