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

#ifndef QORGANIZERITEMOCCURRENCEFETCHREQUEST_H
#define QORGANIZERITEMOCCURRENCEFETCHREQUEST_H

#include <QtCore/qlist.h>

#include <QtOrganizer/qorganizerabstractrequest.h>
#include <QtOrganizer/qorganizeritem.h>

QT_FORWARD_DECLARE_CLASS(QDateTime)

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemFetchHint;

class QOrganizerItemOccurrenceFetchRequestPrivate;

/* Leaf class */

class Q_ORGANIZER_EXPORT QOrganizerItemOccurrenceFetchRequest : public QOrganizerAbstractRequest
{
    Q_OBJECT

public:
    QOrganizerItemOccurrenceFetchRequest(QObject *parent = nullptr);
    ~QOrganizerItemOccurrenceFetchRequest();

    void setParentItem(const QOrganizerItem &item);
    QOrganizerItem parentItem() const;

    void setStartDate(const QDateTime &date);
    QDateTime startDate() const;

    void setEndDate(const QDateTime &date);
    QDateTime endDate() const;

    void setMaxOccurrences(int maxCount);
    int maxOccurrences() const;

    void setFetchHint(const QOrganizerItemFetchHint &hint);
    QOrganizerItemFetchHint fetchHint() const;

    QList<QOrganizerItem> itemOccurrences() const;

private:
    Q_DISABLE_COPY(QOrganizerItemOccurrenceFetchRequest)
    friend class QOrganizerManagerEngine;
    Q_DECLARE_PRIVATE_D(d_ptr, QOrganizerItemOccurrenceFetchRequest)
};

QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERITEMOCCURRENCEFETCHREQUEST_H
