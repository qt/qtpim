/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#include "qversitcontactpluginloader_p.h"

#include <QtCore/qpluginloader.h>

#include "qversitpluginsearch_p.h"

QT_BEGIN_NAMESPACE_VERSIT

/*!
   A less-than function for factory indices (see QVersitContactHandlerFactory::index()).
   Positive values come first (ascendingly), then zero, then negative values (ascendingly).
 */
bool factoryLessThan(QVersitContactHandlerFactory* a, QVersitContactHandlerFactory* b) {
    if ((a->index() > 0 && b->index() > 0)
            || (a->index() < 0 && b->index() < 0))
        // same sign
        return a->index() < b->index();
    else
        // a is zero
        // or b is zero
        // or opposite sign
        return b->index() < a->index();
}

QVersitContactPluginLoader* QVersitContactPluginLoader::mInstance = NULL;

/*!
 * \class QVersitContactPluginLoader
 * This is a singleton class that loads Versit plugins for contacts processing
  \inmodule QtVersit
 */

QVersitContactPluginLoader::QVersitContactPluginLoader()
{
}

/*!
 * Returns the singleton instance of the QVersitContactPluginLoader.
 */
QVersitContactPluginLoader* QVersitContactPluginLoader::instance()
{
    if (!mInstance)
        mInstance = new QVersitContactPluginLoader;
    return mInstance;
}

void QVersitContactPluginLoader::loadPlugins() {
    QStringList plugins = mobilityPlugins(QStringLiteral("versit"));
    if (plugins != mPluginPaths) {
        mPluginPaths = plugins;

        foreach (const QString& pluginPath, mPluginPaths) {
            QPluginLoader qpl(pluginPath);
            QObject* plugin = qpl.instance();
            QVersitContactHandlerFactory* contactPlugin =
                qobject_cast<QVersitContactHandlerFactory*>(plugin);
            if (contactPlugin && !mLoadedFactories.contains(contactPlugin->name())) {
                mLoadedFactories.insert(contactPlugin->name());
                mContactHandlerFactories.append(contactPlugin);
            }
        }
        std::sort(mContactHandlerFactories.begin(), mContactHandlerFactories.end(), factoryLessThan);
    }
}

/*!
 * Creates and returns handlers from the plugin.  If \a profiles is the empty string, only handlers
 * with an empty profile list are returned.  If \a profiles is nonempty, only handlers with either
 * an empty profile list or a profile list that contains the given \a profiles are returned.
 *
 * The caller is responsible for deleting all returned handlers.
 */
QList<QVersitContactHandler*> QVersitContactPluginLoader::createContactHandlers(const QStringList& profiles)
{
    loadPlugins();

    QList<QVersitContactHandler*> handlers;
    foreach (const QVersitContactHandlerFactory* factory, mContactHandlerFactories) {
        // if the plugin specifies no profiles, include it
        QSet<QString> factoryProfiles(factory->profiles());
        bool includePlugin = factory->profiles().isEmpty();
        if (!includePlugin) {
            // if the plugin's profile list intersects with the requested profile list, include it.
            foreach (const QString& profile, profiles) {
                if (factoryProfiles.contains(profile)) {
                    includePlugin = true;
                    break;
                }
            }
        }

        if (includePlugin) {
            QVersitContactHandler* handler = factory->createHandler();
            handlers.append(handler);
        }
    }
    return handlers;
}

QT_END_NAMESPACE_VERSIT
