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
#include <QMetaType>

#include <QtContacts/qcontacts.h>

//TESTED_COMPONENT=src/contacts

QTCONTACTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QContact)
Q_DECLARE_METATYPE(QContactFilter)

static inline QContactId makeId(const QString &managerName, uint id)
{
    return QContactId(QStringLiteral("qtcontacts:basic%1:").arg(managerName), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}

static inline QContactCollectionId makeCollectionId(uint id)
{
    return QContactCollectionId(QStringLiteral("qtcontacts:basic:"), QByteArray(reinterpret_cast<const char *>(&id), sizeof(uint)));
}

class tst_QContactFilter : public QObject
{
Q_OBJECT

public:
    tst_QContactFilter();
    virtual ~tst_QContactFilter();

public slots:
    void init();
    void cleanup();
private slots:
    void classHierarchy();
    void intersectionFilter();
    void unionFilter();
    void detailFilter();
    void detailRangeFilter();
    void changeLogFilter();
    void actionFilter();
    void relationshipFilter();
    void boringFilters();
    void idListFilter();
    void canonicalizedFilter();
    void canonicalizedFilter_data();
    void testFilter();
    void testFilter_data();
    void collectionFilter();

    void datastream();
    void datastream_data();
    void testDebugStreamOut();
    void testDebugStreamOut_data();
    void traits();
};

tst_QContactFilter::tst_QContactFilter()
{
}

tst_QContactFilter::~tst_QContactFilter()
{
}

void tst_QContactFilter::init()
{
}

void tst_QContactFilter::cleanup()
{
}

void tst_QContactFilter::classHierarchy()
{
    /* Test "casting" up and down the hierarchy */
    QContactDetailRangeFilter drf;
    QVERIFY(drf.type() == QContactFilter::ContactDetailRangeFilter);
    drf.setDetailType(QContactDetail::TypeExtendedDetail, QContactExtendedDetail::FieldName);
    drf.setRange(1, 20);

    QContactFilter f = drf;
    QVERIFY(f.type() == QContactFilter::ContactDetailRangeFilter);

    QContactDetailRangeFilter drf2 = f;
    QVERIFY(drf2.type() == QContactFilter::ContactDetailRangeFilter);
    QVERIFY(drf2.detailType() == QContactDetail::TypeExtendedDetail);
    QVERIFY(drf2.detailField() == QContactExtendedDetail::FieldName);
    QVERIFY(drf2.maxValue() == 20);
    QVERIFY(drf2.minValue() == 1);

    /* Now try to check if we dangle pointers at all */
    {
        QContactFilter f2 = drf2;
    }
    QVERIFY(drf2.type() == QContactFilter::ContactDetailRangeFilter);
    QVERIFY(drf2.detailType() == QContactDetail::TypeExtendedDetail);
    QVERIFY(drf2.detailField() == QContactExtendedDetail::FieldName);
    QVERIFY(drf2.maxValue() == 20);
    QVERIFY(drf2.minValue() == 1);

    {
        QContactDetailRangeFilter rf2 = drf2;
        rf2.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldStreet);
        QVERIFY(rf2.detailType() == QContactDetail::TypeAddress);
        QVERIFY(drf2.detailType() == QContactDetail::TypeExtendedDetail);
    }
    QVERIFY(drf2.type() == QContactFilter::ContactDetailRangeFilter);
    QVERIFY(drf2.detailType() == QContactDetail::TypeExtendedDetail);
    QVERIFY(drf2.detailField() == QContactExtendedDetail::FieldName);
    QVERIFY(drf2.maxValue() == 20);
    QVERIFY(drf2.minValue() == 1);

    /* Try creating a default filter and making sure we don't break */
    QContactFilter deff, deff2;

    QVERIFY(deff.type() == QContactFilter::DefaultFilter);
    QVERIFY(deff == deff);
    QVERIFY(deff == deff2);
    QVERIFY(deff != drf2);
    QVERIFY(drf2 != deff);

    QContactFilter fdeff = deff;
    QVERIFY(fdeff.type() == QContactFilter::DefaultFilter);
    QVERIFY(fdeff == deff);
    QVERIFY(fdeff == deff2);

    /* Now some "invalid" filters */
    QContactInvalidFilter iff, iff2;

    QVERIFY(iff.type() == QContactFilter::InvalidFilter);
    QVERIFY(iff == iff);
    QVERIFY(iff == iff2);
    QVERIFY(iff != drf2);
    QVERIFY(drf2 != iff);

    QContactFilter fiff = iff;
    QVERIFY(fiff.type() == QContactFilter::InvalidFilter);
    QVERIFY(fiff == iff);
    QVERIFY(fiff == iff2);

    /* Now test some "cross casting" */

}

