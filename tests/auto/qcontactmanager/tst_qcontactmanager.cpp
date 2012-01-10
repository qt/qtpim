/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
** $QT_END_LICENSE$
**
****************************************************************************/

#define QT_STATICPLUGIN

#include <QtTest/QtTest>
#include <QtGlobal>
#include <QtCore/qnumeric.h>

#include <QtContacts>
#include "../qcontactmanagerdataholder.h"

#if defined(SYMBIAN_BACKEND_S60_VERSION_31) || defined(SYMBIAN_BACKEND_S60_VERSION_32) || defined(SYMBIAN_BACKEND_S60_VERSION_50)
  // for the symbianManager() test.
  #include <e32std.h>
  #include <cntdb.h>
  #include <cntdbobs.h>
  #include <e32base.h>
  #include <s32mem.h>
  #include <cntitem.h>
  #include <cntfldst.h>
#endif
#if defined(USE_VERSIT_PLZ)
// This makes it easier to create specific QContacts
#include "qversitcontactimporter.h"
#include "qversitdocument.h"
#include "qversitreader.h"
#endif

QTCONTACTS_USE_NAMESPACE

#define QCONTACTMANAGER_REMOVE_VERSIONS_FROM_URI(params)  params.remove(QString::fromAscii(QTCONTACTS_VERSION_NAME)); \
                                                          params.remove(QString::fromAscii(QTCONTACTS_IMPLEMENTATION_VERSION_NAME))

#define QTRY_COMPARE_SIGNALS_LOCALID_COUNT(__signalSpy, __expectedCount) \
    do { \
        int __spiedSigCount = 0; \
        const int __step = 50; \
        const int __timeout = 5000; \
        for (int __i = 0; __i < __timeout; __i+=__step) { \
            /* accumulate added from signals */ \
            __spiedSigCount = 0; \
            const QList<QList<QVariant> > __spiedSignals = __signalSpy; \
            foreach (const QList<QVariant> &__arguments, __spiedSignals) { \
                foreach (QContactLocalId __localId, __arguments.first().value<QList<QContactLocalId> >()) { \
                    QVERIFY(__localId!=0); \
                    __spiedSigCount++; \
                } \
            } \
            if(__spiedSigCount == __expectedCount) { \
                break; \
            } \
            QTest::qWait(__step); \
        } \
        QCOMPARE(__spiedSigCount, __expectedCount); \
    } while(0)

//TESTED_COMPONENT=src/contacts

// to get QFETCH to work with the template expression...
typedef QMap<QString,QString> tst_QContactManager_QStringMap;
Q_DECLARE_METATYPE(tst_QContactManager_QStringMap)
Q_DECLARE_METATYPE(QList<QContactLocalId>)

/* A class that no backend can support */
class UnsupportedMetatype {
    int foo;
};
Q_DECLARE_METATYPE(UnsupportedMetatype)
Q_DECLARE_METATYPE(QContact)
Q_DECLARE_METATYPE(QContactManager::Error)
Q_DECLARE_METATYPE(Qt::CaseSensitivity)

class tst_QContactManager : public QObject
{
Q_OBJECT

public:
    tst_QContactManager();
    virtual ~tst_QContactManager();

private:
    void dumpContactDifferences(const QContact& a, const QContact& b);
    void dumpContact(const QContact &c);
    void dumpContacts(QContactManager *cm);
    bool isSuperset(const QContact& ca, const QContact& cb);
    QList<QContactDetail> removeAllDefaultDetails(const QList<QContactDetail>& details);
    void addManagers(); // add standard managers to the data
    QContact createContact(QString firstName, QString lastName, QString phoneNumber);
    void saveContactName(QContact *contact,QContactName *contactName, const QString &name) const;

    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

public slots:
    void initTestCase();
    void cleanupTestCase();
private slots:

    void doDump();
    void doDump_data() {addManagers();}

    /* Special test with special data */
    void uriParsing();
    void nameSynthesis();
    void compatibleContact();

    /* Backend-specific tests */
#if defined(SYMBIAN_BACKEND_S60_VERSION_31) || defined(SYMBIAN_BACKEND_S60_VERSION_32) || defined(SYMBIAN_BACKEND_S60_VERSION_50)
    void symbianManager();
    void symbianManager_data() {addManagers();}
#endif

    /* Tests that are run on all managers */
    void metadata();
    void nullIdOperations();
    void add();
    void update();
    void remove();
    void batch();
    void observerDeletion();
    void signalEmission();
    void displayName();
    void actionPreferences();
    void selfContactId();
    void detailOrders();
    void relationships();
    void contactType();
    void lateDeletion();
    void compareVariant();

#if defined(USE_VERSIT_PLZ)
    void partialSave();
    void partialSave_data() {addManagers();}
#endif

    /* Tests that take no data */
    void errorStayingPut();
    void ctors();
    void invalidManager();
    void memoryManager();
    void changeSet();
    void fetchHint();
    void lazyConnections();

    /* Special test with special data */
    void uriParsing_data();
    void nameSynthesis_data();
    void compatibleContact_data();
    void compareVariant_data();

    /* Tests that are run on all managers */
    void metadata_data() {addManagers();}
    void nullIdOperations_data() {addManagers();}
    void add_data() {addManagers();}
    void update_data() {addManagers();}
    void remove_data() {addManagers();}
    void batch_data() {addManagers();}
    void signalEmission_data() {addManagers();}
    void displayName_data() {addManagers();}
    void actionPreferences_data() {addManagers();}
    void selfContactId_data() {addManagers();}
    void detailOrders_data() {addManagers();}
    void relationships_data() {addManagers();}
    void contactType_data() {addManagers();}
    void lateDeletion_data() {addManagers();}
};

// Helper class that connects to a signal on ctor, and disconnects on dtor
class QTestSignalSink : public QObject {
    Q_OBJECT
public:
    // signal and object must remain valid for the lifetime
    QTestSignalSink(QObject *object, const char *signal)
        : mObject(object), mSignal(signal)
    {
        connect(object, signal, this, SLOT(ignored()));
    }

    ~QTestSignalSink()
    {
        disconnect(mObject, mSignal, this, SLOT(ignored()));
    }

public slots:
    void ignored() {}

private:
    QObject *mObject;
    const char * const mSignal;
};


/* Two backends for testing lazy signal connections */
class QContactLazyEngine2 : public QContactManagerEngineV2
{
public:
    QContactLazyEngine2() {}
    QString managerName() const {return "lazy2";}

    /*! \reimp */
    int managerVersion() const {return 0;}

    /*! \reimp */
    virtual QString synthesizedDisplayLabel(const QContact&, QContactManager::Error* error) const
    {
        *error =  QContactManager::NotSupportedError;
        return QString();
    }

    /*! \reimp */
    virtual QContact compatibleContact(const QContact&, QContactManager::Error* error) const
    {
        *error =  QContactManager::NotSupportedError;
        return QContact();
    }

    void connectNotify(const char *signal)
    {
        connectionCounts[signal]++;
    }
    void disconnectNotify(const char *signal)
    {
        connectionCounts[signal]--;
    }

    static QMap<QString, int> connectionCounts; // signal name to count
};
QMap<QString, int> QContactLazyEngine2::connectionCounts;

class QContactLazyEngine : public QContactManagerEngine
{
public:
    QContactLazyEngine() {}
    QString managerName() const {return "lazy";}

    /*! \reimp */
    int managerVersion() const {return 0;}

    /*! \reimp */
    virtual QString synthesizedDisplayLabel(const QContact&, QContactManager::Error* error) const
    {
        *error =  QContactManager::NotSupportedError;
        return QString();
    }

    /*! \reimp */
    virtual QContact compatibleContact(const QContact&, QContactManager::Error* error) const
    {
        *error =  QContactManager::NotSupportedError;
        return QContact();
    }

    void connectNotify(const char *signal)
    {
        connectionCounts[signal]++;
    }
    void disconnectNotify(const char *signal)
    {
        connectionCounts[signal]--;
    }
    static QMap<QString, int> connectionCounts; // signal name to count
};
QMap<QString, int> QContactLazyEngine::connectionCounts;

/* Static lazy engine factory */
class LazyEngineFactory : public QObject, public QContactManagerEngineFactory
{
    Q_OBJECT
    Q_INTERFACES(QtContacts::QContactManagerEngineFactory)
    public:
        QContactManagerEngine* engine(const QMap<QString, QString>& parameters, QContactManager::Error* error);
        QString managerName() const {return "testlazy";}
};

QContactManagerEngine* LazyEngineFactory::engine(const QMap<QString, QString>& parameters, QContactManager::Error* error)
{
    // Return one or the other
    if (parameters.value("version") == QString("1"))
        return new QContactLazyEngine();
    else
        return new QContactLazyEngine2();
}

Q_EXPORT_PLUGIN2(contacts_testlazy, LazyEngineFactory)
Q_IMPORT_PLUGIN(contacts_testlazy)

tst_QContactManager::tst_QContactManager()
{
}

tst_QContactManager::~tst_QContactManager()
{
}

void tst_QContactManager::initTestCase()
{
    managerDataHolder.reset(new QContactManagerDataHolder());

    /* Make sure these other test plugins are NOT loaded by default */
    // These are now removed from the list of managers in addManagers()
    //QVERIFY(!QContactManager::availableManagers().contains("testdummy"));
    //QVERIFY(!QContactManager::availableManagers().contains("teststaticdummy"));
    //QVERIFY(!QContactManager::availableManagers().contains("maliciousplugin"));
}

void tst_QContactManager::cleanupTestCase()
{
    managerDataHolder.reset(0);
}

void tst_QContactManager::dumpContactDifferences(const QContact& ca, const QContact& cb)
{
    // Try to narrow down the differences
    QContact a(ca);
    QContact b(cb);

    QContactName n1 = a.detail(QContactName::DefinitionName);
    QContactName n2 = b.detail(QContactName::DefinitionName);

    // Check the name components in more detail
    QCOMPARE(n1.firstName(), n2.firstName());
    QCOMPARE(n1.middleName(), n2.middleName());
    QCOMPARE(n1.lastName(), n2.lastName());
    QCOMPARE(n1.prefix(), n2.prefix());
    QCOMPARE(n1.suffix(), n2.suffix());
    QCOMPARE(n1.customLabel(), n2.customLabel());

    // Check the display label
    QCOMPARE(a.displayLabel(), b.displayLabel());

    // Now look at the rest
    QList<QContactDetail> aDetails = a.details();
    QList<QContactDetail> bDetails = b.details();

    // They can be in any order, so loop
    // First remove any matches
    foreach(QContactDetail d, aDetails) {
        foreach(QContactDetail d2, bDetails) {
            if(d == d2) {
                a.removeDetail(&d);
                b.removeDetail(&d2);
                break;
            }
        }
    }

    // Now dump the extra details that were unmatched in A (note that DisplayLabel and Type are always present).
    // We ignore timestamp since it can get autogenerated too
    aDetails = a.details();
    bDetails = b.details();
    foreach(QContactDetail d, aDetails) {
        if (d.definitionName() != QContactDisplayLabel::DefinitionName && d.definitionName() != QContactType::DefinitionName && d.definitionName() != QContactTimestamp::DefinitionName)
            qDebug() << "A contact had extra detail:" << d.definitionName() << d.values();
    }
    // and same for B
    foreach(QContactDetail d, bDetails) {
        if (d.definitionName() != QContactDisplayLabel::DefinitionName && d.definitionName() != QContactType::DefinitionName && d.definitionName() != QContactTimestamp::DefinitionName)
            qDebug() << "B contact had extra detail:" << d.definitionName() << d.values();
    }

    // now test specifically the display label and the type
    if (a.displayLabel() != b.displayLabel()) {
        qDebug() << "A contact display label =" << a.displayLabel();
        qDebug() << "B contact display label =" << b.displayLabel();
    }
    if (a.type() != b.type()) {
        qDebug() << "A contact type =" << a.type();
        qDebug() << "B contact type =" << b.type();
    }
}

bool tst_QContactManager::isSuperset(const QContact& ca, const QContact& cb)
{
    // returns true if contact ca is a superset of contact cb
    // we use this test instead of equality because dynamic information
    // such as presence/location, and synthesised information such as
    // display label and (possibly) type, may differ between a contact
    // in memory and the contact in the managed store.

    QContact a(ca);
    QContact b(cb);
    QList<QContactDetail> aDetails = a.details();
    QList<QContactDetail> bDetails = b.details();

    // They can be in any order, so loop
    // First remove any matches
    foreach(QContactDetail d, aDetails) {
        foreach(QContactDetail d2, bDetails) {
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
    foreach (QContactDetail d, aDetails) {
        foreach (QContactDetail d2, bDetails) {
            if (d.definitionName() == d2.definitionName()) {
                bool canRemove = true;
                QMap<QString, QVariant> d2map = d2.values();
                foreach (QString key, d2map.keys()) {
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

    // check for contact type updates
    if (!a.type().isEmpty())
        if (!b.type().isEmpty())
            if (a.type() != b.type())
                return false; // nonempty type is different.

    // Now check to see if b has any details remaining; if so, a is not a superset.
    // Note that the DisplayLabel and Type can never be removed.
    if (b.details().size() > 2
            || (b.details().size() == 2 && (b.details().value(0).definitionName() != QContactDisplayLabel::DefinitionName
                                            || b.details().value(1).definitionName() != QContactType::DefinitionName)))
        return false;
    return true;
}

void tst_QContactManager::dumpContact(const QContact& contact)
{
    QContactManager m;
    qDebug() << "Contact: " << contact.id().localId() << "(" << m.synthesizedContactDisplayLabel(contact) << ")";
    QList<QContactDetail> details = contact.details();
    foreach(QContactDetail d, details) {
        qDebug() << "  " << d.definitionName() << ":";
        qDebug() << "    Vals:" << d.values();
    }
}

void tst_QContactManager::dumpContacts(QContactManager *cm)
{
    QList<QContactLocalId> ids = cm->contactIds();

    qDebug() << "There are" << ids.count() << "contacts in" << cm->managerUri();

    foreach(QContactLocalId id, ids) {
        QContact c = cm->contact(id);
        dumpContact(c);
    }
}

void tst_QContactManager::uriParsing_data()
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

    QTest::newRow("built") << QContactManager::buildUri("manager", inparameters) << true << "manager" << inparameters;
    QTest::newRow("built with escaped parameters") << QContactManager::buildUri("manager", inparameters2) << true << "manager" << inparameters2;
    QTest::newRow("no scheme") << "this should not split" << false << QString() << tst_QContactManager_QStringMap();
    QTest::newRow("wrong scheme") << "invalidscheme:foo bar" << false << QString() << tst_QContactManager_QStringMap();
    QTest::newRow("right scheme, no colon") << "qtcontacts" << false << QString() << tst_QContactManager_QStringMap();
    QTest::newRow("no manager, colon, no params") << "qtcontacts::" << false  << "manager" << tst_QContactManager_QStringMap();
    QTest::newRow("yes manager, no colon, no params") << "qtcontacts:manager" << true << "manager" << tst_QContactManager_QStringMap();
    QTest::newRow("yes manager, yes colon, no params") << "qtcontacts:manager:" << true << "manager"<< tst_QContactManager_QStringMap();
    QTest::newRow("yes params") << "qtcontacts:manager:foo=bar&bazflag=&bar=glob" << true << "manager" << inparameters;
    QTest::newRow("yes params but misformed") << "qtcontacts:manager:foo=bar&=gloo&bar=glob" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 2") << "qtcontacts:manager:=&=gloo&bar=glob" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 3") << "qtcontacts:manager:==" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 4") << "qtcontacts:manager:&&" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 5") << "qtcontacts:manager:&goo=bar" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 6") << "qtcontacts:manager:goo&bar" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 7") << "qtcontacts:manager:goo&bar&gob" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 8") << "qtcontacts:manager:==&&==&goo=bar" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 9") << "qtcontacts:manager:foo=bar=baz" << false << "manager" << inparameters;
    QTest::newRow("yes params but misformed 10") << "qtcontacts:manager:foo=bar=baz=glob" << false << "manager" << inparameters;
    QTest::newRow("no manager but yes params") << "qtcontacts::foo=bar&bazflag=&bar=glob" << false << QString() << inparameters;
    QTest::newRow("no manager or params") << "qtcontacts::" << false << QString() << inparameters;
    QTest::newRow("no manager or params or colon") << "qtcontacts:" << false << QString() << inparameters;
}

void tst_QContactManager::addManagers()
{
    QTest::addColumn<QString>("uri");

    QStringList managers = QContactManager::availableManagers();

    /* Known one that will not pass */
    managers.removeAll("invalid");
    managers.removeAll("testdummy");
    managers.removeAll("teststaticdummy");
    managers.removeAll("maliciousplugin");
    managers.removeAll("testlazy");

    // "internal" engines
    managers.removeAll("social");
    managers.removeAll("simcard");
    managers.removeAll("com.nokia.messaging.contacts.engines.mail.contactslookup");


    foreach(QString mgr, managers) {
        QMap<QString, QString> params;
        QTest::newRow(QString("mgr='%1'").arg(mgr).toLatin1().constData()) << QContactManager::buildUri(mgr, params);
        if (mgr == "memory") {
            params.insert("id", "tst_QContactManager");
            QTest::newRow(QString("mgr='%1', params").arg(mgr).toLatin1().constData()) << QContactManager::buildUri(mgr, params);
        }
    }
}

/*
 * Helper method for creating a QContact instance with name and phone number
 * details.
 */
QContact tst_QContactManager::createContact(
    QString firstName,
    QString lastName,
    QString phoneNumber)
{
    QContact contact;
    QContactName n;

    if (!firstName.isEmpty()) {
        n.setFirstName(firstName);
    };
    if (!lastName.isEmpty()) {
        n.setLastName(lastName);
    }
    if (!n.isEmpty()) {
        contact.saveDetail(&n);
    }

    if (!phoneNumber.isEmpty()) {
        QContactPhoneNumber ph;
        ph.setNumber(phoneNumber);
        contact.saveDetail(&ph);
    }

    return contact;
}

void tst_QContactManager::saveContactName(QContact *contact, QContactName *contactName, const QString &name) const
{
    contactName->setCustomLabel(name);
    contactName->setFirstName(name);
    contactName->setLastName(name);
     contact->saveDetail(contactName);
}

void tst_QContactManager::metadata()
{
    // ensure that the backend is publishing its metadata (name / parameters / uri) correctly
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(new QContactManager("memory"));
    QVERIFY(QContactManager::buildUri(cm->managerName(), cm->managerParameters()) == cm->managerUri());
}


void tst_QContactManager::nullIdOperations()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(new QContactManager("memory"));
    QVERIFY(!cm->removeContact(QContactLocalId()));
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);


    QContact c = cm->contact(QContactLocalId());
    QVERIFY(c.id() == QContactId());
    QVERIFY(c.isEmpty());
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);
}

