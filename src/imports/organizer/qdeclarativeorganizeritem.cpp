/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include "qdeclarativeorganizeritem_p.h"

#include <QtQml/qqmlengine.h>

#include "qdeclarativeorganizermodel_p.h"

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype OrganizerItem
    \instantiates QDeclarativeOrganizerItem
    \brief The OrganizerItem element represents the in-memory version of a organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main

    A OrganizerItem has a number of mandatory details. Different subclasses of OrganizerItem
    (i.e., Event, EventOccurrence, Journal, Todo, TodoOccurrence, Note) may have more mandatory details.

    Most frequently-used details can also be accessed through convenient properties, e.g. displayLabel,
    while all details can be accessed through detail(), details(), saveDetail(), among others.

    \sa Event, EventOccurrence, Journal, Todo, TodoOccurrence, Note, {QOrganizerManager}, {QOrganizerItem}
 */

/*!
  \qmlsignal OrganizerItem::onItemChanged()

  This signal is emitted, when any of the OrganizerItem's or child element's (like Event, Todo etc) properties have been changed.
 */

/*!
    \internal
 */
QDeclarativeOrganizerItem::QDeclarativeOrganizerItem(QObject *parent)
    : QObject(parent)
    , m_modified(false)
{
}

/*!
    \internal
 */
QDeclarativeOrganizerItem::~QDeclarativeOrganizerItem()
{
    clearDetails();
}

// basic information
/*!
    \qmlproperty bool OrganizerItem::modified

    This property holds the dirty flag of the OrganizerItem object.

    \sa save
 */
bool QDeclarativeOrganizerItem::modified() const
{
    return m_modified;
}

/*!
    \qmlproperty enum OrganizerItem::itemType

    This property holds the type of the OrganizerItem object.
 */
QDeclarativeOrganizerItemType::ItemType QDeclarativeOrganizerItem::itemType() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::ItemType == detail->type())
            return static_cast<QDeclarativeOrganizerItemType *>(detail)->itemType();
    }
    return QDeclarativeOrganizerItemType::Undefined;
}

/*!
    \qmlproperty list<Detail> OrganizerItem::itemDetails

    This property holds the details of the OrganizerItem object.
 */
QQmlListProperty<QDeclarativeOrganizerItemDetail> QDeclarativeOrganizerItem::itemDetails()
{
    return QQmlListProperty<QDeclarativeOrganizerItemDetail>(this, 0,
                                                                     &QDeclarativeOrganizerItem::_q_detail_append,
                                                                     &QDeclarativeOrganizerItem::_q_detail_count,
                                                                     &QDeclarativeOrganizerItem::_q_detail_at,
                                                                     &QDeclarativeOrganizerItem::_q_detail_clear);
}

/*!
    \qmlproperty string OrganizerItem::itemId

    This property holds the id of the OrganizerItem object.
 */
QString QDeclarativeOrganizerItem::itemId() const
{
    return m_id.toString();
}

/*!
    \qmlproperty string OrganizerItem::manager

    This property holds the manager uri which the \l OrganizerItem object comes from.
 */
QString QDeclarativeOrganizerItem::manager() const
{
    return m_id.managerUri();
}

/*!
    \qmlproperty string OrganizerItem::collectionId

    This property holds the id of collection where the item belongs to.
 */
QString QDeclarativeOrganizerItem::collectionId() const
{
    return m_collectionId.toString();
}

void QDeclarativeOrganizerItem::setCollectionId(const QString &collectionId)
{
    QOrganizerCollectionId newCollectionId(QOrganizerCollectionId::fromString(collectionId));

    // in case invalid collectionId-string, fromString() will return default collectionId-string
    // instead of the intended collectionId-string
    if (newCollectionId.toString() == collectionId && m_collectionId.toString() != collectionId) {
        m_collectionId = newCollectionId;
        m_modified = true;
        emit itemChanged();
    }
}

// convenient access to most frequently used details
/*!
    \qmlproperty string OrganizerItem::description

    This property holds the description text of the organizer item.
 */
QString QDeclarativeOrganizerItem::description() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Description == detail->type())
            return static_cast<QDeclarativeOrganizerItemDescription *>(detail)->description();
    }
    return QString::null;
}

void QDeclarativeOrganizerItem::setDescription(const QString &description)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Description == detail->type()) {
            QDeclarativeOrganizerItemDescription *desc = static_cast<QDeclarativeOrganizerItemDescription *>(detail);
            if (desc->description() != description) {
                desc->setDescription(description);
                m_modified = true;
                emit itemChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemDescription *desc = new QDeclarativeOrganizerItemDescription(this);
    desc->setDescription(description);
    m_details.append(desc);
    m_modified = true;
    emit itemChanged();
}

/*!
    \qmlproperty string OrganizerItem::displayLabel

    This property holds the display label of the organizer item.
 */
QString QDeclarativeOrganizerItem::displayLabel() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::DisplayLabel == detail->type())
            return static_cast<QDeclarativeOrganizerItemDisplayLabel *>(detail)->label();
    }
    return QString::null;
}

void QDeclarativeOrganizerItem::setDisplayLabel(const QString &label)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::DisplayLabel == detail->type()) {
            QDeclarativeOrganizerItemDisplayLabel *displayLabel = static_cast<QDeclarativeOrganizerItemDisplayLabel *>(detail);
            if (displayLabel->label() != label) {
                displayLabel->setLabel(label);
                m_modified = true;
                emit itemChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemDisplayLabel *displayLabel = new QDeclarativeOrganizerItemDisplayLabel(this);
    displayLabel->setLabel(label);
    m_details.append(displayLabel);
    m_modified = true;
    emit itemChanged();
}

/*!
    \qmlproperty string OrganizerItem::guid

    This property holds the GUID string of the organizer item.
 */
QString QDeclarativeOrganizerItem::guid() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Guid == detail->type())
            return static_cast<QDeclarativeOrganizerItemGuid *>(detail)->guid();
    }
    return QString::null;
}

