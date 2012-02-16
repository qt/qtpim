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


#ifndef QCONTACTDETAIL_P_H
#define QCONTACTDETAIL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qcontactactiondescriptor.h"
#include "qcontactdetail.h"

#include <QSharedData>
#include <QString>
#include <QHash>

QTCONTACTS_BEGIN_NAMESPACE

class QContactDetailPrivate : public QSharedData
{
public:
    QContactDetailPrivate()
        : QSharedData(),
        m_id(lastDetailKey.fetchAndAddOrdered(1)),
        m_access(QContactDetail::NoConstraint)
    {
    }

    QContactDetailPrivate(const QContactDetailPrivate& other)
        : QSharedData(other),
        m_id(other.m_id),
        m_type(other.m_type),
        m_values(other.m_values),
        m_access(other.m_access)
    {
    }

    ~QContactDetailPrivate()
    {
    }

    int m_id; // internal, unique id.
    QContactDetail::DetailType m_type;
    QMap<int, QVariant> m_values;
    QContactDetail::AccessConstraints m_access;

    static QAtomicInt lastDetailKey;

    static void setAccessConstraints(QContactDetail *d, QContactDetail::AccessConstraints constraint)
    {
        d->d->m_access = constraint;
    }

    static const QContactDetailPrivate* detailPrivate(const QContactDetail& detail)
    {
        return detail.d.constData();
    }
};

QTCONTACTS_END_NAMESPACE

#endif
