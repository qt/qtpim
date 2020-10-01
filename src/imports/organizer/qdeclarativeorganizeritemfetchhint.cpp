/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativeorganizeritemfetchhint_p.h"

#include <QtCore/qset.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
    \qmltype FetchHint
    \instantiates QDeclarativeOrganizerItemFetchHint
    \brief The FetchHint element provides hints to the manager about which organizer item
           information needs to be retrieved in an asynchronous fetch request or a synchronous
           function call.
    \inqmlmodule QtOrganizer
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
  \qmlproperty list<int> FetchHint::detailTypesHint

  This property holds a list of organizer item detail types
  the manager should (at a minimum) retrieve when fetching contacts.
  */
QList<int> QDeclarativeOrganizerItemFetchHint::detailTypesHint() const
{
    QList<int> savedList;
    foreach (const QOrganizerItemDetail::DetailType &detailTypeHint, d.detailTypesHint())
        savedList << static_cast<int>(detailTypeHint);

    return savedList;
}

void QDeclarativeOrganizerItemFetchHint::setDetailTypesHint(const QList<int> &detailTypes)
{
    if (QSet<int>(detailTypes.constBegin(), detailTypes.constEnd()) != QSet<int>(detailTypesHint().constBegin(), detailTypesHint().constEnd())) {
        QList<QOrganizerItemDetail::DetailType> convertedDetailTypes;
        foreach (const int detailType, detailTypes)
            convertedDetailTypes << static_cast<QOrganizerItemDetail::DetailType>(detailType);

        d.setDetailTypesHint(convertedDetailTypes);
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

QT_END_NAMESPACE

#include "moc_qdeclarativeorganizeritemfetchhint_p.cpp"