void QDeclarativeOrganizerItem::setGuid(const QString &guid)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Guid == detail->type()) {
            QDeclarativeOrganizerItemGuid *itemGuid = static_cast<QDeclarativeOrganizerItemGuid *>(detail);
            if (itemGuid->guid() != guid) {
                itemGuid->setGuid(guid);
                m_modified = true;
                emit itemChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemGuid *itemGuid = new QDeclarativeOrganizerItemGuid(this);
    itemGuid->setGuid(guid);
    m_details.append(itemGuid);
    m_modified = true;
    emit itemChanged();
}

// functions
/*!
    \qmlmethod Detail OrganizerItem::detail(type)

    Returns the first detail stored in the organizer item with the given \a type.

    \sa Detail::type
 */
QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItem::detail(int type)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (type == detail->type()) {
            QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail->type());
            QQmlEngine::setObjectOwnership(itemDetail, QQmlEngine::JavaScriptOwnership);
            itemDetail->setDetail(detail->detail());
            return itemDetail;
        }
    }
    return 0;
}

/*!
    \qmlmethod list<Detail> OrganizerItem::details(type)

    Returns all the details stored in the organizer item with the given \a type.

    \sa Detail::type
 */
QVariantList QDeclarativeOrganizerItem::details(int type)
{
    QVariantList list;
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (type == detail->type()) {
            QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail->type());
            QQmlEngine::setObjectOwnership(itemDetail, QQmlEngine::JavaScriptOwnership);
            itemDetail->setDetail(detail->detail());
            list.append(QVariant::fromValue((QObject*)itemDetail));
        }
    }
    return list;
}

/*!
    \qmlmethod void OrganizerItem::setDetail(detail)

    Saves the given \a detail in the organizer item, and sets its id.
 */
void QDeclarativeOrganizerItem::setDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (_q_setDetail(detail))
        emit itemChanged();
}

/*!
    \qmlmethod void OrganizerItem::removeDetail(detail)

    Removes given \a detail from the organizer item.
 */
void QDeclarativeOrganizerItem::removeDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (_q_removeDetail(detail))
        emit itemChanged();
}

/*!
    \qmlmethod OrganizerItem::clearDetails()

    Removes all details from the organizer item.

    \sa removeDetail
 */
void QDeclarativeOrganizerItem::clearDetails()
{
    if (_q_clearDetails())
        emit itemChanged();
}

/*!
    \qmlmethod OrganizerItem::save()

    Saves this OrganizerItem if the item has been modified.

    \sa modified
 */
void QDeclarativeOrganizerItem::save()
{
    if (m_modified) {
        QDeclarativeOrganizerModel *model = qobject_cast<QDeclarativeOrganizerModel *>(parent());
        if (model) {
            model->saveItem(this);
            m_modified = false;
        }
    }
}

// non-QML APIs, used by model only
/*!
    \internal
 */
void QDeclarativeOrganizerItem::setItem(const QOrganizerItem &item)
{
    m_id = item.id();
    m_collectionId = item.collectionId();

    foreach (QDeclarativeOrganizerItemDetail *detail, m_details)
        delete detail;
    m_details.clear();
    QList<QOrganizerItemDetail> details(item.details());
    foreach (const QOrganizerItemDetail &detail, details) {
        QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(static_cast<QDeclarativeOrganizerItemDetail::DetailType>(detail.type()));
        itemDetail->setDetail(detail);
        m_details.append(itemDetail);
    }

    m_modified = false;
    emit itemChanged();
}

/*!
    \internal
 */
QOrganizerItem QDeclarativeOrganizerItem::item() const
{
    QOrganizerItem item;
    item.setId(m_id);
    item.setCollectionId(m_collectionId);
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        QOrganizerItemDetail itemDetail = detail->detail();
        item.saveDetail(&itemDetail);
    }
    return item;
}

/*!
    \internal
 */
bool QDeclarativeOrganizerItem::generatedOccurrence() const
{
    QDeclarativeOrganizerItemType::ItemType type = itemType();
    return (m_id.isNull() && (type == QDeclarativeOrganizerItemType::EventOccurrence || type == QDeclarativeOrganizerItemType::TodoOccurrence));
}

/*!
    \internal
 */
QDateTime QDeclarativeOrganizerItem::itemStartTime() const
{
    switch (itemType()) {
    case QDeclarativeOrganizerItemType::Event:
        return static_cast<const QDeclarativeOrganizerEvent*>(this)->startDateTime();
    case QDeclarativeOrganizerItemType::EventOccurrence:
        return static_cast<const QDeclarativeOrganizerEventOccurrence*>(this)->startDateTime();
    case QDeclarativeOrganizerItemType::Todo:
        return static_cast<const QDeclarativeOrganizerTodo*>(this)->startDateTime();
    case QDeclarativeOrganizerItemType::TodoOccurrence:
        return static_cast<const QDeclarativeOrganizerTodoOccurrence*>(this)->startDateTime();
    case QDeclarativeOrganizerItemType::Journal:
        return static_cast<const QDeclarativeOrganizerJournal*>(this)->dateTime();
    case QDeclarativeOrganizerItemType::Note:
    default:
        break;
    }
    return QDateTime();
}

/*!
    \internal
 */
