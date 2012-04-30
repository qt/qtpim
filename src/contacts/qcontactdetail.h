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


#ifndef QCONTACTDETAIL_H
#define QCONTACTDETAIL_H

#include <qcontactsglobal.h>

#include <QSharedDataPointer>
#include <QStringList>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

QTCONTACTS_BEGIN_NAMESPACE

class QContactActionDescriptor;
class QContactDetailPrivate;

// MSVC needs the function declared before the friend declaration
class QContactDetail;
#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactDetail& detail);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactDetail& detail);
#endif

class Q_CONTACTS_EXPORT QContactDetail
{
public:
    enum DetailType {
        TypeUndefined = 0,
        TypeAddress,
        TypeAnniversary,
        TypeAvatar,
        TypeBirthday,
        TypeDisplayLabel,
        TypeEmailAddress,
        TypeExtendedDetail,
        TypeFamily,
        TypeFavorite,
        TypeGender,
        TypeGeoLocation,
        TypeGlobalPresence,
        TypeGuid,
        TypeHobby,
        TypeName,
        TypeNickname,
        TypeNote,
        TypeOnlineAccount,
        TypeOrganization,
        TypePhoneNumber,
        TypePresence,
        TypeRingtone,
        TypeSyncTarget,
        TypeTag,
        TypeTimestamp,
        TypeType,
        TypeUrl,
        TypeVersion
    };

    QContactDetail();
    explicit QContactDetail(DetailType type);
    ~QContactDetail();

    QContactDetail(const QContactDetail& other);
    QContactDetail& operator=(const QContactDetail& other);

    enum AccessConstraint {
        NoConstraint = 0,
        ReadOnly = 0x01,
        Irremovable = 0x02
    };
    Q_DECLARE_FLAGS(AccessConstraints, AccessConstraint)

    AccessConstraints accessConstraints() const;

    enum DetailContext {
        ContextHome = 0,
        ContextWork,
        ContextOther
    };

    enum DetailField {
        FieldContext = 5000, //to avoid clashing with other detail field values from leaf classes
        FieldDetailUri,
        FieldLinkedDetailUris
    };

    bool operator==(const QContactDetail& other) const;
    bool operator!=(const QContactDetail& other) const {return !(other == *this);}

    DetailType type() const;
    bool isEmpty() const;

    int key() const;
    void resetKey();

    bool setValue(int field, const QVariant& value);
    bool removeValue(int field);
    bool hasValue(int field) const;

    QMap<int, QVariant> values() const;
    QVariant value(int field) const;
    template <typename T> T value(int field) const
    {
        return value(field).value<T>();
    }

    void setContexts(int context)
    {
        QList<int> contexts;
        contexts << context;
        setValue(FieldContext, QVariant::fromValue(contexts));
    }

    void setContexts(const QList<int>& contexts)
    {
        setValue(FieldContext, QVariant::fromValue(contexts));
    }

    QList<int> contexts() const
    {
        return value< QList<int> >(FieldContext);
    }

    void setDetailUri(const QString& detailUri)
    {
        setValue(FieldDetailUri, detailUri);
    }

    QString detailUri() const
    {
        return value(FieldDetailUri).toString();
    }

    void setLinkedDetailUris(const QStringList& linkedDetailUris)
    {
        setValue(FieldLinkedDetailUris, linkedDetailUris);
    }

    void setLinkedDetailUris(const QString& linkedDetailUri)
    {
        setValue(FieldLinkedDetailUris, QStringList(linkedDetailUri));
    }

    QStringList linkedDetailUris() const
    {
        return value<QStringList>(FieldLinkedDetailUris);
    }

protected:
    QContactDetail(const QContactDetail &other, DetailType expectedType);
    QContactDetail& assign(const QContactDetail &other, DetailType expectedType);

private:
    friend class QContact;
    friend class QContactDetailPrivate;
#ifndef QT_NO_DATASTREAM
    Q_CONTACTS_EXPORT friend QDataStream& operator>>(QDataStream& in, QContactDetail& detail);
#endif
    QSharedDataPointer<QContactDetailPrivate> d;
};

Q_CONTACTS_EXPORT uint qHash(const QContactDetail& key);
#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactDetail& detail);
#endif

Q_DECLARE_OPERATORS_FOR_FLAGS(QContactDetail::AccessConstraints)

#define Q_DECLARE_CUSTOM_CONTACT_DETAIL(className) \
    className() : QContactDetail(Type) {} \
    className(const QContactDetail &field) : QContactDetail(field, Type) {} \
    className& operator=(const QContactDetail &other) {assign(other, Type); return *this;} \
    static const DetailType Type;

QTCONTACTS_END_NAMESPACE

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTCONTACTS_PREPEND_NAMESPACE(QContactDetail), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

#endif

