/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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