QDateTime QDeclarativeOrganizerItem::itemEndTime() const
{
    switch (itemType()) {
    case QDeclarativeOrganizerItemType::Event:
        return static_cast<const QDeclarativeOrganizerEvent*>(this)->endDateTime();
    case QDeclarativeOrganizerItemType::EventOccurrence:
        return static_cast<const QDeclarativeOrganizerEventOccurrence*>(this)->endDateTime();
    case QDeclarativeOrganizerItemType::Todo:
        return static_cast<const QDeclarativeOrganizerTodo*>(this)->dueDateTime();
    case QDeclarativeOrganizerItemType::TodoOccurrence:
        return static_cast<const QDeclarativeOrganizerTodoOccurrence*>(this)->dueDateTime();
    case QDeclarativeOrganizerItemType::Journal:
        //there is no end time for journal item,  make it 30mins later for display purpose
        return static_cast<const QDeclarativeOrganizerJournal*>(this)->dateTime().addSecs(60*30);
    case QDeclarativeOrganizerItemType::Note:
    default:
        break;
    }
    return QDateTime();
}

// call-back functions for list property
/*!
    \internal
 */
void QDeclarativeOrganizerItem::_q_detail_append(QQmlListProperty<QDeclarativeOrganizerItemDetail> *property, QDeclarativeOrganizerItemDetail *value)
{
    QDeclarativeOrganizerItem *object = qobject_cast<QDeclarativeOrganizerItem *>(property->object);
    if (object)
        object->m_details.append(value);
}

/*!
    \internal
 */
QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItem::_q_detail_at(QQmlListProperty<QDeclarativeOrganizerItemDetail> *property, int index)
{
    QDeclarativeOrganizerItem *object = qobject_cast<QDeclarativeOrganizerItem *>(property->object);
    if (object)
        return object->m_details.at(index);
    else
        return 0;
}

/*!
    \internal
 */
void QDeclarativeOrganizerItem::_q_detail_clear(QQmlListProperty<QDeclarativeOrganizerItemDetail> *property)
{
    QDeclarativeOrganizerItem *object = qobject_cast<QDeclarativeOrganizerItem *>(property->object);
    if (object) {
        foreach (QDeclarativeOrganizerItemDetail *obj, object->m_details)
            delete obj;
        object->m_details.clear();
    }
}

/*!
    \internal
 */
int QDeclarativeOrganizerItem::_q_detail_count(QQmlListProperty<QDeclarativeOrganizerItemDetail> *property)
{
    QDeclarativeOrganizerItem *object = qobject_cast<QDeclarativeOrganizerItem *>(property->object);
    if (object)
        return object->m_details.size();
    else
        return 0;
}

/*!
    \internal
 */
bool QDeclarativeOrganizerItem::_q_removeDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (!detail)
        return false;

    int key = detail->detail().key();
    int i = 0;
    bool removed = false;
    foreach (QDeclarativeOrganizerItemDetail *itemDetail, m_details) {
        if (key == itemDetail->detail().key()) {
            delete itemDetail;
            m_details.removeAt(i);
            removed = true;
        }
        ++i;
    }
    return removed;
}

/*!
    \internal
 */
bool QDeclarativeOrganizerItem::_q_setDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (!detail)
        return false;

    bool found(false);
    int key = detail->detail().key();
    foreach (QDeclarativeOrganizerItemDetail *itemDetail, m_details) {
        if (key == itemDetail->detail().key()) {
            itemDetail->setDetail(detail->detail());
            found = true;
        }
    }
    if (!found) {
        QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail->type());
        itemDetail->setDetail(detail->detail());
        m_details.append(itemDetail);
    }
    m_modified = true;
    return true;
}

/*!
    \internal
 */
bool QDeclarativeOrganizerItem::_q_clearDetails()
{
    bool ret = false;
    if (!m_details.empty()) {
        foreach (QDeclarativeOrganizerItemDetail *detail, m_details)
            delete detail;
        m_details.clear();
        m_modified = true;
        ret = true;
    }
    return ret;
}


/*!
    \qmltype Event
    \instantiates QDeclarativeOrganizerEvent
    \brief The Event element provides an event in time which may reoccur.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-items

    \sa OrganizerItem, EventOccurrence, Journal, Todo, TodoOccurrence, Note, {QOrganizerEvent}
*/

/*!
  \qmlsignal Event::onItemChanged()

  \sa OrganizerItem::onItemChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerEvent::QDeclarativeOrganizerEvent(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem(QOrganizerEvent());
}

/*!
    \qmlmethod void Event::setDetail(detail)

    Saves the given \a detail in the organizer event, and sets its id.
 */
void QDeclarativeOrganizerEvent::setDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (_q_setDetail(detail))
        emit valueChanged();
}

/*!
    \qmlmethod void Event::removeDetail(detail)

    Removes given \a detail from the organizer event.
 */
void QDeclarativeOrganizerEvent::removeDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (_q_removeDetail(detail))
        emit valueChanged();
}

/*!
    \qmlmethod Event::clearDetails()

    Removes all details from the organizer event.

    \sa removeDetail()
 */
void QDeclarativeOrganizerEvent::clearDetails()
{
    if (_q_clearDetails())
        emit valueChanged();
}

/*!
    \qmlproperty list<Detail> Event::attendees

    This property holds the attendees list of the event.
 */
QQmlListProperty<QDeclarativeOrganizerEventAttendee> QDeclarativeOrganizerEvent::attendees()
{
    return QQmlListProperty<QDeclarativeOrganizerEventAttendee>(this, 0,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_append,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_count,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_at,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_clear);
}

// call-back functions for list property
/*!
    \internal
 */
void QDeclarativeOrganizerEvent::_q_attendee_append(QQmlListProperty<QDeclarativeOrganizerEventAttendee> *property, QDeclarativeOrganizerEventAttendee *value)
{
    QDeclarativeOrganizerEvent *object = qobject_cast<QDeclarativeOrganizerEvent *>(property->object);
    if (object)
        object->setDetail(value);
}

/*!
    \internal
 */