void tst_QContactFilter::intersectionFilter()
{
    /* Test boolean ops */
    QContactDetailFilter df;
    df.setDetailType(QContactDetail::TypeExtendedDetail, QContactExtendedDetail::FieldName);

    QContactDetailFilter df2;
    df2.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldStreet);

    QContactDetailFilter df3;
    df3.setDetailType(QContactDetail::TypePhoneNumber, QContactPhoneNumber::FieldNumber);

    QContactIntersectionFilter bf;
    bf << df << df2;

    QContactFilter f = df & df2;

    QVERIFY(bf == f);

    QContactFilter f2 = bf & df3;
    QVERIFY(f2.type() == QContactFilter::IntersectionFilter);
    QContactIntersectionFilter bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == bf);
    QVERIFY(bf2.filters().at(1) == df3);

    f2 = df3 & bf;
    QVERIFY(f2.type() == QContactFilter::IntersectionFilter);
    bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 2);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == bf);

    /* Save this list */
    QList<QContactFilter> filterList = bf2.filters();

    f2 = df & df2 & df3;
    QVERIFY(f2.type() == QContactFilter::IntersectionFilter);
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
    QContactIntersectionFilter bf3;
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
}

void tst_QContactFilter::unionFilter()
{
    /* Test boolean ops */
    QContactDetailFilter df;
    df.setDetailType(QContactDetail::TypeExtendedDetail, QContactExtendedDetail::FieldName);

    QContactDetailFilter df2;
    df2.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldStreet);

    QContactDetailFilter df3;
    df3.setDetailType(QContactDetail::TypePhoneNumber, QContactPhoneNumber::FieldNumber);

    QContactUnionFilter bf;
    bf << df << df2;

    QContactFilter f = df | df2;

    QVERIFY(bf == f);

    QContactFilter f2 = bf | df3;
    QVERIFY(f2.type() == QContactFilter::UnionFilter);
    QContactUnionFilter bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 3);
    QVERIFY(bf2.filters().at(0) == df);
    QVERIFY(bf2.filters().at(1) == df2);
    QVERIFY(bf2.filters().at(2) == df3);

    f2 = df3 | bf;
    QVERIFY(f2.type() == QContactFilter::UnionFilter);
    bf2 = f2;
    QVERIFY(bf2 == f2);
    QCOMPARE(bf2.filters().count(), 3);
    QVERIFY(bf2.filters().at(0) == df3);
    QVERIFY(bf2.filters().at(1) == df);
    QVERIFY(bf2.filters().at(2) == df2);

    /* Save this list */
    QList<QContactFilter> filterList = bf2.filters();

    f2 = df | df2 | df3;
    QVERIFY(f2.type() == QContactFilter::UnionFilter);
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
    QContactUnionFilter bf3;
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
}

void tst_QContactFilter::actionFilter()
{
    QContactActionFilter af;

    /* Test initial conditions */
    QVERIFY(af.type() == QContactFilter::ActionFilter);
    QVERIFY(af.actionName().isEmpty());

    af.setActionName("Action Name");
    QVERIFY(af.actionName() == "Action Name");

    af.setActionName(QString());
    QVERIFY(af.actionName().isEmpty());

    /* Test op= */
    QContactFilter f = af;
    QVERIFY(f == af);

    QContactActionFilter af2 = f;
    QVERIFY(af2 == af);

    /* Self assignment should do nothing */
    af2 = af2;
    QVERIFY(af2 == af);

    QContactDetailFilter dfil;
    QContactActionFilter af3(dfil);
    QVERIFY(af3.type() == QContactFilter::ActionFilter);
    QContactActionFilter af4(af);
    QVERIFY(af4 == af);
    af = dfil;
    QVERIFY(af == af3);
    af = af3;
    af.setActionName("test"); // should force a detach
}

void tst_QContactFilter::changeLogFilter()
{
    QContactChangeLogFilter cf;
    QContactChangeLogFilter cfadded(QContactChangeLogFilter::EventAdded);
    QContactChangeLogFilter cfchanged(QContactChangeLogFilter::EventChanged);
    QContactChangeLogFilter cfremoved(QContactChangeLogFilter::EventRemoved);

    QVERIFY(cf.type() == QContactFilter::ChangeLogFilter);
    QVERIFY(cf.eventType() == QContactChangeLogFilter::EventAdded);

    QVERIFY(cfadded.type() == QContactFilter::ChangeLogFilter);
    QVERIFY(cfadded.eventType() == QContactChangeLogFilter::EventAdded);

    QVERIFY(cfchanged.type() == QContactFilter::ChangeLogFilter);
    QVERIFY(cfchanged.eventType() == QContactChangeLogFilter::EventChanged);

    QVERIFY(cfremoved.type() == QContactFilter::ChangeLogFilter);
    QVERIFY(cfremoved.eventType() == QContactChangeLogFilter::EventRemoved);


    /* Just to break the naming scheme */
    cfchanged.setEventType(QContactChangeLogFilter::EventAdded);
    QVERIFY(cfchanged.eventType() == QContactChangeLogFilter::EventAdded);

    QVERIFY(cf.since() == QDateTime());

    QDateTime now = QDateTime::currentDateTime();
    cf.setSince(now);

    QVERIFY(cf.since() == now);

    cf.setSince(QDateTime());
    QVERIFY(cf.since() == QDateTime());

    /* Test op= */
    QContactFilter f = cf;
    QVERIFY(f == cf);

    QContactChangeLogFilter cf2 = f;
    QVERIFY(cf2 == cf);

    /* Self assignment should do nothing */
    cf2 = cf2;
    QVERIFY(cf2 == cf);

    QContactDetailFilter dfil;
    QContactChangeLogFilter cf3(dfil);
    QVERIFY(cf3.type() == QContactFilter::ChangeLogFilter);
    QContactChangeLogFilter cf4(cf);
    QVERIFY(cf4 == cf);
    cf = dfil;
    QVERIFY(cf == cf3);
    cf = cf3;
    cf.setEventType(QContactChangeLogFilter::EventRemoved); // force a detach
}

