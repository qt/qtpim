/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTENGINEID_H
#define QCONTACTENGINEID_H

#include <QString>
#include <QSharedDataPointer>

#include "qcontactsglobal.h"

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

QTCONTACTS_BEGIN_NAMESPACE

class Q_CONTACTS_EXPORT QContactEngineId: public QSharedData
{
public:
    virtual ~QContactEngineId() {}

    virtual bool isEqualTo(const QContactEngineId *other) const = 0;
    virtual bool isLessThan(const QContactEngineId *other) const = 0;

    virtual QString managerUri() const = 0;

    virtual QContactEngineId* clone() const = 0;

    virtual QString toString() const = 0;

#ifndef QT_NO_DEBUG_STREAM
    // NOTE: on platforms where Qt is built without debug streams enabled, vtable will differ!
    virtual QDebug& debugStreamOut(QDebug &dbg) const = 0;
#endif
    virtual uint hash() const = 0;
};

QTCONTACTS_END_NAMESPACE

#endif

