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

#ifndef QDECLARATIVEORGANIZERITEMSORTORDER_H
#define QDECLARATIVEORGANIZERITEMSORTORDER_H

#include "qdeclarativeorganizeritemdetail_p.h"
#include <qorganizeritemsortorder.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlparserstatus.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QDeclarativeOrganizerItemSortOrder : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeOrganizerItemDetail::DetailType detail READ detail WRITE setDetail NOTIFY sortOrderChanged)
    Q_PROPERTY(int field READ field WRITE setField NOTIFY sortOrderChanged)
    Q_PROPERTY(BlankPolicy blankPolicy READ blankPolicy WRITE setBlankPolicy NOTIFY sortOrderChanged)
    Q_PROPERTY(Qt::SortOrder direction READ direction WRITE setDirection NOTIFY sortOrderChanged)
    Q_PROPERTY(Qt::CaseSensitivity sensitivity READ caseSensitivity WRITE setCaseSensitivity NOTIFY sortOrderChanged)
    Q_INTERFACES(QQmlParserStatus)
    Q_ENUMS(BlankPolicy)

public:
    enum BlankPolicy {
        BlanksFirst = QOrganizerItemSortOrder::BlanksFirst,
        BlanksLast = QOrganizerItemSortOrder::BlanksLast
    };

    QDeclarativeOrganizerItemSortOrder(QObject *parent = 0);

    void setDetail(QDeclarativeOrganizerItemDetail::DetailType detail);
    QDeclarativeOrganizerItemDetail::DetailType detail() const;

    void setField(int field);
    int field() const;

    void setBlankPolicy(BlankPolicy policy);
    BlankPolicy blankPolicy() const;

    void setDirection(Qt::SortOrder newDirection);
    Qt::SortOrder direction() const;

    void setCaseSensitivity(Qt::CaseSensitivity newSensitivity);
    Qt::CaseSensitivity caseSensitivity() const;

    // from QQmlParserStatus
    void classBegin();
    void componentComplete();

    // used by organizer model
    QOrganizerItemSortOrder sortOrder();

Q_SIGNALS:
    void sortOrderChanged();

private:
    void setDetailDefinitionName();

    QDeclarativeOrganizerItemDetail::DetailType m_detail;
    int m_field;
    bool m_componentCompleted;
    QOrganizerItemSortOrder d;
};

QT_END_NAMESPACE_ORGANIZER

QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemSortOrder))

#endif // QDECLARATIVEORGANIZERITEMSORTORDER_H
