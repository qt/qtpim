/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QCONTACTMANAGERENGINE_H
#define QCONTACTMANAGERENGINE_H

#include <QSharedData>
#include <QMap>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QVariant>
#include <QObject>

#include "qcontact.h"
#include "qcontactmanager.h"
#include "qcontactabstractrequest.h"
#include "qcontactrequests.h"
#include "qcontactfetchhint.h"

QTCONTACTS_BEGIN_NAMESPACE

class QContactFilter;
class QContactSortOrder;

class Q_CONTACTS_EXPORT QContactManagerEngine : public QObject
{
    Q_OBJECT

public:
    QContactManagerEngine() {}

    /* URI reporting */
    virtual QString managerName() const = 0;                       // e.g. "Symbian"
    virtual QMap<QString, QString> managerParameters() const;  // e.g. "filename=private.db"
    virtual int managerVersion() const = 0;

    /* Default and only implementation of this */
    QString managerUri() const;

    /* Filtering */
    virtual QList<QContactId> contactIds(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, QContactManager::Error *error) const;
    virtual QList<QContact> contacts(const QContactFilter &filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint &fetchHint, QContactManager::Error *error) const;
    virtual QContact contact(const QContactId &contactId, const QContactFetchHint &fetchHint, QContactManager::Error *error) const;

    virtual bool saveContact(QContact *contact, QContactManager::Error *error);
    virtual bool removeContact(const QContactId &contactId, QContactManager::Error *error);
    virtual bool saveRelationship(QContactRelationship *relationship, QContactManager::Error *error);
    virtual bool removeRelationship(const QContactRelationship &relationship, QContactManager::Error *error);

