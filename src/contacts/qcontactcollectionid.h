/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Copyright (C) 2017 Canonical Ltd
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

#ifndef QCONTACTCOLLECTIONID_H
#define QCONTACTCOLLECTIONID_H

#include <QtCore/qvariant.h>

#include <QtContacts/qcontactsglobal.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactManagerEngine;

class Q_CONTACTS_EXPORT QContactCollectionId
{
public:
    inline QContactCollectionId() {}
    inline QContactCollectionId(const QString &_managerUri, const QByteArray &_localId)
        : m_managerUri(_localId.isEmpty() ? QString() : _managerUri),
          m_localId(m_managerUri.isEmpty() ? QByteArray() : _localId)
    {}
    // compiler-generated dtor and copy/move ctors/assignment operators are fine!

    inline bool operator==(const QContactCollectionId &other) const
    { return localId() == other.localId() && managerUri() == other.managerUri(); }
    inline bool operator!=(const QContactCollectionId &other) const
    { return !operator==(other); }

    inline bool isNull() const { return m_localId.isEmpty(); }

    inline QString managerUri() const { return m_managerUri; }
    inline QByteArray localId() const { return m_localId; }

    QString toString() const;
    static QContactCollectionId fromString(const QString &idString);

    QByteArray toByteArray() const;
    static QContactCollectionId fromByteArray(const QByteArray &idData);

private:
    QString m_managerUri;
    QByteArray m_localId;
};

inline bool operator<(const QContactCollectionId &id1, const QContactCollectionId &id2)
{ return id1.managerUri() != id2.managerUri() ? id1.managerUri() < id2.managerUri() : id1.localId() < id2.localId(); }

inline size_t qHash(const QContactCollectionId &id)
{ return qHash(id.localId()); }

#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream &operator<<(QDataStream &out, const QContactCollectionId &id);
Q_CONTACTS_EXPORT QDataStream &operator>>(QDataStream &in, QContactCollectionId &id);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactCollectionId &id);
#endif

QT_END_NAMESPACE_CONTACTS

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTCONTACTS_PREPEND_NAMESPACE(QContactCollectionId), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(QTCONTACTS_PREPEND_NAMESPACE(QContactCollectionId))
#endif // QCONTACTCOLLECTIONID_H
