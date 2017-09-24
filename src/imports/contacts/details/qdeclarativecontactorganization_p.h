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

#ifndef QDECLARATIVECONTACTORGANIZATION_H
#define QDECLARATIVECONTACTORGANIZATION_H

#include <QtContacts/qcontactorganization.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactOrganization : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
    Q_PROPERTY(QUrl logoUrl READ logoUrl WRITE setLogoUrl NOTIFY valueChanged)
    Q_PROPERTY(QStringList department READ department WRITE setDepartment NOTIFY valueChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY valueChanged)
    Q_PROPERTY(QString role READ role WRITE setRole NOTIFY valueChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY valueChanged)
    Q_PROPERTY(QString assistantName READ assistantName WRITE setAssistantName NOTIFY valueChanged)
    Q_CLASSINFO("DefaultProperty", "name")
    Q_ENUMS(FieldType)
public:
    enum FieldType {
        Name = QContactOrganization::FieldName,
        LogoUrl = QContactOrganization::FieldLogoUrl,
        Department = QContactOrganization::FieldDepartment,
        Location = QContactOrganization::FieldLocation,
        Role = QContactOrganization::FieldRole,
        Title = QContactOrganization::FieldTitle,
        AssistantName = QContactOrganization::FieldAssistantName
    };

    QDeclarativeContactOrganization(QObject* parent = nullptr)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactOrganization());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Organization;
    }

    void setName(const QString& v)
    {
        if (!readOnly() && v != name()) {
            detail().setValue(QContactOrganization::FieldName, v);
            emit valueChanged();
        }
    }
    QString name() const {return detail().value(QContactOrganization::FieldName).toString();}
    void setLogoUrl(const QUrl& v)
    {
        if (!readOnly() && v != logoUrl()) {
            detail().setValue(QContactOrganization::FieldLogoUrl, v);
            emit valueChanged();
        }
    }
    QUrl logoUrl() const {return detail().value(QContactOrganization::FieldLogoUrl).toString();}
    void setDepartment(const QStringList& v)
    {
        if (!readOnly() && v.toSet() != department().toSet()) {
            detail().setValue(QContactOrganization::FieldDepartment, v);
            emit valueChanged();
        }
    }
    QStringList department() const {return detail().value<QStringList>(QContactOrganization::FieldDepartment);}
    void setLocation(const QString& v)
    {
        if (!readOnly() && v != location()) {
            detail().setValue(QContactOrganization::FieldLocation, v);
            emit valueChanged();
        }
    }
    QString location() const {return detail().value(QContactOrganization::FieldLocation).toString();}
    void setRole(const QString& v)
    {
        if (!readOnly() && v != role()) {
            detail().setValue(QContactOrganization::FieldRole, v);
            emit valueChanged();
        }
    }
    QString role() const {return detail().value(QContactOrganization::FieldRole).toString();}
    void setTitle(const QString& v)
    {
        if (!readOnly() && v != title()) {
            detail().setValue(QContactOrganization::FieldTitle, v);
            emit valueChanged();
        }
    }
    QString title() const {return detail().value(QContactOrganization::FieldTitle).toString();}
    void setAssistantName(const QString& v)
    {
        if (!readOnly() && v != assistantName()) {
            detail().setValue(QContactOrganization::FieldAssistantName, v);
            emit valueChanged();
        }
    }
    QString assistantName() const {return detail().value(QContactOrganization::FieldAssistantName).toString();}

signals:
    void valueChanged();

};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactOrganization)

#endif // QDECLARATIVECONTACTORGANIZATION_H
