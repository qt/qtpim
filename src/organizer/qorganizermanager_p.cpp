/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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

QT_BEGIN_NAMESPACE_ORGANIZER

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

void QOrganizerManagerData::createEngine(const QString &managerName, const QMap<QString, QString> &parameters)
{
    m_engine = 0;

    QString builtManagerName = managerName.isEmpty() ? QOrganizerManager::availableManagers().value(0) : managerName;

    bool found = false;
    bool loadedDynamic = false;

    /* First check static factories */
    loadStaticFactories();

    /* See if we got a fast hit */
    QList<QOrganizerManagerEngineFactory *> factories = m_engines.values(builtManagerName);
    m_lastError = QOrganizerManager::NoError;

    while (!found) {
        foreach (QOrganizerManagerEngineFactory *f, factories) {
            m_engine = f->engine(parameters, &m_lastError);
            if (m_engine) {
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
                if (name != QStringLiteral("invalid") && !name.isEmpty()) {
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
    const QStringList keys = l->keyMap().values();
    if (!m_discovered || keys != m_pluginPaths) {
        m_discovered = true;
        m_pluginPaths = keys;

        for (int i = 0; i < keys.size(); ++i) {
            QOrganizerManagerEngineFactory *f = qobject_cast<QOrganizerManagerEngineFactory *>(l->instance(i));
            if (f) {
                const QString name = f->managerName();
#if !defined QT_NO_DEBUG
                if (showDebug)
                    qDebug() << "Dynamic: found a organizer engine plugin" << f << "with name" << name;
#endif
                if (name != QStringLiteral("invalid") && !name.isEmpty()) {
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
                if (const QObject *instance = l->instance(i))
                    qDebug() << "[qobject:" << instance << "]";
            }
#endif
        }
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

QT_END_NAMESPACE_ORGANIZER
