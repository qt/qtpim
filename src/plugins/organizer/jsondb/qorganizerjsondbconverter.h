/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
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

#include "qorganizerjsondbengine.h"

#include <QtOrganizer/qorganizerabstractrequest.h>
#include <QtOrganizer/qorganizermanager.h>
#include <QtOrganizer/qorganizeritemdetails.h>
#include <QtOrganizer/qorganizeritemdetailfilter.h>
#include <QtOrganizer/qorganizeritemdetailfieldfilter.h>

#include <QtJsonDb/qjsondbconnection.h>
#include <QtJsonDb/qjsondbreadrequest.h>
#include <QtJsonDb/qjsondbwriterequest.h>

QT_USE_NAMESPACE_JSONDB

QT_BEGIN_NAMESPACE_ORGANIZER

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

    bool jsonDbObjectToItem(const QJsonObject &object, QOrganizerItem *item, QOrganizerJsonDbEngine::StorageLocation storageLocation) const;
    bool itemToJsonDbObject(const QOrganizerItem &item, QJsonObject *object) const;

    bool jsonDbObjectToCollection(const QJsonObject &object, QOrganizerCollection *collection, bool *isDefaultCollection, QOrganizerJsonDbEngine::StorageLocation storageLocation) const;
    bool collectionToJsonDbObject(const QOrganizerCollection &collection, bool isDefaultCollection, QJsonObject *object) const;

    // filter handling
    bool singleFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr, int *typeFilterFlag) const;
    bool compoundFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr, int *typeFilterFlag) const;

    // notification handling
    QString jsonDbNotificationObjectToOrganizerType(const QJsonObject &object) const;
    QOrganizerItemId jsonDbNotificationObjectToItemId(const QJsonObject &object, QOrganizerJsonDbEngine::StorageLocation storageLocation) const;
    QOrganizerCollectionId jsonDbNotificationObjectToCollectionId(const QJsonObject &object, QOrganizerJsonDbEngine::StorageLocation storageLocation) const;

    void jsonDbVersionToItemVersion(const QString &jsonDbVersion, QOrganizerItemVersion *itemVersion) const;

    // storage location handling
    const QStringList storageLocationsFlagToStrings(const QOrganizerJsonDbEngine::StorageLocations storageLocationsFlag);
    QOrganizerJsonDbEngine::StorageLocation storageLocationStringToEnum(const QString &storageLocation);
    QOrganizerJsonDbEngine::StorageLocations storageLocationListToFlag(const QList<QOrganizerJsonDbEngine::StorageLocation> storageLocationsList);

    int supportedItemTypeFlag() const;
    bool itemTypeFlagToJsonDbEventQuery(const int &flag, QString *jsonDbQueryStr);
    bool itemTypeFlagToJsonDbTodoQuery(const int &flag, QString *jsonDbQueryStr);

    bool createJsonDbQuery(const QOrganizerItemFilter &filter, const QDateTime &startTime, const QDateTime &endTime, QList<QString> *jsonDbQueryList);

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
    bool detailFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr, int *typeFilterFlag) const;
    bool detailFieldFilterToJsondbQuery(const QOrganizerItemFilter &filter, QString *jsonDbQueryStr, int *typeFilterFlag) const;
    bool isSupportedDetailFieldFilter(const QVariant &fieldValue,
                                      QOrganizerItemDetail::DetailType detailType,
                                      int detailFieldName,
                                      QOrganizerItemFilter::MatchFlags matchFlags) const;
    QString createMatchFlagQuery(const QString &value, QOrganizerItemFilter::MatchFlags flags) const;
    void addFieldToFilterQuery(QOrganizerItemDetail::DetailType detailType,
                               int detailField,
                               const QVariant &fieldValue,
                               QString *jsonDbQueryStr,
                               QOrganizerItemFilter::MatchFlags matchFlags,
                               int *typeFilterFlag) const;

    static const int enumMapEnd;
    static const QMap<int, QString> filterablePropertyNames();
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

QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERJSONDBCONVERTER_H
