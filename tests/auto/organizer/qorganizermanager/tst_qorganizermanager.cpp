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

//TESTED_COMPONENT=src/organizer

#include <QtTest/QtTest>
#include <QtCore/QUuid>

#include <QtOrganizer/qorganizer.h>
#include <QtOrganizer/qorganizeritemchangeset.h>
#include "../qorganizermanagerdataholder.h"

#include <QtOrganizer/qorganizernote.h>
#include <QtOrganizer/qorganizerevent.h>

QTORGANIZER_USE_NAMESPACE

// to get QFETCH to work with the template expression...
typedef QMap<QString,QString> tst_QOrganizerManager_QStringMap;
Q_DECLARE_METATYPE(tst_QOrganizerManager_QStringMap)
Q_DECLARE_METATYPE(QList<QOrganizerItemId>)

/* A class that no backend can support */
class UnsupportedMetatype {
    int foo;
};
Q_DECLARE_METATYPE(UnsupportedMetatype)
Q_DECLARE_METATYPE(QOrganizerItem)
Q_DECLARE_METATYPE(QOrganizerManager::Error)
Q_DECLARE_METATYPE(QList<QDate>)
Q_DECLARE_METATYPE(QList<QOrganizerItemDetail::DetailType>);

static inline QOrganizerItemId makeItemId(uint id)
{
    return QOrganizerItemId(QStringLiteral("qtorganizer:basic:"), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}

static inline QOrganizerCollectionId makeCollectionId(uint id)
{
    return QOrganizerCollectionId(QStringLiteral("qtorganizer:basic:"), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}


class tst_QOrganizerManager : public QObject
{
Q_OBJECT

public:
    tst_QOrganizerManager();
    virtual ~tst_QOrganizerManager();

private:
    void dumpOrganizerItemDifferences(const QOrganizerItem& a, const QOrganizerItem& b);
    void dumpOrganizerItem(const QOrganizerItem &c);
    void dumpOrganizerItems(QOrganizerManager *cm);
    bool isSuperset(const QOrganizerItem& ca, const QOrganizerItem& cb);
    QList<QOrganizerItemDetail> removeAllDefaultDetails(const QList<QOrganizerItemDetail>& details);
    void addManagers(); // add standard managers to the data

    QScopedPointer<QOrganizerManagerDataHolder> managerDataHolder;

public slots:
    void initTestCase();
    void cleanupTestCase();
private slots:

    void doDump();
    void doDump_data() {addManagers();}

    /* Special test with special data */
    void uriParsing();
    void recurrenceWithGenerator();
    void todoRecurrenceWithGenerator();
    void dateRange();

    /* Tests that are run on all managers */
    void metadata();
    void nullIdOperations();
    void add();
    void saveRecurrence();
    void persistence();
    void addExceptions();
    void modifyRecurrence();
    void addExceptionsWithGuid();
    void update();
    void remove();
    void batch();
    void observerDeletion();
    void signalEmission();
    void detailOrders();
    void itemType();
    void collections();
    void dataSerialization();
    void itemFetch();
    void todoItemFetch();
    void itemFetchV2();
    void itemFilterFetch();
    void spanOverDays();
    void incompleteTodoTime();
    void recurrence();
    void idComparison();
    void emptyItemManipulation();
    void partialSave();

    /* Tests that take no data */
    void errorStayingPut();
    void ctors();
    void invalidManager();
    void memoryManager();
    void changeSet();
    void fetchHint();
    void testFilterFunction();
    void testReminder();
    void testIntersectionFilter();
    void testNestCompoundFilter();
    void testUnionFilter();
    void testItemOccurrences();

    /* Special test with special data */
    void uriParsing_data();
    void recurrenceWithGenerator_data();
    void todoRecurrenceWithGenerator_data();
    void dateRange_data();
    /* Tests that are run on all managers */
    void metadata_data() {addManagers();}
    void nullIdOperations_data() {addManagers();}
    void add_data() {addManagers();}
    void saveRecurrence_data() {addManagers();}
    void persistence_data() {addManagers();}
    void addExceptions_data() {addManagers();}
    void addExceptionsWithGuid_data() {addManagers();}
    void modifyRecurrence_data() {addManagers();}
    void update_data() {addManagers();}
    void remove_data() {addManagers();}
    void batch_data() {addManagers();}
    void signalEmission_data() {addManagers();}
    void detailOrders_data() {addManagers();}
    void itemType_data() {addManagers();}
    void collections_data() {addManagers();}
    void dataSerialization_data() {addManagers();}
    void itemFetch_data() {addManagers();}
    void todoItemFetch_data() {addManagers();}
    void itemFetchV2_data() {addManagers();}
    void itemFilterFetch_data() {addManagers();}
    void spanOverDays_data() {addManagers();}
    void incompleteTodoTime_data() {addManagers();}
    void recurrence_data() {addManagers();}
    void idComparison_data() {addManagers();}
    void testReminder_data() {addManagers();}
    void testIntersectionFilter_data() {addManagers();}
    void testNestCompoundFilter_data() {addManagers();}
    void testUnionFilter_data() {addManagers();}
    void emptyItemManipulation_data() {addManagers();}
    void partialSave_data() {addManagers();}
    void testItemOccurrences_data(){addManagers();}

    void testTags_data() { addManagers(); }
    void testTags();

    void testExtendedDetail_data() { addManagers(); }
    void testExtendedDetail();

    void testAttendee_data() { addManagers(); }
    void testAttendee();

    void testRsvp_data() { addManagers(); }
    void testRsvp();

    void testClassification_data() { addManagers(); }
    void testClassification();

    void testVersion_data() { addManagers(); }
    void testVersion();
};

tst_QOrganizerManager::tst_QOrganizerManager()
{
}

tst_QOrganizerManager::~tst_QOrganizerManager()
{
}

void tst_QOrganizerManager::initTestCase()
{
    managerDataHolder.reset(new QOrganizerManagerDataHolder());

    /* Make sure these other test plugins are NOT loaded by default */
    // These are now removed from the list of managers in addManagers()
    QVERIFY(!QOrganizerManager::availableManagers().contains("testdummy"));
    QVERIFY(!QOrganizerManager::availableManagers().contains("teststaticdummy"));
}

void tst_QOrganizerManager::cleanupTestCase()
{
    managerDataHolder.reset(0);
}

void tst_QOrganizerManager::dumpOrganizerItemDifferences(const QOrganizerItem& ca, const QOrganizerItem& cb)
{
    // Try to narrow down the differences
    QOrganizerItem a(ca);
    QOrganizerItem b(cb);

    // Check the display label
    QCOMPARE(a.displayLabel(), b.displayLabel());

    // Now look at the rest
    QList<QOrganizerItemDetail> aDetails = a.details();
    QList<QOrganizerItemDetail> bDetails = b.details();

    // They can be in any order, so loop
    // First remove any matches
    foreach(QOrganizerItemDetail d, aDetails) {
        foreach(QOrganizerItemDetail d2, bDetails) {
            if(d == d2) {
                a.removeDetail(&d);
                b.removeDetail(&d2);
                break;
            }
        }
    }

    // Now dump the extra details that were unmatched in A (note that DisplayLabel and Type are always present).
    aDetails = a.details();
    bDetails = b.details();
    foreach(QOrganizerItemDetail d, aDetails) {
        if (d.type() != QOrganizerItemDetail::TypeDisplayLabel && d.type() != QOrganizerItemDetail::TypeItemType)
            qDebug() << "A item had extra detail:" << d.type() << d.values();
    }
    // and same for B
    foreach(QOrganizerItemDetail d, bDetails) {
        if (d.type() != QOrganizerItemDetail::TypeDisplayLabel && d.type() != QOrganizerItemDetail::TypeItemType)
            qDebug() << "B item had extra detail:" << d.type() << d.values();
    }

    // now test specifically the display label and the type
    if (a.displayLabel() != b.displayLabel()) {
        qDebug() << "A item display label =" << a.displayLabel();
        qDebug() << "B item display label =" << b.displayLabel();
    }
    if (a.type() != b.type()) {
        qDebug() << "A item type =" << a.type();
        qDebug() << "B item type =" << b.type();
    }
}

bool tst_QOrganizerManager::isSuperset(const QOrganizerItem& ca, const QOrganizerItem& cb)
{
    // returns true if item ca is a superset of item cb
    // we use this test instead of equality because dynamic information
    // such as presence/location, and synthesised information such as
    // display label and (possibly) type, may differ between a item
    // in memory and the item in the managed store.

    QOrganizerItem a(ca);
    QOrganizerItem b(cb);
    QList<QOrganizerItemDetail> aDetails = a.details();
    QList<QOrganizerItemDetail> bDetails = b.details();

    // They can be in any order, so loop
    // First remove any matches
    foreach(QOrganizerItemDetail d, aDetails) {
        foreach(QOrganizerItemDetail d2, bDetails) {
            if(d == d2) {
                a.removeDetail(&d);
                b.removeDetail(&d2);
                break;
            }
        }
    }

    // Second remove any superset matches (eg, backend adds a field)
    aDetails = a.details();
    bDetails = b.details();
    foreach (QOrganizerItemDetail d, aDetails) {
        foreach (QOrganizerItemDetail d2, bDetails) {
            if (d.type() == d2.type()) {
                bool canRemove = true;
                QMap<int, QVariant> d2map = d2.values();
                foreach (int key, d2map.keys()) {
                    if (d.value(key) != d2.value(key)) {
                        // d can have _more_ keys than d2,
                        // but not _less_; and it cannot
                        // change the value.
                        canRemove = false;
                    }
                }

                if (canRemove) {
                    // if we get to here, we can remove the details.
                    a.removeDetail(&d);
                    b.removeDetail(&d2);
                    break;
                }
            }
        }
    }

    // check for item type updates
    if (a.type() != QOrganizerItemType::TypeUndefined)
        if (b.type() != QOrganizerItemType::TypeUndefined)
            if (a.type() != b.type())
                return false; // nonempty type is different.

    // Now check to see if b has any details remaining; if so, a is not a superset.
    // Note that the DisplayLabel and Type can never be removed.
    if (b.details().size() > 2
            || (b.details().size() == 2 && (b.details().value(0).type() != QOrganizerItemDetail::TypeDisplayLabel
                                            || b.details().value(1).type() != QOrganizerItemDetail::TypeItemType)))
        return false;
    return true;
}

void tst_QOrganizerManager::dumpOrganizerItem(const QOrganizerItem& item)
{
    QOrganizerManager m;
    qDebug() << "OrganizerItem: " << item.id() << "(" << item.displayLabel() << ")";
    QList<QOrganizerItemDetail> details = item.details();
    foreach(QOrganizerItemDetail d, details) {
        qDebug() << "  " << d.type() << ":";
        qDebug() << "    Vals:" << d.values();
    }
}

void tst_QOrganizerManager::dumpOrganizerItems(QOrganizerManager *cm)
{
    QList<QOrganizerItemId> ids = cm->itemIds();

    qDebug() << "There are" << ids.count() << "items in" << cm->managerUri();

    foreach(QOrganizerItemId id, ids) {
        QOrganizerItem c = cm->item(id);
        dumpOrganizerItem(c);
    }
}

void tst_QOrganizerManager::uriParsing_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<bool>("good"); // is this a good uri or not
    QTest::addColumn<QString>("manager");
    QTest::addColumn<QMap<QString, QString> >("parameters");

    QMap<QString, QString> inparameters;
    inparameters.insert("foo", "bar");
    inparameters.insert("bazflag", QString());
    inparameters.insert("bar", "glob");

    QMap<QString, QString> inparameters2;
    inparameters2.insert("this has spaces", QString());
    inparameters2.insert("and& an", " &amp;");
    inparameters2.insert("and an ", "=quals");

    QTest::newRow("built") << QOrganizerManager::buildUri("manager", inparameters) << true << "manager" << inparameters;
    QTest::newRow("built with escaped parameters") << QOrganizerManager::buildUri("manager", inparameters2) << true << "manager" << inparameters2;
    QTest::newRow("no scheme") << "this should not split" << false << QString() << tst_QOrganizerManager_QStringMap();
    QTest::newRow("wrong scheme") << "invalidscheme:foo bar" << false << QString() << tst_QOrganizerManager_QStringMap();
    QTest::newRow("right scheme, no colon") << "qtorganizer" << false << QString() << tst_QOrganizerManager_QStringMap();
    QTest::newRow("no manager, colon, no params") << "qtorganizer::" << false  << "manager" << tst_QOrganizerManager_QStringMap();
    QTest::newRow("yes manager, no colon, no params") << "qtorganizer:manager" << true << "manager" << tst_QOrganizerManager_QStringMap();
    QTest::newRow("yes manager, yes colon, no params") << "qtorganizer:manager:" << true << "manager"<< tst_QOrganizerManager_QStringMap();
    QTest::newRow("yes params") << "qtorganizer:manager:foo=bar&bazflag=&bar=glob" << true << "manager" << inparameters;
    QTest::newRow("yes params but misformed") << "qtorganizer:manager:foo=bar&=gloo&bar=glob" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 2") << "qtorganizer:manager:=&=gloo&bar=glob" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 3") << "qtorganizer:manager:==" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 4") << "qtorganizer:manager:&&" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 5") << "qtorganizer:manager:&goo=bar" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 6") << "qtorganizer:manager:goo&bar" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 7") << "qtorganizer:manager:goo&bar&gob" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 8") << "qtorganizer:manager:==&&==&goo=bar" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 9") << "qtorganizer:manager:foo=bar=baz" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 10") << "qtorganizer:manager:foo=bar=baz=glob" << false << "manager" << inparameters;
    QTest::newRow("no manager but yes params") << "qtorganizer::foo=bar&bazflag=&bar=glob" << false << QString() << inparameters;
    QTest::newRow("no manager or params") << "qtorganizer::" << false << QString() << inparameters;
    QTest::newRow("no manager or params or colon") << "qtorganizer:" << false << QString() << inparameters;
}

void tst_QOrganizerManager::addManagers()
{
    QTest::addColumn<QString>("uri");

    QStringList managers = QOrganizerManager::availableManagers();

    /* Known one that will not pass */
    managers.removeAll("invalid");
    managers.removeAll("skeleton");
    managers.removeAll("testdummy");
    managers.removeAll("teststaticdummy");
    managers.removeAll("maliciousplugin");

    foreach(QString mgr, managers) {
        QMap<QString, QString> params;
        QTest::newRow(QString("mgr='%1'").arg(mgr).toLatin1().constData()) << QOrganizerManager::buildUri(mgr, params);
        if (mgr == "memory") {
            params.insert("id", "tst_QOrganizerManager");
            QTest::newRow(QString("mgr='%1', params").arg(mgr).toLatin1().constData()) << QOrganizerManager::buildUri(mgr, params);
        }
    }
}

void tst_QOrganizerManager::metadata()
{
    // ensure that the backend is publishing its metadata (name / parameters / uri) correctly
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(new QOrganizerManager("memory"));
    QVERIFY(QOrganizerManager::buildUri(cm->managerName(), cm->managerParameters()) == cm->managerUri());
}


void tst_QOrganizerManager::nullIdOperations()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(new QOrganizerManager("memory"));
    QVERIFY(!cm->removeItem(QOrganizerItemId()));
    QVERIFY(cm->error() == QOrganizerManager::DoesNotExistError);


    QOrganizerItem c = cm->item(QOrganizerItemId());
    QVERIFY(c.id() == QOrganizerItemId());
    QVERIFY(c.isEmpty());
    QVERIFY(cm->error() == QOrganizerManager::DoesNotExistError);


    /* TODO: rewrite tests using toString() / fromString()
    // test that streaming null ids doesn't crash.
    {
        QOrganizerItemId nullId;
        QByteArray buffer;
        QDataStream outBufferStream(&buffer, QIODevice::WriteOnly);
        outBufferStream << nullId;
        QVERIFY(buffer.length() > 0);

        QDataStream inBufferStream(buffer);
        QOrganizerItemId id;
        inBufferStream >> id;
        QVERIFY(id == nullId);
    }

    {
        QOrganizerItemId nullid;
        QByteArray buffer;
        QDataStream outBufferStream(&buffer, QIODevice::WriteOnly);
        outBufferStream << nullid;
        QVERIFY(buffer.length() > 0);

        // cannot stream in local ids; must stream in entire ids.
        //QDataStream inBufferStream(buffer);
        //QOrganizerItemId id;
        //inBufferStream >> id;
        //QVERIFY(id == nullid);
    }

    {
        QOrganizerCollectionId nullId;
        QByteArray buffer;
        QDataStream outBufferStream(&buffer, QIODevice::WriteOnly);
        outBufferStream << nullId;
        QVERIFY(buffer.length() > 0);

        QDataStream inBufferStream(buffer);
        QOrganizerCollectionId id;
        inBufferStream >> id;
        QVERIFY(id == nullId);
    }

    {
        QOrganizerCollectionId nullLocalId;
        QByteArray buffer;
        QDataStream outBufferStream(&buffer, QIODevice::WriteOnly);
        outBufferStream << nullLocalId;
        QVERIFY(buffer.length() > 0);

        // cannot stream in local ids; must stream in entire ids.
        //QDataStream inBufferStream(buffer);
        //QOrganizerCollectionLocalId id;
        //inBufferStream >> id;
        //QVERIFY(id == nullLocalId);
    }
    */
}

void tst_QOrganizerManager::uriParsing()
{
    QFETCH(QString, uri);
    QFETCH(bool, good);
    QFETCH(QString, manager);
    QFETCH(tst_QOrganizerManager_QStringMap, parameters);

    QString outmanager;
    QMap<QString, QString> outparameters;

    if (good) {
        /* Good split */
        /* Test splitting */
        QVERIFY(QOrganizerManager::parseUri(uri, 0, 0)); // no out parms

        // 1 out param
        QVERIFY(QOrganizerManager::parseUri(uri, &outmanager, 0));
        QCOMPARE(manager, outmanager);
        QVERIFY(QOrganizerManager::parseUri(uri, 0, &outparameters));

        QCOMPARE(parameters, outparameters);

        outmanager.clear();
        outparameters.clear();
        QVERIFY(QOrganizerManager::parseUri(uri, &outmanager, &outparameters));

        QCOMPARE(manager, outmanager);
        QCOMPARE(parameters, outparameters);
    } else {
        /* bad splitting */
        outmanager.clear();
        outparameters.clear();
        QVERIFY(QOrganizerManager::parseUri(uri, 0, 0) == false);
        QVERIFY(QOrganizerManager::parseUri(uri, &outmanager, 0) == false);
        QVERIFY(outmanager.isEmpty());
        QVERIFY(QOrganizerManager::parseUri(uri, 0, &outparameters) == false);
        QVERIFY(outparameters.isEmpty());

        /* make sure the in parameters don't change with a bad split */
        outmanager = manager;
        outparameters = parameters;
        QVERIFY(QOrganizerManager::parseUri(uri, &outmanager, 0) == false);
        QCOMPARE(manager, outmanager);
        QVERIFY(QOrganizerManager::parseUri(uri, 0, &outparameters) == false);
        QCOMPARE(parameters, outparameters);
    }
}

void tst_QOrganizerManager::ctors()
{
    /* test the different ctors to make sure we end up with the same uri */
    QVERIFY(QOrganizerManager::availableManagers().count() >= 1); // invalid, and probably something else
    QVERIFY(QOrganizerManager::availableManagers().contains("invalid"));
    QString defaultStore = QOrganizerManager::availableManagers().value(0);

    qDebug() << "Available managers:" << QOrganizerManager::availableManagers();

    QMap<QString, QString> randomParameters;
    randomParameters.insert("something", "old");
    randomParameters.insert("something...", "new");
    randomParameters.insert("something ", "borrowed");
    randomParameters.insert(" something", "blue");

    QObject parent;

    QOrganizerManager cm; // default
    QOrganizerManager cm2(defaultStore);
    QOrganizerManager cm3(defaultStore, QMap<QString, QString>());
    //QOrganizerManager cm4(cm.managerUri()); // should fail

    QScopedPointer<QOrganizerManager> cm5(QOrganizerManager::fromUri(QOrganizerManager::buildUri(defaultStore, QMap<QString, QString>())));
    QScopedPointer<QOrganizerManager> cm6(QOrganizerManager::fromUri(cm.managerUri())); // uri is not a name; should fail.
    QScopedPointer<QOrganizerManager> cm9(QOrganizerManager::fromUri(QString(), &parent));

    QVERIFY(cm9->parent() == &parent);

    /* OLD TEST WAS THIS: */
    //QCOMPARE(cm.managerUri(), cm2.managerUri());
    //QCOMPARE(cm.managerUri(), cm3.managerUri());
    //QCOMPARE(cm.managerUri(), cm5->managerUri());
    //QCOMPARE(cm.managerUri(), cm6->managerUri());
    //QCOMPARE(cm.managerUri(), cm9->managerUri());
    /* NEW TEST IS THIS: Test that the names of the managers are the same */
    QCOMPARE(cm.managerName(), cm2.managerName());
    QCOMPARE(cm.managerName(), cm3.managerName());
    QCOMPARE(cm.managerName(), cm5->managerName());
    QCOMPARE(cm.managerName(), cm6->managerName());
    QCOMPARE(cm.managerName(), cm9->managerName());

    //QVERIFY(cm.managerUri() != cm4.managerUri());

    /* Test that we get invalid stores when we do silly things */
    QOrganizerManager em("non existent");
    QOrganizerManager em2("non existent", QMap<QString, QString>());
    QOrganizerManager em3("memory", randomParameters);

    /* Also invalid, since we don't have one of these anyway */
    QScopedPointer<QOrganizerManager> em4(QOrganizerManager::fromUri("invalid uri"));
    QScopedPointer<QOrganizerManager> em5(QOrganizerManager::fromUri(QOrganizerManager::buildUri("nonexistent", QMap<QString, QString>())));
    QScopedPointer<QOrganizerManager> em6(QOrganizerManager::fromUri(em3.managerUri()));


    /*
     * Sets of stores that should be equivalent:
     * - 1, 2, 4, 5
     * - 3, 6
     */

    /* First some URI testing for equivalent stores */
    QVERIFY(em.managerUri() == em2.managerUri());
    QVERIFY(em.managerUri() == em5->managerUri());
    QVERIFY(em.managerUri() == em4->managerUri());
    QVERIFY(em2.managerUri() == em4->managerUri());
    QVERIFY(em2.managerUri() == em5->managerUri());
    QVERIFY(em4->managerUri() == em5->managerUri());

    QVERIFY(em3.managerUri() == em6->managerUri());

    /* Test the stores that should not be the same */
    QVERIFY(em.managerUri() != em3.managerUri());
    QVERIFY(em.managerUri() != em6->managerUri());

    /* now the components */
    QCOMPARE(em.managerName(), QString("invalid"));
    QCOMPARE(em2.managerName(), QString("invalid"));
    QCOMPARE(em3.managerName(), QString("memory"));
    QCOMPARE(em4->managerName(), QString("invalid"));
    QCOMPARE(em5->managerName(), QString("invalid"));
    QCOMPARE(em6->managerName(), QString("memory"));
    QCOMPARE(em.managerParameters(), tst_QOrganizerManager_QStringMap());
    QCOMPARE(em2.managerParameters(), tst_QOrganizerManager_QStringMap());
    QCOMPARE(em4->managerParameters(), tst_QOrganizerManager_QStringMap());
    QCOMPARE(em5->managerParameters(), tst_QOrganizerManager_QStringMap());
    QCOMPARE(em3.managerParameters(), em6->managerParameters()); // memory engine discards the given params, replaces with id.


    // Finally test the platform specific engines are actually the defaults
    QCOMPARE(defaultStore, QString("invalid"));
}

void tst_QOrganizerManager::doDump()
{
    // Only do this if it has been explicitly selected
    if (QCoreApplication::arguments().contains("doDump")) {
        QFETCH(QString, uri);
        QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

        dumpOrganizerItems(cm.data());
    }
}

Q_DECLARE_METATYPE(QVariant)

void tst_QOrganizerManager::add()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    // Use note & todo item depending on backend support
    QOrganizerItemType::ItemType type;
    if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeNote))
        type = QOrganizerItemType::TypeNote;
    else if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeTodo))
        type = QOrganizerItemType::TypeTodo;
    else
        QSKIP("This manager does not support note or todo item");

    QOrganizerItem item;
    item.setType(type);
    item.setDisplayLabel("This is a note");
    item.setDescription("This note is a particularly notey note");
    int currCount = cm->itemIds().count();
    QVERIFY(cm->saveItem(&item));
    QVERIFY(cm->error() == QOrganizerManager::NoError);

    QVERIFY(!item.id().managerUri().isEmpty());
    QVERIFY(!item.id().isNull());
    QCOMPARE(cm->itemIds().count(), currCount+1);

    QOrganizerItem added = cm->item(item.id());
    QVERIFY(added.id() == item.id());

    if (!isSuperset(added, item)) {
        // XXX TODO: fix the isSuperset so that it ignores timestamps.
        //dumpOrganizerItems(cm.data());
        //dumpOrganizerItemDifferences(added, note);
        QCOMPARE(added, item);
    }

    // now try adding an item that does not exist in the database with non-zero id
    QOrganizerItem nonexistentItem;
    nonexistentItem.setType(type);
    nonexistentItem.setDisplayLabel("Another note.");
    nonexistentItem.setDescription("This is `another note`'s description");
    QVERIFY(cm->saveItem(&nonexistentItem));       // should work
    QVERIFY(cm->removeItem(nonexistentItem.id())); // now nonexistentItem has an id which does not exist

    QVERIFY(!cm->saveItem(&nonexistentItem));      // hence, should fail
    QCOMPARE(cm->error(), QOrganizerManager::DoesNotExistError);
    nonexistentItem.setId(QOrganizerItemId());
    QVERIFY(cm->saveItem(&nonexistentItem));       // after setting id to zero, should save
    QVERIFY(cm->removeItem(nonexistentItem.id()));

    // now try adding a "megaevent"
    // - get list of all definitions supported by the manager
    // - add one detail of each definition to a item
    // - save the item
    // - read it back
    // - ensure that it's the same.
