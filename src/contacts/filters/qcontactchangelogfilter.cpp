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

#include "qcontactchangelogfilter.h"
#include "qcontactchangelogfilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactChangeLogFilter
  \brief The QContactChangeLogFilter class provides a filter based around a
  contact timestamp criterion.

  \inmodule QtContacts

   \ingroup contacts-filters

  It may be used to select contacts which have been updated or created
  within a certain period of time.
 */

/*!
 * \enum QContactChangeLogFilter::EventType
 * Enumerates the type of changes which a changelog filter can match against
 * \value EventAdded
 * \value EventChanged
 * \value EventRemoved
 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactChangeLogFilter);

/*!
 * \fn QContactChangeLogFilter::QContactChangeLogFilter(const QContactFilter& other)
 * Constructs a copy of \a other if possible, otherwise constructs a new changelog filter
 */

/*!
 * Constructs a new changelog filter which matches changes of the specified \a type
 */
QContactChangeLogFilter::QContactChangeLogFilter(QContactChangeLogFilter::EventType type)
    : QContactFilter(new QContactChangeLogFilterPrivate(type))
{
}

/*!
 * Sets the type of change that this filter will match against to \a type
 * \sa eventType()
 */
void QContactChangeLogFilter::setEventType(QContactChangeLogFilter::EventType type)
{
    Q_D(QContactChangeLogFilter);
    d->m_eventType = type;
}

/*!
 * Sets the date and time lower-bound criterion of the filter to \a since
 * \sa since()
 */
void QContactChangeLogFilter::setSince(const QDateTime& since)
{
    Q_D(QContactChangeLogFilter);
    d->m_since = since;
}

/*!
 * Returns the date and time lower-bound criterion of the filter
 * \sa setSince()
 */
QDateTime QContactChangeLogFilter::since() const
{
    Q_D(const QContactChangeLogFilter);
    return d->m_since;
}

/*!
 * Returns the type of change that this filter will match against
 * \sa setEventType()
 */
QContactChangeLogFilter::EventType QContactChangeLogFilter::eventType() const
{
    Q_D(const QContactChangeLogFilter);
    return d->m_eventType;
}

QT_END_NAMESPACE_CONTACTS
