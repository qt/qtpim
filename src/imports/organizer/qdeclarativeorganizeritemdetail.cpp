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

#include "qdeclarativeorganizeritemdetail_p.h"

#include <QtQml/qqmlinfo.h>
#include <QtQml/QJSValue>

#include <QtOrganizer/qorganizeritemdetails.h>
#include <QtOrganizer/qorganizeritemid.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype Detail
    \instantiates QDeclarativeOrganizerItemDetail
    \brief The Detail element represents a single, complete detail about a organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-main

    \sa QOrganizerItemDetail
 */

/*!
  \qmlsignal Detail::onDetailChanged()

  This signal is emitted, when any of the Details's or child element's (like EventTime, DisplayLabel etc) properties have been changed.
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
    \li Detail.Undefined
    \li Detail.Classification
    \li Detail.Comment
    \li Detail.Description
    \li Detail.DisplayLabel
    \li Detail.ItemType
    \li Detail.Guid
    \li Detail.Location
    \li Detail.Parent
    \li Detail.Priority
    \li Detail.Recurrence
    \li Detail.Tag
    \li Detail.Timestamp
    \li Detail.Version
    \li Detail.Reminder
    \li Detail.AudibleReminder
    \li Detail.EmailReminder
    \li Detail.VisualReminder
    \li Detail.ExtendedDetail
    \li Detail.EventAttendee
    \li Detail.EventRsvp
    \li Detail.EventTime
    \li Detail.JournalTime
    \li Detail.TodoTime
    \li Detail.TodoProgress
    \endlist

    \sa Classification, Comment, Description, DisplayLabel, ItemType, Guid, Location, Parent, Priority, Recurrence, Tag, Timestamp
    \sa Version, Reminder, AudibleReminder, EmailReminder, VisualReminder, ExtendedDetail, EventAttendee, EventRsvp, EventTime
    \sa JournalTime, TodoTime TodoProgress
 */
QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemDetail::type() const
{
    return Undefined;
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
    \qmltype EventTime
    \instantiates QDeclarativeOrganizerEventTime
    \brief The EventTime element contains the start and end dates and times of a recurring event series, or occurrence of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li EventTime.FieldStartDateTime
    \li EventTime.FieldEndDateTime
    \li EventTime.FieldAllDay
    \endlist

    \sa QOrganizerEventTime
 */

/*!
  \qmlsignal EventTime::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Comment
    \instantiates QDeclarativeOrganizerItemComment
    \brief The Comment element contains the comment text of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Comment.FieldComment
    \endlist

    \sa QOrganizerItemComment
 */

/*!
  \qmlsignal Comment::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Description
    \instantiates QDeclarativeOrganizerItemDescription
    \brief The Description element contains the description text of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Description.FieldDescription
    \endlist

    \sa QOrganizerItemDescription
 */

/*!
  \qmlsignal Description::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype DisplayLabel
    \instantiates QDeclarativeOrganizerItemDisplayLabel
    \brief The DisplayLabel element contains the display label of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li DisplayLabel.FieldLabel
    \endlist

    \sa QOrganizerItemDisplayLabel
 */

/*!
  \qmlsignal DisplayLabel::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmlproperty string DisplayLabel::label

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
    \qmltype Guid
    \instantiates QDeclarativeOrganizerItemGuid
    \brief The Guid element contains the GUID string of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Guid.FieldGuid
    \endlist

    \sa QOrganizerItemGuid
 */

/*!
  \qmlsignal Guid::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Location
    \instantiates QDeclarativeOrganizerItemLocation
    \brief The Location element contains information about a location which is related to the organizer item in some manner.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Location.FieldLabel
    \li Location.FieldLatitude
    \li Location.FieldLongitude
    \endlist

    \sa QOrganizerItemLocation
 */

/*!
  \qmlsignal Location::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Parent
    \instantiates QDeclarativeOrganizerItemParent
    \brief The Parent element contains information about the event or todo that generated this item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Parent.FieldParentId
    \li Parent.FieldOriginalDate
    \endlist

    \sa QOrganizerItemParent
 */

/*!
  \qmlsignal Parent::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmlmethod variant Parent::value(field)

    \sa Detail::value
 */
QVariant QDeclarativeOrganizerItemParent::value(int field) const
{
    switch (field) {
    case FieldParentId:
    {
        QString id = parentId();
        return id.isNull() ? QVariant() : id;
    }
    case FieldOriginalDate:
    {
        QDateTime date = originalDate();
        return date.isValid() ? date : QVariant();
    }
    default:
    {
        return QVariant();
    }
    }
}

/*!
    \qmlmethod bool Parent::setValue(field, value)

    \sa Detail::setValue
 */
bool QDeclarativeOrganizerItemParent::setValue(int field, const QVariant &value)
{
    switch (field) {
    case FieldParentId:
    {
        if (value.canConvert<QString>()) {
            setParentId(value.toString());
            return true;
        }
        break;
    }
    case FieldOriginalDate:
    {
        if (value.canConvert<QDateTime>()) {
            setOriginalDate(value.toDateTime());
            return true;
        }
        break;
    }
    default:
    {
        return false;
    }
    }
    return false;
}

/*!
    \qmlproperty date Parent::originalDate

    This property holds the original date of this instance origin item.
 */
void QDeclarativeOrganizerItemParent::setOriginalDate(const QDateTime &date)
{
    if (date != originalDate()) {
        // If the value was likely set as a QDate, then assume that the time info can be ignored.
        // This is to ensure that dates like "2002-01-01" don't get interpretted as being
        // "2002-01-01T00:00:00+10:00" if the local timezone is GMT+10, and then being converted
        // to "2001-31-12T14:00:00Z" in UTC before having the (different) date "2001-31-12"
        // extracted for insertion into the FieldResponseDeadline value.
        if (date.timeSpec() == Qt::LocalTime && date.time() == QTime(0,0,0,0)) {
            m_detail.setValue(QOrganizerItemParent::FieldOriginalDate, date.date());
        } else {
            m_detail.setValue(QOrganizerItemParent::FieldOriginalDate, date.toUTC().date());
        }
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerItemParent::originalDate() const
{
    QDateTime retDateTime(m_detail.value(QOrganizerItemParent::FieldOriginalDate).toDate(), QTime(0, 0, 0, 0), Qt::UTC);
    return retDateTime;
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
    \qmltype Priority
    \instantiates QDeclarativeOrganizerItemPriority
    \brief The Priority element contains the priority of the organizer item, which may be used to resolve scheduling conflicts.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Priority.FieldPriority
    \endlist

    \sa QOrganizerItemPriority
 */

/*!
  \qmlsignal Priority::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Recurrence
    \instantiates QDeclarativeOrganizerItemRecurrence
    \brief The Recurrence element contains a list of rules and dates on which the recurrent item occurs,
           and a list of rules and dates on which exceptions occur.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Recurrence.FieldRecurrenceRules
    \li Recurrence.FieldExceptionRules
    \li Recurrence.FieldRecurrenceDates
    \li Recurrence.FieldExceptionDates
    \endlist
 */

/*!
  \qmlsignal Recurrence::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmlmethod variant Recurrence::value(field)

    \sa Detail::value
 */
QVariant QDeclarativeOrganizerItemRecurrence::value(int field) const
{
    switch (field) {
    case FieldRecurrenceDates:
    {
        QVariantList rdates = recurrenceDates();
        return rdates;
    }
    case FieldExceptionDates:
    {
        QVariantList edates = exceptionDates();
        return edates;
    }
    default:
    {
        // TODO: proper handling of FieldRecurrenceRules and FieldExceptionRules --> conversion
        // from QSet<QOrganizerRecurrenceRule> to QVariantList
        return QVariant();
    }
    }
}

/*!
    \qmlmethod bool Recurrence::setValue(field, value)

    \sa Detail::setValue
 */
bool QDeclarativeOrganizerItemRecurrence::setValue(int field, const QVariant &value)
{
    switch (field) {
    case FieldRecurrenceDates:
    {
        if (value.canConvert<QVariantList>()) {
            setRecurrenceDates(value.toList());
            return true;
        }
        break;
    }
    case FieldExceptionDates:
    {
        if (value.canConvert<QVariantList>()) {
            setExceptionDates(value.toList());
            return true;
        }
        break;
    }
    default:
    {
        // TODO: proper handling of FieldRecurrenceRules and FieldExceptionRules --> conversion
        // from QVariantList to QSet<QOrganizerRecurrenceRule>
        return false;
    }
    }
    return false;
}

/*!
    \qmlproperty list<RecurrenceRule> Recurrence::recurrenceRules

    This property holds the list of recurrence rules.

    \sa RecurrenceRule
 */
QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> QDeclarativeOrganizerItemRecurrence::recurrenceRules()
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
    return QQmlListProperty<QDeclarativeOrganizerRecurrenceRule>(this, &m_recurrenceRules, rrule_append, rule_count, rule_at, rrule_clear);
}

/*!
    \qmlproperty list<RecurrenceRule> Recurrence::exceptionRules

    This property holds the list of exception rules.

    \sa RecurrenceRule
 */
QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> QDeclarativeOrganizerItemRecurrence::exceptionRules()
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
    return QQmlListProperty<QDeclarativeOrganizerRecurrenceRule>(this, &m_exceptionRules, xrule_append, rule_count, rule_at, xrule_clear);
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
        Q_FOREACH (const QVariant &date, dates) {
            if (date.canConvert(QVariant::DateTime)) {
                QDateTime dt = date.toDateTime();
                // If the value was likely set as a QDate, then assume that the time info can be ignored.
                // This is to ensure that dates like "2002-01-01" don't get interpretted as being
                // "2002-01-01T00:00:00+10:00" if the local timezone is GMT+10, and then being converted
                // to "2001-31-12T14:00:00Z" in UTC before having the (different) date "2001-31-12"
                // extracted for insertion into the dateSet.
                if (dt.timeSpec() == Qt::LocalTime && dt.time() == QTime(0,0,0,0)) {
                    dateSet.insert(dt.date());
                } else {
                    dateSet.insert(dt.toUTC().date());
                }
            }
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
    foreach (QDate date, dateSet) {
        QDateTime dateTime(date, QTime(0, 0, 0, 0), Qt::UTC);
        dates.append(QVariant(dateTime));
    }
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
        Q_FOREACH (const QVariant &date, dates) {
            if (date.canConvert(QVariant::DateTime)) {
                QDateTime dt = date.toDateTime();
                // If the value was likely set as a QDate, then assume that the time info can be ignored.
                // This is to ensure that dates like "2002-01-01" don't get interpretted as being
                // "2002-01-01T00:00:00+10:00" if the local timezone is GMT+10, and then being converted
                // to "2001-31-12T14:00:00Z" in UTC before having the (different) date "2001-31-12"
                // extracted for insertion into the dateSet.
                if (dt.timeSpec() == Qt::LocalTime && dt.time() == QTime(0,0,0,0)) {
                    dateSet.insert(dt.date());
                } else {
                    dateSet.insert(dt.toUTC().date());
                }
            }
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
    foreach (QDate date, dateSet) {
        QDateTime dateTime(date, QTime(0, 0, 0, 0), Qt::UTC);
        dates.append(QVariant(dateTime));
    }
    return dates;
}

void QDeclarativeOrganizerItemRecurrence::_saveRecurrenceRules()
{
    QSet<QOrganizerRecurrenceRule> rules;
    foreach (const QDeclarativeOrganizerRecurrenceRule *r, m_recurrenceRules)
        rules << r->rule();
    m_detail.setValue(QOrganizerItemRecurrence::FieldRecurrenceRules, QVariant::fromValue(rules));
    emit valueChanged();
}

void QDeclarativeOrganizerItemRecurrence::_saveExceptionRules()
{
    QSet<QOrganizerRecurrenceRule> rules;
    foreach (const QDeclarativeOrganizerRecurrenceRule *r, m_exceptionRules)
        rules << r->rule();
    m_detail.setValue(QOrganizerItemRecurrence::FieldExceptionRules, QVariant::fromValue(rules));
    emit valueChanged();
}

void QDeclarativeOrganizerItemRecurrence::rrule_append(QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> *p,
                                                      QDeclarativeOrganizerRecurrenceRule *item)
{
    QDeclarativeOrganizerItemRecurrence* recurrence = qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object);
    connect(item, SIGNAL(recurrenceRuleChanged()), recurrence, SLOT(_saveRecurrenceRules()));
    static_cast<QList <QDeclarativeOrganizerRecurrenceRule*> *>(p->data)->append(item);
    emit recurrence->recurrenceRulesChanged();
}

void QDeclarativeOrganizerItemRecurrence::xrule_append(QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> *p,
                                                      QDeclarativeOrganizerRecurrenceRule *item)
{
    QDeclarativeOrganizerItemRecurrence* recurrence = qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object);
    connect(item, SIGNAL(recurrenceRuleChanged()), recurrence, SLOT(_saveExceptionRules()));
    static_cast<QList <QDeclarativeOrganizerRecurrenceRule*> *>(p->data)->append(item);
    emit recurrence->exceptionRulesChanged();
}

int  QDeclarativeOrganizerItemRecurrence::rule_count(QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> *p)
{
    return static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->count();
}

QDeclarativeOrganizerRecurrenceRule* QDeclarativeOrganizerItemRecurrence::rule_at(QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> *p, int idx)
{
    return static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->at(idx);
}

void QDeclarativeOrganizerItemRecurrence::rrule_clear(QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> *p)
{
    static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->clear();
    emit qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object)->recurrenceRulesChanged();
}

void QDeclarativeOrganizerItemRecurrence::xrule_clear(QQmlListProperty<QDeclarativeOrganizerRecurrenceRule> *p)
{
    static_cast<QList<QDeclarativeOrganizerRecurrenceRule*>*>(p->data)->clear();
    emit qobject_cast<QDeclarativeOrganizerItemRecurrence*>(p->object)->exceptionRulesChanged();
}


/*!
    \qmltype Tag
    \instantiates QDeclarativeOrganizerItemTag
    \brief The Tag element contains the tag string of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Tag.FieldTag
    \endlist

    \sa QOrganizerItemTag
 */

/*!
  \qmlsignal Tag::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Timestamp
    \instantiates QDeclarativeOrganizerItemTimestamp
    \brief The Timestamp element contains the created and last modified timestamp of an organizer item's creating date and time.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Timestamp.FieldCreated
    \li Timestamp.FieldLastModified
    \endlist

    \sa QOrganizerItemTimestamp
 */

/*!
  \qmlsignal Timestamp::onDetailChanged()

  \sa Detail::onDetailChanged
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
        m_detail.setValue(QOrganizerItemTimestamp::FieldCreated, timestamp.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerItemTimestamp::created() const
{
    return m_detail.value<QDateTime>(QOrganizerItemTimestamp::FieldCreated).toLocalTime();
}

/*!
    \qmlproperty date Timestamp::lastModified

    This property holds the value of the item's last modified date and time.
 */
void QDeclarativeOrganizerItemTimestamp::setLastModified(const QDateTime &timestamp)
{
    if (timestamp != lastModified()) {
        m_detail.setValue(QOrganizerItemTimestamp::FieldLastModified, timestamp.toUTC());
        emit valueChanged();
    }
}

QDateTime QDeclarativeOrganizerItemTimestamp::lastModified() const
{
    return m_detail.value<QDateTime>(QOrganizerItemTimestamp::FieldLastModified).toLocalTime();
}


/*!
    \qmltype ItemType
    \instantiates QDeclarativeOrganizerItemType
    \brief The ItemType element contains the type of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li ItemType.FieldType
    \endlist

    \sa QOrganizerItemType
 */

/*!
  \qmlsignal ItemType::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \li ItemType.Event
    \li ItemType.EventOccurrence
    \li ItemType.Todo
    \li ItemType.TodoOccurrence
    \li ItemType.Note
    \li ItemType.Journal
    \li ItemType.Customized
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
    \qmltype JournalTime
    \instantiates QDeclarativeOrganizerJournalTime
    \brief The JournalTime element contains the entry date and time of a journal item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li JournalTime.FieldEntryDateTime
    \endlist

    \sa QOrganizerJournalTime
 */

/*!
  \qmlsignal JournalTime::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype TodoProgress
    \instantiates QDeclarativeOrganizerTodoProgress
    \brief The TodoProgress element contains information about the progress of a todo item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li TodoProgress.FieldStatus
    \li TodoProgress.FieldPercentage
    \li TodoProgress.FieldFinishedDateTime
    \endlist

    \sa QOrganizerTodoProgress
 */

/*!
  \qmlsignal TodoProgress::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmlproperty int TodoProgress::percentageComplete

    This property holds the value which contains the current completion percentage of the
    todo item.
 */
void QDeclarativeOrganizerTodoProgress::setPercentageComplete(int newPercentageComplete)
{
    if (newPercentageComplete != percentageComplete()) {
        if (newPercentageComplete >=0 && newPercentageComplete <= 100) {
            m_detail.setValue(QOrganizerTodoProgress::FieldPercentageComplete, newPercentageComplete);
            emit valueChanged();
        }
    }
}

int QDeclarativeOrganizerTodoProgress::percentageComplete() const
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
    \li TodoProgress.NotStarted
    \li TodoProgress.InProgress
    \li TodoProgress.Complete
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
    \qmltype TodoTime
    \instantiates QDeclarativeOrganizerTodoTime
    \brief The TodoTime element contains the start and due dates and times of a recurring todo series, or occurrence of an todo item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li TodoTime.FieldStartDateTime
    \li TodoTime.FieldDueDateTime
    \li TodoTime.FieldAllDay
    \endlist

    \sa QOrganizerTodoTime
 */

/*!
  \qmlsignal TodoTime::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype Reminder
    \instantiates QDeclarativeOrganizerItemReminder
    \brief The Reminder element contains information about when and how the user wants to reminded of the item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li Reminder.FieldRepetitionCount
    \li Reminder.FieldRepetitionDelay
    \li Reminder.FieldSecondsBeforeStart
    \endlist

    \sa QOrganizerItemReminder
 */

/*!
  \qmlsignal Reminder::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \li Reminder.NoReminder
    \li Reminder.VisualReminder
    \li Reminder.AudibleReminder
    \li Reminder.EmailReminder
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
    if (seconds != secondsBeforeStart() || !m_detail.hasValue(QOrganizerItemReminder::FieldSecondsBeforeStart)) {
        m_detail.setValue(QOrganizerItemReminder::FieldSecondsBeforeStart, seconds);
        emit reminderChanged();
    }
}

int QDeclarativeOrganizerItemReminder::secondsBeforeStart() const
{
    return m_detail.value<int>(QOrganizerItemReminder::FieldSecondsBeforeStart);
}


/*!
    \qmltype AudibleReminder
    \instantiates QDeclarativeOrganizerItemAudibleReminder
    \brief The AudibleReminder element contains information about an audible reminder of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details
    \inherits Reminder

    The following fields are supported:
    \list
    \li AudibleReminder.FieldRepetitionCount
    \li AudibleReminder.FieldRepetitionDelay
    \li AudibleReminder.FieldSecondsBeforeStart
    \li AudibleReminder.FieldDataUrl
    \endlist

    \sa Reminder QOrganizerItemAudibleReminder
 */

/*!
  \qmlsignal AudibleReminder::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype EmailReminder
    \instantiates QDeclarativeOrganizerItemEmailReminder
    \brief The EmailReminder element contains information about an email reminder of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details
    \inherits Reminder

    The following fields are supported:
    \list
    \li EmailReminder.FieldRepetitionCount
    \li EmailReminder.FieldRepetitionDelay
    \li EmailReminder.FieldSecondsBeforeStart
    \li EmailReminder.FieldSubject
    \li EmailReminder.FieldBody
    \li EmailReminder.FieldRecipients
    \li EmailReminder.FieldAttachments
    \endlist

    \sa Reminder QOrganizerItemEmailReminder
 */

/*!
  \qmlsignal EmailReminder::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype VisualReminder
    \instantiates QDeclarativeOrganizerItemVisualReminder
    \brief The VisualReminder element contains information about a visual reminder of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details
    \inherits Reminder

    The following fields are supported:
    \list
    \li VisualReminder.FieldRepetitionCount
    \li VisualReminder.FieldRepetitionDelay
    \li VisualReminder.FieldSecondsBeforeStart
    \li VisualReminder.FieldDataUrl
    \li VisualReminder.FieldMessage
    \endlist

    \sa Reminder QOrganizerItemVisualReminder
 */

/*!
  \qmlsignal VisualReminder::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmltype ExtendedDetail
    \instantiates QDeclarativeOrganizeritemExtendedDetail
    \brief The ExtendedDetail element contains a extended detail of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li ExtendedDetail.FieldName
    \li ExtendedDetail.FieldData
    \endlist

    \sa QOrganizerItemExtendedDetail
 */

/*!
  \qmlsignal ExtendedDetail::onDetailChanged()

  \sa Detail::onDetailChanged
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
        m_detail.setValue(QOrganizerItemExtendedDetail::FieldName, newDetailName);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemExtendedDetail::name() const
{
    return m_detail.value(QOrganizerItemExtendedDetail::FieldName).toString();
}

/*!
    \qmlproperty variant ExtendedDetail::data

    This property holds the data of the extended detail.
 */
void QDeclarativeOrganizerItemExtendedDetail::setData(const QVariant &newData)
{
    QVariant unboxedData(newData);
    if (newData.userType() == qMetaTypeId<QJSValue>()) {
        unboxedData = newData.value<QJSValue>().toVariant();
    }

    if (unboxedData != data()) {
        setValue(QOrganizerItemExtendedDetail::FieldData, unboxedData);
        emit valueChanged();
    }
}

QVariant QDeclarativeOrganizerItemExtendedDetail::data() const
{
    return m_detail.value(QOrganizerItemExtendedDetail::FieldData);
}

/*!
    \qmltype EventAttendee
    \instantiates QDeclarativeOrganizerEventAttendee
    \brief The EventAttendee element contains information about an attendee of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The following fields are supported:
    \list
    \li EventAttendee.FieldName
    \li EventAttendee.FieldEmailAddress
    \li EventAttendee.FieldAddendeeId
    \li EventAttendee.FieldParticipationStatus
    \li EventAttendee.FieldParticipationRole
    \endlist

    \sa QOrganizerEventAttendee
 */

/*!
  \qmlsignal EventAttendee::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \li EventAttendee.StatusUnknown
    \li EventAttendee.StatusAccepted
    \li EventAttendee.StatusDeclined
    \li EventAttendee.StatusTentative
    \li EventAttendee.StatusDelegated
    \li EventAttendee.StatusInProcess
    \li EventAttendee.StatusCompleted
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
    \li EventAttendee.RoleUnknown
    \li EventAttendee.RoleOrganizer
    \li EventAttendee.RoleChairperson
    \li EventAttendee.RoleHost
    \li EventAttendee.RoleRequiredParticipant
    \li EventAttendee.RoleOptionalParticipant
    \li EventAttendee.RoleNonParticipant
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
    \qmltype EventRsvp
    \instantiates QDeclarativeOrganizerEventRsvp
    \brief The EventRsvp element contains Rsvp-information of an event.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    EventRsvp detail contains user specific information about calendar event like
    participation status and role, information about response
    dates and information about organizer of the event. See more details
    from the properties themselves and the QOrganizerEventRsvp.

    \sa QOrganizerEventRsvp
 */

/*!
  \qmlsignal EventRsvp::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmlmethod variant EventRsvp::value(field)

    \sa Detail::value
 */
QVariant QDeclarativeOrganizerEventRsvp::value(int field) const
{
    switch (field) {
    case FieldResponseDeadline:
    {
        QDateTime date = responseDeadline();
        return date.isValid() ? date : QVariant();
    }
    case FieldResponseDate:
    {
        QDateTime date = responseDate();
        return date.isValid() ? date : QVariant();
    }
    default:
    {
        return m_detail.value(field);
    }
    }
}

/*!
    \qmlmethod bool EventRsvp::setValue(field, value)

    \sa Detail::setValue
 */
bool QDeclarativeOrganizerEventRsvp::setValue(int field, const QVariant &value)
{
    switch (field) {
    case FieldResponseDeadline:
    {
        if (value.canConvert<QDateTime>()) {
            setResponseDeadline(value.toDateTime());
            return true;
        }
        break;
    }
    case FieldResponseDate:
    {
        if (value.canConvert<QDateTime>()) {
            setResponseDate(value.toDateTime());
            return true;
        }
        break;
    }
    default:
    {
        if (m_detail.setValue(field, value))
            return true;
    }
    }
    return false;
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
    \li EventRsvp.ResponseNotRequired
    \li EventRsvp.ResponseRequired
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
void QDeclarativeOrganizerEventRsvp::setResponseDeadline(const QDateTime &date)
{
    if (responseDeadline() != date) {
        // If the value was likely set as a QDate, then assume that the time info can be ignored.
        // This is to ensure that dates like "2002-01-01" don't get interpretted as being
        // "2002-01-01T00:00:00+10:00" if the local timezone is GMT+10, and then being converted
        // to "2001-31-12T14:00:00Z" in UTC before having the (different) date "2001-31-12"
        // extracted for insertion into the FieldResponseDeadline value.
        if (date.timeSpec() == Qt::LocalTime && date.time() == QTime(0,0,0,0)) {
            m_detail.setValue(QOrganizerEventRsvp::FieldResponseDeadline, date.date());
        } else {
            m_detail.setValue(QOrganizerEventRsvp::FieldResponseDeadline, date.toUTC().date());
        }
        emit valueChanged();
     }
}

QDateTime QDeclarativeOrganizerEventRsvp::responseDeadline() const
{
    QDateTime retDateTime(m_detail.value<QDate>(QOrganizerEventRsvp::FieldResponseDeadline), QTime(0, 0, 0, 0), Qt::UTC);
    return retDateTime;
}

/*!
    \qmlproperty variant EventRsvp::responseDate

    This property holds the date when user responded to the event.
 */
void QDeclarativeOrganizerEventRsvp::setResponseDate(const QDateTime &date)
{
    if (responseDate() != date) {
        // If the value was likely set as a QDate, then assume that the time info can be ignored.
        // This is to ensure that dates like "2002-01-01" don't get interpretted as being
        // "2002-01-01T00:00:00+10:00" if the local timezone is GMT+10, and then being converted
        // to "2001-31-12T14:00:00Z" in UTC before having the (different) date "2001-31-12"
        // extracted for insertion into the FieldResponseDate value.
        if (date.timeSpec() == Qt::LocalTime && date.time() == QTime(0,0,0,0)) {
            m_detail.setValue(QOrganizerEventRsvp::FieldResponseDate, date.date());
        } else {
            m_detail.setValue(QOrganizerEventRsvp::FieldResponseDate, date.toUTC().date());
        }
        emit valueChanged();
     }
}

QDateTime QDeclarativeOrganizerEventRsvp::responseDate() const
{
    QDateTime retDateTime(m_detail.value<QDate>(QOrganizerEventRsvp::FieldResponseDate), QTime(0, 0, 0, 0), Qt::UTC);
    return retDateTime;
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
    \qmltype Classification
    \instantiates QDeclarativeOrganizerItemClassification
    \brief The Classification element contains classification-information of an item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    The Classification detail contains classification related information. This can
    be used as a part of security model for the organizer.

    \sa QOrganizerItemClassification
 */

/*!
  \qmlsignal Classification::onDetailChanged()

  \sa Detail::onDetailChanged
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
    \qmlproperty enumeration Classification::classification

    This property holds the calendar item's classification related information. The value can be one of:
    \list
    \li Classification.AccessPublic
    \li Classification.AccessConfidential
    \li Classification.AccessPrivate
    \endlist

 */
void QDeclarativeOrganizerItemClassification::setClassification(AccessClassification newClassification)
{
    if (classification() != newClassification) {
        m_detail.setValue(QOrganizerItemClassification::FieldClassification, newClassification);
        emit valueChanged();
    }
}

QDeclarativeOrganizerItemClassification::AccessClassification QDeclarativeOrganizerItemClassification::classification() const
{
    return static_cast<AccessClassification>(m_detail.value(QOrganizerItemClassification::FieldClassification).toInt());
}


/*!
    \qmltype Version
    \instantiates QDeclarativeOrganizerItemVersion
    \brief The Version element contains versioning information of an organizer item.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-details

    \sa QOrganizerItemVersion
 */

/*!
  \qmlsignal Version::onDetailChanged()

  \sa Detail::onDetailChanged
*/

QDeclarativeOrganizerItemVersion::QDeclarativeOrganizerItemVersion(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    setDetail(QOrganizerItemVersion());
}

QDeclarativeOrganizerItemDetail::DetailType QDeclarativeOrganizerItemVersion::type() const
{
    return QDeclarativeOrganizerItemDetail::Version;
}

/*!
    \qmlproperty int Version::version

    This property holds the integer version of an organizer item, which can be used as the sequence
    number as per iCalendar spec.
 */
void QDeclarativeOrganizerItemVersion::setVersion(int newVersion)
{
    if (version() != newVersion) {
        m_detail.setValue(QOrganizerItemVersion::FieldVersion, newVersion);
        emit valueChanged();
    }
}

int QDeclarativeOrganizerItemVersion::version() const
{
    return m_detail.value(QOrganizerItemVersion::FieldVersion).toInt();
}

/*!
    \qmlproperty string Version::extendedVersion

    This property holds the extended version of an organizer item, which can be used to represent
    the version stored in the back-end.
 */
void QDeclarativeOrganizerItemVersion::setExtendedVersion(const QString &newExtendedVersion)
{
    if (extendedVersion() != newExtendedVersion) {
        m_detail.setValue(QOrganizerItemVersion::FieldExtendedVersion, newExtendedVersion);
        emit valueChanged();
    }
}

QString QDeclarativeOrganizerItemVersion::extendedVersion() const
{
    QByteArray version = m_detail.value(QOrganizerItemVersion::FieldExtendedVersion).toByteArray();
    return QString::fromLatin1(version.constData(), version.length());
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
    else if (type == QDeclarativeOrganizerItemDetail::Version)
        itemDetail = new QDeclarativeOrganizerItemVersion;
    else
        itemDetail = new QDeclarativeOrganizerItemDetail;
    return itemDetail;
}

#include "moc_qdeclarativeorganizeritemdetail_p.cpp"

QT_END_NAMESPACE
