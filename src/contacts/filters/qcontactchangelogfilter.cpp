/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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
