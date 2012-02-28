/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QORGANIZERITEMDETAIL_H
#define QORGANIZERITEMDETAIL_H

#include <qorganizerglobal.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qvariant.h>

QTORGANIZER_BEGIN_NAMESPACE

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
        TypeClassification,
        TypeComment,
        TypeDescription,
        TypeDisplayLabel,
        TypeItemType,
        TypeGuid,
        TypeLocation,
        TypeParent,
        TypePriority,
        TypeRecurrence,
        TypeTag,
        TypeTimestamp,
        TypeVersion,
        TypeReminder,
        TypeAudibleReminder,
        TypeEmailReminder,
        TypeVisualReminder,
        TypeExtendedDetail,
        TypeEventAttendee,
        TypeEventRsvp,
        TypeEventTime,
        TypeJournalTime,
        TypeTodoTime,
        TypeTodoProgress
    };

    QOrganizerItemDetail();
    QOrganizerItemDetail(DetailType detailType);
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

    // to be removed
    DetailType definitionName() const;

protected:
    QOrganizerItemDetail(const QOrganizerItemDetail &other, DetailType expectedDetailType);
    QOrganizerItemDetail &assign(const QOrganizerItemDetail &other, DetailType expectedDetailType);

private:
    friend Q_ORGANIZER_EXPORT uint qHash(const QOrganizerItemDetail &key);
    friend class QOrganizerItem;
    friend class QOrganizerItemDetailPrivate;

#ifndef QT_NO_DATASTREAM
    Q_ORGANIZER_EXPORT friend QDataStream &operator>>(QDataStream &in, QOrganizerItemDetail &detail);
#endif // QT_NO_DATASTREAM

    QSharedDataPointer<QOrganizerItemDetailPrivate> d;
};

#ifndef QT_NO_DEBUG_STREAM
Q_ORGANIZER_EXPORT QDebug operator<<(QDebug dbg, const QOrganizerItemDetail &detail);
#endif // QT_NO_DEBUG_STREAM

#define Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(className) \
    className() : QOrganizerItemDetail(DefinitionName) {} \
    className(const QOrganizerItemDetail &other) : QOrganizerItemDetail(other, DefinitionName) {} \
    className &operator=(const QOrganizerItemDetail &other) {assign(other, DefinitionName); return *this;} \
    static const DetailType DefinitionName;

QTORGANIZER_END_NAMESPACE

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemDetail), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

#endif // QORGANIZERITEMDETAIL_H
