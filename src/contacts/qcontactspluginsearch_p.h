/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTSPLUGINSEARCH_H
#define QCONTACTSPLUGINSEARCH_H

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

#include <QtCore/qcoreapplication.h>
#if !defined QT_NO_DEBUG
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qdir.h>
#include <QtCore/qstringlist.h>

#include <QtContacts/qcontactsglobal.h>

QT_BEGIN_NAMESPACE_CONTACTS

#define CHECKDIR(dir) (dir).exists()

inline QStringList mobilityPlugins(const QString& plugintype)
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

    QStringList paths;
#ifndef QT_NO_LIBRARY
    paths = QCoreApplication::libraryPaths();
#endif
#ifdef QTM_PLUGIN_PATH
    paths << QLatin1String(QTM_PLUGIN_PATH);
#endif
#if !defined QT_NO_DEBUG
    if (showDebug)
        qDebug() << "Plugin paths:" << paths;
#endif

    // Temp variable to avoid multiple identical paths
    // (we don't convert the list to set first, because that loses the order)
    QSet<QString> processed;

    /* The list of discovered plugins */
    QStringList plugins;

    /* Enumerate our plugin paths */
    for (int i=0; i < paths.count(); i++) {
        if (processed.contains(paths.at(i)))
            continue;
        processed.insert(paths.at(i));
        QDir pluginsDir(paths.at(i));
        if (!CHECKDIR(pluginsDir))
            continue;

#if defined(Q_OS_WIN)
        if (pluginsDir.dirName().toLower() == QLatin1String("debug") || pluginsDir.dirName().toLower() == QLatin1String("release"))
            pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
        if (pluginsDir.dirName() == QLatin1String("MacOS")) {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
#endif

        QString subdir(QStringLiteral("plugins/"));
        subdir += plugintype;
        if (pluginsDir.path().endsWith(QStringLiteral("/plugins"))
            || pluginsDir.path().endsWith(QStringLiteral("/plugins/")))
            subdir = plugintype;

        if (CHECKDIR(QDir(pluginsDir.filePath(subdir)))) {
            pluginsDir.cd(subdir);
            QStringList files = pluginsDir.entryList(QDir::Files);

#if !defined QT_NO_DEBUG
            if (showDebug)
                qDebug() << "Looking for " << plugintype << " plugins in" << pluginsDir.path() << files;
#endif

            for (int j=0; j < files.count(); j++) {
                plugins <<  pluginsDir.absoluteFilePath(files.at(j));
            }
        }
    }

    /* Add application path + plugintype */
    QDir appldir(QCoreApplication::applicationDirPath());
    if(appldir.cd(plugintype)){
        if (!processed.contains(appldir.absolutePath())){
            processed.insert(appldir.absolutePath());
            QStringList files = appldir.entryList(QDir::Files);
#if !defined QT_NO_DEBUG
            if (showDebug)
                qDebug() << "Looking for " << plugintype << " plugins in" << appldir.path() << files;
#endif
            for (int j=0; j < files.count(); j++) {
                plugins <<  appldir.absoluteFilePath(files.at(j));
            }
        }
    }

    return  plugins;
}

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTSPLUGINSEARCH_H
