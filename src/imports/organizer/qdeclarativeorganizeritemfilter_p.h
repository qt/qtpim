/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the plugins of the Qt Toolkit.
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

#ifndef QDECLARATIVEORGANIZERITEMFILTER_P_H
#define QDECLARATIVEORGANIZERITEMFILTER_P_H

#include "qdeclarativeorganizeritemdetail_p.h"
#include <qorganizeritemfilters.h>

QTORGANIZER_BEGIN_NAMESPACE

class QDeclarativeOrganizerItemFilter : public QObject
{
    Q_OBJECT

    Q_ENUMS(FilterType)
    Q_FLAGS(MatchFlags)
    Q_PROPERTY(FilterType type READ type NOTIFY filterChanged)

public:
    QDeclarativeOrganizerItemFilter(QObject *parent = 0);

    enum FilterType {
        DefaultFilter = QOrganizerItemFilter::DefaultFilter,
        InvalidFilter = QOrganizerItemFilter::InvalidFilter,
        IntersectionFilter = QOrganizerItemFilter::IntersectionFilter,
        UnionFilter = QOrganizerItemFilter::UnionFilter,
        CollectionFilter = QOrganizerItemFilter::CollectionFilter,
        DetailFilter = QOrganizerItemFilter::DetailFilter,
        DetailRangeFilter = QOrganizerItemFilter::DetailRangeFilter,
        IdFilter = QOrganizerItemFilter::IdFilter
    };

    FilterType type() const;

    enum MatchFlag {
        MatchExactly = QOrganizerItemFilter::MatchExactly,
        MatchContains = QOrganizerItemFilter::MatchContains,
        MatchStartsWith = QOrganizerItemFilter::MatchStartsWith,
        MatchEndsWith = QOrganizerItemFilter::MatchEndsWith,
        MatchFixedString = QOrganizerItemFilter::MatchFixedString,
        MatchCaseSensitive = QOrganizerItemFilter::MatchCaseSensitive
    };
    Q_DECLARE_FLAGS(MatchFlags, MatchFlag)

    // used by model
    virtual QOrganizerItemFilter filter() const;

Q_SIGNALS:
    void filterChanged();
};


class QDeclarativeOrganizerItemInvalidFilter : public QDeclarativeOrganizerItemFilter
{
    Q_OBJECT

public:
    QDeclarativeOrganizerItemInvalidFilter(QObject *parent = 0);

    // used by model
    QOrganizerItemFilter filter() const;
};


class QDeclarativeOrganizerItemCompoundFilter : public QDeclarativeOrganizerItemFilter
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> filters READ filters NOTIFY valueChanged)
    Q_CLASSINFO("DefaultProperty", "filters")

public:
    explicit QDeclarativeOrganizerItemCompoundFilter(QObject *parent = 0);
    virtual ~QDeclarativeOrganizerItemCompoundFilter();

    QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> filters();

    static void filters_append(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> *prop, QDeclarativeOrganizerItemFilter *filter);
    static int filters_count(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> *prop);
    static QDeclarativeOrganizerItemFilter *filters_at(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> *prop, int index);
    static void filters_clear(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> *prop);

Q_SIGNALS:
    void valueChanged();

protected:
    QList<QDeclarativeOrganizerItemFilter *> m_filters;
};


class QDeclarativeOrganizerItemIntersectionFilter : public QDeclarativeOrganizerItemCompoundFilter
{
    Q_OBJECT

public:
    QDeclarativeOrganizerItemIntersectionFilter(QObject *parent = 0);

    // used by model
    QOrganizerItemFilter filter() const;
};


class QDeclarativeOrganizerItemUnionFilter : public QDeclarativeOrganizerItemCompoundFilter
{
    Q_OBJECT
public:
    QDeclarativeOrganizerItemUnionFilter(QObject *parent = 0);

    // used by model
    QOrganizerItemFilter filter() const;
};


class QDeclarativeOrganizerItemCollectionFilter : public QDeclarativeOrganizerItemFilter
{
    Q_OBJECT
    Q_PROPERTY(QStringList ids READ ids WRITE setIds NOTIFY valueChanged)

public:
    QDeclarativeOrganizerItemCollectionFilter(QObject *parent = 0);

    QStringList ids() const;
    void setIds(const QStringList &ids);

    // used by model
    QOrganizerItemFilter filter() const;

Q_SIGNALS:
    void valueChanged();

private:
    QStringList m_ids;
};


