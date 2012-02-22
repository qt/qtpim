/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qorganizeritemremoverequest.h>
#include <private/qorganizeritemrequests_p.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerItemRemoveRequest
    \brief The QOrganizerItemRemoveRequest class allows a client to asynchronously request that certain
           organizer items be removed from a backend.
    \inmodule QtOrganizer
    \ingroup organizer-requests

    This request will remove the items and all the occurrences (both generated and persisted) of the
    given items.
 */

/*!
    Constructs a new organizer item remove request whose parent is the specified \a parent.
*/
QOrganizerItemRemoveRequest::QOrganizerItemRemoveRequest(QObject *parent)
    : QOrganizerAbstractRequest(new QOrganizerItemRemoveRequestPrivate, parent)
{
}

/*!
    Frees memory in use by this request.
*/
QOrganizerItemRemoveRequest::~QOrganizerItemRemoveRequest()
{
    QOrganizerAbstractRequestPrivate::notifyEngine(this);
}

/*!
  Sets the organizer item which will be removed to \a item.
  Equivalent to calling:
  \code
      setOrganizerItems(QList<QOrganizerItem>() << item);
  \endcode
 */
void QOrganizerItemRemoveRequest::setItem(const QOrganizerItem &item)
{
    Q_D(QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_organizeritems.clear();
    d->m_organizeritems.append(item);
}

/*! Sets the organizer items which will be removed to \a items
*/
void QOrganizerItemRemoveRequest::setItems(const QList<QOrganizerItem> &items)
{
    Q_D(QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    d->m_organizeritems = items;
}

/*!
    Returns the list of IDs of organizer items which will be removed.
*/
QList<QOrganizerItem> QOrganizerItemRemoveRequest::items() const
{
    Q_D(const QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_organizeritems;
}

/*!
    Returns the map of input organizer item list indices to errors which occurred.
*/
QMap<int, QOrganizerManager::Error> QOrganizerItemRemoveRequest::errorMap() const
{
    Q_D(const QOrganizerItemRemoveRequest);
    QMutexLocker ml(&d->m_mutex);
    return d->m_errors;
}

#include "moc_qorganizeritemremoverequest.cpp"

QTORGANIZER_END_NAMESPACE
