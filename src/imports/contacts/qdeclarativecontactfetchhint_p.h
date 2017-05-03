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

#ifndef QDECLARATIVECONTACTFETCHHINT_H
#define QDECLARATIVECONTACTFETCHHINT_H

#include <QtCore/qsize.h>
#include <QtCore/qstringlist.h>
#include <QtCore/qvariant.h>

#include <QtQml/qqml.h>

#include <QtContacts/qcontactfetchhint.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactFetchHint : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> detailTypesHint READ detailTypesHint WRITE setDetailTypesHint NOTIFY fetchHintChanged )
    Q_PROPERTY(QStringList relationshipTypesHint READ relationshipTypesHint WRITE setRelationshipTypesHint NOTIFY fetchHintChanged )
    Q_PROPERTY(OptimizationHints optimizationHints READ optimizationHints WRITE setOptimizationHints NOTIFY fetchHintChanged )
    Q_PROPERTY(int imageWidth READ preferredImageWidth WRITE setPreferredImageWidth NOTIFY fetchHintChanged )
    Q_PROPERTY(int imageHeight READ preferredImageHeight WRITE setPreferredImageHeight NOTIFY fetchHintChanged )
    Q_FLAGS(OptimizationHints)
public:
    enum OptimizationHint {
        AllRequired = QContactFetchHint::AllRequired,
        NoRelationships = QContactFetchHint::NoRelationships,
        NoActionPreferences = QContactFetchHint::NoActionPreferences,
        NoBinaryBlobs = QContactFetchHint::NoBinaryBlobs
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    QDeclarativeContactFetchHint(QObject* parent = Q_NULLPTR);
    QList<int> detailTypesHint() const;
    void setDetailTypesHint(const QList<int> &detailTypes);

    QStringList relationshipTypesHint() const;
    void setRelationshipTypesHint(const QStringList& relationshipTypes);

    int preferredImageWidth() const;
    void setPreferredImageWidth(int w);

    int preferredImageHeight() const;
    void setPreferredImageHeight(int h);

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    QContactFetchHint fetchHint() const;
    void setFetchHint(const QContactFetchHint& fetchHint);
signals:
    void fetchHintChanged();
private:
    QContactFetchHint m_fetchHint;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactFetchHint)

#endif // QDECLARATIVECONTACTFETCHHINT_H