class QDeclarativeOrganizerItemDetailFilter : public QDeclarativeOrganizerItemFilter, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemFilter::MatchFlags matchFlags READ matchFlags WRITE setMatchFlags NOTIFY valueChanged)
    Q_PROPERTY(int field READ field WRITE setField NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemDetail::DetailType detail READ detail WRITE setDetail NOTIFY valueChanged)
    Q_INTERFACES(QDeclarativeParserStatus)

public:
    QDeclarativeOrganizerItemDetailFilter(QObject *parent = 0);

    // inherited from QDeclarativeParserStatus
    void classBegin();
    void componentComplete();

    QDeclarativeOrganizerItemDetail::DetailType detail() const;
    void setDetail(QDeclarativeOrganizerItemDetail::DetailType detail);

    int field() const;
    void setField(int field);

    void setValue(const QVariant &value);
    QVariant value() const;

    QDeclarativeOrganizerItemFilter::MatchFlags matchFlags() const;
    void setMatchFlags(QDeclarativeOrganizerItemFilter::MatchFlags flags);

    // used by model
    QOrganizerItemFilter filter() const;

signals:
    void valueChanged();

private:
    void setDetail();

    QDeclarativeOrganizerItemDetail::DetailType m_detail;
    int m_field;
    bool m_componentCompleted;
    QOrganizerItemDetailFilter d;
};


class QDeclarativeOrganizerItemDetailRangeFilter : public QDeclarativeOrganizerItemFilter, public QDeclarativeParserStatus
{
    Q_OBJECT
    Q_ENUMS(RangeFlag)
    Q_FLAGS(RangeFlags)
    Q_PROPERTY(QVariant min READ minValue WRITE setMinValue NOTIFY valueChanged)
    Q_PROPERTY(QVariant max READ maxValue WRITE setMaxValue NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemFilter::MatchFlags matchFlags READ matchFlags WRITE setMatchFlags NOTIFY valueChanged)
    Q_PROPERTY(RangeFlags rangeFlags READ rangeFlags WRITE setRangeFlags NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemDetail::DetailType detail READ detail WRITE setDetail NOTIFY valueChanged)
    Q_PROPERTY(int field READ field WRITE setField NOTIFY valueChanged)
    Q_INTERFACES(QDeclarativeParserStatus)

public:
    enum RangeFlag {
        IncludeLower = QOrganizerItemDetailRangeFilter::IncludeLower,
        IncludeUpper = QOrganizerItemDetailRangeFilter::IncludeUpper,
        ExcludeLower = QOrganizerItemDetailRangeFilter::ExcludeLower,
        ExcludeUpper = QOrganizerItemDetailRangeFilter::ExcludeUpper
    };
    Q_DECLARE_FLAGS(RangeFlags, RangeFlag)

    QDeclarativeOrganizerItemDetailRangeFilter(QObject *parent = 0);

    // inherited from QDeclarativeParserStatus
    void classBegin();
    void componentComplete();

    QDeclarativeOrganizerItemDetail::DetailType detail() const;
    void setDetail(QDeclarativeOrganizerItemDetail::DetailType detail);

    int field() const;
    void setField(int field);

    QDeclarativeOrganizerItemFilter::MatchFlags matchFlags() const;
    void setMatchFlags(QDeclarativeOrganizerItemFilter::MatchFlags flags);

    RangeFlags rangeFlags() const;
    void setRangeFlags(RangeFlags flags);

    QVariant minValue() const;
    void setMinValue(const QVariant &value);

    QVariant maxValue() const;
    void setMaxValue(const QVariant &value);

    // used by model
    QOrganizerItemFilter filter() const;

signals:
    void valueChanged();

private:
    void setDetail();

    QDeclarativeOrganizerItemDetail::DetailType m_detail;
    int m_field;
    bool m_componentCompleted;
    QOrganizerItemDetailRangeFilter d;

};


class QDeclarativeOrganizerItemIdFilter : public QDeclarativeOrganizerItemFilter
{
    Q_OBJECT
    Q_PROPERTY(QStringList ids READ ids WRITE setIds NOTIFY valueChanged)

public:
    QDeclarativeOrganizerItemIdFilter(QObject *parent = 0);

    QStringList ids() const;
    void setIds(const QStringList &ids);

    // used by model
    QOrganizerItemFilter filter() const;

Q_SIGNALS:
    void valueChanged();

private:
    QStringList m_ids;
};

QTORGANIZER_END_NAMESPACE

QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemInvalidFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemCompoundFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemIntersectionFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemUnionFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemCollectionFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemDetailFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemDetailRangeFilter))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemIdFilter))

#endif // QDECLARATIVEORGANIZERITEMFILTER_P_H
