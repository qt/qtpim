/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEORGANIZERITEM_H
#define QDECLARATIVEORGANIZERITEM_H

#include <qorganizeritems.h>
#include "qdeclarativeorganizeritemdetail_p.h"

#include <QtDeclarative/qdeclarativelist.h>

QTORGANIZER_BEGIN_NAMESPACE

class QDeclarativeOrganizerItem : public QObject
{
    Q_OBJECT

    // basic information
    Q_PROPERTY(bool modified READ modified NOTIFY itemChanged)

    Q_PROPERTY(QDeclarativeOrganizerItemType::ItemType itemType READ itemType NOTIFY itemChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> itemDetails READ itemDetails NOTIFY itemChanged)
    Q_PROPERTY(QString itemId READ itemId NOTIFY itemChanged)
    Q_PROPERTY(QString manager READ manager NOTIFY itemChanged)
    Q_PROPERTY(QString collectionId READ collectionId WRITE setCollectionId NOTIFY itemChanged)

    // convenient access to most frequently used details
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY itemChanged)
    Q_PROPERTY(QString displayLabel READ displayLabel WRITE setDisplayLabel NOTIFY itemChanged)
    Q_PROPERTY(QString guid READ guid WRITE setGuid NOTIFY itemChanged)

    // to be removed
    Q_PROPERTY(bool isOccurrence READ isOccurrence)
    Q_PROPERTY(bool isFloatingTime READ isFloatingTime)
    Q_PROPERTY(QDateTime itemStartTime READ itemStartTime NOTIFY itemChanged)
    Q_PROPERTY(QDateTime itemEndTime READ itemEndTime NOTIFY itemChanged)
    Q_CLASSINFO("DefaultProperty", "itemDetails")

public:
    explicit QDeclarativeOrganizerItem(QObject *parent = 0);
    ~QDeclarativeOrganizerItem();

    // basic information
    bool modified() const;

    QDeclarativeOrganizerItemType::ItemType itemType() const;
    QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> itemDetails();
    QString itemId() const;
    QString manager() const;

    QString collectionId() const;
    void setCollectionId(const QString& collectionId);

    // convenient access to most frequently used details
    QString displayLabel() const;
    void setDisplayLabel(const QString &label);

    QString description() const;
    void setDescription(const QString &description);

    QString guid() const;
    void setGuid(const QString& guid);

    // functions
    // use int instead of QDeclarativeOrganizerItemDetail::ItemDetailType as a work-around for QTBUG-20639
    Q_INVOKABLE QDeclarativeOrganizerItemDetail *detail(int type);
    Q_INVOKABLE QVariantList details(int type);

    virtual Q_INVOKABLE void setDetail(QDeclarativeOrganizerItemDetail *detail);
    virtual Q_INVOKABLE void removeDetail(QDeclarativeOrganizerItemDetail *detail);
    virtual Q_INVOKABLE void clearDetails();

    // non-QML APIs, used internal only
    bool _q_setDetail(QDeclarativeOrganizerItemDetail *detail);
    bool _q_removeDetail(QDeclarativeOrganizerItemDetail *detail);
    bool _q_clearDetails();

    Q_INVOKABLE void save();

    // non-QML APIs, used by model only
    void setItem(const QOrganizerItem &item);
    QOrganizerItem item() const;

    // to be removed
    bool isFloatingTime();

    QDateTime itemStartTime() const;
    QDateTime itemEndTime() const;

    bool isOccurrence() const;

    Q_INVOKABLE bool addDetail(QDeclarativeOrganizerItemDetail* detail);

Q_SIGNALS:
    void itemChanged();

protected:
    bool m_modified;

    QOrganizerItemId m_id;
    QOrganizerCollectionId m_collectionId;

    // always create a copy of the detail for QML
    // however, seems the garbage collection can't delete all of them (QTBUG-20377)
    QList<QDeclarativeOrganizerItemDetail *> m_details;

private:
     Q_DISABLE_COPY(QDeclarativeOrganizerItem)

    // call-back functions for list property
    static void _q_detail_append(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property, QDeclarativeOrganizerItemDetail *value);
    static QDeclarativeOrganizerItemDetail *_q_detail_at(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property, int index);
    static void _q_detail_clear(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property);
    static int _q_detail_count(QDeclarativeListProperty<QDeclarativeOrganizerItemDetail> *property);
};


class QDeclarativeOrganizerEvent : public QDeclarativeOrganizerItem
{
    Q_OBJECT

    Q_PROPERTY(bool allDay READ isAllDay WRITE setAllDay NOTIFY valueChanged)
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime endDateTime READ endDateTime WRITE setEndDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemPriority::Priority priority READ priority WRITE setPriority NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemRecurrence *recurrence READ recurrence NOTIFY valueChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> attendees READ attendees NOTIFY valueChanged)

public:
    explicit QDeclarativeOrganizerEvent(QObject *parent = 0);

    QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> attendees();

    virtual Q_INVOKABLE void setDetail(QDeclarativeOrganizerItemDetail *detail);
    virtual Q_INVOKABLE void removeDetail(QDeclarativeOrganizerItemDetail *detail);
    virtual Q_INVOKABLE void clearDetails();

    void setAllDay(bool isAllDay);
    bool isAllDay() const;

    void setStartDateTime(const QDateTime &startDateTime);
    QDateTime startDateTime() const;

    void setEndDateTime(const QDateTime &endDateTime);
    QDateTime endDateTime() const;

    void setPriority(QDeclarativeOrganizerItemPriority::Priority priority);
    QDeclarativeOrganizerItemPriority::Priority priority() const;

    void setLocation(const QString &location);
    QString location() const;

    QDeclarativeOrganizerItemRecurrence *recurrence();

Q_SIGNALS:
    void valueChanged();

private:
    // call-back functions for attendees list property
    static void _q_attendee_append(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property, QDeclarativeOrganizerEventAttendee *value);
    static QDeclarativeOrganizerEventAttendee *_q_attendee_at(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property, int index);
    static void _q_attendee_clear(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property);
    static int _q_attendee_count(QDeclarativeListProperty<QDeclarativeOrganizerEventAttendee> *property);
};


class QDeclarativeOrganizerEventOccurrence : public QDeclarativeOrganizerItem
{
    Q_OBJECT

    Q_PROPERTY(QDate originalDate READ originalDate WRITE setOriginalDate NOTIFY valueChanged)
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime endDateTime READ endDateTime WRITE setEndDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemPriority::Priority priority READ priority WRITE setPriority NOTIFY valueChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY valueChanged)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY valueChanged)

public:
    explicit QDeclarativeOrganizerEventOccurrence(QObject *parent = 0);

    void setOriginalDate(const QDate &date);
    QDate originalDate() const;

    void setStartDateTime(const QDateTime &startDateTime);
    QDateTime startDateTime() const;

    void setEndDateTime(const QDateTime &endDateTime);
    QDateTime endDateTime() const;

    void setPriority(QDeclarativeOrganizerItemPriority::Priority priority);
    QDeclarativeOrganizerItemPriority::Priority priority() const;

    void setLocation(const QString &location);
    QString location() const;

    void setParentId(const QString &parentId);
    QString parentId() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerJournal : public QDeclarativeOrganizerItem
{
    Q_OBJECT

    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY valueChanged)

