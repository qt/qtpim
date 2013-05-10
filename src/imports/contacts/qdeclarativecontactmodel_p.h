/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#ifndef QDECLARATIVECONTACTMODEL_P_H
#define QDECLARATIVECONTACTMODEL_P_H

#include <QAbstractListModel>
#include <QQmlListProperty>
#include <QQmlParserStatus>

#include <qcontact.h>
#include "qcontactrequests.h"
#include "qdeclarativecontact_p.h"
#include <qversitreader.h>
#include <qversitwriter.h>

#include "qdeclarativecontactfetchhint_p.h"
#include "qdeclarativecontactsortorder_p.h"
#include "qdeclarativecontactfilter_p.h"

QT_BEGIN_NAMESPACE_VERSIT
class QVersitReader;
QT_END_NAMESPACE_VERSIT

QTVERSIT_USE_NAMESPACE

QT_BEGIN_NAMESPACE_CONTACTS

class QDeclarativeContactModelPrivate;
class QDeclarativeContactModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_PROPERTY(QString manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(int storageLocations READ storageLocations WRITE setStorageLocations NOTIFY storageLocationsChanged)
    Q_PROPERTY(QStringList availableManagers READ availableManagers)
    Q_PROPERTY(QString error READ error NOTIFY errorChanged)
    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(QDeclarativeContactFilter* filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QDeclarativeContactFetchHint* fetchHint READ fetchHint WRITE setFetchHint NOTIFY fetchHintChanged)
    Q_PROPERTY(QQmlListProperty<QDeclarativeContact> contacts READ contacts NOTIFY contactsChanged)
    Q_PROPERTY(QQmlListProperty<QDeclarativeContactSortOrder> sortOrders READ sortOrders NOTIFY sortOrdersChanged)
    Q_ENUMS(ExportError)
    Q_ENUMS(ImportError)
    Q_ENUMS(StorageLocation)
    Q_INTERFACES(QQmlParserStatus)

public:
    explicit QDeclarativeContactModel(QObject *parent = 0);

    enum {
        ContactRole =  Qt::UserRole + 500
    };

    enum ExportError {
        ExportNoError          = QVersitWriter::NoError,
        ExportUnspecifiedError = QVersitWriter::UnspecifiedError,
        ExportIOError          = QVersitWriter::IOError,
        ExportOutOfMemoryError = QVersitWriter::OutOfMemoryError,
        ExportNotReadyError    = QVersitWriter::NotReadyError
    };

    enum ImportError {
        ImportNoError          = QVersitReader::NoError,
        ImportUnspecifiedError = QVersitReader::UnspecifiedError,
        ImportIOError          = QVersitReader::IOError,
        ImportOutOfMemoryError = QVersitReader::OutOfMemoryError,
        ImportNotReadyError    = QVersitReader::NotReadyError,
        ImportParseError       = QVersitReader::ParseError
    };

    enum StorageLocation {
        UserDataStorage = QContactAbstractRequest::UserDataStorage,
        SystemStorage = QContactAbstractRequest::SystemStorage
    };

    QString manager() const;
    void setManager(const QString& manager);

    int storageLocations() const;
    void setStorageLocations(int storageLocations);

    QStringList availableManagers() const;
    
    QString error() const;

    QDeclarativeContactFilter* filter() const;
    void setFilter(QDeclarativeContactFilter* filter);

    QDeclarativeContactFetchHint* fetchHint() const;
    void setFetchHint(QDeclarativeContactFetchHint* fetchHint);

    // From QQmlParserStatus
    virtual void classBegin() {}
    virtual void componentComplete();

    // From QAbstractListModel
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    bool autoUpdate() const;
    void setAutoUpdate(bool autoUpdate);

    QQmlListProperty<QDeclarativeContact> contacts() ;
    static void contacts_append(QQmlListProperty<QDeclarativeContact>* prop, QDeclarativeContact* contact);
    static int contacts_count(QQmlListProperty<QDeclarativeContact>* prop);
    static QDeclarativeContact* contacts_at(QQmlListProperty<QDeclarativeContact>* prop, int index);
    static void contacts_clear(QQmlListProperty<QDeclarativeContact>* prop);

    static void sortOrder_append(QQmlListProperty<QDeclarativeContactSortOrder> *p, QDeclarativeContactSortOrder *sortOrder);
    static int  sortOrder_count(QQmlListProperty<QDeclarativeContactSortOrder> *p);
    static QDeclarativeContactSortOrder * sortOrder_at(QQmlListProperty<QDeclarativeContactSortOrder> *p, int idx);
    static void  sortOrder_clear(QQmlListProperty<QDeclarativeContactSortOrder> *p);

    QQmlListProperty<QDeclarativeContactSortOrder> sortOrders() ;

    Q_INVOKABLE void removeContact(QString id);
    Q_INVOKABLE void removeContacts(const QStringList& ids);
    Q_INVOKABLE void saveContact(QDeclarativeContact* dc, StorageLocation storageLocation = UserDataStorage);
    Q_INVOKABLE int fetchContacts(const QStringList& contactIds);
    Q_INVOKABLE void importContacts(const QUrl& url, const QStringList& profiles = QStringList());
    Q_INVOKABLE void exportContacts(const QUrl& url, const QStringList& profiles = QStringList(), const QVariantList &declarativeContacts = QVariantList());

signals:
    void managerChanged();
    void storageLocationsChanged();
    void filterChanged();
    void errorChanged();
    void fetchHintChanged();
    void contactsChanged();
    void sortOrdersChanged();
    void autoUpdateChanged();
    void exportCompleted(ExportError error, QUrl url);
    void importCompleted(ImportError error, QUrl url);
    void contactsFetched(int requestId, const QVariantList &fetchedContacts);

public slots:
    void update();

private slots:
    void clearContacts();
    void fetchAgain();
    void requestUpdated();
    void doUpdate();
    void onRequestStateChanged(QContactAbstractRequest::State newState);
    void onContactsAdded(const QList<QContactId>& ids);
    void onContactsRemoved(const QList<QContactId>& ids);
    void onContactsChanged(const QList<QContactId>& ids);
    void startImport(QVersitReader::State state);
    void contactsExported(QVersitWriter::State state);

    // handle fetch request from onContactsAdded()
    void onContactsAddedFetchRequestStateChanged(QContactAbstractRequest::State state);

    // handle fetch request from onContactsChanged()
    void onContactsChangedFetchRequestStateChanged(QContactAbstractRequest::State state);

    // handle fetch request from fetchContacts()
    void onFetchContactsRequestStateChanged(QContactAbstractRequest::State state);

private:
    QList<QContactId> extractContactIdsInStorageLocationFromThisModel(const QList<QContactId> &ids);
    static QContactAbstractRequest::StorageLocations extractStorageLocation(const QContactId &id);
    QContactFetchRequest *createContactFetchRequest(const QList<QContactId> &ids);
    void checkError(const QContactAbstractRequest *request);
    void updateError(QContactManager::Error error);
    int contactIndex(const QDeclarativeContact* contact);

private:
    QDeclarativeContactModelPrivate *d;
};

QT_END_NAMESPACE_CONTACTS

QTCONTACTS_USE_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactModel)

#endif // QDECLARATIVECONTACTMODEL_P_H
