/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QDECLARATIVEORGANIZERITEMDETAIL_H
#define QDECLARATIVEORGANIZERITEMDETAIL_H

#include <qorganizeritemdetails.h>
#include "qdeclarativeorganizerrecurrencerule_p.h"

QTORGANIZER_BEGIN_NAMESPACE

class QDeclarativeOrganizerItemDetail : public QObject
{
    Q_OBJECT

    Q_ENUMS(DetailType)

    Q_PROPERTY(DetailType type READ type)

    // to be removed
    Q_PROPERTY(QList<int> fieldNames READ fieldNames)

public:
    enum DetailType {
        Undefined = QOrganizerItemDetail::TypeUndefined,
        Classification = QOrganizerItemDetail::TypeClassification,
        Comment = QOrganizerItemDetail::TypeComment,
        Description = QOrganizerItemDetail::TypeDescription,
        DisplayLabel = QOrganizerItemDetail::TypeDisplayLabel,
        ItemType = QOrganizerItemDetail::TypeItemType,
        Guid = QOrganizerItemDetail::TypeGuid,
        Location = QOrganizerItemDetail::TypeLocation,
        Parent = QOrganizerItemDetail::TypeParent,
        Priority = QOrganizerItemDetail::TypePriority,
        Recurrence = QOrganizerItemDetail::TypeRecurrence,
        Tag = QOrganizerItemDetail::TypeTag,
        Timestamp = QOrganizerItemDetail::TypeTimestamp,
        Version = QOrganizerItemDetail::TypeVersion,
        Reminder = QOrganizerItemDetail::TypeReminder,
        AudibleReminder = QOrganizerItemDetail::TypeAudibleReminder,
        EmailReminder = QOrganizerItemDetail::TypeEmailReminder,
        VisualReminder = QOrganizerItemDetail::TypeVisualReminder,
        ExtendedDetail = QOrganizerItemDetail::TypeExtendedDetail,
        EventAttendee = QOrganizerItemDetail::TypeEventAttendee,
        EventRsvp = QOrganizerItemDetail::TypeEventRsvp,
        EventTime = QOrganizerItemDetail::TypeEventTime,
        JournalTime = QOrganizerItemDetail::TypeJournalTime,
        TodoTime = QOrganizerItemDetail::TypeTodoTime,
        TodoProgress = QOrganizerItemDetail::TypeTodoProgress
    };

    explicit QDeclarativeOrganizerItemDetail(QObject *parent = 0);
    ~QDeclarativeOrganizerItemDetail();

    virtual DetailType type() const;

    // QML functions
    Q_INVOKABLE QVariant value(int key) const;
    Q_INVOKABLE bool setValue(int key, const QVariant& value);
    Q_INVOKABLE bool removeValue(int key);

    // non-QML APIs
    QOrganizerItemDetail detail() const;
    void setDetail(const QOrganizerItemDetail &detail);

    // to be removed
    QList<int> fieldNames() const;

Q_SIGNALS:
    void detailChanged();

protected:
    QOrganizerItemDetail m_detail;

private:
    Q_DISABLE_COPY(QDeclarativeOrganizerItemDetail)
};


class QDeclarativeOrganizerEventTime : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(EventTimeField)

    Q_PROPERTY(bool isAllDay READ isAllDay WRITE setAllDay NOTIFY valueChanged)// Will be removed, use "allDay" instead!
    Q_PROPERTY(bool allDay READ isAllDay WRITE setAllDay NOTIFY valueChanged)
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime endDateTime READ endDateTime WRITE setEndDateTime NOTIFY valueChanged)

public:
    enum EventTimeField {
        FieldStartDateTime = QOrganizerEventTime::FieldStartDateTime,
        FieldEndDateTime = QOrganizerEventTime::FieldEndDateTime,
        FieldAllDay = QOrganizerEventTime::FieldAllDay
    };

    QDeclarativeOrganizerEventTime(QObject *parent = 0);

    virtual DetailType type() const;

    void setAllDay(bool isAllDay);
    bool isAllDay();

    void setStartDateTime(const QDateTime &datetime);
    QDateTime startDateTime() const;

    void setEndDateTime(const QDateTime &datetime);
    QDateTime endDateTime() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemComment : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(CommentField)

    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY valueChanged)

