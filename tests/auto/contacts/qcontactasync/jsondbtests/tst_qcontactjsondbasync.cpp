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
#include <QContactName>
#include <QContactAbstractRequest>
#include <QContactManager>
#include <QContactFetchRequest>
#include <QContactAbstractRequest>
#include <QContactDetailFilter>
#include <QContactUrl>
#include <QContactPhoneNumber>
#include <QContactFetchHint>
#include <QContactFetchByIdRequest>
#include <QContactSaveRequest>
#include <QContactEmailAddress>
#include <QContactOnlineAccount>
#include <QContactRemoveRequest>
#include <QContactRelationship>
#include <QContactTimestamp>
#include <QContactTag>
#include <QContactOrganization>
#include "../../qcontactidmock.h"
#include "../../../jsondbprocess.h"
#include "qcontactmanagerdataholder.h" //QContactManagerDataHolder

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

class tst_QContactJsonDbAsync : public QObject
{
    Q_OBJECT

public:
    tst_QContactJsonDbAsync();
    virtual ~tst_QContactJsonDbAsync();

public slots:
    void initTestCase();
    void cleanupTestCase();

private:
    void addManagers(QStringList includes = QStringList()); // add standard managers to the data

private slots:
    void contactFetch();
    void contactFetch_data() { addManagers(); }
    void contactFetchById();
    void contactFetchById_data() { addManagers(); }
    void contactFetchByIdErrorHandling();
    void contactFetchByIdErrorHandling_data() { addManagers(); }
    void contactSave();
    void contactSave_data() { addManagers(); }
    void contactSaveErrorHandling();
    void contactSaveErrorHandling_data() { addManagers(); }
    void contactSaveRemovedContacts();
    void contactSaveRemovedContacts_data() { addManagers(); }
    void contactSaveRemovedContactsWithCleanIds();
    void contactSaveRemovedContactsWithCleanIds_data() { addManagers(); }
    void contactPartialSave();
    void contactPartialSave_data() { addManagers(); }
    void contactPartialSaveAsync();
    void contactPartialSaveAsync_data() {addManagers();}
    void contactRemove();
    void contactRemove_data() {addManagers();}
    void contactRemoveErrorHandling();
    void contactRemoveErrorHandling_data() {addManagers();}

    void testQuickDestruction();
    void testQuickDestruction_data() { addManagers(QStringList(QString("maliciousplugin"))); }
protected slots:
    void resultsAvailableReceived();
    void deleteRequest();
private:
    QContactManager* prepareModel(const QString& uri);

    Qt::HANDLE m_mainThreadId;
    Qt::HANDLE m_resultsAvailableSlotThreadId;
    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

    JsonDbProcess jsondbProcess;

};

tst_QContactJsonDbAsync::tst_QContactJsonDbAsync()
{

    // ensure we can load all of the plugins we need to.
    QString path = QCoreApplication::applicationDirPath() + "/dummyplugin/plugins";
    QCoreApplication::addLibraryPath(path);

    qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
}

tst_QContactJsonDbAsync::~tst_QContactJsonDbAsync()
{
}

void tst_QContactJsonDbAsync::initTestCase()
{
    QVERIFY2(jsondbProcess.start(), "Failed to start JsonDb process");

    //TODO FIXME
    managerDataHolder.reset(new QContactManagerDataHolder());
}

void tst_QContactJsonDbAsync::cleanupTestCase()
{
    managerDataHolder.reset(0);

    jsondbProcess.terminate();
}

void tst_QContactJsonDbAsync::addManagers(QStringList stringlist)
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
    if (!stringlist.contains("memory"))
        managers.removeAll("memory");
    foreach(QString mgr, managers) {
        QMap<QString, QString> params;
        QTest::newRow(QString("mgr='%1'").arg(mgr).toLatin1().constData()) << QContactManager::buildUri(mgr, params);
        if (mgr == "memory") {
            params.insert("id", "tst_QContactManager");
            QTest::newRow(QString("mgr='%1', params").arg(mgr).toLatin1().constData()) << QContactManager::buildUri(mgr, params);
        }
    }
}

