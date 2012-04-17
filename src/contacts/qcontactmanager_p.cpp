/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactmanager.h"
#include "qcontactmanager_p.h"
#include "qcontactmanagerengine.h"
#include "qcontactmanagerenginefactory.h"

#include "qcontact_p.h"

#include "qcontactaction.h"
#include "qcontactactiondescriptor.h"

#include <QSharedData>
#include <QtPlugin>
#include <QPluginLoader>
#include <QWeakPointer>

#include <QDebug>
#include <QDir>
#include <QFile>

#include "qcontactinvalidbackend_p.h"
#include "qcontactspluginsearch_p.h"

#include <QtCore/private/qfactoryloader_p.h>
#include <QJsonArray>

QTCONTACTS_BEGIN_NAMESPACE

/* Shared QContactManager stuff here, default engine stuff below */
QHash<QString, QContactManagerEngineFactory*> QContactManagerData::m_engines;
QSet<QContactManager*> QContactManagerData::m_aliveEngines;
QList<QContactActionManagerPlugin*> QContactManagerData::m_actionManagers;

bool QContactManagerData::m_discoveredStatic;
QList<QJsonObject> QContactManagerData::m_pluginPaths;
QList<QJsonObject> QContactManagerData::m_metaData;
QStringList QContactManagerData::m_managerNames;


#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (QT_CONTACT_MANAGER_ENGINE_INTERFACE, QStringLiteral("/contacts")))
#endif

static void qContactsCleanEngines()
{
    // This is complicated by needing to remove any engines before we unload factories
    // guard pointers as one engine could be parent of another manager and cause doubledelete
    QList<QWeakPointer<QContactManager> > aliveManagers;
    foreach(QContactManager* manager, QContactManagerData::m_aliveEngines) {
        aliveManagers << QWeakPointer<QContactManager>(manager);
    }

    foreach(QWeakPointer<QContactManager> manager, aliveManagers) {
        if (!manager) {
            // deleting engine of one manager, could cause deleting next manager in list (aggregation case)
            continue;
        }
        // We don't delete the managers here, we just kill their engines
        // and replace it with an invalid engine (for safety :/)
        QContactManagerData* d = QContactManagerData::managerData(manager.data());

        delete d->m_engine;
        d->m_engine = new QContactInvalidEngine();
    }

    QList<QContactManagerEngineFactory*> factories = QContactManagerData::m_engines.values();

    for (int i=0; i < factories.count(); i++) {
        delete factories.at(i);
    }
    QContactManagerData::m_engines.clear();
    QContactManagerData::m_actionManagers.clear();
    QContactManagerData::m_aliveEngines.clear();
    QContactManagerData::m_pluginPaths.clear();
    QContactManagerData::m_metaData.clear();
    QContactManagerData::m_managerNames.clear();
}

static int parameterValue(const QMap<QString, QString> &parameters, const char *key, int defaultValue)
{
    if (parameters.contains(QString::fromLatin1(key))) {
        bool ok;
        int version = parameters.value(QString::fromLatin1(key)).toInt(&ok);

        if (ok)
            return version;
    }
    return defaultValue;
}

void QContactManagerData::createEngine(const QString &managerName, const QMap<QString, QString> &parameters)
{
    m_engine = 0;

    QString builtManagerName = managerName.isEmpty() ? QContactManager::availableManagers().value(0) : managerName;
    int implementationVersion = parameterValue(parameters, QTCONTACTS_IMPLEMENTATION_VERSION_NAME, -1);

    bool found = false;
    bool loadedDynamic = false;

    /* First check static factories */
    loadStaticFactories();

    /* See if we got a fast hit */
    QList<QContactManagerEngineFactory*> factories = m_engines.values(builtManagerName);
    m_lastError = QContactManager::NoError;

    while (!found) {
        foreach (QContactManagerEngineFactory* f, factories) {
            QList<int> versions = f->supportedImplementationVersions();
            if (implementationVersion == -1 ||//no given implementation version required
                    versions.isEmpty() || //the manager engine factory does not report any version
                    versions.contains(implementationVersion)) {
                m_engine = f->engine(parameters, &m_lastError);
                if (!m_engine) {
                    qWarning() << "Creation of" << managerName << "engine failed.";
                } else {
                    found = true;
                    break;
                }
            }
        }

        // Break if found or if this is the second time through
        if (loadedDynamic || found)
            break;

        // otherwise load dynamic factories and reloop
        loadFactoriesMetadata();
        if (!m_metaData.isEmpty()) {
            QFactoryLoader *l = loader();
            foreach (const QJsonObject &metaDataObject, m_metaData) {
                if (metaDataObject.value(QStringLiteral("MetaData")).toObject().value(QStringLiteral("Keys")).toArray().at(0).toString() ==
                        builtManagerName) {
                    QContactManagerEngineFactory *managerFactory = qobject_cast<QContactManagerEngineFactory *>(l->instance(m_metaData.indexOf(metaDataObject)));
                    QContactActionManagerPlugin *actionFactory = qobject_cast<QContactActionManagerPlugin *>(l->instance(m_metaData.indexOf(metaDataObject)));
                    m_engines.insertMulti(builtManagerName, managerFactory);
                    m_actionManagers.append(actionFactory);
                }
            }
        }
        factories = m_engines.values(builtManagerName);
        loadedDynamic = true;
    }

    // XXX remove this
    // the engine factory could lie to us, so check the real implementation version
    if (m_engine && (implementationVersion != -1 && m_engine->managerVersion() != implementationVersion)) {
        m_lastError = QContactManager::VersionMismatchError;
        m_engine = 0;
    }

    if (!m_engine) {
        if (m_lastError == QContactManager::NoError)
            m_lastError = QContactManager::DoesNotExistError;
        m_engine = new QContactInvalidEngine();
    }
}


