/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtPim module of the Qt Toolkit
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

#include <QContact>
#include <QContactManager>
#include <QContactManagerEngine>
#include <QContactManagerEngineFactory>
#include <QContactAbstractRequest>
#include <QContactChangeSet>
#include <qcontactengineid.h>
#include <qcontactmanagerenginefactory.h>


QTCONTACTS_BEGIN_NAMESPACE

class QContactAbstractRequest;
class QContactManagerEngine;
class QContactMemoryEngine;

class QContactMemoryEngineFactory : public QObject, public QContactManagerEngineFactory
{
    Q_OBJECT
    Q_INTERFACES(QtContacts::QContactManagerEngineFactory)
public:
    QContactManagerEngine* engine(const QMap<QString, QString> &parameters, QContactManager::Error*);
    QString managerName() const;
    QContactEngineId* createContactEngineId(const QMap<QString, QString> &parameters, const QString &idString) const;
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
    QList<QContact> m_contacts;                    // list of contacts
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

    QList<QContactManagerEngine*> m_sharedEngines;   // The list of engines that share this data
};


class  QContactMemoryEngineId : public QContactEngineId
{
public:
    QContactMemoryEngineId();
    ~QContactMemoryEngineId();
    QContactMemoryEngineId(quint32 contactId, const QString &managerUri);
    QContactMemoryEngineId(const QContactMemoryEngineId &other);
    QContactMemoryEngineId(const QString &idString);

    bool isEqualTo(const QContactEngineId *other) const;
    bool isLessThan(const QContactEngineId *other) const;

    QString managerUri() const;

    QContactEngineId* clone() const;

    QString toString() const;

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug &dbg) const;
#endif
    uint hash() const;

private:
    quint32 m_contactId;
    QString m_managerUri;
    friend class QContactMemoryEngine;
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
    /*! \reimp */
    int managerVersion() const {return 1;}

    virtual QList<QContactId> contactIds(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, QContactManager::Error *error) const;
    virtual QList<QContact> contacts(const QContactFilter &filter, const QList<QContactSortOrder> &sortOrders, const QContactFetchHint &fetchHint, QContactManager::Error *error) const;
    virtual QContact contact(const QContactId &contactId, const QContactFetchHint &fetchHint, QContactManager::Error *error) const;

    virtual bool saveContacts(QList<QContact> *contacts, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);
    virtual bool removeContacts(const QList<QContactId> &contactIds, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

    /*! \reimp */
    virtual QContact compatibleContact(const QContact &original, QContactManager::Error *error) const
    {
        return QContactManagerEngine::compatibleContact(original, error);
    }

    /*! \reimp */
    virtual QString synthesizedDisplayLabel(const QContact &contact, QContactManager::Error *error) const
    {
        return QContactManagerEngine::synthesizedDisplayLabel(contact, error);
    }

    /* "Self" contact id (MyCard) */
    virtual bool setSelfContactId(const QContactId &contactId, QContactManager::Error *error);
    virtual QContactId selfContactId(QContactManager::Error *error) const;

    /* Relationships between contacts */
    virtual QList<QContactRelationship> relationships(const QString &relationshipType, const QContact &participant, QContactRelationship::Role role, QContactManager::Error *error) const;
    virtual bool saveRelationships(QList<QContactRelationship> *relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);
    virtual bool removeRelationships(const QList<QContactRelationship> &relationships, QMap<int, QContactManager::Error> *errorMap, QContactManager::Error *error);

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
    virtual bool hasFeature(QContactManager::ManagerFeature feature, const QString &contactType) const;
    virtual bool isRelationshipTypeSupported(const QString &relationshipType, const QString &contactType) const;
    virtual bool isFilterSupported(const QContactFilter &filter) const;
    virtual QList<QVariant::Type> supportedDataTypes() const;
    /*! \reimp */
    virtual QStringList supportedContactTypes() const
    {
        return QContactManagerEngine::supportedContactTypes();
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
    void performAsynchronousOperation(QContactAbstractRequest *request);

    QContactMemoryEngineData *d;
    static QMap<QString, QContactMemoryEngineData*> engineDatas;

    friend class QContactMemoryEngineData;
};

QTCONTACTS_END_NAMESPACE

#endif

