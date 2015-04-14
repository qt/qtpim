/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtTest/QtTest>
#include <QtCore/QDateTime>
#include <QtOrganizer/qorganizer.h>
#include "../qorganizermanagerdataholder.h"

QTORGANIZER_USE_NAMESPACE

class tst_QOrganizerManagerDetails : public QObject
{
Q_OBJECT

private:
    void addManagers();

    bool saveAndLoadItem(QOrganizerManager *manager, QOrganizerItem &original, QOrganizerItem &loaded);
    void saveAndVerifyItem(QOrganizerManager *manager, QOrganizerItem &original);

    QScopedPointer<QOrganizerManagerDataHolder> managerDataHolder;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testRecurrenceRule();
    void testRecurrenceRule_data() {addManagers();}

private slots:

};

void tst_QOrganizerManagerDetails::addManagers()
{
    QTest::addColumn<QString>("uri");

    QStringList managers = QOrganizerManager::availableManagers();

    /* Known one that will not pass */
    managers.removeAll("invalid");
    managers.removeAll("skeleton");
    managers.removeAll("maliciousplugin");

    foreach(QString mgr, managers) {
        QMap<QString, QString> params;
        QTest::newRow(QString("mgr='%1'").arg(mgr).toLatin1().constData()) << QOrganizerManager::buildUri(mgr, params);
    }
}

void tst_QOrganizerManagerDetails::initTestCase()
{
    managerDataHolder.reset(new QOrganizerManagerDataHolder());
}

void tst_QOrganizerManagerDetails::cleanupTestCase()
{
    managerDataHolder.reset(0);
}

bool tst_QOrganizerManagerDetails::saveAndLoadItem(QOrganizerManager *manager, QOrganizerItem &original, QOrganizerItem &loaded)
{
    // Save item
    if(manager->saveItem(&original) == false)
        return false;

    // Check the id
    if(original.id().isNull())
        return false;

    // Load same item from database
    loaded = manager->item(original.id());
    if(manager->error())
        return false;

    // TODO: Ignore some details which are not relevant and will mess
    // up direct comparison between two items.

    return true;
}

void tst_QOrganizerManagerDetails::saveAndVerifyItem(QOrganizerManager *manager, QOrganizerItem &original)
{
    QOrganizerItem loaded;
    QVERIFY(saveAndLoadItem(manager, original, loaded));
    if (original != loaded) {
        qDebug() << "expected: " << original;
        qDebug() << "loaded: " << loaded;
        QCOMPARE(loaded.details().count(), original.details().count());
        QCOMPARE(loaded, original);
    }
}

void tst_QOrganizerManagerDetails::testRecurrenceRule()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> manager(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel("recurring event");
    event.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
    event.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(4, 4, 5)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(QDate(2010, 1, 10));
    event.setRecurrenceRule(rrule);

    saveAndVerifyItem(manager.data(), event);

}
QTEST_MAIN(tst_QOrganizerManagerDetails)
#include "tst_qorganizermanagerdetails.moc"
