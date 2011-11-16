/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativeorganizeritemfilter_p.h"
#include "qdeclarativeorganizermodel_p.h"
#include "qorganizeritemid.h"
#include "qorganizercollectionid.h"

QTORGANIZER_BEGIN_NAMESPACE

/*!
   \qmlclass Filter QDeclarativeOrganizerItemFilter
   \brief The Filter element is used as a property of OrganizerModel, to allow
   selection of organizer items which have certain details or properties.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-main

   \sa QOrganizerItemFilter
 */


/*!
  \qmlproperty enumeration Filter::type

  This property holds the type value of this filter. It can be one of:

  \list
  \o Filter.DefaultFilter - A filter which matches everything (default).
  \o Filter.InvalidFilter - An invalid filter which matches nothing.
  \o Filter.DetailFilter - A filter which matches organizer items containing one or more details of a particular definition with a particular value.
  \o Filter.DetailRangeFilter - A filter which matches organizer items containing one or more details of a particular definition whose values are within a particular range.
  \o Filter.ChangeLogFilter - A filter which matches organizer items whose timestamps have been updated since some particular date and time.
  \o Filter.CollectionFilter - A filter which matches any organizer item that is matched by collection.
  \o Filter.IntersectionFilter - A filter which matches all organizer items that are matched by all filters it includes.
  \o Filter.UnionFilter - A filter which matches any organizer item that is matched by any of the filters it includes.
  \o Filter.IdFilter - A filter which matches any organizer item whose local id is contained in a particular list of organizer item local ids.
  \endlist
  */

/*!
   \qmlclass ChangeLogFilter QDeclarativeOrganizerItemChangeLogFilter
   \brief The ChangeLogFilter element provides a filter based around a organizer item timestamp criterion.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-filters

   \sa QOrganizerItemChangeLogFilter
 */

/*!
  \qmlproperty date ChangeLogFilter::since

  This property holds the date and time lower-bound criterion of the filter.
  */

/*!
  \qmlproperty enumeration ChangeLogFilter::eventType

  This property holds the type of change that this filter will match against. The value can be one of:
  \list
  \o ChangeLogFilter.EventAdded
  \o ChangeLogFilter.EventChanged
  \o ChangeLogFilter.EventRemoved
  \endlist
  */


/*!
   \qmlclass DetailFilter QDeclarativeOrganizerItemDetailFilter
   \brief The \l DetailFilter element provides a filter based around a detail value criterion.
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
    , m_detail(QDeclarativeOrganizerItemDetail::Customized)
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
    setDetailDefinitionName();
    m_componentCompleted = true;
}

/*!
    \qmlproperty enum DetailFilter::detail

    This property holds the detail type of which the detail filter will be matched to. The value
    shuold be the enumeration value of Detail::type.
  */
QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemDetailFilter::detail() const
{
    return m_detail;
}

