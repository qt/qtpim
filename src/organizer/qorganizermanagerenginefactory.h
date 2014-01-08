/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QORGANIZERMANAGERENGINEFACTORY_H
#define QORGANIZERMANAGERENGINEFACTORY_H

#include <QtCore/qfactoryinterface.h>
#include <QtCore/qplugin.h>

#include <QtOrganizer/qorganizermanager.h>

QT_BEGIN_NAMESPACE_ORGANIZER
struct Q_ORGANIZER_EXPORT QOrganizerManagerEngineFactoryInterface : public QFactoryInterface
{
    virtual QOrganizerManagerEngine *engine(const QMap<QString, QString> &parameters, QOrganizerManager::Error *error) = 0;
    virtual QString managerName() const = 0;
    virtual QOrganizerItemEngineId *createItemEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const = 0;
    virtual QOrganizerCollectionEngineId *createCollectionEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const = 0;
};
QT_END_NAMESPACE_ORGANIZER

QT_BEGIN_NAMESPACE
#define QT_ORGANIZER_MANAGER_ENGINE_INTERFACE "org.qt-project.Qt.QOrganizerManagerEngineFactoryInterface"
Q_DECLARE_INTERFACE(QtOrganizer::QOrganizerManagerEngineFactoryInterface, QT_ORGANIZER_MANAGER_ENGINE_INTERFACE)
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE_ORGANIZER
class Q_ORGANIZER_EXPORT QOrganizerManagerEngineFactory : public QObject, public QOrganizerManagerEngineFactoryInterface
{
    Q_OBJECT
    Q_INTERFACES(QtOrganizer::QOrganizerManagerEngineFactoryInterface:QFactoryInterface)

public:
    virtual ~QOrganizerManagerEngineFactory();
    virtual QOrganizerManagerEngine *engine(const QMap<QString, QString> &parameters, QOrganizerManager::Error *error) = 0;
    virtual QString managerName() const = 0;
    virtual QOrganizerItemEngineId *createItemEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const = 0;
    virtual QOrganizerCollectionEngineId *createCollectionEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const = 0;

    QStringList keys() const;
};
QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERMANAGERENGINEFACTORY_H
