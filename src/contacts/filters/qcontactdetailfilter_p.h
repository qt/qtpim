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

#ifndef QCONTACTDETAILFILTER_P_H
#define QCONTACTDETAILFILTER_P_H

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

#include <QtContacts/qcontactdetailfilter.h>
#include <QtContacts/private/qcontactfilter_p.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactDetailFilterPrivate : public QContactFilterPrivate
{
public:
    QContactDetailFilterPrivate()
        : QContactFilterPrivate()
        , m_type(QContactDetail::TypeUndefined)
        , m_fieldId(-1)
        , m_flags(0)
    {
    }

    QContactDetailFilterPrivate(const QContactDetailFilterPrivate& other)
        : QContactFilterPrivate(other),
        m_type(other.m_type),
        m_fieldId(other.m_fieldId),
        m_exactValue(other.m_exactValue),
        m_flags(other.m_flags)
    {
    }

    bool compare(const QContactFilterPrivate* other) const override
    {
        const QContactDetailFilterPrivate *od = static_cast<const QContactDetailFilterPrivate*>(other);
        if (m_type != od->m_type)
            return false;
        if (m_fieldId != od->m_fieldId)
            return false;
        if (m_exactValue != od->m_exactValue)
            return false;
        if (m_flags != od->m_flags)
            return false;
        return true;
    }

    QDataStream& outputToStream(QDataStream& stream, quint8 formatVersion) const override
    {
        if (formatVersion == 1) {
            stream << static_cast<quint32>(m_type) << m_fieldId << m_exactValue << static_cast<quint32>(m_flags);
        }
        return stream;
    }

    QDataStream& inputFromStream(QDataStream& stream, quint8 formatVersion) override
    {
        if (formatVersion == 1) {
            quint32 flags;
            quint32 type;
            stream >> type >> m_fieldId >> m_exactValue >> flags;
            m_flags = QContactFilter::MatchFlags(flags);
            m_type = QContactDetail::DetailType(type);
        }
        return stream;
    }
#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const override
    {
        dbg.nospace() << "QContactDetailFilter(";
        dbg.nospace() << "detailType=" << static_cast<quint32>(m_type) << ","
                      << "detailFieldName=" << m_fieldId << ","
                      << "value=" << m_exactValue << ","
                      << "matchFlags=" << static_cast<quint32>(m_flags);
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif
    Q_IMPLEMENT_CONTACTFILTER_VIRTUALCTORS(QContactDetailFilter, QContactFilter::ContactDetailFilter)

    QContactDetail::DetailType m_type;
    int m_fieldId;
    QVariant m_exactValue;
    QContactFilter::MatchFlags m_flags;
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTDETAILFILTER_P_H
