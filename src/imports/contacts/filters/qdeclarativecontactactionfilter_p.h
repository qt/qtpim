/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#ifndef QDECLARATIVECONTACTACTIONFILTER_H
#define QDECLARATIVECONTACTACTIONFILTER_H

#include <QtContacts/qcontactactionfilter.h>

#include "qdeclarativecontactfilter_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactActionFilter : public QDeclarativeContactFilter
{
    Q_OBJECT
    Q_PROPERTY(QString actionName READ actionName WRITE setActionName NOTIFY valueChanged)
public:
    QDeclarativeContactActionFilter(QObject* parent=0)
        :QDeclarativeContactFilter(parent)
    {
        connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
    }
    void setActionName(const QString& action)
    {
        if (action != d.actionName()) {
            d.setActionName(action);
            emit valueChanged();
        }
    }

    QString actionName() const
    {
        return d.actionName();
    }
    QContactFilter filter() const
    {
        return d;
    }

signals:
    void valueChanged();

private:
    QContactActionFilter d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactActionFilter)

#endif // QDECLARATIVECONTACTACTIONFILTER_H