public:
    enum CommentField {
        FieldComment = QOrganizerItemComment::FieldComment
    };

    QDeclarativeOrganizerItemComment(QObject *parent = 0);

    virtual DetailType type() const;

    void setComment(const QString &newComment);
    QString comment() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemDescription : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(DescriptionField)

    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY valueChanged)

public:
    enum DescriptionField {
        FieldDescription = QOrganizerItemDescription::FieldDescription
    };

    QDeclarativeOrganizerItemDescription(QObject *parent = 0);

    virtual DetailType type() const;

    void setDescription(const QString &desc);
    QString description() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemDisplayLabel : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(DisplayLabelField)

    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY valueChanged)

public:
    enum DisplayLabelField {
        FieldLabel = QOrganizerItemDisplayLabel::FieldLabel
    };

    QDeclarativeOrganizerItemDisplayLabel(QObject *parent = 0);

    virtual DetailType type() const;

    void setLabel(const QString &newLabel);
    QString label() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemGuid : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(GuidField)

    Q_PROPERTY(QString guid READ guid WRITE setGuid NOTIFY valueChanged)

public:
    enum GuidField {
        FieldGuid = QOrganizerItemGuid::FieldGuid
    };

    QDeclarativeOrganizerItemGuid(QObject *parent = 0);

    virtual DetailType type() const;

    void setGuid(const QString &newGuid);
    QString guid() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemLocation : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(LocationField)

    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY valueChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY valueChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY valueChanged)

public:
    enum LocationField {
        FieldLabel = QOrganizerItemLocation::FieldLabel,
        FieldLatitude = QOrganizerItemLocation::FieldLatitude,
        FieldLongitude = QOrganizerItemLocation::FieldLongitude
    };

    QDeclarativeOrganizerItemLocation(QObject *parent = 0);

    virtual DetailType type() const;

    void setLatitude(double newLatitude);
    double latitude() const;

    void setLongitude(double newLongitude);
    double longitude() const;

    void setLabel(const QString &newLabel);
    QString label() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemParent : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(ParentField)

    Q_PROPERTY(QDate originalDate READ originalDate WRITE setOriginalDate NOTIFY valueChanged)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY valueChanged)

public:
    enum ParentField {
        FieldParentId = QOrganizerItemParent::FieldParentId,
        FieldOriginalDate = QOrganizerItemParent::FieldOriginalDate
    };

    QDeclarativeOrganizerItemParent(QObject *parent = 0);

    virtual DetailType type() const;

    void setOriginalDate(const QDate &date);
    QDate originalDate() const;

    void setParentId(const QString &newParentId);
    QString parentId() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemPriority : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(PriorityField)
    Q_ENUMS(Priority)

    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY valueChanged)

public:
    enum PriorityField {
        FieldPriority = QOrganizerItemPriority::FieldPriority
    };

    enum Priority {
        Unknown = QOrganizerItemPriority::UnknownPriority,
        Highest = QOrganizerItemPriority::HighestPriority,
        ExtremelyHigh = QOrganizerItemPriority::ExtremelyHighPriority,
        VeryHigh = QOrganizerItemPriority::VeryHighPriority,
        High = QOrganizerItemPriority::HighPriority,
        Medium = QOrganizerItemPriority::MediumPriority,
        Low = QOrganizerItemPriority::LowPriority,
        VeryLow = QOrganizerItemPriority::VeryLowPriority,
        ExtremelyLow = QOrganizerItemPriority::ExtremelyLowPriority,
        Lowest = QOrganizerItemPriority::LowestPriority
    };

    QDeclarativeOrganizerItemPriority(QObject *parent = 0);

    virtual DetailType type() const;

    void setPriority(Priority newPriority);
    Priority priority() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemRecurrence : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(RecurrenceField)

    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> recurrenceRules READ recurrenceRules NOTIFY recurrenceRulesChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> exceptionRules READ exceptionRules NOTIFY exceptionRulesChanged)
    Q_PROPERTY(QVariantList recurrenceDates READ recurrenceDates WRITE setRecurrenceDates NOTIFY valueChanged)
    Q_PROPERTY(QVariantList exceptionDates  READ exceptionDates WRITE setExceptionDates NOTIFY valueChanged)

