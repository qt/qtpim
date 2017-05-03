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

#ifndef QDECLARATIVECONTACTDETAIL_P_H
#define QDECLARATIVECONTACTDETAIL_P_H

#include <QtCore/qset.h>
#include <QtCore/qstringlist.h>

#include <QtQml/qqml.h>

#include <QtContacts/qcontactdetail.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactDetail : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DetailType type READ detailType NOTIFY detailChanged)
    Q_PROPERTY(QList<int> contexts READ contexts WRITE setContexts NOTIFY detailChanged)
    Q_PROPERTY(QString detailUri READ detailUri WRITE setDetailUri NOTIFY detailChanged)
    Q_PROPERTY(QStringList linkedDetailUris READ linkedDetailUris WRITE setLinkedDetailUris NOTIFY detailChanged)
    Q_PROPERTY(QList<int> fields READ fields NOTIFY detailChanged)
    Q_PROPERTY(bool readOnly READ readOnly NOTIFY detailChanged)
    Q_PROPERTY(bool removable READ removable NOTIFY detailChanged)

    Q_ENUMS(DetailType)
    Q_ENUMS(ContextField)
public:
    QDeclarativeContactDetail(QObject* parent = Q_NULLPTR);


    enum DetailType {
        Address = QContactDetail::TypeAddress,
        Anniversary = QContactDetail::TypeAnniversary,
        Avatar = QContactDetail::TypeAvatar,
        Birthday = QContactDetail::TypeBirthday,
        DisplayLabel = QContactDetail::TypeDisplayLabel,
        Email = QContactDetail::TypeEmailAddress,
        ExtendedDetail = QContactDetail::TypeExtendedDetail,
        Family = QContactDetail::TypeFamily,
        Favorite = QContactDetail::TypeFavorite,
        Gender = QContactDetail::TypeGender,
        Geolocation = QContactDetail::TypeGeoLocation,
        GlobalPresence = QContactDetail::TypeGlobalPresence,
        Guid = QContactDetail::TypeGuid,
        Hobby = QContactDetail::TypeHobby,
        Name = QContactDetail::TypeName,
        NickName = QContactDetail::TypeNickname,
        Note = QContactDetail::TypeNote,
        OnlineAccount = QContactDetail::TypeOnlineAccount,
        Organization = QContactDetail::TypeOrganization,
        PhoneNumber = QContactDetail::TypePhoneNumber,
        Presence = QContactDetail::TypePresence,
        Ringtone = QContactDetail::TypeRingtone,
        SyncTarget = QContactDetail::TypeSyncTarget,
        Tag = QContactDetail::TypeTag,
        Timestamp = QContactDetail::TypeTimestamp,
        Type = QContactDetail::TypeType,
        Url = QContactDetail::TypeUrl,
        Version = QContactDetail::TypeVersion,
        Unknown = QContactDetail::TypeUndefined
    };

    enum ContextField {
        FieldContext = QContactDetail::FieldContext,
        ContextHome = QContactDetail::ContextHome,
        ContextWork = QContactDetail::ContextWork,
        ContextOther = QContactDetail::ContextOther
    };

    ~QDeclarativeContactDetail();

    // QML functions
    Q_INVOKABLE QVariant value(int field) const;
    Q_INVOKABLE bool setValue(int field, const QVariant& value);
    Q_INVOKABLE bool removeValue(int field);

    QContactDetail& detail();
    const QContactDetail& detail() const;
    void setDetail(const QContactDetail& detail);

    bool readOnly() const;
    bool removable() const;

    QList<int> contexts() const;
    void setContexts(const QList<int>& context);

    QString detailUri() const;
    void setDetailUri(const QString& detailUri);

    QStringList linkedDetailUris() const;
    void setLinkedDetailUris(const QStringList& linkedDetailUris);
    virtual DetailType detailType() const;

    QList<int> fields() const;
Q_SIGNALS:
    void detailChanged();

protected:
    QContactDetail m_detail;
};

class QDeclarativeContactDetailFactory
{
public:
    static QDeclarativeContactDetail *createContactDetail(const QDeclarativeContactDetail::DetailType type);
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactDetail)

#endif // QDECLARATIVECONTACTDETAIL_P_H
