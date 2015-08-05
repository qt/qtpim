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
#include <QtContacts/QContact>
#include <QtContacts/qcontactdetails.h>
#include <QElapsedTimer>
#include <QtDebug>

#include <QSet>

//TESTED_COMPONENT=src/contacts

QTCONTACTS_USE_NAMESPACE

namespace {
    QStringList generateNonOverlappingFirstNamesList()
    {
        QStringList retn;
        retn << "Zach" << "Zane" << "Zinedine" << "Zockey"
             << "Yann" << "Yedrez" << "Yarrow" << "Yelter"
             << "Ximmy" << "Xascha" << "Xanthar" << "Xachy"
             << "William" << "Wally" << "Weston" << "Wulther"
             << "Vernon" << "Veston" << "Victoria" << "Vuitton"
             << "Urqhart" << "Uelela" << "Ulrich" << "Umpty"
             << "Timothy" << "Tigga" << "Tabitha" << "Texter"
             << "Stan" << "Steve" << "Sophie" << "Siphonie"
             << "Richard" << "Rafael" << "Rachael" << "Rascal"
             << "Quirky" << "Quilton" << "Quentin" << "Quarreller";
        return retn;
    }

    QStringList generateNonOverlappingLastNamesList()
    {
        QStringList retn;
        retn << "Quilter" << "Qualifa" << "Quarrier" << "Quickson"
             << "Rigger" << "Render" << "Ranger" << "Reader"
             << "Sailor" << "Smith" << "Salter" << "Shelfer"
             << "Tailor" << "Tasker" << "Toppler" << "Tipster"
             << "Underhill" << "Umpire" << "Upperhill" << "Uppsland"
             << "Vintner" << "Vester" << "Victor" << "Vacationer"
             << "Wicker" << "Whaler" << "Whistler" << "Wolf"
             << "Xylophone" << "Xabu" << "Xanadu" << "Xatti"
             << "Yeoman" << "Yesman" << "Yelper" << "Yachtsman"
             << "Zimmerman" << "Zomething" << "Zeltic" << "Zephyr";
        return retn;
    }

    QStringList generateFirstNamesList()
    {
        QStringList retn;
        retn << "Alexandria" << "Andrew" << "Adrien" << "Amos"
             << "Bob" << "Bronte" << "Barry" << "Braxton"
             << "Clarence" << "Chandler" << "Chris" << "Chantelle"
             << "Dominic" << "Diedre" << "David" << "Derrick"
             << "Eric" << "Esther" << "Eddie" << "Eean"
             << "Felicity" << "Fred" << "Fletcher" << "Farraday"
             << "Gary" << "Gertrude" << "Gerry" << "Germaine"
             << "Hillary" << "Henry" << "Hans" << "Haddock"
             << "Jacob" << "Jane" << "Jackson" << "Jennifer"
             << "Larry" << "Lilliane" << "Lambert" << "Lilly"
             << "Mary" << "Mark" << "Mirriam" << "Matthew"
             << "Nathene" << "Nicholas" << "Ned" << "Norris"
             << "Othello" << "Oscar" << "Olaf" << "Odinsdottur"
             << "Penny" << "Peter" << "Patrick" << "Pilborough";
        return retn;
    }

    QStringList generateMiddleNamesList()
    {
        QStringList retn;
        retn << "Aubrey" << "Cody" << "Taylor" << "Leslie";
        return retn;
    }

    QStringList generateLastNamesList()
    {
        QStringList retn;
        retn << "Arkady" << "Addleman" << "Axeman" << "Applegrower" << "Anderson"
             << "Baker" << "Bremmer" << "Bedlam" << "Barrymore" << "Battery"
             << "Cutter" << "Cooper" << "Cutler" << "Catcher" << "Capemaker"
             << "Driller" << "Dyer" << "Diver" << "Daytona" << "Duster"
             << "Eeler" << "Eckhart" << "Eggsman" << "Empty" << "Ellersly"
             << "Farmer" << "Farrier" << "Foster" << "Farseer" << "Fairtime"
             << "Grower" << "Gaston" << "Gerriman" << "Gipsland" << "Guilder"
             << "Helper" << "Hogfarmer" << "Harriet" << "Hope" << "Huxley"
             << "Inker" << "Innman" << "Ipland" << "Instiller" << "Innis"
             << "Joker" << "Jackson" << "Jolt" << "Jockey" << "Jerriman";
        return retn;
    }

    QStringList generatePhoneNumbersList()
    {
        QStringList retn;
        retn << "111222" << "111333" << "111444" << "111555" << "111666"
             << "111777" << "111888" << "111999" << "222333" << "222444"
             << "222555" << "222666" << "222777" << "222888" << "222999"
             << "333444" << "333555" << "333666" << "333777" << "333888"
             << "333999" << "444555" << "444666" << "444777" << "444888"
             << "444999" << "555666" << "555777" << "555888" << "555999"
             << "666111" << "666222" << "666333" << "666444" << "666555"
             << "777111" << "777222" << "777333" << "777444" << "777555"
             << "777666" << "888111" << "888222" << "888333" << "888444"
             << "888555" << "888666" << "888777" << "999111" << "999222"
             << "999333" << "999444" << "999555" << "999666" << "999777"
             << "999888" << "999999";
        return retn;
    }

