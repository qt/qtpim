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
Q_DECLARE_METATYPE(QOrganizerItemFilter)
Q_DECLARE_METATYPE(QOrganizerItemDetailFieldFilter)

static inline QOrganizerItemId makeItemId(uint id)
{
    return QOrganizerItemId(QStringLiteral("qtorganizer:basic:"), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}

static inline QOrganizerCollectionId makeCollectionId(uint id)
{
    return QOrganizerCollectionId(QStringLiteral("qtorganizer:basic:"), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}


class tst_QOrganizerItemFilter : public QObject
{
Q_OBJECT

public:
    tst_QOrganizerItemFilter();
    virtual ~tst_QOrganizerItemFilter();

public slots:
    void init();
    void cleanup();
private slots:
    void classHierarchy();
    void intersectionFilter();
    void unionFilter();
    void detailFilter();
    void detailFieldFilter();
    void detailRangeFilter();
    void boringFilters();
    void idListFilter();
    void collectionFilter();
    void canonicalizedFilter();
    void canonicalizedFilter_data();
    void testFilter();
    void testFilter_data();

    void datastream();
    void datastream_data();
    void traits();

    void testDebugStreamOut();
    void testDebugStreamOut_data();
};

tst_QOrganizerItemFilter::tst_QOrganizerItemFilter()
{
}

tst_QOrganizerItemFilter::~tst_QOrganizerItemFilter()
{
}

void tst_QOrganizerItemFilter::init()
{
}

void tst_QOrganizerItemFilter::cleanup()
{
}

void tst_QOrganizerItemFilter::classHierarchy()
{
    /* Test "casting" up and down the hierarchy */
    QOrganizerItemDetailFieldFilter df;
    QVERIFY(df.type() == QOrganizerItemFilter::DetailFieldFilter);
    df.setDetail(QOrganizerItemDetail::TypeUndefined, 101);
    df.setValue(42);

    QOrganizerItemFilter f = df;
    QVERIFY(f.type() == QOrganizerItemFilter::DetailFieldFilter);

    QOrganizerItemDetailFieldFilter df2 = f;
    QVERIFY(df2.type() == QOrganizerItemFilter::DetailFieldFilter);
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detailField() == 101);
    QVERIFY(df2.value() == 42);

    /* Now try to check if we dangle pointers at all */
    {
        QOrganizerItemFilter f2 = df2;
    }
    QVERIFY(df2.type() == QOrganizerItemFilter::DetailFieldFilter);
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detailField() == 101);
    QVERIFY(df2.value() == 42);

    {
        QOrganizerItemDetailFieldFilter sdf2 = df2;
        sdf2.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
        QVERIFY(sdf2.detailType() == QOrganizerItemDetail::TypeComment);
        QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeUndefined);
    }
    QVERIFY(df2.type() == QOrganizerItemFilter::DetailFieldFilter);
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detailField() == 101);
    QVERIFY(df2.value() == 42);

    /* Try creating a default filter and making sure we don't break */
    QOrganizerItemFilter deff, deff2;

    QVERIFY(deff.type() == QOrganizerItemFilter::DefaultFilter);
    QVERIFY(deff == deff);
    QVERIFY(deff == deff2);
    QVERIFY(deff != df2);
    QVERIFY(df2 != deff);

    QOrganizerItemFilter fdeff = deff;
    QVERIFY(fdeff.type() == QOrganizerItemFilter::DefaultFilter);
    QVERIFY(fdeff == deff);
    QVERIFY(fdeff == deff2);

    /* Now some "invalid" filters */
    QOrganizerItemInvalidFilter iff, iff2;

    QVERIFY(iff.type() == QOrganizerItemFilter::InvalidFilter);
    QVERIFY(iff == iff);
    QVERIFY(iff == iff2);
    QVERIFY(iff != df2);
    QVERIFY(df2 != iff);

    QOrganizerItemFilter fiff = iff;
    QVERIFY(fiff.type() == QOrganizerItemFilter::InvalidFilter);
    QVERIFY(fiff == iff);
    QVERIFY(fiff == iff2);

    /* Now test some "cross casting" */

}

void tst_QOrganizerItemFilter::intersectionFilter()
{
    /* Test boolean ops */
    QOrganizerItemDetailFieldFilter df;
    df.setDetail(QOrganizerItemDetail::TypeUndefined, -1);

    QOrganizerItemDetailFieldFilter df2;
    df2.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);

    QOrganizerItemDetailFieldFilter df3;
    df3.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);

    QOrganizerItemIntersectionFilter bf;
    bf << df << df2;

    QOrganizerItemFilter f = df & df2;

    QVERIFY(bf == f);

    QOrganizerItemFilter f2 = bf & df3;
    QVERIFY(f2.type() == QOrganizerItemFilter::IntersectionFilter);
    QOrganizerItemIntersectionFilter bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == bf);
    QVERIFY(bf2.filters().at(1) == df3);

    f2 = df3 & bf;
    QVERIFY(f2.type() == QOrganizerItemFilter::IntersectionFilter);
    bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == bf);

    /* Save this list */
    QList<QOrganizerItemFilter> filterList = bf2.filters();

    f2 = df & df2 & df3;
    QVERIFY(f2.type() == QOrganizerItemFilter::IntersectionFilter);
    bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == (df & df2));
    QVERIFY(bf2.filters().at(1) == df3);

    /* Self assignment should do nothing */
    bf2 = bf2;
    QVERIFY(bf2 == f2);

    /* Test set filter */
    bf2.setFilters(filterList);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == bf);

    /* Test remove */
    bf2.remove(bf);
    QCOMPARE(bf2.filters().count(), 1);
    QVERIFY(bf2.filters().at(0) == df3);

    /* Double remove, should do nothing */
    bf2.remove(bf);
    QCOMPARE(bf2.filters().count(), 1);
    QVERIFY(bf2.filters().at(0) == df3);

    /* Append/prepend */
    QOrganizerItemIntersectionFilter bf3;
    bf3.append(df);
    QVERIFY(bf3.filters().count() == 1);
    bf3.prepend(df2);
    QVERIFY(bf3.filters().count() == 2);
    QVERIFY(bf3.filters().at(0) == df2);
    QVERIFY(bf3.filters().at(1) == df);
    bf3.append(df3);
    QVERIFY(bf3.filters().count() == 3);
    QVERIFY(bf3.filters().at(0) == df2);
    QVERIFY(bf3.filters().at(1) == df);
    QVERIFY(bf3.filters().at(2) == df3);
    bf3.prepend(df3);
    QVERIFY(bf3.filters().count() == 4);
    QVERIFY(bf3.filters().at(0) == df3);
    QVERIFY(bf3.filters().at(1) == df2);
    QVERIFY(bf3.filters().at(2) == df);
    QVERIFY(bf3.filters().at(3) == df3);

    /* Clear */
    bf3.clear();
    QVERIFY(bf3.filters().isEmpty());
}