void tst_QContactManager::uriParsing()
{
    QFETCH(QString, uri);
    QFETCH(bool, good);
    QFETCH(QString, manager);
    QFETCH(tst_QContactManager_QStringMap, parameters);

    QString outmanager;
    QMap<QString, QString> outparameters;

    if (good) {
        /* Good split */
        /* Test splitting */
        QVERIFY(QContactManager::parseUri(uri, 0, 0)); // no out parms

        // 1 out param
        QVERIFY(QContactManager::parseUri(uri, &outmanager, 0));
        QCOMPARE(manager, outmanager);
        QVERIFY(QContactManager::parseUri(uri, 0, &outparameters));

        QCONTACTMANAGER_REMOVE_VERSIONS_FROM_URI(outparameters);

        QCOMPARE(parameters, outparameters);

        outmanager.clear();
        outparameters.clear();
        QVERIFY(QContactManager::parseUri(uri, &outmanager, &outparameters));

        QCONTACTMANAGER_REMOVE_VERSIONS_FROM_URI(outparameters);

        QCOMPARE(manager, outmanager);
        QCOMPARE(parameters, outparameters);
    } else {
        /* bad splitting */
        outmanager.clear();
        outparameters.clear();
        QVERIFY(QContactManager::parseUri(uri, 0, 0) == false);
        QVERIFY(QContactManager::parseUri(uri, &outmanager, 0) == false);
        QVERIFY(outmanager.isEmpty());
        QVERIFY(QContactManager::parseUri(uri, 0, &outparameters) == false);
        QCONTACTMANAGER_REMOVE_VERSIONS_FROM_URI(outparameters);
        QVERIFY(outparameters.isEmpty());

        /* make sure the in parameters don't change with a bad split */
        outmanager = manager;
        outparameters = parameters;
        QVERIFY(QContactManager::parseUri(uri, &outmanager, 0) == false);
        QCOMPARE(manager, outmanager);
        QVERIFY(QContactManager::parseUri(uri, 0, &outparameters) == false);
        QCONTACTMANAGER_REMOVE_VERSIONS_FROM_URI(outparameters);
        QCOMPARE(parameters, outparameters);
    }
}

void tst_QContactManager::ctors()
{
    /* test the different ctors to make sure we end up with the same uri */
    QVERIFY(QContactManager::availableManagers().count() >= 1); // invalid + something else
    QVERIFY(QContactManager::availableManagers().contains("invalid"));
    QString defaultStore = QContactManager::availableManagers().value(0);

    qDebug() << "Available managers:" << QContactManager::availableManagers();

    QMap<QString, QString> randomParameters;
    randomParameters.insert("something", "old");
    randomParameters.insert("something...", "new");
    randomParameters.insert("something ", "borrowed");
    randomParameters.insert(" something", "blue");

    QObject parent;

    QContactManager cm; // default
    QContactManager cm2(defaultStore);
    QContactManager cm3(defaultStore, QMap<QString, QString>());

    QContactManager cm9b(0); // QObject* ctor, should be same as cm2 etc
    QContactManager cm9c(&parent); // same as cm2 etc.

    QScopedPointer<QContactManager> cm5(QContactManager::fromUri(QContactManager::buildUri(defaultStore, QMap<QString, QString>())));
    QScopedPointer<QContactManager> cm6(QContactManager::fromUri(cm.managerUri())); // uri is not a name; should fail.
    QScopedPointer<QContactManager> cm9(QContactManager::fromUri(QString(), &parent));

    QVERIFY(cm9->parent() == &parent);
    QVERIFY(cm9b.parent() == 0);
    QVERIFY(cm9c.parent() == &parent);


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
    QCOMPARE(cm.managerName(), cm9b.managerName());
    QCOMPARE(cm.managerName(), cm9c.managerName());

    /* Test that we get invalid stores when we do silly things */
    QContactManager em("non existent");
    QContactManager em2("non existent", QMap<QString, QString>());
    QContactManager em3("memory", randomParameters);

    /* Also invalid, since we don't have one of these anyway */
    QScopedPointer<QContactManager> em4(QContactManager::fromUri("invalid uri"));
    QScopedPointer<QContactManager> em5(QContactManager::fromUri(QContactManager::buildUri("nonexistent", QMap<QString, QString>())));
    QScopedPointer<QContactManager> em6(QContactManager::fromUri(em3.managerUri()));


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
    QCOMPARE(em.managerParameters(), tst_QContactManager_QStringMap());
    QCOMPARE(em2.managerParameters(), tst_QContactManager_QStringMap());
    QCOMPARE(em4->managerParameters(), tst_QContactManager_QStringMap());
    QCOMPARE(em5->managerParameters(), tst_QContactManager_QStringMap());
    QCOMPARE(em3.managerParameters(), em6->managerParameters()); // memory engine discards the given params, replaces with id.


    // Finally test the platform specific engines are actually the defaults
#if defined(Q_OS_SYMBIAN)
    QCOMPARE(defaultStore, QString("symbian"));
#elif defined(Q_WS_MAEMO_6)
    QCOMPARE(defaultStore, QString("tracker"));
#elif defined(Q_WS_MAEMO_5)
    QCOMPARE(defaultStore, QString("maemo5"));
#elif defined(QT_JSONDB_ENABLED)
    QCOMPARE(defaultStore, QString("jsondb"));
#elif defined(Q_OS_WINCE)
    QCOMPARE(defaultStore, QString("wince"));
#else
    QCOMPARE(defaultStore, QString("invalid"));
#endif
}

void tst_QContactManager::doDump()
{
    // Only do this if it has been explicitly selected
    if (QCoreApplication::arguments().contains("doDump")) {
        QFETCH(QString, uri);
        QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

        dumpContacts(cm.data());
    }
}

Q_DECLARE_METATYPE(QVariant)


void tst_QContactManager::add()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QContact alice = createContact("Alice", "inWonderland", "1234567");
    int currCount = cm->contactIds().count();
    QVERIFY(cm->saveContact(&alice));
    QVERIFY(cm->error() == QContactManager::NoError);

    QVERIFY(!alice.id().managerUri().isEmpty());
    QVERIFY(alice.id().localId() != 0);
    QCOMPARE(cm->contactIds().count(), currCount+1);

    QContact added = cm->contact(alice.id().localId());
    QVERIFY(added.id() == alice.id());
    
    if (!isSuperset(added, alice)) {
        dumpContacts(cm.data());
        dumpContactDifferences(added, alice);
        QCOMPARE(added, alice);
    }

    // now a contact with many details of a particular definition
    // if the detail is not unique it should then support minimum of two of the same kind
    const int nrOfdetails = 2;
    QContact veryContactable = createContact("Very", "Contactable", "");
    for (int i = 0; i < nrOfdetails; i++) {
        QString phnStr = QString::number(i);
        QContactPhoneNumber vcphn;
        vcphn.setNumber(phnStr);
        QVERIFY(veryContactable.saveDetail(&vcphn));
    }

    // check that all the numbers were added successfully
    QVERIFY(veryContactable.details(QContactPhoneNumber::DefinitionName).size() == nrOfdetails);
    
    // check if it can be saved
    QVERIFY(cm->saveContact(&veryContactable));

    // verify save
    QContact retrievedContactable = cm->contact(veryContactable.id().localId());
    if (!isSuperset(retrievedContactable, veryContactable)) {
        dumpContactDifferences(veryContactable, retrievedContactable);
        QEXPECT_FAIL("mgr='wince'", "Number of phones supported mismatch", Continue);
        QCOMPARE(veryContactable, retrievedContactable);
    }
}

void tst_QContactManager::update()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    if (cm->managerName() == QStringLiteral("maemo5")) {
        // we specifically want to test the update semantics of the maemo5 backend
        // since there are various complexities relating to roster contacts.
        QContact mt;
        QContactName mtn;
        mtn.setFirstName("test");
        mtn.setLastName("maemo");
        QContactPhoneNumber pn;
        pn.setNumber("12345");

        mt.saveDetail(&mtn);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId()); // force reload of (persisted) contact
        QVERIFY(mt.details<QContactPhoneNumber>().count() == 0);

        // now save a single phonenumber
        mt.saveDetail(&pn);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId()); // force reload of (persisted) contact
        QVERIFY(mt.details<QContactPhoneNumber>().count() == 1);

        // edit some other existing detail and save (shouldn't duplicate the phone number)
        mtn.setMiddleName("middle");
        mt.saveDetail(&mtn);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId()); // force reload of (persisted) contact
        QCOMPARE(mt.details<QContactPhoneNumber>().count(), 1);

        // add some other detail and save (shouldn't duplicate the phone number)
        QContactEmailAddress mte;
        mte.setEmailAddress("test@test.com");
        mt.saveDetail(&mte);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId()); // force reload of (persisted) contact
        QCOMPARE(mt.details<QContactPhoneNumber>().count(), 1);

        // add another phone number detail and save (should create a single other phone number)
        QContactPhoneNumber pn2;
        pn2.setNumber("98765");
        mt.saveDetail(&pn2);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId()); // force reload of (persisted) contact
        QCOMPARE(mt.details<QContactPhoneNumber>().count(), 2);

        // here we do something tricky: we save one of the previously saved phone numbers
        // in a _different_ contact, and see if that causes problems with the overwrite vs new detail code.
        QContactPhoneNumber pn2Copy = pn2;
        QContact mt2;
        QContactName mt2n;
        mt2n.setFirstName("test2");
        mt2.saveDetail(&mt2n);
        QContactPhoneNumber shouldBeNew = pn;
        mt2.saveDetail(&shouldBeNew);
        QVERIFY(cm->saveContact(&mt2));
        mt2 = cm->contact(mt2.localId());
        QCOMPARE(mt2.details<QContactPhoneNumber>().count(), 1);
        mt2.saveDetail(&pn2);
        QVERIFY(cm->saveContact(&mt2));
        mt2 = cm->contact(mt2.localId());
        QCOMPARE(mt2.details<QContactPhoneNumber>().count(), 2);
        pn2 = pn2Copy; // reset just in case backend added some fields.

        // remove the other phone number detail, shouldn't cause side effects to the first...
        // NOTE: we need to reload the details before attempting to remove/edit them
        // because the backend can change the ids.
        QList<QContactPhoneNumber> pnums = mt.details<QContactPhoneNumber>();
        foreach (const QContactPhoneNumber& pd, pnums) {
            if (pd.number() == pn2.number())
                pn2 = pd;
            else if (pd.number() == pn.number())
                pn = pd;
        }
        mt.removeDetail(&pn2);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId()); // force reload of (persisted) contact
        QCOMPARE(mt.details<QContactPhoneNumber>().count(), 1);

        // edit the original phone number detail, shouldn't duplicate the phone number
        // NOTE: we need to reload the details before attempting to remove/edit them
        // because the backend can change the ids.
        pnums = mt.details<QContactPhoneNumber>();
        foreach (const QContactPhoneNumber& pd, pnums) {
            if (pd.number() == pn2.number())
                pn2 = pd;
            else if (pd.number() == pn.number())
                pn = pd;
        }
        pn.setNumber("54321");
        mt.saveDetail(&pn);
        cm->saveContact(&mt);
        mt = cm->contact(mt.localId());
        QCOMPARE(mt.details<QContactPhoneNumber>().count(), 1);
        QVERIFY(mt.detail<QContactPhoneNumber>() == pn);

        // we also should do the same test for other details (for example, gender).
        // if the backend cannot save multiple copies of a detail (eg, gender always overwrites)
        // it should FAIL the save operation if the contact has multiple of that detail type,
        // and set error to QContactManager::LimitReachedError.
        QContactGender mtg, mtg2;
        mtg.setGender(QContactGender::GenderFemale);
        mtg2.setGender(QContactGender::GenderMale);
        mt.saveDetail(&mtg);
        QVERIFY(cm->saveContact(&mt)); // one gender is fine
        mt.saveDetail(&mtg2);
        QVERIFY(!cm->saveContact(&mt)); // two is not
        //QCOMPARE(cm->error(), QContactManager::LimitReachedError); // should be LimitReachedError.
        mt = cm->contact(mt.localId());
        QVERIFY(mt.details<QContactGender>().count() == 1);
    }

    /* Save a new contact first */
    int contactCount = cm->contacts().size();
    QContact alice = createContact("Alice", "inWonderland", "1234567");
    QVERIFY(cm->saveContact(&alice));
    QVERIFY(cm->error() == QContactManager::NoError);
    contactCount += 1; // added a new contact.
    QCOMPARE(cm->contacts().size(), contactCount);

    /* Update name */
    QContactName name = alice.detail(QContactName::DefinitionName);
    saveContactName(&alice, &name, "updated");
    QVERIFY(cm->saveContact(&alice));
    QVERIFY(cm->error() == QContactManager::NoError);
    saveContactName(&alice, &name, "updated2");
    QVERIFY(cm->saveContact(&alice));
    QVERIFY(cm->error() == QContactManager::NoError);
    alice = cm->contact(alice.localId()); // force reload of (persisted) alice
    QContact updated = cm->contact(alice.localId());
    QContactName updatedName = updated.detail(QContactName::DefinitionName);
    updatedName.setCustomLabel("updated2");  // Necessary to ensure that updatedName and name are actually the same
    QCOMPARE(updatedName, name);
    QCOMPARE(cm->contacts().size(), contactCount); // contact count should be the same, no new contacts

    /* Test that adding a new detail doesn't cause unwanted side effects */
    int detailCount = alice.details().size();
    QContactEmailAddress email;
    email.setEmailAddress("test@example.com");
    alice.saveDetail(&email);
    QVERIFY(cm->saveContact(&alice));
    QCOMPARE(cm->contacts().size(), contactCount); // contact count shoudl be the same, no new contacts

    // This test is dangerous, since backends can add timestamps etc...
    detailCount += 1;
    QCOMPARE(detailCount, alice.details().size()); // adding a detail should cause the detail count to increase by one.

    /* Test that removal of fields in a detail works */
    QContactPhoneNumber phn = alice.detail<QContactPhoneNumber>();
    phn.setNumber("1234567");
    phn.setContexts(QContactDetail::ContextHome);
    alice.saveDetail(&phn);
    QVERIFY(cm->saveContact(&alice));
    alice = cm->contact(alice.localId()); // force reload of (persisted) alice
    QVERIFY(alice.detail<QContactPhoneNumber>().contexts().contains(QContactDetail::ContextHome)); // check context saved.
    phn = alice.detail<QContactPhoneNumber>(); // reload the detail, since it's key could have changed
    phn.setContexts(QStringList()); // remove context field.
    alice.saveDetail(&phn);
    QVERIFY(cm->saveContact(&alice));
    alice = cm->contact(alice.localId()); // force reload of (persisted) alice
    QVERIFY(alice.detail<QContactPhoneNumber>().contexts().isEmpty()); // check context removed.
    QCOMPARE(cm->contacts().size(), contactCount); // removal of a field of a detail shouldn't affect the contact count

    // This test is dangerous, since backends can add timestamps etc...
    QCOMPARE(detailCount, alice.details().size()); // removing a field from a detail should affect the detail count

    /* Test that removal of details works */
    phn = alice.detail<QContactPhoneNumber>(); // reload the detail, since it's key could have changed
    alice.removeDetail(&phn);
    QVERIFY(cm->saveContact(&alice));
    alice = cm->contact(alice.localId()); // force reload of (persisted) alice
    QVERIFY(alice.details<QContactPhoneNumber>().isEmpty()); // no such detail.
    QCOMPARE(cm->contacts().size(), contactCount); // removal of a detail shouldn't affect the contact count

    // This test is dangerous, since backends can add timestamps etc...
    //detailCount -= 1;
    //QCOMPARE(detailCount, alice.details().size()); // removing a detail should cause the detail count to decrease by one.

    if (cm->hasFeature(QContactManager::Groups)) {
        if (cm->managerName() == QStringLiteral("tracker")) {
            QWARN("The tracker backend does not support checking for existance of a contact. Skipping rest of test .");
            return;
        }
        // Try changing types - not allowed
        // from contact -> group
        alice.setType(QContactType::TypeGroup);
        QContactName na = alice.detail(QContactName::DefinitionName);
        alice.removeDetail(&na);
        QVERIFY(!cm->saveContact(&alice));
        QVERIFY(cm->error() == QContactManager::AlreadyExistsError);

        // from group -> contact
        QContact jabberwock = createContact("", "", "1234567890");
        jabberwock.setType(QContactType::TypeGroup);
        QVERIFY(cm->saveContact(&jabberwock));
        jabberwock.setType(QContactType::TypeContact);
        QVERIFY(!cm->saveContact(&jabberwock));
        QVERIFY(cm->error() == QContactManager::AlreadyExistsError);
    }
}

void tst_QContactManager::remove()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    /* Save a new contact first */
    QContact alice = createContact("Alice", "inWonderland", "1234567");
    QVERIFY(cm->saveContact(&alice));
    QVERIFY(cm->error() == QContactManager::NoError);
    QVERIFY(alice.id() != QContactId());

    /* Remove the created contact */
    const int contactCount = cm->contactIds().count();
    QVERIFY(cm->removeContact(alice.localId()));
    QCOMPARE(cm->contactIds().count(), contactCount - 1);
    QVERIFY(cm->contact(alice.localId()).isEmpty());
    QCOMPARE(cm->error(), QContactManager::DoesNotExistError);
}

