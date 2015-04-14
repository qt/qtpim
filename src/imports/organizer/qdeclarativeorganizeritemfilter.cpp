/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
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

#include "qdeclarativeorganizeritemfilter_p.h"

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype Filter
    \instantiates QDeclarativeOrganizerItemFilter
    \brief The Filter element is used to filter items made available through a backend.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main
    \sa QOrganizerItemFilter
 */

/*!
  \qmlsignal QtOrganizer5::Filter::onFilterChanged()

  This signal is emitted, when any of the Filter's or child element's (like DetailFilter, CollectionFilter etc) properties have been changed.
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
    \li Filter.DetailFilter        A filter which matches organizer items containing exactly one given detail.
    \li Filter.DetailFieldFilter   A filter which matches organizer items containing one or more details
                                  of a particular type with a particular field having a particular value.
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
    \qmltype InvalidFilter
    \instantiates QDeclarativeOrganizerItemInvalidFilter
    \brief the InvalidFilter element provides a filter which will never match any organizer items.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemInvalidFilter
 */

/*!
  \qmlsignal InvalidFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
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
    \qmltype IntersectionFilter
    \instantiates QDeclarativeOrganizerItemIntersectionFilter
    \brief The IntersectionFilter element provides a filter which intersects the results of other filters.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemIntersectionFilter
 */

/*!
  \qmlsignal InvalidFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
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
    \qmltype UnionFilter
    \instantiates QDeclarativeOrganizerItemUnionFilter
    \brief The UnionFilter element provides a filter which unions the results of other filters.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemUnionFilter
 */

/*!
  \qmlsignal UnionFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
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
    \qmltype CollectionFilter
    \instantiates QDeclarativeOrganizerItemCollectionFilter
    \brief The CollectionFilter element provides a filter based around the collection one organizer
           item belongs to.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa Collection
 */

/*!
  \qmlsignal CollectionFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
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
    \qmltype DetailFilter
    \instantiates QDeclarativeOrganizerItemDetailFilter
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

        Type {
            id: typeDetailToMatch
            type: Type.Todo
        }

        DetailFilter {
            id: todoFilter
            detail: typeDetailToMatch
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
  \qmlsignal DetailFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerItemDetailFilter::QDeclarativeOrganizerItemDetailFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
    , m_detail(0)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \qmlproperty Detail DetailFilter::detail

    This property holds the detail instance used by this filter for matching.
  */
QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItemDetailFilter::detail() const
{
    return m_detail;
}