void tst_QOrganizerItemFilter::unionFilter()
{
    /* Test boolean ops */
    QOrganizerItemDetailFieldFilter df;
    df.setDetail(QOrganizerItemDetail::TypeUndefined, -1);

    QOrganizerItemDetailFieldFilter df2;
    df2.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);

    QOrganizerItemDetailFieldFilter df3;
    df3.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);

    QOrganizerItemUnionFilter bf;
    bf << df << df2;

    QOrganizerItemFilter f = df | df2;

    QVERIFY(bf == f);

    QOrganizerItemFilter f2 = bf | df3;
    QVERIFY(f2.type() == QOrganizerItemFilter::UnionFilter);
    QOrganizerItemUnionFilter bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 3);
    QVERIFY(bf2.filters().at(0) == df);
    QVERIFY(bf2.filters().at(1) == df2);
    QVERIFY(bf2.filters().at(2) == df3);

    f2 = df3 | bf;
    QVERIFY(f2.type() == QOrganizerItemFilter::UnionFilter);
    bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 3);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == df);
    QVERIFY(bf2.filters().at(2) == df2);

    /* Save this list */
    QList<QOrganizerItemFilter> filterList = bf2.filters();

    f2 = df | df2 | df3;
    QVERIFY(f2.type() == QOrganizerItemFilter::UnionFilter);
    bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 3);
    QVERIFY(bf2.filters().at(0) == df);
    QVERIFY(bf2.filters().at(1) == df2);
    QVERIFY(bf2.filters().at(2) == df3);

    /* Self assignment should do nothing */
    bf2 = bf2;
    QVERIFY(bf2 == f2);

    /* Test set filter */
    bf2.setFilters(filterList);
    QCOMPARE(bf2.filters().count(), 3);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == df);
    QVERIFY(bf2.filters().at(2) == df2);

    /* Test remove */
    bf2.remove(df);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == df2);

    /* Double remove, should do nothing */
    bf2.remove(df);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == df2);

    /* Append/prepend */
    QOrganizerItemUnionFilter bf3;
    bf3.append(df);
    QVERIFY(bf3.filters().count() == 1);
    bf3.prepend(df2);
    QVERIFY(bf3.filters().count() == 2);
    QVERIFY(bf3.filters().at(0) == df2);
    QVERIFY(bf3.filters().at(1) == df);
    bf3.append(df3);
    QVERIFY(bf3.filters().count() == 3);
    QVERIFY(bf3.filters().at(0) == df2);
    QVERIFY(bf3.filters().at(1) == df);
    QVERIFY(bf3.filters().at(2) == df3);
    bf3.prepend(df3);
    QVERIFY(bf3.filters().count() == 4);
    QVERIFY(bf3.filters().at(0) == df3);
    QVERIFY(bf3.filters().at(1) == df2);
    QVERIFY(bf3.filters().at(2) == df);
    QVERIFY(bf3.filters().at(3) == df3);

    /* Clear */
    bf3.clear();
    QVERIFY(bf3.filters().isEmpty());
}

void tst_QOrganizerItemFilter::detailFilter()
{
    QOrganizerItemDetailFilter df;

    QVERIFY(df.type() == QOrganizerItemFilter::DetailFilter);

    QVERIFY(df.detail().type() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df.detail().isEmpty());

    QOrganizerItemComment comment;
    df.setDetail(comment);
    QVERIFY(df.detail().isEmpty());
    QVERIFY(df.detail().type() == QOrganizerItemDetail::TypeComment);
    QVERIFY(!df.detail().values().keys().contains(QOrganizerItemComment::FieldComment));
    QVERIFY(df.detail().value(QOrganizerItemComment::FieldComment).isNull());

    comment.setComment(QStringLiteral("TestComment"));
    df.setDetail(comment);
    QVERIFY(!df.detail().isEmpty());
    QCOMPARE(df.detail().type(), QOrganizerItemDetail::TypeComment);
    QVERIFY(df.detail().values().keys().contains(QOrganizerItemComment::FieldComment));
    QCOMPARE(df.detail().value(QOrganizerItemComment::FieldComment).toString(), QString(QStringLiteral("TestComment")));


    /* Test op= */
    QOrganizerItemFilter f = df;
    QVERIFY(f == df);

    QOrganizerItemDetailFilter df2 = f;
    QVERIFY(df2 == df);
    QCOMPARE(df2.detail(), df.detail());

    /* Self assignment should do nothing */
    df2 = df2;
    QVERIFY(df2 == df);

    /* Some cross casting */
    QOrganizerItemDetailRangeFilter rf;

    /* Directly */
    df2 = rf;
    QCOMPARE(df2.type(), QOrganizerItemFilter::DetailFilter);
    QCOMPARE(df2.detail().type(), QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detail().isEmpty());

    /* reset it */
    df2 = df;
    QCOMPARE(df2.detail().type(), QOrganizerItemDetail::TypeComment);
    QVERIFY(df2.detail().values().keys().contains(QOrganizerItemComment::FieldComment));

    /* Through base class */
    f = rf;
    df2 = f;
    QCOMPARE(df2.detail().type(), QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detail().isEmpty());

    /* Now test copy ctor */
    QOrganizerItemDetailFilter df3(rf);
    QVERIFY(df3.type() == QOrganizerItemFilter::DetailFilter);
    QCOMPARE(df3.detail().type(), QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df3.detail().isEmpty());

    /* reset it */
    df3 = df;
    QCOMPARE(df3.detail().type(), QOrganizerItemDetail::TypeComment);
    QVERIFY(df3.detail().values().keys().contains(QOrganizerItemComment::FieldComment));

    /* Now test copy ctor through base class */
    QOrganizerItemDetailFilter df4(f);
    QCOMPARE(df4.type(), QOrganizerItemFilter::DetailFilter);
    QCOMPARE(df4.detail().type(), QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df4.detail().isEmpty());

    /* reset it */
    df4 = df;
    QCOMPARE(df4.detail().type(), QOrganizerItemDetail::TypeComment);
    QVERIFY(df4.detail().values().keys().contains(QOrganizerItemComment::FieldComment));
}

void tst_QOrganizerItemFilter::detailFieldFilter()
{
    QOrganizerItemDetailFieldFilter df;

    QVERIFY(df.type() == QOrganizerItemFilter::DetailFieldFilter);

    QVERIFY(df.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df.detailField() == -1);
    QVERIFY(df.matchFlags() == 0);
    QVERIFY(df.value().isNull());

    df.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
    QVERIFY(df.detailType() == QOrganizerItemDetail::TypeComment);
    QVERIFY(df.detailField() == QOrganizerItemComment::FieldComment);
    QVERIFY(df.matchFlags() == 0);
    QVERIFY(df.value().isNull());

    df.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);
    QVERIFY(df.detailType() == QOrganizerItemDetail::TypeDescription);
    QVERIFY(df.detailField() == QOrganizerItemDescription::FieldDescription);
    QVERIFY(df.matchFlags() == 0);
    QVERIFY(df.value().isNull());

    df.setMatchFlags(QOrganizerItemFilter::MatchExactly);
    QVERIFY(df.matchFlags() == QOrganizerItemFilter::MatchExactly);

    df.setValue(5);
    QVERIFY(df.value() == 5);

    df.setValue("String value");
    QVERIFY(df.value() == "String value");

    /* Test op= */
    QOrganizerItemFilter f = df;
    QVERIFY(f == df);

    QOrganizerItemDetailFieldFilter df2 = f;
    QVERIFY(df2 == df);
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeDescription);
    QVERIFY(df2.detailField() == QOrganizerItemDescription::FieldDescription);

    /* Self assignment should do nothing */
    df2 = df2;
    QVERIFY(df2 == df);

    /* Some cross casting */
    QOrganizerItemDetailRangeFilter rf;

    /* Directly */
    df2 = rf;
    QVERIFY(df2.type() == QOrganizerItemFilter::DetailFieldFilter);
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detailField() == -1);
    QVERIFY(df2.value().isNull());

    /* reset it */
    df2 = df;
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeDescription);
    QVERIFY(df2.detailField() == QOrganizerItemDescription::FieldDescription);

    /* Through base class */
    f = rf;
    df2 = f;
    QVERIFY(df2.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df2.detailField() == -1);
    QVERIFY(df2.value().isNull());

    /* Now test copy ctor */
    QOrganizerItemDetailFieldFilter df3(rf);
    QVERIFY(df3.type() == QOrganizerItemFilter::DetailFieldFilter);
    QVERIFY(df3.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df3.detailField() == -1);
    QVERIFY(df3.value().isNull());

    /* reset it */
    df3 = df;
    QVERIFY(df3.detailType() == QOrganizerItemDetail::TypeDescription);
    QVERIFY(df3.detailField() == QOrganizerItemDescription::FieldDescription);

    /* Now test copy ctor through base class */
    QOrganizerItemDetailFieldFilter df4(f);
    QVERIFY(df4.type() == QOrganizerItemFilter::DetailFieldFilter);
    QVERIFY(df4.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(df4.detailField() == -1);
    QVERIFY(df4.value().isNull());

    /* reset it */
    df4 = df;
    QVERIFY(df4.detailType() == QOrganizerItemDetail::TypeDescription);
    QVERIFY(df4.detailField() == QOrganizerItemDescription::FieldDescription);
}

