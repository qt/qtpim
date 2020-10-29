/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
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

#ifndef QCONTACTDETAILRANGEFILTER_P_H
#define QCONTACTDETAILRANGEFILTER_P_H

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

#include <QtContacts/qcontactdetailrangefilter.h>
#include <QtContacts/private/qcontactfilter_p.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactDetailRangeFilterPrivate : public QContactFilterPrivate
{
public:
    QContactDetailRangeFilterPrivate() = default;

    QContactDetailRangeFilterPrivate(const QContactDetailRangeFilterPrivate& other)
        : QContactFilterPrivate(other),
        m_typeId(other.m_typeId),
        m_fieldId(other.m_fieldId),
        m_minValue(other.m_minValue),
        m_maxValue(other.m_maxValue),
        m_flags(other.m_flags),
        m_rangeflags(other.m_rangeflags)
    {
    }

    bool compare(const QContactFilterPrivate* other) const override
    {
        const QContactDetailRangeFilterPrivate *od = static_cast<const QContactDetailRangeFilterPrivate*>(other);
        if (m_typeId != od->m_typeId)
            return false;
        if (m_fieldId != od->m_fieldId)
            return false;
        if (m_minValue != od->m_minValue)
            return false;
        if (m_maxValue!= od->m_maxValue)
            return false;
        if (m_flags != od->m_flags)
            return false;
        if (m_rangeflags != od->m_rangeflags)
            return false;
        return true;
    }

    QDataStream& outputToStream(QDataStream& stream, quint8 formatVersion) const override
    {
        if (formatVersion == 1) {
            stream << quint32(m_typeId) << m_fieldId << m_minValue << m_maxValue
                << static_cast<quint32>(m_flags)
                << static_cast<quint32>(m_rangeflags);
        }
        return stream;
    }

    QDataStream& inputFromStream(QDataStream& stream, quint8 formatVersion) override
    {
        if (formatVersion == 1) {
            quint32 flags;
            quint32 rangeFlags;
            quint32 type;
            stream >> type >> m_fieldId >> m_minValue >> m_maxValue >> flags >> rangeFlags;
            m_flags = QContactFilter::MatchFlags(flags);
            m_rangeflags = QContactDetailRangeFilter::RangeFlags(rangeFlags);
            m_typeId = QContactDetail::DetailType(type);
        }
        return stream;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const override
    {
        dbg.nospace() << "QContactDetailRangeFilter(";
        dbg.nospace() << "detailType=" << static_cast<quint32>(m_typeId) << ","
                      << "detailField=" << m_fieldId << ","
                      << "minValue=" << m_minValue << ","
                      << "maxValue=" << m_maxValue << ","
                      << "matchFlags=" << static_cast<quint32>(m_flags) << ","
                      << "rangeFlags=" << static_cast<quint32>(m_rangeflags);
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    Q_IMPLEMENT_CONTACTFILTER_VIRTUALCTORS(QContactDetailRangeFilter, QContactFilter::ContactDetailRangeFilter)

    QContactDetail::DetailType m_typeId = QContactDetail::TypeUndefined;
    int m_fieldId = -1;
    QVariant m_minValue;
    QVariant m_maxValue;
    QContactFilter::MatchFlags m_flags;
    QContactDetailRangeFilter::RangeFlags m_rangeflags;
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTDETAILRANGEFILTER_P_H
