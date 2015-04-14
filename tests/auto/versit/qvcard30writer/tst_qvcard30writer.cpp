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

#include "tst_qvcard30writer.h"
#ifdef QT_BUILD_INTERNAL
#include <QtVersit/private/qvcard30writer_p.h>
#endif
#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>
#include <QtTest/QtTest>
#include <QByteArray>
#include <QVariant>

// This says "NOKIA" in Katakana encoded with UTF-8
const QString KATAKANA_NOKIA(QString::fromUtf8("\xe3\x83\x8e\xe3\x82\xad\xe3\x82\xa2"));

QTVERSIT_USE_NAMESPACE

Q_DECLARE_METATYPE(QVersitProperty)
#ifdef QT_BUILD_INTERNAL
void tst_QVCard30Writer::init()
{
    mWriter = new QVCard30Writer(QVersitDocument::VCard30Type);
    mWriter->setCodec(QTextCodec::codecForName("UTF-8"));
}

void tst_QVCard30Writer::cleanup()
{
    delete mWriter;
}

void tst_QVCard30Writer::testEncodeVersitProperty()
{
    QFETCH(QVersitProperty, property);
    QFETCH(QByteArray, expectedResult);
    QByteArray encodedProperty;
    QBuffer buffer(&encodedProperty);
    mWriter->setDevice(&buffer);
    buffer.open(QIODevice::WriteOnly);
    mWriter->encodeVersitProperty(property);
    QCOMPARE(encodedProperty, expectedResult);
}


void tst_QVCard30Writer::testEncodeVersitProperty_data()
{
    QTest::addColumn<QVersitProperty>("property");
    QTest::addColumn<QByteArray>("expectedResult");

    QVersitProperty property;
    QByteArray expectedResult;

    // No parameters
    expectedResult = "FN:John Citizen\r\n";
    property.setName(QString::fromLatin1("FN"));
    property.setValue(QString::fromLatin1("John Citizen"));
    QTest::newRow("No parameters") << property << expectedResult;

    // With parameter(s)
    expectedResult = "TEL;TYPE=HOME:123\r\n";
    property.setName(QString::fromLatin1("TEL"));
    property.setValue(QString::fromLatin1("123"));
    property.insertParameter(QString::fromLatin1("TYPE"),QString::fromLatin1("HOME"));
    QTest::newRow("With parameters, plain value") << property << expectedResult;

    // normal FN property is backslash escaped
    property.clear();
    property.setName(QStringLiteral("FN"));
    property.setValue(QStringLiteral(";,:\\"));
    // semicolons, commas and backslashes are escaped (not colons, as per RFC2426)
    expectedResult = "FN:\\;\\,:\\\\\r\n";
    QTest::newRow("FN property") << property << expectedResult;

    // Structured N
    property.setName(QStringLiteral("N"));
    property.setValue(QStringList()
                      << QStringLiteral("La;st")    // needs to be backslash escaped
                      << QStringLiteral("Fi,rst")
                      << QStringLiteral("Mi:ddle")
                      << QStringLiteral("Pr\\efix") // needs to be QP encoded
                      << QStringLiteral("Suffix"));
    property.setValueType(QVersitProperty::CompoundType);
    expectedResult = "N:La\\;st;Fi\\,rst;Mi:ddle;Pr\\\\efix;Suffix\r\n";
    QTest::newRow("N property") << property << expectedResult;

    // Structured CATEGORIES
    property.setName(QStringLiteral("CATEGORIES"));
    property.setValue(QStringList()
                      << QStringLiteral("re;d")
                      << QStringLiteral("gr,een")
                      << QStringLiteral("bl:ue")
                      << QStringLiteral("ye\\llow"));
    property.setValueType(QVersitProperty::ListType);
    expectedResult = "CATEGORIES:re\\;d,gr\\,een,bl:ue,ye\\\\llow\r\n";
    QTest::newRow("CATEGORIES property") << property << expectedResult;

    // Convert X-NICKNAME to NICKNAME
    expectedResult = "NICKNAME:Jack\r\n";
    property.setParameters(QMultiHash<QString,QString>());
    property.setName(QString::fromLatin1("X-NICKNAME"));
    property.setValue(QString::fromLatin1("Jack"));
    QTest::newRow("NICKNAME property") << property << expectedResult;

    // Convert X-IMPP to IMPP;
    expectedResult = "IMPP:msn:msn-address\r\n";
    property.setParameters(QMultiHash<QString,QString>());
    property.setName(QString::fromLatin1("X-IMPP"));
    property.setValue(QString::fromLatin1("msn:msn-address"));
    QTest::newRow("IMPP property") << property << expectedResult;

    // AGENT property
    expectedResult = "AGENT:BEGIN:VCARD\\nVERSION:3.0\\nFN:Secret Agent\\nEND:VCARD\\n\r\n";
    property.setName(QString::fromLatin1("AGENT"));
    property.setValue(QString());
    QVersitDocument document(QVersitDocument::VCard30Type);
    document.setComponentType(QStringLiteral("VCARD"));
    QVersitProperty embeddedProperty;
    embeddedProperty.setName(QString(QString::fromLatin1("FN")));
    embeddedProperty.setValue(QString::fromLatin1("Secret Agent"));
    document.addProperty(embeddedProperty);
    property.setValue(QVariant::fromValue(document));
    QTest::newRow("AGENT property") << property << expectedResult;

    // Value is base64 encoded.
    QByteArray value("value");
    expectedResult = "Springfield.HOUSE.PHOTO;ENCODING=b:" + value.toBase64() + "\r\n";
    QStringList groups(QString::fromLatin1("Springfield"));
    groups.append(QString::fromLatin1("HOUSE"));
    property.setGroups(groups);
    property.setParameters(QMultiHash<QString,QString>());
    property.setName(QString::fromLatin1("PHOTO"));
    property.setValue(value);
    QTest::newRow("base64 encoded") << property << expectedResult;

    // Characters other than ASCII:
    expectedResult = "ORG:" + KATAKANA_NOKIA.toUtf8() + "\r\n";
    property = QVersitProperty();
    property.setName(QStringLiteral("ORG"));
    property.setValue(KATAKANA_NOKIA);
    QTest::newRow("non-ASCII") << property << expectedResult;

    // No CHARSET and QUOTED-PRINTABLE parameters
    expectedResult = "EMAIL:john@" + KATAKANA_NOKIA.toUtf8() + ".com\r\n";
    property = QVersitProperty();
    property.setName(QStringLiteral("EMAIL"));
    property.setValue(QString::fromLatin1("john@%1.com").arg(KATAKANA_NOKIA));
    QTest::newRow("special chars") << property << expectedResult;
}