void tst_QContactManager::batch()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    /* First test null pointer operations */
    QVERIFY(!cm->saveContacts(NULL, NULL));
    QVERIFY(cm->error() == QContactManager::BadArgumentError);

    QVERIFY(!cm->removeContacts(QList<QContactLocalId>(), NULL));
    QVERIFY(cm->error() == QContactManager::BadArgumentError);
    
    // Get supported name field
    QString nameField = QContactName::FieldCustomLabel;

    /* Now add 3 contacts, all valid */
    QContact a;
    QContactName na;
    na.setValue(nameField, "XXXXXX Albert");
    a.saveDetail(&na);

    QContact b;
    QContactName nb;
    nb.setValue(nameField, "XXXXXX Bob");
    b.saveDetail(&nb);

    QContact c;
    QContactName nc;
    nc.setValue(nameField, "XXXXXX Carol");
    c.saveDetail(&nc);

    QList<QContact> contacts;
    contacts << a << b << c;

    QMap<int, QContactManager::Error> errorMap;
    // Add one dummy error to test if the errors are reset
    errorMap.insert(0, QContactManager::NoError);
    QVERIFY(cm->saveContacts(&contacts, &errorMap));
    QVERIFY(cm->error() == QContactManager::NoError);
    QVERIFY(errorMap.count() == 0);

    /* Make sure our contacts got updated too */
    QVERIFY(contacts.count() == 3);
    QVERIFY(contacts.at(0).id() != QContactId());
    QVERIFY(contacts.at(1).id() != QContactId());
    QVERIFY(contacts.at(2).id() != QContactId());

    QVERIFY(contacts.at(0).detail(QContactName::DefinitionName) == na);
    QVERIFY(contacts.at(1).detail(QContactName::DefinitionName) == nb);
    QVERIFY(contacts.at(2).detail(QContactName::DefinitionName) == nc);

    /* Retrieve again */
    a = cm->contact(contacts.at(0).id().localId());
    b = cm->contact(contacts.at(1).id().localId());
    c = cm->contact(contacts.at(2).id().localId());
    QVERIFY(contacts.at(0).detail(QContactName::DefinitionName) == na);
    QVERIFY(contacts.at(1).detail(QContactName::DefinitionName) == nb);
    QVERIFY(contacts.at(2).detail(QContactName::DefinitionName) == nc);

    /* Save again, with a null error map */
    QVERIFY(cm->saveContacts(&contacts, NULL));
    QVERIFY(cm->error() == QContactManager::NoError);

    /* Now make an update to them all */
    QContactPhoneNumber number;
    number.setNumber("1234567");

    QVERIFY(contacts[0].saveDetail(&number));
    number.setNumber("234567");
    QVERIFY(contacts[1].saveDetail(&number));
    number.setNumber("34567");
    QVERIFY(contacts[2].saveDetail(&number));

    QVERIFY(cm->saveContacts(&contacts, &errorMap));
    QVERIFY(cm->error() == QContactManager::NoError);
    QVERIFY(errorMap.count() == 0);

    /* Retrieve them and check them again */
    a = cm->contact(contacts.at(0).id().localId());
    b = cm->contact(contacts.at(1).id().localId());
    c = cm->contact(contacts.at(2).id().localId());
    QVERIFY(contacts.at(0).detail(QContactName::DefinitionName) == na);
    QVERIFY(contacts.at(1).detail(QContactName::DefinitionName) == nb);
    QVERIFY(contacts.at(2).detail(QContactName::DefinitionName) == nc);

    QVERIFY(a.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(b.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(c.details<QContactPhoneNumber>().count() == 1);

    QVERIFY(a.details<QContactPhoneNumber>().at(0).number() == "1234567");
    QVERIFY(b.details<QContactPhoneNumber>().at(0).number() == "234567");
    QVERIFY(c.details<QContactPhoneNumber>().at(0).number() == "34567");

    /* Retrieve them with the batch ID fetch API */
    QList<QContactLocalId> batchIds;
    batchIds << a.localId() << b.localId() << c.localId();

    // Null error map first (doesn't crash)
    QList<QContact> batchFetch = cm->contacts(batchIds, QContactFetchHint(), 0);
    QVERIFY(cm->error() == QContactManager::NoError);
    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support customLabel, skipping...");
    QVERIFY(batchFetch.count() == 3);
    QVERIFY(batchFetch.at(0).detail(QContactName::DefinitionName) == na);
    QVERIFY(batchFetch.at(1).detail(QContactName::DefinitionName) == nb);
    QVERIFY(batchFetch.at(2).detail(QContactName::DefinitionName) == nc);

    // With error map
    batchFetch = cm->contacts(batchIds, QContactFetchHint(), &errorMap);
    QVERIFY(cm->error() == QContactManager::NoError);
    QVERIFY(errorMap.count() == 0);
    QVERIFY(batchFetch.count() == 3);
    QVERIFY(batchFetch.at(0).detail(QContactName::DefinitionName) == na);
    QVERIFY(batchFetch.at(1).detail(QContactName::DefinitionName) == nb);
    QVERIFY(batchFetch.at(2).detail(QContactName::DefinitionName) == nc);

    /* Now an empty id */
    batchIds.clear();
    batchIds << QContactLocalId() << a.localId() << b.localId() << c.localId();
    batchFetch = cm->contacts(batchIds, QContactFetchHint(), 0);
    QVERIFY(cm->error() != QContactManager::NoError);
    QVERIFY(batchFetch.count() == 4);
    QVERIFY(batchFetch.at(0).detail(QContactName::DefinitionName) == QContactDetail());
    QVERIFY(batchFetch.at(1).detail(QContactName::DefinitionName) == na);
    QVERIFY(batchFetch.at(2).detail(QContactName::DefinitionName) == nb);
    QVERIFY(batchFetch.at(3).detail(QContactName::DefinitionName) == nc);

    batchFetch = cm->contacts(batchIds, QContactFetchHint(), &errorMap);
    QVERIFY(cm->error() != QContactManager::NoError);
    QVERIFY(batchFetch.count() == 4);
    QVERIFY(errorMap.count() == 1);
    QVERIFY(errorMap[0] == QContactManager::DoesNotExistError);
    QVERIFY(batchFetch.at(0).detail(QContactName::DefinitionName) == QContactDetail());
    QVERIFY(batchFetch.at(1).detail(QContactName::DefinitionName) == na);
    QVERIFY(batchFetch.at(2).detail(QContactName::DefinitionName) == nb);
    QVERIFY(batchFetch.at(3).detail(QContactName::DefinitionName) == nc);

    /* Now multiple of the same contact */
    batchIds.clear();
    batchIds << c.localId() << b.localId() << c.localId() << a.localId() << a.localId();
    batchFetch = cm->contacts(batchIds, QContactFetchHint(), &errorMap);
    QVERIFY(cm->error() == QContactManager::NoError);
    QVERIFY(batchFetch.count() == 5);
    QVERIFY(errorMap.count() == 0);
    QVERIFY(batchFetch.at(0).detail(QContactName::DefinitionName) == nc);
    QVERIFY(batchFetch.at(1).detail(QContactName::DefinitionName) == nb);
    QVERIFY(batchFetch.at(2).detail(QContactName::DefinitionName) == nc);
    QVERIFY(batchFetch.at(3).detail(QContactName::DefinitionName) == na);
    QVERIFY(batchFetch.at(4).detail(QContactName::DefinitionName) == na);

    /* Now delete them all */
    QList<QContactLocalId> ids;
    ids << a.id().localId() << b.id().localId() << c.id().localId();
    QVERIFY(cm->removeContacts(ids, &errorMap));
    QVERIFY(errorMap.count() == 0);
    QVERIFY(cm->error() == QContactManager::NoError);

    /* Make sure the contacts really don't exist any more */
    QVERIFY(cm->contact(a.id().localId()).id() == QContactId());
    QVERIFY(cm->contact(a.id().localId()).isEmpty());
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);
    QVERIFY(cm->contact(b.id().localId()).id() == QContactId());
    QVERIFY(cm->contact(b.id().localId()).isEmpty());
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);
    QVERIFY(cm->contact(c.id().localId()).id() == QContactId());
    QVERIFY(cm->contact(c.id().localId()).isEmpty());
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);

    if (cm->managerName() == QStringLiteral("tracker")) {
        QWARN("The tracker backend does not support checking for existance of a contact. Skipping rest of test .");
        return;
    }

    /* Now try removing with all invalid ids (e.g. the ones we just removed) */
    ids.clear();
    ids << a.id().localId() << b.id().localId() << c.id().localId();
    QVERIFY(!cm->removeContacts(ids, &errorMap));
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);
    QVERIFY(errorMap.count() == 3);
    QVERIFY(errorMap.values().at(0) == QContactManager::DoesNotExistError);
    QVERIFY(errorMap.values().at(1) == QContactManager::DoesNotExistError);
    QVERIFY(errorMap.values().at(2) == QContactManager::DoesNotExistError);

    /* And again with a null error map */
    QVERIFY(!cm->removeContacts(ids, NULL));
    QVERIFY(cm->error() == QContactManager::DoesNotExistError);

    /* Try adding some new ones again, this time one with an error */
    contacts.clear();
    a.setId(QContactId());
    b.setId(QContactId());
    c.setId(QContactId());

    /* Make B the bad guy */
    QContactDetail bad("does not exist and will break if you add it");
    bad.setValue("This is also bad", "Very bad");
    b.saveDetail(&bad);

    contacts << a << b << c;

    /* Fix up B and re save it */
    QVERIFY(contacts[1].removeDetail(&bad));
    QVERIFY(cm->saveContacts(&contacts, &errorMap));
    QVERIFY(errorMap.count() == 0);
    QVERIFY(cm->error() == QContactManager::NoError);
    
    // Save and remove a fourth contact. Store the id.
    a.setId(QContactId());
    QVERIFY(cm->saveContact(&a));
    QContactLocalId removedId = a.localId();
    QVERIFY(cm->removeContact(removedId));

    /* Now delete 3 items, but with one bad argument */
    ids.clear();
    ids << contacts.at(0).id().localId();
    ids << removedId;
    ids << contacts.at(2).id().localId();

    QVERIFY(!cm->removeContacts(ids, &errorMap));
    QVERIFY(cm->error() != QContactManager::NoError);

    /* Again, the backend has the choice of either removing the successful ones, or not */
    QVERIFY(errorMap.count() > 0);
    QVERIFY(errorMap.count() <= 3);

    // A might have gone through
    if (errorMap.keys().contains(0)) {
        QVERIFY(errorMap.value(0) != QContactManager::NoError);
        QVERIFY(contacts.at(0).id() == QContactId());
    } else {
        QVERIFY(contacts.at(0).id() != QContactId());
    }

    /* B should definitely have failed */
    QVERIFY(errorMap.value(1) == QContactManager::DoesNotExistError);
    QVERIFY(ids.at(1) == removedId);

    // A might have gone through
    if (errorMap.keys().contains(2)) {
        QVERIFY(errorMap.value(2) != QContactManager::NoError);
        QVERIFY(contacts.at(2).id() == QContactId());
    } else {
        QVERIFY(contacts.at(2).id() != QContactId());
    }
}

void tst_QContactManager::invalidManager()
{
    /* Create an invalid manager */
    QContactManager manager("this should never work");
    QVERIFY(manager.managerName() == "invalid");
    QVERIFY(manager.managerVersion() == 0);

    /* also, test the other ctor behaviour is sane also */
    QContactManager anotherManager("this should never work", 15);
    QVERIFY(anotherManager.managerName() == "invalid");
    QVERIFY(anotherManager.managerVersion() == 0);

    /* Now test that all the operations fail */
    QVERIFY(manager.contactIds().count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    QContact foo;
    QContactName nf;
    nf.setLastName("Lastname");
    foo.saveDetail(&nf);

    QVERIFY(manager.synthesizedContactDisplayLabel(foo).isEmpty());
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    QVERIFY(manager.saveContact(&foo) == false);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    QVERIFY(foo.id() == QContactId());
    QVERIFY(manager.contactIds().count() == 0);

    QVERIFY(manager.contact(foo.id().localId()).id() == QContactId());
    QVERIFY(manager.contact(foo.id().localId()).isEmpty());
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    QVERIFY(manager.removeContact(foo.id().localId()) == false);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    QMap<int, QContactManager::Error> errorMap;
    errorMap.insert(0, QContactManager::NoError);
    QVERIFY(!manager.saveContacts(0, &errorMap));
    QVERIFY(manager.errorMap().count() == 0);
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QContactManager::BadArgumentError);

    /* filters */
    QContactFilter f; // matches everything
    QContactDetailFilter df;
    df.setDetailDefinitionName(QContactDisplayLabel::DefinitionName, QContactDisplayLabel::FieldLabel);
    QVERIFY(manager.contactIds(QContactFilter()).count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    QVERIFY(manager.contactIds(df).count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    QVERIFY(manager.contactIds(f | f).count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    QVERIFY(manager.contactIds(df | df).count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    QVERIFY(manager.isFilterSupported(f) == false);
    QVERIFY(manager.isFilterSupported(df) == false);

    QList<QContact> list;
    list << foo;

    QVERIFY(!manager.saveContacts(&list, &errorMap));
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    QVERIFY(!manager.removeContacts(QList<QContactLocalId>(), &errorMap));
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QContactManager::BadArgumentError);

    QList<QContactLocalId> idlist;
    idlist << foo.id().localId();
    QVERIFY(!manager.removeContacts(idlist, &errorMap));
    QVERIFY(errorMap.count() == 0);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);

    /* Self contact id */
    QVERIFY(!manager.setSelfContactId(QContactLocalId(12)));
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    QVERIFY(manager.selfContactId() == QContactLocalId());
    QVERIFY(manager.error() == QContactManager::NotSupportedError || manager.error() == QContactManager::DoesNotExistError);

    /* Relationships */
    QContact one, two;
    QContactRelationship invalidRel;
    invalidRel.setFirst(one);
    invalidRel.setSecond(two);
    QList<QContactRelationship> invalidRelList;
    invalidRelList << invalidRel;
    QVERIFY(!manager.saveRelationship(&invalidRel));
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    QVERIFY(manager.relationships().isEmpty());
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    manager.saveRelationships(&invalidRelList, NULL);
    QVERIFY(manager.error() == QContactManager::NotSupportedError);
    manager.removeRelationships(invalidRelList, NULL);
    QVERIFY(manager.error() == QContactManager::NotSupportedError || manager.error() == QContactManager::DoesNotExistError);

    /* Capabilities */
    QVERIFY(manager.supportedDataTypes().count() == 0);
    QVERIFY(!manager.hasFeature(QContactManager::ActionPreferences));
}

void tst_QContactManager::memoryManager()
{
    QMap<QString, QString> params;
    QContactManager m1("memory");
    params.insert("random", "shouldNotBeUsed");
    QContactManager m2("memory", params);
    params.insert("id", "shouldBeUsed");
    QContactManager m3("memory", params);
    QContactManager m4("memory", params);
    params.insert("id", QString(""));
    QContactManager m5("memory", params); // should be another anonymous

    QVERIFY(m1.hasFeature(QContactManager::ActionPreferences));
    QVERIFY(m1.hasFeature(QContactManager::Anonymous));

    // add a contact to each of m1, m2, m3
    QContact c;
    QContactName nc;
    nc.setFirstName("John");
    nc.setLastName("Civilian");
    c.saveDetail(&nc);
    m1.saveContact(&c);
    c.setId(QContactId());
    QContact c2;
    QContactName nc2 = c2.detail(QContactName::DefinitionName);
    c2 = c;
    nc2.setMiddleName("Public");
    c2.saveDetail(&nc2);
    m2.saveContact(&c2);            // save c2 first; c will be given a higher id
    m2.saveContact(&c);             // save c to m2
    c.setId(QContactId());
    nc.setSuffix("MD");
    c.saveDetail(&nc);
    m3.saveContact(&c);

    /* test that m1 != m2 != m3 and that m3 == m4 */

    // check the counts are correct - especially note m4 and m3.
    QCOMPARE(m1.contactIds().count(), 1);
    QCOMPARE(m2.contactIds().count(), 2);
    QCOMPARE(m3.contactIds().count(), 1);
    QCOMPARE(m4.contactIds().count(), 1);
    QCOMPARE(m5.contactIds().count(), 0);

    // remove c2 from m2 - ensure that this doesn't affect any other manager.
    m2.removeContact(c2.id().localId());
    QCOMPARE(m1.contactIds().count(), 1);
    QCOMPARE(m2.contactIds().count(), 1);
    QCOMPARE(m3.contactIds().count(), 1);
    QCOMPARE(m4.contactIds().count(), 1);
    QCOMPARE(m5.contactIds().count(), 0);

    // check that the contacts contained within are different.
    // note that in the m1->m2 case, only the id will be different!
    QVERIFY(m1.contact(m1.contactIds().at(0)) != m2.contact(m2.contactIds().at(0)));
    QVERIFY(m1.contact(m1.contactIds().at(0)) != m3.contact(m3.contactIds().at(0)));
    QVERIFY(m2.contact(m2.contactIds().at(0)) != m3.contact(m3.contactIds().at(0)));
    QVERIFY(m3.contact(m3.contactIds().at(0)) == m4.contact(m4.contactIds().at(0)));

    // now, we should be able to remove from m4, and have m3 empty
    QVERIFY(m4.removeContact(c.id().localId()));
    QCOMPARE(m3.contactIds().count(), 0);
    QCOMPARE(m4.contactIds().count(), 0);
    QCOMPARE(m5.contactIds().count(), 0);
}

#if defined(SYMBIAN_BACKEND_S60_VERSION_31) || defined(SYMBIAN_BACKEND_S60_VERSION_32) || defined(SYMBIAN_BACKEND_S60_VERSION_50)
/* Some symbian-specific unit tests. */
void tst_QContactManager::symbianManager()
{
    QFETCH(QString, uri);
    QString managerName;
    QMap<QString, QString> managerParameters;
    QContactManager::parseUri(uri, &managerName, &managerParameters);
    if (managerName != QString("symbian"))
        return;

    /* Firstly, a test for invalid storage type crash - QTMOBILITY-470 */
    // open the contact database, and create a new contact
    CContactDatabase* cntdb = CContactDatabase::OpenL();
    CleanupStack::PushL(cntdb);
    CContactItem* testItem = CContactCard::NewLC();

    // create a new thumbnail field with (invalid) storage type KStorageTypeText instead of KStorageTypeStore
    CContactItemField* thumbnailField;
    thumbnailField = CContactItemField::NewLC(KStorageTypeText, KUidContactFieldPicture);
    thumbnailField->SetMapping(KUidContactFieldVCardMapPHOTO);
    thumbnailField->AddFieldTypeL(KUidContactFieldVCardMapBMP);
    thumbnailField->ResetStore();

    // set the thumbnail data in the thumbnail field, and add it to the contact
    _LIT8(KThumbnailDataString, "Dummy Thumbnail Data String");
    thumbnailField->StoreStorage()->SetThingL(KThumbnailDataString);
    testItem->AddFieldL(*thumbnailField);
    CleanupStack::Pop(thumbnailField);

    // save the updated contact.
    cntdb->CommitContactL(*testItem);
    cntdb->CloseContactL(testItem->Id());
    CleanupStack::PopAndDestroy(2); // testItem, cntdb

    // force database to read thumbnail with invalid storage type.  crash if not handled properly.
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));
    QList<QContact> allContacts = cm->contacts();
}
#endif

void tst_QContactManager::nameSynthesis_data()
{
    QTest::addColumn<QString>("expected");

    QTest::addColumn<bool>("addname");
    QTest::addColumn<QString>("prefix");
    QTest::addColumn<QString>("first");
    QTest::addColumn<QString>("middle");
    QTest::addColumn<QString>("last");
    QTest::addColumn<QString>("suffix");

    QTest::addColumn<bool>("addcompany");
    QTest::addColumn<QString>("company");

    QTest::addColumn<bool>("addname2");
    QTest::addColumn<QString>("secondprefix");
    QTest::addColumn<QString>("secondfirst");
    QTest::addColumn<QString>("secondmiddle");
    QTest::addColumn<QString>("secondlast");
    QTest::addColumn<QString>("secondsuffix");

    QTest::addColumn<bool>("addcompany2");
    QTest::addColumn<QString>("secondcompany");

    QString e; // empty string.. gets a work out

    /* Various empty ones */
    QTest::newRow("empty contact") << e
            << false << e << e << e << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("empty name") << e
            << true << e << e << e << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("empty names") << e
            << true << e << e << e << e << e
            << false << e
            << true << e << e << e << e << e
            << false << e;
    QTest::newRow("empty org") << e
            << false << e << e << e << e << e
            << true << e
            << false << e << e << e << e << e
            << true << e;
    QTest::newRow("empty orgs") << e
            << false << e << e << e << e << e
            << true << e
            << false << e << e << e << e << e
            << true << e;
    QTest::newRow("empty orgs and names") << e
            << true << e << e << e << e << e
            << true << e
            << true << e << e << e << e << e
            << true << e;

    /* Single values */
    QTest::newRow("prefix") << "Prefix"
            << true << "Prefix" << e << e << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("first") << "First"
            << true << e << "First" << e << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("middle") << "Middle"
            << true << e << e << "Middle" << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("last") << "Last"
            << true << e << e << e << "Last" << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("suffix") << "Suffix"
            << true << e << e << e << e << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;

    /* Single values in the second name */
    QTest::newRow("prefix in second") << "Prefix"
            << false << "Prefix" << e << e << e << e
            << false << e
            << true << "Prefix" << e << e << e << e
            << false << e;
    QTest::newRow("first in second") << "First"
            << false << e << "First" << e << e << e
            << false << e
            << true << e << "First" << e << e << e
            << false << e;
    QTest::newRow("middle in second") << "Middle"
            << false << e << e << "Middle" << e << e
            << false << e
            << true << e << e << "Middle" << e << e
            << false << e;
    QTest::newRow("last in second") << "Last"
            << false << e << e << e << "Last" << e
            << false << e
            << true << e << e << e << "Last" << e
            << false << e;
    QTest::newRow("suffix in second") << "Suffix"
            << false << e << e << e << e << "Suffix"
            << false << e
            << true << e << e << e << e << "Suffix"
            << false << e;

    /* Multiple name values */
    QTest::newRow("prefix first") << "Prefix First"
            << true << "Prefix" << "First" << e << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("prefix middle") << "Prefix Middle"
            << true << "Prefix" << e << "Middle" << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("prefix last") << "Prefix Last"
            << true << "Prefix" << e << e << "Last" << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("prefix suffix") << "Prefix Suffix"
            << true << "Prefix" << e << e << e << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("first middle") << "First Middle"
            << true << e << "First" << "Middle" << e << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("first last") << "First Last"
            << true << e << "First" << e << "Last" << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("first suffix") << "First Suffix"
            << true << e << "First" << e << e << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("middle last") << "Middle Last"
            << true << e << e << "Middle" << "Last" << e
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("middle suffix") << "Middle Suffix"
            << true << e << e << "Middle" << e << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("last suffix") << "Last Suffix"
            << true << e << e << e << "Last" << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;

    /* Everything.. */
    QTest::newRow("all name") << "Prefix First Middle Last Suffix"
            << true << "Prefix" << "First" << "Middle" << "Last" << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("all name second") << "Prefix First Middle Last Suffix"
            << false << "Prefix" << "First" << "Middle" << "Last" << "Suffix"
            << false << e
            << true << "Prefix" << "First" << "Middle" << "Last" << "Suffix"
            << false << e;

    /* Org */
    QTest::newRow("org") << "Company"
            << false << e << e << e << e << e
            << true << "Company"
            << false << e << e << e << e << e
            << false << e;
    QTest::newRow("second org") << "Company"
            << false << e << e << e << e << e
            << false << e
            << false << e << e << e << e << e
            << true << "Company";

    /* Mix */
    QTest::newRow("org and empty name") << "Company"
            << true << e << e << e << e << e
            << true << "Company"
            << false << e << e << e << e << e
            << false << e;

    QTest::newRow("name and empty org") << "Prefix First Middle Last Suffix"
            << true << "Prefix" << "First" << "Middle" << "Last" << "Suffix"
            << false << e
            << false << e << e << e << e << e
            << false << e;

    /* names are preferred to orgs */
    QTest::newRow("name and org") << "Prefix First Middle Last Suffix"
            << true << "Prefix" << "First" << "Middle" << "Last" << "Suffix"
            << true << "Company"
            << false << e << e << e << e << e
            << false << e;

}

void tst_QContactManager::nameSynthesis()
{
    QContactManager cm("memory");

    QFETCH(QString, expected);

    QFETCH(QString, prefix);
    QFETCH(QString, first);
    QFETCH(QString, middle);
    QFETCH(QString, last);
    QFETCH(QString, suffix);
    QFETCH(QString, company);

    QFETCH(QString, secondprefix);
    QFETCH(QString, secondfirst);
    QFETCH(QString, secondmiddle);
    QFETCH(QString, secondlast);
    QFETCH(QString, secondsuffix);
    QFETCH(QString, secondcompany);

    QFETCH(bool, addname);
    QFETCH(bool, addname2);
    QFETCH(bool, addcompany);
    QFETCH(bool, addcompany2);

    /* Test the default name synthesis code */
    QContact c;

    QContactName name, name2;
    QContactOrganization org, org2;

    name.setPrefix(prefix);
    name.setFirstName(first);
    name.setMiddleName(middle);
    name.setLastName(last);
    name.setSuffix(suffix);

    name2.setPrefix(secondprefix);
    name2.setFirstName(secondfirst);
    name2.setMiddleName(secondmiddle);
    name2.setLastName(secondlast);
    name2.setSuffix(secondsuffix);

    org.setName(company);
    org2.setName(secondcompany);

    if (addname)
        c.saveDetail(&name);
    if (addname2)
        c.saveDetail(&name2);
    if (addcompany)
        c.saveDetail(&org);
    if (addcompany2)
        c.saveDetail(&org2);

    // Finally!
    QCOMPARE(cm.synthesizedContactDisplayLabel(c), expected);
}

void tst_QContactManager::compatibleContact_data()
{
    QTest::addColumn<QContact>("input");
    QTest::addColumn<QContact>("expected");
    QTest::addColumn<QContactManager::Error>("error");

    QContact baseContact;
    QContact emptyContact;
    QContactName name;
    name.setFirstName(QStringLiteral("First"));
    baseContact.saveDetail(&name);

    {
        QTest::newRow("already compatible") << baseContact << baseContact << QContactManager::NoError;
    }

    {
        QContact contact(baseContact);
        QString unknownType("UnknownType");
        contact.setType(unknownType);
        QTest::newRow("unknown type") << contact << emptyContact << QContactManager::InvalidContactTypeError;
    }

    {
        QContact contact(baseContact);
        contact.setType(QContactType::TypeContact);
        QTest::newRow("\"Contact\" type") << contact << contact << QContactManager::NoError;
    }

    {
        QContact contact(baseContact);
        contact.setType(QContactType::TypeGroup);
        QTest::newRow("\"Group\" type") << contact << contact << QContactManager::NoError;
    }
}

void tst_QContactManager::compatibleContact()
{
    QContactManager cm("memory");

    QFETCH(QContact, input);
    QFETCH(QContact, expected);
    QFETCH(QContactManager::Error, error);
    QCOMPARE(cm.compatibleContact(input), expected);
    QCOMPARE(cm.error(), error);
}


void tst_QContactManager::observerDeletion()
{
    QContactManager *manager = new QContactManager("memory");
    QContact c;
    QVERIFY(manager->saveContact(&c));
    QContactLocalId id = c.localId();
    QContactObserver *observer = new QContactObserver(manager, id);
    Q_UNUSED(observer)
    delete manager;
    delete observer;
    // Test for bug MOBILITY-2566 - that QContactObserver doesn't crash when it is
    // destroyed after the associated QContactManager
}

void tst_QContactManager::signalEmission()
{
    QTest::qWait(500); // clear the signal queue
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> m1(QContactManager::fromUri(uri));

    qRegisterMetaType<QContactLocalId>("QContactLocalId");
    qRegisterMetaType<QList<QContactLocalId> >("QList<QContactLocalId>");
    QSignalSpy spyCA(m1.data(), SIGNAL(contactsAdded(QList<QContactLocalId>)));
    QSignalSpy spyCM(m1.data(), SIGNAL(contactsChanged(QList<QContactLocalId>)));
    QSignalSpy spyCR(m1.data(), SIGNAL(contactsRemoved(QList<QContactLocalId>)));

    QTestSignalSink casink(m1.data(), SIGNAL(contactsAdded(QList<QContactLocalId>)));
    QTestSignalSink cmsink(m1.data(), SIGNAL(contactsChanged(QList<QContactLocalId>)));
    QTestSignalSink crsink(m1.data(), SIGNAL(contactsRemoved(QList<QContactLocalId>)));

    QList<QVariant> args;
    QList<QContactLocalId> arg;
    QContact c;
    QList<QContact> batchAdd;
    QList<QContactLocalId> batchRemove;
    int addSigCount = 0; // the expected signal counts.
    int modSigCount = 0;
    int remSigCount = 0;

    // verify add emits signal added
    QContactName nc;
    saveContactName(&c, &nc, "John");
    QVERIFY(m1->saveContact(&c));
    QContactLocalId cid = c.id().localId();
    addSigCount += 1;
    QTRY_COMPARE(spyCA.count(), addSigCount);
    args = spyCA.takeFirst();
    addSigCount -= 1;
    arg = args.first().value<QList<QString> >();
    QVERIFY(arg.count() == 1);
    QCOMPARE(QContactLocalId(arg.at(0)), cid);

    QScopedPointer<QContactObserver> c1Observer(new QContactObserver(m1.data(), cid));
    QScopedPointer<QSignalSpy> spyCOM1(new QSignalSpy(c1Observer.data(), SIGNAL(contactChanged())));
    QScopedPointer<QSignalSpy> spyCOR1(new QSignalSpy(c1Observer.data(), SIGNAL(contactRemoved())));

    // verify save modified emits signal changed
    saveContactName(&c, &nc, "Citizen");
    QVERIFY(m1->saveContact(&c));
    modSigCount += 1;
    QTRY_COMPARE(spyCM.count(), modSigCount);
    QTRY_COMPARE(spyCOM1->count(), 1);
    args = spyCM.takeFirst();
    modSigCount -= 1;
    arg = args.first().value<QList<QString> >();
    QVERIFY(arg.count() == 1);
    QCOMPARE(QContactLocalId(arg.at(0)), cid);

    // verify remove emits signal removed
    m1->removeContact(c.id().localId());
    remSigCount += 1;
    QTRY_COMPARE(spyCR.count(), remSigCount);
    QTRY_COMPARE(spyCOR1->count(), 1);
    args = spyCR.takeFirst();
    remSigCount -= 1;
    arg = args.first().value<QList<QString> >();
    QVERIFY(arg.count() == 1);
    QCOMPARE(QContactLocalId(arg.at(0)), cid);

    // verify multiple adds works as advertised
    QContact c2, c3;
    QContactName nc2, nc3;
    saveContactName(&c2, &nc2, "Mark");
    saveContactName(&c3, &nc3, "Garry");
#if defined(Q_OS_SYMBIAN)
    // TODO: symbiansim backend fails this test currently. Commented out for
    // now. Will be fixed later.
    if(!uri.contains("symbiansim")) {
        QVERIFY(!m1->saveContact(&c)); // saving contact with nonexistent id fails
    }
#endif
    QVERIFY(m1->saveContact(&c2));
    QVERIFY(m1->saveContact(&c3));
    if (uri.contains(QStringLiteral("tracker"))) {
        // tracker backend coalesces signals for performance reasons
        QTRY_COMPARE_SIGNALS_LOCALID_COUNT(spyCM, modSigCount);
        QTRY_COMPARE_SIGNALS_LOCALID_COUNT(spyCA, addSigCount);
    } else {
        QTRY_COMPARE(spyCM.count(), modSigCount);
        QTRY_VERIFY(spyCA.count() > addSigCount);
        addSigCount = spyCA.count();
    }

    spyCOM1->clear();
    spyCOR1->clear();
    QScopedPointer<QContactObserver> c2Observer(new QContactObserver(m1.data(), c2.localId()));
    QScopedPointer<QContactObserver> c3Observer(new QContactObserver(m1.data(), c3.localId()));
    QScopedPointer<QSignalSpy> spyCOM2(new QSignalSpy(c2Observer.data(), SIGNAL(contactChanged())));
    QScopedPointer<QSignalSpy> spyCOM3(new QSignalSpy(c3Observer.data(), SIGNAL(contactChanged())));
    QScopedPointer<QSignalSpy> spyCOR2(new QSignalSpy(c2Observer.data(), SIGNAL(contactRemoved())));
    QScopedPointer<QSignalSpy> spyCOR3(new QSignalSpy(c3Observer.data(), SIGNAL(contactRemoved())));

    // verify multiple modifies works as advertised
    saveContactName(&c2, &nc2, "M.");
    QVERIFY(m1->saveContact(&c2));
    if (uri.contains(QStringLiteral("tracker"))) {
        // tracker backend coalesces signals for performance reasons, so wait a little
         QTest::qWait(1000);
    }
    saveContactName(&c2, &nc2, "Mark");
    saveContactName(&c3, &nc3, "G.");
    QVERIFY(m1->saveContact(&c2));
    QVERIFY(m1->saveContact(&c3));
    if (uri.contains(QStringLiteral("tracker"))) {
        // tracker backend coalesces signals for performance reasons
        QTRY_COMPARE_SIGNALS_LOCALID_COUNT(spyCM, modSigCount);
    } else {
        QTRY_VERIFY(spyCM.count() > modSigCount);
        modSigCount = spyCM.count();
    }
    QTRY_VERIFY(spyCOM2->count() > 0);
    QTRY_VERIFY(spyCOM3->count() > 0);
    QCOMPARE(spyCOM1->count(), 0);

    // verify multiple removes works as advertised
    m1->removeContact(c3.id().localId());
    m1->removeContact(c2.id().localId());
    if (uri.contains(QStringLiteral("tracker"))) {
        // tracker backend coalesces signals for performance reasons
        QTRY_COMPARE_SIGNALS_LOCALID_COUNT(spyCR, remSigCount);
    } else {
        QTRY_VERIFY(spyCR.count() > remSigCount);
        remSigCount = spyCR.count();
    }
    QTRY_VERIFY(spyCOR2->count() > 0);
    QTRY_VERIFY(spyCOR3->count() > 0);
    QCOMPARE(spyCOR1->count(), 0);

    if (! uri.contains(QStringLiteral("tracker"))) {
        // The tracker backend does not support checking for existance of a contact.
        QVERIFY(!m1->removeContact(c.id().localId())); // not saved.
    }

    /* Now test the batch equivalents */
    spyCA.clear();
    spyCM.clear();
    spyCR.clear();

    /* Batch adds - set ids to zero so add succeeds. */
    c.setId(QContactId());
    c2.setId(QContactId());
    c3.setId(QContactId());
    batchAdd << c << c2 << c3;
    QMap<int, QContactManager::Error> errorMap;
    QVERIFY(m1->saveContacts(&batchAdd, &errorMap));

    QVERIFY(batchAdd.count() == 3);
    c = batchAdd.at(0);
    c2 = batchAdd.at(1);
    c3 = batchAdd.at(2);

    // We want to see one contactsAdded signal listing id's for all three contacts.
    QTRY_COMPARE(spyCA.count(), 1);
    {
        QList<QContactLocalId> sigids = spyCA.takeFirst().at(0).value<QList<QContactLocalId> >();
        QVERIFY(sigids.contains(c.localId()));
        QVERIFY(sigids.contains(c2.localId()));
        QVERIFY(sigids.contains(c3.localId()));
    }

    QTRY_COMPARE(spyCM.count(), 0);

    c1Observer.reset(new QContactObserver(m1.data(), c.localId()));
    c2Observer.reset(new QContactObserver(m1.data(), c2.localId()));
    c3Observer.reset(new QContactObserver(m1.data(), c3.localId()));
    spyCOM1.reset(new QSignalSpy(c1Observer.data(), SIGNAL(contactChanged())));
    spyCOM2.reset(new QSignalSpy(c2Observer.data(), SIGNAL(contactChanged())));
    spyCOM3.reset(new QSignalSpy(c3Observer.data(), SIGNAL(contactChanged())));
    spyCOR1.reset(new QSignalSpy(c1Observer.data(), SIGNAL(contactRemoved())));
    spyCOR2.reset(new QSignalSpy(c2Observer.data(), SIGNAL(contactRemoved())));
    spyCOR3.reset(new QSignalSpy(c3Observer.data(), SIGNAL(contactRemoved())));
    QTRY_COMPARE(spyCR.count(), 0);

    /* Batch modifies */
    QContactName modifiedName = c.detail(QContactName::DefinitionName);
    saveContactName(&c, &modifiedName, "Modified number 1");
    modifiedName = c2.detail(QContactName::DefinitionName);
    saveContactName(&c2, &modifiedName, "Modified number 2");
    modifiedName = c3.detail(QContactName::DefinitionName);
    saveContactName(&c3, &modifiedName, "Modified number 3");

    batchAdd.clear();
    batchAdd << c << c2 << c3;
    QVERIFY(m1->saveContacts(&batchAdd, &errorMap));

    // We want to see one contactsChanged signal listing id's for all three contacts.
    QTRY_COMPARE(spyCM.count(), 1);
    {
        QList<QContactLocalId> sigids = spyCM.takeFirst().at(0).value<QList<QContactLocalId> >();
        QVERIFY(sigids.contains(c.localId()));
        QVERIFY(sigids.contains(c2.localId()));
        QVERIFY(sigids.contains(c3.localId()));
    }
    QTRY_COMPARE(spyCOM1->count(), 1);
    QTRY_COMPARE(spyCOM2->count(), 1);
    QTRY_COMPARE(spyCOM3->count(), 1);

    /* Batch removes */
    batchRemove << c.id().localId() << c2.id().localId() << c3.id().localId();
    QVERIFY(m1->removeContacts(batchRemove, &errorMap));

    // We want to see one contactsRemoved signal listing id's for all three contacts.
    QTRY_COMPARE(spyCR.count(), 1);
    {
        QList<QContactLocalId> sigids = spyCR.takeFirst().at(0).value<QList<QContactLocalId> >();
        QVERIFY(sigids.contains(c.localId()));
        QVERIFY(sigids.contains(c2.localId()));
        QVERIFY(sigids.contains(c3.localId()));
    }
    QTRY_COMPARE(spyCOR1->count(), 1);
    QTRY_COMPARE(spyCOR2->count(), 1);
    QTRY_COMPARE(spyCOR3->count(), 1);

    QTRY_COMPARE(spyCA.count(), 0);
    QTRY_COMPARE(spyCM.count(), 0);

    QScopedPointer<QContactManager> m2(QContactManager::fromUri(uri));
    
    // During construction SIM backend (m2) will try writing contacts with 
    // nickname, email and additional number to find out if the SIM card
    // will support these fields. The other backend (m1) will then receive
    // signals about that. These need to be caught so they don't interfere
    // with the tests. (This trial and error method is used because existing
    // API for checking the availability of these fields is not public.)
	// NOTE: This applies only to pre 10.1 platforms (S60 3.1, 3.2, ect.)
    if (uri.contains("symbiansim")) {
        QTest::qWait(0);
        spyCA.clear();
        spyCM.clear();
        spyCR.clear();
    }

    QVERIFY(m1->hasFeature(QContactManager::Anonymous) ==
        m2->hasFeature(QContactManager::Anonymous));

    /* Now some cross manager testing */
    if (!m1->hasFeature(QContactManager::Anonymous)) {
        // verify that signals are emitted for modifications made to other managers (same id).
        QContactName ncs = c.detail(QContactName::DefinitionName);
        saveContactName(&c, &ncs, "Test");
        c.setId(QContactId()); // reset id so save can succeed.
        QVERIFY(m2->saveContact(&c));
        saveContactName(&c, &ncs, "Test2");
        QVERIFY(m2->saveContact(&c));
        QTRY_COMPARE(spyCA.count(), 1); // check that we received the update signals.
        QTRY_COMPARE(spyCM.count(), 1); // check that we received the update signals.
        m2->removeContact(c.localId());
        QTRY_COMPARE(spyCR.count(), 1); // check that we received the remove signal.
    }
}

