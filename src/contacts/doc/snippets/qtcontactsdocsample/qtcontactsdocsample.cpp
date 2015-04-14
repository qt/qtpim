/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtContacts/qcontacts.h>
#include "requestexample.h"

#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>

QTCONTACTS_USE_NAMESPACE

static void loadDefault();
static void queryManagerCapabilities();
static void contactDetailManipulation();
static void contactManipulation();
static void addContact(QContactManager*);
static void callContact(QContactManager*);
static void matchCall(QContactManager*, const QString&);
static void viewSpecificDetail(QContactManager*);
static void viewDetails(QContactManager*);
static void detailSharing(QContactManager*);
static void addPlugin(QContactManager*);
static void editView(QContactManager*);
static void loadManager();
static void loadManagerWithParameters();

int stopCompilerWarnings()
{
    // manager configuration examples
    loadManager();
    loadManagerWithParameters();

    // synchronous API examples
    QContactManager* cm = new QContactManager();
    addContact(cm);
    callContact(cm);
    matchCall(cm, "111-222-333"); // unknown number.
    matchCall(cm, "12345678");    // alice's number.
    viewSpecificDetail(cm);
    viewDetails(cm);
    detailSharing(cm);
    addPlugin(cm);
    editView(cm);

    // asynchronous API example
    RequestExample re;
    re.setManager(cm);
    QTimer::singleShot(10, &re, SLOT(performRequest()));
    delete cm;

    // more doc snippet examples
    loadDefault();
    queryManagerCapabilities();
    contactDetailManipulation();
    contactManipulation();

    // async doc snippet examples
    AsyncRequestExample example;
    QTimer::singleShot(10, &example, SLOT(performRequests()));

    return 0;
}


void loadDefault()
{
//! [Loading the default manager for the platform]
    QContactManager stackDefaultContactManager;
//! [Loading the default manager for the platform]

//! [Loading the default manager for the platform on heap]
    QContactManager *heapDefaultContactManager = new QContactManager;
    // ... perform contact manipulation
    delete heapDefaultContactManager;
//! [Loading the default manager for the platform on heap]
}

void queryManagerCapabilities()
{
//! [Querying a manager for capabilities]
    QContactManager cm;
    qDebug() << "The default manager for the platform is:" << cm.managerName();
    qDebug() << "It" << (cm.isRelationshipTypeSupported(QContactRelationship::HasAssistant()) ? "supports" : "does not support") << "assistant relationships.";
    qDebug() << "It" << (cm.supportedContactTypes().contains(QContactType::TypeGroup) ? "supports" : "does not support") << "groups.";
//! [Querying a manager for capabilities]
}

void contactDetailManipulation()
{
//! [Adding a detail to a contact]
    QContact exampleContact;

    QContactName nameDetail;
    nameDetail.setFirstName("Adam");
    nameDetail.setLastName("Unlikely");

    QContactPhoneNumber phoneNumberDetail;
    phoneNumberDetail.setNumber("+123 4567");

    exampleContact.saveDetail(&nameDetail);
    exampleContact.saveDetail(&phoneNumberDetail);
//! [Adding a detail to a contact]

//! [Updating a detail in a contact]
    phoneNumberDetail.setNumber("+123 9876");
    exampleContact.saveDetail(&phoneNumberDetail); // overwrites old value on save
//! [Updating a detail in a contact]

//! [Removing a detail from a contact]
    exampleContact.removeDetail(&phoneNumberDetail);
//! [Removing a detail from a contact]
}

