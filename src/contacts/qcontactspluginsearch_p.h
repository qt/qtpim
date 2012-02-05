/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTSPLUGINSEARCH_H
#define QCONTACTSPLUGINSEARCH_H

#include <qcontactsglobal.h>

#include <QCoreApplication>
#include <QStringList>
#include <QDir>
#include <QDebug>

QTCONTACTS_BEGIN_NAMESPACE

#define CHECKDIR(dir) (dir).exists()

inline QStringList mobilityPlugins(const QString& plugintype)
{
#if !defined QT_NO_DEBUG
    const bool showDebug = qgetenv("QT_DEBUG_PLUGINS").toInt() > 0;
#endif

    QStringList paths = QCoreApplication::libraryPaths();
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

        QString subdir(QLatin1String("plugins/"));
        subdir += plugintype;
        if (pluginsDir.path().endsWith(QLatin1String("/plugins"))
            || pluginsDir.path().endsWith(QLatin1String("/plugins/")))
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

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTSPLUGINSEARCH_H