void tst_QContactFilter::detailFilter()
{
    QContactDetailFilter df;

    QVERIFY(df.type() == QContactFilter::ContactDetailFilter);
    QVERIFY(df.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(df.detailField() == -1);
    QVERIFY(df.matchFlags() == 0);
    QVERIFY(df.value().isNull());

    df.setDetailType(QContactDetail::TypeAddress);
    QVERIFY(df.detailType() == QContactDetail::TypeAddress);
    QVERIFY(df.detailField() == -1);
    QVERIFY(df.matchFlags() == 0);
    QVERIFY(df.value().isNull());

    df.setDetailType(QContactDetail::TypeAddress, QContactAddress::FieldStreet);
    QVERIFY(df.detailType() == QContactDetail::TypeAddress);
    QVERIFY(df.detailField() == QContactAddress::FieldStreet);
    QVERIFY(df.matchFlags() == 0);
    QVERIFY(df.value().isNull());

    df.setMatchFlags(QContactFilter::MatchExactly);
    QVERIFY(df.matchFlags() == QContactFilter::MatchExactly);

    df.setValue(5);
    QVERIFY(df.value() == 5);

    df.setValue("String value");
    QVERIFY(df.value() == "String value");

    /* Test op= */
    QContactFilter f = df;
    QVERIFY(f == df);

    QContactDetailFilter df2 = f;
    QVERIFY(df2 == df);
    QVERIFY(df2.detailType() == QContactDetail::TypeAddress);
    QVERIFY(df2.detailField() == QContactAddress::FieldStreet);

    /* Self assignment should do nothing */
    df2 = df2;
    QVERIFY(df2 == df);

    /* Some cross casting */
    QContactDetailRangeFilter rf;

    /* Directly */
    df2 = rf;
    QVERIFY(df2.type() == QContactFilter::ContactDetailFilter);
    QVERIFY(df2.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(df2.detailField() == -1);
    QVERIFY(df2.value().isNull());

    /* reset it */
    df2 = df;
    QVERIFY(df2.detailType() == QContactDetail::TypeAddress);
    QVERIFY(df2.detailField() == QContactAddress::FieldStreet);

    /* Through base class */
    f = rf;
    df2 = f;
    QVERIFY(df2.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(df2.detailField() == -1);
    QVERIFY(df2.value().isNull());

    /* Now test copy ctor */
    QContactDetailFilter df3(rf);
    QVERIFY(df3.type() == QContactFilter::ContactDetailFilter);
    QVERIFY(df3.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(df3.detailField() == -1);
    QVERIFY(df3.value().isNull());

    /* reset it */
    df3 = df;
    QVERIFY(df3.detailType() == QContactDetail::TypeAddress);
    QVERIFY(df3.detailField() == QContactAddress::FieldStreet);

    /* Now test copy ctor through base class */
    QContactDetailFilter df4(f);
    QVERIFY(df4.type() == QContactFilter::ContactDetailFilter);
    QVERIFY(df4.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(df4.detailField() == -1);
    QVERIFY(df4.value().isNull());

    /* reset it */
    df4 = df;
    QVERIFY(df4.detailType() == QContactDetail::TypeAddress);
    QVERIFY(df4.detailField() == QContactAddress::FieldStreet);
}

void tst_QContactFilter::detailRangeFilter()
{
    QContactDetailRangeFilter rf;

    QVERIFY(rf.type() == QContactFilter::ContactDetailRangeFilter);

    QVERIFY(rf.detailType() == QContactDetail::TypeUndefined);
    QVERIFY(rf.detailField() == -1);
    QVERIFY(rf.matchFlags() == 0);

    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setDetailType(QContactDetail::TypeExtendedDetail);
    QVERIFY(rf.detailType() == QContactDetail::TypeExtendedDetail);
    QVERIFY(rf.detailField() == -1);
    QVERIFY(rf.matchFlags() == 0);

    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setDetailType(QContactDetail::TypeExtendedDetail, QContactExtendedDetail::FieldName);
    QVERIFY(rf.detailType() == QContactDetail::TypeExtendedDetail);
    QVERIFY(rf.detailField() == QContactExtendedDetail::FieldName);
    QVERIFY(rf.matchFlags() == 0);

    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setMatchFlags(QContactFilter::MatchExactly);
    QVERIFY(rf.matchFlags() == QContactFilter::MatchExactly);

    rf.setMatchFlags(QContactFilter::MatchCaseSensitive);
    QVERIFY(rf.matchFlags() == QContactFilter::MatchCaseSensitive);

    // Contains is not allowed
    rf.setMatchFlags(QContactFilter::MatchCaseSensitive | QContactFilter::MatchContains);
    QVERIFY(rf.matchFlags() == QContactFilter::MatchCaseSensitive);

    rf.setMatchFlags(QContactFilter::MatchEndsWith);
    QVERIFY(rf.matchFlags() == QContactFilter::MatchExactly); // 0

    rf.setRange(5, 10);
    QVERIFY(rf.minValue() == 5);
    QVERIFY(rf.maxValue() == 10);
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setRange(QVariant(), 11);
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue() == 11);
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setRange(6, QVariant());
    QVERIFY(rf.minValue() == 6);
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setRange(QVariant(), QVariant());
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setRange(5, 10, QContactDetailRangeFilter::ExcludeLower);
    QVERIFY(rf.minValue() == 5);
    QVERIFY(rf.maxValue() == 10);
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::ExcludeLower));

    rf.setRange(QVariant(), 11, QContactDetailRangeFilter::IncludeUpper);
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue() == 11);
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::IncludeUpper | QContactDetailRangeFilter::IncludeLower));

    rf.setRange(6, QVariant(), QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::IncludeUpper);
    QVERIFY(rf.minValue() == 6);
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::IncludeUpper | QContactDetailRangeFilter::ExcludeLower));

    rf.setRange(QVariant(), QVariant(), QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower);
    QVERIFY(rf.minValue().isNull());
    QVERIFY(rf.maxValue().isNull());
    QVERIFY(rf.rangeFlags() == (QContactDetailRangeFilter::ExcludeUpper | QContactDetailRangeFilter::IncludeLower));

    /* Test op= */
    QContactFilter f = rf;
    QVERIFY(f == rf);

    QContactDetailRangeFilter rf2 = f;
    QVERIFY(rf2 == rf);

    rf2 = rf;
    QVERIFY(rf2 == f);

    /* Self assignment should do nothing */
    rf2 = rf2;
    QVERIFY(rf2 == rf);
}

