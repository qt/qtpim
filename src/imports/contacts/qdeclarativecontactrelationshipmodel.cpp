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

#include "qdeclarativecontactrelationshipmodel_p.h"

#include <QtGui/qpixmap.h>

#include <QtQml/qqmlinfo.h>

#include <QtContacts/qcontactmanager.h>
#include <QtContacts/qcontactrequests.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/*!
   \qmltype RelationshipModel
    \instantiates QDeclarativeContactRelationshipModel
   \brief The RelationshipModel provides a model of contact relationships from the contacts store.

   \ingroup qml-contacts-main
   \inqmlmodule QtContacts

   This element is part of the \b{QtContacts} module.

    The contents of the model can be specified with \l participantId, \l role and \l relationshipType properties.
    Whether the model is automatically updated when the store or filter changes, can be controlled
    with \l RelationshipModel::autoUpdate property.

    There are two ways of accessing the relationship data: through model by using views and delegates,
    or alternatively via \l relationships list property.

    At the moment only data role provided by the model is \c relationship (\l Relationship).
    Through that one can access any data provided by the Relationship element.

   \sa Relationship, {QContactRelationship}
 */

class QDeclarativeContactRelationshipModelPrivate
{
public:
    QDeclarativeContactRelationshipModelPrivate()
        : m_manager(0)
        , m_participant(0)
        , m_role(QDeclarativeContactRelationship::Either)
    {
    }

    ~QDeclarativeContactRelationshipModelPrivate()
    {
        delete m_manager;
    }

    QContactManager *m_manager;
    QDeclarativeContactRelationship m_relationshipTypeHolder;
    QDeclarativeContact* m_participant;
    QDeclarativeContactRelationship::RelationshipRole m_role;
    QList<QContactRelationship> m_relationships;
    QList<QDeclarativeContactRelationship *> m_declarativeRelationships;
};

QDeclarativeContactRelationshipModel::QDeclarativeContactRelationshipModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new QDeclarativeContactRelationshipModelPrivate)
{
    QHash<int, QByteArray> roleNames;
    roleNames = QAbstractItemModel::roleNames();
    roleNames.insert(RelationshipRole, "relationship");
    setRoleNames(roleNames);
    connect(this, SIGNAL(managerChanged()), SLOT(fetchAgain()));
    connect(this, SIGNAL(participantChanged()), SLOT(fetchAgain()));
    connect(this, SIGNAL(relationshipTypeChanged()), SLOT(fetchAgain()));
    connect(this, SIGNAL(roleChanged()), SLOT(fetchAgain()));
}

QDeclarativeContactRelationshipModel::~QDeclarativeContactRelationshipModel()
{
    delete d;
}

/*!
  \qmlproperty string RelationshipModel::manager

  This property holds the manager uri of the contact backend engine.
  */
QString QDeclarativeContactRelationshipModel::manager() const
{
    if (d->m_manager)
        return d->m_manager->managerName();
    return QString();
}


/*!
  \qmlproperty string RelationshipModel::error

  This property holds the latest error code returned by the contact manager.

  This property is read only.
  */
QString QDeclarativeContactRelationshipModel::error() const
{
    switch (d->m_manager->error()) {
    case QContactManager::DoesNotExistError:
        return QStringLiteral("DoesNotExist");
    case QContactManager::AlreadyExistsError:
        return QStringLiteral("AlreadyExists");
    case QContactManager::InvalidDetailError:
        return QStringLiteral("InvalidDetail");
    case QContactManager::InvalidRelationshipError:
        return QStringLiteral("InvalidRelationship");
    case QContactManager::LockedError:
        return QStringLiteral("LockedError");
    case QContactManager::DetailAccessError:
        return QStringLiteral("DetailAccessError");
    case QContactManager::PermissionsError:
        return QStringLiteral("PermissionsError");
    case QContactManager::OutOfMemoryError:
        return QStringLiteral("OutOfMemory");
    case QContactManager::NotSupportedError:
        return QStringLiteral("NotSupported");
    case QContactManager::BadArgumentError:
        return QStringLiteral("BadArgument");
    case QContactManager::UnspecifiedError:
        return QStringLiteral("UnspecifiedError");
    case QContactManager::VersionMismatchError:
        return QStringLiteral("VersionMismatch");
    case QContactManager::LimitReachedError:
        return QStringLiteral("LimitReached");
    case QContactManager::InvalidContactTypeError:
        return QStringLiteral("InvalidContactType");
    default:
        break;
    }
    return QStringLiteral("NoError");
}
void QDeclarativeContactRelationshipModel::setManager(const QString& manager)
{
    if (d->m_manager == 0 || manager != d->m_manager->managerName() ) {
        d->m_manager = new QContactManager(manager,QMap<QString,QString>(), this);
        connect(d->m_manager,SIGNAL(relationshipsAdded(QList<QContactId>)), this, SLOT(fetchAgain()));
        connect(d->m_manager,SIGNAL(relationshipsRemoved(QList<QContactId>)), this, SLOT(fetchAgain()));
        emit managerChanged();
    }
}


