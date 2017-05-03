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

#ifndef QCONTACTMANAGERENGINEFACTORY_H
#define QCONTACTMANAGERENGINEFACTORY_H

#include <QtCore/qfactoryinterface.h>
#include <QtCore/qplugin.h>

#include <QtContacts/qcontactmanager.h>

QT_BEGIN_NAMESPACE_CONTACTS

struct Q_CONTACTS_EXPORT QContactManagerEngineFactoryInterface : public QFactoryInterface
{
    virtual QContactManagerEngine *engine(const QMap<QString, QString> &parameters, QContactManager::Error *error) = 0;
    virtual QString managerName() const = 0;
};
QT_END_NAMESPACE_CONTACTS

QT_BEGIN_NAMESPACE
#define QT_CONTACT_MANAGER_ENGINE_INTERFACE "org.qt-project.Qt.QContactManagerEngineFactoryInterface"
Q_DECLARE_INTERFACE(QtContacts::QContactManagerEngineFactoryInterface, QT_CONTACT_MANAGER_ENGINE_INTERFACE)
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE_CONTACTS
class Q_CONTACTS_EXPORT QContactManagerEngineFactory : public QObject, public QContactManagerEngineFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QtContacts::QContactManagerEngineFactoryInterface:QFactoryInterface)
public:
    // engine factory functions
    virtual QList<int> supportedImplementationVersions() const;
    virtual ~QContactManagerEngineFactory();
    virtual QContactManagerEngine* engine(const QMap<QString, QString> &parameters, QContactManager::Error *error) = 0;
    virtual QString managerName() const = 0;
    virtual QStringList keys() const;
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTMANAGERENGINEFACTORY_H
