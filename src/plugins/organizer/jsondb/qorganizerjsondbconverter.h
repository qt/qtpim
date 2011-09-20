/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
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

QTPIM_USE_NAMESPACE
Q_USE_JSONDB_NAMESPACE


struct QOrganizerJsonDbEnumConversionData
{
    int enumValue;
    const QLatin1String enumStr;
};


class QOrganizerJsonDbConverter
{
public:
    QOrganizerJsonDbConverter();

    QOrganizerManager::Error convertJsondbErrorToOrganizerError(JsonDbError::ErrorCode jsonErrorCode);

    bool convertJsonDbObjectToItem(const QVariantMap& object, QOrganizerItem* item) const;
    bool convertItemToJsonDbObject(const QOrganizerItem& item, QVariantMap* object) const;
    bool convertJsonDbObjectToCollection(const QVariantMap& object, QOrganizerCollection* collection, bool& isDefaultCollection);
    bool convertCollectionToJsonDbObject(const QOrganizerCollection& collection, bool isDefaultCollection, QVariantMap* object) const;

private:
    void convertJsonDbObjectToRecurrenceRule(const QVariantMap& object, QOrganizerRecurrenceRule* rule) const;
    void convertRecurrenceRuleToJsonDbObject(const QOrganizerRecurrenceRule& rule, QVariantMap* object) const;

    void convertItemReminderDetailToJsonDbObject(const QOrganizerItemReminder& itemReminder, QVariantMap& reminderObject) const;
    void convertJsonDbObjectToItemReminderDetailCommon(const QVariantMap& object, QOrganizerItemReminder* itemReminder) const;

    int convertStringToEnum(const QOrganizerJsonDbEnumConversionData* const conversionData, QString enumStr) const;
    QString convertEnumToString(const QOrganizerJsonDbEnumConversionData* const conversionData, int enumValue) const;
};

#endif // QORGANIZERJSONDBCONVERTER_H