void QDeclarativeOrganizerItemDetailFilter::setDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (m_detail != detail) {
        m_detail = detail;
        if (m_detail && m_detail->detail() != d.detail()) {
            d.setDetail(m_detail->detail());
            emit valueChanged();
        }
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
    \qmltype DetailFieldFilter
    \instantiates QDeclarativeOrganizerItemDetailFieldFilter
    \brief The DetailFieldFilter element provides a filter based around a detail value criterion.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters

    Simple example how to utilize DetailFieldFilter element together with OrganizerModel and ListView elements:
    \code
    Rectangle {
        height: 400; width: 400;

        OrganizerModel{
            id: organizer
            startPeriod: "2009-01-01"
            endPeriod: "2012-12-31"
            filter: todoFilter
        }

        DetailFieldFilter {
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

    \sa QOrganizerItemDetailFieldFilter
 */

/*!
  \qmlsignal DetailFieldFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerItemDetailFieldFilter::QDeclarativeOrganizerItemDetailFieldFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \qmlproperty enum DetailFieldFilter::detail

    This property holds the detail type of which the detail filter will be matched to. The value
    shuold be the enumeration value of Detail::type.
  */
QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDetailFieldFilter::detail() const
{
    return static_cast<QDeclarativeOrganizerItemDetail::DetailType>(d.detailType());
}

void QDeclarativeOrganizerItemDetailFieldFilter::setDetail(QDeclarativeOrganizerItemDetail::DetailType detail)
{
    if (detail != static_cast<QDeclarativeOrganizerItemDetail::DetailType>(d.detailType())) {
        d.setDetail(static_cast<QOrganizerItemDetail::DetailType>(detail), d.detailField());
        emit valueChanged();
    }
}

/*!
    \qmlproperty enum DetailFieldFilter::field

    This property holds the detail field type of which the detail field filter will be matched to. The
    value should be the filld enumeration value defined in each detail element.

    \sa EventTime, JournalTime, TodoTime, TodoProgress, Reminder, AudibleReminder, VisualReminder,
        EmailReminder, Comment, Description, DisplayLabel, Guid, Location, Parent, Priority, Recurrence,
        Timestamp, ItemType, Tag
  */
int QDeclarativeOrganizerItemDetailFieldFilter::field() const
{
    return d.detailField();
}

void QDeclarativeOrganizerItemDetailFieldFilter::setField(int field)
{
    if (field != d.detailField()) {
        d.setDetail(d.detailType(), field);
        emit valueChanged();
    }
}

/*!
  \qmlproperty variant DetailFieldFilter::value

  This property holds the value criterion of the detail field filter.
  */
QVariant QDeclarativeOrganizerItemDetailFieldFilter::value() const
{
    return d.value();
}

void QDeclarativeOrganizerItemDetailFieldFilter::setValue(const QVariant &newValue)
{
    if (newValue != value()) {
        if (QVariant::DateTime == newValue.type()) {
            // handling dates and datetimes internally as UTC
            d.setValue(newValue.toDateTime().toUTC());
        } else {
            d.setValue(newValue);
        }
        emit valueChanged();
    }
}

/*!
  \qmlproperty enumeration DetailFieldFilter::matchFlags

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
QDeclarativeOrganizerItemFilter::MatchFlags QDeclarativeOrganizerItemDetailFieldFilter::matchFlags() const
{
    QDeclarativeOrganizerItemFilter::MatchFlags newFlags;
    newFlags = ~newFlags & (int)d.matchFlags();
    return newFlags;
}

void QDeclarativeOrganizerItemDetailFieldFilter::setMatchFlags(QDeclarativeOrganizerItemFilter::MatchFlags flags)
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
QOrganizerItemFilter QDeclarativeOrganizerItemDetailFieldFilter::filter() const
{
    return d;
}


/*!
    \qmltype DetailRangeFilter
    \instantiates QDeclarativeOrganizerItemDetailRangeFilter
    \brief The DetailRangeFilter element provides a filter based around a detail value range criterion.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa QOrganizerItemDetailRangeFilter
 */

/*!
  \qmlsignal DetailRangeFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerItemDetailRangeFilter::QDeclarativeOrganizerItemDetailRangeFilter(QObject *parent)
    : QDeclarativeOrganizerItemFilter(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(filterChanged()));
}

/*!
    \qmlproperty enum DetailRangeFilter::detail

    This property holds the detail type of which the detail filter will be matched to. The value
    shuold be the enumeration value of Detail::type.
  */
QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDetailRangeFilter::detail() const
{
    return static_cast<QDeclarativeOrganizerItemDetail::DetailType>(d.detailType());
}

void QDeclarativeOrganizerItemDetailRangeFilter::setDetail(QDeclarativeOrganizerItemDetail::DetailType detail)
{
    if (detail != static_cast<QDeclarativeOrganizerItemDetail::DetailType>(d.detailType())) {
        d.setDetail(static_cast<QOrganizerItemDetail::DetailType>(detail), d.detailField());
        emit valueChanged();
    }
}

/*!
    \qmlproperty enum DetailRangeFilter::field

    This property holds the detail field type of which the detail filter will be matched to. The
    value should be the filld enumeration value defined in each detail element.

    \sa EventTime, JournalTime, TodoTime, TodoProgress, Reminder, AudibleReminder, VisualReminder,
        EmailReminder, Comment, Description, DisplayLabel, Guid, Location, Parent, Priority, Recurrence,
        Timestamp, ItemType, Tag
  */
int QDeclarativeOrganizerItemDetailRangeFilter::field() const
{
    return d.detailField();
}

void QDeclarativeOrganizerItemDetailRangeFilter::setField(int field)
{
    if (field != d.detailField()) {
        d.setDetail(d.detailType(), field);
        emit valueChanged();
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
  \sa DetailFieldFilter::matchFlags
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
    \qmltype IdFilter
    \instantiates QDeclarativeOrganizerItemIdFilter
    \brief The IdFilter element provides a filter based around a list of organizer item IDs.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
    \sa {QOrganizerItemIdFilter}
 */

/*!
  \qmlsignal IdFilter::onFilterChanged()

  \sa QtOrganizer5::Filter::onFilterChanged
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

QT_END_NAMESPACE
