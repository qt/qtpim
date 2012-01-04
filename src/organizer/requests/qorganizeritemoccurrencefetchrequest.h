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

#ifndef QORGANIZERITEMOCCURRENCEFETCHREQUEST_H
#define QORGANIZERITEMOCCURRENCEFETCHREQUEST_H

#include <qorganizerabstractrequest.h>
#include <qorganizeritem.h>

QTORGANIZER_BEGIN_NAMESPACE

class QOrganizerItemOccurrenceFetchRequestPrivate;
class Q_ORGANIZER_EXPORT QOrganizerItemOccurrenceFetchRequest : public QOrganizerAbstractRequest
{
    Q_OBJECT

public:
    QOrganizerItemOccurrenceFetchRequest(QObject *parent = 0);
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

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERITEMOCCURRENCEFETCHREQUEST_H
