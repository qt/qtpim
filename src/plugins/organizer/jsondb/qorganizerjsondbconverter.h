/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtPIM module of the Qt Toolkit.
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

#ifndef QORGANIZERJSONDBCONVERTER_H
#define QORGANIZERJSONDBCONVERTER_H

#include <QVariant>

#include "qorganizer.h"

#include <jsondb-error.h>

Q_USE_JSONDB_NAMESPACE

QTORGANIZER_BEGIN_NAMESPACE

struct QOrganizerJsonDbEnumConversionData
{
    int enumValue;
    const QString enumStr;
};

class QOrganizerJsonDbConverter
{
public:
    QOrganizerJsonDbConverter();

    QOrganizerManager::Error jsondbErrorToOrganizerError(JsonDbError::ErrorCode jsonErrorCode);

    bool jsonDbObjectToItem(const QVariantMap &object, QOrganizerItem *item) const;
    bool itemToJsonDbObject(const QOrganizerItem &item, QVariantMap *object) const;

    bool jsonDbObjectToCollection(const QVariantMap &object, QOrganizerCollection *collection, bool &isDefaultCollection);
    bool collectionToJsonDbObject(const QOrganizerCollection &collection, bool isDefaultCollection, QVariantMap *object) const;

    // filter handling
    bool singleFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const;
    bool compoundFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const;

    // notification handling
    QString jsonDbNotificationObjectToOrganizerType(const QVariantMap &object) const;
    QOrganizerItemId jsonDbNotificationObjectToItemId(const QVariantMap &object) const;
    QOrganizerCollectionId jsonDbNotificationObjectToCollectionId(const QVariantMap &object) const;

    bool itemToJsondbAlarmObject(const QOrganizerItem &item, QVariantMap &alarmObject) const;

    void jsonDbVersionToItemVersion(const QString &jsonDbVersion, QOrganizerItemVersion *itemVersion) const;

private:
    void itemVersionToJsonDbVersion(const QOrganizerItemVersion &itemVersion, QString *jsonDbVersion) const;

    void jsonDbObjectToRecurrenceRule(const QVariantMap &object, QOrganizerRecurrenceRule *rule) const;
    void recurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule &rule, QVariantMap *object) const;

    void audibleReminderDetailToJsonDbObject(const QOrganizerItemAudibleReminder &itemReminder, QVariantMap &object) const;
    void jsonDbObjectToAudibleReminderDetail(const QVariantMap &object, QOrganizerItemAudibleReminder *itemReminder) const;

    int stringToEnum(const QOrganizerJsonDbEnumConversionData *const conversionData, const QString &enumStr) const;
    QString enumToString(const QOrganizerJsonDbEnumConversionData *const conversionData, int enumValue) const;

    void extendedDetailToJsonDbProperty(const QOrganizerItemExtendedDetail &extendedDetail, QVariant& property) const;

    void attendeeDetailToJsonDbObject(const QOrganizerEventAttendee &attendeeDetail, QVariantMap *object) const;
    void jsonDbObjectToAttendeeDetail(const QVariantMap &object, QOrganizerEventAttendee *attendeeDetail) const;

    void rsvpDetailToJsonDbObject(const QOrganizerEventRsvp &rsvpDetail, QVariantMap *object) const;
    void jsonDbObjectToRsvpDetail(const QVariantMap &object, QOrganizerEventRsvp *rsvpDetail) const;

    void locationDetailToJsonDbObject(const QOrganizerItemLocation &locationDetail, QVariantMap *object) const;
    void jsonDbObjectToLocationDetail(const QVariantMap &object, QOrganizerItemLocation *locationDetail) const;

    void dataToList(const QVariant &data, QVariantList &list) const;
    void dataToMap(const QVariant &data, QVariantMap &map) const;

    // separate filter type specific handling
    bool collectionFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const;
    bool idFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const;
    bool detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString &jsonDbQueryStr) const;
    bool isSupportedDetailFilter(
        const QOrganizerItemDetailFilter &filter, QOrganizerItemDetail::DetailType detailType, int detailFieldName) const;
    QString createMatchFlagQuery(const QString &value, QOrganizerItemFilter::MatchFlags flags) const;

    static const int enumMapEnd;
    static const QOrganizerJsonDbEnumConversionData *organizerPriorityEnumMap();
    static const QOrganizerJsonDbEnumConversionData *organizerFrequencyEnumMap();
    static const QOrganizerJsonDbEnumConversionData *organizerDayEnumMap();
    static const QOrganizerJsonDbEnumConversionData *organizerMonthEnumMap();
    static const QOrganizerJsonDbEnumConversionData *organizerParticipationStatusMap();
    static const QOrganizerJsonDbEnumConversionData *organizerParticipationRoleMap();
    static const QOrganizerJsonDbEnumConversionData *organizerResponseRequirementMap();
    static const QOrganizerJsonDbEnumConversionData *organizerItemTypeMap();
};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBCONVERTER_H
