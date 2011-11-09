/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QCONTACTONLINEACCOUNT_H
#define QCONTACTONLINEACCOUNT_H

#include <QString>

#include <qcontactsglobal.h>
#include "qcontactdetail.h"
#include "qcontact.h"

QTCONTACTS_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactOnlineAccount : public QContactDetail
{
public:
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactOnlineAccount, "OnlineAccount")
    const static QString FieldAccountUri;
    const static QString FieldServiceProvider;
    const static QString FieldProtocol;
    const static QString FieldCapabilities;
    const static QString FieldSubTypes;
    const static QString SubTypeSip;
    const static QString SubTypeSipVoip;
    const static QString SubTypeImpp;
    const static QString SubTypeVideoShare;
    const static QString ProtocolAim;
    const static QString ProtocolIcq;
    const static QString ProtocolIrc;
    const static QString ProtocolJabber;
    const static QString ProtocolMsn;
    const static QString ProtocolQq;
    const static QString ProtocolSkype;
    const static QString ProtocolYahoo;

    void setAccountUri(const QString& accountUri) {setValue(FieldAccountUri, accountUri);}
    QString accountUri() const {return value(FieldAccountUri).toString();}

    void setServiceProvider(const QString& serviceProvider) {setValue(FieldServiceProvider, serviceProvider);}
    QString serviceProvider() const {return value(FieldServiceProvider).toString();}

    void setProtocol(const QString& protocol) {setValue(FieldProtocol, protocol);}
    QString protocol() const {return value(FieldProtocol).toString();}

    void setCapabilities(const QStringList& capabilities) {setValue(FieldCapabilities, capabilities);}
    QStringList capabilities() const {return value<QStringList>(FieldCapabilities);}

    void setSubTypes(const QStringList& subTypes) {setValue(FieldSubTypes, subTypes);}
    void setSubTypes(const QString& subType) {setValue(FieldSubTypes, QStringList(subType));}
    QStringList subTypes() const {return value<QStringList>(FieldSubTypes);}
};

QTCONTACTS_END_NAMESPACE

#endif
