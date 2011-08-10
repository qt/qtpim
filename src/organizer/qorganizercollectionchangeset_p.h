/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QORGANIZERCOLLECTIONCHANGESET_P_H
#define QORGANIZERCOLLECTIONCHANGESET_P_H

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

#include "qtorganizerglobal.h"
#include <QSharedData>
#include <QSet>
#include <QPair>

QTPIM_BEGIN_NAMESPACE

class QOrganizerCollectionChangeSetData : public QSharedData
{
public:
    QOrganizerCollectionChangeSetData()
        : QSharedData(),
        m_dataChanged(false)
    {
    }

    QOrganizerCollectionChangeSetData(const QOrganizerCollectionChangeSetData& other)
        : QSharedData(other),
        m_dataChanged(other.m_dataChanged),
        m_addedCollections(other.m_addedCollections),
        m_changedCollections(other.m_changedCollections),
        m_removedCollections(other.m_removedCollections)
    {
    }

    ~QOrganizerCollectionChangeSetData()
    {
    }

    bool m_dataChanged;
    QSet<QOrganizerCollectionId> m_addedCollections;
    QSet<QOrganizerCollectionId> m_changedCollections;
    QSet<QOrganizerCollectionId> m_removedCollections;
};

QTPIM_END_NAMESPACE

#endif