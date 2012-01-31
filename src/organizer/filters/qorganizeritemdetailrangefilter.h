/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QORGANIZERITEMDETAILRANGEFILTER_H
#define QORGANIZERITEMDETAILRANGEFILTER_H

#include <qorganizeritemfilter.h>
#include <qorganizeritemdetail.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemDetailRangeFilterPrivate;
class Q_ORGANIZER_EXPORT QOrganizerItemDetailRangeFilter : public QOrganizerItemFilter
{
public:
    QOrganizerItemDetailRangeFilter();
    QOrganizerItemDetailRangeFilter(const QOrganizerItemFilter &other);

    enum RangeFlag {
        IncludeLower = 0,
        IncludeUpper = 1,
        ExcludeLower = 2,
        ExcludeUpper = 0
    };
    Q_DECLARE_FLAGS(RangeFlags, RangeFlag)

    void setDetail(QOrganizerItemDetail::DetailType detailType, int field);
    QOrganizerItemDetail::DetailType detailType() const;
    int detailField() const;

    void setMatchFlags(QOrganizerItemFilter::MatchFlags flags);
    QOrganizerItemFilter::MatchFlags matchFlags() const;

    void setRange(const QVariant &min, const QVariant &max, RangeFlags flags = 0);
    QVariant minValue() const;
    QVariant maxValue() const;
    RangeFlags rangeFlags() const;

    // to be removed
    QOrganizerItemDetail::DetailType detailDefinitionName() const;
    int detailFieldName() const;
    void setDetailDefinitionName(QOrganizerItemDetail::DetailType definition, int field = -1);

private:
    Q_DECLARE_ORGANIZERITEMFILTER_PRIVATE(QOrganizerItemDetailRangeFilter)
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QOrganizerItemDetailRangeFilter::RangeFlags)

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERITEMDETAILRANGEFILTER_H
