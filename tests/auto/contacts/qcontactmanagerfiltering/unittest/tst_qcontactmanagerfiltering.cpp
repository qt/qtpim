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

#define QT_STATICPLUGIN
#include <QtTest/QtTest>

#ifdef INCLUDE_TESTACTIONS
#include <QServiceManager>
#endif
#include <QtContacts>
#include "qcontactmanagerdataholder.h" //QContactManagerDataHolder

#include <QLocale>
#ifdef Q_OS_UNIX
#include <locale.h>
#endif

//TESTED_COMPONENT=src/contacts

// Q_ASSERT replacement, since we often run in release builds
#define Q_FATAL_VERIFY(statement)                                         \
do {                                                                      \
    if (!QTest::qVerify((statement), #statement, "", __FILE__, __LINE__)) \
        qFatal("severe failure encountered, test cannot continue");       \
} while (0)

QTCONTACTS_USE_NAMESPACE
/*
 * This test is mostly just for testing sorting and filtering -
 * having it in tst_QContactManager makes maintenance more
 * difficult!
 */

Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(QContactManager*)
Q_DECLARE_METATYPE(QContactDetail::DetailType)

/*
 * Global variables:
 * These are the definition and field names used by the actions for their matching.
 */
QMap<QString, QPair<QContactDetail::DetailType, int> > defAndFieldNamesForTypeForActions;

namespace {

bool validDetailType(const QPair<QContactDetail::DetailType, int> &pair)
{
    return pair.first != QContactDetail::TypeUndefined;
}

bool validDetailField(const QPair<QContactDetail::DetailType, int> &pair)
{
    return pair.first != QContactDetail::TypeUndefined && pair.second != -1;
}

}

/*
 * We use this code to compare the output and expected lists of filtering
 * where no sort order is implied.
 * TODO: use this instead of QCOMPARE in the various filtering tests!
 */
#define QCOMPARE_UNSORTED(output, expected) if (output.size() != expected.size()) { \
                                                QCOMPARE(output, expected); \
                                            } else { \
                                                for (int i = 0; i < output.size(); i++) { \
                                                    if (!expected.contains(output.at(i))) { \
                                                        QCOMPARE(output, expected); \
                                                    } \
                                                } \
                                            }

class tst_QContactManagerFiltering : public QObject
{
Q_OBJECT

public:
    tst_QContactManagerFiltering();
    virtual ~tst_QContactManagerFiltering();

private:
    void dumpContactDifferences(const QContact& a, const QContact& b);
    void dumpContact(const QContact &c);
    void dumpContacts();
    bool isSuperset(const QContact& ca, const QContact& cb);

    QList<QContactId> prepareModel(QContactManager* cm); // add the standard contacts

    QString convertIds(QList<QContactId> allIds, QList<QContactId> ids, QChar minimumContact = 'a', QChar maximumContact = 'z'); // convert back to "abcd"
    QContact createContact(QContactManager* cm, QContactType::TypeValues type, const QString &name);

    QMap<QContactManager*, QMap<QString, QPair<QContactDetail::DetailType, int> > > defAndFieldNamesForTypePerManager;
    QMultiMap<QContactManager*, QContactId> contactsAddedToManagers;
    QList<QContactManager*> managers;
    QScopedPointer<QContactManagerDataHolder> managerDataHolder;

    QTestData& newMRow(const char *tag, QContactManager *cm);

private slots:

    void initTestCase();
    void cleanupTestCase();

    void rangeFiltering(); // XXX should take all managers
    void rangeFiltering_data();

    void detailStringFiltering(); // XXX should take all managers
    void detailStringFiltering_data();

    void detailPhoneNumberFiltering();
    void detailPhoneNumberFiltering_data();

#ifdef INCLUDE_TESTACTIONS
    void actionPlugins();
    void actionFiltering();
    void actionFiltering_data();
#endif

    void detailVariantFiltering();
    void detailVariantFiltering_data();

    void intersectionFiltering();
    void intersectionFiltering_data();

    void unionFiltering();
    void unionFiltering_data();

    void relationshipFiltering();
    void relationshipFiltering_data();

    void changelogFiltering();
    void changelogFiltering_data();

    void idListFiltering();
    void idListFiltering_data();

    void convenienceFiltering();
    void convenienceFiltering_data();

    void sorting(); // XXX should take all managers
    void sorting_data();

    void multiSorting();
    void multiSorting_data();

    void invalidFiltering_data();
    void invalidFiltering();

    void allFiltering_data();
    void allFiltering();

    void fetchHint_data();
    void fetchHint();

private:
    enum CollationSemantics {
        UpperFirst = 0,             // XYxy
        LowerFirst = 1,             // xyXY
        InterleavedUpperFirst = 2,  // XxYy
        InterleavedLowerFirst = 3,  // xXyY
        FirstPassInterleavedUpperFirst = 4, // xxy < Xxz even though xxy > Xxy
        FirstPassInterleavedLowerFirst = 5, // XXY < Xxz even though XXY > Xxy
        UnknownCollation = 6
    };
    QHash<QContactManager *, int> m_collation;
    QString expectedSorting(QContactManager *m, const QString &ufs, const QString &lfs, const QString &iufs, const QString &ilfs, const QString &fpiufs, const QString &fpilfs) {
        switch (m_collation[m]) {
            case UpperFirst: return ufs;
            case LowerFirst: return lfs;
            case InterleavedUpperFirst: return iufs;
            case InterleavedLowerFirst: return ilfs;
            case FirstPassInterleavedUpperFirst: return fpiufs;
            case FirstPassInterleavedLowerFirst: return fpilfs;
            default: return QLatin1String("unknowncollation");
        }
    }
};

tst_QContactManagerFiltering::tst_QContactManagerFiltering()
{
    // In order to make our tests reliable, set the C locale
    QLocale::setDefault(QLocale::c());

#if defined(Q_OS_UNIX) && !defined(QT_USE_ICU)
    // Without ICU, we also need to set the locale on unix
    ::setlocale(LC_ALL, "C");
#endif
}

tst_QContactManagerFiltering::~tst_QContactManagerFiltering()
{
}

void tst_QContactManagerFiltering::initTestCase()
{
    managerDataHolder.reset(new QContactManagerDataHolder());

    // firstly, build a list of the managers we wish to test.
    QStringList managerNames = QContactManager::availableManagers();

    /* Known one that will not pass */
    managerNames.removeAll("invalid");
    managerNames.removeAll("testdummy");
    managerNames.removeAll("teststaticdummy");
    managerNames.removeAll("maliciousplugin");

    // Some internal engines on Maemo6
    managerNames.removeAll("social");
    managerNames.removeAll("simcard");
    managerNames.removeAll("com.nokia.messaging.contacts.engines.mail.contactslookup");

    foreach(QString mgr, managerNames) {
        QMap<QString, QString> params;
        QString mgrUri = QContactManager::buildUri(mgr, params);
        QContactManager* cm = QContactManager::fromUri(mgrUri);
        cm->setObjectName(mgr);
        managers.append(cm);

        if (mgr == "memory") {
            params.insert("id", "tst_QContactManager");
            mgrUri = QContactManager::buildUri(mgr, params);
            cm = QContactManager::fromUri(mgrUri);
            cm->setObjectName("memory[params]");
            managers.append(cm);
        }
    }

    // for each manager that we wish to test, prepare the model.
    foreach (QContactManager* cm, managers) {
        QList<QContactId> addedContacts = prepareModel(cm);
        if (addedContacts != contactsAddedToManagers.values(cm)) {
            qDebug() << "prepareModel returned:" << addedContacts;
            qDebug() << "contactsAdded are:    " << contactsAddedToManagers.values(cm);
            qFatal("returned list different from saved contacts list!");
        }

        // Also determine the string collation / sorting semantics.
        // Case sensitivity is handled differently with/without ICU
        // (in one case, the char sequence is 'A-Za-z', in the other it
        // is 'AaBb..Zz') - the results are therefore highly divergent.
        // To do so, we need to add some extra contacts.
        QContact contactxxy, contactXxy, contactXXY, contactXxz;
        QContactName xxn;
        xxn.setFirstName("xxy");
        contactxxy.saveDetail(&xxn);
        xxn.setFirstName("Xxy");
        contactXxy.saveDetail(&xxn);
        xxn.setFirstName("XXY");
        contactXXY.saveDetail(&xxn);
        xxn.setFirstName("Xxz");
        contactXxz.saveDetail(&xxn);
        Q_FATAL_VERIFY(cm->saveContact(&contactxxy));
        Q_FATAL_VERIFY(cm->saveContact(&contactXxy));
        Q_FATAL_VERIFY(cm->saveContact(&contactXXY));
        Q_FATAL_VERIFY(cm->saveContact(&contactXxz));

        // Now fetch sorted contacts to determine sorting semantics.
        QContactSortOrder s;
        s.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        s.setDirection(Qt::AscendingOrder);
        QList<QContact> sortedContacts = cm->contacts(s);
        int xanderIndex = -1, XanderIndex = -1, yarrowIndex = -1, YarrowIndex = -1;
        int xxyIndex = -1, XxyIndex = -1, XXYIndex = -1, XxzIndex = -1;
        for (int i = 0; i < sortedContacts.size(); ++i) {
            if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("xander")) {
                xanderIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("Xander")) {
                XanderIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("yarrow")) {
                yarrowIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("Yarrow")) {
                YarrowIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("xxy")) {
                xxyIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("Xxy")) {
                XxyIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("XXY")) {
                XXYIndex = i;
            } else if (sortedContacts[i].detail<QContactName>().firstName() == QStringLiteral("Xxz")) {
                XxzIndex = i;
            }
        }

        if (XanderIndex < YarrowIndex && YarrowIndex < xanderIndex && xanderIndex < yarrowIndex &&
                    XXYIndex < XxyIndex && XxyIndex < XxzIndex && XxzIndex < xxyIndex) {
            //qDebug() << "XYxy, and XXY<Xxy<Xxz<xxy :. upper first sorting:"
            //         << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
            //         << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = UpperFirst;
        } else if (xanderIndex < yarrowIndex && yarrowIndex < XanderIndex && XanderIndex < YarrowIndex &&
                   xxyIndex < XxyIndex && XxyIndex < XxzIndex && XxzIndex < XXYIndex) {
            //qDebug() << "xyXY, and xxy<Xxy<Xxz<XXY :. lower first sorting:"
            //         << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
            //         << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = LowerFirst;
        } else if (XanderIndex < xanderIndex && xanderIndex < YarrowIndex && YarrowIndex < yarrowIndex &&
                   XXYIndex < XxyIndex && XxyIndex < XxzIndex && XxzIndex < xxyIndex) {
            //qDebug() << "XxYy, and XXY<Xxy<Xxz<xxy :. interleaved upper first sorting without first-pass case-insensitive characterwise comparison:"
            //         << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
            //         << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = InterleavedUpperFirst;
        } else if (xanderIndex < XanderIndex && XanderIndex < yarrowIndex && yarrowIndex < YarrowIndex &&
                   xxyIndex < XxyIndex && XxyIndex < XxzIndex && XxzIndex < XXYIndex) {
            //qDebug() << "xXyY, and xxy<Xxy<Xxz<XXY :. interleaved lower first sorting without first-pass case-insensitive characterwise comparison:"
            //         << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
            //         << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = InterleavedLowerFirst;
        } else if (XanderIndex < xanderIndex && xanderIndex < YarrowIndex && YarrowIndex < yarrowIndex &&
                   XXYIndex < XxyIndex && XxyIndex < xxyIndex && xxyIndex < XxzIndex) {
            //qDebug() << "XxYy, and XXY<Xxy<xxy<Xxz :. interleaved upper first sorting without first-pass case-insensitive characterwise comparison:"
            //         << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
            //         << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = FirstPassInterleavedUpperFirst;
        } else if (xanderIndex < XanderIndex && XanderIndex < yarrowIndex && yarrowIndex < YarrowIndex &&
                   xxyIndex < XxyIndex && XxyIndex < XXYIndex && XXYIndex < XxzIndex) {
            //qDebug() << "xXyY, and xxy<Xxy<XXY<Xxz :. interleaved lower first sorting with first-pass case-insensitive characterwise comparison:"
            //         << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
            //         << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = FirstPassInterleavedLowerFirst;
        } else {
            qDebug() << "unknown sorting:"
                     << "x:" << xanderIndex << "X:" << XanderIndex << "y:" << yarrowIndex << "Y:" << YarrowIndex
                     << "xxy:" << xxyIndex << "Xxy:" << XxyIndex << "XXY:" << XXYIndex << "Xxz:" << XxzIndex;
            m_collation[cm] = UnknownCollation;
        }

        QList<QContactId> cleanup;
        cleanup << contactxxy.id() << contactXxy.id() << contactXXY.id() << contactXxz.id();
        Q_FATAL_VERIFY(cm->removeContacts(cleanup));
    }

#ifdef INCLUDE_TESTACTIONS
    qDebug() << "Finished preparing each manager for test!  About to load test actions:";
    QServiceManager sm;
    QStringList allServices = sm.findServices();
    foreach(const QString& serv, allServices) {
        if (serv.startsWith("tst_qcontactmanagerfiltering:")) {
            if (!sm.removeService(serv)) {
                qDebug() << " tst_qca: ctor: cleaning up test service" << serv << "failed:" << sm.error();
            }
        }
    }
    QStringList myServices;
    myServices << "BooleanAction" << "DateAction" << "IntegerAction" << "NumberAction" << "PhoneNumberAction";
    foreach (const QString& serv, myServices) {
        QString builtPath = QCoreApplication::applicationDirPath() + "/plugins/contacts/xmldata/" + serv.toLower() + "service.xml";
        if (!sm.addService(builtPath)) {
            qDebug() << " tst_qca: ctor: unable to add" << serv << "service:" << sm.error();
        }
    }
#endif
    qDebug() << "Done!";
}

void tst_QContactManagerFiltering::cleanupTestCase()
{
    // first, remove any contacts that we've added to any managers.
    foreach (QContactManager* manager, managers) {
        QList<QContactId> contactIds = contactsAddedToManagers.values(manager);
        manager->removeContacts(contactIds, 0);
    }
    contactsAddedToManagers.clear();

    // finally, we can delete all of our manager instances
    qDeleteAll(managers);
    managers.clear();
    defAndFieldNamesForTypePerManager.clear();

    // And restore old contacts
    managerDataHolder.reset(0);

#ifdef INCLUDE_TESTACTIONS
    // clean up any actions/services.
    QServiceManager sm;
    QStringList allServices = sm.findServices();
    foreach(const QString& serv, allServices) {
        if (serv.startsWith("tst_qcontactmanagerfiltering:")) {
            if (!sm.removeService(serv)) {
                qDebug() << " tst_qca: ctor: cleaning up test service" << serv << "failed:" << sm.error();
            }
        }
    }
#endif
}

QString tst_QContactManagerFiltering::convertIds(QList<QContactId> allIds, QList<QContactId> ids, QChar minimumContact, QChar maximumContact)
{
    QString ret;
    /* Expected is of the form "abcd".. it's possible that there are some extra contacts */
    for (int i = 0; i < ids.size(); i++) {
        if (allIds.indexOf(ids.at(i)) >= 0) {
            QChar curr = ('a' + allIds.indexOf(ids.at(i)));
            if (curr >= minimumContact && curr <= maximumContact) {
                ret += curr;
            }
        }
    }

    return ret;
}

QTestData& tst_QContactManagerFiltering::newMRow(const char *tag, QContactManager *cm)
{
    // allocate a tag
    QString foo = QString("%1[%2]").arg(tag).arg(cm->objectName());
    return QTest::newRow(foo.toLatin1().constData());
}


