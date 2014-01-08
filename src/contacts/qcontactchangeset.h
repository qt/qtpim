/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTCHANGESET_H
#define QCONTACTCHANGESET_H

#include <QtCore/qlist.h>
#include <QtCore/qpair.h>
#include <QtCore/qset.h>
#include <QtCore/qshareddata.h>

#include <QtContacts/qcontactid.h>

QT_BEGIN_NAMESPACE_CONTACTS

class QContactManagerEngine;

class QContactChangeSetData;
class Q_CONTACTS_EXPORT QContactChangeSet
{
public:
    QContactChangeSet();
    QContactChangeSet(const QContactChangeSet& other);
    ~QContactChangeSet();

    QContactChangeSet& operator=(const QContactChangeSet& other);

    void setDataChanged(bool dataChanged);
    bool dataChanged();

    QSet<QContactId> addedContacts() const;
    void insertAddedContact(QContactId addedContactId);
    void insertAddedContacts(const QList<QContactId>& addedContactIds);
    void clearAddedContacts();

    QSet<QContactId> changedContacts() const;
    void insertChangedContact(QContactId addedContactId);
    void insertChangedContacts(const QList<QContactId>& addedContactIds);
    void clearChangedContacts();

    QSet<QContactId> removedContacts() const;
    void insertRemovedContact(QContactId addedContactId);
    void insertRemovedContacts(const QList<QContactId>& addedContactIds);
    void clearRemovedContacts();

    QSet<QContactId> addedRelationshipsContacts() const;
    void insertAddedRelationshipsContact(QContactId affectedContactId);
    void insertAddedRelationshipsContacts(const QList<QContactId>& affectedContactIds);
    void clearAddedRelationshipsContacts();

    QSet<QContactId> removedRelationshipsContacts() const;
    void insertRemovedRelationshipsContact(QContactId affectedContactId);
    void insertRemovedRelationshipsContacts(const QList<QContactId>& affectedContactIds);
    void clearRemovedRelationshipsContacts();

    void setOldAndNewSelfContactId(const QPair<QContactId, QContactId>& oldAndNewContactId);
    QPair<QContactId, QContactId> oldAndNewSelfContactId() const;

    void clearAll();

    void emitSignals(QContactManagerEngine *engine);

private:
    QSharedDataPointer<QContactChangeSetData> d;
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTCHANGESET_H
