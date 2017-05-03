/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QORGANIZERITEMFETCHREQUEST_H
#define QORGANIZERITEMFETCHREQUEST_H

#include <QtCore/qlist.h>

#include <QtOrganizer/qorganizerabstractrequest.h>
#include <QtOrganizer/qorganizeritem.h>
#include <QtOrganizer/qorganizeritemsortorder.h>

QT_FORWARD_DECLARE_CLASS(QDateTime)

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemFetchHint;
class QOrganizerItemFilter;

class QOrganizerItemFetchRequestPrivate;

/* Leaf class */

class Q_ORGANIZER_EXPORT QOrganizerItemFetchRequest : public QOrganizerAbstractRequest
{
    Q_OBJECT

public:
    QOrganizerItemFetchRequest(QObject *parent = Q_NULLPTR);
    ~QOrganizerItemFetchRequest();

    void setFilter(const QOrganizerItemFilter &filter);
    QOrganizerItemFilter filter() const;

    void setSorting(const QList<QOrganizerItemSortOrder> &sorting);
    QList<QOrganizerItemSortOrder> sorting() const;

    void setFetchHint(const QOrganizerItemFetchHint &fetchHint);
    QOrganizerItemFetchHint fetchHint() const;

    void setStartDate(const QDateTime &date);
    QDateTime startDate() const;

    void setEndDate(const QDateTime &date);
    QDateTime endDate() const;

    void setMaxCount(int maxCount);
    int maxCount() const;

    QList<QOrganizerItem> items() const;

private:
    Q_DISABLE_COPY(QOrganizerItemFetchRequest)
    friend class QOrganizerManagerEngine;
    Q_DECLARE_PRIVATE_D(d_ptr, QOrganizerItemFetchRequest)
};

QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERITEMFETCHREQUEST_H
