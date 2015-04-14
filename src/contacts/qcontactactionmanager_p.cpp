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

#include "qcontactactionmanager_p.h"

#include "qcontactaction.h"
#include "qcontactactionfactory.h"
#include "qcontactmanager_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

Q_GLOBAL_STATIC(QContactActionManager, contactActionManagerInstance)

/*!
  \internal
  \class QContactActionManager
  This class uses a plugin to delegate discovery of actions (to avoid a dependency on SFW for QtContacts)
  It is an implementation detail of QContactAction.
 */

QContactActionManager* QContactActionManager::instance()
{
    return contactActionManagerInstance();
}

QContactActionManager::QContactActionManager()
    : QObject(),
    m_plugin(0)
{
}

void QContactActionManager::init()
{
    // We ask the qcontactmanager engine loading code, since it has to enumerate things anyway
    QContactManagerData::loadFactoriesMetadata();
    m_plugin = QContactManagerData::m_actionManagers.value(0);
}

QContactActionManager::~QContactActionManager()
{
    // Allow our subordinates to do their thing when they want,
    // since we just cache stuff
}

QList<QContactActionDescriptor> QContactActionManager::availableActions(const QContact &contact)
{
    QMutexLocker locker(&m_instanceMutex);
    init();

    QList<QContactActionDescriptor> ret;
    if (m_plugin) {
        QHash<QContactActionDescriptor, QContactActionFactory*> hash = m_plugin->actionFactoryHash();
        QHash<QContactActionDescriptor, QContactActionFactory*>::const_iterator it;
        for (it = hash.constBegin(); it != hash.constEnd(); ++it) {
            QContactActionFactory* curr = it.value();
            if (curr && curr->supportsContact(contact, it.key())) {
                ret.append(it.key());
            }
        }
    }

    return ret;
}

QList<QContactActionDescriptor> QContactActionManager::actionDescriptors(const QString& actionName)
{
    QMutexLocker locker(&m_instanceMutex);
    init();

    if (m_plugin) {
        if (actionName.isEmpty())
            return m_plugin->descriptorHash().values();
        return m_plugin->descriptorHash().values(actionName);
    }
    return QList<QContactActionDescriptor>();
}

/*! The caller takes ownership of the returned action pointer, and must delete it to avoid leaking memory. */
QContactAction* QContactActionManager::action(const QContactActionDescriptor& descriptor)
{
    QMutexLocker locker(&m_instanceMutex);
    init();

    if (m_plugin) {
        QContactActionFactory *factory = m_plugin->actionFactoryHash().value(descriptor);
        if (factory)
            return factory->create(descriptor);
    }
    return 0;
}

#include "moc_qcontactactionmanager_p.cpp"

QT_END_NAMESPACE_CONTACTS
