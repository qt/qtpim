/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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


#ifndef QDECLARATIVECONTACTSYNCTARGET_H
#define QDECLARATIVECONTACTSYNCTARGET_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontactsynctarget.h"

QTCONTACTS_BEGIN_NAMESPACE

class  QDeclarativeContactSyncTarget : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString syncTarget READ syncTarget WRITE setSyncTarget NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "syncTarget")
public:
    enum FieldType {
        SyncTarget = QContactSyncTarget::FieldSyncTarget
    };

    QDeclarativeContactSyncTarget(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactSyncTarget());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::SyncTarget;
    }

    void setSyncTarget(const QString& v)
    {
        if (!readOnly() && v != syncTarget()) {
            detail().setValue(QContactSyncTarget::FieldSyncTarget, v);
            emit valueChanged();
        }
    }
    QString syncTarget() const {return detail().value(QContactSyncTarget::FieldSyncTarget).toString();}
signals:
    void valueChanged();

};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactSyncTarget))

#endif

