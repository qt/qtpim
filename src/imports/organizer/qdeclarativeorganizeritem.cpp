/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "qdeclarativeorganizeritem_p.h"
#include "qdeclarativeorganizermodel_p.h"

#include <QtDeclarative/qdeclarativeengine.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass OrganizerItem QDeclarativeOrganizerItem

    \brief The OrganizerItem element represents the in-memory version of a organizer item.

    \ingroup qml-organizer

    A OrganizerItem has a number of mandatory details. Different subclasses of OrganizerItem
    (i.e., Event, EventOccurrence, Journal, Todo, TodoOccurrence, Note) may have more mandatory details.

    Most frequently-used details can also be accessed through convenient properties, e.g. displayLabel,
    while all details can be accessed through detail(), details(), saveDetail(), among others.

    \sa Event, EventOccurrence, Journal, Todo, TodoOccurrence, Note, {QOrganizerManager}, {QOrganizerItem}

    The OrganizerItem element is part of the \bold{QtOrganizer 5.0} module.
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

    \sa OrganizerItem::save
 */
bool QDeclarativeOrganizerItem::modified() const
{
    return m_modified;
}

/*!
    \qmlproperty enum OrganizerItem::itemType

    This property holds the type of the OrganizerItem object.
 */
QDeclarativeOrganizerItemType::OrganizerItemType QDeclarativeOrganizerItem::itemType() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Type == detail->type())
            return static_cast<QDeclarativeOrganizerItemType *>(detail)->itemType();
    }
    return QDeclarativeOrganizerItemType::Customized;
}

/*!
    \qmlproperty list<Detail> OrganizerItem::itemDetails

    This property holds the details of the OrganizerItem object.
 */
QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> QDeclarativeOrganizerItem::itemDetails()
{
    return QDeclarativeListProperty<QDeclarativeOrganizerItemDetail>(this, 0,
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
    \qmlmethod Detail OrganizerItem::detail(Detail::ItemDetailType)

    Returns the first detail stored in the organizer item with the given \a type.
 */
QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItem::detail(int type)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (type == detail->type()) {
            QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail->type());
            QDeclarativeEngine::setObjectOwnership(itemDetail, QDeclarativeEngine::JavaScriptOwnership);
            itemDetail->setDetail(detail->detail());
            return itemDetail;
        }
    }
    return 0;
}

/*!
    \qmlmethod list<Detail> OrganizerItem::details(Detail::ItemDetailType)

    Returns all the details stored in the organizer item with the given \a type.
 */
