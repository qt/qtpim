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


#ifndef QDECLARATIVECONTACTNAME_H
#define QDECLARATIVECONTACTNAME_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontactname.h"

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactName : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY valueChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY valueChanged)
    Q_PROPERTY(QString middleName READ middleName WRITE setMiddleName NOTIFY valueChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY valueChanged)
    Q_PROPERTY(QString suffix READ suffix WRITE setSuffix NOTIFY valueChanged)
    Q_PROPERTY(QString customLabel READ customLabel WRITE setCustomLabel NOTIFY valueChanged)

    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "firstName")
public:
    enum FieldType {
        Prefix = 0,
        FirstName,
        MiddleName,
        LastName,
        Suffix,
        CustomLabel
    };

    QDeclarativeContactName(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactName());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::Name;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case Prefix:
            return QContactName::FieldPrefix;
        case FirstName:
            return QContactName::FieldFirstName;
        case MiddleName:
            return QContactName::FieldMiddleName;
        case LastName:
            return QContactName::FieldLastName;
        case Suffix:
            return QContactName::FieldSuffix;
        case CustomLabel:
            return QContactName::FieldCustomLabel;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
    }
    QString prefix() const {return detail().value(QContactName::FieldPrefix).toString();}
    QString firstName() const {return detail().value(QContactName::FieldFirstName).toString();}
    QString middleName() const {return detail().value(QContactName::FieldMiddleName).toString();}
    QString lastName() const {return detail().value(QContactName::FieldLastName).toString();}
    QString suffix() const {return detail().value(QContactName::FieldSuffix).toString();}
    QString customLabel() const{return detail().value(QContactName::FieldCustomLabel).toString();}

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
    void setCustomLabel(const QString& v)
    {
        if (!readOnly() && v != customLabel()) {
            detail().setValue(QContactName::FieldCustomLabel, v);
            emit valueChanged();
        }
    }

signals:
    void valueChanged();
};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactName))

#endif