    virtual bool saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);
    virtual bool removeContacts(const QList<QContactId> &contactIds, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

    /* Return a pruned or modified contact which is valid and can be saved in the backend */
    virtual QContact compatibleContact(const QContact &original, QContactManager::Error *error) const;

    /* Synthesize the display label of a contact */
    virtual QString synthesizedDisplayLabel(const QContact &contact, QContactManager::Error *error) const;

    /* "Self" contact id (MyCard) */
    virtual bool setSelfContactId(const QContactId &contactId, QContactManager::Error *error);
    virtual QContactId selfContactId(QContactManager::Error *error) const;

    /* Relationships between contacts */
    virtual QList<QContactRelationship> relationships(const QString &relationshipType, const QContact& participant, QContactRelationship::Role role, QContactManager::Error *error) const;
    virtual bool saveRelationships(QList<QContactRelationship> *relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error *error);
    virtual bool removeRelationships(const QList<QContactRelationship> &relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

    /* Validation for saving */
    virtual bool validateContact(const QContact &contact, QContactManager::Error *error) const;

    /* Asynchronous Request Support */
    virtual void requestDestroyed(QContactAbstractRequest *req);
    virtual bool startRequest(QContactAbstractRequest *req);
    virtual bool cancelRequest(QContactAbstractRequest *req);
    virtual bool waitForRequestFinished(QContactAbstractRequest *req, int msecs);

    /* Capabilities reporting */
    virtual bool hasFeature(QContactManager::ManagerFeature feature, const QString &contactType) const;
    virtual bool isRelationshipTypeSupported(const QString &relationshipType, const QString &contactType) const;
    virtual bool isFilterSupported(const QContactFilter &filter) const;
    virtual QList<QVariant::Type> supportedDataTypes() const;
    virtual QStringList supportedContactTypes() const;

Q_SIGNALS:
    void dataChanged();
    void contactsAdded(const QList<QContactId> &contactIds);
    void contactsChanged(const QList<QContactId> &contactIds);
    void contactsRemoved(const QList<QContactId> &contactIds);
    void relationshipsAdded(const QList<QContactId> &affectedContactIds);
    void relationshipsRemoved(const QList<QContactId> &affectedContactIds);
    void selfContactIdChanged(const QContactId &oldId, const QContactId &newId);

public:
    // Async update functions
    static void updateRequestState(QContactAbstractRequest *req, QContactAbstractRequest::State state);

    static void updateContactIdFetchRequest(QContactIdFetchRequest *req, const QList<QContactId>& result, QContactManager::Error error, QContactAbstractRequest::State);
    static void updateContactFetchRequest(QContactFetchRequest *req, const QList<QContact> &result, QContactManager::Error error, QContactAbstractRequest::State);
    static void updateContactRemoveRequest(QContactRemoveRequest *req, QContactManager::Error error, const QMap<int, QContactManager::Error> &errorMap, QContactAbstractRequest::State);
    static void updateContactSaveRequest(QContactSaveRequest *req, const QList<QContact> &result, QContactManager::Error error, const QMap<int, QContactManager::Error> &errorMap, QContactAbstractRequest::State);
    static void updateRelationshipSaveRequest(QContactRelationshipSaveRequest *req, const QList<QContactRelationship> &result, QContactManager::Error error, const QMap<int, QContactManager::Error> &errorMap, QContactAbstractRequest::State);
    static void updateRelationshipRemoveRequest(QContactRelationshipRemoveRequest *req, QContactManager::Error error, const QMap<int, QContactManager::Error> &errorMap, QContactAbstractRequest::State);
    static void updateRelationshipFetchRequest(QContactRelationshipFetchRequest *req, const QList<QContactRelationship> &result, QContactManager::Error error, QContactAbstractRequest::State);

    // Other protected area update functions
    static void setDetailAccessConstraints(QContactDetail *detail, QContactDetail::AccessConstraints constraints);
    static void setContactDisplayLabel(QContact *contact, const QString &displayLabel);
    static void setContactRelationships(QContact *contact, const QList<QContactRelationship> &relationships);

    /* Helper functions */
    static int compareContact(const QContact &a, const QContact &b, const QList<QContactSortOrder> &sortOrders);
    static void addSorted(QList<QContact>* sorted, const QContact &toAdd, const QList<QContactSortOrder> &sortOrders);
    static int compareVariant(const QVariant &first, const QVariant &second, Qt::CaseSensitivity sensitivity);
    static bool testFilter(const QContactFilter& filter, const QContact &contact);
    static QList<QContactId> sortContacts(const QList<QContact> &contacts, const QList<QContactSortOrder> &sortOrders);

    static QContactFilter canonicalizedFilter(const QContactFilter &filter);
    static const QContactEngineId* engineContactId(const QContactId &id);


private:
    /* QContactChangeSet is a utility class used to emit the appropriate signals */
    friend class QContactChangeSet;
};


class QContactIdFetchRequest;
class QContactFetchRequest;
class QContactSaveRequest;
class QContactRemoveRequest;
class QContactDetailDefinitionFetchRequest;
class QContactDetailDefinitionRemoveRequest;
class QContactDetailDefinitionSaveRequest;
class QContactRelationshipFetchRequest;
class QContactRelationshipSaveRequest;
class QContactRelationshipRemoveRequest;

class Q_CONTACTS_EXPORT QContactManagerEngineV2 : public QContactManagerEngine
{
    Q_OBJECT
public:
    QContactManagerEngineV2() : QContactManagerEngine() {}

    // This is the V1 function - c++ overloading rules require this here, or to use "using"
    bool saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error* error);
    virtual bool saveContacts(QList<QContact> *contacts,  const QStringList &definitionMask, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

    // Again, this is the v1 function
    QList<QContact> contacts(const QContactFilter &filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint &fetchHint, QContactManager::Error* error) const;
    virtual QList<QContact> contacts(const QList<QContactId> &contactIds, const QContactFetchHint& fetchHint, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error) const;
    static void updateContactFetchByIdRequest(QContactFetchByIdRequest *req, const QList<QContact>& result, QContactManager::Error error, const QMap<int, QContactManager::Error> &errorMap, QContactAbstractRequest::State);
};

QTCONTACTS_END_NAMESPACE

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtContacts::QContactManagerEngineV2, "org.qt-project.Qt.QContactManagerEngineV2")
QT_END_NAMESPACE

#endif
