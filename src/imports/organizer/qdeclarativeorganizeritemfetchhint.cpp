/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

#include "qdeclarativeorganizeritemfetchhint_p.h"

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \qmlclass FetchHint QDeclarativeOrganizerItemFetchHint
    \brief The FetchHint element provides hints to the manager about which organizer item
           information needs to be retrieved in an asynchronous fetch request or a synchronous
           function call.
    \inqmlmodule QtOrganizer
    \ingroup qml-organizer-filters
 */

/*!
    \internal
 */
QDeclarativeOrganizerItemFetchHint::QDeclarativeOrganizerItemFetchHint(QObject *parent)
    : QObject(parent)
{
}

/*!
    \qmlproperty enumeration FetchHint::optimizationHints

    This property holds the optimization hint flags specified by the client.

    \list
    \o AllRequired          Tells the backend that all information is required.
    \o NoActionPreferences  Tells the backend that the client does not require retrieved
                            organizer items to include a cache of action preferences.
    \o NoBinaryBlobs        Tells the backend that the client does not require retrieved
                            organizer items to include binary blobs such as thumbnail images.
    \endlist
 */
QDeclarativeOrganizerItemFetchHint::OptimizationHints QDeclarativeOrganizerItemFetchHint::optimizationHints() const
{
    OptimizationHints hints;
    hints = ~hints & (int)d.optimizationHints();
    return hints;
}

void QDeclarativeOrganizerItemFetchHint::setOptimizationHints(OptimizationHints hints)
{
    if (hints != d.optimizationHints()) {
        QOrganizerItemFetchHint::OptimizationHints newHints;
        newHints = ~newHints & (int)hints;
        d.setOptimizationHints(newHints);
        emit fetchHintChanged();
    }
}

/*!
    \internal
 */
QOrganizerItemFetchHint QDeclarativeOrganizerItemFetchHint::fetchHint() const
{
    return d;
}

/*!
    \internal
 */
void QDeclarativeOrganizerItemFetchHint::setFetchHint(const QOrganizerItemFetchHint &fetchHint)
{
    d = fetchHint;
    emit fetchHintChanged();
}

#include "moc_qdeclarativeorganizeritemfetchhint_p.cpp"

QTORGANIZER_END_NAMESPACE