void QDeclarativeOrganizerItemDetailFilter::setDetail(QDeclarativeOrganizerItemDetail::ItemDetailType detail)
{
    if (m_detail != detail) {
        m_detail = detail;
        if (m_componentCompleted)
            setDetailDefinitionName();
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
            setDetailDefinitionName();
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
  \o MatchExactly - Performs QVariant-based matching (default).
  \o MatchContains - The search term is contained in the item.
  \o MatchStartsWith - The search term matches the start of the item.
  \o MatchEndsWith - The search term matches the end of the item.
  \o MatchFixedString - Performs string-based matching. String-based comparisons are case-insensitive unless the \c MatchCaseSensitive flag is also specified.
  \o MatchCaseSensitive - The search is case sensitive.
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
void QDeclarativeOrganizerItemDetailFilter::setDetailDefinitionName()
{
    d.setDetailDefinitionName(QDeclarativeOrganizerItemDetail::definitionName(m_detail),
                              QDeclarativeOrganizerItemDetail::fieldName(m_detail, m_field));
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
    , m_detail(QDeclarativeOrganizerItemDetail::Customized)
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
    setDetailDefinitionName();
    m_componentCompleted = true;
}

/*!
    \qmlproperty enum DetailRangeFilter::detail

    This property holds the detail type of which the detail filter will be matched to. The value
    shuold be the enumeration value of Detail::type.
  */
QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemDetailRangeFilter::detail() const
{
    return m_detail;
}

void QDeclarativeOrganizerItemDetailRangeFilter::setDetail(QDeclarativeOrganizerItemDetail::ItemDetailType detail)
{
    if (m_detail != detail) {
        m_detail = detail;
        if (m_componentCompleted)
            setDetailDefinitionName();
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
            setDetailDefinitionName();
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
    \o DetailRangeFilter.IncludeLower
    \o DetailRangeFilter.IncludeUpper
    \o DetailRangeFilter.ExcludeLower
    \o DetailRangeFilter.ExcludeUpper
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
void QDeclarativeOrganizerItemDetailRangeFilter::setDetailDefinitionName()
{
    d.setDetailDefinitionName(QDeclarativeOrganizerItemDetail::definitionName(m_detail),
                              QDeclarativeOrganizerItemDetail::fieldName(m_detail, m_field));
    emit valueChanged();
}


/*!
   \qmlclass IntersectionFilter QDeclarativeOrganizerItemIntersectionFilter
   \brief The IntersectionFilter element provides a filter which intersects the results of other filters.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-filters

   \sa QOrganizerItemIntersectionFilter
 */

/*!
  \qmlproperty list<filter> IntersectionFilter::filters

  This property holds the list of filters which form the intersection filter.
  */


/*!
   \qmlclass UnionFilter QDeclarativeOrganizerItemUnionFilter
   \brief The UnionFilter element provides a filter which unions the results of other filters.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-filters

   \sa QOrganizerItemUnionFilter
 */

/*!
  \qmlproperty list<filter> UnionFilter::filters

  This property holds the list of filters which form the union filter.
  */

/*!
   \qmlclass IdFilter QDeclarativeOrganizerItemIdFilter
   \brief The IdFilter element provides a filter based around a list of organizer item ids.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-filters

   \sa {QOrganizerItemIdFilter}
 */

/*!
  \qmlproperty list<string> IdFilter::ids

  This property holds the list of ids of organizer items which match this filter.
  */


/*!
   \qmlclass CollectionFilter QDeclarativeOrganizerItemCollectionFilter
   \brief The CollectionFilter element provides a filter based around a list of organizer item ids.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-filters
   \sa Collection
 */


/*!
  \qmlproperty list<variant> CollectionFilter::ids

  This property holds the list of ids of organizer items which match this filter.

 */

/*!
   \qmlclass InvalidFilter QDeclarativeOrganizerItemInvalidFilter
   \brief the InvalidFilter element provides a filter which will never match any organizer items.
   \inqmlmodule QtOrganizer
   \ingroup qml-organizer-filters

   \sa QOrganizerItemInvalidFilter
 */


QOrganizerItemFilter QDeclarativeOrganizerItemIdFilter::filter() const
{
    QOrganizerItemIdFilter f;
    QList<QOrganizerItemId> ids;

    foreach(const QVariant& id, m_ids) {
        QOrganizerItemId itemId = QOrganizerItemId::fromString(id.toString());
        if (!itemId.isNull())
            ids << itemId;
    }

    f.setIds(ids);
    return f;
}
QOrganizerItemFilter QDeclarativeOrganizerItemCollectionFilter::filter() const
{
    QOrganizerItemCollectionFilter f;
    QSet<QOrganizerCollectionId> ids;

    foreach(const QVariant& id, m_ids) {
        QOrganizerCollectionId cId = QOrganizerCollectionId::fromString(id.toString());
        if (!cId.isNull())
            ids << cId;
    }

    f.setCollectionIds(ids);
    return f;
}

QDeclarativeListProperty<QDeclarativeOrganizerItemFilter> QDeclarativeOrganizerItemCompoundFilter::filters()
{
    return QDeclarativeListProperty<QDeclarativeOrganizerItemFilter>(this,
                                                          0, // opaque data parameter
                                                          filters_append,
                                                          filters_count,
                                                          filters_at,
                                                          filters_clear);
}

void QDeclarativeOrganizerItemCompoundFilter::filters_append(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter>* prop, QDeclarativeOrganizerItemFilter* filter)
{
    QDeclarativeOrganizerItemCompoundFilter* compoundFilter = static_cast<QDeclarativeOrganizerItemCompoundFilter*>(prop->object);
    compoundFilter->m_filters.append(filter);
    QObject::connect(filter, SIGNAL(filterChanged()), compoundFilter, SIGNAL(valueChanged()));
    emit compoundFilter->valueChanged();
}

int QDeclarativeOrganizerItemCompoundFilter::filters_count(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter>* prop)
{
    // The 'prop' is in a sense 'this' for this static function (as given in filters() function)
    return static_cast<QDeclarativeOrganizerItemCompoundFilter*>(prop->object)->m_filters.count();
}

QDeclarativeOrganizerItemFilter* QDeclarativeOrganizerItemCompoundFilter::filters_at(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter>* prop, int index)
{
    return static_cast<QDeclarativeOrganizerItemCompoundFilter*>(prop->object)->m_filters.at(index);
}

void QDeclarativeOrganizerItemCompoundFilter::filters_clear(QDeclarativeListProperty<QDeclarativeOrganizerItemFilter>* prop)
{
    QDeclarativeOrganizerItemCompoundFilter* filter = static_cast<QDeclarativeOrganizerItemCompoundFilter*>(prop->object);
    if (!filter->m_filters.isEmpty()) {
        filter->m_filters.clear();
        emit filter->valueChanged();
    }
}

#include "moc_qdeclarativeorganizeritemfilter_p.cpp"

QTORGANIZER_END_NAMESPACE
