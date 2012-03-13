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

#include <QCoreApplication>
#include <QScopedPointer>

#include <QContact>
#include <QContactAbstractRequest>
#include <QContactTimestamp>
#include <QContactFetchRequest>
#include <QContactDetailFilter>
#include <QContactUrl>
#include <QContactPhoneNumber>
#include <QContactFetchByIdRequest>
#include <QContactIdFetchRequest>
#include <QContactRemoveRequest>
#include <QContactSaveRequest>
#include <QContactEmailAddress>
#include <QContactOnlineAccount>
#include <QContactTag>
#include <QContactOrganization>
#include <QContactRelationshipFetchRequest>
#include <QContactRelationshipRemoveRequest>
#include <QContactRelationshipSaveRequest>
#include <QContactName>
#include <QContactIdFilter>

#include "../../../jsondbprocess.h"
#include "../../qcontactidmock.h"
#include "../../qcontactmanagerdataholder.h" //QContactManagerDataHolder

QTCONTACTS_USE_NAMESPACE

/* Define an innocuous request (fetch ie doesn't mutate) to "fill up" any queues */
#define FILL_QUEUE_WITH_FETCH_REQUESTS() QContactFetchRequest fqcfr1, fqcfr2, fqcfr3; \
                                         fqcfr1.start(); \
                                         fqcfr2.start(); \
                                         fqcfr3.start();

//TESTED_COMPONENT=src/contacts

// Unfortunately the plumbing isn't in place to allow cancelling requests at arbitrary points
// in their processing.  So we do multiple loops until things work out.. or not
#define MAX_OPTIMISTIC_SCHEDULING_LIMIT 100


// Thread capable QThreadSignalSpy (to avoid data races with count/appendArgS)
class QThreadSignalSpy: public QObject
{
public:
    QThreadSignalSpy(QObject *obj, const char *aSignal)
    {
        QMutexLocker m(&lock);
#ifdef Q_CC_BOR
        const int memberOffset = QObject::staticMetaObject.methodCount();
#else
        static const int memberOffset = QObject::staticMetaObject.methodCount();
#endif
        Q_ASSERT(obj);
        Q_ASSERT(aSignal);

        if (((aSignal[0] - '0') & 0x03) != QSIGNAL_CODE) {
            qWarning("QThreadSignalSpy: Not a valid signal, use the SIGNAL macro");
            return;
        }

        QByteArray ba = QMetaObject::normalizedSignature(aSignal + 1);
        const QMetaObject *mo = obj->metaObject();
        int sigIndex = mo->indexOfMethod(ba.constData());
        if (sigIndex < 0) {
            qWarning("QThreadSignalSpy: No such signal: '%s'", ba.constData());
            return;
        }

        if (!QMetaObject::connect(obj, sigIndex, this, memberOffset,
                    Qt::DirectConnection, 0)) {
            qWarning("QThreadSignalSpy: QMetaObject::connect returned false. Unable to connect.");
            return;
        }
        sig = ba;
        initArgs(mo->method(sigIndex));
    }

    inline bool isValid() const { return !sig.isEmpty(); }
    inline QByteArray signal() const { return sig; }

    int qt_metacall(QMetaObject::Call call, int methodId, void **a)
    {
        methodId = QObject::qt_metacall(call, methodId, a);
        if (methodId < 0)
            return methodId;

        if (call == QMetaObject::InvokeMetaMethod) {
            if (methodId == 0) {
                appendArgs(a);
            }
            --methodId;
        }
        return methodId;
    }

    // The QList<QVariantList> API we actually use
    int count() const
    {
        QMutexLocker m(&lock);
        return savedArgs.count();
    }
    void clear()
    {
        QMutexLocker m(&lock);
        savedArgs.clear();
    }

private:
    void initArgs(const QMetaMethod &member)
    {
        QList<QByteArray> params = member.parameterTypes();
        for (int i = 0; i < params.count(); ++i) {
            int tp = QMetaType::type(params.at(i).constData());
            if (tp == QMetaType::Void)
                qWarning("Don't know how to handle '%s', use qRegisterMetaType to register it.",
                         params.at(i).constData());
            args << tp;
        }
    }

    void appendArgs(void **a)
    {
        QMutexLocker m(&lock);
        QList<QVariant> list;
        for (int i = 0; i < args.count(); ++i) {
            QMetaType::Type type = static_cast<QMetaType::Type>(args.at(i));
            list << QVariant(type, a[i + 1]);
        }
        savedArgs.append(list);
    }

    // the full, normalized signal name
    QByteArray sig;
    // holds the QMetaType types for the argument list of the signal
    QList<int> args;

    mutable QMutex lock;
    // Different API
    QList< QVariantList> savedArgs;
};

class tst_QContactAsync : public QObject
{
    Q_OBJECT

public:
    tst_QContactAsync();
    virtual ~tst_QContactAsync();

public slots:
    void initTestCase();
    void cleanupTestCase();

private:
    void addManagers(QStringList includes = QStringList()); // add standard managers to the data

private slots:
    void testDestructor();
    void testDestructor_data() { addManagers(QStringList(QString("maliciousplugin"))); }

    void contactFetch();
    void contactFetch_data() { addManagers(); }
    void contactFetchById();
    void contactFetchById_data() { addManagers(); }
    void contactIdFetch();
    void contactIdFetch_data() { addManagers(); }
    void contactRemove();
    void contactRemove_data() { addManagers(); }
    void contactSave();
    void contactSave_data() { addManagers(); }
    void contactPartialSave();
    void contactPartialSave_data() { addManagers(); }
    void contactPartialSaveAsync();
    void contactPartialSaveAsync_data() {addManagers();}

    void relationshipFetch();
    void relationshipFetch_data() { addManagers(); }
    void relationshipRemove();
    void relationshipRemove_data() { addManagers(); }
    void relationshipSave();
    void relationshipSave_data() { addManagers(); }

    void maliciousManager(); // uses it's own custom data (manager)

    void testQuickDestruction();
    void testQuickDestruction_data() { addManagers(QStringList(QString("maliciousplugin"))); }

    void threadDelivery();
    void threadDelivery_data() { addManagers(QStringList(QString("maliciousplugin"))); }
protected slots:
    void resultsAvailableReceived();
    void deleteRequest();

private:
    bool compareContactLists(QList<QContact> lista, QList<QContact> listb);
    bool compareContacts(QContact ca, QContact cb);
    bool containsIgnoringTimestamps(const QList<QContact>& list, const QContact& c);
    bool compareIgnoringTimestamps(const QContact& ca, const QContact& cb);
    QContactManager* prepareModel(const QString& uri);

    Qt::HANDLE m_mainThreadId;
    Qt::HANDLE m_resultsAvailableSlotThreadId;
    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

    JsonDbProcess jsondbProcess;

};

tst_QContactAsync::tst_QContactAsync()
{
    // ensure we can load all of the plugins we need to.
    QString path = QCoreApplication::applicationDirPath() + QStringLiteral("/dummyplugin/plugins");
    QCoreApplication::addLibraryPath(path);

    qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
}

tst_QContactAsync::~tst_QContactAsync()
{
}

