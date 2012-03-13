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

#ifndef QDECLARATIVECONTACTRELATIONSHIPFILTER_H
#define QDECLARATIVECONTACTRELATIONSHIPFILTER_H

#include "qdeclarativecontactfilter_p.h"
#include "qcontactrelationshipfilter.h"
#include "qdeclarativecontactrelationship_p.h"
#include "qdeclarativecontact_p.h"
#include <qcontactmanager.h>
#include <QDebug>

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactRelationshipFilter : public QDeclarativeContactFilter
{
    Q_OBJECT
    Q_PROPERTY(QVariant relationshipType READ relationshipType WRITE setRelationshipType NOTIFY valueChanged())
    Q_PROPERTY(QDeclarativeContact* relatedContact READ relatedContact WRITE setRelatedContact NOTIFY valueChanged())
    Q_PROPERTY(QDeclarativeContactRelationship::RelationshipRole relatedContactRole READ relatedContactRole WRITE setRelatedContactRole NOTIFY valueChanged())

public:

    QDeclarativeContactRelationshipFilter(QObject* parent = 0)
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

    QDeclarativeContact* relatedContact() const
    {
        QDeclarativeContact *v = new QDeclarativeContact();
        v->setContact(d.relatedContact());
        return v;
    }

    void setRelatedContact(const QDeclarativeContact* v)
    {
        if (v->contact() != d.relatedContact()) {
            d.setRelatedContact(v->contact());
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


QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactRelationshipFilter))

#endif
