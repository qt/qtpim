/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
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

#ifndef QVERSITORGANIZERHANDLER_H
#define QVERSITORGANIZERHANDLER_H

#include <QtVersitOrganizer/qversitorganizerimporter.h>
#include <QtVersitOrganizer/qversitorganizerexporter.h>

QT_BEGIN_NAMESPACE_VERSITORGANIZER

// qdoc seems to not find QVersitOrganizerHandler if it is declared first, hence this forward
// declaration
class QVersitOrganizerHandler;

class Q_VERSIT_ORGANIZER_EXPORT QVersitOrganizerHandlerFactory
{
public:
    virtual ~QVersitOrganizerHandlerFactory() {}
    virtual QSet<QString> profiles() const { return QSet<QString>(); }
    virtual QString name() const = 0;
    virtual int index() const { return 0; }
    virtual QVersitOrganizerHandler* createHandler() const = 0;

    inline static const QString ProfileSync() {return QStringLiteral("Sync");};
    inline static const QString ProfileBackup() {return QStringLiteral("Backup");};
};

class Q_VERSIT_ORGANIZER_EXPORT QVersitOrganizerHandler : public QVersitOrganizerImporterPropertyHandler,
                                                public QVersitOrganizerExporterDetailHandler
{
public:
    virtual ~QVersitOrganizerHandler() {}
};

QT_END_NAMESPACE_VERSITORGANIZER

#define QT_VERSIT_ORGANIZER_HANDLER_INTERFACE "org.qt-project.Qt.QVersitOrganizerHandlerFactory"
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QtVersitOrganizer::QVersitOrganizerHandlerFactory, QT_VERSIT_ORGANIZER_HANDLER_INTERFACE)
QT_END_NAMESPACE

#endif // QVERSITORGANIZERHANDLER_H
