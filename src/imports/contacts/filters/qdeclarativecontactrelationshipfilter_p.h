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

#ifndef QDECLARATIVECONTACTRELATIONSHIPFILTER_H
#define QDECLARATIVECONTACTRELATIONSHIPFILTER_H

#include <QtContacts/qcontactrelationshipfilter.h>

#include "qdeclarativecontact_p.h"
#include "qdeclarativecontactfilter_p.h"
#include "qdeclarativecontactrelationship_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactRelationshipFilter : public QDeclarativeContactFilter
{
    Q_OBJECT
    Q_PROPERTY(QVariant relationshipType READ relationshipType WRITE setRelationshipType NOTIFY valueChanged)
    Q_PROPERTY(QString relatedContactId READ relatedContactId WRITE setRelatedContactId NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeContactRelationship::RelationshipRole relatedContactRole READ relatedContactRole WRITE setRelatedContactRole NOTIFY valueChanged)

public:

    QDeclarativeContactRelationshipFilter(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactFilter(parent)
    {
        connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
    }

    QVariant relationshipType() const
    {
        return d.relationshipType();
    }
    void setRelationshipType(const QVariant& v)
    {
        QString rt;
        if (v.type() != QVariant::String) {
            switch (v.toInt()) {
            case QDeclarativeContactRelationship::HasMember:
                rt = QContactRelationship::HasMember();
                break;
            case QDeclarativeContactRelationship::Aggregates:
                rt = QContactRelationship::Aggregates();
                break;
            case QDeclarativeContactRelationship::IsSameAs:
                rt = QContactRelationship::IsSameAs();
                break;
            case QDeclarativeContactRelationship::HasAssistant:
                rt = QContactRelationship::HasAssistant();
                break;
            case QDeclarativeContactRelationship::HasManager:
                rt = QContactRelationship::HasManager();
                break;
            case QDeclarativeContactRelationship::HasSpouse:
                rt = QContactRelationship::HasSpouse();
                break;
           default:
                break;
            }
        } else {
            rt = v.toString();
        }

        if (rt != relationshipType()) {
            d.setRelationshipType(rt);
            emit valueChanged();
        }
    }

    QString relatedContactId() const
    {
        const QContactId contactId(d.relatedContactId());
        if (!contactId.isNull())
            return contactId.toString();
        return QString();
    }

    void setRelatedContactId(const QString& id)
    {
        const QContactId contactId(QContactId::fromString(id));
        if (contactId != d.relatedContactId()) {
            d.setRelatedContactId(contactId);
            emit valueChanged();
        }
    }

    QDeclarativeContactRelationship::RelationshipRole relatedContactRole() const
    {
        switch (d.relatedContactRole()) {
        case QContactRelationship::First:
            return QDeclarativeContactRelationship::First;
        case QContactRelationship::Second:
            return QDeclarativeContactRelationship::Second;
        default:
            break;
        }
        return QDeclarativeContactRelationship::Either;
    }

    void setRelatedContactRole(QDeclarativeContactRelationship::RelationshipRole v)
    {
        if (v != relatedContactRole()) {
            switch (v) {
            case QDeclarativeContactRelationship::First:
                d.setRelatedContactRole(QContactRelationship::First);
                break;
            case QDeclarativeContactRelationship::Second:
                d.setRelatedContactRole(QContactRelationship::Second);
                break;
            case QDeclarativeContactRelationship::Either:
                d.setRelatedContactRole(QContactRelationship::Either);
                break;
            }
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
    QContactRelationshipFilter d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactRelationshipFilter)

#endif // QDECLARATIVECONTACTRELATIONSHIPFILTER_H
