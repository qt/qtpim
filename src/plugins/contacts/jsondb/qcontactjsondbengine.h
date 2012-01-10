/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QCONTACTJSONDBENGINE_H
#define QCONTACTJSONDBENGINE_H

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

#include <QMap>
#include <QString>
#include <QList>
#include <QQueue>
#include <QDebug>
#include <QVariantList>

#include <qcontacts.h>
#include <qcontactmanager.h>
#include <private/qcontactmanager_p.h>
#include "qcontactjsondbenginedata.h"

#define QCONTACT_JSONDB_ENGINE_VERSION 0.1

QTCONTACTS_BEGIN_NAMESPACE

class QContactJsonDbEngine : public QContactManagerEngine
{
    Q_OBJECT

    // XXX
public:

    // static JsonDbEngine *createJsonDbEngine(const QMap<QString, QString>& parameters); xxx organizer does this way, contacts not, check!

    ~QContactJsonDbEngine();
    QContactJsonDbEngine();
    QContactJsonDbEngine& operator=(const QContactJsonDbEngine& other);

    QString managerName() const;
    bool validateContact(const QContact&, QContactManager::Error* error) const;
    QContact compatibleContact(const QContact& contact, QContactManager::Error* error) const;

    /* "Self" contact id (MyCard) */
    QContactLocalId selfContactId(QContactManager::Error* errors) const;

    /* Filtering */
    QList<QContactLocalId> contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const;

    /* Contacts - Accessors and Mutators */
    QContact contact(const QContactLocalId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const;
    QList<QContact> contacts(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint& fetchHint, QContactManager::Error* error ) const;
    bool saveContact(QContact* contact, QContactManager::Error* error);
    bool removeContact(const QContactLocalId& contactId, QContactManager::Error* error);
    bool saveContacts(QList<QContact>*, QMap<int, QContactManager::Error>*, QContactManager::Error* error); // implemented in terms of the singular saveContact
    bool removeContacts(const QList<QContactLocalId>&, QMap<int, QContactManager::Error>*, QContactManager::Error* error); // implemented in terms of the singular removeContact

    /* Version Reporting */
    int implementationVersion() const { return QCONTACT_JSONDB_ENGINE_VERSION; };
    int managerVersion() const { return QCONTACT_JSONDB_ENGINE_VERSION; };

    /* Capabilities reporting */
    bool hasFeature(QContactManager::ManagerFeature feature, const QString& contactType) const;
    bool isFilterSupported(const QContactFilter& filter) const;
    QList<QVariant::Type> supportedDataTypes() const;
    QStringList supportedContactTypes() const {return (QStringList() << QContactType::TypeContact);}

    /* Asynchronous Request Support - synchronous versions until thread worker is stable */
    void requestDestroyed(QContactAbstractRequest *req);
    bool startRequest(QContactAbstractRequest *req);
    bool cancelRequest(QContactAbstractRequest *req);
    bool waitForRequestProgress(QContactAbstractRequest* req, int msecs);
    bool waitForRequestFinished(QContactAbstractRequest* req, int msecs);

#ifdef Q_QDOC
    static const QLatin1Constant EngineName;
#else
    Q_DECLARE_LATIN1_CONSTANT (EngineName, "jsondb");
#endif
  signals:    
    void requestReceived(QContactAbstractRequest* req);
  private:
    bool doSyncRequest(QContactAbstractRequest* req, int msecs) const;

    QQueue<QContactAbstractRequest*> m_asynchronousOperations; // async requests to be performed.
    QSharedDataPointer<QContactJsonDbEngineData> d;
    QThread* m_thread;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBENGINE_H
