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

#include "requestexample.h"

#include <QtContacts/qcontacts.h>

#include <QDebug>
#include <QCoreApplication>
#include <QObject>
#include <QTimer>

QTCONTACTS_USE_NAMESPACE

AsyncRequestExample::AsyncRequestExample()
    : QObject()
{
    m_manager = new QContactManager("memory");
}

AsyncRequestExample::~AsyncRequestExample()
{
    delete m_manager;
}

//! [Example of an asynchronous request slot]
void AsyncRequestExample::contactFetchRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState == QContactAbstractRequest::FinishedState) {
        QContactFetchRequest *request = qobject_cast<QContactFetchRequest*>(QObject::sender());
        if (request->error() != QContactManager::NoError) {
            qDebug() << "Error" << request->error() << "occurred during fetch request!";
            return;
        }

        QList<QContact> results = request->contacts();
        for (int i = 0; i < results.size(); i++) {
            qDebug() << "Retrieved contact:" << results.at(i);
        }
    } else if (newState == QContactAbstractRequest::CanceledState) {
        qDebug() << "Fetch operation canceled!";
    }
}
//! [Example of an asynchronous request slot]

void AsyncRequestExample::contactSaveRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState == QContactAbstractRequest::FinishedState)
        qDebug() << "Finished saving the contacts!";
    else if (newState == QContactAbstractRequest::CanceledState)
        qDebug() << "Save operation canceled!";
}

void AsyncRequestExample::contactRemoveRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState == QContactAbstractRequest::FinishedState)
        qDebug() << "Finished removing the contacts!";
    else if (newState == QContactAbstractRequest::CanceledState)
        qDebug() << "Remove operation canceled!";
}

void AsyncRequestExample::relationshipFetchRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState == QContactAbstractRequest::FinishedState)
        qDebug() << "Finished fetching the contacts!";
    else if (newState == QContactAbstractRequest::CanceledState)
        qDebug() << "Fetch operation canceled!";
}

void AsyncRequestExample::relationshipSaveRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState == QContactAbstractRequest::FinishedState)
        qDebug() << "Finished saving the contacts!";
    else if (newState == QContactAbstractRequest::CanceledState)
        qDebug() << "Save operation canceled!";
}

void AsyncRequestExample::relationshipRemoveRequestStateChanged(QContactAbstractRequest::State newState)
{
    if (newState == QContactAbstractRequest::FinishedState)
        qDebug() << "Finished removing the contacts!";
    else if (newState == QContactAbstractRequest::CanceledState)
        qDebug() << "Remove operation canceled!";
}

