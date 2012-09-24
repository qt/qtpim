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

// This module implements no userdata storage location related test data for
// a set of tests shared with other similar modules for cases of no system
// storage and no storages at all.

#include "tst_missingstorage.h"

Q_DECLARE_METATYPE(QContactAbstractRequest::StorageLocations)
Q_DECLARE_METATYPE(QContactManager::Error)

tst_StoragesLocationsMissing::tst_StoragesLocationsMissing()
{
    QString partitions_json = QFINDTESTDATA("partitions.json");
    QVERIFY2(!partitions_json.isEmpty(), "partitions.json file is missing");
    QVERIFY2(m_jsondbProcess.start(partitions_json), "Failed to start JsonDb process");
}

tst_StoragesLocationsMissing::~tst_StoragesLocationsMissing()
{
        m_jsondbProcess.terminate();
}

void tst_StoragesLocationsMissing::init()
{
    // Make sure all tests in this test case start with an empty system storage.
    QContactIdFetchRequest idFetchRequest;
    idFetchRequest.setManager(m_contactManager);
    idFetchRequest.setStorageLocations(QContactAbstractRequest::SystemStorage);
    idFetchRequest.start();
    idFetchRequest.waitForFinished();
    if (!idFetchRequest.ids().isEmpty()) {
        QContactRemoveRequest removeRequest;
        removeRequest.setManager(m_contactManager);
        removeRequest.setContactIds(idFetchRequest.ids());
        removeRequest.start();
        removeRequest.waitForFinished();
    }
}

void tst_StoragesLocationsMissing::fetchData()
{
    QTest::addColumn<int>("storageLocations");
    QTest::addColumn<QContactManager::Error>("expectedError");

    QTest::newRow("from user") << int(QContactAbstractRequest::UserDataStorage)
                               << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from system") << int(QContactAbstractRequest::SystemStorage)
                                 << QContactManager::DoesNotExistError;

    QTest::newRow("from both") << int(QContactAbstractRequest::SystemStorage | QContactAbstractRequest::UserDataStorage)
                               << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from invalid") << (0x8)
                                  << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from invalid with user") << int(0x8 | QContactAbstractRequest::UserDataStorage)
                                            << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from invalid with system") << int(0x8 | QContactAbstractRequest::SystemStorage)
                                              << QContactManager::InvalidStorageLocationError;
}

void tst_StoragesLocationsMissing::idData()
{
    QTest::addColumn<QString>("jsonId");
    QTest::addColumn<int>("storageLocation");
    QTest::addColumn<QContactManager::Error>("expectedError");

    QTest::newRow("from user") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                               << int(QContactAbstractRequest::UserDataStorage)
                               << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from system") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                                 << int(QContactAbstractRequest::SystemStorage)
                                 << QContactManager::DoesNotExistError;

    QTest::newRow("from invalid") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                                  << (0x8)
                                  << QContactManager::InvalidStorageLocationError;

    QTest::newRow("with empty id") << QString()
                                   << (0x0)
                                   << QContactManager::InvalidStorageLocationError;
}

void tst_StoragesLocationsMissing::idsData()
{
    QTest::addColumn<QString>("jsonId");
    QTest::addColumn<int>("firstStorageLocation");
    QTest::addColumn<int>("secondStorageLocation");
    QTest::addColumn<QContactManager::Error>("expectedError");

    QTest::newRow("from user") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                               << int(QContactAbstractRequest::UserDataStorage)
                               << int(QContactAbstractRequest::UserDataStorage)
                               << QContactManager::InvalidStorageLocationError;


    QTest::newRow("from system") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                                 << int(QContactAbstractRequest::SystemStorage)
                                 << int(QContactAbstractRequest::SystemStorage)
                                 << QContactManager::DoesNotExistError;

    QTest::newRow("from user and system") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                               << int(QContactAbstractRequest::UserDataStorage)
                               << int(QContactAbstractRequest::SystemStorage)
                               << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from invalid with user") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                                  << (0x8)
                                  << int(QContactAbstractRequest::UserDataStorage)
                                  << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from invalid with system") << QString("a096c227-fc6f-46ad-89b8-1883fb490a38")
                                  << (0x8)
                                  << int(QContactAbstractRequest::SystemStorage)
                                  << QContactManager::InvalidStorageLocationError;

    QTest::newRow("from user and system with an empty id") << QString()
                                  << int(QContactAbstractRequest::UserDataStorage)
                                  << int(QContactAbstractRequest::SystemStorage)
                                  << QContactManager::InvalidStorageLocationError;
}

void tst_StoragesLocationsMissing::saveData()
{
    QTest::addColumn<int>("storageLocation");
    QTest::addColumn<QContactManager::Error>("expectedError");

    QTest::newRow("to user") << int(QContactAbstractRequest::UserDataStorage)
                             << QContactManager::InvalidStorageLocationError;

    QTest::newRow("to system") << int(QContactAbstractRequest::SystemStorage)
                               << QContactManager::NoError;

    QTest::newRow("to invalid") << (0x8)
                                << QContactManager::InvalidStorageLocationError;
}


