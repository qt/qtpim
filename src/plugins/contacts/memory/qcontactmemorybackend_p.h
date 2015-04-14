/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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


#ifndef QCONTACTMEMORYBACKEND_P_H
#define QCONTACTMEMORYBACKEND_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtContacts/qcontact.h>
#include <QtContacts/qcontactmanager.h>
#include <QtContacts/qcontactmanagerengine.h>
#include <QtContacts/qcontactchangeset.h>
#include <QtContacts/qcontactmanagerenginefactory.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactMemoryEngine;

class QContactMemoryEngineFactory : public QContactManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QContactManagerEngineFactoryInterface" FILE "memory.json")
public:
    QContactManagerEngine* engine(const QMap<QString, QString> &parameters, QContactManager::Error*);
    QString managerName() const;
};

class QContactMemoryEngineData : public QSharedData
{
public:
    QContactMemoryEngineData()
        : QSharedData()
        , m_refCount(QAtomicInt(1))
        , m_selfContactId()
        , m_nextContactId(1)
        , m_anonymous(false)
    {
    }

    QContactMemoryEngineData(const QContactMemoryEngineData &other)
        : QSharedData(other),
        m_refCount(QAtomicInt(1)),
        m_selfContactId(other.m_selfContactId),
        m_nextContactId(other.m_nextContactId),
        m_anonymous(other.m_anonymous)
    {
    }

    ~QContactMemoryEngineData()
    {
    }

    static QContactMemoryEngineData *data(QContactMemoryEngine *engine);

    QAtomicInt m_refCount;
    QString m_id;                                  // the id parameter value

    QContactId m_selfContactId;               // the "MyCard" contact id
    QList<QContact> m_contacts;               // list of contacts
    QHash<QContactCollectionId, QContactId> m_contactsInCollections;   // hash of contacts for each collection
    QHash<QContactCollectionId, QContactCollection> m_idToCollectionHash; // hash of id to the collection identified by that id
    QList<QContactId> m_contactIds;           // list of contact Id's
    QList<QContactRelationship> m_relationships;   // list of contact relationships
    QMap<QContactId, QList<QContactRelationship> > m_orderedRelationships; // map of ordered lists of contact relationships
    QList<QString> m_definitionIds;                // list of definition types (id's)
    quint32 m_nextContactId;
    bool m_anonymous;                              // Is this backend ever shared?
    QString m_managerUri;                        // for faster lookup.


    void emitSharedSignals(QContactChangeSet *cs)
    {
        foreach(QContactManagerEngine* engine, m_sharedEngines)
            cs->emitSignals(engine);
    }

    void emitSharedSignals(QContactCollectionChangeSet *cs)
    {
        foreach (QContactManagerEngine *engine, m_sharedEngines)
            cs->emitSignals(engine);
    }

    QList<QContactManagerEngine*> m_sharedEngines;   // The list of engines that share this data
};


class QContactMemoryEngine : public QContactManagerEngine
{
    Q_OBJECT

public:
    static QContactMemoryEngine *createMemoryEngine(const QMap<QString, QString> &parameters);

    ~QContactMemoryEngine();

    /* URI reporting */
    QString managerName() const;
    QMap<QString, QString> managerParameters() const;
    QMap<QString, QString> idInterpretationParameters() const;

    /*! \reimp */
    int managerVersion() const {return 1;}

    virtual QList<QContactId> contactIds(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, QContactManager::Error *error) const;
    virtual QList<QContact> contacts(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, const QContactFetchHint &fetchHint, QContactManager::Error *error) const;
    virtual QContact contact(const QContactId &contactId, const QContactFetchHint &fetchHint, QContactManager::Error *error) const;

    virtual bool saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);
    virtual bool removeContacts(const QList<QContactId> &contactIds, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

    /* "Self" contact id (MyCard) */
    virtual bool setSelfContactId(const QContactId &contactId, QContactManager::Error *error);
    virtual QContactId selfContactId(QContactManager::Error *error) const;

    /* Relationships between contacts */
    virtual QList<QContactRelationship> relationships(const QString &relationshipType, const QContactId &participantId, QContactRelationship::Role role, QContactManager::Error *error) const;
    virtual bool saveRelationships(QList<QContactRelationship> *relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);
    virtual bool removeRelationships(const QList<QContactRelationship> &relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

    // collections
    QContactCollectionId defaultCollectionId() const;
    QContactCollection collection(const QContactCollectionId &collectionId, QContactManager::Error *error);
    QList<QContactCollection> collections(QContactManager::Error* error);
    bool saveCollection(QContactCollection* collection, QContactManager::Error* error);
    bool removeCollection(const QContactCollectionId& collectionId, QContactManager::Error* error);

    /*! \reimp */
    virtual bool validateContact(const QContact &contact, QContactManager::Error *error) const
    {
        return QContactManagerEngine::validateContact(contact, error);
    }

    /* Asynchronous Request Support */
    virtual void requestDestroyed(QContactAbstractRequest *req);
    virtual bool startRequest(QContactAbstractRequest *req);
    virtual bool cancelRequest(QContactAbstractRequest *req);
    virtual bool waitForRequestFinished(QContactAbstractRequest *req, int msecs);

    /* Capabilities reporting */
    virtual bool isRelationshipTypeSupported(const QString &relationshipType, QContactType::TypeValues contactType) const;
    virtual bool isFilterSupported(const QContactFilter &filter) const;
    virtual QList<QVariant::Type> supportedDataTypes() const;
    /*! \reimp */
    virtual QList<QContactType::TypeValues> supportedContactTypes() const
    {
        return QContactManagerEngine::supportedContactTypes();
    }
    virtual QList<QContactDetail::DetailType> supportedContactDetailTypes() const
    {
        return QContactManagerEngine::supportedContactDetailTypes();
    }


protected:
    QContactMemoryEngine(QContactMemoryEngineData *data);

protected:
    /* Implement "signal coalescing" for batch functions via change set */
    virtual bool saveContact(QContact *theContact, QContactChangeSet &changeSet, QContactManager::Error *error);
    virtual bool removeContact(const QContactId &contactId, QContactChangeSet &changeSet, QContactManager::Error *error);
    virtual bool saveRelationship(QContactRelationship *relationship, QContactChangeSet &changeSet, QContactManager::Error *error);
    virtual bool removeRelationship(const QContactRelationship &relationship, QContactChangeSet &changeSet, QContactManager::Error *error);

private:
    /* For partial save */
    bool saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error, const QList<QContactDetail::DetailType> &mask);
    bool saveContact(QContact *theContact, QContactChangeSet &changeSet, QContactManager::Error *error, const QList<QContactDetail::DetailType> &mask);
    void partiallySyncDetails(QContact *to, const QContact &from, const QList<QContactDetail::DetailType> &mask);

    void performAsynchronousOperation(QContactAbstractRequest *request);

    QContactMemoryEngineData *d;
    static QMap<QString, QContactMemoryEngineData*> engineDatas;

    friend class QContactMemoryEngineData;
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTMEMORYBACKEND_P_H