void tst_QContactFilter::relationshipFilter()
{
    QContactRelationshipFilter crf;

    QVERIFY(crf.type() == QContactFilter::RelationshipFilter);


    QVERIFY(crf.relationshipType() == QString());
    QVERIFY(crf.relatedContactId() == QContactId());

    QContact newContact;
    newContact.setId(makeId("test", 5));
    crf.setRelatedContactId(newContact.id());

    QVERIFY(crf.relationshipType() == QString());
    QVERIFY(crf.relatedContactId() == newContact.id());

    crf.setRelatedContactRole(QContactRelationship::First);

    QVERIFY(crf.relationshipType() == QString());
    QVERIFY(crf.relatedContactId() == newContact.id());

    crf.setRelationshipType(QContactRelationship::HasManager());

    QVERIFY(crf.relationshipType() == QContactRelationship::HasManager());
    QVERIFY(crf.relatedContactId() == newContact.id());

    /* Test op= */
    QContactFilter f = crf;
    QVERIFY(f == crf);

    QContactRelationshipFilter crf2 = f;
    QVERIFY(crf2 == crf);

    /* Self assignment should do nothing */
    crf2 = crf2;
    QVERIFY(crf2 == crf);

    QContactDetailFilter dfil;
    QContactRelationshipFilter crf3(dfil);
    QVERIFY(crf3.type() == QContactFilter::RelationshipFilter); // should be a blank rel fil
    QContactRelationshipFilter crf4(crf);
    QVERIFY(crf4 == crf);
    crf = dfil;
    QVERIFY(crf == crf3);
    crf = crf3;
    crf.setRelationshipType("test"); // force a detach
}



void tst_QContactFilter::boringFilters()
{
    QContactFilter all;
    QVERIFY(all.type() == QContactFilter::DefaultFilter);

    QContactInvalidFilter invalid;
    QVERIFY(invalid.type() == QContactFilter::InvalidFilter);

    QVERIFY(all != invalid);
    QVERIFY(!(all == invalid));

    /* Test op= */
    QContactFilter f = all;
    QVERIFY(f == all);

    QContactFilter f2;
    f2 = f;
    QVERIFY(f2 == all);

    /* Self assignment should do nothing */
    f2 = f2;
    QVERIFY(f2 == all);

    /* InvalidFilter, op= */
    QContactInvalidFilter inv2 = invalid;
    QVERIFY(inv2 == invalid);

    QContactInvalidFilter inv3;
    inv3 = inv2;
    QVERIFY(inv3 == invalid);

    inv3 = inv3;
    QVERIFY(inv3 == invalid);

    inv3 = all;
    QVERIFY(inv3 == invalid); // won't be all
}

void tst_QContactFilter::idListFilter()
{
    QContactIdFilter idf;

    QVERIFY(idf.type() == QContactFilter::IdFilter);

    QVERIFY(idf.ids().count() == 0);

    QList<QContactId> ids;
    ids << QContactId() << QContactId() << QContactId();

    idf.setIds(ids);
    QVERIFY(idf.ids() == ids);

    idf.setIds(QList<QContactId>());
    QVERIFY(idf.ids().count() == 0);

    /* Test op= */
    idf.setIds(ids);
    QContactFilter f = idf;
    QVERIFY(f == idf);

    QContactIdFilter idf2 = f;
    QVERIFY(idf2 == idf);
    QVERIFY(idf2.ids() == ids);

    idf2 = idf;
    QVERIFY(idf2 == f);

    /* Self assignment should do nothing */
    idf2 = idf2;
    QVERIFY(idf2 == idf);

    QContactDetailFilter dfil;
    QContactIdFilter idf3(dfil);
    QVERIFY(idf3.type() == QContactFilter::IdFilter); // should be a blank id list filter
    QContactIdFilter idf4(idf);
    QVERIFY(idf4 == idf); // should be a copy of idf.
    idf = dfil; // now assign.
    QVERIFY(idf == idf3); // again, should be a blank id list filter.
    idf = idf3;
    idf.setIds(ids); // force a detach
}