void contactManipulation()
{
    QContactManager m_manager("memory");
//! [Synchronously creating a new contact in a manager]
    QContact exampleContact;

    QContactName nameDetail;
    nameDetail.setFirstName("Adam");
    nameDetail.setLastName("Unlikely");

    QContactPhoneNumber phoneNumberDetail;
    phoneNumberDetail.setNumber("+123 4567");

    exampleContact.saveDetail(&nameDetail);
    exampleContact.saveDetail(&phoneNumberDetail);

    // save the newly created contact in the manager
    if (!m_manager.saveContact(&exampleContact))
        qDebug() << "Error" << m_manager.error() << "occurred whilst saving contact!";
//! [Synchronously creating a new contact in a manager]

//! [Synchronously filtering contacts from a manager]
    QList<QContact> results = m_manager.contacts(QContactPhoneNumber::match("+123 4567"));
//! [Synchronously filtering contacts from a manager]

//! [Synchronously retrieving an existing contact from a manager]
    QContact existing = m_manager.contact(exampleContact.id());
//! [Synchronously retrieving an existing contact from a manager]

//! [Synchronously updating an existing contact in a manager]
    phoneNumberDetail.setNumber("+123 9876");
    exampleContact.saveDetail(&phoneNumberDetail);
    m_manager.saveContact(&exampleContact);
//! [Synchronously updating an existing contact in a manager]

//! [Synchronously removing a contact from a manager]
    m_manager.removeContact(exampleContact.id());
//! [Synchronously removing a contact from a manager]

//! [Synchronously creating a new relationship between two contacts]
    // first, create the group and the group member
    QContact exampleGroup;
    exampleGroup.setType(QContactType::TypeGroup);
    QContactNickname groupName;
    groupName.setNickname("Example Group");
    exampleGroup.saveDetail(&groupName);

    QContact exampleGroupMember;
    QContactName groupMemberName;
    groupMemberName.setFirstName("Member");
    exampleGroupMember.saveDetail(&groupMemberName);

    // second, save those contacts in the manager
    QMap<int, QContactManager::Error> errorMap;
    QList<QContact> saveList;
    saveList << exampleGroup << exampleGroupMember;
    m_manager.saveContacts(&saveList, &errorMap);

    // third, create the relationship between those contacts
    QContactRelationship groupRelationship;
    groupRelationship.setFirst(exampleGroup);
    groupRelationship.setRelationshipType(QContactRelationship::HasMember());
    groupRelationship.setSecond(exampleGroupMember);

    // finally, save the relationship in the manager
    m_manager.saveRelationship(&groupRelationship);
//! [Synchronously creating a new relationship between two contacts]

//! [Synchronously retrieving relationships between contacts]
    QList<QContactRelationship> groupRelationships = m_manager.relationships(exampleGroup, QContactRelationship::First);
    QList<QContactRelationship> result;
    for (int i = 0; i < groupRelationships.size(); i++) {
        if (groupRelationships.at(i).second() == exampleGroupMember) {
            result.append(groupRelationships.at(i));
        }
    }
//! [Synchronously retrieving relationships between contacts]

//! [Retrieving relationships from cache]
    exampleGroup = m_manager.contact(exampleGroup.id()); // refresh the group contact
    groupRelationships = exampleGroup.relationships(QContactRelationship::HasMember());
    for (int i = 0; i < groupRelationships.size(); i++) {
        if (groupRelationships.at(i).second() == exampleGroupMember) {
            result.append(groupRelationships.at(i));
        }
    }
//! [Retrieving relationships from cache]

//! [Synchronously providing a fetch hint]
    QContactFetchHint hasMemberRelationshipsOnly;
    hasMemberRelationshipsOnly.setRelationshipTypesHint(QStringList(QContactRelationship::HasMember()));

    // retrieve all contacts, with no specified sort order, requesting that
    // HasMember relationships be included in the cache of result contacts
    QList<QContact> allContacts = m_manager.contacts(QContactFilter(), QList<QContactSortOrder>(), hasMemberRelationshipsOnly);
//! [Synchronously providing a fetch hint]

//! [Synchronously removing a relationship]
    m_manager.removeRelationship(groupRelationship);
//! [Synchronously removing a relationship]
}

//! [Creating a new contact]
void addContact(QContactManager* cm)
{
    QContact alice;

    QContactDisplayLabel displayLabel;
    displayLabel.setLabel("Ally Jones");
    alice.saveDetail(&displayLabel);

    /* Set the contact's name */
    QContactName aliceName;
    aliceName.setFirstName("Alice");
    aliceName.setLastName("Jones");
    alice.saveDetail(&aliceName);

    /* Add a phone number */
    QContactPhoneNumber number;
    QList<int> subTypeMobile;
    subTypeMobile << QContactPhoneNumber::SubTypeMobile;
    number.setSubTypes(subTypeMobile);
    number.setContexts(QContactDetail::ContextHome);
    number.setSubTypes(subTypeMobile);
    number.setNumber("12345678");
    alice.saveDetail(&number);
    alice.setPreferredDetail("DialAction", number);

    /* Add a second phone number */
    QContactPhoneNumber number2;
    QList<int> subTypeLandline;
    subTypeLandline << QContactPhoneNumber::SubTypeMobile;
    number.setSubTypes(subTypeLandline);
    number2.setContexts(QContactDetail::ContextWork);
    number2.setSubTypes(subTypeLandline);
    number2.setNumber("555-4444");
    alice.saveDetail(&number2);

    /* Save the contact */
    cm->saveContact(&alice) ? qDebug() << "Successfully saved"
                            : qDebug() << "Failed to save";
    qDebug() << "The display label for the contact:" << alice.details(QContactDisplayLabel::Type).value(1).value(QContactDisplayLabel::FieldLabel);
}
//! [Creating a new contact]

