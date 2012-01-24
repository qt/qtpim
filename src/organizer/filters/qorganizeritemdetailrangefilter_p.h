/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERITEMDETAILRANGEFILTER_P_H
#define QORGANIZERITEMDETAILRANGEFILTER_P_H

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

#include "qorganizeritemfilter_p.h"

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemDetailRangeFilterPrivate : public QOrganizerItemFilterPrivate
{
public:
    QOrganizerItemDetailRangeFilterPrivate()
        : QOrganizerItemFilterPrivate(), m_detailType(QOrganizerItemDetail::TypeUndefined), m_detailField(-1), m_flags(0), m_rangeflags(0)
    {
    }

    QOrganizerItemDetailRangeFilterPrivate(const QOrganizerItemDetailRangeFilterPrivate& other)
        : QOrganizerItemFilterPrivate(other), m_detailType(other.m_detailType), m_detailField(other.m_detailField),
          m_minValue(other.m_minValue), m_maxValue(other.m_maxValue), m_flags(other.m_flags), m_rangeflags(other.m_rangeflags)
    {
    }

    virtual bool compare(const QOrganizerItemFilterPrivate *other) const
    {
        const QOrganizerItemDetailRangeFilterPrivate *od = static_cast<const QOrganizerItemDetailRangeFilterPrivate *>(other);
        if (od) {
            return (m_detailType == od->m_detailType) && (m_detailField == od->m_detailField)
                   && (m_minValue == od->m_minValue) && (m_maxValue == od->m_maxValue)
                   && (m_flags == od->m_flags) && (m_rangeflags == od->m_rangeflags);
        }
        return false;
    }

#ifndef QT_NO_DATASTREAM
    QDataStream &outputToStream(QDataStream &stream, quint8 formatVersion) const
    {
        if (formatVersion == 1) {
            stream << m_detailType << m_detailField << m_minValue << m_maxValue
                   << static_cast<quint32>(m_flags)
                   << static_cast<quint32>(m_rangeflags);
        }
        return stream;
    }

    QDataStream &inputFromStream(QDataStream &stream, quint8 formatVersion)
    {
        if (formatVersion == 1) {
            quint32 flags;
            quint32 rangeFlags;
            quint32 defId;
            stream >> defId >> m_detailField >> m_minValue >> m_maxValue >> flags >> rangeFlags;
            m_detailType = static_cast<QOrganizerItemDetail::DetailType>(defId);
            m_flags = static_cast<QOrganizerItemFilter::MatchFlags>(flags);
            m_rangeflags = static_cast<QOrganizerItemDetailRangeFilter::RangeFlags>(rangeFlags);
        }
        return stream;
    }
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
    QDebug &debugStreamOut(QDebug &dbg) const
    {
        dbg.nospace() << "QOrganizerItemDetailRangeFilter(";
        dbg.nospace() << "detailType=";
        dbg.nospace() << m_detailType;
        dbg.nospace() << ",";
        dbg.nospace() << "detailField=";
        dbg.nospace() << m_detailField;
        dbg.nospace() << ",";
        dbg.nospace() << "minValue=";
        dbg.nospace() << m_minValue;
        dbg.nospace() << ",";
        dbg.nospace() << "maxValue=";
        dbg.nospace() << m_maxValue;
        dbg.nospace() << ",";
        dbg.nospace() << "matchFlags=";
        dbg.nospace() << static_cast<quint32>(m_flags);
        dbg.nospace() << ",";
        dbg.nospace() << "rangeFlags=";
        dbg.nospace() << static_cast<quint32>(m_rangeflags);
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif // QT_NO_DEBUG_STREAM

    Q_IMPLEMENT_ORGANIZERITEMFILTER_VIRTUALCTORS(QOrganizerItemDetailRangeFilter, QOrganizerItemFilter::DetailRangeFilter)

    QOrganizerItemDetail::DetailType m_detailType;
    int m_detailField;
    QVariant m_minValue;
    QVariant m_maxValue;
    QOrganizerItemFilter::MatchFlags m_flags;
    QOrganizerItemDetailRangeFilter::RangeFlags m_rangeflags;
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERITEMDETAILRANGEFILTER_P_H
