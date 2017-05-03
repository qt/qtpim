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

#ifndef QCONTACTPHONENUMBER_H
#define QCONTACTPHONENUMBER_H

#include <QtContacts/qcontactdetail.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactFilter;

/* Leaf class */
//! [0]
class Q_CONTACTS_EXPORT QContactPhoneNumber : public QContactDetail
{
public:
#ifndef Q_QDOC
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactPhoneNumber)
#else
    static const DetailType Type;
#endif

    enum PhoneNumberField {
        FieldNumber = 0,
        FieldNormalizedNumber,
        FieldSubTypes
    };

    enum SubType {
        SubTypeLandline = 0,
        SubTypeMobile,
        SubTypeFax,
        SubTypePager,
        SubTypeVoice,
        SubTypeModem,
        SubTypeVideo,
        SubTypeCar,
        SubTypeBulletinBoardSystem,
        SubTypeMessagingCapable,
        SubTypeAssistant,
        SubTypeDtmfMenu,
        SubTypeIsdn
    };

    void setNumber(const QString &_number);
    QString number() const;

    void setNormalizedNumber(const QString &_normalizedNumber);
    QString normalizedNumber() const;

    void setSubTypes(const QList<int> &_subTypes);
    QList<int> subTypes() const;

    // Convenience filter
    static QContactFilter match(const QString& _number);
};
//! [0]

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTPHONENUMBER_H
