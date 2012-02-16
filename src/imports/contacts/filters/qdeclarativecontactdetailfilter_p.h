/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QDECLARATIVECONTACTDETAILFILTER_H
#define QDECLARATIVECONTACTDETAILFILTER_H

#include "qdeclarativecontactfilter_p.h"
#include "qcontactdetailfilter.h"
#include "qdeclarativecontactdetail_p.h"
#include <QDeclarativeParserStatus>

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactDetailFilter : public QDeclarativeContactFilter, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeContactDetail::DetailType detail READ detail WRITE setDetail NOTIFY valueChanged())
    Q_PROPERTY(int field READ field WRITE setField NOTIFY valueChanged())
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged())
    Q_PROPERTY(MatchFlags matchFlags READ matchFlags WRITE setMatchFlags NOTIFY valueChanged())
    Q_INTERFACES(QDeclarativeParserStatus)
public:

    QDeclarativeContactDetailFilter(QObject* parent = 0)
        :QDeclarativeContactFilter(parent),
          m_componentCompleted(false)
    {
        connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
    }
    //from QDeclarativeParserStatus
    void classBegin() {}
    void componentComplete()
    {
        m_componentCompleted = true;
    }

    void setDetail(QDeclarativeContactDetail::DetailType detail)
    {
        if (m_detail != detail) {
            m_detail = detail;
            d.setDetailType(static_cast<QContactDetail::DetailType>(m_detail),
                            m_field);
            emit valueChanged();
         }
    }

    QDeclarativeContactDetail::DetailType detail() const
    {
        return m_detail;
    }

    void setField(int field)
    {
        if (field != m_field) {
            m_field = field;
            d.setDetailType(static_cast<QContactDetail::DetailType>(m_detail),
                            m_field);
            emit valueChanged();
        }
    }

    int field() const
    {
        return m_field;
    }


    QDeclarativeContactFilter::MatchFlags matchFlags() const
    {
        QDeclarativeContactFilter::MatchFlags flags;
        flags = ~flags & (int)d.matchFlags();
        return flags;
    }
    void setMatchFlags(QDeclarativeContactFilter::MatchFlags flags)
    {
        QContactFilter::MatchFlags newFlags;
        newFlags = ~newFlags & (int)flags;
        if (newFlags != d.matchFlags()) {
            d.setMatchFlags(newFlags);
            emit valueChanged();
        }
    }

    QVariant value() const
    {
        return d.value();
    }
    void setValue(const QVariant& value)
    {
        if (value != d.value()) {
            d.setValue(value);
            emit valueChanged();
        }
    }

    QContactFilter filter() const
    {
        return d;
    }
signals:
    void valueChanged();


private:
    bool m_componentCompleted;
    int m_field;
    QDeclarativeContactDetail::DetailType m_detail;
    QContactDetailFilter d;
};



QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactDetailFilter))

#endif