public:
    enum RecurrenceField {
        FieldRecurrenceRules = QOrganizerItemRecurrence::FieldRecurrenceRules,
        FieldExceptionRules = QOrganizerItemRecurrence::FieldExceptionRules,
        FieldRecurrenceDates = QOrganizerItemRecurrence::FieldRecurrenceDates,
        FieldExceptionDates = QOrganizerItemRecurrence::FieldExceptionDates
    };

    QDeclarativeOrganizerItemRecurrence(QObject *parent = 0);

    virtual DetailType type() const;

    QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> recurrenceRules();

    QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> exceptionRules();

    void setRecurrenceDates(const QVariantList &dates);
    QVariantList recurrenceDates() const;

    void setExceptionDates(const QVariantList &dates);
    QVariantList exceptionDates() const;

Q_SIGNALS:
    void recurrenceRulesChanged();
    void exceptionRulesChanged();
    void valueChanged();

private Q_SLOTS:
    void _saveRecurrenceRules();
    void _saveExceptionRules();

private:
    static void rrule_append(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p, QDeclarativeOrganizerRecurrenceRule *item);
    static void xrule_append(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p, QDeclarativeOrganizerRecurrenceRule *item);
    static int  rule_count(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p);
    static QDeclarativeOrganizerRecurrenceRule *rule_at(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p, int idx);
    static void  rrule_clear(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p);
    static void  xrule_clear(QDeclarativeListProperty<QDeclarativeOrganizerRecurrenceRule> *p);

    QList<QDeclarativeOrganizerRecurrenceRule*>   m_recurrenceRules;
    QList<QDeclarativeOrganizerRecurrenceRule*>   m_exceptionRules;
};


class QDeclarativeOrganizerItemTag : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(TagField)

    Q_PROPERTY(QString tag READ tag WRITE setTag NOTIFY valueChanged)

public:
    enum TagField {
        FieldTag = QOrganizerItemTag::FieldTag
    };

    QDeclarativeOrganizerItemTag(QObject *parent = 0);

    virtual DetailType type() const;

    void setTag(const QString &newTag);
    QString tag() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemTimestamp : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(TimestampField)

    Q_PROPERTY(QDateTime created READ created WRITE setCreated NOTIFY valueChanged)
    Q_PROPERTY(QDateTime lastModified READ lastModified WRITE setLastModified NOTIFY valueChanged)

public:
    enum TimestampField {
        FieldCreated = QOrganizerItemTimestamp::FieldCreated,
        FieldLastModified = QOrganizerItemTimestamp::FieldLastModified
    };

    QDeclarativeOrganizerItemTimestamp(QObject *parent = 0);

    virtual DetailType type() const;

    void setCreated(const QDateTime &timestamp);
    QDateTime created() const;

    void setLastModified(const QDateTime &timestamp);
    QDateTime lastModified() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemType : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(ItemTypeField)
    Q_ENUMS(ItemType)

    Q_PROPERTY(ItemType itemType READ itemType WRITE setItemType NOTIFY valueChanged)

public:
    enum ItemTypeField {
        FieldType = QOrganizerItemType::FieldType
    };

    enum ItemType {
        Undefined = QOrganizerItemType::TypeUndefined,
        Event = QOrganizerItemType::TypeEvent,
        EventOccurrence = QOrganizerItemType::TypeEventOccurrence,
        Todo = QOrganizerItemType::TypeTodo,
        TodoOccurrence = QOrganizerItemType::TypeTodoOccurrence,
        Journal = QOrganizerItemType::TypeJournal,
        Note = QOrganizerItemType::TypeNote
    };

    QDeclarativeOrganizerItemType(QObject *parent = 0);

    virtual DetailType type() const;

    void setItemType(ItemType newType);
    ItemType itemType() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerJournalTime : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(JournalTimeField)

    Q_PROPERTY(QDateTime entryDateTime READ entryDateTime WRITE setEntryDateTime NOTIFY valueChanged)

public:
    enum JournalTimeField {
        FieldEntryDateTime = QOrganizerJournalTime::FieldEntryDateTime
    };

    QDeclarativeOrganizerJournalTime(QObject *parent = 0);

    virtual DetailType type() const;

    void setEntryDateTime(const QDateTime &datetime);
    QDateTime entryDateTime() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerTodoProgress : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(TodoProgressField)
    Q_ENUMS(StatusType)

    Q_PROPERTY(int percentage READ percentageComplete WRITE setPercentageComplete NOTIFY valueChanged)// Will be removed, use "percentageComplete" instead!
    Q_PROPERTY(int percentageComplete READ percentageComplete WRITE setPercentageComplete NOTIFY valueChanged)
    Q_PROPERTY(QDateTime finishedDateTime READ finishedDateTime WRITE setFinishedDateTime NOTIFY valueChanged)
    Q_PROPERTY(StatusType status READ status WRITE setStatus NOTIFY valueChanged)

public:
    enum TodoProgressField {
        FieldStatus = QOrganizerTodoProgress::FieldStatus,
        FieldPercentageComplete = QOrganizerTodoProgress::FieldPercentageComplete,
        FieldFinishedDateTime = QOrganizerTodoProgress::FieldFinishedDateTime
    };

    enum StatusType {
        NotStarted = QOrganizerTodoProgress::StatusNotStarted,
        InProgress = QOrganizerTodoProgress::StatusInProgress,
        Complete = QOrganizerTodoProgress::StatusComplete
    };

    QDeclarativeOrganizerTodoProgress(QObject *parent = 0);

    virtual DetailType type() const;

    void setPercentageComplete(int percentageComplete);
    int percentageComplete() const;

    void setFinishedDateTime(const QDateTime &datetime);
    QDateTime finishedDateTime() const;

    void setStatus(StatusType newStatus);
    StatusType status() const;

Q_SIGNALS:
    void valueChanged();
};

class QDeclarativeOrganizerTodoTime : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(TodoTimeField)

    Q_PROPERTY(bool allDay READ isAllDay WRITE setAllDay NOTIFY valueChanged)
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime dueDateTime READ dueDateTime WRITE setDueDateTime NOTIFY valueChanged)

public:
    enum TodoTimeField {
        FieldStartDateTime = QOrganizerTodoTime::FieldStartDateTime,
        FieldDueDateTime = QOrganizerTodoTime::FieldDueDateTime,
        FieldAllDay = QOrganizerTodoTime::FieldAllDay
    };

    QDeclarativeOrganizerTodoTime(QObject *parent = 0);

    virtual DetailType type() const;

    void setAllDay(bool isAllDay);
    bool isAllDay();

    void setStartDateTime(const QDateTime &datetime);
    QDateTime startDateTime() const;

    void setDueDateTime(const QDateTime &dateTime);
    QDateTime dueDateTime() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemReminder : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(ReminderField)
    Q_ENUMS(ReminderType)

    Q_PROPERTY(ReminderType reminderType READ reminderType NOTIFY reminderChanged)
    Q_PROPERTY(int repetitionCount READ repetitionCount WRITE setRepetitionCount NOTIFY reminderChanged)
    Q_PROPERTY(int repetitionDelay READ repetitionDelay WRITE setRepetitionDelay NOTIFY reminderChanged)
    Q_PROPERTY(int secondsBeforeStart READ secondsBeforeStart WRITE setSecondsBeforeStart NOTIFY reminderChanged)