    QStringList generateEmailProvidersList()
    {
        QStringList retn;
        retn << "@test.com" << "@testing.com" << "@testers.com"
             << "@test.org" << "@testing.org" << "@testers.org"
             << "@test.net" << "@testing.net" << "@testers.net"
             << "@test.fi" << "@testing.fi" << "@testers.fi"
             << "@test.com.au" << "@testing.com.au" << "@testers.com.au"
             << "@test.co.uk" << "@testing.co.uk" << "@testers.co.uk"
             << "@test.co.jp" << "@test.co.jp" << "@testers.co.jp";
        return retn;
    }

    QStringList generateAvatarsList()
    {
        QStringList retn;
        retn << "-smiling.jpg" << "-laughing.jpg" << "-surprised.jpg"
             << "-smiling.png" << "-laughing.png" << "-surprised.png"
             << "-curious.jpg" << "-joking.jpg" << "-grinning.jpg"
             << "-curious.png" << "-joking.png" << "-grinning.png";
        return retn;
    }

    QStringList generateHobbiesList()
    {
        QStringList retn;
        retn << "tennis" << "soccer" << "squash" << "volleyball"
             << "chess" << "photography" << "painting" << "sketching";
        return retn;
    }

    QContact generateContact(const QString &syncTarget = QString(QLatin1String("local")))
    {
        static const QStringList firstNames(generateFirstNamesList());
        static const QStringList middleNames(generateMiddleNamesList());
        static const QStringList lastNames(generateLastNamesList());
        static const QStringList nonOverlappingFirstNames(generateNonOverlappingFirstNamesList());
        static const QStringList nonOverlappingLastNames(generateNonOverlappingLastNamesList());
        static const QStringList phoneNumbers(generatePhoneNumbersList());
        static const QStringList emailProviders(generateEmailProvidersList());
        static const QStringList avatars(generateAvatarsList());
        static const QStringList hobbies(generateHobbiesList());

        QContact retn;
        int random = qrand();
        bool moreRandom = qrand() % 2 == 0;

        QContactSyncTarget synctarget;
        synctarget.setSyncTarget(syncTarget);
        retn.saveDetail(&synctarget);

        QContactName name;
        name.setFirstName(moreRandom ?
                nonOverlappingFirstNames.at(random % nonOverlappingFirstNames.size()) :
                firstNames.at(random % firstNames.size()));
        name.setLastName(moreRandom ?
                nonOverlappingLastNames.at(random % nonOverlappingLastNames.size()) :
                lastNames.at(random % lastNames.size()));
        name.setMiddleName(middleNames.at(random % middleNames.size()));
        name.setPrefix(QLatin1String("Dr."));
        retn.saveDetail(&name);

        // Favorite
        QContactFavorite fav;
        fav.setFavorite(true);
        retn.saveDetail(&fav);

        // Phone number
        QContactPhoneNumber phn;
        QString randomPhn = phoneNumbers.at(random % phoneNumbers.size());
        phn.setNumber(moreRandom ? QString(QString::number(random % 500000) + randomPhn) : randomPhn);
        phn.setContexts(QContactDetail::ContextWork);
        retn.saveDetail(&phn);

        // Email
        QContactEmailAddress em;
        em.setEmailAddress(QString(QLatin1String("%1%2%3%4"))
                .arg(moreRandom ? QString(QString::number(random % 500000) + syncTarget) : QString())
                .arg(name.firstName()).arg(name.lastName())
                .arg(emailProviders.at(random % emailProviders.size())));
        if (random % 9) em.setContexts(QContactDetail::ContextWork);
        retn.saveDetail(&em);

        // Avatar
        QContactAvatar av;
        av.setImageUrl(name.firstName() + avatars.at(random % avatars.size()));
        retn.saveDetail(&av);

        // Hobby
        QContactHobby h1;
        h1.setHobby(hobbies.at(random % hobbies.size()));
        retn.saveDetail(&h1);
        if (moreRandom) {
            QContactHobby h2;
            h2.setHobby(hobbies.at((random+1) % hobbies.size()));
            retn.saveDetail(&h2);
        }

        return retn;
    }
}

//---------------------------------------------

class tst_detailsbenchmark : public QObject
{
    Q_OBJECT

public:
    tst_detailsbenchmark() {}
    ~tst_detailsbenchmark() {}

public slots:
    void init() {}
    void cleanup() {}

private slots:
    void initTestCase() {}
    void details() {
        qsrand(55555); // seed with constant so we get identical runs.
        QElapsedTimer syncTimer;
        static const int howMany = 10000;
        QList<QContact> newTestData;
        newTestData.reserve(howMany);
        qDebug() << "About to generate:" << howMany << "contacts.";
        syncTimer.start();
        for (int j = 0; j < howMany; ++j) {
            newTestData.append(generateContact(QString::fromLatin1("testing")));
        }
        qint64 ste = syncTimer.nsecsElapsed();
        double result = static_cast<double>(ste) / howMany; // nsec per contact
        qDebug() << "Generating" << howMany << "took" << ste << "nanoseconds (" << result << "nsec per contact )";
        QTest::setBenchmarkResult(result, QTest::WalltimeNanoseconds);
    }
};

QTEST_MAIN(tst_detailsbenchmark)
#include "tst_detailsbenchmark.moc"
