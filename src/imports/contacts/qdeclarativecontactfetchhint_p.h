/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
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