void tst_QContactManager::errorStayingPut()
{
    /* Make sure that when we clone a manager, we don't clone the error */
    QMap<QString, QString> params;
    params.insert("id", "error isolation test");
    QContactManager m1("memory",params);

    QVERIFY(m1.error() == QContactManager::NoError);

    /* Remove an invalid contact to get an error */
    QVERIFY(m1.removeContact(0) == false);
    QVERIFY(m1.error() == QContactManager::DoesNotExistError);

    /* Create a new manager with hopefully the same backend */
    QContactManager m2("memory", params);

    QVERIFY(m1.error() == QContactManager::DoesNotExistError);
    QVERIFY(m2.error() == QContactManager::NoError);

    /* Cause an error on the other ones and check the first is not affected */
    m2.saveContacts(0, 0);
    QVERIFY(m1.error() == QContactManager::DoesNotExistError);
    QVERIFY(m2.error() == QContactManager::BadArgumentError);

    QContact c;
    QContactDetail d("This does not exist and if it does this will break");
    d.setValue("Value that also doesn't exist", 5);
    c.saveDetail(&d);

    QVERIFY(m1.saveContact(&c) == false);
    QVERIFY(m1.error() == QContactManager::InvalidDetailError);
    QVERIFY(m2.error() == QContactManager::BadArgumentError);
}


void tst_QContactManager::displayName()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    /*
     * Very similar to the tst_QContact functions, except we test
     * saving and retrieving contacts updates the display label
     */

    /* Try to make this a bit more consistent by using a single name */
    QContact d;
    QContactName name;
    saveContactName(&d, &name, "Wesley");

    QVERIFY(d.displayLabel().isEmpty());

    QString synth = cm->synthesizedContactDisplayLabel(d);

    // Make sure this doesn't crash
    cm->synthesizeContactDisplayLabel(0);

    // Make sure this gives the same results
    cm->synthesizeContactDisplayLabel(&d);
    QCOMPARE(d.displayLabel(), synth);

    /*
     * The display label is not updated until you save the contact or call synthCDL
     */
    QVERIFY(cm->saveContact(&d));
    d = cm->contact(d.id().localId());
    QVERIFY(!d.isEmpty());
    QCOMPARE(d.displayLabel(), synth);

    /* Remove the detail via removeDetail */
    QContactDisplayLabel old;
    int count = d.details().count();
    QVERIFY(!d.removeDetail(&old)); // should fail.
    QVERIFY(d.isEmpty() == false);
    QVERIFY(d.details().count() == count); // it should not be removed!

    /* Save the contact again */
    QVERIFY(cm->saveContact(&d));
    d = cm->contact(d.id().localId());
    QVERIFY(!d.isEmpty());

    /* Make sure the label is still the synth version */
    QCOMPARE(d.displayLabel(), synth);

    /* And delete the contact */
    QVERIFY(cm->removeContact(d.id().localId()));
}

void tst_QContactManager::actionPreferences()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    // early out if the manager doesn't support action preference saving.
    if (!cm->hasFeature(QContactManager::ActionPreferences)) {
        QSKIP("Manager does not support action preferences");
    }

    // create a sample contact
    QContactAvatar a;
    a.setImageUrl(QUrl("test.png"));
    QContactPhoneNumber p1;
    p1.setNumber("12345");
    QContactPhoneNumber p2;
    p2.setNumber("34567");
    QContactPhoneNumber p3;
    p3.setNumber("56789");
    QContactUrl u;
    u.setUrl("http://test.nokia.com");
    QContactName n;
    QContact c;
    saveContactName(&c, &n, "TestContact");
    c.saveDetail(&a);
    c.saveDetail(&p1);
    c.saveDetail(&p2);
    c.saveDetail(&p3);
    c.saveDetail(&u);

    // set a preference for dialing a particular saved phonenumber.
    c.setPreferredDetail("Dial", p2);

    QVERIFY(cm->saveContact(&c));          // save the contact
    QContact loaded = cm->contact(c.id().localId()); // reload the contact

    // test that the preference was saved correctly.
    QContactDetail pref = loaded.preferredDetail("Dial");
    QVERIFY(pref == p2);

    cm->removeContact(c.id().localId());
}

void tst_QContactManager::changeSet()
{
    QContactLocalId id(1);

    QContactChangeSet changeSet;
    QVERIFY(changeSet.addedContacts().isEmpty());
    QVERIFY(changeSet.changedContacts().isEmpty());
    QVERIFY(changeSet.removedContacts().isEmpty());

    changeSet.insertAddedContact(id);
    QVERIFY(!changeSet.addedContacts().isEmpty());
    QVERIFY(changeSet.changedContacts().isEmpty());
    QVERIFY(changeSet.removedContacts().isEmpty());
    QVERIFY(changeSet.addedContacts().contains(id));

    changeSet.insertChangedContact(id);
    changeSet.insertChangedContacts(QList<QContactLocalId>() << id);
    QVERIFY(changeSet.changedContacts().size() == 1); // set, should only be added once.
    QVERIFY(!changeSet.addedContacts().isEmpty());
    QVERIFY(!changeSet.changedContacts().isEmpty());
    QVERIFY(changeSet.removedContacts().isEmpty());
    QVERIFY(changeSet.changedContacts().contains(id));
    changeSet.clearChangedContacts();
    QVERIFY(changeSet.changedContacts().isEmpty());

    changeSet.insertRemovedContacts(QList<QContactLocalId>() << id);
    QVERIFY(changeSet.removedContacts().contains(id));
    changeSet.clearRemovedContacts();
    QVERIFY(changeSet.removedContacts().isEmpty());

    QVERIFY(changeSet.dataChanged() == false);
    QContactChangeSet changeSet2;
    changeSet2 = changeSet;
    QVERIFY(changeSet.addedContacts() == changeSet2.addedContacts());
    changeSet.emitSignals(0);

    changeSet2.clearAddedContacts();
    QVERIFY(changeSet2.addedContacts().isEmpty());
    changeSet2.insertAddedContacts(changeSet.addedContacts().toList());
    QVERIFY(changeSet.addedContacts() == changeSet2.addedContacts());

    changeSet2.clearAll();
    QVERIFY(changeSet.addedContacts() != changeSet2.addedContacts());

    QContactChangeSet changeSet3(changeSet2);
    QVERIFY(changeSet.addedContacts() != changeSet3.addedContacts());
    QVERIFY(changeSet2.addedContacts() == changeSet3.addedContacts());

    changeSet.setDataChanged(true);
    QVERIFY(changeSet.dataChanged() == true);
    QVERIFY(changeSet.dataChanged() != changeSet2.dataChanged());
    QVERIFY(changeSet.dataChanged() != changeSet3.dataChanged());
    changeSet.emitSignals(0);

    changeSet.addedRelationshipsContacts().insert(id);
    changeSet.insertAddedRelationshipsContacts(QList<QContactLocalId>() << id);
    QVERIFY(changeSet.addedRelationshipsContacts().contains(id));
    changeSet.clearAddedRelationshipsContacts();
    QVERIFY(changeSet.addedRelationshipsContacts().isEmpty());
    changeSet.insertRemovedRelationshipsContacts(QList<QContactLocalId>() << id);
    QVERIFY(changeSet.removedRelationshipsContacts().contains(id));
    changeSet.clearRemovedRelationshipsContacts();
    QVERIFY(changeSet.removedRelationshipsContacts().isEmpty());
    changeSet.emitSignals(0);
    changeSet.removedRelationshipsContacts().insert(id);
    changeSet.emitSignals(0);

    changeSet.setOldAndNewSelfContactId(QPair<QContactLocalId, QContactLocalId>(QContactLocalId("0"), id));
    changeSet2 = changeSet;
    QVERIFY(changeSet2.addedRelationshipsContacts() == changeSet.addedRelationshipsContacts());
    QVERIFY(changeSet2.removedRelationshipsContacts() == changeSet.removedRelationshipsContacts());
    QVERIFY(changeSet2.oldAndNewSelfContactId() == changeSet.oldAndNewSelfContactId());
    changeSet.emitSignals(0);
    changeSet.setOldAndNewSelfContactId(QPair<QContactLocalId, QContactLocalId>(id, QContactLocalId("0")));
    QVERIFY(changeSet2.oldAndNewSelfContactId() != changeSet.oldAndNewSelfContactId());
    changeSet.setDataChanged(true);
    changeSet.emitSignals(0);
}

void tst_QContactManager::fetchHint()
{
    // This just tests the accessors and mutators (API).
    // See tst_qcontactmanagerfiltering for the "backend support" test.
    QContactFetchHint hint;
    hint.setOptimizationHints(QContactFetchHint::NoBinaryBlobs);
    QCOMPARE(hint.optimizationHints(), QContactFetchHint::NoBinaryBlobs);
    QStringList rels;
    rels << QString(QStringLiteral(QContactRelationship::HasMember));
    hint.setRelationshipTypesHint(rels);
    QCOMPARE(hint.relationshipTypesHint(), rels);
    QStringList defs;
    defs << QString(QContactName::DefinitionName)
         << QString(QContactPhoneNumber::DefinitionName);
    hint.setDetailDefinitionsHint(defs);
    QCOMPARE(hint.detailDefinitionsHint(), defs);
    QSize prefImageSize(33, 33);
    hint.setPreferredImageSize(prefImageSize);
    QCOMPARE(hint.preferredImageSize(), prefImageSize);
    int limit = 15;
    hint.setMaxCountHint(limit);
    QCOMPARE(hint.maxCountHint(), limit);
}

void tst_QContactManager::selfContactId()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    // early out if the manager doesn't support self contact id saving
    QContactLocalId selfContact = cm->selfContactId();
    if (!cm->hasFeature(QContactManager::SelfContact)) {
        if (cm->managerName() == "jsondb")
        {
            QVERIFY(cm->error() == QContactManager::NotSupportedError);
            QSKIP("JSONDB backend does not support selfContact at the moment, skipping...");
        }
        else
        {
            // ensure that the error codes / return values are meaningful failures.
            QEXPECT_FAIL("mgr='maemo5'", "maemo5 supports getting the self contact but not setting it.", Continue);
            QVERIFY(cm->error() == QContactManager::DoesNotExistError);
            QVERIFY(!cm->setSelfContactId(QContactLocalId(123)));
            QVERIFY(cm->error() == QContactManager::NotSupportedError);
            QSKIP("Manager does not support the concept of a self-contact");
        }
    }

    // create a new "self" contact and retrieve its Id
    QVERIFY(cm->error() == QContactManager::NoError || cm->error() == QContactManager::DoesNotExistError);
    QContact self;
    QContactPhoneNumber selfPhn;
    selfPhn.setNumber("12345");
    self.saveDetail(&selfPhn);
    if (!cm->saveContact(&self)) {
        QSKIP("Unable to save the generated self contact");
    }
    QContactLocalId newSelfContact = self.localId();

    // Setup signal spy
    qRegisterMetaType<QContactLocalId>("QContactLocalId");
    QSignalSpy spy(cm.data(), SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId)));
    QTestSignalSink sink(cm.data(), SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId)));

    // Set new self contact
    QVERIFY(cm->setSelfContactId(newSelfContact));
    QVERIFY(cm->error() == QContactManager::NoError);
    QTRY_VERIFY(spy.count() == 1);
    QVERIFY(spy.at(0).count() == 2);
    // note: for some reason qvariant_cast<QContactLocalId>(spy.at(0).at(0)) returns always zero
    // because the type is not recognized. Hence the ugly casting below.
    QVERIFY(*((const QContactLocalId*) spy.at(0).at(0).constData()) == selfContact);
    QVERIFY(*((const QContactLocalId*) spy.at(0).at(1).constData()) == newSelfContact);
    QVERIFY(cm->selfContactId() == newSelfContact);

    // Remove self contact
    if(!cm->removeContact(self.localId())) {
        QSKIP("Unable to remove self contact");
    }
    QTRY_VERIFY(spy.count() == 2);
    QVERIFY(spy.at(1).count() == 2);
    QVERIFY(*((const QContactLocalId*) spy.at(1).at(0).constData()) == newSelfContact);
    QVERIFY(*((const QContactLocalId*) spy.at(1).at(1).constData()) == QContactLocalId("0"));
    QVERIFY(cm->selfContactId() == QContactLocalId("0")); // ensure reset after removed.

    // reset to original state.
    cm->setSelfContactId(selfContact);
}

QList<QContactDetail> tst_QContactManager::removeAllDefaultDetails(const QList<QContactDetail>& details)
{
    QList<QContactDetail> newlist;
    foreach (const QContactDetail d, details) {
        if (d.definitionName() != QContactDisplayLabel::DefinitionName
                && d.definitionName() != QContactType::DefinitionName
                && d.definitionName() != QContactTimestamp::DefinitionName) {
            newlist << d;
        }
    }
    return newlist;
}