public:
    explicit QDeclarativeOrganizerJournal(QObject *parent = 0);

    void setDateTime(const QDateTime &dateTime);
    QDateTime dateTime() const;

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerNote : public QDeclarativeOrganizerItem
{
    Q_OBJECT

public:
    explicit QDeclarativeOrganizerNote(QObject *parent = 0);

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerTodo : public QDeclarativeOrganizerItem
{
    Q_OBJECT

    Q_PROPERTY(bool isAllDay READ isAllDay WRITE setAllDay NOTIFY valueChanged)// Will be removed, use "allDay" instead!
    Q_PROPERTY(bool allDay READ isAllDay WRITE setAllDay NOTIFY valueChanged)
    Q_PROPERTY(int progressPercentage READ percentageComplete WRITE setPercentageComplete NOTIFY valueChanged)// Will be removed, use "percentageComplete" instead!
    Q_PROPERTY(int percentageComplete READ percentageComplete WRITE setPercentageComplete NOTIFY valueChanged)
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime dueDateTime READ dueDateTime WRITE setDueDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime finishedDateTime READ finishedDateTime WRITE setFinishedDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemPriority::Priority priority READ priority WRITE setPriority NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerTodoProgress::StatusType status READ status WRITE setStatus NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemRecurrence* recurrence READ recurrence NOTIFY valueChanged)

public:
    explicit QDeclarativeOrganizerTodo(QObject *parent = 0);

    void setAllDay(bool isAllDay);
    bool isAllDay() const;

    void setPercentageComplete(int percentageComplete);
    int percentageComplete() const;

    void setStartDateTime(const QDateTime &startDateTime);
    QDateTime startDateTime() const;

    void setDueDateTime(const QDateTime &dueDateTime);
    QDateTime dueDateTime() const;

    void setFinishedDateTime(const QDateTime &finishedDateTime);
    QDateTime finishedDateTime() const;

    void setPriority(QDeclarativeOrganizerItemPriority::Priority priority);
    QDeclarativeOrganizerItemPriority::Priority priority() const;

    void setStatus(QDeclarativeOrganizerTodoProgress::StatusType status);
    QDeclarativeOrganizerTodoProgress::StatusType status() const;

    QDeclarativeOrganizerItemRecurrence *recurrence();

Q_SIGNALS:
    void valueChanged();
};


class QDeclarativeOrganizerTodoOccurrence : public QDeclarativeOrganizerItem
{
    Q_OBJECT

    Q_PROPERTY(int progressPercentage READ percentageComplete WRITE setPercentageComplete NOTIFY valueChanged)// Will be removed, use "percentageComplete" instead!
    Q_PROPERTY(int percentageComplete READ percentageComplete WRITE setPercentageComplete NOTIFY valueChanged)
    Q_PROPERTY(QDate originalDate READ originalDate WRITE setOriginalDate NOTIFY valueChanged)
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime dueDateTime READ dueDateTime WRITE setDueDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDateTime finishedDateTime READ finishedDateTime WRITE setFinishedDateTime NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerItemPriority::Priority priority READ priority WRITE setPriority NOTIFY valueChanged)
    Q_PROPERTY(QDeclarativeOrganizerTodoProgress::StatusType status READ status WRITE setStatus NOTIFY valueChanged)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY valueChanged)

public:
    explicit QDeclarativeOrganizerTodoOccurrence(QObject *parent = 0);

    void setPercentageComplete(int percentageComplete);
    int percentageComplete() const;

    void setOriginalDate(const QDate &date);
    QDate originalDate() const;

    void setStartDateTime(const QDateTime &startDateTime);
    QDateTime startDateTime() const;

    void setDueDateTime(const QDateTime &dueDateTime);
    QDateTime dueDateTime() const;

    void setFinishedDateTime(const QDateTime &finishedDateTime);
    QDateTime finishedDateTime() const;

    void setPriority(QDeclarativeOrganizerItemPriority::Priority priority);
    QDeclarativeOrganizerItemPriority::Priority priority() const;

    void setStatus(QDeclarativeOrganizerTodoProgress::StatusType status);
    QDeclarativeOrganizerTodoProgress::StatusType status() const;

    QString parentId() const;
    void setParentId(const QString &parentId);

Q_SIGNALS:
    void valueChanged();
};

QTORGANIZER_END_NAMESPACE

QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItem))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerEvent))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerEventOccurrence))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerJournal))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerNote))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerTodo))
QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerTodoOccurrence))

#endif // QDECLARATIVEORGANIZERITEM_H