void tst_QContactAsync::initTestCase()
{
    // Start JsonDb daemon if needed
    if (QContactManager::availableManagers().contains("jsondb"))
        QVERIFY2(jsondbProcess.start(), "Failed to start JsonDb process");

    managerDataHolder.reset(new QContactManagerDataHolder());
}

void tst_QContactAsync::cleanupTestCase()
{
    managerDataHolder.reset(0);

    if (QContactManager::availableManagers().contains("jsondb"))
        jsondbProcess.terminate();
}

bool tst_QContactAsync::compareContactLists(QList<QContact> lista, QList<QContact> listb)
{
    // NOTE: This compare is contact order insensitive.  
    
    // Remove matching contacts
    foreach (QContact a, lista) {
        foreach (QContact b, listb) {
            if (compareContacts(a, b)) {
                lista.removeOne(a);
                listb.removeOne(b);
                break;
            }
        }
    }    
    return (lista.count() == 0 && listb.count() == 0);
}

bool tst_QContactAsync::compareContacts(QContact ca, QContact cb)
{
    // NOTE: This compare is contact detail order insensitive.
    
    if (ca.id() != cb.id())
        return false;
    
    QList<QContactDetail> aDetails = ca.details();
    QList<QContactDetail> bDetails = cb.details();

    // Remove matching details
    foreach (QContactDetail ad, aDetails) {
        foreach (QContactDetail bd, bDetails) {
            if (ad == bd) {
                ca.removeDetail(&ad);
                cb.removeDetail(&bd);
                break;
            }
            
            // Special handling for timestamp
            if (ad.type() == QContactTimestamp::Type &&
                bd.type() == QContactTimestamp::Type) {
                QContactTimestamp at = static_cast<QContactTimestamp>(ad);
                QContactTimestamp bt = static_cast<QContactTimestamp>(bd);
                if (at.created().toString() == bt.created().toString() &&
                    at.lastModified().toString() == bt.lastModified().toString()) {
                    ca.removeDetail(&ad);
                    cb.removeDetail(&bd);
                    break;
                }
                    
            }            
        }
    }
    return (ca == cb);
}

bool tst_QContactAsync::containsIgnoringTimestamps(const QList<QContact>& list, const QContact& c)
{
    QList<QContact> cl = list;
    QContact a(c);
    for (int i = 0; i < cl.size(); i++) {
        QContact b(cl.at(i));
        if (compareIgnoringTimestamps(a, b))
            return true;
    }

    return false;
}

bool tst_QContactAsync::compareIgnoringTimestamps(const QContact& ca, const QContact& cb)
{
    // Compares two contacts, ignoring any timestamp details
    QContact a(ca);
    QContact b(cb);
    QList<QContactDetail> aDetails = a.details();
    QList<QContactDetail> bDetails = b.details();

    // They can be in any order, so loop
    // First remove any matches, and any timestamps
    foreach (QContactDetail d, aDetails) {
        foreach (QContactDetail d2, bDetails) {
            if (d == d2) {
                a.removeDetail(&d);
                b.removeDetail(&d2);
                break;
            }

            if (d.type() == QContactTimestamp::Type) {
                a.removeDetail(&d);
            }

            if (d2.type() == QContactTimestamp::Type) {
                b.removeDetail(&d2);
            }
        }
    }

    if (a == b)
        return true;
    return false;
}

void tst_QContactAsync::testDestructor()
{
    QFETCH(QString, uri);
    QContactManager* cm = prepareModel(uri);
    QContactFetchRequest* req = new QContactFetchRequest;
    req->setManager(cm);

    QContactManager* cm2 = prepareModel(uri);
    QContactFetchRequest* req2 = new QContactFetchRequest;
    req2->setManager(cm2);

    // first, delete manager then request
    delete cm;
    delete req;

    // second, delete request then manager
    delete req2;
    delete cm2;
}

void tst_QContactAsync::deleteRequest()
{
    // Delete the sender (request) - check that it doesn't crash in this common coding error
    delete sender();
}

