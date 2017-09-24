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
    QDeclarativeContactChangeLogFilter(QObject* parent = nullptr)
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