void tst_QOrganizerItemFilter::detailRangeFilter()
{
    QOrganizerItemDetailRangeFilter rf;

    QVERIFY(rf.type() == QOrganizerItemFilter::DetailRangeFilter);

    QVERIFY(rf.detailType() == QOrganizerItemDetail::TypeUndefined);
    QVERIFY(rf.detailField() == -1);
    QVERIFY(rf.matchFlags() == 0);

    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setDetail(QOrganizerItemDetail::TypeEventTime, QOrganizerEventTime::FieldStartDateTime);
    QVERIFY(rf.detailType() == QOrganizerItemDetail::TypeEventTime);
    QVERIFY(rf.detailField() == QOrganizerEventTime::FieldStartDateTime);
    QVERIFY(rf.matchFlags() == 0);

    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setDetail(QOrganizerItemDetail::TypeEventTime, QOrganizerEventTime::FieldStartDateTime);
    QVERIFY(rf.detailType() == QOrganizerItemDetail::TypeEventTime);
    QVERIFY(rf.detailField() == QOrganizerEventTime::FieldStartDateTime);
    QVERIFY(rf.matchFlags() == 0);

    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setMatchFlags(QOrganizerItemFilter::MatchExactly);
    QVERIFY(rf.matchFlags() == QOrganizerItemFilter::MatchExactly);

    rf.setMatchFlags(QOrganizerItemFilter::MatchCaseSensitive);
    QVERIFY(rf.matchFlags() == QOrganizerItemFilter::MatchCaseSensitive);

    // Contains is not allowed
    rf.setMatchFlags(QOrganizerItemFilter::MatchCaseSensitive | QOrganizerItemFilter::MatchContains);
    QVERIFY(rf.matchFlags() == QOrganizerItemFilter::MatchCaseSensitive);

    rf.setMatchFlags(QOrganizerItemFilter::MatchEndsWith);
    QVERIFY(rf.matchFlags() == QOrganizerItemFilter::MatchExactly); // 0

    rf.setRange(5, 10);
    QVERIFY(rf.minValue() == 5);
    QVERIFY(rf.maxValue() == 10);
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setRange(QVariant(), 11);
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue() == 11);
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setRange(6, QVariant());
    QVERIFY(rf.minValue() == 6);
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setRange(QVariant(), QVariant());
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setRange(5, 10, QOrganizerItemDetailRangeFilter::ExcludeLower);
    QVERIFY(rf.minValue() == 5);
    QVERIFY(rf.maxValue() == 10);
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::ExcludeLower));

    rf.setRange(QVariant(), 11, QOrganizerItemDetailRangeFilter::IncludeUpper);
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue() == 11);
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::IncludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    rf.setRange(6, QVariant(), QOrganizerItemDetailRangeFilter::ExcludeLower | QOrganizerItemDetailRangeFilter::IncludeUpper);
    QVERIFY(rf.minValue() == 6);
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::IncludeUpper | QOrganizerItemDetailRangeFilter::ExcludeLower));

    rf.setRange(QVariant(), QVariant(), QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower);
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower));

    /* Test op= */
    QOrganizerItemFilter f = rf;
    QVERIFY(f == rf);

    QOrganizerItemDetailRangeFilter rf2 = f;
    QVERIFY(rf2 == rf);

    rf2 = rf;
    QVERIFY(rf2 == f);

    /* Self assignment should do nothing */
    rf2 = rf2;
    QVERIFY(rf2 == rf);
}

void tst_QOrganizerItemFilter::boringFilters()
{
    QOrganizerItemFilter all;
    QVERIFY(all.type() == QOrganizerItemFilter::DefaultFilter);

    QOrganizerItemInvalidFilter invalid;
    QVERIFY(invalid.type() == QOrganizerItemFilter::InvalidFilter);

    QVERIFY(all != invalid);
    QVERIFY(!(all == invalid));

    /* Test op= */
    QOrganizerItemFilter f = all;
    QVERIFY(f == all);

    QOrganizerItemFilter f2;
    f2 = f;
    QVERIFY(f2 == all);

    /* Self assignment should do nothing */
    f2 = f2;
    QVERIFY(f2 == all);

    /* InvalidFilter, op= */
    QOrganizerItemInvalidFilter inv2 = invalid;
    QVERIFY(inv2 == invalid);

    QOrganizerItemInvalidFilter inv3;
    inv3 = inv2;
    QVERIFY(inv3 == invalid);

    inv3 = inv3;
    QVERIFY(inv3 == invalid);

    inv3 = all;
    QVERIFY(inv3 == invalid); // won't be all
}

void tst_QOrganizerItemFilter::idListFilter()
{
    QOrganizerItemIdFilter idf;

    QVERIFY(idf.type() == QOrganizerItemFilter::IdFilter);

    QVERIFY(idf.ids().count() == 0);

    QList<QOrganizerItemId> ids;
    ids << makeItemId(5) << makeItemId(6) << makeItemId(17);

    idf.setIds(ids);
    QVERIFY(idf.ids() == ids);

    idf.setIds(QList<QOrganizerItemId>());
    QVERIFY(idf.ids().count() == 0);

    QOrganizerItemId singleId = makeItemId(12);
    idf.insert(singleId);
    QVERIFY(idf.ids().contains(singleId));
    idf.remove(singleId);
    QVERIFY(!idf.ids().contains(singleId));
    QList<QOrganizerItemId> allIds = idf.ids();
    idf.remove(singleId); // remove again
    QVERIFY(idf.ids() == allIds);
    idf.clear();
    QVERIFY(idf.ids().isEmpty());
    idf.setIds(allIds);

    /* Test op= */
    idf.setIds(ids);
    QOrganizerItemFilter f = idf;
    QVERIFY(f == idf);

    QOrganizerItemIdFilter idf2 = f;
    QVERIFY(idf2 == idf);
    QVERIFY(idf2.ids() == ids);

    idf2 = idf;
    QVERIFY(idf2 == f);

    /* Self assignment should do nothing */
    idf2 = idf2;
    QVERIFY(idf2 == idf);

    QOrganizerItemDetailFieldFilter dfil;
    QOrganizerItemIdFilter idf3(dfil);
    QVERIFY(idf3.type() == QOrganizerItemFilter::IdFilter); // should be a blank id list filter
    QOrganizerItemIdFilter idf4(idf);
    QVERIFY(idf4 == idf); // should be a copy of idf.
    idf = dfil; // now assign.
    QVERIFY(idf == idf3); // again, should be a blank id list filter.
    idf = idf3;
    idf.setIds(ids); // force a detach
}

