/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QTORGANIZER_H
#define QTORGANIZER_H

// this file includes all of the public header files
// provided by the Qt Organizer API

#include <QtOrganizer/qorganizerglobal.h>                        // global exports

#include <QtOrganizer/qorganizerabstractrequest.h>                // asynchronous request
#include <QtOrganizer/qorganizeritemdetail.h>                     // organizer item detail
#include <QtOrganizer/qorganizeritemfetchhint.h>                  // fetch hint class
#include <QtOrganizer/qorganizeritemfilter.h>                     // organizer item filter
#include <QtOrganizer/qorganizeritem.h>                           // organizer item
#include <QtOrganizer/qorganizeritemid.h>                         // organizer item identifier
#include <QtOrganizer/qorganizeritemobserver.h>                   // organizer item observer
#include <QtOrganizer/qorganizermanager.h>                        // manager
#include <QtOrganizer/qorganizermanagerengine.h>                  // manager backend
#include <QtOrganizer/qorganizermanagerenginefactory.h>           // manage backend instantiator
#include <QtOrganizer/qorganizerrecurrencerule.h>                 // a single recurrence rule
#include <QtOrganizer/qorganizeritemsortorder.h>                  // organizer item sorting

#include <QtOrganizer/qorganizercollection.h>                     // collection of items
#include <QtOrganizer/qorganizercollectionid.h>                   // collection identifier

#include <QtOrganizer/qorganizercollectionchangeset.h>            // engine-specific collection changeset
#include <QtOrganizer/qorganizeritemchangeset.h>                  // engine-specific item changeset

#include <QtOrganizer/qorganizeritems.h>                          // item derived classes
#include <QtOrganizer/qorganizeritemdetails.h>                    // detail derived classes
#include <QtOrganizer/qorganizeritemrequests.h>                   // request derived classes
#include <QtOrganizer/qorganizeritemfilters.h>                    // detail derived classes

QT_BEGIN_NAMESPACE_ORGANIZER
QT_END_NAMESPACE_ORGANIZER

#endif // QTORGANIZER_H