void tst_QContactFilter::canonicalizedFilter()
{
    QFETCH(QContactFilter, in);
    QFETCH(QContactFilter, expected);

    QContactFilter out = QContactManagerEngine::canonicalizedFilter(in);
    QCOMPARE(out, expected);
}

void tst_QContactFilter::canonicalizedFilter_data()
{
    QTest::addColumn<QContactFilter>("in");
    QTest::addColumn<QContactFilter>("expected");

    QContactFilter detailFilter1 = QContactName::match("1");
    QContactFilter detailFilter2 = QContactName::match("2");
    QContactInvalidFilter invalidFilter;
    QContactFilter defaultFilter;

    {
        QTest::newRow("Normal detail filter")
                << static_cast<QContactFilter>(detailFilter1)
                << static_cast<QContactFilter>(detailFilter1);
    }

    {
        QContactIntersectionFilter qcif;
        qcif << detailFilter1;
        qcif << detailFilter2;
        QTest::newRow("Normal intersection filter")
                << static_cast<QContactFilter>(qcif)
                << static_cast<QContactFilter>(qcif);
    }

    {
        QContactUnionFilter qcuf;
        qcuf << detailFilter1;
        qcuf << detailFilter2;
        QTest::newRow("Normal intersection filter")
                << static_cast<QContactFilter>(qcuf)
                << static_cast<QContactFilter>(qcuf);
    }

    {
        QContactIntersectionFilter qcif;
        QTest::newRow("Empty intersection")
                << static_cast<QContactFilter>(qcif)
                << static_cast<QContactFilter>(defaultFilter);
    }

    {
        QContactUnionFilter qcuf;
        QTest::newRow("Empty union")
                << static_cast<QContactFilter>(qcuf)
                << static_cast<QContactFilter>(invalidFilter);
    }

    {
        QContactIntersectionFilter qcif;
        qcif << detailFilter1;
        QTest::newRow("Single entry intersection filter")
                << static_cast<QContactFilter>(qcif)
                << static_cast<QContactFilter>(detailFilter1);
    }

    {
        QContactUnionFilter qcuf;
        qcuf << detailFilter1;
        QTest::newRow("Single entry union filter")
                << static_cast<QContactFilter>(qcuf)
                << static_cast<QContactFilter>(detailFilter1);
    }

    {
        QContactIntersectionFilter qcif;
        qcif << invalidFilter;
        qcif << detailFilter1;
        qcif << detailFilter2;
        QTest::newRow("Intersection with invalid")
                << static_cast<QContactFilter>(qcif)
                << static_cast<QContactFilter>(invalidFilter);
    }

    {
        QContactIntersectionFilter qcif;
        qcif << defaultFilter;
        qcif << detailFilter1;
        qcif << detailFilter2;
        QContactIntersectionFilter expected;
        expected << detailFilter1;
        expected << detailFilter2;
        QTest::newRow("Intersection with default")
                << static_cast<QContactFilter>(qcif)
                << static_cast<QContactFilter>(expected);
    }

    {
        QContactUnionFilter qcuf;
        qcuf << invalidFilter;
        qcuf << detailFilter1;
        qcuf << detailFilter2;
        QContactUnionFilter expected;
        expected << detailFilter1;
        expected << detailFilter2;
        QTest::newRow("Union with invalid")
                << static_cast<QContactFilter>(qcuf)
                << static_cast<QContactFilter>(expected);
    }

    {
        QContactUnionFilter qcuf;
        qcuf << defaultFilter;
        qcuf << detailFilter1;
        qcuf << detailFilter2;
        QTest::newRow("Union with default")
                << static_cast<QContactFilter>(qcuf)
                << static_cast<QContactFilter>(defaultFilter);
    }

    {
        QContactIdFilter qclif;
        QTest::newRow("Empty local id filter")
                << static_cast<QContactFilter>(qclif)
                << static_cast<QContactFilter>(invalidFilter);
    }

    {
        QContactIdFilter qclif;
        qclif.setIds(QList<QContactId>() << QContactId() << QContactId());
        QTest::newRow("Normal local id filter")
                << static_cast<QContactFilter>(qclif)
                << static_cast<QContactFilter>(qclif);
    }

    {
        QContactDetailRangeFilter qcdrf;
        qcdrf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        QContactDetailFilter expected;
        expected.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        QTest::newRow("Null valued range filter")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(expected);
    }

    {
        QContactDetailRangeFilter qcdrf;
        qcdrf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        qcdrf.setRange(QStringLiteral("a"), QStringLiteral("a"));
        qcdrf.setMatchFlags(QContactFilter::MatchFixedString);
        QContactDetailFilter expected;
        expected.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        expected.setValue(QStringLiteral("a"));
        expected.setMatchFlags(QContactFilter::MatchFixedString);
        QTest::newRow("Equal valued range filter")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(expected);
    }

    {
        QContactDetailRangeFilter qcdrf;
        qcdrf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        qcdrf.setRange(QStringLiteral("a"), QStringLiteral("a"),
               QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper);
        qcdrf.setMatchFlags(QContactFilter::MatchFixedString);
        QTest::newRow("Equal valued range filter with excluded bounds")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(invalidFilter);
    }

    {
        QContactDetailRangeFilter qcdrf;
        qcdrf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        qcdrf.setRange(QStringLiteral("a"), QStringLiteral("b"));
        qcdrf.setMatchFlags(QContactFilter::MatchFixedString);
        QTest::newRow("Normal range filter")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(qcdrf);
    }

    {
        QContactDetailRangeFilter qcdrf;
        qcdrf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        qcdrf.setRange(QVariant(QVariant::String), QVariant(QVariant::String)); // null bounds
        qcdrf.setMatchFlags(QContactFilter::MatchFixedString);
        QContactDetailFilter qcdf;
        qcdf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        qcdf.setMatchFlags(QContactFilter::MatchFixedString);
        qcdf.setValue(QVariant(QVariant::String));
        QTest::newRow("Null valued range filter")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(qcdf);
    }

    {
        QContactDetailRangeFilter qcdrf;
        qcdrf.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        qcdrf.setRange(QVariant(QVariant::String), QStringLiteral("a")); // min is null
        qcdrf.setMatchFlags(QContactFilter::MatchFixedString);
        QTest::newRow("One sided range filter")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(qcdrf);
    }

    {
        QContactDetailRangeFilter qcdrf;
        QTest::newRow("Empty range filter")
                << static_cast<QContactFilter>(qcdrf)
                << static_cast<QContactFilter>(invalidFilter);
    }

    {
        QContactDetailFilter qcdf;
        QTest::newRow("Empty detail filter")
                << static_cast<QContactFilter>(qcdf)
                << static_cast<QContactFilter>(invalidFilter);
    }
}

