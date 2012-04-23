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

#ifndef QORGANIZERJSONDBCONVERTER_H
#define QORGANIZERJSONDBCONVERTER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtOrganizer/qorganizerabstractrequest.h>
#include <QtOrganizer/qorganizermanager.h>
#include <QtOrganizer/qorganizeritemdetails.h>
#include <QtOrganizer/qorganizeritemdetailfilter.h>

#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbreadrequest.h>
#include <QtJsonDb/qjsondbwriterequest.h>

QT_USE_NAMESPACE_JSONDB

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

    QOrganizerManager::Error jsonDbConnectionErrorToOrganizerError(QJsonDbConnection::ErrorCode error) const;
    QOrganizerManager::Error jsonDbRequestErrorToOrganizerError(QJsonDbRequest::ErrorCode error) const;

    bool jsonDbObjectToItem(const QJsonObject &object, QOrganizerItem *item, QOrganizerAbstractRequest::StorageLocation storageLocation) const;
    bool itemToJsonDbObject(const QOrganizerItem &item, QJsonObject *object) const;

    bool jsonDbObjectToCollection(const QJsonObject &object, QOrganizerCollection *collection, bool *isDefaultCollection, QOrganizerAbstractRequest::StorageLocation storageLocation) const;
    bool collectionToJsonDbObject(const QOrganizerCollection &collection, bool isDefaultCollection, QJsonObject *object) const;

    // filter handling
    bool singleFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const;
    bool compoundFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const;

    // notification handling
    QString jsonDbNotificationObjectToOrganizerType(const QJsonObject &object) const;
    QOrganizerItemId jsonDbNotificationObjectToItemId(const QJsonObject &object, QOrganizerAbstractRequest::StorageLocation storageLocation) const;
    QOrganizerCollectionId jsonDbNotificationObjectToCollectionId(const QJsonObject &object, QOrganizerAbstractRequest::StorageLocation storageLocation) const;

    void jsonDbVersionToItemVersion(const QString &jsonDbVersion, QOrganizerItemVersion *itemVersion) const;

    // storage location handling
    const QStringList storageLocationsFlagToStrings(const QOrganizerAbstractRequest::StorageLocations storageLocationsFlag);
    QOrganizerAbstractRequest::StorageLocation storageLocationStringToEnum(const QString &storageLocation);
    QOrganizerAbstractRequest::StorageLocations storageLocationListToFlag(const QList<QOrganizerAbstractRequest::StorageLocation> storageLocationsList);

private:
    void itemVersionToJsonDbVersion(const QOrganizerItemVersion &itemVersion, QString *jsonDbVersion) const;

    void jsonDbObjectToRecurrenceRule(const QJsonObject &object, QOrganizerRecurrenceRule *rule) const;
    void recurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule &rule, QJsonObject *object) const;

    void audibleReminderDetailToJsonDbObject(const QOrganizerItemAudibleReminder &itemReminder, QJsonObject *object) const;
    void jsonDbObjectToAudibleReminderDetail(const QJsonObject &object, QOrganizerItemAudibleReminder *itemReminder,
                                             QOrganizerItemExtendedDetail *extendedDetail) const;

    int stringToEnum(const QOrganizerJsonDbEnumConversionData *const conversionData, const QString &enumStr) const;
    QString enumToString(const QOrganizerJsonDbEnumConversionData *const conversionData, int enumValue) const;

    void attendeeDetailToJsonDbObject(const QOrganizerEventAttendee &attendeeDetail, QJsonObject *object) const;
    void jsonDbObjectToAttendeeDetail(const QJsonObject &object, QOrganizerEventAttendee *attendeeDetail) const;

    void rsvpDetailToJsonDbObject(const QOrganizerEventRsvp &rsvpDetail, QJsonObject *object) const;
    void jsonDbObjectToRsvpDetail(const QJsonObject &object, QOrganizerEventRsvp *rsvpDetail,
                                  QOrganizerItemExtendedDetail *extendedDetail) const;

    void locationDetailToJsonDbObject(const QOrganizerItemLocation &locationDetail, QJsonObject *object) const;
    void jsonDbObjectToLocationDetail(const QJsonObject &object, QOrganizerItemLocation *locationDetail,
                                      QOrganizerItemExtendedDetail *extendedDetail) const;

    // separate filter type specific handling
    bool collectionFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const;
    bool idFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const;
    bool detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr) const;
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
    static const QOrganizerJsonDbEnumConversionData *organizerTodoStatusMap();
    static const QOrganizerJsonDbEnumConversionData *organizerStorageLocationMap();

};

QTORGANIZER_END_NAMESPACE

#endif // QORGANIZERJSONDBCONVERTER_H
