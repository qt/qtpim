/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QORGANIZERITEMDETAIL_H
#define QORGANIZERITEMDETAIL_H

#include <qorganizerglobal.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qvariant.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemDetailPrivate;

// MSVC needs the function declared before the friend declaration
class QOrganizerItemDetail;
Q_ORGANIZER_EXPORT uint qHash(const QOrganizerItemDetail &key);
#ifndef QT_NO_DATASTREAM
Q_ORGANIZER_EXPORT QDataStream &operator<<(QDataStream &out, const QOrganizerItemDetail &detail);
Q_ORGANIZER_EXPORT QDataStream &operator>>(QDataStream &in, QOrganizerItemDetail &detail);
#endif // QT_NO_DATASTREAM

class Q_ORGANIZER_EXPORT QOrganizerItemDetail
{
public:
    enum DetailType {
        TypeUndefined = 0,
        TypeClassification = 100,
        TypeComment = 200,
        TypeDescription = 300,
        TypeDisplayLabel = 400,
        TypeItemType = 500,
        TypeGuid = 600,
        TypeLocation = 700,
        TypeParent = 800,
        TypePriority = 900,
        TypeRecurrence = 1000,
        TypeTag = 1100,
        TypeTimestamp = 1200,
        TypeVersion = 1300,
        TypeReminder = 1400,
        TypeAudibleReminder = 1500,
        TypeEmailReminder = 1600,
        TypeVisualReminder = 1700,
        TypeExtendedDetail = 1800,
        TypeEventAttendee = 1900,
        TypeEventRsvp = 2000,
        TypeEventTime = 2100,
        TypeJournalTime = 2200,
        TypeTodoProgress = 2300,
        TypeTodoTime = 2400
    };

    QOrganizerItemDetail(DetailType detailType = TypeUndefined);
    QOrganizerItemDetail(const QOrganizerItemDetail &other);
    ~QOrganizerItemDetail();

    QOrganizerItemDetail &operator=(const QOrganizerItemDetail &other);

    bool operator==(const QOrganizerItemDetail &other) const;
    bool operator!=(const QOrganizerItemDetail &other) const {return !(other == *this);}

    DetailType type() const;
    bool isEmpty() const;

    int key() const;
    void resetKey();

    bool setValue(int field, const QVariant &value);
    bool removeValue(int field);
    bool hasValue(int field) const;

    QMap<int, QVariant> values() const;
    QVariant value(int field) const;
    template <typename T> T value(int field) const
    {
        return value(field).value<T>();
    }

protected:
    QOrganizerItemDetail(const QOrganizerItemDetail &other, DetailType expectedDetailType);
    QOrganizerItemDetail &assign(const QOrganizerItemDetail &other, DetailType expectedDetailType);

private:
    friend Q_ORGANIZER_EXPORT uint qHash(const QOrganizerItemDetail &key);
    friend class QOrganizerItem;
    friend class QOrganizerItemDetailPrivate;

#ifndef QT_NO_DATASTREAM
    friend Q_ORGANIZER_EXPORT QDataStream &operator>>(QDataStream &in, QOrganizerItemDetail &detail);
#endif // QT_NO_DATASTREAM

    QSharedDataPointer<QOrganizerItemDetailPrivate> d;
};

#ifndef QT_NO_DEBUG_STREAM
Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerItemDetail &detail);
#endif // QT_NO_DEBUG_STREAM

#define Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(className, detailType) \
    className() : QOrganizerItemDetail(detailType) {} \
    className(const QOrganizerItemDetail &other) : QOrganizerItemDetail(other, detailType) {} \
    className &operator=(const QOrganizerItemDetail &other) {assign(other, detailType); return *this;}

QT_END_NAMESPACE_ORGANIZER

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemDetail), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

#endif // QORGANIZERITEMDETAIL_H