void tst_QContactManagerFiltering::detailStringFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QContactDetail::DetailType>("detailType");
    QTest::addColumn<int>("detailField");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<int>("matchflags");
    QTest::addColumn<QString>("expected");

    QVariant ev; // empty variant
    QString es; // empty string

    QContactDetail::DetailType name = QContactName::Type;
    int firstname = QContactName::FieldFirstName;
    int lastname = QContactName::FieldLastName;
    int middlename = QContactName::FieldMiddleName;
    int prefixname = QContactName::FieldPrefix;
    int suffixname = QContactName::FieldSuffix;
    QContactDetail::DetailType nickname = QContactNickname::Type;
    int nicknameField = QContactNickname::FieldNickname;
    QContactDetail::DetailType emailaddr = QContactEmailAddress::Type;
    int emailfield = QContactEmailAddress::FieldEmailAddress;
    QContactDetail::DetailType phonenumber = QContactPhoneNumber::Type;
    int number = QContactPhoneNumber::FieldNumber;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        newMRow("Name == Aaro", manager) << manager << name << firstname << QVariant("Aaro") << 0 << es;
        newMRow("Name == Aaron", manager) << manager << name << firstname << QVariant("Aaron") << 0 << "a";
        newMRow("Name == aaron", manager) << manager << name << firstname << QVariant("aaron") << 0 << "a";
        newMRow("Name == Aaron, case sensitive", manager) << manager << name << firstname << QVariant("Aaron") << (int)(QContactFilter::MatchCaseSensitive) << "a";
        newMRow("Name == aaron, case sensitive", manager) << manager << name << firstname << QVariant("aaron") << (int)(QContactFilter::MatchCaseSensitive) << es;

        newMRow("Name == A, begins", manager) << manager << name << firstname << QVariant("A") << (int)(QContactFilter::MatchStartsWith) << "a";
        newMRow("Name == Aaron, begins", manager) << manager << name << firstname << QVariant("Aaron") << (int)(QContactFilter::MatchStartsWith) << "a";
        newMRow("Name == aaron, begins", manager) << manager << name << firstname << QVariant("aaron") << (int)(QContactFilter::MatchStartsWith) << "a";
        newMRow("Name == Aaron, begins, case sensitive", manager) << manager << name << firstname << QVariant("Aaron") << (int)(QContactFilter::MatchStartsWith | QContactFilter::MatchCaseSensitive) << "a";
        newMRow("Name == aaron, begins, case sensitive", manager) << manager << name << firstname << QVariant("aaron") << (int)(QContactFilter::MatchStartsWith | QContactFilter::MatchCaseSensitive) << es;
        newMRow("Name == Aaron1, begins", manager) << manager << name << firstname << QVariant("Aaron1") << (int)(QContactFilter::MatchStartsWith) << es;
        newMRow("Last name == A, begins", manager) << manager << name << lastname << QVariant("A") << (int)(QContactFilter::MatchStartsWith) << "abc";
        newMRow("Last name == Aaronson, begins", manager) << manager << name << lastname << QVariant("Aaronson") << (int)(QContactFilter::MatchStartsWith) << "a";
        newMRow("Last Name == Aaronson1, begins", manager) << manager << name << lastname << QVariant("Aaronson1") << (int)(QContactFilter::MatchStartsWith) << es;

        newMRow("Name == Aar, begins", manager) << manager << name << firstname << QVariant("Aar") << (int)(QContactFilter::MatchStartsWith) << "a";
        newMRow("Name == aar, begins", manager) << manager << name << firstname << QVariant("aar") << (int)(QContactFilter::MatchStartsWith) << "a";
        newMRow("Name == Aar, begins, case sensitive", manager) << manager << name << firstname << QVariant("Aar") << (int)(QContactFilter::MatchStartsWith | QContactFilter::MatchCaseSensitive) << "a";
        newMRow("Name == aar, begins, case sensitive", manager) << manager << name << firstname << QVariant("aar") << (int)(QContactFilter::MatchStartsWith | QContactFilter::MatchCaseSensitive) << es;

        newMRow("Name == aro, contains", manager) << manager << name << firstname << QVariant("aro") << (int)(QContactFilter::MatchContains) << "a";
        newMRow("Name == ARO, contains", manager) << manager << name << firstname << QVariant("ARO") << (int)(QContactFilter::MatchContains) << "a";
        newMRow("Name == aro, contains, case sensitive", manager) << manager << name << firstname << QVariant("aro") << (int)(QContactFilter::MatchContains | QContactFilter::MatchCaseSensitive) << "a";
        newMRow("Name == ARO, contains, case sensitive", manager) << manager << name << firstname << QVariant("ARO") << (int)(QContactFilter::MatchContains | QContactFilter::MatchCaseSensitive) << es;

        newMRow("Name == ron, ends", manager) << manager << name << firstname << QVariant("ron") << (int)(QContactFilter::MatchEndsWith) << "a";
        newMRow("Name == ARON, ends", manager) << manager << name << firstname << QVariant("ARON") << (int)(QContactFilter::MatchEndsWith) << "a";
        newMRow("Name == aron, ends, case sensitive", manager) << manager << name << firstname << QVariant("aron") << (int)(QContactFilter::MatchEndsWith | QContactFilter::MatchCaseSensitive) << "a";
        newMRow("Name == ARON, ends, case sensitive", manager) << manager << name << firstname << QVariant("ARON") << (int)(QContactFilter::MatchEndsWith | QContactFilter::MatchCaseSensitive) << es;
        newMRow("Last name == n, ends", manager) << manager << name << lastname << QVariant("n") << (int)(QContactFilter::MatchEndsWith) << "abc";

        newMRow("Name == Aaron, fixed", manager) << manager << name << firstname << QVariant("Aaron") << (int)(QContactFilter::MatchFixedString) << "a";
        newMRow("Name == aaron, fixed", manager) << manager << name << firstname << QVariant("aaron") << (int)(QContactFilter::MatchFixedString) << "a";
        newMRow("Name == Aaron, fixed, case sensitive", manager) << manager << name << firstname << QVariant("Aaron") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchCaseSensitive) << "a";
        newMRow("Name == aaron, fixed, case sensitive", manager) << manager << name << firstname << QVariant("aaron") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchCaseSensitive) << es;

        // middle name
        newMRow("MName == Arne", manager) << manager << name << middlename << QVariant("Arne") << (int)(QContactFilter::MatchContains) << "a";

        // prefix
        newMRow("Prefix == Sir", manager) << manager << name << prefixname << QVariant("Sir") << (int)(QContactFilter::MatchContains) << "a";

        // suffix
        newMRow("Suffix == Dr.", manager) << manager << name << suffixname << QVariant("Dr.") << (int)(QContactFilter::MatchContains) << "a";

        // nickname
        newMRow("Nickname detail exists", manager) << manager << nickname << -1 << QVariant() << 0 << "ab";
        newMRow("Nickname == Aaron, contains", manager) << manager << nickname << nicknameField << QVariant("Aaron") << (int)(QContactFilter::MatchContains) << "a";

        // email
        newMRow("Email == Aaron@Aaronson.com", manager) << manager << emailaddr << emailfield << QVariant("Aaron@Aaronson.com") << 0 << "a";
        newMRow("Email == Aaron@Aaronsen.com", manager) << manager << emailaddr << emailfield << QVariant("Aaron@Aaronsen.com") << 0 << es;

        // phone number
        newMRow("Phone number detail exists", manager) << manager << phonenumber << -1 << QVariant("") << 0 << "ab";
        newMRow("Phone number = 5551212", manager) << manager << phonenumber << number << QVariant("5551212") << (int) QContactFilter::MatchExactly << "a";
        newMRow("Phone number = 34, contains", manager) << manager << phonenumber << number << QVariant("34") << (int) QContactFilter::MatchContains << "b";
        newMRow("Phone number = 555, starts with", manager) << manager << phonenumber << number << QVariant("555") <<  (int) QContactFilter::MatchStartsWith << "ab";
        newMRow("Phone number = 1212, ends with", manager) << manager << phonenumber << number << QVariant("1212") << (int) QContactFilter::MatchEndsWith << "a";
        newMRow("Phone number = 555-1212, match phone number", manager) << manager << phonenumber << number << QVariant("555-1212") << (int) QContactFilter::MatchPhoneNumber << "a"; // hyphens will be ignored by the match algorithm
        newMRow("Phone number = 555, keypad collation", manager) << manager << phonenumber << number << QVariant("555") << (int) (QContactFilter::MatchKeypadCollation | QContactFilter::MatchStartsWith) << "ab";

        /* Converting other types to strings */
        QPair<QContactDetail::DetailType, int> defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        if (validDetailField(defAndFieldNames)) {
            QTest::newRow("integer == 20") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("20") << 0 << es;
            QTest::newRow("integer == 20, as string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("20") << (int)(QContactFilter::MatchFixedString) << "b";
            QTest::newRow("integer == 20, begins with, string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("20") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchStartsWith) << "b";
            QTest::newRow("integer == 2, begins with, string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("2") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchStartsWith) << "b";
            QTest::newRow("integer == 20, ends with, string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("20") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchEndsWith) << "bc";
            QTest::newRow("integer == 0, ends with, string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("0") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchEndsWith) << "abc";
            QTest::newRow("integer == 20, contains, string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("20") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchContains) << "bc";
            QTest::newRow("integer == 0, contains, string") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant("0") << (int)(QContactFilter::MatchFixedString | QContactFilter::MatchContains) << "abc";
        }

        /* Detail filter semantics: empty definition or field */
        newMRow("Invalid type", manager) << manager << static_cast<QContactDetail::DetailType>(-1) << -1 << QVariant("A") << (int)(QContactFilter::MatchStartsWith) << es; // empty definition name means filter matches nothing
        newMRow("Invalid field", manager) << manager << name << -1 << QVariant("A") << (int)(QContactFilter::MatchStartsWith) << "abcdefghijk"; // invalid field matches any with a name detail
    }
}

void tst_QContactManagerFiltering::detailStringFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QContactDetail::DetailType, detailType);
    QFETCH(int, detailField);
    QFETCH(QVariant, value);
    QFETCH(int, matchflags);
    QFETCH(QString, expected);

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    QContactDetailFilter df;
    df.setDetailType(detailType, detailField);
    df.setValue(value);
    df.setMatchFlags(QContactFilter::MatchFlags(matchflags));

    if (cm->managerName() == "memory") {
        /* At this point, since we're using memory, assume the filter isn't really supported */
        QVERIFY(cm->isFilterSupported(df) == false);
    }

    ids = cm->contactIds(df);

    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QEXPECT_FAIL("integer == 20", "Not sure if this should pass or fail", Continue);
    QCOMPARE_UNSORTED(output, expected);
}

void tst_QContactManagerFiltering::detailPhoneNumberFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QContactDetail::DetailType>("detailType");
    QTest::addColumn<int>("detailField");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<int>("matchflags");
    QTest::addColumn<QString>("expected");

    // ITU-T standard keypad collation:
    // 2 = abc, 3 = def, 4 = ghi, 5 = jkl, 6 = mno, 7 = pqrs, 8 = tuv, 9 = wxyz, 0 = space

    QContactDetail::DetailType phoneType = QContactPhoneNumber::Type;
    int phoneField = QContactPhoneNumber::FieldNumber;
    QContactDetail::DetailType nameType = QContactName::Type;
    int nameField = QContactName::FieldFirstName; // just test the first name.

    // purely to test phone number filtering.
    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        // first, keypad collation testing (ITU-T / T9 testing)
        QTest::newRow("t9 aaron") << manager << nameType << nameField << QVariant(QString("22766")) << (int)(QContactFilter::MatchKeypadCollation) << "a";
        QTest::newRow("t9 bob") << manager << nameType << nameField << QVariant(QString("262")) << (int)(QContactFilter::MatchKeypadCollation) << "b";
        QTest::newRow("t9 john") << manager << nameType << nameField << QVariant(QString("5646")) << (int)(QContactFilter::MatchKeypadCollation) << "efg";
        QTest::newRow("t9 bo") << manager << nameType << nameField << QVariant(QString("26")) << (int)(QContactFilter::MatchKeypadCollation | QContactFilter::MatchStartsWith) << "bc"; // bob, boris
        QTest::newRow("t9 zzzz") << manager << nameType << nameField << QVariant(QString("9999")) << (int)(QContactFilter::MatchKeypadCollation) << ""; // nobody.

        // now do phone number matching - first, aaron's phone number
        QTest::newRow("a phone hyphen") << manager << phoneType << phoneField << QVariant(QString("555-1212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone plus") << manager << phoneType << phoneField << QVariant(QString("+5551212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone brackets") << manager << phoneType << phoneField << QVariant(QString("(555)1212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone nospaces") << manager << phoneType << phoneField << QVariant(QString("5551212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone single space") << manager << phoneType << phoneField << QVariant(QString("555 1212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone random spaces") << manager << phoneType << phoneField << QVariant(QString("55 512 12")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone every space") << manager << phoneType << phoneField << QVariant(QString("5 5 5 1 2 1 2")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone plus hyphen") << manager << phoneType << phoneField << QVariant(QString("+555-1212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone plus brackets") << manager << phoneType << phoneField << QVariant(QString("+5(55)1212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone plus brackets hyphen") << manager << phoneType << phoneField << QVariant(QString("+5(55)1-212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";
        QTest::newRow("a phone plus brackets hyphen spaces") << manager << phoneType << phoneField << QVariant(QString("+5 (55) 1-212")) << (int)(QContactFilter::MatchPhoneNumber) << "a";

        // XXX TODO: should we test for character to number conversions (eg, dial 1800-PESTCONTROL) etc ?
        //QTest::newRow("a phone characters") << manager << phoneType << phoneField << QVariant(QString("jjj1a1a")) << (int)(QContactFilter::MatchPhoneNumber) << "a"; // 5551212
        //QTest::newRow("a phone characters") << manager << phoneType << phoneField << QVariant(QString("jkl1b1a")) << (int)(QContactFilter::MatchPhoneNumber) << "a"; // 5551212

        // then matches bob's phone number
        QTest::newRow("b phone hyphen") << manager << phoneType << phoneField << QVariant(QString("555-3456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone plus") << manager << phoneType << phoneField << QVariant(QString("+5553456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone brackets") << manager << phoneType << phoneField << QVariant(QString("(555)3456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone nospaces") << manager << phoneType << phoneField << QVariant(QString("5553456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone single space") << manager << phoneType << phoneField << QVariant(QString("555 3456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone random spaces") << manager << phoneType << phoneField << QVariant(QString("55 534 56")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone every space") << manager << phoneType << phoneField << QVariant(QString("5 5 5 3 4 5 6")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone plus hyphen") << manager << phoneType << phoneField << QVariant(QString("+555-3456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone plus brackets") << manager << phoneType << phoneField << QVariant(QString("+5(55)3456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone plus brackets hyphen") << manager << phoneType << phoneField << QVariant(QString("+5(55)3-456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";
        QTest::newRow("b phone plus brackets hyphen spaces") << manager << phoneType << phoneField << QVariant(QString("+5 (55) 3-456")) << (int)(QContactFilter::MatchPhoneNumber) << "b";

        // then match no phone numbers (negative testing) -- 555-9999 matches nobody in our test set.
        QTest::newRow("no phone hyphen") << manager << phoneType << phoneField << QVariant(QString("555-9999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone plus") << manager << phoneType << phoneField << QVariant(QString("+5559999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone brackets") << manager << phoneType << phoneField << QVariant(QString("(555)9999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone nospaces") << manager << phoneType << phoneField << QVariant(QString("5559999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone single space") << manager << phoneType << phoneField << QVariant(QString("555 9999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone random spaces") << manager << phoneType << phoneField << QVariant(QString("55 599 99")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone every space") << manager << phoneType << phoneField << QVariant(QString("5 5 5 9 9 9 9")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone plus hyphen") << manager << phoneType << phoneField << QVariant(QString("+555-9999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone plus brackets") << manager << phoneType << phoneField << QVariant(QString("+5(55)9999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone plus brackets hyphen") << manager << phoneType << phoneField << QVariant(QString("+5(55)9-999")) << (int)(QContactFilter::MatchPhoneNumber) << "";
        QTest::newRow("no phone plus brackets hyphen spaces") << manager << phoneType << phoneField << QVariant(QString("+5 (55) 9-999")) << (int)(QContactFilter::MatchPhoneNumber) << "";

        // then match both aaron and bob via starts with
        QTest::newRow("ab phone starts nospace") << manager << phoneType << phoneField << QVariant(QString("555")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts hyphen") << manager << phoneType << phoneField << QVariant(QString("555-")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts space") << manager << phoneType << phoneField << QVariant(QString("55 5")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts brackets") << manager << phoneType << phoneField << QVariant(QString("(555)")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts plus") << manager << phoneType << phoneField << QVariant(QString("+555")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts hyphen space") << manager << phoneType << phoneField << QVariant(QString("5 55-")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts hyphen space brackets") << manager << phoneType << phoneField << QVariant(QString("5 (55)-")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("ab phone starts hyphen space brackets plus") << manager << phoneType << phoneField << QVariant(QString("+5 (55)-")) << (int)(QContactFilter::MatchPhoneNumber | QContactFilter::MatchStartsWith) << "ab";
    }
}

void tst_QContactManagerFiltering::detailPhoneNumberFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QContactDetail::DetailType, detailType);
    QFETCH(int, detailField);
    QFETCH(QVariant, value);
    QFETCH(int, matchflags);
    QFETCH(QString, expected);

    // note: this test is exactly the same as string filtering, but uses different fields and specific matchflags.

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    QContactDetailFilter df;
    df.setDetailType(detailType, detailField);
    df.setValue(value);
    df.setMatchFlags(QContactFilter::MatchFlags(matchflags));

    if (cm->managerName() == "memory") {
        /* At this point, since we're using memory, assume the filter isn't really supported */
        QVERIFY(cm->isFilterSupported(df) == false);
    }

    ids = cm->contactIds(df);

    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    //QSKIP("TODO: fix default implementation of phone number matching!");
    QCOMPARE_UNSORTED(output, expected);
}

void tst_QContactManagerFiltering::detailVariantFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QContactDetail::DetailType>("detailType");
    QTest::addColumn<int>("detailField");
    QTest::addColumn<bool>("setValue");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<QString>("expected");

    QVariant ev; // empty variant
    QString es; // empty string
    const int invalidField(666);

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        /* Nothings */
        newMRow("no field", manager) << manager << static_cast<QContactDetail::DetailType>(-1) << -1 << false << ev << es;
        newMRow("no type", manager) << manager << static_cast<QContactDetail::DetailType>(-1) << static_cast<int>(QContactName::FieldFirstName) << false << ev << es;

        /* Strings (name) */
        newMRow("first name presence", manager) << manager << QContactName::Type << static_cast<int>(QContactName::FieldFirstName) << false << ev << "abcdefghijk";
        newMRow("first name == Aaron", manager) << manager << QContactName::Type << static_cast<int>(QContactName::FieldFirstName) << true << QVariant("Aaron") << "a";

        /*
         * Doubles
         * B has double(4.0)
         * C has double(4.0)
         * D has double(-128.0)
         */
        QPair<QContactDetail::DetailType, int> defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Double");
        if (validDetailField(defAndFieldNames)) {
            newMRow("double presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "bcd";
            QTest::newRow("double presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "bcd";
            QTest::newRow("double presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("double value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("double value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(QDateTime()) << es;
            QTest::newRow("double value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(3.5) << es;
            newMRow("double value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(4.0) << "bc";
            QTest::newRow("double value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(4.0) << es;
            QTest::newRow("double value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(-128.0) << "d";
            QTest::newRow("double value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(-128.0) << es;
        }

        /*
         * Integers
         * A has 10
         * B has 20
         * C has -20
         */
        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        if (validDetailField(defAndFieldNames)) {
            newMRow("integer presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "abc";
            QTest::newRow("integer presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "abc";
            QTest::newRow("integer presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("integer value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(50) << es;
            QTest::newRow("integer value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("integer value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(50) << es;
            newMRow("integer value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(10) << "a";
            QTest::newRow("integer value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(10) << es;
            QTest::newRow("integer value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(-20) << "c";
            QTest::newRow("integer value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(-20) << es;
        }

        /*
         * Date times
         * A has QDateTime(QDate(2009, 06, 29), QTime(16, 52, 23, 0))
         * C has QDateTime(QDate(2009, 06, 29), QTime(16, 54, 17, 0))
         */
        const QDateTime adt(QDate(2009, 06, 29), QTime(16, 52, 23, 0));
        const QDateTime cdt(QDate(2009, 06, 29), QTime(16, 54, 17, 0));

        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("DateTime");
        if (validDetailField(defAndFieldNames)) {
            newMRow("datetime presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "ac";
            QTest::newRow("datetime presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "ac";
            QTest::newRow("datetime presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("datetime value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(QDateTime(QDate(2100,5,13), QTime(5,5,5))) << es;
            QTest::newRow("datetime value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("datetime value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(QDateTime(QDate(2100,5,13), QTime(5,5,5))) << es;
            newMRow("datetime value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(adt) << "a";
            QTest::newRow("datetime value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(adt) << es;
            QTest::newRow("datetime value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(cdt)<< "c";
            QTest::newRow("datetime value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(cdt) << es;
        }

        /*
         * Dates
         * A has QDate(1988, 1, 26)
         * B has QDate(2492, 5, 5)
         * D has QDate(2770, 10, 1)
         */
        const QDate ad(1988, 1, 26);
        const QDate bd(2492, 5, 5);
        const QDate dd(2770, 10, 1);

        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Date");
        if (validDetailField(defAndFieldNames)) {
            newMRow("date presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "abd";
            QTest::newRow("date presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "abd";
            QTest::newRow("date presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("date value (no match)") << manager << defAndFieldNames.first <<defAndFieldNames.second << true << QVariant(QDate(2100,5,13)) << es;
            QTest::newRow("date value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("date value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(QDate(2100,5,13)) << es;
            newMRow("date value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(ad) << "a";
            QTest::newRow("date value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(ad) << es;
            QTest::newRow("date value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(bd) << es;
            QTest::newRow("date value 3 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(dd) << es;
            QTest::newRow("date value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(bd)<< "b";
            QTest::newRow("date value 3") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(dd)<< "d";
        }

        /*
         * Times
         * A has QTime(16,52,23,0)
         * B has QTime(15,52,23,0)
         */
        const QTime at = QTime(16,52,23,0);
        const QTime bt = QTime(15,52,23,0);

        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Time");
        if (validDetailField(defAndFieldNames)) {
            newMRow("time presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "ab";
            QTest::newRow("time presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "ab";
            QTest::newRow("time presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("time value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(QTime(5,5,5)) << es;
            QTest::newRow("time value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("time value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(QTime(5,5,5)) << es;
            newMRow("time value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(at) << "a";
            QTest::newRow("time value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(at) << es;
            QTest::newRow("time value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(bt)<< "b";
            QTest::newRow("time value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(bt) << es;
        }


        /*
         * Bool
         * A has bool(true)
         * B has bool(false)
         * C has bool(false)
         */
        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Bool");
        if (validDetailField(defAndFieldNames)) {
            newMRow("bool presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "abc";
            QTest::newRow("bool presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "abc";
            QTest::newRow("bool presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("bool value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(4.0) << es;
            newMRow("bool value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(true) << "a";
            QTest::newRow("bool value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(true) << es;
            QTest::newRow("bool value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(false) << "bc";
            QTest::newRow("bool value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(false) << es;
        }

        /*
         * LongLong
         * C has LongLong(8000000000LL)
         * D has LongLong(-14000000000LL)
         */
        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("LongLong");
        if (validDetailField(defAndFieldNames)) {
            newMRow("longlong presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "cd";
            QTest::newRow("longlong presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "cd";
            QTest::newRow("longlong presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("longlong value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(50000000000LL) << es;
            QTest::newRow("longlong value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("longlong value (wrong field, no match)") << manager << defAndFieldNames.first<< invalidField << true << QVariant(50000000000LL) << es;
            newMRow("longlong value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(8000000000LL) << "c";
            QTest::newRow("longlong value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(8000000000LL) << es;
            QTest::newRow("longlong value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(-14000000000LL) << "d";
            QTest::newRow("longlong value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(-14000000000LL) << es;
        }

        /*
         * ULongLong
         * A has ULongLong(120000000000ULL)
         * B has ULongLong(80000000000ULL)
         * C has ULongLong(80000000000ULL)
         */
        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("ULongLong");
        if (validDetailField(defAndFieldNames)) {
            newMRow("ulonglong presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "abc";
            QTest::newRow("ulonglong presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "abc";
            QTest::newRow("ulonglong presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("ulonglong value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(50000000000ULL) << es;
            QTest::newRow("ulonglong value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("ulonglong value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(50000000000ULL) << es;
            newMRow("ulonglong value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(120000000000ULL) << "a";
            QTest::newRow("ulonglong value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(120000000000ULL) << es;
            QTest::newRow("ulonglong value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(80000000000ULL) << "bc";
            QTest::newRow("ulonglong value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(80000000000ULL) << es;
        }

        /*
         * UInt
         * B has UInt(4000000000u)
         * D has UInt(3000000000u)
         */
        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("UInt");
        if (validDetailField(defAndFieldNames)) {
            newMRow("unsigned integer presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "bd";
            QTest::newRow("unsigned integer presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "bd";
            QTest::newRow("unsigned integer presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("unsigned integer value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3500000000u) << es;
            QTest::newRow("unsigned integer value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("unsigned integer value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(3500000000u) << es;
            newMRow("unsigned integer value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(4000000000u) << "b";
            QTest::newRow("unsigned integer value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(4000000000u) << es;
            QTest::newRow("unsigned integer value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3000000000u) << "d";
            QTest::newRow("unsigned integer value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(3000000000u) << es;
        }

        /*
         * Char
         * B has QChar('b')
         * C has QChar('c')
         */
        const QChar bchar('b');
        const QChar cchar('c');
        defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Char");
        if (validDetailField(defAndFieldNames)) {
            newMRow("char presence", manager) << manager << defAndFieldNames.first << -1 << false << ev << "bc";
            QTest::newRow("char presence (inc field)") << manager << defAndFieldNames.first << defAndFieldNames.second << false << ev << "bc";
            QTest::newRow("char presence (wrong field)") << manager << defAndFieldNames.first << invalidField << false << ev << es;
            QTest::newRow("char value (no match)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(QChar('a')) << es;
            QTest::newRow("char value (wrong type)") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(3.5) << es;
            QTest::newRow("char value (wrong field, no match)") << manager << defAndFieldNames.first << invalidField << true << QVariant(QChar('a')) << es;
            newMRow("char value", manager) << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(bchar) << "b";
            QTest::newRow("char value (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(bchar) << es;
            QTest::newRow("char value 2") << manager << defAndFieldNames.first << defAndFieldNames.second << true << QVariant(cchar)<< "c";
            QTest::newRow("char value 2 (wrong field)") << manager << defAndFieldNames.first << invalidField << true << QVariant(cchar) << es;
        }
    }
}

void tst_QContactManagerFiltering::detailVariantFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QContactDetail::DetailType, detailType);
    QFETCH(int, detailField);
    QFETCH(bool, setValue);
    QFETCH(QVariant, value);
    QFETCH(QString, expected);

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    QContactDetailFilter df;
    df.setDetailType(detailType, detailField);
    if (setValue)
        df.setValue(value);

    if (cm->managerName() == "memory") {
        /* At this point, since we're using memory, assume the filter isn't really supported */
        QVERIFY(cm->isFilterSupported(df) == false);
    }

    ids = cm->contactIds(df);

    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QCOMPARE_UNSORTED(output, expected);
}

void tst_QContactManagerFiltering::rangeFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QContactDetail::DetailType>("detailType");
    QTest::addColumn<int>("detailField");
    QTest::addColumn<QVariant>("minrange");
    QTest::addColumn<QVariant>("maxrange");
    QTest::addColumn<bool>("setrfs");
    QTest::addColumn<int>("rangeflagsi");
    QTest::addColumn<bool>("setmfs");
    QTest::addColumn<int>("matchflagsi");
    QTest::addColumn<QString>("expected");

    QVariant ev; // empty variant
    QString es; // empty string
    const int invalidField(666);

    QContactDetail::DetailType nameType = QContactName::Type;
    int firstname = QContactName::FieldFirstName;

    QContactDetail::DetailType phoneType = QContactPhoneNumber::Type;
    int phonenum = QContactPhoneNumber::FieldNumber;

    int csflag = (int)QContactFilter::MatchCaseSensitive;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        /* First, cover the "empty detailType / detailField / ranges" cases */
        newMRow("invalid detailType", manager) << manager << QContactDetail::TypeUndefined << firstname << QVariant("A") << QVariant("Bob") << false << 0 << true << 0 << es;
        newMRow("defn presence test", manager) << manager << nameType << -1 << QVariant("A") << QVariant("Bob") << false << 0 << true << 0 << "abcdefghijk";
        newMRow("field presence test", manager) << manager << phoneType << phonenum << QVariant() << QVariant() << false << 0 << true << 0 << "ab";
        newMRow("good type, bad field", manager) << manager << nameType << invalidField << QVariant("A") << QVariant("Bob") << false << 0 << true << 0 << es;
        newMRow("bad def", manager) << manager << static_cast<QContactDetail::DetailType>(-1) << -1 << QVariant("A") << QVariant("Bob") << false << 0 << true << 0 << es;

        /* Presence for fields that aren't there */
        newMRow("defn presence test negative", manager) << manager << QContactFamily::Type << -1 << ev << ev << false << 0 << false << 0 << es;
        newMRow("field presence test negative", manager) << manager << QContactFamily::Type << invalidField << ev << ev << false << 0 << false << 0 << es;
        newMRow("defn yes, field no presence test negative", manager) << manager << nameType << invalidField << ev << ev << false << 0 << false << 0 << es;

        newMRow("no max, all results", manager) << manager << nameType << firstname << QVariant("a") << QVariant() << false << 0 << true << 0 << "abcdefghijk";
        newMRow("no max, some results", manager) << manager << nameType << firstname << QVariant("bob") << QVariant() << false << 0 << true << 0 << "bcdefghijk";
        newMRow("no max, no results", manager) << manager << nameType << firstname << QVariant("ZamBeZI") << QVariant() << false << 0 << true << 0 << es;
        newMRow("no min, all results", manager) << manager << nameType << firstname << QVariant() << QVariant("zambezi") << false << 0 << true << 0 << "abcdefghijk";
        newMRow("no min, some results", manager) << manager << nameType << firstname << QVariant() << QVariant("bOb") << false << 0 << true << 0 << "a";
        newMRow("no min, no results", manager) << manager << nameType << firstname << QVariant() << QVariant("aardvark") << false << 0 << true << 0 << es;

        /* now case sensitive */
        newMRow("no max, cs, all results", manager) << manager << nameType << firstname << QVariant("A") << QVariant() << false << 0 << true << csflag << "abcdefghijk";
        newMRow("no max, cs, some results", manager) << manager << nameType << firstname << QVariant("Bob") << QVariant() << false << 0 << true << csflag << "bcdefghijk";
        newMRow("no max, cs, no results", manager) << manager << nameType << firstname << QVariant("Xambezi") << QVariant() << false << 0 << true << csflag << "hijk";
        newMRow("no min, cs, most results", manager) << manager << nameType << firstname << QVariant() << QVariant("Xambezi") << false << 0 << true << csflag << "abcdefg";
        newMRow("no min, cs, some results", manager) << manager << nameType << firstname << QVariant() << QVariant("Bob") << false << 0 << true << csflag << "a";
        newMRow("no min, cs, no results", manager) << manager << nameType << firstname << QVariant() << QVariant("Aardvark") << false << 0 << true << csflag << es;
        newMRow("no max, cs, badcase, all results", manager) << manager << nameType << firstname << QVariant("A") << QVariant() << false << 0 << true << csflag << "abcdefghijk";
        newMRow("no max, cs, badcase, some results", manager) << manager << nameType << firstname << QVariant("bob") << QVariant() << false << 0 << true << csflag << expectedSorting(manager, "hj", "abcdefghijk", "defghijk", "bcdefghijk", "cdefghijk", "bcdefghijk");
        newMRow("no max, cs, badcase, no results", manager) << manager << nameType << firstname << QVariant("XAMBEZI") << QVariant() << false << 0 << true << csflag << "hijk";
        newMRow("no min, cs, badcase, all results", manager) << manager << nameType << firstname << QVariant() << QVariant("XAMBEZI") << false << 0 << true << csflag << "abcdefg";
        newMRow("no min, cs, badcase, some results", manager) << manager << nameType << firstname << QVariant() << QVariant("BOB") << false << 0 << true << csflag << expectedSorting(manager, "a", "abchj", "a", "abc", "a", "ab");
        newMRow("no min, cs, badcase, no results", manager) << manager << nameType << firstname << QVariant() << QVariant("AARDVARK") << false << 0 << true << csflag << es;

        /* 'a' has phone number ("5551212") */
        QTest::newRow("range1") << manager << phoneType << phonenum << QVariant("5551200") << QVariant("5551220") << false << 0 << false << 0 << "a";

        /* A(Aaron Aaronson), B(Bob Aaronsen), C(Boris Aaronsun), D(Dennis FitzMacyntire) */
        // string range matching - no matchflags set.
        QTest::newRow("string range - no matchflags - 1") << manager << nameType << firstname << QVariant("A") << QVariant("Bob") << false << 0 << true << 0 << "a";
        QTest::newRow("string range - no matchflags - 2") << manager << nameType << firstname << QVariant("A") << QVariant("Bob") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << 0 << "a";
        QTest::newRow("string range - no matchflags - 3") << manager << nameType << firstname << QVariant("A") << QVariant("Bob") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << 0 << "a";
        QTest::newRow("string range - no matchflags - 4") << manager << nameType << firstname << QVariant("A") << QVariant("Bob") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << true << 0 << "ab";
        QTest::newRow("string range - no matchflags - 5") << manager << nameType << firstname << QVariant("A") << QVariant("Bob") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::IncludeUpper) << true << 0 << "ab";
        QTest::newRow("string range - no matchflags - 6") << manager << nameType << firstname << QVariant("Bob") << QVariant("Boris") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::IncludeUpper) << true << 0 << "c";
        QTest::newRow("string range - no matchflags - 7") << manager << nameType << firstname << QVariant("Bob") << QVariant("Boris") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << 0 << "b";
        QTest::newRow("string range - no matchflags - 8") << manager << nameType << firstname << QVariant("Bob") << QVariant("Boris") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << true << 0 << "bc";
        QTest::newRow("string range - no matchflags - 9") << manager << nameType << firstname << QVariant("Bob") << QVariant("Boris") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << 0 << "";
        QTest::newRow("string range - no matchflags - 10") << manager << nameType << firstname << QVariant("Barry") << QVariant("C") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << 0 << "bc";

        // string range matching - QContactFilter::MatchStartsWith should produce the same results as without matchflags set.
        QTest::newRow("string range - startswith - 1") << manager << nameType << firstname << QVariant("A") << QVariant("Bo") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "a";
        QTest::newRow("string range - startswith - 2") << manager << nameType << firstname << QVariant("A") << QVariant("Bo") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "a";
        QTest::newRow("string range - startswith - 3") << manager << nameType << firstname << QVariant("A") << QVariant("Bo") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("string range - startswith - 4") << manager << nameType << firstname << QVariant("A") << QVariant("Bo") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::IncludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "ab";
        QTest::newRow("string range - startswith - 5") << manager << nameType << firstname << QVariant("Bo") << QVariant("C") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::IncludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "c";
        QTest::newRow("string range - startswith - 6") << manager << nameType << firstname << QVariant("Bo") << QVariant("C") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "bc";
        QTest::newRow("string range - startswith - 7") << manager << nameType << firstname << QVariant("Bo") << QVariant("C") << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "bc";
        QTest::newRow("string range - startswith - 8") << manager << nameType << firstname << QVariant("Bo") << QVariant("C") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "c";
        QTest::newRow("string range - startswith - 9") << manager << nameType << firstname << QVariant("Barry") << QVariant("C") << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "bc";

        // Open ended starts with
        QTest::newRow("string range - startswith open top - 1") << manager << nameType << firstname << QVariant("A") << ev << true << (int)(QContactDetailRangeFilter::IncludeLower) << true << (int)(QContactFilter::MatchStartsWith) << "abcdefghijk";
        QTest::newRow("string range - startswith open top - 2") << manager << nameType << firstname << QVariant("A") << ev << true << (int)(QContactDetailRangeFilter::ExcludeLower) << true << (int)(QContactFilter::MatchStartsWith) << "abcdefghijk";
        QTest::newRow("string range - startswith open top - 3") << manager << nameType << firstname << QVariant("Aaron") << ev << true << (int)(QContactDetailRangeFilter::IncludeLower) << true << (int)(QContactFilter::MatchStartsWith) << "abcdefghijk";
        QTest::newRow("string range - startswith open top - 4") << manager << nameType << firstname << QVariant("Aaron") << ev << true << (int)(QContactDetailRangeFilter::ExcludeLower) << true << (int)(QContactFilter::MatchStartsWith) << "bcdefghijk";
        QTest::newRow("string range - startswith open bottom - 1") << manager << nameType << firstname << ev << QVariant("Borit") << true << (int)(QContactDetailRangeFilter::IncludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "abc";
        QTest::newRow("string range - startswith open bottom - 2") << manager << nameType << firstname << ev << QVariant("Borit") << true << (int)(QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "abc";
        QTest::newRow("string range - startswith open bottom - 3") << manager << nameType << firstname << ev << QVariant("Boris") << true << (int)(QContactDetailRangeFilter::IncludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "abc";
        QTest::newRow("string range - startswith open bottom - 4") << manager << nameType << firstname << ev << QVariant("Boris") << true << (int)(QContactDetailRangeFilter::ExcludeUpper) << true << (int)(QContactFilter::MatchStartsWith) << "ab";

        /* A(10), B(20), C(-20) */
        // Now integer range testing
        QPair<QContactDetail::DetailType, int> defAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        if (validDetailField(defAndFieldNames)) {
            QTest::newRow("int range - no rangeflags - 1") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(9) << QVariant(9) << false << 0 << false << 0 << es;
            QTest::newRow("int range - no rangeflags - 2") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(9) << QVariant(10) << false << 0 << false << 0 << es;
            QTest::newRow("int range - no rangeflags - 3") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(9) << QVariant(11) << false << 0 << false << 0 << "a";
            QTest::newRow("int range - no rangeflags - 4") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(10) << QVariant(10) << false << 0 << false << 0 << es;
            QTest::newRow("int range - rangeflags - 1") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(10) << QVariant(10) << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper) << false << 0 << es;
            QTest::newRow("int range - rangeflags - 2") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(10) << QVariant(10) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::ExcludeUpper) << false << 0 << es;
            QTest::newRow("int range - rangeflags - 3") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(10) << QVariant(10) << true << (int)(QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << es;
            QTest::newRow("int range - rangeflags - 4") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(10) << QVariant(10) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << "a";
            QTest::newRow("int range - rangeflags - 5") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(10) << QVariant(11) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << "a";
            QTest::newRow("int range - rangeflags - 6") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(11) << QVariant(11) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << es;
            QTest::newRow("int range - rangeflags - 7") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(-30) << QVariant(-19) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << "c";
            QTest::newRow("int range - rangeflags - 8") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(-20) << QVariant(-30) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << es;
            QTest::newRow("int range - rangeflags - variant - 1") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant(9) << QVariant() << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << "ab";
            QTest::newRow("int range - rangeflags - variant - 2") << manager << defAndFieldNames.first << defAndFieldNames.second << QVariant() << QVariant(11) << true << (int)(QContactDetailRangeFilter::IncludeLower | QContactDetailRangeFilter::IncludeUpper) << false << 0 << "ac";
        }
    }
}

void tst_QContactManagerFiltering::rangeFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QContactDetail::DetailType, detailType);
    QFETCH(int, detailField);
    QFETCH(QVariant, minrange);
    QFETCH(QVariant, maxrange);
    QFETCH(bool, setrfs);
    QFETCH(int, rangeflagsi);
    QFETCH(bool, setmfs);
    QFETCH(int, matchflagsi);
    QFETCH(QString, expected);

    QContactDetailRangeFilter::RangeFlags rangeflags = (QContactDetailRangeFilter::RangeFlags)rangeflagsi;
    QContactFilter::MatchFlags matchflags = (QContactFilter::MatchFlags) matchflagsi;

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    /* Build the range filter */
    QContactDetailRangeFilter drf;
    drf.setDetailType(detailType, detailField);
    if (setrfs)
        drf.setRange(minrange, maxrange, rangeflags);
    else
        drf.setRange(minrange, maxrange);
    if (setmfs)
        drf.setMatchFlags(matchflags);

    if (cm->managerName() == "memory") {
        /* At this point, since we're using memory, assume the filter isn't really supported */
        QVERIFY(cm->isFilterSupported(drf) == false);
    }
    ids = cm->contactIds(drf);

    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QEXPECT_FAIL("string range - startswith - 3", "No handling for startsWith exists in QContactDetailRangeFilter implementation", Continue);
    QEXPECT_FAIL("string range - startswith - 4", "No handling for startsWith exists in QContactDetailRangeFilter implementation", Continue);
    QEXPECT_FAIL("string range - startswith - 5", "No handling for startsWith exists in QContactDetailRangeFilter implementation", Continue);
    QEXPECT_FAIL("string range - startswith - 8", "No handling for startsWith exists in QContactDetailRangeFilter implementation", Continue);
    QCOMPARE_UNSORTED(output, expected);
}

void tst_QContactManagerFiltering::intersectionFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<bool>("firstfilter");
    QTest::addColumn<int>("fftype"); // 1 = detail, 2 = detailrange, 3 = groupmembership, 4 = union, 5 = intersection
    QTest::addColumn<QContactDetail::DetailType>("ffdetailtype");
    QTest::addColumn<int>("ffdetailfield");
    QTest::addColumn<bool>("ffsetvalue");
    QTest::addColumn<QVariant>("ffvalue");
    QTest::addColumn<QVariant>("ffminrange");
    QTest::addColumn<QVariant>("ffmaxrange");
    QTest::addColumn<bool>("secondfilter");
    QTest::addColumn<int>("sftype");
    QTest::addColumn<QContactDetail::DetailType>("sfdetailtype");
    QTest::addColumn<int>("sfdetailfield");
    QTest::addColumn<bool>("sfsetvalue");
    QTest::addColumn<QVariant>("sfvalue");
    QTest::addColumn<QVariant>("sfminrange");
    QTest::addColumn<QVariant>("sfmaxrange");
    QTest::addColumn<QString>("order");
    QTest::addColumn<QString>("expected");

    QString es; // empty string.

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        // for the following tests, terminology:
        // X will be an (empty) intersection filter created in the test
        // Y will be the first filter defined here
        // Z will be the second filter defined here

        // WITH Y AND Z AS DETAIL FILTERS (with no overlap between Y and Z results)
        // For these tests, Y matches "bc" and Z matches "a"
        // X && Y - X empty so es
        QPair<QContactDetail::DetailType, int> integerDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        QPair<QContactDetail::DetailType, int> booleanDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Bool");
        if (validDetailField(integerDefAndFieldNames) && validDetailField(booleanDefAndFieldNames)) {
            QTest::newRow("A1") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(10) << QVariant() << QVariant()
                                << "XY" << es;
            // Y && X - X empty so es
            QTest::newRow("A2") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(10) << QVariant() << QVariant()
                                << "YX" << es;
            // Y && Z  - matches "a" and "bc" - so intersected = es
            QTest::newRow("A3") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "YZ" << es;
            // Z && Y - matches "bc" and "a" - so intersected = es
            QTest::newRow("A4") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "ZY" << es;
            // X && Z - X empty so es
            QTest::newRow("A5") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "XZ" << es;
            // Z && X - X empty so es
            QTest::newRow("A6") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "ZX" << es;
            // X && Y && Z - X empty so es
            QTest::newRow("A7") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "XYZ" << es;
            // X && Z && Y - X empty so es
            QTest::newRow("A8") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "XZY" << es;
            // Y && X && Z - X empty so es
            QTest::newRow("A9") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "YXZ" << es;
            // Z && X && Y - X empty so es
            QTest::newRow("A10") << manager
                                 << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                 << "ZXY" << es;
            // Y && Z && X - X empty so es
            QTest::newRow("A11") << manager
                                 << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                 << "YZX" << es;
            // Z && Y && X - X empty so es
            QTest::newRow("A12") << manager
                                 << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                 << "ZYX" << es;

            // WITH Y AND Z AS DETAIL FILTERS (with some overlap between Y and Z results)
            // For these tests, Y matches "bc", Z matches "b"
            // X && Y - X empty so es
            QTest::newRow("B1") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(20) << QVariant() << QVariant()
                                << "XY" << es;
            // Y && X - X empty so es
            QTest::newRow("B2") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(20) << QVariant() << QVariant()
                                << "YX" << es;
            // Y && Z  - matches "b" and "bc" - so intersected = "b"
            QTest::newRow("B3") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "YZ" << "b";
            // Z && Y - matches "bc" and "b" - so intersected = "b"
            QTest::newRow("B4") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "ZY" << "b";
            // X && Z - X empty so es
            QTest::newRow("B5") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "XZ" << es;
            // Z && X - X empty so es
            QTest::newRow("B6") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "ZX" << es;
            // X && Y && Z - X empty so es
            QTest::newRow("B7") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "XYZ" << es;
            // X && Z && Y - X empty so es
            QTest::newRow("B8") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "XZY" << es;
            // Y && X && Z - X empty so es
            QTest::newRow("B9") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "YXZ" << es;
            // Z && X && Y - X empty so es
            QTest::newRow("B10") << manager
                                 << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                 << "ZXY" << es;
            // Y && Z && X - X empty so es
            QTest::newRow("B11") << manager
                                 << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                 << "YZX" << es;
            // Z && Y && X - X empty so es
            QTest::newRow("B12") << manager
                                 << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                 << "ZYX" << es;
        }

        //---------------------------

        // WITH Y AND Z AS RANGE FILTERS (with no overlap between Y and Z results)
        // For these tests, Y matches "a", Z matches "b"
        // X && Y - X empty so es
        if (validDetailField(integerDefAndFieldNames)) {
            QTest::newRow("C1") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XY" << es;
            // Y && X - X empty so es
            QTest::newRow("C2") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YX" << es;
            // Y && Z - no overlap so es
            QTest::newRow("C3") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YZ" << es;
            // Z && Y - no overlap so es
            QTest::newRow("C4") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZY" << es;
            // X && Z - X empty so es
            QTest::newRow("C5") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZ" << es;
            // Z && X - X empty so es
            QTest::newRow("C6") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZX" << es;
            // X && Y && Z - X empty so es
            QTest::newRow("C7") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XYZ" << es;
            // X && Z && Y - X empty so es
            QTest::newRow("C8") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZY" << es;
            // Y && X && Z - X empty so es
            QTest::newRow("C9") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YXZ" << es;
            // Z && X && Y - X empty so es
            QTest::newRow("C10") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZXY" << es;
            // Y && Z && X - X empty so es
            QTest::newRow("C11") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "YZX" << es;
            // Z && Y && X - X empty so es
            QTest::newRow("C12") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZYX" << es;

            // WITH Y AND Z AS RANGE FILTERS (with some overlap between Y and Z results)
            // For these tests, Y matches "ab", Z matches "b"
            // X && Y - X empty so es
            QTest::newRow("D1") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XY" << es;
            // Y && X - X empty so es
            QTest::newRow("D2") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YX" << es;
            // Y && Z - Y matches "ab", Z matches "b", intersection = "b"
            QTest::newRow("D3") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YZ" << "b";
            // Z && Y - Y matches "ab", Z matches "b", intersection = "b"
            QTest::newRow("D4") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZY" << "b";
            // X && Z - X empty so es
            QTest::newRow("D5") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZ" << es;
            // Z && X - X empty so es
            QTest::newRow("D6") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZX" << es;
            // X && Y && Z - X empty so es
            QTest::newRow("D7") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XYZ" << es;
            // X && Z && Y - X empty so es
            QTest::newRow("D8") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZY" << es;
            // Y && X && Z - X empty so es
            QTest::newRow("D9") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YXZ" << es;
            // Z && X && Y - X empty so es
            QTest::newRow("D10") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZXY" << es;
            // Y && Z && X - X empty so es
            QTest::newRow("D11") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "YZX" << es;
            // Z && Y && X - X empty so es
            QTest::newRow("D12") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZYX" << es;
        }
    }
}

void tst_QContactManagerFiltering::intersectionFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(bool, firstfilter);
    QFETCH(int, fftype); // 1 = detail, 2 = detailrange, 3 = groupmembership, 4 = union, 5 = intersection
    QFETCH(QContactDetail::DetailType, ffdetailtype);
    QFETCH(int, ffdetailfield);
    QFETCH(bool, ffsetvalue);
    QFETCH(QVariant, ffvalue);
    QFETCH(QVariant, ffminrange);
    QFETCH(QVariant, ffmaxrange);
    QFETCH(bool, secondfilter);
    QFETCH(int, sftype);
    QFETCH(QContactDetail::DetailType, sfdetailtype);
    QFETCH(int, sfdetailfield);
    QFETCH(bool, sfsetvalue);
    QFETCH(QVariant, sfvalue);
    QFETCH(QVariant, sfminrange);
    QFETCH(QVariant, sfmaxrange);
    QFETCH(QString, order);
    QFETCH(QString, expected);

    QContactFilter *x = new QContactIntersectionFilter();
    QContactFilter *y = 0, *z = 0;

    if (firstfilter) {
        switch (fftype) {
            case 1: // detail filter
                y = new QContactDetailFilter();
                static_cast<QContactDetailFilter*>(y)->setDetailType(ffdetailtype, ffdetailfield);
                if (ffsetvalue)
                    static_cast<QContactDetailFilter*>(y)->setValue(ffvalue);
                break;
            case 2: // range filter
                y = new QContactDetailRangeFilter();
                static_cast<QContactDetailRangeFilter*>(y)->setDetailType(ffdetailtype, ffdetailfield);
                static_cast<QContactDetailRangeFilter*>(y)->setRange(ffminrange, ffmaxrange);
                break;
            case 3: // group membership filter
            case 4: // union filter
            case 5: // intersection filter
                break;

            default:
                QVERIFY(false); // force fail.
            break;
        }
    }

    if (secondfilter) {
        switch (sftype) {
            case 1: // detail filter
                z = new QContactDetailFilter();
                static_cast<QContactDetailFilter*>(z)->setDetailType(sfdetailtype, sfdetailfield);
                if (sfsetvalue)
                    static_cast<QContactDetailFilter*>(z)->setValue(sfvalue);
                break;
            case 2: // range filter
                z = new QContactDetailRangeFilter();
                static_cast<QContactDetailRangeFilter*>(z)->setDetailType(sfdetailtype, sfdetailfield);
                static_cast<QContactDetailRangeFilter*>(z)->setRange(sfminrange, sfmaxrange);
                break;
            case 3: // group membership filter
            case 4: // union filter
            case 5: // intersection filter
                break;

            default:
                QVERIFY(false); // force fail.
            break;
        }
    }

    // control variables - order: starts, ends, mids
    bool sX = false;
    bool sY = false;
    bool sZ = false;
    bool eX = false;
    bool eY = false;
    bool eZ = false;
    bool mX = false;
    bool mY = false;
    bool mZ = false;

    if (order.startsWith("X"))
        sX = true;
    if (order.startsWith("Y"))
        sY = true;
    if (order.startsWith("Z"))
        sZ = true;
    if (order.endsWith("X"))
        eX = true;
    if (order.endsWith("Y"))
        eY = true;
    if (order.endsWith("Z"))
        eZ = true;
    if (order.size() > 2) {
        if (order.at(1) == 'X')
            mX = true;
        if (order.at(1) == 'Y')
            mY = true;
        if (order.at(1) == 'Z')
            mZ = true;
    }

    // now perform the filtering.
    QContactIntersectionFilter resultFilter;
    if (sX) {
        if (mY && eZ)
            resultFilter = *x & *y & *z;
        else if (mZ && eY)
            resultFilter = *x & *z & *y;
        else if (eY)
            resultFilter = *x & *y;
        else if (eZ)
            resultFilter = *x & *z;
    } else if (sY) {
        if (mX && eZ)
            resultFilter = *y & *x & *z;
        else if (mZ && eX)
            resultFilter = *y & *z & *x;
        else if (eX)
            resultFilter = *y & *x;
        else if (eZ)
            resultFilter = *y & *z;
    } else if (sZ) {
        if (mX && eY)
            resultFilter = *z & *x & *y;
        else if (mY && eX)
            resultFilter = *z & *y & *x;
        else if (eX)
            resultFilter = *z & *x;
        else if (eY)
            resultFilter = *z & *y;
    }

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    ids = cm->contactIds(resultFilter);

    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QCOMPARE_UNSORTED(output, expected);

    delete x;
    if (y) delete y;
    if (z) delete z;
}

void tst_QContactManagerFiltering::unionFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<bool>("firstfilter");
    QTest::addColumn<int>("fftype"); // 1 = detail, 2 = detailrange, 3 = groupmembership, 4 = union, 5 = intersection
    QTest::addColumn<QContactDetail::DetailType>("ffdetailtype");
    QTest::addColumn<int>("ffdetailfield");
    QTest::addColumn<bool>("ffsetvalue");
    QTest::addColumn<QVariant>("ffvalue");
    QTest::addColumn<QVariant>("ffminrange");
    QTest::addColumn<QVariant>("ffmaxrange");
    QTest::addColumn<bool>("secondfilter");
    QTest::addColumn<int>("sftype");
    QTest::addColumn<QContactDetail::DetailType>("sfdetailtype");
    QTest::addColumn<int>("sfdetailfield");
    QTest::addColumn<bool>("sfsetvalue");
    QTest::addColumn<QVariant>("sfvalue");
    QTest::addColumn<QVariant>("sfminrange");
    QTest::addColumn<QVariant>("sfmaxrange");
    QTest::addColumn<QString>("order");
    QTest::addColumn<QString>("expected");

    QString es; // empty string.

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        // for the following tests, terminology:
        // X will be an (empty) union filter created in the test
        // Y will be the first filter defined here
        // Z will be the second filter defined here

        // WITH Y AND Z AS DETAIL FILTERS (with no overlap between Y and Z results)
        // For these tests, Y matches "bc" and Z matches "a"
        // X || Y - X empty, Y matches "bc" so union = "bc"
        QPair<QContactDetail::DetailType, int> integerDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        QPair<QContactDetail::DetailType, int> booleanDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Bool");
        if (validDetailField(integerDefAndFieldNames) && validDetailField(booleanDefAndFieldNames)) {
            QTest::newRow("A1") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(10) << QVariant() << QVariant()
                                << "XY" << "bc";
            // Y || X - Y matches "bc", X empty so union = "bc"
            QTest::newRow("A2") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(10) << QVariant() << QVariant()
                                << "YX" << "bc";
            // Y || Z  - Y matches "bc" and Z matches "a" - so union = "abc"
            QTest::newRow("A3") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "YZ" << "abc";
            // Z || Y - Y matches "bc" and Z matches "a" - so union = "abc"
            QTest::newRow("A4") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "ZY" << "abc";
            // X || Z - X empty, Z matches "a" so "a"
            QTest::newRow("A5") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << false << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "XZ" << "a";
            // Z || X - X empty, Z matches "a" so "a"
            QTest::newRow("A6") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << false << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "ZX" << "a";
            // X || Y || Z - X empty, Y matches "bc", Z matches "a" so "abc"
            QTest::newRow("A7") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "XYZ" << "abc";
            // X || Z || Y - X empty, Y matches "bc", Z matches "a" so "abc"
            QTest::newRow("A8") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "XZY" << "abc";
            // Y || X || Z - X empty, Y matches "bc", Z matches "a" so "abc"
            QTest::newRow("A9") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                << "YXZ" << "abc";
            // Z || X || Y - X empty, Y matches "bc", Z matches "a" so "abc"
            QTest::newRow("A10") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                 << "ZXY" << "abc";
            // Y || Z || X - X empty, Y matches "bc", Z matches "a" so "abc"
            QTest::newRow("A11") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                 << "YZX" << "abc";
            // Z || Y || X - X empty, Y matches "bc", Z matches "a" so "abc"
            QTest::newRow("A12") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(10) << QVariant() << QVariant()
                                 << "ZYX" << "abc";

            // WITH Y AND Z AS DETAIL FILTERS (with some overlap between Y and Z results)
            // For these tests, Y matches "bc", Z matches "b"
            // X || Y - X empty, Y matches "b", so "bc"
            QTest::newRow("B1") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(20) << QVariant() << QVariant()
                                << "XY" << "bc";
            // Y || X - X empty, Y matches "bc", so "bc"
            QTest::newRow("B2") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << false << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(20) << QVariant() << QVariant()
                                << "YX" << "bc";
            // Y || Z  - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B3") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "YZ" << "bc";
            // Z || Y - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B4") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "ZY" << "bc";
            // X || Z - X empty, Z matches "b" so "b"
            QTest::newRow("B5") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "XZ" << "b";
            // Z || X - X empty, Z matches "b" so "b"
            QTest::newRow("B6") << manager
                                << false << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "ZX" << "b";
            // X || Y || Z - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B7") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "XYZ" << "bc";
            // X || Z || Y - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B8") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "XZY" << "bc";
            // Y || X || Z - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B9") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                << "YXZ" << "bc";
            // Z || X || Y - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B10") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                 << "ZXY" << "bc";
            // Y || Z || X - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B11") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                 << "YZX" << "bc";
            // Z || Y || X - X empty, Y matches "bc", Z matches "b" so "bc"
            QTest::newRow("B12") << manager
                                << true << 1 << booleanDefAndFieldNames.first << booleanDefAndFieldNames.second << true << QVariant(false) << QVariant() << QVariant()
                                 << true << 1 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << true << QVariant(20) << QVariant() << QVariant()
                                 << "ZYX" << "bc";
        }

        //---------------------------

        // WITH Y AND Z AS RANGE FILTERS (with no overlap between Y and Z results)
        // For these tests, Y matches "a", Z matches "b"
        // X || Y - X empty, Y matches "a" so "a"
        if (validDetailField(integerDefAndFieldNames)) {
            QTest::newRow("C1") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XY" << "a";
            // Y || X - X empty, Y matches "a" so "a"
            QTest::newRow("C2") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YX" << "a";
            // Y || Z - Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C3") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YZ" << "ab";
            // Z || Y - Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C4") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZY" << "ab";
            // X || Z - X empty, Z matches "b" so "b"
            QTest::newRow("C5") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZ" << "b";
            // Z || X - X empty, Z matches "b" so "b"
            QTest::newRow("C6") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZX" << "b";
            // X || Y || Z - X empty, Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C7") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XYZ" << "ab";
            // X || Z || Y - X empty, Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C8") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZY" << "ab";
            // Y || X || Z - X empty, Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C9") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YXZ" << "ab";
            // Z || X || Y - X empty, Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C10") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZXY" << "ab";
            // Y || Z || X - X empty, Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C11") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "YZX" << "ab";
            // Z || Y || X - X empty, Y matches "a", Z matches "b" so "ab"
            QTest::newRow("C12") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(15)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZYX" << "ab";

            // WITH Y AND Z AS RANGE FILTERS (with some overlap between Y and Z results)
            // For these tests, Y matches "ab", Z matches "b"
            // X || Y - X empty, Y matches "ab" so "ab"
            QTest::newRow("D1") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XY" << "ab";
            // Y || X - X empty, Y matches "ab" so "ab"
            QTest::newRow("D2") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YX" << "ab";
            // Y || Z - Y matches "ab", Z matches "b", union = "ab"
            QTest::newRow("D3") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YZ" << "ab";
            // Z || Y - Y matches "ab", Z matches "b", union = "ab"
            QTest::newRow("D4") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZY" << "ab";
            // X || Z - X empty, Z matches "b" so "b"
            QTest::newRow("D5") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZ" << "b";
            // Z || X - X empty, Z matches "b" so "b"
            QTest::newRow("D6") << manager
                                << false << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "ZX" << "b";
            // X || Y || Z - X empty, Y matches "ab", Z matches "b" so "ab"
            QTest::newRow("D7") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XYZ" << "ab";
            // X || Z || Y - X empty, Y matches "ab", Z matches "b" so "ab"
            QTest::newRow("D8") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "XZY" << "ab";
            // Y || X || Z - X empty, Y matches "ab", Z matches "b" so "ab"
            QTest::newRow("D9") << manager
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                << "YXZ" << "ab";
            // Z || X || Y - X empty, Y matches "ab", Z matches "b" so "ab"
            QTest::newRow("D10") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZXY" << "ab";
            // Y || Z || X - X empty, Y matches "ab", Z matches "b" so "ab"
            QTest::newRow("D11") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "YZX" << "ab";
            // Z || Y || X - X empty, Y matches "ab", Z matches "b" so "ab"
            QTest::newRow("D12") << manager
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(5) << QVariant(25)
                                 << true << 2 << integerDefAndFieldNames.first << integerDefAndFieldNames.second << false << QVariant(0) << QVariant(15) << QVariant(25)
                                 << "ZYX" << "ab";
        }
    }
}

void tst_QContactManagerFiltering::unionFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(bool, firstfilter);
    QFETCH(int, fftype); // 1 = detail, 2 = detailrange, 3 = groupmembership, 4 = union, 5 = intersection
    QFETCH(QContactDetail::DetailType, ffdetailtype);
    QFETCH(int, ffdetailfield);
    QFETCH(bool, ffsetvalue);
    QFETCH(QVariant, ffvalue);
    QFETCH(QVariant, ffminrange);
    QFETCH(QVariant, ffmaxrange);
    QFETCH(bool, secondfilter);
    QFETCH(int, sftype);
    QFETCH(QContactDetail::DetailType, sfdetailtype);
    QFETCH(int, sfdetailfield);
    QFETCH(bool, sfsetvalue);
    QFETCH(QVariant, sfvalue);
    QFETCH(QVariant, sfminrange);
    QFETCH(QVariant, sfmaxrange);
    QFETCH(QString, order);
    QFETCH(QString, expected);

    QContactFilter *x = new QContactUnionFilter();
    QContactFilter *y = 0, *z = 0;

    if (firstfilter) {
        switch (fftype) {
            case 1: // detail filter
                y = new QContactDetailFilter();
                static_cast<QContactDetailFilter*>(y)->setDetailType(ffdetailtype, ffdetailfield);
                if (ffsetvalue)
                    static_cast<QContactDetailFilter*>(y)->setValue(ffvalue);
                break;
            case 2: // range filter
                y = new QContactDetailRangeFilter();
                static_cast<QContactDetailRangeFilter*>(y)->setDetailType(ffdetailtype, ffdetailfield);
                static_cast<QContactDetailRangeFilter*>(y)->setRange(ffminrange, ffmaxrange);
                break;
            case 3: // group membership filter
            case 4: // union filter
            case 5: // intersection filter
                break;

            default:
                QVERIFY(false); // force fail.
            break;
        }
    }

    if (secondfilter) {
        switch (sftype) {
            case 1: // detail filter
                z = new QContactDetailFilter();
                static_cast<QContactDetailFilter*>(z)->setDetailType(sfdetailtype, sfdetailfield);
                if (sfsetvalue)
                    static_cast<QContactDetailFilter*>(z)->setValue(sfvalue);
                break;
            case 2: // range filter
                z = new QContactDetailRangeFilter();
                static_cast<QContactDetailRangeFilter*>(z)->setDetailType(sfdetailtype, sfdetailfield);
                static_cast<QContactDetailRangeFilter*>(z)->setRange(sfminrange, sfmaxrange);
                break;
            case 3: // group membership filter
            case 4: // union filter
            case 5: // intersection filter
                break;

            default:
                QVERIFY(false); // force fail.
            break;
        }
    }

    // control variables - order: starts, ends, mids
    bool sX = false;
    bool sY = false;
    bool sZ = false;
    bool eX = false;
    bool eY = false;
    bool eZ = false;
    bool mX = false;
    bool mY = false;
    bool mZ = false;

    if (order.startsWith("X"))
        sX = true;
    if (order.startsWith("Y"))
        sY = true;
    if (order.startsWith("Z"))
        sZ = true;
    if (order.endsWith("X"))
        eX = true;
    if (order.endsWith("Y"))
        eY = true;
    if (order.endsWith("Z"))
        eZ = true;
    if (order.size() > 2) {
        if (order.at(1) == 'X')
            mX = true;
        if (order.at(1) == 'Y')
            mY = true;
        if (order.at(1) == 'Z')
            mZ = true;
    }

    // now perform the filtering.
    QContactUnionFilter resultFilter;
    if (sX) {
        if (mY && eZ)
            resultFilter = *x | *y | *z;
        else if (mZ && eY)
            resultFilter = *x | *z | *y;
        else if (eY)
            resultFilter = *x | *y;
        else if (eZ)
            resultFilter = *x | *z;
    } else if (sY) {
        if (mX && eZ)
            resultFilter = *y | *x | *z;
        else if (mZ && eX)
            resultFilter = *y | *z | *x;
        else if (eX)
            resultFilter = *y | *x;
        else if (eZ)
            resultFilter = *y | *z;
    } else if (sZ) {
        if (mX && eY)
            resultFilter = *z | *x | *y;
        else if (mY && eX)
            resultFilter = *z | *y | *x;
        else if (eX)
            resultFilter = *z | *x;
        else if (eY)
            resultFilter = *z | *y;
    }

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    ids = cm->contactIds(resultFilter);

    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QCOMPARE_UNSORTED(output, expected);

    delete x;
    if (y) delete y;
    if (z) delete z;
}

void tst_QContactManagerFiltering::relationshipFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<int>("relatedContactRole");
    QTest::addColumn<QString>("relationshipType");
    QTest::addColumn<char>("relatedContact");
    QTest::addColumn<QString>("expected");

    const int firstRole(static_cast<int>(QContactRelationship::First));
    const int secondRole(static_cast<int>(QContactRelationship::Second));
    const int eitherRole(static_cast<int>(QContactRelationship::Either));

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        // HasMember
        QTest::newRow("RF-1") << manager << secondRole << QContactRelationship::HasMember() << '\0' << "a";
        QTest::newRow("RF-2") << manager << firstRole << QContactRelationship::HasMember() << '\0' << "b";
        QTest::newRow("RF-3") << manager << eitherRole << QContactRelationship::HasMember() << '\0' << "ab";

        // match any contact that has an assistant
        QTest::newRow("RF-4") << manager << secondRole << QContactRelationship::HasAssistant() << '\0' << "a";
        // match any contact that is an assistant
        QTest::newRow("RF-5") << manager << firstRole << QContactRelationship::HasAssistant() << '\0' << "b";
        // match any contact that has an assistant or is an assistant
        QTest::newRow("RF-6") << manager << eitherRole << QContactRelationship::HasAssistant() << '\0' << "ab";

        // IsSameAs
        QTest::newRow("RF-7") << manager << secondRole << QContactRelationship::IsSameAs() << '\0' << "a";
        QTest::newRow("RF-8") << manager << firstRole << QContactRelationship::IsSameAs() << '\0' << "b";
        QTest::newRow("RF-9") << manager << eitherRole << QContactRelationship::IsSameAs() << '\0' << "ab";

        // Aggregates
        QTest::newRow("RF-10") << manager << secondRole << QContactRelationship::Aggregates() << '\0' << "a";
        QTest::newRow("RF-11") << manager << firstRole << QContactRelationship::Aggregates() << '\0' << "b";
        QTest::newRow("RF-12") << manager << eitherRole << QContactRelationship::Aggregates() << '\0' << "ab";

        // HasManager
        QTest::newRow("RF-13") << manager << secondRole << QContactRelationship::HasManager() << '\0' << "a";
        QTest::newRow("RF-14") << manager << firstRole << QContactRelationship::HasManager() << '\0' << "b";
        QTest::newRow("RF-15") << manager << eitherRole << QContactRelationship::HasManager() << '\0' << "ab";

        // HasSpouse
        QTest::newRow("RF-16") << manager << secondRole << QContactRelationship::HasSpouse() << '\0' << "a";
        QTest::newRow("RF-17") << manager << firstRole << QContactRelationship::HasSpouse() << '\0' << "b";
        QTest::newRow("RF-18") << manager << eitherRole << QContactRelationship::HasSpouse() << '\0' << "ab";

        // Unknown relationship
        QTest::newRow("RF-19") << manager << secondRole << QStringLiteral("UnknownRelationship") << '\0' << "a";
        QTest::newRow("RF-20") << manager << firstRole << QStringLiteral("UnknownRelationship") << '\0' << "b";
        QTest::newRow("RF-21") << manager << eitherRole << QStringLiteral("UnknownRelationship") << '\0' << "ab";

        // match any contact that is the related contact in a relationship with contact-A
        QTest::newRow("RF-22") << manager << secondRole << QString() << 'a' << "";
        // match any contact has contact-A as the related contact
        QTest::newRow("RF-23") << manager << firstRole << QString() << 'a' << "b";
        // match any contact that has any relationship with contact-A
        QTest::newRow("RF-24") << manager << eitherRole << QString() << 'a' << "b";

        // match any contact that is the related contact in a relationship with contact-B
        QTest::newRow("RF-25") << manager << secondRole << QString() << 'b' << "a";
        // match any contact has contact-B as the related contact
        QTest::newRow("RF-26") << manager << firstRole << QString() << 'b' << "";
        // match any contact that has any relationship with contact-B
        QTest::newRow("RF-27") << manager << eitherRole << QString() << 'b' << "a";
    }
}

QContact tst_QContactManagerFiltering::createContact(QContactManager* cm, QContactType::TypeValues type, const QString &name)
{
    QContact contact;
    contact.setType(type);
    QContactName contactName;
    for (int i = QContactName::FieldPrefix; i <= QContactName::FieldSuffix; ++i) {
        contactName.setValue(i, name);
    }
    contact.saveDetail(&contactName);
    cm->saveContact(&contact);
    return contact;
}

void tst_QContactManagerFiltering::relationshipFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(int, relatedContactRole);
    QFETCH(QString, relationshipType);
    QFETCH(char, relatedContact);
    QFETCH(QString, expected);

    // TODO: A little re-factoring could be used to make the test case more readable

    // 1. Create contacts to be used in relationship testing
    QContact contactA;
    if(relationshipType == QContactRelationship::HasMember()) {
        // Change contact type to group as this is required at least by symbian backend
        // TODO: should it be possible to query this constraint from the backend?
        contactA = createContact(cm, QContactType::TypeGroup, "ContactA");
    } else {
        contactA = createContact(cm, QContactType::TypeContact, "ContactA");
    }
    QContact contactB = createContact(cm, QContactType::TypeContact, "ContactB");

    // 2. Create the relationship between the contacts
    QContact first;
    first.setId(contactA.id());
    QContact second;
    second.setId(contactB.id());

    QContactRelationship h2i;
    h2i.setFirst(first.id());
    h2i.setSecond(second.id());
    h2i.setRelationshipType(relationshipType);
    // save and check error code
    bool succeeded = false;
    if(cm->isRelationshipTypeSupported(relationshipType, contactA.type())
        && cm->isRelationshipTypeSupported(relationshipType, contactB.type())) {
        succeeded = true;
        QVERIFY(cm->saveRelationship(&h2i));
        QCOMPARE(cm->error(), QContactManager::NoError);
    } else {
        QVERIFY(!cm->saveRelationship(&h2i));
        QCOMPARE(cm->error(), QContactManager::NotSupportedError);
    }

    // 3. Construct the filter
    QContactRelationshipFilter crf;
    crf.setRelatedContactRole(static_cast<QContactRelationship::Role>(relatedContactRole));
    crf.setRelationshipType(relationshipType);
    if (relatedContact == 'a') {
        crf.setRelatedContactId(first.id());
    } else if (relatedContact == 'b') {
        crf.setRelatedContactId(second.id());
    }

    // 4. Grab the filtering results
    QList<QContactId> contacts;
    contacts.append(contactA.id());
    contacts.append(contactB.id());
    QList<QContactId> ids = cm->contactIds(crf);
    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts

    // Check that a different relationship type does not match
    crf.setRelationshipType(QStringLiteral("Some other type"));
    QList<QContactId> ids2 = cm->contactIds(crf);

    // 5. Remove the created relationship and contacts
    if(succeeded) {
        // Check that an existing relationship can be removed
        QVERIFY(cm->removeRelationship(h2i));
        QCOMPARE(cm->error(), QContactManager::NoError);
    } else {
        // Check that non-existing relationship cannot be removed
        QVERIFY(!cm->removeRelationship(h2i));
        //TODO: what is the expected error code?
        //QCOMPARE(cm->error(), QContactManager::DoesNotExistError);
    }
    foreach (const QContactId& cid, contacts) {
        cm->removeContact(cid);
    }

    // 6. Verify the filtering result
    if(succeeded) {
        QCOMPARE_UNSORTED(output, expected);
        QCOMPARE(ids2, QList<QContactId>());
    } else {
        QString msg = "Manager does not support relationship type " + relationshipType + " between " + contactA.type() + " and " + contactB.type() + " type contacts.";
        QSKIP(msg.toLatin1());
    }
}

void tst_QContactManagerFiltering::sorting_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QContactDetail::DetailType>("detailType");
    QTest::addColumn<int>("detailField");
    QTest::addColumn<int>("directioni");
    QTest::addColumn<bool>("setbp");
    QTest::addColumn<int>("blankpolicyi");
    QTest::addColumn<int>("casesensitivityi");
    QTest::addColumn<QString>("expected");
    QTest::addColumn<QString>("unstable");

    int firstname = QContactName::FieldFirstName;
    int lastname = QContactName::FieldLastName;
    QContactDetail::DetailType nameType = QContactName::Type;
    QContactDetail::DetailType dlType = QContactDisplayLabel::Type;
    int dlfld = QContactDisplayLabel::FieldLabel;

    int asc = Qt::AscendingOrder;
    int desc = Qt::DescendingOrder;
    int bll = QContactSortOrder::BlanksLast;
    int blf = QContactSortOrder::BlanksFirst;
    int cs = Qt::CaseSensitive;
    int ci = Qt::CaseInsensitive;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        QPair<QContactDetail::DetailType, int> integerDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        QPair<QContactDetail::DetailType, int> stringDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("String");

        newMRow("first ascending", manager) << manager << nameType << firstname << asc << false << 0 << cs << expectedSorting(manager, "abcdefgikhj", "hjabcdefgik", "abcdefgihkj", "abcdefghijk", "abcdefgihkj", "abcdefghijk") << "efg";  // efg have the same first name
        newMRow("first descending", manager) << manager << nameType << firstname << desc << false << 0 << cs << expectedSorting(manager, "jhkigfedcba", "kigfedcbajh", "jkhigfedcba", "kjihgfedcba", "jkhigfedcba", "kjihgfedcba") << "efg";// efg have the same first name
        newMRow("last ascending", manager) << manager << nameType << lastname << asc << false << 0 << cs << "bacdefghijk" << "hijk";       // all have a well defined, sortable last name except hijk
        newMRow("last descending", manager) << manager << nameType << lastname << desc << false << 0 << cs << "gfedcabhijk" << "hijk";     // all have a well defined, sortable last name except hijk
        if (validDetailField(integerDefAndFieldNames)) {
            newMRow("integer ascending, blanks last", manager) << manager << integerDefAndFieldNames.first << integerDefAndFieldNames.second << asc << true << bll << cs << "cabgfedhijk" << "gfedhijk"; // gfedhijk have no integer
            newMRow("integer descending, blanks last", manager) << manager << integerDefAndFieldNames.first << integerDefAndFieldNames.second << desc << true << bll << cs << "bacgfedhijk" << "gfedhijk"; // gfedhijk have no integer
            newMRow("integer ascending, blanks first", manager) << manager << integerDefAndFieldNames.first << integerDefAndFieldNames.second << asc << true << blf << cs << "hijkdefgcab" << "gfedhijk"; // gfedhijk have no integer
            newMRow("integer descending, blanks first", manager) << manager << integerDefAndFieldNames.first << integerDefAndFieldNames.second << desc << true << blf << cs << "hijkdefgbac" << "gfedhijk"; // gfedhijk have no integer
        }
        if (validDetailField(stringDefAndFieldNames)) {
            QTest::newRow("string ascending (null value), blanks first") << manager << stringDefAndFieldNames.first << stringDefAndFieldNames.second << asc << true << blf << cs << "feabcdg" << "fehijk"; // f and e have blank string
            QTest::newRow("string ascending (null value), blanks last") << manager << stringDefAndFieldNames.first << stringDefAndFieldNames.second << asc << true << bll << cs << "abcdgef" << "efhijk";   // f and e have blank string
        }

        newMRow("display label insensitive", manager) << manager << dlType << dlfld << asc << false << 0 << ci << "abcdefghjik" << "efghji";
        newMRow("display label sensitive", manager) << manager << dlType << dlfld << asc << false << 0 << cs << "abcdefghjik" << "efg";
    }
}

void tst_QContactManagerFiltering::sorting()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QContactDetail::DetailType, detailType);
    QFETCH(int, detailField);
    QFETCH(int, directioni);
    QFETCH(bool, setbp);
    QFETCH(int, blankpolicyi);
    QFETCH(int, casesensitivityi);
    QFETCH(QString, expected);
    QFETCH(QString, unstable);

    Qt::SortOrder direction = (Qt::SortOrder)directioni;
    QContactSortOrder::BlankPolicy blankpolicy = (QContactSortOrder::BlankPolicy) blankpolicyi;
    Qt::CaseSensitivity casesensitivity = (Qt::CaseSensitivity) casesensitivityi;

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    /* Build the sort order */
    QContactSortOrder s;
    s.setDetailType(detailType, detailField);
    s.setDirection(direction);
    if (setbp)
        s.setBlankPolicy(blankpolicy);
    s.setCaseSensitivity(casesensitivity);

    ids = cm->contactIds(s);
    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts

    // It's possible to get some contacts back in an arbitrary order (since we single sort)
    if (unstable.length() > 1) {
        // ensure that the maximum distance between unstable elements in the output is the size of the unstable string.
        int firstIndex = -1;
        int lastIndex = -1;

        for (int i = 0; i < output.size(); i++) {
            if (unstable.contains(output.at(i))) {
                firstIndex = i;
                break;
            }
        }

        for (int i = output.size() - 1; i >= 0; i--) {
            if (unstable.contains(output.at(i))) {
                lastIndex = i;
                break;
            }
        }

        if (firstIndex == -1 || lastIndex == -1) {
            bool containsAllUnstableElements = false;
            QVERIFY(containsAllUnstableElements);
        }

        bool unstableElementsAreGrouped = ((lastIndex - firstIndex) == (unstable.length() - 1));
        QVERIFY(unstableElementsAreGrouped);

        // now remove all unstable elements from the output
        for (int i = 1; i < unstable.length(); i++) {
            output.remove(unstable.at(i));
            expected.remove(unstable.at(i));
        }
    }

    // These tests pass only due to the un-sorted order matching the tested order:
    //QEXPECT_FAIL("display label insensitive[memory]", "memory backend does not add QContactDisplayLabel details", Continue);
    //QEXPECT_FAIL("display label insensitive[memory[params]]", "memory backend does not add QContactDisplayLabel details", Continue);
    QEXPECT_FAIL("display label sensitive[memory]", "memory backend does not add QContactDisplayLabel details", Continue);
    QEXPECT_FAIL("display label sensitive[memory[params]]", "memory backend does not add QContactDisplayLabel details", Continue);
    QCOMPARE(output, expected);

    /* Now do a check with a filter involved; the filter should not affect the sort order */
    QContactDetailFilter presenceName;
    presenceName.setDetailType(QContactName::Type);

    ids = cm->contactIds(presenceName, s);

    output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts

    // It's possible to get some contacts back in an arbitrary order (since we single sort)
    if (unstable.length() > 1) {
        // ensure that the maximum distance between unstable elements in the output is the size of the unstable string.
        int firstIndex = -1;
        int lastIndex = -1;

        for (int i = 0; i < output.size(); i++) {
            if (unstable.contains(output.at(i))) {
                firstIndex = i;
                break;
            }
        }

        for (int i = output.size() - 1; i >= 0; i--) {
            if (unstable.contains(output.at(i))) {
                lastIndex = i;
                break;
            }
        }

        if (firstIndex == -1 || lastIndex == -1) {
            bool containsAllUnstableElements = false;
            QVERIFY(containsAllUnstableElements);
        }

        bool unstableElementsAreGrouped = ((lastIndex - firstIndex) == (unstable.length() - 1));
        QVERIFY(unstableElementsAreGrouped);

        // now remove all unstable elements from the output
        for (int i = 1; i < unstable.length(); i++) {
            output.remove(unstable.at(i));
            expected.remove(unstable.at(i));
        }
    }

    //QEXPECT_FAIL("display label insensitive[memory]", "memory backend does not add QContactDisplayLabel details", Continue);
    //QEXPECT_FAIL("display label insensitive[memory[params]]", "memory backend does not add QContactDisplayLabel details", Continue);
    QEXPECT_FAIL("display label sensitive[memory]", "memory backend does not add QContactDisplayLabel details", Continue);
    QEXPECT_FAIL("display label sensitive[memory[params]]", "memory backend does not add QContactDisplayLabel details", Continue);
    QCOMPARE(output, expected);
}

void tst_QContactManagerFiltering::multiSorting_data()
{
    QTest::addColumn<QContactManager *>("cm");

    QTest::addColumn<bool>("firstsort");
    QTest::addColumn<QContactDetail::DetailType>("fsdetailtype");
    QTest::addColumn<int>("fsdetailfield");
    QTest::addColumn<int>("fsdirectioni");

    QTest::addColumn<bool>("secondsort");
    QTest::addColumn<QContactDetail::DetailType>("ssdetailtype");
    QTest::addColumn<int>("ssdetailfield");
    QTest::addColumn<int>("ssdirectioni");

    QTest::addColumn<QString>("expected");
    QTest::addColumn<bool>("efgunstable");


    QString es;

    int firstname = QContactName::FieldFirstName;
    int lastname = QContactName::FieldLastName;
    QContactDetail::DetailType nameType = QContactName::Type;
    QContactDetail::DetailType phoneType = QContactPhoneNumber::Type;
    int numberfield = QContactPhoneNumber::FieldNumber;

    int asc = Qt::AscendingOrder;
    int desc = Qt::DescendingOrder;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        QPair<QContactDetail::DetailType, int> stringDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("String");

        QTest::newRow("1") << manager
                           << true << nameType << firstname << asc
                           << true << nameType << lastname << asc
                           << "abcdefg" << false;
        QTest::newRow("2") << manager
                           << true << nameType << firstname << asc
                           << true << nameType << lastname << desc
                           << "abcdgfe" << false;
        QTest::newRow("3") << manager
                           << true << nameType << firstname << desc
                           << true << nameType << lastname << asc
                           << "efgdcba" << false;
        QTest::newRow("4") << manager
                           << true << nameType << firstname << desc
                           << true << nameType << lastname << desc
                           << "gfedcba" << false;

        QTest::newRow("5") << manager
                           << true << nameType << firstname << asc
                           << false << nameType << lastname << asc
                           << "abcdefg" << true;

        QTest::newRow("5b") << manager
                           << true << nameType << firstname << asc
                           << true << static_cast<QContactDetail::DetailType>(-1) << -1 << asc
                           << "abcdefg" << true;

        QTest::newRow("6") << manager
                           << false << nameType << firstname << asc
                           << true << nameType << lastname << asc
                           << "bacdefg" << false;

        // This test is completely unstable; no sort criteria means dependent upon internal sort order of manager.
        //QTest::newRow("7") << manager
        //                   << false << nameType << firstname << asc
        //                   << false << nameType << lastname << asc
        //                   << "abcdefg" << false; // XXX Isn't this totally unstable?

        if (validDetailField(stringDefAndFieldNames)) {
            QTest::newRow("8") << manager
                               << true << stringDefAndFieldNames.first << stringDefAndFieldNames.second << asc
                               << false << stringDefAndFieldNames.first << stringDefAndFieldNames.second << desc
                               << "abcdgef" << false; // default policy = blanks last, and ef have no value (e is empty, f is null)

            QTest::newRow("8b") << manager
                               << true << stringDefAndFieldNames.first << stringDefAndFieldNames.second << asc
                               << false << static_cast<QContactDetail::DetailType>(-1) << -1 << desc
                               << "abcdgef" << false; // default policy = blanks last, and ef have no value (e is empty, f is null)
        }

        QTest::newRow("9") << manager
                           << true << phoneType << numberfield << asc
                           << true << nameType << lastname << desc
                           << "abgfedc" << false;

        QTest::newRow("10") << manager
                            << true << nameType << firstname << asc
                            << true << nameType << firstname << desc
                            << "abcdefg" << true;

    }
}

void tst_QContactManagerFiltering::multiSorting()
{
    QFETCH(QContactManager*, cm);
    QFETCH(bool, firstsort);
    QFETCH(QContactDetail::DetailType, fsdetailtype);
    QFETCH(int, fsdetailfield);
    QFETCH(int, fsdirectioni);
    QFETCH(bool, secondsort);
    QFETCH(QContactDetail::DetailType, ssdetailtype);
    QFETCH(int, ssdetailfield);
    QFETCH(int, ssdirectioni);
    QFETCH(QString, expected);
    QFETCH(bool, efgunstable);

    Qt::SortOrder fsdirection = (Qt::SortOrder)fsdirectioni;
    Qt::SortOrder ssdirection = (Qt::SortOrder)ssdirectioni;

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);

    /* Build the sort orders */
    QContactSortOrder fs;
    fs.setDetailType(fsdetailtype, fsdetailfield);
    fs.setDirection(fsdirection);
    QContactSortOrder ss;
    ss.setDetailType(ssdetailtype, ssdetailfield);
    ss.setDirection(ssdirection);
    QList<QContactSortOrder> sortOrders;
    if (firstsort)
        sortOrders.append(fs);
    if (secondsort)
        sortOrders.append(ss);

    QList<QContactId> ids = cm->contactIds(sortOrders);
    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts

    // Remove the display label tests
    output.remove('h');
    output.remove('i');
    output.remove('j');
    output.remove('k');

    // Just like the single sort test, we might get some contacts back in indeterminate order
    // (but their relative position with other contacts should not change)
    if (efgunstable) {
        QVERIFY(output.count('e') == 1);
        QVERIFY(output.count('f') == 1);
        QVERIFY(output.count('g') == 1);
        output.remove('f');
        output.remove('g');
        expected.remove('f');
        expected.remove('g');
    }

    QCOMPARE(output, expected);
}

#ifdef INCLUDE_TESTACTIONS
void tst_QContactManagerFiltering::actionPlugins()
{
    QStringList actions = QContactAction::availableActions();
    QVERIFY(actions.contains("Boolean"));
    QVERIFY(actions.contains("Number"));

    /* Ignore the version if the vendor is not set */
    actions = QContactAction::availableActions(QString());
    QVERIFY(actions.contains("Boolean"));
    QVERIFY(actions.contains("Number"));

    actions = QContactAction::availableActions("NumberCo");
    QVERIFY(actions.contains("Number"));
    QVERIFY(!actions.contains("Boolean"));

    actions = QContactAction::availableActions("IntegerCo");
    QVERIFY(actions.contains("Number"));
    QVERIFY(!actions.contains("Boolean"));

    actions = QContactAction::availableActions("BooleanCo");
    QVERIFY(!actions.contains("Number"));
    QVERIFY(actions.contains("Boolean"));

    actions = QContactAction::availableActions("IntegerCo");
    QVERIFY(actions.contains("Number"));
    QVERIFY(!actions.contains("Boolean"));

    actions = QContactAction::availableActions("BooleanCo");
    QVERIFY(!actions.contains("Number"));
    QVERIFY(actions.contains("Boolean"));
}

void tst_QContactManagerFiltering::actionFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QString>("actionName");
    QTest::addColumn<QString>("expected");

    QString es;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        QPair<QContactDetail::DetailType, int> booleanDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Bool");
        QPair<QContactDetail::DetailType, int> integerDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Integer");
        QPair<QContactDetail::DetailType, int> dateDefAndFieldNames = defAndFieldNamesForTypePerManager.value(manager).value("Date");

        newMRow("bad actionname", manager) << manager << "No such action"  << "";

        QString expected;
        if (validDetailField(integerDefAndFieldNames) || validDetailField(booleanDefAndFieldNames)) {
                 expected = "abcd";
        } else if (validDetailField(dateDefAndFieldNames)) {
            expected = "abd";
        } else {
            /* contact a,b have phone number, so at least phone number action can match them */
            expected = "ab";
        }

        QTest::newRow("empty (any action matches)") << manager << es << expected;

        if (validDetailField(integerDefAndFieldNames)) {
            newMRow("Number", manager) << manager << "NumberAction" << "abcd";
            QTest::newRow("Number (NumberCo)") << manager << "NumberAction" << "abcd";
        }

        if (validDetailField(booleanDefAndFieldNames)) {
            /* Boolean testing */
            newMRow("Boolean action", manager) << manager << "BooleanAction" << "a";
            newMRow("BooleanCo", manager) << manager << es << "a";
        }

        if (validDetailField(booleanDefAndFieldNames)) {
            newMRow("Boolean action matching true", manager) << manager << es << "a";
            newMRow("Boolean action matching false", manager) << manager << es << es;
        }

        /* Recursive filtering */
        QTest::newRow("Recursive action 1") << manager << "IntersectionRecursive" << es;
        QTest::newRow("Recursive action 2") << manager << "UnionRecursive" << es;
        QTest::newRow("Recursive action 3") << manager << "PairRecursive" << es;
        QTest::newRow("Recursive action 4") << manager << "AnotherPairRecursive" << es;
        QTest::newRow("Recursive action 5") << manager << "Recursive" << es;
    }
}

void tst_QContactManagerFiltering::actionFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QString, actionName);
    QFETCH(QString, expected);

    // only test the memory engine - action filtering + service framework plugin loading
    // codepaths are tested fully this way since the codepath for other engines is that of
    // the memory engine, and only the memory engine has the required definitions and fields.
    if (cm->managerName() != QString(QLatin1String("memory")))
        return;

    /* Load the definition and field names for the various variant types for the current manager */
    defAndFieldNamesForTypeForActions = defAndFieldNamesForTypePerManager.value(cm);
    if (!defAndFieldNamesForTypeForActions.isEmpty()) {
        QContactActionFilter af;
        af.setActionName(actionName);

        QList<QContactId> ids = cm->contactIds(af);
        QList<QContactId> contacts = contactsAddedToManagers.values(cm);

qDebug() << "   actionName =" << actionName;

        QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
        QCOMPARE_UNSORTED(output, expected);
    }
}
#endif

void tst_QContactManagerFiltering::idListFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("expected");

    QString es;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        newMRow("empty", manager) << manager << es << es;
        newMRow("a", manager) << manager << "a" << "a";
        newMRow("ab", manager) << manager << "ab" << "ab";
        newMRow("aa", manager) << manager << "aa" << "a";
        newMRow("ba", manager) << manager << "ba" << "ab";
        newMRow("abcd", manager) << manager << "abcd" << "abcd";
        newMRow("abcde", manager) << manager << "abcde" << "abcd";
    }
}

void tst_QContactManagerFiltering::idListFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QString, input);
    QFETCH(QString, expected);

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QList<QContactId> ids;

    // extra id that won't exist
    QContactId e;

    /* Convert the input to a list of ids */
    foreach(QChar c, input) {
        if (c == 'a')
            ids << contacts.at(0);
        else if (c == 'b')
            ids << contacts.at(1);
        else if (c == 'c')
            ids << contacts.at(2);
        else if (c == 'd')
            ids << contacts.at(3);
        else if (c == 'e')
            ids << e;
    }

    /* And do the search */
    QContactIdFilter idf;
    idf.setIds(ids);

    /* Retrieve contacts matching the filter, and compare (unsorted) output */
    ids = cm->contactIds(idf);
    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QCOMPARE_UNSORTED(output, expected);
}

void tst_QContactManagerFiltering::convenienceFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QString>("addressSubString");
    QTest::addColumn<bool>("addressEnabled");
    QTest::addColumn<QString>("emailAddressSubString");
    QTest::addColumn<bool>("emailEnabled");
    QTest::addColumn<QString>("phoneSubString");
    QTest::addColumn<bool>("phoneEnabled");
    QTest::addColumn<QString>("displayLabelSubString");
    QTest::addColumn<bool>("displayLabelEnabled");
    QTest::addColumn<QString>("nameSubString");
    QTest::addColumn<bool>("nameEnabled");
    QTest::addColumn<bool>("favoriteEnabled");
    QTest::addColumn<QString>("tagSubString");
    QTest::addColumn<bool>("tagEnabled");
    QTest::addColumn<QString>("expected");

    QString es; // empty string

    foreach (QContactManager *manager, managers) {
        const QList<QContactDetail::DetailType> supportedTypes(manager->supportedContactDetailTypes());
        if (supportedTypes.contains(QContactAddress::Type)) {
            newMRow("address matching only", manager) << manager
                                                      << "streetstring" << true
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << false // Favorite has no substring associated.
                                                      << es << false
                                                      << "l";
        }
        if (supportedTypes.contains(QContactEmailAddress::Type)) {
            newMRow("emailAddress matching only", manager) << manager
                                                      << es << false
                                                      << "@test.com" << true
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << false
                                                      << es << false
                                                      << "m";
        }
        if (supportedTypes.contains(QContactPhoneNumber::Type)) {
            newMRow("phone matching only", manager) << manager
                                                      << es << false
                                                      << es << false
                                                      << "12345" << true
                                                      << es << false
                                                      << es << false
                                                      << false
                                                      << es << false
                                                      << "n";
        }
        if (supportedTypes.contains(QContactDisplayLabel::Type)) {
            newMRow("displayLabel matching only", manager) << manager
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << "Freddy" << true
                                                      << es << false
                                                      << false
                                                      << es << false
                                                      << "o";
        }
        if (supportedTypes.contains(QContactName::Type)) {
            newMRow("name matching only", manager) << manager
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << "Frederic" << true
                                                      << false
                                                      << es << false
                                                      << "p";
        }
        if (supportedTypes.contains(QContactFavorite::Type)) {
            newMRow("favorite matching only", manager) << manager
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << true
                                                      << es << false
                                                      << "q";
        }
        if (supportedTypes.contains(QContactTag::Type)) {
            newMRow("tag matching only", manager) << manager
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << false
                                                      << "Football" << true
                                                      << "r";
        }
        if (supportedTypes.contains(QContactAddress::Type) &&
            supportedTypes.contains(QContactPhoneNumber::Type)) {
            newMRow("address or phone matching", manager) << manager
                                                      << "streetstring" << true
                                                      << es << false
                                                      << "12345" << true
                                                      << es << false
                                                      << es << false
                                                      << false
                                                      << es << false
                                                      << "ln";
        }
        if (supportedTypes.contains(QContactFavorite::Type) &&
            supportedTypes.contains(QContactTag::Type)) {
            newMRow("favorite or tag matching", manager) << manager
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << es << false
                                                      << true
                                                      << "Football" << true
                                                      << "qr";
        }
    }
}

void tst_QContactManagerFiltering::convenienceFiltering()
{
    QFETCH(QContactManager*, cm);
    QFETCH(QString, addressSubString);
    QFETCH(bool, addressEnabled);
    QFETCH(QString, emailAddressSubString);
    QFETCH(bool, emailEnabled);
    QFETCH(QString, phoneSubString);
    QFETCH(bool, phoneEnabled);
    QFETCH(QString, displayLabelSubString);
    QFETCH(bool, displayLabelEnabled);
    QFETCH(QString, nameSubString);
    QFETCH(bool, nameEnabled);
    QFETCH(bool, favoriteEnabled);
    QFETCH(QString, tagSubString);
    QFETCH(bool, tagEnabled);
    QFETCH(QString, expected);

    QContactFilter af = QContactAddress::match(addressSubString);
    QContactFilter ef = QContactEmailAddress::match(emailAddressSubString);
    QContactFilter pf = QContactPhoneNumber::match(phoneSubString);
    QContactFilter df = QContactDisplayLabel::match(displayLabelSubString);
    QContactFilter nf = QContactName::match(nameSubString);
    QContactFilter ff = QContactFavorite::match();
    QContactFilter tf = QContactTag::match(tagSubString);

    QList<QContactFilter> convenienceFilters;
    if (addressEnabled) convenienceFilters << af;
    if (emailEnabled) convenienceFilters << ef;
    if (phoneEnabled) convenienceFilters << pf;
    if (displayLabelEnabled) convenienceFilters << df;
    if (nameEnabled) convenienceFilters << nf;
    if (favoriteEnabled) convenienceFilters << ff;
    if (tagEnabled) convenienceFilters << tf;

    QContactFilter finalFilter;
    finalFilter = convenienceFilters.at(0);
    if (convenienceFilters.size() > 1) {
        for (int i = 1; i < convenienceFilters.size(); ++i) {
            // if more than one filter, we union them.
            finalFilter = (finalFilter | convenienceFilters.at(i));
        }
    }

    /* Retrieve contacts matching the filter, and ensure that the results are expected */
    QList<QContactId> ids = cm->contactIds(finalFilter);

    // build a string containing letters corresponding to the ids we retrieved.
    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QString resultString = convertIds(contacts, ids, 'l', 'r'); // just the convenience filtering contacts (L->R)
    QEXPECT_FAIL("displayLabel matching only[memory]", "memory backend does not add QContactDisplayLabel details", Continue);
    QEXPECT_FAIL("displayLabel matching only[memory[params]]", "memory backend does not add QContactDisplayLabel details", Continue);
    QCOMPARE(resultString, expected);
}

void tst_QContactManagerFiltering::invalidFiltering_data()
{
    QTest::addColumn<QContactManager*>("cm");

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        QTest::newRow(manager->managerName().toLatin1().constData()) << manager;
    }
}

void tst_QContactManagerFiltering::invalidFiltering()
{
    QFETCH(QContactManager*, cm);

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QContactInvalidFilter f; // invalid
    QList<QContactId> ids = cm->contactIds(f);
    QVERIFY(ids.count() == 0);

    // Try unions/intersections of invalids too
    ids = cm->contactIds(f | f);
    QVERIFY(ids.count() == 0);

    ids = cm->contactIds(f & f);
    QVERIFY(ids.count() == 0);
}

void tst_QContactManagerFiltering::allFiltering_data()
{
    QTest::addColumn<QContactManager*>("cm");

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        QTest::newRow(manager->managerName().toLatin1().constData()) << manager;
    }
}

void tst_QContactManagerFiltering::allFiltering()
{
    QFETCH(QContactManager*, cm);

    QList<QContactId> contacts = contactsAddedToManagers.values(cm);
    QContactFilter f; // default = permissive
    QList<QContactId> ids = cm->contactIds(f);
    QVERIFY(ids.count() == contacts.size());
    QString output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QString expected = convertIds(contacts, contacts, 'a', 'k'); // :)
    QCOMPARE_UNSORTED(output, expected);

    // Try unions/intersections of defaults
    ids = cm->contactIds(f | f);
    output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QCOMPARE_UNSORTED(output, expected);

    ids = cm->contactIds(f & f);
    output = convertIds(contacts, ids, 'a', 'k'); // don't include the convenience filtering contacts
    QCOMPARE_UNSORTED(output, expected);
}

void tst_QContactManagerFiltering::fetchHint_data()
{
    QTest::addColumn<QContactManager*>("cm");

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);
        QTest::newRow(manager->managerName().toLatin1().constData()) << manager;
    }
}

void tst_QContactManagerFiltering::fetchHint()
{
    QFETCH(QContactManager*, cm);

    // if no fetch hint is provided, the manager should return all data
    // if a fetch hint is provided, it should have a clearly defined effect,
    // unless it is ignored by the manager, in which case the result should
    // be equivalent to not providing a fetch hint.

    // we use a defined sort order for the retrieval of contacts to make comparison simple.
    // We sort on name, because we include name details in the fetch hint.
    QList<QContactSortOrder> nameSort;
    QContactSortOrder firstNameSort, middleNameSort, lastNameSort;
    firstNameSort.setDetailType(QContactName::Type, QContactName::FieldFirstName);
    middleNameSort.setDetailType(QContactName::Type, QContactName::FieldMiddleName);
    lastNameSort.setDetailType(QContactName::Type, QContactName::FieldLastName);
    nameSort << lastNameSort << middleNameSort << firstNameSort;

    // fetch all contacts from the manager.
    QList<QContact> allContacts = cm->contacts(nameSort);

    // define some maximum count limit, and some list of detail definitions to retrieve.
    int countLimit = (allContacts.size() / 2) + 1;
    QList<QContactDetail::DetailType> defs;
    defs << QContactName::Type
         << QContactPhoneNumber::Type;

    // test that the manager doesn't incorrectly implement fetch hints.
    // we test max count limit, and detail definition limits.
    // XXX TODO: other hints!
    QContactFetchHint mclh; // max count limited hint
    QContactFetchHint ddh;  // detail definitions hint
    mclh.setMaxCountHint(countLimit);
    ddh.setDetailTypesHint(defs);

    // the next part of the test requires some contacts to be saved in the manager.
    if (allContacts.size() == 0) {
        QSKIP("No contacts in manager; skipping fetch hint limit test.");
    }

    // test with a hint which sets a maximum count limit for retrieved contacts.
    QList<QContact> mclhContacts = cm->contacts(nameSort, mclh);
    QVERIFY(allContacts.size() >= mclhContacts.size());
    if (allContacts.size() > mclh.maxCountHint()) {
        // shouldn't return an arbitrarily smaller amount of contacts.
        QVERIFY(mclhContacts.size() == mclh.maxCountHint()
                || mclhContacts.size() == allContacts.size());
    }
    for (int i = 0; i < mclhContacts.size(); ++i) {
        // the sort order should still be defined.
        QVERIFY(mclhContacts.at(i) == allContacts.at(i));
    }

    // now test with a hint which describes which details the client is interested in.
    QList<QContact> ddhContacts = cm->contacts(nameSort, ddh);
    QCOMPARE(ddhContacts.size(), allContacts.size());
    for (int i = 0; i < allContacts.size(); ++i) {
        QContact a = allContacts.at(i);
        QContact b = ddhContacts.at(i);

        // since we're sorting on a detail which should exist (since it was included in the hint)
        // the order of the contacts returned shouldn't have changed.
        QVERIFY(a.id() == b.id());

        // check that the hint didn't remove names or phones.
        QCOMPARE(a.details(QContactName::Type).size(),
                 b.details(QContactName::Type).size());
        QCOMPARE(a.details(QContactPhoneNumber::Type).size(),
                 b.details(QContactPhoneNumber::Type).size());

        // other details are not necessarily returned.
        QVERIFY(a.details().size() >= b.details().size());
    }
}


void tst_QContactManagerFiltering::changelogFiltering_data()
{
    QTest::addColumn<QContactManager *>("cm");
    QTest::addColumn<QList<QContactId> >("contacts");
    QTest::addColumn<int>("eventType");
    QTest::addColumn<QDateTime>("since");
    QTest::addColumn<QString>("expected");

    int added = (int)QContactChangeLogFilter::EventAdded;
    int changed = (int)QContactChangeLogFilter::EventChanged;
    int removed = (int)QContactChangeLogFilter::EventRemoved;

    for (int i = 0; i < managers.size(); i++) {
        QContactManager *manager = managers.at(i);

        QList<QContactId> contacts = contactsAddedToManagers.values(manager);
        QContact a,b,c,d;
        a = manager->contact(contacts.at(0));
        b = manager->contact(contacts.at(1));
        c = manager->contact(contacts.at(2));
        d = manager->contact(contacts.at(3));

        QDateTime ac = a.detail<QContactTimestamp>().created();
        QDateTime bc = b.detail<QContactTimestamp>().created();
        QDateTime cc = c.detail<QContactTimestamp>().created();
        QDateTime dc = d.detail<QContactTimestamp>().created();

        QDateTime am = a.detail<QContactTimestamp>().lastModified();
        QDateTime bm = b.detail<QContactTimestamp>().lastModified();
        QDateTime cm = c.detail<QContactTimestamp>().lastModified();
        QDateTime dm = d.detail<QContactTimestamp>().lastModified();

        newMRow("Added since before start", manager) << manager << contacts << added << ac.addSecs(-1) << "abcdefg";
        newMRow("Added since first", manager) << manager << contacts << added << ac << "abcdefg";
        newMRow("Added since second", manager) << manager << contacts << added << bc << "bcdefg";
        newMRow("Added since third", manager) << manager << contacts << added << cc << "cdefg";
        newMRow("Added since fourth", manager) << manager << contacts << added << dc << "defg";
        newMRow("Added since after fourth", manager) << manager << contacts << added << dc.addSecs(1) << "efg";
        newMRow("Added since first changed", manager) << manager << contacts << added << am << "";
        newMRow("Added since second changed", manager) << manager << contacts << added << bm << "";
        newMRow("Added since third changed", manager) << manager << contacts << added << cm << "";
        newMRow("Added since fourth changed", manager) << manager << contacts << added << cm << "";

        newMRow("Changed since before start", manager) << manager << contacts << changed << ac.addSecs(-1) << "abcdefg";
        newMRow("Changed since first", manager) << manager << contacts << changed << ac << "abcdefg";
        newMRow("Changed since second", manager) << manager << contacts << changed << bc << "abcdefg";
        newMRow("Changed since third", manager) << manager << contacts << changed << cc << "abcdefg";
        newMRow("Changed since fourth", manager) << manager << contacts << changed << dc << "abcdefg";
        newMRow("Changed since after fourth", manager) << manager << contacts << changed << dc.addSecs(1) << "abcefg";
        newMRow("Changed since first changed", manager) << manager << contacts << changed << am << "a";
        newMRow("Changed since second changed", manager) << manager << contacts << changed << bm << "ab";
        newMRow("Changed since third changed", manager) << manager << contacts << changed << cm << "abc";
        newMRow("Changed since fourth changed", manager) << manager << contacts << changed << dm << "abcdefg";

        // These are currently useless..
        newMRow("Removed since before start", manager) << manager << contacts << removed << ac.addSecs(-1) << "";
        newMRow("Removed since first", manager) << manager << contacts << removed << ac << "";
        newMRow("Removed since second", manager) << manager << contacts << removed << bc << "";
        newMRow("Removed since third", manager) << manager << contacts << removed << cc << "";
        newMRow("Removed since fourth", manager) << manager << contacts << removed << dc << "";
        newMRow("Removed since after fourth", manager) << manager << contacts << removed << dc.addSecs(1) << "";
    }
}

void tst_QContactManagerFiltering::changelogFiltering()
{
    QFETCH(int, eventType);
    QFETCH(QDateTime, since);
    QFETCH(QString, expected);
    QFETCH(QContactManager*, cm);
    QFETCH(QList<QContactId>, contacts);

    QList<QContactId> ids;

    QContactChangeLogFilter clf((QContactChangeLogFilter::EventType)eventType);
    clf.setSince(since);

    ids = cm->contactIds(clf);

    QString output = convertIds(contacts, ids, 'a', 'g'); // don't include the convenience filtering contacts
    QCOMPARE(output, expected); // unsorted? or sorted?
}

QList<QContactId> tst_QContactManagerFiltering::prepareModel(QContactManager *cm)
{
    // Sleep between contact additions to allow change log filtering to differentiate contacts by timestamp
    // Assume 1-second resolution or better
    const int napTime = 1000;

    QMap<QString, QPair<QContactDetail::DetailType, int> > definitionDetails;
    definitionDetails.insert("String", qMakePair(QContactGuid::Type, static_cast<int>(QContactGuid::FieldGuid)));
    definitionDetails.insert("Integer", qMakePair(QContactPresence::Type, static_cast<int>(QContactPresence::FieldPresenceState)));
    definitionDetails.insert("DateTime", qMakePair(QContactAnniversary::Type, static_cast<int>(QContactAnniversary::FieldOriginalDate)));
    definitionDetails.insert("Date", qMakePair(QContactBirthday::Type, static_cast<int>(QContactBirthday::FieldBirthday)));
    definitionDetails.insert("Bool", qMakePair(QContactFavorite::Type, static_cast<int>(QContactFavorite::FieldFavorite)));
    definitionDetails.insert("Double", qMakePair(QContactDetail::TypeGeoLocation, static_cast<int>(QContactGeoLocation::FieldLatitude)));
    // Currently unused:
    definitionDetails.insert("LongLong", qMakePair(QContactDetail::TypeUndefined, -1));
    definitionDetails.insert("ULongLong", qMakePair(QContactDetail::TypeUndefined, -1));
    definitionDetails.insert("Time", qMakePair(QContactDetail::TypeUndefined, -1));
    definitionDetails.insert("UInt", qMakePair(QContactDetail::TypeUndefined, -1));
    definitionDetails.insert("Char", qMakePair(QContactDetail::TypeUndefined, -1));

    defAndFieldNamesForTypePerManager.insert(cm, definitionDetails);

    /* Add some contacts */
    QContact contactA, contactB, contactC, contactD;
    QContactName name;
    QContactPhoneNumber number;

    // Create details for each relevant value type
    QContactDetail stringDetail(definitionDetails.value("String").first);
    QContactDetail integerDetail(definitionDetails.value("Integer").first);
    QContactDetail datetimeDetail(definitionDetails.value("DateTime").first);
    QContactDetail doubleDetail(definitionDetails.value("Double").first);
    QContactDetail boolDetail(definitionDetails.value("Bool").first);
    QContactDetail longlongDetail(definitionDetails.value("LongLong").first);
    QContactDetail ulonglongDetail(definitionDetails.value("ULongLong").first);
    QContactDetail dateDetail(definitionDetails.value("Date").first);
    QContactDetail timeDetail(definitionDetails.value("Time").first);
    QContactDetail uintDetail(definitionDetails.value("UInt").first);
    QContactDetail charDetail(definitionDetails.value("Char").first);

    name.setFirstName("Aaron");
    name.setLastName("Aaronson");
    name.setMiddleName("Arne");
    name.setPrefix("Sir");
    name.setSuffix("Dr.");
    QContactNickname nick;
    nick.setNickname("Sir Aaron");
    QContactEmailAddress emailAddr;
    emailAddr.setEmailAddress("Aaron@Aaronson.com");
    number.setNumber("5551212");
    stringDetail.setValue(definitionDetails.value("String").second, "Aaron Aaronson");
    integerDetail.setValue(definitionDetails.value("Integer").second, 10);
    datetimeDetail.setValue(definitionDetails.value("DateTime").second, QDateTime(QDate(2009, 06, 29), QTime(16, 52, 23, 0)));
    boolDetail.setValue(definitionDetails.value("Bool").second, true);
    ulonglongDetail.setValue(definitionDetails.value("ULongLong").second, (qulonglong)120000000000LL); // 120B
    dateDetail.setValue(definitionDetails.value("Date").second, QDate(1988, 1, 26));
    timeDetail.setValue(definitionDetails.value("Time").second, QTime(16,52,23,0));

    contactA.saveDetail(&name);
    contactA.saveDetail(&nick);
    contactA.saveDetail(&emailAddr);
    contactA.saveDetail(&number);
    if (validDetailField(definitionDetails.value("String")))
        contactA.saveDetail(&stringDetail);
    if (validDetailField(definitionDetails.value("Integer")))
        contactA.saveDetail(&integerDetail);
    if (validDetailField(definitionDetails.value("DateTime")))
        contactA.saveDetail(&datetimeDetail);
    if (validDetailField(definitionDetails.value("Bool")))
        contactA.saveDetail(&boolDetail);
    if (validDetailField(definitionDetails.value("ULongLong")))
        contactA.saveDetail(&ulonglongDetail);
    if (validDetailField(definitionDetails.value("Date")))
        contactA.saveDetail(&dateDetail);
    if (validDetailField(definitionDetails.value("Time")))
        contactA.saveDetail(&timeDetail);

    name = QContactName();
    name.setFirstName("Bob");
    name.setLastName("Aaronsen");
    nick.setNickname("Sir Bob");
    number.setNumber("5553456");
    stringDetail.setValue(definitionDetails.value("String").second, "Bob Aaronsen");
    integerDetail.setValue(definitionDetails.value("Integer").second, 20);
    doubleDetail.setValue(definitionDetails.value("Double").second, 4.0);
    boolDetail.setValue(definitionDetails.value("Bool").second, false);
    ulonglongDetail.setValue(definitionDetails.value("ULongLong").second, (qulonglong) 80000000000LL); // 80B
    uintDetail.setValue(definitionDetails.value("UInt").second, 4000000000u); // 4B
    dateDetail.setValue(definitionDetails.value("Date").second, QDate(2492, 5, 5));
    timeDetail.setValue(definitionDetails.value("Time").second, QTime(15,52,23,0));
    charDetail.setValue(definitionDetails.value("Char").second, QVariant(QChar('b')));

    contactB.saveDetail(&name);
    contactB.saveDetail(&nick);
    contactB.saveDetail(&number);
    if (validDetailField(definitionDetails.value("String")))
        contactB.saveDetail(&stringDetail);
    if (validDetailField(definitionDetails.value("Integer")))
        contactB.saveDetail(&integerDetail);
    if (validDetailField(definitionDetails.value("Double")))
        contactB.saveDetail(&doubleDetail);
    if (validDetailField(definitionDetails.value("Bool")))
        contactB.saveDetail(&boolDetail);
    if (validDetailField(definitionDetails.value("ULongLong")))
        contactB.saveDetail(&ulonglongDetail);
    if (validDetailField(definitionDetails.value("UInt")))
        contactB.saveDetail(&uintDetail);
    if (validDetailField(definitionDetails.value("Date")))
        contactB.saveDetail(&dateDetail);
    if (validDetailField(definitionDetails.value("Time")))
        contactB.saveDetail(&timeDetail);
    if (validDetailField(definitionDetails.value("Char")))
        contactB.saveDetail(&charDetail);

    name.setFirstName("Boris");
    name.setLastName("Aaronsun");
    stringDetail.setValue(definitionDetails.value("String").second, "Boris Aaronsun");
    integerDetail.setValue(definitionDetails.value("Integer").second, -20);
    datetimeDetail.setValue(definitionDetails.value("DateTime").second, QDateTime(QDate(2009, 06, 29), QTime(16, 54, 17, 0)));
    longlongDetail.setValue(definitionDetails.value("LongLong").second, (qlonglong)8000000000LL); // 8B
    charDetail.setValue(definitionDetails.value("Char").second, QVariant(QChar('c')));

    contactC.saveDetail(&name);
    if (validDetailField(definitionDetails.value("String")))
        contactC.saveDetail(&stringDetail);
    if (validDetailField(definitionDetails.value("Integer")))
        contactC.saveDetail(&integerDetail);
    if (validDetailField(definitionDetails.value("DateTime")))
        contactC.saveDetail(&datetimeDetail);
    if (validDetailField(definitionDetails.value("Double")))
        contactC.saveDetail(&doubleDetail);
    if (validDetailField(definitionDetails.value("Bool")))
        contactC.saveDetail(&boolDetail);
    if (validDetailField(definitionDetails.value("LongLong")))
        contactC.saveDetail(&longlongDetail);
    if (validDetailField(definitionDetails.value("ULongLong")))
        contactC.saveDetail(&ulonglongDetail);
    if (validDetailField(definitionDetails.value("Char")))
        contactC.saveDetail(&charDetail);

    name.setFirstName("Dennis");
    name.setLastName("FitzMacintyre");
    stringDetail.setValue(definitionDetails.value("String").second, "Dennis FitzMacintyre");
    doubleDetail.setValue(definitionDetails.value("Double").second, -128.0);
    longlongDetail.setValue(definitionDetails.value("LongLong").second, (qlonglong)-14000000000LL);
    uintDetail.setValue(definitionDetails.value("UInt").second, 3000000000u); // 3B
    dateDetail.setValue(definitionDetails.value("Date").second, QDate(2770, 10, 1));

    contactD.saveDetail(&name);
    if (validDetailField(definitionDetails.value("String")))
        contactD.saveDetail(&stringDetail);
    if (validDetailField(definitionDetails.value("Double")))
        contactD.saveDetail(&doubleDetail);
    if (validDetailField(definitionDetails.value("LongLong")))
        contactD.saveDetail(&longlongDetail);
    if (validDetailField(definitionDetails.value("UInt")))
        contactD.saveDetail(&uintDetail);
    if (validDetailField(definitionDetails.value("Date")))
        contactD.saveDetail(&dateDetail);

    qDebug() << "Generating contacts with different timestamps, please wait..";
    int originalContactCount = cm->contactIds().count();
    bool successfulSave = cm->saveContact(&contactA);
    Q_FATAL_VERIFY(successfulSave);
    QTest::qSleep(napTime);
    successfulSave = cm->saveContact(&contactB);
    Q_FATAL_VERIFY(successfulSave);
    QTest::qSleep(napTime);
    successfulSave = cm->saveContact(&contactC);
    Q_FATAL_VERIFY(successfulSave);
    QTest::qSleep(napTime);
    successfulSave = cm->saveContact(&contactD);
    Q_FATAL_VERIFY(successfulSave);
    QTest::qSleep(napTime);

    /* Now add some contacts specifically for multisorting */
    QContact contactE,contactF,contactG;
    QContactName n;
    n.setFirstName("John");
    n.setLastName("Smithee");
    stringDetail.setValue(definitionDetails.value("String").second, "");
    if (validDetailField(definitionDetails.value("String")))
        contactE.saveDetail(&stringDetail);
    contactE.saveDetail(&n);
    n = QContactName();
    n.setFirstName("John");
    n.setLastName("Smithey");
    contactF.saveDetail(&n);
    n = QContactName();
    n.setFirstName("John");
    n.setLastName("Smithy");
    stringDetail.setValue(definitionDetails.value("String").second, "zzz");
    if (validDetailField(definitionDetails.value("String")))
        contactG.saveDetail(&stringDetail);
    contactG.saveDetail(&n);
    successfulSave = cm->saveContact(&contactE);
    Q_FATAL_VERIFY(successfulSave);
    successfulSave = cm->saveContact(&contactF);
    Q_FATAL_VERIFY(successfulSave);
    successfulSave = cm->saveContact(&contactG);
    Q_FATAL_VERIFY(successfulSave);
    originalContactCount += 7;
    Q_FATAL_VERIFY(cm->contactIds().count() == originalContactCount);

    /* Now some for the locale aware sorting */
    /* In the C locale, all lowercase chars sort after uppercase */
    QContact contactH, contactI, contactJ, contactK;
    QContactName n2;
    n2.setFirstName("xander");
    contactH.saveDetail(&n2);
    n2.setFirstName("Xander");
    contactI.saveDetail(&n2);
    n2.setFirstName("yarrow");
    contactJ.saveDetail(&n2);
    n2.setFirstName("Yarrow");
    contactK.saveDetail(&n2);

    Q_FATAL_VERIFY(cm->saveContact(&contactH));
    Q_FATAL_VERIFY(cm->saveContact(&contactI));
    Q_FATAL_VERIFY(cm->saveContact(&contactJ));
    Q_FATAL_VERIFY(cm->saveContact(&contactK));

    /* Ensure the last modified times are different */
    QTest::qSleep(napTime);
    QContactName modifiedName = contactC.detail(QContactName::Type);
    contactC.saveDetail(&modifiedName);
    cm->saveContact(&contactC);
    QTest::qSleep(napTime);
    modifiedName = contactB.detail(QContactName::Type);
    contactB.saveDetail(&modifiedName);
    cm->saveContact(&contactB);
    QTest::qSleep(napTime);
    modifiedName = contactA.detail(QContactName::Type);
    contactA.saveDetail(&modifiedName);
    cm->saveContact(&contactA);
    QTest::qSleep(napTime);

    /* Now some for convenience filtering */
    QList<QContactDetail::DetailType> allTypes = cm->supportedContactDetailTypes();
    // Contact L ----------------------------------------
    QContact contactL;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress ladr;
        ladr.setStreet("streetstring road"); // Contact L matches streetstring.
        ladr.setLocality("testplace");
        ladr.setRegion("somewhere");
        contactL.saveDetail(&ladr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress led;
        led.setEmailAddress("frad@test.domain");
        contactL.saveDetail(&led);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber lp;
        lp.setNumber("11111");
        contactL.saveDetail(&lp);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName ln;
        ln.setFirstName("Fradarick");
        ln.setLastName("Gumboots");
        contactL.saveDetail(&ln);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag lt;
        lt.setTag("Soccer");
        contactL.saveDetail(&lt);
    }
    // Contact M ----------------------------------------
    QContact contactM;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress madr;
        madr.setStreet("some road");
        madr.setLocality("testplace");
        madr.setRegion("somewhere");
        contactM.saveDetail(&madr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress med;
        med.setEmailAddress("frbd@test.com"); // Contact M matches @test.com
        contactM.saveDetail(&med);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber mp;
        mp.setNumber("22222");
        contactM.saveDetail(&mp);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName mn;
        mn.setFirstName("Frbdbrick");
        mn.setLastName("Gumboots");
        contactM.saveDetail(&mn);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag mt;
        mt.setTag("Soccer");
        contactM.saveDetail(&mt);
    }
    // Contact N ----------------------------------------
    QContact contactN;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress nadr;
        nadr.setStreet("some road");
        nadr.setLocality("testplace");
        nadr.setRegion("somewhere");
        contactN.saveDetail(&nadr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress ned;
        ned.setEmailAddress("frcd@test.domain");
        contactN.saveDetail(&ned);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber np;
        np.setNumber("12345"); // Contact N matches 12345
        contactN.saveDetail(&np);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName nn;
        nn.setFirstName("Frcdcrick");
        nn.setLastName("Gumboots");
        contactN.saveDetail(&nn);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag nt;
        nt.setTag("Soccer");
        contactN.saveDetail(&nt);
    }
    // Contact O ----------------------------------------
    QContact contactO;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress oadr;
        oadr.setStreet("some road");
        oadr.setLocality("testplace");
        oadr.setRegion("somewhere");
        contactO.saveDetail(&oadr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress oed;
        oed.setEmailAddress("frdd@test.domain");
        contactO.saveDetail(&oed);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber op;
        op.setNumber("44444");
        contactO.saveDetail(&op);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName on;
        on.setFirstName("Freddy"); // Contact O matches Freddy
        on.setLastName("Gumboots");
        contactO.saveDetail(&on);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag ot;
        ot.setTag("Soccer");
        contactO.saveDetail(&ot);
    }
    // Contact P ----------------------------------------
    QContact contactP;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress padr;
        padr.setStreet("some road");
        padr.setLocality("testplace");
        padr.setRegion("somewhere");
        contactP.saveDetail(&padr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress ped;
        ped.setEmailAddress("fred@test.domain");
        contactP.saveDetail(&ped);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber pp;
        pp.setNumber("55555");
        contactP.saveDetail(&pp);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName pn;
        pn.setFirstName("Frederick"); // Contact P matches Frederic (contains).
        pn.setLastName("Gumboots");
        contactP.saveDetail(&pn);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag pt;
        pt.setTag("Soccer");
        contactP.saveDetail(&pt);
    }
    // Contact Q ----------------------------------------
    QContact contactQ;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress qadr;
        qadr.setStreet("some road");
        qadr.setLocality("testplace");
        qadr.setRegion("somewhere");
        contactQ.saveDetail(&qadr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress qed;
        qed.setEmailAddress("frfd@test.domain");
        contactQ.saveDetail(&qed);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber qp;
        qp.setNumber("66666");
        contactQ.saveDetail(&qp);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName qn;
        qn.setFirstName("Frfdfrick");
        qn.setLastName("Gumboots");
        contactQ.saveDetail(&qn);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag qt;
        qt.setTag("Soccer");
        contactQ.saveDetail(&qt);
    }
    if (allTypes.contains(QContactFavorite::Type)) {
        QContactFavorite qf;
        qf.setFavorite(true); // Contact Q matches favorite = true
        contactQ.saveDetail(&qf);
    }
    // Contact R ----------------------------------------
    QContact contactR;
    if (allTypes.contains(QContactAddress::Type)) {
        QContactAddress radr;
        radr.setStreet("some road");
        radr.setLocality("testplace");
        radr.setRegion("somewhere");
        contactR.saveDetail(&radr);
    }
    if (allTypes.contains(QContactEmailAddress::Type)) {
        QContactEmailAddress red;
        red.setEmailAddress("frgd@test.domain");
        contactR.saveDetail(&red);
    }
    if (allTypes.contains(QContactPhoneNumber::Type)) {
        QContactPhoneNumber rp;
        rp.setNumber("77777");
        contactR.saveDetail(&rp);
    }
    if (allTypes.contains(QContactName::Type)) {
        QContactName rn;
        rn.setFirstName("Frgdgrick");
        rn.setLastName("Gumboots");
        contactR.saveDetail(&rn);
    }
    if (allTypes.contains(QContactTag::Type)) {
        QContactTag rt;
        rt.setTag("Football"); // Contact R matches Football
        contactR.saveDetail(&rt);
    }
    // --------------------- save.
    Q_FATAL_VERIFY(cm->saveContact(&contactL));
    Q_FATAL_VERIFY(cm->saveContact(&contactM));
    Q_FATAL_VERIFY(cm->saveContact(&contactN));
    Q_FATAL_VERIFY(cm->saveContact(&contactO));
    Q_FATAL_VERIFY(cm->saveContact(&contactP));
    Q_FATAL_VERIFY(cm->saveContact(&contactQ));
    Q_FATAL_VERIFY(cm->saveContact(&contactR));
    // --------------------- end.

    /* Reload the contacts to pick up any changes */
    contactA = cm->contact(contactA.id());
    contactB = cm->contact(contactB.id());
    contactC = cm->contact(contactC.id());
    contactD = cm->contact(contactD.id());
    contactE = cm->contact(contactE.id());
    contactF = cm->contact(contactF.id());
    contactG = cm->contact(contactG.id());
    contactH = cm->contact(contactH.id());
    contactI = cm->contact(contactI.id());
    contactJ = cm->contact(contactJ.id());
    contactK = cm->contact(contactK.id());
    contactL = cm->contact(contactL.id());
    contactM = cm->contact(contactM.id());
    contactN = cm->contact(contactN.id());
    contactO = cm->contact(contactO.id());
    contactP = cm->contact(contactP.id());
    contactQ = cm->contact(contactQ.id());
    contactR = cm->contact(contactR.id());

    QList<QContactId> list;

    /* Add our newly saved contacts to our internal list of added contacts */
    foreach (const QContactId &id, (QList<QContactId>() << contactR.id()
                                                        << contactQ.id()
                                                        << contactP.id()
                                                        << contactO.id()
                                                        << contactN.id()
                                                        << contactM.id()
                                                        << contactL.id()
                                                        << contactK.id()
                                                        << contactJ.id()
                                                        << contactI.id()
                                                        << contactH.id()
                                                        << contactG.id()
                                                        << contactF.id()
                                                        << contactE.id()
                                                        << contactD.id()
                                                        << contactC.id()
                                                        << contactB.id()
                                                        << contactA.id())) {
        if (!id.isNull()) {
            contactsAddedToManagers.insert(cm, id);
            list.prepend(id);
        }
    }

    return list;
}

/* ============ Utility functions ============= */

void tst_QContactManagerFiltering::dumpContactDifferences(const QContact& ca, const QContact& cb)
{
    // Try to narrow down the differences
    QContact a(ca);
    QContact b(cb);

    QContactName n1 = a.detail(QContactName::Type);
    QContactName n2 = b.detail(QContactName::Type);

    // Check the name components in more detail
    QCOMPARE(n1.firstName(), n2.firstName());
    QCOMPARE(n1.middleName(), n2.middleName());
    QCOMPARE(n1.lastName(), n2.lastName());
    QCOMPARE(n1.prefix(), n2.prefix());
    QCOMPARE(n1.suffix(), n2.suffix());

    // Check the display label
    QCOMPARE(a.detail<QContactDisplayLabel>().label(), b.detail<QContactDisplayLabel>().label());

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

    // Now dump the extra details that were unmatched in A
    aDetails = a.details();
    bDetails = b.details();
    foreach(QContactDetail d, aDetails) {
        if (d.type() != QContactDisplayLabel::Type)
            qDebug() << "A contact had extra detail:" << d.type() << d.values();
    }
    // and same for B
    foreach(QContactDetail d, bDetails) {
        if (d.type() != QContactDisplayLabel::Type)
            qDebug() << "B contact had extra detail:" << d.type() << d.values();
    }

    QCOMPARE(b, a);
}

bool tst_QContactManagerFiltering::isSuperset(const QContact& ca, const QContact& cb)
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

    // check for contact type updates
    if (a.type() != b.type())
        return false; // nonempty type is different.

    // Now check to see if b has any details remaining; if so, a is not a superset.
    // Note that the DisplayLabel and Type can never be removed.
    if (b.details().size() > 2
            || (b.details().size() == 2 && (b.details().value(0).type() != QContactDisplayLabel::Type
                                            || b.details().value(1).type() != QContactType::Type)))
        return false;
    return true;
}

void tst_QContactManagerFiltering::dumpContact(const QContact& contact)
{
    QContactManager m;
    qDebug() << "Contact: " << contact.id() << "(" << contact.detail<QContactDisplayLabel>().label() << ")";
    QList<QContactDetail> details = contact.details();
    foreach(QContactDetail d, details) {
        qDebug() << "  " << d.type() << ":";
        qDebug() << "    Vals:" << d.values();
    }
}

void tst_QContactManagerFiltering::dumpContacts()
{
    QContactManager m;
    QList<QContactId> ids = m.contactIds();

    foreach(QContactId id, ids) {
        QContact c = m.contact(id);
        dumpContact(c);
    }
}

QTEST_MAIN(tst_QContactManagerFiltering)
#include "tst_qcontactmanagerfiltering.moc"
#include "qcontactmanager.h"