void tst_QContactAsync::contactFetch()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactFetchRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::ContactFetchRequest);

    // initial state - not started, no manager.
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.start());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());

    // "all contacts" retrieval
    QContactFilter fil;
    cfr.setManager(cm.data());
    QCOMPARE(cfr.manager(), cm.data());
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());
    qRegisterMetaType<QContactFetchRequest*>("QContactFetchRequest*");
    QThreadSignalSpy spy(&cfr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    cfr.setFilter(fil);
    QCOMPARE(cfr.filter(), fil);
    QVERIFY(!cfr.cancel()); // not started

    QVERIFY(cfr.start());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContactId> contactIds = cm->contactIds();
    QList<QContact> contacts = cfr.contacts();
    QCOMPARE(contactIds.size(), contacts.size());
    for (int i = 0; i < contactIds.size(); i++) {
        QContact curr = cm->contact(contactIds.at(i));
        QVERIFY(contacts.at(i) == curr);
    }

    // asynchronous detail filtering
    QContactDetailFilter dfil;
    dfil.setDetailType(QContactUrl::Type, QContactUrl::FieldUrl);
    cfr.setFilter(dfil);
    QVERIFY(cfr.filter() == dfil);
    QVERIFY(!cfr.cancel()); // not started

    QVERIFY(cfr.start());
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    contactIds = cm->contactIds(dfil);
    contacts = cfr.contacts();
    QCOMPARE(contactIds.size(), contacts.size());
    for (int i = 0; i < contactIds.size(); i++) {
        QContact curr = cm->contact(contactIds.at(i));
        QVERIFY(contacts.at(i) == curr);
    }

    // sort order
    QContactSortOrder sortOrder;
    sortOrder.setDetailType(QContactPhoneNumber::Type, QContactPhoneNumber::FieldNumber);
    QList<QContactSortOrder> sorting;
    sorting.append(sortOrder);
    cfr.setFilter(fil);
    cfr.setSorting(sorting);
    QCOMPARE(cfr.sorting(), sorting);
    QVERIFY(!cfr.cancel()); // not started
    QVERIFY(cfr.start());
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    contactIds = cm->contactIds(sorting);
    contacts = cfr.contacts();
    QCOMPARE(contactIds.size(), contacts.size());
    for (int i = 0; i < contactIds.size(); i++) {
        QContact curr = cm->contact(contactIds.at(i));
        QVERIFY(contacts.at(i) == curr);
    }

    // restrictions
    sorting.clear();
    cfr.setFilter(fil);
    cfr.setSorting(sorting);
    QContactFetchHint fetchHint;
    QList<QContactDetail::DetailType> typeHints;
    typeHints << QContactName::Type;
    fetchHint.setDetailTypesHint(typeHints);
    cfr.setFetchHint(fetchHint);
    QCOMPARE(cfr.fetchHint().detailTypesHint(), typeHints);
    QVERIFY(!cfr.cancel()); // not started
    QVERIFY(cfr.start());
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    contactIds = cm->contactIds(sorting);
    contacts = cfr.contacts();
    QCOMPARE(contactIds.size(), contacts.size());
    for (int i = 0; i < contactIds.size(); i++) {
        // create a contact from the restricted data only (id + display label)
        QContact currFull = cm->contact(contactIds.at(i));
        QContact currRestricted;
        currRestricted.setId(currFull.id());
        QList<QContactName> names = currFull.details<QContactName>();
        foreach (const QContactName& name, names) {
            QContactName fullName = name;
            if (!fullName.isEmpty()) {
                currRestricted.saveDetail(&fullName);
            }
        }

        // now find the contact in the retrieved list which our restricted contact mimics
        QContact retrievedRestricted;
        bool found = false;
        foreach (const QContact& retrieved, contacts) {
            if (retrieved.id() == currRestricted.id()) {
                retrievedRestricted = retrieved;
                found = true;
            }
        }

        QVERIFY(found); // must exist or fail.

        // ensure that the contact is the same (except synth fields)
        QList<QContactDetail> retrievedDetails = retrievedRestricted.details();
        QList<QContactDetail> expectedDetails = currRestricted.details();
        foreach (const QContactDetail& det, expectedDetails) {
            // ignore backend synthesised details
            // again, this requires a "default contact details" function to work properly.
            if (det.type() == QContactDisplayLabel::Type
                || det.type() == QContactTimestamp::Type) {
                continue;
            }

            // everything else in the expected contact should be in the retrieved one.
            QVERIFY(retrievedDetails.contains(det));
        }
    }

    // cancelling
    sorting.clear();
    cfr.setFilter(fil);
    cfr.setSorting(sorting);
    cfr.setFetchHint(QContactFetchHint());

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!cfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(cfr.start());
        if (!cfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            spy.clear();
            cfr.waitForFinished();
            sorting.clear();
            cfr.setFilter(fil);
            cfr.setSorting(sorting);
            cfr.setFetchHint(QContactFetchHint());
            cfr.setFetchHint(QContactFetchHint());
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(cfr.waitForFinished());
        QVERIFY(cfr.isCanceled());

        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!cfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(cfr.start());
        if (!cfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            cfr.waitForFinished();
            sorting.clear();
            cfr.setFilter(fil);
            cfr.setSorting(sorting);
            cfr.setFetchHint(QContactFetchHint());
            bailoutCount -= 1;
            spy.clear();
            if (!bailoutCount) {
                //qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            continue;
        }
        cfr.waitForFinished();
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        QVERIFY(!cfr.isActive());
        QVERIFY(cfr.state() == QContactAbstractRequest::CanceledState);
        break;
    }

    // Now test deletion in the first slot called
    QContactFetchRequest *cfr2 = new QContactFetchRequest();
    QPointer<QObject> obj(cfr2);
    cfr2->setManager(cm.data());
    connect(cfr2, SIGNAL(stateChanged(QContactAbstractRequest::State)), this, SLOT(deleteRequest()));
    QVERIFY(cfr2->start());
    int i = 100;
    // at this point we can't even call wait for finished..
    while(obj && i > 0) {
        QTest::qWait(50); // force it to process events at least once.
        i--;
    }
    QVERIFY(obj == NULL);
}

void tst_QContactAsync::contactFetchById()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactFetchByIdRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::ContactFetchByIdRequest);

    // initial state - not started, no manager.
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.start());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());

    // get all contact ids
    QList<QContactId> contactIds(cm->contactIds());

    // "all contacts" retrieval
    cfr.setManager(cm.data());
    cfr.setIds(contactIds);
    QCOMPARE(cfr.manager(), cm.data());
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());
    qRegisterMetaType<QContactFetchByIdRequest*>("QContactFetchByIdRequest*");
    QThreadSignalSpy spy(&cfr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QVERIFY(!cfr.cancel()); // not started

    QVERIFY(cfr.start());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContact> contacts = cfr.contacts();
    QCOMPARE(contactIds.size(), contacts.size());
    for (int i = 0; i < contactIds.size(); i++) {
        QContact curr = cm->contact(contactIds.at(i));
        QVERIFY(contacts.at(i) == curr);
    }
}


