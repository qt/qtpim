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

#include "qdeclarativeorganizeritemfilter_p.h"

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass Filter QDeclarativeOrganizerItemFilter
    \brief The Filter element is used to filter items made available through a backend.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main
    \sa QOrganizerItemFilter
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemFilter::QDeclarativeOrganizerItemFilter(QObject *parent)
    : QObject(parent)
{
    //for grouped filter: intersect /union filters
    if (parent && qobject_cast<QDeclarativeOrganizerItemFilter *>(parent))
        connect(this, SIGNAL(filterChanged()), parent, SIGNAL(filterChanged()));
}

/*!
    \qmlproperty enumeration Filter::type

    This property holds the type value of this filter. It can be one of:
    \list
    \li Filter.DefaultFilter       A filter which matches everything (default).
    \li Filter.InvalidFilter       An invalid filter which matches nothing.
    \li Filter.IntersectionFilter  A filter which matches all organizer items that are matched by all
                                  filters it includes.
    \li Filter.UnionFilter         A filter which matches any organizer item that is matched by any
                                  of the filters it includes.
    \li Filter.CollectionFilter    A filter which matches any organizer item that is matched by collection.
    \li Filter.DetailFilter        A filter which matches organizer items containing one or more details
                                  of a particular type with a particular value.
    \li Filter.DetailRangeFilter   A filter which matches organizer items containing one or more details
                                  of a particular type whose values are within a particular range.
    \li Filter.IdFilter            A filter which matches any organizer item whose ID is contained in
                                  a particular list of organizer item IDs.
    \endlist
 */
QDeclarativeOrganizerItemFilter::FilterType QDeclarativeOrganizerItemFilter::type() const
{
    return static_cast<QDeclarativeOrganizerItemFilter::FilterType>(filter().type());
}

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemFilter::filter() const
{
    return QOrganizerItemFilter();
}


/*!
    \qmlclass InvalidFilter QDeclarativeOrganizerItemInvalidFilter
    \brief the InvalidFilter element provides a filter which will never match any organizer items.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemInvalidFilter
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemInvalidFilter::QDeclarativeOrganizerItemInvalidFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
{
}

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemInvalidFilter::filter() const
{
    return QOrganizerItemInvalidFilter();
}


/*!
    \internal
 */
QDeclarativeOrganizerItemCompoundFilter::QDeclarativeOrganizerItemCompoundFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \internal
 */
QDeclarativeOrganizerItemCompoundFilter::~QDeclarativeOrganizerItemCompoundFilter()
{
}

/*!
    \internal
 */