#if 0 // This test is currently excluded; can it be reinstated?
    QOrganizerEvent megaevent;
    QMap<QString, QOrganizerItemDetailDefinition> defmap = cm->detailDefinitions(QOrganizerItemType::TypeEvent);
    QList<QOrganizerItemDetailDefinition> defs = defmap.values();
    foreach (const QOrganizerItemDetailDefinition def, defs) {

        // This is probably read-only
        if (def.name() == QOrganizerItemTimestamp::DefinitionName)
            continue;

        // otherwise, create a new detail of the given type and save it to the item
        QOrganizerItemDetail det(def.name());
        QMap<QString, QOrganizerItemDetailFieldDefinition> fieldmap = def.fields();
        QStringList fieldKeys = fieldmap.keys();
        foreach (const QString& fieldKey, fieldKeys) {
            // get the field, and check to see that it's not constrained.
            QOrganizerItemDetailFieldDefinition currentField = fieldmap.value(fieldKey);

            // Attempt to create a worthy value
            if (!currentField.allowableValues().isEmpty()) {
                // we want to save a value that will be accepted.
                if (currentField.dataType() == QVariant::StringList)
                    det.setValue(fieldKey, QStringList() << currentField.allowableValues().first().toString());
                else if (currentField.dataType() == QVariant::List)
                    det.setValue(fieldKey, QVariantList() << currentField.allowableValues().first());
                else
                    det.setValue(fieldKey, currentField.allowableValues().first());
            } else {
                // any value of the correct type will be accepted
                bool savedSuccessfully = false;
                QVariant dummyValue = QVariant(fieldKey); // try to get some unique string data
                if (currentField.dataType() < static_cast<int>(QVariant::UserType)) {
                    QVariant::Type type = static_cast<QVariant::Type>(currentField.dataType());
                    // It is not a user-defined type
                    if (dummyValue.canConvert(type)) {
                        savedSuccessfully = dummyValue.convert(type);
                        if (savedSuccessfully) {
                            // we have successfully created a (supposedly) valid field for this detail.
                            det.setValue(fieldKey, dummyValue);
                            continue;
                        }
                    }

                    // nope, couldn't save the string value (test); try a date.
                    dummyValue = QVariant(QDate::currentDate());
                    if (dummyValue.canConvert(type)) {
                        savedSuccessfully = dummyValue.convert(type);
                        if (savedSuccessfully) {
                            // we have successfully created a (supposedly) valid field for this detail.
                            det.setValue(fieldKey, dummyValue);
                            continue;
                        }
                    }

                    // nope, couldn't convert a string or a date - try the integer value (42)
                    dummyValue = QVariant(42);
                    if (dummyValue.canConvert(type)) {
                        savedSuccessfully = dummyValue.convert(type);
                        if (savedSuccessfully) {
                            // we have successfully created a (supposedly) valid field for this detail.
                            det.setValue(fieldKey, dummyValue);
                            continue;
                        }
                    }
                }

                // if we get here, we don't know what sort of value can be saved...
            }
        }
        if (!det.isEmpty())
            megaevent.saveDetail(&det);
    }

    QVERIFY(cm->saveItem(&megaevent)); // must be able to save since built from definitions.
    QOrganizerItem retrievedMegaitem = cm->item(megaevent.id());
    if (!isSuperset(retrievedMegaitem, megaevent)) {
        dumpOrganizerItemDifferences(megaevent, retrievedMegaitem);
    }

    // now a item with many details of a particular definition
    // if the detail is not unique it should then support minumum of two of the same kind
    // const int nrOfdetails = 2;
    // XXX TODO.
#endif
}

void tst_QOrganizerManager::saveRecurrence()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel(QStringLiteral("meeting"));
    event.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 1, 1), QTime(12, 0, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    rrule.setLimit(QDate(2011, 1, 1));
    rrule.setInterval(2);
    rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Monday << Qt::Tuesday);
    rrule.setDaysOfMonth(QSet<int>() << 1 << 2);
    rrule.setDaysOfYear(QSet<int>() << 1 << 2);
    rrule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
            << QOrganizerRecurrenceRule::January
            << QOrganizerRecurrenceRule::February);
    rrule.setWeeksOfYear(QSet<int>() << 1 << 2);
    rrule.setFirstDayOfWeek(Qt::Tuesday);

    //this is disabled because certain backend doesn't support it.
    //rrule.setPositions(QSet<int>() << 1 << 2);

    event.setRecurrenceRule(rrule);
    event.setExceptionRule(rrule);
    QSet<QDate> rdates;
    rdates << QDate(2010, 1, 4) << QDate(2010, 4, 1);
    event.setRecurrenceDates(rdates);
    event.setExceptionDates(rdates);

    QVERIFY(cm->saveItem(&event));
    QOrganizerEvent savedEvent(cm->item(event.id()));
    QCOMPARE(event.recurrenceRule(), savedEvent.recurrenceRule());
    QCOMPARE(event.exceptionRule(), savedEvent.exceptionRule());
    QCOMPARE(event.recurrenceDates(), savedEvent.recurrenceDates());
    QCOMPARE(event.exceptionDates(), savedEvent.exceptionDates());
}

void tst_QOrganizerManager::persistence()
{
    // Test that changes in one manager are visible from another
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));
    if (cm->managerName() == "memory")
        return; // memory engine is not persistent

    cm->removeItems(cm->itemIds());
    QScopedPointer<QOrganizerManager> cm2(QOrganizerManager::fromUri(uri));
    QCOMPARE(cm->items().size(), 0);

    // Add an event
    QOrganizerEvent event;
    event.setDisplayLabel(QStringLiteral("meeting"));
    event.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 1, 1), QTime(12, 0, 0)));
    QVERIFY(cm->saveItem(&event));
    QTest::qWait(500);
    QCOMPARE(cm->items().size(), 1);
    QCOMPARE(cm2->items().size(), 1);

    // Remove the event
    cm->removeItems(cm->itemIds());
    QTest::qWait(500);
    QCOMPARE(cm->items().size(), 0);
    QCOMPARE(cm2->items().size(), 0);

//#if 0
    // This is disabled because it'll fail on managers that don't support collections

    // Remove all non-default collections
    QList<QOrganizerCollection> collections(cm->collections());
    QOrganizerCollectionId defaultCollectionId(cm->defaultCollectionId());
    foreach (const QOrganizerCollection &col, collections) {
        QOrganizerCollectionId id(col.id());
        if (id != defaultCollectionId)
            cm->removeCollection(id);
    }
    QTest::qWait(500);
    QCOMPARE(cm2->collections().size(), cm->collections().size());

    QOrganizerCollection collection;
    collection.setMetaData(QOrganizerCollection::KeyName, QStringLiteral("test collection"));
    QVERIFY(cm->saveCollection(&collection));
    QTest::qWait(500);
    QCOMPARE(cm2->collections().size(), cm->collections().size());
//#endif
}

void tst_QOrganizerManager::addExceptions()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel(QStringLiteral("meeting"));
    event.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 1, 1), QTime(12, 0, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    rrule.setLimit(3);
    event.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event));
    QVERIFY(!event.id().isNull());
    event = cm->item(event.id());
    // the guid must be set so when it is exported to iCalendar, the relationship can be represented
    QVERIFY(!event.guid().isEmpty());

    QList<QOrganizerItem> items =
        cm->itemOccurrences(event, QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)),
                                 QDateTime(QDate(2010, 2, 1), QTime(0, 0, 0)));
    QCOMPARE(items.size(), 3);
    QOrganizerItem secondItem = items.at(1);
    QCOMPARE(secondItem.type(), QOrganizerItemType::TypeEventOccurrence);
    QOrganizerEventOccurrence secondEvent = static_cast<QOrganizerEventOccurrence>(secondItem); // not sure this is the best way...
    QCOMPARE(secondEvent.startDateTime(), QDateTime(QDate(2010, 1, 8), QTime(11, 0, 0)));
    QCOMPARE(secondEvent.id(), QOrganizerItemId());
    QCOMPARE(secondEvent.parentId(), event.id());

    // save a change to an occurrence's detail (ie. create an exception)
    secondEvent.setDisplayLabel(QStringLiteral("seminar"));
    QVERIFY(cm->saveItem(&secondEvent));
    event = cm->item(event.id()); // reload the event to pick up any changed exception dates.
    items = cm->itemOccurrences(event, QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)),
                                     QDateTime(QDate(2010, 2, 1), QTime(0, 0, 0)));
    QCOMPARE(items.size(), 3); // shouldn't change the count.

    // save a change to an occurrence's time
    QOrganizerEventOccurrence thirdEvent = static_cast<QOrganizerEventOccurrence>(items.at(2));
    QCOMPARE(thirdEvent.id(), QOrganizerItemId());
    QCOMPARE(thirdEvent.parentId(), event.id());
    thirdEvent.setStartDateTime(QDateTime(QDate(2010, 1, 15), QTime(13, 0, 0)));
    thirdEvent.setEndDateTime(QDateTime(QDate(2010, 1, 15), QTime(14, 0, 0)));
    QVERIFY(cm->saveItem(&thirdEvent));
    event = cm->item(event.id()); // reload the event to pick up any changed exception dates.
    items = cm->itemOccurrences(event, QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)),
                                     QDateTime(QDate(2010, 2, 1), QTime(0, 0, 0)));
    QCOMPARE(items.size(), 3); // shouldn't change the count.

    QOrganizerItem firstItem;
    bool foundFirst = false;
    bool foundSecond = false;
    bool foundThird = false;
    foreach (const QOrganizerItem& item, items) {
        if (item.id().isNull()) {
            foundFirst = true;
            firstItem = item;
        }

        if (item.id() == secondEvent.id()) {
            foundSecond = true;
            secondEvent = item;
        }

        if (item.id() == thirdEvent.id()) {
            foundThird = true;
            thirdEvent = item;
        }
    }

    // check that saving an exception doesn't change other items
    QVERIFY(foundFirst); // there should still be one "generated" occurrence
    QCOMPARE(firstItem.displayLabel(), QStringLiteral("meeting")); // and it should have the original label.
    // item occurrences which are not exceptions should have zero id
    QVERIFY(firstItem.id().isNull());

    // the exception's changes have been persisted
    QVERIFY(foundSecond);
    QCOMPARE(secondEvent.displayLabel(), QStringLiteral("seminar"));
    // item occurrences which are persisted exceptions should have non-zero id
    QVERIFY(!secondEvent.id().isNull());

    QVERIFY(foundThird);
    QCOMPARE(thirdEvent.startDateTime(), QDateTime(QDate(2010, 1, 15), QTime(13, 0, 0)));
    QCOMPARE(thirdEvent.endDateTime(), QDateTime(QDate(2010, 1, 15), QTime(14, 0, 0)));
    QVERIFY(!thirdEvent.id().isNull());

    // remove persisted exception, it should not modify parent item's exception dates
    cm->removeItem(secondEvent.id());
    event = cm->item(event.id()); // reload the event to pick up any changed exception dates.
    items = cm->itemOccurrences(event, QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)),
                                     QDateTime(QDate(2010, 2, 1), QTime(0, 0, 0)));
    QCOMPARE(items.size(), 2); // shouldn't change the count.

    // remove the parent item, persisted exceptions should be removed also

    cm->removeItem(event.id());
    QVERIFY(cm->item(thirdEvent.id()).isEmpty());

}

void tst_QOrganizerManager::modifyRecurrence()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel(QStringLiteral("recurring meeting"));
    event.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 1, 1), QTime(12, 0, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
    rrule.setLimit(QDate(2014, 1, 1));
    event.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event));
    QVERIFY(!event.id().isNull());
    event = cm->item(event.id());
    // the guid must be set so when it is exported to iCalendar, the relationship can be represented
    QVERIFY(!event.guid().isEmpty());
    QList<QOrganizerItem> items = cm->itemOccurrences(event);
    QCOMPARE(items.size(), 5);
    QOrganizerItem secondItem = items.at(1);
    QCOMPARE(secondItem.type(), QOrganizerItemType::TypeEventOccurrence);
    QOrganizerEventOccurrence secondEvent = static_cast<QOrganizerEventOccurrence>(secondItem); // not sure this is the best way...
    QCOMPARE(secondEvent.startDateTime(), QDateTime(QDate(2011, 1, 1), QTime(11, 0, 0)));
    QCOMPARE(secondEvent.id(), QOrganizerItemId());
    QCOMPARE(secondEvent.parentId(), event.id());

    // save a change to an occurrence's detail (ie. create an exception)
    secondEvent.setDisplayLabel(QStringLiteral("exceptional display label"));
    QVERIFY(cm->saveItem(&secondEvent));
    event = cm->item(event.id()); // reload the event to pick up any changed exception dates.
    items = cm->itemOccurrences(event);
    QCOMPARE(items.size(), 5); // shouldn't change the count.

    // save a change to an occurrence's time
    QOrganizerEventOccurrence thirdEvent = static_cast<QOrganizerEventOccurrence>(items.at(2));
    QCOMPARE(thirdEvent.id(), QOrganizerItemId());
    QCOMPARE(thirdEvent.parentId(), event.id());
    thirdEvent.setStartDateTime(QDateTime(QDate(2012, 1, 2), QTime(15, 0, 0)));
    thirdEvent.setEndDateTime(QDateTime(QDate(2012, 1, 2), QTime(16, 0, 0)));
    QVERIFY(cm->saveItem(&thirdEvent));
    event = cm->item(event.id()); // reload the event to pick up any changed exception dates.
    items = cm->itemOccurrences(event);
    QCOMPARE(items.size(), 5); // shouldn't change the count.

    QCOMPARE(items[0].id(), QOrganizerItemId());
    QCOMPARE(items[1].id(), secondEvent.id());
    QCOMPARE(items[2].id(), thirdEvent.id());
    QCOMPARE(items[3].id(), QOrganizerItemId());
    QCOMPARE(items[4].id(), QOrganizerItemId());

    // modify recurrence rule
    rrule.setLimit(QDate(2011, 1, 1));
    event.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event));
    items = cm->itemOccurrences(event);
    QCOMPARE(items.size(), 2); // only one generated and one exception should be left

    // use different function to fetch events
    items = cm->items();
    QCOMPARE(items.size(), 2); // only one generated and one exception should be left
}


/*test ItemOccurrences function maxCount parameters*/
void tst_QOrganizerManager::testItemOccurrences()
{
    //Get the uri value
    QFETCH (QString, uri);
    //Create the QOrganizerManager
    QScopedPointer<QOrganizerManager> cm (QOrganizerManager::fromUri (uri));

    //Create the weekly recurrence event data that is used by the test
    QOrganizerEvent event;
    event.setDisplayLabel (QStringLiteral ("meeting"));
    event.setStartDateTime (QDateTime (QDate (2010, 1, 1), QTime (11, 0, 0)));
    event.setEndDateTime (QDateTime (QDate (2010, 1, 1), QTime (12, 0, 0)));

    //Create weekly recurrence rule and count limit is 3
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency (QOrganizerRecurrenceRule::Weekly);
    rrule.setLimit (3);
    event.setRecurrenceRule (rrule);

    //Save the event
    QVERIFY (cm->saveItem (&event));

    //Varify the event data guid
    QVERIFY (!event.id().isNull ());
    event = cm->item (event.id ());
    //The guid(Globally unique identifier) must be set so when it is exported to iCalendar, the relationship can be represented
    QVERIFY (!event.guid().isEmpty ());

    //Use default parameter value to fetch
    QList<QOrganizerItem> items = cm->itemOccurrences(
                                            event,     //parantItem
                                            QDateTime (QDate (2010, 1, 1), QTime (0, 0, 0)), //start date
                                            QDateTime (QDate (2010, 2, 1), QTime (0, 0, 0))  //end date
                                            );
    //The result should be same as rrule's limitation 3
    QCOMPARE(items.size(), 3);

    //Assign maxCount negative value to get same result as default value
    QList<QOrganizerItem> items2 = cm->itemOccurrences(
                                            event,     //parantItem
                                            QDateTime (QDate (2010, 1, 1), QTime (0, 0, 0)), //start date
                                            QDateTime (QDate (2010, 2, 1), QTime (0, 0, 0)), //end date
                                            -5   //maxCount
                                            );
    //The result should be same as rrule's limitation
    QCOMPARE(items2.size(), 3);

    //Assign maxCount bigger value to get same result as default value
    QList<QOrganizerItem> items3 = cm->itemOccurrences(
                                            event,
                                            QDateTime (QDate (2010, 1, 1), QTime (0, 0, 0)),
                                            QDateTime (QDate (2010, 2, 1), QTime (0, 0, 0)),
                                            4
                                            );
    //The result should be same as rrule's limitation since the maxCount is bigger than the actually data
    QCOMPARE(items3.size(), 3);

/*------------------------------------------------------------------------
    Create 2nd Daily event data, now We have Weekly and Daily 2 different events and make the situation more complex.
    maxCount is also smaller than the actual item data.
*/
    //Create 2nd Daily event data and limitation is 20
    QOrganizerEvent event2;
    event2.setDisplayLabel (QStringLiteral ("meeting2"));
    event2.setStartDateTime (QDateTime (QDate (2010, 1, 1), QTime (11, 0, 0)));
    event2.setEndDateTime (QDateTime (QDate (2010, 1, 1), QTime (12, 0, 0)));
    rrule.setFrequency (QOrganizerRecurrenceRule::Daily);
    rrule.setLimit (20);
    event2.setRecurrenceRule (rrule);
    QVERIFY (cm->saveItem (&event2));
    event2 = cm->item (event2.id ());
    // The guid must be set so when it is exported to iCalendar, the relationship can be represented
    QVERIFY (!event2.guid().isEmpty ());

    //Use default maxCount parameter value to fetch
    QList<QOrganizerItem> items4 = cm->itemOccurrences(
                                            event2,
                                            QDateTime (QDate (2010, 1, 1), QTime (0, 0, 0)),
                                            QDateTime (QDate (2010, 2, 1), QTime (0, 0, 0))
                                            );

    //The result should be same as rrule's limitation 20
    QCOMPARE (items4.size (), 20);

    //Normal daily event items fetch with 5 max count limitation
    QList<QOrganizerItem> items5 = cm->itemOccurrences(
                                            event2,
                                            QDateTime (QDate (2010, 1, 1), QTime (0, 0, 0)),
                                            QDateTime (QDate (2010, 2, 1), QTime (0, 0, 0)),
                                            5
                                            );
    //The result should be same as maxCount 5 which is smaller than event rrule's limitation 20
    QCOMPARE (items5.size (), 5);

    //Normal daily event items fetch with 0 max count limitation
    QList<QOrganizerItem> items6 = cm->itemOccurrences(
                                            event2,
                                            QDateTime (QDate (2010, 1, 1), QTime (0, 0, 0)),
                                            QDateTime (QDate (2010, 2, 1), QTime (0, 0, 0)),
                                            0
                                            );
    //The result should be 0
    QCOMPARE (items6.size (), 0);
}

void tst_QOrganizerManager::addExceptionsWithGuid()
{
    // It should be possible to save an exception that has at least an originalDate and either a
    // guid or a parentId.  If guid and parentId are both specified, the client should
    // ensure they are consistent and the manager should fail if they are not.  If only one of the
    // guid or parentId are specified, the manager should generate the other one.
    // This test case tests all of this.
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    // Set up some recurring items
    QOrganizerEvent christmas;
    christmas.setGuid("christmas");
    christmas.setStartDateTime(QDateTime(QDate(2009, 12, 25), QTime(0, 0, 0)));
    christmas.setEndDateTime(QDateTime(QDate(2009, 12, 26), QTime(0, 0, 0)));
    christmas.setDisplayLabel(QStringLiteral("Christmas"));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
    christmas.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&christmas));
    QVERIFY(!christmas.id().managerUri().isEmpty());
    QVERIFY(!christmas.id().isNull());

    QOrganizerEvent newYearsDay;
    newYearsDay.setGuid("newyear");
    newYearsDay.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)));
    newYearsDay.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(0, 0, 0)));
    newYearsDay.setDisplayLabel(QStringLiteral("New Years Day"));
    newYearsDay.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&newYearsDay));

    QOrganizerTodo report;
    report.setGuid("report");
    report.setDueDateTime(QDateTime(QDate(2010, 1, 11), QTime(0, 0, 0)));
    report.setDisplayLabel(QStringLiteral("Report"));
    QVERIFY(cm->saveItem(&report));

    // The tests:
    // exception with no guid or parentId fails
    QOrganizerEventOccurrence exception;
    exception.setOriginalDate(QDate(2010, 12, 25));
    exception.setStartDateTime(QDateTime(QDate(2010, 12, 25), QTime(0, 0, 0)));
    exception.setEndDateTime(QDateTime(QDate(2010, 12, 26), QTime(0, 0, 0)));
    exception.setDisplayLabel(QStringLiteral("Xmas"));
//    if (cm->detailDefinitions(QOrganizerItemType::TypeEventOccurrence).contains(QOrganizerItemComment::DefinitionName))
//        exception.addComment(QLatin1String("With the in-laws"));
    QVERIFY(!cm->saveItem(&exception));
    QCOMPARE(cm->error(), QOrganizerManager::InvalidOccurrenceError);

    // exception with invalid guid fails
    exception.setId(QOrganizerItemId());
    exception.setGuid(QStringLiteral("halloween"));
    QVERIFY(!cm->saveItem(&exception));
    QCOMPARE(cm->error(), QOrganizerManager::InvalidOccurrenceError);

    // with the guid set, it should work
    exception.setId(QOrganizerItemId());
    exception.setGuid(QStringLiteral("christmas"));
    QVERIFY(cm->saveItem(&exception));
    QVERIFY(!exception.id().isNull());
    QOrganizerEventOccurrence savedException = cm->item(exception.id());
    QCOMPARE(savedException.parentId(), christmas.id()); // parentId should be set by manager

    // with the id, guid and the parentId all set and consistent, it should work
    exception = savedException;
    QVERIFY(cm->saveItem(&exception));
    savedException = cm->item(exception.id());
    QCOMPARE(savedException.parentId(), christmas.id());

    // Make a fresh exception object on a fresh date to avoid clashing with the previously saved one
    // can't set parentId to a non-event
    QOrganizerEventOccurrence exception2;
    exception2.setOriginalDate(QDate(2011, 12, 25));
    exception2.setStartDateTime(QDateTime(QDate(2011, 12, 25), QTime(0, 0, 0)));
    exception2.setEndDateTime(QDateTime(QDate(2011, 12, 26), QTime(0, 0, 0)));
    exception2.setDisplayLabel(QStringLiteral("XMas"));
//    if (cm->detailDefinitions(QOrganizerItemType::TypeEventOccurrence).contains(QOrganizerItemComment::DefinitionName))
//        exception2.addComment(QLatin1String("With the in-laws"));
    exception2.setParentId(report.id()); // report is not an event
    QVERIFY(!cm->saveItem(&exception2));
    QCOMPARE(cm->error(), QOrganizerManager::InvalidOccurrenceError);

    // can't set guid to a non-event
    exception2.setGuid(QStringLiteral("report"));
    exception2.setParentId(QOrganizerItemId());
    QVERIFY(!cm->saveItem(&exception2));
    QCOMPARE(cm->error(), QOrganizerManager::InvalidOccurrenceError);

    // can't make the guid inconsistent with the parentId
    exception2.setParentId(christmas.id());
    exception2.setGuid(QStringLiteral("newyear"));
    QVERIFY(!cm->saveItem(&exception2));
    QCOMPARE(cm->error(), QOrganizerManager::InvalidOccurrenceError);

    // with just the parentId set to a valid parent, it should work
    exception2.setGuid(QStringLiteral(""));
    QVERIFY(cm->saveItem(&exception2));
    savedException = cm->item(exception2.id());
    QCOMPARE(savedException.parentId(), christmas.id());
    QCOMPARE(savedException.guid(), QStringLiteral("christmas")); // guid should be set by manager

    // Make a fresh exception object on a fresh date to avoid clashing with the previously saved one
    // exception without originalDate fails
    QOrganizerEventOccurrence exception3;
    exception3.setStartDateTime(QDateTime(QDate(2012, 12, 25), QTime(0, 0, 0)));
    exception3.setEndDateTime(QDateTime(QDate(2012, 12, 26), QTime(0, 0, 0)));
    exception3.setDisplayLabel(QStringLiteral("XMas"));
//    if (cm->detailDefinitions(QOrganizerItemType::TypeEventOccurrence).contains(QOrganizerItemComment::DefinitionName))
//        exception3.addComment(QLatin1String("With the in-laws"));
    exception3.setParentId(christmas.id());
    exception3.setGuid(QStringLiteral("christmas"));
    QVERIFY(!cm->saveItem(&exception3));
    QCOMPARE(cm->error(), QOrganizerManager::InvalidOccurrenceError);

    // with original date, guid and parentId set and consistent, and id=0, it should work
    exception3.setOriginalDate(QDate(2012, 12, 25));
    QVERIFY(cm->saveItem(&exception3));
    QVERIFY(!exception3.id().isNull());
    savedException = cm->item(exception3.id());
    QCOMPARE(savedException.parentId(), christmas.id());
}

void tst_QOrganizerManager::update()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    // Use note & todo item depending on backend support
    QOrganizerItemType::ItemType type;
    if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeNote))
        type = QOrganizerItemType::TypeNote;
    else if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeTodo))
        type = QOrganizerItemType::TypeTodo;
    else
        QSKIP("This manager does not support note or todo item");

    // Save a new item first
    QOrganizerItem item;
    item.setType(type);
    item.setDisplayLabel("Yet another note");
    item.setDescription("Surprisingly, this note is not a particularly notey note");
    QVERIFY(cm->saveItem(&item));
    QVERIFY(cm->error() == QOrganizerManager::NoError);

    // Update name
    QOrganizerItemDescription descr = item.detail(QOrganizerItemDetail::TypeDescription);
    descr.setDescription("This note is now slightly noteworthy");
    item.saveDetail(&descr);
    QVERIFY(cm->saveItem(&item));
    QVERIFY(cm->error() == QOrganizerManager::NoError);
    descr.setDescription("This is a very noteworthy note");
    item.saveDetail(&descr);
    QVERIFY(cm->saveItem(&item));
    QVERIFY(cm->error() == QOrganizerManager::NoError);
    QOrganizerItem updated = cm->item(item.id());
    QOrganizerItemDescription updatedDescr = updated.detail(QOrganizerItemDetail::TypeDescription);
    QCOMPARE(updatedDescr, descr);

    /* Create a recurring event, update an occurrence and save (should persist as an exceptional occurrence) */
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerEvent recEvent;
    recEvent.setDescription("a recurring event");
    recEvent.setStartDateTime(QDateTime(QDate(2010, 10, 20), QTime(8, 0, 0)));
    recEvent.setEndDateTime(QDateTime(QDate(2010, 10, 20), QTime(10, 0, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(3);
    recEvent.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&recEvent));
    int persistentCount = cm->itemsForExport().size();
    QCOMPARE(persistentCount, 1); // just the parent
    QList<QOrganizerItem> items = cm->items();
    QCOMPARE(items.size(), 3);
    bool foundException = false;
    QOrganizerEventOccurrence exception;
    foreach (const QOrganizerEventOccurrence& curr, items) {
        if (curr.startDateTime() == QDateTime(QDate(2010, 10, 21), QTime(8, 0, 0))) {
            exception = curr;
            foundException = true;
            break;
        }
    }
    QVERIFY(foundException);
    exception.setDescription("different description");
    QVERIFY(cm->saveItem(&exception));
    persistentCount = cm->itemsForExport().size();
    QCOMPARE(persistentCount, 2); // parent plus one exception
    items = cm->items();
    QCOMPARE(items.size(), 3);
    foreach (const QOrganizerEventOccurrence& curr, items) {
        if (curr.startDateTime() == QDateTime(QDate(2010, 10, 21), QTime(8, 0, 0))) {
            QVERIFY(!curr.id().isNull());
            QCOMPARE(curr.description(), QStringLiteral("different description"));
        } else {
            QVERIFY(curr.id().isNull());
        }
    }

    /* Save a non-updated occurrence - should still persist as an exceptional occurrence */
    QOrganizerEventOccurrence secondException;
    foreach (const QOrganizerEventOccurrence& curr, items) {
        if (curr.startDateTime() == QDateTime(QDate(2010, 10, 22), QTime(8, 0, 0))) {
            secondException = curr;
            foundException = true;
            break;
        }
    }
    QVERIFY(foundException);
    QVERIFY(cm->saveItem(&secondException)); // no changes, but should save as an exception anyway.
    persistentCount = cm->itemsForExport().size();
    QCOMPARE(persistentCount, 3); // parent plus two exceptions
    items = cm->items();
    QCOMPARE(items.size(), 3);
    foreach (const QOrganizerEventOccurrence& curr, items) {
        if (curr.startDateTime() == QDateTime(QDate(2010, 10, 20), QTime(8, 0, 0))) {
            QVERIFY(curr.id().isNull());  // only the first occurrence is not an exception.
        } else {
            QVERIFY(!curr.id().isNull()); // we have two exceptions this time
        }
    }
}

