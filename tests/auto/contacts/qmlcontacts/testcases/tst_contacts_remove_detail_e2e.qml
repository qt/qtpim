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
    id: testcase
    name: "ContactsRemoveDetailE2ETests"

    ContactModel {
        id: model
        manager: getManagerUnderTest()
        autoUpdate: true
    }

    // Tests

    Contact {
        id: contact1
        Name {
            firstName: "1"
        }
    }

    function test_remove_detail() {
        contact1.removeDetail(contact1.detail(ContactDetail.Name))
        verifyIsUndefined(contact1.detail(ContactDetail.Name))
    }

    Contact {
        id: contact2
        Name {
            firstName: "2"
        }
    }

    function test_remove_detail_of_contact_in_the_model() {
        model.saveContact(contact2)
        waitForContactsChanged()
        var contact = model.contacts[0]
        contact.removeDetail(contact.detail(ContactDetail.Name))
        verifyIsUndefined(contact.detail(ContactDetail.Name));
    }

    Contact {
        id: contact3
        Name {
            firstName: "3"
        }
    }

    function test_remove_detail_and_save_modified() {
        model.saveContact(contact3)
        waitForContactsChanged()
        var contact = model.contacts[0]
        contact.removeDetail(contact.detail(ContactDetail.Name))
        contact.save()
        waitForContactsChanged()
        verifyIsUndefined(contact.detail(ContactDetail.Name))
    }

    Contact {
        id: contact3_1
        Name {
            firstName: "3_1"
        }
    }

    function test_remove_detail_save_modified_and_refetch_from_model() {
        model.saveContact(contact3_1)
        waitForContactsChanged()
        var contact = model.contacts[0]
        contact.removeDetail(contact.detail(ContactDetail.Name))
        contact.save()
        waitForContactsChanged()
        contact = model.contacts[0]
        verifyIsUndefined(contact.detail(ContactDetail.Name))
    }

    Contact {
        id: contact4
        Name {
            firstName: "4"
        }
    }

    function test_remove_detail_and_save_through_model() {
        model.saveContact(contact4)
        waitForContactsChanged()
        var contact = model.contacts[0]
        contact.removeDetail(contact.detail(ContactDetail.Name))

        model.saveContact(contact)
        waitForContactsChanged()

        contact = model.contacts[0]
        verifyIsUndefined(contact.detail(ContactDetail.Name))
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

    function cleanupTestCase() {
        emptyContacts(model);
        finishTestForModel(model);
    }

    // Helpers

    function verifyIsUndefined(object) {
        verify(!object, "Object " + object + " is undefined");
    }
}
