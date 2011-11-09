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

#ifndef QVCARDRESTOREHANDLER_P_H
#define QVCARDRESTOREHANDLER_P_H

#include <qversitglobal.h>
#include <qcontactdetail.h>

QTCONTACTS_USE_NAMESPACE

QTVERSIT_BEGIN_NAMESPACE
class QVersitProperty;

// This is a map from Versit group names to the details that were generated from properties with the
// said groups.  Multiple details can be associated with a single group.
class DetailGroupMap
{
public:
    QList<QContactDetail> detailsInGroup(const QString& groupName) const;
    void insert(const QString& groupName, const QContactDetail& detail);
    void update(const QContactDetail& detail);
    void clear();

private:
    QHash<int, QString> mDetailGroupName; // detailid -> group name
    QHash<int, QContactDetail> mDetailById; // detailid -> detail
};

class QVCardRestoreHandler
{
public:
    bool propertyProcessed(const QVersitProperty& property,
                           QList<QContactDetail>* updatedDetails);
    void documentProcessed();

    const static QString PropertyName;
    const static QString DetailDefinitionParameter;
    const static QString FieldParameter;
    const static QString DatatypeParameter;
    const static QString DatatypeParameterVariant;
    const static QString DatatypeParameterDate;
    const static QString DatatypeParameterDateTime;
    const static QString DatatypeParameterTime;
    const static QString DatatypeParameterBool;
    const static QString DatatypeParameterInt;
    const static QString DatatypeParameterUInt;
    const static QString DatatypeParameterUrl;
    const static QString GroupPrefix;

private:
    static QVariant deserializeValue(const QVersitProperty& property);
    DetailGroupMap mDetailGroupMap; // remembers which details came from which groups
};

QTVERSIT_END_NAMESPACE

#endif