void tst_QOrganizerManager::remove()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));
    // for fetch results
    QList<QOrganizerItem> items;
    // for removeItems request
    QList<QOrganizerItem> itemList;

    // Use note & todo item depending on backend support
    QOrganizerItemType::ItemType type;
    if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeNote))
        type = QOrganizerItemType::TypeNote;
    else if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeTodo))
        type = QOrganizerItemType::TypeTodo;
    else
        QSKIP("This manager does not support note or todo item");

    // Save a new item first
    QOrganizerItem item;
    item.setType(type);
    item.setDisplayLabel("Not another note");
    item.setDescription("Yes, another note!");
    QVERIFY(cm->saveItem(&item));
    QVERIFY(cm->error() == QOrganizerManager::NoError);
    QVERIFY(item.id() != QOrganizerItemId());

    // Remove the created item
    const int itemCount = cm->itemIds().count();
    QVERIFY(cm->removeItem(item.id()));
    QCOMPARE(cm->itemIds().count(), itemCount - 1);
    QVERIFY(cm->item(item.id()).isEmpty());
    QCOMPARE(cm->error(), QOrganizerManager::DoesNotExistError);

    //delete item not exist
    QVERIFY(!cm->removeItem(item.id()));
    QCOMPARE(cm->error(), QOrganizerManager::DoesNotExistError);

    //delete item id is empty
    QOrganizerItemId id;
    QVERIFY(!cm->removeItem(id));
    QCOMPARE(cm->error(), QOrganizerManager::DoesNotExistError);

    // delete an event by item (not id)
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QCOMPARE(cm->items().size(), 0);
    QOrganizerEvent event1;
    event1.setDisplayLabel("first event");
    QOrganizerEvent event2;
    event1.setDisplayLabel("second event");
    QOrganizerEvent event3;
    event1.setDisplayLabel("third event");
    itemList << event1 << event2 << event3;
    QVERIFY(cm->saveItems(&itemList));
    QVERIFY(cm->error() == QOrganizerManager::NoError);
    items = cm->items();
    QCOMPARE(cm->items().size(), 3);
    QVERIFY(cm->removeItem(&(items[2])));
    items = cm->items();
    QCOMPARE(cm->items().size(), 2);
    QVERIFY(cm->removeItems(&items));
    items = cm->items();
    QCOMPARE(cm->items().size(), 0);

    /* Create a recurring event, save an exception, remove the recurring event should remove all children occurrences incl. persisted exceptions. */
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerEvent recEvent;
    recEvent.setDescription("a recurring event");
    recEvent.setStartDateTime(QDateTime(QDate(2010, 10, 20), QTime(8, 0, 0)));
    recEvent.setEndDateTime(QDateTime(QDate(2010, 10, 20), QTime(10, 0, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(3);
    recEvent.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&recEvent));
    items = cm->items();
    QCOMPARE(items.size(), 3);
    QOrganizerEventOccurrence exception = items.at(1);
    exception.setStartDateTime(QDateTime(QDate(2010, 10, 21), QTime(7, 0, 0)));
    QVERIFY(cm->saveItem(&exception));
    items = cm->items();
    QCOMPARE(items.size(), 3);
    QCOMPARE(cm->itemsForExport().size(), 2);
    QVERIFY(cm->removeItem(recEvent.id()));
    QCOMPARE(cm->itemsForExport().size(), 0);
    QCOMPARE(cm->items().size(), 0);

    /* Create a recurring event, save an exception, remove the saved exception should remove the persisted exception, but the exdate should remain in the parent */
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    recEvent.setId(QOrganizerItemId());
    recEvent.setDescription("a recurring event");
    recEvent.setStartDateTime(QDateTime(QDate(2010, 10, 20), QTime(8, 0, 0)));
    recEvent.setEndDateTime(QDateTime(QDate(2010, 10, 20), QTime(10, 0, 0)));
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(3);
    recEvent.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&recEvent));
    items = cm->items();
    QCOMPARE(items.size(), 3);
    exception = items.at(1);
    exception.setStartDateTime(QDateTime(QDate(2010, 10, 21), QTime(7, 0, 0)));
    QVERIFY(cm->saveItem(&exception));
    items = cm->items();
    QCOMPARE(items.size(), 3);
    QCOMPARE(cm->itemsForExport().size(), 2);
    QVERIFY(cm->removeItem(exception.id()));
    QCOMPARE(cm->itemsForExport().size(), 1); // only parent remains as persistent
    items = cm->items();
    QCOMPARE(items.size(), 2);          // the exception date remains in parent, so only 2 occurrences are generated.

    // Create a recurring event, remove a generated occurrence should add an exdate in the parent
    QOrganizerItem itemOccurrence = items[0];
    cm->removeItem(&itemOccurrence);
    items = cm->items();
    QCOMPARE(items.size(), 1);

    // Remove both parent item and generated occurrence in the same request
    // --> request should fail because
    itemList.clear();
    itemList << recEvent << items[0];
    QVERIFY(cm->removeItems(&itemList));
    items = cm->items();
    QCOMPARE(cm->items().size(), 0);
}

void tst_QOrganizerManager::batch()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    /* First test null pointer operations */
    QVERIFY(!cm->saveItems(NULL));
    QVERIFY(cm->error() == QOrganizerManager::BadArgumentError);

    QVERIFY(!cm->removeItems(QList<QOrganizerItemId>()));
    QVERIFY(cm->error() == QOrganizerManager::BadArgumentError);

    // Use note & todo item depending on backend support
    QOrganizerItemType::ItemType type;
    if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeNote))
        type = QOrganizerItemType::TypeNote;
    else if (cm->supportedItemTypes().contains(QOrganizerItemType::TypeTodo))
        type = QOrganizerItemType::TypeTodo;
    else
        QSKIP("This manager does not support note or todo item");

    QOrganizerItem a;
    QOrganizerItem b;
    QOrganizerItem c;
    a.setType(type);
    b.setType(type);
    c.setType(type);

    /* Now add 3 items, all valid */
    QOrganizerItemDisplayLabel da;
    da.setValue(QOrganizerItemDisplayLabel::FieldLabel, "XXXXXX A Note");
    a.saveDetail(&da);

    QOrganizerItemDisplayLabel db;
    db.setValue(QOrganizerItemDisplayLabel::FieldLabel, "XXXXXX B Note");
    b.saveDetail(&db);

    QOrganizerItemDisplayLabel dc;
    dc.setValue(QOrganizerItemDisplayLabel::FieldLabel, "XXXXXX C Note");
    c.saveDetail(&dc);

    QList<QOrganizerItem> items;
    items << a << b << c;

    QMap<int, QOrganizerManager::Error> errorMap;
    // Add one dummy error to test if the errors are reset
    errorMap.insert(0, QOrganizerManager::NoError);
    QVERIFY(cm->saveItems(&items));
    QVERIFY(cm->error() == QOrganizerManager::NoError);
    errorMap = cm->errorMap();
    QVERIFY(errorMap.count() == 0);

    /* Make sure our items got updated too */
    QVERIFY(items.count() == 3);
    QVERIFY(items.at(0).id() != QOrganizerItemId());
    QVERIFY(items.at(1).id() != QOrganizerItemId());
    QVERIFY(items.at(2).id() != QOrganizerItemId());

    QVERIFY(items.at(0).detail(QOrganizerItemDetail::TypeDisplayLabel) == da);
    QVERIFY(items.at(1).detail(QOrganizerItemDetail::TypeDisplayLabel) == db);
    QVERIFY(items.at(2).detail(QOrganizerItemDetail::TypeDisplayLabel) == dc);

    /* Retrieve again */
    a = cm->item(items.at(0).id());
    b = cm->item(items.at(1).id());
    c = cm->item(items.at(2).id());
    QVERIFY(items.at(0).detail(QOrganizerItemDetail::TypeDisplayLabel) == da);
    QVERIFY(items.at(1).detail(QOrganizerItemDetail::TypeDisplayLabel) == db);
    QVERIFY(items.at(2).detail(QOrganizerItemDetail::TypeDisplayLabel) == dc);

    /* Now make an update to them all */
    QOrganizerItemDescription descr;
    descr.setDescription("This note looks slightly shifty");

    QVERIFY(items[0].saveDetail(&descr));
    descr.setDescription("This note is definitely up to no good");
    QVERIFY(items[1].saveDetail(&descr));
    descr.setDescription("This note is a terrible note");
    QVERIFY(items[2].saveDetail(&descr));

    QVERIFY(cm->saveItems(&items));
    QVERIFY(cm->error() == QOrganizerManager::NoError);
    errorMap = cm->errorMap();
    QVERIFY(errorMap.count() == 0);

    /* Retrieve them and check them again */
    a = cm->item(items.at(0).id());
    b = cm->item(items.at(1).id());
    c = cm->item(items.at(2).id());
    QVERIFY(items.at(0).detail(QOrganizerItemDetail::TypeDisplayLabel) == da);
    QVERIFY(items.at(1).detail(QOrganizerItemDetail::TypeDisplayLabel) == db);
    QVERIFY(items.at(2).detail(QOrganizerItemDetail::TypeDisplayLabel) == dc);

    QVERIFY(a.details(QOrganizerItemDetail::TypeDescription).count() == 1);
    QVERIFY(b.details(QOrganizerItemDetail::TypeDescription).count() == 1);
    QVERIFY(c.details(QOrganizerItemDetail::TypeDescription).count() == 1);

    QVERIFY(a.details(QOrganizerItemDetail::TypeDescription).at(0).value(QOrganizerItemDescription::FieldDescription) == "This note looks slightly shifty");
    QVERIFY(b.details(QOrganizerItemDetail::TypeDescription).at(0).value(QOrganizerItemDescription::FieldDescription) == "This note is definitely up to no good");
    QVERIFY(c.details(QOrganizerItemDetail::TypeDescription).at(0).value(QOrganizerItemDescription::FieldDescription) == "This note is a terrible note");

    /* Now delete them all */
    QList<QOrganizerItemId> ids;
    ids << a.id() << b.id() << c.id();
    QVERIFY(cm->removeItems(ids));
    errorMap = cm->errorMap();
    QVERIFY(errorMap.count() == 0);
    QVERIFY(cm->error() == QOrganizerManager::NoError);

    /* Make sure the items really don't exist any more */
    QVERIFY(cm->item(a.id()).id() == QOrganizerItemId());
    QVERIFY(cm->item(a.id()).isEmpty());
    QVERIFY(cm->error() == QOrganizerManager::DoesNotExistError);
    QVERIFY(cm->item(b.id()).id() == QOrganizerItemId());
    QVERIFY(cm->item(b.id()).isEmpty());
    QVERIFY(cm->error() == QOrganizerManager::DoesNotExistError);
    QVERIFY(cm->item(c.id()).id() == QOrganizerItemId());
    QVERIFY(cm->item(c.id()).isEmpty());
    QVERIFY(cm->error() == QOrganizerManager::DoesNotExistError);

    /* Now try removing with all invalid ids (e.g. the ones we just removed) */
    ids.clear();
    ids << a.id() << b.id() << c.id();
    QVERIFY(!cm->removeItems(ids));
    QVERIFY(cm->error() == QOrganizerManager::DoesNotExistError);
    errorMap = cm->errorMap();
    QVERIFY(errorMap.count() == 3);
    QVERIFY(errorMap.values().at(0) == QOrganizerManager::DoesNotExistError);
    QVERIFY(errorMap.values().at(1) == QOrganizerManager::DoesNotExistError);
    QVERIFY(errorMap.values().at(2) == QOrganizerManager::DoesNotExistError);

    /* Try adding some new ones again, this time one with an error */
    items.clear();
    a.setId(QOrganizerItemId());
    b.setId(QOrganizerItemId());
    c.setId(QOrganizerItemId());

    items << a << b << c;

//    /* Make B the bad guy */
//    QOrganizerItemDetail bad("does not exist and will break if you add it");
//    bad.setValue("This is also bad", "Very bad");
//    b.saveDetail(&bad);

//    QVERIFY(!cm->saveItems(&items)); // since we don't setCollectionId() in any of the items, they go in default collection.
//    /* We can't really say what the error will be.. maybe bad argument, maybe invalid detail */
//    QVERIFY(cm->error() != QOrganizerManager::NoError);

//    /* It's permissible to fail all the adds, or to add the successful ones */
//    errorMap = cm->errorMap();
//    QVERIFY(errorMap.count() > 0);
//    QVERIFY(errorMap.count() <= 3);

//    // A might have gone through
//    if (errorMap.keys().contains(0)) {
//        QVERIFY(errorMap.value(0) != QOrganizerManager::NoError);
//        QVERIFY(items.at(0).id() == QOrganizerItemId());
//    } else {
//        QVERIFY(items.at(0).id() != QOrganizerItemId());
//    }

//    // B should have failed
//    QVERIFY(errorMap.value(1) == QOrganizerManager::InvalidDetailError);
//    QVERIFY(items.at(1).id() == QOrganizerItemId());

//    // C might have gone through
//    if (errorMap.keys().contains(2)) {
//        QVERIFY(errorMap.value(2) != QOrganizerManager::NoError);
//        QVERIFY(items.at(2).id() == QOrganizerItemId());
//    } else {
//        QVERIFY(items.at(2).id() != QOrganizerItemId());
//    }

//    /* Fix up B and re save it */
//    QVERIFY(items[1].removeDetail(&bad));
//    QVERIFY(cm->saveItems(&items));
//    errorMap = cm->errorMap();
//    QVERIFY(errorMap.count() == 0);
//    QVERIFY(cm->error() == QOrganizerManager::NoError);

    // Save and remove a fourth item. Store the id.
    QOrganizerItem d;
    d.setType(type);
    d.setDisplayLabel("XXXXXX D Note");
    QVERIFY(cm->saveItem(&d));
    QOrganizerItemId removedId = d.id();
    QVERIFY(cm->removeItem(removedId));

    /* Now delete 3 items, but with one bad argument */
    ids.clear();
    ids << items.at(0).id();
    ids << removedId;
    ids << items.at(2).id();

    QVERIFY(!cm->removeItems(ids));
    QVERIFY(cm->error() != QOrganizerManager::NoError);

    /* Again, the backend has the choice of either removing the successful ones, or not */
    errorMap = cm->errorMap();
    QVERIFY(errorMap.count() > 0);
    QVERIFY(errorMap.count() <= 3);

    // A might have gone through
    if (errorMap.keys().contains(0)) {
        QVERIFY(errorMap.value(0) != QOrganizerManager::NoError);
        QVERIFY(items.at(0).id() == QOrganizerItemId());
    } else {
        QVERIFY(items.at(0).id() != QOrganizerItemId());
    }

    /* B should definitely have failed */
    QVERIFY(errorMap.value(1) == QOrganizerManager::DoesNotExistError);
    QVERIFY(ids.at(1) == removedId);

    // A might have gone through
    if (errorMap.keys().contains(2)) {
        QVERIFY(errorMap.value(2) != QOrganizerManager::NoError);
        QVERIFY(items.at(2).id() == QOrganizerItemId());
    } else {
        QVERIFY(items.at(2).id() != QOrganizerItemId());
    }
}

void tst_QOrganizerManager::invalidManager()
{
    /* Create an invalid manager */
    QOrganizerManager manager("this should never work");
    QVERIFY(manager.managerName() == "invalid");

    /* Now test that all the operations fail */
    QVERIFY(manager.itemIds().count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    QOrganizerItem foo;
    QOrganizerItemDisplayLabel dl;
    dl.setLabel("some label");
    foo.saveDetail(&dl);

    QVERIFY(manager.saveItem(&foo) == false);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);
    QVERIFY(foo.id() == QOrganizerItemId());
    QVERIFY(manager.itemIds().count() == 0);

    QVERIFY(manager.item(foo.id()).id() == QOrganizerItemId());
    QVERIFY(manager.item(foo.id()).isEmpty());
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    QVERIFY(manager.removeItem(foo.id()) == false);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    QMap<int, QOrganizerManager::Error> errorMap;
    errorMap.insert(0, QOrganizerManager::NoError);
    QVERIFY(!manager.saveItems(0));
    errorMap = manager.errorMap();
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::BadArgumentError);

    /* filters */
    QOrganizerItemFilter f; // matches everything
    QOrganizerItemDetailFieldFilter dff;
    dff.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
    QVERIFY(manager.itemIds(QDateTime(), QDateTime(), QOrganizerItemFilter()).count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);
    QVERIFY(manager.itemIds(QDateTime(), QDateTime(), dff).count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);
    QVERIFY(manager.itemIds(QDateTime(), QDateTime(), f | f).count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);
    QVERIFY(manager.itemIds(QDateTime(), QDateTime(), dff | dff).count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    QVERIFY(manager.supportedFilters().size() == 0);

    QList<QOrganizerItem> list;
    list << foo;

    QVERIFY(!manager.saveItems(&list));
    errorMap = manager.errorMap();
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    QVERIFY(!manager.removeItems(QList<QOrganizerItemId>()));
    errorMap = manager.errorMap();
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::BadArgumentError);

    QList<QOrganizerItemId> idlist;
    idlist << foo.id();
    QVERIFY(!manager.removeItems(idlist));
    errorMap = manager.errorMap();
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    /* Collections */
    QOrganizerCollection testCollection;
    testCollection.setExtendedMetaData("test", "example");
    QVERIFY(!manager.saveCollection(&testCollection));
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError || manager.error() == QOrganizerManager::InvalidCollectionError);
    QVERIFY(!manager.removeCollection(testCollection.id()));
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError || manager.error() == QOrganizerManager::DoesNotExistError);
    QVERIFY(manager.collection(manager.defaultCollectionId()) == QOrganizerCollection());
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);
    QVERIFY(manager.collections().isEmpty());
    QVERIFY(manager.error() == QOrganizerManager::NotSupportedError);

    /* Requests */
    QOrganizerItemFetchRequest ifr;
    QOrganizerItemFetchForExportRequest ifer;
    QOrganizerItemSaveRequest isr;
    QOrganizerItemRemoveRequest irr;
    QOrganizerItemRemoveByIdRequest irbir;
    QOrganizerCollectionFetchRequest cfr;
    QOrganizerCollectionSaveRequest csr;
    QOrganizerCollectionRemoveRequest crr;

    ifr.setManager(&manager);
    QVERIFY(!ifr.start());
    QVERIFY(!ifr.cancel());
    ifr.waitForFinished();
    //QVERIFY(ifr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...

    ifer.setManager(&manager);
    QVERIFY(!ifer.start());
    QVERIFY(!ifer.cancel());
    ifer.waitForFinished();
    //QVERIFY(ifer.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...

    isr.setManager(&manager);
    isr.setItem(foo);
    QVERIFY(!isr.start());
    QVERIFY(!isr.cancel());
    isr.waitForFinished();
    //QVERIFY(isr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...

    irr.setManager(&manager);
    irr.setItem(foo);

    QVERIFY(!irr.start());
    QVERIFY(!irr.cancel());
    irr.waitForFinished();
    //QVERIFY(irr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...

    irbir.setManager(&manager);
    irbir.setItemId(foo.id());

    qDebug() << foo.id();
    QVERIFY(!irbir.start());
    QVERIFY(!irbir.cancel());
    irbir.waitForFinished();
    //QVERIFY(irr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...


    cfr.setManager(&manager);
    QVERIFY(!cfr.start());
    QVERIFY(!cfr.cancel());
    cfr.waitForFinished();
    //QVERIFY(cfr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...

    csr.setManager(&manager);
    csr.setCollection(testCollection);
    QVERIFY(!csr.start());
    QVERIFY(!csr.cancel());
    csr.waitForFinished();
    //QVERIFY(csr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...

    crr.setManager(&manager);
    crr.setCollectionId(testCollection.id());
    QVERIFY(!crr.start());
    QVERIFY(!crr.cancel());
    crr.waitForFinished();
    //QVERIFY(crr.error() == QOrganizerManager::NotSupportedError); // XXX TODO: if start fails, should be not supported error...
}

void tst_QOrganizerManager::memoryManager()
{
    QMap<QString, QString> params;
    QOrganizerManager m1("memory");
    params.insert("random", "shouldNotBeUsed");
    QOrganizerManager m2("memory", params);
    params.insert("id", "shouldBeUsed");
    QOrganizerManager m3("memory", params);
    QOrganizerManager m4("memory", params);
    params.insert("id", QString(""));
    QOrganizerManager m5("memory", params);

    // add a item to each of m1, m2, m3
    QOrganizerEvent c;
    QOrganizerItemDisplayLabel c1dl;
    c1dl.setLabel("c1dl");
    c.saveDetail(&c1dl);
    QVERIFY(m1.saveItem(&c));
    c.setId(QOrganizerItemId());
    QOrganizerItem c2;
    QOrganizerItemDisplayLabel c2dl = c2.detail(QOrganizerItemDetail::TypeDisplayLabel);
    c2 = c;
    c2dl.setLabel("c2dl");
    c2.saveDetail(&c2dl);
    c2.setCollectionId(QOrganizerCollectionId());
    c.setCollectionId(QOrganizerCollectionId());
    QVERIFY(m2.saveItem(&c2));            // save c2 first; c will be given a higher id
    QVERIFY(m2.saveItem(&c));             // save c to m2
    c.setId(QOrganizerItemId());
    c1dl.setLabel("c3dl");
    c.saveDetail(&c1dl);
    c.setCollectionId(QOrganizerCollectionId());
    QVERIFY(m3.saveItem(&c));

    /* test that m1 != m2 != m3 and that m3 == m4 */

    // check the counts are correct - especially note m4 and m3.
    QCOMPARE(m1.itemIds().count(), 1);
    QCOMPARE(m2.itemIds().count(), 2);
    QCOMPARE(m3.itemIds().count(), 1);
    QCOMPARE(m4.itemIds().count(), 1);
    QCOMPARE(m5.itemIds().count(), 0);

    // remove c2 from m2 - ensure that this doesn't affect any other manager.
    m2.removeItem(c2.id());
    QCOMPARE(m1.itemIds().count(), 1);
    QCOMPARE(m2.itemIds().count(), 1);
    QCOMPARE(m3.itemIds().count(), 1);
    QCOMPARE(m4.itemIds().count(), 1);
    QCOMPARE(m5.itemIds().count(), 0);

    // check that the items contained within are different.
    // note that in the m1->m2 case, only the id will be different!
    QVERIFY(m1.item(m1.itemIds().at(0)) != m2.item(m2.itemIds().at(0)));
    QVERIFY(m1.item(m1.itemIds().at(0)) != m3.item(m3.itemIds().at(0)));
    QVERIFY(m2.item(m2.itemIds().at(0)) != m3.item(m3.itemIds().at(0)));
    QVERIFY(m3.item(m3.itemIds().at(0)) == m4.item(m4.itemIds().at(0)));

    // now, we should be able to remove from m4, and have m3 empty
    QVERIFY(m4.removeItem(c.id()));
    QCOMPARE(m3.itemIds().count(), 0);
    QCOMPARE(m4.itemIds().count(), 0);
    QCOMPARE(m5.itemIds().count(), 0);
}

