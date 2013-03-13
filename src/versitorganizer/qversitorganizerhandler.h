/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
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

#ifndef QVERSITORGANIZERHANDLER_H
#define QVERSITORGANIZERHANDLER_H

#include <qversitorganizerimporter.h>
#include <qversitorganizerexporter.h>

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
#endif
