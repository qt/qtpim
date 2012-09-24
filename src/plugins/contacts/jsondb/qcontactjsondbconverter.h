/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QCONTACTJSONDBCONVERTER_H
#define QCONTACTJSONDBCONVERTER_H

#include <QHash>
#include <QtJsonDb/qjsondbrequest.h>

#include "qcontact.h"
#include "qcontactjsondbengine.h"
#include "qcontactabstractrequest.h"
#include "qcontactfilter.h"

QT_USE_NAMESPACE_JSONDB

QTCONTACTS_BEGIN_NAMESPACE

class QContactJsonDbConverter
{
public:
    enum SanitizeError {
        NoError = 0x0000,
        EmptyArgumentError = 0x0001,
        InvalidArgumentError = 0x0002
    };
    QContactJsonDbConverter();
    ~QContactJsonDbConverter();
    QContactManager::Error jsonDbRequestErrorToContactError(QJsonDbRequest::ErrorCode error) const;
    bool toQContact(const QJsonObject& object, QContact* contact, const QString &partitionName);
    bool toQContacts(const QList<QJsonObject> &jsonObjects, QList<QContact>& convertedContacts, QContactManager::Error& error, const QString &partitionName);
    bool toJsonContact(QJsonObject* object, const QContact& contact, const QList<QContactDetail::DetailType> &detailMask = QList<QContactDetail::DetailType>());
    bool updateContexts(const QJsonObject& object, QContactDetail* detail);
    bool updateContexts(const QContactDetail& detail, QJsonObject* object);
    bool queryFromRequest(QContactAbstractRequest* request,QString &jsonDbQueryStr);
    bool singleFilterToJsondbQuery(const QContactFilter& filter,QString &jsonDbQueryStr) const;
    bool compoundFilterToJsondbQuery(const QContactFilter &filter, QString &jsonDbQueryStr) const;
    QString convertSortOrder(const QList<QContactSortOrder>& sortOrders) const;
    QContactId jsonDbNotificationObjectToContactId(const QJsonObject &object, QContactAbstractRequest::StorageLocation storageLocation) const;
    QString contactIdToUuid(const QContactId &id) const;
    QContactId uuidtoContactId(QString &uuid, const QString &partitionName) const;
    void jsonDbVersionToContactVersion(const QString &jsonDbVersion, QContactVersion *contactVersion) const;
    void contactVersionToJsonDbVersion(const QContactVersion &contactVersion, QString *jsonDbVersion) const;
    bool sanitizePhoneNumberString(QString *phoneNumberString) const;
    const QStringList storageLocationsToPartitionNames(QContactAbstractRequest::StorageLocations storageLocations);
    QContactAbstractRequest::StorageLocation partitionNameToStorageLocation(const QString partitionName);
    SanitizeError sanitizeContactDetailString(QString *stringToBeSanitized, int maxStringLength = 50) const;

private:
    void initializeMappings();
    void createMatchFlagQuery(QString& queryString, QContactFilter::MatchFlags flags, const QString& value) const;
    QString toJsonDbDate(const QDateTime& dateAsQDateTime) const;
    QDateTime toQDateTime(const QString& jsonDbDate) const;
    bool idFilterToJsondbQuery(const QContactFilter &filter, QString &jsonDbQueryStr) const;
    bool detailFilterToJsondbQuery(const QContactFilter &filter, QString &jsonDbQueryStr) const;
    void idsToJsondbQuery(const QList<QContactId> &ids, QString &newJsonDbQuery) const;

    static bool isValidPhoneNumberCharacter (const QChar *character);

    static const int jsonDbVersionLength;

    QHash<QContactDetail::DetailType, QString> detailsToJsonMapping;
    QHash<int, QString> contactNameFieldsMapping;
    QHash<QContactOrganization::OrganizationField, QString> organizationFieldsMapping;
    QHash<QContactAddress::AddressField, QString> addressFieldsMapping;
    QHash<int, QString> phoneNumbersSubtypesMapping;
    QHash<int, QString> genderValuesMapping;
    QHash<QContactDetail::DetailContext, QString> contextsToJsonMapping;
    QHash<QContactAbstractRequest::StorageLocation, QString> storageLocationMapping;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBCONVERTER_H
