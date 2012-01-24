/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVECONTACTORGANIZATION_H
#define QDECLARATIVECONTACTORGANIZATION_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontactorganization.h"
#include <QSet>

QTCONTACTS_BEGIN_NAMESPACE

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
    Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate NOTIFY valueChanged)
    Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate NOTIFY valueChanged)
    Q_CLASSINFO("DefaultProperty", "name")
    Q_ENUMS(FieldType)
public:
    enum FieldType {
        Name = 0,
        LogoUrl,
        Department,
        Location,
        Role,
        Title,
        AssistantName,
        StartDate,
        EndDate
    };

    QDeclarativeContactOrganization(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactOrganization());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::Organization;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case Name:
            return QContactOrganization::FieldName;
        case LogoUrl:
            return QContactOrganization::FieldLogoUrl;
        case Department:
            return QContactOrganization::FieldDepartment;
        case Location:
            return QContactOrganization::FieldLocation;
        case Role:
            return QContactOrganization::FieldRole;
        case Title:
            return QContactOrganization::FieldTitle;
        case AssistantName:
            return QContactOrganization::FieldAssistantName;
        case StartDate:
            return QContactOrganization::FieldStartDate;
        case EndDate:
            return QContactOrganization::FieldEndDate;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
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
    void setStartDate(const QDateTime& v)
    {
        if (!readOnly() && v != startDate()) {
            detail().setValue(QContactOrganization::FieldStartDate, v);
            emit valueChanged();
        }
    }
    QDateTime startDate() const {return detail().value<QDateTime>(QContactOrganization::FieldStartDate);}
    void setEndDate(const QDateTime& v)
    {
        if (!readOnly() && v != endDate()) {
            detail().setValue(QContactOrganization::FieldEndDate, v);
            emit valueChanged();
        }
    }
    QDateTime endDate() const {return detail().value<QDateTime>(QContactOrganization::FieldEndDate);}
signals:
    void valueChanged();

};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactOrganization))

#endif