void tst_QOrganizerManager::recurrenceWithGenerator_data()
{
    QTest::addColumn<QString>("uri");
    QTest::addColumn<QDate>("generatorDate");
    QTest::addColumn<QOrganizerRecurrenceRule>("recurrenceRule");
    QTest::addColumn<QOrganizerRecurrenceRule>("exceptionRule");
    QTest::addColumn<QSet<QDate> >("recurrenceDates");
    QTest::addColumn<QSet<QDate> >("exceptionDates");
    QTest::addColumn<QDate>("startDate");
    QTest::addColumn<QDate>("endDate");
    QTest::addColumn<QList<QDate> >("occurrenceDates");

    QStringList managers = QOrganizerManager::availableManagers();

    /* Known one that will not pass */
    managers.removeAll("invalid");
    managers.removeAll("testdummy");
    managers.removeAll("teststaticdummy");
    managers.removeAll("maliciousplugin");
    managers.removeAll("skeleton");

    QOrganizerRecurrenceRule exrule;
    QSet<QDate> rdates;
    QSet<QDate> exdates;
    foreach(QString mgr, managers) {
        QString managerUri = QOrganizerManager::buildUri(mgr, QMap<QString, QString>());

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
            rrule.setLimit(QDate(2010, 1, 22));
            QTest::newRow(QString("mgr=%1, weekly recurrence").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 1, 20)
                // stops at the 15th because the query end date is the 20th
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 8) << QDate(2010, 1, 15));

            // change the end date of the query to 2010-02-01
            QTest::newRow(QString("mgr=%1, weekly recurrence, end date is inclusive").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 2, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 8) << QDate(2010, 1, 15) << QDate(2010, 1, 22));

            // Now let's fiddle with the recurrence end date and see what happens
            rrule.setLimit(QDate(2010, 1, 23));
            QTest::newRow(QString("mgr=%1, weekly recurrence, end date observed (+1)").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 2, 1)
                // now stop on the 22nd
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 8) << QDate(2010, 1, 15) << QDate(2010, 1, 22));

            rrule.setLimit(QDate(2010, 1, 21));
            QTest::newRow(QString("mgr=%1, weekly recurrence, end date observed (-1)").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 2, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 8) << QDate(2010, 1, 15));

            rrule.setLimit(QDate());
            rrule.setLimit(2);
            QTest::newRow(QString("mgr=%1, weekly recurrence, count").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 2, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 8));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
            rrule.setLimit(QDate(2010, 1, 5));
            QTest::newRow(QString("mgr=%1, daily").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 2)
                                   << QDate(2010, 1, 3) << QDate(2010, 1, 4)
                                   << QDate(2010, 1, 5));

            rrule.setInterval(3);
            QTest::newRow(QString("mgr=%1, daily, interval").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 4));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
            rrule.setFirstDayOfWeek(Qt::Monday);
            rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Friday << Qt::Saturday << Qt::Sunday);
            rrule.setLimit(QDate(2010, 1, 27));
            QTest::newRow(QString("mgr=%1, weekly, days of week").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 2) << QDate(2010, 1, 3)
                                   << QDate(2010, 1, 8) << QDate(2010, 1, 9) << QDate(2010, 1, 10)
                                   << QDate(2010, 1, 15) << QDate(2010, 1, 16) << QDate(2010, 1, 17)
                                   << QDate(2010, 1, 22) << QDate(2010, 1, 23) << QDate(2010, 1, 24));

            rrule.setInterval(3);
            QTest::newRow(QString("mgr=%1, weekly, days of week, interval").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 2) << QDate(2010, 1, 3)
                                   << QDate(2010, 1, 22) << QDate(2010, 1, 23) << QDate(2010, 1, 24));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Monthly);
            rrule.setDaysOfMonth(QSet<int>() << 1 << 10);
            rrule.setLimit(QDate(2010, 4, 15));
            QTest::newRow(QString("mgr=%1, monthly recurrence").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 10)
                                   << QDate(2010, 2, 1) << QDate(2010, 2, 10)
                                   << QDate(2010, 3, 1) << QDate(2010, 3, 10)
                                   << QDate(2010, 4, 1) << QDate(2010, 4, 10));

            rrule.setInterval(3);
            QTest::newRow(QString("mgr=%1, monthly recurrence, interval").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 1, 10)
                                   << QDate(2010, 4, 1) << QDate(2010, 4, 10));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
            rrule.setDaysOfYear(QSet<int>() << 1 << 32);
            rrule.setLimit(QDate(2012, 3, 15));
            QTest::newRow(QString("mgr=%1, yearly recurrence").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 2, 1)
                                   << QDate(2011, 1, 1) << QDate(2011, 2, 1)
                                   << QDate(2012, 1, 1) << QDate(2012, 2, 1));

            rrule.setLimit(QDate(2013, 3, 15));
            rrule.setInterval(3);
            QTest::newRow(QString("mgr=%1, yearly recurrence, interval").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 2, 1)
                                   << QDate(2013, 1, 1) << QDate(2013, 2, 1));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
            rrule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                    << QOrganizerRecurrenceRule::January
                    << QOrganizerRecurrenceRule::March);
            rrule.setLimit(QDate(2011, 3, 15));
            QTest::newRow(QString("mgr=%1, yearly recurrence, by month").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1) << QDate(2010, 3, 1)
                                   << QDate(2011, 1, 1) << QDate(2011, 3, 1));
            // The day-of-month should be inferred from the day-of-month of the original event
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
            rrule.setWeeksOfYear(QSet<int>() << 1 << 4);
            rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Thursday);
            rrule.setLimit(QDate(2011, 3, 15));
            QTest::newRow(QString("mgr=%1, yearly recurrence, by week").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 7) // this is the first day of week 1
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 7) << QDate(2010, 1, 28)
                                   << QDate(2011, 1, 6) << QDate(2011, 1, 27));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
            rrule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                    << QOrganizerRecurrenceRule::April);
            rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Sunday);
            rrule.setPositions(QSet<int>() << 1);
            QTest::newRow(QString("mgr=%1, yearly recurrence, first Sunday of April").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 4, 4) // this is the first Sunday of April 2010
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 4, 4)
                                   << QDate(2011, 4, 3)
                                   << QDate(2012, 4, 1)
                                   << QDate(2013, 4, 7)
                                   << QDate(2014, 4, 6));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Yearly);
            rrule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                    << QOrganizerRecurrenceRule::October);
            rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Sunday);
            rrule.setPositions(QSet<int>() << -1);
            QTest::newRow(QString("mgr=%1, yearly recurrence, last Sunday of October").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 10, 31) // this is the last Sunday of October 2010
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 10, 31)
                                   << QDate(2011, 10, 30)
                                   << QDate(2012, 10, 28)
                                   << QDate(2013, 10, 27)
                                   << QDate(2014, 10, 26));
        }

        {
            QOrganizerRecurrenceRule rrule; // empty
            QSet<QDate> rdates;
            rdates << QDate(2010, 1, 5) << QDate(2010, 1, 8);
            QTest::newRow(QString("mgr=%1, rdates").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2015, 1, 1)
                << (QList<QDate>() << QDate(2010, 1, 1)
                                   << QDate(2010, 1, 5)
                                   << QDate(2010, 1, 8));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
            QSet<QDate> exdates;
            exdates << QDate(2010, 1, 2) << QDate(2010, 1, 3);
            QTest::newRow(QString("mgr=%1, exdates").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 1, 5)
                << (QList<QDate>() << QDate(2010, 1, 1)
                                   << QDate(2010, 1, 4)
                                   << QDate(2010, 1, 5));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
            QOrganizerRecurrenceRule exrule;
            exrule.setFrequency(QOrganizerRecurrenceRule::Monthly);
            exrule.setDaysOfMonth(QSet<int>() << 2 << 3);
            QTest::newRow(QString("mgr=%1, exrule").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2010, 1, 1)
                << rrule << exrule << rdates << exdates
                << QDate(2010, 1, 1) << QDate(2010, 1, 5)
                << (QList<QDate>() << QDate(2010, 1, 1)
                                   << QDate(2010, 1, 4)
                                   << QDate(2010, 1, 5));
        }

        {
            QOrganizerRecurrenceRule rrule;
            rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
            rrule.setLimit(8);
            rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>()
                                << Qt::Monday
                                << Qt::Tuesday
                                << Qt::Wednesday);
            QOrganizerRecurrenceRule exrule;
            QTest::newRow(QString("mgr=%1, weekly, count, days of week").arg(mgr).toLatin1().constData())
                << managerUri << QDate(2014, 1, 22)
                << rrule << exrule << rdates << exdates
                << QDate(2014, 1, 1) << QDate(2014, 5, 1)
                << (QList<QDate>() << QDate(2014, 1, 22) << QDate(2014, 1, 27)
                                   << QDate(2014, 1, 28) << QDate(2014, 1, 29)
                                   << QDate(2014, 2,  3) << QDate(2014, 2,  4)
                                   << QDate(2014, 2,  5) << QDate(2014, 2, 10));
        }
    }
}

void tst_QOrganizerManager::recurrenceWithGenerator()
{
    QFETCH(QString, uri);
    QFETCH(QDate, generatorDate);
    QFETCH(QOrganizerRecurrenceRule, recurrenceRule);
    QFETCH(QOrganizerRecurrenceRule, exceptionRule);
    QFETCH(QSet<QDate>, recurrenceDates);
    QFETCH(QSet<QDate>, exceptionDates);
    QFETCH(QDate, startDate);
    QFETCH(QDate, endDate);
    QFETCH(QList<QDate>, occurrenceDates);

    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel("event");
    event.setStartDateTime(QDateTime(generatorDate, QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(generatorDate, QTime(11, 30, 0)));
    event.setRecurrenceRule(recurrenceRule);
    event.setExceptionRule(exceptionRule);
    event.setRecurrenceDates(recurrenceDates);
    event.setExceptionDates(exceptionDates);

    if (cm->saveItem(&event)) {
        QList<QOrganizerItem> items = cm->itemOccurrences(event,
                QDateTime(startDate, QTime(0, 0, 0)),
                QDateTime(endDate, QTime(23, 59, 59, 999)));

        QList<QDate> actualDates;
        for (int i = 0; i < items.size(); i++) {
            QOrganizerItem item = items.at(i);
            QCOMPARE(item.type(), QOrganizerItemType::TypeEventOccurrence);
            QDate occurrenceDate = item.detail(QOrganizerItemDetail::TypeEventTime).value(QOrganizerEventTime::FieldStartDateTime).toDateTime().date();
            //QCOMPARE(occurrenceDate, occurrenceDates.at(i));
            actualDates << occurrenceDate;
        }
        if (actualDates != occurrenceDates) {
            qDebug() << "Actual: " << actualDates;
            qDebug() << "Expected: " << occurrenceDates;
            QCOMPARE(actualDates, occurrenceDates);
        }
    } else {
        // Allow backend specific limitations
        QCOMPARE(cm->error(), QOrganizerManager::NotSupportedError);
        qWarning() << "The event not supported by the backend";
    }
}

void tst_QOrganizerManager::todoRecurrenceWithGenerator_data()
{
    recurrenceWithGenerator_data();
}

// This is just a copy of recurrenceWithGenerator, but for todos, not events
void tst_QOrganizerManager::todoRecurrenceWithGenerator()
{
    QFETCH(QString, uri);
    QFETCH(QDate, generatorDate);
    QFETCH(QOrganizerRecurrenceRule, recurrenceRule);
    QFETCH(QOrganizerRecurrenceRule, exceptionRule);
    QFETCH(QSet<QDate>, recurrenceDates);
    QFETCH(QSet<QDate>, exceptionDates);
    QFETCH(QDate, startDate);
    QFETCH(QDate, endDate);
    QFETCH(QList<QDate>, occurrenceDates);

    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerTodo todo;
    todo.setDisplayLabel("todo");
    todo.setStartDateTime(QDateTime(generatorDate, QTime(11, 0, 0)));
    todo.setDueDateTime(QDateTime(generatorDate, QTime(11, 30, 0)));
    todo.setRecurrenceRule(recurrenceRule);
    todo.setExceptionRule(exceptionRule);
    todo.setRecurrenceDates(recurrenceDates);
    todo.setExceptionDates(exceptionDates);

    if (cm->saveItem(&todo)) {
        QList<QOrganizerItem> items = cm->itemOccurrences(todo,
                QDateTime(startDate, QTime(0, 0, 0)),
                QDateTime(endDate, QTime(23, 59, 59, 999)));

        QList<QDate> actualDates;
        for (int i = 0; i < items.size(); i++) {
            QOrganizerItem item = items.at(i);
            QCOMPARE(item.type(), QOrganizerItemType::TypeTodoOccurrence);
            QDate occurrenceDate = item.detail(QOrganizerItemDetail::TypeTodoTime).value(QOrganizerTodoTime::FieldStartDateTime).toDateTime().date();
            //QCOMPARE(occurrenceDate, occurrenceDates.at(i));
            actualDates << occurrenceDate;
        }

        if (actualDates != occurrenceDates) {
            qDebug() << "Actual: " << actualDates;
            qDebug() << "Expected: " << occurrenceDates;
            QCOMPARE(actualDates, occurrenceDates);
        }
    } else {
        // Allow backend specific limitations
        QCOMPARE(cm->error(), QOrganizerManager::NotSupportedError);
        qWarning() << "The todo not supported by the backend";
    }
}

void tst_QOrganizerManager::observerDeletion()
{
    QOrganizerManager *manager = new QOrganizerManager("memory");
    QOrganizerEvent c;
    QVERIFY(manager->saveItem(&c));
    QOrganizerItemId id = c.id();
    QOrganizerItemObserver *observer = new QOrganizerItemObserver(manager, id);
    Q_UNUSED(observer)
    delete manager;
    delete observer;
    // Test for bug MOBILITY-2566 - that QOrganizerItemObserver doesn't crash when it is
    // destroyed after the associated QOrganizerManager
}

void tst_QOrganizerManager::signalEmission()
{
    QTest::qWait(500); // clear the signal queue
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> m1(QOrganizerManager::fromUri(uri));

    qRegisterMetaType<QOrganizerItemId>("QOrganizerItemId");
    qRegisterMetaType<QList<QOrganizerItemId> >("QList<QOrganizerItemId>");
    qRegisterMetaType<QList<QOrganizerItemDetail::DetailType> >("QList<QOrganizerItemDetail::DetailType>");
    QSignalSpy spyAdded(m1.data(), SIGNAL(itemsAdded(QList<QOrganizerItemId>)));
    QSignalSpy spyModified(m1.data(), SIGNAL(itemsChanged(QList<QOrganizerItemId>, QList<QOrganizerItemDetail::DetailType>)));
    QSignalSpy spyRemoved(m1.data(), SIGNAL(itemsRemoved(QList<QOrganizerItemId>)));
    QSignalSpy spyChanged(m1.data(), SIGNAL(dataChanged()));

    QList<QVariant> args;
    QList<QOrganizerItemId> arg;
    QOrganizerTodo todo;
    QList<QOrganizerItem> batchAdd;
    QList<QOrganizerItemId> batchRemove;
    int addSigCount = 0; // the expected signal counts.
    int modSigCount = 0;
    int remSigCount = 0;

    // verify add emits signal added
    QOrganizerItemDisplayLabel nc;
    nc.setLabel("label me this");
    QVERIFY(todo.saveDetail(&nc));
    QVERIFY(m1->saveItem(&todo));
    QOrganizerItemId cid = todo.id();
    QTRY_VERIFY(spyAdded.count() > addSigCount);
    ++addSigCount;
    args = spyAdded.takeFirst();
    --addSigCount;
    arg = args.first().value<QList<QOrganizerItemId> >();
    QVERIFY(arg.count() == 1);
    QCOMPARE(QOrganizerItemId(arg.at(0)), cid);

    QScopedPointer<QOrganizerItemObserver> todo1Observer(new QOrganizerItemObserver(m1.data(), cid));
    QScopedPointer<QSignalSpy> spyObserverModified1(new QSignalSpy(todo1Observer.data(), SIGNAL(itemChanged(QList<QOrganizerItemDetail::DetailType>))));
    QScopedPointer<QSignalSpy> spyObserverRemoved1(new QSignalSpy(todo1Observer.data(), SIGNAL(itemRemoved())));

    // verify save modified emits signal changed
    nc.setLabel("label me that");
    QVERIFY(todo.saveDetail(&nc));
    QVERIFY(m1->saveItem(&todo));
    QTRY_VERIFY(spyModified.count() > modSigCount);
    ++modSigCount;
    QTRY_VERIFY(spyObserverModified1->count() > 0);
    args = spyModified.takeFirst();
    --modSigCount;
    arg = args.first().value<QList<QOrganizerItemId> >();
    QVERIFY(arg.count() == 1);
    QCOMPARE(QOrganizerItemId(arg.at(0)), cid);

    // verify remove emits signal removed
    QVERIFY(m1->removeItem(todo.id()));
    QTRY_VERIFY(spyRemoved.count() > remSigCount);
    QTRY_VERIFY(spyObserverRemoved1->count() > 0);
    args = spyRemoved.takeFirst();
    --remSigCount;
    arg = args.first().value<QList<QOrganizerItemId> >();
    QVERIFY(arg.count() == 1);
    QCOMPARE(QOrganizerItemId(arg.at(0)), cid);

    // verify multiple adds works as advertised
    QOrganizerTodo todo2, todo3;
    QOrganizerItemDisplayLabel nc2, nc3;
    nc2.setLabel("Mark");
    nc3.setLabel("Garry");
    QVERIFY(todo2.saveDetail(&nc2));
    QVERIFY(todo3.saveDetail(&nc3));
    QVERIFY(m1->saveItem(&todo2));
    QVERIFY(m1->saveItem(&todo3));
    QTRY_COMPARE(spyModified.count(), modSigCount);
    QTRY_VERIFY(spyAdded.count() > addSigCount);
    addSigCount = spyAdded.count();

    spyObserverModified1->clear();
    spyObserverRemoved1->clear();
    QScopedPointer<QOrganizerItemObserver> todo2Observer(new QOrganizerItemObserver(m1.data(), todo2.id()));
    QScopedPointer<QOrganizerItemObserver> todo3Observer(new QOrganizerItemObserver(m1.data(), todo3.id()));
    QScopedPointer<QSignalSpy> spyObserverModified2(new QSignalSpy(todo2Observer.data(), SIGNAL(itemChanged(QList<QOrganizerItemDetail::DetailType>))));
    QScopedPointer<QSignalSpy> spyObserverModified3(new QSignalSpy(todo3Observer.data(), SIGNAL(itemChanged(QList<QOrganizerItemDetail::DetailType>))));
    QScopedPointer<QSignalSpy> spyObserverRemoved2(new QSignalSpy(todo2Observer.data(), SIGNAL(itemRemoved())));
    QScopedPointer<QSignalSpy> spyObserverRemoved3(new QSignalSpy(todo3Observer.data(), SIGNAL(itemRemoved())));

    // verify multiple modifies works as advertised
    nc2.setLabel("M.");
    QVERIFY(todo2.saveDetail(&nc2));
    QVERIFY(m1->saveItem(&todo2));
    nc2.setLabel("Mark");
    nc3.setLabel("G.");
    QVERIFY(todo2.saveDetail(&nc2));
    QVERIFY(todo3.saveDetail(&nc3));
    QVERIFY(m1->saveItem(&todo2));
    QVERIFY(m1->saveItem(&todo3));
    QTRY_VERIFY(spyModified.count() > modSigCount);
    modSigCount = spyModified.count();
    QTRY_VERIFY(spyObserverModified2->count() > 0);
    QTRY_VERIFY(spyObserverModified3->count() > 0);
    QCOMPARE(spyObserverModified1->count(), 0);

    // verify multiple removes works as advertised
    m1->removeItem(todo3.id());
    m1->removeItem(todo2.id());
    QTRY_VERIFY(spyRemoved.count() > remSigCount);
    remSigCount = spyRemoved.count();
    QTRY_VERIFY(spyObserverRemoved2->count() > 0);
    QTRY_VERIFY(spyObserverRemoved3->count() > 0);
    QCOMPARE(spyObserverRemoved1->count(), 0);

    QVERIFY(!m1->removeItem(todo.id())); // not saved.

    /* Now test the batch equivalents */
    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();

    /* Batch adds - set ids to zero so add succeeds. */
    todo.setId(QOrganizerItemId());
    todo2.setId(QOrganizerItemId());
    todo3.setId(QOrganizerItemId());
    batchAdd << todo << todo2 << todo3;
    QMap<int, QOrganizerManager::Error> errorMap;
    QVERIFY(m1->saveItems(&batchAdd));
    errorMap = m1->errorMap();

    QVERIFY(batchAdd.count() == 3);
    todo = batchAdd.at(0);
    todo2 = batchAdd.at(1);
    todo3 = batchAdd.at(2);

    // We want to see one itemsAdded signal listing the id's for all three items
    QTRY_COMPARE(spyAdded.count(), 1);
    {
        QList<QOrganizerItemId> sigids = spyAdded.takeFirst().at(0).value<QList<QOrganizerItemId> >();
        QVERIFY(sigids.contains(todo.id()));
        QVERIFY(sigids.contains(todo2.id()));
        QVERIFY(sigids.contains(todo3.id()));
    }
    QTRY_COMPARE(spyModified.count(), 0);

    todo1Observer.reset(new QOrganizerItemObserver(m1.data(), todo.id()));
    todo2Observer.reset(new QOrganizerItemObserver(m1.data(), todo2.id()));
    todo3Observer.reset(new QOrganizerItemObserver(m1.data(), todo3.id()));
    spyObserverModified1.reset(new QSignalSpy(todo1Observer.data(), SIGNAL(itemChanged(QList<QOrganizerItemDetail::DetailType>))));
    spyObserverModified2.reset(new QSignalSpy(todo2Observer.data(), SIGNAL(itemChanged(QList<QOrganizerItemDetail::DetailType>))));
    spyObserverModified3.reset(new QSignalSpy(todo3Observer.data(), SIGNAL(itemChanged(QList<QOrganizerItemDetail::DetailType>))));
    spyObserverRemoved1.reset(new QSignalSpy(todo1Observer.data(), SIGNAL(itemRemoved())));
    spyObserverRemoved2.reset(new QSignalSpy(todo2Observer.data(), SIGNAL(itemRemoved())));
    spyObserverRemoved3.reset(new QSignalSpy(todo3Observer.data(), SIGNAL(itemRemoved())));
    QTRY_COMPARE(spyRemoved.count(), 0);

    /* Batch modifies */
    QOrganizerItemDisplayLabel modifiedName = todo.detail(QOrganizerItemDetail::TypeDisplayLabel);
    modifiedName.setLabel("Modified number 1");
    modifiedName = todo2.detail(QOrganizerItemDetail::TypeDisplayLabel);
    modifiedName.setLabel("Modified number 2");
    modifiedName = todo3.detail(QOrganizerItemDetail::TypeDisplayLabel);
    modifiedName.setLabel("Modified number 3");

    batchAdd.clear();
    batchAdd << todo << todo2 << todo3;
    QVERIFY(m1->saveItems(&batchAdd));
    errorMap = m1->errorMap();

    // We want to see one itemsChanged signal listing the id's for all three items.
    QTRY_COMPARE(spyModified.count(), 1);
    {
        QList<QOrganizerItemId> sigids = spyModified.takeFirst().at(0).value<QList<QOrganizerItemId> >();
        QVERIFY(sigids.contains(todo.id()));
        QVERIFY(sigids.contains(todo2.id()));
        QVERIFY(sigids.contains(todo3.id()));
    }
    QTRY_COMPARE(spyObserverModified1->count(), 1);
    QTRY_COMPARE(spyObserverModified2->count(), 1);
    QTRY_COMPARE(spyObserverModified3->count(), 1);

    /* Batch removes */
    batchRemove << todo.id() << todo2.id() << todo3.id();
    QVERIFY(m1->removeItems(batchRemove));
    errorMap = m1->errorMap();

    // We want to see one itemsRemoved signal listing the id's for all three items.
    QTRY_COMPARE(spyRemoved.count(), 1);
    {
        QList<QOrganizerItemId> sigids = spyRemoved.takeFirst().at(0).value<QList<QOrganizerItemId> >();
        QVERIFY(sigids.contains(todo.id()));
        QVERIFY(sigids.contains(todo2.id()));
        QVERIFY(sigids.contains(todo3.id()));
    }
    QTRY_COMPARE(spyObserverRemoved1->count(), 1);
    QTRY_COMPARE(spyObserverRemoved2->count(), 1);
    QTRY_COMPARE(spyObserverRemoved3->count(), 1);

    QTRY_COMPARE(spyAdded.count(), 0);
    QTRY_COMPARE(spyModified.count(), 0);

    /* Now some cross manager testing */
    if (m1->managerName() == QStringLiteral("memory"))
        QSKIP("Not supported by memory back-end");

    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();
    spyChanged.clear();


    // verify that signals are emitted for modifications made to other managers (same id).
    QScopedPointer<QOrganizerManager> m2(QOrganizerManager::fromUri(uri));

    // add one new item
    QOrganizerEvent newEvent;
    newEvent.setStartDateTime(QDateTime::currentDateTime());
    newEvent.setEndDateTime(QDateTime::currentDateTime());
    newEvent.setDisplayLabel(QStringLiteral("a new event"));

    QVERIFY(m2->saveItem(&newEvent));
    QTRY_VERIFY(spyChanged.count() || (spyAdded.count() > 0));

    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();
    spyChanged.clear();

    // modify the item
    newEvent.setEndDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m2->saveItem(&newEvent));
    QTRY_VERIFY(spyChanged.count() || (spyModified.count() > 0));

    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();
    spyChanged.clear();

    // remove the item
    QVERIFY(m2->removeItem(newEvent.id()));
    QTRY_VERIFY(spyChanged.count() || (spyRemoved.count() > 0));

    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();
    spyChanged.clear();

    // add several new items
    QOrganizerEvent moreEvents[10];
    for (int i = 0; i < 10; ++i) {;
        moreEvents[i].setStartDateTime(QDateTime::currentDateTime());
        moreEvents[i].setEndDateTime(QDateTime::currentDateTime());
        moreEvents[i].setDisplayLabel(QStringLiteral("yet another event"));

        QVERIFY(m2->saveItem(&moreEvents[i]));
    }
    QTRY_VERIFY(spyChanged.count() || (spyAdded.count() > 0));

    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();
    spyChanged.clear();

    // modify several items
    for (int i = 0; i < 10; ++i) {
        moreEvents[i].setEndDateTime(QDateTime::currentDateTime().addDays(i + 1));
        QVERIFY(m2->saveItem(&moreEvents[i]));
    }
    QTRY_VERIFY(spyChanged.count() || (spyModified.count() > 0));

    spyAdded.clear();
    spyModified.clear();
    spyRemoved.clear();
    spyChanged.clear();

    // remove several items
    for (int i = 0; i < 10; ++i)
        QVERIFY(m2->removeItem(moreEvents[i].id()));
    QTRY_VERIFY(spyChanged.count() || (spyRemoved.count() > 0));

    spyChanged.clear();

    QSignalSpy spyCollectionAdded(m1.data(), SIGNAL(collectionsAdded(QList<QOrganizerCollectionId>)));
    QSignalSpy spyCollectionModified(m1.data(), SIGNAL(collectionsChanged(QList<QOrganizerCollectionId>)));
    QSignalSpy spyCollectionRemoved(m1.data(), SIGNAL(collectionsRemoved(QList<QOrganizerCollectionId>)));

    // add one collection
    QOrganizerCollection newCollection;
    newCollection.setMetaData(QOrganizerCollection::KeyName, QVariant(QStringLiteral("a new collection")));

    QVERIFY(m2->saveCollection(&newCollection));
    QTRY_VERIFY(spyChanged.count() || (spyCollectionAdded.count() > 0));

    spyCollectionAdded.clear();
    spyCollectionModified.clear();
    spyCollectionRemoved.clear();
    spyChanged.clear();

    // modify one collection
    newCollection.setMetaData(QOrganizerCollection::KeyName, QVariant(QStringLiteral("a new collection, modified")));

    QVERIFY(m2->saveCollection(&newCollection));
    QTRY_VERIFY(spyChanged.count() || (spyCollectionModified.count() > 0));

    spyCollectionAdded.clear();
    spyCollectionModified.clear();
    spyCollectionRemoved.clear();
    spyChanged.clear();

    // remove one collection
    QVERIFY(m2->removeCollection(newCollection.id()));
    QTRY_VERIFY(spyChanged.count() || (spyCollectionRemoved.count() > 0));
}

void tst_QOrganizerManager::errorStayingPut()
{
    /* Make sure that when we clone a manager, we don't clone the error */
    QMap<QString, QString> params;
    params.insert("id", "error isolation test");
    QOrganizerManager m1("memory",params);

    QVERIFY(m1.error() == QOrganizerManager::NoError);

    /* Remove an invalid item to get an error */
    QVERIFY(m1.removeItem(QOrganizerItemId()) == false);
    QVERIFY(m1.error() == QOrganizerManager::DoesNotExistError);

    /* Create a new manager with hopefully the same backend */
    QOrganizerManager m2("memory", params);

    QVERIFY(m1.error() == QOrganizerManager::DoesNotExistError);
    QVERIFY(m2.error() == QOrganizerManager::NoError);

    /* Cause an error on the other ones and check the first is not affected */
    m2.saveItems(0);
    QVERIFY(m1.error() == QOrganizerManager::DoesNotExistError);
    QVERIFY(m2.error() == QOrganizerManager::BadArgumentError);

    QOrganizerItem c;
    QOrganizerItemDetail d(QOrganizerItemDetail::TypeUndefined);
    d.setValue(101, 5);
    c.saveDetail(&d);

//    QVERIFY(m1.saveItem(&c) == false);
//    QVERIFY(m1.error() == QOrganizerManager::InvalidDetailError);
    QVERIFY(m2.error() == QOrganizerManager::BadArgumentError);
}

