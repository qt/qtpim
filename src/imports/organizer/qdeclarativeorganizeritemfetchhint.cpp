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

#include "qdeclarativeorganizeritemfetchhint_p.h"

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype FetchHint
    \instantiates QDeclarativeOrganizerItemFetchHint
    \brief The FetchHint element provides hints to the manager about which organizer item
           information needs to be retrieved in an asynchronous fetch request or a synchronous
           function call.
    \inqmlmodule QtOrganizer 5.0
    \ingroup qml-organizer-filters
 */

/*!
  \qmlsignal FetchHint::onFetchHintChanged()

  This signal is emitted, when any of the FetchHint's properties have been changed.
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
    \li AllRequired          Tells the backend that all information is required.
    \li NoActionPreferences  Tells the backend that the client does not require retrieved
                            organizer items to include a cache of action preferences.
    \li NoBinaryBlobs        Tells the backend that the client does not require retrieved
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

QT_END_NAMESPACE