void tst_QOrganizerItemFilter::collectionFilter()
{
    QOrganizerItemCollectionFilter icf;

    QVERIFY(icf.collectionIds().isEmpty());

    QOrganizerCollectionId id1 = makeCollectionId(5);
    QOrganizerCollectionId id2 = makeCollectionId(6);
    QOrganizerCollectionId id3 = makeCollectionId(7);
    QOrganizerCollectionId id4 = makeCollectionId(12);
    QSet<QOrganizerCollectionId> ids;
    ids << id1 << id2 << id3;

    icf.setCollectionIds(ids);
    QVERIFY(icf.collectionIds() == ids);

    icf.setCollectionId(id4);
    ids.clear();
    ids << id4;
    QVERIFY(icf.collectionIds() == ids);

    QOrganizerItemCollectionFilter icf2;
    icf2 = icf;
    QVERIFY(icf2.collectionIds() == ids);

    QOrganizerItemFilter fil;
    fil = icf;
    QVERIFY(fil.type() == QOrganizerItemFilter::CollectionFilter);

    QOrganizerItemCollectionFilter icf3(fil);
    QVERIFY(fil.type() == QOrganizerItemFilter::CollectionFilter);
    QVERIFY(icf3.collectionIds() == ids);
}

void tst_QOrganizerItemFilter::canonicalizedFilter()
{
    QFETCH(QOrganizerItemFilter, in);
    QFETCH(QOrganizerItemFilter, expected);

    QOrganizerItemFilter out = QOrganizerManagerEngine::canonicalizedFilter(in);
    QCOMPARE(out, expected);
}

void tst_QOrganizerItemFilter::canonicalizedFilter_data()
{
    QTest::addColumn<QOrganizerItemFilter>("in");
    QTest::addColumn<QOrganizerItemFilter>("expected");

    QOrganizerItemDetailFieldFilter detailFilter1;
    detailFilter1.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
    detailFilter1.setValue("1");
    detailFilter1.setMatchFlags(QOrganizerItemFilter::MatchContains);
    QOrganizerItemDetailFieldFilter detailFilter2;
    detailFilter2.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
    detailFilter2.setValue("2");
    detailFilter2.setMatchFlags(QOrganizerItemFilter::MatchContains);
    QOrganizerItemInvalidFilter invalidFilter;
    QOrganizerItemFilter defaultFilter;

    {
        QTest::newRow("Normal detail filter")
                << static_cast<QOrganizerItemFilter>(detailFilter1)
                << static_cast<QOrganizerItemFilter>(detailFilter1);
    }

    {
        QOrganizerItemIntersectionFilter qcif;
        qcif << detailFilter1;
        qcif << detailFilter2;
        QTest::newRow("Normal intersection filter")
                << static_cast<QOrganizerItemFilter>(qcif)
                << static_cast<QOrganizerItemFilter>(qcif);
    }

    {
        QOrganizerItemUnionFilter qcuf;
        qcuf << detailFilter1;
        qcuf << detailFilter2;
        QTest::newRow("Normal intersection filter")
                << static_cast<QOrganizerItemFilter>(qcuf)
                << static_cast<QOrganizerItemFilter>(qcuf);
    }

    {
        QOrganizerItemIntersectionFilter qcif;
        QTest::newRow("Empty intersection")
                << static_cast<QOrganizerItemFilter>(qcif)
                << static_cast<QOrganizerItemFilter>(defaultFilter);
    }

    {
        QOrganizerItemUnionFilter qcuf;
        QTest::newRow("Empty union")
                << static_cast<QOrganizerItemFilter>(qcuf)
                << static_cast<QOrganizerItemFilter>(invalidFilter);
    }

    {
        QOrganizerItemIntersectionFilter qcif;
        qcif << detailFilter1;
        QTest::newRow("Single entry intersection filter")
                << static_cast<QOrganizerItemFilter>(qcif)
                << static_cast<QOrganizerItemFilter>(detailFilter1);
    }

    {
        QOrganizerItemUnionFilter qcuf;
        qcuf << detailFilter1;
        QTest::newRow("Single entry union filter")
                << static_cast<QOrganizerItemFilter>(qcuf)
                << static_cast<QOrganizerItemFilter>(detailFilter1);
    }

    {
        QOrganizerItemIntersectionFilter qcif;
        qcif << invalidFilter;
        qcif << detailFilter1;
        qcif << detailFilter2;
        QTest::newRow("Intersection with invalid")
                << static_cast<QOrganizerItemFilter>(qcif)
                << static_cast<QOrganizerItemFilter>(invalidFilter);
    }

    {
        QOrganizerItemIntersectionFilter qcif;
        qcif << defaultFilter;
        qcif << detailFilter1;
        qcif << detailFilter2;
        QOrganizerItemIntersectionFilter expected;
        expected << detailFilter1;
        expected << detailFilter2;
        QTest::newRow("Intersection with default")
                << static_cast<QOrganizerItemFilter>(qcif)
                << static_cast<QOrganizerItemFilter>(expected);
    }

    {
        QOrganizerItemUnionFilter qcuf;
        qcuf << invalidFilter;
        qcuf << detailFilter1;
        qcuf << detailFilter2;
        QOrganizerItemUnionFilter expected;
        expected << detailFilter1;
        expected << detailFilter2;
        QTest::newRow("Union with invalid")
                << static_cast<QOrganizerItemFilter>(qcuf)
                << static_cast<QOrganizerItemFilter>(expected);
    }

    {
        QOrganizerItemUnionFilter qcuf;
        qcuf << defaultFilter;
        qcuf << detailFilter1;
        qcuf << detailFilter2;
        QTest::newRow("Union with default")
                << static_cast<QOrganizerItemFilter>(qcuf)
                << static_cast<QOrganizerItemFilter>(defaultFilter);
    }

    {
        QOrganizerItemIdFilter qclif;
        QTest::newRow("Empty local id filter")
                << static_cast<QOrganizerItemFilter>(qclif)
                << static_cast<QOrganizerItemFilter>(invalidFilter);
    }

    {
        QOrganizerItemIdFilter qclif;
        qclif.setIds(QList<QOrganizerItemId>() << makeItemId(1) << makeItemId(2));
        QTest::newRow("Normal local id filter")
                << static_cast<QOrganizerItemFilter>(qclif)
                << static_cast<QOrganizerItemFilter>(qclif);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        qcdrf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        QOrganizerItemDetailFieldFilter expected;
        expected.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        QTest::newRow("Null valued range filter")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(expected);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        qcdrf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        qcdrf.setRange(QStringLiteral("a"), QStringLiteral("a"));
        qcdrf.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        QOrganizerItemDetailFieldFilter expected;
        expected.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        expected.setValue(QStringLiteral("a"));
        expected.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        QTest::newRow("Equal valued range filter")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(expected);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        qcdrf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        qcdrf.setRange(QStringLiteral("a"), QStringLiteral("a"),
               QOrganizerItemDetailRangeFilter::ExcludeLower | QOrganizerItemDetailRangeFilter::ExcludeUpper);
        qcdrf.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        QTest::newRow("Equal valued range filter with excluded bounds")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(invalidFilter);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        qcdrf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        qcdrf.setRange(QStringLiteral("a"), QStringLiteral("b"));
        qcdrf.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        QTest::newRow("Normal range filter")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(qcdrf);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        qcdrf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        qcdrf.setRange(QVariant(QVariant::String), QVariant(QVariant::String)); // null bounds
        qcdrf.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        QOrganizerItemDetailFieldFilter qcdf;
        qcdf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        qcdf.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        qcdf.setValue(QVariant(QVariant::String));
        QTest::newRow("Null valued range filter")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(qcdf);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        qcdrf.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        qcdrf.setRange(QVariant(QVariant::String), QStringLiteral("a")); // min is null
        qcdrf.setMatchFlags(QOrganizerItemFilter::MatchFixedString);
        QTest::newRow("One sided range filter")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(qcdrf);
    }

    {
        QOrganizerItemDetailRangeFilter qcdrf;
        QTest::newRow("Empty range filter")
                << static_cast<QOrganizerItemFilter>(qcdrf)
                << static_cast<QOrganizerItemFilter>(invalidFilter);
    }

    {
        QOrganizerItemDetailFieldFilter qcdf;
        QTest::newRow("Empty detail filter")
                << static_cast<QOrganizerItemFilter>(qcdf)
                << static_cast<QOrganizerItemFilter>(invalidFilter);
    }
}