void tst_QContactManager::detailOrders()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    if (!cm->hasFeature(QContactManager::DetailOrdering))
        QSKIP("Skipping: This manager does not support detail ordering!");

    QContact a;
    //phone numbers

    QString contextOther = QContactDetail::ContextOther;
    QContactPhoneNumber number1, number2, number3;
    
    number1.setNumber("11111111");
    number1.setContexts(QContactPhoneNumber::ContextHome);

    number2.setNumber("22222222");
    number2.setContexts(QContactPhoneNumber::ContextWork);

    number3.setNumber("33333333");
    if (!contextOther.isEmpty())
        number3.setContexts(contextOther);

    a.saveDetail(&number1);
    a.saveDetail(&number2);
    a.saveDetail(&number3);

    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    
    QList<QContactDetail> details = a.details(QContactPhoneNumber::DefinitionName);
    QVERIFY(details.count() == 3);
    QVERIFY(details.at(0).value(QContactPhoneNumber::FieldContext) == QContactPhoneNumber::ContextHome);
    QVERIFY(details.at(1).value(QContactPhoneNumber::FieldContext) == QContactPhoneNumber::ContextWork);
    QVERIFY(details.at(2).value(QContactPhoneNumber::FieldContext) == contextOther);
    
    QVERIFY(a.removeDetail(&number2));
    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    details = a.details(QContactPhoneNumber::DefinitionName);
    QVERIFY(details.count() == 2);
    QVERIFY(details.at(0).value(QContactPhoneNumber::FieldContext) == QContactPhoneNumber::ContextHome);
    QVERIFY(details.at(1).value(QContactPhoneNumber::FieldContext) == contextOther);

    a.saveDetail(&number2);
    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    
    details = a.details(QContactPhoneNumber::DefinitionName);
    QVERIFY(details.count() == 3);
    QVERIFY(details.at(0).value(QContactPhoneNumber::FieldContext) == QContactPhoneNumber::ContextHome);
    QVERIFY(details.at(1).value(QContactPhoneNumber::FieldContext) == contextOther);
    QVERIFY(details.at(2).value(QContactPhoneNumber::FieldContext) == QContactPhoneNumber::ContextWork);

    //addresses
    
    contextOther = QContactDetail::ContextOther;
    
    QContactAddress address1, address2, address3;
    
    address1.setStreet("Brandl St");
    address1.setRegion("Brisbane");
    address3 = address2 = address1;

    address1.setContexts(QContactAddress::ContextHome);
    address2.setContexts(QContactAddress::ContextWork);
    if (!contextOther.isEmpty())
        address3.setContexts(contextOther);

    a.saveDetail(&address1);
    a.saveDetail(&address2);
    a.saveDetail(&address3);

    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    
    details = a.details(QContactAddress::DefinitionName);
    QVERIFY(details.count() == 3);
    
    QVERIFY(details.at(0).value(QContactAddress::FieldContext) == QContactAddress::ContextHome);
    QVERIFY(details.at(1).value(QContactAddress::FieldContext) == QContactAddress::ContextWork);
    QVERIFY(details.at(2).value(QContactAddress::FieldContext) == contextOther);

    QVERIFY(a.removeDetail(&address2));
    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    details = a.details(QContactAddress::DefinitionName);
    QVERIFY(details.count() == 2);
    QVERIFY(details.at(0).value(QContactAddress::FieldContext) == QContactAddress::ContextHome);
    QVERIFY(details.at(1).value(QContactAddress::FieldContext) == contextOther);

    a.saveDetail(&address2);
    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    
    details = a.details(QContactAddress::DefinitionName);
    QVERIFY(details.count() == 3);
    QVERIFY(details.at(0).value(QContactAddress::FieldContext) == QContactAddress::ContextHome);
    QVERIFY(details.at(1).value(QContactAddress::FieldContext) == contextOther);
    QVERIFY(details.at(2).value(QContactAddress::FieldContext) == QContactAddress::ContextWork);


    //emails
    contextOther = QContactDetail::ContextOther;

    QContactEmailAddress email1, email2, email3;

    email1.setEmailAddress("aaron@example.com");
    email3 = email2 = email1;
    email1.setContexts(QContactEmailAddress::ContextHome);
    email2.setContexts(QContactEmailAddress::ContextWork);
    if (!contextOther.isEmpty())
        email3.setContexts(contextOther);

    a.saveDetail(&email1);
    a.saveDetail(&email2);
    a.saveDetail(&email3);

    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    
    details = a.details(QContactEmailAddress::DefinitionName);
    QVERIFY(details.count() == 3);
    
    QVERIFY(details.at(0).value(QContactEmailAddress::FieldContext) == QContactEmailAddress::ContextHome);
    QVERIFY(details.at(1).value(QContactEmailAddress::FieldContext) == QContactEmailAddress::ContextWork);
    QVERIFY(details.at(2).value(QContactEmailAddress::FieldContext) == contextOther);

    QVERIFY(a.removeDetail(&email2));
    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    details = a.details(QContactEmailAddress::DefinitionName);
    QVERIFY(details.count() == 2);
    QVERIFY(details.at(0).value(QContactEmailAddress::FieldContext) == QContactEmailAddress::ContextHome);
    QVERIFY(details.at(1).value(QContactEmailAddress::FieldContext) == contextOther);

    a.saveDetail(&email2);
    QVERIFY(cm->saveContact(&a));
    a = cm->contact(a.id().localId());
    
    details = a.details(QContactEmailAddress::DefinitionName);
    QVERIFY(details.count() == 3);
    QVERIFY(details.at(0).value(QContactEmailAddress::FieldContext) == QContactEmailAddress::ContextHome);
    QVERIFY(details.at(1).value(QContactEmailAddress::FieldContext) == contextOther);
    QVERIFY(details.at(2).value(QContactEmailAddress::FieldContext) == QContactEmailAddress::ContextWork);

    QVERIFY(cm->removeContact(a.id().localId()));
    QVERIFY(cm->error() == QContactManager::NoError);
}

void tst_QContactManager::relationships()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    // save some contacts
    QContact source;
    QContact dest1, dest2, dest3, dest4;
    QContactPhoneNumber n1, n2, n3, n4;
    n1.setNumber("1");
    n2.setNumber("2");
    n3.setNumber("3");
    n4.setNumber("4");

    dest1.saveDetail(&n1);
    dest2.saveDetail(&n2);
    dest3.saveDetail(&n3);
    dest4.saveDetail(&n4);

    cm->saveContact(&source);
    cm->saveContact(&dest1);
    cm->saveContact(&dest2);
    cm->saveContact(&dest3);
    cm->saveContact(&dest4);

    // check if manager supports relationships
    if (!cm->hasFeature(QContactManager::Relationships)) {
        // ensure that the operations all fail as required.
        QContactRelationship r1, r2, r3;
        r1.setFirst(source);
        r1.setSecond(dest1);
        r1.setRelationshipType(QContactRelationship::HasManager);
        r2.setFirst(source);
        r2.setSecond(dest2);
        r2.setRelationshipType(QContactRelationship::HasManager);
        r3.setFirst(source);
        r3.setSecond(dest3);
        r3.setRelationshipType(QContactRelationship::HasManager);

        QList<QContactRelationship> batchList;
        batchList << r2 << r3;

        // test save and remove
        QVERIFY(!cm->saveRelationship(&r1));
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        QVERIFY(!cm->removeRelationship(r1));
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        cm->saveRelationships(&batchList, NULL);
        QVERIFY(cm->error() == QContactManager::NotSupportedError);

        // test retrieval
        QList<QContactRelationship> retrieveList;
        retrieveList = cm->relationships(source, QContactRelationship::First);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        retrieveList = cm->relationships(source, QContactRelationship::Second);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        retrieveList = cm->relationships(source, QContactRelationship::Either); // Either
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);


        retrieveList = cm->relationships(QContactRelationship::HasManager, source, QContactRelationship::First);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        retrieveList = cm->relationships(QContactRelationship::HasManager, source, QContactRelationship::Second);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        retrieveList = cm->relationships(QContactRelationship::HasManager, source, QContactRelationship::Either);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        retrieveList = cm->relationships(QContactRelationship::HasManager, source);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        retrieveList = cm->relationships(QContactRelationship::HasManager);
        QVERIFY(retrieveList.isEmpty());
        QVERIFY(cm->error() == QContactManager::NotSupportedError);
        return;
    }

    // Get supported relationship types
    QStringList availableRelationshipTypes;
    if (cm->isRelationshipTypeSupported(QContactRelationship::HasMember))
        availableRelationshipTypes << QContactRelationship::HasMember;    
    if (cm->isRelationshipTypeSupported(QContactRelationship::HasAssistant))
        availableRelationshipTypes << QContactRelationship::HasAssistant;
    if (cm->isRelationshipTypeSupported(QContactRelationship::HasManager))
        availableRelationshipTypes << QContactRelationship::HasManager;
    if (cm->isRelationshipTypeSupported(QContactRelationship::HasSpouse))
        availableRelationshipTypes << QContactRelationship::HasSpouse;
    if (cm->isRelationshipTypeSupported(QContactRelationship::IsSameAs))
        availableRelationshipTypes << QContactRelationship::IsSameAs;

    // Check arbitrary relationship support
    if (cm->hasFeature(QContactManager::ArbitraryRelationshipTypes)) {
        // add some arbitrary type for testing
        if (availableRelationshipTypes.count())
            availableRelationshipTypes.insert(0, "test-arbitrary-relationship-type");
        else {
            availableRelationshipTypes.append("test-arbitrary-relationship-type");
            availableRelationshipTypes.append(QContactRelationship::HasMember);
            availableRelationshipTypes.append(QContactRelationship::HasAssistant);
        }
    }

    // Verify that we have relationship types. If there are none then the manager
    // is saying it supports relationships but does not actually implement any 
    // relationship type.
    QVERIFY(!availableRelationshipTypes.isEmpty());

    // Some backends (eg. symbian) require that when type is "HasMember" 
    // then "first" contact must be a group.
    if (availableRelationshipTypes.at(0) == QContactRelationship::HasMember) {
        cm->removeContact(source.localId());
        source.setId(QContactId());
        source.setType(QContactType::TypeGroup);
        cm->saveContact(&source);
    }

    // build our relationship - source is the manager all of the dest contacts.
    QContactRelationship customRelationshipOne;
    customRelationshipOne.setFirst(source);
    customRelationshipOne.setSecond(dest1);
    customRelationshipOne.setRelationshipType(availableRelationshipTypes.at(0));
    QCOMPARE(customRelationshipOne.first(), source);
    QCOMPARE(customRelationshipOne.second(), dest1);
    QVERIFY(customRelationshipOne.relationshipType() == availableRelationshipTypes.at(0));

    // save the relationship
    int managerRelationshipsCount = cm->relationships(availableRelationshipTypes.at(0)).count();
    QVERIFY(cm->saveRelationship(&customRelationshipOne));

    // test our accessors.
    QCOMPARE(cm->relationships(availableRelationshipTypes.at(0)).count(), (managerRelationshipsCount + 1));
    QVERIFY(cm->relationships(availableRelationshipTypes.at(0), source).count() == 1);

    // remove the dest1 contact, relationship should be removed.
    cm->removeContact(dest1.localId());
    QCOMPARE(cm->relationships(availableRelationshipTypes.at(0), dest1, QContactRelationship::Second).count(), 0);

    // modify and save the relationship
    customRelationshipOne.setSecond(dest2);
    QVERIFY(cm->saveRelationship(&customRelationshipOne));

    // attempt to save the relationship again.  XXX TODO: what should the result be?  currently succeeds (overwrites)
    int relationshipsCount = cm->relationships().count();
    QVERIFY(cm->saveRelationship(&customRelationshipOne));    // succeeds, but just overwrites
    QCOMPARE(relationshipsCount, cm->relationships().count()); // shouldn't change; save should have overwritten.

    // removing the source contact should result in removal of the relationship.
    QVERIFY(cm->removeContact(source.localId()));
    QCOMPARE(cm->relationships().count(), relationshipsCount - 1); // the relationship should have been removed.

    // now ensure that qcontact relationship caching works as required - perhaps this should be in tst_QContact?
    source.setId(QContactId());         // reset id so we can resave
    QVERIFY(cm->saveContact(&source));  // save source again.
    customRelationshipOne.setFirst(source);
    customRelationshipOne.setSecond(dest2);
    QVERIFY(cm->saveRelationship(&customRelationshipOne));

    // Add a second relationship
    QContactRelationship customRelationshipTwo;
    customRelationshipTwo.setFirst(source);
    if (availableRelationshipTypes.count() > 1)
        customRelationshipTwo.setRelationshipType(availableRelationshipTypes.at(1));
    else
        customRelationshipTwo.setRelationshipType(availableRelationshipTypes.at(0));
    customRelationshipTwo.setSecond(dest3);
    QVERIFY(cm->saveRelationship(&customRelationshipTwo));

    // currently, the contacts are "stale" - no cached relationships
    QVERIFY(dest3.relatedContacts().isEmpty());
    QVERIFY(dest3.relationships().isEmpty());
    QVERIFY(dest2.relatedContacts().isEmpty());
    QVERIFY(dest2.relationships().isEmpty());

    // now refresh the contacts
    dest3 = cm->contact(dest3.id().localId());
    dest2 = cm->contact(dest2.id().localId());
    source = cm->contact(source.id().localId());

    // and test again.
    QVERIFY(source.relatedContacts(QString(), QContactRelationship::First).isEmpty()); // source is always the first, so this should be empty.
    QVERIFY(source.relatedContacts(QString(), QContactRelationship::Second).contains(dest2));
    QVERIFY(source.relatedContacts(QString(), QContactRelationship::Either).contains(dest2));
    QVERIFY(source.relatedContacts(QString(), QContactRelationship::Second).contains(dest3));
    QVERIFY(source.relatedContacts(QString(), QContactRelationship::Either).contains(dest3));
    QVERIFY(source.relatedContacts(availableRelationshipTypes.at(0), QContactRelationship::Second).contains(dest2));
    QVERIFY(source.relatedContacts(availableRelationshipTypes.at(0), QContactRelationship::First).isEmpty());

    QVERIFY(dest2.relatedContacts().contains(source));
    QVERIFY(dest2.relationships().contains(customRelationshipOne));
    QVERIFY(!dest2.relationships().contains(customRelationshipTwo));
    QVERIFY(dest2.relationships(availableRelationshipTypes.at(0)).contains(customRelationshipOne));
    QVERIFY(!dest2.relationships(availableRelationshipTypes.at(0)).contains(customRelationshipTwo));
    QVERIFY(dest2.relatedContacts(availableRelationshipTypes.at(0)).contains(source));
    QVERIFY(dest2.relatedContacts(availableRelationshipTypes.at(0), QContactRelationship::First).contains(source));
    QVERIFY(dest2.relatedContacts(availableRelationshipTypes.at(0), QContactRelationship::Second).isEmpty());
    QVERIFY(!dest2.relatedContacts(availableRelationshipTypes.at(0), QContactRelationship::Second).contains(source));

    QVERIFY(dest3.relatedContacts().contains(source));
    QVERIFY(!dest3.relationships().contains(customRelationshipOne));
    QVERIFY(dest3.relationships().contains(customRelationshipTwo));
    QVERIFY(!dest3.relationships(availableRelationshipTypes.at(0)).contains(customRelationshipOne));

    // Test iteration
    QList<QContactRelationship> relats = source.relationships();
    QList<QContactRelationship>::iterator it = relats.begin();

    while (it != relats.end()) {
        QContact firstContact = it->first();
        QVERIFY(firstContact == source);
        QVERIFY(it->second() == dest2 || it->second() == dest3);
        it++;
    }

    if (availableRelationshipTypes.count() > 1) {
        QVERIFY(source.relatedContacts(availableRelationshipTypes.at(1), QContactRelationship::Second).contains(dest3));
        QVERIFY(source.relatedContacts(availableRelationshipTypes.at(1), QContactRelationship::First).isEmpty());
        
        QVERIFY(dest2.relationships(availableRelationshipTypes.at(1)).isEmpty());
        
        QVERIFY(!dest3.relationships(availableRelationshipTypes.at(0)).contains(customRelationshipTwo));
        QVERIFY(dest3.relationships(availableRelationshipTypes.at(1)).contains(customRelationshipTwo));
        QVERIFY(!dest3.relationships(availableRelationshipTypes.at(1)).contains(customRelationshipOne));
        QVERIFY(dest3.relatedContacts(availableRelationshipTypes.at(1)).contains(source));
        QVERIFY(!dest3.relatedContacts(availableRelationshipTypes.at(0)).contains(source));
        QVERIFY(dest3.relatedContacts(availableRelationshipTypes.at(1)).contains(source)); // role = either
        QVERIFY(!dest3.relatedContacts(availableRelationshipTypes.at(1), QContactRelationship::Second).contains(source));
        QVERIFY(dest3.relatedContacts(availableRelationshipTypes.at(1), QContactRelationship::First).contains(source));
        QVERIFY(dest2.relatedContacts(availableRelationshipTypes.at(1)).isEmpty());
    } else {
        QVERIFY(source.relatedContacts(availableRelationshipTypes.at(0), QContactRelationship::Second).contains(dest3));
    }

    // Cleanup a bit
    QMap<int, QContactManager::Error> errorMap;
    QList<QContactRelationship> moreRels;

    moreRels << customRelationshipOne << customRelationshipTwo;
    errorMap.insert(5, QContactManager::BadArgumentError);
    QVERIFY(cm->removeRelationships(moreRels, &errorMap));
    QVERIFY(errorMap.count() == 0);

    // test batch API and ordering in contacts
    QList<QContactRelationship> currentRelationships = cm->relationships(source, QContactRelationship::First);
    QList<QContactRelationship> batchList;
    QContactRelationship br1, br2, br3;
    br1.setFirst(source);
    br1.setSecond(dest2);
    br1.setRelationshipType(availableRelationshipTypes.at(0));    
    br2.setFirst(source);
    br2.setSecond(dest3);
    br2.setRelationshipType(availableRelationshipTypes.at(0));
    if (availableRelationshipTypes.count() > 1) {
        br3.setFirst(source);
        br3.setSecond(dest3);
        br3.setRelationshipType(availableRelationshipTypes.at(1));
    } else {
        br3.setFirst(source);
        br3.setSecond(dest4);
        br3.setRelationshipType(availableRelationshipTypes.at(0));
    }
    batchList << br1 << br2 << br3;

    // ensure that the batch save works properly
    cm->saveRelationships(&batchList, NULL);
    QCOMPARE(cm->error(), QContactManager::NoError);
    QList<QContactRelationship> batchRetrieve = cm->relationships(source, QContactRelationship::First);
    QVERIFY(batchRetrieve.contains(br1));
    QVERIFY(batchRetrieve.contains(br2));
    QVERIFY(batchRetrieve.contains(br3));
    
    // remove a single relationship
    QVERIFY(cm->removeRelationship(br3));
    batchRetrieve = cm->relationships(source, QContactRelationship::First);
    QVERIFY(batchRetrieve.contains(br1));
    QVERIFY(batchRetrieve.contains(br2));
    QVERIFY(!batchRetrieve.contains(br3)); // has already been removed.

    // now ensure that the batch remove works and we get returned to the original state.
    batchList.removeOne(br3);
    cm->removeRelationships(batchList, NULL);
    QVERIFY(cm->error() == QContactManager::NoError);
    QCOMPARE(cm->relationships(source, QContactRelationship::First), currentRelationships);

    // attempt to save relationships between an existing source but non-existent destination
    QContact nonexistentDest;
    quint32 idSeed = 0x5544;
    QContactLocalId nonexistentLocalId = QContactLocalId(QString::number(idSeed));
    nonexistentDest.id().setManagerUri(cm->managerUri());qDebug()<<__FILE__<<__LINE__;
    while (true) {
        nonexistentLocalId = cm->contact(nonexistentLocalId).localId();
        if (nonexistentLocalId == QContactLocalId("")) {
            // found a "spare" local id (no contact with that id)
            break;
        }

        // keep looking...
        idSeed += 1;
        nonexistentLocalId = QContactLocalId(QString::number(idSeed));
        QVERIFY(nonexistentLocalId != QContactLocalId("0")); // integer overflow check.
    }
    nonexistentDest.id().setLocalId(nonexistentLocalId);
    QContactRelationship maliciousRel;
    maliciousRel.setFirst(source);
    maliciousRel.setSecond(nonexistentDest);
    maliciousRel.setRelationshipType("nokia-test-invalid-relationship-type");
    QVERIFY(!cm->saveRelationship(&maliciousRel));

    // attempt to save a circular relationship - should fail!
    maliciousRel.setFirst(source);
    maliciousRel.setSecond(source);
    maliciousRel.setRelationshipType(availableRelationshipTypes.at(0));
    QVERIFY(!cm->saveRelationship(&maliciousRel));

    // more negative testing, but force manager to recognise the empty URI
    QContact circularContact = source;
    maliciousRel.setFirst(circularContact);
    maliciousRel.setSecond(circularContact);
    maliciousRel.setRelationshipType(availableRelationshipTypes.at(0));
    QVERIFY(!cm->saveRelationship(&maliciousRel));
    maliciousRel.setFirst(source);
    maliciousRel.setSecond(circularContact);
    maliciousRel.setRelationshipType(availableRelationshipTypes.at(0));
    QVERIFY(!cm->saveRelationship(&maliciousRel));
    maliciousRel.setFirst(circularContact);
    maliciousRel.setSecond(source);
    maliciousRel.setRelationshipType(availableRelationshipTypes.at(0));
    QVERIFY(!cm->saveRelationship(&maliciousRel));

    // attempt to save a relationship where the source contact comes from another manager
    QContactId invalidId;
    invalidId.setManagerUri("test-nokia-invalid-manager-uri");
    invalidId.setLocalId(circularContact.id().localId());
    circularContact.setId(invalidId);
    maliciousRel.setFirst(circularContact);   // an invalid source contact
    maliciousRel.setSecond(dest2);       // a valid destination contact
    maliciousRel.setRelationshipType(availableRelationshipTypes.at(0));
    QVERIFY(!cm->saveRelationship(&maliciousRel));

    // remove the nonexistent relationship
    relationshipsCount = cm->relationships().count();
    QVERIFY(!cm->removeRelationship(maliciousRel));         // does not exist; fail remove.
    QVERIFY(cm->error() == QContactManager::DoesNotExistError || cm->error() == QContactManager::InvalidRelationshipError);
    QCOMPARE(cm->relationships().count(), relationshipsCount); // should be unchanged.

    // now we want to ensure that a relationship is removed if one of the contacts is removed.
    customRelationshipOne.setFirst(source);
    customRelationshipOne.setSecond(dest2);
    customRelationshipOne.setRelationshipType(availableRelationshipTypes.at(0));

    // Test batch save with an error map
    moreRels.clear();
    moreRels << customRelationshipOne;
    errorMap.insert(0, QContactManager::BadArgumentError);
    QVERIFY(cm->saveRelationships(&moreRels, &errorMap));
    QVERIFY(cm->error() == QContactManager::NoError);
    QVERIFY(errorMap.count() == 0); // should be reset
    source = cm->contact(source.localId());
    dest2 = cm->contact(dest2.localId());
    QVERIFY(cm->removeContact(dest2.localId())); // remove dest2, the relationship should be removed
    QVERIFY(cm->relationships(availableRelationshipTypes.at(0), dest2, QContactRelationship::Second).isEmpty());
    source = cm->contact(source.localId());
    QVERIFY(!source.relatedContacts().contains(dest2)); // and it shouldn't appear in cache.

    if (cm->managerName() == QStringLiteral("tracker")) {
        QWARN("The tracker backend does not support checking for existance of a contact. Skipping rest of test.");
        return;
    }

    // now clean up and remove our dests.
    QVERIFY(cm->removeContact(source.localId()));
    QVERIFY(cm->removeContact(dest3.localId()));

    // attempt to save relationships with nonexistent contacts
    QVERIFY(!cm->saveRelationship(&br1));
    QVERIFY(cm->error() == QContactManager::InvalidRelationshipError);
    cm->saveRelationships(&batchList, NULL);
    QVERIFY(cm->error() == QContactManager::InvalidRelationshipError);
    QVERIFY(!cm->removeRelationship(br1));
    QVERIFY(cm->error() == QContactManager::DoesNotExistError || cm->error() == QContactManager::InvalidRelationshipError);
    cm->removeRelationships(batchList, NULL);
    QVERIFY(cm->error() == QContactManager::DoesNotExistError || cm->error() == QContactManager::InvalidRelationshipError);
}

