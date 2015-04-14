/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTMULTIACTION_P_H
#define QCONTACTMULTIACTION_P_H

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

#include <QtContacts/qcontactaction.h>
#include <QtContacts/qcontactactionfactory.h>

#include "qserviceinterfacedescriptor.h"
#include "qserviceplugininterface.h"
#include "qservicecontext.h"
#include "qabstractsecuritysession.h"

#include <QSharedData>
#include <QString>
#include <QVariantMap>

QTM_USE_NAMESPACE

//! [Example Contact Action Plugin Declaration]

/*
   This action plugin is capable of producing two actions which each have the
   same action name, service name, interface name and implementation (minor) version,
   but internally use a different implementation.  This difference is reported via the
   meta data function of the factory (which is exposed to clients via the descriptor
   which provides a "front end" to the factory).

   Example use case:
   Company "Example VoIP Solutions" wants to provide a "Call" action with different implementations.
       -> it provides a SINGLE plugin which provides two actions, both of which are:
           - ServiceName = "Example VoIP Solution"
           - InterfaceName = "org.qt-project.Qt.SampleContactsActionPlugin" (QContactActionFactory::InterfaceName)
           - Major Version = "1"
           - Minor Version = "1"
           - ActionName = "call" (this is a custom property in the service interface xml)
       -> BUT one of the actions has the custom property:
           - Provider = "sip"
       -> where the other action has the custom property:
           - Provider = "example proprietary protocol"
       -> the custom properties are available to clients via the QContactActionDescriptor::metaData() function.
 */

class QContactMultiActionPlugin : public QObject, public QServicePluginInterface
{
    Q_OBJECT
    Q_INTERFACES(QtMobility::QServicePluginInterface)

public:
    QObject* createInstance(const QServiceInterfaceDescriptor& descriptor,
                            QServiceContext* context,
                            QAbstractSecuritySession* session);
};

class QContactMultiActionFactory : public QContactActionFactory
{
    Q_OBJECT

public:
    QContactMultiActionFactory();
    ~QContactMultiActionFactory();

    QList<QContactActionDescriptor> actionDescriptors() const;
    QContactAction* create(const QContactActionDescriptor& which) const;

    QSet<QContactActionTarget> supportedTargets(const QContact& contact, const QContactActionDescriptor& which) const;
    QContactFilter contactFilter(const QContactActionDescriptor& which) const;
    QVariant metaData(const QString& key, const QList<QContactActionTarget>& targets, const QVariantMap& parameters, const QContactActionDescriptor& which) const;

    bool supportsContact(const QContact& contact, const QContactActionDescriptor& which) const;

private:
    QContactActionDescriptor m_actionOneDescriptor;
    QContactActionDescriptor m_actionTwoDescriptor;
};

class QContactActionOne : public QContactAction
{
    Q_OBJECT

public:
    QContactActionOne();
    ~QContactActionOne();

    bool invokeAction(const QContactActionTarget& target, const QVariantMap& params = QVariantMap());
    bool invokeAction(const QList<QContactActionTarget>& targets, const QVariantMap& params = QVariantMap());
    QVariantMap results() const;
    State state() const;

private slots:
    void performAction();
};

class QContactActionTwo : public QContactAction
{
    Q_OBJECT

public:
    QContactActionTwo();
    ~QContactActionTwo();

    bool invokeAction(const QContactActionTarget& target, const QVariantMap& params = QVariantMap());
    bool invokeAction(const QList<QContactActionTarget>& targets, const QVariantMap& params = QVariantMap());
    QVariantMap results() const;
    State state() const;

private slots:
    void performAction();
};

//! [Example Contact Action Plugin Declaration]

#endif
