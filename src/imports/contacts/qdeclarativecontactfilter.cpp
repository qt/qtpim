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

#include "qdeclarativecontactfilter_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
   \qmltype Filter
    \instantiates QDeclarativeContactFilter
   \brief The Filter element is used as a property of ContactModel, to allow
   selection of contacts which have certain details or properties.

   \ingroup qml-contacts-main
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactFilter
 */


/*!
  \qmlproperty enumeration Filter::type

  This property holds the type value of this filter. It can be one of:

  \list
  \li Filter.DefaultFilter - A filter which matches everything (default).
  \li Filter.InvalidFilter - An invalid filter which matches nothing.
  \li Filter.DetailFilter - A filter which matches contacts containing one or more details of a particular definition with a particular value.
  \li Filter.DetailRangeFilter - A filter which matches contacts containing one or more details of a particular definition whose values are within a particular range.
  \li Filter.ChangeLogFilter - A filter which matches contacts whose timestamps have been updated since some particular date and time.
  \li Filter.ActionFilter - A filter which matches contacts for which a particular action is available, or which contain a detail with a particular value for which a particular action is available.
  \li Filter.RelationshipFilter - A filter which matches contacts which participate in a particular type of relationship, or relationship with a specified contact.
  \li Filter.IntersectionFilter - A filter which matches all contacts that are matched by all filters it includes.
  \li Filter.UnionFilter - A filter which matches any contact that is matched by any of the filters it includes.
  \li Filter.IdFilter - A filter which matches any contact whose local id is contained in a particular list of contact local ids.
  \endlist
  */

/*!
   \qmltype ActionFilter
    \instantiates QDeclarativeContactActionFilter
   \brief The ActionFilter element provides a filter based around an action availability criterion.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactActionFilter
 */

/*!
  \qmlproperty string ActionFilter::actionName

  This property holds the action name criterion of the filter.
  */

/*!
   \qmltype ChangeLogFilter
    \instantiates QDeclarativeContactChangeLogFilter
   \brief The ChangeLogFilter element provides a filter based around a contact timestamp criterion.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactChangeLogFilter
 */

/*!
  \qmlproperty date ChangeLogFilter::since

  This property holds the date and time lower-bound criterion of the filter.The value can be one of:
  \list
  \li ChangeLogFilter.EventAdded
  \li ChangeLogFilter.EventChanged
  \li ChangeLogFilter.EventRemoved
  \endlist
  */

/*!
  \qmlproperty enumeration ChangeLogFilter::eventType

  This property holds the type of change that this filter will match against.
  */

/*!
   \qmltype DetailFilter
    \instantiates QDeclarativeContactDetailFilter
   \brief The DetailFilter element provides a filter based around a detail value criterion.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactDetailFilter
 */

/*!
  \qmlproperty enumeration DetailFilter::detail

  This property holds the detail type of which details will be matched to.

  \sa ContactDetail::type
  */

/*!
  \qmlproperty int DetailFilter::field

  This property holds the detail field type of which detail fields will be matched to.
  Detail field types are enumeration values defined in each detail elements.

  \sa Address
  \sa Anniversary
  \sa Avatar
  \sa Birthday
  \sa DisplayLabel
  \sa EmailAddress
  \sa Family
  \sa Favorite
  \sa Gender
  \sa GeoLocation
  \sa GlobalPresence
  \sa Guid
  \sa Name
  \sa Nickname
  \sa Note
  \sa OnlineAccount
  \sa Organization
  \sa PhoneNumber
  \sa Presence
  \sa Ringtone
  \sa SyncTarget
  \sa Tag
  \sa Timestamp
  \sa Url
  \sa Hobby
  */

/*!
  \qmlproperty variant DetailFilter::value

  This property holds the value criterion of the detail filter.
  */

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

/*!
   \qmltype DetailRangeFilter
    \instantiates QDeclarativeContactDetailRangeFilter
   \brief The DetailRangeFilter element provides a filter based around a detail value range criterion.


   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactDetailRangeFilter
 */

/*!
  \qmlproperty enumeration DetailRangeFilter::detail

  This property holds the detail type of which details will be matched to.

  \sa ContactDetail::type
  \sa DetailFilter::detail
  */

/*!
  \qmlproperty int DetailRangeFilter::field

  This property holds the detail field type of which detail fields will be matched to.
  Detail field types are enumeration values defined in each detail elements.

  \sa DetailFilter::field
  */
/*!
  \qmlproperty variant DetailRangeFilter::min

  This property holds the lower bound of the value range criterion. By default, there is no lower bound.
  */
/*!
  \qmlproperty variant DetailRangeFilter::max

  This property holds the upper bound of the value range criterion. By default, there is no upper bound.
  */

/*!
  \qmlproperty enumeration DetailRangeFilter::matchFlags

  This property holds the match flags of the criterion, which define semantics such as case sensitivity, and exact matching.

  \sa DetailFilter::matchFlags
  */

