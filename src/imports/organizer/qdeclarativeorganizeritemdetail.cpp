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

#include "qdeclarativeorganizeritemdetail_p.h"
#include <QtDeclarative/qdeclarativeinfo.h>
#include <qorganizeritemdetails.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass Detail QDeclarativeOrganizerItemDetail
    \brief The Detail element represents a single, complete detail about a organizer item.
    \ingroup qml-organizer

    \sa QOrganizerItemDetail

    The Detail element is part of the \bold{QtOrganizer 2.0} module.
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
    \qmlproperty bool Detail::readOnly

    This property indicates whether or not this detail is read-only.

    This property is read only.
 */
bool QDeclarativeOrganizerItemDetail::readOnly() const
{
    return m_detail.accessConstraints().testFlag(QOrganizerItemDetail::ReadOnly);
}

/*!
    \qmlproperty bool Detail::removable

    This property indicates whether or not this detail is removale.

    This property is read only.
 */
bool QDeclarativeOrganizerItemDetail::removable() const
{
    return !m_detail.accessConstraints().testFlag(QOrganizerItemDetail::Irremovable);
}

/*!
    \qmlproperty enumeration Detail::type

    This property holds the type of the detail and is read only. It can be one of:

    \list
    \o Detail.EventTime
    \o Detail.JournalTime
    \o Detail.TodoTime
    \o Detail.TodoProgress
    \o Detail.AudibleReminder
    \o Detail.VisualReminder
    \o Detail.EmailReminder
    \o Detail.Comment
    \o Detail.Description
    \o Detail.DisplayLabel
    \o Detail.Guid
    \o Detail.Location
    \o Detail.Parent
    \o Detail.Priority
    \o Detail.Recurrence
    \o Detail.Timestamp
    \o Detail.Type
    \o Detail.Tag
    \endlist
 */
QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemDetail::type() const
{
    return Customized;
}

/*!
    \qmlproperty list<string> Detail::fieldNames

    This property holds the list of all field names which this detail currently contains. Thse names are
    used as the key for value(), setValue() and removeValue().

    This property is read only.
 */
QStringList QDeclarativeOrganizerItemDetail::fieldNames() const
{
    return m_detail.values().keys();
}

/*!
    \qmlmethod variant Detail::value(key)

    Returns the value stored in this detail for the given \a key, or an empty variant if not available.
 */
QVariant QDeclarativeOrganizerItemDetail::value(const QString &key) const
{
    return m_detail.value(key);
}

/*!
    \qmlmethod bool Detail::setValue(key, value)

    Inserts \a value into the detail for the given \a key if value is valid. If value is invalid, removes
    the field with the given key from the detail. Returns true if the given value was set for the key (if
    the value was valid), or if the given key was removed from detail (if the value was invalid), otherwise
    returns false if the key was unable to be removed (and the value was invalid).
 */
bool QDeclarativeOrganizerItemDetail::setValue(const QString &key, const QVariant &value)
{
    bool ok = m_detail.setValue(key, value);
    if (ok)
        emit detailChanged();
    return ok;
}

/*!
    \qmlmethod bool Detail::removeValue(key)

    Removes the value stored in this detail for the given \a key. Returns true if a value was stored for
    the given key and the operation succeeded, and false otherwise.
 */
