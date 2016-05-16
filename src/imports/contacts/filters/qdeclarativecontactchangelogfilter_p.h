/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
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

#ifndef QDECLARATIVECONTACTCHANGELOGFILTER_H
#define QDECLARATIVECONTACTCHANGELOGFILTER_H

#include <QtContacts/qcontactchangelogfilter.h>

#include "qdeclarativecontactfilter_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactChangeLogFilter: public QDeclarativeContactFilter
{
    Q_OBJECT
    Q_PROPERTY(QDateTime since READ since WRITE setSince NOTIFY valueChanged)
    Q_PROPERTY(EventType eventType READ eventType WRITE setEventType NOTIFY valueChanged)

    Q_ENUMS(EventType)
public:
    enum EventType {
        EventAdded = QContactChangeLogFilter::EventAdded,
        EventChanged = QContactChangeLogFilter::EventChanged,
        EventRemoved = QContactChangeLogFilter::EventRemoved
    };
    QDeclarativeContactChangeLogFilter(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactFilter(parent)
    {
        connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
    }

    void setEventType(EventType type)
    {
       if (type != eventType()) {
           d.setEventType(static_cast<QContactChangeLogFilter::EventType>(type));
           emit valueChanged();
       }
    }

    EventType eventType() const
    {
        return static_cast<EventType>(d.eventType());
    }

    void setSince(const QDateTime& since)
    {
        if (since != d.since()) {
            d.setSince(since);
            emit valueChanged();
        }
    }

    QDateTime since() const
    {
        return d.since();
    }

    QContactFilter filter() const
    {
        return d;
    }
signals:
    void valueChanged();


private:
    QContactChangeLogFilter d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactChangeLogFilter)

#endif // QDECLARATIVECONTACTCHANGELOGFILTER_H
