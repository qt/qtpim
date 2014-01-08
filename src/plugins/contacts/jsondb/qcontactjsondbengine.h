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

#ifndef QCONTACTJSONDBENGINE_H
#define QCONTACTJSONDBENGINE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qqueue.h>
#include <QtCore/qvariant.h>

#include <QtContacts/qcontacts.h>
#include <QtContacts/qcontactmanager.h>
#include <QtContacts/private/qcontactmanager_p.h>

#include "qcontactjsondbstring.h"

QT_FORWARD_DECLARE_CLASS(QThread)

QT_BEGIN_NAMESPACE_CONTACTS

class QContactJsonDbRequestHandler;

class QContactJsonDbEngine : public QContactManagerEngine
{
    Q_OBJECT

public:
    ~QContactJsonDbEngine();
    QContactJsonDbEngine(const QMap<QString, QString> &parameters = (QMap<QString, QString>()));

    QString managerName() const;
    bool validateContact(const QContact&, QContactManager::Error* error) const;

    /* "Self" contact id (MyCard) */
    QContactId selfContactId(QContactManager::Error* errors) const;

    /* Filtering */
    QList<QContactId> contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const;

    /* Contacts - Accessors and Mutators */
    QContact contact(const QContactId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const;
    QList<QContact> contacts(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint& fetchHint, QContactManager::Error* error ) const;
    bool saveContact(QContact* contact, QContactManager::Error* error);
    bool removeContact(const QContactId& contactId, QContactManager::Error* error);
    bool saveContacts(QList<QContact>*, QMap<int, QContactManager::Error>*, QContactManager::Error* error); // implemented in terms of the singular saveContact
    bool removeContacts(const QList<QContactId>&, QMap<int, QContactManager::Error>*, QContactManager::Error* error); // implemented in terms of the singular removeContact

    /* Version Reporting */
    int implementationVersion() const { return QContactJsonDbStr::ContactJsonDbEngineVersion; }
    int managerVersion() const { return QContactJsonDbStr::ContactJsonDbEngineVersion; }

    bool isFilterSupported(const QContactFilter& filter) const;
    QList<QVariant::Type> supportedDataTypes() const;
    QList<QContactType::TypeValues>  supportedContactTypes() const {return (QList<QContactType::TypeValues> () << QContactType::TypeContact);}
    QList<QContactDetail::DetailType> supportedContactDetailTypes() const;

    /* Asynchronous Request Support - synchronous versions until thread worker is stable */
    void requestDestroyed(QContactAbstractRequest *req);
    bool startRequest(QContactAbstractRequest *req);
    bool cancelRequest(QContactAbstractRequest *req);
    bool waitForRequestProgress(QContactAbstractRequest* req, int msecs);
    bool waitForRequestFinished(QContactAbstractRequest* req, int msecs);
  signals:    
    void requestReceived(QContactAbstractRequest* req);
  private:
    bool doSyncRequest(QContactAbstractRequest* req, int msecs) const;

    Q_DISABLE_COPY(QContactJsonDbEngine);

    QQueue<QContactAbstractRequest*> m_asynchronousOperations; // async requests to be performed.
    QContactJsonDbRequestHandler *m_requestHandler;
    QThread* m_thread;
};

QT_END_NAMESPACE_CONTACTS

#endif // QCONTACTJSONDBENGINE_H
