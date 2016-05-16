/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVECONTACTFILTER_P_H
#define QDECLARATIVECONTACTFILTER_P_H

#include <QtQml/qqml.h>

#include <QtContacts/qcontactfilter.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FilterType type READ type NOTIFY filterChanged)
    Q_ENUMS(FilterType)
    Q_FLAGS(MatchFlags)
public:
    QDeclarativeContactFilter(QObject *parent=0)
        :QObject(parent)
    {
        //for grouped filter: intersect /union filters
        if (parent && qobject_cast<QDeclarativeContactFilter*>(parent)) {
            connect(this, SIGNAL(filterChanged()), parent, SIGNAL(filterChanged()));
        }
    }

    enum FilterType {
        InvalidFilter = QContactFilter::InvalidFilter,
        DetailFilter = QContactFilter::ContactDetailFilter,
        DetailRangeFilter = QContactFilter::ContactDetailRangeFilter,
        ChangeLogFilter = QContactFilter::ChangeLogFilter,
        ActionFilter = QContactFilter::ActionFilter,
        RelationshipFilter = QContactFilter::RelationshipFilter,
        IntersectionFilter = QContactFilter::IntersectionFilter,
        UnionFilter = QContactFilter::UnionFilter,
        IdFilter = QContactFilter::IdFilter,
        DefaultFilter = QContactFilter::DefaultFilter
    };

    FilterType type() const {
        return static_cast<FilterType>(filter().type());
    }

    enum MatchFlag {
        MatchExactly = QContactFilter::MatchExactly,
        MatchContains = QContactFilter::MatchContains,
        MatchStartsWith = QContactFilter::MatchStartsWith,
        MatchEndsWith = QContactFilter::MatchEndsWith,
        MatchFixedString = QContactFilter::MatchFixedString,
        MatchCaseSensitive = QContactFilter::MatchCaseSensitive,
        MatchPhoneNumber = QContactFilter::MatchPhoneNumber,
        MatchKeypadCollation = QContactFilter::MatchKeypadCollation
    };
    Q_DECLARE_FLAGS(MatchFlags, MatchFlag)

    virtual QContactFilter filter() const
    {
        return QContactFilter();
    }

signals:
    void filterChanged();
};

class QDeclarativeContactCompoundFilter : public QDeclarativeContactFilter
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QDeclarativeContactFilter> filters READ filters)
    Q_CLASSINFO("DefaultProperty", "filters")

public:
    explicit QDeclarativeContactCompoundFilter(QObject* parent = Q_NULLPTR) : QDeclarativeContactFilter(parent){}
    virtual ~QDeclarativeContactCompoundFilter() {}
    // 'READ' accessor for the filters, basically this is also a 'WRITE' accessor
    // as per QQmlListProperty's design.
    QQmlListProperty<QDeclarativeContactFilter> filters();

    static void filters_append(QQmlListProperty<QDeclarativeContactFilter>* prop, QDeclarativeContactFilter* filter);
    static int filters_count(QQmlListProperty<QDeclarativeContactFilter>* prop);
    static QDeclarativeContactFilter* filters_at(QQmlListProperty<QDeclarativeContactFilter>* prop, int index);
    static void filters_clear(QQmlListProperty<QDeclarativeContactFilter>* prop);

protected:
    QList<QDeclarativeContactFilter*> m_filters;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactFilter)
QML_DECLARE_TYPE(QDeclarativeContactCompoundFilter)

#endif // QDECLARATIVECONTACTFILTER_P_H