public:
    enum ReminderField {
        FieldRepetitionCount = QOrganizerItemReminder::FieldRepetitionCount,
        FieldRepetitionDelay = QOrganizerItemReminder::FieldRepetitionDelay,
        FieldSecondsBeforeStart = QOrganizerItemReminder::FieldSecondsBeforeStart
    };

    enum ReminderType {
        NoReminder = QOrganizerItemReminder::NoReminder,
        VisualReminder = QOrganizerItemReminder::VisualReminder,
        AudibleReminder = QOrganizerItemReminder::AudibleReminder,
        EmailReminder = QOrganizerItemReminder::EmailReminder
    };

    QDeclarativeOrganizerItemReminder(QObject *parent = 0);

    virtual DetailType type() const;

    ReminderType reminderType() const;

    void setRepetitionCount(int count);
    int repetitionCount() const;

    void setRepetitionDelay(int delaySeconds);
    int repetitionDelay() const;

    void setSecondsBeforeStart(int seconds);
    int secondsBeforeStart() const;

Q_SIGNALS:
    void reminderChanged();
};


class QDeclarativeOrganizerItemAudibleReminder : public QDeclarativeOrganizerItemReminder
{
    Q_OBJECT

    Q_ENUMS(AudibleReminderField)

    Q_PROPERTY(QUrl dataUrl READ dataUrl WRITE setDataUrl NOTIFY valueChanged)

public:
    enum AudibleReminderField {
        FieldDataUrl = QOrganizerItemAudibleReminder::FieldDataUrl
    };

    QDeclarativeOrganizerItemAudibleReminder(QObject *parent = 0);

    virtual DetailType type() const;

    void setDataUrl(const QUrl &url);
    QUrl dataUrl() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemEmailReminder : public QDeclarativeOrganizerItemReminder
{
    Q_OBJECT

    Q_ENUMS(EmailReminderField)

    Q_PROPERTY(QString body READ body WRITE setBody NOTIFY valueChanged)
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY valueChanged)
    Q_PROPERTY(QStringList recipients READ recipients WRITE setRecipients NOTIFY valueChanged)
    Q_PROPERTY(QVariantList attachments READ attachments WRITE setAttachments NOTIFY valueChanged)

public:
    enum EmailReminderField {
        FieldSubject = QOrganizerItemEmailReminder::FieldSubject,
        FieldBody = QOrganizerItemEmailReminder::FieldBody,
        FieldRecipients = QOrganizerItemEmailReminder::FieldRecipients,
        FieldAttachments = QOrganizerItemEmailReminder::FieldAttachments
    };

    QDeclarativeOrganizerItemEmailReminder(QObject *parent = 0);

    virtual DetailType type() const;

    void setBody(const QString &newBody);
    QString body() const;

    void setSubject(const QString &newSubject);
    QString subject() const;

    void setRecipients(const QStringList &newRecipients);
    QStringList recipients() const;

    void setAttachments(const QVariantList &newAttachments);
    QVariantList attachments();

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemVisualReminder : public QDeclarativeOrganizerItemReminder
{
    Q_OBJECT

    Q_ENUMS(VisualReminderField)

    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY valueChanged)
    Q_PROPERTY(QUrl dataUrl READ dataUrl WRITE setDataUrl NOTIFY valueChanged)

public:
    enum VisualReminderField {
        FieldDataUrl = QOrganizerItemVisualReminder::FieldDataUrl,
        FieldMessage = QOrganizerItemVisualReminder::FieldMessage
    };

    QDeclarativeOrganizerItemVisualReminder(QObject *parent = 0);

    virtual DetailType type() const;

    void setMessage(const QString &msg);
    QString message() const;

    void setDataUrl(const QUrl &url);
    QUrl dataUrl() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemExtendedDetail : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(ExtendedDetailField)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY valueChanged)

