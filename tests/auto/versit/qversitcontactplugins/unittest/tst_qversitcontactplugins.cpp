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

#include <QtVersit/qversitcontacthandler.h>
#include <QtVersit/qversitproperty.h>
#include <QtContacts/qcontactname.h>

#include <QObject>
#include <QtTest/QtTest>

QTCONTACTS_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

class TestFactory1 : public QVersitContactHandlerFactory
{
    public:
        QString name() const { return "factory1"; }
        QVersitContactHandler* createHandler() const { return NULL; }
};

class tst_QVersitContactPlugins : public QObject
{
    Q_OBJECT

public slots:
    void init();

private slots:
    void testDefaultFactory();
    void testImporterPlugins();
    void testExporterPlugins();
};

void tst_QVersitContactPlugins::init() {
    // on windows, depending on how the unit test is invoked, we need these
    QCoreApplication::addLibraryPath("./debug/plugins/");
    QCoreApplication::addLibraryPath("./release/plugins/");
    QCoreApplication::addLibraryPath("./plugins/");
}

void tst_QVersitContactPlugins::testDefaultFactory() {
    TestFactory1 factory;
    QCOMPARE(factory.profiles(), QSet<QString>());
    QCOMPARE(factory.index(), 0);
}

void tst_QVersitContactPlugins::testImporterPlugins() {
    QVersitContactImporter importer("Test");
    QVersitDocument document;
    document.setComponentType("VCARD");
    QVersitProperty property;
    property.setName("FN");
    property.setValue("Bob");
    document.addProperty(property);
    QVERIFY(importer.importDocuments(QList<QVersitDocument>() << document));
    QCOMPARE(importer.contacts().size(), 1);
    QList<QContactDetail> details(importer.contacts().first().details(QContactDetail::TypeExtendedDetail));
    QCOMPARE(details.size(), 5);
    int pluginField = 0;
    // The plugins have had their index set such that they should be executed in reverse order
    // Check that they are all loaded, and run in the correct order
    QCOMPARE(details.at(0).value<int>(pluginField), 5);
    QCOMPARE(details.at(1).value<int>(pluginField), 4);
    QCOMPARE(details.at(2).value<int>(pluginField), 3);
    QCOMPARE(details.at(3).value<int>(pluginField), 2);
    QCOMPARE(details.at(4).value<int>(pluginField), 1);
}

void tst_QVersitContactPlugins::testExporterPlugins() {
    QVersitContactExporter exporter("Test");
    QContact contact;
    QContactName name;
    name.setFirstName("first name");
    contact.saveDetail(&name);
    QVERIFY(exporter.exportContacts(QList<QContact>() << contact));
    QCOMPARE(exporter.documents().size(), 1);
    QList<QVersitProperty> properties(exporter.documents().first().properties());

    // The plugins have had their index set such that they should be executed in reverse order
    // Check that they are all loaded, and run in the correct order
    int n = 0;
    foreach (QVersitProperty property, properties) {
        if (property.name() == "TEST-PROPERTY") {
            switch (n) {
                case 0: QCOMPARE(property.value(), QStringLiteral("5")); break;
                case 1: QCOMPARE(property.value(), QStringLiteral("4")); break;
                case 2: QCOMPARE(property.value(), QStringLiteral("3")); break;
                case 3: QCOMPARE(property.value(), QStringLiteral("2")); break;
                case 4: QCOMPARE(property.value(), QStringLiteral("1")); break;
            }
            n++;
        }
    }
    QCOMPARE(n, 5);
}

QTEST_MAIN(tst_QVersitContactPlugins)

#include "tst_qversitcontactplugins.moc"