void tst_QContactFilter::testFilter()
{
    QFETCH(QContact, contact);
    QFETCH(QContactFilter, filter);
    QFETCH(bool, expected);

    QCOMPARE(QContactManagerEngine::testFilter(filter, contact), expected);
}

void tst_QContactFilter::testFilter_data()
{
    QTest::addColumn<QContact>("contact");
    QTest::addColumn<QContactFilter>("filter");
    QTest::addColumn<bool>("expected");

    {
        QContact contact;
        QContactName name;
        name.setFirstName(QStringLiteral("first"));
        name.setMiddleName(QStringLiteral("middle"));
        name.setLastName(QStringLiteral("last"));
        name.setPrefix(QStringLiteral("prefix"));
        name.setSuffix(QStringLiteral("suffix"));
        contact.saveDetail(&name);

        QTest::newRow("QContactName::match firstname")
                << contact
                << QContactName::match("first")
                << true;
        QTest::newRow("QContactName::match lastname")
                << contact
                << QContactName::match("last")
                << true;
        QTest::newRow("QContactName::match middlename")
                << contact
                << QContactName::match("middle")
                << true;
        QTest::newRow("QContactName::match prefix")
                << contact
                << QContactName::match("prefix")
                << true;
        QTest::newRow("QContactName::match suffix")
                << contact
                << QContactName::match("suffix")
                << true;
        QTest::newRow("QContactName::match first last")
                << contact
                << QContactName::match(QStringLiteral("first"), QStringLiteral("last"))
                << true;
        QTest::newRow("QContactName::match substring")
                << contact
                << QContactName::match(QStringLiteral("irs"))
                << true;
        QTest::newRow("QContactName::match first last substring")
                << contact
                << QContactName::match(QStringLiteral("irs"), QStringLiteral("as"))
                << true;
        QTest::newRow("QContactName::match negative")
                << contact
                << QContactName::match("foo")
                << false;

        QContactDetailFilter dfWithEmptyValue;
        dfWithEmptyValue.setDetailType(QContactName::Type, QContactName::FieldFirstName);
        dfWithEmptyValue.setMatchFlags(QContactFilter::MatchExactly);
        dfWithEmptyValue.setValue("");

        QTest::newRow("QContactDetailFilter exact match for empty value")
                << contact
                << (QContactFilter)dfWithEmptyValue
                << false;

        QContactDetailFilter dfWithEmptyFieldName;
        dfWithEmptyFieldName.setDetailType(QContactName::Type);
        dfWithEmptyFieldName.setMatchFlags(QContactFilter::MatchExactly);
        dfWithEmptyFieldName.setValue("");

        QTest::newRow("QContactDetailFilter exact match for detail name but empty field name")
                << contact
                << (QContactFilter)dfWithEmptyFieldName
                << true;
    }

    {
        QContact contact;
        QContactDisplayLabel displaylabel;
        displaylabel.setLabel("foo");
        contact.saveDetail(&displaylabel);
        QTest::newRow("QContactDisplayLabel::match positive")
                << contact
                << QContactDisplayLabel::match("foo")
                << true;
        QTest::newRow("QContactDisplayLabel::match positive substring")
                << contact
                << QContactDisplayLabel::match("o")
                << true;
        QTest::newRow("QContactDisplayLabel::match negative")
                << contact
                << QContactDisplayLabel::match("bar")
                << false;
    }

    {
        QContact contact;
        QContactPhoneNumber phone;
        phone.setNumber("1234");
        contact.saveDetail(&phone);
        QTest::newRow("QContactPhoneNumber::match positive")
                << contact
                << QContactPhoneNumber::match("1234")
                << true;
        QTest::newRow("QContactPhoneNumber::match negative")
                << contact
                << QContactPhoneNumber::match("5678")
                << false;
    }

    {
        QContact contact;
        QContactEmailAddress email;
        email.setEmailAddress("foo");
        contact.saveDetail(&email);
        QTest::newRow("QContactEmailAddress::match positive")
                << contact
                << QContactEmailAddress::match("foo")
                << true;
        QTest::newRow("QContactEmailAddress::match positive substring")
                << contact
                << QContactEmailAddress::match("o")
                << true;
        QTest::newRow("QContactEmailAddress::match negative")
                << contact
                << QContactEmailAddress::match("bar")
                << false;
    }
    {
        QContact contact;
        QContactOrganization org;
        org.setDepartment(QStringList("one")); // Single department as a stringlist
        contact.saveDetail(&org);
        QContactDetailFilter df;
        df.setDetailType(QContactOrganization::Type, QContactOrganization::FieldDepartment);

        // First case sensitive
        df.setMatchFlags(QContactDetailFilter::MatchCaseSensitive);
        df.setValue("one"); // this is a string
        QTest::newRow("QContactOrganization::match single positive against string")
                << contact
                << QContactFilter(df)
                << true;

        df.setValue(QStringList("one")); // this is a stringlist of the same length
        QTest::newRow("QContactOrganization::match single positive against stringlist")
                << contact
                << QContactFilter(df)
                << true;

        df.setValue(QString("two"));
        QTest::newRow("QContactOrganization::match negative string")
                << contact
                << QContactFilter(df)
                << false;

        df.setValue(QStringList("two"));
        QTest::newRow("QContactOrganization::match negative stringlist")
                << contact
                << QContactFilter(df)
                << false;

        df.setValue(QString("ONE"));
        QTest::newRow("QContactOrganization::match negative case sensitive string")
                << contact
                << QContactFilter(df)
                << false;

        df.setValue(QStringList("ONE"));
        QTest::newRow("QContactOrganization::match negative case sensitive stringlist")
                << contact
                << QContactFilter(df)
                << false;

        // Now case insensitive
        df.setMatchFlags(0);
        df.setValue("one"); // this is a string
        QTest::newRow("QContactOrganization::match positive insensitive against string")
                << contact
                << QContactFilter(df)
                << true;

        df.setValue(QStringList("one")); // this is a stringlist of the same length
        QTest::newRow("QContactOrganization::match positive insensitive against stringlist")
                << contact
                << QContactFilter(df)
                << true;

        df.setValue(QString("two"));
        QTest::newRow("QContactOrganization::match negative insensitive string")
                << contact
                << QContactFilter(df)
                << false;

        df.setValue(QStringList("two"));
        QTest::newRow("QContactOrganization::match negative insensitive stringlist")
                << contact
                << QContactFilter(df)
                << false;

        df.setValue(QString("ONE"));
        QTest::newRow("QContactOrganization::match positive case insensitive string 2")
                << contact
                << QContactFilter(df)
                << true;

        df.setValue(QStringList("ONE"));
        QTest::newRow("QContactOrganization::match positive case insensitive stringlist 2")
                << contact
                << QContactFilter(df)
                << true;
    }
}

