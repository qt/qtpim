/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qdeclarativeorganizeritemdetail_p.h"
#include <QtDeclarative/qdeclarativeinfo.h>
#include <qorganizeritemdetails.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass Detail QDeclarativeOrganizerItemDetail
    \brief The Detail element represents a single, complete detail about a organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main

    \sa QOrganizerItemDetail
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemDetail::QDeclarativeOrganizerItemDetail(QObject *parent)
    : QObject(parent)
{
}

/*!
    \internal
 */
QDeclarativeOrganizerItemDetail::~QDeclarativeOrganizerItemDetail()
{
}

/*!
    \qmlproperty enumeration Detail::type

    This property holds the type of the detail and is read only. It can be one of:

    \list
    \o Detail.Undefined
    \o Detail.Classification
    \o Detail.Comment
    \o Detail.Description
    \o Detail.DisplayLabel
    \o Detail.ItemType
    \o Detail.Guid
    \o Detail.Location
    \o Detail.Parent
    \o Detail.Priority
    \o Detail.Recurrence
    \o Detail.Tag
    \o Detail.Timestamp
    \o Detail.Reminder
    \o Detail.AudibleReminder
    \o Detail.EmailReminder
    \o Detail.VisualReminder
    \o Detail.ExtendedDetail
    \o Detail.EventAttendee
    \o Detail.EventRsvp
    \o Detail.EventTime
    \o Detail.JournalTime
    \o Detail.TodoTime
    \o Detail.TodoProgress
    \endlist
 */
QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDetail::type() const
{
    return Undefined;
}

/*!
    \qmlproperty list<int> Detail::fieldNames

    This property holds the list of all field names which this detail currently contains. Thse names are
    used as the key for value(), setValue() and removeValue().

    This property is read only.
 */
QList<int> QDeclarativeOrganizerItemDetail::fieldNames() const
{
    return m_detail.values().keys();
}

/*!
    \qmlmethod variant Detail::value(field)

    Returns the value stored in this detail for the given \a field, or an empty variant if not available.
 */
QVariant QDeclarativeOrganizerItemDetail::value(int field) const
{
    return m_detail.value(field);
}

/*!
    \qmlmethod bool Detail::setValue(field, value)

    Inserts \a value into the detail for the given \a key if value is valid. If value is invalid, removes
    the field with the given key from the detail. Returns true if the given value was set for the key (if
    the value was valid), or if the given key was removed from detail (if the value was invalid), otherwise
    returns false if the key was unable to be removed (and the value was invalid).
 */
bool QDeclarativeOrganizerItemDetail::setValue(int field, const QVariant &value)
{
    bool ok = m_detail.setValue(field, value);
    if (ok)
        emit detailChanged();
    return ok;
}

/*!
    \qmlmethod bool Detail::removeValue(field)

    Removes the value stored in this detail for the given \a field. Returns true if a value was stored for
    the given key and the operation succeeded, and false otherwise.
 */
bool QDeclarativeOrganizerItemDetail::removeValue(int field)
{
    bool ok = m_detail.removeValue(field);
    if (ok)
        emit detailChanged();
    return ok;
}

// non-QML APIs
/*!
    \internal
 */
QOrganizerItemDetail QDeclarativeOrganizerItemDetail::detail() const
{
    return m_detail;
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemDetail::setDetail(const QOrganizerItemDetail &detail)
{
    m_detail = detail;
    emit detailChanged();
}


/*!
    \qmlclass EventTime QDeclarativeOrganizerEventTime
    \brief The EventTime element contains the start and end dates and times of a recurring event series, or occurrence of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o EventTime.FieldStartDateTime
    \o EventTime.FieldEndDateTime
    \o EventTime.FieldAllDay
    \endlist
 */
QDeclarativeOrganizerEventTime::QDeclarativeOrganizerEventTime(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerEventTime());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerEventTime::type() const
{
    return QDeclarativeOrganizerItemDetail::EventTime;
}

/*!
    \qmlproperty date EventTime::allDay

    This property holds whether the time is significant in the start datetime.
 */
void QDeclarativeOrganizerEventTime::setAllDay(bool allDay)
{
    if (allDay != isAllDay()) {
        m_detail.setValue(QOrganizerEventTime::FieldAllDay, allDay);
        emit valueChanged();
    }
}

bool QDeclarativeOrganizerEventTime::isAllDay()
{
    return m_detail.value<bool>(QOrganizerEventTime::FieldAllDay);
}

/*!
    \qmlproperty date EventTime::startDateTime

    This property holds the start date and time value of the event.
 */
void QDeclarativeOrganizerEventTime::setStartDateTime(const QDateTime &datetime)
{
     if (datetime != startDateTime()) {
        m_detail.setValue(QOrganizerEventTime::FieldStartDateTime, datetime.toUTC());
        emit valueChanged();
     }
}

QDateTime QDeclarativeOrganizerEventTime::startDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerEventTime::FieldStartDateTime).toLocalTime();
}

/*!
    \qmlproperty date EventTime::endDateTime

    This property holds the end date and time value of the event.
 */