void tst_QOrganizerManager::changeSet()
{
    QOrganizerItemId id;

    QOrganizerItemChangeSet changeSet;
    QVERIFY(changeSet.addedItems().isEmpty());
    QVERIFY(changeSet.changedItems().isEmpty());
    QVERIFY(changeSet.removedItems().isEmpty());

    changeSet.insertAddedItem(id);
    QVERIFY(!changeSet.addedItems().isEmpty());
    QVERIFY(changeSet.changedItems().isEmpty());
    QVERIFY(changeSet.removedItems().isEmpty());
    QVERIFY(changeSet.addedItems().contains(id));
    changeSet.clearAddedItems();
    changeSet.insertAddedItems(QList<QOrganizerItemId>() << id);

    changeSet.insertChangedItem(id, QList<QOrganizerItemDetail::DetailType>());
    changeSet.insertChangedItems(QList<QOrganizerItemId>() << id, QList<QOrganizerItemDetail::DetailType>());
    QCOMPARE(changeSet.changedItems().size(), 1); // set, should only be added once.
    QCOMPARE(changeSet.changedItems().first().second.size(), 1); // only one changed item ID
    QVERIFY(!changeSet.addedItems().isEmpty());
    QVERIFY(!changeSet.changedItems().isEmpty());
    QVERIFY(changeSet.removedItems().isEmpty());

    changeSet.clearChangedItems();
    changeSet.insertChangedItems(QList<QOrganizerItemId>() << id, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeDescription);
    changeSet.insertChangedItems(QList<QOrganizerItemId>() << id, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeTag);
    QCOMPARE(changeSet.changedItems().size(), 2); // should be added twice with differing change types
    QVERIFY(!changeSet.addedItems().isEmpty());
    QVERIFY(!changeSet.changedItems().isEmpty());
    QVERIFY(changeSet.removedItems().isEmpty());
    QSet<QOrganizerItemId> changedIds;
    QSet<QOrganizerItemDetail::DetailType> changedTypes;
    foreach (const QOrganizerItemChangeSet::ItemChangeList &changes, changeSet.changedItems()) {
        changedIds |= changes.second.toSet();
        if (changes.second.contains(id)) {
            changedTypes |= changes.first.toSet();
        }
    }
    QCOMPARE(changedIds, (QList<QOrganizerItemId>() << id).toSet());
    QCOMPARE(changedTypes, (QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeDescription << QOrganizerItemDetail::TypeTag).toSet());
    changeSet.clearChangedItems();
    QVERIFY(changeSet.changedItems().isEmpty());

    QList<QOrganizerItemId> l1, l2;
    foreach (int n, QList<int>() << 1 << 1 << 1 << 2 << 2 << 3 << 3 << 4 << 4 << 4 << 5 << 10 << 9 << 8 << 8 << 8 << 7 << 7 << 6) {
        ((qrand() % 2) ? l1 : l2).append(makeItemId(n));
    }
    changeSet.clearChangedItems();
    changeSet.insertChangedItems(l1, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeDescription << QOrganizerItemDetail::TypeTag);
    changeSet.insertChangedItems(l2, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeTag << QOrganizerItemDetail::TypeDescription << QOrganizerItemDetail::TypeTag);
    QCOMPARE(changeSet.changedItems().size(), 1);
    QList<QOrganizerItemId> expected((l1.toSet() | l2.toSet()).toList());
    qSort(expected);
    QCOMPARE(changeSet.changedItems().first().second, expected);

    changeSet.insertRemovedItems(QList<QOrganizerItemId>() << id);
    QVERIFY(changeSet.removedItems().contains(id));
    changeSet.clearRemovedItems();
    QVERIFY(changeSet.removedItems().isEmpty());

    QVERIFY(changeSet.dataChanged() == false);
    QOrganizerItemChangeSet changeSet2;
    changeSet2 = changeSet;
    QVERIFY(changeSet.addedItems() == changeSet2.addedItems());
    changeSet.emitSignals(0);

    changeSet2.clearAddedItems();
    QVERIFY(changeSet2.addedItems().isEmpty());
    changeSet2.insertAddedItems(changeSet.addedItems().toList());
    QVERIFY(changeSet.addedItems() == changeSet2.addedItems());

    changeSet2.clearAll();
    QVERIFY(changeSet.addedItems() != changeSet2.addedItems());

    QOrganizerItemChangeSet changeSet3(changeSet2);
    QVERIFY(changeSet.addedItems() != changeSet3.addedItems());
    QVERIFY(changeSet2.addedItems() == changeSet3.addedItems());

    changeSet.setDataChanged(true);
    QVERIFY(changeSet.dataChanged() == true);
    QVERIFY(changeSet.dataChanged() != changeSet2.dataChanged());
    QVERIFY(changeSet.dataChanged() != changeSet3.dataChanged());
    changeSet.emitSignals(0);

    QOrganizerCollectionId colId;

    QOrganizerCollectionChangeSet colChangeSet;
    QVERIFY(colChangeSet.addedCollections().isEmpty());
    QVERIFY(colChangeSet.changedCollections().isEmpty());
    QVERIFY(colChangeSet.removedCollections().isEmpty());

    colChangeSet.insertAddedCollection(colId);
    QVERIFY(!changeSet.addedItems().isEmpty());
    QVERIFY(colChangeSet.changedCollections().isEmpty());
    QVERIFY(colChangeSet.removedCollections().isEmpty());
    QVERIFY(colChangeSet.addedCollections().contains(colId));
    colChangeSet.clearAddedCollections();
    colChangeSet.insertAddedCollections(QList<QOrganizerCollectionId>() << colId);

    colChangeSet.insertChangedCollection(colId);
    colChangeSet.insertChangedCollections(QList<QOrganizerCollectionId>() << colId);
    QVERIFY(colChangeSet.changedCollections().size() == 1); // set, should only be added once.
    QVERIFY(!colChangeSet.addedCollections().isEmpty());
    QVERIFY(!colChangeSet.changedCollections().isEmpty());
    QVERIFY(colChangeSet.removedCollections().isEmpty());
    QVERIFY(colChangeSet.changedCollections().contains(colId));
    colChangeSet.clearChangedCollections();
    QVERIFY(colChangeSet.changedCollections().isEmpty());

    colChangeSet.insertRemovedCollections(QList<QOrganizerCollectionId>() << colId);
    QVERIFY(colChangeSet.removedCollections().contains(colId));
    colChangeSet.clearRemovedCollections();
    QVERIFY(colChangeSet.removedCollections().isEmpty());

    QVERIFY(colChangeSet.dataChanged() == false);
    QOrganizerCollectionChangeSet colChangeSet2;
    colChangeSet2 = colChangeSet;
    QVERIFY(colChangeSet.addedCollections() == colChangeSet2.addedCollections());
    colChangeSet.emitSignals(0);

    colChangeSet2.clearAddedCollections();
    QVERIFY(colChangeSet2.addedCollections().isEmpty());
    colChangeSet2.insertAddedCollections(colChangeSet.addedCollections().toList());
    QVERIFY(colChangeSet.addedCollections() == colChangeSet2.addedCollections());

    colChangeSet2.clearAll();
    QVERIFY(colChangeSet.addedCollections() != colChangeSet2.addedCollections());

    QOrganizerCollectionChangeSet colChangeSet3(colChangeSet2);
    QVERIFY(colChangeSet.addedCollections() != colChangeSet3.addedCollections());
    QVERIFY(colChangeSet2.addedCollections() == colChangeSet3.addedCollections());

    colChangeSet.setDataChanged(true);
    QVERIFY(colChangeSet.dataChanged() == true);
    QVERIFY(colChangeSet.dataChanged() != colChangeSet2.dataChanged());
    QVERIFY(colChangeSet.dataChanged() != colChangeSet3.dataChanged());
    colChangeSet.emitSignals(0);
}

void tst_QOrganizerManager::fetchHint()
{
    QOrganizerItemFetchHint hint;
    hint.setOptimizationHints(QOrganizerItemFetchHint::NoBinaryBlobs);
    QCOMPARE(hint.optimizationHints(), QOrganizerItemFetchHint::NoBinaryBlobs);
}

void tst_QOrganizerManager::testFilterFunction()
{
    QOrganizerEvent item;
    item.setId(makeItemId(10));
    item.setStartDateTime(QDateTime(QDate(2010,10,10), QTime(10,10)));
    item.setEndDateTime(QDateTime(QDate(2010,10,10), QTime(12,10)));
    item.setDisplayLabel("test");
    item.setCollectionId(makeCollectionId(1));

    // Test for QOrganizerItemFilter::InvalidFilter
    QOrganizerItemInvalidFilter fif;
    QVERIFY(!QOrganizerManagerEngine::testFilter(fif, item));

    // Test for QOrganizerItemFilter::IdFilter:
    QOrganizerItemIdFilter fidf;
    fidf.setIds(QList<QOrganizerItemId>() << makeItemId(10));
    QVERIFY(QOrganizerManagerEngine::testFilter(fidf, item));

    // test for nonexistent id
    fidf.setIds(QList<QOrganizerItemId>() << makeItemId(11));
    QVERIFY(!QOrganizerManagerEngine::testFilter(fidf, item));

    // Test for QOrganizerItemFilter::DefaultFilter:
    QOrganizerItemFilter fdf;
    QVERIFY(fdf.type() == QOrganizerItemFilter::DefaultFilter);
    QVERIFY(QOrganizerManagerEngine::testFilter(fdf, item));

    // Test for QOrganizerItemFilter::OrganizerItemDetailFieldFilter:
    QOrganizerItemDetailFieldFilter fdef;
    fdef.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
    fdef.setValue("invalid");
    // test for nonexistent label
    QVERIFY(!QOrganizerManagerEngine::testFilter(fdef, item));

    fdef.setValue("test");
    // test for existent label
    QVERIFY(QOrganizerManagerEngine::testFilter(fdef, item));

    // Test for QOrganizerItemFilter::OrganizerItemDetailRangeFilter:
    QOrganizerItemDetailRangeFilter fdrf;
    fdrf.setDetail(QOrganizerItemDetail::TypeEventTime, QOrganizerEventTime::FieldStartDateTime);
    fdrf.setRange(QDateTime(QDate(2010,10,9)), QDateTime(QDate(2010,10,11)));
    // test for a valid range
    QVERIFY(QOrganizerManagerEngine::testFilter(fdrf, item));

    fdrf.setRange(QDateTime(QDate(2010,10,11)), QDateTime(QDate(2010,10,12)));
    // test for item not in the range
    QVERIFY(!QOrganizerManagerEngine::testFilter(fdrf, item));

    // Test for QOrganizerItemFilter::IntersectionFilter:
    QOrganizerItemIntersectionFilter oiif;
    oiif.setFilters(QList<QOrganizerItemFilter>() << fif << fdf);
    // check for an invalid filter and default filter intersection
    QVERIFY(!QOrganizerManagerEngine::testFilter(oiif, item));

    oiif.setFilters(QList<QOrganizerItemFilter>() << fdef << fdf);
    // check for a detail filter and default filter intersection
    QVERIFY(QOrganizerManagerEngine::testFilter(oiif, item));

    // Test for QOrganizerItemFilter::UnionFilter:
    QOrganizerItemUnionFilter oiuf;
    oiuf.setFilters(QList<QOrganizerItemFilter>() << fif << fdf);
    // check for an invalid filter and default filter union
    QVERIFY(QOrganizerManagerEngine::testFilter(oiuf, item));

    oiuf.setFilters(QList<QOrganizerItemFilter>() << fdef << fdf);
    // check for a detail filter and default filter union
    QVERIFY(QOrganizerManagerEngine::testFilter(oiuf, item));

    // Test for QOrganizerItemFilter::CollectionFilter:
    QOrganizerItemCollectionFilter oicf;
    oicf.setCollectionId(makeCollectionId(1));
    // check for existing collection id
    QVERIFY(QOrganizerManagerEngine::testFilter(oicf, item));

    oicf.setCollectionId(makeCollectionId(2));
    // check for nonexisting collection id
    QVERIFY(!QOrganizerManagerEngine::testFilter(oicf, item));
}


void tst_QOrganizerManager::dataSerialization()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel("event");
    event.setStartDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 30, 0)));

    if (cm->saveItem(&event)) {
        QByteArray buffer;
        QDataStream outBufferStream(&buffer, QIODevice::WriteOnly);
        outBufferStream << event.id().toString();
        QVERIFY(buffer.length() > 0);

        QDataStream inBufferStream(buffer);
        QString inString;
        inBufferStream >> inString;
        QOrganizerItemId id = QOrganizerItemId::fromString(inString);

        QVERIFY(id == event.id());
    }
}

void tst_QOrganizerManager::itemFilterFetch()
{
    // Some of the tests present on itemFetch()-tests, but this test extends the cases a bit
    // Preparations
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));
    QOrganizerItemExtendedDetail extDetail;
    QOrganizerItemComment comment;
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    for (int i=0; i<6; i++) {
        QOrganizerEvent event;
        event.setDisplayLabel(QString("event %1").arg(i));
        event.setStartDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 0, 0)));
        event.setEndDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 30, 0)));
        if (i==0) {
            // 1
            event.setPriority(QOrganizerItemPriority::VeryLowPriority);
        } else if (i==1) {
            // 2
            comment.setComment("my comment");
            event.saveDetail(&comment);
        } else if (i==2) {
            // 3
            QOrganizerItemDisplayLabel label;
            label.setLabel("my 3rd event!");
            event.saveDetail(&label);
        } else if (i==3) {
            // 4
            QOrganizerItemDisplayLabel label;
            label.setLabel("my 4th event!");
            event.saveDetail(&label);
        } else if (i==4) {
            // 5
            event.setEndDateTime(QDateTime(QDate(2010, 10, 10), QTime(11, 0, 0)));
        } else if (i==5) {
            // 6
            extDetail.setName("DetailOfMine");
            extDetail.setData(42);
            event.saveDetail(&extDetail);
        }
        QVERIFY(cm->saveItem(&event));
    }
    QCOMPARE(cm->items().count(), 6);

    // DetailFieldFilter Checks
    QOrganizerItemDetailFieldFilter dFieldFilter;
    // 1
    dFieldFilter.setDetail(QOrganizerItemDetail::TypePriority, QOrganizerItemPriority::FieldPriority);
    dFieldFilter.setValue(QOrganizerItemPriority::VeryHighPriority);
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 0);
    dFieldFilter.setValue(QOrganizerItemPriority::VeryLowPriority);
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 1);
    // 2
    dFieldFilter.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
    dFieldFilter.setValue("my comment");
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 1);
    // 3-4
    dFieldFilter.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
    dFieldFilter.setValue("my 3rd event!");
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 1);
    dFieldFilter.setMatchFlags(QOrganizerItemFilter::MatchEndsWith);
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 1);
    dFieldFilter.setValue("event!");
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 2);
    dFieldFilter.setValue("event");
    dFieldFilter.setMatchFlags(QOrganizerItemFilter::MatchContains);
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), cm->items().count());
    dFieldFilter.setValue("my");
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 2);
    dFieldFilter.setMatchFlags(QOrganizerItemFilter::MatchStartsWith);
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 2);
    dFieldFilter.setValue("event");
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 4);
    // 5
    dFieldFilter.setMatchFlags(QOrganizerItemFilter::MatchExactly);
    dFieldFilter.setDetail(QOrganizerItemDetail::TypeEventTime, QOrganizerEventTime::FieldEndDateTime);
    dFieldFilter.setValue(QDateTime(QDate(2010, 10, 10), QTime(11, 0, 0)));
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 1);
    // 6
    dFieldFilter.setDetail(QOrganizerItemDetail::TypeExtendedDetail, QOrganizerItemExtendedDetail::FieldName);
    dFieldFilter.setValue("DetailOfMine");
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dFieldFilter).count(), 1);

    // DetailFilter Checks
    if (cm->managerName() == QStringLiteral("memory")) {
        QOrganizerItemDetailFilter dFilter;
        // 1
        QOrganizerItemPriority priority;
        priority.setPriority(QOrganizerItemPriority::VeryHighPriority);
        dFilter.setDetail(priority);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 0);
        priority.setPriority(QOrganizerItemPriority::VeryLowPriority);
        dFilter.setDetail(priority);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 1);
        // 2
        dFilter.setDetail(comment);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 1);
        // 3-4
        QOrganizerItemDisplayLabel displayLabel;
        displayLabel.setLabel(QStringLiteral("my 3rd event!"));
        dFilter.setDetail(displayLabel);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 1);
        displayLabel.setLabel(QStringLiteral("my non existing event!"));
        dFilter.setDetail(displayLabel);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 0);
        // 5
        QOrganizerEventTime eventTime;
        eventTime.setEndDateTime(QDateTime(QDate(2010, 10, 10), QTime(11, 0, 0)));
        eventTime.setStartDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 0, 0)));
        dFilter.setDetail(eventTime);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 1);
        // 6
        dFilter.setDetail(extDetail);
        QCOMPARE(cm->items(QDateTime(), QDateTime(), dFilter).count(), 1);
    }

}

void tst_QOrganizerManager::itemFetch()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one

    QOrganizerEvent event;
    event.setDisplayLabel("event");
    event.setStartDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 30, 0)));
    QVERIFY(cm->saveItem(&event));

    QOrganizerEvent recEvent;
    recEvent.setDisplayLabel("daily event");
    recEvent.setStartDateTime(QDateTime(QDate(2010, 9, 1), QTime(16, 0, 0)));
    recEvent.setEndDateTime(QDateTime(QDate(2010, 9, 1), QTime(16, 30, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(QDate(2010, 9, 10));
    recEvent.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&recEvent));

    //fetch all recurrences
    QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2010, 9, 8)), QDateTime(QDate(2010, 9, 12)));
    QCOMPARE(items.count(), 4); // should return event + 3 x occurrencesOfRecEvent

    //fetch only the originating items
    items = cm->itemsForExport(QDateTime(QDate(2010, 9, 8)), QDateTime(QDate(2010, 9, 12)),
                               QOrganizerItemFilter(), QList<QOrganizerItemSortOrder>(), QOrganizerItemFetchHint());
    QCOMPARE(items.count(), 2);

    // test semantics of items():
    // first - save event with multiple occurrences; call items() -- should get back just occurrences.
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    rrule.setLimit(QDate(2010, 9, 3));
    recEvent.setRecurrenceRule(rrule);
    recEvent.setId(QOrganizerItemId());
    cm->saveItem(&recEvent);
    items = cm->items();
    QCOMPARE(items.count(), 3);
    foreach (const QOrganizerItem &item, items)
        QVERIFY(item.type() == QOrganizerItemType::TypeEventOccurrence);

    // second - the same situation, but giving a time span that only covers the first day - should get back a single occurrence.
    items = cm->items(QDateTime(QDate(2010, 9, 1), QTime(15, 0, 0)), QDateTime(QDate(2010, 9, 1), QTime(18, 0, 0)));
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeEventOccurrence);
    }

    // third - save event with no recurrence; call items() -- should get back that parent, not an occurrence.
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    recEvent.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>()); // clear rrule.
    recEvent.setId(QOrganizerItemId());
    cm->saveItem(&recEvent);
    items = cm->items();
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem &item, items)
        QVERIFY(item.type() == QOrganizerItemType::TypeEvent);

    // fourth - the same situation, but giving a time span.  should still get back the parent.
    items = cm->items(QDateTime(QDate(2010, 9, 1), QTime(15, 0, 0)), QDateTime(QDate(2010, 9, 1), QTime(18, 0, 0)));
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem &item, items)
        QVERIFY(item.type() == QOrganizerItemType::TypeEvent);

    // test semantics of itemsForExport():
    // first - save event with multiple occurrences; call ife() -- get back that parent
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    recEvent.setRecurrenceRule(rrule);
    recEvent.setId(QOrganizerItemId());
    cm->saveItem(&recEvent);
    items = cm->itemsForExport();
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeEvent);
    }

    // second - call items, resave only the first occurrence as an exception,
    // call ife() -- get back parent + exception
    items = cm->items();
    QCOMPARE(items.size(), 3); // 3 occurrences.
    int eventCount = 0;
    int eventOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
            if (eventOccurrenceCount == 0) {
                QOrganizerEventOccurrence exception(item);
                exception.setDisplayLabel("exception");
                QVERIFY(cm->saveItem(&exception));
            }
            eventOccurrenceCount++;
        } else if (item.type() == QOrganizerItemType::TypeEvent) {
            eventCount++;
        }
    }
    QCOMPARE(eventOccurrenceCount, 3);
    QCOMPARE(eventCount, 0);
    items = cm->items(); // reload items after saving exception
    QCOMPARE(items.size(), 3); // saving the exception shouldn't have added more items.
    items = cm->itemsForExport();
    QCOMPARE(items.count(), 2);
    eventCount = 0;
    eventOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeEvent) {
            eventCount += 1;
        } else if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
            eventOccurrenceCount += 1;
        }
        QVERIFY(!item.id().isNull()); // should NEVER be null, since that would be a generated occurrence.
    }
    QCOMPARE(eventCount, 1);
    QCOMPARE(eventOccurrenceCount, 1);

    //make a parent filter and test item count
    QOrganizerItemDetailFieldFilter dff;
    dff.setDetail(QOrganizerItemDetail::TypeParent, QOrganizerItemParent::FieldParentId);
    dff.setValue(QVariant::fromValue(recEvent.id()));
    QCOMPARE(cm->items(QDateTime(), QDateTime(), dff).count(), 3);
    QCOMPARE(cm->itemsForExport(QDateTime(), QDateTime(), dff).count(), 2);

    // third, have all occurrences persisted
    items = cm->items();
    QCOMPARE(items.size(), 3); // should be three occurrences
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
            QOrganizerEventOccurrence exception(item);
            exception.setDisplayLabel("exception");
            QVERIFY(cm->saveItem(&exception));
        }
    }
    items = cm->itemsForExport();
    QCOMPARE(items.size(), 4); // parent + 3 persisted exceptions
    eventCount = 0;
    eventOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeEvent) {
            eventCount += 1;
        } else if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
            eventOccurrenceCount += 1;
        }
        QVERIFY(!item.id().isNull()); // should NEVER be null, since that would be a generated occurrence.
    }
    QCOMPARE(eventCount, 1);
    QCOMPARE(eventOccurrenceCount, 3);

    // fourth, all occurrences persisted, time period excludes parent item, but it's still fetched
    items = cm->itemsForExport(QDateTime(QDate(2010, 9, 2)),
                               QDateTime(QDate(2010, 9, 4)));
    QCOMPARE(items.size(), 3); // parent + 2 persisted exceptions
    eventCount = 0;
    eventOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeEvent) {
            eventCount += 1;
        } else if (item.type() == QOrganizerItemType::TypeEventOccurrence) {
            eventOccurrenceCount += 1;
        }
        QVERIFY(!item.id().isNull()); // should NEVER be null, since that would be a generated occurrence.
    }
    QCOMPARE(eventCount, 1);
    QCOMPARE(eventOccurrenceCount, 2);


}

// This is mostly a copy of itemFetch(), but for todos
void tst_QOrganizerManager::todoItemFetch()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));
    QList<QOrganizerItem> items;
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one

    QOrganizerTodo todoStartDueDate;
    todoStartDueDate.setDisplayLabel("todo startdue");
    todoStartDueDate.setStartDateTime(QDateTime(QDate(2011, 3, 27), QTime(11, 0, 0)));
    todoStartDueDate.setDueDateTime(QDateTime(QDate(2011, 3, 29), QTime(11, 30, 0)));
    QVERIFY(cm->saveItem(&todoStartDueDate));

    QOrganizerTodo todoStartDate;
    todoStartDate.setDisplayLabel("todo start");
    todoStartDate.setStartDateTime(QDateTime(QDate(2011, 3, 25), QTime(11, 0, 0)));
    QVERIFY(cm->saveItem(&todoStartDate));

    QOrganizerTodo todoDueDate;
    todoDueDate.setDisplayLabel("todo due");
    todoDueDate.setDueDateTime(QDateTime(QDate(2011, 3, 25), QTime(11, 30, 0)));
    QVERIFY(cm->saveItem(&todoDueDate));

    QOrganizerTodo todoNoDate;
    todoNoDate.setDisplayLabel("todo nodate");
    QVERIFY(cm->saveItem(&todoNoDate));

    items = cm->items();
    QCOMPARE(items.count(), 4);

    items = cm->items(QDateTime(), QDateTime(QDate(2011, 3, 25), QTime(13, 0, 0)));
    QCOMPARE(items.count(), 2);

    items = cm->items(QDateTime(), QDateTime(QDate(2011, 3, 25), QTime(11, 15, 0)));
    QCOMPARE(items.count(), 1);

    items = cm->items(QDateTime(QDate(2011, 3, 27), QTime(10, 0, 0)), QDateTime());
    QCOMPARE(items.count(), 1);

    items = cm->items(QDateTime(QDate(2011, 3, 29), QTime(13, 0, 0)), QDateTime());
    QCOMPARE(items.count(), 0);

    items = cm->items(QDateTime(QDate(2011, 3, 26), QTime(11, 0, 0)),
                      QDateTime(QDate(2011, 3, 28), QTime(11, 0, 0)));
    QCOMPARE(items.count(), 1);

    items = cm->items(QDateTime(QDate(2011, 3, 28), QTime(11, 0, 0)),
                      QDateTime(QDate(2011, 3, 30), QTime(11, 0, 0)));
    QCOMPARE(items.count(), 1);

    items = cm->items(QDateTime(QDate(2011, 3, 28), QTime(11, 0, 0)),
                      QDateTime(QDate(2011, 3, 28), QTime(11, 0, 0)));
    QCOMPARE(items.count(), 1);


    cm->removeItems(cm->itemIds());

    QOrganizerTodo todo;
    todo.setDisplayLabel("todo");
    todo.setStartDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 0, 0)));
    todo.setDueDateTime(QDateTime(QDate(2010, 9, 9), QTime(11, 30, 0)));
    QVERIFY(cm->saveItem(&todo));

    QOrganizerTodo recTodo;
    recTodo.setDisplayLabel("daily todo");
    recTodo.setStartDateTime(QDateTime(QDate(2010, 9, 1), QTime(16, 0, 0)));
    recTodo.setDueDateTime(QDateTime(QDate(2010, 9, 1), QTime(16, 30, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(QDate(2010, 9, 10));
    recTodo.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&recTodo));

    //fetch all recurrences
    items = cm->items(QDateTime(QDate(2010, 9, 8)), QDateTime(QDate(2010, 9, 12)));
    QCOMPARE(items.count(), 4); // should return todo + 3 x occurrencesOfRecTodo

    //fetch only the originating items
    items = cm->itemsForExport(QDateTime(QDate(2010, 9, 8)), QDateTime(QDate(2010, 9, 12)),
                               QOrganizerItemFilter(), QList<QOrganizerItemSortOrder>(), QOrganizerItemFetchHint());
    QCOMPARE(items.count(), 2);

    // test semantics of items():
    // first - save todo with multiple occurrences; call items() -- should get back just occurrences.
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    rrule.setLimit(QDate(2010, 9, 3));
    recTodo.setRecurrenceRule(rrule);
    recTodo.setId(QOrganizerItemId());
    cm->saveItem(&recTodo);
    items = cm->items();
    QCOMPARE(items.count(), 3);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeTodoOccurrence);
    }

    // second - the same situation, but giving a time span that only covers the first day - should get back a single occurrence.
    items = cm->items(QDateTime(QDate(2010, 9, 1), QTime(15, 0, 0)), QDateTime(QDate(2010, 9, 1), QTime(18, 0, 0)));
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeTodoOccurrence);
    }

    // third - save event with no recurrence; call items() -- should get back that parent, not an occurrence.
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    recTodo.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>()); // clear rrule.
    recTodo.setId(QOrganizerItemId());
    cm->saveItem(&recTodo);
    items = cm->items();
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeTodo);
    }

    // fourth - the same situation, but giving a time span.  should still get back the parent.
    items = cm->items(QDateTime(QDate(2010, 9, 1), QTime(15, 0, 0)), QDateTime(QDate(2010, 9, 1), QTime(18, 0, 0)));
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeTodo);
    }

    // test semantics of itemsForExport():
    // first - save event with multiple occurrences; call ife() -- get back that parent
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    recTodo.setRecurrenceRule(rrule);
    recTodo.setId(QOrganizerItemId());
    cm->saveItem(&recTodo);
    items = cm->itemsForExport();
    QCOMPARE(items.count(), 1);
    foreach (const QOrganizerItem& item, items) {
        QVERIFY(item.type() == QOrganizerItemType::TypeTodo);
    }

    // second - call items, resave only the first occurrence as an exception,
    // call ife() -- get back parent + exception
    items = cm->items();
    int todoCount = 0;
    int todoOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeTodoOccurrence) {
            if (todoOccurrenceCount == 0) {
                QOrganizerTodoOccurrence exception(item);
                exception.setDisplayLabel("exception");
                QVERIFY(cm->saveItem(&exception));
            }
            todoOccurrenceCount++;
        } else if (item.type() == QOrganizerItemType::TypeTodo) {
            todoCount++;
        }
    }
    QCOMPARE(todoOccurrenceCount, 3);
    QCOMPARE(todoCount, 0);
    items = cm->itemsForExport();
    QCOMPARE(items.count(), 2);
    todoCount = 0;
    todoOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeTodo) {
            todoCount += 1;
        } else if (item.type() == QOrganizerItemType::TypeTodoOccurrence) {
            todoOccurrenceCount += 1;
        }
        QVERIFY(!item.id().isNull()); // should NEVER be null, since that would be a generated occurrence.
    }
    QCOMPARE(todoCount, 1);
    QCOMPARE(todoOccurrenceCount, 1);

    // third, have all occurrences persisted
    items = cm->items();
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeTodoOccurrence) {
            QOrganizerTodoOccurrence exception(item);
            exception.setDisplayLabel("exception");
            QVERIFY(cm->saveItem(&exception));
        }
    }
    items = cm->itemsForExport();
    QCOMPARE(items.size(), 4);
    todoCount = 0;
    todoOccurrenceCount = 0;
    foreach (const QOrganizerItem& item, items) {
        if (item.type() == QOrganizerItemType::TypeTodo) {
            todoCount += 1;
        } else if (item.type() == QOrganizerItemType::TypeTodoOccurrence) {
            todoOccurrenceCount += 1;
        }
        QVERIFY(!item.id().isNull()); // should NEVER be null, since that would be a generated occurrence.
    }
    QCOMPARE(todoCount, 1);
    QCOMPARE(todoOccurrenceCount, 3);
}

