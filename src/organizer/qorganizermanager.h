/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QORGANIZERMANAGER_H
#define QORGANIZERMANAGER_H

#include <QtCore/qdatetime.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qobject.h>
#include <QtCore/qpair.h>
#include <QtCore/qstringlist.h>

#include <QtOrganizer/qorganizercollection.h>
#include <QtOrganizer/qorganizeritem.h>
#include <QtOrganizer/qorganizeritemfilter.h>
#include <QtOrganizer/qorganizeritemfetchhint.h>
#include <QtOrganizer/qorganizeritemsortorder.h>

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerManagerData;

class Q_ORGANIZER_EXPORT QOrganizerManager : public QObject
{
    Q_OBJECT

public:
#ifdef Q_QDOC // qdoc's parser fails to recognize the default map argument
    explicit QOrganizerManager(const QString &managerName, const QMap<QString, QString> &parameters = 0, QObject *parent = nullptr);
#else
    explicit QOrganizerManager(const QString &managerName, const QMap<QString, QString> &parameters = (QMap<QString, QString>()), QObject *parent = nullptr);
#endif
    explicit QOrganizerManager(QObject *parent = nullptr);
    ~QOrganizerManager();

    static QOrganizerManager *fromUri(const QString &uri, QObject *parent = nullptr);
    static QStringList availableManagers();

    QString managerName() const;
    QMap<QString, QString> managerParameters() const;
    int managerVersion() const;
    QString managerUri() const;

    static bool parseUri(const QString &uri, QString *managerName, QMap<QString, QString> *params);
    static QString buildUri(const QString &managerName, const QMap<QString, QString> &params);

    // error reporting
    enum Error {
        NoError = 0,
        DoesNotExistError,
        AlreadyExistsError,
        InvalidDetailError,
        LockedError,
        DetailAccessError,
        PermissionsError,
        OutOfMemoryError,
        NotSupportedError,
        BadArgumentError,
        UnspecifiedError,
        LimitReachedError,
        InvalidItemTypeError,
        InvalidCollectionError,
        InvalidOccurrenceError,
        TimeoutError
    };

    enum Operation {
        Add,
        Change,
        Remove
    };

    /* Error reporting */
    QOrganizerManager::Error error() const;
    QMap<int, QOrganizerManager::Error> errorMap() const;

    // items
    QOrganizerItem item(const QOrganizerItemId &itemId, const QOrganizerItemFetchHint &fetchHint = QOrganizerItemFetchHint());

    QList<QOrganizerItem> items(const QList<QOrganizerItemId> &itemIds, const QOrganizerItemFetchHint &fetchHint = QOrganizerItemFetchHint());

    QList<QOrganizerItem> items(const QDateTime &startDateTime = QDateTime(), const QDateTime &endDateTime = QDateTime(),
                                const QOrganizerItemFilter &filter = QOrganizerItemFilter(), int maxCount = -1,
                                const QList<QOrganizerItemSortOrder> &sortOrders = QList<QOrganizerItemSortOrder>(),
                                const QOrganizerItemFetchHint &fetchHint = QOrganizerItemFetchHint());

    QList<QOrganizerItemId> itemIds(const QDateTime &startDateTime = QDateTime(), const QDateTime &endDateTime = QDateTime(),
                                    const QOrganizerItemFilter &filter = QOrganizerItemFilter(),
                                    const QList<QOrganizerItemSortOrder> &sortOrders = QList<QOrganizerItemSortOrder>());

    QList<QOrganizerItem> itemOccurrences(const QOrganizerItem &parentItem, const QDateTime &startDateTime = QDateTime(),
                                          const QDateTime &endDateTime = QDateTime(), int maxCount = -1,
                                          const QOrganizerItemFetchHint &fetchHint = QOrganizerItemFetchHint());

    QList<QOrganizerItem> itemsForExport(const QDateTime &startDateTime = QDateTime(), const QDateTime &endDateTime = QDateTime(),
                                         const QOrganizerItemFilter &filter = QOrganizerItemFilter(),
                                         const QList<QOrganizerItemSortOrder> &sortOrders = QList<QOrganizerItemSortOrder>(),
                                         const QOrganizerItemFetchHint &fetchHint = QOrganizerItemFetchHint());

    bool saveItem(QOrganizerItem *item, const QList<QOrganizerItemDetail::DetailType> &detailMask = QList<QOrganizerItemDetail::DetailType>());

    bool saveItems(QList<QOrganizerItem> *items,
                   const QList<QOrganizerItemDetail::DetailType> &detailMask = QList<QOrganizerItemDetail::DetailType>());

    bool removeItem(const QOrganizerItemId &itemId);

    bool removeItem(const QOrganizerItem *item);

    bool removeItems(const QList<QOrganizerItemId>& itemIds);

    bool removeItems(const QList<QOrganizerItem> *items);

    // collections
    QOrganizerCollectionId defaultCollectionId() const;
    QOrganizerCollection collection(const QOrganizerCollectionId& collectionId) const;
    QList<QOrganizerCollection> collections() const;
    bool saveCollection(QOrganizerCollection* collection);
    bool removeCollection(const QOrganizerCollectionId& collectionId);

    // functionality reporting
    QList<QOrganizerItemFilter::FilterType> supportedFilters() const;
    QList<QOrganizerItemDetail::DetailType> supportedItemDetails(QOrganizerItemType::ItemType itemType) const;
    QList<QOrganizerItemType::ItemType> supportedItemTypes() const;

    // helper
    static QList<QOrganizerItemId> extractIds(const QList<QOrganizerItem> &items);

Q_SIGNALS:
    void dataChanged();
    void itemsAdded(const QList<QOrganizerItemId> &itemIds);
    void itemsChanged(const QList<QOrganizerItemId> &itemIds, const QList<QOrganizerItemDetail::DetailType> &typesChanged);
    void itemsRemoved(const QList<QOrganizerItemId> &itemIds);
    void itemsModified(const QList<QPair<QOrganizerItemId, QOrganizerManager::Operation> > &itemIds);
    void collectionsAdded(const QList<QOrganizerCollectionId> &collectionIds);
    void collectionsChanged(const QList<QOrganizerCollectionId> &collectionIds);
    void collectionsRemoved(const QList<QOrganizerCollectionId> &collectionIds);
    void collectionsModified(const QList<QPair<QOrganizerCollectionId, QOrganizerManager::Operation> > &collectionIds);

private:
    friend class QOrganizerManagerData;
    Q_DISABLE_COPY(QOrganizerManager)
    QOrganizerManagerData *d;

    void createEngine(const QString &managerName, const QMap<QString, QString> &parameters);

    Q_PRIVATE_SLOT(d, void _q_itemsUpdated(const QList<QOrganizerItemId> &ids, const QList<QOrganizerItemDetail::DetailType> &typesChanged))
    Q_PRIVATE_SLOT(d, void _q_itemsDeleted(const QList<QOrganizerItemId> &ids))
};

QT_END_NAMESPACE_ORGANIZER

#endif // QORGANIZERMANAGER_H
