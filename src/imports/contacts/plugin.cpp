/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlextensionplugin.h>

#include <QtContacts/qcontactid.h>
#include <QtContacts/qcontactabstractrequest.h>

#include "qdeclarativecontact_p.h"
#include "qdeclarativecontactcollection_p.h"
#include "qdeclarativecontactdetail_p.h"
#include "qdeclarativecontactfetchhint_p.h"
#include "qdeclarativecontactfilter_p.h"
#include "qdeclarativecontactfilters_p.h"
#include "qdeclarativecontactmodel_p.h"
#include "qdeclarativecontactrelationship_p.h"
#include "qdeclarativecontactrelationshipmodel_p.h"
#include "qdeclarativecontactsortorder_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QContactQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QStringLiteral("QtContacts"));

        int major = 5;
        int minor = 0;
        qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
        qRegisterMetaType<QContactId>("QContactId");
        qRegisterMetaType<QList<QContactId> >("QList<QContactId>");
        qRegisterMetaType<QList<QContactCollectionId> >("QList<QContactCollectionId>");
        qmlRegisterType<QDeclarativeContactModel>(uri, major, minor, "ContactModel");
        qmlRegisterType<QDeclarativeContact>(uri, major, minor, "Contact");
        qmlRegisterType<QDeclarativeContactCollection>(uri, major, minor, "Collection");
        qmlRegisterType<QDeclarativeContactFetchHint>(uri, major, minor, "FetchHint");
        qmlRegisterType<QDeclarativeContactRelationshipModel>(uri, major, minor, "RelationshipModel");
        qmlRegisterType<QDeclarativeContactRelationship>(uri, major, minor, "Relationship");
        qmlRegisterType<QDeclarativeContactSortOrder>(uri, major, minor, "SortOrder");

        //details
        qmlRegisterType<QDeclarativeContactDetail>(uri, major, minor, "ContactDetail");
        qmlRegisterType<QDeclarativeContactAddress>(uri, major, minor, "Address");
        qmlRegisterType<QDeclarativeContactAnniversary>(uri, major, minor, "Anniversary");
        qmlRegisterType<QDeclarativeContactAvatar>(uri, major, minor, "Avatar");
        qmlRegisterType<QDeclarativeContactBirthday>(uri, major, minor, "Birthday");
        qmlRegisterType<QDeclarativeContactDisplayLabel>(uri, major, minor, "DisplayLabel");
        qmlRegisterType<QDeclarativeContactEmailAddress>(uri, major, minor, "EmailAddress");
        qmlRegisterType<QDeclarativeContactFamily>(uri, major, minor, "Family");
        qmlRegisterType<QDeclarativeContactFavorite>(uri, major, minor, "Favorite");
        qmlRegisterType<QDeclarativeContactGender>(uri, major, minor, "Gender");
        qmlRegisterType<QDeclarativeContactGeoLocation>(uri, major, minor, "Location");
        qmlRegisterType<QDeclarativeContactGlobalPresence>(uri, major, minor, "GlobalPresence");
        qmlRegisterType<QDeclarativeContactGuid>(uri, major, minor, "Guid");
        qmlRegisterType<QDeclarativeContactName>(uri, major, minor, "Name");
        qmlRegisterType<QDeclarativeContactNickname>(uri, major, minor, "Nickname");
        qmlRegisterType<QDeclarativeContactNote>(uri, major, minor, "Note");
        qmlRegisterType<QDeclarativeContactOnlineAccount>(uri, major, minor, "OnlineAccount");
        qmlRegisterType<QDeclarativeContactOrganization>(uri, major, minor, "Organization");
        qmlRegisterType<QDeclarativeContactPhoneNumber>(uri, major, minor, "PhoneNumber");
        qmlRegisterType<QDeclarativeContactPresence>(uri, major, minor, "Presence");
        qmlRegisterType<QDeclarativeContactRingtone>(uri, major, minor, "Ringtone");
        qmlRegisterType<QDeclarativeContactSyncTarget>(uri, major, minor, "SyncTarget");
        qmlRegisterType<QDeclarativeContactTag>(uri, major, minor, "Tag");
        qmlRegisterType<QDeclarativeContactTimestamp>(uri, major, minor, "Timestamp");
        qmlRegisterType<QDeclarativeContactType>(uri, major, minor, "Type");
        qmlRegisterType<QDeclarativeContactUrl>(uri, major, minor, "Url");
        qmlRegisterType<QDeclarativeContactVersion>(uri, major, minor, "Version");
        qmlRegisterType<QDeclarativeContactHobby>(uri, major, minor, "Hobby");
        qmlRegisterType<QDeclarativeContactExtendedDetail>(uri, major, minor, "ExtendedDetail");

        //filters
        qmlRegisterType<QDeclarativeContactFilter>(uri, major, minor, "Filter");
        qmlRegisterType<QDeclarativeContactActionFilter>(uri, major, minor, "ActionFilter");
        qmlRegisterType<QDeclarativeContactChangeLogFilter>(uri, major, minor, "ChangeLogFilter");
        qmlRegisterType<QDeclarativeContactCollectionFilter>(uri, major, minor, "CollectionFilter");
        qmlRegisterType<QDeclarativeContactDetailFilter>(uri, major, minor, "DetailFilter");
        qmlRegisterType<QDeclarativeContactDetailRangeFilter>(uri, major, minor, "DetailRangeFilter");
        qmlRegisterType<QDeclarativeContactIdFilter>(uri, major, minor, "IdFilter");
        qmlRegisterType<QDeclarativeContactRelationshipFilter>(uri, major, minor, "RelationshipFilter");
        qmlRegisterType<QDeclarativeContactIntersectionFilter>(uri, major, minor, "IntersectionFilter");
        qmlRegisterType<QDeclarativeContactUnionFilter>(uri, major, minor, "UnionFilter");
        qmlRegisterType<QDeclarativeContactInvalidFilter>(uri, major, minor, "InvalidFilter");
        qmlRegisterType<QDeclarativeContactCompoundFilter>();

    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(engine)
        Q_UNUSED(uri)
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