QQmlListProperty<QDeclarativeOrganizerItemFilter> QDeclarativeOrganizerItemCompoundFilter::filters()
{
    return QQmlListProperty<QDeclarativeOrganizerItemFilter>(this, 0, filters_append, filters_count, filters_at, filters_clear);
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemCompoundFilter::filters_append(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop,
                                                             QDeclarativeOrganizerItemFilter *filter)
{
    QDeclarativeOrganizerItemCompoundFilter *compoundFilter = static_cast<QDeclarativeOrganizerItemCompoundFilter *>(prop->object);
    compoundFilter->m_filters.append(filter);
    QObject::connect(filter, SIGNAL(filterChanged()), compoundFilter, SIGNAL(valueChanged()));
    emit compoundFilter->valueChanged();
}

/*!
    \internal
 */
int QDeclarativeOrganizerItemCompoundFilter::filters_count(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop)
{
    return static_cast<QDeclarativeOrganizerItemCompoundFilter *>(prop->object)->m_filters.count();
}

/*!
    \internal
 */
QDeclarativeOrganizerItemFilter *QDeclarativeOrganizerItemCompoundFilter::filters_at(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop, int index)
{
    return static_cast<QDeclarativeOrganizerItemCompoundFilter *>(prop->object)->m_filters.at(index);
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemCompoundFilter::filters_clear(QQmlListProperty<QDeclarativeOrganizerItemFilter> *prop)
{
    QDeclarativeOrganizerItemCompoundFilter *filter = static_cast<QDeclarativeOrganizerItemCompoundFilter *>(prop->object);
    if (!filter->m_filters.isEmpty()) {
        filter->m_filters.clear();
        emit filter->valueChanged();
    }
}


/*!
    \qmlclass IntersectionFilter QDeclarativeOrganizerItemIntersectionFilter
    \brief The IntersectionFilter element provides a filter which intersects the results of other filters.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemIntersectionFilter
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemIntersectionFilter::QDeclarativeOrganizerItemIntersectionFilter(QObject *parent)
    : QDeclarativeOrganizerItemCompoundFilter(parent)
{
}

/*!
    \qmlproperty list<filter> IntersectionFilter::filters

    This property holds the list of filters which form the intersection filter.
 */

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemIntersectionFilter::filter() const
{
    QList<QOrganizerItemFilter> filters;
    foreach (const QDeclarativeOrganizerItemFilter *filter, m_filters)
        filters << filter->filter();
    QOrganizerItemIntersectionFilter f;
    f.setFilters(filters);
    return f;
}


/*!
    \qmlclass UnionFilter QDeclarativeOrganizerItemUnionFilter
    \brief The UnionFilter element provides a filter which unions the results of other filters.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemUnionFilter
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemUnionFilter::QDeclarativeOrganizerItemUnionFilter(QObject *parent)
    : QDeclarativeOrganizerItemCompoundFilter(parent)
{
}

/*!
    \qmlproperty list<filter> UnionFilter::filters

    This property holds the list of filters which form the union filter.
 */

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemUnionFilter::filter() const
{
    QList<QOrganizerItemFilter> filters;
    foreach (const QDeclarativeOrganizerItemFilter *filter, m_filters)
        filters << filter->filter();
    QOrganizerItemUnionFilter f;
    f.setFilters(filters);
    return f;
}


/*!
    \qmlclass CollectionFilter QDeclarativeOrganizerItemCollectionFilter
    \brief The CollectionFilter element provides a filter based around the collection one organizer
           item belongs to.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa Collection
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemCollectionFilter::QDeclarativeOrganizerItemCollectionFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \qmlproperty list<string> CollectionFilter::ids

    This property holds the list of IDs of organizer collections which the items should belong to.
 */
QStringList QDeclarativeOrganizerItemCollectionFilter::ids() const
{
    return m_ids;
}

void QDeclarativeOrganizerItemCollectionFilter::setIds(const QStringList &ids)
{
    foreach (const QString &id, ids) {
        if (!m_ids.contains(id)) {
            m_ids = ids;
            emit valueChanged();
            return;
        }
    }

    foreach (const QString &id, m_ids) {
        if (!ids.contains(id)) {
            m_ids = ids;
            emit valueChanged();
            return;
        }
    }
}

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemCollectionFilter::filter() const
{
    QOrganizerItemCollectionFilter f;
    QSet<QOrganizerCollectionId> ids;

    foreach (const QVariant &id, m_ids) {
        QOrganizerCollectionId cId = QOrganizerCollectionId::fromString(id.toString());
        if (!cId.isNull())
            ids << cId;
    }

    f.setCollectionIds(ids);
    return f;
}


/*!
    \qmlclass DetailFilter QDeclarativeOrganizerItemDetailFilter
    \brief The DetailFilter element provides a filter based around a detail value criterion.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters

    Simple example how to utilize DetailFilter element together with OrganizerModel and ListView elements:
    \code
    Rectangle {
        height: 400; width: 400;

        OrganizerModel{
            id: organizer
            startPeriod: "2009-01-01"
            endPeriod: "2012-12-31"
            filter: todoFilter
        }

        DetailFilter {
            id: todoFilter
            detail: Detail.Type
            field: Type.FieldType
            value: Type.Todo
        }

        ListView {
            width: parent.width; height: parent.height;
            model: organizer.items
            delegate: Text {text: displayLabel}
        }
    }
    \endcode

    \sa QOrganizerItemDetailFilter
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemDetailFilter::QDeclarativeOrganizerItemDetailFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
    , m_detail(QDeclarativeOrganizerItemDetail::Undefined)
    , m_field(-1)
    , m_componentCompleted(false)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetailFilter::classBegin()
{
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetailFilter::componentComplete()
{
    setDetail();
    m_componentCompleted = true;
}

/*!
    \qmlproperty enum DetailFilter::detail

    This property holds the detail type of which the detail filter will be matched to. The value
    shuold be the enumeration value of Detail::type.
  */
QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDetailFilter::detail() const
{
    return m_detail;
}

void QDeclarativeOrganizerItemDetailFilter::setDetail(QDeclarativeOrganizerItemDetail::DetailType detail)
{
    if (m_detail != detail) {
        m_detail = detail;
        if (m_componentCompleted)
            setDetail();
    }
}

/*!
    \qmlproperty enum DetailFilter::field

    This property holds the detail field type of which the detail filter will be matched to. The
    value should be the filld enumeration value defined in each detail element.

    \sa EventTime, JournalTime, TodoTime, TodoProgress, Reminder, AudibleReminder, VisualReminder,
        EmailReminder, Comment, Description, DisplayLabel, Guid, Location, Parent, Priority, Recurrence,
        Timestamp, Type, Tag
  */
int QDeclarativeOrganizerItemDetailFilter::field() const
{
    return m_field;
}

void QDeclarativeOrganizerItemDetailFilter::setField(int field)
{
    if (field != m_field) {
        m_field = field;
        if (m_componentCompleted)
            setDetail();
    }
}

/*!
  \qmlproperty variant DetailFilter::value

  This property holds the value criterion of the detail filter.
  */
QVariant QDeclarativeOrganizerItemDetailFilter::value() const
{
    // UTC time is used with details internally
    if (QVariant::DateTime == d.value().type())
        return d.value().toDateTime().toLocalTime();
    else
        return d.value();
}

void QDeclarativeOrganizerItemDetailFilter::setValue(const QVariant &newValue)
{
     if (newValue != value()) {
        // UTC time is used with details internally
        if (QVariant::DateTime == newValue.type())
            d.setValue(newValue.toDateTime().toUTC());
        else
            d.setValue(newValue);
        emit valueChanged();
    }
}

/*!
  \qmlproperty enumeration DetailFilter::matchFlags

  This property holds the semantics of the value matching criterion. The valid match flags include:
  \list
  \li MatchExactly - Performs QVariant-based matching (default).
  \li MatchContains - The search term is contained in the item.
  \li MatchStartsWith - The search term matches the start of the item.
  \li MatchEndsWith - The search term matches the end of the item.
  \li MatchFixedString - Performs string-based matching. String-based comparisons are case-insensitive unless the \c MatchCaseSensitive flag is also specified.
  \li MatchCaseSensitive - The search is case sensitive.
  \endlist
 */
QDeclarativeOrganizerItemFilter::MatchFlags QDeclarativeOrganizerItemDetailFilter::matchFlags() const
{
    QDeclarativeOrganizerItemFilter::MatchFlags newFlags;
    newFlags = ~newFlags & (int)d.matchFlags();
    return newFlags;
}

void QDeclarativeOrganizerItemDetailFilter::setMatchFlags(QDeclarativeOrganizerItemFilter::MatchFlags flags)
{
    QOrganizerItemFilter::MatchFlags newFlags;
    newFlags = ~newFlags & (int)flags;
    if (newFlags != d.matchFlags()) {
        d.setMatchFlags(newFlags);
        emit valueChanged();
    }
}

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemDetailFilter::filter() const
{
    return d;
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetailFilter::setDetail()
{
    d.setDetail(static_cast<QOrganizerItemDetail::DetailType>(m_detail), m_field);
    emit valueChanged();
}


/*!
    \qmlclass DetailRangeFilter QDeclarativeOrganizerItemDetailRangeFilter
    \brief The DetailRangeFilter element provides a filter based around a detail value range criterion.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemDetailRangeFilter
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemDetailRangeFilter::QDeclarativeOrganizerItemDetailRangeFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
    , m_detail(QDeclarativeOrganizerItemDetail::Undefined)
    , m_field(-1)
    , m_componentCompleted(false)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetailRangeFilter::classBegin()
{
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetailRangeFilter::componentComplete()
{
    setDetail();
    m_componentCompleted = true;
}

/*!
    \qmlproperty enum DetailRangeFilter::detail

    This property holds the detail type of which the detail filter will be matched to. The value
    shuold be the enumeration value of Detail::type.
  */
QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDetailRangeFilter::detail() const
{
    return m_detail;
}

void QDeclarativeOrganizerItemDetailRangeFilter::setDetail(QDeclarativeOrganizerItemDetail::DetailType detail)
{
    if (m_detail != detail) {
        m_detail = detail;
        if (m_componentCompleted)
            setDetail();
    }
}

/*!
    \qmlproperty enum DetailRangeFilter::field

    This property holds the detail field type of which the detail filter will be matched to. The
    value should be the filld enumeration value defined in each detail element.

    \sa EventTime, JournalTime, TodoTime, TodoProgress, Reminder, AudibleReminder, VisualReminder,
        EmailReminder, Comment, Description, DisplayLabel, Guid, Location, Parent, Priority, Recurrence,
        Timestamp, Type, Tag
  */
int QDeclarativeOrganizerItemDetailRangeFilter::field() const
{
    return m_field;
}

void QDeclarativeOrganizerItemDetailRangeFilter::setField(int field)
{
    if (field != m_field || m_componentCompleted) {
        m_field = field;
        if (m_componentCompleted)
            setDetail();
        emit filterChanged();
    }
}


/*!
  \qmlproperty variant DetailRangeFilter::min

  This property holds the lower bound of the value range criterion. By default, there is no lower bound.
  */
void QDeclarativeOrganizerItemDetailRangeFilter::setMinValue(const QVariant &value)
{
    if (value != d.minValue()) {
        d.setRange(value, d.maxValue(), d.rangeFlags());
        emit valueChanged();
    }
}

QVariant QDeclarativeOrganizerItemDetailRangeFilter::minValue() const
{
    return d.minValue();
}

/*!
  \qmlproperty variant DetailRangeFilter::max

  This property holds the upper bound of the value range criterion. By default, there is no upper bound.
  */
void QDeclarativeOrganizerItemDetailRangeFilter::setMaxValue(const QVariant &value)
{
    if (value != d.maxValue()) {
        d.setRange(d.minValue(), value, d.rangeFlags());
        emit valueChanged();
    }
}

QVariant QDeclarativeOrganizerItemDetailRangeFilter::maxValue() const
{
    return d.maxValue();
}

/*!
  \qmlproperty enumeration DetailRangeFilter::matchFlags

  This property holds the match flags of the criterion, which define semantics such as case sensitivity, and exact matching.
  \sa DetailFilter::matchFlags
  */
void QDeclarativeOrganizerItemDetailRangeFilter::setMatchFlags(QDeclarativeOrganizerItemFilter::MatchFlags flags)
{
    QOrganizerItemFilter::MatchFlags newFlags;
    newFlags = ~newFlags & (int)flags;
    if (newFlags != d.matchFlags()) {
        d.setMatchFlags(newFlags);
        emit valueChanged();
    }
}

QDeclarativeOrganizerItemFilter::MatchFlags QDeclarativeOrganizerItemDetailRangeFilter::matchFlags() const
{
    QDeclarativeOrganizerItemFilter::MatchFlags newFlags;
    newFlags = ~newFlags & (int)d.matchFlags();
    return newFlags;
}

/*!
  \qmlproperty enumeration DetailRangeFilter::rangeFlags

  This property holds a set of flags which defines the boundary condition semantics of the value range criterion. The valid range flags include:
    \list
    \li DetailRangeFilter.IncludeLower
    \li DetailRangeFilter.IncludeUpper
    \li DetailRangeFilter.ExcludeLower
    \li DetailRangeFilter.ExcludeUpper
    \endlist
  */
void QDeclarativeOrganizerItemDetailRangeFilter::setRangeFlags(RangeFlags flags)
{
    QOrganizerItemDetailRangeFilter::RangeFlags newFlags;
    newFlags = ~newFlags & (int)flags;
    if (newFlags != d.rangeFlags()) {
        d.setRange(d.minValue(), d.maxValue(), newFlags);
        emit valueChanged();
    }
}
QDeclarativeOrganizerItemDetailRangeFilter::RangeFlags QDeclarativeOrganizerItemDetailRangeFilter::rangeFlags() const
{
    QDeclarativeOrganizerItemDetailRangeFilter::RangeFlags newFlags;
    newFlags = ~newFlags & (int)d.rangeFlags();
    return newFlags;
}

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemDetailRangeFilter::filter() const
{
    return d;
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetailRangeFilter::setDetail()
{
    d.setDetail(static_cast<QOrganizerItemDetail::DetailType>(m_detail), m_field);
    emit valueChanged();
}


/*!
    \qmlclass IdFilter QDeclarativeOrganizerItemIdFilter
    \brief The IdFilter element provides a filter based around a list of organizer item IDs.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa {QOrganizerItemIdFilter}
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemIdFilter::QDeclarativeOrganizerItemIdFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \qmlproperty list<string> IdFilter::ids

    This property holds the list of IDs of organizer items which match this filter.
 */
QStringList QDeclarativeOrganizerItemIdFilter::ids() const
{
    return m_ids;
}

void QDeclarativeOrganizerItemIdFilter::setIds(const QStringList &ids)
{
    foreach (const QString& id, ids) {
        if (!m_ids.contains(id)) {
            m_ids = ids;
            emit valueChanged();
            return;
        }
    }

    foreach (const QString& id, m_ids) {
        if (!ids.contains(id)) {
            m_ids = ids;
            emit valueChanged();
        }
    }
}

/*!
    \internal
 */
QOrganizerItemFilter QDeclarativeOrganizerItemIdFilter::filter() const
{
    QOrganizerItemIdFilter f;
    QList<QOrganizerItemId> ids;
    foreach (const QString& id, m_ids) {
        QOrganizerItemId itemId = QOrganizerItemId::fromString(id);
        if (!itemId.isNull())
            ids << itemId;
    }

    f.setIds(ids);
    return f;
}

#include "moc_qdeclarativeorganizeritemfilter_p.cpp"

QTORGANIZER_END_NAMESPACE
