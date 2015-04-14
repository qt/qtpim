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

#include "qorganizeritem.h"
#include "qorganizeritem_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qorganizeritemdetail_p.h"
#include "qorganizeritemdetails.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \macro Q_DECLARE_CUSTOM_ORGANIZER_ITEM
    \relates QOrganizerItem

    Macro for simplifying declaring convenience leaf classes for QOrganizerItem.

    The first argument is the name of the class, and the second argument is the
    item type.
 */

/*!
    \class QOrganizerItem
    \brief The QOrganizerItem class is the base class of an event, todo, note, or journal entry.
    \inmodule QtOrganizer
    \ingroup organizer-main

    A QOrganizerItem object has an id and a collection of details (like a start date and location), as
    well as a collection id which identifies which QOrganizerCollection the item is part of in a manager.
    Each detail (which can have multiple fields) is stored in an appropriate subclass of QOrganizerItemDetail,
    and the QOrganizerItem allows retrieving these details in various ways. QOrganizerItemExtendedDetail is
    supposed to be used to store user specific details that are not pre-defined in the detal leaf classes.

    Most clients will want to use the convenient subclasses of QOrganizerItem (i.e., QOrganizerEvent
    (and QOrganizerEventOccurence), QOrganizerTodo (and QOrganizerTodoOccurence), QOrganizerJournal and
    QOrganizerNote) instead of manipulating instances of QOrganizerItem directly.

    A QOrganizerItem instance represents the in-memory version of an organizer item.
    It is possible for the contents of a QOrganizerItem to change independently of the contents
    that are stored persistently in a QOrganizerManager.  A QOrganizerItem has an id associated
    with it when it is first retrieved from a QOrganizerManager, or after it has been first saved,
    and this allows clients to track changes using the signals in QOrganizerManager.  When saved
    in a manager, every item is placed into a QOrganizerCollection in that manager, according
    to the collection id set in the item prior to save (or the default collection if no
    collection id was set in the item).

    Different QOrganizerManagers may require an item to have certain details saved in it before
    it can be stored in that manager.  By default, every item must have a QOrganizerItemType
    detail which identifies the type of the item.  Different subclasses of QOrganizerItem
    (i.e., QOrganizerEvent (and QOrganizerEventOccurence), QOrganizerTodo (and QOrganizerTodoOccurence),
    QOrganizerJournal and QOrganizerNote) may have other mandatory details, depending on the manager.

    \sa QOrganizerManager, QOrganizerItemDetail
 */

/*!
    \fn QOrganizerItem::operator!=(const QOrganizerItem &other) const

    Returns true if this organizer item's id or details are different to those of the \a other organizer item.
 */

/*!
    Construct an empty organizer item.

    The organizer item will have an empty item ID, and an empty collection ID. It's of type \l QOrganizerItemType::TypeUndefined.
 */
QOrganizerItem::QOrganizerItem()
    : d(new QOrganizerItemData)
{
    QOrganizerItemType organizeritemType;
    organizeritemType.setType(QOrganizerItemType::TypeUndefined);
    d->m_details.append(organizeritemType);
}

/*!
    Constructs an item that is a copy of \a other.
 */
QOrganizerItem::QOrganizerItem(const QOrganizerItem &other)
    : d(other.d)
{
}

/*!
    \internal

    Constructs a new, empty item of the given type \a type.
*/
QOrganizerItem::QOrganizerItem(QOrganizerItemType::ItemType type)
    : d(new QOrganizerItemData)
{
    QOrganizerItemType organizeritemType;
    organizeritemType.setType(type);
    d->m_details.append(organizeritemType);
}

/*!
    \internal

    Constructs an item that is a copy of \a other if \a other is of the expected type
    identified by \a expectedType, else constructs a new, empty item of the
    type identified by the \a expectedType.

    The \a expectedType pointer must be valid for the lifetime of the program.
*/
QOrganizerItem::QOrganizerItem(const QOrganizerItem &other, QOrganizerItemType::ItemType expectedType)
{
    if (other.type() == expectedType) {
        d = other.d;
    } else {
        d = new QOrganizerItemData;
        setType(expectedType);
    }
}

/*!
    \internal

    Assigns this item to \a other if the type of \a other is that identified
    by the given \a expectedType, else assigns this item to be a new, empty
    item of the type identified by the given \a expectedType
*/
QOrganizerItem &QOrganizerItem::assign(const QOrganizerItem &other, QOrganizerItemType::ItemType expectedType)
{
    if (this != &other) {
        if (other.type() == expectedType) {
            d = other.d;
        } else {
            d = new QOrganizerItemData;
            setType(expectedType);
        }
    }
    return *this;
}