void callContact(QContactManager* cm)
{
    QList<QContactId> contactIds = cm->contactIds();
    QContact a = cm->contact(contactIds.first());

    /* Get this contact's first phone number */
    QContact contact;

    //! [Details with action]
    // Get the first "Call" action
    QContactActionDescriptor callDescriptor = QContactAction::actionDescriptors("Call").value(0);
    QContactAction* action = QContactAction::action(callDescriptor);
    QSet<QContactActionTarget> targets = callDescriptor.supportedTargets(contact);

    if (targets.count() == 0) {
        // Can't call this contact
    } else if (targets.count() == 1) {
        // Just call this specific detail
        action->invokeAction(*targets.begin());
    } else {
        // Offer the user the choice of details to call
        // ...
    }
    //! [Details with action]
}

//! [Filtering by definition and value]
void matchCall(QContactManager* cm, const QString& incomingCallNbr)
{
    QContactDetailFilter phoneFilter;
    phoneFilter.setDetailType(QContactPhoneNumber::Type, QContactPhoneNumber::FieldNumber);
    phoneFilter.setValue(incomingCallNbr);
    phoneFilter.setMatchFlags(QContactFilter::MatchExactly);

    QList<QContactId> matchingContacts = cm->contactIds(phoneFilter);
    if (matchingContacts.size() == 0) {
        qDebug() << "Incoming call from unknown contact (" << incomingCallNbr << ")";
    } else {
        QContact match = cm->contact(matchingContacts.at(0));
        QContactDisplayLabel displayLabel;
        displayLabel.setLabel("Match");
        match.saveDetail(&displayLabel);
        qDebug() << "Incoming call from"
                 << match.details(QContactDisplayLabel::Type).value(0).value(QContactDisplayLabel::FieldLabel)
                 << "(" << incomingCallNbr << ")";
    }
}
//! [Filtering by definition and value]

//! [Viewing a specific detail of a contact]
void viewSpecificDetail(QContactManager* cm)
{
    QList<QContactId> contactIds = cm->contactIds();
    QContact exampleContact = cm->contact(contactIds.first());
    qDebug() << "The first phone number is" << exampleContact.detail(QContactPhoneNumber::Type).value(QContactPhoneNumber::FieldNumber);
}
//! [Viewing a specific detail of a contact]

//! [Viewing the details of a contact]
void viewDetails(QContactManager* cm)
{
    QList<QContactId> contactIds = cm->contactIds();
    QContact exampleContact = cm->contact(contactIds.first());

    QList<QContactDetail> allDetails = exampleContact.details();
    for (int i = 0; i < allDetails.size(); i++) {
        QContactDetail detail = allDetails.at(i);
        QMap<int, QVariant> fields = detail.values();

        qDebug("\tDetail #%d (%d):", i, detail.type());
        foreach (const int& fieldKey, fields.keys()) {
            qDebug() << "\t\t" << fieldKey << "(" << fields.value(fieldKey) << ") =" << detail.value(fieldKey);
        }
        qDebug();
    }
}
//! [Viewing the details of a contact]

