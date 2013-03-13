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

#include "qorganizerjsondbengine.h"
#include "qorganizerjsondbrequestthread.h"
#include "qorganizerjsondbstring.h"

#include <QtOrganizer/qorganizeritemdetails.h>

#include <QtCore/qeventloop.h>
#include <QtNetwork/qabstractsocket.h>

QT_BEGIN_NAMESPACE_ORGANIZER

QOrganizerJsonDbEngine::QOrganizerJsonDbEngine(QOrganizerManager::Error *error)
    : m_requestHandlerThread(new QOrganizerJsonDbRequestThread)
{
    // Register types to be able to send them as parameters in cross-thread signals
    qRegisterMetaType<QOrganizerAbstractRequest::State>("QOrganizerAbstractRequest::State");
    qRegisterMetaType<QList<QOrganizerItemId> >("QList<QOrganizerItemId>");
    qRegisterMetaType<QList<QOrganizerCollectionId> >("QList<QOrganizerCollectionId>");
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
    qRegisterMetaType<QList<QPair<QOrganizerItemId, QOrganizerManager::Operation> > >("QList<QPair<QOrganizerItemId,QOrganizerManager::Operation> >");
    qRegisterMetaType<QList<QPair<QOrganizerCollectionId, QOrganizerManager::Operation> > >("QList<QPair<QOrganizerCollectionId,QOrganizerManager::Operation> >");
    qRegisterMetaType<QOrganizerItemId>("QOrganizerItemId");
    qRegisterMetaType<QOrganizerCollectionId>("QOrganizerCollectionId");

    m_requestHandlerThread->moveToThread(m_requestHandlerThread);
    // making sure that thread is started before exiting this function
    QEventLoop loop;
    connect(m_requestHandlerThread, SIGNAL(initialized()), &loop, SLOT(quit()));
    m_requestHandlerThread->start();
    loop.exec();
    connect(this, SIGNAL(requestReceived(QOrganizerAbstractRequest*)),
            m_requestHandlerThread, SLOT(handleRequest(QOrganizerAbstractRequest*)));
    m_requestHandlerThread->setEngine(this);

    *error = QOrganizerManager::NoError;
}

QOrganizerJsonDbEngine::~QOrganizerJsonDbEngine()
{
    if (m_requestHandlerThread) {
        m_requestHandlerThread->exit();
        m_requestHandlerThread->wait();
        delete m_requestHandlerThread;
    }
}

QString QOrganizerJsonDbEngine::managerName() const
{
    return QOrganizerJsonDbStr::jsonDbManagerName();
}