/* Caller takes ownership of the id */
QContactEngineId* QContactManagerData::createEngineContactId(const QString &managerName, const QMap<QString, QString>& parameters, const QString &engineIdString)
{
    loadFactoriesMetadata();
    QContactManagerEngineFactory *engineFactory = m_engines.value(managerName);
    return engineFactory ? engineFactory->createContactEngineId(parameters, engineIdString) : NULL;
}


void QContactManagerData::loadStaticFactories()
{
    if (!m_discoveredStatic) {
#if !defined QT_NO_DEBUG
        const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

        m_discoveredStatic = true;

        /* Clean stuff up at the end */
        qAddPostRoutine(qContactsCleanEngines);

        /* Loop over all the static plugins */
        QObjectList staticPlugins = QPluginLoader::staticInstances();
        for (int i=0; i < staticPlugins.count(); i++ ){
            QContactManagerEngineFactory *f = qobject_cast<QContactManagerEngineFactory*>(staticPlugins.at(i));
            if (f) {
                QString name = f->managerName();

#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Static: found an engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name)) {
                        qWarning() << "Static contacts plugin" << name << "has the same name as a currently loaded plugin; ignored";
                    } else {
                        m_engines.insertMulti(name, f);
                    }
                } else {
                    qWarning() << "Static contacts plugin with reserved name" << name << "ignored";
                }
            }
        }
    }
}


/* Plugin loader */
void QContactManagerData::loadFactoriesMetadata()
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

    // Always do this..
    loadStaticFactories();

    QFactoryLoader *l = loader();
    QList<QJsonObject> metaData = l->metaData();
    m_metaData = metaData;
    if (m_metaData != m_pluginPaths) {
        m_pluginPaths = m_metaData;
        QString currentManagerName;
        /* Now discover the dynamic plugins */
        foreach (const QJsonObject &metaDataObject, metaData) {
            currentManagerName = metaDataObject.value(QStringLiteral("MetaData")).toObject().value(QStringLiteral("Keys")).toArray().at(0).toString();

#if !defined QT_NO_DEBUG
            if (showDebug)
                qDebug() << "Loading metadata of plugin " << currentManagerName;
#endif

            if (!currentManagerName.isEmpty())
                m_managerNames << currentManagerName;
        }
    }
}

// Observer stuff

void QContactManagerData::registerObserver(QContactManager *manager, QContactObserver *observer)
{
    if (!manager)
        return;

    QContactManagerData* d = QContactManagerData::get(manager);

    d->m_observerForContact.insert(observer->contactId(), observer);

    // If this is the first observer, connect to the engine too
    if (d->m_observerForContact.size() == 1) {
        // This takes advantage of the manager connectNotify code
        QObject::connect(manager, SIGNAL(contactsChanged(QList<QContactId>)),
                manager, SLOT(_q_contactsUpdated(QList<QContactId>)));
        QObject::connect(manager, SIGNAL(contactsRemoved(QList<QContactId>)),
                manager, SLOT(_q_contactsDeleted(QList<QContactId>)));
    }
}

void QContactManagerData::unregisterObserver(QContactManager *manager, QContactObserver *observer)
{
    Q_ASSERT(manager);

    QContactManagerData* d = QContactManagerData::get(manager);

    QContactId key = d->m_observerForContact.key(observer);
    if (!key.isNull()) {
        d->m_observerForContact.remove(key, observer);

        // If there are now no more observers, disconnect from the engine
        if (d->m_observerForContact.size() == 0) {
            // This takes advantage of the manager disconnectNotify code
            QObject::disconnect(manager, SIGNAL(contactsChanged(QList<QContactId>)),
                    manager, SLOT(_q_contactsUpdated(QList<QContactId>)));
            QObject::disconnect(manager, SIGNAL(contactsRemoved(QList<QContactId>)),
                    manager, SLOT(_q_contactsDeleted(QList<QContactId>)));
        }
    }
}

void QContactManagerData::_q_contactsUpdated(const QList<QContactId> &ids)
{
    foreach (const QContactId &id, ids) {
        QList<QContactObserver*> observers = m_observerForContact.values(id);
        foreach (QContactObserver* observer, observers) {
            QMetaObject::invokeMethod(observer, "contactChanged");
        }
    }
}

void QContactManagerData::_q_contactsDeleted(const QList<QContactId> &ids)
{
    foreach (const QContactId &id, ids) {
        QList<QContactObserver*> observers = m_observerForContact.values(id);
        foreach (QContactObserver* observer, observers) {
            QMetaObject::invokeMethod(observer, "contactRemoved");
        }
    }
}

// trampolines for private classes
QContactManagerData* QContactManagerData::get(const QContactManager *manager)
{
    return manager->d;
}

QContactManagerEngine* QContactManagerData::engine(const QContactManager *manager)
{
    if (manager)
        return manager->d->m_engine;
    return 0;
}

QTCONTACTS_END_NAMESPACE

