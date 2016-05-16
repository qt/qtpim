/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVECONTACTURL_H
#define QDECLARATIVECONTACTURL_H

#include <QtContacts/qcontacturl.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactUrl : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY valueChanged)
    Q_PROPERTY(UrlSubType subType READ subType WRITE setSubType NOTIFY valueChanged)

    Q_ENUMS(FieldType)
    Q_ENUMS(UrlSubType)
public:
    enum FieldType {
        Url = QContactUrl::FieldUrl,
        SubType = QContactUrl::FieldSubType
    };

    enum UrlSubType {
        Unknown = 0,
        HomePage = QContactUrl::SubTypeHomePage,
        Blog = QContactUrl::SubTypeBlog,
        Favourite = QContactUrl::SubTypeFavourite
    };

    QDeclarativeContactUrl(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactUrl());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Url;
    }

    void setUrl(const QString& v)
    {
        if (!readOnly() && v != url()) {
            detail().setValue(QContactUrl::FieldUrl, v);
            emit valueChanged();
        }
    }
    QString url() const {return detail().value(QContactUrl::FieldUrl).toString();}

    void setSubType(const UrlSubType& v)
    {
        switch (v) {
        case HomePage:
        case Blog:
        case Favourite:
            if (v != detail().value(QContactUrl::FieldSubType)) {
                detail().setValue(QContactUrl::FieldSubType, v);
                emit valueChanged();
            }
            break;
        default:
            if (detail().value(QContactUrl::FieldSubType) != Unknown) {
                detail().setValue(QContactUrl::FieldSubType, Unknown);
                emit valueChanged();
            }
        }
    }

    UrlSubType subType() const
    {
        return static_cast<UrlSubType>(detail().value(QContactUrl::FieldSubType).toInt());
    }
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactUrl)

#endif // QDECLARATIVECONTACTURL_H
