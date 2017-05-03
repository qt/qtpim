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

#include "qcontactactionfilter.h"
#include "qcontactactionfilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactActionFilter
  \brief The QContactActionFilter class provides a filter based around an
  action availability criterion.


  \inmodule QtContacts

  \ingroup contacts-filters

  It may be used to select contacts for which a particular action is
  available, or contacts which contain a detail of a particular value for
  which the action is available.
 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactActionFilter);

/*!
 * \fn QContactActionFilter::QContactActionFilter(const QContactFilter& other)
 * Constructs a copy of \a other if possible, otherwise constructs a new action filter
 */

/*!
 * Constructs a new action filter
 */
QContactActionFilter::QContactActionFilter()
    : QContactFilter(new QContactActionFilterPrivate)
{
}

/*!
 * Sets the name of the action whose availability is required to \a action
 * \sa actionName()
 */
void QContactActionFilter::setActionName(const QString& action)
{
    Q_D(QContactActionFilter);
    d->m_action = action;
}

/*!
 * Returns the action name criterion of the filter
 * \sa setActionName()
 */
QString QContactActionFilter::actionName() const
{
    Q_D(const QContactActionFilter);
    return d->m_action;
}

QT_END_NAMESPACE_CONTACTS
