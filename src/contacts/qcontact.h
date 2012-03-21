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

#ifndef QCONTACT_H
#define QCONTACT_H

#include <QVariant>
#include <QString>
#include <QPair>
#include <QMultiMap>
#include <QList>
#include <QDateTime>
#include <QSharedDataPointer>

#include <qcontactsglobal.h>
#include <qcontactdetail.h>
#include <qcontactid.h>
#include <qcontactrelationship.h>

#include <qcontactdisplaylabel.h>
#include <qcontactrelationshipfilter.h>
#include <qcontacttype.h>

QT_BEGIN_NAMESPACE
class QDataStream;
QT_END_NAMESPACE

QTCONTACTS_BEGIN_NAMESPACE

class QContactManager;
class QContactData;
class QContactName;
class QContactAction;
class QContactActionDescriptor;

// MSVC needs the function declared before the friend declaration
class QContact;
#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContact& contact);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContact& contact);
#endif

class Q_CONTACTS_EXPORT QContact
{
public:
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
    bool saveDetail(QContactDetail* detail);
    bool removeDetail(QContactDetail* detail);

    /* Relationships that this contact was involved in when it was retrieved from the manager */
    QList<QContactRelationship> relationships(const QString& relationshipType = QString()) const;
    QList<QContact> relatedContacts(const QString& relationshipType = QString(), QContactRelationship::Role role = QContactRelationship::Either) const;

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

Q_CONTACTS_EXPORT uint qHash(const QContact& key);
#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContact& contact);
#endif

QTCONTACTS_END_NAMESPACE

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTCONTACTS_PREPEND_NAMESPACE(QContact), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

#endif
