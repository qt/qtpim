/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include <private/qorganizermanager_p.h>
#include <qorganizeritemobserver.h>
#include <qorganizermanagerenginefactory.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qpluginloader.h>
#include <QtCore/private/qfactoryloader_p.h>

QTORGANIZER_BEGIN_NAMESPACE

QHash<QString, QOrganizerManagerEngineFactory *> QOrganizerManagerData::m_engines;
bool QOrganizerManagerData::m_discovered;
bool QOrganizerManagerData::m_discoveredStatic;
QStringList QOrganizerManagerData::m_pluginPaths;

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (QT_ORGANIZER_MANAGER_ENGINE_INTERFACE, QStringLiteral("/organizer")))
#endif

static void qOrganizerItemsCleanEngines()
{
    QOrganizerManagerData::m_discovered = false;
    QList<QOrganizerManagerEngineFactory *> factories = QOrganizerManagerData::m_engines.values();
    for (int i=0; i < factories.count(); i++)
        delete factories.at(i);
    QOrganizerManagerData::m_engines.clear();
}


static int parameterValue(const QMap<QString, QString> &parameters, const char *key, int defaultValue)
{
    if (parameters.contains(QString::fromAscii(key))) {
        bool ok;
        int version = parameters.value(QString::fromAscii(key)).toInt(&ok);

        if (ok)
            return version;
    }
    return defaultValue;
}

void QOrganizerManagerData::createEngine(const QString &managerName, const QMap<QString, QString> &parameters)
{
    m_engine = 0;

    QString builtManagerName = managerName.isEmpty() ? QOrganizerManager::availableManagers().value(0) : managerName;

    int implementationVersion = parameterValue(parameters, QTORGANIZER_IMPLEMENTATION_VERSION_NAME, -1);

    bool found = false;
    bool loadedDynamic = false;

    /* First check static factories */
    loadStaticFactories();

    /* See if we got a fast hit */
    QList<QOrganizerManagerEngineFactory *> factories = m_engines.values(builtManagerName);
    m_lastError = QOrganizerManager::NoError;

    while (!found) {
        foreach (QOrganizerManagerEngineFactory *f, factories) {
            QList<int> versions = f->supportedImplementationVersions();
            if (implementationVersion == -1 //no given implementation version required
                || versions.isEmpty() //the manager engine factory does not report any version
                || versions.contains(implementationVersion)) {
                m_engine = f->engine(parameters, &m_lastError);
                found = true;
                break;
            }
        }

        // Break if found or if this is the second time through
        if (loadedDynamic || found)
            break;

        // otherwise load dynamic factories and reloop
        loadFactories();
        factories = m_engines.values(builtManagerName);
        loadedDynamic = true;
    }

    // XXX remove this
    // the engine factory could lie to us, so check the real implementation version
    if (m_engine && (implementationVersion != -1 && m_engine->managerVersion() != implementationVersion)) {
        m_lastError = QOrganizerManager::VersionMismatchError;
        m_engine = 0;
    }

    if (!m_engine) {
        if (m_lastError == QOrganizerManager::NoError)
            m_lastError = QOrganizerManager::DoesNotExistError;
        m_engine = new QOrganizerManagerEngine();
    }
}

void QOrganizerManagerData::loadStaticFactories()
{
    if (!m_discoveredStatic) {
#if !defined QT_NO_DEBUG
        const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

        m_discoveredStatic = true;

        /* Clean stuff up at the end */
        qAddPostRoutine(qOrganizerItemsCleanEngines);

        /* Loop over all the static plugins */
        QObjectList staticPlugins = QPluginLoader::staticInstances();
        for (int i = 0; i < staticPlugins.count(); i++ ){
            QOrganizerManagerEngineFactory *f = qobject_cast<QOrganizerManagerEngineFactory *>(staticPlugins.at(i));
            if (f) {
                QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Static: found an engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name))
                        qWarning() << "Static organizeritems plugin" << name << "has the same name as a currently loaded plugin; ignored";
                    else
                        m_engines.insertMulti(name, f);
                } else {
                    qWarning() << "Static organizeritems plugin with reserved name" << name << "ignored";
                }
            }
        }
    }
}