/*!
  \qmlproperty int RelationshipModel::participantId

  This property holds the participant which the list of relationships returned by RelationshipModel should contain.

  \sa RelationshipFilter::relatedContactId
  \sa RelationshipModel::role
  */
QDeclarativeContact* QDeclarativeContactRelationshipModel::participant() const
{
    return d->m_participant;
}
void QDeclarativeContactRelationshipModel::setParticipant(QDeclarativeContact* participant)
{
    if (d->m_participant != participant) {
        d->m_participant = participant;
        emit participantChanged();
    }
}

/*!
  \qmlproperty variant RelationshipModel::relationshipType

  This property holds the relationship type which the list of relationships returned by RelationshipModel should contain.

  \sa Relationship::type
  */
QVariant QDeclarativeContactRelationshipModel::relationshipType() const
{
    return d->m_relationshipTypeHolder.relationshipType();
}
void QDeclarativeContactRelationshipModel::setRelationshipType(const QVariant& type)
{
    if (type != relationshipType()) {
        d->m_relationshipTypeHolder.setRelationshipType(type);
        emit relationshipTypeChanged();
    }
}

/*!
  \qmlproperty enumeration RelationshipModel::role

  This property holds the relationship role which the list of relationships returned by RelationshipModel should contain.

  \sa RelationshipFilter::relatedContactRole
  */
QDeclarativeContactRelationship::RelationshipRole QDeclarativeContactRelationshipModel::role() const
{
    return d->m_role;
}
void QDeclarativeContactRelationshipModel::setRole(QDeclarativeContactRelationship::RelationshipRole role)
{
    if (d->m_role != role) {
        d->m_role = role;
        emit roleChanged();
    }
}

/*!
  \qmlproperty bool RelationshipModel::autoUpdate

  This property indicates whether or not the relationship model should be updated automatically, default value is true.
  */
bool QDeclarativeContactRelationshipModel::autoUpdate() const
{
    //TODO
    return true;
}
void QDeclarativeContactRelationshipModel::setAutoUpdate(bool autoUpdate)
{
    Q_UNUSED(autoUpdate);
    //TODO
}

/*!
  \qmlproperty list<Relationship> RelationshipModel::relationships

  This property holds a list of relationships.

  \sa Relationship
  */
QQmlListProperty<QDeclarativeContactRelationship> QDeclarativeContactRelationshipModel::relationships()
{
    return QQmlListProperty<QDeclarativeContactRelationship>(this, d->m_declarativeRelationships);
}

int QDeclarativeContactRelationshipModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d->m_declarativeRelationships.count();
}

QVariant QDeclarativeContactRelationshipModel::data(const QModelIndex &index, int role) const
{
    QDeclarativeContactRelationship* dcr = d->m_declarativeRelationships.value(index.row());
    if (role == RelationshipRole) {
        return QVariant::fromValue(dcr);
    } else if (role == Qt::DisplayRole) {
        return QString(QStringLiteral("%1 %2 %3")).arg(dcr->relationship().first().toString()).arg(dcr->relationship().relationshipType()).arg(dcr->relationship().second().toString());
    }
    return QVariant();
}

void QDeclarativeContactRelationshipModel::fetchAgain()
{
    if (d->m_manager) {
        QContactRelationshipFetchRequest* req = new QContactRelationshipFetchRequest(this);
        req->setManager(d->m_manager);
        if (d->m_participant) {
            QContact contact (d->m_participant->contact());
            if (d->m_role == QDeclarativeContactRelationship::First || d->m_role == QDeclarativeContactRelationship::Either)
                req->setFirst(contact.id());
            if (d->m_role == QDeclarativeContactRelationship::Second || d->m_role == QDeclarativeContactRelationship::Either)
                req->setSecond(contact.id());
            req->setRelationshipType(d->m_relationshipTypeHolder.relationship().relationshipType());
            connect(req,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(requestUpdated()));
            req->start();
        }
    }
}

