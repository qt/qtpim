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

#include <QtOrganizer/qorganizer.h>

//TESTED_COMPONENT=src/organizer

QTORGANIZER_USE_NAMESPACE

Q_DECLARE_METATYPE(QOrganizerItem)
Q_DECLARE_METATYPE(QOrganizerItemSortOrder)

class tst_QOrganizerItemSortOrder : public QObject
{
Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void traits();

    void sortObject();

    void compareItem_data();
    void compareItem();

    void datastream_data();
    void datastream();

    void debugstream_data();
    void debugstream();
};

void tst_QOrganizerItemSortOrder::init()
{
}

void tst_QOrganizerItemSortOrder::cleanup()
{
}

void tst_QOrganizerItemSortOrder::traits()
{
    QTypeInfo<QOrganizerItemSortOrder> ti;
    QVERIFY(ti.isComplex);
    QVERIFY(!ti.isStatic);
    QVERIFY(!ti.isLarge);
    QVERIFY(!ti.isPointer);
    QVERIFY(!ti.isDummy);
    QCOMPARE(uint(ti.sizeOf), uint(sizeof(void *)));
}

void tst_QOrganizerItemSortOrder::sortObject()
{
    QOrganizerItemSortOrder sortorder;

    /* Defaults */
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QOrganizerItemSortOrder());

    /* Blank Policy */
    sortorder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder != QOrganizerItemSortOrder());

    sortorder.setBlankPolicy(QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QOrganizerItemSortOrder());

    /* Direction */
    sortorder.setDirection(Qt::DescendingOrder);
    QVERIFY(sortorder.direction() == Qt::DescendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder != QOrganizerItemSortOrder());

    sortorder.setDirection(Qt::AscendingOrder);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QOrganizerItemSortOrder());

    /* Case sensitivity */
    sortorder.setCaseSensitivity(Qt::CaseInsensitive);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseInsensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder != QOrganizerItemSortOrder());

    sortorder.setCaseSensitivity(Qt::CaseSensitive);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.caseSensitivity() == Qt::CaseSensitive);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QOrganizerItemSortOrder());

    /* Definitions */
    sortorder.setDetail(QOrganizerItemDetail::TypeUndefined, -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(!sortorder.isValid());
    QVERIFY(sortorder == QOrganizerItemSortOrder());

    sortorder.setDetail(QOrganizerItemDetail::TypeComment, -1);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(sortorder.detailField() == -1);
    QVERIFY(sortorder.isValid());

    sortorder.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(sortorder.detailField() == QOrganizerItemComment::FieldComment);
    QVERIFY(sortorder.isValid());

    sortorder.setDetail(QOrganizerItemDetail::TypeUndefined, 101);
    QVERIFY(sortorder.direction() == Qt::AscendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksLast);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(sortorder.detailField() == 101);
    QVERIFY(!sortorder.isValid());

    /* Copy ctor */
    sortorder.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
    sortorder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
    sortorder.setDirection(Qt::DescendingOrder);
    QVERIFY(sortorder.direction() == Qt::DescendingOrder);
    QVERIFY(sortorder.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst);
    QVERIFY(sortorder.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(sortorder.detailField() == QOrganizerItemComment::FieldComment);
    QVERIFY(sortorder.isValid());

    QOrganizerItemSortOrder other(sortorder);
    QVERIFY(other.direction() == Qt::DescendingOrder);
    QVERIFY(other.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst);
    QVERIFY(other.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(other.detailField() == QOrganizerItemComment::FieldComment);
    QVERIFY(other.isValid());
    QVERIFY(other == sortorder);
    QVERIFY(!(other != sortorder));

    other.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
    QVERIFY(other != sortorder);

    other.setDetail(QOrganizerItemDetail::TypeComment, 2);
    QVERIFY(other != sortorder);

    /* Assignment operator */
    QOrganizerItemSortOrder another;
    another = other;
    QVERIFY(another.direction() == Qt::DescendingOrder);
    QVERIFY(another.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst);
    QVERIFY(another.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(another.detailField() == 2);
    QVERIFY(another.isValid());
    QVERIFY(another == other);
    QVERIFY(!(other != another));

    /* Self assignment */
    another = another;
    QVERIFY(another.direction() == Qt::DescendingOrder);
    QVERIFY(another.blankPolicy() == QOrganizerItemSortOrder::BlanksFirst);
    QVERIFY(another.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(another.detailField() == 2);
    QVERIFY(another.isValid());
    QVERIFY(another == other);
    QVERIFY(!(other != another));
}

void tst_QOrganizerItemSortOrder::compareItem_data()
{
    QTest::addColumn<QOrganizerItem>("item1");
    QTest::addColumn<QOrganizerItem>("item2");
    QTest::addColumn<QOrganizerItemSortOrder>("sortOrder");
    QTest::addColumn<int>("expected");

    QOrganizerItem emptyItem;

    {
        QOrganizerItem item1;
        item1.setDescription(QLatin1String("description"));
        item1.setDisplayLabel(QLatin1String("My label"));
        item1.setGuid(QLatin1String("98765"));

        QOrganizerItem item2;
        item2.setDescription(QLatin1String("DESCRIPTION"));
        item2.setDisplayLabel(QLatin1String("label"));
        item2.setGuid(QLatin1String("12345"));
        QOrganizerItemLocation name;
        name.setLabel("test location");
        item2.saveDetail(&name);

        QOrganizerItemSortOrder sortOrder;

        QTest::newRow("empty sortorder") << item1 << item2 << sortOrder << 0;

        sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
        QTest::newRow("empty items") << emptyItem << emptyItem << sortOrder << 0;

        QTest::newRow("equals") << item1 << item1 << sortOrder << 0;
        QTest::newRow("equals") << item2 << item2 << sortOrder << 0;

        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
        {
            sortOrder.setDetail(QOrganizerItemDetail::TypeLocation, -1);
            QTest::newRow("blanks first, field presence") << emptyItem << item2 << sortOrder << -1;
            QTest::newRow("blanks first, field presence") << item1 << item2 << sortOrder << -1;

            sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
            QTest::newRow("blanks first") << emptyItem << item2 << sortOrder << -1;
            QTest::newRow("blanks first") << item2 << emptyItem << sortOrder << 1;
            sortOrder.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
            QTest::newRow("blanks first") << emptyItem << item2 << sortOrder << -1;
            sortOrder.setDetail(QOrganizerItemDetail::TypeTag, QOrganizerItemTag::FieldTag);
            QTest::newRow("blanks first") << item1 << item2 << sortOrder << 0;
        }

        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksLast);
        {
            sortOrder.setDetail(QOrganizerItemDetail::TypeLocation, -1);
            QTest::newRow("blanks last, field presence") << emptyItem << item2 << sortOrder << 1;
            QTest::newRow("blanks last, field presence") << item1 << item2 << sortOrder << 1;

            sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
            QTest::newRow("blanks last") << emptyItem << item2 << sortOrder << 1;
            QTest::newRow("blanks last") << item2 << emptyItem << sortOrder << -1;
            sortOrder.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
            QTest::newRow("blanks first") << emptyItem << item2 << sortOrder << 1;
            sortOrder.setDetail(QOrganizerItemDetail::TypeTag, QOrganizerItemTag::FieldTag);
            QTest::newRow("blanks first") << item1 << item2 << sortOrder << 0;
        }

        sortOrder.setDirection(Qt::AscendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseSensitive);
        {
            sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
            QTest::newRow("asc, cs") << item1 << item2 << sortOrder << 1;
            QTest::newRow("asc, cs") << item2 << item1 << sortOrder << -1;

            sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
            QTest::newRow("asc, ci") << item1 << item2 << sortOrder << 0;
            sortOrder.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
            QTest::newRow("asc, ci") << item1 << item2 << sortOrder << 1;
            QTest::newRow("asc, ci") << item2 << item1 << sortOrder << -1;
        }

        sortOrder.setDirection(Qt::DescendingOrder);
        {
            sortOrder.setCaseSensitivity(Qt::CaseSensitive);
            sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
            QTest::newRow("desc, cs") << item1 << item2 << sortOrder << -1;
            QTest::newRow("desc, cs") << item2 << item1 << sortOrder << 1;

            sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
            QTest::newRow("desc, ci") << item1 << item2 << sortOrder << 0;
            sortOrder.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
            QTest::newRow("desc, ci") << item1 << item2 << sortOrder << -1;
            QTest::newRow("desc, ci") << item2 << item1 << sortOrder << 1;
        }
    }

    { // ensure non-existence is treated just like blank-ness
        QOrganizerItem item1;
        QOrganizerEventTime time1;
        time1.setStartDateTime(QDateTime::currentDateTime());
        time1.setEndDateTime(QDateTime::currentDateTime().addDays(1));
        item1.saveDetail(&time1);

        QOrganizerItem item2;
        QOrganizerEventTime time2;
        time2.setStartDateTime(QDateTime::currentDateTime());
        item2.saveDetail(&time2);

        QOrganizerItemSortOrder sortOrder;
        sortOrder.setDetail(QOrganizerItemDetail::TypeEventTime, QOrganizerEventTime::FieldEndDateTime);
        sortOrder.setDirection(Qt::AscendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);

        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
        QTest::newRow("non-existence vs blank-ness, blanks first") << item1 << item2 << sortOrder << 1;
        QTest::newRow("non-existence vs blank-ness, blanks first") << emptyItem << item1 << sortOrder << -1;
        QTest::newRow("non-existence vs blank-ness, blanks first") << emptyItem << item2 << sortOrder << 0;

        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksLast);
        QTest::newRow("non-existence vs blank-ness, blanks last") << item1 << item2 << sortOrder << -1;
        QTest::newRow("non-existence vs blank-ness, blanks last") << emptyItem << item1 << sortOrder << 1;
        QTest::newRow("non-existence vs blank-ness, blanks last") << emptyItem << item2 << sortOrder << 0;
    }
}

void tst_QOrganizerItemSortOrder::compareItem()
{
    QFETCH(QOrganizerItem, item1);
    QFETCH(QOrganizerItem, item2);
    QFETCH(QOrganizerItemSortOrder, sortOrder);
    QFETCH(int, expected);

    int actual = QOrganizerManagerEngine::compareItem(item1, item2, (QList<QOrganizerItemSortOrder>() << sortOrder));
    actual = qBound(-1, actual, 1);
    QCOMPARE(actual, expected);
}

void tst_QOrganizerItemSortOrder::datastream_data()
{
    QTest::addColumn<QOrganizerItemSortOrder>("sortOrderIn");

    {
        QOrganizerItemSortOrder sortOrder;
        QTest::newRow("default") << sortOrder;
    }

    {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("invalid") << sortOrder;
    }

    {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
        QTest::newRow("detail") << sortOrder;
    }

    {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("complete") << sortOrder;
    }
}

void tst_QOrganizerItemSortOrder::datastream()
{
    QFETCH(QOrganizerItemSortOrder, sortOrderIn);

    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    stream1 << sortOrderIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QOrganizerItemSortOrder sortOrderOut;
    stream2 >> sortOrderOut;
    QCOMPARE(sortOrderOut, sortOrderIn);
}

void tst_QOrganizerItemSortOrder::debugstream_data()
{
    QTest::addColumn<QOrganizerItemSortOrder>("sortOrder");
    QTest::addColumn<QString>("messageExpected");

    {
        QOrganizerItemSortOrder sortOrder;
        QTest::newRow("default") << sortOrder << "QOrganizerItemSortOrder(detailType=0,detailField=-1,blankPolicy=1,direction=0,caseSensitivity=1)";
    }

    {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("invalid") << sortOrder << "QOrganizerItemSortOrder(detailType=0,detailField=-1,blankPolicy=0,direction=1,caseSensitivity=0)";
    }

    {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
        QTest::newRow("detail") << sortOrder << "QOrganizerItemSortOrder(detailType=300,detailField=301,blankPolicy=1,direction=0,caseSensitivity=1)";
    }

    {
        QOrganizerItemSortOrder sortOrder;
        sortOrder.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
        sortOrder.setBlankPolicy(QOrganizerItemSortOrder::BlanksFirst);
        sortOrder.setDirection(Qt::DescendingOrder);
        sortOrder.setCaseSensitivity(Qt::CaseInsensitive);
        QTest::newRow("complete") << sortOrder << "QOrganizerItemSortOrder(detailType=300,detailField=301,blankPolicy=0,direction=1,caseSensitivity=0)";
    }
}

void tst_QOrganizerItemSortOrder::debugstream()
{
    QFETCH(QOrganizerItemSortOrder, sortOrder);
    QFETCH(QString, messageExpected);

    QTest::ignoreMessage(QtDebugMsg, messageExpected.toUtf8());
    qDebug() << sortOrder;
}

QTEST_MAIN(tst_QOrganizerItemSortOrder)
#include "tst_qorganizeritemsortorder.moc"
