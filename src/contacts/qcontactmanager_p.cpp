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

#include "qcontactmanager.h"
#include "qcontactmanager_p.h"

#if !defined(QT_NO_DEBUG)
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qjsonarray.h>
#include <QtCore/qpluginloader.h>
#include <QtCore/qpointer.h>
#include <QtCore/private/qfactoryloader_p.h>

#include "qcontact_p.h"
#include "qcontactaction.h"
#include "qcontactactiondescriptor.h"
#include "qcontactmanagerenginefactory.h"

#include "qcontactinvalidbackend_p.h"
#include "qcontactspluginsearch_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/* Shared QContactManager stuff here, default engine stuff below */
QHash<QString, QContactManagerEngineFactory*> QContactManagerData::m_engines;
QSet<QContactManager*> QContactManagerData::m_aliveEngines;
QList<QContactActionManagerPlugin*> QContactManagerData::m_actionManagers;

bool QContactManagerData::m_discoveredStatic;
QList<QJsonObject> QContactManagerData::m_pluginPaths;
QList<QJsonObject> QContactManagerData::m_metaData;
QStringList QContactManagerData::m_managerNames;


#ifndef QT_NO_LIBRARY
Q_GLOBAL_STATIC_WITH_ARGS(QFactoryLoader, loader, (QT_CONTACT_MANAGER_ENGINE_INTERFACE, QLatin1String("/contacts")))
#endif

static void qContactsCleanEngines()
{
    // This is complicated by needing to remove any engines before we unload factories
    // guard pointers as one engine could be parent of another manager and cause doubledelete
    QList<QPointer<QContactManager> > aliveManagers;
    foreach(QContactManager* manager, QContactManagerData::m_aliveEngines) {
        aliveManagers << QPointer<QContactManager>(manager);
    }

    foreach(QPointer<QContactManager> manager, aliveManagers) {
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
                    qWarning("Creation of %s engine failed.", qPrintable(managerName));
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
                if (name != QStringLiteral("invalid") && !name.isEmpty()) {
                    // we also need to ensure that we haven't already loaded this factory.
                    if (m_engines.keys().contains(name)) {
                        qWarning("Static contacts plugin %s has the same name as a currently loaded plugin; ignored", qPrintable(name));
                    } else {
                        m_engines.insertMulti(name, f);
                    }
                } else {
                    qWarning("Static contacts plugin with reserved name %s ignored", qPrintable(name));
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
        QObject::connect(manager, SIGNAL(contactsChanged(QList<QContactId>,QList<QContactDetail::DetailType>)),
                manager, SLOT(_q_contactsUpdated(QList<QContactId>,QList<QContactDetail::DetailType>)));
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
            QObject::disconnect(manager, SIGNAL(contactsChanged(QList<QContactId>,QList<QContactDetail::DetailType>)),
                    manager, SLOT(_q_contactsUpdated(QList<QContactId>,QList<QContactDetail::DetailType>)));
            QObject::disconnect(manager, SIGNAL(contactsRemoved(QList<QContactId>)),
                    manager, SLOT(_q_contactsDeleted(QList<QContactId>)));
        }
    }
}

void QContactManagerData::_q_contactsUpdated(const QList<QContactId> &ids, const QList<QContactDetail::DetailType> &typesChanged)
{
    foreach (const QContactId &id, ids) {
        QList<QContactObserver*> observers = m_observerForContact.values(id);
        foreach (QContactObserver* observer, observers) {
            QMetaObject::invokeMethod(observer, "contactChanged", Q_ARG(QList<QContactDetail::DetailType>, typesChanged));
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
bool QContactManagerData::parseUri(const QString &uriString, QString *managerName, QMap<QString, QString> *params, bool strict)
{
    // Format: qtcontacts:<managerid>:<key>=<value>&<key>=<value>
    // we assume that the prefix, managerid, and params cannot contain `:', `=', or `&'
    // similarly, that neither param keys nor param values can contain these characters.

    const QStringList colonSplit = uriString.split(QLatin1Char(':'), QString::KeepEmptyParts);
    if ((colonSplit.size() != 3) && (strict || colonSplit.size() != 2))
        return false;

    const QString prefix = colonSplit.at(0);
    const QString mgrName = colonSplit.at(1);
    const QString paramString = colonSplit.value(2);

    if (prefix != QStringLiteral("qtcontacts") || mgrName.isEmpty())
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
    passing to QContactId::fromString().
*/
QString QContactManagerData::buildUri(const QString &managerName, const QMap<QString, QString> &params)
{
    // Format: qtcontacts:<managerid>:<key>=<value>&<key>=<value>
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

    return QStringLiteral("qtcontacts:") + escapeParam(managerName) + QLatin1Char(':') + paramString;
}

/*!
    Parses the individual components of the given \a idData and fills the
    \a managerName, \a params, \a managerUri and \a localId.
    Returns true if the parts could be parsed successfully, false otherwise.
*/
bool QContactManagerData::parseIdData(const QByteArray &idData, QString *managerName, QMap<QString, QString> *params, QString *managerUri, QByteArray *localId)
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
    passing to QContactId::fromString().
*/
QByteArray QContactManagerData::buildIdData(const QString &managerUri, const QByteArray &localId)
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
    passing to QContactId::fromString().
*/
QByteArray QContactManagerData::buildIdData(const QString &managerName, const QMap<QString, QString> &params, const QByteArray &localId)
{
    return buildIdData(buildUri(managerName, params), localId);
}

/*!
    Returns a cached instance of the manager URI string that matches \a managerUri.
    This instance should be preferred when constructing ID objects in order to promote
    data sharing of the URI string.
*/
QString QContactManagerData::cachedUri(const QString &managerUri)
{
    static QStringList managerUris;

    int index = managerUris.indexOf(managerUri);
    if (index != -1)
        return managerUris.at(index);

    managerUris.append(managerUri);
    return managerUri;
}

QT_END_NAMESPACE_CONTACTS
