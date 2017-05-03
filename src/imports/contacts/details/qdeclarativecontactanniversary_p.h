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

#ifndef QDECLARATIVECONTACTANNIVERSARY_H
#define QDECLARATIVECONTACTANNIVERSARY_H

#include <QtContacts/qcontactanniversary.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactAnniversary : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QString calendarId READ calendarId WRITE setCalendarId NOTIFY valueChanged)
    Q_PROPERTY(QDate originalDate READ originalDate WRITE setOriginalDate NOTIFY valueChanged)
    Q_PROPERTY(QDateTime originalDateTime READ originalDateTime WRITE setOriginalDateTime NOTIFY valueChanged)
    Q_PROPERTY(QString event READ event WRITE setEvent NOTIFY valueChanged)
    Q_PROPERTY(AnniversarySubType subType READ subType WRITE setSubType NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(AnniversarySubType)
public:
    enum FieldType {
        CalendarId = QContactAnniversary::FieldCalendarId,
        OriginalDate = QContactAnniversary::FieldOriginalDate,
        Event = QContactAnniversary::FieldEvent,
        SubType = QContactAnniversary::FieldSubType
    };

    enum AnniversarySubType {
        Wedding = QContactAnniversary::SubTypeWedding,
        Engagement = QContactAnniversary::SubTypeEngagement,
        House = QContactAnniversary::SubTypeHouse,
        Employment = QContactAnniversary::SubTypeEmployment,
        Memorial = QContactAnniversary::SubTypeMemorial
    };

    QDeclarativeContactAnniversary(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactAnniversary());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }

    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Anniversary;
    }

    void setOriginalDate(const QDate& v)
    {
        if (!readOnly() && v != originalDate()) {
            detail().setValue(QContactAnniversary::FieldOriginalDate, v);
            emit valueChanged();
        }
    }
    QDate originalDate() const {return detail().value<QDate>(QContactAnniversary::FieldOriginalDate);}
    void setOriginalDateTime(const QDateTime& v)
    {
        if (!readOnly() && v != originalDateTime()) {
            detail().setValue(QContactAnniversary::FieldOriginalDate, v);
            emit valueChanged();
        }
    }
    QDateTime originalDateTime() const {return detail().value<QDateTime>(QContactAnniversary::FieldOriginalDate);}

    void setCalendarId(const QString& v)
    {
        if (!readOnly() && v != calendarId()) {
            detail().setValue(QContactAnniversary::FieldCalendarId, v);
            emit valueChanged();
        }
    }
    QString calendarId() const {return detail().value(QContactAnniversary::FieldCalendarId).toString();}
    void setEvent(const QString& v)
    {
        if (!readOnly() && v != event()) {
            detail().setValue(QContactAnniversary::FieldEvent, v);
            emit valueChanged();
        }
    }
    QString event() const {return detail().value(QContactAnniversary::FieldEvent).toString();}

    void setSubType(AnniversarySubType v)
    {
        if (!readOnly() && v != subType()) {
            detail().setValue(QContactAnniversary::FieldSubType, v);
        }
    }

    AnniversarySubType subType() const
    {
        return static_cast<AnniversarySubType>(detail().value<int>(QContactAnniversary::FieldSubType));
    }
signals:
    void valueChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactAnniversary)

#endif // QDECLARATIVECONTACTANNIVERSARY_H
