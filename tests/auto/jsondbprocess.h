/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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

#ifndef JSONDBPROCESS_H
#define JSONDBPROCESS_H

#include <QtTest/QtTest>
#include <QProcess>
#include <QTemporaryDir>
#include <QCoreApplication>
#include <QLibraryInfo>

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

class JsonDbProcess
{
public:
    JsonDbProcess()
        : jsondbWorkingDirectory( QCoreApplication::applicationDirPath() + "/jsondbXXXXXX" )
    {
    }

    bool start(bool forceNewProcess = false) {

        // Start new process only if there is no existing JsonDb process
        if (system("pidof jsondb") > 0 || forceNewProcess) {

            if (!jsondbWorkingDirectory.isValid()) {
                qWarning() << Q_FUNC_INFO << "JsonDb working directory is not valid.";
                return false;
            }

            QString jsondbPath = QLibraryInfo::location(QLibraryInfo::BinariesPath) + "/jsondb";
            if (!QFileInfo(jsondbPath).exists()) {
                qWarning() << Q_FUNC_INFO << "Cannot find JsonDb binaries.";
                return false;
            }

            m_process.start(jsondbPath, QStringList() << (jsondbWorkingDirectory.path() + "/"));
            if (!m_process.waitForStarted()) {
                qWarning() << Q_FUNC_INFO << m_process.errorString();
                return false;
            }

        }

        return true;
    }

    void terminate() {
        if (m_process.state() == QProcess::NotRunning)
            return;

        m_process.terminate();
        if (m_process.state() != QProcess::NotRunning && !m_process.waitForFinished()) {
            qWarning() << Q_FUNC_INFO << "JsonDb did not terminate cleanly.  Killing.";
            m_process.kill();
        }
    }

private:
    QProcess m_process;
    QTemporaryDir jsondbWorkingDirectory;

};

#endif // JSONDBPROCESS_H
