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


#ifndef QCONTACTRELATIONSHIP_H
#define QCONTACTRELATIONSHIP_H

#include <QString>
#include <QList>
#include <QPair>
#include <QSharedDataPointer>

#include <qcontactsglobal.h>

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

QTCONTACTS_BEGIN_NAMESPACE

class QContactRelationshipPrivate;
class QContact;

class Q_CONTACTS_EXPORT QContactRelationship
{
public:
    inline static const QString HasMember() {return QStringLiteral("HasMember");}
    inline static const QString Aggregates() {return QStringLiteral("Aggregates");}
    inline static const QString IsSameAs() {return QStringLiteral("IsSameAs");}
    inline static const QString HasAssistant() {return QStringLiteral("HasAssistant");}
    inline static const QString HasManager() {return QStringLiteral("HasManager");}
    inline static const QString HasSpouse() {return QStringLiteral("HasSpouse");}

    QContactRelationship();
    ~QContactRelationship();

    QContactRelationship(const QContactRelationship& other);
    QContactRelationship& operator=(const QContactRelationship& other);
    bool operator==(const QContactRelationship &other) const;
    bool operator!=(const QContactRelationship &other) const { return !(*this==other); }

    QContact first() const;
    QContact second() const;
    QString relationshipType() const;

    void setFirst(const QContact& firstContact);
    void setSecond(const QContact& secondContact);
    void setRelationshipType(const QString& relationshipType);

    enum Role {
        First = 0,
        Second,
        Either
    };

private:
    QSharedDataPointer<QContactRelationshipPrivate> d;
};

Q_CONTACTS_EXPORT uint qHash(const QContactRelationship& key);
#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactRelationship& rel);
#endif
#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactRelationship& rel);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactRelationship& rel);
#endif

QTCONTACTS_END_NAMESPACE

#endif

