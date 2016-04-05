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

#include <QCoreApplication>
#include <QScopedPointer>

#include <QtContacts/qcontacts.h>

#include "qcontactmanagerdataholder.h" //QContactManagerDataHolder

QTCONTACTS_USE_NAMESPACE

/* Define an innocuous request (fetch ie doesn't mutate) to "fill up" any queues */
#define FILL_QUEUE_WITH_FETCH_REQUESTS() QContactFetchRequest fqcfr1, fqcfr2, fqcfr3; \
                                         fqcfr1.start(); \
                                         fqcfr2.start(); \
                                         fqcfr3.start();

/* Define an innocuous request (fetch ie doesn't mutate) to "fill up" any queues */
#define FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(manager) QContactFetchRequest fqifr1, fqifr2, fqifr3; \
                                                fqifr1.setManager(manager); fqifr1.start(); \
                                                fqifr2.setManager(manager); fqifr2.start(); \
                                                fqifr3.setManager(manager); fqifr3.start();


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


static inline QContactId makeId(const QString &managerName, uint id)
{
    return QContactId(QStringLiteral("qtcontacts:basic%1:").arg(managerName), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}


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

    void contactFetchByIdWithEmptyIds();
    void contactFetchByIdWithEmptyIds_data() { addManagers(); }
    void contactFetchByIdMixingEmptyIds();
    void contactFetchByIdMixingEmptyIds_data() { addManagers(); }
    void contactFetchByIdWithNonExistingButValidIds();
    void contactFetchByIdWithNonExistingButValidIds_data() { addManagers(); }
    void contactFetchByIdErrorHandling();
    void contactFetchByIdErrorHandling_data() { addManagers(); }

    void contactIdFetch();
    void contactIdFetch_data() { addManagers(); }
    void contactRemove();
    void contactRemove_data() { addManagers(); }
    void contactRemoveErrorHandling();
    void contactRemoveErrorHandling_data() {addManagers();}
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

    void relationshipFetch();
    void relationshipFetch_data() { addManagers(); }
    void relationshipRemove();
    void relationshipRemove_data() { addManagers(); }
    void relationshipSave();
    void relationshipSave_data() { addManagers(); }

    void collectionFetch();
    void collectionFetch_data() { addManagers(); }
    void collectionRemove();
    void collectionRemove_data() { addManagers(); }
    void collectionSave();
    void collectionSave_data() { addManagers(); }

    void maliciousManager(); // uses it's own custom data (manager)

    void testQuickDestruction();
    void testQuickDestruction_data() { addManagers(QStringList(QString("maliciousplugin"))); }

    void threadDelivery();
    void threadDelivery_data() { addManagers(QStringList(QString("maliciousplugin"))); }
protected slots:
    void resultsAvailableReceived();

private:
    bool compareContactLists(QList<QContact> lista, QList<QContact> listb);
    bool compareContacts(QContact ca, QContact cb);
    bool containsIgnoringTimestamps(const QList<QContact>& list, const QContact& c);
    bool compareIgnoringTimestamps(const QContact& ca, const QContact& cb);
    bool containsAllCollectionIds(const QList<QContactCollectionId>& target, const QList<QContactCollectionId>& ids);
    QContactManager* prepareModel(const QString& uri);

    Qt::HANDLE m_mainThreadId;
    Qt::HANDLE m_resultsAvailableSlotThreadId;
    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

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
    managerDataHolder.reset(new QContactManagerDataHolder());
}

void tst_QContactAsync::cleanupTestCase()
{
    managerDataHolder.reset(0);
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

bool tst_QContactAsync::containsAllCollectionIds(const QList<QContactCollectionId> &target, const QList<QContactCollectionId> &ids)
{
    bool containsAllIds = true;
    foreach (QContactCollectionId id, ids) {
        if (!target.contains(id)) {
            containsAllIds = false;
            break;
        }
    }
    return containsAllIds;
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
        // create a contact from the restricted data only (id)
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
            if (det.type() == QContactTimestamp::Type) {
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

void tst_QContactAsync::contactFetchByIdWithEmptyIds()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactFetchByIdRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::ContactFetchByIdRequest);

    // List of qmpty ids.
    QList<QContactId> contactIds;
    contactIds.append(QContactId());
    contactIds.append(QContactId());
    contactIds.append(QContactId());
    int expectedCount = 3;

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
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContact> contacts = cfr.contacts();
    QCOMPARE(contacts.size(), expectedCount);
    int contactIndex = 0;
    foreach (const QContactId id, contactIds) {
            QCOMPARE(contacts.at(contactIndex), QContact());
            QCOMPARE(cfr.errorMap().value(contactIndex),QContactManager::DoesNotExistError);
        contactIndex++;
    }
}

void tst_QContactAsync::contactFetchByIdMixingEmptyIds()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactFetchByIdRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::ContactFetchByIdRequest);

    // List of ids contains few empty ones among good ones.
    int expectedCount = cm->contactIds().size();
    QList<QContactId> contactIds;
    foreach (const QContactId id, cm->contactIds()) {
        contactIds.append(QContactId());
        expectedCount ++;
        contactIds.append(id);
    }
    contactIds.append(QContactId());
    expectedCount++;

    // "Contacts" retrieval
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
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContact> contacts = cfr.contacts();
    QCOMPARE(contacts.size(), expectedCount);
    int contactIndex = 0;
    foreach (const QContactId id, contactIds) {
        if (id == QContactId()) {
            QCOMPARE(contacts.at(contactIndex), QContact());
            QCOMPARE(cfr.errorMap().value(contactIndex),QContactManager::DoesNotExistError);
        } else {
            QCOMPARE(contacts.at(contactIndex).id(), id);
        }
        contactIndex++;
    }
}

void tst_QContactAsync::contactFetchByIdWithNonExistingButValidIds()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactFetchByIdRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::ContactFetchByIdRequest);

    // List of ids contains few empty ones among good ones.
    int expectedCount = 0;
    QList<QContactId> contactIds;
    foreach (const QContactId id, cm->contactIds()) {
        QVERIFY(cm->removeContact(id));
        contactIds.append(id);
        expectedCount ++;
    }

    // "Contacts" retrieval
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
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContact> contacts = cfr.contacts();
    QCOMPARE(contacts.size(), expectedCount);
    int contactIndex = 0;
    foreach (const QContactId id, contactIds) {
        QCOMPARE(contacts.at(contactIndex), QContact());
        QCOMPARE(cfr.errorMap().value(contactIndex),QContactManager::DoesNotExistError);
        contactIndex++;
    }
}

void tst_QContactAsync::contactFetchByIdErrorHandling()
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
    // for all backends.
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

void tst_QContactAsync::contactRemoveErrorHandling() {
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
    QContactId failingId = makeId("Failing", 0);
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

    // check if the contact was saved on default collection
    QCOMPARE(result.first().collectionId().toString(), cm->defaultCollectionId().toString());
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

void tst_QContactAsync::contactSaveErrorHandling()
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

    // Save a group of contacts, including few TypeGroup contacts which may not be supported by all backends.
    QContact testContact1, testContact2, testContact3, testContact4, testContact5, testContact6, testContact7,testContact8;
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
    nameDetail.setFirstName("Test Contact7");
    testContact7.saveDetail(&nameDetail);
    nameDetail.setFirstName("Test Contact8");
    testContact8.saveDetail(&nameDetail);

    // Set group type to first, middle and last contact in the list.
    QContactType typeDetail;
    typeDetail.setType(QContactType::TypeGroup);
    testContact1.saveDetail(&typeDetail);
    testContact3.saveDetail(&typeDetail);
    testContact6.saveDetail(&typeDetail);

    // Set an invalid phone number
    QContactPhoneNumber invalidPhone;
    invalidPhone.setNumber("      hjkjkjhkjkhjkhj       //////");
    testContact7.saveDetail(&invalidPhone);

    // This phone number can be cleaned up, so no error should be generated
    QContactPhoneNumber sanitizablePhone;
    sanitizablePhone.setNumber("       +123458++++++++*#");
    testContact5.saveDetail(&sanitizablePhone);

    // Set an invalid name
    QContactName invalidName;
    invalidName.setFirstName("     ");
    testContact8.saveDetail(&invalidName);

    QList<QContact> saveList;
    saveList << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6 << testContact7 << testContact8;

    csr.setManager(cm.data());
    QCOMPARE(csr.manager(), cm.data());
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));

    QList<QContact> testContacts;
    testContacts << testContact1 << testContact2 << testContact3 << testContact4 << testContact5 << testContact6 << testContact7 << testContact8;
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

    // Note, the returned value is actually set/remapped in to the errorMap by common code in qcontactmanagerengine
    if (!cm->supportedContactTypes().contains(QContactType::TypeGroup)) {
        QCOMPARE(csr.errorMap().value(0), QContactManager::InvalidContactTypeError);
        QCOMPARE(csr.errorMap().value(2), QContactManager::InvalidContactTypeError);
        QCOMPARE(csr.errorMap().value(5), QContactManager::InvalidContactTypeError);
        QCOMPARE(csr.error(), QContactManager::InvalidContactTypeError);

        // With errors in the batch, we can't know that the other contacts have no error...
    } else {
        QCOMPARE(csr.errorMap().value(0), QContactManager::NoError);
        QCOMPARE(csr.errorMap().value(2), QContactManager::NoError);
        QCOMPARE(csr.errorMap().value(5), QContactManager::NoError);
        QCOMPARE(csr.error(), QContactManager::NoError);

        QCOMPARE(csr.errorMap().value(1), QContactManager::NoError);
        QCOMPARE(csr.errorMap().value(3), QContactManager::NoError);
        QCOMPARE(csr.errorMap().value(4), QContactManager::NoError);
        QCOMPARE(csr.errorMap().value(7), QContactManager::NoError);
    }
}

void tst_QContactAsync::contactSaveRemovedContacts()
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

    //test case for existing and non existing contacts after remove in contact
    //save request
    nameDetail.setFirstName("Test4");
    QContact testContact4;
    testContact4.saveDetail(&nameDetail);
    testContacts = csr.contacts();
    testContacts << testContact4;
    csr.setContacts(testContacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QCOMPARE(csr.errorMap().value(0), QContactManager::DoesNotExistError);
    QCOMPARE(csr.errorMap().value(1), QContactManager::DoesNotExistError);
    QCOMPARE(csr.errorMap().value(2), QContactManager::DoesNotExistError);
    QCOMPARE(csr.errorMap().value(3), QContactManager::NoError);
}

void tst_QContactAsync::contactSaveRemovedContactsWithCleanIds()
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

void tst_QContactAsync::contactPartialSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QList<QContact> contacts(cm->contacts());
    QList<QContact> originalContacts(contacts);
    QCOMPARE(contacts.count(), 3);

    qDebug() << Q_FUNC_INFO << "contacts:" << contacts;

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
    QContactId badId = makeId("BadManager", 0);
    contacts[3].setId(badId);
    csr.setContacts(contacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() == QContactManager::DoesNotExistError); // error in the middle in this partial save
    QMap<int, QContactManager::Error> errorMap(csr.errorMap());
    QCOMPARE(errorMap.count(), 1);//one error in error map, related to the fetch phase of this partial save
    QCOMPARE(errorMap[3], QContactManager::DoesNotExistError);

    // 7) Have a non existing contact in the middle
    badId = makeId(cm->managerName(), 0);
    contacts[3].setId(badId);
    csr.setContacts(contacts);
    QVERIFY(csr.start());
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.error() == QContactManager::DoesNotExistError); // error in the middle in this partial save
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
    QVERIFY(csr.error() == QContactManager::DoesNotExistError); // error in the middle in this partial save
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
    rfr.setFirst(aContact.id());
    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    rels = cm->relationships(aContact.id(), QContactRelationship::First);
    result = rfr.relationships();
    QCOMPARE(rels, result);

    // specific participant retrieval #1 - destination participant
    rfr.setFirst(QContactId());
    contacts = cm->contactIds();
    QContact bContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Bob")) {
            bContact = curr;
            break;
        }
    }
    rfr.setSecond(bContact.id());

    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // retrieve rels where second = id of B, and ensure that we get the same results
    rels = cm->relationships(bContact.id(), QContactRelationship::Second);
    result = rfr.relationships();
    QCOMPARE(rels, result);

    // specific participant retrieval #2 - source participant
    rfr.setFirst(QContactId());
    contacts = cm->contactIds();
    QContact cContact;
    foreach (const QContactId& currId, contacts) {
        QContact curr = cm->contact(currId);
        if (curr.detail(QContactName::Type).value(QContactName::FieldFirstName) == QString("Borris")) {
            cContact = curr;
            break;
        }
    }
    rfr.setSecond(cContact.id());

    QVERIFY(!rfr.cancel()); // not started
    QVERIFY(rfr.start());

    QVERIFY((rfr.isActive() && rfr.state() == QContactAbstractRequest::ActiveState) || rfr.isFinished());
    //QVERIFY(rfr.isFinished() || !rfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(rfr.waitForFinished());
    QVERIFY(rfr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // retrieve rels where first = id of C and compare the results
    rfr.setFirst(cContact.id());
    rfr.setSecond(QContactId());
    QVERIFY(rfr.start());
    QVERIFY(rfr.waitForFinished());
    result = rfr.relationships();
    rels = cm->relationships(cContact.id(), QContactRelationship::First);
    QCOMPARE(rels, result);

    // cancelling
    rfr.setRelationshipType(QString());

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
    r.setFirst(aContact.id());
    r.setSecond(cContact.id());
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
    QCOMPARE(cm->relationships(QContactRelationship::HasAssistant(), cContact.id(), QContactRelationship::Second).size(), 1);

    // remove (asynchronously) a nonexistent relationship - should fail.
    r.setFirst(cContact.id());
    r.setSecond(aContact.id());
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

    QCOMPARE(cm->relationships(QContactRelationship::HasManager(), cContact.id(), QContactRelationship::First).size(), 0);
//    QCOMPARE(rrr.error(), QContactManager::DoesNotExistError);

    // cancelling
    r.setFirst(cContact.id());
    r.setSecond(QContactId());
    relationships.clear();
    relationships.push_back(r);

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

        QVERIFY(cm->relationships(cContact.id()).size() != 0); // didn't remove them.
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

        QVERIFY(cm->relationships(cContact.id()).size() != 0); // didn't remove them.
        break;
    }
}

void tst_QContactAsync::relationshipSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

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
    int originalCount = cm->relationships(aContact.id()).size();
    QContactRelationship testRel;
    testRel.setFirst(aContact.id());
    testRel.setRelationshipType(QContactRelationship::HasSpouse());
    testRel.setSecond(bContact.id());
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

    QList<QContactRelationship> expected = cm->relationships(QContactRelationship::HasSpouse(), aContact.id(), QContactRelationship::First);
    QList<QContactRelationship> result = rsr.relationships();
    QCOMPARE(expected, result);
    QVERIFY(result.contains(testRel));
    QCOMPARE(cm->relationships(aContact.id()).size(), originalCount + 1); // should be one extra

    // save a new relationship
    testRel.setSecond(cContact.id());
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
    expected = cm->relationships(QContactRelationship::HasSpouse(), aContact.id(), QContactRelationship::First);
    result = rsr.relationships();
    QCOMPARE(result, QList<QContactRelationship>() << testRel);
    QVERIFY(expected.contains(testRel));
    QCOMPARE(cm->relationships(aContact.id()).size(), originalCount + 2); // should now be two extra

    // cancelling
    testRel.setSecond(aContact.id()); // shouldn't get saved (circular anyway)
    saveList.clear();
    saveList << testRel;
    rsr.setRelationships(saveList);

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
        QList<QContactRelationship> aRels = cm->relationships(aContact.id(), QContactRelationship::First);
        QVERIFY(!aRels.contains(testRel));
        QCOMPARE(cm->relationships(aContact.id()).size(), originalCount + 2); // should still only be two extra

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
        QList<QContactRelationship> aRels = cm->relationships(aContact.id(), QContactRelationship::First);
        QVERIFY(!aRels.contains(testRel));
        QCOMPARE(cm->relationships(aContact.id()).size(), originalCount + 2); // should still only be two extra

        break;
    }
}

void tst_QContactAsync::collectionFetch()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));

    QContactCollectionFetchRequest cfr;
    QVERIFY(cfr.type() == QContactAbstractRequest::CollectionFetchRequest);

    // initial state - not started, no manager.
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.start());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());

    // retrieve all collections.
    cfr.setManager(cm.data());
    QCOMPARE(cfr.manager(), cm.data());
    QVERIFY(!cfr.isActive());
    QVERIFY(!cfr.isFinished());
    QVERIFY(!cfr.cancel());
    QVERIFY(!cfr.waitForFinished());
    qRegisterMetaType<QContactCollectionFetchRequest*>("QContactCollectionFetchRequest*");
    QThreadSignalSpy spy(&cfr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QVERIFY(!cfr.cancel()); // not started

    QVERIFY(cfr.start());
    //QVERIFY(cfr.isFinished() || !cfr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY((cfr.isActive() && cfr.state() == QContactAbstractRequest::ActiveState) || cfr.isFinished());
    QVERIFY(cfr.waitForFinished());
    QVERIFY(cfr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContactCollection> syncCols = cm->collections();
    QList<QContactCollection> cols = cfr.collections();
    QCOMPARE(cols.size(), syncCols.size());
    for (int i = 0; i < cols.size(); i++) {
        QContactCollection curr = cols.at(i);
        QVERIFY(syncCols.contains(curr));
    }

    // cancelling
    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!cfr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(cm.data());
        QVERIFY(cfr.start());
        if (!cfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            spy.clear();
            cfr.waitForFinished();
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
        FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(cm.data());
        QVERIFY(cfr.start());
        if (!cfr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            cfr.waitForFinished();
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
}

void tst_QContactAsync::collectionRemove()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactCollectionRemoveRequest crr;
    QVERIFY(crr.type() == QContactAbstractRequest::CollectionRemoveRequest);

    // initial state - not started, no manager.
    QVERIFY(!crr.isActive());
    QVERIFY(!crr.isFinished());
    QVERIFY(!crr.start());
    QVERIFY(!crr.cancel());
    QVERIFY(!crr.waitForFinished());

    // specific collection set
    QContactCollectionId removeId = cm->collections().last().id();
    if (cm->defaultCollectionId() == removeId)
        removeId = cm->collections().first().id();
    crr.setCollectionId(removeId);
    QVERIFY(crr.collectionIds() == QList<QContactCollectionId>() << removeId);
    int originalCount = cm->collections().size();
    crr.setManager(cm.data());
    QCOMPARE(crr.manager(), cm.data());
    QVERIFY(!crr.isActive());
    QVERIFY(!crr.isFinished());
    QVERIFY(!crr.cancel());
    QVERIFY(!crr.waitForFinished());
    qRegisterMetaType<QContactCollectionRemoveRequest*>("QContactCollectionRemoveRequest*");
    QThreadSignalSpy spy(&crr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    QVERIFY(!crr.cancel()); // not started
    QVERIFY(crr.start());
    QVERIFY((crr.isActive() &&crr.state() == QContactAbstractRequest::ActiveState) || crr.isFinished());
    //QVERIFY(crr.isFinished() || !crr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());

    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QCOMPARE(cm->collections().size(), originalCount - 1); // should have removed that particular collection.
    QVERIFY(crr.error() == QContactManager::NoError);
    QVERIFY(crr.errorMap().isEmpty());

    // remove all collections
    QList<QContactCollectionId> allCollectionIds;
    QList<QContactCollection> allCollections = cm->collections();
    for (int i = 0; i < allCollections.size(); ++i)
        allCollectionIds << allCollections.at(i).id();
    crr.setCollectionIds(allCollectionIds);

    QVERIFY(!crr.cancel()); // not started
    QVERIFY(crr.start());

    QVERIFY((crr.isActive() && crr.state() == QContactAbstractRequest::ActiveState) || crr.isFinished());
    //QVERIFY(crr.isFinished() || !crr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(crr.waitForFinished());
    QVERIFY(crr.isFinished());

    QVERIFY(cm->collections().size() >= 1); // at least one collection must be left, since default collection cannot be removed.
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    // remove empty list
    QList<QContactCollectionId> collectionIdList;
    QContactCollectionRemoveRequest crr1;
    crr1.setManager(cm.data());
    crr1.setCollectionIds(collectionIdList);
    crr1.start();
    crr1.waitForFinished();
    QVERIFY(crr1.isFinished());
    QVERIFY(crr1.error() == QContactManager::NoError);

    // cancelling
    QContactCollection temp;
    temp.setMetaData(QContactCollection::KeyDescription, "Should not be removed!");
    cm->saveCollection(&temp);
    crr.setCollectionId(temp.id());

    int collectionCount = cm->collections().size();
    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!crr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(cm.data());
        QVERIFY(spy.count() == 0);
        QVERIFY(crr.start());
        if (!crr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            crr.waitForFinished();
            temp.setId(QContactCollectionId());
            if (!cm->saveCollection(&temp)) {
                QSKIP("Unable to save temporary item for remove request cancellation test!");
            }
            crr.setCollectionId(temp.id());
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
        QCOMPARE(cm->collections().size(), collectionCount); // temp collection should not have been removed
        QList<QContactCollectionId> removeCollectionIds;
        QList<QContactCollection> removeCollections = cm->collections();
        for (int i = 0; i < removeCollections.size(); ++i)
            removeCollectionIds << removeCollections.at(i).id();
        QVERIFY(containsAllCollectionIds(removeCollectionIds, crr.collectionIds()));
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

    // restart, and wait for progress after cancel.
    while (true) {
        QVERIFY(!crr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(cm.data());
        QVERIFY(crr.start());
        if (!crr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            crr.waitForFinished();
            temp.setId(QContactCollectionId());
            if (!cm->saveCollection(&temp)) {
                QSKIP("Unable to save temporary item for remove request cancellation test!");
            }
            crr.setCollectionId(temp.id());
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
        QCOMPARE(cm->collections().size(), collectionCount); // temp collection should not have been removed
        QList<QContactCollectionId> removeCollectionIds;
        QList<QContactCollection> removeCollections = cm->collections();
        for (int i = 0; i < removeCollections.size(); ++i)
            removeCollectionIds << removeCollections.at(i).id();
        QVERIFY(containsAllCollectionIds(removeCollectionIds, crr.collectionIds()));
        QVERIFY(spy.count() >= 1); // active + cancelled progress signals
        spy.clear();
        break;
    }

    // now clean up our temp collection.
    cm->removeCollection(temp.id());

}

void tst_QContactAsync::collectionSave()
{
    QFETCH(QString, uri);
    QScopedPointer<QContactManager> cm(prepareModel(uri));
    QContactCollectionSaveRequest csr;
    QVERIFY(csr.type() == QContactAbstractRequest::CollectionSaveRequest);

    // initial state - not started, no manager.
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.start());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());

    // save a new item
    int originalCount = cm->collections().size();
    QContactCollection testCollection;
    testCollection.setMetaData(QContactCollection::KeyDescription, "test description");
    testCollection.setMetaData(QContactCollection::KeyName, "New collection");
    QList<QContactCollection> saveList;
    saveList << testCollection;
    csr.setManager(cm.data());
    QCOMPARE(csr.manager(), cm.data());
    QVERIFY(!csr.isActive());
    QVERIFY(!csr.isFinished());
    QVERIFY(!csr.cancel());
    QVERIFY(!csr.waitForFinished());
    qRegisterMetaType<QContactCollectionSaveRequest*>("QContactCollectionSaveRequest*");
    QThreadSignalSpy spy(&csr, SIGNAL(stateChanged(QContactAbstractRequest::State)));
    csr.setCollection(testCollection);
    QCOMPARE(csr.collections(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());

    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    //QVERIFY(csr.isFinished() || !csr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(csr.waitForFinished());
    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    QList<QContactCollection> expected = csr.collections();
    QCOMPARE(expected.size(), 1);
    QList<QContactCollection> result;
    result << cm->collection(csr.collections().at(0).id());

    // find the saved one, compare.
    foreach (const QContactCollection &col, result)
        QVERIFY(col.id() == expected.at(0).id());

    // update a previously saved collection
    QVERIFY(!result.isEmpty()); // make sure that we were able to retrieve the required collection.
    testCollection = result.first();
    testCollection.setMetaData(QContactCollection::KeyName, "test name");
    saveList.clear();
    saveList << testCollection;
    csr.setCollections(saveList);
    QCOMPARE(csr.collections(), saveList);
    QVERIFY(!csr.cancel()); // not started
    QVERIFY(csr.start());

    QVERIFY((csr.isActive() && csr.state() == QContactAbstractRequest::ActiveState) || csr.isFinished());
    //QVERIFY(csr.isFinished() || !csr.start());  // already started. // thread scheduling means this is untestable
    QVERIFY(csr.waitForFinished());

    QVERIFY(csr.isFinished());
    QVERIFY(spy.count() >= 1); // active + finished progress signals
    spy.clear();

    expected = csr.collections();
    result.clear();
    result = cm->collections();
    // find the saved one, compare.
    foreach (const QContactCollection& col, result) {
        if (col.id() == expected.at(0).id()) {
            QVERIFY(col == expected.at(0)); // XXX TODO: if we change the semantic so that save merely updates the id...?
        }
    }
    QCOMPARE(cm->collections().size(), originalCount + 1); // ie shouldn't have added an extra one (would be +2)
    QVERIFY(csr.error() == QContactManager::NoError);
    QVERIFY(csr.errorMap().isEmpty());

    // save empty list
    QList<QContactCollection> collectionList;
    QContactCollectionSaveRequest csr1;
    csr1.setManager(cm.data());
    csr1.setCollections(collectionList);
    csr1.start();
    csr1.waitForFinished();
    QVERIFY(csr1.isFinished());
    QVERIFY(csr1.error() == QContactManager::NoError);

    // cancelling
    QContactCollection temp;
    temp.setMetaData(testCollection.metaData());
    temp.setExtendedMetaData("test", "shouldn't be saved");
    saveList.clear();
    saveList << temp;
    csr.setCollections(saveList);

    int bailoutCount = MAX_OPTIMISTIC_SCHEDULING_LIMIT; // attempt to cancel 40 times.  If it doesn't work due to threading, bail out.
    while (true) {
        QVERIFY(!csr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(cm.data());
        QVERIFY(csr.start());
        if (!csr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            csr.waitForFinished();
            saveList = csr.collections();
            if (cm->collections().size() > (originalCount + 1) && !cm->removeCollection(saveList.at(0).id())) {
                QSKIP("Unable to remove saved collection to test cancellation of collection save request");
            }
            saveList.clear();
            saveList << temp;
            csr.setCollections(saveList);
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
        QList<QContactCollection> allCollections = cm->collections();
        QVERIFY(!allCollections.contains(temp)); // should NOT contain it since it was cancelled.
        QCOMPARE(allCollections.size(), originalCount + 1);
        break;
    }
    // restart, and wait for progress after cancel.

    while (true) {
        QVERIFY(!csr.cancel()); // not started
        FILL_QUEUE_WITH_FETCH_REQUESTS_WITH_MANAGER(cm.data());
        QVERIFY(csr.start());
        if (!csr.cancel()) {
            // due to thread scheduling, async cancel might be attempted
            // after the request has already finished.. so loop and try again.
            csr.waitForFinished();
            saveList = csr.collections();
            if (cm->collections().size() > (originalCount + 1) && !cm->removeCollection(saveList.at(0).id())) {
                QSKIP("Unable to remove saved item to test cancellation of item save request");
            }
            saveList.clear();
            saveList << temp;
            csr.setCollections(saveList);
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
        QList<QContactCollection> allCollections = cm->collections();
        QVERIFY(!allCollections.contains(temp));
        QCOMPARE(cm->collections().size(), originalCount + 1);
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
    if (!QContactManager::availableManagers().contains("maliciousplugin"))
        QSKIP("Malicious plugin is not installed, skipping test.");
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
    Q_ASSERT(req);
    m_resultsAvailableSlotThreadId = req->thread()->currentThreadId();
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

    if (cm->contacts().size() != 3)
        qWarning() << Q_FUNC_INFO << "Failed to prepare model!";

    QContactRelationship arb;
    arb.setFirst(a.id());
    arb.setSecond(b.id());
    arb.setRelationshipType(QContactRelationship::HasManager());
    cm->saveRelationship(&arb);

    QContactRelationship brc;
    brc.setFirst(b.id());
    brc.setSecond(c.id());
    brc.setRelationshipType(QContactRelationship::HasAssistant());
    cm->saveRelationship(&brc);

    QContactRelationship cra;
    cra.setFirst(c.id());
    cra.setSecond(a.id());
    cra.setRelationshipType(QContactRelationship::HasSpouse());
    cm->saveRelationship(&cra);

    QContactRelationship arc;
    arc.setFirst(a.id());
    arc.setSecond(c.id());
    arc.setRelationshipType(QContactRelationship::HasAssistant());
    cm->saveRelationship(&arc);

    QContactRelationship crb;
    crb.setFirst(c.id());
    crb.setSecond(b.id());
    crb.setRelationshipType(QContactRelationship::IsSameAs());
    cm->saveRelationship(&crb);

    QContactCollection testCollection;
    testCollection.setMetaData(QContactCollection::KeyName, "Test Collection");
    testCollection.setMetaData(QContactCollection::KeyDescription, "test collection");
    cm->saveCollection(&testCollection);

    return cm;

    // TODO: cleanup once test is complete
}

QTEST_MAIN(tst_QContactAsync)
#include "tst_qcontactasync.moc"