/*!
    Returns true if this QOrganizerItem is empty, false if not.

    Note that the type detail of the organizer item is irrelevant.
 */
bool QOrganizerItem::isEmpty() const
{
    return (d->m_details.count() == 1);
}

/*!
    Removes all details of the organizer item, and resets the type to be \l QOrganizerItemType::TypeUndefined.
 */
void QOrganizerItem::clearDetails()
{
    d->m_details.clear();

    QOrganizerItemType organizeritemType;
    organizeritemType.setType(QOrganizerItemType::TypeUndefined);
    d->m_details.append(organizeritemType);
}

/*!
    Replace the contents of this organizer item with the \a other.
 */
QOrganizerItem &QOrganizerItem::operator=(const QOrganizerItem &other)
{
    d = other.d;
    return *this;
}

/*!
    Frees the memory used by this item.
 */
QOrganizerItem::~QOrganizerItem()
{
}

/*!
    Returns the QOrganizerItemId that identifies this organizer item.

    This may have been set when the organizer item was retrieved from
    a particular manager, or when the organizer item was first saved
    in a manager.  The QOrganizerItemId is only valid within a specific
    manager.  See \l QOrganizerManager::saveItem() for more
    information.
 */
QOrganizerItemId QOrganizerItem::id() const
{
    return d->m_id;
}

/*!
    Returns the id of the collection which this item is part of, in the manager
    in which the item has been saved, if the item has previously been saved in
    a manager.  If the item has not previously been saved in a manager, this function
    will return the id of the collection into which the client wishes the item to be
    saved when \l QOrganizerManager::saveItem() is called, which is set by calling
    \l setId(); otherwise, returns a null id.

    An item always belongs to exactly one collection in a particular manager after it
    has been saved in the manager.  If the item has previously been saved in the manager,
    in a particular collection, and the client sets the collection id of the item to
    the id of a different collection within that manager and then resaves the item,
    the item will be moved from its original collection into the specified collection
    if the move operation is supported by the manager; otherwise, the
    \l QOrganizerManager::saveItem() operation will fail and calling
    \l QOrganizerManager::error() will return \c QOrganizerManager::NotSupportedError.
 */
QOrganizerCollectionId QOrganizerItem::collectionId() const
{
    return d->m_collectionId;
}

/*!
    Sets the id of the collection into which the client wishes the item to be saved
    to the given \a collectionId.

    If the given \a collectionId is the null collection id, the client is specifying
    that the item should be saved into the collection in which the item is already
    saved (if the item has previously been saved in the manager, without having been
    removed since), or into the default collection of the manager (if the item has
    not previously been saved in the manager, or has been removed since the last time
    it was saved).

    If the item has previously been saved in a particular manager, and the given
    \a collectionId is the id of a different collection than the one which the
    item is currently a part of in that manager, saving the item with
    \l QOrganizerManager::saveItem() will move the item from its original
    collection to the collection whose id is \a collectionId, if \a collectionId
    identifies a valid collection and the operation is supported by the manager.
 */
void QOrganizerItem::setCollectionId(const QOrganizerCollectionId &collectionId)
{
    d->m_collectionId = collectionId;
}

/*!
    Sets the id of this organizer item to \a id.

    Note that this only affects this object, not any corresponding structures stored
    by a QOrganizerManager.

    If you change the id of a organizer item and save the organizer item
    in a manager, the previously existing organizer item will still
    exist.  You can do this to create copies (possibly modified)
    of an existing organizer item, or to save a organizer item in a different manager.

    \sa QOrganizerManager::saveItem()
 */
void QOrganizerItem::setId(const QOrganizerItemId &id)
{
    d->m_id = id;
    // TODO - reset collection id?
}

/*!
    Returns the first detail stored in the organizer item with the given \a detailType. If the
    given \a detailType is TypeUndefined, it returns the first detail found.
 */
QOrganizerItemDetail QOrganizerItem::detail(QOrganizerItemDetail::DetailType detailType) const
{
    if (detailType == QOrganizerItemDetail::TypeUndefined)
        return d->m_details.first();

    for (int i = 0; i < d->m_details.size(); i++) {
        const QOrganizerItemDetail &existing = d->m_details.at(i);
        if (existing.d->m_detailType == detailType)
            return existing;
    }

    return QOrganizerItemDetail();
}