QVariantList QDeclarativeOrganizerItem::details(int type)
{
    QVariantList list;
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (type == detail->type()) {
            QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail->type());
            QDeclarativeEngine::setObjectOwnership(itemDetail, QDeclarativeEngine::JavaScriptOwnership);
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

    \sa OrganizerItem::removeDetail()
 */
void QDeclarativeOrganizerItem::clearDetails()
{
    if (_q_clearDetails())
        emit itemChanged();
}

/*!
    \qmlmethod OrganizerItem::save()

    Saves this OrganizerItem if the item has been modified.

    \sa  OrganizerItem::modified
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
        QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail.definitionName());
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

// call-back functions for list property
/*!
    \internal
 */
void QDeclarativeOrganizerItem::_q_detail_append(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property, QDeclarativeOrganizerItemDetail *value)
{
    QDeclarativeOrganizerItem *object = qobject_cast<QDeclarativeOrganizerItem *>(property->object);
    if (object)
        object->m_details.append(value);
}

/*!
    \internal
 */
QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItem::_q_detail_at(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property, int index)
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
void QDeclarativeOrganizerItem::_q_detail_clear(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property)
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
int QDeclarativeOrganizerItem::_q_detail_count(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property)
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
    if (!detail->removable())
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

// to be removed
/*!
    WARNING: This is to be removed soon.
 */
QString QDeclarativeOrganizerItem::type() const
{
    QDeclarativeOrganizerItemType::OrganizerItemType type = itemType();
    switch (type) {
    case QDeclarativeOrganizerItemType::Event:
        return QOrganizerItemType::TypeEvent;
    case QDeclarativeOrganizerItemType::EventOccurrence:
        return QOrganizerItemType::TypeEventOccurrence;
    case QDeclarativeOrganizerItemType::Todo:
        return QOrganizerItemType::TypeTodo;
    case QDeclarativeOrganizerItemType::TodoOccurrence:
        return QOrganizerItemType::TypeTodoOccurrence;
    case QDeclarativeOrganizerItemType::Journal:
        return QOrganizerItemType::TypeJournal;
    case QDeclarativeOrganizerItemType::Note:
        return QOrganizerItemType::TypeNote;
    default:
        return QOrganizerItemType::TypeNote; // XX Customized
    }
}

/*!
    WARNING: This is to be removed soon.
 */
bool QDeclarativeOrganizerItem::isFloatingTime()
{
    switch (itemType()) {
    case QDeclarativeOrganizerItemType::Event:
        return static_cast<QDeclarativeOrganizerEvent *>(this)->startDateTime().timeSpec() == Qt::LocalTime;
    case QDeclarativeOrganizerItemType::EventOccurrence:
        return static_cast<QDeclarativeOrganizerEventOccurrence *>(this)->startDateTime().timeSpec() == Qt::LocalTime;
    case QDeclarativeOrganizerItemType::Todo:
        return static_cast<QDeclarativeOrganizerTodo *>(this)->startDateTime().timeSpec() == Qt::LocalTime;
    case QDeclarativeOrganizerItemType::TodoOccurrence:
        return static_cast<QDeclarativeOrganizerTodoOccurrence *>(this)->startDateTime().timeSpec() == Qt::LocalTime;
    case QDeclarativeOrganizerItemType::Journal:
        return static_cast<QDeclarativeOrganizerJournal *>(this)->dateTime().timeSpec() == Qt::LocalTime;
    case QDeclarativeOrganizerItemType::Note:
    default:
        break;
    }

    QDeclarativeOrganizerItemTimestamp *timestamp = static_cast<QDeclarativeOrganizerItemTimestamp *>(detail(QDeclarativeOrganizerItemDetail::Timestamp));
    bool floatingTime = timestamp->created().timeSpec() == Qt::LocalTime;
    delete timestamp;
    return floatingTime;
}

/*!
    WARNING: This is to be removed soon.
 */
QDateTime QDeclarativeOrganizerItem::itemStartTime() const
{
    QDeclarativeOrganizerItemType::OrganizerItemType type = itemType();
    switch (type) {
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
    WARNING: This is to be removed soon.
 */
QDateTime QDeclarativeOrganizerItem::itemEndTime() const
{
    QDeclarativeOrganizerItemType::OrganizerItemType type = itemType();
    switch (type) {
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

/*!
    WARNING: This is to be removed soon.
 */
bool QDeclarativeOrganizerItem::addDetail(QDeclarativeOrganizerItemDetail *detail)
{
    if (!detail)
        return false;

    QDeclarativeOrganizerItemDetail *itemDetail = QDeclarativeOrganizerItemDetailFactory::createItemDetail(detail->type());
    itemDetail->setDetail(detail->detail());
    m_details.append(itemDetail);

    m_modified = true;
    emit itemChanged();
    return true;
}

/*!
    WARNING: This is to be removed soon.
 */
bool QDeclarativeOrganizerItem::isOccurrence() const
{
    QDeclarativeOrganizerItemType::OrganizerItemType type = itemType();
    return type == QDeclarativeOrganizerItemType::EventOccurrence || type == QDeclarativeOrganizerItemType::TodoOccurrence;
}


/*!
    \qmlclass Event QDeclarativeOrganizerEvent
    \brief The Event element provides an event in time which may reoccur.
    \ingroup qml-organizer

    The Event element is part of the \bold{QtOrganizer 5.0} module.

    \sa OrganizerItem, EventOccurrence, Journal, Todo, TodoOccurrence, Note, {QOrganizerEvent}
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

    \sa Event::removeDetail()
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
QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> QDeclarativeOrganizerEvent::attendees()
{
    return QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee>(this, 0,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_append,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_count,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_at,
                                                                     &QDeclarativeOrganizerEvent::_q_attendee_clear);
}

// call-back functions for list property
/*!
    \internal
 */
void QDeclarativeOrganizerEvent::_q_attendee_append(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property, QDeclarativeOrganizerEventAttendee *value)
{
    QDeclarativeOrganizerEvent *object = qobject_cast<QDeclarativeOrganizerEvent *>(property->object);
    if (object)
        object->setDetail(value);
}

/*!
    \internal
 */
QDeclarativeOrganizerEventAttendee *QDeclarativeOrganizerEvent::_q_attendee_at(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property, int index)
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
void QDeclarativeOrganizerEvent::_q_attendee_clear(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property)
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
int QDeclarativeOrganizerEvent::_q_attendee_count(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property)
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
    \o Priority.Unknown
    \o Priority.Highest
    \o Priority.ExtremelyHigh
    \o Priority.VeryHigh
    \o Priority.High
    \o Priority.Medium
    \o Priority.Low
    \o Priority.VeryLow
    \o Priority.ExtremelyLow
    \o Priority.Lowest
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
    return detail;
}


/*!
    \qmlclass EventOccurrence QDeclarativeOrganizerEventOccurrence
    \brief The EventOccurrence element provides an occurrence of an event.
    \ingroup qml-organizer

    The EventOccurrence element is part of the \bold{QtOrganizer 5.0} module.
    \sa OrganizerItem, Event, Journal, Todo, TodoOccurrence, Note, {QOrganizerEventOccurrence}
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
void QDeclarativeOrganizerEventOccurrence::setOriginalDate(const QDate &date)
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

QDate QDeclarativeOrganizerEventOccurrence::originalDate() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type())
            return static_cast<QDeclarativeOrganizerItemParent *>(detail)->originalDate();
    }
    return QDate();
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
    \o Priority.Unknown
    \o Priority.Highest
    \o Priority.ExtremelyHigh
    \o Priority.VeryHigh
    \o Priority.High
    \o Priority.Medium
    \o Priority.Low
    \o Priority.VeryLow
    \o Priority.ExtremelyLow
    \o Priority.Lowest
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
    \qmlclass Journal QDeclarativeOrganizerJournal
    \brief The Journal element provides a journal which is associated with a particular point in time.
    \ingroup qml-organizer

    The Journal element is part of the \bold{QtOrganizer 5.0} module.
    \sa OrganizerItem, Event, EventOccurrence, Todo, TodoOccurrence, Note, {QOrganizerJournal}
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
    \qmlclass Note QDeclarativeOrganizerNote
    \brief The Note element provides a note which is not associated with any particular point in time.
    \ingroup qml-organizer

    The Note element is part of the \bold{QtOrganizer 5.0} module.
    \sa OrganizerItem, Event, EventOccurrence, Journal, Todo, TodoOccurrence, {QOrganizerNote}
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
    \qmlclass Todo QDeclarativeOrganizerTodo
    \brief The Todo element provides a task which should be completed.
    \ingroup qml-organizer

    The Todo element is part of the \bold{QtOrganizer 5.0} module.
    \sa OrganizerItem, Event, EventOccurrence, Journal, TodoOccurrence, Note, {QOrganizerTodo}
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
    \qmlproperty int Todo::progressPercentage

    This property holds the percentage of progress completed on the task described by the todo item.
 */
void QDeclarativeOrganizerTodo::setProgressPercentage(int percentage)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->percentage() != percentage) {
                todoProgress->setPercentage(percentage);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setPercentage(percentage);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

int QDeclarativeOrganizerTodo::progressPercentage() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->percentage();
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
    \o Priority.Unknown
    \o Priority.Highest
    \o Priority.ExtremelyHigh
    \o Priority.VeryHigh
    \o Priority.High
    \o Priority.Medium
    \o Priority.Low
    \o Priority.VeryLow
    \o Priority.ExtremelyLow
    \o Priority.Lowest
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
    \o TodoProgress.NotStarted
    \o TodoProgress.InProgress
    \o TodoProgress.Complete
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
    return detail;
}


/*!
    \qmlclass TodoOccurrence QDeclarativeOrganizerTodoOccurrence
    \brief The TodoOccurrence element provides an occurrence of an event.
    \ingroup qml-organizer

    The TodoOccurrence element is part of the \bold{QtOrganizer 5.0} module.
    \sa OrganizerItem, Event, EventOccurrence, Journal, Todo, Note, {QOrganizerTodoOccurrence}
 */
QDeclarativeOrganizerTodoOccurrence::QDeclarativeOrganizerTodoOccurrence(QObject *parent)
    : QDeclarativeOrganizerItem(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(itemChanged()));
    setItem(QOrganizerTodoOccurrence());
}

/*!
    \qmlproperty int TodoOccurrence::progressPercentage

    This property holds the percentage of progress completed on the task described by the todo item.
 */
void QDeclarativeOrganizerTodoOccurrence::setProgressPercentage(int percentage)
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type()) {
            QDeclarativeOrganizerTodoProgress *todoProgress = static_cast<QDeclarativeOrganizerTodoProgress *>(detail);
            if (todoProgress->percentage() != percentage) {
                todoProgress->setPercentage(percentage);
                m_modified = true;
                emit valueChanged();
            }
            return;
        }
    }

    QDeclarativeOrganizerTodoProgress *todoProgress = new QDeclarativeOrganizerTodoProgress(this);
    todoProgress->setPercentage(percentage);
    m_details.append(todoProgress);
    m_modified = true;
    emit valueChanged();
}