void tst_QOrganizerItemFilter::testFilter()
{
    QFETCH(QOrganizerItem, item);
    QFETCH(QOrganizerItemDetailFieldFilter, filter);
    QFETCH(bool, expected);

    QCOMPARE(QOrganizerManagerEngine::testFilter(filter, item), expected);
}

void tst_QOrganizerItemFilter::testFilter_data()
{
    QTest::addColumn<QOrganizerItem>("item");
    QTest::addColumn<QOrganizerItemDetailFieldFilter>("filter");
    QTest::addColumn<bool>("expected");

    // XXX TODO: other detail types (comment, description, ...)

    {
        QOrganizerItem item;
        QOrganizerItemLocation name;
        name.setLabel("test location");
        item.saveDetail(&name);

        QOrganizerItemDetailFieldFilter filter;
        filter.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        filter.setMatchFlags(QOrganizerItemFilter::MatchContains);

        filter.setValue("test location");
        QTest::newRow("QOrganizerItemLocation::match location")
                << item
                << filter
                << true;

        filter.setValue("ocati");
        QTest::newRow("QOrganizerItemLocation::match substring")
                << item
                << filter
                << true;

        filter.setValue("foo");
        QTest::newRow("QOrganizerItemLocation::match negative")
                << item
                << filter
                << false;
    }

    {
        QOrganizerItem item;
        item.setDisplayLabel(QStringLiteral("foo"));

        QOrganizerItemDetailFieldFilter filter;
        filter.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);
        filter.setMatchFlags(QOrganizerItemFilter::MatchContains);

        filter.setValue("foo");
        QTest::newRow("QOrganizerItemDisplayLabel::match positive")
                << item
                << filter
                << true;

        filter.setValue("o");
        QTest::newRow("QOrganizerItemDisplayLabel::match positive substring")
                << item
                << filter
                << true;

        filter.setValue("bar");
        QTest::newRow("QOrganizerItemDisplayLabel::match negative")
                << item
                << filter
                << false;
    }

    {
        QOrganizerItem item;
        QOrganizerItemPriority priority;
        priority.setPriority(QOrganizerItemPriority::VeryHighPriority);
        item.saveDetail(&priority);

        QOrganizerItemDetailFieldFilter filter;
        filter.setDetail(QOrganizerItemDetail::TypePriority, QOrganizerItemPriority::FieldPriority);
        filter.setMatchFlags(QOrganizerItemFilter::MatchContains);

        filter.setValue(QOrganizerItemPriority::VeryHighPriority);
        QTest::newRow("QOrganizerItemPriority::match positive")
                << item
                << filter
                << true;

        filter.setValue(QOrganizerItemPriority::VeryLowPriority);
        QTest::newRow("QOrganizerItemPhoneNumber::match negative")
                << item
                << filter
                << false;
    }
}