//! [Demonstration of detail sharing semantics]
void detailSharing(QContactManager* cm)
{
    QList<QContactId> contactIds = cm->contactIds();
    QContact a = cm->contact(contactIds.first());

    /* Create a new phone number detail. */
    QContactPhoneNumber newNumber;
    newNumber.setNumber("123123123");
    qDebug() << "\tThe new phone number is" << newNumber.number();

    /*
     * Create a copy of that detail.  These will be implicitly shared;
     * changes to nnCopy will not affect newNumber, and vice versa.
     * However, attempting to save them will cause overwrite to occur.
     * Removal is done purely via key() checking, also.
     */
    QContactPhoneNumber nnCopy(newNumber);
    nnCopy.setNumber("456456456");
    qDebug() << "\tThat number is still" << newNumber.number() << ", the copy is" << nnCopy.number();

    /* Save the detail in the contact, then remove via the copy, then resave. */
    a.saveDetail(&newNumber);
    a.removeDetail(&nnCopy);  // identical to a.removeDetail(&newNumber);
    a.saveDetail(&newNumber); // since newNumber.key() == nnCopy.key();

    /* Saving will cause overwrite */
    qDebug() << "\tPrior to saving nnCopy has" << a.details().count() << "details.";
    a.saveDetail(&nnCopy);
    qDebug() << "\tAfter saving nnCopy still has" << a.details().count() << "details.";

    /* In order to save nnCopy as a new detail, we must reset its key */
    nnCopy.resetKey();
    qDebug() << "\tThe copy key is now" << nnCopy.key() << ", whereas the original key is" << newNumber.key();
    qDebug() << "\tPrior to saving (key reset) nnCopy has" << a.details().count() << "details.";
    a.saveDetail(&nnCopy);
    qDebug() << "\tAfter saving (key reset) nnCopy still has" << a.details().count() << "details.";
    a.removeDetail(&nnCopy);

    /*
     * Note that changes made to details are not
     * propagated automatically to the contact.
     * To persist changes to a detail, you must call saveDetail().
     */
    QList<QContactPhoneNumber> allNumbers = a.details<QContactPhoneNumber>();
    foreach (const QContactPhoneNumber& savedPhn, allNumbers) {
        if (savedPhn.key() != newNumber.key()) {
            continue;
        }

        /*
         * This phone number is the saved copy of the newNumber detail.
         * It is detached from the newNumber detail, so changes to newNumber
         * shouldn't affect savedPhn until saveDetail() is called again.
         */
        qDebug() << "\tCurrently, the (stack) newNumber is" << newNumber.number()
                 << ", and the saved newNumber is" << savedPhn.number();
        newNumber.setNumber("678678678");
        qDebug() << "\tNow, the (stack) newNumber is" << newNumber.number()
                 << ", but the saved newNumber is" << savedPhn.number();
    }

    /*
     * Removal of the detail depends only on the key of the detail; the fact
     * that the values differ is not taken into account by the remove operation.
     */
    a.removeDetail(&newNumber) ? qDebug() << "\tSucceeded in removing the temporary detail."
                               : qDebug() << "\tFailed to remove the temporary detail.\n";
}
//! [Demonstration of detail sharing semantics]

//! [Modifying an existing contact]
void editView(QContactManager* cm)
{
    QList<QContactId> contactIds = cm->contactIds();
    QContact a = cm->contact(contactIds.first());

    /* Change the first phone number */
    QList<QContactDetail> numbers = a.details(QContactPhoneNumber::Type);
    QContactPhoneNumber phone = numbers.value(0);
    phone.setNumber("123-4445");

    /* Add an email address */
    QContactEmailAddress email;
    email.setEmailAddress("alice.jones@example");
    email.setContexts(QContactDetail::ContextHome);
    int emailField = QContactEmailAddress::FieldEmailAddress;
    email.setValue(emailField, "Alice's Work Email Address");

    /* Save the updated details to the contact. */
    a.saveDetail(&phone);
    a.saveDetail(&email);

    /* Now we must save the updated contact back to the database. */
    cm->saveContact(&a);
    viewDetails(cm);
}
//! [Modifying an existing contact]

void displayLabel()
{
    QContactManager *manager = new QContactManager();
    QContactId myId;
//! [Updating the display label of a contact]
    /* Retrieve a contact */
    QContact exampleContact = manager->contact(myId);

    /* Set the display label */
    QContactDisplayLabel displayLabel;
    displayLabel.setLabel("Abigail Arkansas");
    exampleContact.saveDetail(&displayLabel);

    /* Update some fields that might influence the display label */
    QContactName name = exampleContact.detail<QContactName>();
    name.setFirstName("Abigail");
    name.setLastName("Arkansas");
    exampleContact.saveDetail(&name);

    qDebug() << "The display label for the contact:" << exampleContact.details(QContactDisplayLabel::Type).value(1).value(QContactDisplayLabel::FieldLabel);
//! [Updating the display label of a contact]
}

