/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTMANAGERDATAHOLDER_H
#define QCONTACTMANAGERDATAHOLDER_H

#include <QtCore/qdebug.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qstring.h>

#include <QtContacts/qcontactmanager.h>

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

QT_BEGIN_NAMESPACE_CONTACTS
class QContact;
QT_END_NAMESPACE_CONTACTS

QTCONTACTS_USE_NAMESPACE

class QContactManagerDataHolder
{
public:
    QContactManagerDataHolder()
    {
        QStringList managerNames = QContactManager::availableManagers();

        foreach(const QString& mgr, managerNames) {
            // Don't bother with these
            if (mgr == "memory" || mgr == "invalid" || mgr == "testdummy" || mgr == "maliciousplugin" || mgr == "testlazy")
                continue;
            QMap<QString, QString> params;
            QString mgrUri = QContactManager::buildUri(mgr, params);
            QContactManager* cm = QContactManager::fromUri(mgrUri);
            if (cm) {
                qDebug() << "Saving contacts for" << mgrUri;
                QList<QContact> contacts = cm->contacts();
                savedContacts.insert(cm->managerName(),contacts);
                QList<QContactId> ids;
                foreach(const QContact& contact, contacts)
                    ids.append(contact.id());
                cm->removeContacts(ids, 0);
                delete cm;
            }
        }
    }

    ~QContactManagerDataHolder()
    {
        foreach(const QString& mgrUri, savedContacts.keys()) {
            QContactManager* cm = QContactManager::fromUri(mgrUri);
            if (cm) {
                qDebug() << "Restoring contacts for" << mgrUri;
                QList<QContact> contacts = savedContacts.value(mgrUri);
                cm->saveContacts(&contacts, 0);
                // XXX this doesn't restore relationships..
                delete cm;
            }
        }
    }
private:
    QMap<QString, QList<QContact> > savedContacts;
};

#endif // QCONTACTMANAGERDATAHOLDER_H