void tst_QOrganizerManager::itemFetchV2()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one

    QOrganizerEvent event1;
    event1.setDisplayLabel("event1");
    event1.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(11, 0, 0)));
    event1.setEndDateTime(QDateTime(QDate(2010, 1, 1), QTime(11, 30, 0)));
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(QDate(2010, 1, 2));
    event1.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event1));

    QOrganizerEvent event2;
    event2.setDisplayLabel("event2");
    event2.setStartDateTime(QDateTime(QDate(2010, 1, 1), QTime(13, 0, 0)));
    event2.setEndDateTime(QDateTime(QDate(2010, 1, 1), QTime(13, 30, 0)));
    rrule = QOrganizerRecurrenceRule();
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(QDate(2010, 1, 2));
    event2.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event2));

    // Get items without a maxCount, check that they're date sorted
    QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)), QDateTime());
    QCOMPARE(items.size(), 4);
    QCOMPARE(items[0].displayLabel(), QStringLiteral("event1"));
    QCOMPARE(items[1].displayLabel(), QStringLiteral("event2"));
    QCOMPARE(items[2].displayLabel(), QStringLiteral("event1"));
    QCOMPARE(items[3].displayLabel(), QStringLiteral("event2"));

    // Get the next 3 items from 2010-02-01
    items = cm->items(QDateTime(QDate(2010, 1, 1), QTime(0, 0, 0)),
                      QDateTime(),  // no end date limit
                      QOrganizerItemFilter(),
                      3);           // maxCount
    QCOMPARE(items.size(), 3);
    QCOMPARE(items[0].displayLabel(), QStringLiteral("event1"));
    QCOMPARE(items[1].displayLabel(), QStringLiteral("event2"));
    QCOMPARE(items[2].displayLabel(), QStringLiteral("event1"));
}

void tst_QOrganizerManager::spanOverDays()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one

    QOrganizerEvent event;
    event.setDisplayLabel("huge event");
    event.setStartDateTime(QDateTime(QDate(2010, 8, 9), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    QVERIFY(cm->saveItem(&event));

    // just fetch one day from the event
    QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2010, 8, 9)),
                                            QDateTime(QDate(2010, 8, 9), QTime(23,59,59)));
    QCOMPARE(items.count(), 1);

    // fetch the next day
    items = cm->items(QDateTime(QDate(2010, 8, 10), QTime(0,0,0)), QDateTime(QDate(2010, 8, 10), QTime(23,59,59)));
    QCOMPARE(items.count(), 1);

    // fetch the last day
    items = cm->items(QDateTime(QDate(2010, 8, 11), QTime(0,0,0)), QDateTime(QDate(2010, 11, 10), QTime(23,59,59)));
    QCOMPARE(items.count(), 1);

    // fetch an interval starting before the event until infinity
    items = cm->items(QDateTime(QDate(2010, 8, 5), QTime(0,0,0)), QDateTime());
    QCOMPARE(items.count(), 1);

    // fetch an interval ending after the event
    items = cm->items(QDateTime(), QDateTime(QDate(2010, 12, 10), QTime(23,59,59)));
    QCOMPARE(items.count(), 1);

    // fetch an interval starting before the event and ending at almost end of the year
    items = cm->items(QDateTime(QDate(2010, 8, 5), QTime(0,0,0)), QDateTime(QDate(2010, 12, 10), QTime(23,59,59)));
    QCOMPARE(items.count(), 1);

    // fetch an interval ending in the middle of the event
    items = cm->items(QDateTime(), QDateTime(QDate(2010, 8, 10), QTime(23,59,59)));
    QCOMPARE(items.count(), 1);

    // fetch an interval starting from the middle of the event until infinity
    items = cm->items(QDateTime(QDate(2010, 8, 10), QTime(0,0,0)), QDateTime());
    QCOMPARE(items.count(), 1);

    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one

    QOrganizerEvent event2;
    QOrganizerRecurrenceRule rrule;
    event2.setDisplayLabel("huge recurring event");
    event2.setStartDateTime(QDateTime(QDate(2010, 8, 9), QTime(11, 0, 0)));
    event2.setEndDateTime(QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    rrule.setLimit(QDate(2010, 9, 2));
    event2.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event2));

    items = cm->items(QDateTime(QDate(2010, 8, 8)), QDateTime(QDate(2010, 9, 3)));
    QCOMPARE(items.count(), 4);
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[0]).startDateTime(), QDateTime(QDate(2010, 8, 9), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[0]).endDateTime(), QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[1]).startDateTime(), QDateTime(QDate(2010, 8, 16), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[1]).endDateTime(), QDateTime(QDate(2010, 8, 18), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[2]).startDateTime(), QDateTime(QDate(2010, 8, 23), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[2]).endDateTime(), QDateTime(QDate(2010, 8, 25), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[3]).startDateTime(), QDateTime(QDate(2010, 8, 30), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerEventOccurrence>(items[3]).endDateTime(), QDateTime(QDate(2010, 9, 1), QTime(11, 30, 0)));

    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerTodo todo;
    QOrganizerRecurrenceRule rrule2;
    todo.setDisplayLabel("huge overlapping recurring todo");
    todo.setStartDateTime(QDateTime(QDate(2010, 8, 9), QTime(11, 0, 0)));
    todo.setDueDateTime(QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    rrule2.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule2.setLimit(4);
    todo.setRecurrenceRule(rrule2);
    QVERIFY(cm->saveItem(&todo));

    items = cm->items(QDateTime(QDate(2010, 8, 8)), QDateTime(QDate(2010, 9, 3)));
    QCOMPARE(items.count(), 4);
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[0]).startDateTime(), QDateTime(QDate(2010, 8, 9), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[0]).dueDateTime(), QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[1]).startDateTime(), QDateTime(QDate(2010, 8, 10), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[1]).dueDateTime(), QDateTime(QDate(2010, 8, 12), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[2]).startDateTime(), QDateTime(QDate(2010, 8, 11), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[2]).dueDateTime(), QDateTime(QDate(2010, 8, 13), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[3]).startDateTime(), QDateTime(QDate(2010, 8, 12), QTime(11, 0, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[3]).dueDateTime(), QDateTime(QDate(2010, 8, 14), QTime(11, 30, 0)));
}


void tst_QOrganizerManager::incompleteTodoTime()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerTodo todo;
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(4);
    todo.setDisplayLabel("recurring todo without start date");
    todo.setDueDateTime(QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    todo.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&todo));

    QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2010, 8, 8)), QDateTime(QDate(2010, 9, 3)));
    QCOMPARE(items.count(), 4);
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[0]).dueDateTime(), QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[1]).dueDateTime(), QDateTime(QDate(2010, 8, 12), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[2]).dueDateTime(), QDateTime(QDate(2010, 8, 13), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[3]).dueDateTime(), QDateTime(QDate(2010, 8, 14), QTime(11, 30, 0)));

    // same test as previous, but with date limit
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerTodo todo2;
    todo2.setDisplayLabel("recurring todo without start date with date limit");
    todo2.setDueDateTime(QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    QOrganizerRecurrenceRule rrule2;
    rrule2.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule2.setLimit(QDate(2010, 8, 13));
    todo2.setRecurrenceRule(rrule2);
    QVERIFY(cm->saveItem(&todo2));

    items = cm->items(QDateTime(QDate(2010, 8, 8)), QDateTime(QDate(2010, 9, 3)));
    QCOMPARE(items.count(), 3);
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[0]).dueDateTime(), QDateTime(QDate(2010, 8, 11), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[1]).dueDateTime(), QDateTime(QDate(2010, 8, 12), QTime(11, 30, 0)));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[2]).dueDateTime(), QDateTime(QDate(2010, 8, 13), QTime(11, 30, 0)));

    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerTodo todo3;
    todo3.setDisplayLabel("recurring todo without start and due date");
    todo3.setRecurrenceRule(rrule2);
    QVERIFY(cm->saveItem(&todo3));
    items = cm->items();
    QCOMPARE(items.count(), 0);
    items = cm->items(QDateTime(QDate(2010, 8, 8)), QDateTime(QDate(2010, 9, 3)));
    QCOMPARE(items.count(), 6);

    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[0]).originalDate(), QDate(2010, 8, 8));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[1]).originalDate(), QDate(2010, 8, 9));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[2]).dueDateTime(), QDateTime());
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[3]).dueDateTime(), QDateTime());
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[4]).originalDate(), QDate(2010, 8, 12));
    QCOMPARE(static_cast<QOrganizerTodoOccurrence>(items[5]).originalDate(), QDate(2010, 8, 13));
    cm->removeItem(todo3.id());
}

void tst_QOrganizerManager::recurrence()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent event;
    event.setDisplayLabel("recurrent event");
    event.setStartDateTime(QDateTime(QDate(2012, 8, 9), QTime(11, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2012, 8, 9), QTime(11, 30, 0)));

    // first, test count limiting.
    QOrganizerRecurrenceRule rrule;
    rrule.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule.setLimit(3);
    event.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event));

    {
        // Fetch all events with occurrences
        QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2012, 8, 9)),
                                                QDateTime(QDate(2012, 8, 12), QTime(23,59,59)));
        QCOMPARE(items.count(), 3);

        // Fetch events for the first day
        items = cm->items(QDateTime(QDate(2012, 8, 9), QTime(0,0,0)), QDateTime(QDate(2012, 8, 9), QTime(23,59,59)));
        QCOMPARE(items.count(), 1);

        // Fetch events for the second day
        items = cm->items(QDateTime(QDate(2012, 8, 10), QTime(0,0,0)), QDateTime(QDate(2012, 8, 10), QTime(23,59,59)));
        QCOMPARE(items.count(), 1);

        // Create an exception on the second day
        QOrganizerEventOccurrence ex = static_cast<QOrganizerEventOccurrence>(items.at(0));
        ex.setStartDateTime(QDateTime(QDate(2012, 8, 10), QTime(10, 30, 0)));
        QVERIFY(cm->saveItem(&ex));

        // Fetch again the events for the second day
        items = cm->items(QDateTime(QDate(2012, 8, 10), QTime(0,0,0)), QDateTime(QDate(2012, 8, 10), QTime(23,59,59)));
        QCOMPARE(items.count(), 1);
        QOrganizerItem item = items.at(0);
        QVERIFY(!item.id().isNull());
        QVERIFY(item.type() == QOrganizerItemType::TypeEventOccurrence);

        // Add a normal event to the first day
        QOrganizerEvent event2;
        event2.setDisplayLabel("event");
        event2.setStartDateTime(QDateTime(QDate(2012, 8, 9), QTime(15, 0, 0)));
        event2.setEndDateTime(QDateTime(QDate(2012, 8, 9), QTime(16, 0, 0)));
        QVERIFY(cm->saveItem(&event2));

        // Fetch the whole period again
        items = cm->items(QDateTime(QDate(2012, 8, 9)), QDateTime(QDate(2012, 8, 12), QTime(23,59,59)));
        QCOMPARE(items.count(), 4);
        foreach(QOrganizerItem item, items) {
            // check if the item is the recurrence exception
            if (item.id() == ex.id()) {
                QOrganizerEventOccurrence exc = static_cast<QOrganizerEventOccurrence>(item);
                QCOMPARE(exc.guid(), ex.guid());
                QCOMPARE(exc.startDateTime(), ex.startDateTime());
                QCOMPARE(exc.endDateTime(), ex.endDateTime());
            } else if (item.id() == event2.id()) {
                // check if the item is the normal event
                QOrganizerEvent ev = static_cast<QOrganizerEvent>(item);
                QCOMPARE(ev.guid(), event2.guid());
                QCOMPARE(ev.startDateTime(), event2.startDateTime());
                QCOMPARE(ev.endDateTime(), event2.endDateTime());
            } else {
                // item must be event occurrence type and has to be a generated one
                QVERIFY(item.type() == QOrganizerItemType::TypeEventOccurrence);
                QVERIFY(item.id().isNull());
            }
        }

        // Fetch events on a day where the recurrence is no longer valid
        items = cm->items(QDateTime(QDate(2012, 8, 12), QTime(0,0,0)), QDateTime(QDate(2012, 8, 12), QTime(23,59,59)));
        QCOMPARE(items.count(), 0);
    }

    //test for unlimited count limit
    //for bug:MOBILITY-2125
    cm->removeItems(cm->itemIds());
    event.setId(QOrganizerItemId());
    rrule.setLimit(INT_MAX);
    rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
    rrule.setInterval(4);
    rrule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Friday);
    event.setEndDateTime(QDateTime(QDate(2013, 8, 9), QTime(11, 30, 0)));
    event.setRecurrenceRule(rrule);
    QVERIFY(cm->saveItem(&event));
    {
        // Fetch all events with occurrences
        QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2012, 8, 9)),
                                                QDateTime(QDate(2013, 8, 12), QTime(23,59,59)));
        QVERIFY(items.count() > 1);
    }
    // second, test date limit.  The results should be the same as the count limit, if the limit date is the 11th.
    cm->removeItems(cm->itemIds()); // empty the calendar to prevent the previous test from interfering this one
    QOrganizerRecurrenceRule rrule2;
    rrule2.setFrequency(QOrganizerRecurrenceRule::Daily);
    rrule2.setLimit(QDate(2012, 8, 11));
    event.setRecurrenceRule(rrule2);
    event.setId(QOrganizerItemId());
    QVERIFY(cm->saveItem(&event));

    {
        // Fetch all events with occurrences
        QList<QOrganizerItem> items = cm->items(QDateTime(QDate(2012, 8, 9)),
                                                QDateTime(QDate(2012, 8, 12), QTime(23,59,59)));
        QCOMPARE(items.count(), 3);

        // Fetch events for the first day
        items = cm->items(QDateTime(QDate(2012, 8, 9), QTime(0,0,0)), QDateTime(QDate(2012, 8, 9), QTime(23,59,59)));
        QCOMPARE(items.count(), 1);

        // Fetch events for the second day
        items = cm->items(QDateTime(QDate(2012, 8, 10), QTime(0,0,0)), QDateTime(QDate(2012, 8, 10), QTime(23,59,59)));
        QCOMPARE(items.count(), 1);

        // Create an exception on the second day
        QOrganizerEventOccurrence ex = static_cast<QOrganizerEventOccurrence>(items.at(0));
        ex.setStartDateTime(QDateTime(QDate(2012, 8, 10), QTime(10, 30, 0)));
        QVERIFY(cm->saveItem(&ex));

        // Fetch again the events for the second day
        items = cm->items(QDateTime(QDate(2012, 8, 10), QTime(0,0,0)), QDateTime(QDate(2012, 8, 10), QTime(23,59,59)));
        QCOMPARE(items.count(), 1);
        QOrganizerItem item = items.at(0);
        QVERIFY(!item.id().isNull());
        QVERIFY(item.type() == QOrganizerItemType::TypeEventOccurrence);

        // Add a normal event to the first day
        QOrganizerEvent event2;
        event2.setDisplayLabel("event");
        event2.setStartDateTime(QDateTime(QDate(2012, 8, 9), QTime(15, 0, 0)));
        event2.setEndDateTime(QDateTime(QDate(2012, 8, 9), QTime(16, 0, 0)));
        QVERIFY(cm->saveItem(&event2));

        // Fetch the whole period again
        items = cm->items(QDateTime(QDate(2012, 8, 9)), QDateTime(QDate(2012, 8, 12), QTime(23,59,59)));
        QCOMPARE(items.count(), 4);
        foreach(QOrganizerItem item, items) {
            // check if the item is the recurrence exception
            if (item.id() == ex.id()) {
                QOrganizerEventOccurrence exc = static_cast<QOrganizerEventOccurrence>(item);
                QCOMPARE(exc.guid(), ex.guid());
                QCOMPARE(exc.startDateTime(), ex.startDateTime());
                QCOMPARE(exc.endDateTime(), ex.endDateTime());
            } else if (item.id() == event2.id()) {
                // check if the item is the normal event
                QOrganizerEvent ev = static_cast<QOrganizerEvent>(item);
                QCOMPARE(ev.guid(), event2.guid());
                QCOMPARE(ev.startDateTime(), event2.startDateTime());
                QCOMPARE(ev.endDateTime(), event2.endDateTime());
            } else {
                // item must be event occurrence type and has to be a generated one
                QVERIFY(item.type() == QOrganizerItemType::TypeEventOccurrence);
                QVERIFY(item.id().isNull());
            }
        }
    }
}

void tst_QOrganizerManager::idComparison()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    // Can we run this test?
    if (!cm->supportedItemTypes().contains(QOrganizerItemType::TypeJournal)) {
        QSKIP("Backend not compatible with this test");
        // TODO: The test should be refactored so it could run on all platforms
    }

    // delete all collections in the database so that we know there can be no interference from previous test runs.
    QList<QOrganizerCollection> allCollections = cm->collections();
    for (int i = 0; i < allCollections.size(); ++i) {
        QOrganizerCollectionId currentId = allCollections.at(i).id();
        if (currentId != cm->defaultCollectionId()) {
            cm->removeCollection(currentId);
        }
    }

    // step one: make a few items and collections to save (and harvest their ids)
    QOrganizerEvent e1;
    e1.setDescription("test event one");
    e1.setDisplayLabel("event one");
    e1.setStartDateTime(QDateTime::currentDateTime());
    e1.setEndDateTime(QDateTime::currentDateTime().addSecs(3600));

    QOrganizerEvent e2;
    e2.setDescription("test event two");
    e2.setDisplayLabel("event two");
    e2.setStartDateTime(QDateTime::currentDateTime().addDays(1));
    e2.setEndDateTime(QDateTime::currentDateTime().addDays(1).addSecs(1800));

    QOrganizerTodo t1;
    QOrganizerRecurrenceRule r1;
    r1.setFrequency(QOrganizerRecurrenceRule::Weekly);
    t1.setDisplayLabel("todo one");
    t1.setDescription("test todo one");
    t1.setDueDateTime(QDateTime::currentDateTime().addDays(5));
    t1.setRecurrenceRule(r1);

    QOrganizerCollection c1;
    c1.setMetaData(QOrganizerCollection::KeyName, "IdComparisonTest");

    // step two: save and harvest the ids
    QVERIFY(cm->saveItem(&e1));
    QVERIFY(cm->saveItem(&e2));
    QVERIFY(cm->saveItem(&t1));
    QVERIFY(cm->saveCollection(&c1));
    QOrganizerItemId e1id = e1.id();
    QOrganizerItemId e2id = e2.id();
    QOrganizerItemId t1id = t1.id();
    QOrganizerCollectionId c1id = c1.id();

    // step three: make some basic ids as controlled data for our unit test
    QOrganizerItemId biid1 = makeItemId(5);
    QOrganizerItemId biid2 = makeItemId(12);
    QOrganizerCollectionId bcid1 = makeCollectionId(5);
    QOrganizerCollectionId bcid2 = makeCollectionId(17);
    QOrganizerCollectionId bcid3 = makeCollectionId(15);

    // finally: do the testing.
    QVERIFY(biid1 != biid2);
    QVERIFY(bcid1 != bcid2);

    QVERIFY(e1id != e2id);
    QVERIFY(e1id != t1id);

    QList<QOrganizerItemId> idList;
    idList << e1id << t1id << biid1;
    QVERIFY(!idList.contains(biid2));
    QVERIFY(!idList.contains(e2id));
    QVERIFY(idList.contains(e1id));
    QVERIFY(idList.contains(t1id));
    QVERIFY(idList.contains(biid1));

    QVERIFY(((bcid1 < bcid2) || (bcid2 < bcid1)) && (bcid1 != bcid2));
    QVERIFY(((bcid3 < bcid2) || (bcid3 < bcid1)) && (bcid3 != bcid2));
    QVERIFY(((e1id < e2id) || (e2id < e1id)) && (e1id != e2id));

    // now we do some tests which might be unstable
    QVERIFY(bcid1 < c1id); // collectionIds: the first comparison should be manager uri, and bcid manager uri is early in the alphabet.
    QVERIFY(!(c1id < bcid1)); // collectionIds: ensure that less-than is consistent no matter which is on LHS or RHS.
    QVERIFY(biid2 < e1id); // itemIds: first comparison should be manager uri, and biid manager uri is early in the alphabet.
    QVERIFY(!(e1id < biid2)); // itemIds: ensure that less-than is consistent no matter which is on LHS or RHS.

    // null testing
    QOrganizerItemId n1;
    QOrganizerItemId n2;
    QVERIFY(n1 == n2);   // both null means they're equal
    QVERIFY(!(n1 < n2)); // not less than
    QVERIFY(!(n2 < n1)); // and not less than with LHS/RHS swapped.

    QVERIFY(n1 < biid1); // null id is always less than any other id
    QVERIFY(!(biid1 < n1));
    QVERIFY(n2 < e2id);
    QVERIFY(!(e2id < n2));

    QVERIFY(!idList.contains(n2));
    idList << n1;
    QVERIFY(idList.contains(n2)); // any two null ids are equal, so .contains should work.

    QMap<QOrganizerItemId, int> testMap;
    testMap.insert(e1id, 1);
    testMap.insert(biid1, 12);
    testMap.insert(biid2, 11);
    testMap.insert(t1id, 6);
    testMap.insert(n1, 12);

    QCOMPARE(testMap.value(e1id), 1);
    QCOMPARE(testMap.value(n2), 12); // again, n1 == n2.
    QCOMPARE(testMap.value(biid1), 12);
    QCOMPARE(testMap.value(biid2), 11);
    QCOMPARE(testMap.value(t1id), 6);
    QCOMPARE(testMap.value(QOrganizerItemId()), 12); // again, n1 == null

    QVERIFY(testMap.size() == 5);
    testMap.remove(QOrganizerItemId());
    QVERIFY(testMap.size() == 4);
    QVERIFY(testMap.value(QOrganizerItemId()) != 12); // removed this entry.
}

void tst_QOrganizerManager::emptyItemManipulation()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerItem i;
    QOrganizerEvent e;
    QOrganizerTodo t;

    // attempt to save an empty item
    if (cm->saveItem(&i)) {
        // if the backend allowed us to save it, it should definitely allow us to remove it again.
        QVERIFY(cm->removeItem(i.id()));
    } else {
        // if the backend didn't allow us to save it, there should be nothing to remove.
        QVERIFY(!cm->removeItem(i.id()));
    }

    // attempt to save an empty event.
    if (cm->saveItem(&e)) {
        // if the backend allowed us to save it, it should definitely allow us to remove it again.
        QVERIFY(cm->removeItem(e.id()));
    } else {
        // if the backend didn't allow us to save it, there should be nothing to remove.
        QVERIFY(!cm->removeItem(e.id()));
    }

    // attempt to save an empty event.
    if (cm->saveItem(&t)) {
        // if the backend allowed us to save it, it should definitely allow us to remove it again.
        QVERIFY(cm->removeItem(t.id()));
    } else {
        // if the backend didn't allow us to save it, there should be nothing to remove.
        QVERIFY(!cm->removeItem(t.id()));
    }

    // now attempt to remove some invalid ids.
    QOrganizerItemId invalidId;
    QVERIFY(!cm->removeItem(invalidId)); // null id
    invalidId = makeItemId(50);
    QVERIFY(!cm->removeItem(invalidId)); // id from different manager
}