void tst_QContactAsync::contactIdFetch()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactIdFetchRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::ContactIdFetchRequest);

    // initial state - not started, no manager.
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.start());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());

    // "all contacts" retrieval
    QContactFilter fil;
    cfr.setManager(cm.data());
    QCOMPARE(cfr.manager(), cm.data());
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());
    qRegisterMetaType<QContactIdFetchRequest*>("QContactIdFetchRequest*");

    QThreadSignalSpy spy(&cfr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    cfr.setFilter(fil);
    QCOMPARE(cfr.filter(), fil);
    QVERIFY(!cfr.cancel()); // not started
    QVERIFY(cfr.start());

    QVERIFY((cfr.isActive() &&cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContactId> contactIds = cm->contactIds();
    QList<QContactId> result = cfr.ids();
    QCOMPARE(contactIds, result);

    // asynchronous detail filtering
    QContactDetailFilter dfil;
    dfil.setDetailType(QContactUrl::Type, QContactUrl::FieldUrl);
    cfr.setFilter(dfil);
    QVERIFY(cfr.filter() == dfil);
    QVERIFY(!cfr.cancel()); // not started

    QVERIFY(cfr.start());
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    contactIds = cm->contactIds(dfil);
    result = cfr.ids();
    QCOMPARE(contactIds, result);

    // sort order
    QContactSortOrder sortOrder;
    sortOrder.setDetailType(QContactPhoneNumber::Type, QContactPhoneNumber::FieldNumber);
    QList<QContactSortOrder> sorting;
    sorting.append(sortOrder);
    cfr.setFilter(fil);
    cfr.setSorting(sorting);
    QCOMPARE(cfr.sorting(), sorting);
    QVERIFY(!cfr.cancel()); // not started
    QVERIFY(cfr.start());
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    contactIds = cm->contactIds(sorting);
    result = cfr.ids();
    QCOMPARE(contactIds, result);

    // cancelling
    sorting.clear();
    cfr.setFilter(fil);
    cfr.setSorting(sorting);

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!cfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(cfr.start());
        if (!cfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            cfr.waitForFinished();
            sorting.clear();
            cfr.setFilter(fil);
            cfr.setSorting(sorting);
            bailoutCount -= 1;
            spy.clear();
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(cfr.waitForFinished());
        QVERIFY(cfr.isCanceled());

        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!cfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(cfr.start());
        if (!cfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            cfr.waitForFinished();
            sorting.clear();
            cfr.setFilter(fil);
            cfr.setSorting(sorting);
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            continue;
        }
        cfr.waitForFinished();
        QVERIFY(cfr.isCanceled());

        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

    // Error cases not tested here as can not generate common error case
    // for all backends. For example, for jsondb backend can not generate
    // error at all.
}

void tst_QContactAsync::contactRemove()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactRemoveRequest crr;
    QVERIFY(crr.type() == QContactAbstractRequest::ContactRemoveRequest);

    // initial state - not started, no manager.
    QVERIFY(!crr.isActive());
    QVERIFY(!crr.isFinished());
    QVERIFY(!crr.start());
    QVERIFY(!crr.cancel());
    QVERIFY(!crr.waitForFinished());

    // specific contact set
    crr.setContactId(cm->contactIds()[0]);
    QVERIFY(crr.contactIds() == QList<QContactId>() << cm->contactIds()[0]);

    // specific contact removal via detail filter
    int originalCount = cm->contactIds().size();
    QContactDetailFilter dfil;
    dfil.setDetailType(QContactUrl::Type);
    crr.setContactIds(cm->contactIds(dfil));
    crr.setManager(cm.data());
    QCOMPARE(crr.manager(), cm.data());
    QVERIFY(!crr.isActive());
    QVERIFY(!crr.isFinished());
    QVERIFY(!crr.cancel());
    QVERIFY(!crr.waitForFinished());
    qRegisterMetaType<QContactRemoveRequest*>("QContactRemoveRequest*");
    QThreadSignalSpy spy(&crr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QVERIFY(!crr.cancel()); // not started

    QVERIFY(!cm->contactIds(dfil).isEmpty());

    QVERIFY(crr.start());

    QVERIFY((crr.isActive() &&crr.state() == QContactAbstractRequest::ActiveState) || crr.isFinished());
    //QVERIFY(crr.isFinished() || !crr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QCOMPARE(cm->contactIds().size(), originalCount - 1);
    QVERIFY(cm->contactIds(dfil).isEmpty());

    // remove all contacts
    // delete everything.
    crr.setContactIds(cm->contactIds());
    
    QVERIFY(!crr.cancel()); // not started
    QVERIFY(crr.start());

    QVERIFY((crr.isActive() && crr.state() == QContactAbstractRequest::ActiveState) || crr.isFinished());
    //QVERIFY(crr.isFinished() || !crr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());

    QCOMPARE(cm->contactIds().size(), 0); // no contacts should be left.
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // cancelling
    QContact temp;
    QContactName nameDetail;
    nameDetail.setFirstName("Should not be removed");
    temp.saveDetail(&nameDetail);
    cm->saveContact(&temp);
    crr.setContactIds(cm->contactIds(dfil));

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
    while (true) {
        QVERIFY(!crr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(spy.count() == 0);
        QVERIFY(crr.start());
        if (!crr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            crr.waitForFinished();
            crr.setContactIds(cm->contactIds(dfil));
            temp.setId(QContactId());
            if (!cm->saveContact(&temp)) {
                QSKIP("Unable to save temporary contact for remove request cancellation test!");
            }
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(crr.waitForFinished());
        QVERIFY(crr.isCanceled());
        QCOMPARE(cm->contactIds().size(), 1);
        QCOMPARE(cm->contactIds(), crr.contactIds());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!crr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(crr.start());
        if (!crr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            crr.waitForFinished();
            crr.setContactIds(cm->contactIds(dfil));
            temp.setId(QContactId());
            cm->saveContact(&temp);
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }
        crr.waitForFinished();
        QVERIFY(crr.isCanceled());
        QCOMPARE(cm->contactIds().size(), 1);
        QCOMPARE(cm->contactIds(), crr.contactIds());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

}

void tst_QContactAsync::contactSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactSaveRequest csr;
    QVERIFY(csr.type() == QContactAbstractRequest::ContactSaveRequest);

    // initial state - not started, no manager.
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.start());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());

    // save a new contact
    int originalCount = cm->contactIds().size();
    QContact testContact;
    QContactName nameDetail;
    nameDetail.setFirstName("Test Contact");
    testContact.saveDetail(&nameDetail);
    QList<QContact> saveList;
    saveList << testContact;
    csr.setManager(cm.data());
    QCOMPARE(csr.manager(), cm.data());
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    csr.setContact(testContact);
    QCOMPARE(csr.contacts(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());

    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    //QVERIFY(csr.isFinished() || !csr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContact> expected = csr.contacts();
    QCOMPARE(expected.size(), 1);
    QList<QContact> result;
    result << cm->contact(expected.first().id());
    //some backends add extra fields, so this doesn't work:
    //QCOMPARE(result, expected);
    // XXX: really, we should use isSuperset() from tst_QContactManager, but this will do for now:
    QVERIFY(result.first().detail<QContactName>() == nameDetail);
    QCOMPARE(cm->contactIds().size(), originalCount + 1);

    // update a previously saved contact
    QContactPhoneNumber phn;
    phn.setNumber("12345678");
    testContact = result.first();
    testContact.saveDetail(&phn);
    saveList.clear();
    saveList << testContact;
    csr.setContacts(saveList);
    QCOMPARE(csr.contacts(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());

    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    //QVERIFY(csr.isFinished() || !csr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(csr.waitForFinished());

    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    expected = csr.contacts();
    result.clear();
    result << cm->contact(expected.first().id());
    //QVERIFY(compareContactLists(result, expected));

    //here we can't compare the whole contact details, testContact would be updated by async call because we just use QThreadSignalSpy to receive signals.
    //QVERIFY(containsIgnoringTimestamps(result, testContact));
    // XXX: really, we should use isSuperset() from tst_QContactManager, but this will do for now:
    QVERIFY(result.first().detail<QContactPhoneNumber>().number() == phn.number());
    
    QCOMPARE(cm->contactIds().size(), originalCount + 1);

    // cancelling
    QContact temp = testContact;
    QContactUrl url;
    url.setUrl("should not get saved");
    temp.saveDetail(&url);
    saveList.clear();
    saveList << temp;
    csr.setContacts(saveList);

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!csr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(csr.start());
        if (!csr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            csr.waitForFinished();
            saveList = csr.contacts();
            if (cm->contactIds().size() > (originalCount + 1) && !cm->removeContact(saveList.at(0).id())) {
                QSKIP("Unable to remove saved contact to test cancellation of contact save request");
            }
            saveList.clear();
            saveList << temp;
            csr.setContacts(saveList);
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(csr.waitForFinished());
        QVERIFY(csr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        // verify that the changes were not saved
        expected.clear();
        QList<QContactId> allContacts = cm->contactIds();
        for (int i = 0; i < allContacts.size(); i++) {
            expected.append(cm->contact(allContacts.at(i)));
        }
        QVERIFY(!expected.contains(temp));
        QCOMPARE(cm->contactIds().size(), originalCount + 1);
        break;
    }
    // restart, and wait for progress after cancel.

    while (true) {
        QVERIFY(!csr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(csr.start());
        if (!csr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            csr.waitForFinished();
            saveList = csr.contacts();
            if (cm->contactIds().size() > (originalCount + 1) && !cm->removeContact(saveList.at(0).id())) {
                QSKIP("Unable to remove saved contact to test cancellation of contact save request");
            }
            saveList.clear();
            saveList << temp;
            csr.setContacts(saveList);
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }
        csr.waitForFinished(); // now wait until finished (if it hasn't already).
        QVERIFY(csr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        // verify that the changes were not saved
        expected.clear();
        QList<QContactId> allContacts = cm->contactIds();
        for (int i = 0; i < allContacts.size(); i++) {
            expected.append(cm->contact(allContacts.at(i)));
        }
        QVERIFY(!expected.contains(temp));
        QCOMPARE(cm->contactIds().size(), originalCount + 1);
        break;
    }
}

void tst_QContactAsync::contactPartialSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QList<QContact> contacts(cm->contacts());
    QList<QContact> originalContacts(contacts);
    QCOMPARE(contacts.count(), 3);

    QContactId aId = contacts[0].id();
    QContactId bId = contacts[1].id();

    // Test 1: saving a contact with a changed detail masked out does nothing
    QContactPhoneNumber phn(contacts[0].detail<QContactPhoneNumber>());
    phn.setNumber("new number");
    contacts[0].saveDetail(&phn);

    QContactSaveRequest csr;
    csr.setManager(cm.data());
    csr.setContacts(contacts);
    QList<QContactDetail::DetailType> typeMasks;
    typeMasks << QContactDetail::TypeEmailAddress;
    csr.setTypeMask(typeMasks);
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QVERIFY(csr.start());
    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts[0] = cm->contact(aId);
    QCOMPARE(contacts[0].detail<QContactPhoneNumber>().number(),
            originalContacts[0].detail<QContactPhoneNumber>().number());

    // Test 2: saving a contact with a changed detail in the mask changes it
    QContactEmailAddress email;
    email.setEmailAddress("me@example.com");
    contacts[1].saveDetail(&email);
    csr.setContacts(contacts);
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts[1] = cm->contact(bId);
    QCOMPARE(contacts[1].detail<QContactEmailAddress>().emailAddress(), QString("me@example.com"));

    // 3) Remove an email address and a phone number
    QCOMPARE(contacts[1].details<QContactPhoneNumber>().count(), 1);
    QCOMPARE(contacts[1].details<QContactEmailAddress>().count(), 1);
    // Next line added because otherwise the removeDetail fails because the email details key
    // changes in: contacts[1] = cm->contact(bId); and it does not match email variables key.
    email = contacts[1].detail<QContactEmailAddress>();
    QVERIFY(contacts[1].removeDetail(&email));
    phn = contacts[1].detail<QContactPhoneNumber>();
    QVERIFY(contacts[1].removeDetail(&phn));
    QVERIFY(contacts[1].details<QContactEmailAddress>().count() == 0);
    QVERIFY(contacts[1].details<QContactPhoneNumber>().count() == 0);
    csr.setContacts(contacts);
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts[1] = cm->contact(bId);
    QCOMPARE(contacts[1].details<QContactEmailAddress>().count(), 0);
    QCOMPARE(contacts[1].details<QContactPhoneNumber>().count(), 1);

    // 4 - New contact, no details in the mask
    QContact newContact;
    newContact.saveDetail(&email);
    newContact.saveDetail(&phn);
    contacts.append(newContact);
    csr.setContacts(contacts);
    typeMasks.clear();
    typeMasks << QContactDetail::TypeOnlineAccount;
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts = csr.contacts();
    QCOMPARE(contacts.size()-1, 3);  // Just check that we are dealing with the contact at index 3
    QContactId dId = contacts[3].id();
    contacts[3] = cm->contact(dId);
    QVERIFY(contacts[3].details<QContactEmailAddress>().count() == 0); // not saved
    QVERIFY(contacts[3].details<QContactPhoneNumber>().count() == 0); // not saved

    // 5 - New contact, some details in the mask
    QVERIFY(newContact.id().isNull());
    QVERIFY(newContact.details<QContactEmailAddress>().count() == 1);
    QVERIFY(newContact.details<QContactPhoneNumber>().count() == 1);
    contacts.append(newContact);
    csr.setContacts(contacts);
    typeMasks.clear();
    typeMasks << QContactDetail::TypePhoneNumber;
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts = csr.contacts();
    QCOMPARE(contacts.size()-1, 4);  // Just check that we are dealing with the contact at index 4
    QContactId eId = contacts[4].id();
    contacts[4] = cm->contact(eId);
    QCOMPARE(contacts[4].details<QContactEmailAddress>().count(), 0); // not saved
    QCOMPARE(contacts[4].details<QContactPhoneNumber>().count(), 1); // saved

    // 6) Have a bad manager uri in the middle
    QContactId badId = QContactIdMock::createId("BadManager", 0);
    contacts[3].setId(badId);
    csr.setContacts(contacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() == QContactManager::NoError);//no save error in this partial save
    QMap<int, QContactManager::Error> errorMap(csr.errorMap());
    QCOMPARE(errorMap.count(), 1);//one error in error map, related to the fetch phase of this partial save
    QCOMPARE(errorMap[3], QContactManager::DoesNotExistError);

    // 7) Have a non existing contact in the middle
    badId = QContactIdMock::createId(cm->managerName(), 0);
    contacts[3].setId(badId);
    csr.setContacts(contacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() == QContactManager::NoError);//no save error in this partial save
    errorMap = csr.errorMap();
    QCOMPARE(errorMap.count(), 1);//one error in error map, related to the fetch phase of this partial save
    QCOMPARE(errorMap[3], QContactManager::DoesNotExistError);

    // 8) A list entirely of new contacts, with no details in the mask
    QList<QContact> contacts2;
    QVERIFY(newContact.id().isNull());
    QVERIFY(newContact.details<QContactEmailAddress>().count() == 1);
    QVERIFY(newContact.details<QContactPhoneNumber>().count() == 1);
    contacts2.append(newContact);
    csr.setContacts(contacts2);
    typeMasks.clear();
    typeMasks << QContactDetail::TypeOrganization;
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts2 = csr.contacts();
    QCOMPARE(contacts2.size(), 1);
    contacts2[0] = cm->contact(contacts2[0].id());
    QCOMPARE(contacts2[0].details<QContactEmailAddress>().count(), 0); // not saved
    QCOMPARE(contacts2[0].details<QContactPhoneNumber>().count(), 0); // saved

    // 9) A list entirely of new contacts, with some details in the mask
    contacts2.clear();
    QVERIFY(newContact.id().isNull());
    QVERIFY(newContact.details<QContactEmailAddress>().count() == 1);
    QVERIFY(newContact.details<QContactPhoneNumber>().count() == 1);
    contacts2.append(newContact);
    csr.setContacts(contacts2);
    typeMasks.clear();
    typeMasks << QContactDetail::TypePhoneNumber;
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());
    contacts2 = csr.contacts();
    QCOMPARE(contacts2.size(), 1);
    contacts2[0] = cm->contact(contacts2[0].id());
    QCOMPARE(contacts2[0].details<QContactEmailAddress>().count(), 0); // not saved
    QCOMPARE(contacts2[0].details<QContactPhoneNumber>().count(), 1); // saved

    // 10) A list entirely of new contacts
    contacts2.clear();
    QVERIFY(newContact.id().isNull());
    QVERIFY(newContact.details<QContactEmailAddress>().count() == 1);
    QVERIFY(newContact.details<QContactPhoneNumber>().count() == 1);
    contacts2.append(newContact);
    contacts2.append(newContact);
    contacts2[0].setId(badId);
    csr.setContacts(contacts2);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() == QContactManager::NoError);//no save error in this partial save
    errorMap = csr.errorMap();
    QCOMPARE(errorMap.count(), 1);//one error in error map, related to the fetch phase of this partial save
    QCOMPARE(errorMap[0], QContactManager::DoesNotExistError);
}

void tst_QContactAsync::contactPartialSaveAsync()
{
    QFETCH(QString, uri);
    QContactManager* cm = QContactManager::fromUri(uri);

    QList<QContact> contacts;

    int numContacts = 10;
    // add contacts
    for (int i = 0; i < numContacts; i++) {
        QContact c;
        QContactName name;
        name.setFirstName("John");
        name.setMiddleName(QString::number(i));
        name.setLastName("Doe");
        QContactPhoneNumber phone;
        QString number = "555-100"+QString::number(i);
        phone.setNumber(number);
        c.saveDetail(&name);
        c.saveDetail(&phone);
        contacts.append(c);
    }

    QContactSaveRequest *saveRequest = new QContactSaveRequest();
    saveRequest->setManager(cm);
    saveRequest->setContacts(contacts);
    saveRequest->start();
    saveRequest->waitForFinished(20000);
    QVERIFY(saveRequest->isFinished());
    QCOMPARE(saveRequest->contacts().count(), numContacts);
    delete saveRequest;
    qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");

    saveRequest = new QContactSaveRequest();
    saveRequest->setManager(cm);
    saveRequest->setContacts(contacts);
    QList<QContactDetail::DetailType> typeMasks;
    typeMasks << QContactDetail::TypeTag;
    saveRequest->setTypeMask(typeMasks);
    saveRequest->start();
    QTest::qWait(1000);
    QVERIFY(saveRequest->waitForFinished());
    QVERIFY(saveRequest->isFinished());
}

void tst_QContactAsync::relationshipFetch()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    if (!cm->hasFeature(QContactManager::Relationships)) {
       QSKIP("This contact manager does not support relationships!");
    }
    
    if (cm->managerName() == "symbian") {
        QSKIP("This contact manager does not support the required relationship types for this test to pass!");
    }
    
    QContactRelationshipFetchRequest rfr;
    QVERIFY(rfr.type() == QContactAbstractRequest::RelationshipFetchRequest);

    // initial state - not started, no manager.
    QVERIFY(!rfr.isActive());
    QVERIFY(!rfr.isFinished());
    QVERIFY(!rfr.start());
    QVERIFY(!rfr.cancel());
    QVERIFY(!rfr.waitForFinished());

    // "all relationships" retrieval
    rfr.setManager(cm.data());
    QCOMPARE(rfr.manager(), cm.data());
    QVERIFY(!rfr.isActive());
    QVERIFY(!rfr.isFinished());
    QVERIFY(!rfr.cancel());
    QVERIFY(!rfr.waitForFinished());
    qRegisterMetaType<QContactRelationshipFetchRequest*>("QContactRelationshipFetchRequest*");
    QThreadSignalSpy spy(&rfr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());

    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContactRelationship> rels = cm->relationships();
    QList<QContactRelationship> result = rfr.relationships();
    QCOMPARE(rels, result);

    // specific relationship type retrieval
    rfr.setRelationshipType(QContactRelationship::HasManager());
    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    rels = cm->relationships(QContactRelationship::HasManager());
    result = rfr.relationships();
    QCOMPARE(rels, result);

    // specific source contact retrieval
    rfr.setRelationshipType(QString());
    QList<QContactId> contacts = cm->contactIds();
    QContact aContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Aaron")) {
            aContact = curr;
            break;
        }
    }
    rfr.setFirst(aContact);
    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    rels = cm->relationships(aContact, QContactRelationship::First);
    result = rfr.relationships();
    QCOMPARE(rels, result);

    // specific participant retrieval #1 - destination participant
    rfr.setFirst(QContact());
    contacts = cm->contactIds();
    QContact bContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Bob")) {
            bContact = curr;
            break;
        }
    }
    rfr.setSecond(bContact);
    
    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // retrieve rels where second = id of B, and ensure that we get the same results
    rels = cm->relationships(bContact, QContactRelationship::Second);
    result = rfr.relationships();
    QCOMPARE(rels, result);

    // specific participant retrieval #2 - source participant
    rfr.setFirst(QContact());
    contacts = cm->contactIds();
    QContact cContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Borris")) {
            cContact = curr;
            break;
        }
    }
    rfr.setSecond(cContact);
    
    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // retrieve rels where first = id of C and compare the results
    rfr.setFirst(cContact);
    rfr.setSecond(QContact());
    QVERIFY(rfr.start());
    QVERIFY(rfr.waitForFinished());
    result = rfr.relationships();
    rels = cm->relationships(cContact, QContactRelationship::First);
    QCOMPARE(rels, result);

    // cancelling
    rfr.setRelationshipType(QString());

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!rfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(rfr.start());
        if (!rfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            rfr.waitForFinished();
            rfr.setRelationshipType(QString());
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(rfr.waitForFinished());
        QVERIFY(rfr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!rfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(rfr.start());
        if (!rfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            rfr.waitForFinished();
            rfr.setRelationshipType(QString());
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }
        rfr.waitForFinished();
        QVERIFY(rfr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }
}

void tst_QContactAsync::relationshipRemove()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    
    if (!cm->hasFeature(QContactManager::Relationships)) {
       QSKIP("This contact manager does not support relationships!");
    }
    
    if (cm->managerName() == "symbian") {
        QSKIP("This contact manager does not support the required relationship types for this test to pass!");
    }
    
    QContactRelationshipRemoveRequest rrr;
    QVERIFY(rrr.type() == QContactAbstractRequest::RelationshipRemoveRequest);

    // initial state - not started, no manager.
    QVERIFY(!rrr.isActive());
    QVERIFY(!rrr.isFinished());
    QVERIFY(!rrr.start());
    QVERIFY(!rrr.cancel());
    QVERIFY(!rrr.waitForFinished());

    QList<QContactId> contacts = cm->contactIds();
    QContact aContact, bContact, cContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Aaron")) {
            aContact = curr;
            continue;
        }
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Bob")) {
            bContact = curr;
            continue;
        }
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Borris")) {
            cContact = curr;
            continue;
        }
    }

    // specific source, destination and type removal
    QList<QContactRelationship> relationships;
    QContactRelationship r;
    r.setFirst(aContact);
    r.setSecond(cContact);
    r.setRelationshipType(QContactRelationship::HasAssistant());
    relationships.push_back(r);

    rrr.setRelationships(relationships);
    rrr.setManager(cm.data());
    qRegisterMetaType<QContactRelationshipRemoveRequest*>("QContactRelationshipRemoveRequest*");
    QThreadSignalSpy spy(&rrr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QCOMPARE(rrr.manager(), cm.data());
    QVERIFY(!rrr.isActive());
    QVERIFY(!rrr.isFinished());
    QVERIFY(!rrr.cancel());
    QVERIFY(!rrr.waitForFinished());
    
    QVERIFY(!rrr.cancel()); // not started
    QVERIFY(rrr.start());

    QVERIFY((rrr.isActive() && rrr.state() == QContactAbstractRequest::ActiveState) || rrr.isFinished());
    //QVERIFY(rrr.isFinished() || !rrr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rrr.waitForFinished());
    QVERIFY(rrr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();
    QCOMPARE(cm->relationships(QContactRelationship::HasAssistant(), cContact, QContactRelationship::Second).size(), 1);

    // remove (asynchronously) a nonexistent relationship - should fail.
    r.setFirst(cContact);
    r.setSecond(aContact);
    r.setRelationshipType(QContactRelationship::HasManager());
    relationships.clear();
    relationships.push_back(r);
    rrr.setRelationship(r);
    QVERIFY(rrr.relationships() == relationships);
    rrr.setManager(cm.data());
    QVERIFY(!rrr.cancel()); // not started
    QVERIFY(rrr.start());

    QVERIFY((rrr.isActive() && rrr.state() == QContactAbstractRequest::ActiveState) || rrr.isFinished());
    //QVERIFY(rrr.isFinished() || !rrr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rrr.waitForFinished());
    QVERIFY(rrr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QCOMPARE(cm->relationships(QContactRelationship::HasManager(), cContact, QContactRelationship::First).size(), 0);
//    QCOMPARE(rrr.error(), QContactManager::DoesNotExistError);

    // cancelling
    r.setFirst(cContact);
    r.setSecond(QContact());
    relationships.clear();
    relationships.push_back(r);

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!rrr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(rrr.start());
        if (!rrr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            rrr.waitForFinished();
            rrr.setRelationships(relationships);
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(rrr.waitForFinished());
        QVERIFY(rrr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        QVERIFY(cm->relationships(cContact).size() != 0); // didn't remove them.
        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!rrr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(rrr.start());
        if (!rrr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            rrr.waitForFinished();
            rrr.setRelationships(relationships);
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }
        rrr.waitForFinished();
        QVERIFY(rrr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        QVERIFY(cm->relationships(cContact).size() != 0); // didn't remove them.
        break;
    }
}

void tst_QContactAsync::relationshipSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    
    if (!cm->hasFeature(QContactManager::Relationships)) {
       QSKIP("This contact manager does not support relationships!");
    }
    
    if (cm->managerName() == "symbian") {
        QSKIP("This contact manager does not support the required relationship types for this test to pass!");
    }    
    
    QContactRelationshipSaveRequest rsr;
    QVERIFY(rsr.type() == QContactAbstractRequest::RelationshipSaveRequest);

    // initial state - not started, no manager.
    QVERIFY(!rsr.isActive());
    QVERIFY(!rsr.isFinished());
    QVERIFY(!rsr.start());
    QVERIFY(!rsr.cancel());
    QVERIFY(!rsr.waitForFinished());

    QList<QContactId> contacts = cm->contactIds();
    QContact cContact, aContact, bContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Borris")) {
            cContact = curr;
        } else if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Bob")) {
            bContact = curr;
        } else if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Aaron")) {
            aContact = curr;
        }
    }

    // save a new relationship
    int originalCount = cm->relationships(aContact).size();
    QContactRelationship testRel;
    testRel.setFirst(aContact);
    testRel.setRelationshipType(QContactRelationship::HasSpouse());
    testRel.setSecond(bContact);
    QList<QContactRelationship> saveList;
    saveList << testRel;
    rsr.setManager(cm.data());
    QCOMPARE(rsr.manager(), cm.data());
    QVERIFY(!rsr.isActive());
    QVERIFY(!rsr.isFinished());
    QVERIFY(!rsr.cancel());
    QVERIFY(!rsr.waitForFinished());
    qRegisterMetaType<QContactRelationshipSaveRequest*>("QContactRelationshipSaveRequest*");
    QThreadSignalSpy spy(&rsr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    rsr.setRelationship(testRel);
    QCOMPARE(rsr.relationships(), saveList);
    QVERIFY(!rsr.cancel()); // not started
    QVERIFY(rsr.start());

    QVERIFY((rsr.isActive() && rsr.state() == QContactAbstractRequest::ActiveState) || rsr.isFinished());
    //QVERIFY(rsr.isFinished() || !rsr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rsr.waitForFinished());
    QVERIFY(rsr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContactRelationship> expected = cm->relationships(QContactRelationship::HasSpouse(), aContact, QContactRelationship::First);
    QList<QContactRelationship> result = rsr.relationships();
    QCOMPARE(expected, result);
    QVERIFY(result.contains(testRel));
    QCOMPARE(cm->relationships(aContact).size(), originalCount + 1); // should be one extra

    // save a new relationship
    testRel.setSecond(cContact);
    saveList.clear();
    saveList << testRel;
    rsr.setRelationships(saveList);
    QCOMPARE(rsr.relationships(), saveList);
    QVERIFY(!rsr.cancel()); // not started
    QVERIFY(rsr.start());

    QVERIFY((rsr.isActive() && rsr.state() == QContactAbstractRequest::ActiveState) || rsr.isFinished());
    //QVERIFY(rsr.isFinished() || !rsr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rsr.waitForFinished());
    QVERIFY(rsr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    expected.clear();
    expected = cm->relationships(QContactRelationship::HasSpouse(), aContact, QContactRelationship::First);
    result = rsr.relationships();
    QCOMPARE(result, QList<QContactRelationship>() << testRel);
    QVERIFY(expected.contains(testRel));
    QCOMPARE(cm->relationships(aContact).size(), originalCount + 2); // should now be two extra

    // cancelling
    testRel.setSecond(aContact); // shouldn't get saved (circular anyway)
    saveList.clear();
    saveList << testRel;
    rsr.setRelationships(saveList);

    if (cm->managerName() == "jsondb")
        QSKIP("JSONDB backend does not support request cancelling, skipping...");

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!rsr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(rsr.start());
        if (!rsr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            rsr.waitForFinished();
            saveList.clear();
            saveList << testRel;
            rsr.setRelationships(saveList);
            cm->removeRelationship(testRel); // probably shouldn't have been saved anyway (circular)
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }

        // if we get here, then we are cancelling the request.
        QVERIFY(rsr.waitForFinished());
        QVERIFY(rsr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        // verify that the changes were not saved
        QList<QContactRelationship> aRels = cm->relationships(aContact, QContactRelationship::First);
        QVERIFY(!aRels.contains(testRel));
        QCOMPARE(cm->relationships(aContact).size(), originalCount + 2); // should still only be two extra

        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!rsr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS();
        QVERIFY(rsr.start());
        if (!rsr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            rsr.waitForFinished();
            saveList.clear();
            saveList << testRel;
            rsr.setRelationships(saveList);
            cm->removeRelationship(testRel); // probably shouldn't have been saved anyway (circular)
            bailoutCount -= 1;
            if (!bailoutCount) {
//                qWarning("Unable to test cancelling due to thread scheduling!");
                bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT;
                break;
            }
            spy.clear();
            continue;
        }
        rsr.waitForFinished();
        QVERIFY(rsr.isCanceled());
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();

        // verify that the changes were not saved
        QList<QContactRelationship> aRels = cm->relationships(aContact, QContactRelationship::First);
        QVERIFY(!aRels.contains(testRel));
        QCOMPARE(cm->relationships(aContact).size(), originalCount + 2); // should still only be two extra

        break;
    }
}

void tst_QContactAsync::maliciousManager()
{
    // use the invalid manager: passes all requests through to base class
    QContactManager cm("invalid");
    QContactFilter fil; // matches everything
    QContactFetchRequest cfr;
    cfr.setFilter(fil);
    cfr.setManager(&cm);
    QVERIFY(!cfr.start());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());
    QVERIFY(!cfr.start());

    // ensure that the base class implementation of requestDestroyed() is called
    QContactFetchRequest *destroyedRequest = new QContactFetchRequest;
    destroyedRequest->setManager(&cm);
    destroyedRequest->setFilter(fil);
    QVERIFY(!destroyedRequest->start());
    delete destroyedRequest;

    // now use a malicious manager that deliberately calls
    // things in a different thread
    QContactManager mcm("maliciousplugin");
    QCOMPARE(mcm.managerName(), QString("maliciousplugin"));
    QList<QContact> emptyCList;
    QList<QContactId> emptyIList;
    QStringList emptyDNList;
    cfr.setFilter(fil);
    cfr.setManager(&mcm);
    QVERIFY(cfr.start());

    QContactIdFetchRequest cifr;
    cifr.setFilter(fil);
    cifr.setManager(&mcm);
    QVERIFY(cifr.start());

    QContactRemoveRequest crr;
    crr.setContactIds(mcm.contactIds(fil));
    crr.setManager(&mcm);
    QVERIFY(crr.start());

    QContactSaveRequest csr;
    csr.setContacts(emptyCList);
    csr.setManager(&mcm);
    QVERIFY(csr.start());

}

void tst_QContactAsync::testQuickDestruction()
{
    QFETCH(QString, uri);

    // in this test, we create a manager, fire off a request, and delete the manager, all in quick succession
    // this is to test for segfaults etc.
    for (int i = 0; i < 10; i++) {
        QContactFetchRequest cfr;
        QContactManager *cm = prepareModel(uri);
        cfr.setManager(cm);
        cfr.start();
        delete cm;
    }
    // in this test, we create a manager, fire off a request, delete the request, then delete the manager, all in quick succession
    // this is to test for segfaults, etc.
    for (int i = 0; i < 10; i++) {
        QContactFetchRequest *cfr = new QContactFetchRequest;
        QContactManager *cm = prepareModel(uri);
        cfr->setManager(cm);
        cfr->start();
        delete cfr;
        delete cm;
    }
    // in this test, we create a manager, fire off a request, delete the manager, then delete the request, all in quick succession
    // this is to test for segfaults, etc.
    for (int i = 0; i < 10; i++) {
        QContactFetchRequest *cfr = new QContactFetchRequest;
        QContactManager *cm = prepareModel(uri);
        cfr->setManager(cm);
        cfr->start();
        delete cm;
        delete cfr;
    }
    // in this test, we create a manager, fire off a request, and delete the request, all in quick succession
    // this is to test for segfaults, etc.
    QContactManager *cm = prepareModel(uri);
    for (int i = 0; i < 10; i++) {
        QContactFetchRequest *cfr = new QContactFetchRequest;
        cfr->setManager(cm);
        cfr->start();
        delete cfr;
    }
    delete cm;
}

void tst_QContactAsync::threadDelivery()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    m_mainThreadId = cm->thread()->currentThreadId();
    m_resultsAvailableSlotThreadId = m_mainThreadId;

    // now perform a fetch request and check that the progress is delivered to the correct thread.
    QContactFetchRequest *req = new QContactFetchRequest;
    req->setManager(cm.data());
    connect(req, SIGNAL(resultsAvailable()), this, SLOT(resultsAvailableReceived()));
    req->start();

    int totalWaitTime = 0;
    QTest::qWait(1); // force it to process events at least once.
    while (req->state() != QContactAbstractRequest::FinishedState) {
        // ensure that the progress signal was delivered to the main thread.
        QCOMPARE(m_mainThreadId, m_resultsAvailableSlotThreadId);

        QTest::qWait(5); // spin until done
        totalWaitTime += 5;

        // break after 30 seconds.
        if (totalWaitTime > 30000) {
            delete req;
            QSKIP("Asynchronous request not complete after 30 seconds!");
        }
    }

    // ensure that the progress signal was delivered to the main thread.
    QCOMPARE(m_mainThreadId, m_resultsAvailableSlotThreadId);
    delete req;
}

void tst_QContactAsync::resultsAvailableReceived()
{
    QContactFetchRequest *req = qobject_cast<QContactFetchRequest *>(QObject::sender());
    if (req)
        m_resultsAvailableSlotThreadId = req->thread()->currentThreadId();
    else
        qWarning() << "resultsAvailableReceived() : request deleted; unable to set thread id!";
}

void tst_QContactAsync::addManagers(QStringList stringlist)
{
    QTest::addColumn<QString>("uri");

    // retrieve the list of available managers
    QStringList managers = QContactManager::availableManagers();

    // remove ones that we know will not pass
    if (!stringlist.contains("invalid"))
        managers.removeAll("invalid");
    if (!stringlist.contains("maliciousplugin"))
        managers.removeAll("maliciousplugin");
    if (!stringlist.contains("testdummy"))
        managers.removeAll("testdummy");
    if (!stringlist.contains("symbiansim"))
        managers.removeAll("symbiansim"); // SIM backend does not support all the required details for tests to pass.
    if (!stringlist.contains("social"))
        managers.removeAll("social");
    if (!stringlist.contains("simcard"))
        managers.removeAll("simcard");
    if (!stringlist.contains("com.nokia.messaging.contacts.engines.mail.contactslookup"))
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

QContactManager* tst_QContactAsync::prepareModel(const QString& managerUri)
{
    QContactManager* cm = QContactManager::fromUri(managerUri);

    // XXX TODO: ensure that this is the case:
    // there should be no contacts in the database.
    QList<QContactId> toRemove = cm->contactIds();
    foreach (const QContactId& removeId, toRemove)
        cm->removeContact(removeId);

    QContact a, b, c;
    QContactName aNameDetail;
    aNameDetail.setFirstName("Aaron");
    aNameDetail.setLastName("Aaronson");
    a.saveDetail(&aNameDetail);
    QContactName bNameDetail;
    bNameDetail.setFirstName("Bob");
    bNameDetail.setLastName("Aaronsen");
    b.saveDetail(&bNameDetail);
    QContactName cNameDetail;
    cNameDetail.setFirstName("Borris");
    cNameDetail.setLastName("Aaronsun");
    c.saveDetail(&cNameDetail);

    QContactPhoneNumber phn;
    phn.setNumber("0123");
    c.saveDetail(&phn);
    phn.setNumber("3456");
    b.saveDetail(&phn);
    phn.setNumber("6789");
    a.saveDetail(&phn);

    QContactUrl url;
    url.setUrl("http://test.nokia.com");
    a.saveDetail(&url);

    cm->saveContact(&a);
    cm->saveContact(&b);
    cm->saveContact(&c);
    
    if (!cm->hasFeature(QContactManager::Relationships)) {
        return cm;
    }
    
    if (cm->managerName() == "symbian") {
        // Symbian backend does not support other relationships than HasMember (which is same as groups)
        return cm;
    }

    QContactRelationship arb;
    arb.setFirst(a);
    arb.setSecond(b);
    arb.setRelationshipType(QContactRelationship::HasManager());
    cm->saveRelationship(&arb);

    QContactRelationship brc;
    brc.setFirst(b);
    brc.setSecond(c);
    brc.setRelationshipType(QContactRelationship::HasAssistant());
    cm->saveRelationship(&brc);

    QContactRelationship cra;
    cra.setFirst(c);
    cra.setSecond(a);
    cra.setRelationshipType(QContactRelationship::HasSpouse());
    cm->saveRelationship(&cra);

    QContactRelationship arc;
    arc.setFirst(a);
    arc.setSecond(c);
    arc.setRelationshipType(QContactRelationship::HasAssistant());
    cm->saveRelationship(&arc);

    QContactRelationship crb;
    crb.setFirst(c);
    crb.setSecond(b);
    crb.setRelationshipType(QContactRelationship::IsSameAs());
    cm->saveRelationship(&crb);

    return cm;

    // TODO: cleanup once test is complete
}

QTEST_MAIN(tst_QContactAsync)
#include "tst_qcontactasync.moc"
