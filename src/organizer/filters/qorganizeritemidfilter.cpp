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

#include "qorganizeritemidfilter.h"
#include "qorganizeritemidfilter_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

/*!
    \class QOrganizerItemIdFilter
    \brief The QOrganizerItemIdFilter class provides a filter based around a list of organizer item IDs.
    \inmodule QtOrganizer
    \ingroup organizer-filters

    It may be used to select organizer items whose IDs are contained in the given list.

    Note: a QOrganizerItemIdFilter will not be preserved if streamed to a QDataStream.
 */

Q_IMPLEMENT_ORGANIZERITEMFILTER_PRIVATE(QOrganizerItemIdFilter)

/*!
    \fn QOrganizerItemIdFilter::QOrganizerItemIdFilter(const QOrganizerItemFilter &other)

    Constructs a copy of \a other if possible, otherwise constructs a new organizer item ID filter.
 */

/*!
    Constructs a new organizer item ID filter.
 */
QOrganizerItemIdFilter::QOrganizerItemIdFilter()
    : QOrganizerItemFilter(new QOrganizerItemIdFilterPrivate)
{
}

/*!
    Sets the list which contains the IDs of possible matching organizer items to \a ids.

    \sa ids()
 */
void QOrganizerItemIdFilter::setIds(const QList<QOrganizerItemId> &ids)
{
    Q_D(QOrganizerItemIdFilter);
    d->m_ids = ids;
}

/*!
    Inserts the \a id into the list which contains the IDs of possible matching items.

    \sa setIds()
 */
void QOrganizerItemIdFilter::insert(const QOrganizerItemId &id)
{
    Q_D(QOrganizerItemIdFilter);
    if (!d->m_ids.contains(id))
        d->m_ids.append(id);
}

/*!
    Removes the id \a id from the list which contains the IDs of possible matching items.

    \sa clear()
 */
void QOrganizerItemIdFilter::remove(const QOrganizerItemId &id)
{
    Q_D(QOrganizerItemIdFilter);
    d->m_ids.removeAll(id);
}

/*!
    Clears the list which contains the IDs of possible matching items. Note that an item ID filter
    with an empty list will match no items.

    \sa setIds()
 */
void QOrganizerItemIdFilter::clear()
{
    Q_D(QOrganizerItemIdFilter);
    d->m_ids.clear();
}

/*!
    Returns the list of IDs of organizer items which match this filter.

    \sa setIds()
 */
QList<QOrganizerItemId> QOrganizerItemIdFilter::ids() const
{
    Q_D(const QOrganizerItemIdFilter);
    return d->m_ids;
}

QT_END_NAMESPACE_ORGANIZER
