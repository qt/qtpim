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

#ifndef QCONTACTACTIONTARGET_H
#define QCONTACTACTIONTARGET_H

#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <QtContacts/qcontact.h>
#include <QtContacts/qcontactdetail.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContact;

class QContactActionTargetPrivate;
class Q_CONTACTS_EXPORT QContactActionTarget
{
public:
    explicit QContactActionTarget(const QContact& contact = QContact(), const QList<QContactDetail>& details = QList<QContactDetail>());
    QContactActionTarget(const QContact& contact, const QContactDetail& detail);

    QContactActionTarget(const QContactActionTarget& other);

    QContactActionTarget& operator=(const QContactActionTarget& other);
    ~QContactActionTarget();

    enum Type {
        Invalid, // no contact
        WholeContact, // no details
        SingleDetail, // a single detail
        MultipleDetails
    };

    Type type() const; // No setter
    bool isValid() const;
    bool operator==(const QContactActionTarget& other) const;
    bool operator!=(const QContactActionTarget& other) const;

    void setContact(const QContact& contact);
    void setDetails(const QList<QContactDetail>& details);

    QContact contact() const;
    QList<QContactDetail> details() const;

private:
#ifndef QT_NO_DATASTREAM
    Q_CONTACTS_EXPORT friend QDataStream& operator<<(QDataStream& out, const QContactActionTarget& target);
    Q_CONTACTS_EXPORT friend QDataStream& operator>>(QDataStream& in, QContactActionTarget& target);
#endif
    QSharedDataPointer<QContactActionTargetPrivate> d;
};

Q_CONTACTS_EXPORT size_t qHash(const QContactActionTarget& key);
#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactActionTarget& target);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactActionTarget& target);
#endif
#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug& operator<<(QDebug dbg, const QContactActionTarget& target);
#endif

QT_END_NAMESPACE_CONTACTS

QT_BEGIN_NAMESPACE
Q_DECLARE_TYPEINFO(QTCONTACTS_PREPEND_NAMESPACE(QContactActionTarget), Q_MOVABLE_TYPE);
QT_END_NAMESPACE

#endif // QCONTACTACTIONTARGET_H