/*!
  \qmlproperty enumeration DetailRangeFilter::rangeFlags

  This property holds a set of flags which defines the boundary condition semantics of the value range criterion.The valid range flags include:
    \list
    \li DetailRangeFilter.IncludeLower
    \li DetailRangeFilter.IncludeUpper
    \li DetailRangeFilter.ExcludeLower
    \li DetailRangeFilter.ExcludeUpper
    \endlist
  */
/*!
   \qmltype IntersectionFilter
    \instantiates QDeclarativeContactIntersectionFilter
   \brief The IntersectionFilter element provides a filter which intersects the results of other filters.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactIntersectionFilter
 */

/*!
  \qmlproperty list<Filter> IntersectionFilter::filters

  This property holds the list of filters which form the intersection filter.
  */


/*!
   \qmltype UnionFilter
    \instantiates QDeclarativeContactUnionFilter
   \brief The UnionFilter element provides a filter which unions the results of other filters.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactUnionFilter
 */

/*!
  \qmlproperty list<Filter> UnionFilter::filters

  This property holds the list of filters which form the union filter.
  */

/*!
   \qmltype IdFilter
    \instantiates QDeclarativeContactIdFilter
   \brief The IdFilter element provides a filter based around a list of contact ids.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactIdFilter
 */

/*!
  \qmlproperty list<string> IdFilter::ids

  This property holds the list of ids of contacts which match this filter.
  */


/*!
   \qmltype RelationshipFilter
    \instantiates QDeclarativeContactRelationshipFilter
   \brief The RelationshipFilter element provides a filter based around relationship criteria.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts


   This element is part of the \b{QtContacts} module.

   \sa Relationship
   \sa RelationshipModel
   \sa QContactRelationshipFilter
   \sa QContactRelationship
 */


/*!
  \qmlproperty variant RelationshipFilter::relationshipType

  This property holds the type of relationship that a contact must have in order to match the filter.

  \sa Relationship::type
 */

/*!
  \qmlproperty int RelationshipFilter::relatedContactId

  This property holds the id of the contact with whom the tested contact must have a relationship in order for the tested contact to match this filter
  */
/*!
  \qmlproperty enumeration RelationshipFilter::relatedContactRole

  This property holds the role in the relationship with the tested contact that the related contact must play in order for the tested contact to match this filter.
  The role can be one of:
  \list
  \li Relationship.First - The contact is the first contact in the relationship.
  \li Relationship.Second - The contact is the second contact in the relationship.
  \li Relationship.Either - The contact is either the first or second contact in the relationship (default).
  \endlist
  */

/*!
   \qmltype InvalidFilter
    \instantiates QDeclarativeContactInvalidFilter
   \brief the InvalidFilter element provides a filter which will never match any contacts.

   \ingroup qml-contacts-filters
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactInvalidFilter
 */

QQmlListProperty<QDeclarativeContactFilter> QDeclarativeContactCompoundFilter::filters()
{
    return QQmlListProperty<QDeclarativeContactFilter>(this,
                                                          0, // opaque data parameter
                                                          filters_append,
                                                          filters_count,
                                                          filters_at,
                                                          filters_clear);
}

void QDeclarativeContactCompoundFilter::filters_append(QQmlListProperty<QDeclarativeContactFilter>* prop, QDeclarativeContactFilter* filter)
{
    QDeclarativeContactCompoundFilter* compoundFilter = static_cast<QDeclarativeContactCompoundFilter*>(prop->object);
    compoundFilter->m_filters.append(filter);
    QObject::connect(filter, SIGNAL(filterChanged()), compoundFilter, SIGNAL(filterChanged()), Qt::UniqueConnection);
    emit compoundFilter->filterChanged();
}

int QDeclarativeContactCompoundFilter::filters_count(QQmlListProperty<QDeclarativeContactFilter>* prop)
{
    // The 'prop' is in a sense 'this' for this static function (as given in filters() function)
    return static_cast<QDeclarativeContactCompoundFilter*>(prop->object)->m_filters.count();
}

QDeclarativeContactFilter* QDeclarativeContactCompoundFilter::filters_at(QQmlListProperty<QDeclarativeContactFilter>* prop, int index)
{
    return static_cast<QDeclarativeContactCompoundFilter*>(prop->object)->m_filters.at(index);
}

void QDeclarativeContactCompoundFilter::filters_clear(QQmlListProperty<QDeclarativeContactFilter>* prop)
{
    QDeclarativeContactCompoundFilter* compoundFilter = static_cast<QDeclarativeContactCompoundFilter*>(prop->object);
    if (!compoundFilter->m_filters.isEmpty()) {
        foreach (QDeclarativeContactFilter* filter, compoundFilter->m_filters) {
            QObject::disconnect(filter, SIGNAL(filterChanged()), compoundFilter, SIGNAL(filterChanged()));
        }
        compoundFilter->m_filters.clear();
        emit compoundFilter->filterChanged();
    }
}

#include "moc_qdeclarativecontactfilter_p.cpp"

QT_END_NAMESPACE