void tst_QOrganizerManager::partialSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QList<QOrganizerItem> items;
    QOrganizerEvent event = QOrganizerEvent();
    event.setDisplayLabel("One");
    event.setStartDateTime(QDateTime(QDate(2010, 12, 25), QTime(1, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 12, 25), QTime(1, 30, 0)));
    event.setDescription("One description");
    items.append(event);

    event = QOrganizerEvent();
    event.setDisplayLabel("Two");
    event.setStartDateTime(QDateTime(QDate(2010, 12, 25), QTime(2, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 12, 25), QTime(2, 30, 0)));
    event.setDescription("Two description");
    items.append(event);

    event = QOrganizerEvent();
    event.setDisplayLabel("Three");
    event.setStartDateTime(QDateTime(QDate(2010, 12, 25), QTime(3, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 12, 25), QTime(3, 30, 0)));
    items.append(event);

    event = QOrganizerEvent();
    event.setDisplayLabel("Four");
    event.setStartDateTime(QDateTime(QDate(2010, 12, 25), QTime(4, 0, 0)));
    event.setEndDateTime(QDateTime(QDate(2010, 12, 25), QTime(4, 30, 0)));
    items.append(event);

    // First save these items
    QVERIFY(cm->saveItems(&items));
    QList<QOrganizerItem> originalItems = items;

    items[0].setDescription("One changed description");

    // 0) empty mask == full save
    QVERIFY(cm->saveItems(&items));

    // That should have updated everything
    QOrganizerItem a = cm->item(originalItems[0].id());
    QVERIFY(a.description() == "One changed description");

    // 1) Change the description for b, mask it out
    items[1].setDescription("Two changed description");
    QVERIFY(cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeEventTime));
    QVERIFY(cm->errorMap().isEmpty());

    QOrganizerItem b = cm->item(originalItems[1].id());
    QCOMPARE(b.description(), QString("Two description"));

    // 2) save a modified detail in the mask
    items[1].setDescription("Two changed description");

    QVERIFY(cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeDescription));
    QVERIFY(cm->errorMap().isEmpty());
    b = cm->item(originalItems[1].id());
    QCOMPARE(b.description(), QString("Two changed description"));

    // 3) Remove a description
    QOrganizerItemDescription desc = items[1].detail(QOrganizerItemDetail::TypeDescription);
    QVERIFY(items[1].removeDetail(&desc));
    // Mask it out, so it shouldn't work.
    QVERIFY(cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeEventTime));
    QVERIFY(cm->errorMap().isEmpty());
    b = cm->item(originalItems[1].id());
    QCOMPARE(b.details(QOrganizerItemDetail::TypeDescription).count(), 1);
    // Now include it in the mask
    QVERIFY(cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeDescription));
    QVERIFY(cm->errorMap().isEmpty());
    b = cm->item(originalItems[1].id());
    QCOMPARE(b.details(QOrganizerItemDetail::TypeDescription).count(), 0);

    // 4 - New item, no details in the mask
    QOrganizerItem newItem = originalItems[3];
    newItem.setId(QOrganizerItemId());

    items.append(newItem); // this is items[4]
    QVERIFY(cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeTag));
    QVERIFY(cm->errorMap().isEmpty());
    QVERIFY(!items[4].id().isNull()); // Saved
    b = cm->item(items[4].id());
    QCOMPARE(b.details(QOrganizerItemDetail::TypeDisplayLabel).count(), 0); // not saved
    QCOMPARE(b.details(QOrganizerItemDetail::TypeEventTime).count(), 0); // not saved

    // 5 - New item, some details in the mask
    newItem = originalItems[2];
    newItem.setId(QOrganizerItemId());
    items.append(newItem); // this is items[5]
    QVERIFY(cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeDisplayLabel));
    QVERIFY(cm->errorMap().isEmpty());
    QVERIFY(!items[5].id().isNull()); // Saved
    b = cm->item(items[5].id());
    QCOMPARE(b.details(QOrganizerItemDetail::TypeDisplayLabel).count(), 1);
    QCOMPARE(b.details(QOrganizerItemDetail::TypeEventTime).count(), 0); // not saved

    // 6 Have a non existing item in the middle followed by a save error
    cm->removeItem(items[4].id());
    QOrganizerItemDetail badDetail(QOrganizerItemDetail::TypeUndefined);
    badDetail.setValue(101, "BadValue");
    items[5].saveDetail(&badDetail);
    QVERIFY(!cm->saveItems(&items, QList<QOrganizerItemDetail::DetailType>() << QOrganizerItemDetail::TypeUndefined));
    QMap<int, QOrganizerManager::Error> errorMap = cm->errorMap();
//    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[4], QOrganizerManager::DoesNotExistError);
//    QCOMPARE(errorMap[5], QOrganizerManager::InvalidDetailError);
}

void tst_QOrganizerManager::dateRange()
{
    QFETCH(QOrganizerItem, item);
    QFETCH(QDateTime, startPeriod);
    QFETCH(QDateTime, endPeriod);
    QFETCH(bool, result);
    QCOMPARE(QOrganizerManagerEngine::isItemBetweenDates(item, startPeriod, endPeriod), result);
}

void tst_QOrganizerManager::dateRange_data()
{
    QTest::addColumn<QOrganizerItem>("item");
    QTest::addColumn<QDateTime>("startPeriod");
    QTest::addColumn<QDateTime>("endPeriod");
    QTest::addColumn<bool>("result");

    QOrganizerEvent ev;
    ev.setStartDateTime(QDateTime(QDate(2010, 10, 10), QTime(10,0,0)));
    ev.setEndDateTime(QDateTime(QDate(2010, 10, 12), QTime(11,0,0)));

    QTest::newRow("event - month") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,1)) << QDateTime(QDate(2010,10,31)) << true;
    QTest::newRow("event - first day") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,10)) << QDateTime(QDate(2010,10,10), QTime(23,59,59)) << true;
    QTest::newRow("event - second day") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,11)) << QDateTime(QDate(2010,10,11), QTime(23,59,59)) << true;
    QTest::newRow("event - last day") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,12)) << QDateTime(QDate(2010,10,12), QTime(23,59,59)) << true;
    QTest::newRow("event - undefined period") << QOrganizerItem(ev) << QDateTime() << QDateTime() << true;
    QTest::newRow("event - undefined start") << QOrganizerItem(ev) << QDateTime() << QDateTime(QDate(2010,10,11)) << true;
    QTest::newRow("event - undefined end") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,11)) << QDateTime() << true;
    QTest::newRow("event - before") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,8)) << QDateTime(QDate(2010,10,9)) << false;
    QTest::newRow("event - after") << QOrganizerItem(ev) << QDateTime(QDate(2010,10,13)) << QDateTime(QDate(2010,10,14)) << false;

    QOrganizerTodo todo;
    todo.setStartDateTime(QDateTime(QDate(2010, 10, 10), QTime(10,0,0)));
    todo.setDueDateTime(QDateTime(QDate(2010, 10, 12), QTime(11,0,0)));

    QTest::newRow("todo - month") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,1)) << QDateTime(QDate(2010,10,31)) << true;
    QTest::newRow("todo - first day") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,10)) << QDateTime(QDate(2010,10,10), QTime(23,59,59)) << true;
    QTest::newRow("todo - second day") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,11)) << QDateTime(QDate(2010,10,11), QTime(23,59,59)) << true;
    QTest::newRow("todo - last day") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,12)) << QDateTime(QDate(2010,10,12), QTime(23,59,59)) << true;
    QTest::newRow("todo - undefined period") << QOrganizerItem(todo) << QDateTime() << QDateTime() << true;
    QTest::newRow("todo - undefined start") << QOrganizerItem(todo) << QDateTime() << QDateTime(QDate(2010,10,11)) << true;
    QTest::newRow("todo - undefined end") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,11)) << QDateTime() << true;
    QTest::newRow("todo - before") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,8)) << QDateTime(QDate(2010,10,9)) << false;
    QTest::newRow("todo - after") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,13)) << QDateTime(QDate(2010,10,14)) << false;

    todo.setDueDateTime(QDateTime());
    QTest::newRow("todo missing due date - undefined start") << QOrganizerItem(todo) << QDateTime() << QDateTime(QDate(2010,10,11)) << true;
    QTest::newRow("todo missing due date - undefined end") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,10)) << QDateTime() << true;

    todo.setStartDateTime(QDateTime());
    todo.setDueDateTime(QDateTime(QDate(2010, 10, 12), QTime(11,0,0)));
    QTest::newRow("todo missing start date - undefined start") << QOrganizerItem(todo) << QDateTime() << QDateTime(QDate(2010,10,13)) << true;
    QTest::newRow("todo missing start date - undefined end") << QOrganizerItem(todo) << QDateTime(QDate(2010,10,11)) << QDateTime() << true;

    QOrganizerJournal journal;
    journal.setDateTime(QDateTime(QDate(2010, 10, 10), QTime(10,0,0)));
    QTest::newRow("journal - month") << QOrganizerItem(journal) << QDateTime(QDate(2010,10,1)) << QDateTime(QDate(2010,10,31)) << true;
    QTest::newRow("journal - first day") << QOrganizerItem(journal) << QDateTime(QDate(2010,10,10)) << QDateTime(QDate(2010,10,10), QTime(23,59,59)) << true;
    QTest::newRow("journal - second day") << QOrganizerItem(journal) << QDateTime(QDate(2010,10,11)) << QDateTime(QDate(2010,10,11), QTime(23,59,59)) << false;
    QTest::newRow("journal - undefined period") << QOrganizerItem(journal) << QDateTime() << QDateTime() << true;
    QTest::newRow("journal - undefined start") << QOrganizerItem(journal) << QDateTime() << QDateTime(QDate(2010,10,11)) << true;
    QTest::newRow("journal - undefined end") << QOrganizerItem(journal) << QDateTime(QDate(2010,10,10)) << QDateTime() << true;
    QTest::newRow("journal - before") << QOrganizerItem(journal) << QDateTime(QDate(2010,10,8)) << QDateTime(QDate(2010,10,9)) << false;
    QTest::newRow("journal - after") << QOrganizerItem(journal) << QDateTime(QDate(2010,10,13)) << QDateTime(QDate(2010,10,14)) << false;

    QOrganizerNote note;
    QTest::newRow("note") << QOrganizerItem(note) << QDateTime(QDate(2010,10,1)) << QDateTime() << false;
    QTest::newRow("note - undefined period") << QOrganizerItem(note) << QDateTime() << QDateTime() << true;
}

QList<QOrganizerItemDetail> tst_QOrganizerManager::removeAllDefaultDetails(const QList<QOrganizerItemDetail>& details)
{
    QList<QOrganizerItemDetail> newlist;
    foreach (const QOrganizerItemDetail d, details) {
        if (d.type() != QOrganizerItemDetail::TypeDisplayLabel
                && d.type() != QOrganizerItemDetail::TypeItemType
                && d.type() != QOrganizerItemDetail::TypeTimestamp) {
            newlist << d;
        }
    }
    return newlist;
}

void tst_QOrganizerManager::detailOrders()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> cm(QOrganizerManager::fromUri(uri));

    QOrganizerEvent a;

    // comments
    QOrganizerItemComment comment1, comment2, comment3;

    comment1.setComment("11111111");
    comment2.setComment("22222222");
    comment3.setComment("33333333");

    a.saveDetail(&comment1);
    a.saveDetail(&comment2);
    a.saveDetail(&comment3);

    QVERIFY(cm->saveItem(&a));
    a = cm->item(a.id());

    QList<QOrganizerItemDetail> details = a.details(QOrganizerItemDetail::TypeComment);
    QVERIFY(details.count() == 3);

    comment2 = a.details(QOrganizerItemDetail::TypeComment).at(1);
    QVERIFY(a.removeDetail(&comment2));
    QVERIFY(cm->saveItem(&a));
    a = cm->item(a.id());
    details = a.details(QOrganizerItemDetail::TypeComment);
    QVERIFY(details.count() == 2);

    a.saveDetail(&comment2);
    QVERIFY(cm->saveItem(&a));
    a = cm->item(a.id());

    details = a.details(QOrganizerItemDetail::TypeComment);
    QVERIFY(details.count() == 3);

    //addresses
    {
        QOrganizerItemLocation address1, address2, address3;

        address1.setLabel("Brandl St");
        address3 = address2 = address1;

        a.saveDetail(&address1);
        a.saveDetail(&address2);
        a.saveDetail(&address3);

        QVERIFY(cm->saveItem(&a));
        a = cm->item(a.id());

        QList<QOrganizerItemDetail> details = a.details(QOrganizerItemDetail::TypeLocation);
        QVERIFY(details.count() == 1); // 1 location - they're unique

        // Detail keys for the moment are not persistent through an item save / fetch
        address3 = details.at(0);

        QVERIFY(a.removeDetail(&address3)); // remove the most recent.
        QVERIFY(cm->saveItem(&a));
        a = cm->item(a.id());
        details = a.details(QOrganizerItemDetail::TypeLocation);
        QVERIFY(details.count() == 0); // unique, remove one means none left.

        a.saveDetail(&address2);
        QVERIFY(cm->saveItem(&a));
        a = cm->item(a.id());

        details = a.details(QOrganizerItemDetail::TypeLocation);
        QVERIFY(details.count() == 1); // add one back.
    }
}

void tst_QOrganizerManager::itemType()
{
    // XXX TODO!
}

void tst_QOrganizerManager::collections()
{
    // XXX TODO: break test into smaller sub-tests (per operation).

    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> oim(QOrganizerManager::fromUri(uri));

    // delete all collections in the database so that we know there can be no interference from previous test runs.
    QList<QOrganizerCollection> allCollections = oim->collections();
    for (int i = 0; i < allCollections.size(); ++i) {
        QOrganizerCollectionId currentId = allCollections.at(i).id();
        if (currentId != oim->defaultCollectionId()) {
            oim->removeCollection(currentId);
        }
    }

    QOrganizerCollection c1, c2, c3, sc1;
    c1.setMetaData(QOrganizerCollection::KeyName, "Test One");
    c1.setMetaData(QOrganizerCollection::KeyDescription, "This collection is for testing purposes.");
    c1.setExtendedMetaData("key", "value");
    c2.setMetaData(QOrganizerCollection::KeyName, "Test Two");
    c2.setMetaData(QOrganizerCollection::KeyColor, QColor(Qt::blue));
    // c3 doesn't have any meta-data, just an id.

    QOrganizerEvent i1, i2, i3, i4, i5;
    i1.setDisplayLabel("one");
    i2.setDisplayLabel("two");
    i2.setDescription("this is the second item");
    i3.setDisplayLabel("three");
    i4.setDisplayLabel("four");
    i4.setGuid(QUuid::createUuid().toString());
    i5.setDisplayLabel("five");
//    i5.setLocation("test location address");

    // modify default collection
    if (oim->managerName() != "memory") // modifying default collection is not allowed in memory engine
    {
        int initialCollectionCount = oim->collections().size();
        QOrganizerCollectionId defaultCollectionId = oim->defaultCollectionId();
        QOrganizerCollection defaultCollection = oim->collection(defaultCollectionId);
        defaultCollection.setMetaData(QOrganizerCollection::KeyName, "NewName");
        QCOMPARE(defaultCollection.id(), defaultCollectionId);
        QVERIFY(oim->saveCollection(&defaultCollection));
        int finalCollectionCount = oim->collections().size();
        QCOMPARE(finalCollectionCount, initialCollectionCount);
        QCOMPARE(oim->collection(defaultCollectionId).metaData(QOrganizerCollection::KeyName).toString(), QString("NewName"));
        QCOMPARE(defaultCollection.id(), defaultCollectionId);
    }

    // first test
    {
        // save a collection
        QVERIFY(c1.id().isNull()); // should have a null id to start with.
        QVERIFY(oim->saveCollection(&c1));
        QVERIFY(!c1.id().isNull()); // should have been set by the save operation
        QVERIFY(oim->collections().contains(c1));
        sc1 = oim->collection(c1.id());
        QVERIFY(sc1.metaData(QOrganizerCollection::KeyName) == "Test One");
        QVERIFY(sc1.metaData(QOrganizerCollection::KeyDescription) == "This collection is for testing purposes.");
        QVERIFY(sc1.extendedMetaData("key") == "value");

        // save an item in that collection
        QOrganizerItemCollectionFilter fil;
        fil.setCollectionId(c1.id());
        i1.setCollectionId(c1.id());
        QVERIFY(oim->saveItem(&i1));
        QVERIFY(i1.collectionId() == c1.id());

        QList<QOrganizerItem> c1Items = oim->items(QDateTime(), QDateTime(), fil);
        int itemIndex = -1;
        for (int i = 0; i < c1Items.count(); i++) {
            if (c1Items.at(i).id() == i1.id()) {
                itemIndex = i;
                break;
            }
        }
        QVERIFY(itemIndex >= 0);
        QVERIFY(oim->items(QDateTime(), QDateTime(), fil).contains(i1) || isSuperset(c1Items.at(itemIndex), i1));

        fil.setCollectionId(oim->defaultCollectionId());
        QVERIFY(!oim->items(QDateTime(), QDateTime(), fil).contains(i1)); // it should not be in the default collection.
    }

    // second test
    {
        // save multiple collections. // XXX TODO: batch save for collections?
        int originalColCount = oim->collections().count();
        QVERIFY(oim->saveCollection(&c2));
        QVERIFY(oim->saveCollection(&c3));
        QVERIFY(oim->collections().count() == (originalColCount + 2));

        // save i5 in c3 as a canary value.
        i5.setCollectionId(c3.id());

        // save multiple items in collection c2
        QList<QOrganizerItem> saveList;
        i2.setCollectionId(c2.id());
        i3.setCollectionId(c2.id());
        i4.setCollectionId(c2.id());
        saveList << i2 << i3 << i4 << i5;
        int originalItemCount = oim->items().count();
        QVERIFY(oim->saveItems(&saveList));
        i2 = saveList.at(0); // update from save list because manager might have added details / set ids etc.
        i3 = saveList.at(1);
        i4 = saveList.at(2);
        i5 = saveList.at(3);
        QList<QOrganizerItem> fetchedItems = oim->items();
        QCOMPARE(fetchedItems.count(), originalItemCount + 4);
        //fails here
        QVERIFY(fetchedItems.contains(i2)); // these three should have been added
        QVERIFY(fetchedItems.contains(i3));
        QVERIFY(fetchedItems.contains(i4));
        QVERIFY(fetchedItems.contains(i5)); // i5 should not have been removed.

        // update a collection shouldn't remove its items.
        c2.setMetaData(QOrganizerCollection::KeyName, "Test Two Updated");
        QVERIFY(oim->saveCollection(&c2));
        fetchedItems = oim->items();
        QVERIFY(fetchedItems.contains(i2)); // no items should have been removed
        QVERIFY(fetchedItems.contains(i3)); // nor should they have changed collection.
        QVERIFY(fetchedItems.contains(i4));
        QVERIFY(fetchedItems.contains(i5));

        // exceptions must be saved in the same collection as their parent.
        QOrganizerEvent recurringEvent;
        recurringEvent.setDescription("A recurring test event parent.");
        //            recurringEvent.setLocation("Some Location");
        recurringEvent.setStartDateTime(QDateTime(QDate(2010,10,5), QTime(10,30)));
        recurringEvent.setEndDateTime(QDateTime(QDate(2010,10,5), QTime(11,30)));
        QOrganizerRecurrenceRule rrule;
        rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        rrule.setLimit(5); // count limited.
        recurringEvent.setRecurrenceRule(rrule);
        recurringEvent.setCollectionId(c2.id());
        QVERIFY(oim->saveItem(&recurringEvent));
        recurringEvent = oim->item(recurringEvent.id()); // reload it.
        QVERIFY(recurringEvent.collectionId() == c2.id());
        QList<QOrganizerItem> occ(oim->itemOccurrences(recurringEvent, QDateTime(), QDateTime()));
        QVERIFY(occ.size() == 5);
        QOrganizerEventOccurrence someException = occ.at(2); // there should be five, so this shouldn't segfault.
        //            someException.setLocation("Other Location");
        someException.setCollectionId(c3.id()); // different to parent.
        QVERIFY(!oim->saveItem(&someException)); // shouldn't work.
        someException.setCollectionId(c2.id()); // same as parent.
        QVERIFY(oim->saveItem(&someException)); // should work.

        // remove a collection, removes its items.
        QVERIFY(oim->removeCollection(c2.id()));
        fetchedItems = oim->items();
        QCOMPARE(fetchedItems.count(), originalItemCount + 1); // i5 should remain, i2->i4 should be removed.
        QVERIFY(!fetchedItems.contains(i2)); // these three should have been removed
        QVERIFY(!fetchedItems.contains(i3));
        QVERIFY(!fetchedItems.contains(i4));
        QVERIFY(!fetchedItems.contains(recurringEvent)); // the parent
        QVERIFY(!fetchedItems.contains(someException));  // and exceptions too.
        QVERIFY(fetchedItems.contains(i5)); // i5 should not have been removed.

        // attempt to save an item in a non-existent collection should fail.
        i2.setId(QOrganizerItemId()); // reset Id so save can succeed...
        i2.setCollectionId(c2.id());
        QVERIFY(!oim->saveItem(&i2));
        fetchedItems = oim->items();
        QVERIFY(!fetchedItems.contains(i2)); // shouldn't have been added.
        QVERIFY(fetchedItems.contains(i5)); // i5 should not have been removed.
    }
}

void tst_QOrganizerManager::testReminder()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> oim(QOrganizerManager::fromUri(uri));

    /*audible reminder test*/
    QOrganizerItemAudibleReminder audioReminder;
    QOrganizerEvent oi;
    oi.setDisplayLabel("test reminder");
    audioReminder.setRepetition(3, 100);
    QVERIFY(oi.saveDetail(&audioReminder));
    oi.setStartDateTime(QDateTime::currentDateTime());
    QVERIFY(oim->saveItem(&oi));

    QList<QOrganizerItem> fetchedItems = oim->items();
    //After adding StartDateTime detail the item compare function does not work. Need fix later
    //QVERIFY(fetchedItems.contains(oi));
    foreach (QOrganizerItem item, fetchedItems) {
        if (oi.id() == item.id())
            QVERIFY(item.detail(QOrganizerItemDetail::TypeAudibleReminder) == audioReminder);
    }

    //Test SecondsBeforeStart properties
    audioReminder.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(oi.saveDetail(&audioReminder));
    QVERIFY(oim->saveItem (&oi));
    fetchedItems = oim->items();
    //After adding StartDateTime detail the item compare function does not work. Need fix later
    //QVERIFY(fetchedItems.contains(oi));
    foreach (QOrganizerItem item, fetchedItems) {
        if (oi.id() == item.id())
            QVERIFY(item.detail(QOrganizerItemDetail::TypeAudibleReminder) == audioReminder);
    }

    // update
    audioReminder.setSecondsBeforeStart(300);
    audioReminder.setDataUrl(QUrl("http://www.test.com"));
    QVERIFY(audioReminder.dataUrl() == QUrl("http://www.test.com"));
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeAudibleReminder) != audioReminder);
    QVERIFY(oi.saveDetail(&audioReminder));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeAudibleReminder).size() == 1); // should update, not add another
    QVERIFY(oi.detail(QOrganizerItemDetail::TypeAudibleReminder) == audioReminder);
    oim->saveItem (&oi);
    fetchedItems = oim->items();
    // After adding StartDateTime detail the item compare function does not work. Need fix later
    //QVERIFY(fetchedItems.contains(oi));
    foreach (QOrganizerItem item, fetchedItems) {
        if (oi.id() == item.id())
            QVERIFY(item.detail(QOrganizerItemDetail::TypeAudibleReminder) == audioReminder);
    }

    // remove
    QVERIFY(oi.removeDetail(&audioReminder));
    QVERIFY(oi.details(QOrganizerItemDetail::TypeAudibleReminder).size() == 0);
    oim->saveItem (&oi);
    fetchedItems = oim->items();
    // After adding StartDateTime detail the item compare function does not work. Need fix later
    //QVERIFY(fetchedItems.contains(oi));
    foreach (QOrganizerItem item, fetchedItems) {
        if (oi.id() == item.id())
            QVERIFY(item.detail(QOrganizerItemDetail::TypeAudibleReminder) == oi.detail(QOrganizerItemDetail::TypeAudibleReminder));
    }

    /*Email reminder test*/
    QOrganizerItemEmailReminder emailReminder;
    QOrganizerEvent emailEvent;
    QVERIFY(emailReminder.reminderType() == QOrganizerItemReminder::EmailReminder);
    emailReminder.setRepetition(3, 100);

    QVERIFY(emailEvent.saveDetail(&emailReminder));
    QVERIFY(oim->saveItem (&emailEvent));
    fetchedItems = oim->items();

    QVERIFY(fetchedItems.contains(emailEvent));

    emailReminder.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(emailReminder.secondsBeforeStart() == 30);
    QVERIFY(emailEvent.saveDetail(&emailReminder));
    QVERIFY(oim->saveItem (&emailEvent));
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(emailEvent));

    // update
    emailReminder.setSecondsBeforeStart(300);
    emailReminder.setContents("subject", "body", QVariantList());
    emailReminder.setRecipients(QStringList() << "recipient" << "other recipient");
    QVERIFY(emailReminder.subject() == QString("subject"));
    QVERIFY(emailReminder.body() == QString("body"));
    QVERIFY(emailReminder.attachments() == QVariantList());
    QVERIFY(emailReminder.recipients() == (QStringList() << "recipient" << "other recipient"));
    QVERIFY(emailEvent.detail(QOrganizerItemDetail::TypeEmailReminder) != emailReminder);
    QVERIFY(emailEvent.saveDetail(&emailReminder));
    oim->saveItem (&emailEvent);
    fetchedItems = oim->items();

//    dumpOrganizerItem (emailEvent);
//    foreach (QOrganizerItem item, fetchedItems) {
//        if (emailEvent.id() == item.id())
//            dumpOrganizerItem (item);
//    }

    QVERIFY(fetchedItems.contains(emailEvent));

    // remove
    QVERIFY(emailEvent.removeDetail(&emailReminder));
    QVERIFY(emailEvent.details(QOrganizerItemDetail::TypeEmailReminder).size() == 0);
    oim->saveItem (&emailEvent);
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(emailEvent));

    /*Visual reminder test*/
    QOrganizerItemVisualReminder visualReminder;
    QOrganizerEvent visualEvent;
    QVERIFY(visualReminder.reminderType() == QOrganizerItemReminder::VisualReminder);
    visualReminder.setRepetition(3, 100);

    QVERIFY(visualEvent.saveDetail(&visualReminder));
    QVERIFY(oim->saveItem (&visualEvent));
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(visualEvent));

    visualReminder.setSecondsBeforeStart(30); // reminder, 30 seconds before the item is due to start.
    QVERIFY(visualReminder.secondsBeforeStart() == 30);
    QVERIFY(visualEvent.saveDetail(&visualReminder));
    QVERIFY(oim->saveItem (&visualEvent));
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(visualEvent));

    // update
    visualReminder.setSecondsBeforeStart(300);
    visualReminder.setMessage("test");
    visualReminder.setDataUrl(QUrl("http://www.test.com"));
    QVERIFY(visualReminder.message() == QString("test"));
    QVERIFY(visualReminder.dataUrl() == QUrl("http://www.test.com"));
    QVERIFY(visualEvent.detail(QOrganizerItemDetail::TypeVisualReminder) != visualReminder);
    QVERIFY(visualEvent.saveDetail(&visualReminder));
    oim->saveItem (&visualEvent);
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(visualEvent));

    // remove
    QVERIFY(visualEvent.removeDetail(&visualReminder));
    QVERIFY(visualEvent.details(QOrganizerItemDetail::TypeVisualReminder).size() == 0);
    oim->saveItem (&visualEvent);
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(visualEvent));

    /*Event with more reminder*/
    QOrganizerEvent bigEvent;
    bigEvent.saveDetail(&audioReminder);
    bigEvent.saveDetail(&visualReminder);
    bigEvent.saveDetail(&emailReminder);
    oim->saveItem (&bigEvent);
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(bigEvent));

    // remove
    QVERIFY(bigEvent.removeDetail(&visualReminder));
    oim->saveItem (&bigEvent);
    fetchedItems = oim->items();
    QVERIFY(fetchedItems.contains(bigEvent));
}

