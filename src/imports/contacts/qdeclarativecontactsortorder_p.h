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

#ifndef QDECLARATIVECONTACTSORTORDER_P_H
#define QDECLARATIVECONTACTSORTORDER_P_H

#include <QtQml/qqml.h>

#include <QtContacts/qcontactsortorder.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactSortOrder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int detail READ detail WRITE setDetail NOTIFY sortOrderChanged)
    Q_PROPERTY(int field READ field WRITE setField NOTIFY sortOrderChanged)
    Q_PROPERTY(Qt::SortOrder direction READ direction WRITE setDirection NOTIFY sortOrderChanged)
    Q_PROPERTY(BlankPolicy blankPolicy READ blankPolicy WRITE setBlankPolicy NOTIFY sortOrderChanged)
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity NOTIFY sortOrderChanged)
    Q_ENUMS(BlankPolicy)
public:

    enum BlankPolicy {
        BlanksFirst = QContactSortOrder::BlanksFirst,
        BlanksLast = QContactSortOrder::BlanksLast
    };


    QDeclarativeContactSortOrder(QObject* parent = Q_NULLPTR);

    void setDetail(const int detailType);

    int detail() const;

    void setField(const int fieldType);

    int field() const;

    BlankPolicy blankPolicy() const;
    void setBlankPolicy(BlankPolicy blankPolicy);
    Qt::SortOrder direction() const;
    void setDirection(Qt::SortOrder direction);
    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity sensitivity);
    QContactSortOrder sortOrder();
    void setSortOrder(const QContactSortOrder& sortOrder);

signals:
    void sortOrderChanged();
private:
    QContactSortOrder m_sortOrder;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactSortOrder)

#endif // QDECLARATIVECONTACTSORTORDER_P_H