bool QDeclarativeOrganizerItemDetail::removeValue(const QString &key)
{
    bool ok = m_detail.removeValue(key);
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
    \internal
 */
QString QDeclarativeOrganizerItemDetail::fieldName(QDeclarativeOrganizerItemDetail::ItemDetailType detailType, int fieldType)
{
    switch (detailType) {
    case QDeclarativeOrganizerItemDetail::EventTime:
        return QDeclarativeOrganizerEventTime::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::JournalTime:
        return QDeclarativeOrganizerJournalTime::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::TodoTime:
        return QDeclarativeOrganizerTodoTime::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::TodoProgress:
        return QDeclarativeOrganizerTodoProgress::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Reminder:
        return QDeclarativeOrganizerItemReminder::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::AudibleReminder:
        return QDeclarativeOrganizerItemAudibleReminder::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::VisualReminder:
        return QDeclarativeOrganizerItemVisualReminder::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::EmailReminder:
        return QDeclarativeOrganizerItemEmailReminder::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Comment:
        return QDeclarativeOrganizerItemComment::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Description:
        return QDeclarativeOrganizerItemDescription::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::DisplayLabel:
        return QDeclarativeOrganizerItemDisplayLabel::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Guid:
        return QDeclarativeOrganizerItemGuid::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Location:
        return QDeclarativeOrganizerItemLocation::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Parent:
        return QDeclarativeOrganizerItemParent::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Priority:
        return QDeclarativeOrganizerItemPriority::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Recurrence:
        return QDeclarativeOrganizerItemRecurrence::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Timestamp:
        return QDeclarativeOrganizerItemTimestamp::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Type:
        return QDeclarativeOrganizerItemType::fieldNameFromFieldType(fieldType);
    case QDeclarativeOrganizerItemDetail::Customized:
        return QDeclarativeOrganizeritemExtendedDetail::fieldNameFromFieldType(fieldType);
    default:
        break;
    }
    qmlInfo(0) << QString(tr("Can't find the field name for detail type '%1' and field type '%2'")).arg(detailType).arg(fieldType);
    return QString();
}

/*!
    \internal
 */
QString QDeclarativeOrganizerItemDetail::definitionName(QDeclarativeOrganizerItemDetail::ItemDetailType type)
{
    switch (type) {
    case QDeclarativeOrganizerItemDetail::EventTime:
        return QOrganizerEventTime::DefinitionName;
    case QDeclarativeOrganizerItemDetail::JournalTime:
        return QOrganizerJournalTime::DefinitionName;
    case QDeclarativeOrganizerItemDetail::TodoTime:
        return QOrganizerTodoTime::DefinitionName;
    case QDeclarativeOrganizerItemDetail::TodoProgress:
        return QOrganizerTodoProgress::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Reminder:
        return QOrganizerItemReminder::DefinitionName;
    case QDeclarativeOrganizerItemDetail::AudibleReminder:
        return QOrganizerItemAudibleReminder::DefinitionName;
    case QDeclarativeOrganizerItemDetail::VisualReminder:
        return QOrganizerItemVisualReminder::DefinitionName;
    case QDeclarativeOrganizerItemDetail::EmailReminder:
        return QOrganizerItemEmailReminder::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Comment:
        return QOrganizerItemComment::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Description:
        return QOrganizerItemDescription::DefinitionName;
    case QDeclarativeOrganizerItemDetail::DisplayLabel:
        return QOrganizerItemDisplayLabel::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Guid:
        return QOrganizerItemGuid::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Location:
        return QOrganizerItemLocation::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Parent:
        return QOrganizerItemParent::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Priority:
        return QOrganizerItemPriority::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Recurrence:
        return QOrganizerItemRecurrence::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Timestamp:
        return QOrganizerItemTimestamp::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Type:
        return QOrganizerItemType::DefinitionName;
    case QDeclarativeOrganizerItemDetail::Customized:
        return QOrganizerItemExtendedDetail::DefinitionName;
    default:
        break;
    }
    qmlInfo(0) << QString(tr("Can't find the detail definition name for detail type '%1'")).arg(type);
    return QString();
}

/*!
    \internal
 */
QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemDetail::detailTypeByDefinitionName(const QString &definitionName)
{
    if (definitionName == QOrganizerEventTime::DefinitionName)
        return QDeclarativeOrganizerItemDetail::EventTime;
    if (definitionName == QOrganizerJournalTime::DefinitionName)
        return QDeclarativeOrganizerItemDetail::JournalTime;
    if (definitionName == QOrganizerTodoTime::DefinitionName)
        return QDeclarativeOrganizerItemDetail::TodoTime;
    if (definitionName == QOrganizerTodoProgress::DefinitionName)
        return QDeclarativeOrganizerItemDetail::TodoProgress;
    if (definitionName == QOrganizerItemReminder::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Reminder;
    if (definitionName == QOrganizerItemAudibleReminder::DefinitionName)
        return QDeclarativeOrganizerItemDetail::AudibleReminder;
    if (definitionName == QOrganizerItemVisualReminder::DefinitionName)
        return QDeclarativeOrganizerItemDetail::VisualReminder;
    if (definitionName == QOrganizerItemEmailReminder::DefinitionName)
        return QDeclarativeOrganizerItemDetail::EmailReminder;
    if (definitionName == QOrganizerItemComment::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Comment;
    if (definitionName == QOrganizerItemDescription::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Description;
    if (definitionName == QOrganizerItemDisplayLabel::DefinitionName)
        return QDeclarativeOrganizerItemDetail::DisplayLabel;
    if (definitionName == QOrganizerItemGuid::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Guid;
    if (definitionName == QOrganizerItemLocation::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Location;
    if (definitionName == QOrganizerItemParent::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Parent;
    if (definitionName == QOrganizerItemPriority::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Priority;
    if (definitionName == QOrganizerItemRecurrence::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Recurrence;
    if (definitionName == QOrganizerItemTimestamp::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Timestamp;
    if (definitionName == QOrganizerItemType::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Type;
    if (definitionName == QOrganizerItemExtendedDetail::DefinitionName)
        return QDeclarativeOrganizerItemDetail::Customized;
    qmlInfo(0) << QString(tr("Can't find the detail type for detail name '%1'")).arg(definitionName);
    return QDeclarativeOrganizerItemDetail::Customized;
}

// to be removed
QString QDeclarativeOrganizerItemDetail::definitionName() const
{
    return m_detail.definitionName();
}


/*!
    \qmlclass EventTime QDeclarativeOrganizerEventTime
    \brief The EventTime element contains the start and end dates and times of a recurring event series, or occurrence of an event.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerEventTime::QDeclarativeOrganizerEventTime(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerEventTime());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerEventTime::type() const
{
    return QDeclarativeOrganizerItemDetail::EventTime;
}

/*!
    \qmlproperty date EventTime::allDay

    This property holds whether the time is significant in the start datetime.
 */
void QDeclarativeOrganizerEventTime::setAllDay(bool allDay)
{
    if (allDay != isAllDay() && !readOnly()) {
        m_detail.setValue(QOrganizerEventTime::FieldAllDay, allDay);
        emit detailChanged();
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
     if (datetime != startDateTime() && !readOnly()) {
        m_detail.setValue(QOrganizerEventTime::FieldStartDateTime, datetime.toUTC());
        emit detailChanged();
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
    if (datetime != endDateTime() && !readOnly()) {
        m_detail.setValue(QOrganizerEventTime::FieldEndDateTime, datetime.toUTC());
        emit detailChanged();
    }
}

QDateTime QDeclarativeOrganizerEventTime::endDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerEventTime::FieldEndDateTime).toLocalTime();
}


/*!
    \qmlclass Comment QDeclarativeOrganizerItemComment
    \brief The Comment element contains the comment text of an organizer item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemComment::QDeclarativeOrganizerItemComment(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemComment());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemComment::type() const
{
    return QDeclarativeOrganizerItemDetail::Comment;
}

/*!
    \qmlproperty string Comment::comment

    This property holds the text of the comment.
 */
void QDeclarativeOrganizerItemComment::setComment(const QString &newComment)
{
    if (newComment != comment() && !readOnly()) {
        m_detail.setValue(QOrganizerItemComment::FieldComment, newComment);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemComment::comment() const
{
    return m_detail.value(QOrganizerItemComment::FieldComment).toString();
}


/*!
    \qmlclass Description QDeclarativeOrganizerItemDescription
    \brief The Description element contains the description text of an organizer item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemDescription::QDeclarativeOrganizerItemDescription(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemDescription());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemDescription::type() const
{
    return QDeclarativeOrganizerItemDetail::Description;
}

/*!
    \qmlproperty string Description::description

    This property holds the text of the description.
 */
void QDeclarativeOrganizerItemDescription::setDescription(const QString &desc)
{
    if (desc != description() && !readOnly()) {
        m_detail.setValue(QOrganizerItemDescription::FieldDescription, desc);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemDescription::description() const
{
    return m_detail.value(QOrganizerItemDescription::FieldDescription).toString();
}


/*!
    \qmlclass DisplayLabel QDeclarativeOrganizerItemDisplayLabel
    \brief The DisplayLabel element contains the display label of an organizer item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemDisplayLabel::QDeclarativeOrganizerItemDisplayLabel(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemDisplayLabel());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemDisplayLabel::type() const
{
    return QDeclarativeOrganizerItemDetail::DisplayLabel;
}

/*!
    \qmlproperty string DisplayLabel::displayLabel

    This property holds the display label text.
 */
void QDeclarativeOrganizerItemDisplayLabel::setLabel(const QString &newLabel)
{
    if (newLabel != label() && !readOnly()) {
        m_detail.setValue(QOrganizerItemDisplayLabel::FieldLabel, newLabel);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemDisplayLabel::label() const
{
    return m_detail.value(QOrganizerItemDisplayLabel::FieldLabel).toString();
}


/*!
    \qmlclass Guid QDeclarativeOrganizerItemGuid
    \brief The Guid element contains the GUID string of an organizer item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemGuid::QDeclarativeOrganizerItemGuid(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemGuid());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemGuid::type() const
{
    return QDeclarativeOrganizerItemDetail::Guid;
}

/*!
    \qmlproperty string Guid::guid

    This property holds the GUID string.
 */
void QDeclarativeOrganizerItemGuid::setGuid(const QString &newGuid)
{
    if (newGuid != guid() && !readOnly()) {
        m_detail.setValue(QOrganizerItemGuid::FieldGuid, newGuid);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemGuid::guid() const
{
    return m_detail.value(QOrganizerItemGuid::FieldGuid).toString();
}


/*!
    \qmlclass Location QDeclarativeOrganizerItemLocation
    \brief The Location element contains information about a location which is related to the organizer item in some manner.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemLocation::QDeclarativeOrganizerItemLocation(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemLocation());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemLocation::type() const
{
    return QDeclarativeOrganizerItemDetail::Location;
}

/*!
    \qmlproperty double Location::latitude

    This property holds the location latitude value.
 */
void QDeclarativeOrganizerItemLocation::setLatitude(double newLatitude)
{
    if (!qFuzzyCompare(newLatitude, latitude()) && !readOnly()) {
        m_detail.setValue(QOrganizerItemLocation::FieldLatitude, newLatitude);
        emit detailChanged();
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
    if (!qFuzzyCompare(newLongitude, longitude()) && !readOnly()) {
        m_detail.setValue(QOrganizerItemLocation::FieldLongitude, newLongitude);
        emit detailChanged();
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
    if (newLabel != label() && !readOnly()) {
        m_detail.setValue(QOrganizerItemLocation::FieldLabel, newLabel);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemLocation::label() const
{
    return m_detail.value(QOrganizerItemLocation::FieldLabel).toString();
}


/*!
    \qmlclass Parent QDeclarativeOrganizerItemParent
    \brief The Parent element contains information about the event or todo that generated this item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemParent::QDeclarativeOrganizerItemParent(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemParent());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemParent::type() const
{
    return QDeclarativeOrganizerItemDetail::Parent;
}

/*!
    \qmlproperty date Parent::originalDate

    This property holds the original date of this instance origin item.
 */
void QDeclarativeOrganizerItemParent::setOriginalDate(const QDate &date)
{
    if (date != originalDate() && !readOnly()) {
        m_detail.setValue(QOrganizerItemParent::FieldOriginalDate, date);
        emit detailChanged();
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
    if (newParentId != parentId() && !readOnly()) {
        m_detail.setValue(QOrganizerItemParent::FieldParentId,
                          QVariant::fromValue(QOrganizerItemId::fromString(newParentId)));
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemParent::parentId() const
{
    return m_detail.value(QOrganizerItemParent::FieldParentId).value<QOrganizerItemId>().toString();
}


/*!
    \qmlclass Priority QDeclarativeOrganizerItemPriority
    \brief The Priority element contains the priority of the organizer item, which may be used to resolve scheduling conflicts.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemPriority::QDeclarativeOrganizerItemPriority(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemPriority());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemPriority::type() const
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
    if (newPriority != priority() && !readOnly()) {
        m_detail.setValue(QOrganizerItemPriority::FieldPriority, static_cast<int>(newPriority));
        emit detailChanged();
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
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemRecurrence::QDeclarativeOrganizerItemRecurrence(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemRecurrence());
    connect(this, SIGNAL(recurrenceRulesChanged()), SLOT(_saveRecurrenceRules()));
    connect(this, SIGNAL(exceptionRulesChanged()), SLOT(_saveExceptionRules()));
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemRecurrence::type() const
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
    if (dates != recurrenceDates() && !readOnly()) {
        QSet<QDate> dateSet;
        QVariant dateSetVariant;
        foreach (QVariant date, dates) {
            if (date.canConvert(QVariant::Date))
                dateSet.insert(date.toDate());
        }
        dateSetVariant.setValue(dateSet);
        m_detail.setValue(QOrganizerItemRecurrence::FieldRecurrenceDates, dateSetVariant);
        emit detailChanged();
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
    if (dates != exceptionDates() && !readOnly()) {
        QSet<QDate> dateSet;
        QVariant dateSetVariant;
        foreach (QVariant date, dates) {
            if (date.canConvert(QVariant::Date))
                dateSet.insert(date.toDate());
        }
        dateSetVariant.setValue(dateSet);
        m_detail.setValue(QOrganizerItemRecurrence::FieldExceptionDates, dateSetVariant);
        emit detailChanged();
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
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemTag::QDeclarativeOrganizerItemTag(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemTag());
}

QDeclarativeOrganizerItemTag::ItemDetailType QDeclarativeOrganizerItemTag::type() const
{
    return QDeclarativeOrganizerItemDetail::Tag;
}

/*!
    \qmlproperty string Tag::tag

    This property holds the tag string.
 */
void QDeclarativeOrganizerItemTag::setTag(const QString &newTag)
{
    if (newTag != tag() && !readOnly()) {
        m_detail.setValue(QOrganizerItemTag::FieldTag, newTag);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizerItemTag::tag() const
{
    return m_detail.value(QOrganizerItemTag::FieldTag).toString();
}


/*!
    \qmlclass Timestamp QDeclarativeOrganizerItemTimestamp
    \brief The Timestamp element contains the created and last modified timestamp of an organizer item's creating date and time.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemTimestamp::QDeclarativeOrganizerItemTimestamp(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemTimestamp());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemTimestamp::type() const
{
    return QDeclarativeOrganizerItemDetail::Timestamp;
}

/*!
    \qmlproperty date Timestamp::created

    This property holds the value of the item's creation date and time.
 */
void QDeclarativeOrganizerItemTimestamp::setCreated(const QDateTime &timestamp)
{
    if (timestamp != created() && !readOnly()) {
        m_detail.setValue(QOrganizerItemTimestamp::FieldCreationTimestamp, timestamp.toUTC());
        emit detailChanged();
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
    if (timestamp != lastModified() && !readOnly()) {
        m_detail.setValue(QOrganizerItemTimestamp::FieldModificationTimestamp, timestamp.toUTC());
        emit detailChanged();
    }
}

QDateTime QDeclarativeOrganizerItemTimestamp::lastModified() const
{
    return m_detail.value<QDateTime>(QOrganizerItemTimestamp::FieldModificationTimestamp).toLocalTime();
}


/*!
    \qmlclass Type QDeclarativeOrganizerItemType
    \brief The Type element contains the type string of an organizer item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemType::QDeclarativeOrganizerItemType(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemType());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemType::type() const
{
    return QDeclarativeOrganizerItemDetail::Type;
}

/*!
    \qmlproperty enum Type::itemType

    This property holds the type of the item. The value can be one of:
    \list
    \o Type.Event
    \o Type.EventOccurrence
    \o Type.Todo
    \o Type.TodoOccurrence
    \o Type.Note
    \o Type.Journal
    \o Type.Customized
    \endlist
 */
void QDeclarativeOrganizerItemType::setItemType(OrganizerItemType newType)
{
    if (newType != itemType() && !readOnly()) {
        switch (newType) {
        case Event:
            m_detail.setValue(QOrganizerItemType::FieldType, QOrganizerItemType::TypeEvent);
            break;
        case EventOccurrence:
            m_detail.setValue(QOrganizerItemType::FieldType, QOrganizerItemType::TypeEventOccurrence);
            break;
        case Todo:
            m_detail.setValue(QOrganizerItemType::FieldType, QOrganizerItemType::TypeTodo);
            break;
        case TodoOccurrence:
            m_detail.setValue(QOrganizerItemType::FieldType, QOrganizerItemType::TypeTodoOccurrence);
            break;
        case Note:
            m_detail.setValue(QOrganizerItemType::FieldType, QOrganizerItemType::TypeNote);
            break;
        case Journal:
            m_detail.setValue(QOrganizerItemType::FieldType, QOrganizerItemType::TypeJournal);
            break;
        default:
            return;
        }
        emit detailChanged();
    }
}

QDeclarativeOrganizerItemType::OrganizerItemType QDeclarativeOrganizerItemType::itemType() const
{
    QString typeString = m_detail.value(QOrganizerItemType::FieldType).toString();
    if (typeString == QOrganizerItemType::TypeEvent)
        return Event;
    else if (typeString == QOrganizerItemType::TypeEventOccurrence)
        return EventOccurrence;
    else if (typeString == QOrganizerItemType::TypeTodo)
        return Todo;
    else if (typeString == QOrganizerItemType::TypeTodoOccurrence)
        return TodoOccurrence;
    else if (typeString == QOrganizerItemType::TypeNote)
        return Note;
    else if (typeString == QOrganizerItemType::TypeJournal)
        return Journal;
    return Customized;
}


/*!
    \qmlclass JournalTime QDeclarativeOrganizerJournalTime
    \brief The JournalTime element contains the entry date and time of a journal item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerJournalTime::QDeclarativeOrganizerJournalTime(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerJournalTime());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerJournalTime::type() const
{
    return QDeclarativeOrganizerItemDetail::JournalTime;
}

/*!
  \qmlproperty date JournalTime::entryDateTime

  This property holds the entry date and time value of the journal.
  */
void QDeclarativeOrganizerJournalTime::setEntryDateTime(const QDateTime &datetime)
{
    if (datetime != entryDateTime() && !readOnly()) {
        m_detail.setValue(QOrganizerJournalTime::FieldEntryDateTime, datetime.toUTC());
        emit detailChanged();
    }
}
QDateTime QDeclarativeOrganizerJournalTime::entryDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerJournalTime::FieldEntryDateTime).toLocalTime();
}


/*!
    \qmlclass TodoProgress QDeclarativeOrganizerTodoProgress
    \brief The TodoProgress element contains information about the progress of a todo item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerTodoProgress::QDeclarativeOrganizerTodoProgress(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerTodoProgress());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerTodoProgress::type() const
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
    if (newPercentage != percentage() && !readOnly()) {
        if (newPercentage >=0 && newPercentage <= 100) {
            m_detail.setValue(QOrganizerTodoProgress::FieldPercentageComplete, newPercentage);
            emit detailChanged();
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
    if (datetime != finishedDateTime() && !readOnly()) {
        m_detail.setValue(QOrganizerTodoProgress::FieldFinishedDateTime, datetime.toUTC());
        emit detailChanged();
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
    if (newStatus != status() && !readOnly()) {
        m_detail.setValue(QOrganizerTodoProgress::FieldStatus, (int) newStatus);
        emit detailChanged();
    }
}

QDeclarativeOrganizerTodoProgress::StatusType QDeclarativeOrganizerTodoProgress::status() const
{
    return (StatusType) m_detail.value<int>(QOrganizerTodoProgress::FieldStatus);
}


/*!
    \qmlclass TodoTime QDeclarativeOrganizerTodoTime
    \brief The TodoTime element contains the start and due dates and times of a recurring todo series, or occurrence of an todo item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerTodoTime::QDeclarativeOrganizerTodoTime(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerTodoTime());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerTodoTime::type() const
{
    return QDeclarativeOrganizerItemDetail::TodoTime;
}

/*!
    \qmlproperty date TodoTime::allDay

    This property holds whether the time is significant in the start datetime.
 */
void QDeclarativeOrganizerTodoTime::setAllDay(bool allDay)
{
    if (allDay != isAllDay() && !readOnly()) {
        m_detail.setValue(QOrganizerTodoTime::FieldAllDay, allDay);
        emit detailChanged();
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
    if (datetime != startDateTime() && !readOnly()) {
        m_detail.setValue(QOrganizerTodoTime::FieldStartDateTime, datetime.toUTC());
        emit detailChanged();
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
    if (dateTime != dueDateTime() && !readOnly()) {
        m_detail.setValue(QOrganizerTodoTime::FieldDueDateTime, dateTime.toUTC());
        emit detailChanged();
    }
}

QDateTime QDeclarativeOrganizerTodoTime::dueDateTime() const
{
    return m_detail.value<QDateTime>(QOrganizerTodoTime::FieldDueDateTime).toLocalTime();
}


/*!
    \qmlclass Reminder QDeclarativeOrganizerItemReminder
    \brief The Reminder element contains information about when and how the user wants to reminded of the item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemReminder::QDeclarativeOrganizerItemReminder(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemReminder());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemReminder::type() const
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
    if (m_detail.definitionName() == QOrganizerItemAudibleReminder::DefinitionName)
        return QDeclarativeOrganizerItemReminder::AudibleReminder;
    else if (m_detail.definitionName() == QOrganizerItemEmailReminder::DefinitionName)
        return QDeclarativeOrganizerItemReminder::EmailReminder;
    else if (m_detail.definitionName() == QOrganizerItemVisualReminder::DefinitionName)
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
    if (count != repetitionCount() && !readOnly()) {
        m_detail.setValue(QOrganizerItemReminder::FieldRepetitionCount, count);
        emit detailChanged();
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
    if (delaySeconds != repetitionDelay() && !readOnly()) {
        m_detail.setValue(QOrganizerItemReminder::FieldRepetitionDelay, delaySeconds);
        emit detailChanged();
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
    if (seconds != secondsBeforeStart() && !readOnly()) {
        m_detail.setValue(QOrganizerItemReminder::FieldSecondsBeforeStart, seconds);
        emit detailChanged();
    }
}

int QDeclarativeOrganizerItemReminder::secondsBeforeStart() const
{
    return m_detail.value<int>(QOrganizerItemReminder::FieldSecondsBeforeStart);
}


/*!
    \qmlclass AudibleReminder QDeclarativeOrganizerItemAudibleReminder
    \brief The AudibleReminder element contains information about an audible reminder of an item.
    \ingroup qml-organizer
    \inherits Reminder

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemAudibleReminder::QDeclarativeOrganizerItemAudibleReminder(QObject *parent)
    : QDeclarativeOrganizerItemReminder(parent)
{
    setDetail(QOrganizerItemAudibleReminder());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemAudibleReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::AudibleReminder;
}

/*!
    \qmlproperty url AudibleReminder::dataUrl

    This property holds the url of the audible data to play.
 */
void QDeclarativeOrganizerItemAudibleReminder::setDataUrl(const QUrl &url)
{
    if (url != dataUrl() && !readOnly()) {
        m_detail.setValue(QOrganizerItemAudibleReminder::FieldDataUrl, url);
        emit detailChanged();
    }
}

QUrl QDeclarativeOrganizerItemAudibleReminder::dataUrl() const
{
    return m_detail.value<QUrl>(QOrganizerItemAudibleReminder::FieldDataUrl);
}


/*!
    \qmlclass EmailReminder QDeclarativeOrganizerItemEmailReminder
    \brief The EmailReminder element contains information about an email reminder of an item.
    \ingroup qml-organizer
    \inherits Reminder

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemEmailReminder::QDeclarativeOrganizerItemEmailReminder(QObject *parent)
    : QDeclarativeOrganizerItemReminder(parent)
{
    setDetail(QOrganizerItemEmailReminder());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemEmailReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::EmailReminder;
}

/*!
    \qmlproperty string EmailReminder::body

    This property holds the body of the email.
 */
void QDeclarativeOrganizerItemEmailReminder::setBody(const QString &newBody)
{
    if (newBody != body() && !readOnly()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldBody, newBody);
        emit detailChanged();
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
    if (newSubject != subject() && !readOnly()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldSubject, newSubject);
        emit detailChanged();
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
    if (newRecipients != recipients() && !readOnly()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldRecipients, newRecipients);
        emit detailChanged();
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
    if (newAttachments != attachments() && !readOnly()) {
        m_detail.setValue(QOrganizerItemEmailReminder::FieldAttachments, newAttachments);
        emit detailChanged();
    }
}

QVariantList QDeclarativeOrganizerItemEmailReminder::attachments()
{
    return m_detail.value<QVariantList>(QOrganizerItemEmailReminder::FieldAttachments);
}


/*!
    \qmlclass VisualReminder QDeclarativeOrganizerItemVisualReminder
    \brief The VisualReminder element contains information about a visual reminder of an item.
    \ingroup qml-organizer
    \inherits Reminder

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizerItemVisualReminder::QDeclarativeOrganizerItemVisualReminder(QObject *parent)
    : QDeclarativeOrganizerItemReminder(parent)
{
    setDetail(QOrganizerItemVisualReminder());
}

QDeclarativeOrganizerItemDetail::ItemDetailType QDeclarativeOrganizerItemVisualReminder::type() const
{
    return QDeclarativeOrganizerItemDetail::VisualReminder;
}

/*!
    \qmlproperty string VisualReminder::message

    This property holds the message which the user wishes to be displayed as part of the reminder.
 */
void QDeclarativeOrganizerItemVisualReminder::setMessage(const QString &msg)
{
    if (msg != message() && !readOnly()) {
        m_detail.setValue(QOrganizerItemVisualReminder::FieldMessage, msg);
        emit detailChanged();
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
    if (url != dataUrl() && !readOnly()) {
        m_detail.setValue(QOrganizerItemVisualReminder::FieldDataUrl, url);
        emit detailChanged();
    }
}

QUrl QDeclarativeOrganizerItemVisualReminder::dataUrl() const
{
    return m_detail.value<QUrl>(QOrganizerItemVisualReminder::FieldDataUrl);
}


/*!
    \qmlclass ExtendedDetail QDeclarativeOrganizeritemExtendedDetail
    \brief The ExtendedDetail element contains a extended detail of an organizer item.
    \ingroup qml-organizer

    This element is part of the \bold{QtOrganizer 2.0} module.
 */
QDeclarativeOrganizeritemExtendedDetail::QDeclarativeOrganizeritemExtendedDetail(QObject *parent)
    : QDeclarativeOrganizerItemDetail(parent)
{
    setDetail(QOrganizerItemExtendedDetail());
}

QDeclarativeOrganizeritemExtendedDetail::ItemDetailType QDeclarativeOrganizeritemExtendedDetail::type() const
{
    return QDeclarativeOrganizerItemDetail::Customized;
}

/*!
    \qmlproperty string ExtendedDetail::name

    This property holds the name of the extended detail.
 */
void QDeclarativeOrganizeritemExtendedDetail::setName(const QString &newDetailName)
{
    if (newDetailName != name() && !readOnly()) {
        m_detail.setValue(QOrganizerItemExtendedDetail::FieldExtendedDetailName, newDetailName);
        emit detailChanged();
    }
}

QString QDeclarativeOrganizeritemExtendedDetail::name() const
{
    return m_detail.value(QOrganizerItemExtendedDetail::FieldExtendedDetailName).toString();
}

/*!
    \qmlproperty variant ExtendedDetail::data

    This property holds the data of the extended detail.
 */
void QDeclarativeOrganizeritemExtendedDetail::setData(const QVariant &data)
{
    if (!readOnly()) {
        setValue(QOrganizerItemExtendedDetail::FieldExtendedDetailData, data);
        emit detailChanged();
    }
}

QVariant QDeclarativeOrganizeritemExtendedDetail::data() const
{
    return m_detail.value(QOrganizerItemExtendedDetail::FieldExtendedDetailData);
}


QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItemDetailFactory::createItemDetail(QDeclarativeOrganizerItemDetail::ItemDetailType type)
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
    else if (type == QDeclarativeOrganizerItemDetail::Type)
        itemDetail = new QDeclarativeOrganizerItemType;
    else if (type == QDeclarativeOrganizerItemDetail::VisualReminder)
        itemDetail = new QDeclarativeOrganizerItemVisualReminder;
    else if (type == QDeclarativeOrganizerItemDetail::JournalTime)
        itemDetail = new QDeclarativeOrganizerJournalTime;
    else if (type == QDeclarativeOrganizerItemDetail::TodoProgress)
        itemDetail = new QDeclarativeOrganizerTodoProgress;
    else if (type == QDeclarativeOrganizerItemDetail::TodoTime)
        itemDetail = new QDeclarativeOrganizerTodoTime;
    else if (type == QDeclarativeOrganizerItemDetail::Customized)
        itemDetail = new QDeclarativeOrganizeritemExtendedDetail;
    else
        itemDetail = new QDeclarativeOrganizerItemDetail;
    return itemDetail;
}

QDeclarativeOrganizerItemDetail *QDeclarativeOrganizerItemDetailFactory::createItemDetail(const QString &definitionName)
{
    QDeclarativeOrganizerItemDetail *itemDetail;
    if (definitionName == QOrganizerEventTime::DefinitionName)
        itemDetail = new QDeclarativeOrganizerEventTime;
    else if (definitionName == QOrganizerItemAudibleReminder::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemAudibleReminder;
    else if (definitionName == QOrganizerItemComment::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemComment;
    else if (definitionName == QOrganizerItemDescription::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemDescription;
    else if (definitionName == QOrganizerItemDisplayLabel::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemDisplayLabel;
    else if (definitionName == QOrganizerItemEmailReminder::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemEmailReminder;
    else if (definitionName == QOrganizerItemGuid::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemGuid;
    else if (definitionName == QOrganizerItemLocation::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemLocation;
    else if (definitionName == QOrganizerItemParent::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemParent;
    else if (definitionName == QOrganizerItemPriority::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemPriority;
    else if (definitionName == QOrganizerItemRecurrence::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemRecurrence;
    else if (definitionName == QOrganizerItemReminder::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemReminder;
    else if (definitionName == QOrganizerItemTag::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemTag;
    else if (definitionName == QOrganizerItemTimestamp::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemTimestamp;
    else if (definitionName == QOrganizerItemType::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemType;
    else if (definitionName == QOrganizerItemVisualReminder::DefinitionName)
        itemDetail = new QDeclarativeOrganizerItemVisualReminder;
    else if (definitionName == QOrganizerJournalTime::DefinitionName)
        itemDetail = new QDeclarativeOrganizerJournalTime;
    else if (definitionName == QOrganizerTodoProgress::DefinitionName)
        itemDetail = new QDeclarativeOrganizerTodoProgress;
    else if (definitionName == QOrganizerTodoTime::DefinitionName)
        itemDetail = new QDeclarativeOrganizerTodoTime;
    else if (definitionName == QOrganizerItemExtendedDetail::DefinitionName)
        itemDetail = new QDeclarativeOrganizeritemExtendedDetail;
    else
        itemDetail = new QDeclarativeOrganizerItemDetail;
    return itemDetail;
}

// used by filters
QString QDeclarativeOrganizerEventTime::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerEventTime::FieldStartDateTime;
    case 1:
        return QOrganizerEventTime::FieldEndDateTime;
    case 2:
        return QOrganizerEventTime::FieldAllDay;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

QString QDeclarativeOrganizerItemType::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemType::FieldType;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerJournalTime::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerJournalTime::FieldEntryDateTime;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerTodoProgress::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerTodoProgress::FieldStatus;
    case 1:
        return QOrganizerTodoProgress::FieldPercentageComplete;
    case 2:
        return QOrganizerTodoProgress::FieldFinishedDateTime;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

QString QDeclarativeOrganizerTodoTime::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerTodoTime::FieldAllDay;
    case 1:
        return QOrganizerTodoTime::FieldStartDateTime;
    case 2:
        return QOrganizerTodoTime::FieldDueDateTime;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemComment::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemComment::FieldComment;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemDescription::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemDescription::FieldDescription;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemDisplayLabel::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemDisplayLabel::FieldLabel;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemGuid::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemGuid::FieldGuid;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemParent::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemParent::FieldParentId;
    case 1:
        return QOrganizerItemParent::FieldOriginalDate;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

QString QDeclarativeOrganizerItemLocation::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemLocation::FieldLatitude;
    case 1:
        return QOrganizerItemLocation::FieldLongitude;
    case 2:
        return QOrganizerItemLocation::FieldLabel;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

QString QDeclarativeOrganizerItemPriority::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemPriority::FieldPriority;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

QString QDeclarativeOrganizerItemRecurrence::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemRecurrence::FieldRecurrenceRules;
    case 1:
        return QOrganizerItemRecurrence::FieldExceptionRules;
    case 2:
        return QOrganizerItemRecurrence::FieldRecurrenceDates;
    case 3:
        return QOrganizerItemRecurrence::FieldExceptionDates;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemReminder::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemReminder::FieldSecondsBeforeStart;
    case 1:
        return QOrganizerItemReminder::FieldRepetitionCount;
    case 2:
        return QOrganizerItemReminder::FieldRepetitionDelay;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemAudibleReminder::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemAudibleReminder::FieldDataUrl;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}



QString QDeclarativeOrganizerItemVisualReminder::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemVisualReminder::FieldDataUrl;
    case 1:
        return QOrganizerItemVisualReminder::FieldMessage;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}


QString QDeclarativeOrganizerItemEmailReminder::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemEmailReminder::FieldSubject;
    case 1:
        return QOrganizerItemEmailReminder::FieldBody;
    case 2:
        return QOrganizerItemEmailReminder::FieldRecipients;
    case 3:
        return QOrganizerItemEmailReminder::FieldAttachments;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}



QString QDeclarativeOrganizerItemTimestamp::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemTimestamp::FieldModificationTimestamp;
    case 1:
        return QOrganizerItemTimestamp::FieldCreationTimestamp;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

QString QDeclarativeOrganizeritemExtendedDetail::fieldNameFromFieldType(int type)
{
    switch (type) {
    case 0:
        return QOrganizerItemExtendedDetail::FieldExtendedDetailName;
    case 1:
        return QOrganizerItemExtendedDetail::FieldExtendedDetailData;
    }
    qmlInfo(0) << tr("invalid field type:") << type;
    return QString();
}

#include "moc_qdeclarativeorganizeritemdetail_p.cpp"

QTORGANIZER_END_NAMESPACE