//! [Asynchronous contact request]
void RequestExample::performRequest()
{
    // retrieve any contact whose first name is "Alice"
    QContactDetailFilter dfil;
    dfil.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    dfil.setValue("Alice");
    dfil.setMatchFlags(QContactFilter::MatchExactly);

    // m_fetchRequest was created with m_fetchRequest = new QContactFetchRequest() in the ctor.
    m_fetchRequest->setManager(this->m_manager); // m_manager is a QContactManager*.
    m_fetchRequest->setFilter(dfil);
    connect(m_fetchRequest, SIGNAL(resultsAvailable()), this, SLOT(printContacts()));
    connect(m_fetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)),
            this, SLOT(stateChanged(QContactAbstractRequest::State)));
    if (!m_fetchRequest->start()) {
        qDebug() << "Unable to request contacts!";
        QCoreApplication::exit(0);
    } else {
        qDebug() << "Requested contacts; awaiting results...";
    }
}

void RequestExample::printContacts()
{
    QList<QContact> results = m_fetchRequest->contacts();
    for (m_previousLastIndex = 0; m_previousLastIndex < results.size(); ++m_previousLastIndex) {
        qDebug() << "Found an Alice:" << results.at(m_previousLastIndex);
    }
}

void RequestExample::stateChanged(QContactAbstractRequest::State state)
{
    // once we've finished retrieving results, stop processing events.
    if (state == QContactAbstractRequest::FinishedState
        || state == QContactAbstractRequest::CanceledState) {
        qDebug() << "Finished displaying asynchronously retrieved contacts!";
        QCoreApplication::exit(0);
    }
}
//! [Asynchronous contact request]


void shortsnippets()
{
    QContact contact;
    QContact groupContact;
    {
        //! [0]
        QContactDetail detail = contact.detail(QContactName::Type);
        //! [0]
        //! [1]
        QContactName name = contact.detail<QContactName>();
        //! [1]
        //! [2]
        QList<QContactDetail> details = contact.details(QContactPhoneNumber::Type);
        //! [2]
        //! [3]
        QList<QContactPhoneNumber> phoneNumbers = contact.details<QContactPhoneNumber>();
        //! [3]
        //! [4]
        //QList<QContactPhoneNumber> homePhones = contact.details(QContactPhoneNumber::Type).value(0).value(QContactPhoneNumber::FieldContext);
        //! [4]
        //! [5]
        QList<QContactRelationship> spouseRelationships = contact.relationships(QContactRelationship::HasSpouse());
        // For each relationship in spouseRelationships, contact.id() will either be first() or second()
        //! [5]
        //! [6]
        // Who are the members of a group contact?
        QList<QContact> groupMembers = groupContact.relatedContacts(QContactRelationship::HasMember(), QContactRelationship::Second);
        // What groups is this contact in?
        QList<QContact> contactGroups = contact.relatedContacts(QContactRelationship::HasMember(), QContactRelationship::First);
        // An alternative to QContact::relationships()
        QList<QContact> spouses = contact.relatedContacts(QContactRelationship::HasSpouse(), QContactRelationship::Either);
        if (spouses.count() > 1) {
            // Custom relationship type
            QList<QContact> therapists = contact.relatedContacts("HasTherapist", QContactRelationship::Second);
        }
        //! [6]
        //! [Getting all tags]
        QSet<QString> tags;
        foreach (const QContactTag& tag, contact.details<QContactTag>()) {
             tags.insert(tag.tag());
        }
        //! [Getting all tags]
        //! [Checking for a specific tag]
        if (contact.details<QContactTag>().count() > 0) {
            // Do something with it
        }
        //! [Checking for a specific tag]
    }
}

void loadManager()
{
//! [Loading a specific manager backend]
    QContactManager contactManager("KABC");
//! [Loading a specific manager backend]
}

void loadManagerWithParameters()
{
//! [Loading a specific manager backend with parameters]
    QMap<QString, QString> parameters;
    parameters.insert("Settings", "~/.qcontactmanager-kabc-settings.ini");
    QContactManager contactManager("KABC", parameters);
//! [Loading a specific manager backend with parameters]
}