public:
    enum ExtendedDetailField {
        FieldName = QOrganizerItemExtendedDetail::FieldExtendedDetailName,
        FieldData = QOrganizerItemExtendedDetail::FieldExtendedDetailData
    };

    QDeclarativeOrganizerItemExtendedDetail(QObject *parent = 0);

    virtual DetailType type() const;

    void setName(const QString &newDetailName);
    QString name() const;

    void setData(const QVariant &data);
    QVariant data() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerEventAttendee : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT

    Q_ENUMS(EventAttendeeField)
    Q_ENUMS(ParticipationStatus)
    Q_ENUMS(ParticipationRole)

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
    Q_PROPERTY(QString emailAddress READ emailAddress WRITE setEmailAddress NOTIFY valueChanged)
    Q_PROPERTY(QString attendeeId READ attendeeId WRITE setAttendeeId NOTIFY valueChanged)
    Q_PROPERTY(ParticipationStatus participationStatus READ participationStatus WRITE setParticipationStatus NOTIFY valueChanged)
    Q_PROPERTY(ParticipationRole participationRole READ participationRole WRITE setParticipationRole NOTIFY valueChanged)

public:
    enum EventAttendeeField {
        FieldName = QOrganizerEventAttendee::FieldName,
        FieldEmailAddress = QOrganizerEventAttendee::FieldEmailAddress,
        FieldAddendeeId = QOrganizerEventAttendee::FieldAttendeeId,
        FieldParticipationStatus = QOrganizerEventAttendee::FieldParticipationStatus,
        FieldParticipationRole = QOrganizerEventAttendee::FieldParticipationRole
    };

    enum ParticipationStatus {
        StatusUnknown = QOrganizerEventAttendee::StatusUnknown,
        StatusAccepted = QOrganizerEventAttendee::StatusAccepted,
        StatusDeclined = QOrganizerEventAttendee::StatusDeclined,
        StatusTentative = QOrganizerEventAttendee::StatusTentative,
        StatusDelegated = QOrganizerEventAttendee::StatusDelegated,
        StatusInProcess = QOrganizerEventAttendee::StatusInProcess,
        StatusCompleted = QOrganizerEventAttendee::StatusCompleted
    };

    enum ParticipationRole {
        RoleUnknown = QOrganizerEventAttendee::RoleUnknown,
        RoleOrganizer = QOrganizerEventAttendee::RoleOrganizer,
        RoleChairperson = QOrganizerEventAttendee::RoleChairperson,
        RoleHost = QOrganizerEventAttendee::RoleHost,
        RoleRequiredParticipant = QOrganizerEventAttendee::RoleRequiredParticipant,
        RoleOptionalParticipant = QOrganizerEventAttendee::RoleOptionalParticipant,
        RoleNonParticipant = QOrganizerEventAttendee::RoleNonParticipant
    };

    QDeclarativeOrganizerEventAttendee(QObject *parent = 0);

    virtual DetailType type() const;

    void setName(const QString &newName);
    QString name() const;

    void setEmailAddress(const QString &newEmailAddress);
    QString emailAddress() const;

    void setParticipationStatus(ParticipationStatus status);
    ParticipationStatus participationStatus() const;

    void setParticipationRole(ParticipationRole role);
    ParticipationRole participationRole() const;

    void setAttendeeId(const QString &newAttendeeId);
    QString attendeeId() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerEventRsvp : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeOrganizerEventAttendee::ParticipationStatus participationStatus READ participationStatus WRITE setParticipationStatus NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerEventAttendee::ParticipationRole participationRole READ participationRole WRITE setParticipationRole NOTIFY valueChanged)
    Q_PROPERTY(ResponseRequirement responseRequirement READ responseRequirement WRITE setResponseRequirement NOTIFY valueChanged)
    Q_PROPERTY(QDate responseDeadline READ responseDeadline WRITE setResponseDeadline NOTIFY valueChanged)
    Q_PROPERTY(QDate responseDate READ responseDate WRITE setResponseDate NOTIFY valueChanged)
    Q_PROPERTY(QString organizerName READ organizerName WRITE setOrganizerName NOTIFY valueChanged)
    Q_PROPERTY(QString organizerEmail READ organizerEmail WRITE setOrganizerEmail NOTIFY valueChanged)
    Q_ENUMS(EventRsvpField)
    Q_ENUMS(ResponseRequirement)

