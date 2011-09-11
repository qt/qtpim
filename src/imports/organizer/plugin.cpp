/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtDeclarative>
#include <QDeclarativeExtensionPlugin>

#include "qdeclarativeorganizermodel_p.h"
#include "qdeclarativeorganizeritem_p.h"
#include "qdeclarativeorganizeritemdetail_p.h"
#include "qdeclarativeorganizeritemfilter_p.h"
#include "qdeclarativeorganizerrecurrencerule_p.h"
#include "qdeclarativeorganizeritemfetchhint_p.h"
#include "qdeclarativeorganizeritemsortorder_p.h"
#include "qdeclarativeorganizercollection_p.h"

QT_USE_NAMESPACE

class QOrganizerQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("QtAddOn.organizer"));

        int major = 2;
        int minor = 0;
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

        qmlRegisterType<QDeclarativeOrganizerItemReminder>(uri, major, minor, "Reminder");
        qmlRegisterType<QDeclarativeOrganizerItemAudibleReminder>(uri, major, minor, "AudibleReminder");
        qmlRegisterType<QDeclarativeOrganizerItemEmailReminder>(uri, major, minor, "EmailReminder");
        qmlRegisterType<QDeclarativeOrganizerItemVisualReminder>(uri, major, minor, "VisualReminder");

        //filters
        qmlRegisterType<QDeclarativeOrganizerItemFilter>(uri, major, minor, "Filter");
        qmlRegisterType<QDeclarativeOrganizerItemChangeLogFilter>(uri, major, minor, "ChangeLogFilter");
        qmlRegisterType<QDeclarativeOrganizerItemCollectionFilter>(uri, major, minor, "CollectionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailFilter>(uri, major, minor, "DetailFilter");
        qmlRegisterType<QDeclarativeOrganizerItemDetailRangeFilter>(uri, major, minor, "DetailRangeFilter");
        qmlRegisterType<QDeclarativeOrganizerItemIdFilter>(uri, major, minor, "IdFilter");
        qmlRegisterType<QDeclarativeOrganizerItemIntersectionFilter>(uri, major, minor, "IntersectionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemUnionFilter>(uri, major, minor, "UnionFilter");
        qmlRegisterType<QDeclarativeOrganizerItemInvalidFilter>(uri, major, minor, "InvalidFilter");
        qmlRegisterType<QDeclarativeOrganizerItemCompoundFilter>();
    }
};

#include "plugin.moc"

Q_EXPORT_PLUGIN2(qorganizerqmlplugin, QOrganizerQmlPlugin)
