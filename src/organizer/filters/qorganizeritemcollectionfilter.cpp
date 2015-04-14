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

#include "qorganizeritemcollectionfilter.h"
#include "qorganizeritemcollectionfilter_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemCollectionFilter
    \brief The QOrganizerItemCollectionFilter class provides a filter based around the collection one
           organizer item belongs to.
    \inmodule QtOrganizer
    \ingroup organizeritems-filters

    It may be used to select organizer items belonging to certain collections.
 */

Q_IMPLEMENT_ORGANIZERITEMFILTER_PRIVATE(QOrganizerItemCollectionFilter)

/*!
    \fn QOrganizerItemCollectionFilter::QOrganizerItemCollectionFilter(const QOrganizerItemFilter &other)

    Constructs a copy of \a other if possible, otherwise constructs a new organizeritem collection filter.
 */

/*!
    Constructs a new organizeritem collection filter.
 */
QOrganizerItemCollectionFilter::QOrganizerItemCollectionFilter()
    : QOrganizerItemFilter(new QOrganizerItemCollectionFilterPrivate)
{
}

/*!
    Sets the \a id of the collection, which the organizer items should belong to.
 */
void QOrganizerItemCollectionFilter::setCollectionId(const QOrganizerCollectionId &id)
{
    Q_D(QOrganizerItemCollectionFilter);
    d->m_ids.clear();
    d->m_ids.insert(id);
}

/*!
    Sets the list of collection \a ids, which the organizer items should belong to.
 */
void QOrganizerItemCollectionFilter::setCollectionIds(const QSet<QOrganizerCollectionId> &ids)
{
    Q_D(QOrganizerItemCollectionFilter);
    d->m_ids = ids;
}

/*!
    Returns the list of collection IDs of organizeritems should belong to.
 */
QSet<QOrganizerCollectionId> QOrganizerItemCollectionFilter::collectionIds() const
{
    Q_D(const QOrganizerItemCollectionFilter);
    return d->m_ids;
}

QT_END_NAMESPACE_ORGANIZER
