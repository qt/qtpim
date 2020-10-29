/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#ifndef QVERSITCONTACTHANDLER_H
#define QVERSITCONTACTHANDLER_H

#include <QtCore/qfactoryinterface.h>
#include <QtCore/qset.h>

#include <QtVersit/qversitcontactimporter.h>
#include <QtVersit/qversitcontactexporter.h>

QT_BEGIN_NAMESPACE_VERSIT

// qdoc seems to not find QVersitContactHandler if it is declared first.. ugh
class QVersitContactHandler;

struct Q_VERSIT_EXPORT QVersitContactHandlerFactoryInterface : public QFactoryInterface
{
    virtual QSet<QString> profiles() const =0;
    virtual QString name() const = 0;
    virtual int index() const = 0;
    virtual QVersitContactHandler* createHandler() const = 0;
};
QT_END_NAMESPACE_VERSIT

QT_BEGIN_NAMESPACE
#define QT_VERSIT_CONTACT_HANDLER_INTERFACE "org.qt-project.Qt.QVersitContactHandlerFactoryInterface"
Q_DECLARE_INTERFACE(QtVersit::QVersitContactHandlerFactoryInterface, QT_VERSIT_CONTACT_HANDLER_INTERFACE)
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

class Q_VERSIT_EXPORT QVersitContactHandlerFactory : public QObject, public QVersitContactHandlerFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QtVersit::QVersitContactHandlerFactoryInterface:QFactoryInterface)
public:
    virtual ~QVersitContactHandlerFactory() {}
    virtual QSet<QString> profiles() const override { return QSet<QString>(); }
    virtual QString name() const override = 0;
    virtual int index() const override { return 0; }
    virtual QVersitContactHandler* createHandler() const override = 0;
    virtual QStringList keys() const override;

    inline static const QString ProfileSync() {return QStringLiteral("Sync");};
    inline static const QString ProfileBackup() {return QStringLiteral("Backup");};
};

class Q_VERSIT_EXPORT QVersitContactHandler : public QVersitContactImporterPropertyHandlerV2,
                                              public QVersitContactExporterDetailHandlerV2
{
public:
    virtual ~QVersitContactHandler() {}
};
QT_END_NAMESPACE_VERSIT

#endif // QVERSITCONTACTHANDLER_H
