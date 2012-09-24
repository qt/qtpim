/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
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

#include "tst_missingstorage.h"

Q_DECLARE_METATYPE(QContactAbstractRequest::StorageLocations)
Q_DECLARE_METATYPE(QContactManager::Error)

void tst_StoragesLocationsMissing::initTestCase()
{
    m_contactManager = new QContactManager("jsondb");
}

void tst_StoragesLocationsMissing::cleanupTestCase()
{
    if (m_contactManager)
        delete m_contactManager;
}

QContactManager::Error tst_StoragesLocationsMissing::errorFromManager(QContactAbstractRequest &request, QContactManager *cm)
{
    request.setManager(cm);
    request.start();
    request.waitForFinished();
    request.isFinished();
    return request.error();
}

void tst_StoragesLocationsMissing::fetch()
{
    QFETCH(int, storageLocations);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QContactFetchRequest request;
    request.setStorageLocations(QContactAbstractRequest::StorageLocations(storageLocations));
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}

void tst_StoragesLocationsMissing::idFetch()
{
    QFETCH(int, storageLocations);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QContactIdFetchRequest request;
    request.setStorageLocations(QContactAbstractRequest::StorageLocations(storageLocations));
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}

void tst_StoragesLocationsMissing::fetchById()
{
    QFETCH(QString, jsonId);
    QFETCH(int, storageLocation);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QList<QContactId> contactIds;
    QContactEngineId *engineId = new QContactJsonDbId(jsonId , QContactAbstractRequest::StorageLocation(storageLocation));
    contactIds.append(QContactId(engineId)); // QContactId takes the ownership of the pointer.
    QContactFetchByIdRequest request;
    request.setIds(contactIds);
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}

void tst_StoragesLocationsMissing::fetchByIds()
{
    QFETCH(QString, jsonId);
    QFETCH(int, firstStorageLocation);
    QFETCH(int, secondStorageLocation);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QContactEngineId *firstEngineId = new QContactJsonDbId(jsonId , QContactAbstractRequest::StorageLocation(firstStorageLocation));
    QContactEngineId *secondEngineId = new QContactJsonDbId(jsonId , QContactAbstractRequest::StorageLocation(secondStorageLocation));

    QList<QContactId> contactIds;
    contactIds.append(QContactId(firstEngineId)); // QContactId takes the ownership of the pointer.
    contactIds.append(QContactId(secondEngineId));

    QContactFetchByIdRequest request;
    request.setIds(contactIds);
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}

void tst_StoragesLocationsMissing::removeById()
{
    QFETCH(QString, jsonId);
    QFETCH(int, storageLocation);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QContactEngineId *engineId = new QContactJsonDbId(jsonId , QContactAbstractRequest::StorageLocation(storageLocation));
    QList<QContactId> contactIds;
    contactIds.append(QContactId(engineId)); // QContactId takes the ownership of the pointer.

    QContactRemoveRequest request;
    request.setContactIds(contactIds);
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}

void tst_StoragesLocationsMissing::removeByIds()
{
    QFETCH(QString, jsonId);
    QFETCH(int, firstStorageLocation);
    QFETCH(int, secondStorageLocation);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QContactEngineId *firstEngineId = new QContactJsonDbId(jsonId , QContactAbstractRequest::StorageLocation(firstStorageLocation));
    QContactEngineId *secondEngineId = new QContactJsonDbId(jsonId , QContactAbstractRequest::StorageLocation(secondStorageLocation));

    QList<QContactId> contactIds;
    contactIds.append(QContactId(firstEngineId)); // QContactId takes the ownership of the pointer.
    contactIds.append(QContactId(secondEngineId));

    QContactRemoveRequest request;
    request.setContactIds(contactIds);
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}

QList<QContact> tst_StoragesLocationsMissing::makeTestContactsForSave()
{
    QList<QContact> contacts;
    contacts << QContact();
    return contacts;
}

void tst_StoragesLocationsMissing::save()
{
    QFETCH(int, storageLocation);
    QFETCH(QContactManager::Error, expectedError);

    QContactManager *cm = new QContactManager("jsondb");

    QList<QContact> testContacts = makeTestContactsForSave();
    QContactSaveRequest request;
    request.setContacts(testContacts);
    request.setStorageLocation(QContactAbstractRequest::StorageLocation(storageLocation));
    QCOMPARE(errorFromManager(request, cm), expectedError);

    delete cm;
}


QTEST_MAIN(tst_StoragesLocationsMissing)
