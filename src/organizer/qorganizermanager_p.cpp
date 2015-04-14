/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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
    ret.reserve(len + (len >> 3));
    for (QString::const_iterator it = param.begin(), end = param.end(); it != end; ++it) {
        switch (it->unicode()) {
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
            ret += *it;
            break;
        }
    }
    return ret;
}

static inline QByteArray escapeColon(const QByteArray &param)
{
    QByteArray ret;
    const int len = param.length();
    ret.reserve(len + (len >> 3));
    for (QByteArray::const_iterator it = param.begin(), end = param.end(); it != end; ++it) {
        switch (*it) {
        case ':':
            ret += "&#58;";
            break;
        default:
            ret += *it;
            break;
        }
    }
    return ret;
}

static inline QString unescapeParam(const QString &param)
{
    QString ret(param);
    int index = 0;
    while ((index = ret.indexOf(QLatin1Char('&'), index)) != -1) {
        const QString partial(ret.mid(index, 5));
        if (partial == QStringLiteral("&#58;"))
            ret.replace(index, 5, QStringLiteral(":"));
        else if (partial == QStringLiteral("&equ;"))
            ret.replace(index, 5, QStringLiteral("="));
        else if (partial == QStringLiteral("&amp;"))
            ret.replace(index, 5, QStringLiteral("&"));
        ++index;
    }
    return ret;
}

static inline QByteArray unescapeColon(const QByteArray &param)
{
    QByteArray ret(param);
    int index = 0;
    while ((index = ret.indexOf('&', index)) != -1) {
        const QByteArray partial(ret.mid(index, 5));
        if (partial == "&#58;")
            ret.replace(index, 5, ":");
        ++index;
    }
    return ret;
}

/*!
    Parses the individual components of the given \a uriString and fills the
    \a managerName, \a params and \a managerUri and \a localId.
    Returns true if the parts could be parsed successfully, false otherwise.
*/
bool QOrganizerManagerData::parseUri(const QString &uriString, QString *managerName, QMap<QString, QString> *params, bool strict)
{
    // Format: qtorganizer:<managerid>:<key>=<value>&<key>=<value>
    // we assume that the prefix, managerid, and params cannot contain `:', `=', or `&'
    // similarly, that neither param keys nor param values can contain these characters.

    const QStringList colonSplit = uriString.split(QLatin1Char(':'), QString::KeepEmptyParts);
    if ((colonSplit.size() != 3) && (strict || colonSplit.size() != 2))
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

    return true;
}

/*!
    Returns an ID string that describes a manager name and parameters with which to instantiate
    a manager object, from the given \a managerName and \a params.
    If \a localId is non-null, the generated ID string is suitable for
    passing to QOrganizerCollectionId::fromString() or QOrganizerItemId::fromString().
*/
QString QOrganizerManagerData::buildUri(const QString &managerName, const QMap<QString, QString> &params)
{
    // Format: qtorganizer:<managerid>:<key>=<value>&<key>=<value>
    // if the prefix, managerid, param keys, or param values contain `:', `=', or `&',
    // we escape them to `&#58;', `&equ;', and `&amp;', respectively.

    QString paramString;
    QMap<QString, QString>::const_iterator it = params.constBegin();
    for ( ; it != params.constEnd(); ++it) {
        if (it.key().isEmpty())
            continue;
        if (!paramString.isEmpty())
            paramString += QLatin1Char('&');
        paramString += escapeParam(it.key()) + QLatin1Char('=') + escapeParam(it.value());
    }

    return QStringLiteral("qtorganizer:") + escapeParam(managerName) + QLatin1Char(':') + paramString;
}

/*!
    Parses the individual components of the given \a idData and fills the
    \a managerName, \a params, \a managerUri and \a localId.
    Returns true if the parts could be parsed successfully, false otherwise.
*/
bool QOrganizerManagerData::parseIdData(const QByteArray &idData, QString *managerName, QMap<QString, QString> *params, QString *managerUri, QByteArray *localId)
{
    // Format: <managerUri>:<localId>
    int splitIndex = idData.lastIndexOf(':');
    if (splitIndex == -1)
        return false;

    const QString uriString(QString::fromUtf8(idData.mid(0, splitIndex)));
    if (!parseUri(uriString, managerName, params))
        return false;

    if (managerUri)
        *managerUri = uriString;
    if (localId)
        *localId = unescapeColon(idData.mid(splitIndex + 1));

    return true;
}

/*!
    Returns an ID string that describes a manager name and parameters with which to instantiate
    a manager object, from the given \a managerUri.
    If \a localId is non-null, the generated ID string is suitable for
    passing to QOrganizerCollectionId::fromString() or QOrganizerItemId::fromString().
*/
QByteArray QOrganizerManagerData::buildIdData(const QString &managerUri, const QByteArray &localId)
{
    // Format: <managerUri>:<localId>
    // localId cannot contain ':' so it must be escaped
    QByteArray rv = managerUri.toUtf8();
    if (!localId.isEmpty())
        rv.append(':').append(escapeColon(localId));
    return rv;
}

/*!
    Returns an ID string that describes a manager name and parameters with which to instantiate
    a manager object, from the given \a managerName and \a params.
    If \a localId is non-null, the generated ID string is suitable for
    passing to QOrganizerCollectionId::fromString() or QOrganizerItemId::fromString().
*/
QByteArray QOrganizerManagerData::buildIdData(const QString &managerName, const QMap<QString, QString> &params, const QByteArray &localId)
{
    return buildIdData(buildUri(managerName, params), localId);
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
