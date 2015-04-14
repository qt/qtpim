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

//TESTED_COMPONENT=src/versit

#include "tst_qversitproperty.h"
#include <QtVersit/qversitproperty.h>
#include <QtVersit/private/qversitproperty_p.h>
#include <QtVersit/qversitdocument.h>
#include <QtTest/QtTest>

QTVERSIT_USE_NAMESPACE

void tst_QVersitProperty::init()
{
    mVersitProperty = new QVersitProperty();
    QVERIFY(mVersitProperty);
}

void tst_QVersitProperty::cleanup()
{
   delete mVersitProperty;
}

void tst_QVersitProperty::testGroup()
{
    // One group
    QStringList group(QStringLiteral("GROUP_NAME"));
    mVersitProperty->setGroups(group);
    QCOMPARE(mVersitProperty->groups(), group);

    // Several groups
    QStringList groupList;
    groupList.append(QStringLiteral("GROUP1"));
    groupList.append(QStringLiteral("Group2"));
    groupList.append(QStringLiteral("group3"));
    mVersitProperty->setGroups(groupList);
    QCOMPARE(mVersitProperty->groups(), groupList);
}

void tst_QVersitProperty::testName()
{
    // Name in upper case
    QString name(QStringLiteral("TEL"));
    mVersitProperty->setName(name);
    QCOMPARE(mVersitProperty->name(), name);

    // Name in lower case, converted automatically to upper case
    mVersitProperty->setName(QStringLiteral("tel"));
    QCOMPARE(mVersitProperty->name(), name);
}

void tst_QVersitProperty::testParameters()
{
    QString typeParameterName(QStringLiteral("TYPE"));

    QString name(QStringLiteral("type"));
    QString value1(QStringLiteral("home"));
    mVersitProperty->insertParameter(name,value1);
    QMultiHash<QString,QString> parameters = mVersitProperty->parameters();
    QCOMPARE(parameters.count(), 1);
    QVERIFY(parameters.contains(typeParameterName,QStringLiteral("home")));

    QString value2(QStringLiteral("voice"));
    mVersitProperty->insertParameter(name,value2);
    parameters = mVersitProperty->parameters();
    QCOMPARE(parameters.count(), 2);
    QVERIFY(parameters.contains(typeParameterName,QStringLiteral("home")));
    QVERIFY(parameters.contains(typeParameterName,QStringLiteral("voice")));

    mVersitProperty->removeParameter(name,value1);
    QCOMPARE(mVersitProperty->parameters().count(), 1);
    QVERIFY(parameters.contains(typeParameterName,QStringLiteral("home")));

    mVersitProperty->removeParameter(name,value2);
    QCOMPARE(mVersitProperty->parameters().count(), 0);

    mVersitProperty->insertParameter(name, value1);
    mVersitProperty->insertParameter(name, value2);
    QCOMPARE(mVersitProperty->parameters().count(), 2);
    mVersitProperty->removeParameters(name);
    QCOMPARE(mVersitProperty->parameters().count(), 0);
}

void tst_QVersitProperty::testValue()
{
    QString value(QStringLiteral("050484747"));
    mVersitProperty->setValue(value);
    QCOMPARE(mVersitProperty->value(), value);
}

void tst_QVersitProperty::testEmbeddedDocument()
{
    QVersitDocument document;
    QVersitProperty property;
    property.setName(QStringLiteral("X-tension"));
    document.addProperty(property);
    mVersitProperty->setValue(QVariant::fromValue(document));
    QList<QVersitProperty> embeddedDocumentProperties =
        mVersitProperty->value<QVersitDocument>().properties();
    QCOMPARE(embeddedDocumentProperties.count(),1);
    QCOMPARE(embeddedDocumentProperties[0].name(),QStringLiteral("X-TENSION"));
}

void tst_QVersitProperty::testEquality()
{
    QVersitProperty property1;
    QVersitProperty property2;
    QVERIFY(property1.isEmpty());
    QVERIFY(property1 == property2);
    QVERIFY(!(property1 != property2));
    property2.setName(QStringLiteral("FN"));
    property2.setValue(QStringLiteral("John Citizen"));
    QVERIFY(!(property1 == property2));
    QVERIFY(property1 != property2);
    QVERIFY(!property2.isEmpty());

    property1.setName(QStringLiteral("FN"));
    property1.setValue(QStringLiteral("John Citizen"));
    QVERIFY(property1 == property2);
    QVERIFY(!(property1 != property2));

    property2.clear();
    QVERIFY(property2.isEmpty());

    property1.clear();
    QVERIFY(property1 == property2);
    QVERIFY(!(property1 != property2));
}

void tst_QVersitProperty::testHash()
{
    QVersitProperty property1;
    property1.setGroups(QStringList() << QStringLiteral("group1") << QStringLiteral("group2"));
    property1.setName(QStringLiteral("name"));
    property1.setValue(QStringLiteral("value"));
    property1.insertParameter(QStringLiteral("param"), QStringLiteral("value"));
    QVersitProperty property2;
    property2.setGroups(QStringList() << QStringLiteral("group1") << QStringLiteral("group2"));
    property2.setName(QStringLiteral("name"));
    property2.setValue(QStringLiteral("value"));
    property2.insertParameter(QStringLiteral("param"), QStringLiteral("value"));
    QVersitProperty property3; // no groups
    property3.setName(QStringLiteral("name"));
    property3.setValue(QStringLiteral("value"));
    property3.insertParameter(QStringLiteral("param"), QStringLiteral("value"));
    QVersitProperty property4; // no params
    property4.setGroups(QStringList() << QStringLiteral("group1") << QStringLiteral("group2"));
    property4.setName(QStringLiteral("name"));
    property4.setValue(QStringLiteral("value"));

    QVERIFY(qHash(property1) == qHash(property2));
    QVERIFY(qHash(property1) != qHash(property3));
    QVERIFY(qHash(property1) != qHash(property4));
    QVERIFY(qHash(property3) != qHash(property4));
    QSet<QVersitProperty> set;
    set.insert(property1);
    set.insert(property2);
    set.insert(property3);
    set.insert(property4);
    QCOMPARE(set.size(), 3);
}

QTEST_MAIN(tst_QVersitProperty)