QDeclarativeOrganizerEventAttendee *QDeclarativeOrganizerEvent::_q_attendee_at(QQmlListProperty<QDeclarativeOrganizerEventAttendee> *property, int index)
{
    QDeclarativeOrganizerEvent *object = qobject_cast<QDeclarativeOrganizerEvent *>(property->object);
    QDeclarativeOrganizerEventAttendee *ret = 0;
    int i = 0;
    foreach (QDeclarativeOrganizerItemDetail *detail, object->m_details) {
        if (QDeclarativeOrganizerItemDetail::EventAttendee == detail->type()) {
            if (i == index) {
                ret = qobject_cast<QDeclarativeOrganizerEventAttendee *>(detail);
                break;
            } else {
                ++i;
            }
        }
    }
    return ret;
}

/*!
    \internal
 */
void QDeclarativeOrganizerEvent::_q_attendee_clear(QQmlListProperty<QDeclarativeOrganizerEventAttendee> *property)
{
    QDeclarativeOrganizerEvent *object = qobject_cast<QDeclarativeOrganizerEvent *>(property->object);
    if (object) {
        int i = 0;
        bool removed = false;
        foreach (QDeclarativeOrganizerItemDetail *obj, object->m_details) {
            if (obj->type() == QDeclarativeOrganizerItemDetail::EventAttendee) {
                delete obj;
                object->m_details.removeAt(i);
                removed = true;
            } else {// Index should not increase if some thing is removed
                ++i;
            }
        }
        if (removed)
            emit object->valueChanged();
    }
}

/*!
    \internal
 */
int QDeclarativeOrganizerEvent::_q_attendee_count(QQmlListProperty<QDeclarativeOrganizerEventAttendee> *property)
{
    QDeclarativeOrganizerEvent *object = qobject_cast<QDeclarativeOrganizerEvent *>(property->object);
    int ret = 0;
    if (object) {
        foreach (QDeclarativeOrganizerItemDetail *detail, object->m_details) {
            if (QDeclarativeOrganizerItemDetail::EventAttendee == detail->type())
                ++ret;
        }
    }
    return ret;
}

/*!
    \qmlproperty bool Event::allDay

    This property indicates whether the time-of-day component of the event's start date-time or end date-time is
    insignificant. If allDay is true, the time-of-day component is considered insignificant, and the event will
    be an all-day item.
 */
void QDeclarativeOrganizerEvent::setAllDay(bool allDay)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type()) {
            QDeclarativeOrganizerEventTime *eventTime = static_cast<QDeclarativeOrganizerEventTime *>(detail);
            if (eventTime->isAllDay() != allDay) {
                eventTime->setAllDay(allDay);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerEventTime *eventTime = new QDeclarativeOrganizerEventTime(this);
    eventTime->setAllDay(allDay);
    m_details.append(eventTime);
    m_modified = true;
    emit valueChanged();
}

bool QDeclarativeOrganizerEvent::isAllDay() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type())
            return static_cast<QDeclarativeOrganizerEventTime *>(detail)->isAllDay();
    }
    return false;
}

/*!
    \qmlproperty date Event::startDateTime

    This property holds the start date time of the event.
 */
void QDeclarativeOrganizerEvent::setStartDateTime(const QDateTime &datetime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type()) {
            QDeclarativeOrganizerEventTime *eventTime = static_cast<QDeclarativeOrganizerEventTime *>(detail);
            if (eventTime->startDateTime() != datetime) {
                eventTime->setStartDateTime(datetime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerEventTime *eventTime = new QDeclarativeOrganizerEventTime(this);
    eventTime->setStartDateTime(datetime);
    m_details.append(eventTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerEvent::startDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type())
            return static_cast<QDeclarativeOrganizerEventTime *>(detail)->startDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty date Event::endDateTime

    This property holds the end date time of the event.
 */
void QDeclarativeOrganizerEvent::setEndDateTime(const QDateTime& datetime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type()) {
            QDeclarativeOrganizerEventTime *eventTime = static_cast<QDeclarativeOrganizerEventTime *>(detail);
            if (eventTime->endDateTime() != datetime) {
                eventTime->setEndDateTime(datetime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerEventTime *eventTime = new QDeclarativeOrganizerEventTime(this);
    eventTime->setEndDateTime(datetime);
    m_details.append(eventTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerEvent::endDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type())
            return static_cast<QDeclarativeOrganizerEventTime *>(detail)->endDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty enumeration Event::priority

    This property holds the priority of the event. The value can be one of:
    \list
    \li Priority.Unknown
    \li Priority.Highest
    \li Priority.ExtremelyHigh
    \li Priority.VeryHigh
    \li Priority.High
    \li Priority.Medium
    \li Priority.Low
    \li Priority.VeryLow
    \li Priority.ExtremelyLow
    \li Priority.Lowest
    \endlist
 */
void QDeclarativeOrganizerEvent::setPriority(QDeclarativeOrganizerItemPriority::Priority priority)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type()) {
            QDeclarativeOrganizerItemPriority *itemPriority = static_cast<QDeclarativeOrganizerItemPriority *>(detail);
            if (itemPriority->priority() != priority) {
                itemPriority->setPriority(priority);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemPriority *itemPriority = new QDeclarativeOrganizerItemPriority(this);
    itemPriority->setPriority(priority);
    m_details.append(itemPriority);
    m_modified = true;
    emit valueChanged();
}

QDeclarativeOrganizerItemPriority::Priority QDeclarativeOrganizerEvent::priority() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type())
            return static_cast<QDeclarativeOrganizerItemPriority *>(detail)->priority();
    }
    return QDeclarativeOrganizerItemPriority::Unknown;
}

/*!
    \qmlproperty string Event::location

    This property holds the label of the location at which the event occurs.
 */

void QDeclarativeOrganizerEvent::setLocation(const QString &location)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Location == detail->type()) {
            QDeclarativeOrganizerItemLocation *itemLocation = static_cast<QDeclarativeOrganizerItemLocation *>(detail);
            if (itemLocation->label() != location) {
                itemLocation->setLabel(location);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemLocation *itemLocation = new QDeclarativeOrganizerItemLocation(this);
    itemLocation->setLabel(location);
    m_details.append(itemLocation);
    m_modified = true;
    emit valueChanged();
}

QString QDeclarativeOrganizerEvent::location() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Location == detail->type())
            return static_cast<QDeclarativeOrganizerItemLocation *>(detail)->label();
    }
    return QString::null;
}

