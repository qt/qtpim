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

#ifndef QCONTACTORGANIZATION_H
#define QCONTACTORGANIZATION_H

#include <QString>
#include <QUrl>

#include <qcontactsglobal.h>
#include "qcontactdetail.h"
#include "qcontact.h"

QTCONTACTS_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactOrganization : public QContactDetail
{
public:
#ifndef Q_QDOC
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactOrganization)
#else
    static const DetailType Type;
#endif

    enum OrganizationField {
        FieldName = 0,
        FieldLogoUrl,
        FieldDepartment,
        FieldLocation,
        FieldRole,
        FieldTitle,
        FieldAssistantName,
        FieldStartDate,
        FieldEndDate
    };

    void setName(const QString& name) {setValue(FieldName, name);}
    QString name() const {return value(FieldName).toString();}
    void setLogoUrl(const QUrl& logo) {setValue(FieldLogoUrl, logo);}
    QUrl logoUrl() const {return value<QUrl>(FieldLogoUrl);}
    void setDepartment(const QStringList& department) {setValue(FieldDepartment, department);}
    QStringList department() const {return value<QStringList>(FieldDepartment);}
    void setLocation(const QString& location) {setValue(FieldLocation, location);}
    QString location() const {return value(FieldLocation).toString();}
    void setRole(const QString& role) {setValue(FieldRole, role);}
    QString role() const {return value(FieldRole).toString();}
    void setTitle(const QString& title) {setValue(FieldTitle, title);}
    QString title() const {return value(FieldTitle).toString();}
    void setAssistantName(const QString& assistantName) {setValue(FieldAssistantName, assistantName);}
    QString assistantName() const {return value(FieldAssistantName).toString();}
    void setStartDate(const QDateTime& dateTime) {setValue(FieldStartDate, dateTime);}
    QDateTime startDate() const {return value<QDateTime>(FieldStartDate);}
    void setEndDate(const QDateTime& dateTime) {setValue(FieldEndDate, dateTime);}
    QDateTime endDate() const {return value<QDateTime>(FieldEndDate);}
};

QTCONTACTS_END_NAMESPACE

#endif
