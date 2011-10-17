/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

//TESTED_COMPONENT=src/contacts

#include <QtTest/QtTest>

#include <qcontacts.h>

//TESTED_CLASS=
//TESTED_FILES=

QTCONTACTS_USE_NAMESPACE

class tst_QContactRelationship: public QObject
{
Q_OBJECT

public:
    tst_QContactRelationship();
    virtual ~tst_QContactRelationship();

private slots:
    void operations();
    void emptiness();
    void hash();
    void datastream();
};

tst_QContactRelationship::tst_QContactRelationship()
{
}

tst_QContactRelationship::~tst_QContactRelationship()
{
}

void tst_QContactRelationship::operations()
{
    QContactRelationship r1;

    QContact contact1, contact2;
    contact1.id().setLocalId("1");
    contact1.id().setManagerUri("test");
    contact2.id().setLocalId("2");
    contact2.id().setManagerUri("test");

    QVERIFY(r1.first() == QContact());
    QVERIFY(r1.second() == QContact());
    QVERIFY(r1.relationshipType() == QString());

    r1.setFirst(contact1);
    QVERIFY(r1.first() == contact1);
    r1.setSecond(contact2);
    QVERIFY(r1.second() == contact2);
    QVERIFY(r1.first() == contact1);
    QVERIFY(r1.relationshipType() == QString());

    r1.setRelationshipType(QContactRelationship::HasSpouse);
    QVERIFY(r1.relationshipType() == QString(QLatin1String(QContactRelationship::HasSpouse)));
}

void tst_QContactRelationship::emptiness()
{
    QContactRelationship r1, r2, r3;

    QContact contact1, contact2, contact3;
    QContactId id;
    id.setLocalId("1");
    id.setManagerUri("test");
    contact1.setId(id);
    id.setLocalId("2");
    contact2.setId(id);
    id.setLocalId("3");
    contact3.setId(id);
    QVERIFY(r1.first() == QContact());
    QVERIFY(r1.second() == QContact());
    QVERIFY(r1.relationshipType() == QString());

    r1.setFirst(contact1);
    QVERIFY(r1.first() == contact1);
    r1.setSecond(contact2);
    QVERIFY(r1.second() == contact2);
    QVERIFY(r1.first() == contact1);
    QVERIFY(r1.relationshipType() == QString());

    r1.setRelationshipType(QContactRelationship::HasSpouse);
    QVERIFY(r1.relationshipType() == QString(QLatin1String(QContactRelationship::HasSpouse)));

    r2 = r1;
    QVERIFY(r1 == r2);
    QVERIFY(r1 != r3);
    QVERIFY(r2 != r3);

    r3.setFirst(contact3);
    r3.setSecond(contact2);
    r3.setRelationshipType(QContactRelationship::HasAssistant);

    r2.setFirst(contact3);
    QVERIFY(r1 != r2);
    QVERIFY(r2 != r3);
    QVERIFY(r3 != r1);
}

void tst_QContactRelationship::hash()
{
    QContactRelationship r1;
    QContact contact1;
    QContactId id;
    id.setLocalId("1");
    id.setManagerUri("a");
    contact1.setId(id);
    r1.setFirst(contact1);
    QContact contact2;
    id.setLocalId("2");
    id.setManagerUri("b");
    contact2.setId(id);
    r1.setSecond(contact2);
    r1.setRelationshipType(QContactRelationship::HasMember);

    QContactRelationship r2;
    r2.setFirst(contact1);
    r2.setSecond(contact2);
    r2.setRelationshipType(QContactRelationship::HasMember);

    QContactRelationship r3;
    r3.setFirst(contact1);
    QContact contact3;
    id.setManagerUri("c");
    id.setLocalId("3");
    contact3.setId(id);
    r3.setSecond(contact3);
    r3.setRelationshipType(QContactRelationship::HasMember);

    QVERIFY(qHash(r1) == qHash(r2));
    QVERIFY(qHash(r1) != qHash(r3));

}

void tst_QContactRelationship::datastream()
{
    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    QContactRelationship relationshipIn;
    QContact contact1;
    contact1.id().setManagerUri("a");
    contact1.id().setLocalId("1");
    relationshipIn.setFirst(contact1);
    QContact contact2;
    contact2.id().setManagerUri("b");
    contact2.id().setLocalId("2");
    relationshipIn.setSecond(contact2);
    relationshipIn.setRelationshipType(QContactRelationship::HasMember);
    stream1 << relationshipIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QContactRelationship relationshipOut;
    stream2 >> relationshipOut;
    QCOMPARE(relationshipOut, relationshipIn);
}

QTEST_MAIN(tst_QContactRelationship)
#include "tst_qcontactrelationship.moc"
