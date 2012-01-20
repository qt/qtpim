/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTANNIVERSARY_H
#define QCONTACTANNIVERSARY_H

#include <QString>

#include <qcontactsglobal.h>
#include <qcontactdetail.h>
#include <qcontact.h>

QTCONTACTS_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactAnniversary : public QContactDetail
{
public:
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactAnniversary, "Anniversary")
    const static QString FieldCalendarId;
    const static QString FieldOriginalDate;
    const static QString FieldEvent;
    const static QString FieldSubType;
    const static QString SubTypeWedding;
    const static QString SubTypeEngagement;
    const static QString SubTypeHouse;
    const static QString SubTypeEmployment;
    const static QString SubTypeMemorial;

    void setOriginalDate(const QDate& date) {setValue(FieldOriginalDate, date);}
    QDate originalDate() const {return value<QDate>(FieldOriginalDate);}
    void setOriginalDateTime(const QDateTime& dateTime) {setValue(FieldOriginalDate, dateTime);}
    QDateTime originalDateTime() const {return value<QDateTime>(FieldOriginalDate);}

    void setCalendarId(const QString& calendarId) {setValue(FieldCalendarId, calendarId);}
    QString calendarId() const {return value(FieldCalendarId).toString();}
    void setEvent(const QString& event) {setValue(FieldEvent, event);}
    QString event() const {return value(FieldEvent).toString();}

    void setSubType(const QString& subType) {setValue(FieldSubType, subType);}
    QString subType() const {return value(FieldSubType).toString();}
};

QTCONTACTS_END_NAMESPACE

#endif

