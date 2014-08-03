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

#include "qorganizermanager_p.h"

#include <QtCore/qcoreapplication.h>
#if !defined(QT_NO_DEBUG)
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qpluginloader.h>
#include <QtCore/private/qfactoryloader_p.h>

#include "qorganizeritemobserver.h"
#include "qorganizermanagerenginefactory.h"

QT_BEGIN_NAMESPACE_ORGANIZER

QHash<QString, QOrganizerManagerEngineFactory *> QOrganizerManagerData::m_engines;
bool QOrganizerManagerData::m_discovered;
bool QOrganizerManagerData::m_discoveredStatic;
QStringList QOrganizerManagerData::m_pluginPaths;

#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (QT_ORGANIZER_MANAGER_ENGINE_INTERFACE, QLatin1String("/organizer")))
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
                        qWarning("Static organizeritems plugin %s has the same name as a currently loaded plugin; ignored", qPrintable(name));
                    else
                        m_engines.insertMulti(name, f);
                } else {
                    qWarning("Static organizeritems plugin with reserved name %s ignored", qPrintable(name));
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
                        qWarning("Organizer plugin %s has the same name as currently loaded plugin %s; ignored", qPrintable(m_pluginPaths.at(i)), qPrintable(name));
                    else
                        m_engines.insertMulti(name, f);
                } else {
                    qWarning("Organizer plugin %s with reserved name %s ignored", qPrintable(m_pluginPaths.at(i)), qPrintable(name));
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

void QOrganizerManagerData::_q_itemsUpdated(const QList<QOrganizerItemId> &ids, const QList<QOrganizerItemDetail::DetailType> &typesChanged)
{
    foreach (QOrganizerItemId id, ids) {
        QList<QOrganizerItemObserver *> observers = m_observerForItem.values(id);
        foreach (QOrganizerItemObserver *observer, observers)
            QMetaObject::invokeMethod(observer, "itemChanged", Q_ARG(QList<QOrganizerItemDetail::DetailType>, typesChanged));
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

static inline QString escapeParam(const QString &param)
{
    QString ret;
    const int len = param.length();
    ret.reserve(int(len * 1.1));
    for (int i = 0; i < len; ++i) {
        switch (param.at(i).unicode()) {
        case ':':
            ret += QStringLiteral("&#58;");
            break;
        case '=':
            ret += QStringLiteral("&equ;");
            break;
        case '&':
            ret += QStringLiteral("&amp;");
            break;
        default:
            ret += param.at(i);
            break;
        }
    }
    return ret;
}

static inline QString unescapeParam(const QString &param)
{
    QString ret(param);
    if (ret.contains(QLatin1Char('&'))) {
        ret.replace(QStringLiteral("&#58;"), QStringLiteral(":"));
        ret.replace(QStringLiteral("&equ;"), QStringLiteral("="));
        ret.replace(QStringLiteral("&amp;"), QStringLiteral("&"));
    }
    return ret;
}

/*!
    Parses the individual components of the given \a idString and fills the
    \a managerName, \a params, \a managerUri and \a engineIdString.
    Returns true if the parts could be parsed successfully, false otherwise.
*/
bool QOrganizerManagerData::parseIdString(const QString &idString, QString *managerName, QMap<QString, QString> *params, QString *managerUri, QString *engineIdString)
{
    // Format: qtorganizer:<managerid>:<key>=<value>&<key>=<value>:<engineIdString>
    // we assume that the prefix, managerid, params, and engineIdString cannot contain `:', `=', or `&'
    // similarly, that neither param keys nor param values can contain these characters

    const QStringList colonSplit = idString.split(QLatin1Char(':'), QString::KeepEmptyParts);
    if (colonSplit.size() < 2 || (engineIdString && colonSplit.size() != 4))
        return false;

    const QString prefix = colonSplit.at(0);
    const QString mgrName = colonSplit.at(1);
    const QString paramString = colonSplit.value(2);

    if (prefix != QStringLiteral("qtorganizer") || mgrName.isEmpty())
        return false;

    if (!paramString.isEmpty()) {
        // Now we have to decode each parameter
        QMap<QString, QString> outParams;
        const QStringList pairs = paramString.split(QRegExp(QStringLiteral("&(?!(amp;|equ;|#))")), QString::KeepEmptyParts);
        for (int i = 0; i < pairs.size(); ++i) {
            // This should be something like "foo&amp;bar&equ;=grob&amp;"
            const QStringList pair = pairs.at(i).split(QLatin1Char('='), QString::KeepEmptyParts);
            if (pair.size() != 2)
                return false;

            QString arg = pair.at(0);
            QString param = pair.at(1);

            if (arg.isEmpty())
                return false;

            arg = unescapeParam(arg);
            param = unescapeParam(param);

            outParams.insert(arg, param);
        }

        if (params)
            *params = outParams;
    }

    if (managerName)
        *managerName = unescapeParam(mgrName);

    if (managerUri)
        *managerUri = cachedUri(prefix + QLatin1Char(':') + mgrName + QLatin1Char(':') + paramString);

    // and unescape the engine id string
    if (engineIdString)
        *engineIdString = unescapeParam(colonSplit.at(3));

    return true;
}

/*!
    Returns an ID string that describes a manager name and parameters with which to instantiate
    a manager object, from the given \a managerUri.
    If \a engineIdString is non-null, the generated ID string is suitable for
    passing to QOrganizerCollectionId::fromString() or QOrganizerItemId::fromString().
*/
QString QOrganizerManagerData::buildIdString(const QString &managerUri, const QString &engineIdString)
{
    if (!engineIdString.isNull())
        return managerUri + QLatin1Char(':') + escapeParam(engineIdString);

    return managerUri;
}

/*!
    Returns a ID string that describes a manager name and parameters with which to instantiate
    a manager object, from the given \a managerName and \a params.
    If \a engineIdString is non-null, the generated ID string is suitable for
    passing to QOrganizerCollectionId::fromString() or QOrganizerItemId::fromString().
*/
QString QOrganizerManagerData::buildIdString(const QString &managerName, const QMap<QString, QString> &params, const QString &engineIdString)
{
    // Format: qtorganizer:<managerid>:<key>=<value>&<key>=<value>:<engineIdString>
    // if the prefix, managerid, param keys, param values, or engineIdString contain `:', `=', or `&',
    // we escape them to `&#58;', `&equ;', and `&amp;', respectively

    QString idString;

    // we have to escape each param
    QMap<QString, QString>::const_iterator it = params.constBegin();
    for ( ; it != params.constEnd(); ++it) {
        if (it.key().isEmpty())
            continue;
        if (!idString.isEmpty())
            idString += QLatin1Char('&');
        idString += escapeParam(it.key()) + QLatin1Char('=') + escapeParam(it.value());
    }

    idString = QStringLiteral("qtorganizer:") + escapeParam(managerName) + QLatin1Char(':') + idString;
    if (!engineIdString.isNull())
        idString += QLatin1Char(':') + escapeParam(engineIdString);

    return idString;
}

/*!
    Returns a cached instance of the manager URI string that matches \a managerUri.
    This instance should be preferred when constructing ID objects in order to promote
    data sharing of the URI string.
*/
QString QOrganizerManagerData::cachedUri(const QString &managerUri)
{
    static QStringList managerUris;

    int index = managerUris.indexOf(managerUri);
    if (index != -1)
        return managerUris.at(index);

    managerUris.append(managerUri);
    return managerUri;
}

QT_END_NAMESPACE_ORGANIZER
