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

    QDeclarativeContactGender(QObject* parent = nullptr)
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