void tst_QContactManager::contactType()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    if (!cm->hasFeature(QContactManager::Groups))
        QSKIP("Skipping: This manager does not support group contacts!");

    QContact g1, g2, c;
    g1.setType(QContactType::TypeGroup);
    g2.setType(QContactType::TypeGroup);

    QContactPhoneNumber g1p, g2p, cp;
    g1p.setNumber("22222");
    g2p.setNumber("11111");
    cp.setNumber("33333");

    g1.saveDetail(&g1p);
    g2.saveDetail(&g2p);
    c.saveDetail(&cp);

    QVERIFY(cm->saveContact(&g1));
    QVERIFY(cm->saveContact(&g2));
    QVERIFY(cm->saveContact(&c));

    // test that the accessing by type works properly
    QContactDetailFilter groupFilter;
    groupFilter.setDetailDefinitionName(QContactType::DefinitionName, QContactType::FieldType);
    groupFilter.setValue(QContactType::TypeGroup);
    QVERIFY(cm->contactIds(groupFilter).contains(g1.localId()));
    QVERIFY(cm->contactIds(groupFilter).contains(g2.localId()));
    QVERIFY(!cm->contactIds(groupFilter).contains(c.localId()));

    QList<QContactSortOrder> sortOrders;
    QContactSortOrder byPhoneNumber;
    byPhoneNumber.setDetailDefinitionName(QContactPhoneNumber::DefinitionName, QContactPhoneNumber::FieldNumber);
    sortOrders.append(byPhoneNumber);

    // and ensure that sorting works properly with typed contacts also
    QList<QContactLocalId> sortedIds = cm->contactIds(groupFilter, sortOrders);
    QVERIFY(sortedIds.indexOf(g2.localId()) < sortedIds.indexOf(g1.localId()));

    cm->removeContact(g1.localId());
    cm->removeContact(g2.localId());
    cm->removeContact(c.localId());
}

