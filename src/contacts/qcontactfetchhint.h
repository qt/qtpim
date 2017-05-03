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

#ifndef QCONTACTFETCHHINT_H
#define QCONTACTFETCHHINT_H

#include <QtCore/qshareddata.h>
#include <QtCore/qsize.h>
#include <QtCore/qstringlist.h>

#include <QtContacts/qcontactdetail.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactFetchHintPrivate;
class Q_CONTACTS_EXPORT QContactFetchHint {
public:
    QContactFetchHint();
    QContactFetchHint(const QContactFetchHint& other);
    ~QContactFetchHint();
    QContactFetchHint& operator=(const QContactFetchHint& other);

    QList<QContactDetail::DetailType> detailTypesHint() const;
    void setDetailTypesHint(const QList<QContactDetail::DetailType> &types);

    QStringList relationshipTypesHint() const;
    void setRelationshipTypesHint(const QStringList& relationshipTypes);

    QSize preferredImageSize() const;
    void setPreferredImageSize(const QSize& size);

    enum OptimizationHint {
        AllRequired = 0x0,
        NoRelationships = 0x1,
        NoActionPreferences = 0x2,
        NoBinaryBlobs = 0x4
        // any other optimization hints?
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    int maxCountHint() const;
    void setMaxCountHint(int count);

private:
    QSharedDataPointer<QContactFetchHintPrivate> d;
};

#ifndef QT_NO_DATASTREAM
Q_CONTACTS_EXPORT QDataStream& operator<<(QDataStream& out, const QContactFetchHint& hint);
Q_CONTACTS_EXPORT QDataStream& operator>>(QDataStream& in, QContactFetchHint& hint);
#endif

#ifndef QT_NO_DEBUG_STREAM
Q_CONTACTS_EXPORT QDebug operator<<(QDebug dbg, const QContactFetchHint& hint);
#endif

Q_DECLARE_OPERATORS_FOR_FLAGS(QContactFetchHint::OptimizationHints)

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTFETCHHINT_H
