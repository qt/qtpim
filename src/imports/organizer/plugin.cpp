/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlextensionplugin.h>

#include "qdeclarativeorganizercollection_p.h"
#include "qdeclarativeorganizeritem_p.h"
#include "qdeclarativeorganizeritemdetail_p.h"
#include "qdeclarativeorganizeritemfetchhint_p.h"
#include "qdeclarativeorganizeritemfilter_p.h"
#include "qdeclarativeorganizeritemsortorder_p.h"
#include "qdeclarativeorganizermodel_p.h"
#include "qdeclarativeorganizerrecurrencerule_p.h"

QT_BEGIN_NAMESPACE

class QOrganizerQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface" FILE "organizer.json")

public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QStringLiteral("QtOrganizer"));

        int major = 5;
        int minor = 0;

        qRegisterMetaType<QOrganizerAbstractRequest::State>("QOrganizerAbstractRequest::State");
        qRegisterMetaType<QOrganizerItemId>("QOrganizerItemId");
        qRegisterMetaType<QList<QOrganizerItemId> >("QList<QOrganizerItemId>");
        qRegisterMetaType<QOrganizerCollectionId>("QOrganizerCollectionId");
        qRegisterMetaType<QList<QOrganizerCollectionId> >("QList<QOrganizerCollectionId>");

        qmlRegisterType<QDeclarativeOrganizerModel>(uri, major, minor, "OrganizerModel");
        qmlRegisterType<QDeclarativeOrganizerRecurrenceRule>(uri, major, minor, "RecurrenceRule");
        qmlRegisterType<QDeclarativeOrganizerItemFetchHint>(uri, major, minor, "FetchHint");
        qmlRegisterType<QDeclarativeOrganizerItemSortOrder>(uri, major, minor, "SortOrder");
        qmlRegisterType<QDeclarativeOrganizerCollection>(uri, major, minor, "Collection");

        //items
        qmlRegisterType<QDeclarativeOrganizerItem>(uri, major, minor, "OrganizerItem");
        qmlRegisterType<QDeclarativeOrganizerEvent>(uri, major, minor, "Event");
        qmlRegisterType<QDeclarativeOrganizerEventOccurrence>(uri, major, minor, "EventOccurrence");
        qmlRegisterType<QDeclarativeOrganizerJournal>(uri, major, minor, "Journal");
        qmlRegisterType<QDeclarativeOrganizerNote>(uri, major, minor, "Note");
        qmlRegisterType<QDeclarativeOrganizerTodo>(uri, major, minor, "Todo");
        qmlRegisterType<QDeclarativeOrganizerTodoOccurrence>(uri, major, minor, "TodoOccurrence");

        //details
        qmlRegisterType<QDeclarativeOrganizerItemDetail>(uri, major, minor, "Detail");
        qmlRegisterType<QDeclarativeOrganizerEventTime>(uri, major, minor, "EventTime");
        qmlRegisterType<QDeclarativeOrganizerItemComment>(uri, major, minor, "Comment");
        qmlRegisterType<QDeclarativeOrganizerItemDescription>(uri, major, minor, "Description");
        qmlRegisterType<QDeclarativeOrganizerItemDisplayLabel>(uri, major, minor, "DisplayLabel");
        qmlRegisterType<QDeclarativeOrganizerItemGuid>(uri, major, minor, "Guid");
        qmlRegisterType<QDeclarativeOrganizerItemLocation>(uri, major, minor, "Location");
        qmlRegisterType<QDeclarativeOrganizerItemParent>(uri, major, minor, "Parent");
        qmlRegisterType<QDeclarativeOrganizerItemPriority>(uri, major, minor, "Priority");
        qmlRegisterType<QDeclarativeOrganizerItemRecurrence>(uri, major, minor, "Recurrence");
        qmlRegisterType<QDeclarativeOrganizerItemTag>(uri, major, minor, "Tag");
        qmlRegisterType<QDeclarativeOrganizerItemTimestamp>(uri, major, minor, "Timestamp");
        qmlRegisterType<QDeclarativeOrganizerItemType>(uri, major, minor, "Type");
        qmlRegisterType<QDeclarativeOrganizerJournalTime>(uri, major, minor, "JournalTime");
        qmlRegisterType<QDeclarativeOrganizerTodoProgress>(uri, major, minor, "TodoProgress");
        qmlRegisterType<QDeclarativeOrganizerTodoTime>(uri, major, minor, "TodoTime");
        qmlRegisterType<QDeclarativeOrganizerItemExtendedDetail>(uri, major, minor, "ExtendedDetail");
        qmlRegisterType<QDeclarativeOrganizerEventAttendee>(uri, major, minor, "EventAttendee");
        qmlRegisterType<QDeclarativeOrganizerEventRsvp>(uri, major, minor, "EventRsvp");
        qmlRegisterType<QDeclarativeOrganizerItemClassification>(uri, major, minor, "Classification");
        qmlRegisterType<QDeclarativeOrganizerItemVersion>(uri, major, minor, "Version");

        qmlRegisterType<QDeclarativeOrganizerItemReminder>(uri, major, minor, "Reminder");
        qmlRegisterType<QDeclarativeOrganizerItemAudibleReminder>(uri, major, minor, "AudibleReminder");
        qmlRegisterType<QDeclarativeOrganizerItemEmailReminder>(uri, major, minor, "EmailReminder");
        qmlRegisterType<QDeclarativeOrganizerItemVisualReminder>(uri, major, minor, "VisualReminder");

        //filters
        qmlRegisterType<QDeclarativeOrganizerItemFilter>(uri, major, minor, "Filter");
        qmlRegisterType<QDeclarativeOrganizerItemCollectionFilter>(uri, major, minor, "CollectionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailFilter>(uri, major, minor, "DetailFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailFieldFilter>(uri, major, minor, "DetailFieldFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailRangeFilter>(uri, major, minor, "DetailRangeFilter");
        qmlRegisterType<QDeclarativeOrganizerItemIdFilter>(uri, major, minor, "IdFilter");
        qmlRegisterType<QDeclarativeOrganizerItemIntersectionFilter>(uri, major, minor, "IntersectionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemUnionFilter>(uri, major, minor, "UnionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemInvalidFilter>(uri, major, minor, "InvalidFilter");
        qmlRegisterType<QDeclarativeOrganizerItemCompoundFilter>();
    }
};

#include "plugin.moc"

QT_END_NAMESPACE