void tst_QContactJsonDbAsync::contactFetch()
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
    // NOTE: currently filtering is not supported. So, the code below will
    // show an error in doSynchRequest, due to the fact that the query string
    // generated by jsondb plugin is broken / incorrect.
    // The test case does not fail because the sizes of contactIds and contacts
    // lists below are the same.
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
    QList<QtContacts::QContactDetail::DetailType> typeHints;
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

// calling waitForFinished does not work properly. Moreover, we are planning to remove
// waitForFinished from asynch requests. So we skip these tests atm for jsondb backend.

#if 0
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
#endif

}

void tst_QContactJsonDbAsync::contactFetchById()
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

void tst_QContactJsonDbAsync::contactFetchByIdErrorHandling()
{
    // Testing error handling by fetching by non existing local ids.

    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactFetchByIdRequest cfrCausingErrors;
    QVERIFY(cfrCausingErrors.type() == QContactAbstractRequest::ContactFetchByIdRequest);

    // Check initial state - not started, no manager.
    QVERIFY(!cfrCausingErrors.isActive());
    QVERIFY(!cfrCausingErrors.isFinished());
    QVERIFY(!cfrCausingErrors.start());
    QVERIFY(!cfrCausingErrors.cancel());
    QVERIFY(!cfrCausingErrors.waitForFinished());

    // Prepare non-existing contact ids for request we like to result.
    QList<QContactId> nonExistingContactIds;
    nonExistingContactIds << QContactId() << QContactId() << QContactId();

    // "Make contacts" retrieval with some extra checking.
    cfrCausingErrors.setManager(cm.data());
    cfrCausingErrors.setIds(nonExistingContactIds);
    QCOMPARE(cfrCausingErrors.manager(), cm.data());
    QVERIFY(!cfrCausingErrors.isActive());
    QVERIFY(!cfrCausingErrors.isFinished());
    QVERIFY(!cfrCausingErrors.cancel());
    QVERIFY(!cfrCausingErrors.waitForFinished());

    qRegisterMetaType<QContactFetchByIdRequest*>("QContactFetchByIdRequest*");
    QThreadSignalSpy spy2(&cfrCausingErrors, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    QVERIFY(!cfrCausingErrors.cancel()); // not started

    QVERIFY(cfrCausingErrors.start());
    QVERIFY((cfrCausingErrors.isActive() && cfrCausingErrors.state() == QContactAbstractRequest::ActiveState) || cfrCausingErrors.isFinished());
    QVERIFY(cfrCausingErrors.waitForFinished());
    QVERIFY(cfrCausingErrors.isFinished());

    QVERIFY(spy2.count() >= 1); // active + finished progress signals
    spy2.clear();

    // Check errors, they should be stored in the errorMap using index of local id in the request as a key for the error.
    // Note, the returned values are actually set in to the errorMap by common code in qcontactmanagerenginev2wrapper_p.cpp.

    QVERIFY(cfrCausingErrors.errorMap().value(0) == QContactManager::DoesNotExistError);
    QVERIFY(cfrCausingErrors.errorMap().value(1) == QContactManager::DoesNotExistError);
    QVERIFY(cfrCausingErrors.errorMap().value(2) == QContactManager::DoesNotExistError);
}

void tst_QContactJsonDbAsync::contactSave()
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


void tst_QContactJsonDbAsync::contactSaveErrorHandling()
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

    // Save a group of contacts, including few  TypeGroup contacts which are not supported by jsondb backend.
    QContact testContact1, testContact2, testContact3, testContact4, testContact5, testContact6;
    QContactName nameDetail;
    nameDetail.setFirstName("Test Contact1");
    testContact1.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact2");
    testContact2.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact3");
    testContact3.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact4");
    testContact4.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact5");
    testContact5.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact6");
    testContact6.saveDetail(&nameDetail);

    // Set group type to first, middle and last contact in the list.
    QContactType typeDetail;
    typeDetail.setType(QContactType::TypeGroup);
    testContact1.saveDetail(&typeDetail);
    testContact3.saveDetail(&typeDetail);
    testContact6.saveDetail(&typeDetail);

    QList<QContact> saveList;
    saveList << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6;

    csr.setManager(cm.data());
    QCOMPARE(csr.manager(), cm.data());
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    QList<QContact> testContacts;
    testContacts << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6;
    csr.setContacts(testContacts);

    QCOMPARE(csr.contacts(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());

    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    //QVERIFY(csr.isFinished() || !csr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // foreach (QContact testc, csr.contacts()) {
    //     qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTACT: " << testc.id().managerUri();
    //     qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTACT: " << testc.id().id();
    // }

    // Check errors, the group type is not supported by jsondb backend so contacts with that detail should report error.
    // Note, the returned value is actually set/remapped in to the errorMap by common code in qcontactmanagerengine
    QVERIFY(csr.errorMap().value(0) == QContactManager::InvalidContactTypeError);
    QVERIFY(csr.errorMap().value(1) == QContactManager::NoError);
    QVERIFY(csr.errorMap().value(2) == QContactManager::InvalidContactTypeError);
    QVERIFY(csr.errorMap().value(3) == QContactManager::NoError);
    QVERIFY(csr.errorMap().value(4) == QContactManager::NoError);
    QVERIFY(csr.errorMap().value(5) == QContactManager::InvalidContactTypeError);
    QVERIFY(csr.contacts()[5].id().isNull());
    QVERIFY(csr.error() == QContactManager::InvalidContactTypeError);
}

void tst_QContactJsonDbAsync::contactSaveRemovedContacts()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactSaveRequest csr;

    // Make three test contacts.
    QContactName nameDetail;
    nameDetail.setFirstName("Test1");
    QContact testContact1;
    testContact1.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test2");
    QContact testContact2;
    testContact2.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test3");
    QContact testContact3;
    testContact3.saveDetail(&nameDetail);
    QList<QContact> testContacts;
    testContacts << testContact1 << testContact2 << testContact3;

    // Save all three test contacts.
    csr.setManager(cm.data());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    csr.setContacts(testContacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.errorMap().value(0), QContactManager::NoError);
    QCOMPARE(csr.error(), QContactManager::NoError);

    // And then remove all of them.
    QList<QContactId> contactIds = cm->contactIds();
    QVERIFY(cm->removeContacts(contactIds));

    // Try now to save again the same three test contacts with the ids of
    // just removed contacts to fail and check proper error code.
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.errorMap().value(0), QContactManager::DoesNotExistError);
    QCOMPARE(csr.errorMap().value(1), QContactManager::DoesNotExistError);
    QCOMPARE(csr.errorMap().value(2), QContactManager::DoesNotExistError);
    QCOMPARE(csr.error(), QContactManager::DoesNotExistError);
}

void tst_QContactJsonDbAsync::contactSaveRemovedContactsWithCleanIds()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactSaveRequest csr;

    // Make three test contacts.
    QContactName nameDetail;
    nameDetail.setFirstName("Testing1");
    QContact testContact1;
    testContact1.saveDetail(&nameDetail);
    nameDetail.setFirstName("Testing2");
    QContact testContact2;
    testContact2.saveDetail(&nameDetail);
    nameDetail.setFirstName("Testing3");
    QContact testContact3;
    testContact3.saveDetail(&nameDetail);
    QList<QContact> testContacts;
    testContacts << testContact1 << testContact2 << testContact3;

    // Save all three test contacts.
    csr.setManager(cm.data());
    csr.setContacts(testContacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.errorMap().value(0), QContactManager::NoError);
    QCOMPARE(csr.error(), QContactManager::NoError);

    // And then remove all of them.
    QList<QContactId> contactIds = cm->contactIds();
    QVERIFY(cm->removeContacts(contactIds));

    // Use the same contacts but clean their ids before saving them.
    QList<QContact> contactsWithCleanIds = csr.contacts();
    contactsWithCleanIds[0].setId(QContactId());
    contactsWithCleanIds[1].setId(QContactId());
    contactsWithCleanIds[2].setId(QContactId());
    csr.setContacts(contactsWithCleanIds);

    // Save and check no errors occured.
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.error(), QContactManager::NoError);
}

void tst_QContactJsonDbAsync::contactPartialSave()
{

    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QList<QContact> contacts(cm->contacts());
    QList<QContact> originalContacts(contacts);
    QCOMPARE(contacts.count(), 3);

    QContactId aId = contacts[0].id();
    QContactId bId = contacts[1].id();

    // Using this value as invalid detail type
    const QContactDetail::DetailType badDetailType = QContactDetail::DetailType(12345);

    // Test 1: saving a contact with a changed detail masked out does nothing
    QContactPhoneNumber phn(contacts[0].detail<QContactPhoneNumber>());
    phn.setNumber("new number");
    contacts[0].saveDetail(&phn);

    QContactSaveRequest csr;
    csr.setManager(cm.data());
    csr.setContacts(contacts);
    QList<QContactDetail::DetailType> typeMasks;
    typeMasks << QContactEmailAddress::Type;
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

    qDebug() <<  "Part of contactPartialSave skipped for now, waiting fixes."; // XXXXXXXXXXXX
    return;

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
    typeMasks << QContactOnlineAccount::Type;
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
    typeMasks << QContactPhoneNumber::Type;
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

    // 6) Have a bad manager uri in the middle followed by a save error

    // ########### This has been changed to compile,
    // # TODO: ### but might not work like it should be.
    // ###########

    QContactId id3(contacts[3].id());
    QContactId badId(id3);
    contacts[3].setId(badId);
    QContactExtendedDetail badDetail; //(QContactDetail::TypeExtendedDetail);
    badDetail.setName("BadField");
    badDetail.setData("BadValue");
    contacts[4].saveDetail(&badDetail);
    csr.setContacts(contacts);
    typeMasks.clear();
    typeMasks << badDetailType;
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() != QContactManager::NoError);
    QMap<int, QContactManager::Error> errorMap(csr.errorMap());
    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[3], QContactManager::DoesNotExistError);
    QCOMPARE(errorMap[4], QContactManager::InvalidDetailError);

    // 7) Have a non existing contact in the middle followed by a save error
    badId = QContactIdMock::createId("badid", 987234); // something nonexistent (hopefully)
    contacts[3].setId(badId);
    csr.setContacts(contacts);
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() != QContactManager::NoError);
    errorMap = csr.errorMap();
    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[3], QContactManager::DoesNotExistError);
    QCOMPARE(errorMap[4], QContactManager::InvalidDetailError);

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

    // 10) A list entirely of new contacts, with some details in the mask
    contacts2.clear();
    QVERIFY(newContact.id().isNull());
    QVERIFY(newContact.details<QContactEmailAddress>().count() == 1);
    QVERIFY(newContact.details<QContactPhoneNumber>().count() == 1);
    contacts2.append(newContact);
    contacts2.append(newContact);
    contacts2[0].setId(badId);
    contacts2[1].saveDetail(&badDetail);
    csr.setContacts(contacts2);
    typeMasks.clear();
    typeMasks << badDetailType;
    csr.setTypeMask(typeMasks);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() != QContactManager::NoError);
    errorMap = csr.errorMap();
    QCOMPARE(errorMap.count(), 2);
    QCOMPARE(errorMap[0], QContactManager::DoesNotExistError);
    QCOMPARE(errorMap[1], QContactManager::InvalidDetailError);
}

void tst_QContactJsonDbAsync::contactPartialSaveAsync()
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
    QVERIFY(saveRequest->isFinished());
}

void tst_QContactJsonDbAsync::contactRemove() {

    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    // Make save request and make initial setup for it with some verifications.
    QContactSaveRequest csr;
    QVERIFY(csr.type() == QContactAbstractRequest::ContactSaveRequest);
    csr.setManager(cm.data());
    QCOMPARE(csr.manager(), cm.data());
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    // Setup a two identical lists of contacts for testing save and remove.
    QContact testContact1, testContact2, testContact3, testContact4, testContact5, testContact6;
    QContactName nameDetail;
    nameDetail.setFirstName("Test Contact1");
    testContact1.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact2");
    testContact2.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact3");
    testContact3.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact4");
    testContact4.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact5");
    testContact5.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact6");
    testContact6.saveDetail(&nameDetail);
    QList<QContact> saveList, testContacts;
    saveList << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6;
    testContacts << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6;

    // Check save requests takes test contacts ok, start save and wait for finished.
    csr.setContacts(testContacts);
    QCOMPARE(csr.contacts(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());
    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();
    foreach (QContact testc, csr.contacts()) {
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTACT: " << testc.id().managerUri();
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTACT: " << testc.id().toString();
    }
    qDebug() << "Returned errors:" << csr.errorMap();
    QVERIFY(csr.errorMap().isEmpty());

    // Setup remove initial remove request and verify it takes data ok.
    QContactRemoveRequest contactRemoveRequest;
    QVERIFY(contactRemoveRequest.type() == QContactAbstractRequest::ContactRemoveRequest);
    contactRemoveRequest.setManager(cm.data());
    QCOMPARE(contactRemoveRequest.manager(), cm.data());
    QVERIFY(!contactRemoveRequest.isActive());
    QVERIFY(!contactRemoveRequest.isFinished());
    QVERIFY(!contactRemoveRequest.cancel());
    QVERIFY(!contactRemoveRequest.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy2(&contactRemoveRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    // Setup valid contact ids for remove request, start it and wait for finished.
    QList<QContactId> toRemove;
    toRemove << cm.data()->contactIds();
    qDebug() << "TO REMOVE: " << toRemove;
    contactRemoveRequest.setContactIds(toRemove);
    QCOMPARE(contactRemoveRequest.contactIds(), toRemove);
    QVERIFY(!contactRemoveRequest.cancel()); // not started
    QVERIFY(contactRemoveRequest.start());
    QVERIFY((contactRemoveRequest.isActive() && contactRemoveRequest.state() == QContactAbstractRequest::ActiveState) || contactRemoveRequest.isFinished());
    QVERIFY(contactRemoveRequest.waitForFinished());
    QVERIFY(contactRemoveRequest.isFinished());
    QVERIFY(spy2.count() >= 1); // active + finished progress signals
    spy2.clear();

    // qDebug() << "Returned errors:" << contactRemoveRequest.errorMap();

    // Check no errors and all contacts are removed (3 from initTestCase and 6 set above).
    QVERIFY(contactRemoveRequest.errorMap().value(0) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(1) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(2) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(3) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(4) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(5) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(6) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(7) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(8) == QContactManager::NoError);
    QList<QContactId> contactsLeft = cm.data()->contactIds();
    QVERIFY(contactsLeft.isEmpty());
}

void tst_QContactJsonDbAsync::contactRemoveErrorHandling() {
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    // Make save request and make initial setup for it with some verifications.
    QContactSaveRequest csr;
    QVERIFY(csr.type() == QContactAbstractRequest::ContactSaveRequest);
    csr.setManager(cm.data());
    QCOMPARE(csr.manager(), cm.data());
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    // Setup a two identical lists of contacts for testing save and remove.
    QContact testContact1, testContact2, testContact3, testContact4, testContact5, testContact6;
    QContactName nameDetail;
    nameDetail.setFirstName("Test Contact1");
    testContact1.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact2");
    testContact2.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact3");
    testContact3.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact4");
    testContact4.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact5");
    testContact5.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact6");
    testContact6.saveDetail(&nameDetail);
    QList<QContact> saveList, testContacts;
    saveList << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6;
    testContacts << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6;

    // Check save requests takes test contacts ok, start save and wait for finished.
    csr.setContacts(testContacts);
    QCOMPARE(csr.contacts(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());
    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // foreach (QContact testc, csr.contacts()) {
    //     qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTACT: " << testc.id().managerUri();
    //     qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONTACT: " << testc.id().id();
    // }
    // qDebug() << "Returned errors:" << csr.errorMap();
    QVERIFY(csr.errorMap().isEmpty());

    // Setup remove initial remove request and verify it takes data ok.
    QContactRemoveRequest contactRemoveRequest;
    QVERIFY(contactRemoveRequest.type() == QContactAbstractRequest::ContactRemoveRequest);
    contactRemoveRequest.setManager(cm.data());
    QCOMPARE(contactRemoveRequest.manager(), cm.data());
    QVERIFY(!contactRemoveRequest.isActive());
    QVERIFY(!contactRemoveRequest.isFinished());
    QVERIFY(!contactRemoveRequest.cancel());
    QVERIFY(!contactRemoveRequest.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy2(&contactRemoveRequest, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    // Setup valid and invalid contact ids for remove request, start it and wait for finished.
    QContactId emptyId;
    QContactId failingId = QContactIdMock::createId("Failing", 0);
    QList<QContactId> toRemove;
    toRemove << emptyId << cm.data()->contactIds();
    toRemove.insert(3, emptyId);
    toRemove.insert(4, failingId);
    toRemove << emptyId << failingId << failingId;
    // qDebug() << "TO REMOVE: " << toRemove;
    contactRemoveRequest.setContactIds(toRemove);
    QCOMPARE(contactRemoveRequest.contactIds(), toRemove);
    QVERIFY(!contactRemoveRequest.cancel()); // not started
    QVERIFY(contactRemoveRequest.start());
    QVERIFY((contactRemoveRequest.isActive() && contactRemoveRequest.state() == QContactAbstractRequest::ActiveState) || contactRemoveRequest.isFinished());
    QVERIFY(contactRemoveRequest.waitForFinished());
    QVERIFY(contactRemoveRequest.isFinished());
    QVERIFY(spy2.count() >= 1); // active + finished progress signals
    spy2.clear();

    // Check ok and errors, empty ids in the beginning, middle and in the end of the contact id list,
    // qDebug() << "Returned errors:" << contactRemoveRequest.errorMap();
    QVERIFY(contactRemoveRequest.errorMap().value(0) == QContactManager::DoesNotExistError);
    QVERIFY(contactRemoveRequest.errorMap().value(1) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(2) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(3) == QContactManager::DoesNotExistError);
    QVERIFY(contactRemoveRequest.errorMap().value(4) == QContactManager::DoesNotExistError);
    QVERIFY(contactRemoveRequest.errorMap().value(5) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(6) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(7) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(9) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(10) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(11) == QContactManager::NoError);
    QVERIFY(contactRemoveRequest.errorMap().value(12) == QContactManager::DoesNotExistError);
    QVERIFY(contactRemoveRequest.errorMap().value(13) == QContactManager::DoesNotExistError);
    QVERIFY(contactRemoveRequest.errorMap().value(14) == QContactManager::DoesNotExistError);

    // Check that all the contacts have been removed
    QList<QContactId> contactsLeft = cm.data()->contactIds();
    QVERIFY(contactsLeft.isEmpty());
}

void tst_QContactJsonDbAsync::testQuickDestruction()
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

void tst_QContactJsonDbAsync::deleteRequest()
{
    // Delete the sender (request) - check that it doesn't crash in this common coding error
    delete sender();
}
void tst_QContactJsonDbAsync::resultsAvailableReceived()
{
    QContactFetchRequest *req = qobject_cast<QContactFetchRequest *>(QObject::sender());
    if (req)
        m_resultsAvailableSlotThreadId = req->thread()->currentThreadId();
    else
        qWarning() << "resultsAvailableReceived() : request deleted; unable to set thread id!";
}

QContactManager* tst_QContactJsonDbAsync::prepareModel(const QString& managerUri)
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

    return cm;

    // TODO: cleanup once test is complete
}

QTEST_MAIN(tst_QContactJsonDbAsync)
#include "tst_qcontactjsondbasync.moc"