void tst_QOrganizerManager::testIntersectionFilter()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));

    // initialize db
    mgr->removeItems(mgr->itemIds());
    QCOMPARE(mgr->items().count(), 0);

    //prepare filter data
    QOrganizerCollection c1;
    QOrganizerCollection c2;
    mgr->saveCollection(&c1);
    mgr->saveCollection(&c2);
    QOrganizerItem event1;
    event1.setType(QOrganizerItemType::TypeEvent);
    QOrganizerItem event2;
    event2.setType(QOrganizerItemType::TypeEvent);
    event1.setCollectionId(c1.id());
    event2.setCollectionId(c2.id());
    mgr->saveItem(&event1);
    mgr->saveItem(&event2);
    //Test intersection filter with 2 different collection filter
    QOrganizerItemCollectionFilter cf1;
    cf1.setCollectionId(c1.id());
    QOrganizerItemCollectionFilter cf2;
    cf2.setCollectionId(c2.id());
    QOrganizerItemIntersectionFilter isf;
    isf.append(cf1);
    isf.append(cf2);
    QList<QOrganizerItem> itemList = mgr->items(QDateTime(), QDateTime(), cf1);
    QCOMPARE(itemList.size(), 1);
    QCOMPARE(itemList.at(0), event1);
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 0);

    //Test intersection filter with 2 same collection filter
    isf.remove(cf2);
    cf2.setCollectionId(c1.id());
    isf.append(cf2);
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 1);

    //Test intersection filter with 2 collection filter and one of them is bigger than another
    isf.remove(cf2);
    QSet<QOrganizerCollectionId> collectionList;
    collectionList << c1.id() << c2.id();
    cf2.setCollectionIds(collectionList);
    isf.append(cf2);
    isf.append(cf1);
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 1);

    //Bad case: one empty filter
    isf.remove(cf2);
    cf2.setCollectionId(QOrganizerCollectionId());
    isf.append(cf2);
    QCOMPARE(isf.filters().size(), 2);
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 0);

    //QOrganizerItemIdFilter test
    QOrganizerItemIdFilter idFilter;
    QList<QOrganizerItemId> ids;
    QOrganizerItem event3;
    event3.setType(QOrganizerItemType::TypeEvent);
    mgr->saveItem(&event3);
    ids << event1.id() << event3.id();
    idFilter.setIds(ids);
    itemList = mgr->items(QDateTime(), QDateTime(), idFilter);
    QCOMPARE(itemList.size(), 2);
    if (itemList.at(0).id() == event1.id()){
        QCOMPARE(itemList.at(0), event1);
        QCOMPARE(itemList.at(1), event3);
    } else {
        QCOMPARE(itemList.at(1), event1);
        QCOMPARE(itemList.at(0), event3);
    }
    //a bad id inside the list
    // NOTE: empty id in id filter will match all generated occurrences
    QOrganizerItem badEvent;
    ids.prepend(badEvent.id());
    idFilter.setIds(ids);
    itemList = mgr->items(QDateTime(), QDateTime(), idFilter);
    QCOMPARE(itemList.size(), 2);

    //intersection filter contains QOrganizerItemIdFilter and QOrganizerItemCollectionFilter
    cf2.setCollectionIds(collectionList);
    isf.clear();
    isf.append(cf2);//event1 event2
    isf.append(idFilter);//event1 event3
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 1);
    QCOMPARE(itemList.at(0), event1);//expect event1

    //3 filters intersection
    ids.clear();
    ids.append(event1.id());
    idFilter.setIds(ids);
    isf.append(idFilter);//event1
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 1);

    //empty intersection
    ids.clear();
    ids.append(event3.id());
    idFilter.setIds(ids);
    isf.append(idFilter);//event3
    itemList = mgr->items(QDateTime(), QDateTime(), isf);
    QCOMPARE(itemList.size(), 0);
}

void tst_QOrganizerManager::testNestCompoundFilter()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));
    //simple case: nest intersection filter
    //prepare filter data
    QOrganizerCollection c1;
    QOrganizerCollection c2;
    mgr->saveCollection(&c1);
    mgr->saveCollection(&c2);
    QOrganizerItem event1;
    event1.setType(QOrganizerItemType::TypeEvent);
    QOrganizerItem event2;
    event2.setType(QOrganizerItemType::TypeEvent);
    event1.setCollectionId(c1.id());
    event2.setCollectionId(c2.id());
    mgr->saveItem(&event1);
    mgr->saveItem(&event2);
    //Test intersection filter with 2 different collection filter
    QOrganizerItemCollectionFilter cf1;
    cf1.setCollectionId(c1.id());
    QOrganizerItemCollectionFilter cf2;
    cf2.setCollectionId(c2.id());
    QOrganizerItemIntersectionFilter isf;
    isf.append(cf1);

    QOrganizerItemIntersectionFilter isfLevel1;
    isfLevel1.append(isf);
    QList<QOrganizerItem> itemList = mgr->items(QDateTime(), QDateTime(), isfLevel1);
    QCOMPARE(itemList.size(), 1);

    QOrganizerItemIntersectionFilter isfLevel2;
    isfLevel2.append(isfLevel1);

    itemList = mgr->items(QDateTime(), QDateTime(), isfLevel2);
    QCOMPARE(itemList.size(), 1);

    QOrganizerItemIntersectionFilter isfLevel3;
    isfLevel3.append(isfLevel2);

    itemList = mgr->items(QDateTime(), QDateTime(), isfLevel3);
    QCOMPARE(itemList.size(), 1);

    // union filter nest in interseion filter
    QOrganizerItemUnionFilter unf;
    unf.append(cf2);
    unf.append(cf1);

    isfLevel1.append(unf);
    itemList = mgr->items(QDateTime(), QDateTime(), isfLevel1);
    QCOMPARE(itemList.size(), 1);

    isfLevel2.append(unf);
    itemList = mgr->items(QDateTime(), QDateTime(), isfLevel2);
    QCOMPARE(itemList.size(), 1);

    QList<QOrganizerItemId> idList;
    idList << event1.id() << event2.id();

    QOrganizerItemIdFilter idf;
    idf.setIds(idList);
    isfLevel3.append(idf);

    itemList = mgr->items(QDateTime(), QDateTime(), isfLevel3);
    QCOMPARE(itemList.size(), 1);

    QOrganizerItemUnionFilter unfLevel4;
    unfLevel4.append(isfLevel3);
    unfLevel4.append(idf);

    itemList = mgr->items(QDateTime(), QDateTime(), unfLevel4);
    QCOMPARE(itemList.size(), 2);

    //attach each other
    isfLevel3.append(unfLevel4);
    itemList = mgr->items(QDateTime(), QDateTime(), isfLevel3);
    QCOMPARE(itemList.size(), 1);

    //actual use case test
    int count = mgr->items().size();

    QOrganizerTodo todo;
    todo.setDisplayLabel("myTodo");
    todo.setCollectionId(c1.id());
    mgr->saveItem(&todo);
    itemList = mgr->items();
    QCOMPARE(itemList.size(), ++count);
    //collection
    QOrganizerItemCollectionFilter collectionFilter;
    collectionFilter.setCollectionId(c1.id());
    itemList = mgr->items(QDateTime(), QDateTime(), collectionFilter);
    QCOMPARE(itemList.size(), 2);
    //event
    QOrganizerItemDetailFieldFilter detailFieldFilter;
    detailFieldFilter.setDetail(QOrganizerItemDetail::TypeItemType, QOrganizerItemType::FieldType);
    detailFieldFilter.setValue(QOrganizerItemType::TypeEvent);
    itemList = mgr->items(QDateTime(), QDateTime(), detailFieldFilter);
    QVERIFY(itemList.size() >= 2);
    //event + collection
    QOrganizerItemIntersectionFilter intersFilter;
    intersFilter.append(detailFieldFilter);
    intersFilter.append(collectionFilter);
    itemList = mgr->items(QDateTime(), QDateTime(), intersFilter);
    QCOMPARE(itemList.size(), 1);
    //filter event + collection + id
    idList << event1.id() << event2.id() << todo.id();
    QOrganizerItemIdFilter idf1;
    idf1.setIds(idList);
    QOrganizerItemIntersectionFilter intersFilter2;
    intersFilter2.append(idf1);
    intersFilter2.append(intersFilter);
    itemList = mgr->items(QDateTime(), QDateTime(), intersFilter);
    QCOMPARE(itemList.size(), 1);
    QCOMPARE(itemList[0].id(), event1.id());

    //case 2: myTodo or event + collection + id
    QOrganizerItemDetailFieldFilter detailFieldFilter2;
    detailFieldFilter2.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
    detailFieldFilter2.setValue("myTodo");
    itemList = mgr->items(QDateTime(), QDateTime(), detailFieldFilter2);
    QCOMPARE(itemList.size(), 1);

    QOrganizerItemUnionFilter unf2;
    unf2.append(detailFieldFilter2);
    unf2.append(detailFieldFilter);
    itemList = mgr->items(QDateTime(), QDateTime(), unf2);
    QVERIFY(itemList.size() >= 3);

    //event or myTodo + collection
    QOrganizerItemIntersectionFilter intersFilter3;
    intersFilter3.append(unf2);
    intersFilter3.append(collectionFilter);
    itemList = mgr->items(QDateTime(), QDateTime(), intersFilter3);
    QCOMPARE(itemList.size(), 2);
    // ... + id
    QOrganizerItemIntersectionFilter intersFilter4;
    intersFilter4.append(idf1);
    intersFilter4.append(intersFilter3);
    itemList = mgr->items(QDateTime(), QDateTime(), intersFilter4);
    QCOMPARE(itemList.size(), 2);
    QVERIFY(itemList.contains(event1));
    QVERIFY(itemList.contains(todo));
}

void tst_QOrganizerManager::testUnionFilter()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));

    //prepare filter data
    QOrganizerCollection c1;
    QOrganizerCollection c2;
    mgr->saveCollection(&c1);
    mgr->saveCollection(&c2);
    QOrganizerItem event1;
    event1.setType(QOrganizerItemType::TypeEvent);
    QOrganizerItem event2;
    event2.setType(QOrganizerItemType::TypeEvent);
    event1.setCollectionId(c1.id());
    event2.setCollectionId(c2.id());
    mgr->saveItem(&event1);
    mgr->saveItem(&event2);
    //Test union filter with 2 different collection filter
    QOrganizerItemCollectionFilter cf1;
    cf1.setCollectionId(c1.id());
    QOrganizerItemCollectionFilter cf2;
    cf2.setCollectionId(c2.id());
    QOrganizerItemUnionFilter unf;
    unf.append(cf1);
    unf.append(cf2);
    QList<QOrganizerItem> itemList = mgr->items(QDateTime(), QDateTime(), cf1);
    QCOMPARE(itemList.size(), 1);
    QCOMPARE(itemList.at(0), event1);
    itemList = mgr->items(QDateTime(), QDateTime(), unf);
    QCOMPARE(itemList.size(), 2);

    //Test union filter with 2 same collection filter
    unf.remove(cf2);
    cf2.setCollectionId(c1.id());
    unf.append(cf2);
    itemList = mgr->items(QDateTime(), QDateTime(), unf);
    QCOMPARE(itemList.size(), 1);

    //Test union filter with 2 collection filter and one of them is biger than another
    unf.remove(cf2);
    QSet<QOrganizerCollectionId> collectionList;
    collectionList << c1.id() << c2.id();
    cf2.setCollectionIds(collectionList);
    unf.append(cf2);
    unf.append(cf1);
    itemList = mgr->items(QDateTime(), QDateTime(), unf);
    QCOMPARE(itemList.size(), 2);

    //Bad case: one empty filter
    unf.remove(cf2);
    cf2.setCollectionId(QOrganizerCollectionId());
    unf.append(cf2);
    QCOMPARE(unf.filters().size(), 2);
    itemList = mgr->items(QDateTime(), QDateTime(), unf);
    QCOMPARE(itemList.size(), 1);

    //union filter contains QOrganizerItemIdFilter and QOrganizerItemCollectionFilter
    QList<QOrganizerItemId> ids;
    QOrganizerItemIdFilter idFilter;
    QOrganizerItem event3;
    event3.setType(QOrganizerItemType::TypeEvent);
    mgr->saveItem(&event3);
    ids << event3.id();
    idFilter.setIds(ids);

    cf2.setCollectionIds(collectionList);
    unf.clear();
    unf.append(cf2);//event1 event2
    unf.append(idFilter);// event3
    itemList = mgr->items(QDateTime(), QDateTime(), unf);
    QCOMPARE(itemList.size(), 3);//expect event1 event2 event3

    //3 filters union
    QOrganizerItem event4;
    event4.setType(QOrganizerItemType::TypeEvent);
    mgr->saveItem(&event4);
    ids.clear();
    ids.append(event4.id());
    idFilter.setIds(ids);
    unf.append(idFilter);//event4
    itemList = mgr->items(QDateTime(), QDateTime(), unf);
    QCOMPARE(itemList.size(), 4);//expect event1 event2 event3 event4
}

void tst_QOrganizerManager::testTags()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));

    // save & load
    QOrganizerEvent event;
    event.setTags(QStringList() << QString::fromLatin1("Tag1") << QString::fromLatin1("Tag2"));
    event.addTag(QString::fromLatin1("Tag3"));
    QVERIFY(mgr->saveItem(&event));
    QOrganizerItemId id = event.id();
    QOrganizerItem item = mgr->item(id);
    QVERIFY(item.tags().size() == 3);
    QVERIFY(item.tags().contains(QString::fromLatin1("Tag1")));
    QVERIFY(item.tags().contains(QString::fromLatin1("Tag2")));
    QVERIFY(item.tags().contains(QString::fromLatin1("Tag3")));

    // update
    item.addTag(QString::fromLatin1("Tag4"));QVERIFY(mgr->saveItem(&event));
    QList<QOrganizerItemDetail> details = item.details(QOrganizerItemDetail::TypeTag);
    QOrganizerItemTag tag = details.at(1);
    tag.setTag(QString::fromLatin1("Tag222"));
    item.saveDetail(&tag);
    item.removeDetail(&details[2]);
    QVERIFY(mgr->saveItem(&item));
    QOrganizerItemId id2 = item.id();
    QOrganizerItem item2 = mgr->item(id2);
    QVERIFY(item2.tags().size() == 3);
    QVERIFY(item2.tags().contains(QString::fromLatin1("Tag1")));
    QVERIFY(item2.tags().contains(QString::fromLatin1("Tag222")));
    QVERIFY(item2.tags().contains(QString::fromLatin1("Tag4")));
}

void tst_QOrganizerManager::testExtendedDetail()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));
    QOrganizerEvent event;

    // simple string
    QOrganizerItemExtendedDetail basicString;
    basicString.setName(QStringLiteral("basic-string"));
    basicString.setData(QString(QStringLiteral("Qt Everywhere")));
    event.saveDetail(&basicString);
    QVERIFY(mgr->saveItem(&event));

    event = mgr->item(event.id());
    basicString = event.detail(QOrganizerItemDetail::TypeExtendedDetail);
    QCOMPARE(basicString.name(), QStringLiteral("basic-string"));
    QCOMPARE(basicString.data().toString(), QStringLiteral("Qt Everywhere"));

    // simple list
    QOrganizerItemExtendedDetail basicList;
    basicList.setName(QStringLiteral("basic-list"));
    QVariantList basicListData;
    basicListData << QString(QStringLiteral("data 1"))
                  << QString(QStringLiteral("data 2"))
                  << QVariant(3);
    basicList.setData(basicListData);
    event.saveDetail(&basicList);
    QVERIFY(mgr->saveItem(&event));

    event = mgr->item(event.id());
    QList<QOrganizerItemDetail> extendedDetails = event.details(QOrganizerItemDetail::TypeExtendedDetail);
    QCOMPARE(extendedDetails.size(), 2);
    foreach (const QOrganizerItemExtendedDetail &extendedDetail, extendedDetails) {
        if (extendedDetail.name() == QStringLiteral("basic-list")) {
            QVariantList data = extendedDetail.data().toList();
            QCOMPARE(data.size(), 3);
            QCOMPARE(data.at(0).toString(), QStringLiteral("data 1"));
            QCOMPARE(data.at(1).toString(), QStringLiteral("data 2"));
            QCOMPARE(data.at(2).toInt(), 3);
            break;
        }
    }

    // simple map
    QOrganizerItemExtendedDetail basicMap;
    basicMap.setName(QStringLiteral("basic map"));
    QVariantMap basicMapData;
    basicMapData.insert(QString(QStringLiteral("key1")), QString(QStringLiteral("data-1")));
    basicMapData.insert(QString(QStringLiteral("key-2")), QString(QStringLiteral("data2")));
    basicMapData.insert(QString(QStringLiteral("key_3")), QVariant(1989));
    basicMap.setData(basicMapData);
    event.saveDetail(&basicMap);
    QVERIFY(mgr->saveItem(&event));

    event = mgr->item(event.id());
    extendedDetails = event.details(QOrganizerItemDetail::TypeExtendedDetail);
    QCOMPARE(extendedDetails.size(), 3);
    foreach (const QOrganizerItemExtendedDetail &extendedDetail, extendedDetails) {
        if (extendedDetail.name() == QStringLiteral("basic map")) {
            QVariantMap data = extendedDetail.data().toMap();
            QCOMPARE(data.size(), 3);
            QCOMPARE(data.value(QStringLiteral("key1")).toString(), QStringLiteral("data-1"));
            QCOMPARE(data.value(QStringLiteral("key-2")).toString(), QStringLiteral("data2"));
            QCOMPARE(data.value(QStringLiteral("key_3")).toInt(), 1989);
            break;
        }
    }

    // map inside a list
    QOrganizerItemExtendedDetail mapInList;
    mapInList.setName(QStringLiteral("map in list"));
    mapInList.setData(QVariantList() << QString(QStringLiteral("Qt is cute")) << basicMapData);
    event.saveDetail(&mapInList);
    QVERIFY(mgr->saveItem(&event));

    event = mgr->item(event.id());
    extendedDetails = event.details(QOrganizerItemDetail::TypeExtendedDetail);
    QCOMPARE(extendedDetails.size(), 4);
    foreach (const QOrganizerItemExtendedDetail &extendedDetail, extendedDetails) {
        if (extendedDetail.name() == QStringLiteral("map in list")) {
            QVariantList data = extendedDetail.data().toList();
            QCOMPARE(data.size(), 2);
            QCOMPARE(data.at(0).toString(), QStringLiteral("Qt is cute"));

            QVariantMap map = data.at(1).toMap();
            QCOMPARE(map.value(QStringLiteral("key1")).toString(), QStringLiteral("data-1"));
            QCOMPARE(map.value(QStringLiteral("key-2")).toString(), QStringLiteral("data2"));
            QCOMPARE(map.value(QStringLiteral("key_3")).toInt(), 1989);
            break;
        }
    }

    QVERIFY(mgr->removeItem(event.id()));
}

void tst_QOrganizerManager::testAttendee()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));
    QOrganizerEvent event;

    // Save item and verify
    QOrganizerEventAttendee attendee;
    attendee.setName("people");
    attendee.setAttendeeId("123456");
    attendee.setEmailAddress("people@nokia.com");
    attendee.setParticipationRole(QOrganizerEventAttendee::RoleRequiredParticipant);
    attendee.setParticipationStatus(QOrganizerEventAttendee::StatusAccepted);
    event.saveDetail(&attendee);
    QVERIFY(mgr->saveItem(&event));
    QOrganizerItemId id = event.id();
    QOrganizerItem item = mgr->item(id);
    QVERIFY(item.details(QOrganizerItemDetail::TypeEventAttendee).count() == 1);
    QVERIFY(item == event);//This will compare all details and their values

    // Update
    attendee.setName("newpeople");
    attendee.setAttendeeId("54321");
    attendee.setEmailAddress("newpeople@nokia.com");
    event.saveDetail(&attendee);
    QVERIFY(item != event);
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QVERIFY(item.details(QOrganizerItemDetail::TypeEventAttendee).count() == 1);
    QVERIFY(item == event);//This will compare all details and their values

    // Add one more attendee
    QOrganizerEventAttendee a1;
    a1.setAttendeeId("777777");
    a1.setName("people1");
    a1.setEmailAddress("people1@nokia.com");
    event.saveDetail(&a1);
    QVERIFY(item != event);
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QVERIFY(item.details(QOrganizerItemDetail::TypeEventAttendee).count() == 2);

    // Remove
    QVERIFY(event.removeDetail(&attendee));
    QVERIFY(event.details(QOrganizerItemDetail::TypeEventAttendee).size() == 1);
    QVERIFY(event.removeDetail(&a1));
    QVERIFY(event.details(QOrganizerItemDetail::TypeEventAttendee).size() == 0);
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QVERIFY(item.details(QOrganizerItemDetail::TypeEventAttendee).count() == 0);
}

void tst_QOrganizerManager::testRsvp()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));
    QOrganizerEvent event;

    // Save item and verify
    QOrganizerEventRsvp rsvp;
    rsvp.setOrganizerName("Donald Duck");
    rsvp.setOrganizerEmail("don@duck.com");
    rsvp.setResponseDate(QDate(2010, 10, 10));
    rsvp.setResponseDeadline(QDate(2010, 11, 11));
    rsvp.setParticipationRole(QOrganizerEventAttendee::RoleOrganizer);
    rsvp.setParticipationStatus(QOrganizerEventAttendee::StatusAccepted);
    rsvp.setResponseRequirement(QOrganizerEventRsvp::ResponseRequired);
    QVERIFY(event.saveDetail(&rsvp));
    QVERIFY(mgr->saveItem(&event));
    QOrganizerItemId id = event.id();
    QOrganizerItem item = mgr->item(id);
    QCOMPARE(1, item.details(QOrganizerItemDetail::TypeEventRsvp).count());
    QVERIFY(item == event);//This will compare all details and their values

    // Update
    rsvp.setOrganizerName("Mickey Mouse");
    rsvp.setOrganizerEmail("mick@mouse.com");
    rsvp.setResponseDate(QDate(2011, 11, 11));
    rsvp.setResponseDeadline(QDate(2011, 12, 12));
    rsvp.setParticipationRole(QOrganizerEventAttendee::RoleChairperson);
    rsvp.setParticipationStatus(QOrganizerEventAttendee::StatusDelegated);
    rsvp.setResponseRequirement(QOrganizerEventRsvp::ResponseNotRequired);
    QVERIFY(event.saveDetail(&rsvp));
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QCOMPARE(1, event.details(QOrganizerItemDetail::TypeEventRsvp).size());
    QVERIFY(item == event);//This will compare all details and their values

    // Remove
    QVERIFY(event.removeDetail(&rsvp));
    QCOMPARE(0, event.details(QOrganizerItemDetail::TypeEventRsvp).size());
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QVERIFY(item.details(QOrganizerItemDetail::TypeEventRsvp).count() == 0);
}

void tst_QOrganizerManager::testClassification()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));
    if (!mgr->supportedItemDetails(QOrganizerItemType::TypeEvent).contains(QOrganizerItemDetail::TypeClassification))
        QSKIP("Classification -detail not supported by this backend.");

    // Save item and verify
    QOrganizerEvent event;
    QOrganizerItemClassification classification;
    classification.setClassification(QOrganizerItemClassification::AccessPrivate);
    QVERIFY(event.saveDetail(&classification));
    QVERIFY(mgr->saveItem(&event));
    QOrganizerItemId id = event.id();
    QOrganizerItem item = mgr->item(id);
    QCOMPARE(1, item.details(QOrganizerItemDetail::TypeClassification).count());
    QVERIFY(item == event);//This will compare all details and their values

    // Update
    classification.setClassification(QOrganizerItemClassification::AccessConfidential);
    QVERIFY(event.saveDetail(&classification));
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QCOMPARE(1, event.details(QOrganizerItemDetail::TypeClassification).size());
    QVERIFY(item == event);//This will compare all details and their values

    // Remove
    QVERIFY(event.removeDetail(&classification));
    QCOMPARE(0, event.details(QOrganizerItemDetail::TypeClassification).size());
    QVERIFY(mgr->saveItem(&event));
    item = mgr->item(id);
    QVERIFY(item.details(QOrganizerItemDetail::TypeClassification).count() == 0);
}

void tst_QOrganizerManager::testVersion()
{
    QFETCH(QString, uri);
    QScopedPointer<QOrganizerManager> mgr(QOrganizerManager::fromUri(uri));
    if (!mgr->supportedItemDetails(QOrganizerItemType::TypeEvent).contains(QOrganizerItemDetail::TypeVersion))
        QSKIP("Version detail not supported by this backend.");

    QOrganizerEvent event;
    QVERIFY(event.detail(QOrganizerItemDetail::TypeVersion).isEmpty());
    QVERIFY(mgr->saveItem(&event));

    QOrganizerItemVersion version = event.detail(QOrganizerItemDetail::TypeVersion);
    QVERIFY(!version.isEmpty());

    event.setDisplayLabel("Qt rules!");
    QVERIFY(mgr->saveItem(&event));

    QOrganizerItemVersion version2 = event.detail(QOrganizerItemDetail::TypeVersion);
    QVERIFY(!version2.isEmpty());
    QVERIFY((version2.version() > version.version()) || (version2.extendedVersion() != version.extendedVersion()));
}

#if 0 // This test is currently excluded; can it be reinstated?
class errorSemanticsTester : public QObject {
    Q_OBJECT;
public:
    bool initialErrorWasDoesNotExist;
    bool slotErrorWasBadArgument;
    QOrganizerManager* mManager;

    errorSemanticsTester(QOrganizerManager* manager)
        : initialErrorWasDoesNotExist(false),
        slotErrorWasBadArgument(false),
        mManager(manager)
    {
        connect(manager, SIGNAL(itemsAdded(QList<QOrganizerItemId>)), this, SLOT(handleAdded()));
    }

public slots:
    void handleAdded()
    {
        // Make sure the initial error state is correct
        initialErrorWasDoesNotExist = mManager->error() == QOrganizerManager::DoesNotExistError;
        // Now force a different error
        mManager->removeItems(QList<QOrganizerItemId>());
        slotErrorWasBadArgument = mManager->error() == QOrganizerManager::BadArgumentError;
        // and return
    }
};
#endif

QTEST_MAIN(tst_QOrganizerManager)
#include "tst_qorganizermanager.moc"