/*!
  \qmlproperty Recurrence Event::recurrence

  This property holds the recurrence element of the event item.
  */
QDeclarativeOrganizerItemRecurrence *QDeclarativeOrganizerEvent::recurrence()
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Recurrence == detail->type())
            return static_cast<QDeclarativeOrganizerItemRecurrence *>(detail);
    }

    QDeclarativeOrganizerItemRecurrence *detail = new QDeclarativeOrganizerItemRecurrence;
    m_details.append(detail);
    m_modified = true;
    emit valueChanged();
    return detail;
}


/*!
    \qmltype EventOccurrence
    \instantiates QDeclarativeOrganizerEventOccurrence
    \brief The EventOccurrence element provides an occurrence of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-items

    \sa OrganizerItem, Event, Journal, Todo, TodoOccurrence, Note, {QOrganizerEventOccurrence}
 */

/*!
  \qmlsignal EventOccurrence::onItemChanged()

  \sa OrganizerItem::onItemChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerEventOccurrence::QDeclarativeOrganizerEventOccurrence(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem (QOrganizerEventOccurrence());
}

/*!
    \qmlproperty date EventOccurrence::originalDate

    This property holds the date at which the occurrence was originally going to occur.
 */
void QDeclarativeOrganizerEventOccurrence::setOriginalDate(const QDateTime &date)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type()) {
            QDeclarativeOrganizerItemParent *parent = static_cast<QDeclarativeOrganizerItemParent *>(detail);
            if (parent->originalDate() != date) {
                parent->setOriginalDate(date);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemParent *parent = new QDeclarativeOrganizerItemParent(this);
    parent->setOriginalDate(date);
    m_details.append(parent);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerEventOccurrence::originalDate() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type())
            return static_cast<QDeclarativeOrganizerItemParent *>(detail)->originalDate();
    }
    return QDateTime();
}

/*!
    \qmlproperty date EventOccurrence::startDateTime

    This property holds the start date time of the event occurrence.
 */
void QDeclarativeOrganizerEventOccurrence::setStartDateTime(const QDateTime &datetime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type()) {
            QDeclarativeOrganizerEventTime *eventTime = static_cast<QDeclarativeOrganizerEventTime *>(detail);
            if (eventTime->startDateTime() != datetime) {
                eventTime->setStartDateTime(datetime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerEventTime *eventTime = new QDeclarativeOrganizerEventTime(this);
    eventTime->setStartDateTime(datetime);
    m_details.append(eventTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerEventOccurrence::startDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type())
            return static_cast<QDeclarativeOrganizerEventTime *>(detail)->startDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty date EventOccurrence::endDateTime

    This property holds the date time at which the event occurrence ends.
 */
void QDeclarativeOrganizerEventOccurrence::setEndDateTime(const QDateTime &datetime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type()) {
            QDeclarativeOrganizerEventTime *eventTime = static_cast<QDeclarativeOrganizerEventTime *>(detail);
            if (eventTime->endDateTime() != datetime) {
                eventTime->setEndDateTime(datetime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerEventTime *eventTime = new QDeclarativeOrganizerEventTime(this);
    eventTime->setEndDateTime(datetime);
    m_details.append(eventTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerEventOccurrence::endDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type())
            return static_cast<QDeclarativeOrganizerEventTime *>(detail)->endDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty enumeration EventOccurrence::priority

    This property holds the priority of the event occurrence. The value can be one of:
    \list
    \li Priority.Unknown
    \li Priority.Highest
    \li Priority.ExtremelyHigh
    \li Priority.VeryHigh
    \li Priority.High
    \li Priority.Medium
    \li Priority.Low
    \li Priority.VeryLow
    \li Priority.ExtremelyLow
    \li Priority.Lowest
    \endlist
 */
void QDeclarativeOrganizerEventOccurrence::setPriority(QDeclarativeOrganizerItemPriority::Priority priority)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type()) {
            QDeclarativeOrganizerItemPriority *itemPriority = static_cast<QDeclarativeOrganizerItemPriority *>(detail);
            if (itemPriority->priority() != priority) {
                itemPriority->setPriority(priority);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemPriority *itemPriority = new QDeclarativeOrganizerItemPriority(this);
    itemPriority->setPriority(priority);
    m_details.append(itemPriority);
    m_modified = true;
    emit valueChanged();
}

QDeclarativeOrganizerItemPriority::Priority QDeclarativeOrganizerEventOccurrence::priority() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type())
            return static_cast<QDeclarativeOrganizerItemPriority *>(detail)->priority();
    }
    return QDeclarativeOrganizerItemPriority::Unknown;
}

/*!
    \qmlproperty string EventOccurrence::location

    This property holds the label of the location at which the event occurrence is held.
 */
void QDeclarativeOrganizerEventOccurrence::setLocation(const QString &location)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Location == detail->type()) {
            QDeclarativeOrganizerItemLocation *itemLocation = static_cast<QDeclarativeOrganizerItemLocation *>(detail);
            if (itemLocation->label() != location) {
                itemLocation->setLabel(location);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemLocation *itemLocation = new QDeclarativeOrganizerItemLocation(this);
    itemLocation->setLabel(location);
    m_details.append(itemLocation);
    m_modified = true;
    emit valueChanged();
}

QString QDeclarativeOrganizerEventOccurrence::location() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Location == detail->type())
            return static_cast<QDeclarativeOrganizerItemLocation *>(detail)->label();
    }
    return QString::null;
}

