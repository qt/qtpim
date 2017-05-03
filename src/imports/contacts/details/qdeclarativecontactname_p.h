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

#ifndef QDECLARATIVECONTACTNAME_H
#define QDECLARATIVECONTACTNAME_H

#include <QtContacts/qcontactname.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactName : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY valueChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY valueChanged)
    Q_PROPERTY(QString middleName READ middleName WRITE setMiddleName NOTIFY valueChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY valueChanged)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY valueChanged)

    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "firstName")
public:
    enum FieldType {
        Prefix = QContactName::FieldPrefix,
        FirstName = QContactName::FieldFirstName,
        MiddleName = QContactName::FieldMiddleName,
        LastName = QContactName::FieldLastName,
        Suffix = QContactName::FieldSuffix,
    };

    QDeclarativeContactName(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactName());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Name;
    }

    QString prefix() const {return detail().value(QContactName::FieldPrefix).toString();}
    QString firstName() const {return detail().value(QContactName::FieldFirstName).toString();}
    QString middleName() const {return detail().value(QContactName::FieldMiddleName).toString();}
    QString lastName() const {return detail().value(QContactName::FieldLastName).toString();}
    QString suffix() const {return detail().value(QContactName::FieldSuffix).toString();}

    void setPrefix(const QString& v)
    {
        if (!readOnly() && v != prefix()) {
            detail().setValue(QContactName::FieldPrefix, v);
            emit valueChanged();
        }
    }

    void setFirstName(const QString& v)
    {
        if (!readOnly() && v != firstName()) {
            detail().setValue(QContactName::FieldFirstName, v);
            emit valueChanged();
        }
    }
    void setMiddleName(const QString& v)
    {
        if (!readOnly() && v != middleName()) {
            detail().setValue(QContactName::FieldMiddleName, v);
            emit valueChanged();
        }
    }
    void setLastName(const QString& v)
    {
        if (!readOnly() && v != lastName()) {
            detail().setValue(QContactName::FieldLastName, v);
            emit valueChanged();
        }
    }
    void setSuffix(const QString& v)
    {
        if (!readOnly() && v != suffix()) {
            detail().setValue(QContactName::FieldSuffix, v);
            emit valueChanged();
        }
    }

signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactName)

#endif // QDECLARATIVECONTACTNAME_H
