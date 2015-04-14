/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactidfilter.h"
#include "qcontactidfilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactIdFilter
  \brief The QContactIdFilter class provides a filter based around a list of contact ids


  \inmodule QtContacts

  \ingroup contacts-filters

  It may be used to select contacts whose ids are contained in the given list of ids.
 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactIdFilter);

/*!
 * \fn QContactIdFilter::QContactIdFilter(const QContactFilter& other)
 * Constructs a copy of \a other if possible, otherwise constructs a new contact id filter
 */

/*!
 * Constructs a new contact id filter
 */
QContactIdFilter::QContactIdFilter()
    : QContactFilter(new QContactIdFilterPrivate)
{
}

/*!
 * Sets the list which contains the ids of possible matching contacts to \a ids
 * \sa ids()
 */
void QContactIdFilter::setIds(const QList<QContactId>& ids)
{
    Q_D(QContactIdFilter);
    d->m_ids = ids;
}

/*!
 * Adds the id \a id into the list which contains the ids of possible matching contacts
 * \sa setIds()
 */
void QContactIdFilter::add(const QContactId& id)
{
    Q_D(QContactIdFilter);
    if (!d->m_ids.contains(id))
        d->m_ids.append(id);
}

/*!
 * Removes the id \a id from the list which contains the ids of possible matching contacts,
 * if it is contained in the list, otherwise has no effect.
 * \sa clear()
 */
void QContactIdFilter::remove(const QContactId& id)
{
    Q_D(QContactIdFilter);
    d->m_ids.removeAll(id);
}

/*!
 * Clears the list which contains the ids of possible matching contacts.
 * An id filter with a cleared list will match no contacts.
 * \sa setIds()
 */
void QContactIdFilter::clear()
{
    Q_D(QContactIdFilter);
    d->m_ids.clear();
}

/*!
 * Returns the list of ids of contacts which match this filter
 * \sa setIds()
 */
QList<QContactId> QContactIdFilter::ids() const
{
    Q_D(const QContactIdFilter);
    return d->m_ids;
}

QT_END_NAMESPACE_CONTACTS
