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
import QtContacts 2.0

Item {
    id:myModel;
    property ContactModel model;

    property Contact myContact : Contact {
        id:myContact;
        PhoneNumber {
            number: ""
        }
    }

    property Contact mycontact2 : Contact {
        id:mycontact2
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
            myModel.myContact.phoneNumbers[0].number = contactData.phoneNumber;
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