/*!
    Returns a list of details with the given \a detailType. If the given \a detailType is of TypeUndefined,
    it returns all the details.
 */
QList<QOrganizerItemDetail> QOrganizerItem::details(QOrganizerItemDetail::DetailType detailType) const
{
    if (detailType == QOrganizerItemDetail::TypeUndefined)
        return d->m_details;

    QList<QOrganizerItemDetail> sublist;
    for (int i = 0; i < d->m_details.size(); i++) {
        const QOrganizerItemDetail &existing = d->m_details.at(i);
        if (existing.d->m_detailType == detailType)
            sublist.append(existing);
    }
    return sublist;
}

/*!
    Saves the given \a detail in the list of stored details, and sets the detail's id.
    If another detail of the same type and id has been previously saved in
    this organizer item, that detail is overwritten.  Otherwise, a new id is generated
    and set in the detail, and the detail is added to the organizer item.

    If \a detail is a QOrganizerItemType, the existing organizer item type will
    be overwritten with \a detail.  There is never more than one organizer item type
    in a organizer item.

    Returns true if the detail was saved successfully, otherwise returns false.

    Note that the caller retains ownership of the detail.
 */
bool QOrganizerItem::saveDetail(QOrganizerItemDetail *detail)
{
    if (!detail)
        return false;

    // we only allow one instance of these details per item
    if (detail->d.constData()->m_detailType == QOrganizerItemDetail::TypeItemType
        || detail->d.constData()->m_detailType == QOrganizerItemDetail::TypeDescription
        || detail->d.constData()->m_detailType == QOrganizerItemDetail::TypeDisplayLabel
        || detail->d.constData()->m_detailType == QOrganizerItemDetail::TypeClassification
        || detail->d.constData()->m_detailType == QOrganizerItemDetail::TypeVersion) {
        for (int i = 0; i < d.constData()->m_details.size(); i++) {
            if (detail->d.constData()->m_detailType == d.constData()->m_details.at(i).d.constData()->m_detailType) {
                d->m_details.replace(i, *detail);
                return true;
            }
        }
        // doesn't already exist; append it.
        d->m_details.append(*detail);
        return true;
    }

    // try to find the "old version" of this field
    // ie, the one with the same type and id, but different value or attributes.
    for (int i = 0; i < d.constData()->m_details.size(); i++) {
        const QOrganizerItemDetail& curr = d.constData()->m_details.at(i);
        if (detail->d.constData()->m_detailType == curr.d.constData()->m_detailType && detail->d.constData()->m_id == curr.d.constData()->m_id) {
            // update the detail constraints of the supplied detail
            // Found the old version.  Replace it with this one.
            d->m_details[i] = *detail;
            return true;
        }
    }
    // this is a new detail!  add it to the organizer item.
    d->m_details.append(*detail);
    return true;
}

/*!
    Removes the \a detail from the organizer item.

    The detail in the organizer item which has the same key as that of the given \a detail
    will be removed if it exists.  Only the key is used for comparison - that is, the
    information in the detail may be different.

    Returns true if the detail was removed successfully, false if an error occurred.

    Note that the caller retains ownership of the detail.
 */
bool QOrganizerItem::removeDetail(QOrganizerItemDetail *detail)
{
    if (!detail)
        return false;

    // find the detail stored in the organizer item which has the same key as the detail argument
    int removeIndex = -1;
    for (int i = 0; i < d.constData()->m_details.size(); i++) {
        if (d.constData()->m_details.at(i).key() == detail->key()) {
            removeIndex = i;
            break;
        }
    }

    // make sure the detail exists (in some form) in the organizer item.
    if (removeIndex < 0)
        return false;

    // Type -detail is specific case which cannot be deleted
    if (QOrganizerItemDetail::TypeItemType == detail->d.constData()->m_detailType)
        return false;

    if (!d.constData()->m_details.contains(*detail))
        return false;

    // then remove the detail.
    d->m_details.removeAt(removeIndex);
    return true;
}

/*!
    Returns true if this organizer item is equal to the \a other organizer item, false if either the
    id, collection id or stored details are not the same.
 */
bool QOrganizerItem::operator==(const QOrganizerItem &other) const
{
    if (d == other.d)
        return true;

    if (other.d->m_id != d->m_id
        || other.d->m_collectionId != d->m_collectionId
        || d->m_details.size() != other.d->m_details.size()) {
        return false;
    }

    QList<QOrganizerItemDetail> searchList(d->m_details);
    foreach (const QOrganizerItemDetail &detail, other.d->m_details) {
        if (!searchList.removeOne(detail))
            return false;
    }

    return true;
}