void tst_QContactFilter::collectionFilter()
{
    QContactCollectionFilter icf;

    QVERIFY(icf.collectionIds().isEmpty());

    QContactCollectionId id1 = makeCollectionId(5);
    QContactCollectionId id2 = makeCollectionId(6);
    QContactCollectionId id3 = makeCollectionId(7);
    QContactCollectionId id4 = makeCollectionId(12);
    QSet<QContactCollectionId> ids;
    ids << id1 << id2 << id3;

    icf.setCollectionIds(ids);
    QVERIFY(icf.collectionIds() == ids);

    icf.setCollectionId(id4);
    ids.clear();
    ids << id4;
    QVERIFY(icf.collectionIds() == ids);

    QContactCollectionFilter icf2;
    icf2 = icf;
    QVERIFY(icf2.collectionIds() == ids);

    QContactFilter fil;
    fil = icf;
    QVERIFY(fil.type() == QContactFilter::CollectionFilter);

    QContactCollectionFilter icf3(fil);
    QVERIFY(fil.type() == QContactFilter::CollectionFilter);
    QVERIFY(icf3.collectionIds() == ids);
}

void tst_QContactFilter::datastream()
{
    QFETCH(QContactFilter, filterIn);

    QByteArray buffer;
    QDataStream stream1(&buffer, QIODevice::WriteOnly);
    stream1 << filterIn;

    QVERIFY(buffer.size() > 0);

    QDataStream stream2(buffer);
    QContactFilter filterOut;
    stream2 >> filterOut;
    QCOMPARE(filterOut, filterIn);
}