/*!
    \qmlproperty int EventOccurrence::parentId

    This property holds the id of the event which is this occurrence's parent.
 */
void QDeclarativeOrganizerEventOccurrence::setParentId(const QString &parentId)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type()) {
            QDeclarativeOrganizerItemParent *parent = static_cast<QDeclarativeOrganizerItemParent *>(detail);
            if (parent->parentId() != parentId) {
                parent->setParentId(parentId);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemParent *parent = new QDeclarativeOrganizerItemParent(this);
    parent->setParentId(parentId);
    m_details.append(parent);
    m_modified = true;
    emit valueChanged();
}

QString QDeclarativeOrganizerEventOccurrence::parentId() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type())
            return static_cast<QDeclarativeOrganizerItemParent *>(detail)->parentId();
    }
    return QString::null;
}

/*!
    \qmlproperty bool EventOccurrence::allDay

    This property indicates whether the time-of-day component of the event occurrence's start date-time or end date-time is
    insignificant. If allDay is true, the time-of-day component is considered insignificant, and the event occurrence will
    be an all-day item.
 */
void QDeclarativeOrganizerEventOccurrence::setAllDay(bool allDay)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type()) {
            QDeclarativeOrganizerEventTime *eventTime = static_cast<QDeclarativeOrganizerEventTime *>(detail);
            if (eventTime->isAllDay() != allDay) {
                eventTime->setAllDay(allDay);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerEventTime *eventTime = new QDeclarativeOrganizerEventTime(this);
    eventTime->setAllDay(allDay);
    m_details.append(eventTime);
    m_modified = true;
    emit valueChanged();
}

bool QDeclarativeOrganizerEventOccurrence::isAllDay() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::EventTime == detail->type())
            return static_cast<QDeclarativeOrganizerEventTime *>(detail)->isAllDay();
    }
    return false;
}

/*!
    \qmltype Journal
    \instantiates QDeclarativeOrganizerJournal
    \brief The Journal element provides a journal which is associated with a particular point in time.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-items

    \sa OrganizerItem, Event, EventOccurrence, Todo, TodoOccurrence, Note, {QOrganizerJournal}
 */

/*!
  \qmlsignal Journal::onItemChanged()

  \sa OrganizerItem::onItemChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerJournal::QDeclarativeOrganizerJournal(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem (QOrganizerJournal());
}

/*!
    \qmlproperty date Journal::dateTime

    This property holds the date time associated with this journal.
 */
void QDeclarativeOrganizerJournal::setDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::JournalTime == detail->type()) {
            QDeclarativeOrganizerJournalTime *journalTime = static_cast<QDeclarativeOrganizerJournalTime *>(detail);
            if (journalTime->entryDateTime() != dateTime) {
                journalTime->setEntryDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerJournalTime *journalTime = new QDeclarativeOrganizerJournalTime(this);
    journalTime->setEntryDateTime(dateTime);
    m_details.append(journalTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerJournal::dateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::JournalTime == detail->type())
            return static_cast<QDeclarativeOrganizerJournalTime *>(detail)->entryDateTime();
    }
    return QDateTime();
}


/*!
    \qmltype Note
    \instantiates QDeclarativeOrganizerNote
    \brief The Note element provides a note which is not associated with any particular point in time.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-items

    \sa OrganizerItem, Event, EventOccurrence, Journal, Todo, TodoOccurrence, {QOrganizerNote}
 */

/*!
  \qmlsignal Note::onItemChanged()

  \sa OrganizerItem::onItemChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerNote::QDeclarativeOrganizerNote(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem(QOrganizerNote());
}


/*!
    \qmltype Todo
    \instantiates QDeclarativeOrganizerTodo
    \brief The Todo element provides a task which should be completed.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-items

    \sa OrganizerItem, Event, EventOccurrence, Journal, TodoOccurrence, Note, {QOrganizerTodo}
*/

/*!
  \qmlsignal Todo::onItemChanged()

  \sa OrganizerItem::onItemChanged
*/

/*!
    \internal
 */
QDeclarativeOrganizerTodo::QDeclarativeOrganizerTodo(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem(QOrganizerTodo());
}

/*!
    \qmlproperty bool Todo::allDay

    This property indicates whether the time-of-day component of the Todo's start date-time or due date-time is
    insignificant. If allDay is true, the time-of-day component is considered insignificant, and the todo will
    be an all-day item.
 */
void QDeclarativeOrganizerTodo::setAllDay(bool allDay)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type()) {
            QDeclarativeOrganizerTodoTime *todoTime = static_cast<QDeclarativeOrganizerTodoTime *>(detail);
            if (todoTime->isAllDay() != allDay) {
                todoTime->setAllDay(allDay);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoTime *todoTime = new QDeclarativeOrganizerTodoTime(this);
    todoTime->setAllDay(allDay);
    m_details.append(todoTime);
    m_modified = true;
    emit valueChanged();
}

bool QDeclarativeOrganizerTodo::isAllDay() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type())
            return static_cast<QDeclarativeOrganizerTodoTime *>(detail)->isAllDay();
    }
    return false;
}

/*!
    \qmlproperty int Todo::percentageComplete

    This property holds the percentage of progress completed on the task described by the todo item.
 */
void QDeclarativeOrganizerTodo::setPercentageComplete(int percentageComplete)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->percentageComplete() != percentageComplete) {
                todoProgress->setPercentageComplete(percentageComplete);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setPercentageComplete(percentageComplete);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

int QDeclarativeOrganizerTodo::percentageComplete() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->percentageComplete();
    }
    return 0;
}

/*!
    \qmlproperty date Todo::startDateTime

    This property holds the date time at which the task should be started.
 */
