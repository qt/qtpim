/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#include "qvcardrestorehandler_p.h"

#include <QtCore/qdatastream.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qurl.h>

#include "qversitproperty.h"

/*
    When these conditions are satisfied, QStringLiteral is implemented by
    gcc's statement-expression extension.  However, in this file it will
    not work, because "statement-expressions are not allowed outside functions
    nor in template-argument lists".

    Fall back to the less-performant QLatin1String in this case.
*/
#if defined(QStringLiteral) && defined(QT_UNICODE_LITERAL_II) && defined(Q_CC_GNU) && !defined(Q_COMPILER_LAMBDA)
# undef QStringLiteral
# define QStringLiteral QLatin1String
#endif

QT_BEGIN_NAMESPACE_VERSIT

const QString QVCardRestoreHandler::PropertyName(QStringLiteral("X-NOKIA-QCONTACTFIELD"));
const QString QVCardRestoreHandler::DetailTypeParameter(QStringLiteral("DETAIL"));
const QString QVCardRestoreHandler::FieldParameter(QStringLiteral("FIELD"));
const QString QVCardRestoreHandler::DatatypeParameter(QStringLiteral("DATATYPE"));
const QString QVCardRestoreHandler::DatatypeParameterVariant(QStringLiteral("VARIANT"));
const QString QVCardRestoreHandler::DatatypeParameterDate(QStringLiteral("DATE"));
const QString QVCardRestoreHandler::DatatypeParameterDateTime(QStringLiteral("DATETIME"));
const QString QVCardRestoreHandler::DatatypeParameterTime(QStringLiteral("TIME"));
const QString QVCardRestoreHandler::DatatypeParameterBool(QStringLiteral("BOOL"));
const QString QVCardRestoreHandler::DatatypeParameterInt(QStringLiteral("INT"));
const QString QVCardRestoreHandler::DatatypeParameterUInt(QStringLiteral("UINT"));
const QString QVCardRestoreHandler::DatatypeParameterUrl(QStringLiteral("URL"));
const QString QVCardRestoreHandler::GroupPrefix(QStringLiteral("G"));


/*
 * Returns a list of details generated from a Versit group.
 */
QList<QContactDetail> DetailGroupMap::detailsInGroup(const QString& groupName) const
{
    QList<int> detailIds = mDetailGroupName.keys(groupName);
    QList<QContactDetail> details;
    foreach (int detailId, detailIds) {
        details << mDetailById[detailId];
    }
    return details;
}

/*
 * Inserts the association between \a detail and \a groupName to the map.
 * The detail must have a key (ie. have already been saved in a contact) and the group name must not
 * be the empty string.
 */
void DetailGroupMap::insert(const QString& groupName, const QContactDetail& detail)
{
    Q_ASSERT(!groupName.isEmpty());
    mDetailGroupName[detail.key()] = groupName;
    mDetailById[detail.key()] = detail;
}

/*
 * Replaces the detail currently in the map with \a detail.
 * The detail must have a key (ie. have already been saved in a contact).
 */
void DetailGroupMap::update(const QContactDetail& detail)
{
    Q_ASSERT(detail.key());
    mDetailById[detail.key()] = detail;
}

/*!
 * Removes details and groups from the map.
 */
void DetailGroupMap::clear()
{
    mDetailGroupName.clear();
    mDetailById.clear();
}




bool QVCardRestoreHandler::propertyProcessed(
        const QVersitProperty& property,
        QList<QContactDetail>* updatedDetails)
{
    bool success = false;
    QString group;
    if (!property.groups().isEmpty())
        group = property.groups().first();
    if (property.name() == PropertyName) {
        if (property.groups().size() != 1)
            return false;
        QMultiHash<QString, QString> parameters = property.parameters();
        QContactDetail::DetailType detailType = QContactDetail::DetailType(parameters.value(DetailTypeParameter).toUInt());
        QString fieldName = parameters.value(FieldParameter);
        // Find a detail previously seen with the same definitionName, which was generated from
        // a property from the same group
        QContactDetail detail(detailType);
        foreach (const QContactDetail& previousDetail, mDetailGroupMap.detailsInGroup(group)) {
            if (previousDetail.type() == detailType) {
                detail = previousDetail;
            }
        }
        // If not found, it's a new empty detail with the definitionName set.

        detail.setValue(fieldName.toInt(), deserializeValue(property));

        // Replace the equivalent detail in updatedDetails with the new one
        QMutableListIterator<QContactDetail> it(*updatedDetails);
        while (it.hasNext()) {
            if (it.next().key() == detail.key()) {
                it.remove();
                break;
            }
        }
        updatedDetails->append(detail);
        success = true;
    }
    if (!group.isEmpty()) {
        // Keep track of which details were generated from which Versit groups
        foreach (const QContactDetail& detail, *updatedDetails) {
            mDetailGroupMap.insert(group, detail);
        }
    }
    return success;
}

QVariant QVCardRestoreHandler::deserializeValue(const QVersitProperty& property)
{
    // Import the field
    if (property.parameters().contains(DatatypeParameter, DatatypeParameterVariant)) {
        // The value was stored as a QVariant serialized in a QByteArray
        QDataStream stream(property.variantValue().toByteArray());
        QVariant value;
        stream >> value;
        return value;
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterDate)) {
        // The value was a QDate serialized as a string
        return QDate::fromString(property.value(), Qt::ISODate);
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterTime)) {
        // The value was a QTime serialized as a string
        return QTime::fromString(property.value(), Qt::ISODate);
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterDateTime)) {
        // The value was a QDateTime serialized as a string
        return QDateTime::fromString(property.value(), Qt::ISODate);
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterBool)) {
        // The value was a bool serialized as a string
        return property.value().toInt() != 0;
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterInt)) {
        // The value was an int serialized as a string
        return property.value().toInt();
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterUInt)) {
        // The value was a uint serialized as a string
        return property.value().toUInt();
    } else if (property.parameters().contains(DatatypeParameter, DatatypeParameterUrl)) {
        // The value was a QUrl serialized as a string
        return QUrl(property.value());
    } else {
        // The value was stored as a QString or QByteArray
        return property.variantValue();
    }
}

void QVCardRestoreHandler::documentProcessed()
{
    mDetailGroupMap.clear();
}

QT_END_NAMESPACE_VERSIT
