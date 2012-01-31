/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QDECLARATIVEORGANIZERITEMFETCHHINT_H
#define QDECLARATIVEORGANIZERITEMFETCHHINT_H

#include <qorganizeritemfetchhint.h>
#include <QtDeclarative/qdeclarative.h>

QTORGANIZER_BEGIN_NAMESPACE

class QDeclarativeOrganizerItemFetchHint : public QObject
{
    Q_OBJECT

    Q_PROPERTY(OptimizationHints optimizationHints READ optimizationHints WRITE setOptimizationHints NOTIFY fetchHintChanged)

    Q_ENUMS(OptimizationHint)
    Q_FLAGS(OptimizationHints)

public:
    QDeclarativeOrganizerItemFetchHint(QObject *parent = 0);

    enum OptimizationHint {
        AllRequired = QOrganizerItemFetchHint::AllRequired,
        NoActionPreferences = QOrganizerItemFetchHint::NoActionPreferences,
        NoBinaryBlobs = QOrganizerItemFetchHint::NoBinaryBlobs
    };
    Q_DECLARE_FLAGS(OptimizationHints, OptimizationHint)

    OptimizationHints optimizationHints() const;
    void setOptimizationHints(OptimizationHints hints);

    QOrganizerItemFetchHint fetchHint() const;
    void setFetchHint(const QOrganizerItemFetchHint &fetchHint);

Q_SIGNALS:
    void fetchHintChanged();

private:
    QOrganizerItemFetchHint d;
};

QTORGANIZER_END_NAMESPACE

QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerItemFetchHint))

#endif // QDECLARATIVEORGANIZERITEMFETCHHINT_H
