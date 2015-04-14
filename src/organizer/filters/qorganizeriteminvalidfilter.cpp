/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include "qorganizeriteminvalidfilter.h"

#include "qorganizeritemfilter_p.h"

/*!
    \class QOrganizerItemInvalidFilter
    \brief The QOrganizerItemInvalidFilter class matches no organizeritems.
    \inmodule QtOrganizer
    \ingroup organizer-filters

    This class provides a filter which will never match any organizer items.
 */

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemInvalidFilterPrivate : public QOrganizerItemFilterPrivate
{
public:
    QOrganizerItemInvalidFilterPrivate()
       : QOrganizerItemFilterPrivate()
    {
    }

    bool compare(const QOrganizerItemFilterPrivate *) const
    {
        return true;
    }

#ifndef QT_NO_DATASTREAM
    QDataStream &outputToStream(QDataStream &stream, quint8 formatVersion) const
    {
        Q_UNUSED(formatVersion)
        return stream;
    }

    QDataStream &inputFromStream(QDataStream &stream, quint8 formatVersion)
    {
        Q_UNUSED(formatVersion)
        return stream;
    }
#endif // QT_NO_DATASTREAM

#ifndef QT_NO_DEBUG_STREAM
    QDebug &debugStreamOut(QDebug &dbg) const
    {
        dbg.nospace() << "QOrganizerItemInvalidFilter()";
        return dbg.maybeSpace() ;
    }
#endif // QT_NO_DEBUG_STREAM

    QOrganizerItemFilterPrivate *clone() const
    {
        return new QOrganizerItemInvalidFilterPrivate();
    }

    QOrganizerItemFilter::FilterType type() const
    {
        return QOrganizerItemFilter::InvalidFilter;
    }

    QList<QOrganizerItemFilter> m_filters;
};

/*!
    Constructs a new invalid filter.
 */
QOrganizerItemInvalidFilter::QOrganizerItemInvalidFilter()
    : QOrganizerItemFilter(new QOrganizerItemInvalidFilterPrivate)
{
}

/*!
    Constructs a new invalid filter, ignoring the \a other filter.
 */
QOrganizerItemInvalidFilter::QOrganizerItemInvalidFilter(const QOrganizerItemFilter &other)
    : QOrganizerItemFilter(new QOrganizerItemInvalidFilterPrivate)
{
    Q_UNUSED(other);
}

QT_END_NAMESPACE_ORGANIZER