public:
    enum EventRsvpField {
        FieldParticipationStatus = QOrganizerEventRsvp::FieldParticipationStatus,
        FieldParticipationRole = QOrganizerEventRsvp::FieldParticipationRole,
        FieldResponseRequirement = QOrganizerEventRsvp::FieldResponseRequirement,
        FieldResponseDeadline = QOrganizerEventRsvp::FieldResponseDeadline,
        FieldResponseDate = QOrganizerEventRsvp::FieldResponseDate,
        FieldOrganizerName = QOrganizerEventRsvp::FieldOrganizerName,
        FieldOrganizerEmail = QOrganizerEventRsvp::FieldOrganizerEmail
    };

    enum ResponseRequirement {
        ResponseNotRequired = QOrganizerEventRsvp::ResponseNotRequired,
        ResponseRequired = QOrganizerEventRsvp::ResponseRequired
    };

    QDeclarativeOrganizerEventRsvp(QObject *parent = 0);

    virtual DetailType type() const;

    void setParticipationStatus(QDeclarativeOrganizerEventAttendee::ParticipationStatus status);
    QDeclarativeOrganizerEventAttendee::ParticipationStatus participationStatus() const;

    void setParticipationRole(QDeclarativeOrganizerEventAttendee::ParticipationRole role);
    QDeclarativeOrganizerEventAttendee::ParticipationRole participationRole() const;

    void setResponseRequirement(ResponseRequirement requirement);
    ResponseRequirement responseRequirement() const;

    void setResponseDeadline(const QDate &date);
    QDate responseDeadline() const;

    void setResponseDate(const QDate &date);
    QDate responseDate() const;

    void setOrganizerName(const QString &name);
    QString organizerName() const;

    void setOrganizerEmail(const QString &email);
    QString organizerEmail() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemClassification : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT
    Q_ENUMS(Field)
    Q_PROPERTY(QString classification READ classification WRITE setClassification NOTIFY valueChanged)

public:
    enum Field {
        FieldClassification = QOrganizerItemClassification::FieldClassification
    };

    QDeclarativeOrganizerItemClassification(QObject *parent = 0);

    virtual DetailType type() const;

    void setClassification(const QString &newClassification);
    QString classification() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemVersion : public QDeclarativeOrganizerItemDetail
{
    Q_OBJECT
    Q_ENUMS(Field)
    Q_PROPERTY(int version READ version WRITE setVersion NOTIFY valueChanged)
    Q_PROPERTY(QString extendedVersion READ extendedVersion WRITE setExtendedVersion NOTIFY valueChanged)

public:
    enum Field {
        FieldVersion = QOrganizerItemVersion::FieldVersion,
        FieldExtendedVersion = QOrganizerItemVersion::FieldExtendedVersion
    };

    QDeclarativeOrganizerItemVersion(QObject *parent = 0);

    virtual DetailType type() const;

    void setVersion(int newVersion);
    int version() const;

    void setExtendedVersion(const QString &newExtendedVersion);
    QString extendedVersion() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerItemDetailFactory
{
public:
    static QDeclarativeOrganizerItemDetail *createItemDetail(QDeclarativeOrganizerItemDetail::DetailType type);
};

QTORGANIZER_END_NAMESPACE

QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemDetail))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerEventTime))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemComment))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemDescription))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemDisplayLabel))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemGuid))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemLocation))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemParent))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemPriority))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemRecurrence))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemTag))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemTimestamp))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemType))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerJournalTime))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerTodoProgress))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerTodoTime))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemReminder))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemAudibleReminder))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemEmailReminder))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemVisualReminder))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemExtendedDetail))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerEventAttendee))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerEventRsvp))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemClassification))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemVersion))

#endif // QDECLARATIVEORGANIZERITEMDETAIL_H