void QDeclarativeOrganizerEventTime::setEndDateTime(const QDateTime &datetime)
{
    if (datetime != endDateTime()) {
        m_detail.setValue(QOrganizerEventTime::FieldEndDateTime, datetime.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerEventTime::endDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerEventTime::FieldEndDateTime).toLocalTime();
}


/*!
    \qmlclass Comment QDeclarativeOrganizerItemComment
    \brief The Comment element contains the comment text of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Comment.FieldComment
    \endlist
 */
QDeclarativeOrganizerItemComment::QDeclarativeOrganizerItemComment(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemComment());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemComment::type() const
{
    return QDeclarativeOrganizerItemDetail::Comment;
}

/*!
    \qmlproperty string Comment::comment

    This property holds the text of the comment.
 */
void QDeclarativeOrganizerItemComment::setComment(const QString &newComment)
{
    if (newComment != comment()) {
        m_detail.setValue(QOrganizerItemComment::FieldComment, newComment);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemComment::comment() const
{
    return m_detail.value(QOrganizerItemComment::FieldComment).toString();
}


/*!
    \qmlclass Description QDeclarativeOrganizerItemDescription
    \brief The Description element contains the description text of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Description.FieldDescription
    \endlist
 */
QDeclarativeOrganizerItemDescription::QDeclarativeOrganizerItemDescription(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemDescription());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDescription::type() const
{
    return QDeclarativeOrganizerItemDetail::Description;
}

/*!
    \qmlproperty string Description::description

    This property holds the text of the description.
 */
void QDeclarativeOrganizerItemDescription::setDescription(const QString &desc)
{
    if (desc != description()) {
        m_detail.setValue(QOrganizerItemDescription::FieldDescription, desc);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemDescription::description() const
{
    return m_detail.value(QOrganizerItemDescription::FieldDescription).toString();
}


/*!
    \qmlclass DisplayLabel QDeclarativeOrganizerItemDisplayLabel
    \brief The DisplayLabel element contains the display label of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o DisplayLabel.FieldLabel
    \endlist
 */
QDeclarativeOrganizerItemDisplayLabel::QDeclarativeOrganizerItemDisplayLabel(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemDisplayLabel());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDisplayLabel::type() const
{
    return QDeclarativeOrganizerItemDetail::DisplayLabel;
}

/*!
    \qmlproperty string DisplayLabel::displayLabel

    This property holds the display label text.
 */
void QDeclarativeOrganizerItemDisplayLabel::setLabel(const QString &newLabel)
{
    if (newLabel != label()) {
        m_detail.setValue(QOrganizerItemDisplayLabel::FieldLabel, newLabel);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemDisplayLabel::label() const
{
    return m_detail.value(QOrganizerItemDisplayLabel::FieldLabel).toString();
}


/*!
    \qmlclass Guid QDeclarativeOrganizerItemGuid
    \brief The Guid element contains the GUID string of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Guid.FieldGuid
    \endlist
 */
QDeclarativeOrganizerItemGuid::QDeclarativeOrganizerItemGuid(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemGuid());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemGuid::type() const
{
    return QDeclarativeOrganizerItemDetail::Guid;
}

/*!
    \qmlproperty string Guid::guid

    This property holds the GUID string.
 */
void QDeclarativeOrganizerItemGuid::setGuid(const QString &newGuid)
{
    if (newGuid != guid()) {
        m_detail.setValue(QOrganizerItemGuid::FieldGuid, newGuid);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemGuid::guid() const
{
    return m_detail.value(QOrganizerItemGuid::FieldGuid).toString();
}


/*!
    \qmlclass Location QDeclarativeOrganizerItemLocation
    \brief The Location element contains information about a location which is related to the organizer item in some manner.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Location.FieldLabel
    \o Location.FieldLatitude
    \o Location.FieldLongitude
    \endlist
 */
QDeclarativeOrganizerItemLocation::QDeclarativeOrganizerItemLocation(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemLocation());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemLocation::type() const
{
    return QDeclarativeOrganizerItemDetail::Location;
}

/*!
    \qmlproperty double Location::latitude

    This property holds the location latitude value.
 */
void QDeclarativeOrganizerItemLocation::setLatitude(double newLatitude)
{
    if (!qFuzzyCompare(newLatitude, latitude())) {
        m_detail.setValue(QOrganizerItemLocation::FieldLatitude, newLatitude);
        emit valueChanged();
    }
}

double QDeclarativeOrganizerItemLocation::latitude() const
{
    return m_detail.value<double>(QOrganizerItemLocation::FieldLatitude);
}

/*!
    \qmlproperty double Location::longitude

    This property holds the location longitude value .
 */
void QDeclarativeOrganizerItemLocation::setLongitude(double newLongitude)
{
    if (!qFuzzyCompare(newLongitude, longitude())) {
        m_detail.setValue(QOrganizerItemLocation::FieldLongitude, newLongitude);
        emit valueChanged();
    }
}

double QDeclarativeOrganizerItemLocation::longitude() const
{
    return m_detail.value<double>(QOrganizerItemLocation::FieldLongitude);
}

/*!
    \qmlproperty string Location::label

    This property holds the location label value.
 */
void QDeclarativeOrganizerItemLocation::setLabel(const QString &newLabel)
{
    if (newLabel != label()) {
        m_detail.setValue(QOrganizerItemLocation::FieldLabel, newLabel);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemLocation::label() const
{
    return m_detail.value(QOrganizerItemLocation::FieldLabel).toString();
}


/*!
    \qmlclass Parent QDeclarativeOrganizerItemParent
    \brief The Parent element contains information about the event or todo that generated this item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Parent.FieldParentId
    \o Parent.FieldOriginalDate
    \endlist
 */
QDeclarativeOrganizerItemParent::QDeclarativeOrganizerItemParent(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemParent());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemParent::type() const
{
    return QDeclarativeOrganizerItemDetail::Parent;
}

/*!
    \qmlproperty date Parent::originalDate

    This property holds the original date of this instance origin item.
 */
void QDeclarativeOrganizerItemParent::setOriginalDate(const QDate &date)
{
    if (date != originalDate()) {
        m_detail.setValue(QOrganizerItemParent::FieldOriginalDate, date);
        emit valueChanged();
    }
}

QDate QDeclarativeOrganizerItemParent::originalDate() const
{
    return m_detail.value(QOrganizerItemParent::FieldOriginalDate).toDate();
}

/*!
    \qmlproperty string Parent::parentId

    This property holds the organizer item id of the parent recurrent event or todo.
 */
void QDeclarativeOrganizerItemParent::setParentId(const QString &newParentId)
{
    if (newParentId != parentId()) {
        m_detail.setValue(QOrganizerItemParent::FieldParentId,
                          QVariant::fromValue(QOrganizerItemId::fromString(newParentId)));
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemParent::parentId() const
{
    return m_detail.value(QOrganizerItemParent::FieldParentId).value<QOrganizerItemId>().toString();
}


/*!
    \qmlclass Priority QDeclarativeOrganizerItemPriority
    \brief The Priority element contains the priority of the organizer item, which may be used to resolve scheduling conflicts.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Priority.FieldPriority
    \endlist
 */
QDeclarativeOrganizerItemPriority::QDeclarativeOrganizerItemPriority(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemPriority());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemPriority::type() const
{
    return QDeclarativeOrganizerItemDetail::Priority;
}


/*!
    \qmlproperty enumeration Priority::priority

    This property holds the priority associated with an organizer item. The value can be one of:
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
void QDeclarativeOrganizerItemPriority::setPriority(QDeclarativeOrganizerItemPriority::Priority newPriority)
{
    if (newPriority != priority()) {
        m_detail.setValue(QOrganizerItemPriority::FieldPriority, static_cast<int>(newPriority));
        emit valueChanged();
    }
}

QDeclarativeOrganizerItemPriority::Priority QDeclarativeOrganizerItemPriority::priority() const
{
    return static_cast<Priority>(m_detail.value<int>(QOrganizerItemPriority::FieldPriority));
}


/*!
    \qmlclass Recurrence QDeclarativeOrganizerItemRecurrence
    \brief The Recurrence element contains a list of rules and dates on which the recurrent item occurs,
           and a list of rules and dates on which exceptions occur.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Recurrence.FieldRecurrenceRules
    \o Recurrence.FieldExceptionRules
    \o Recurrence.FieldRecurrenceDates
    \o Recurrence.FieldExceptionDates
    \endlist
 */
QDeclarativeOrganizerItemRecurrence::QDeclarativeOrganizerItemRecurrence(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemRecurrence());
    connect(this, SIGNAL(recurrenceRulesChanged()), SLOT(_saveRecurrenceRules()));
    connect(this, SIGNAL(exceptionRulesChanged()), SLOT(_saveExceptionRules()));
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemRecurrence::type() const
{
    return QDeclarativeOrganizerItemDetail::Recurrence;
}

/*!
    \qmlproperty list<RecurrenceRule> Recurrence::recurrenceRules

    This property holds the list of recurrence rules.

    \sa RecurrenceRule
 */
QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> QDeclarativeOrganizerItemRecurrence::recurrenceRules()
{
    QSet<QOrganizerRecurrenceRule> ruleSet = m_detail.value(QOrganizerItemRecurrence::FieldRecurrenceRules).value< QSet<QOrganizerRecurrenceRule> >();
    if (m_recurrenceRules.isEmpty() && !ruleSet.isEmpty()) {
        foreach (QOrganizerRecurrenceRule rule, ruleSet) {
            QDeclarativeOrganizerRecurrenceRule* drule = new QDeclarativeOrganizerRecurrenceRule(this);
            drule->setRule(rule);
            connect(drule, SIGNAL(recurrenceRuleChanged()), this, SLOT(_saveRecurrenceRules()));
            m_recurrenceRules.append(drule);
        }
    }
    return QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule>(this, &m_recurrenceRules, rrule_append, rule_count, rule_at, rrule_clear);
}

/*!
    \qmlproperty list<RecurrenceRule> Recurrence::exceptionRules

    This property holds the list of exception rules.

    \sa RecurrenceRule
 */
QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> QDeclarativeOrganizerItemRecurrence::exceptionRules()
{
    QSet<QOrganizerRecurrenceRule> ruleSet = m_detail.value(QOrganizerItemRecurrence::FieldExceptionRules).value< QSet<QOrganizerRecurrenceRule> >();
    if (m_exceptionRules.isEmpty() && !ruleSet.isEmpty()) {
        foreach (QOrganizerRecurrenceRule rule, ruleSet) {
            QDeclarativeOrganizerRecurrenceRule* drule = new QDeclarativeOrganizerRecurrenceRule(this);
            drule->setRule(rule);
            connect(drule, SIGNAL(recurrenceRuleChanged()), this, SLOT(_saveExceptionRules()));
            m_exceptionRules.append(drule);
        }
    }
    return QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule>(this, &m_exceptionRules, xrule_append, rule_count, rule_at, xrule_clear);
}

/*!
    \qmlproperty list<date> Recurrence::recurrenceDates

    This property holds the list of recurrence dates.
 */
void QDeclarativeOrganizerItemRecurrence::setRecurrenceDates(const QVariantList &dates)
{
    if (dates != recurrenceDates()) {
        QSet<QDate> dateSet;
        QVariant dateSetVariant;
        foreach (QVariant date, dates) {
            if (date.canConvert(QVariant::Date))
                dateSet.insert(date.toDate());
        }
        dateSetVariant.setValue(dateSet);
        m_detail.setValue(QOrganizerItemRecurrence::FieldRecurrenceDates, dateSetVariant);
        emit valueChanged();
    }
}

QVariantList QDeclarativeOrganizerItemRecurrence::recurrenceDates() const
{
    QVariant dateSetVariant = m_detail.value(QOrganizerItemRecurrence::FieldRecurrenceDates);
    QSet<QDate> dateSet = dateSetVariant.value<QSet <QDate> >();
    QVariantList dates;
    foreach (QDate date, dateSet)
        dates.append(QVariant(date));
    return dates;
}

/*!
    \qmlproperty list<date> Recurrence::exceptionDates

    This property holds the list of exception dates.
 */
void QDeclarativeOrganizerItemRecurrence::setExceptionDates(const QVariantList& dates)
{
    if (dates != exceptionDates()) {
        QSet<QDate> dateSet;
        QVariant dateSetVariant;
        foreach (QVariant date, dates) {
            if (date.canConvert(QVariant::Date))
                dateSet.insert(date.toDate());
        }
        dateSetVariant.setValue(dateSet);
        m_detail.setValue(QOrganizerItemRecurrence::FieldExceptionDates, dateSetVariant);
        emit valueChanged();
    }
}

QVariantList QDeclarativeOrganizerItemRecurrence::exceptionDates() const
{
    QVariant dateSetVariant = m_detail.value(QOrganizerItemRecurrence::FieldExceptionDates);
    QSet<QDate> dateSet = dateSetVariant.value<QSet <QDate> >();
    QVariantList dates;
    foreach (QDate date, dateSet)
        dates.append(QVariant(date));
    return dates;
}

void QDeclarativeOrganizerItemRecurrence::rrule_append(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p,
                                                      QDeclarativeOrganizerRecurrenceRule *item)
{
    QDeclarativeOrganizerItemRecurrence* recurrence = qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object);
    connect(item, SIGNAL(recurrenceRuleChanged()), recurrence, SLOT(_saveRecurrenceRules()));
    static_cast<QList <QDeclarativeOrganizerRecurrenceRule*> *>(p->data)->append(item);
    emit recurrence->recurrenceRulesChanged();
}

void QDeclarativeOrganizerItemRecurrence::xrule_append(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p,
                                                      QDeclarativeOrganizerRecurrenceRule *item)
{
    QDeclarativeOrganizerItemRecurrence* recurrence = qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object);
    connect(item, SIGNAL(recurrenceRuleChanged()), recurrence, SLOT(_saveExceptionRules()));
    static_cast<QList <QDeclarativeOrganizerRecurrenceRule*> *>(p->data)->append(item);
    emit recurrence->exceptionRulesChanged();
}

int  QDeclarativeOrganizerItemRecurrence::rule_count(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p)
{
    return static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->count();
}

QDeclarativeOrganizerRecurrenceRule* QDeclarativeOrganizerItemRecurrence::rule_at(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p, int idx)
{
    return static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->at(idx);
}

void QDeclarativeOrganizerItemRecurrence::rrule_clear(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p)
{
    static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->clear();
    emit qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object)->recurrenceRulesChanged();
}

void QDeclarativeOrganizerItemRecurrence::xrule_clear(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p)
{
    static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->clear();
    emit qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object)->exceptionRulesChanged();
}


/*!
    \qmlclass Tag QDeclarativeOrganizerItemTag
    \brief The Tag element contains the tag string of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Tag.FieldTag
    \endlist
 */
QDeclarativeOrganizerItemTag::QDeclarativeOrganizerItemTag(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemTag());
}

QDeclarativeOrganizerItemTag::DetailType QDeclarativeOrganizerItemTag::type() const
{
    return QDeclarativeOrganizerItemDetail::Tag;
}

/*!
    \qmlproperty string Tag::tag

    This property holds the tag string.
 */
void QDeclarativeOrganizerItemTag::setTag(const QString &newTag)
{
    if (newTag != tag()) {
        m_detail.setValue(QOrganizerItemTag::FieldTag, newTag);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemTag::tag() const
{
    return m_detail.value(QOrganizerItemTag::FieldTag).toString();
}


/*!
    \qmlclass Timestamp QDeclarativeOrganizerItemTimestamp
    \brief The Timestamp element contains the created and last modified timestamp of an organizer item's creating date and time.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Timestamp.FieldCreated
    \o Timestamp.FieldLastModified
    \endlist
 */
QDeclarativeOrganizerItemTimestamp::QDeclarativeOrganizerItemTimestamp(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemTimestamp());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemTimestamp::type() const
{
    return QDeclarativeOrganizerItemDetail::Timestamp;
}

/*!
    \qmlproperty date Timestamp::created

    This property holds the value of the item's creation date and time.
 */
void QDeclarativeOrganizerItemTimestamp::setCreated(const QDateTime &timestamp)
{
    if (timestamp != created()) {
        m_detail.setValue(QOrganizerItemTimestamp::FieldCreationTimestamp, timestamp.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerItemTimestamp::created() const
{
    return m_detail.value<QDateTime>(QOrganizerItemTimestamp::FieldCreationTimestamp).toLocalTime();
}

/*!
    \qmlproperty date Timestamp::lastModified

    This property holds the value of the item's last modified date and time.
 */
void QDeclarativeOrganizerItemTimestamp::setLastModified(const QDateTime &timestamp)
{
    if (timestamp != lastModified()) {
        m_detail.setValue(QOrganizerItemTimestamp::FieldModificationTimestamp, timestamp.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerItemTimestamp::lastModified() const
{
    return m_detail.value<QDateTime>(QOrganizerItemTimestamp::FieldModificationTimestamp).toLocalTime();
}


/*!
    \qmlclass ItemType QDeclarativeOrganizerItemType
    \brief The ItemType element contains the type of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o ItemType.FieldType
    \endlist
 */
QDeclarativeOrganizerItemType::QDeclarativeOrganizerItemType(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemType());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemType::type() const
{
    return QDeclarativeOrganizerItemDetail::ItemType;
}

/*!
    \qmlproperty enum ItemType::itemType

    This property holds the type of the item. The value can be one of:
    \list
    \o ItemType.Event
    \o ItemType.EventOccurrence
    \o ItemType.Todo
    \o ItemType.TodoOccurrence
    \o ItemType.Note
    \o ItemType.Journal
    \o ItemType.Customized
    \endlist
 */
void QDeclarativeOrganizerItemType::setItemType(ItemType newType)
{
    if (newType != itemType()) {
        m_detail.setValue(QOrganizerItemType::FieldType, static_cast<QOrganizerItemType::ItemType>(newType));
        emit valueChanged();
    }
}

QDeclarativeOrganizerItemType::ItemType QDeclarativeOrganizerItemType::itemType() const
{
    return static_cast<ItemType>(m_detail.value(QOrganizerItemType::FieldType).toInt());
}


/*!
    \qmlclass JournalTime QDeclarativeOrganizerJournalTime
    \brief The JournalTime element contains the entry date and time of a journal item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o JournalTime.FieldEntryDateTime
    \endlist
 */
QDeclarativeOrganizerJournalTime::QDeclarativeOrganizerJournalTime(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerJournalTime());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerJournalTime::type() const
{
    return QDeclarativeOrganizerItemDetail::JournalTime;
}

/*!
  \qmlproperty date JournalTime::entryDateTime

  This property holds the entry date and time value of the journal.
  */
void QDeclarativeOrganizerJournalTime::setEntryDateTime(const QDateTime &datetime)
{
    if (datetime != entryDateTime()) {
        m_detail.setValue(QOrganizerJournalTime::FieldEntryDateTime, datetime.toUTC());
        emit valueChanged();
    }
}
QDateTime QDeclarativeOrganizerJournalTime::entryDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerJournalTime::FieldEntryDateTime).toLocalTime();
}


/*!
    \qmlclass TodoProgress QDeclarativeOrganizerTodoProgress
    \brief The TodoProgress element contains information about the progress of a todo item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o TodoProgress.FieldStatus
    \o TodoProgress.FieldPercentage
    \o TodoProgress.FieldFinishedDateTime
    \endlist
 */
QDeclarativeOrganizerTodoProgress::QDeclarativeOrganizerTodoProgress(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerTodoProgress());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerTodoProgress::type() const
{
    return QDeclarativeOrganizerItemDetail::TodoProgress;
}

/*!
    \qmlproperty int TodoProgress::percentage

    This property holds the value which contains the current completion percentage of the
    todo item.
 */
void QDeclarativeOrganizerTodoProgress::setPercentage(int newPercentage)
{
    if (newPercentage != percentage()) {
        if (newPercentage >=0 && newPercentage <= 100) {
            m_detail.setValue(QOrganizerTodoProgress::FieldPercentageComplete, newPercentage);
            emit valueChanged();
        }
    }
}

int QDeclarativeOrganizerTodoProgress::percentage() const
{
    return m_detail.value<int>(QOrganizerTodoProgress::FieldPercentageComplete);
}

/*!
    \qmlproperty date TodoProgress::finishedDateTime

    This property holds the date time value which contains the date and time at which the
    todo item was completed.
 */
void QDeclarativeOrganizerTodoProgress::setFinishedDateTime(const QDateTime &datetime)
{
    if (datetime != finishedDateTime()) {
        m_detail.setValue(QOrganizerTodoProgress::FieldFinishedDateTime, datetime.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerTodoProgress::finishedDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerTodoProgress::FieldFinishedDateTime).toLocalTime();
}

/*!
    \qmlproperty enumeration TodoProgress::status

    This property holds the value which describes the current completion status of the
    todo item. The value can be one of:
    \list
    \o TodoProgress.NotStarted
    \o TodoProgress.InProgress
    \o TodoProgress.Complete
    \endlist
 */
void QDeclarativeOrganizerTodoProgress::setStatus(QDeclarativeOrganizerTodoProgress::StatusType newStatus)
{
    if (newStatus != status()) {
        m_detail.setValue(QOrganizerTodoProgress::FieldStatus, (int) newStatus);
        emit valueChanged();
    }
}

QDeclarativeOrganizerTodoProgress::StatusType QDeclarativeOrganizerTodoProgress::status() const
{
    return (StatusType) m_detail.value<int>(QOrganizerTodoProgress::FieldStatus);
}


/*!
    \qmlclass TodoTime QDeclarativeOrganizerTodoTime
    \brief The TodoTime element contains the start and due dates and times of a recurring todo series, or occurrence of an todo item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o TodoTime.FieldStartDateTime
    \o TodoTime.FieldDueDateTime
    \o TodoTime.FieldAllDay
    \endlist
 */
QDeclarativeOrganizerTodoTime::QDeclarativeOrganizerTodoTime(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerTodoTime());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerTodoTime::type() const
{
    return QDeclarativeOrganizerItemDetail::TodoTime;
}

/*!
    \qmlproperty date TodoTime::allDay

    This property holds whether the time is significant in the start datetime.
 */
void QDeclarativeOrganizerTodoTime::setAllDay(bool allDay)
{
    if (allDay != isAllDay()) {
        m_detail.setValue(QOrganizerTodoTime::FieldAllDay, allDay);
        emit valueChanged();
    }
}

bool QDeclarativeOrganizerTodoTime::isAllDay()
{
    return m_detail.value<bool>(QOrganizerTodoTime::FieldAllDay);
}

/*!
    \qmlproperty date TodoTime::startDateTime

    This property holds the start date and time value of the todo item.
 */
void QDeclarativeOrganizerTodoTime::setStartDateTime(const QDateTime &datetime)
{
    if (datetime != startDateTime()) {
        m_detail.setValue(QOrganizerTodoTime::FieldStartDateTime, datetime.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerTodoTime::startDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerTodoTime::FieldStartDateTime).toLocalTime();
}

/*!
    \qmlproperty date TodoTime::dueDateTime

    This property holds the end date and time value of the todo item.
 */
void QDeclarativeOrganizerTodoTime::setDueDateTime(const QDateTime &dateTime)
{
    if (dateTime != dueDateTime()) {
        m_detail.setValue(QOrganizerTodoTime::FieldDueDateTime, dateTime.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerTodoTime::dueDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerTodoTime::FieldDueDateTime).toLocalTime();
}


/*!
    \qmlclass Reminder QDeclarativeOrganizerItemReminder
    \brief The Reminder element contains information about when and how the user wants to reminded of the item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o Reminder.FieldRepetitionCount
    \o Reminder.FieldRepetitionDelay
    \o Reminder.FieldSecondsBeforeStart
    \endlist
 */
QDeclarativeOrganizerItemReminder::QDeclarativeOrganizerItemReminder(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(reminderChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemReminder());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::Reminder;
}

/*!
    \qmlproperty enumeration Reminder::reminderType

    This property holds the reminder type of this reminder for an organizer item. The value can be one of:
    \list
    \o Reminder.NoReminder
    \o Reminder.VisualReminder
    \o Reminder.AudibleReminder
    \o Reminder.EmailReminder
    \endlist
 */
QDeclarativeOrganizerItemReminder::ReminderType QDeclarativeOrganizerItemReminder::reminderType() const
{
    if (m_detail.type() == QOrganizerItemDetail::TypeAudibleReminder)
        return QDeclarativeOrganizerItemReminder::AudibleReminder;
    else if (m_detail.type() == QOrganizerItemDetail::TypeEmailReminder)
        return QDeclarativeOrganizerItemReminder::EmailReminder;
    else if (m_detail.type() == QOrganizerItemDetail::TypeVisualReminder)
        return QDeclarativeOrganizerItemReminder::VisualReminder;
    else
        return QDeclarativeOrganizerItemReminder::NoReminder;
}

/*!
    \qmlproperty int Reminder::repetitionCount

    This property holds the number of times the user should be reminded of the item.
 */
void QDeclarativeOrganizerItemReminder::setRepetitionCount(int count)
{
    if (count != repetitionCount()) {
        m_detail.setValue(QOrganizerItemReminder::FieldRepetitionCount, count);
        emit reminderChanged();
    }
}

int QDeclarativeOrganizerItemReminder::repetitionCount() const
{
    return m_detail.value<int>(QOrganizerItemReminder::FieldRepetitionCount);
}

/*!
    \qmlproperty int Reminder::repetitionDelay

    This property holds the delay (in seconds) between each repetition of the reminder.
 */
void QDeclarativeOrganizerItemReminder::setRepetitionDelay(int delaySeconds)
{
    if (delaySeconds != repetitionDelay()) {
        m_detail.setValue(QOrganizerItemReminder::FieldRepetitionDelay, delaySeconds);
        emit reminderChanged();
    }
}

int QDeclarativeOrganizerItemReminder::repetitionDelay() const
{
    return m_detail.value<int>(QOrganizerItemReminder::FieldRepetitionDelay);
}

/*!
    \qmlproperty int Reminder::secondsBeforeStart

    This property holds the number of seconds prior to the activation of the item
    at which the user wants to be reminded of the item.
 */
void QDeclarativeOrganizerItemReminder::setSecondsBeforeStart(int seconds)
{
    if (seconds != secondsBeforeStart()) {
        m_detail.setValue(QOrganizerItemReminder::FieldSecondsBeforeStart, seconds);
        emit reminderChanged();
    }
}

int QDeclarativeOrganizerItemReminder::secondsBeforeStart() const
{
    return m_detail.value<int>(QOrganizerItemReminder::FieldSecondsBeforeStart);
}


/*!
    \qmlclass AudibleReminder QDeclarativeOrganizerItemAudibleReminder
    \brief The AudibleReminder element contains information about an audible reminder of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details
    \inherits Reminder

    The following fields are supported:
    \list
    \o AudibleReminder.FieldRepetitionCount
    \o AudibleReminder.FieldRepetitionDelay
    \o AudibleReminder.FieldSecondsBeforeStart
    \o AudibleReminder.FieldDataUrl
    \endlist
 */
QDeclarativeOrganizerItemAudibleReminder::QDeclarativeOrganizerItemAudibleReminder(QObject *parent)
    : QDeclarativeOrganizerItemReminder(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(reminderChanged()));
    setDetail(QOrganizerItemAudibleReminder());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemAudibleReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::AudibleReminder;
}

/*!
    \qmlproperty url AudibleReminder::dataUrl

    This property holds the url of the audible data to play.
 */
void QDeclarativeOrganizerItemAudibleReminder::setDataUrl(const QUrl &url)
{
    if (url != dataUrl()) {
        m_detail.setValue(QOrganizerItemAudibleReminder::FieldDataUrl, url);
        emit valueChanged();
    }
}

QUrl QDeclarativeOrganizerItemAudibleReminder::dataUrl() const
{
    return m_detail.value<QUrl>(QOrganizerItemAudibleReminder::FieldDataUrl);
}


/*!
    \qmlclass EmailReminder QDeclarativeOrganizerItemEmailReminder
    \brief The EmailReminder element contains information about an email reminder of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details
    \inherits Reminder

    The following fields are supported:
    \list
    \o EmailReminder.FieldRepetitionCount
    \o EmailReminder.FieldRepetitionDelay
    \o EmailReminder.FieldSecondsBeforeStart
    \o EmailReminder.FieldSubject
    \o EmailReminder.FieldBody
    \o EmailReminder.FieldRecipients
    \o EmailReminder.FieldAttachments
    \endlist
 */
QDeclarativeOrganizerItemEmailReminder::QDeclarativeOrganizerItemEmailReminder(QObject *parent)
    : QDeclarativeOrganizerItemReminder(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(reminderChanged()));
    setDetail(QOrganizerItemEmailReminder());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemEmailReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::EmailReminder;
}

/*!
    \qmlproperty string EmailReminder::body

    This property holds the body of the email.
 */
void QDeclarativeOrganizerItemEmailReminder::setBody(const QString &newBody)
{
    if (newBody != body()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldBody, newBody);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemEmailReminder::body() const
{
    return m_detail.value(QOrganizerItemEmailReminder::FieldBody).toString();
}

/*!
    \qmlproperty string EmailReminder::subject

    This property holds the subject of the email.
 */
void QDeclarativeOrganizerItemEmailReminder::setSubject(const QString &newSubject)
{
    if (newSubject != subject()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldSubject, newSubject);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemEmailReminder::subject() const
{
    return m_detail.value(QOrganizerItemEmailReminder::FieldSubject).toString();
}


/*!
    \qmlproperty list<string> EmailReminder::recipients

    This property holds the list of recipients that the user wishes to be sent an email as part of the reminder.
 */
void QDeclarativeOrganizerItemEmailReminder::setRecipients(const QStringList &newRecipients)
{
    if (newRecipients != recipients()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldRecipients, newRecipients);
        emit valueChanged();
    }
}

QStringList QDeclarativeOrganizerItemEmailReminder::recipients() const
{
    return m_detail.value<QStringList>(QOrganizerItemEmailReminder::FieldRecipients);
}

/*!
    \qmlproperty list<variant> EmailReminder::attachments

    This property holds the attachments of the email.
 */
void QDeclarativeOrganizerItemEmailReminder::setAttachments(const QVariantList &newAttachments)
{
    if (newAttachments != attachments()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldAttachments, newAttachments);
        emit valueChanged();
    }
}

QVariantList QDeclarativeOrganizerItemEmailReminder::attachments()
{
    return m_detail.value<QVariantList>(QOrganizerItemEmailReminder::FieldAttachments);
}


/*!
    \qmlclass VisualReminder QDeclarativeOrganizerItemVisualReminder
    \brief The VisualReminder element contains information about a visual reminder of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details
    \inherits Reminder

    The following fields are supported:
    \list
    \o VisualReminder.FieldRepetitionCount
    \o VisualReminder.FieldRepetitionDelay
    \o VisualReminder.FieldSecondsBeforeStart
    \o VisualReminder.FieldDataUrl
    \o VisualReminder.FieldMessage
    \endlist
 */
QDeclarativeOrganizerItemVisualReminder::QDeclarativeOrganizerItemVisualReminder(QObject *parent)
    : QDeclarativeOrganizerItemReminder(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(reminderChanged()));
    setDetail(QOrganizerItemVisualReminder());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemVisualReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::VisualReminder;
}

/*!
    \qmlproperty string VisualReminder::message

    This property holds the message which the user wishes to be displayed as part of the reminder.
 */
void QDeclarativeOrganizerItemVisualReminder::setMessage(const QString &msg)
{
    if (msg != message()) {
        m_detail.setValue(QOrganizerItemVisualReminder::FieldMessage, msg);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemVisualReminder::message() const
{
    return m_detail.value<QString>(QOrganizerItemVisualReminder::FieldMessage);
}

/*!
    \qmlproperty url VisualReminder::dataUrl

    This property holds the url of the visual data which the user wishes to be displayed as part of the reminder.
 */
void QDeclarativeOrganizerItemVisualReminder::setDataUrl(const QUrl &url)
{
    if (url != dataUrl()) {
        m_detail.setValue(QOrganizerItemVisualReminder::FieldDataUrl, url);
        emit valueChanged();
    }
}

QUrl QDeclarativeOrganizerItemVisualReminder::dataUrl() const
{
    return m_detail.value<QUrl>(QOrganizerItemVisualReminder::FieldDataUrl);
}


/*!
    \qmlclass ExtendedDetail QDeclarativeOrganizeritemExtendedDetail
    \brief The ExtendedDetail element contains a extended detail of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o ExtendedDetail.FieldName
    \o ExtendedDetail.FieldData
    \endlist
 */
QDeclarativeOrganizerItemExtendedDetail::QDeclarativeOrganizerItemExtendedDetail(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemExtendedDetail());
}

QDeclarativeOrganizerItemExtendedDetail::DetailType QDeclarativeOrganizerItemExtendedDetail::type() const
{
    return QDeclarativeOrganizerItemDetail::ExtendedDetail;
}

/*!
    \qmlproperty string ExtendedDetail::name

    This property holds the name of the extended detail.
 */
void QDeclarativeOrganizerItemExtendedDetail::setName(const QString &newDetailName)
{
    if (newDetailName != name()) {
        m_detail.setValue(QOrganizerItemExtendedDetail::FieldExtendedDetailName, newDetailName);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemExtendedDetail::name() const
{
    return m_detail.value(QOrganizerItemExtendedDetail::FieldExtendedDetailName).toString();
}

/*!
    \qmlproperty variant ExtendedDetail::data

    This property holds the data of the extended detail.
 */
void QDeclarativeOrganizerItemExtendedDetail::setData(const QVariant &newData)
{
    if (newData != data()) {
        setValue(QOrganizerItemExtendedDetail::FieldExtendedDetailData, newData);
        emit valueChanged();
    }
}

QVariant QDeclarativeOrganizerItemExtendedDetail::data() const
{
    return m_detail.value(QOrganizerItemExtendedDetail::FieldExtendedDetailData);
}

/*!
    \qmlclass EventAttendee QDeclarativeOrganizerEventAttendee
    \brief The EventAttendee element contains information about an attendee of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \o EventAttendee.FieldName
    \o EventAttendee.FieldEmailAddress
    \o EventAttendee.FieldAddendeeId
    \o EventAttendee.FieldParticipationStatus
    \o EventAttendee.FieldParticipationRole
    \endlist
 */
QDeclarativeOrganizerEventAttendee::QDeclarativeOrganizerEventAttendee(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerEventAttendee());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerEventAttendee::type() const
{
    return QDeclarativeOrganizerItemDetail::EventAttendee;
}

/*!
    \qmlproperty variant EventAttendee::name

    This property holds the name of the attendee.
 */
void QDeclarativeOrganizerEventAttendee::setName(const QString &newName)
{
    if (name() != newName) {
        m_detail.setValue(QOrganizerEventAttendee::FieldName, newName);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerEventAttendee::name() const
{
    return m_detail.value(QOrganizerEventAttendee::FieldName).toString();
}

/*!
    \qmlproperty variant EventAttendee::emailAddress

    This property holds the email address of the attendee.
 */
void QDeclarativeOrganizerEventAttendee::setEmailAddress(const QString &newEmailAddress)
{
    if (emailAddress() != newEmailAddress) {
        m_detail.setValue(QOrganizerEventAttendee::FieldEmailAddress, newEmailAddress);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerEventAttendee::emailAddress() const
{
    return m_detail.value(QOrganizerEventAttendee::FieldEmailAddress).toString();
}

/*!
    \qmlproperty variant EventAttendee::participationStatus

    This property holds the participation status of the attendee of the event. The value can be one of:
    \list
    \o EventAttendee.StatusUnknown
    \o EventAttendee.StatusAccepted
    \o EventAttendee.StatusDeclined
    \o EventAttendee.StatusTentative
    \o EventAttendee.StatusDelegated
    \o EventAttendee.StatusInProcess
    \o EventAttendee.StatusCompleted
    \endlist
 */
void QDeclarativeOrganizerEventAttendee::setParticipationStatus(ParticipationStatus status)
{
    if (participationStatus() != status) {
        m_detail.setValue(QOrganizerEventAttendee::FieldParticipationStatus, status);
        emit valueChanged();
    }
}

QDeclarativeOrganizerEventAttendee::ParticipationStatus QDeclarativeOrganizerEventAttendee::participationStatus() const
{
    return static_cast<ParticipationStatus>(m_detail.value(QOrganizerEventAttendee::FieldParticipationStatus).toInt());
}

/*!
    \qmlproperty variant EventAttendee::participationRole

    This property holds the participation role of the attendee of the event.The value can be one of:
    \list
    \o EventAttendee.RoleUnknown
    \o EventAttendee.RoleOrganizer
    \o EventAttendee.RoleChairperson
    \o EventAttendee.RoleHost
    \o EventAttendee.RoleRequiredParticipant
    \o EventAttendee.RoleOptionalParticipant
    \o EventAttendee.RoleNonParticipant
    \endlist
 */
void QDeclarativeOrganizerEventAttendee::setParticipationRole(ParticipationRole role)
{
    if (participationRole() != role) {
        m_detail.setValue(QOrganizerEventAttendee::FieldParticipationRole, role);
        emit valueChanged();
    }
}

QDeclarativeOrganizerEventAttendee::ParticipationRole QDeclarativeOrganizerEventAttendee::participationRole() const
{
    return static_cast<ParticipationRole>(m_detail.value(QOrganizerEventAttendee::FieldParticipationRole).toInt());
}

/*!
    \qmlproperty variant EventAttendee::attendeeId

    This property holds the unique identifier of the attendee.
 */
void QDeclarativeOrganizerEventAttendee::setAttendeeId(const QString &newAttendeeId)
{
    if (attendeeId() != newAttendeeId) {
        m_detail.setValue(QOrganizerEventAttendee::FieldAttendeeId, newAttendeeId);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerEventAttendee::attendeeId() const
{
    return m_detail.value(QOrganizerEventAttendee::FieldAttendeeId).toString();
}

/*!
    \qmlclass Rsvp QDeclarativeOrganizerEventRsvp
    \brief The Rsvp element contains Rsvp-information of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    RSVP detail contains user specific information about calendar event like
    participation status and role, information about response
    dates and information about organizer of the event. See more details
    from the properties themselves and the QOrganizerEventRsvp.

    \sa QOrganizerEventRsvp
 */
QDeclarativeOrganizerEventRsvp::QDeclarativeOrganizerEventRsvp(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerEventRsvp());
}

QDeclarativeOrganizerEventRsvp::DetailType QDeclarativeOrganizerEventRsvp::type() const
{
    return QDeclarativeOrganizerItemDetail::EventRsvp;
}

/*!
    \qmlproperty variant EventRsvp::participationStatus

    This property holds the calendar user's participation status related to the event. See EventAttendee::participationStatus
    for more details.

    \sa EventAttendee::participationStatus
 */
void QDeclarativeOrganizerEventRsvp::setParticipationStatus(QDeclarativeOrganizerEventAttendee::ParticipationStatus status)
{
    if (participationStatus() != status) {
        m_detail.setValue(QOrganizerEventRsvp::FieldParticipationStatus, status);
        emit valueChanged();
    }
}

QDeclarativeOrganizerEventAttendee::ParticipationStatus QDeclarativeOrganizerEventRsvp::participationStatus() const
{
    return static_cast<QDeclarativeOrganizerEventAttendee::ParticipationStatus>(m_detail.value(QOrganizerEventRsvp::FieldParticipationStatus).toInt());
}

/*!
    \qmlproperty variant EventRsvp::participationRole

    This property holds the calendar user's participation role related to the event. See EventAttendee::participationRole
    for more details.

    \sa EventAttendee::participationRole
 */
void QDeclarativeOrganizerEventRsvp::setParticipationRole(QDeclarativeOrganizerEventAttendee::ParticipationRole role)
{
    if (participationRole() != role) {
        m_detail.setValue(QOrganizerEventRsvp::FieldParticipationRole, role);
        emit valueChanged();
    }
}

QDeclarativeOrganizerEventAttendee::ParticipationRole QDeclarativeOrganizerEventRsvp::participationRole() const
{
    return static_cast<QDeclarativeOrganizerEventAttendee::ParticipationRole>(m_detail.value(QOrganizerEventRsvp::FieldParticipationRole).toInt());
}

/*!
    \qmlproperty variant EventRsvp::responseRequirement

    This property holds the response requirement of the event. The value can be one of:
    \list
    \o EventRsvp.ResponseNotRequired
    \o EventRsvp.ResponseRequired
    \endlist
 */
void QDeclarativeOrganizerEventRsvp::setResponseRequirement(ResponseRequirement requirement)
{
    if (responseRequirement() != requirement) {
        m_detail.setValue(QOrganizerEventRsvp::FieldResponseRequirement, requirement);
        emit valueChanged();
    }
}

QDeclarativeOrganizerEventRsvp::ResponseRequirement QDeclarativeOrganizerEventRsvp::responseRequirement() const
{
    return static_cast<ResponseRequirement>(m_detail.value(QOrganizerEventRsvp::FieldResponseRequirement).toInt());
}

/*!
    \qmlproperty variant EventRsvp::responseDeadline

    This property holds the last date for responding the event.
 */
void QDeclarativeOrganizerEventRsvp::setResponseDeadline(const QDate &date)
{
    if (responseDeadline() != date) {
        m_detail.setValue(QOrganizerEventRsvp::FieldResponseDeadline, date);
        emit valueChanged();
     }
}

QDate QDeclarativeOrganizerEventRsvp::responseDeadline() const
{
    return m_detail.value<QDate>(QOrganizerEventRsvp::FieldResponseDeadline);
}

/*!
    \qmlproperty variant EventRsvp::responseDate

    This property holds the date when user responded to the event.
 */
void QDeclarativeOrganizerEventRsvp::setResponseDate(const QDate &date)
{
    if (responseDate() != date) {
        m_detail.setValue(QOrganizerEventRsvp::FieldResponseDate, date);
        emit valueChanged();
     }
}

QDate QDeclarativeOrganizerEventRsvp::responseDate() const
{
    return m_detail.value<QDate>(QOrganizerEventRsvp::FieldResponseDate);
}

/*!
    \qmlproperty variant EventRsvp::organizerName

    This property holds organizer's name of the event.
 */
void QDeclarativeOrganizerEventRsvp::setOrganizerName(const QString &name)
{
    if (organizerName() != name) {
        m_detail.setValue(QOrganizerEventRsvp::FieldOrganizerName, name);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerEventRsvp::organizerName() const
{
    return m_detail.value(QOrganizerEventRsvp::FieldOrganizerName).toString();
}

/*!
    \qmlproperty variant EventRsvp::organizerEmail

    This property holds organizer's email of the event.
 */
void QDeclarativeOrganizerEventRsvp::setOrganizerEmail(const QString &email)
{
    if (organizerEmail() != email) {
        m_detail.setValue(QOrganizerEventRsvp::FieldOrganizerEmail, email);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerEventRsvp::organizerEmail() const
{
    return m_detail.value(QOrganizerEventRsvp::FieldOrganizerEmail).toString();
}

/*!
    \qmlclass Classification QDeclarativeOrganizerItemClassification
    \brief The Classification element contains classification-information of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The Classification detail contains classification related information. This can
    be used as a part of security model for the organizer.

    \sa QOrganizerItemClassification
 */
QDeclarativeOrganizerItemClassification::QDeclarativeOrganizerItemClassification(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemClassification());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemClassification::type() const
{
    return QDeclarativeOrganizerItemDetail::Classification;
}

/*!
    \qmlproperty string Classification::classification

    This property holds the calendar item's classification related information. Value can be
    either one defined by iCalendar specification ("PUBLIC", "PRIVATE", "CONFIDENTIAL") or custom string.
 */
void QDeclarativeOrganizerItemClassification::setClassification(const QString &newClassification)
{
    if (classification() != newClassification) {
        m_detail.setValue(QOrganizerItemClassification::FieldClassification, newClassification);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemClassification::classification() const
{
    return m_detail.value(QOrganizerItemClassification::FieldClassification).toString();
}


QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItemDetailFactory::createItemDetail(QDeclarativeOrganizerItemDetail::DetailType type)
{
    QDeclarativeOrganizerItemDetail *itemDetail;
    if (type == QDeclarativeOrganizerItemDetail::EventTime)
        itemDetail = new QDeclarativeOrganizerEventTime;
    else if (type == QDeclarativeOrganizerItemDetail::AudibleReminder)
        itemDetail = new QDeclarativeOrganizerItemAudibleReminder;
    else if (type == QDeclarativeOrganizerItemDetail::Comment)
        itemDetail = new QDeclarativeOrganizerItemComment;
    else if (type == QDeclarativeOrganizerItemDetail::Description)
        itemDetail = new QDeclarativeOrganizerItemDescription;
    else if (type == QDeclarativeOrganizerItemDetail::DisplayLabel)
        itemDetail = new QDeclarativeOrganizerItemDisplayLabel;
    else if (type == QDeclarativeOrganizerItemDetail::EmailReminder)
        itemDetail = new QDeclarativeOrganizerItemEmailReminder;
    else if (type == QDeclarativeOrganizerItemDetail::Guid)
        itemDetail = new QDeclarativeOrganizerItemGuid;
    else if (type == QDeclarativeOrganizerItemDetail::Location)
        itemDetail = new QDeclarativeOrganizerItemLocation;
    else if (type == QDeclarativeOrganizerItemDetail::Parent)
        itemDetail = new QDeclarativeOrganizerItemParent;
    else if (type == QDeclarativeOrganizerItemDetail::Priority)
        itemDetail = new QDeclarativeOrganizerItemPriority;
    else if (type == QDeclarativeOrganizerItemDetail::Recurrence)
        itemDetail = new QDeclarativeOrganizerItemRecurrence;
    else if (type == QDeclarativeOrganizerItemDetail::Reminder)
        itemDetail = new QDeclarativeOrganizerItemReminder;
    else if (type == QDeclarativeOrganizerItemDetail::Tag)
        itemDetail = new QDeclarativeOrganizerItemTag;
    else if (type == QDeclarativeOrganizerItemDetail::Timestamp)
        itemDetail = new QDeclarativeOrganizerItemTimestamp;
    else if (type == QDeclarativeOrganizerItemDetail::ItemType)
        itemDetail = new QDeclarativeOrganizerItemType;
    else if (type == QDeclarativeOrganizerItemDetail::VisualReminder)
        itemDetail = new QDeclarativeOrganizerItemVisualReminder;
    else if (type == QDeclarativeOrganizerItemDetail::JournalTime)
        itemDetail = new QDeclarativeOrganizerJournalTime;
    else if (type == QDeclarativeOrganizerItemDetail::TodoProgress)
        itemDetail = new QDeclarativeOrganizerTodoProgress;
    else if (type == QDeclarativeOrganizerItemDetail::TodoTime)
        itemDetail = new QDeclarativeOrganizerTodoTime;
    else if (type == QDeclarativeOrganizerItemDetail::ExtendedDetail)
        itemDetail = new QDeclarativeOrganizerItemExtendedDetail;
    else if (type == QDeclarativeOrganizerItemDetail::EventAttendee)
        itemDetail = new QDeclarativeOrganizerEventAttendee;
    else if (type == QDeclarativeOrganizerItemDetail::EventRsvp)
        itemDetail = new QDeclarativeOrganizerEventRsvp;
    else if (type == QDeclarativeOrganizerItemDetail::Classification)
        itemDetail = new QDeclarativeOrganizerItemClassification;
    else
        itemDetail = new QDeclarativeOrganizerItemDetail;
    return itemDetail;
}

#include "moc_qdeclarativeorganizeritemdetail_p.cpp"

QTORGANIZER_END_NAMESPACE
