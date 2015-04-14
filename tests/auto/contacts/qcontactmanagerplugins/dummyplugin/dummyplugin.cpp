/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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


/* We build multiple plugins with the same code, just different names */
#ifndef DUMMYPLUGINTARGET
#define DUMMYPLUGINTARGET contacts_testdummy
#endif

#define makestr(x) (#x)
#define makename(x) makestr(x)

#include <QtContacts/qcontactmanager.h>
#include <QtContacts/qcontactmanagerengine.h>
#include <QtContacts/qcontactmanagerenginefactory.h>

#include "dummyplugin.h"

QTCONTACTS_USE_NAMESPACE

QContactManagerEngine* DummyEngineFactory::engine(const QMap<QString, QString>& parameters, QContactManager::Error* error)
{
    return new DummyEngine(parameters, error);
}

QString DummyEngineFactory::managerName() const
{
#ifdef DUMMYPLUGINNAME
    return QString(makename(DUMMYPLUGINNAME));
#else
    return QString();
#endif
}

DummyEngine::DummyEngine(const QMap<QString, QString>& parameters, QContactManager::Error* error)
{
    Q_UNUSED(parameters);
    *error = QContactManager::AlreadyExistsError; // Another random choice
}

DummyEngine::DummyEngine(const DummyEngine& other)
    : QContactManagerEngine()
{
    Q_UNUSED(other);
}

DummyEngine& DummyEngine::operator=(const DummyEngine& other)
{
    Q_UNUSED(other);
    return *this;
}

DummyEngine::~DummyEngine()
{
}

QContactManagerEngine* DummyEngine::clone()
{
    // Disallow sharing
    return new DummyEngine(*this);
}

void DummyEngine::deref()
{
    delete this;
}

QString DummyEngine::managerName() const
{
#ifdef DUMMYPLUGINNAME
    return QString(makename(DUMMYPLUGINNAME));
#else
    return QString();
#endif
}

QList<QContactId> DummyEngine::contacts(QContactManager::Error* error) const
{
    QList<QContactId> allCIds;

    if (allCIds.count() > 0 && *error == QContactManager::NoError)
        *error = QContactManager::DoesNotExistError;

    return allCIds;
}

QContact DummyEngine::contact(const QContactId& contactId, QContactManager::Error* error) const
{
    Q_UNUSED(contactId);
    *error = QContactManager::DoesNotExistError;
    return QContact();
}

QContact DummyEngine::contact(const QContactId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    Q_UNUSED(contactId);
    Q_UNUSED(fetchHint);
    *error = QContactManager::DoesNotExistError;
    return QContact();
}

bool DummyEngine::saveContact(QContact* contact, bool batch, QContactManager::Error* error)
{
    // ensure that the contact's details conform to their definitions
    if (!validateContact(*contact, error)) {
        *error = QContactManager::InvalidDetailError;
        return false;
    }

    // success!
    QContactId newId;
    contact->setId(newId);
    *error = QContactManager::NoError;

    // if we need to emit signals (ie, this isn't part of a batch operation)
    // then emit the correct one.
    if (!batch) {
        QList<QContactId> emitList;
        emitList.append(contact->id());
        emit contactsAdded(emitList);
    }

    return true;
}

bool DummyEngine::removeContact(const QContactId& contactId, bool batch, QContactManager::Error* error)
{
    /* TODO: fix this
    if (contactId != "5") {
        *error = QContactManager::DoesNotExistError;
        return false;
    }*/

    *error = QContactManager::NoError;

    // if we need to emit signals (ie, this isn't part of a batch operation)
    // then emit the correct one.
    if (!batch) {
        QList<QContactId> emitList;
        emitList.append(contactId);
        emit contactsRemoved(emitList);
    }

    return true;
}

/*! Returns the capabilities of the in-memory engine. */
QStringList DummyEngine::capabilities() const
{
    QStringList caplist;
    caplist << "Locking" << "Batch" << "ReadOnly" << "Filtering" << "Sorting" << "Preferences";
    // ie, doesn't support: Changelog, Volatile, Asynchronous.
    return caplist;
}

/*!
 * Returns a list of definition identifiers which are natively (fast) filterable
 * on the default backend store managed by the manager from which the capabilities object was accessed
 */
QStringList DummyEngine::fastFilterableDefinitions() const
{
    QStringList fastlist;
    fastlist << "Name::First" << "Name::Last" << "PhoneNumber::PhoneNumber" << "EmailAddress::EmailAddress";
    return fastlist;
}

/*!
 * Returns the list of data types supported by the vCard engine
 */
QList<QVariant::Type> DummyEngine::supportedDataTypes() const
{
    QList<QVariant::Type> st;
    st.append(QVariant::String);
    st.append(QVariant::Date);
    st.append(QVariant::DateTime);

    return st;
}

#include "moc_dummyplugin.cpp"