void tst_QOrganizerItemFilter::datastream()
{
    QFETCH(QOrganizerItemFilter, filterIn);

    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    stream1 << filterIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QOrganizerItemFilter filterOut;
    stream2 >> filterOut;
    QCOMPARE(filterOut, filterIn);
}

void tst_QOrganizerItemFilter::datastream_data()
{
    QTest::addColumn<QOrganizerItemFilter>("filterIn");

    {
        QOrganizerItemFilter filter;
        QTest::newRow("default") << filter;
    }

    {
        QOrganizerItemDetailFieldFilter filter;
        filter.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
        filter.setMatchFlags(QOrganizerItemFilter::MatchEndsWith);
        filter.setValue("ski");
        QTest::newRow("detail") << (QOrganizerItemFilter)filter;
    }

    {
        QOrganizerItemIntersectionFilter filter;
        QTest::newRow("intersection") << (QOrganizerItemFilter)filter;
    }

    {
        QOrganizerItemInvalidFilter filter;
        QTest::newRow("invalid") << (QOrganizerItemFilter)filter;
    }

    {
        QOrganizerItemUnionFilter filter;
        QTest::newRow("union") << (QOrganizerItemFilter)filter;
    }

    // NOTE: LocalIdFilter streaming is not supported
}

void tst_QOrganizerItemFilter::traits()
{
    QCOMPARE(sizeof(QOrganizerItemFilter), sizeof(void *));
    QTypeInfo<QOrganizerItemFilter> ti;
    QVERIFY(ti.isComplex);
    QVERIFY(!ti.isStatic);
    QVERIFY(!ti.isLarge);
    QVERIFY(!ti.isPointer);
    QVERIFY(!ti.isDummy);
}

void tst_QOrganizerItemFilter::testDebugStreamOut()
{
    QFETCH(QOrganizerItemFilter, filterIn);
    QFETCH(QString, messageExpected);

    QTest::ignoreMessage(QtDebugMsg, messageExpected.toUtf8());
    qDebug() << filterIn;
}