QMap<QString, QString> QOrganizerJsonDbEngine::managerParameters() const
{
    /* TODO - in case you have any actual parameters that are relevant that you saved in the factory method, return them here */
    return QMap<QString, QString>();
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::itemOccurrences(const QOrganizerItem &parentItem,
                                                              const QDateTime &startDateTime,
                                                              const QDateTime &endDateTime, int maxCount,
                                                              const QOrganizerItemFetchHint &fetchHint,
                                                              QOrganizerManager::Error *error)
{
    QOrganizerItemOccurrenceFetchRequest occurrenceFetchReq;
    occurrenceFetchReq.setParentItem(parentItem);
    occurrenceFetchReq.setStartDate(startDateTime);
    occurrenceFetchReq.setEndDate(endDateTime);
    occurrenceFetchReq.setMaxOccurrences(maxCount);
    occurrenceFetchReq.setFetchHint(fetchHint);
    *error = QOrganizerManager::NoError;

    startRequest(&occurrenceFetchReq);
    if (waitForRequestFinished(&occurrenceFetchReq, 0)) {
        *error = occurrenceFetchReq.error();
        return occurrenceFetchReq.itemOccurrences();
    }
    return QList<QOrganizerItem>();
}

QList<QOrganizerItemId> QOrganizerJsonDbEngine::itemIds(const QOrganizerItemFilter &filter,
                                                        const QDateTime &startDateTime, const QDateTime &endDateTime,
                                                        const QList<QOrganizerItemSortOrder> &sortOrders,
                                                        QOrganizerManager::Error *error)
{
    QOrganizerItemIdFetchRequest idFetchReq;
    idFetchReq.setStartDate(startDateTime);
    idFetchReq.setEndDate(endDateTime);
    idFetchReq.setFilter(filter);
    idFetchReq.setSorting(sortOrders);
    if (startRequest(&idFetchReq)) {
        if (waitForRequestFinished(&idFetchReq, 0))
            *error = idFetchReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return idFetchReq.itemIds();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItemId>();
    }
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::items(const QOrganizerItemFilter &filter, const QDateTime &startDateTime,
                                                          const QDateTime &endDateTime, int maxCount,
                                                          const QList<QOrganizerItemSortOrder> &sortOrders,
                                                          const QOrganizerItemFetchHint &fetchHint, QOrganizerManager::Error *error)
{
    QOrganizerItemFetchRequest request;
    request.setFilter(filter);
    request.setStartDate(startDateTime);
    request.setEndDate(endDateTime);
    request.setMaxCount(maxCount);
    request.setSorting(sortOrders);
    request.setFetchHint(fetchHint);

    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0))
            *error = request.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return request.items();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::itemsForExport(const QDateTime &startDateTime,
                                                             const QDateTime &endDateTime,
                                                             const QOrganizerItemFilter &filter,
                                                             const QList<QOrganizerItemSortOrder> &sortOrders,
                                                             const QOrganizerItemFetchHint &fetchHint,
                                                             QOrganizerManager::Error *error)
{
    QOrganizerItemFetchForExportRequest request;
    request.setStartDate(startDateTime);
    request.setEndDate(endDateTime);
    request.setFilter(filter);
    request.setSorting(sortOrders);
    request.setFetchHint(fetchHint);
    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0))
            *error = request.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return request.items();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

QList<QOrganizerItem> QOrganizerJsonDbEngine::items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint,
                                                    QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    QOrganizerItemFetchByIdRequest fetchByIdReq;
    fetchByIdReq.setIds(itemIds);
    fetchByIdReq.setFetchHint(fetchHint);
    if (startRequest(&fetchByIdReq)) {
        if (waitForRequestFinished(&fetchByIdReq, 0)) {
            *error = fetchByIdReq.error();
            *errorMap = fetchByIdReq.errorMap();
        } else {
            *error = QOrganizerManager::TimeoutError;
        }
        return fetchByIdReq.items();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerItem>();
    }
}

