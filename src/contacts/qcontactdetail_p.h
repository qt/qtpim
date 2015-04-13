/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd and/or its subsidiary(-ies).
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

#include <QtCore/qmap.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qvariant.h>

#include <QtContacts/qcontactdetail.h>

QT_BEGIN_NAMESPACE_CONTACTS

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

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTDETAIL_P_H
