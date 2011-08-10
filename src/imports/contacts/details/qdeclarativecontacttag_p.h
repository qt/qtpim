
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

#ifndef QDECLARATIVECONTACTTAG_H
#define QDECLARATIVECONTACTTAG_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontacttag.h"

QTPIM_BEGIN_NAMESPACE

class QDeclarativeContactTag : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ tag WRITE setTag NOTIFY fieldsChanged)
    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "tag")
public:
    enum FieldType {
        Tag = 0
    };

    QDeclarativeContactTag(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactTag());
        connect(this, SIGNAL(fieldsChanged()), SIGNAL(valueChanged()));
    }
    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::Tag;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case Tag:
            return QContactTag::FieldTag;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
    }
    void setTag(const QString& v)
    {
        if (!readOnly() && v != tag()) {
            detail().setValue(QContactTag::FieldTag, v);
            emit fieldsChanged();
        }
    }
    QString tag() const {return detail().value(QContactTag::FieldTag);}
signals:
    void fieldsChanged();
};

QTPIM_END_NAMESPACE

QML_DECLARE_TYPE(QTPIM_PREPEND_NAMESPACE(QDeclarativeContactTag))

#endif