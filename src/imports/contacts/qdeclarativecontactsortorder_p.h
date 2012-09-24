/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
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
#ifndef QDECLARATIVECONTACTSORTORDER_P_H
#define QDECLARATIVECONTACTSORTORDER_P_H

#include <qqml.h>
#include <QQmlExtensionPlugin>
#include <QQmlParserStatus>

#include "qcontactsortorder.h"
#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactSortOrder :public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(int detail READ detail WRITE setDetail NOTIFY sortOrderChanged)
    Q_PROPERTY(int field READ field WRITE setField NOTIFY sortOrderChanged)
    Q_PROPERTY(Qt::SortOrder direction READ direction WRITE setDirection NOTIFY sortOrderChanged)
    Q_PROPERTY(BlankPolicy blankPolicy READ blankPolicy WRITE setBlankPolicy NOTIFY sortOrderChanged)
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity NOTIFY sortOrderChanged)
    Q_ENUMS(BlankPolicy)
    Q_INTERFACES(QQmlParserStatus)
public:

    enum BlankPolicy {
        BlanksFirst = QContactSortOrder::BlanksFirst,
        BlanksLast = QContactSortOrder::BlanksLast
    };


    QDeclarativeContactSortOrder(QObject* parent = 0);

    void setDetail(const int detailType);

    int detail() const;

    void setField(const int fieldType);

    int field() const;

    //from QQmlParserStatus
    void classBegin() {}
    void componentComplete();

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
    int m_field;
    int m_detail;
    QContactSortOrder m_sortOrder;
};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactSortOrder))

#endif