void QDeclarativeOrganizerTodo::setStartDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type()) {
            QDeclarativeOrganizerTodoTime *todoTime = static_cast<QDeclarativeOrganizerTodoTime *>(detail);
            if (todoTime->startDateTime() != dateTime) {
                todoTime->setStartDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoTime *todoTime = new QDeclarativeOrganizerTodoTime(this);
    todoTime->setStartDateTime(dateTime);
    m_details.append(todoTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodo::startDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type())
            return static_cast<QDeclarativeOrganizerTodoTime *>(detail)->startDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty date Todo::dueDateTime

    This property holds the date time by which the task should be completed.
 */
void QDeclarativeOrganizerTodo::setDueDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type()) {
            QDeclarativeOrganizerTodoTime *todoTime = static_cast<QDeclarativeOrganizerTodoTime *>(detail);
            if (todoTime->dueDateTime() != dateTime) {
                todoTime->setDueDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoTime *todoTime = new QDeclarativeOrganizerTodoTime(this);
    todoTime->setDueDateTime(dateTime);
    m_details.append(todoTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodo::dueDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type())
            return static_cast<QDeclarativeOrganizerTodoTime *>(detail)->dueDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty date Todo::finishedDateTime

    This property holds the date and time at which the task was completed, if known.
 */
void QDeclarativeOrganizerTodo::setFinishedDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->finishedDateTime() != dateTime) {
                todoProgress->setFinishedDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setFinishedDateTime(dateTime);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodo::finishedDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->finishedDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty enumeration Todo::priority

    This property holds the priority of the todo item. The value can be one of:
    \list
    \li Priority.Unknown
    \li Priority.Highest
    \li Priority.ExtremelyHigh
    \li Priority.VeryHigh
    \li Priority.High
    \li Priority.Medium
    \li Priority.Low
    \li Priority.VeryLow
    \li Priority.ExtremelyLow
    \li Priority.Lowest
    \endlist
 */
void QDeclarativeOrganizerTodo::setPriority(QDeclarativeOrganizerItemPriority::Priority priority)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type()) {
            QDeclarativeOrganizerItemPriority *itemPriority = static_cast<QDeclarativeOrganizerItemPriority *>(detail);
            if (itemPriority->priority() != priority) {
                itemPriority->setPriority(priority);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemPriority *itemPriority = new QDeclarativeOrganizerItemPriority(this);
    itemPriority->setPriority(priority);
    m_details.append(itemPriority);
    m_modified = true;
    emit valueChanged();
}

QDeclarativeOrganizerItemPriority::Priority QDeclarativeOrganizerTodo::priority() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type())
            return static_cast<QDeclarativeOrganizerItemPriority *>(detail)->priority();
    }
    return QDeclarativeOrganizerItemPriority::Unknown;
}

/*!
    \qmlproperty enumeration Todo::status

    This property holds the progress status of the task described by the todo. The value can be one of:
    \list
    \li TodoProgress.NotStarted
    \li TodoProgress.InProgress
    \li TodoProgress.Complete
    \endlist
 */
void QDeclarativeOrganizerTodo::setStatus(QDeclarativeOrganizerTodoProgress::StatusType status)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->status() != status) {
                todoProgress->setStatus(status);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setStatus(status);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

QDeclarativeOrganizerTodoProgress::StatusType QDeclarativeOrganizerTodo::status() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->status();
    }
    return QDeclarativeOrganizerTodoProgress::NotStarted;
}

/*!
    \qmlproperty Recurrence Todo::recurrence

    This property holds the recurrence element of the todo item.
 */
QDeclarativeOrganizerItemRecurrence* QDeclarativeOrganizerTodo::recurrence()
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Recurrence == detail->type())
            return static_cast<QDeclarativeOrganizerItemRecurrence *>(detail);
    }

    QDeclarativeOrganizerItemRecurrence *detail = new QDeclarativeOrganizerItemRecurrence;
    m_details.append(detail);
    m_modified = true;
    emit valueChanged();
    return detail;
}


/*!
    \qmltype TodoOccurrence
    \instantiates QDeclarativeOrganizerTodoOccurrence
    \brief The TodoOccurrence element provides an occurrence of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-items

    \sa OrganizerItem, Event, EventOccurrence, Journal, Todo, Note, {QOrganizerTodoOccurrence}
 */

/*!
  \qmlsignal TodoOccurrence::onItemChanged()

  \sa OrganizerItem::onItemChanged
*/

QDeclarativeOrganizerTodoOccurrence::QDeclarativeOrganizerTodoOccurrence(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem(QOrganizerTodoOccurrence());
}

/*!
    \qmlproperty bool TodoOccurrence::allDay

    This property indicates whether the time-of-day component of the todo occurrence's start date-time or due date-time is
    insignificant. If allDay is true, the time-of-day component is considered insignificant, and the todo occurrence will
    be an all-day item.
 */
void QDeclarativeOrganizerTodoOccurrence::setAllDay(bool allDay)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type()) {
            QDeclarativeOrganizerTodoTime *todoTime = static_cast<QDeclarativeOrganizerTodoTime *>(detail);
            if (todoTime->isAllDay() != allDay) {
                todoTime->setAllDay(allDay);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoTime *todoTime = new QDeclarativeOrganizerTodoTime(this);
    todoTime->setAllDay(allDay);
    m_details.append(todoTime);
    m_modified = true;
    emit valueChanged();
}

bool QDeclarativeOrganizerTodoOccurrence::isAllDay() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type())
            return static_cast<QDeclarativeOrganizerTodoTime *>(detail)->isAllDay();
    }
    return false;
}

/*!
    \qmlproperty int TodoOccurrence::percentageComplete

    This property holds the percentage of progress completed on the task described by the todo item.
 */
void QDeclarativeOrganizerTodoOccurrence::setPercentageComplete(int percentageComplete)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->percentageComplete() != percentageComplete) {
                todoProgress->setPercentageComplete(percentageComplete);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setPercentageComplete(percentageComplete);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

int QDeclarativeOrganizerTodoOccurrence::percentageComplete() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->percentageComplete();
    }
    return 0;
}