void AsyncRequestExample::performRequests()
{
//! [Creating a new contact in a manager]
    QContact exampleContact;

    QContactName nameDetail;
    nameDetail.setFirstName("Adam");
    nameDetail.setLastName("Unlikely");

    QContactPhoneNumber phoneNumberDetail;
    phoneNumberDetail.setNumber("+123 4567");

    exampleContact.saveDetail(&nameDetail);
    exampleContact.saveDetail(&phoneNumberDetail);

    // save the newly created contact in the manager
    connect(&m_contactSaveRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(contactSaveRequestStateChanged(QContactAbstractRequest::State)));
    m_contactSaveRequest.setManager(m_manager);
    m_contactSaveRequest.setContacts(QList<QContact>() << exampleContact);
    m_contactSaveRequest.start();
//! [Creating a new contact in a manager]

    m_contactSaveRequest.waitForFinished();

//! [Creating a new contact in a manager waiting until finished]
    m_contactSaveRequest.setManager(m_manager);
    m_contactSaveRequest.setContacts(QList<QContact>() << exampleContact);
    m_contactSaveRequest.start();
    m_contactSaveRequest.waitForFinished();
    QList<QContact> savedContacts = m_contactSaveRequest.contacts();
//! [Creating a new contact in a manager waiting until finished]

//! [Filtering contacts from a manager]
    connect(&m_contactFetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(contactFetchRequestStateChanged(QContactAbstractRequest::State)));
    m_contactFetchRequest.setManager(m_manager);
    m_contactFetchRequest.setFilter(QContactPhoneNumber::match("+123 4567"));
    m_contactFetchRequest.start();
//! [Filtering contacts from a manager]

    m_contactFetchRequest.waitForFinished();

//! [Retrieving an existing contact from a manager]
    QContactIdFilter idListFilter;
    idListFilter.setIds(QList<QContactId>() << exampleContact.id());
    m_contactFetchRequest.setManager(m_manager);
    m_contactFetchRequest.setFilter(idListFilter);
    m_contactFetchRequest.start();
//! [Retrieving an existing contact from a manager]

    m_contactFetchRequest.waitForFinished();

//! [Updating an existing contact in a manager]
    phoneNumberDetail.setNumber("+123 9876");
    exampleContact.saveDetail(&phoneNumberDetail);
    m_contactSaveRequest.setManager(m_manager);
    m_contactSaveRequest.setContacts(QList<QContact>() << exampleContact);
    m_contactSaveRequest.start();
//! [Updating an existing contact in a manager]

    m_contactFetchRequest.waitForFinished();

//! [Removing a contact from a manager]
    connect(&m_contactRemoveRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(contactRemoveRequestStateChanged(QContactAbstractRequest::State)));
    m_contactRemoveRequest.setManager(m_manager);
    m_contactRemoveRequest.setContactIds(QList<QContactId>() << exampleContact.id());
    m_contactRemoveRequest.start();
//! [Removing a contact from a manager]

    m_contactFetchRequest.waitForFinished();

//! [Creating a new relationship between two contacts]
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
    QList<QContact> saveList;
    saveList << exampleGroup << exampleGroupMember;
    m_contactSaveRequest.setContacts(saveList);
    m_contactSaveRequest.start();
    m_contactSaveRequest.waitForFinished();

    // third, create the relationship between those contacts
    QContactRelationship groupRelationship;
    groupRelationship.setFirst(exampleGroup);
    groupRelationship.setRelationshipType(QContactRelationship::HasMember());
    groupRelationship.setSecond(exampleGroupMember);

    // finally, save the relationship in the manager
    connect(&m_relationshipSaveRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(relationshipSaveRequestStateChanged(QContactAbstractRequest::State)));
    m_relationshipSaveRequest.setManager(m_manager);
    m_relationshipSaveRequest.setRelationships(QList<QContactRelationship>() << groupRelationship);
    m_relationshipSaveRequest.start();
//! [Creating a new relationship between two contacts]

    m_contactFetchRequest.waitForFinished();

//! [Retrieving relationships between contacts]
    connect(&m_relationshipFetchRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(relationshipFetchRequestStateChanged(QContactAbstractRequest::State)));
    m_relationshipFetchRequest.setManager(m_manager);
    // retrieve the list of relationships between the example group contact and the example member contact
    // where the group contact is the first contact in the relationship, and the member contact is the
    // second contact in the relationship.  In order to fetch all relationships between them, another
    // relationship fetch must be performed with their roles reversed, and the results added together.
    m_relationshipFetchRequest.setFirst(exampleGroup);
    m_relationshipFetchRequest.setSecond(exampleGroupMember);
    m_relationshipFetchRequest.start();
//! [Retrieving relationships between contacts]

    m_contactFetchRequest.waitForFinished();

//! [Providing a fetch hint]
    QContactFetchHint hasMemberRelationshipsOnly;
    hasMemberRelationshipsOnly.setRelationshipTypesHint(QStringList(QContactRelationship::HasMember()));

    m_contactFetchRequest.setManager(m_manager);
    m_contactFetchRequest.setFilter(QContactFilter()); // all contacts
    m_contactFetchRequest.setFetchHint(hasMemberRelationshipsOnly);
    m_contactFetchRequest.start();
//! [Providing a fetch hint]

//! [Removing a relationship]
    connect(&m_relationshipRemoveRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(relationshipRemoveRequestStateChanged(QContactAbstractRequest::State)));
    m_relationshipRemoveRequest.setManager(m_manager);
    m_relationshipRemoveRequest.setRelationships(QList<QContactRelationship>() << groupRelationship);
    m_relationshipRemoveRequest.start();
//! [Removing a relationship]

    QCoreApplication::exit(0);
}
