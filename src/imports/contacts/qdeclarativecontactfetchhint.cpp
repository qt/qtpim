/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#include "qdeclarativecontactfetchhint_p.h"

#include <QtCore/qset.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
   \qmltype FetchHint
    \instantiates QDeclarativeContactFetchHint
   \brief The FetchHint element provides hints to the manager about which contact
  information needs to be retrieved in an asynchronous fetch request or a synchronous
  function call.

   \ingroup qml-contacts-main
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

   \sa QContactFetchHint
 */

QDeclarativeContactFetchHint::QDeclarativeContactFetchHint(QObject* parent)
    :QObject(parent)
{
}

/*!
  \qmlproperty list<int> FetchHint::detailTypesHint

  This property holds a list of contact detail types
  the manager should (at a minimum) retrieve when fetching contacts.
  */
QList<int> QDeclarativeContactFetchHint::detailTypesHint() const
{
    QList<int> savedList;
    foreach (const QContactDetail::DetailType &detailTypeHint, m_fetchHint.detailTypesHint()) {
        savedList << static_cast<int>(detailTypeHint);
    }

    return savedList;
}
void QDeclarativeContactFetchHint::setDetailTypesHint(const QList<int> &detailTypes)
{
    if (detailTypes.toSet() != detailTypesHint().toSet()) {
        QList<QContactDetail::DetailType> convertedDetailTypes;
        foreach (const int detailType, detailTypes) {
            convertedDetailTypes << static_cast<QContactDetail::DetailType>(detailType);
        }
        m_fetchHint.setDetailTypesHint(convertedDetailTypes);
        emit fetchHintChanged();
    }
}

/*!
  \qmlproperty list<string> FetchHint::relationshipTypesHint

  This property holds a list of relationship types that the manager should (at a minimum) retrieve
  when fetching contacts.
  */
QStringList QDeclarativeContactFetchHint::relationshipTypesHint() const
{
    return m_fetchHint.relationshipTypesHint();
}
void QDeclarativeContactFetchHint::setRelationshipTypesHint(const QStringList& relationshipTypes)
{
    if (relationshipTypes.toSet() != m_fetchHint.relationshipTypesHint().toSet()) {
        m_fetchHint.setRelationshipTypesHint(relationshipTypes);
        emit fetchHintChanged();
    }
}


/*!
  \qmlproperty int FetchHint::imageWidth

  This property holds the preferred pixel width for any images returned
  by the manager for a given request.  This hint may be ignored by the manager.
  */
int QDeclarativeContactFetchHint::preferredImageWidth() const
{
    return m_fetchHint.preferredImageSize().width();
}
void QDeclarativeContactFetchHint::setPreferredImageWidth(int w)
{
    if (m_fetchHint.preferredImageSize().width() != w) {
        QSize s = m_fetchHint.preferredImageSize();
        s.setWidth(w);
        m_fetchHint.setPreferredImageSize(s);
        emit fetchHintChanged();
    }
}

/*!
  \qmlproperty int FetchHint::imageHeight

  This property holds the preferred pixel height for any images returned
  by the manager for a given request.  This hint may be ignored by the manager.
  */
int QDeclarativeContactFetchHint::preferredImageHeight() const
{
    return m_fetchHint.preferredImageSize().height();
}
void QDeclarativeContactFetchHint::setPreferredImageHeight(int h)
{
    if (m_fetchHint.preferredImageSize().height() != h) {
        QSize s = m_fetchHint.preferredImageSize();
        s.setHeight(h);
        m_fetchHint.setPreferredImageSize(s);
        emit fetchHintChanged();
    }
}

/*!
  \qmlproperty FetchHint::OptimizationHints  FetchHint::optimizationHints

  This property holds the optimization hint flags specified by the client.
  These hints may be ignored by the backend, in which case it will return
  the full set of information accessible in a contact, including
  relationships, action preferences, and binary blobs. The value of the flags can be:
  \list
  \li FetchHint.AllRequired - (default).
  \li FetchHint.NoRelationships
  \li FetchHint.NoActionPreferences
  \li FetchHint.NoBinaryBlobs
  \endlist

  */
QDeclarativeContactFetchHint::OptimizationHints QDeclarativeContactFetchHint::optimizationHints() const
{
    QDeclarativeContactFetchHint::OptimizationHints hints;
    hints = ~hints & (int)m_fetchHint.optimizationHints();
    return hints;
}
void QDeclarativeContactFetchHint::setOptimizationHints(QDeclarativeContactFetchHint::OptimizationHints hints)
{
    QContactFetchHint::OptimizationHints newHints;
    newHints = ~newHints & (int)hints;
    if (newHints != m_fetchHint.optimizationHints()) {
        m_fetchHint.setOptimizationHints(newHints);
        emit fetchHintChanged();
    }
}

QContactFetchHint QDeclarativeContactFetchHint::fetchHint() const
{
    return m_fetchHint;
}

void QDeclarativeContactFetchHint::setFetchHint(const QContactFetchHint& fetchHint)
{
    m_fetchHint = fetchHint;
    emit fetchHintChanged();
}

QT_END_NAMESPACE

#include "moc_qdeclarativecontactfetchhint_p.cpp"