void tst_QVCard30Writer::testEncodeParameters()
{
    QByteArray encodedParameters;
    QBuffer buffer(&encodedParameters);
    mWriter->setDevice(&buffer);
    buffer.open(QIODevice::WriteOnly);

    QString typeParameterName(QString::fromLatin1("TYPE"));
    QString encodingParameterName(QString::fromLatin1("ENCODING"));

    // No parameters
    QMultiHash<QString,QString> parameters;
    mWriter->encodeParameters(parameters);
    QCOMPARE(encodedParameters, QByteArray(""));

    // One TYPE parameter
    parameters.insert(typeParameterName,QString::fromLatin1("HOME"));
    mWriter->writeCrlf(); // so it doesn't start folding
    buffer.close();
    encodedParameters.clear();
    buffer.open(QIODevice::WriteOnly);
    mWriter->encodeParameters(parameters);
    QCOMPARE(encodedParameters, QByteArray(";TYPE=HOME"));

    // Two TYPE parameters
    parameters.insert(typeParameterName,QString::fromLatin1("VOICE"));
    mWriter->writeCrlf(); // so it doesn't start folding
    buffer.close();
    encodedParameters.clear();
    buffer.open(QIODevice::WriteOnly);
    mWriter->encodeParameters(parameters);
    QCOMPARE(encodedParameters, QByteArray(";TYPE=VOICE,HOME"));

    // One ENCODING parameter
    parameters.clear();
    parameters.insert(encodingParameterName,QString::fromLatin1("8BIT"));
    mWriter->writeCrlf(); // so it doesn't start folding
    buffer.close();
    encodedParameters.clear();
    buffer.open(QIODevice::WriteOnly);
    mWriter->encodeParameters(parameters);
    QCOMPARE(encodedParameters, QByteArray(";ENCODING=8BIT"));

    // Two parameters
    parameters.insert(QString::fromLatin1("X-PARAM"),QString::fromLatin1("VALUE"));
    mWriter->writeCrlf(); // so it doesn't start folding
    buffer.close();
    encodedParameters.clear();
    buffer.open(QIODevice::WriteOnly);
    mWriter->encodeParameters(parameters);
    QCOMPARE(encodedParameters, QByteArray(";ENCODING=8BIT;X-PARAM=VALUE"));

    // Parameter with characters that require backslash escaping
    parameters.clear();
    parameters.insert(QString::fromLatin1("X-P;ARAM"),QString::fromLatin1("VA,LUE"));
    mWriter->writeCrlf(); // so it doesn't start folding
    buffer.close();
    encodedParameters.clear();
    buffer.open(QIODevice::WriteOnly);
    mWriter->encodeParameters(parameters);
    QCOMPARE(encodedParameters, QByteArray(";X-P\\;ARAM=VA\\,LUE"));
}

void tst_QVCard30Writer::testBackSlashEscape()
{
    // Empty string
    QString input;
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString());

    // Nothing to escape in the string
    input = QString::fromLatin1("Nothing to escape");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("Nothing to escape"));

    // Line break in the beginning
    input = QString::fromLatin1("\r\n input");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("\\n input"));

    // Line break in the end
    input = QString::fromLatin1("input\r\n");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("input\\n"));

    // Semicolon in the beginning
    input = QString::fromLatin1(";input");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("\\;input"));

    // Semicolon in the end
    input = QString::fromLatin1("input;");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("input\\;"));

    // Comma in the beginning
    input = QString::fromLatin1(",input");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("\\,input"));

    // Comma in the end
    input = QString::fromLatin1("input,");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("input\\,"));

    // Backslash in the beginning
    input = QString::fromLatin1("\\input");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("\\\\input"));

    // Backslash in the end
    input = QString::fromLatin1("input\\");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input,QString::fromLatin1("input\\\\"));

    // Line break, semicolon, backslash and comma in the middle of the string
    input = QString::fromLatin1("Escape these \r\n ; , \\ ");
    QVCard30Writer::backSlashEscape(&input);
    QCOMPARE(input, QString::fromLatin1("Escape these \\n \\; \\, \\\\ "));
}
#endif
QTEST_MAIN(tst_QVCard30Writer)