/*!
    \relates QOrganizerItem

    Returns the hash value for \a key.
 */
uint qHash(const QOrganizerItem &key)
{
    uint hash = qHash(key.id());
    hash += qHash(key.collectionId());
    foreach (const QOrganizerItemDetail &detail, key.details())
        hash += qHash(detail);
    return hash;
}

#ifndef QT_NO_DEBUG_STREAM
/*!
    \relates QOrganizerItem
    Streams the \a item to the given debug stream \a dbg, and returns the stream.
 */
QDebug operator<<(QDebug dbg, const QOrganizerItem &item)
{
    dbg.nospace() << "QOrganizerItem(" << item.id() << ") in collection(" << item.collectionId() << ")";
    foreach (const QOrganizerItemDetail& detail, item.details())
        dbg.space() << '\n' << detail;
    return dbg.maybeSpace();
}
#endif // QT_NO_DEBUG_STREAM

#ifndef QT_NO_DATASTREAM
/*!
    \relates QOrganizerItem
    Writes \a item to the stream \a out.
 */
QDataStream &operator<<(QDataStream &out, const QOrganizerItem &item)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QOrganizerItem
    out << formatVersion
        << item.id().toString()
        << item.collectionId().toString()
        << item.details();
    return out;
}

/*!
    \relates QOrganizerItem
    Reads an item from stream \a in into \a item.
 */
