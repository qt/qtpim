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

#include "qcontactobserver.h"

#include <QtCore/qpointer.h>

#include "qcontactmanager.h"
#include "qcontactmanager_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

class QContactObserverPrivate
{
    public:
        QContactId m_contactId;
        QPointer<QContactManager> m_manager;
};

/*!
  \class QContactObserver
  \brief The QContactObserver class is a simple class that emits a signal when a single particular
  contact is updated or deleted.
  \inmodule QtContacts

  \ingroup contacts-main
 */

/*!
  Constructs a QContactObserver to observe the contact in \a manager with the
  given \a contactId and \a parent object.
 */
QContactObserver::QContactObserver(QContactManager* manager,
                                   QContactId contactId,
                                   QObject* parent)
    : QObject(parent),
      d(new QContactObserverPrivate)
{
    d->m_contactId = contactId;
    d->m_manager = manager;
    QContactManagerData::registerObserver(manager, this);
}

/*!
  Destroys this observer.
 */
QContactObserver::~QContactObserver()
{
    if (!d->m_manager.isNull()) {
        QContactManagerData::unregisterObserver(d->m_manager.data(), this);
    }
    delete d;
}

/*!
  Returns the contact id of the contact that this object observes.
 */
QContactId QContactObserver::contactId() const {
    return d->m_contactId;
}

/*!
  \fn QContactObserver::contactChanged(const QList<QContactDetail::DetailType> &typesChanged)

  This signal is emitted when the observed contact is changed in the manager.

  The set of contact detail types modified in the reported change is a subset of those listed in \a typesChanged,
  unless \a typesChanged is empty, in which case no limitation on the reported changes may be assumed.
 */

/*!
  \fn QContactObserver::contactRemoved()

  This signal is emitted when the observed contact is removed from the manager.
 */

#include "moc_qcontactobserver.cpp"

QT_END_NAMESPACE_CONTACTS
