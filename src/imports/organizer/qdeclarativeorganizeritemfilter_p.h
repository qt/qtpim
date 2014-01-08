/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
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

#ifndef QDECLARATIVEORGANIZERITEMFILTER_P_H
#define QDECLARATIVEORGANIZERITEMFILTER_P_H

#include <QtQml/qqml.h>

#include <QtOrganizer/qorganizeritemfilters.h>

#include "qdeclarativeorganizeritemdetail_p.h"

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

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
        DetailFieldFilter = QOrganizerItemFilter::DetailFieldFilter,
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
    Q_PROPERTY(QQmlListProperty<QDeclarativeOrganizerItemFilter> filters READ filters NOTIFY valueChanged)
    Q_CLASSINFO("DefaultProperty", "filters")

public:
    explicit QDeclarativeOrganizerItemCompoundFilter(QObject *parent = 0);
    virtual ~QDeclarativeOrganizerItemCompoundFilter();

    QQmlListProperty<QDeclarativeOrganizerItemFilter> filters();

    static void filters_append(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop, QDeclarativeOrganizerItemFilter *filter);
    static int filters_count(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop);
    static QDeclarativeOrganizerItemFilter *filters_at(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop, int index);
    static void filters_clear(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop);

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


class QDeclarativeOrganizerItemDetailFilter : public QDeclarativeOrganizerItemFilter, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeOrganizerItemDetail *detail READ detail WRITE setDetail NOTIFY valueChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    QDeclarativeOrganizerItemDetailFilter(QObject *parent = 0);

    // inherited from QQmlParserStatus
    void classBegin();
    void componentComplete();

    QDeclarativeOrganizerItemDetail *detail() const;
    void setDetail(QDeclarativeOrganizerItemDetail *detail);

    // used by model
    QOrganizerItemFilter filter() const;

signals:
    void valueChanged();

private:
    void setDetail();

    QDeclarativeOrganizerItemDetail *m_detail;
    bool m_componentCompleted;
    QOrganizerItemDetailFilter d;
};


class QDeclarativeOrganizerItemDetailFieldFilter : public QDeclarativeOrganizerItemFilter, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemFilter::MatchFlags matchFlags READ matchFlags WRITE setMatchFlags NOTIFY valueChanged)
    Q_PROPERTY(int field READ field WRITE setField NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemDetail::DetailType detail READ detail WRITE setDetail NOTIFY valueChanged)
    Q_INTERFACES(QQmlParserStatus)

public:
    QDeclarativeOrganizerItemDetailFieldFilter(QObject *parent = 0);

    // inherited from QQmlParserStatus
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
    QOrganizerItemDetailFieldFilter d;
};


class QDeclarativeOrganizerItemDetailRangeFilter : public QDeclarativeOrganizerItemFilter, public QQmlParserStatus
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
    Q_INTERFACES(QQmlParserStatus)

public:
    enum RangeFlag {
        IncludeLower = QOrganizerItemDetailRangeFilter::IncludeLower,
        IncludeUpper = QOrganizerItemDetailRangeFilter::IncludeUpper,
        ExcludeLower = QOrganizerItemDetailRangeFilter::ExcludeLower,
        ExcludeUpper = QOrganizerItemDetailRangeFilter::ExcludeUpper
    };
    Q_DECLARE_FLAGS(RangeFlags, RangeFlag)

    QDeclarativeOrganizerItemDetailRangeFilter(QObject *parent = 0);

    // inherited from QQmlParserStatus
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

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeOrganizerItemFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemInvalidFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemCompoundFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemIntersectionFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemUnionFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemCollectionFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemDetailFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemDetailFieldFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemDetailRangeFilter)
QML_DECLARE_TYPE(QDeclarativeOrganizerItemIdFilter)

#endif // QDECLARATIVEORGANIZERITEMFILTER_P_H
