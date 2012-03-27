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

#ifndef QORGANIZERITEMIDFETCHREQUEST_H
#define QORGANIZERITEMIDFETCHREQUEST_H

#include <qorganizerabstractrequest.h>
#include <qorganizeritemid.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemIdFetchRequestPrivate;
class Q_ORGANIZER_EXPORT QOrganizerItemIdFetchRequest : public QOrganizerAbstractRequest
{
    Q_OBJECT

public:
    QOrganizerItemIdFetchRequest(QObject *parent = 0);
    ~QOrganizerItemIdFetchRequest();

    void setFilter(const QOrganizerItemFilter &filter);
    QOrganizerItemFilter filter() const;

    void setSorting(const QList<QOrganizerItemSortOrder> &sorting);
    QList<QOrganizerItemSortOrder> sorting() const;

    void setStartDate(const QDateTime &date);
    QDateTime startDate() const;

    void setEndDate(const QDateTime &date);
    QDateTime endDate() const;

    void setStorageLocations(QOrganizerAbstractRequest::StorageLocations storageLocations);
    QOrganizerAbstractRequest::StorageLocations storageLocations() const;

    QList<QOrganizerItemId> itemIds() const;

private:
    Q_DISABLE_COPY(QOrganizerItemIdFetchRequest)
    friend class QOrganizerManagerEngine;
    Q_DECLARE_PRIVATE_D(d_ptr, QOrganizerItemIdFetchRequest)
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERITEMIDFETCHREQUEST_H
