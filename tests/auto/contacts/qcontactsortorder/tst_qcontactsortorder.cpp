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
#include <QtCore/QMetaType>

#include <QtContacts/qcontacts.h>

//TESTED_COMPONENT=src/contacts

QTCONTACTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QContact)
Q_DECLARE_METATYPE(QContactSortOrder)

class tst_QContactSortOrder : public QObject
{
Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void traits();

    void sortObject();

    void compareContact_data();
    void compareContact();

    void datastream_data();
    void datastream();

    void debugstream_data();
    void debugstream();
};

void tst_QContactSortOrder::init()
{
}

void tst_QContactSortOrder::cleanup()
{
}

void tst_QContactSortOrder::traits()
{
    QTypeInfo<QContactSortOrder> ti;
    QVERIFY(ti.isComplex);
    QVERIFY(!ti.isStatic);
    QVERIFY(!ti.isLarge);
    QVERIFY(!ti.isPointer);
    QVERIFY(!ti.isDummy);
    QCOMPARE(uint(ti.sizeOf), uint(sizeof(void *)));
}

void tst_QContactSortOrder::sortObject()
{
    QContactSortOrder sortorder;

    /* Defaults */
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QContactSortOrder());

    /* Blank Policy */
    sortorder.setBlankPolicy(QContactSortOrder::BlanksFirst);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksFirst);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder != QContactSortOrder());

    sortorder.setBlankPolicy(QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QContactSortOrder());

    /* Direction */
    sortorder.setDirection(Qt::DescendingOrder);
    QVERIFY(sortorder.direction() == Qt::DescendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder != QContactSortOrder());

    sortorder.setDirection(Qt::AscendingOrder);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QContactSortOrder());

    /* Case sensitivity */
    sortorder.setCaseSensitivity(Qt::CaseInsensitive);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseInsensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder != QContactSortOrder());

    sortorder.setCaseSensitivity(Qt::CaseSensitive);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QContactSortOrder());

    /* Definitions */
    sortorder.setDetailType(QContactDetail::DetailType(), -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(!sortorder.isValid());

    sortorder.setDetailType(QContactDetail::TypeUndefined, -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(!sortorder.isValid());

    sortorder.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldStreet);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeAddress);
    QVERIFY(sortorder.detailField() == QContactAddress::FieldStreet);
    QVERIFY(sortorder.isValid());

    sortorder.setDetailType(QContactDetail::DetailType(), QContactAddress::FieldStreet);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == QContactAddress::FieldStreet);
    QVERIFY(!sortorder.isValid());

    /* Copy ctor */
    sortorder.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldStreet);
    sortorder.setBlankPolicy(QContactSortOrder::BlanksFirst);
    sortorder.setDirection(Qt::DescendingOrder);
    QVERIFY(sortorder.direction() == Qt::DescendingOrder);
    QVERIFY(sortorder.blankPolicy() == QContactSortOrder::BlanksFirst);
    QVERIFY(sortorder.detailType() == QContactDetail::TypeAddress);
    QVERIFY(sortorder.detailField() == QContactAddress::FieldStreet);
    QVERIFY(sortorder.isValid());

    QContactSortOrder other(sortorder);
    QVERIFY(other.direction() == Qt::DescendingOrder);
    QVERIFY(other.blankPolicy() == QContactSortOrder::BlanksFirst);
    QVERIFY(other.detailType() == QContactDetail::TypeAddress);
    QVERIFY(other.detailField() == QContactAddress::FieldStreet);
    QVERIFY(other.isValid());
    QVERIFY(other == sortorder);
    QVERIFY(!(other != sortorder));

    other.setDetailType(QContactDetail::TypePhoneNumber, QContactAddress::FieldStreet);
    QVERIFY(other != sortorder);

    other.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldRegion);
    QVERIFY(other != sortorder);

    /* Assignment operator */
    QContactSortOrder another;
    another = other;
    QVERIFY(another.direction() == Qt::DescendingOrder);
    QVERIFY(another.blankPolicy() == QContactSortOrder::BlanksFirst);
    QVERIFY(another.detailType() == QContactDetail::TypeAddress);
    QVERIFY(another.detailField() == QContactAddress::FieldRegion);
    QVERIFY(another.isValid());
    QVERIFY(another == other);
    QVERIFY(!(other != another));

    /* Self assignment */
    another = another;
    QVERIFY(another.direction() == Qt::DescendingOrder);
    QVERIFY(another.blankPolicy() == QContactSortOrder::BlanksFirst);
    QVERIFY(another.detailType() == QContactDetail::TypeAddress);
    QVERIFY(another.detailField() == QContactAddress::FieldRegion);
    QVERIFY(another.isValid());
    QVERIFY(another == other);
    QVERIFY(!(other != another));
}

