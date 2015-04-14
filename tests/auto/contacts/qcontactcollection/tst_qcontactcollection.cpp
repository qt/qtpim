/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2015 Canonical Ltd
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

#include <QtCore/qset.h>

#include <QtContacts/qcontacts.h>


QTCONTACTS_USE_NAMESPACE

static inline QContactCollectionId makeId(const QString &managerName, uint id)
{
    return QContactCollectionId(QStringLiteral("qtcontacts:%1:").arg(managerName), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}

class tst_QContactCollection: public QObject
{
Q_OBJECT

public:
    tst_QContactCollection();
    virtual ~tst_QContactCollection();

private slots:
    void metaData();
    void compare();
    void idComparison();
    void idHash();
    void idStringFunctions();
    void hash();
    void datastream();
    void traits();
    void idTraits();
};

tst_QContactCollection::tst_QContactCollection()
{
}

tst_QContactCollection::~tst_QContactCollection()
{
}

void tst_QContactCollection::metaData()
{
    QContactCollection c;
    QVERIFY(c.metaData().isEmpty());
    c.setExtendedMetaData(QString(QStringLiteral("test")), 5);
    QCOMPARE(c.extendedMetaData(QString(QStringLiteral("test"))).toInt(), 5);

    QMap<QContactCollection::MetaDataKey, QVariant> mdm;
    mdm.insert(QContactCollection::KeyName, QString(QStringLiteral("test2")));
    c.setMetaData(mdm);
    QCOMPARE(c.metaData(), mdm);
    QCOMPARE(c.metaData(QContactCollection::KeyName).toString(), QString(QStringLiteral("test2")));
}

void tst_QContactCollection::compare()
{
    QContactCollection c, c2;
    QVERIFY(c == c2);
    c.setExtendedMetaData(QStringLiteral("test"), 5);
    QVERIFY(c != c2);
    c2.setExtendedMetaData(QStringLiteral("test"), 5);
    QVERIFY(c == c2);

    QMap<QContactCollection::MetaDataKey, QVariant> mdm;
    mdm.insert(QContactCollection::KeyName, QStringLiteral("test2"));
    c.setMetaData(mdm);
    QVERIFY(c != c2);
    c2.setMetaData(mdm);
    QVERIFY(c == c2);

    c2 = QContactCollection();
    QVERIFY(c != c2);
    c2 = c;
    QVERIFY(c == c2);

    c.setId(makeId(QStringLiteral("a"), 1));
    QVERIFY(c != c2);
    c2.setId(makeId(QStringLiteral("a"), 1));
    QVERIFY(c == c2);
    c.setId(makeId(QStringLiteral("b"), 1));
    QVERIFY(c != c2);
    c2.setId(c.id());
    QVERIFY(c == c2);
    c.setId(makeId(QStringLiteral("b"), 2));
}

void tst_QContactCollection::idComparison()
{
    QContactCollectionId id1(makeId("a", 1));
    QContactCollectionId id2(makeId("a", 1));
    QVERIFY(!(id1 < id2));
    QVERIFY(!(id2 < id1));
    QVERIFY(id1 == id2);

    QContactCollectionId id3(makeId("a", 2));
    QContactCollectionId id4(makeId("b", 1));
    QContactCollectionId id5(makeId("b", 2));
    QVERIFY((((id1 < id3) && !(id3 < id1)) || ((id3 < id1) && !(id1 < id3))) && (id1 != id3));
    QVERIFY((((id1 < id4) && !(id4 < id1)) || ((id4 < id1) && !(id1 < id4))) && (id1 != id4));
    QVERIFY((((id3 < id4) && !(id4 < id3)) || ((id4 < id3) && !(id3 < id4))) && (id3 != id4));
    QVERIFY((((id1 < id5) && !(id5 < id1)) || ((id5 < id1) && !(id1 < id5))) && (id3 != id4));

    QContactCollectionId id6;
    QContactCollectionId id7(QString(), "1");
    QContactCollectionId id8(QString(), "2");
    QContactCollectionId id9(QStringLiteral("qtcontacts:basic:"), QByteArray());
    QVERIFY(id6.isNull());
    QVERIFY(id7.isNull());
    QVERIFY(id8.isNull());
    QVERIFY(id9.isNull());
    QVERIFY(id6 == id7);
    QVERIFY(!(id6 < id7));
    QVERIFY(id7 == id6);
    QVERIFY(!(id7 < id6));
    QVERIFY(id7 == id8);
    QVERIFY(!(id7 < id8));
    QVERIFY(id8 == id7);
    QVERIFY(!(id9 < id8));
    QVERIFY(id8 == id9);
    QVERIFY(!(id8 < id9));
    QVERIFY(id9 == id8);
    QVERIFY(!(id9 < id8));

    QVERIFY(!(id1 == id6));
    QVERIFY(!(id1 < id6));
    QVERIFY(id6 < id1);
    QVERIFY(!(id1 == id7));
    QVERIFY(!(id1 < id7));
    QVERIFY(id7 < id1);
    QVERIFY(!(id1 == id8));
    QVERIFY(!(id1 < id8));
    QVERIFY(id8 < id1);
    QVERIFY(!(id1 == id9));
    QVERIFY(!(id1 < id9));
    QVERIFY(id9 < id1);
}

void tst_QContactCollection::idHash()
{
    QContactCollectionId id1(makeId("a", 1));
    QContactCollectionId id2(makeId("a", 1));
    QContactCollectionId id3(makeId("b", 1));
    QContactCollectionId id4(makeId("a", 2));
    // note that the hash function ignores the managerUri
    QCOMPARE(qHash(id1), qHash(id2));
    QCOMPARE(qHash(id1), qHash(id3));
    QVERIFY(qHash(id1) != qHash(id4));

    QSet<QContactCollectionId> set;
    set.insert(id1);
    set.insert(id2);
    set.insert(id3);
    set.insert(id4);
    QCOMPARE(set.size(), 3);
}

void tst_QContactCollection::idStringFunctions()
{
    // TODO: review test
    QContactCollectionId id1(makeId("a", 1));
    QContactCollectionId id2(makeId("a", 1));
    QContactCollectionId id3(makeId("b", 1));
    QContactCollectionId id4(makeId("a", 2));
    QVERIFY(qHash(id1) == qHash(id2));
    QVERIFY(qHash(id1) != qHash(id4));

    // note that the toString and fromString functions are
    // engine and id specific.  This test merely checks that
    // the API is hooked up correctly.

    QVERIFY(id1.toString() == id2.toString());
    QVERIFY(id1.toString() != id3.toString());
    QVERIFY(id1.toString() != id4.toString());
    QVERIFY(id3.toString() != id4.toString());

    // this should "work" -- string of the correct format
    const uint numericId2 = 2u;
    const QByteArray localId2 = QByteArray(reinterpret_cast<const char *>(&numericId2), sizeof(uint));
    QString prebuiltidstring = QString("qtcontacts") + QString(":") + QString("a") + QString("::") + localId2.toHex();
    QContactCollectionId rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format and one parameter, but requires a working backend
    prebuiltidstring = QString("qtcontacts") + QString(":") + QString("a") + QString(":") + QString("key=value") + QString(":") + localId2.toHex();
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format and some parameters, but requires a working backend
    prebuiltidstring = QString("qtcontacts") + QString(":") + QString("a") + QString(":") + QString("key=value&key2=value2") + QString(":") + localId2.toHex();
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string has the right format but misses the value for a parameter
    prebuiltidstring = QString("qtcontacts") + QString(":") + QString("a") + QString(":") + QString("key=value&key2=") + QString(":") + localId2.toHex();
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid != id4);
    QVERIFY(rebuiltid.localId() == id4.localId());

    // this string misses a field (the parameters)
    prebuiltidstring = QString("qtcontacts") + QString(":") + QString("a") + QString(":") + localId2.toHex();
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QContactCollectionId()); // invalid so should be null.

    // this string misses two fields (params plus manager uri)
    prebuiltidstring = QString("qtcontacts") + QString(":") + QString::number(2);
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QContactCollectionId()); // invalid so should be null.

    // this string misses the prefix (qtcontacts)
    prebuiltidstring = QString("notcontacts") + QString(":") + QString("a") + QString("::") + localId2.toHex();
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QContactCollectionId()); // invalid so should be null.

    // this string misses the manager uri
    prebuiltidstring = QString("notcontacts") + QString(":::") + localId2.toHex();
    rebuiltid = QContactCollectionId::fromString(prebuiltidstring);
    QVERIFY(rebuiltid == QContactCollectionId()); // invalid so should be null.
}