void tst_QContactFilter::datastream_data()
{
    QTest::addColumn<QContactFilter>("filterIn");

    {
        QContactFilter filter;
        QTest::newRow("default") << filter;
    }

    {
        QContactActionFilter filter;
        filter.setActionName("action name");
        QTest::newRow("action") << (QContactFilter)filter;
    }

    {
        QContactChangeLogFilter filter;
        filter.setEventType(QContactChangeLogFilter::EventAdded);
        filter.setSince(QDateTime(QDate(2010, 6, 1), QTime(1, 2, 3)));
        QTest::newRow("changelog") << (QContactFilter)filter;
    }

    {
        QContactDetailFilter filter;
        filter.setDetailType(QContactDetail::TypeAddress, QContactPhoneNumber::FieldNumber);
        filter.setMatchFlags(QContactFilter::MatchEndsWith);
        filter.setValue("ski");
        QTest::newRow("detail") << (QContactFilter)filter;
    }

    {
        QContactIntersectionFilter filter;
        QTest::newRow("intersection") << (QContactFilter)filter;
    }

    {
        QContactInvalidFilter filter;
        QTest::newRow("invalid") << (QContactFilter)filter;
    }

    {
        QContactIdFilter filter;
        filter.setIds(QList<QContactId>() << QContactId() << QContactId() << QContactId());
        QTest::newRow("localid") << (QContactFilter)filter;
    }

    {
        QContactRelationshipFilter filter;
        filter.setRelationshipType("member");
        filter.setRelatedContactId(QContactId());
        filter.setRelatedContactRole(QContactRelationship::First);
        QTest::newRow("relationship") << (QContactFilter)filter;
    }

    {
        QContactUnionFilter filter;
        QTest::newRow("union") << (QContactFilter)filter;
    }
}

void tst_QContactFilter::testDebugStreamOut()
{
    QFETCH(QContactFilter, filterIn);
    QFETCH(QString, messageExpected);

    QTest::ignoreMessage(QtDebugMsg, messageExpected.toUtf8());
    qDebug() << filterIn;
}

void tst_QContactFilter::testDebugStreamOut_data()
{
    QTest::addColumn<QContactFilter>("filterIn");
    QTest::addColumn<QString>("messageExpected");

    {
        QContactCollectionFilter filter;
        QContactCollectionId id1 = makeCollectionId(5);
        QContactCollectionId id2 = makeCollectionId(6);
        QContactCollectionId id3 = makeCollectionId(7);
        QContactCollectionId id4 = makeCollectionId(12);
        QSet<QContactCollectionId> ids;
        ids << id1 << id2 << id3;
        filter.setCollectionIds(ids);
        // Testing method setCollectionIds
        QTest::newRow("collection") << (QContactFilter)filter << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::05000000), QContactCollectionId(qtcontacts:basic::06000000), QContactCollectionId(qtcontacts:basic::07000000))))";

        filter.setCollectionId(id2);
        // Testing method setCollectionId (and the related clearing of the collection)
        QTest::newRow("collection") << (QContactFilter)filter << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::06000000))))";
        filter.setCollectionId(id4);
        // Testing again method setCollectionId (and the related clearing of the collection)
        QTest::newRow("collection") << (QContactFilter)filter << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::0c000000))))";
        ids.clear();
        ids << id4;
        // Testing again method setCollectionIds
        QTest::newRow("collection") << (QContactFilter)filter << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::0c000000))))";

        QContactCollectionFilter filter2;
        filter2 = filter;
        // Testing again method setCollectionIds on the copied filter
        QTest::newRow("collection") << (QContactFilter)filter2 << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::0c000000))))";

        QContactFilter fil;
        fil = filter;
        // Testing that the assignment/conversion went fine
        QTest::newRow("collection") << (QContactFilter)fil << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::0c000000))))";

        QContactCollectionFilter filter3(fil);
        QTest::newRow("collection") << (QContactFilter)filter3 << "QContactFilter(QContactCollectionFilter(collectionIds=(QContactCollectionId(qtcontacts:basic::0c000000))))";
    }
}

void tst_QContactFilter::traits()
{
    QCOMPARE(sizeof(QContactFilter), sizeof(void *));
    QVERIFY(QTypeInfo<QContactFilter>::isComplex);
    QVERIFY(!QTypeInfo<QContactFilter>::isStatic);
    QVERIFY(!QTypeInfo<QContactFilter>::isLarge);
    QVERIFY(!QTypeInfo<QContactFilter>::isPointer);
    QVERIFY(!QTypeInfo<QContactFilter>::isDummy);

    QCOMPARE(sizeof(QContactDetailFilter), sizeof(void*));
    QCOMPARE(sizeof(QContactChangeLogFilter), sizeof(void*));
}

QTEST_MAIN(tst_QContactFilter)
#include "tst_qcontactfilter.moc"