void tst_QContactSortOrder::compareContact_data()
{
    QTest::addColumn<QContact>("contact1");
    QTest::addColumn<QContact>("contact2");
    QTest::addColumn<QContactSortOrder>("sortOrder");
    QTest::addColumn<int>("expected");

    QContact emptyContact;

    {
        QContact contact1;
        QContactNote note1;
        note1.setNote(QLatin1String("note"));
        contact1.saveDetail(&note1);
        QContactDisplayLabel displayLabel1;
        displayLabel1.setLabel(QLatin1String("My label"));
        contact1.saveDetail(&displayLabel1);
        QContactGuid guid1;
        guid1.setGuid(QLatin1String("98765"));
        contact1.saveDetail(&guid1);

        QContact contact2;
        QContactNote note2;
        note2.setNote(QLatin1String("NOTE"));
        contact2.saveDetail(&note2);
        QContactDisplayLabel displayLabel2;
        displayLabel2.setLabel(QLatin1String("label"));
        contact2.saveDetail(&displayLabel2);
        QContactGuid guid2;
        guid2.setGuid(QLatin1String("12345"));
        contact2.saveDetail(&guid2);
        QContactName name;
        name.setFirstName("John");
        contact2.saveDetail(&name);

        QContactSortOrder sortOrder;

        QTest::newRow("empty sortorder") << contact1 << contact2 << sortOrder << 0;

        sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
        QTest::newRow("empty contacts") << emptyContact << emptyContact << sortOrder << 0;

        QTest::newRow("equals") << contact1 << contact1 << sortOrder << 0;
        QTest::newRow("equals") << contact2 << contact2 << sortOrder << 0;

        sortOrder.setBlankPolicy(QContactSortOrder::BlanksFirst);
        {
            sortOrder.setDetailType(QContactDetail::TypeName, -1);
            QTest::newRow("blanks first, field presence") << emptyContact << contact2 << sortOrder << -1;
            QTest::newRow("blanks first, field presence") << contact1 << contact2 << sortOrder << -1;

            sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
            QTest::newRow("blanks first") << emptyContact << contact2 << sortOrder << -1;
            QTest::newRow("blanks first") << contact2 << emptyContact << sortOrder << 1;
            sortOrder.setDetailType(QContactDetail::TypeName, QContactName::FieldFirstName);
            QTest::newRow("blanks first") << emptyContact << contact2 << sortOrder << -1;
            sortOrder.setDetailType(QContactDetail::TypeTag, QContactTag::FieldTag);
            QTest::newRow("blanks first") << contact1 << contact2 << sortOrder << 0;
        }

        sortOrder.setBlankPolicy(QContactSortOrder::BlanksLast);
        {
            sortOrder.setDetailType(QContactDetail::TypeName, -1);
            QTest::newRow("blanks last, field presence") << emptyContact << contact2 << sortOrder << 1;
            QTest::newRow("blanks last, field presence") << contact1 << contact2 << sortOrder << 1;

            sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
            QTest::newRow("blanks last") << emptyContact << contact2 << sortOrder << 1;
            QTest::newRow("blanks last") << contact2 << emptyContact << sortOrder << -1;
            sortOrder.setDetailType(QContactDetail::TypeName, QContactName::FieldFirstName);
            QTest::newRow("blanks first") << emptyContact << contact2 << sortOrder << 1;
            sortOrder.setDetailType(QContactDetail::TypeTag, QContactTag::FieldTag);
            QTest::newRow("blanks first") << contact1 << contact2 << sortOrder << 0;
        }

        sortOrder.setDirection(Qt::AscendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseSensitive);
        {
            sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
            QTest::newRow("asc, cs") << contact1 << contact2 << sortOrder << -1;
            QTest::newRow("asc, cs") << contact2 << contact1 << sortOrder << 1;

            sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
            QTest::newRow("asc, ci") << contact1 << contact2 << sortOrder << 0;
            sortOrder.setDetailType(QContactDetail::TypeDisplayLabel, QContactDisplayLabel::FieldLabel);
            QTest::newRow("asc, ci") << contact1 << contact2 << sortOrder << 1;
            QTest::newRow("asc, ci") << contact2 << contact1 << sortOrder << -1;
        }

        sortOrder.setDirection(Qt::DescendingOrder);
        {
            sortOrder.setCaseSensitivity(Qt::CaseSensitive);
            sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
            QTest::newRow("desc, cs") << contact1 << contact2 << sortOrder << 1;
            QTest::newRow("desc, cs") << contact2 << contact1 << sortOrder << -1;

            sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
            QTest::newRow("desc, ci") << contact1 << contact2 << sortOrder << 0;
            sortOrder.setDetailType(QContactDetail::TypeDisplayLabel, QContactDisplayLabel::FieldLabel);
            QTest::newRow("desc, ci") << contact1 << contact2 << sortOrder << -1;
            QTest::newRow("desc, ci") << contact2 << contact1 << sortOrder << 1;
        }
    }

    { // ensure non-existence is treated just like blank-ness
        QContact contact1;
        QContactName name1;
        name1.setFirstName("John");
        name1.setLastName("Smith");
        contact1.saveDetail(&name1);

        QContact contact2;
        QContactName name2;
        name2.setFirstName("John");
        contact2.saveDetail(&name2);

        QContactSortOrder sortOrder;
        sortOrder.setDetailType(QContactDetail::TypeName, QContactName::FieldLastName);
        sortOrder.setDirection(Qt::AscendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);

        sortOrder.setBlankPolicy(QContactSortOrder::BlanksFirst);
        QTest::newRow("non-existence vs blank-ness, blanks first") << contact1 << contact2 << sortOrder << 1;
        QTest::newRow("non-existence vs blank-ness, blanks first") << emptyContact << contact1 << sortOrder << -1;
        QTest::newRow("non-existence vs blank-ness, blanks first") << emptyContact << contact2 << sortOrder << 0;

        sortOrder.setBlankPolicy(QContactSortOrder::BlanksLast);
        QTest::newRow("non-existence vs blank-ness, blanks last") << contact1 << contact2 << sortOrder << -1;
        QTest::newRow("non-existence vs blank-ness, blanks last") << emptyContact << contact1 << sortOrder << 1;
        QTest::newRow("non-existence vs blank-ness, blanks last") << emptyContact << contact2 << sortOrder << 0;
    }
}