#if defined(USE_VERSIT_PLZ)
void tst_QContactManager::partialSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(QContactManager::fromUri(uri));

    QVersitContactImporter imp;
    QVersitReader reader(QByteArray(
            "BEGIN:VCARD\r\nFN:Alice\r\nN:Alice\r\nTEL:12345\r\nEND:VCARD\r\n"
            "BEGIN:VCARD\r\nFN:Bob\r\nN:Bob\r\nTEL:5678\r\nEND:VCARD\r\n"
            "BEGIN:VCARD\r\nFN:Carol\r\nN:Carol\r\nEMAIL:carol@example.com\r\nEND:VCARD\r\n"
            "BEGIN:VCARD\r\nFN:David\r\nN:David\r\nORG:DavidCorp\r\nEND:VCARD\r\n"));
    reader.startReading();
    reader.waitForFinished();
    QCOMPARE(reader.error(), QVersitReader::NoError);

    QCOMPARE(reader.results().count(), 4);
    QVERIFY(imp.importDocuments(reader.results()));
    QCOMPARE(imp.contacts().count(), 4);
    QVERIFY(imp.contacts()[0].displayLabel() == QStringLiteral("Alice"));
    QVERIFY(imp.contacts()[1].displayLabel() == QStringLiteral("Bob"));
    QVERIFY(imp.contacts()[2].displayLabel() == QStringLiteral("Carol"));
    QVERIFY(imp.contacts()[3].displayLabel() == QStringLiteral("David"));

    QList<QContact> contacts = imp.contacts();
    QMap<int, QContactManager::Error> errorMap;

    // First save these contacts
    QVERIFY(cm->saveContacts(&contacts, &errorMap));
    QList<QContact> originalContacts = contacts;

    // Now try some partial save operations
    // 0) empty mask == full save
    // 1) Ignore an added phonenumber
    // 2) Only save a modified phonenumber, not a modified email
    // 3) Remove an email address & phone, mask out phone
    // 4) new contact, no details in the mask
    // 5) new contact, some details in the mask
    // 6) Have a bad manager uri in the middle
    // 7) Have a non existing contact in the middle
    // 8) A list entirely of new contacts

    QContactPhoneNumber pn;
    pn.setNumber("111111");
    contacts[0].saveDetail(&pn);

    // 0) empty mask
    QVERIFY(cm->saveContacts(&contacts, QStringList(), &errorMap));

    // That should have updated everything
    QContact a = cm->contact(originalContacts[0].localId());
    QVERIFY(a.details<QContactPhoneNumber>().count() == 2);

    // 1) Add a phone number to b, mask it out
    contacts[1].saveDetail(&pn);
    QVERIFY(cm->saveContacts(&contacts, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());

    QContact b = cm->contact(originalContacts[1].localId());
    QVERIFY(b.details<QContactPhoneNumber>().count() == 1);

    // 2) save a modified detail in the mask
    QContactEmailAddress e;
    e.setEmailAddress("example@example.com");
    contacts[1].saveDetail(&e); // contacts[1] should have both phone and email

    QVERIFY(cm->saveContacts(&contacts, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());
    b = cm->contact(originalContacts[1].localId());
    QVERIFY(b.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(b.details<QContactEmailAddress>().count() == 1);

    // 3) Remove an email address and a phone number
    QVERIFY(contacts[1].removeDetail(&e));
    QVERIFY(contacts[1].removeDetail(&pn));
    QVERIFY(contacts[1].details<QContactEmailAddress>().count() == 0);
    QVERIFY(contacts[1].details<QContactPhoneNumber>().count() == 1);
    QVERIFY(cm->saveContacts(&contacts, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());
    b = cm->contact(originalContacts[1].localId());
    QVERIFY(b.details<QContactPhoneNumber>().count() == 1);
    QVERIFY(b.details<QContactEmailAddress>().count() == 0);

    // 4 - New contact, no details in the mask
    QContact newContact = originalContacts[3];
    newContact.setId(QContactId());

    contacts.append(newContact);
    QVERIFY(cm->saveContacts(&contacts, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());
    QVERIFY(contacts[4].localId() != 0); // Saved
    b = cm->contact(contacts[4].localId());
    QVERIFY(b.details<QContactOrganization>().count() == 0); // not saved
    QVERIFY(b.details<QContactName>().count() == 0); // not saved

    // 5 - New contact, some details in the mask
    newContact = originalContacts[2];
    newContact.setId(QContactId());
    contacts.append(newContact);
    QVERIFY(cm->saveContacts(&contacts, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());
    QVERIFY(contacts[5].localId() != 0); // Saved
    b = cm->contact(contacts[5].localId());
    QVERIFY(b.details<QContactEmailAddress>().count() == 1);
    QVERIFY(b.details<QContactName>().count() == 0); // not saved

    // 6) Have a bad manager uri in the middle followed by a save error
    QContactId id4(contacts[4].id());
    QContactId badId(id4);
    badId.setManagerUri(QString());
    contacts[4].setId(badId);
    QContactDetail badDetail("BadDetail");
    badDetail.setValue("BadField", "BadValue");
    contacts[5].saveDetail(&badDetail);
    QVERIFY(!cm->saveContacts(&contacts, QStringList("BadDetail"), &errorMap));
    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[4], QContactManager::DoesNotExistError);
    QCOMPARE(errorMap[5], QContactManager::InvalidDetailError);

    // 7) Have a non existing contact in the middle followed by a save error
    badId = id4;
    badId.setLocalId("987234"); // something nonexistent
    contacts[4].setId(badId);
    QVERIFY(!cm->saveContacts(&contacts, QStringList("BadDetail"), &errorMap));
    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[4], QContactManager::DoesNotExistError);
    QCOMPARE(errorMap[5], QContactManager::InvalidDetailError);

    // 8 - New contact, no details in the mask
    newContact = originalContacts[3];
    QCOMPARE(newContact.details<QContactOrganization>().count(), 1);
    QCOMPARE(newContact.details<QContactName>().count(), 1);
    newContact.setId(QContactId());
    QList<QContact> contacts2;
    contacts2.append(newContact);
    QVERIFY(cm->saveContacts(&contacts2, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());
    QVERIFY(contacts2[0].localId() != 0); // Saved
    b = cm->contact(contacts2[0].localId());
    QVERIFY(b.details<QContactOrganization>().count() == 0); // not saved
    QVERIFY(b.details<QContactName>().count() == 0); // not saved

    // 9 - A list with only a new contact, with some details in the mask
    newContact = originalContacts[2];
    newContact.setId(QContactId());
    contacts2.clear();
    contacts2.append(newContact);
    QVERIFY(cm->saveContacts(&contacts2, QStringList(QContactEmailAddress::DefinitionName), &errorMap));
    QVERIFY(errorMap.isEmpty());
    QVERIFY(contacts2[0].localId() != 0); // Saved
    b = cm->contact(contacts2[0].localId());
    QVERIFY(b.details<QContactEmailAddress>().count() == 1);
    QVERIFY(b.details<QContactName>().count() == 0); // not saved

    // 10 - A list with new a contact for the wrong manager, followed by a new contact with an
    // invalid detail
    newContact = originalContacts[2];
    newContact.setId(QContactId());
    contacts2.clear();
    contacts2.append(newContact);
    contacts2.append(newContact);
    contacts2[0].setId(badId);
    contacts2[1].saveDetail(&badDetail);
    QVERIFY(!cm->saveContacts(&contacts2, QStringList("BadDetail"), &errorMap));
    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[0], QContactManager::DoesNotExistError);
    QCOMPARE(errorMap[1], QContactManager::InvalidDetailError);
}
#endif

void tst_QContactManager::lateDeletion()
{
    // Create some engines, but make them get deleted at shutdown
    QFETCH(QString, uri);
    QContactManager* cm = QContactManager::fromUri(uri);

    cm->setParent(qApp); // now do nothing
}

void tst_QContactManager::lazyConnections()
{
    QMap<QString, QString> parameters;
    parameters["version"] = QString("1");
    QContactManager lazy1("testlazy", parameters);
    QContactManager lazy2("testlazy");

    QCOMPARE(lazy1.managerName(), QString("lazy"));
    QCOMPARE(lazy2.managerName(), QString("lazy2"));

    // Make sure the initial connection counts are empty
    QCOMPARE(QContactLazyEngine::connectionCounts.count(), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.count(), 0);

    // Lazy 1 first
    {
        QTestSignalSink casink(&lazy1, SIGNAL(contactsAdded(QList<QContactLocalId>)));

        // See if we got one connection
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.count(), 1);

        // Go out of scope, and see if disconnect is called
    }
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.count(), 1);

    // Lazy2 second
    {
        QTestSignalSink casink(&lazy2, SIGNAL(contactsAdded(QList<QContactLocalId>)));

        // See if we got one connection
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.count(), 1);

        // Go out of scope, and see if disconnect is called
    }
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.count(), 1);

    // Just make sure all the signals get connected correctly
    {
        QTestSignalSink casink(&lazy1, SIGNAL(contactsAdded(QList<QContactLocalId>)));
        QTestSignalSink crsink(&lazy1, SIGNAL(contactsRemoved(QList<QContactLocalId>)));
        QTestSignalSink cmsink(&lazy1, SIGNAL(contactsChanged(QList<QContactLocalId>)));
        QTestSignalSink dcsink(&lazy1, SIGNAL(dataChanged()));
        QTestSignalSink rasink(&lazy1, SIGNAL(relationshipsAdded(QList<QContactLocalId>)));
        QTestSignalSink rrsink(&lazy1, SIGNAL(relationshipsRemoved(QList<QContactLocalId>)));
        QTestSignalSink scsink(&lazy1, SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId)));

        // See if we got all the connections
        QCOMPARE(QContactLazyEngine::connectionCounts.count(), 7);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsChanged(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsRemoved(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(dataChanged())), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(relationshipsAdded(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(relationshipsRemoved(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId))), 1);
    }
    QCOMPARE(QContactLazyEngine::connectionCounts.count(), 7);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsChanged(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(contactsRemoved(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(dataChanged())), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(relationshipsAdded(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(relationshipsRemoved(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine::connectionCounts.value(SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId))), 0);

    // and for lazy2
    {
        QTestSignalSink casink(&lazy2, SIGNAL(contactsAdded(QList<QContactLocalId>)));
        QTestSignalSink crsink(&lazy2, SIGNAL(contactsRemoved(QList<QContactLocalId>)));
        QTestSignalSink cmsink(&lazy2, SIGNAL(contactsChanged(QList<QContactLocalId>)));
        QTestSignalSink dcsink(&lazy2, SIGNAL(dataChanged()));
        QTestSignalSink rasink(&lazy2, SIGNAL(relationshipsAdded(QList<QContactLocalId>)));
        QTestSignalSink rrsink(&lazy2, SIGNAL(relationshipsRemoved(QList<QContactLocalId>)));
        QTestSignalSink scsink(&lazy2, SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId)));

        // See if we got all the connections
        QCOMPARE(QContactLazyEngine2::connectionCounts.count(), 7);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsChanged(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsRemoved(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(dataChanged())), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(relationshipsAdded(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(relationshipsRemoved(QList<QContactLocalId>))), 1);
        QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId))), 1);
    }
    QCOMPARE(QContactLazyEngine2::connectionCounts.count(), 7);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsAdded(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsChanged(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(contactsRemoved(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(dataChanged())), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(relationshipsAdded(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(relationshipsRemoved(QList<QContactLocalId>))), 0);
    QCOMPARE(QContactLazyEngine2::connectionCounts.value(SIGNAL(selfContactIdChanged(QContactLocalId,QContactLocalId))), 0);
}

void tst_QContactManager::compareVariant()
{
    // Exercise this function a bit
    QFETCH(QVariant, a);
    QFETCH(QVariant, b);
    QFETCH(Qt::CaseSensitivity, cs);
    QFETCH(int, expected);

    int comparison = QContactManagerEngine::compareVariant(a, b, cs);
    // Since compareVariant is a little imprecise (just sign matters)
    // convert that here.
    if (comparison < 0)
        comparison = -1;
    else if (comparison > 0)
        comparison = 1;

    QCOMPARE(comparison, expected);

    comparison = QContactManagerEngine::compareVariant(b, a, cs);
    if (comparison < 0)
        comparison = -1;
    else if (comparison > 0)
        comparison = 1;

    // The sign should be flipped now
    QVERIFY((comparison + expected) == 0);
}

void tst_QContactManager::compareVariant_data()
{
    QTest::addColumn<QVariant>("a");
    QTest::addColumn<QVariant>("b");

    QTest::addColumn<Qt::CaseSensitivity>("cs");
    QTest::addColumn<int>("expected");

    // bool
    QTest::newRow("bool <") << QVariant(false) << QVariant(true) << Qt::CaseInsensitive << -1;
    QTest::newRow("bool =") << QVariant(false) << QVariant(false) << Qt::CaseInsensitive << -0;
    QTest::newRow("bool >") << QVariant(true) << QVariant(false) << Qt::CaseInsensitive << 1;

    // char (who uses these??)
    QTest::newRow("char <") << QVariant(QChar('a')) << QVariant(QChar('b')) << Qt::CaseInsensitive << -1;
    QTest::newRow("char < ci") << QVariant(QChar('A')) << QVariant(QChar('b')) << Qt::CaseInsensitive << -1;
    QTest::newRow("char < ci 2") << QVariant(QChar('a')) << QVariant(QChar('B')) << Qt::CaseInsensitive << -1;
    QTest::newRow("char < cs") << QVariant(QChar('a')) << QVariant(QChar('b')) << Qt::CaseSensitive << -1;
    QTest::newRow("char < cs") << QVariant(QChar('A')) << QVariant(QChar('b')) << Qt::CaseSensitive << -1;

    QTest::newRow("char = ci") << QVariant(QChar('a')) << QVariant(QChar('a')) << Qt::CaseInsensitive << 0;
    QTest::newRow("char = ci 2") << QVariant(QChar('a')) << QVariant(QChar('A')) << Qt::CaseInsensitive << 0;
    QTest::newRow("char = ci 3") << QVariant(QChar('A')) << QVariant(QChar('a')) << Qt::CaseInsensitive << 0;
    QTest::newRow("char = ci 4") << QVariant(QChar('A')) << QVariant(QChar('A')) << Qt::CaseInsensitive << 0;
    QTest::newRow("char = cs") << QVariant(QChar('a')) << QVariant(QChar('a')) << Qt::CaseSensitive << 0;
    QTest::newRow("char = cs 2") << QVariant(QChar('A')) << QVariant(QChar('A')) << Qt::CaseSensitive << 0;

    QTest::newRow("char >") << QVariant(QChar('b')) << QVariant(QChar('a')) << Qt::CaseInsensitive << 1;
    QTest::newRow("char > ci") << QVariant(QChar('b')) << QVariant(QChar('A')) << Qt::CaseInsensitive << 1;
    QTest::newRow("char > ci 2") << QVariant(QChar('B')) << QVariant(QChar('a')) << Qt::CaseInsensitive << 1;
    QTest::newRow("char > cs") << QVariant(QChar('b')) << QVariant(QChar('a')) << Qt::CaseSensitive << 1;
    QTest::newRow("char > cs") << QVariant(QChar('b')) << QVariant(QChar('A')) << Qt::CaseSensitive << 1;

    // Some numeric types
    // uint
    QTest::newRow("uint < boundary") << QVariant(uint(1)) << QVariant(uint(-1)) << Qt::CaseInsensitive << -1;
    QTest::newRow("uint <") << QVariant(uint(1)) << QVariant(uint(2)) << Qt::CaseInsensitive << -1;
    QTest::newRow("uint =") << QVariant(uint(2)) << QVariant(uint(2)) << Qt::CaseInsensitive << 0;
    QTest::newRow("uint = 0") << QVariant(uint(0)) << QVariant(uint(0)) << Qt::CaseInsensitive << 0;
    QTest::newRow("uint = boundary") << QVariant(uint(-1)) << QVariant(uint(-1)) << Qt::CaseInsensitive << 0;
    QTest::newRow("uint >") << QVariant(uint(5)) << QVariant(uint(2)) << Qt::CaseInsensitive << 1;
    QTest::newRow("uint > boundary") << QVariant(uint(-1)) << QVariant(uint(2)) << Qt::CaseInsensitive << 1; // boundary

    // int (hmm, signed 32 bit assumed)
    QTest::newRow("int < boundary") << QVariant(int(0x80000000)) << QVariant(int(0x7fffffff)) << Qt::CaseInsensitive << -1;
    QTest::newRow("int <") << QVariant(int(1)) << QVariant(int(2)) << Qt::CaseInsensitive << -1;
    QTest::newRow("int =") << QVariant(int(2)) << QVariant(int(2)) << Qt::CaseInsensitive << 0;
    QTest::newRow("int = 0") << QVariant(int(0)) << QVariant(int(0)) << Qt::CaseInsensitive << 0;
    QTest::newRow("int = boundary") << QVariant(int(0x80000000)) << QVariant(int(0x80000000)) << Qt::CaseInsensitive << 0;
    QTest::newRow("int >") << QVariant(int(5)) << QVariant(int(2)) << Qt::CaseInsensitive << 1;
    QTest::newRow("int > boundary") << QVariant(int(0x7fffffff)) << QVariant(int(0x80000000)) << Qt::CaseInsensitive << 1; // boundary

    // ulonglong
    QTest::newRow("ulonglong < boundary") << QVariant(qulonglong(1)) << QVariant(qulonglong(-1)) << Qt::CaseInsensitive << -1;
    QTest::newRow("ulonglong <") << QVariant(qulonglong(1)) << QVariant(qulonglong(2)) << Qt::CaseInsensitive << -1;
    QTest::newRow("ulonglong =") << QVariant(qulonglong(2)) << QVariant(qulonglong(2)) << Qt::CaseInsensitive << 0;
    QTest::newRow("ulonglong = 0") << QVariant(qulonglong(0)) << QVariant(qulonglong(0)) << Qt::CaseInsensitive << 0;
    QTest::newRow("ulonglong = boundary") << QVariant(qulonglong(-1)) << QVariant(qulonglong(-1)) << Qt::CaseInsensitive << 0;
    QTest::newRow("ulonglong >") << QVariant(qulonglong(5)) << QVariant(qulonglong(2)) << Qt::CaseInsensitive << 1;
    QTest::newRow("ulonglong > boundary") << QVariant(qulonglong(-1)) << QVariant(qulonglong(2)) << Qt::CaseInsensitive << 1; // boundary

    // longlong
    QTest::newRow("longlong < boundary") << QVariant(qlonglong(0x8000000000000000LL)) << QVariant(qlonglong(0x7fffffffffffffffLL)) << Qt::CaseInsensitive << -1;
    QTest::newRow("longlong <") << QVariant(qlonglong(1)) << QVariant(qlonglong(2)) << Qt::CaseInsensitive << -1;
    QTest::newRow("longlong =") << QVariant(qlonglong(2)) << QVariant(qlonglong(2)) << Qt::CaseInsensitive << 0;
    QTest::newRow("longlong = 0") << QVariant(qlonglong(0)) << QVariant(qlonglong(0)) << Qt::CaseInsensitive << 0;
    QTest::newRow("longlong = boundary") << QVariant(qlonglong(0x8000000000000000LL)) << QVariant(qlonglong(0x8000000000000000LL)) << Qt::CaseInsensitive << 0;
    QTest::newRow("longlong >") << QVariant(qlonglong(5)) << QVariant(qlonglong(2)) << Qt::CaseInsensitive << 1;
    QTest::newRow("longlong > boundary") << QVariant(qlonglong(0x7fffffffffffffffLL)) << QVariant(qlonglong(0x8000000000000000LL)) << Qt::CaseInsensitive << 1; // boundary

    // double (hmm, skips NaNs etc)
    QTest::newRow("double < inf 2") << QVariant(-qInf()) << QVariant(qInf()) << Qt::CaseInsensitive << -1;
    QTest::newRow("double < inf") << QVariant(1.0) << QVariant(qInf()) << Qt::CaseInsensitive << -1;
    QTest::newRow("double <") << QVariant(1.0) << QVariant(2.0) << Qt::CaseInsensitive << -1;
    QTest::newRow("double =") << QVariant(2.0) << QVariant(2.0) << Qt::CaseInsensitive << 0;
    QTest::newRow("double = 0") << QVariant(0.0) << QVariant(0.0) << Qt::CaseInsensitive << 0;
    QTest::newRow("double = inf") << QVariant(qInf()) << QVariant(qInf()) << Qt::CaseInsensitive << 0;
    QTest::newRow("double >") << QVariant(5.0) << QVariant(2.0) << Qt::CaseInsensitive << 1;
    QTest::newRow("double > inf") << QVariant(qInf()) << QVariant(5.0) << Qt::CaseInsensitive << 1;
    QTest::newRow("double > inf 2") << QVariant(0.0) << QVariant(-qInf()) << Qt::CaseInsensitive << 1;
    QTest::newRow("double > inf 3") << QVariant(qInf()) << QVariant(-qInf()) << Qt::CaseInsensitive << 1;

    // strings
    QTest::newRow("string <") << QVariant(QString("a")) << QVariant(QString("b")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string <") << QVariant(QString("a")) << QVariant(QString("B")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string <") << QVariant(QString("A")) << QVariant(QString("b")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string <") << QVariant(QString("A")) << QVariant(QString("B")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string < cs") << QVariant(QString("a")) << QVariant(QString("b")) << Qt::CaseSensitive << -1;
    QTest::newRow("string < cs 2") << QVariant(QString("A")) << QVariant(QString("b")) << Qt::CaseSensitive << -1;

    QTest::newRow("string < length") << QVariant(QString("a")) << QVariant(QString("aa")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string < length cs") << QVariant(QString("a")) << QVariant(QString("aa")) << Qt::CaseSensitive << -1;
    QTest::newRow("string < length 2") << QVariant(QString("a")) << QVariant(QString("ba")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string < length cs 2") << QVariant(QString("a")) << QVariant(QString("ba")) << Qt::CaseSensitive << -1;

    QTest::newRow("string aa < b") << QVariant(QString("aa")) << QVariant(QString("b")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string aa < b cs") << QVariant(QString("aa")) << QVariant(QString("b")) << Qt::CaseSensitive << -1;

    QTest::newRow("string '' < a") << QVariant(QString("")) << QVariant(QString("aa")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string '' < aa cs") << QVariant(QString("")) << QVariant(QString("aa")) << Qt::CaseSensitive << -1;
    QTest::newRow("string 0 < a") << QVariant(QString()) << QVariant(QString("aa")) << Qt::CaseInsensitive << -1;
    QTest::newRow("string 0 < aa cs") << QVariant(QString()) << QVariant(QString("aa")) << Qt::CaseSensitive << -1;

    QTest::newRow("string '' = ''") << QVariant(QString("")) << QVariant(QString("")) << Qt::CaseInsensitive << 0;
    QTest::newRow("string '' = '' cs") << QVariant(QString("")) << QVariant(QString("")) << Qt::CaseSensitive << 0;
    QTest::newRow("string 0 = 0") << QVariant(QString()) << QVariant(QString()) << Qt::CaseInsensitive << 0;
    QTest::newRow("string 0 = 0 cs") << QVariant(QString()) << QVariant(QString()) << Qt::CaseSensitive << 0;
    QTest::newRow("string a = a") << QVariant(QString("a")) << QVariant(QString("a")) << Qt::CaseInsensitive << 0;
    QTest::newRow("string a = a cs") << QVariant(QString("a")) << QVariant(QString("a")) << Qt::CaseSensitive << 0;

    // Stringlists
    // {} < {"a"} < {"aa"} < {"aa","bb"} < {"aa", "cc"} < {"bb"}

    QStringList empty;
    QStringList listA("a");
    QStringList listAA("aa");
    QStringList listAABB;
    listAABB << "aa" << "bb";
    QStringList listAACC;
    listAACC << "aa" << "cc";
    QStringList listBB;
    listBB << "bb";
    QStringList listCCAA;
    listCCAA << "cc" << "aa";
    QStringList listA2("A");
    QStringList listAA2("AA");

    QTest::newRow("stringlist {} < {a}") << QVariant(empty) << QVariant(listA) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {} < {a} cs") << QVariant(empty) << QVariant(listA) << Qt::CaseSensitive << -1;
    QTest::newRow("stringlist {} < {A}") << QVariant(empty) << QVariant(listA2) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {} < {A} cs") << QVariant(empty) << QVariant(listA2) << Qt::CaseSensitive << -1;

    QTest::newRow("stringlist {a} < {aa}") << QVariant(listA) << QVariant(listAA) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {a} < {aa} cs") << QVariant(listA) << QVariant(listAA) << Qt::CaseSensitive << -1;
    QTest::newRow("stringlist {a} < {AA}") << QVariant(listA) << QVariant(listAA2) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {a} < {AA} cs") << QVariant(listA) << QVariant(listAA2) << Qt::CaseSensitive << -1;

    QTest::newRow("stringlist {A} < {aa,bb}") << QVariant(listA2) << QVariant(listAABB) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {A} < {aa,bb} cs") << QVariant(listA2) << QVariant(listAABB) << Qt::CaseSensitive << -1;
    QTest::newRow("stringlist {aa} < {aa,bb}") << QVariant(listAA) << QVariant(listAABB) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {aa} < {aa,bb} cs") << QVariant(listAA) << QVariant(listAABB) << Qt::CaseSensitive << -1;

    QTest::newRow("stringlist {aa,bb} < {aa,cc}") << QVariant(listAABB) << QVariant(listAACC) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {aa,bb} < {aa,cc} cs") << QVariant(listAABB) << QVariant(listAACC) << Qt::CaseSensitive << -1;

    QTest::newRow("stringlist {aa,cc} < {bb}") << QVariant(listAACC) << QVariant(listBB) << Qt::CaseInsensitive << -1;
    QTest::newRow("stringlist {aa,cc} < {bb} cs") << QVariant(listAACC) << QVariant(listBB) << Qt::CaseSensitive << -1;

    // equality
    QTest::newRow("stringlist {} = {}") << QVariant(empty) << QVariant(empty) << Qt::CaseInsensitive << 0;
    QTest::newRow("stringlist {} = {} cs") << QVariant(empty) << QVariant(empty) << Qt::CaseSensitive << 0;
    QTest::newRow("stringlist {aa} = {aa}") << QVariant(listAA) << QVariant(listAA) << Qt::CaseInsensitive << 0;
    QTest::newRow("stringlist {aa} = {AA}") << QVariant(listAA) << QVariant(listAA2) << Qt::CaseInsensitive << 0;
    QTest::newRow("stringlist {aa} = {aa} cs") << QVariant(listAA) << QVariant(listAA) << Qt::CaseSensitive << 0;

    // Times
    QTime t0;
    QTime t1(0,0,0,0);
    QTime t2(0,59,0,0);
    QTime t3(1,0,0,0);
    QTime t4(23,59,59,999);

    QTest::newRow("times t0 < t1") << QVariant(t0) << QVariant(t1) << Qt::CaseInsensitive << -1;
    QTest::newRow("times t1 < t2") << QVariant(t1) << QVariant(t2) << Qt::CaseInsensitive << -1;
    QTest::newRow("times t2 < t3") << QVariant(t2) << QVariant(t3) << Qt::CaseInsensitive << -1;
    QTest::newRow("times t3 < t4") << QVariant(t3) << QVariant(t4) << Qt::CaseInsensitive << -1;
    QTest::newRow("times t0 = t0") << QVariant(t0) << QVariant(t0) << Qt::CaseInsensitive << 0;
    QTest::newRow("times t4 = t4") << QVariant(t4) << QVariant(t4) << Qt::CaseInsensitive << 0;

    // Dates
    QDate d0;
    QDate d1 = QDate::fromJulianDay(1);
    QDate d2(1,1,1);
    QDate d3(2011,6,9);
    QDate d4 = QDate::fromJulianDay(0x7fffffff);
    QDate d5 = QDate::fromJulianDay(0x80000000);
    QDate d6 = QDate::fromJulianDay(0xffffffff);

    QTest::newRow("dates d0 < d1") << QVariant(d0) << QVariant(d1) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d1 < d2") << QVariant(d1) << QVariant(d2) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d2 < d3") << QVariant(d2) << QVariant(d3) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d3 < d4") << QVariant(d3) << QVariant(d4) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d4 < d5") << QVariant(d4) << QVariant(d5) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d5 < d6") << QVariant(d5) << QVariant(d6) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d0 < d6") << QVariant(d0) << QVariant(d6) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d1 < d6") << QVariant(d1) << QVariant(d6) << Qt::CaseInsensitive << -1;
    QTest::newRow("dates d0 = d0") << QVariant(d0) << QVariant(d0) << Qt::CaseInsensitive << 0;
    QTest::newRow("dates d1 = d1") << QVariant(d1) << QVariant(d1) << Qt::CaseInsensitive << 0;
    QTest::newRow("dates d2 = d2") << QVariant(d2) << QVariant(d2) << Qt::CaseInsensitive << 0;
    QTest::newRow("dates d3 = d3") << QVariant(d3) << QVariant(d3) << Qt::CaseInsensitive << 0;
    QTest::newRow("dates d4 = d4") << QVariant(d4) << QVariant(d4) << Qt::CaseInsensitive << 0;
    QTest::newRow("dates d5 = d5") << QVariant(d5) << QVariant(d5) << Qt::CaseInsensitive << 0;
    QTest::newRow("dates d6 = d6") << QVariant(d6) << QVariant(d6) << Qt::CaseInsensitive << 0;

    // DateTimes
    // Somewhat limited testing here
    QDateTime dt0;
    QDateTime dt1(d1, t1);
    QDateTime dt2(d1, t2);
    QDateTime dt3(d4, t4);
    QDateTime dt4(d5, t1);
    QDateTime dt5(d6, t4); // end of the universe

    QTest::newRow("datetimes dt0 < dt1") << QVariant(dt0) << QVariant(dt1) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt1 < dt2") << QVariant(dt1) << QVariant(dt2) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt2 < dt3") << QVariant(dt2) << QVariant(dt3) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt3 < dt4") << QVariant(dt3) << QVariant(dt4) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt4 < dt5") << QVariant(dt4) << QVariant(dt5) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt0 < dt5") << QVariant(dt0) << QVariant(dt5) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt1 < dt5") << QVariant(dt1) << QVariant(dt5) << Qt::CaseInsensitive << -1;
    QTest::newRow("datetimes dt0 = dt0") << QVariant(dt0) << QVariant(dt0) << Qt::CaseInsensitive << 0;
    QTest::newRow("datetimes dt1 = dt1") << QVariant(dt1) << QVariant(dt1) << Qt::CaseInsensitive << 0;
    QTest::newRow("datetimes dt2 = dt2") << QVariant(dt2) << QVariant(dt2) << Qt::CaseInsensitive << 0;
    QTest::newRow("datetimes dt3 = dt3") << QVariant(dt3) << QVariant(dt3) << Qt::CaseInsensitive << 0;
    QTest::newRow("datetimes dt4 = dt4") << QVariant(dt4) << QVariant(dt4) << Qt::CaseInsensitive << 0;
    QTest::newRow("datetimes dt5 = dt5") << QVariant(dt5) << QVariant(dt5) << Qt::CaseInsensitive << 0;
}


QTEST_MAIN(tst_QContactManager)
#include "tst_qcontactmanager.moc"
