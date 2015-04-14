/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#ifndef QCONTACTMALICIOUSPLUGIN_P_H
#define QCONTACTMALICIOUSPLUGIN_P_H

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

#include <QtContacts/qcontactmanager.h>
#include <QtContacts/qcontactmanagerengine.h>
#include <QtContacts/qcontactmanagerenginefactory.h>

#include <QObject>
#include <QString>

QT_BEGIN_NAMESPACE
class QThread;
QT_END_NAMESPACE

QTCONTACTS_USE_NAMESPACE

class MaliciousThreadObject;
class MaliciousAsyncManagerEngine : public QContactManagerEngine
{
    Q_OBJECT

public:
    MaliciousAsyncManagerEngine();
    ~MaliciousAsyncManagerEngine();

    QString managerName() const;
    bool startRequest(QContactAbstractRequest* req);
    bool cancelRequest(QContactAbstractRequest *req);

    QMap<QString, QString> managerParameters() const {return QMap<QString, QString>();}
    int managerVersion() const {return 0;}

    QList<QContactId> contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sort, QContactManager::Error* error) const
    {
        return QContactManagerEngine::contactIds(filter, sort, error);
    }

    QList<QContact> contacts(const QContactFilter& filter, const QList<QContactSortOrder>& sort, const QContactFetchHint& fetch, QContactManager::Error* error) const
    {
        return QContactManagerEngine::contacts(filter, sort, fetch, error);
    }

    QContact contact(const QContactId& id, const QContactFetchHint& fetch, QContactManager::Error* error) const
    {
        return QContactManagerEngine::contact(id, fetch, error);
    }

    bool saveContacts(QList<QContact>* contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
    {
        return QContactManagerEngine::saveContacts(contacts, errorMap, error);
    }

    bool removeContacts(const QList<QContactId>& contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
    {
        return QContactManagerEngine::removeContacts(contacts, errorMap, error);
    }

    /* "Self" contact id (MyCard) */
    bool setSelfContactId(const QContactId& id, QContactManager::Error* error)
    {
        return QContactManagerEngine::setSelfContactId(id, error);
    }

    QContactId selfContactId(QContactManager::Error* error) const
    {
        return QContactManagerEngine::selfContactId(error);
    }

    /* Relationships between contacts */
    QList<QContactRelationship> relationships(const QString& relType, const QContactId& contactId, QContactRelationship::Role role, QContactManager::Error* error) const
    {
        return QContactManagerEngine::relationships(relType, contactId, role, error);
    }

    bool saveRelationships(QList<QContactRelationship>* relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
    {
        return QContactManagerEngine::saveRelationships(relationships, errorMap, error);
    }

    bool removeRelationships(const QList<QContactRelationship>& relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
    {
        return QContactManagerEngine::removeRelationships(relationships, errorMap, error);
    }

    /* Contact validation */
    bool validateContact(const QContact& contact, QContactManager::Error* error) const
    {
        return QContactManagerEngine::validateContact(contact, error);
    }

    /* Asynchronous Request Support */
    void requestDestroyed(QContactAbstractRequest* req);
    bool waitForRequestFinished(QContactAbstractRequest* req, int msecs) {return QContactManagerEngine::waitForRequestFinished(req, msecs);}

    bool isRelationshipTypeSupported(const QString& relType, QContactType::TypeValues type) const
    {
        return QContactManagerEngine::isRelationshipTypeSupported(relType, type);
    }

    bool isFilterSupported(const QContactFilter& fil) const
    {
        return QContactManagerEngine::isFilterSupported(fil);
    }

    QList<QVariant::Type> supportedDataTypes() const
    {
        return QContactManagerEngine::supportedDataTypes();
    }
    QList<QContactType::TypeValues> supportedContactTypes() const
    {
        return QContactManagerEngine::supportedContactTypes();
    }

signals:
    void doStartRequest(QContactAbstractRequest *req);
    void doFinishRequest(QContactAbstractRequest *req);
private:
    QThread* thread;
    MaliciousThreadObject* threadObject;
};

class MaliciousEngineFactory : public QContactManagerEngineFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QContactManagerEngineFactoryInterface" FILE "malicious.json")

    public:
        QContactManagerEngine* engine(const QMap<QString, QString>& parameters, QContactManager::Error* error);
        QString managerName() const;

    private:
        MaliciousAsyncManagerEngine mame;
};

#endif