void tst_QContactSortOrder::compareContact()
{
    QFETCH(QContact, contact1);
    QFETCH(QContact, contact2);
    QFETCH(QContactSortOrder, sortOrder);
    QFETCH(int, expected);

    int actual = QContactManagerEngine::compareContact(contact1, contact2, (QList<QContactSortOrder>() << sortOrder));
    actual = qBound(-1, actual, 1);
    QCOMPARE(actual, expected);
}

void tst_QContactSortOrder::datastream_data()
{
    QTest::addColumn<QContactSortOrder>("sortOrderIn");

    {
        QContactSortOrder sortOrder;
        QTest::newRow("default") << sortOrder;
    }

    {
        QContactSortOrder sortOrder;
        sortOrder.setBlankPolicy(QContactSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("invalid") << sortOrder;
    }

    {
        QContactSortOrder sortOrder;
        sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
        QTest::newRow("detail") << sortOrder;
    }

    {
        QContactSortOrder sortOrder;
        sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
        sortOrder.setBlankPolicy(QContactSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("complete") << sortOrder;
    }
}

void tst_QContactSortOrder::datastream()
{
    QFETCH(QContactSortOrder, sortOrderIn);

    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    stream1 << sortOrderIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QContactSortOrder sortOrderOut;
    stream2 >> sortOrderOut;
    QCOMPARE(sortOrderOut, sortOrderIn);
}

void tst_QContactSortOrder::debugstream_data()
{
    QTest::addColumn<QContactSortOrder>("sortOrder");
    QTest::addColumn<QString>("messageExpected");

    {
        QContactSortOrder sortOrder;
        QTest::newRow("default") << sortOrder << "QContactSortOrder(detailType=0,detailField=-1,blankPolicy=1,direction=0,caseSensitivity=1)";
    }

    {
        QContactSortOrder sortOrder;
        sortOrder.setBlankPolicy(QContactSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("invalid") << sortOrder << "QContactSortOrder(detailType=0,detailField=-1,blankPolicy=0,direction=1,caseSensitivity=0)";
    }

    {
        QContactSortOrder sortOrder;
        sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
        QTest::newRow("detail") << sortOrder << "QContactSortOrder(detailType=17,detailField=0,blankPolicy=1,direction=0,caseSensitivity=1)";
    }

    {
        QContactSortOrder sortOrder;
        sortOrder.setDetailType(QContactDetail::TypeNote, QContactNote::FieldNote);
        sortOrder.setBlankPolicy(QContactSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("complete") << sortOrder << "QContactSortOrder(detailType=17,detailField=0,blankPolicy=0,direction=1,caseSensitivity=0)";
    }
}

void tst_QContactSortOrder::debugstream()
{
    QFETCH(QContactSortOrder, sortOrder);
    QFETCH(QString, messageExpected);

    QTest::ignoreMessage(QtDebugMsg, messageExpected.toUtf8());
    qDebug() << sortOrder;
}

QTEST_MAIN(tst_QContactSortOrder)
#include "tst_qcontactsortorder.moc"
