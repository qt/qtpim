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

#ifndef QCONTACTRELATIONSHIP_H
#define QCONTACTRELATIONSHIP_H

#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

#include <QtContacts/qcontactsglobal.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactId;

class QContactRelationshipPrivate;
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

    QContactId first() const;
    QContactId second() const;
    QString relationshipType() const;

    void setFirst(const QContactId& firstContact);
    void setSecond(const QContactId& secondContact);
    void setRelationshipType(const QString& relationshipType);

    enum Role {
        First = 0,
        Second,
        Either
    };

private:
    QSharedDataPointer<QContactRelationshipPrivate> d;
};

Q_CONTACTS_EXPORT size_t qHash(const QContactRelationship& key);
#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactRelationship& rel);
#endif
#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactRelationship& rel);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactRelationship& rel);
#endif

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTRELATIONSHIP_H
