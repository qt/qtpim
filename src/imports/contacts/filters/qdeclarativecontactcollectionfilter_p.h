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

#ifndef QDECLARATIVECONTACTCOLLECTIONFILTER_H
#define QDECLARATIVECONTACTCOLLECTIONFILTER_H

#include <QtContacts/qcontactcollectionfilter.h>

#include "qdeclarativecontactfilter_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactCollectionFilter : public QDeclarativeContactFilter
{
    Q_OBJECT
    Q_PROPERTY(QStringList ids READ ids WRITE setIds NOTIFY valueChanged)

public:
    QDeclarativeContactCollectionFilter(QObject *parent = Q_NULLPTR)
        : QDeclarativeContactFilter(parent)
    {
        connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
    }

    QStringList ids() const
    {
        return m_ids;
    }

    void setIds(const QStringList &ids)
    {
        foreach (const QString &id, ids) {
            if (!m_ids.contains(id)) {
                m_ids = ids;
                emit valueChanged();
                return;
            }
        }

        foreach (const QString &id, m_ids) {
            if (!ids.contains(id)) {
                m_ids = ids;
                emit valueChanged();
                return;
            }
        }
    }

    // used by model
    QContactFilter filter() const
    {
        QContactCollectionFilter f;
        QSet<QContactCollectionId> ids;

        foreach (const QVariant &id, m_ids) {
            QContactCollectionId cId = QContactCollectionId::fromString(id.toString());
            if (!cId.isNull())
                ids << cId;
        }

        f.setCollectionIds(ids);
        return f;
    }

Q_SIGNALS:
    void valueChanged();

private:
    QStringList m_ids;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactCollectionFilter)

#endif // QDECLARATIVECONTACTCOLLECTIONFILTER_H