/* Plugin loader */
void QOrganizerManagerData::loadFactories()
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

    // Always do this..
    loadStaticFactories();

    QFactoryLoader *l = loader();
    QStringList keys = l->keys();
    if (!m_discovered || keys != m_pluginPaths) {
        m_discovered = true;
        m_pluginPaths = keys;

        for (int i = 0; i < keys.size(); ++i) {
            QOrganizerManagerEngineFactory *f = qobject_cast<QOrganizerManagerEngineFactory *>(l->instance(keys.at(i)));
            if (f) {
                const QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Dynamic: found a organizer engine plugin" << f << "with name" << name;
#endif
                if (name != QLatin1String("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name))
                        qWarning() << "Organizer plugin" << m_pluginPaths.at(i) << "has the same name as currently loaded plugin" << name << "; ignored";
                    else
                        m_engines.insertMulti(name, f);
                } else {
                    qWarning() << "Organizer plugin" << m_pluginPaths.at(i) << "with reserved name" << name << "ignored";
                }
            }

#if !defined QT_NO_DEBUG
            if (showDebug && !f) {
                qDebug() << "Unknown plugin!";
                if (l->instance(keys.at(i)))
                    qDebug() << "[qobject:" << l->instance(keys.at(i)) << "]";
            }
#endif
        }

        QStringList engineNames;
        foreach (QOrganizerManagerEngineFactory *f, m_engines.values()) {
            QStringList versions;
            foreach (int v, f->supportedImplementationVersions())
                versions << QString::fromAscii("%1").arg(v);
            engineNames << QString::fromAscii("%1[%2]").arg(f->managerName()).arg(versions.join(QString::fromAscii(",")));
        }
#if !defined QT_NO_DEBUG
        if (showDebug)
            qDebug() << "Found engines:" << engineNames;
#endif
    }
}

QOrganizerItemEngineId *QOrganizerManagerData::createEngineItemId(const QString &managerName, const QMap<QString, QString> &parameters, const QString &engineIdString)
{
    // caller takes ownership of the ID
    loadFactories();
    QOrganizerManagerEngineFactory *engineFactory = m_engines.value(managerName);
    return engineFactory ? engineFactory->createItemEngineId(parameters, engineIdString) : NULL;
}

QOrganizerCollectionEngineId *QOrganizerManagerData::createEngineCollectionId(const QString &managerName, const QMap<QString, QString> &parameters, const QString &engineIdString)
{
    // caller takes ownership of the ID
    loadFactories();
    QOrganizerManagerEngineFactory *engineFactory = m_engines.value(managerName);
    return engineFactory ? engineFactory->createCollectionEngineId(parameters, engineIdString) : NULL;
}

void QOrganizerManagerData::registerObserver(QOrganizerItemObserver *observer)
{
    m_observerForItem.insert(observer->itemId(), observer);
}

void QOrganizerManagerData::unregisterObserver(QOrganizerItemObserver *observer)
{
    QOrganizerItemId key = m_observerForItem.key(observer);
    if (!key.isNull())
        m_observerForItem.remove(key, observer);
}

void QOrganizerManagerData::_q_itemsUpdated(const QList<QOrganizerItemId> &ids)
{
    foreach (QOrganizerItemId id, ids) {
        QList<QOrganizerItemObserver *> observers = m_observerForItem.values(id);
        foreach (QOrganizerItemObserver *observer, observers)
            QMetaObject::invokeMethod(observer, "itemChanged");
    }
}

void QOrganizerManagerData::_q_itemsDeleted(const QList<QOrganizerItemId> &ids)
{
    foreach (QOrganizerItemId id, ids) {
        QList<QOrganizerItemObserver *> observers = m_observerForItem.values(id);
        foreach (QOrganizerItemObserver *observer, observers)
            QMetaObject::invokeMethod(observer, "itemRemoved");
    }
}

QOrganizerManagerData *QOrganizerManagerData::get(const QOrganizerManager *manager)
{
    return manager->d;
}

QOrganizerManagerEngine *QOrganizerManagerData::engine(const QOrganizerManager *manager)
{
    if (manager)
        return manager->d->m_engine;
    return 0;
}

QTORGANIZER_END_NAMESPACE