int QDeclarativeOrganizerTodoOccurrence::progressPercentage() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::TodoProgress == detail->type())
            return static_cast<QDeclarativeOrganizerTodoProgress *>(detail)->percentage();
    }
    return 0;
}

/*!
    \qmlproperty date TodoOccurrence::originalDate

    This property holds the date at which the occurrence was originally going to occur.
 */
void QDeclarativeOrganizerTodoOccurrence::setOriginalDate(const QDate &date)
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

QDate QDeclarativeOrganizerTodoOccurrence::originalDate() const
{
    foreach (QDeclarativeOrganizerItemDetail *detail, m_details) {
        if (QDeclarativeOrganizerItemDetail::Parent == detail->type())
            return static_cast<QDeclarativeOrganizerItemParent *>(detail)->originalDate();
    }
    return QDate();
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
    \o Priority.Unknown
    \o Priority.Highest
    \o Priority.ExtremelyHigh
    \o Priority.VeryHigh
    \o Priority.High
    \o Priority.Medium
    \o Priority.Low
    \o Priority.VeryLow
    \o Priority.ExtremelyLow
    \o Priority.Lowest
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
    \o TodoProgress.NotStarted
    \o TodoProgress.InProgress
    \o TodoProgress.Complete
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

QTORGANIZER_END_NAMESPACE
