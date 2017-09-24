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

#ifndef QDECLARATIVEORGANIZERITEMFETCHHINT_H
#define QDECLARATIVEORGANIZERITEMFETCHHINT_H

#include <QtQml/qqml.h>

#include <QtOrganizer/qorganizeritemfetchhint.h>

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeOrganizerItemFetchHint : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<int> detailTypesHint READ detailTypesHint WRITE setDetailTypesHint NOTIFY fetchHintChanged )
    Q_PROPERTY(OptimizationHints optimizationHints READ optimizationHints WRITE setOptimizationHints NOTIFY fetchHintChanged)

    Q_ENUMS(OptimizationHint)
    Q_FLAGS(OptimizationHints)

public:
    QDeclarativeOrganizerItemFetchHint(QObject *parent = nullptr);

    enum OptimizationHint {
        AllRequired = QOrganizerItemFetchHint::AllRequired,
        NoActionPreferences = QOrganizerItemFetchHint::NoActionPreferences,
        NoBinaryBlobs = QOrganizerItemFetchHint::NoBinaryBlobs
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    QList<int> detailTypesHint() const;
    void setDetailTypesHint(const QList<int> &detailTypes);

    QOrganizerItemFetchHint fetchHint() const;
    void setFetchHint(const QOrganizerItemFetchHint &fetchHint);

Q_SIGNALS:
    void fetchHintChanged();

private:
    QOrganizerItemFetchHint d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeOrganizerItemFetchHint)

#endif // QDECLARATIVEORGANIZERITEMFETCHHINT_H