/*!
  \qmlmethod RelationshipModel::addRelationship(relationship)
  Addes the given \a relationship to the backend store.
  */
void QDeclarativeContactRelationshipModel::addRelationship(QDeclarativeContactRelationship* dcr)
{
    if (dcr) {
        QContactRelationship cr = dcr->relationship();
        QContactRelationshipSaveRequest* req = new QContactRelationshipSaveRequest(this);
        req->setManager(d->m_manager);
        req->setRelationship(cr);
        connect(req, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(relationshipsSaved()));
        req->start();
    }
}

/*!
  \qmlmethod RelationshipModel::removeRelationship(relationship)
  Removes the given \a relationship from the backend store.
  */
void QDeclarativeContactRelationshipModel::removeRelationship(QDeclarativeContactRelationship* dcr)
{
    if (dcr) {
        QContactRelationship cr = dcr->relationship();
        QContactRelationshipRemoveRequest* req = new QContactRelationshipRemoveRequest(this);
        req->setManager(d->m_manager);
        req->setRelationship(cr);
        connect(req,SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(relationshipsRemoved()));
        req->start();
    }
}

void QDeclarativeContactRelationshipModel::requestUpdated()
{
    QContactRelationshipFetchRequest* req = qobject_cast<QContactRelationshipFetchRequest*>(sender());
    Q_ASSERT(req);
    if (req->isFinished() && req->error() == QContactManager::NoError) {

        QList<QContactRelationship> relationships = req->relationships();

        reset();
        beginInsertRows(QModelIndex(), 0, relationships.count());

        foreach(QDeclarativeContactRelationship* dcr, d->m_declarativeRelationships) {
            dcr->deleteLater();
        }
        d->m_declarativeRelationships.clear();
        d->m_relationships.clear();

        foreach (const QContactRelationship& cr, relationships) {
            QDeclarativeContactRelationship* dcr = new QDeclarativeContactRelationship(this);
            dcr->setRelationship(cr);
            d->m_declarativeRelationships.append(dcr);
            d->m_relationships.append(cr);
        }
        endInsertRows();
        req->deleteLater();
        emit relationshipsChanged();
    }
}

void QDeclarativeContactRelationshipModel::relationshipsSaved()
{
    QContactRelationshipSaveRequest* req = qobject_cast<QContactRelationshipSaveRequest*>(sender());
    Q_ASSERT(req);
    if (req->isFinished()) {
        QList<QContactRelationship> rs = req->relationships();
        QList<int> errorIds = req->errorMap().keys();

        for( int i = 0; i < rs.count(); i++) {
            if (!errorIds.contains(i)) {
                //saved
                QContactRelationship r = rs.at(i);

                if (!d->m_relationships.contains(r)) {
                    //new relationship saved
                    QDeclarativeContactRelationship* dcr = new QDeclarativeContactRelationship(this);
                    dcr->setRelationship(r);
                    beginInsertRows(QModelIndex(), d->m_declarativeRelationships.count(), d->m_declarativeRelationships.count());
                    d->m_declarativeRelationships.append(dcr);
                    d->m_relationships.append(r);
                    endInsertRows();
                }
            }
        }
        req->deleteLater();
    }
}

void QDeclarativeContactRelationshipModel::relationshipsRemoved()
{
    QContactRelationshipRemoveRequest* req = qobject_cast<QContactRelationshipRemoveRequest*>(sender());
    Q_ASSERT(req);
    if (req->isFinished()) {
        QList<QContactRelationship> rs = req->relationships();
        QList<int> errorIds = req->errorMap().keys();


        for( int i = 0; i < rs.count(); i++) {
            if (!errorIds.contains(i)) {
                int row = 0;
                QContactRelationship r = rs.at(i);
                for (; row < d->m_relationships.count(); row++) {
                    if (d->m_relationships.at(row) == r)
                        break;
                }
                if (row < d->m_relationships.count()) {
                    beginRemoveRows(QModelIndex(), row, row);
                    d->m_declarativeRelationships.removeAt(row);
                    d->m_relationships.removeAt(row);
                    endRemoveRows();
                } else {
                    //impossible?
                    qmlInfo(this) << tr("this relationship '") << row << tr("' was already removed!");
                }
            }
        }
        req->deleteLater();
    }
}

#include "moc_qdeclarativecontactrelationshipmodel_p.cpp"

QT_END_NAMESPACE
