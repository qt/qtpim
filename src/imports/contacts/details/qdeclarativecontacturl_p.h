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

#ifndef QDECLARATIVECONTACTURL_H
#define QDECLARATIVECONTACTURL_H
#include <QUrl>

#include "qdeclarativecontactdetail_p.h"
#include "qcontacturl.h"

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactUrl : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY valueChanged)
    Q_PROPERTY(QVariant subType READ subType WRITE setSubType NOTIFY valueChanged)

    Q_ENUMS(FieldType)
    Q_ENUMS(UrlSubType)
public:
    enum FieldType {
        Url = 0,
        SubType
    };

    enum UrlSubType {
        Unknown = 0,
        HomePage,
        Blog,
        Favourite
    };

    QDeclarativeContactUrl(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactUrl());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::Url;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case Url:
            return QContactUrl::FieldUrl;
        case SubType:
            return QContactUrl::FieldSubType;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
    }
    void setUrl(const QString& v)
    {
        if (!readOnly() && v != url()) {
            detail().setValue(QContactUrl::FieldUrl, v);
            emit valueChanged();
        }
    }
    QString url() const {return detail().value(QContactUrl::FieldUrl).toString();}

    void setSubType(const QVariant& v)
    {
        if (!readOnly()) {
            QString st;
            if (v.type() != QVariant::String) {
                switch (v.toInt()) {
                case HomePage:
                    st = QContactUrl::SubTypeHomePage;
                    break;
                case Blog:
                    st = QContactUrl::SubTypeBlog;
                    break;
                case Favourite:
                    st = QContactUrl::SubTypeFavourite;
                    break;
                default:break;
                }
            } else {
                st = v.toString();
            }

            if (st != detail().value(QContactUrl::FieldSubType)) {
                detail().setValue(QContactUrl::FieldSubType, st);
                emit valueChanged();
            }
        }
    }
    QVariant subType() const
    {
        return detail().value(QContactUrl::FieldSubType);
    }
signals:
    void valueChanged();
};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactUrl))

#endif