void tst_QContactCollection::hash()
{
    // TODO: review tests
    QContactCollectionId id(makeId("a", 1));
    QContactCollection c1;
    c1.setId(id);
    c1.setExtendedMetaData("key", "value");
    QContactCollection c2;
    c2.setId(id);
    c2.setExtendedMetaData("key", "value");
    QContactCollection c3;
    c3.setId(id);
    c3.setExtendedMetaData("key", "another value");
    QContactCollection c4; // no details
    c4.setId(id);
    QContactCollection c5;
    c5.setId(id);
    c5.setExtendedMetaData("key", "value");
    QVERIFY(qHash(c1) == qHash(c2));
    QVERIFY(qHash(c1) != qHash(c3));
    QVERIFY(qHash(c1) != qHash(c4));
    QVERIFY(qHash(c1) == qHash(c5));
}

void tst_QContactCollection::datastream()
{
    // collection datastreaming
    QByteArray buffer;
    QContactCollection collectionIn;
    collectionIn.setExtendedMetaData("key", "value");
    QContactCollection collectionOut;
    QContactCollectionId originalId;

    // first, stream an item with a complete id
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        QContactManager om("memory");
        QVERIFY(om.saveCollection(&collectionIn)); // fill in its ID
        originalId = collectionIn.id();
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QCOMPARE(collectionOut, collectionIn); // can use QCOMPARE for collections, since no detail keys.
    }

    // second, stream an item with an id with the mgr uri set, local id null
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        collectionIn.setId(QContactCollectionId());
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QCOMPARE(collectionOut, collectionIn); // can use QCOMPARE for collections, since no detail keys.
    }

    // third, stream an item with a null id
    {
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        collectionIn.setId(QContactCollectionId());
        stream1 << collectionIn;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> collectionOut;
        QVERIFY(collectionOut.metaData() == collectionIn.metaData());
        QVERIFY(collectionOut.id() == collectionIn.id()); // should both be null ids.
    }

    // id datastreaming
    buffer.clear();
    QContactCollectionId inputId;
    QContactCollectionId outputId;

    // first, stream the whole id (mgr uri set, local id set)
    {
        inputId = originalId;
        QString serializedId = inputId.toString();
        outputId = QContactCollectionId::fromString(serializedId);
        QCOMPARE(inputId, outputId);

        inputId = originalId;
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }

    // second, stream a null id
    {
        inputId = QContactCollectionId();
        QString serializedId = inputId.toString();
        outputId = QContactCollectionId::fromString(serializedId);
        QCOMPARE(inputId, outputId);

        inputId = QContactCollectionId();
        buffer.clear();
        QDataStream stream1(&buffer, QIODevice::WriteOnly);
        stream1 << inputId;
        QVERIFY(buffer.size() > 0);
        QDataStream stream2(buffer);
        stream2 >> outputId;
        QCOMPARE(inputId, outputId);
    }
}

void tst_QContactCollection::traits()
{
    QCOMPARE(sizeof(QContactCollection), sizeof(void *));
    QVERIFY(QTypeInfo<QContactCollection>::isComplex);
    QVERIFY(!QTypeInfo<QContactCollection>::isStatic);
    QVERIFY(!QTypeInfo<QContactCollection>::isLarge);
    QVERIFY(!QTypeInfo<QContactCollection>::isPointer);
    QVERIFY(!QTypeInfo<QContactCollection>::isDummy);
}

void tst_QContactCollection::idTraits()
{
    QCOMPARE(sizeof(QContactCollectionId), 2*sizeof(void *));
    QVERIFY(QTypeInfo<QContactCollectionId>::isComplex);
    QVERIFY(!QTypeInfo<QContactCollectionId>::isStatic);
    QVERIFY(QTypeInfo<QContactCollectionId>::isLarge);
    QVERIFY(!QTypeInfo<QContactCollectionId>::isPointer);
    QVERIFY(!QTypeInfo<QContactCollectionId>::isDummy);
}

QTEST_MAIN(tst_QContactCollection)
#include "tst_qcontactcollection.moc"