QDataStream &operator>>(QDataStream &in, QOrganizerItem &item)
{
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        item = QOrganizerItem();
        QString itemIdString;
        QString collectionIdString;
        QList<QOrganizerItemDetail> details;
        in >> itemIdString >> collectionIdString >> details;
        item.setId(QOrganizerItemId::fromString(itemIdString));
        item.setCollectionId(QOrganizerCollectionId::fromString(collectionIdString));
        item.d->m_details = details;
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif // QT_NO_DATASTREAM

/*!
    Returns the type of the organizer item.
 */
QOrganizerItemType::ItemType QOrganizerItem::type() const
{
    // type is always the first detail
    QOrganizerItemType type = static_cast<QOrganizerItemType>(d->m_details.at(0));
    return type.type();
}

/*!
    Sets the type of the organizer item to the given \a type.
 */
void QOrganizerItem::setType(QOrganizerItemType::ItemType type)
{
    if (d->m_details.isEmpty()) {
        QOrganizerItemType organizeritemType;
        organizeritemType.setType(type);
        d->m_details.append(organizeritemType);
    } else {
        // type is always the first detail
        d->m_details.first().setValue(QOrganizerItemType::FieldType, type);
    }
}

/*!
    Returns the display label of the item.
 */
QString QOrganizerItem::displayLabel() const
{
    QOrganizerItemDisplayLabel dl = detail(QOrganizerItemDetail::TypeDisplayLabel);
    return dl.label();
}

/*!
    Sets the display label of the item to \a label.
 */
void QOrganizerItem::setDisplayLabel(const QString &label)
{
    QOrganizerItemDisplayLabel dl = detail(QOrganizerItemDetail::TypeDisplayLabel);
    dl.setLabel(label);
    saveDetail(&dl);
}

/*!
    Returns the human-readable description of the item.
 */
QString QOrganizerItem::description() const
{
    QOrganizerItemDescription descr = detail(QOrganizerItemDetail::TypeDescription);
    return descr.description();
}

/*!
    Sets the human-readable description of the item to \a description.
 */
void QOrganizerItem::setDescription(const QString &description)
{
    QOrganizerItemDescription descr = detail(QOrganizerItemDetail::TypeDescription);
    descr.setDescription(description);
    saveDetail(&descr);
}

/*!
    Returns the list of comments of this item.
 */
QStringList QOrganizerItem::comments() const
{
    QStringList commentList;
    for (int i = 0; i < d->m_details.size(); ++i) {
        const QOrganizerItemDetail &detail = d->m_details.at(i);
        if (detail.d->m_detailType == QOrganizerItemDetail::TypeComment)
            commentList.append(detail.d->m_values.value(QOrganizerItemComment::FieldComment).toString());
    }
    return commentList;
}

/*!
    Removes all comments of this item.
 */
void QOrganizerItem::clearComments()
{
    d->removeOnly(QOrganizerItemDetail::TypeComment);
}

/*!
    Sets the list of comments associated with the item to \a comments.
 */
void QOrganizerItem::setComments(const QStringList &comments)
{
    d->removeOnly(QOrganizerItemDetail::TypeComment);
    foreach (const QString &comment, comments)
        addComment(comment);
}

/*!
    Adds the \a comment to this item
 */
void QOrganizerItem::addComment(const QString &comment)
{
    QOrganizerItemComment detail;
    detail.setComment(comment);
    saveDetail(&detail);
}

/*!
    Returns the list of tags for this item.
 */
QStringList QOrganizerItem::tags() const
{
    QStringList tagList;
    for (int i = 0; i < d->m_details.size(); ++i) {
        const QOrganizerItemDetail &detail = d->m_details.at(i);
        if (detail.d->m_detailType == QOrganizerItemDetail::TypeTag)
            tagList.append(detail.d->m_values.value(QOrganizerItemTag::FieldTag).toString());
    }
    return tagList;
}

/*!
    Removes all tags associated with the item.
 */
void QOrganizerItem::clearTags()
{
    d->removeOnly(QOrganizerItemDetail::TypeTag);
}

/*!
    Adds the \a tag to this item.
 */
void QOrganizerItem::addTag(const QString &tag)
{
    QOrganizerItemTag tagDetail;
    tagDetail.setTag(tag);
    saveDetail(&tagDetail);
}

/*!
    Sets the list of tags associated with the item to \a tags.
 */
void QOrganizerItem::setTags(const QStringList &tags)
{
    d->removeOnly(QOrganizerItemDetail::TypeTag);
    foreach (const QString &tag, tags)
        addTag(tag);
}

/*!
    Returns the globally unique identifier which identifies this item,
    which is used for synchronization purposes.
 */
QString QOrganizerItem::guid() const
{
    QOrganizerItemGuid guid = detail(QOrganizerItemDetail::TypeGuid);
    return guid.guid();
}

/*!
    Sets the item's globally unique identifier to \a guid.
 */
void QOrganizerItem::setGuid(const QString &guid)
{
    QOrganizerItemGuid guidDetail = detail(QOrganizerItemDetail::TypeGuid);
    guidDetail.setGuid(guid);
    saveDetail(&guidDetail);
}

/*!
    Returns the data of the extended detail with the given \a name.
 */
QVariant QOrganizerItem::data(const QString &name) const
{
    for (int i = 0; i < d->m_details.size(); ++i) {
        const QOrganizerItemDetail &detail = d->m_details.at(i);
        if (detail.d->m_detailType == QOrganizerItemDetail::TypeExtendedDetail
            && detail.d->m_values.value(QOrganizerItemExtendedDetail::FieldName).toString() == name) {
            return detail.d->m_values.value(QOrganizerItemExtendedDetail::FieldData);
        }
    }
    return QVariant();
}

/*!
    Sets the \a data of a extended detail with the given \a name.
 */
void QOrganizerItem::setData(const QString &name, const QVariant &data)
{
    for (int i = 0; i < d->m_details.size(); ++i) {
        const QOrganizerItemDetail &detail = d->m_details.at(i);
        if (detail.d->m_detailType == QOrganizerItemDetail::TypeExtendedDetail
            && detail.d->m_values.value(QOrganizerItemExtendedDetail::FieldName).toString() == name) {
            QOrganizerItemDetail newDetail = d->m_details.at(i);
            newDetail.d->m_values.insert(QOrganizerItemExtendedDetail::FieldData, data);
            saveDetail(&newDetail);
            return;
        }
    }

    QOrganizerItemExtendedDetail newDetail;
    newDetail.setName(name);
    newDetail.setData(data);
    saveDetail(&newDetail);
}

/*!
    \internal
 */
void QOrganizerItemData::removeOnly(QOrganizerItemDetail::DetailType detailType)
{
    QList<QOrganizerItemDetail>::iterator dit = m_details.begin();
    while (dit != m_details.end()) {
        // XXX this doesn't check type or display label
        if (dit->type() == detailType)
            dit = m_details.erase(dit);
        else
            ++dit;
    }
}

/*!
    \internal
 */
void QOrganizerItemData::removeOnly(const QSet<QOrganizerItemDetail::DetailType> &detailTypes)
{
    QList<QOrganizerItemDetail>::iterator dit = m_details.begin();
    while (dit != m_details.end()) {
        // XXX this doesn't check type or display label
        if (detailTypes.contains(dit->type()))
            dit = m_details.erase(dit);
        else
            ++dit;
    }
}

QT_END_NAMESPACE_ORGANIZER
