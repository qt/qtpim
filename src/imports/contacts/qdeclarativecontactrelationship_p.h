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

#ifndef QDECLARATIVECONTACTRELATIONSHIP_P_H
#define QDECLARATIVECONTACTRELATIONSHIP_P_H

#include <QtQml/qqml.h>

#include <QtContacts/qcontactrelationship.h>

#include "qdeclarativecontact_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactRelationship : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString first READ first WRITE setFirst NOTIFY valueChanged)
    Q_PROPERTY(QString second READ second WRITE setSecond NOTIFY valueChanged)
    Q_PROPERTY(QVariant type READ relationshipType WRITE setRelationshipType NOTIFY valueChanged)
    Q_ENUMS(RelationshipRole)
    Q_ENUMS(RelationshipType)
public:
    enum RelationshipRole {
        First = QContactRelationship::First,
        Second = QContactRelationship::Second,
        Either = QContactRelationship::Either
    };

    enum RelationshipType {
        Unknown = 0,
        HasMember,
        Aggregates,
        IsSameAs,
        HasAssistant,
        HasManager,
        HasSpouse
    };

    QDeclarativeContactRelationship(QObject* parent = Q_NULLPTR);

    QString first() const;
    QString second() const;
    QVariant relationshipType() const;

    void setFirst(const QString& firstContactId);
    void setSecond(const QString& secondContactId);
    void setRelationshipType(const QVariant& relationshipType);

    QContactRelationship relationship() const;
    void setRelationship(const QContactRelationship& relationship);

signals:
    void valueChanged();
private:
    QContactRelationship m_relationship;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactRelationship)

#endif // QDECLARATIVECONTACTRELATIONSHIP_P_H
