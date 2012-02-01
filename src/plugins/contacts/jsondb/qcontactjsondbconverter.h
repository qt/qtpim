/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QCONTACTJSONDBCONVERTER_H
#define QCONTACTJSONDBCONVERTER_H

#include <QVariantMap>
#include <QHash>
#include "qcontact.h"
#include "qcontactjsondbengine.h"
#include "qcontactabstractrequest.h"
#include "qcontactfilter.h"

QTCONTACTS_BEGIN_NAMESPACE

class QContactJsonDbConverter
{
public:
    QContactJsonDbConverter();
    ~QContactJsonDbConverter();
    bool toQContact(const QVariantMap& object, QContact* contact, const QContactJsonDbEngine& engine);
    bool toQContacts(const QVariantList& jsonObjects, QList<QContact>& convertedContacts, const QContactJsonDbEngine& engine, QContactManager::Error& error);
    bool toJsonContact(QVariantMap* object, const QContact& contact);
    bool updateContexts(const QVariantMap& object, QContactDetail* detail);
    bool updateContexts(const QContactDetail& detail, QVariantMap* object);
    QString queryFromRequest(QContactAbstractRequest* request);
    QString convertFilter(const QContactFilter& filter) const;
    QString convertSortOrder(const QList<QContactSortOrder>& sortOrders) const;
    QString jsonDbNotificationObjectToContactType(const QVariantMap &object) const;
    QContactId jsonDbNotificationObjectToContactId(const QVariantMap &object) const;
    QString convertId(const QContactId &id) const;
    void jsonDbVersionToContactVersion(const QString &jsonDbVersion, QContactVersion *contactVersion) const;
    void contactVersionToJsonDbVersion(const QContactVersion &contactVersion, QString *jsonDbVersion) const;

    QHash<QString, QString> detailsToJsonMapping;
    QHash<QString, QString> contactNameFieldsMapping;
    QHash<QString, QString> organizationFieldsMapping;
    QHash<QString, QString> addressFieldsMapping;
    QHash<QString, QString> phoneNumbersSubtypesMapping;
private:
    void initializeMappings();
    void createMatchFlagQuery(QString& queryString, QContactFilter::MatchFlags flags, const QString& value, const QString& UriScheme = "") const;
    QString toJsonDate(const QDateTime& date) const;
    QDateTime toContactDate(const QString& dateString) const;
    void extendedDetailToJsonDbProperty(const QContactExtendedDetail &extendedDetail, QVariant& property) const;
    void dataToList(const QVariant &data, QVariantList &list) const;
    void dataToMap(const QVariant &data, QVariantMap &map) const;

    static const int jsonDbVersionLength;
};

QTCONTACTS_END_NAMESPACE

#endif // QCONTACTJSONDBCONVERTER_H
