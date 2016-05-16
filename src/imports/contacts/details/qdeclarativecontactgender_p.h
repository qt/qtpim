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

#ifndef QDECLARATIVECONTACTGENDER_H
#define QDECLARATIVECONTACTGENDER_H

#include <QtContacts/qcontactgender.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactGender : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(GenderType gender READ gender WRITE setGender NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(GenderType)
    Q_CLASSINFO("DefaultProperty", "gender")
public:
    enum FieldType {
        Gender = QContactGender::FieldGender
    };

    enum GenderType {
        Male = QContactGender::GenderMale,
        Female = QContactGender::GenderFemale,
        Unspecified = QContactGender::GenderUnspecified
    };

    QDeclarativeContactGender(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactGender());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Gender;
    }

    void setGender(const GenderType v)
    {
        if (!readOnly() && v != gender()) {
            switch (v) {
            case Male:
            case Female:
                detail().setValue(QContactGender::FieldGender, v);
                break;
            default:
                detail().setValue(QContactGender::FieldGender, Unspecified);
            }
            emit valueChanged();
            }
        }

    GenderType gender() const
    {
        if (detail().value(QContactGender::FieldGender) == QContactGender::GenderMale) {
            return Male;
        }
        return Female;
    }
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactGender)

#endif // QDECLARATIVECONTACTGENDER_H