void tst_QOrganizerItemFilter::testDebugStreamOut_data()
{
    QTest::addColumn<QOrganizerItemFilter>("filterIn");
    QTest::addColumn<QString>("messageExpected");

    {
        QOrganizerItemFilter filter;
        QTest::newRow("default") << filter << "QOrganizerItemFilter((null))";
    }

    {
        QOrganizerItemCollectionFilter filter;
        QOrganizerCollectionId id1 = makeCollectionId(5);
        QOrganizerCollectionId id2 = makeCollectionId(6);
        QOrganizerCollectionId id3 = makeCollectionId(7);
        QOrganizerCollectionId id4 = makeCollectionId(12);
        QSet<QOrganizerCollectionId> ids;
        ids << id1 << id2 << id3;
        filter.setCollectionIds(ids);
        // Testing method setCollectionIds
        QTest::newRow("collection") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::05000000), QOrganizerCollectionId(qtorganizer:basic::06000000), QOrganizerCollectionId(qtorganizer:basic::07000000))))";

        filter.setCollectionId(id2);
        // Testing method setCollectionId (and the related clearing of the collection)
        QTest::newRow("collection") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::06000000))))";
        filter.setCollectionId(id4);
        // Testing again method setCollectionId (and the related clearing of the collection)
        QTest::newRow("collection") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::0c000000))))";
        ids.clear();
        ids << id4;
        // Testing again method setCollectionIds
        QTest::newRow("collection") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::0c000000))))";

        QOrganizerItemCollectionFilter filter2;
        filter2 = filter;
        // Testing again method setCollectionIds on the copied filter
        QTest::newRow("collection") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::0c000000))))";

        QOrganizerItemFilter fil;
        fil = filter;
        // Testing that the assignment/conversion went fine
        QTest::newRow("collection") << (QOrganizerItemFilter)fil << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::0c000000))))";

        QOrganizerItemCollectionFilter filter3(fil);
        QTest::newRow("collection") << (QOrganizerItemFilter)filter3 << "QOrganizerItemFilter(QOrganizerItemCollectionFilter(collectionIds=(QOrganizerCollectionId(qtorganizer:basic::0c000000))))";
    }

    {
        QOrganizerItemDetailFieldFilter filter;
        filter.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);
        filter.setMatchFlags(QOrganizerItemFilter::MatchEndsWith);
        filter.setValue("ski");
        QTest::newRow("detail") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailFieldFilter(detailType=200,detailField=201,value=QVariant(QString, \"ski\"),matchFlags=3))";
    }

    {
        QOrganizerItemDetailRangeFilter filter;
        // Testing the empty fields
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=0,detailField=-1,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

        // Testing the method setDetail
        filter.setDetail(QOrganizerItemDetail::TypeLocation, QOrganizerItemLocation::FieldLabel);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

        // Testing the method rangeFlags
        filter.setMatchFlags(QOrganizerItemFilter::MatchExactly);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

        // Testing the method matchFlags
        filter.setMatchFlags(QOrganizerItemFilter::MatchCaseSensitive);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=16,rangeFlags=0))";

        // Contains is not allowed
        filter.setMatchFlags(QOrganizerItemFilter::MatchCaseSensitive | QOrganizerItemFilter::MatchContains);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=16,rangeFlags=0))";
        filter.setMatchFlags(QOrganizerItemFilter::MatchEndsWith);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

        // Testing the minValue and maxValue
        filter.setRange(5, 10);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(int, 5),maxValue=QVariant(int, 10),matchFlags=0,rangeFlags=0))";

        // Testing the setRange
        filter.setRange(QVariant(), 11);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(int, 11),matchFlags=0,rangeFlags=0))";

        filter.setRange(6, QVariant());
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(int, 6),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

        filter.setRange(QVariant(), QVariant());
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

        filter.setRange(5, 10, QOrganizerItemDetailRangeFilter::ExcludeLower);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(int, 5),maxValue=QVariant(int, 10),matchFlags=0,rangeFlags=2))";  // *

        filter.setRange(QVariant(), 11, QOrganizerItemDetailRangeFilter::IncludeUpper);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(int, 11),matchFlags=0,rangeFlags=1))";

        filter.setRange(6, QVariant(), QOrganizerItemDetailRangeFilter::ExcludeLower | QOrganizerItemDetailRangeFilter::IncludeUpper);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(int, 6),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=3))";

    filter.setRange(QVariant(), QVariant(), QOrganizerItemDetailRangeFilter::ExcludeUpper | QOrganizerItemDetailRangeFilter::IncludeLower);
        QTest::newRow("detailRange") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

    // Test op=
    QOrganizerItemFilter f = filter;
    QTest::newRow("detailRange") << (QOrganizerItemFilter)f << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

    QOrganizerItemDetailRangeFilter filter2 = f;
    QTest::newRow("detailRange") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

    filter2 = filter;
    QTest::newRow("detailRange") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";

    // Self assignment should do nothing
    filter2 = filter2;
    QTest::newRow("detailRange") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemDetailRangeFilter(detailType=700,detailField=703,minValue=QVariant(Invalid),maxValue=QVariant(Invalid),matchFlags=0,rangeFlags=0))";
    }

    {
        // Testing creation of an empty filter
        QOrganizerItemIdFilter filter;
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";

        // Testing the method setIds
        QList<QOrganizerItemId> ids;
        ids << makeItemId(5) << makeItemId(6) << makeItemId(17);
        filter.setIds(ids);
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";

        // Resetting the list of Ids
        filter.setIds(QList<QOrganizerItemId>());
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";

        // Testing the method insert
        QOrganizerItemId singleId = makeItemId(12);
        filter.insert(singleId);
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::0c000000))))";

        // Testing the method remove
        filter.remove(singleId);
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";

        QList<QOrganizerItemId> allIds = filter.ids();
        filter.remove(singleId); // remove again
        filter.clear();
        filter.setIds(allIds);
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";

        // Test op=
        filter.setIds(ids);
        QOrganizerItemFilter f = filter;
        QTest::newRow("Id") << (QOrganizerItemFilter)f << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";
        QOrganizerItemIdFilter filter2 = f;
        QTest::newRow("Id") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";
        filter2 = filter;
        QTest::newRow("Id") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";

        // Self assignment should do nothing
        filter2 = filter2;
        QTest::newRow("Id") << (QOrganizerItemFilter)filter2 << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";

        QOrganizerItemDetailFieldFilter dfil;
        QOrganizerItemIdFilter filter3(dfil);
        QTest::newRow("Id") << (QOrganizerItemFilter)filter3 << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";

        QOrganizerItemIdFilter filter4(filter);
        QTest::newRow("Id") << (QOrganizerItemFilter)filter4 << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";
        filter = dfil; // now assign.
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=()))";
        filter = filter3;
        filter.setIds(ids); // force a detach
        QTest::newRow("Id") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIdFilter(ids=(QOrganizerItemId(qtorganizer:basic::05000000), QOrganizerItemId(qtorganizer:basic::06000000), QOrganizerItemId(qtorganizer:basic::11000000))))";
    }

    {

        // Test empty filter
        QOrganizerItemIntersectionFilter filter;
        QTest::newRow("intersection") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIntersectionFilter(filters=()))";

        // Test boolean ops
        QOrganizerItemDetailFieldFilter filter1;
        filter1.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);

        QOrganizerItemDetailFieldFilter filter2;
        filter2.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);

        QOrganizerItemDetailFieldFilter filter3;
        filter3.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);

        filter << filter1 << filter2;
        QTest::newRow("intersection") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemIntersectionFilter(filters=(QOrganizerItemFilter(QOrganizerItemDetailFieldFilter(detailType=200,detailField=201,value=QVariant(Invalid),matchFlags=0)), QOrganizerItemFilter(QOrganizerItemDetailFieldFilter(detailType=300,detailField=301,value=QVariant(Invalid),matchFlags=0)))))";
    }

    {
        QOrganizerItemInvalidFilter filter;
        QTest::newRow("invalid") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemInvalidFilter())";
    }

    {
        QOrganizerItemUnionFilter filter;
        QTest::newRow("union") << (QOrganizerItemFilter)filter << "QOrganizerItemFilter(QOrganizerItemUnionFilter(filters=()))";

        // Test boolean ops
        QOrganizerItemDetailFieldFilter df;
        df.setDetail(QOrganizerItemDetail::TypeComment, QOrganizerItemComment::FieldComment);

        QOrganizerItemDetailFieldFilter df2;
        df2.setDetail(QOrganizerItemDetail::TypeDescription, QOrganizerItemDescription::FieldDescription);

        QOrganizerItemDetailFieldFilter df3;
        df3.setDetail(QOrganizerItemDetail::TypeDisplayLabel, QOrganizerItemDisplayLabel::FieldLabel);

        QOrganizerItemUnionFilter bf;
        bf << df << df2;
        QTest::newRow("union") << (QOrganizerItemFilter)bf << "QOrganizerItemFilter(QOrganizerItemUnionFilter(filters=(QOrganizerItemFilter(QOrganizerItemDetailFieldFilter(detailType=200,detailField=201,value=QVariant(Invalid),matchFlags=0)), QOrganizerItemFilter(QOrganizerItemDetailFieldFilter(detailType=300,detailField=301,value=QVariant(Invalid),matchFlags=0)))))";
    }

}


QTEST_MAIN(tst_QOrganizerItemFilter)
#include "tst_qorganizeritemfilter.moc"