/*!
    \qmlproperty date TodoOccurrence::originalDate

    This property holds the date at which the occurrence was originally going to occur.
 */
void QDeclarativeOrganizerTodoOccurrence::setOriginalDate(const QDateTime &date)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type()) {
            QDeclarativeOrganizerItemParent *parent = static_cast<QDeclarativeOrganizerItemParent *>(detail);
            if (parent->originalDate() != date) {
                parent->setOriginalDate(date);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemParent *parent = new QDeclarativeOrganizerItemParent(this);
    parent->setOriginalDate(date);
    m_details.append(parent);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodoOccurrence::originalDate() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type())
            return static_cast<QDeclarativeOrganizerItemParent *>(detail)->originalDate();
    }
    return QDateTime();
}

/*!
    \qmlproperty date TodoOccurrence::startDateTime

    This property holds the date time at which the task should be started.
 */
void QDeclarativeOrganizerTodoOccurrence::setStartDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type()) {
            QDeclarativeOrganizerTodoTime *todoTime = static_cast<QDeclarativeOrganizerTodoTime *>(detail);
            if (todoTime->startDateTime() != dateTime) {
                todoTime->setStartDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoTime *todoTime = new QDeclarativeOrganizerTodoTime(this);
    todoTime->setStartDateTime(dateTime);
    m_details.append(todoTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodoOccurrence::startDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type())
            return static_cast<QDeclarativeOrganizerTodoTime *>(detail)->startDateTime();
    }
    return QDateTime();
}
/*!
    \qmlproperty date TodoOccurrence::dueDateTime

    This property holds the date time by which the task should be completed.
 */
void QDeclarativeOrganizerTodoOccurrence::setDueDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type()) {
            QDeclarativeOrganizerTodoTime *todoTime = static_cast<QDeclarativeOrganizerTodoTime *>(detail);
            if (todoTime->dueDateTime() != dateTime) {
                todoTime->setDueDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoTime *todoTime = new QDeclarativeOrganizerTodoTime(this);
    todoTime->setDueDateTime(dateTime);
    m_details.append(todoTime);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodoOccurrence::dueDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoTime == detail->type())
            return static_cast<QDeclarativeOrganizerTodoTime *>(detail)->dueDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty date TodoOccurrence::finishedDateTime

    This property holds the date and time at which the task was completed, if known.
 */
void QDeclarativeOrganizerTodoOccurrence::setFinishedDateTime(const QDateTime &dateTime)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->finishedDateTime() != dateTime) {
                todoProgress->setFinishedDateTime(dateTime);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setFinishedDateTime(dateTime);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

QDateTime QDeclarativeOrganizerTodoOccurrence::finishedDateTime() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->finishedDateTime();
    }
    return QDateTime();
}

/*!
    \qmlproperty enumeration TodoOccurrence::priority

    This property holds the priority of the todo occurrence. The value can be one of:
    \list
    \li Priority.Unknown
    \li Priority.Highest
    \li Priority.ExtremelyHigh
    \li Priority.VeryHigh
    \li Priority.High
    \li Priority.Medium
    \li Priority.Low
    \li Priority.VeryLow
    \li Priority.ExtremelyLow
    \li Priority.Lowest
    \endlist
 */
void QDeclarativeOrganizerTodoOccurrence::setPriority(QDeclarativeOrganizerItemPriority::Priority priority)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type()) {
            QDeclarativeOrganizerItemPriority *itemPriority = static_cast<QDeclarativeOrganizerItemPriority *>(detail);
            if (itemPriority->priority() != priority) {
                itemPriority->setPriority(priority);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemPriority *itemPriority = new QDeclarativeOrganizerItemPriority(this);
    itemPriority->setPriority(priority);
    m_details.append(itemPriority);
    m_modified = true;
    emit valueChanged();
}

QDeclarativeOrganizerItemPriority::Priority QDeclarativeOrganizerTodoOccurrence::priority() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Priority == detail->type())
            return static_cast<QDeclarativeOrganizerItemPriority *>(detail)->priority();
    }
    return QDeclarativeOrganizerItemPriority::Unknown;
}

/*!
    \qmlproperty enumeration TodoOccurrence::status

    This property holds the progress status of the task described by the todo occurrence. The value can be one of:
    \list
    \li TodoProgress.NotStarted
    \li TodoProgress.InProgress
    \li TodoProgress.Complete
    \endlist
 */
void QDeclarativeOrganizerTodoOccurrence::setStatus(QDeclarativeOrganizerTodoProgress::StatusType status)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->status() != status) {
                todoProgress->setStatus(status);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setStatus(status);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

QDeclarativeOrganizerTodoProgress::StatusType QDeclarativeOrganizerTodoOccurrence::status() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->status();
    }
    return QDeclarativeOrganizerTodoProgress::NotStarted;
}

/*!
    \qmlproperty int TodoOccurrence::parentId

    This property holds the id of the todo which is this occurrence's parent.
 */
void QDeclarativeOrganizerTodoOccurrence::setParentId(const QString &parentId)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type()) {
            QDeclarativeOrganizerItemParent *parent = static_cast<QDeclarativeOrganizerItemParent *>(detail);
            if (parent->parentId() != parentId) {
                parent->setParentId(parentId);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerItemParent *parent = new QDeclarativeOrganizerItemParent(this);
    parent->setParentId(parentId);
    m_details.append(parent);
    m_modified = true;
    emit valueChanged();
}

QString QDeclarativeOrganizerTodoOccurrence::parentId() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type())
            return static_cast<QDeclarativeOrganizerItemParent *>(detail)->parentId();
    }
    return QString::null;
}

#include "moc_qdeclarativeorganizeritem_p.cpp"

QT_END_NAMESPACE