bool QOrganizerJsonDbEngine::saveItems(QList<QOrganizerItem> *items, const QList<QOrganizerItemDetail::DetailType> &detailMask,
                                       QMap<int, QOrganizerManager::Error> *errorMap, QOrganizerManager::Error *error)
{
    QOrganizerItemSaveRequest request;
    request.setItems(*items);
    request.setDetailMask(detailMask);
    if (startRequest(&request)) {
        if (waitForRequestFinished(&request, 0)) {
            *error = request.error();
            *errorMap = request.errorMap();
        } else {
            *error = QOrganizerManager::TimeoutError;
        }
        *items = request.items();
        return (*error == QOrganizerManager::NoError);
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

bool QOrganizerJsonDbEngine::removeItems(const QList<QOrganizerItemId> &itemIds, QMap<int, QOrganizerManager::Error> *errorMap,
                                         QOrganizerManager::Error *error)
{
    QOrganizerItemRemoveByIdRequest removeByIdReq;
    removeByIdReq.setItemIds(itemIds);
    startRequest(&removeByIdReq);

    if (waitForRequestFinished(&removeByIdReq, 0)) {
        *errorMap = removeByIdReq.errorMap();
        *error = removeByIdReq.error();
    }
    return *error == QOrganizerManager::NoError;
}

bool QOrganizerJsonDbEngine::removeItems(const QList<QOrganizerItem> *items, QMap<int, QOrganizerManager::Error>* errorMap, QOrganizerManager::Error* error)
{
    QOrganizerItemRemoveRequest removeReq;
    removeReq.setItems(*items);
    if (startRequest(&removeReq)) {
        if (waitForRequestFinished(&removeReq, 0)) {
            *errorMap = removeReq.errorMap();
            *error = removeReq.error();
        } else {
            *error = QOrganizerManager::TimeoutError;
        }
    } else {
        *error = QOrganizerManager::NotSupportedError;
    }
    return (*error == QOrganizerManager::NoError);
}

QOrganizerCollection QOrganizerJsonDbEngine::defaultCollection(QOrganizerManager::Error* error)
{
    *error = QOrganizerManager::NoError;
    return m_requestHandlerThread->defaultCollection();
}

QOrganizerCollection QOrganizerJsonDbEngine::collection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    *error = QOrganizerManager::NoError;
    QList<QOrganizerCollection> collectionList = collections(error);
    for (int i = 0; i < collectionList.size(); i++) {
        if (collectionList[i].id() == collectionId)
            return collectionList[i];
    }
    *error = QOrganizerManager::DoesNotExistError;
    return QOrganizerCollection();
}

QList<QOrganizerCollection> QOrganizerJsonDbEngine::collections(QOrganizerManager::Error* error)
{
    QOrganizerCollectionFetchRequest collectionFetchReq;
    if (startRequest(&collectionFetchReq)) {
        if (waitForRequestFinished(&collectionFetchReq, 0))
            *error = collectionFetchReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return collectionFetchReq.collections();
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return QList<QOrganizerCollection>();
    }
}

bool QOrganizerJsonDbEngine::saveCollection(QOrganizerCollection* collection, QOrganizerManager::Error* error)
{
    QOrganizerCollectionSaveRequest collectionSaveReq;
    collectionSaveReq.setCollection(*collection);
    if (startRequest(&collectionSaveReq)) {
        if (waitForRequestFinished(&collectionSaveReq, 0))
            *error = collectionSaveReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        *collection = collectionSaveReq.collections().at(0);
        return *error == QOrganizerManager::NoError;
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

bool QOrganizerJsonDbEngine::removeCollection(const QOrganizerCollectionId& collectionId, QOrganizerManager::Error* error)
{
    QOrganizerCollectionRemoveRequest collectionRemoveReq;
    collectionRemoveReq.setCollectionId(collectionId);
    if (startRequest(&collectionRemoveReq)) {
        if (waitForRequestFinished(&collectionRemoveReq, 0))
            *error = collectionRemoveReq.error();
        else
            *error = QOrganizerManager::TimeoutError;
        return *error == QOrganizerManager::NoError;
    } else {
        *error = QOrganizerManager::NotSupportedError;
        return false;
    }
}

bool QOrganizerJsonDbEngine::startRequest(QOrganizerAbstractRequest* req)
{
    QList<QOrganizerItem> itemList;
    QList<QOrganizerItemId> idList;
    QList<QOrganizerCollection> collectionList;
    QOrganizerManager::Error error = QOrganizerManager::NoError;
    QMap<int, QOrganizerManager::Error> errorMap;
    m_requestHandlerThread->addRequest(req);

    switch (req->type()) {
    case QOrganizerAbstractRequest::ItemFetchRequest: {
        QOrganizerManagerEngine::updateItemFetchRequest(static_cast<QOrganizerItemFetchRequest*>(req),
                                                        itemList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemIdFetchRequest: {
        QOrganizerManagerEngine::updateItemIdFetchRequest(static_cast<QOrganizerItemIdFetchRequest*>(req),
                                                          idList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchByIdRequest: {
        QOrganizerManagerEngine:: updateItemFetchByIdRequest(static_cast<QOrganizerItemFetchByIdRequest*>(req),
                                                              itemList, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemFetchForExportRequest: {
        QOrganizerManagerEngine::updateItemFetchForExportRequest(static_cast<QOrganizerItemFetchForExportRequest*>(req),
                                                                 itemList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemOccurrenceFetchRequest: {
        QOrganizerManagerEngine::updateItemOccurrenceFetchRequest(static_cast<QOrganizerItemOccurrenceFetchRequest*>(req),
                                                                  itemList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemSaveRequest: {
        QOrganizerItemSaveRequest* saveReq = static_cast<QOrganizerItemSaveRequest*>(req);
        itemList = saveReq->items();
        QOrganizerManagerEngine::updateItemSaveRequest(saveReq, itemList, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemRemoveRequest: {
        QOrganizerManagerEngine::updateItemRemoveRequest(static_cast<QOrganizerItemRemoveRequest*>(req),
                                                         error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::ItemRemoveByIdRequest: {
        QOrganizerManagerEngine::updateItemRemoveByIdRequest(static_cast<QOrganizerItemRemoveByIdRequest*>(req),
                                                             error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::CollectionFetchRequest: {
        QOrganizerManagerEngine::updateCollectionFetchRequest(static_cast<QOrganizerCollectionFetchRequest*>(req),
                                                              collectionList, error, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::CollectionSaveRequest: {
        QOrganizerCollectionSaveRequest* collectionSaveReq = static_cast<QOrganizerCollectionSaveRequest*>(req);
        collectionList = collectionSaveReq->collections();
        QOrganizerManagerEngine::updateCollectionSaveRequest(collectionSaveReq, collectionList, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    case QOrganizerAbstractRequest::CollectionRemoveRequest: {
        QOrganizerCollectionRemoveRequest* collectionRemoveReq = static_cast<QOrganizerCollectionRemoveRequest*>(req);
        QOrganizerManagerEngine::updateCollectionRemoveRequest(collectionRemoveReq, error, errorMap, QOrganizerAbstractRequest::ActiveState);
        break;
    }
    default:
        return false;
    }

    emit requestReceived(req);
    return true;
}

bool QOrganizerJsonDbEngine::cancelRequest(QOrganizerAbstractRequest* req)
{
    /*
        TODO

        Cancel an in progress async request.  If not possible, return false from here.
    */
    return QOrganizerManagerEngine::cancelRequest(req);
}

bool QOrganizerJsonDbEngine::waitForRequestFinished(QOrganizerAbstractRequest* req, int msecs)
{
    return m_requestHandlerThread->waitForRequestFinished(req, msecs);
}

void QOrganizerJsonDbEngine::requestDestroyed(QOrganizerAbstractRequest* req)
{
    m_requestHandlerThread->requestDestroyed(req);
}

QList<QOrganizerItemFilter::FilterType> QOrganizerJsonDbEngine::supportedFilters() const
{
    QList<QOrganizerItemFilter::FilterType> supported;

    supported << QOrganizerItemFilter::InvalidFilter
              << QOrganizerItemFilter::DetailFilter
              << QOrganizerItemFilter::DetailFieldFilter
              << QOrganizerItemFilter::IntersectionFilter
              << QOrganizerItemFilter::UnionFilter
              << QOrganizerItemFilter::IdFilter
              << QOrganizerItemFilter::CollectionFilter
              << QOrganizerItemFilter::DefaultFilter;

    return supported;
}

QList<QOrganizerItemDetail::DetailType> QOrganizerJsonDbEngine::supportedItemDetails(QOrganizerItemType::ItemType itemType) const
{
    QList<QOrganizerItemDetail::DetailType> supportedDetails;
    supportedDetails << QOrganizerItemDetail::TypeItemType
                     << QOrganizerItemDetail::TypeGuid
//                     << QOrganizerItemDetail::TypeTimestamp
                     << QOrganizerItemDetail::TypeDisplayLabel
                     << QOrganizerItemDetail::TypeDescription
                     << QOrganizerItemDetail::TypeComment
                     << QOrganizerItemDetail::TypeTag
                     << QOrganizerItemDetail::TypeExtendedDetail
                     << QOrganizerItemDetail::TypeVersion;

    if (itemType == QOrganizerItemType::TypeEvent) {
        supportedDetails << QOrganizerItemDetail::TypeRecurrence
                         << QOrganizerItemDetail::TypeEventTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeLocation
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder
                         << QOrganizerItemDetail::TypeEventAttendee
                         << QOrganizerItemDetail::TypeEventRsvp;
    } else if (itemType == QOrganizerItemType::TypeTodo) {
        supportedDetails << QOrganizerItemDetail::TypeRecurrence
                         << QOrganizerItemDetail::TypeTodoTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeTodoProgress
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder;
    } else if (itemType == QOrganizerItemType::TypeEventOccurrence) {
        supportedDetails << QOrganizerItemDetail::TypeParent
                         << QOrganizerItemDetail::TypeEventTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeLocation
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder
                         << QOrganizerItemDetail::TypeEventAttendee
                         << QOrganizerItemDetail::TypeEventRsvp;
    } else if (itemType == QOrganizerItemType::TypeTodoOccurrence) {
        supportedDetails << QOrganizerItemDetail::TypeParent
                         << QOrganizerItemDetail::TypeTodoTime
                         << QOrganizerItemDetail::TypePriority
                         << QOrganizerItemDetail::TypeTodoProgress
                         << QOrganizerItemDetail::TypeReminder
                         << QOrganizerItemDetail::TypeAudibleReminder;
    } else {
        // We don't support Journal and Note, yet ;)
        supportedDetails.clear();
    }

    return supportedDetails;
}

QList<QOrganizerItemType::ItemType> QOrganizerJsonDbEngine::supportedItemTypes() const
{
    QList<QOrganizerItemType::ItemType> ret;

    ret << QOrganizerItemType::TypeEvent;
    ret << QOrganizerItemType::TypeEventOccurrence;
//    ret << QOrganizerItemType::TypeJournal;
//    ret << QOrganizerItemType::TypeNote;
    ret << QOrganizerItemType::TypeTodo;
    ret << QOrganizerItemType::TypeTodoOccurrence;

    return  ret;
}

const QList<int> QOrganizerJsonDbEngine::supportedDetailFields(QOrganizerItemDetail::DetailType detailType)
{
    static QMap<int, QList<int> > supportedFieldsByType;
    if (supportedFieldsByType.isEmpty()) {
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeAudibleReminder,
                                     QList<int>()  << QOrganizerItemAudibleReminder::FieldDataUrl
                                     << QOrganizerItemAudibleReminder::FieldRepetitionCount
                                     << QOrganizerItemAudibleReminder::FieldRepetitionDelay
                                     << QOrganizerItemAudibleReminder::FieldSecondsBeforeStart);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeClassification,
                                     QList<int>() << QOrganizerItemClassification::FieldClassification);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeComment,
                                     QList<int>() << QOrganizerItemComment::FieldComment);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeDescription,
                                     QList<int>() << QOrganizerItemDescription::FieldDescription);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeDisplayLabel,
                                     QList<int>() << QOrganizerItemDisplayLabel::FieldLabel);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeEmailReminder,
                                     QList<int>() << QOrganizerItemEmailReminder::FieldAttachments
                                     << QOrganizerItemEmailReminder::FieldBody
                                     << QOrganizerItemEmailReminder::FieldRecipients
                                     << QOrganizerItemEmailReminder::FieldRepetitionCount
                                     << QOrganizerItemEmailReminder::FieldRepetitionDelay
                                     << QOrganizerItemEmailReminder::FieldSecondsBeforeStart
                                     << QOrganizerItemEmailReminder::FieldSubject);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeEventAttendee,
                                     QList<int>() << QOrganizerEventAttendee::FieldAttendeeId
                                     << QOrganizerEventAttendee::FieldEmailAddress
                                     << QOrganizerEventAttendee::FieldName
                                     << QOrganizerEventAttendee::FieldParticipationRole
                                     << QOrganizerEventAttendee::FieldParticipationStatus);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeEventRsvp,
                                     QList<int>() << QOrganizerEventRsvp::FieldOrganizerEmail
                                     << QOrganizerEventRsvp::FieldOrganizerName
                                     << QOrganizerEventRsvp::FieldParticipationRole
                                     << QOrganizerEventRsvp::FieldParticipationStatus
                                     << QOrganizerEventRsvp::FieldResponseDate
                                     << QOrganizerEventRsvp::FieldResponseDeadline
                                     << QOrganizerEventRsvp::FieldResponseRequirement);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeEventTime,
                                     QList<int>() << QOrganizerEventTime::FieldAllDay
                                     << QOrganizerEventTime::FieldEndDateTime
                                     << QOrganizerEventTime::FieldStartDateTime);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeExtendedDetail,
                                     QList<int>() << QOrganizerItemExtendedDetail::FieldData
                                     << QOrganizerItemExtendedDetail::FieldName);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeGuid,
                                     QList<int>() << QOrganizerItemGuid::FieldGuid);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeItemType,
                                     QList<int>() << QOrganizerItemType::FieldType);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeJournalTime,
                                     QList<int>() << QOrganizerJournalTime::FieldEntryDateTime);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeLocation,
                                     QList<int>() << QOrganizerItemLocation::FieldLabel
                                     << QOrganizerItemLocation::FieldLatitude
                                     << QOrganizerItemLocation::FieldLongitude);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeParent,
                                     QList<int>() << QOrganizerItemParent::FieldOriginalDate
                                     << QOrganizerItemParent::FieldParentId);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypePriority,
                                     QList<int>() << QOrganizerItemPriority::FieldPriority);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeRecurrence,
                                     QList<int>() << QOrganizerItemRecurrence::FieldExceptionDates
                                     << QOrganizerItemRecurrence::FieldExceptionRules
                                     << QOrganizerItemRecurrence::FieldRecurrenceDates
                                     << QOrganizerItemRecurrence::FieldRecurrenceRules);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeReminder,
                                     QList<int>() << QOrganizerItemReminder::FieldRepetitionCount
                                     << QOrganizerItemReminder::FieldRepetitionDelay
                                     << QOrganizerItemReminder::FieldSecondsBeforeStart);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeTag,
                                     QList<int>() << QOrganizerItemTag::FieldTag);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeTimestamp,
                                     QList<int>() << QOrganizerItemTimestamp::FieldCreated
                                     << QOrganizerItemTimestamp::FieldLastModified);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeTodoProgress,
                                     QList<int>() << QOrganizerTodoProgress::FieldFinishedDateTime
                                     << QOrganizerTodoProgress::FieldPercentageComplete
                                     << QOrganizerTodoProgress::FieldStatus);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeTodoTime,
                                     QList<int>() << QOrganizerTodoTime::FieldAllDay
                                     << QOrganizerTodoTime::FieldDueDateTime
                                     << QOrganizerTodoTime::FieldStartDateTime);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeUndefined,
                                     QList<int>());
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeVersion,
                                     QList<int>() << QOrganizerItemVersion::FieldExtendedVersion
                                     << QOrganizerItemVersion::FieldVersion);
        supportedFieldsByType.insert(QOrganizerItemDetail::TypeVisualReminder,
                                     QList<int>() << QOrganizerItemVisualReminder::FieldDataUrl
                                     << QOrganizerItemVisualReminder::FieldMessage
                                     << QOrganizerItemVisualReminder::FieldRepetitionCount
                                     << QOrganizerItemVisualReminder::FieldRepetitionDelay
                                     << QOrganizerItemVisualReminder::FieldSecondsBeforeStart);
    }
    return supportedFieldsByType.value(detailType, QList<int>());
}

#include "moc_qorganizerjsondbengine.cpp"

QT_END_NAMESPACE_ORGANIZER

Q_DECLARE_METATYPE(QTORGANIZER_PREPEND_NAMESPACE(QOrganizerAbstractRequest::State))
Q_DECLARE_METATYPE(QList<QTORGANIZER_PREPEND_NAMESPACE(QOrganizerItemId)>)
