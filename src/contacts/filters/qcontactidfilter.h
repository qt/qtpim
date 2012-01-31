/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QCONTACTIDFILTER_H
#define QCONTACTIDFILTER_H

#include "qcontactfilter.h"
#include "qcontactid.h"

QTCONTACTS_BEGIN_NAMESPACE

class QContactIdFilterPrivate;
class Q_CONTACTS_EXPORT QContactIdFilter : public QContactFilter
{
public:
    QContactIdFilter();
    QContactIdFilter(const QContactFilter& other);

    /* Mutators */
    void setIds(const QList<QContactId>& ids);
    void add(const QContactId& id);
    void remove(const QContactId& id);
    void clear();

    /* Accessors */
    QList<QContactId> ids() const;

private:
    Q_DECLARE_CONTACTFILTER_PRIVATE(QContactIdFilter)
};

QTCONTACTS_END_NAMESPACE

#endif
