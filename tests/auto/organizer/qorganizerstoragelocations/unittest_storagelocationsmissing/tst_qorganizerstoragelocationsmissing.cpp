/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtTest/QtTest>

#include <qorganizermanager.h>
#include <qorganizercollectionfetchrequest.h>
#include <qorganizeritemfetchrequest.h>
#include "../../../jsondbprocess.h"

QTORGANIZER_USE_NAMESPACE

class tst_QOrganizerStorageLocationsMissing : public QObject
{
    Q_OBJECT

private slots:
    void testMissingStorageLocations_data() { addManager(); }
    void testMissingStorageLocations();

private:
    void addManager();

    JsonDbProcess jsondbProcess;
};

void tst_QOrganizerStorageLocationsMissing::addManager()
{
    QTest::addColumn<QString>("managerName");

    QStringList availableManagers = QOrganizerManager::availableManagers();
    availableManagers.removeAll("invalid");
    availableManagers.removeAll("skeleton");

    foreach (const QString &manager, availableManagers)
        QTest::newRow(manager.toLatin1().constData()) << manager;
}

void tst_QOrganizerStorageLocationsMissing::testMissingStorageLocations()
{
    QFETCH(QString, managerName);
    if (managerName != "jsondb")
        QSKIP("Test is only for jsondb backend.");

    QStringList emptyList;
    QVERIFY2(jsondbProcess.start(), "Failed to start JsonDb process");

    QOrganizerManager organizerManager(managerName);

    // try some operatios, they should return with StorageLocationsNotExistingError
    QOrganizerCollectionFetchRequest cfr;
    cfr.setManager(&organizerManager);
    QVERIFY(cfr.start());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QCOMPARE(cfr.error(), QOrganizerManager::StorageLocationsNotExistingError);
    QOrganizerItemFetchRequest ifr;
    ifr.setManager(&organizerManager);
    QVERIFY(ifr.start());
    QVERIFY(ifr.waitForFinished());
    QVERIFY(ifr.isFinished());
    QCOMPARE(ifr.error(), QOrganizerManager::StorageLocationsNotExistingError);

    jsondbProcess.terminate();
}

QTEST_MAIN(tst_QOrganizerStorageLocationsMissing)
#include "tst_qorganizerstoragelocationsmissing.moc"
