/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERITEMID_H
#define QORGANIZERITEMID_H

#include <qorganizerglobal.h>
#include <qorganizeritemengineid.h>

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif
#include <QtCore/qshareddata.h>

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

QTORGANIZER_BEGIN_NAMESPACE

// MSVC needs the function declared before the friend declaration
class QOrganizerItemId;
Q_ORGANIZER_EXPORT uint qHash(const QOrganizerItemId& key);
#ifndef QT_NO_DATASTREAM
Q_ORGANIZER_EXPORT QDataStream& operator<<(QDataStream& out, const QOrganizerItemId& itemId);
Q_ORGANIZER_EXPORT QDataStream& operator>>(QDataStream& in, QOrganizerItemId& itemId);
#endif
#ifndef QT_NO_DEBUG_STREAM
Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerItemId& id);
#endif

class QOrganizerManagerEngine;

class Q_ORGANIZER_EXPORT QOrganizerItemId
{
public:
    QOrganizerItemId();
    explicit QOrganizerItemId(QOrganizerItemEngineId* engineId);
    ~QOrganizerItemId();

    QOrganizerItemId(const QOrganizerItemId& other);
    QOrganizerItemId& operator=(const QOrganizerItemId& other);

    bool operator==(const QOrganizerItemId& other) const;
    bool operator!=(const QOrganizerItemId& other) const;
    bool operator<(const QOrganizerItemId& other) const;

    bool isNull() const;

    QString managerUri() const;

    QString toString() const;
    static QOrganizerItemId fromString(const QString& idString);

private:
    QSharedDataPointer<QOrganizerItemEngineId> d;

#ifndef QT_NO_DEBUG_STREAM
    Q_ORGANIZER_EXPORT friend QDebug operator<<(QDebug dbg, const QOrganizerItemId& id);
#endif
    Q_ORGANIZER_EXPORT friend uint qHash(const QOrganizerItemId& key);
    friend class QOrganizerManagerEngine;
};

QTORGANIZER_END_NAMESPACE

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemId), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

Q_DECLARE_METATYPE(QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemId));

#endif

