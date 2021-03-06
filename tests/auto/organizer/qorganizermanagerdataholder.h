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


#ifndef QORGANIZERITEMMANAGERDATAHOLDER_H
#define QORGANIZERITEMMANAGERDATAHOLDER_H

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

#include <QtCore/qdebug.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>
#include <QtCore/qstring.h>

#include <QtOrganizer/qorganizermanager.h>

QTORGANIZER_USE_NAMESPACE

class QOrganizerManagerDataHolder
{
public:
    QOrganizerManagerDataHolder()
    {
        QStringList managerNames = QOrganizerManager::availableManagers();

        foreach(const QString& mgr, managerNames) {
            // Don't bother with these
            if (mgr == "memory" || mgr == "invalid" || mgr == "testdummy" || mgr == "maliciousplugin" || mgr == "skeleton")
                continue;
            QMap<QString, QString> params;
            QString mgrUri = QOrganizerManager::buildUri(mgr, params);
            QOrganizerManager* cm = QOrganizerManager::fromUri(mgrUri);
            if (cm) {
                qDebug() << "Saving items for" << mgrUri;
                QList<QOrganizerItem> items = cm->itemsForExport(QDateTime(), QDateTime(), QOrganizerItemFilter());
                savedItems.insert(cm->managerName(),items);
                QList<QOrganizerItemId> ids;
                foreach(const QOrganizerItem& item, items)
                    ids.append(item.id());
                cm->removeItems(ids);
                delete cm;
            }
        }
    }

    ~QOrganizerManagerDataHolder()
    {
        foreach(const QString& mgrUri, savedItems.keys()) {
            QOrganizerManager* cm = QOrganizerManager::fromUri(mgrUri);
            if (cm) {
                qDebug() << "Restoring items for" << mgrUri;
                QList<QOrganizerItem> items = savedItems.value(mgrUri);
                cm->saveItems(&items);
                // XXX this doesn't restore relationships..
                delete cm;
            }
        }
    }
private:
    QMap<QString, QList<QOrganizerItem> > savedItems;
};

#endif

