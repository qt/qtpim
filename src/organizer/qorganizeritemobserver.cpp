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

#include "qorganizeritemobserver.h"

#include <QtCore/qpointer.h>

#include "qorganizermanager.h"
#include "qorganizermanager_p.h"

QT_BEGIN_NAMESPACE_ORGANIZER

class QOrganizerItemObserverPrivate
{
public:
    QOrganizerItemId m_id;
    QPointer<QOrganizerManager> m_manager;
    QOrganizerManagerData *m_managerPrivate;
};

/*!
    \class QOrganizerItemObserver
    \brief The QOrganizerItemObserver class is a simple class that emits a signal when a single
           particular item is updated or deleted.
    \inmodule QtOrganizer
    \ingroup organizer-main
 */

/*!
    Constructs a QOrganizerItemObserver to observe the item in \a manager with the given \a itemId
    and a \a parent object.
 */
QOrganizerItemObserver::QOrganizerItemObserver(QOrganizerManager *manager, const QOrganizerItemId &itemId, QObject *parent)
    : QObject(parent), d(new QOrganizerItemObserverPrivate)
{
    d->m_id = itemId;
    d->m_manager = manager;
    d->m_managerPrivate = QOrganizerManagerData::get(manager);
    d->m_managerPrivate->registerObserver(this);
}

/*!
    Destroys this observer.
 */
QOrganizerItemObserver::~QOrganizerItemObserver()
{
    if (d->m_manager.data())
        d->m_managerPrivate->unregisterObserver(this);
    delete d;
}

/*!
    Returns the ID of the item that this object observes.
 */
QOrganizerItemId QOrganizerItemObserver::itemId() const
{
    return d->m_id;
}

/*!
    \fn void QOrganizerItemObserver::itemChanged(const QList<QOrganizerItemDetail::DetailType> &typesChanged)

    This signal is emitted when the observed item is changed in the manager.

    The set of item detail types modified in the reported change is a subset of those listed in \a typesChanged,
    unless \a typesChanged is empty, in which case no limitation on the reported changes may be assumed.
 */

/*!
    \fn void QOrganizerItemObserver::itemRemoved()

    This signal is emitted when the observed item is removed from the manager.
 */

#include "moc_qorganizeritemobserver.cpp"

QT_END_NAMESPACE_ORGANIZER
