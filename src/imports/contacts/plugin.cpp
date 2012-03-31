/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
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


#include <QtQml>
#include <QQmlExtensionPlugin>

#include "qdeclarativecontactmodel_p.h"
#include "qdeclarativecontact_p.h"
#include "qdeclarativecontactdetail_p.h"
#include "qdeclarativecontactfilter_p.h"
#include "qdeclarativecontactsortorder_p.h"
#include "qdeclarativecontactrelationshipmodel_p.h"
#include "qdeclarativecontactrelationship_p.h"
#include "qdeclarativecontactfetchhint_p.h"
#include "qdeclarativecontactfilters_p.h"

QTCONTACTS_BEGIN_NAMESPACE

class QContactQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface" FILE "contacts.json")
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("QtContacts"));

        int major = 5;
        int minor = 0;
        qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
        qRegisterMetaType<QContactId>("QContactId");
        qRegisterMetaType<QList<QContactId> >("QList<QContactId>");
        qmlRegisterType<QDeclarativeContactModel>(uri, major, minor, "ContactModel");
        qmlRegisterType<QDeclarativeContact>(uri, major, minor, "Contact");
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
        qmlRegisterType<QDeclarativeContactPersonId>(uri, major, minor, "Personid");
        qmlRegisterType<QDeclarativeContactExtendedDetail>(uri, major, minor, "ExtendedDetail");

        //filters
        qmlRegisterType<QDeclarativeContactFilter>(uri, major, minor, "Filter");
        qmlRegisterType<QDeclarativeContactActionFilter>(uri, major, minor, "ActionFilter");
        qmlRegisterType<QDeclarativeContactChangeLogFilter>(uri, major, minor, "ChangeLogFilter");
        qmlRegisterType<QDeclarativeContactDetailFilter>(uri, major, minor, "DetailFilter");
        qmlRegisterType<QDeclarativeContactDetailRangeFilter>(uri, major, minor, "DetailRangeFilter");
        qmlRegisterType<QDeclarativeContactIdFilter>(uri, major, minor, "IdFilter");
        qmlRegisterType<QDeclarativeContactRelationshipFilter>(uri, major, minor, "RelationshipFilter");
        qmlRegisterType<QDeclarativeContactIntersectionFilter>(uri, major, minor, "IntersectionFilter");
        qmlRegisterType<QDeclarativeContactUnionFilter>(uri, major, minor, "UnionFilter");
        qmlRegisterType<QDeclarativeContactCompoundFilter>();

    }

    void initializeEngine(QQmlEngine *engine, const char *uri)
    {
        Q_UNUSED(engine)
        Q_UNUSED(uri)
    }
};

#include "plugin.moc"

QTCONTACTS_END_NAMESPACE
