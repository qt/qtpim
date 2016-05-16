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

#ifndef QDECLARATIVECONTACTRELATIONSHIPMODEL_P_H
#define QDECLARATIVECONTACTRELATIONSHIPMODEL_P_H

#include <QtCore/qabstractitemmodel.h>

#include <QtQml/qqml.h>

#include "qdeclarativecontactrelationship_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactRelationshipModelPrivate;
class QDeclarativeContactRelationshipModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(QDeclarativeContact* participant READ participant WRITE setParticipant NOTIFY participantChanged)
    Q_PROPERTY(QVariant relationshipType READ relationshipType WRITE setRelationshipType NOTIFY relationshipTypeChanged)
    Q_PROPERTY(QDeclarativeContactRelationship::RelationshipRole role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(QQmlListProperty<QDeclarativeContactRelationship> relationships READ relationships NOTIFY relationshipsChanged)
    Q_PROPERTY(QString error READ error)

public:
    QDeclarativeContactRelationshipModel(QObject *parent = Q_NULLPTR);
    ~QDeclarativeContactRelationshipModel();
    enum {
        RelationshipRole = Qt::UserRole + 500

    };

    QString manager() const;
    void setManager(const QString& manager);
    bool autoUpdate() const;
    void setAutoUpdate(bool autoUpdate);
    QString error() const;
    QDeclarativeContact* participant() const;
    void setParticipant(QDeclarativeContact* participant);

    QVariant relationshipType() const;
    void setRelationshipType(const QVariant& type);

    QDeclarativeContactRelationship::RelationshipRole role() const;
    void setRole(QDeclarativeContactRelationship::RelationshipRole role);

    QQmlListProperty<QDeclarativeContactRelationship> relationships();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void removeRelationship(QDeclarativeContactRelationship* dcr);
    Q_INVOKABLE void addRelationship(QDeclarativeContactRelationship* dcr);
signals:
    void managerChanged();
    void participantChanged();
    void relationshipTypeChanged();
    void roleChanged();
    void relationshipsChanged();
    void autoUpdateChanged();
private slots:
    void fetchAgain();
    void requestUpdated();

    void relationshipsSaved();

    void relationshipsRemoved();

private:
    QDeclarativeContactRelationshipModelPrivate* d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactRelationshipModel)

#endif // QDECLARATIVECONTACTRELATIONSHIPMODEL_P_H
