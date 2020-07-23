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

#ifndef QCONTACT_H
#define QCONTACT_H

#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qshareddata.h>
#include <QtCore/qstring.h>

#include <QtContacts/qcontactdetail.h>
#include <QtContacts/qcontactrelationship.h>
#include <QtContacts/qcontacttype.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactActionDescriptor;
class QContactId;
class QContactManager;
class QContactCollectionId;

class QContactData;
class Q_CONTACTS_EXPORT QContact
{
public:
    enum AccessConstraintsEnforcement {
        ReplaceAccessConstraints,
        IgnoreAccessConstraints,
        EnforceAccessConstraints,
    };

    QContact();

    ~QContact();

    QContact(const QContact& other);
    QContact& operator=(const QContact& other);

    bool operator==(const QContact &other) const;
    bool operator!=(const QContact &other) const {return !(other == *this);}

    /* Unique ID */
    QContactId id() const;
    void setId(const QContactId& id);

    /* Type - contact, group, metacontact, ... */
    QContactType::TypeValues type() const;
    void setType(const QContactType::TypeValues& type);

    QStringList tags() const;
    void clearTags();
    void addTag(const QString& tag);
    void setTags(const QStringList& tags);

    /* Is this an empty contact? */
    bool isEmpty() const;
    void clearDetails();

    /* Access details of particular type */
    QContactDetail detail(QContactDetail::DetailType type) const;
    QList<QContactDetail> details(QContactDetail::DetailType type = QContactDetail::TypeUndefined) const;
    bool appendDetail(const QContactDetail &detail);
    /* Templated (type-specific) detail retrieval */
    template<typename T> QList<T> details() const
    {
        QList<QContactDetail> props = details(T::Type);
        QList<T> ret;
        for (int i=0; i<props.count(); i++)
            ret.append(T(props.at(i)));
        return ret;
    }

    template<typename T> T detail() const
    {
        return T(detail(T::Type));
    }

    /* generic detail addition/removal functions */
    bool saveDetail(QContactDetail* detail, AccessConstraintsEnforcement enforce = EnforceAccessConstraints);
    bool removeDetail(QContactDetail* detail, AccessConstraintsEnforcement enforce = EnforceAccessConstraints);

    /* Relationships that this contact was involved in when it was retrieved from the manager */
    QList<QContactRelationship> relationships(const QString& relationshipType = QString()) const;
    QList<QContactId> relatedContacts(const QString& relationshipType = QString(), QContactRelationship::Role role = QContactRelationship::Either) const;

    QContactCollectionId collectionId() const;
    void setCollectionId(const QContactCollectionId &collectionId);

    /* Actions available to be performed on this contact */
    QList<QContactActionDescriptor> availableActions(const QString& serviceName = QString()) const;

    /* Preferences (eg, set a particular detail preferred for the SMS action) - subject to change! */
    bool setPreferredDetail(const QString& actionName, const QContactDetail& preferredDetail);
    bool isPreferredDetail(const QString& actionName, const QContactDetail& detail) const;
    QContactDetail preferredDetail(const QString& actionName) const;
    QMap<QString, QContactDetail> preferredDetails() const;

private:
    friend class QContactData;
    friend class QContactManager;
    friend class QContactManagerData;
    friend class QContactManagerEngine;
    Q_CONTACTS_EXPORT friend QDataStream& operator<<(QDataStream& out, const QContact& contact);
    Q_CONTACTS_EXPORT friend QDataStream& operator>>(QDataStream& in, QContact& contact);

    QSharedDataPointer<QContactData> d;
};

Q_CONTACTS_EXPORT size_t qHash(const QContact& key);
#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContact& contact);
#endif
#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContact& contact);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContact& contact);
#endif

QT_END_NAMESPACE_CONTACTS

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTCONTACTS_PREPEND_NAMESPACE(QContact), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

#endif // QCONTACT_H
