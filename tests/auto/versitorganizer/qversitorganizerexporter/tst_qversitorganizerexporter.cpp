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

#include "tst_qversitorganizerexporter.h"
#include <QtTest/QtTest>

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

Q_DECLARE_METATYPE(QList<QOrganizerItem>)
Q_DECLARE_METATYPE(QList<QOrganizerItemDetail>)
Q_DECLARE_METATYPE(QList<QVersitProperty>)

void tst_QVersitOrganizerExporter::testExport()
{
    QFETCH(QList<QOrganizerItem>, items);
    QFETCH(QVersitDocument, expectedDocument);

    QVersitOrganizerExporter exporter;
    QVERIFY(exporter.exportItems(items));
    QVERIFY(exporter.errorMap().isEmpty());
    QVersitDocument document = exporter.document();
    if (document != expectedDocument) {
        qDebug() << "Actual:" << document;
        qDebug() << "Expected:" << expectedDocument;
        QCOMPARE(document, expectedDocument);
    }
}

void tst_QVersitOrganizerExporter::testExport_data()
{
    QTest::addColumn<QList<QOrganizerItem> >("items");
    QTest::addColumn<QVersitDocument>("expectedDocument");

    {
        QVersitDocument document(QVersitDocument::ICalendar20Type);
        document.setComponentType(QStringLiteral("VCALENDAR"));
        QVersitDocument nested(QVersitDocument::ICalendar20Type);
        nested.setComponentType(QStringLiteral("VEVENT"));
        QVersitProperty property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("Bastille Day Party"));
        nested.addProperty(property);
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("19970714T170000Z"));
        nested.addProperty(property);
        property.setName(QStringLiteral("DTEND"));
        property.setValue(QStringLiteral("19970715T035959Z"));
        nested.addProperty(property);
        document.addSubDocument(nested);

        nested.clear();
        nested.setType(QVersitDocument::ICalendar20Type);
        nested.setComponentType(QStringLiteral("VTODO"));
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("Take out the garbage"));
        nested.addProperty(property);
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("20100609T080000"));
        nested.addProperty(property);
        property.setName(QStringLiteral("DUE"));
        property.setValue(QStringLiteral("20100610T080000"));
        nested.addProperty(property);
        document.addSubDocument(nested);

        nested.clear();
        nested.setType(QVersitDocument::ICalendar20Type);
        nested.setComponentType(QStringLiteral("VJOURNAL"));
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("Trip to Thailand"));
        nested.addProperty(property);
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("20100615T112300"));
        nested.addProperty(property);
        document.addSubDocument(nested);

        QOrganizerEvent event;
        event.setDisplayLabel(QStringLiteral("Bastille Day Party"));
        event.setStartDateTime(QDateTime(QDate(1997, 7, 14), QTime(17, 0, 0), Qt::UTC));
        event.setEndDateTime(QDateTime(QDate(1997, 7, 15), QTime(3, 59, 59), Qt::UTC));

        QOrganizerTodo todo;
        todo.setDisplayLabel(QStringLiteral("Take out the garbage"));
        todo.setStartDateTime(QDateTime(QDate(2010, 6, 9), QTime(8, 0, 0)));
        todo.setDueDateTime(QDateTime(QDate(2010, 6, 10), QTime(8, 0, 0)));

        QOrganizerJournal journal;
        journal.setDisplayLabel(QStringLiteral("Trip to Thailand"));
        journal.setDateTime(QDateTime(QDate(2010, 6, 15), QTime(11, 23, 0)));

        QList<QOrganizerItem> items;
        items << static_cast<QOrganizerItem>(event);
        items << static_cast<QOrganizerItem>(todo);
        items << static_cast<QOrganizerItem>(journal);

        QTest::newRow("sample event, todo and journal") << items << document;
    }

    {
        QVersitDocument document(QVersitDocument::ICalendar20Type);
        document.setComponentType(QStringLiteral("VCALENDAR"));
        QVersitDocument nested(QVersitDocument::ICalendar20Type);
        nested.setComponentType(QStringLiteral("VEVENT"));
        QVersitProperty property;
        property.setName(QStringLiteral("UID"));
        property.setValue(QStringLiteral("1234"));
        nested.addProperty(property);
        property.setName(QStringLiteral("RECURRENCE-ID"));
        property.setValue(QStringLiteral("20100608"));
        nested.addProperty(property);
        document.addSubDocument(nested);

        // An event occurrence with OriginalDate and Guid set
        QOrganizerEventOccurrence event;
        event.setGuid(QStringLiteral("1234"));
        event.setOriginalDate(QDate(2010, 6, 8));

        QList<QOrganizerItem> items;
        items << static_cast<QOrganizerItem>(event);

        QTest::newRow("event occurrence exception") << items << document;
    }
}

/*! Test that bad input causes the exporter to fail with the correct error */
void tst_QVersitOrganizerExporter::testExportError()
{
    QFETCH(QList<QOrganizerItem>, items);
    QFETCH(QVersitDocument, expectedDocument);
    QFETCH(int, expectedError);

    QVersitOrganizerExporter exporter;
    QVERIFY(!exporter.exportItems(items));
    QVERIFY(!exporter.errorMap().isEmpty());
    QVERIFY(exporter.errorMap()[0] == expectedError);
    QVersitDocument document = exporter.document();
    if (document != expectedDocument) {
        qDebug() << "Actual:" << document;
        qDebug() << "Expected:" << expectedDocument;
        QCOMPARE(document, expectedDocument);
    }
}

void tst_QVersitOrganizerExporter::testExportError_data()
{
    QTest::addColumn<QList<QOrganizerItem> >("items");
    QTest::addColumn<QVersitDocument>("expectedDocument");
    QTest::addColumn<int>("expectedError");

    QVersitDocument document(QVersitDocument::ICalendar20Type);
    document.setComponentType(QStringLiteral("VCALENDAR"));

    // An event occurrence with OriginalDate set but no Guid
    QOrganizerEventOccurrence event;
    event.setOriginalDate(QDate(2010, 6, 8));

    QList<QOrganizerItem> items;
    items << static_cast<QOrganizerItem>(event);

    QTest::newRow("event occurrence exception with no guid") << items << document
        << (int)QVersitOrganizerExporter::UnderspecifiedOccurrenceError;
}

void tst_QVersitOrganizerExporter::testExportEventDetails()
{
    QFETCH(QList<QOrganizerItemDetail>, details);
    QFETCH(QList<QVersitProperty>, expectedProperties);

    QVersitOrganizerExporter exporter;
    QOrganizerEvent item;
    foreach (QOrganizerItemDetail detail, details) {
        item.saveDetail(&detail);
    }
    QVERIFY(exporter.exportItems(QList<QOrganizerItem>() << item));
    QVERIFY(exporter.errorMap().isEmpty());
    QVersitDocument document = exporter.document();
    QList<QVersitDocument> subDocuments = document.subDocuments();
    QCOMPARE(subDocuments.size(), 1);
    int currentVAlarmSubDocumentIndex = 0;
    foreach(const QVersitProperty& expectedProperty, expectedProperties) {
        QList<QVersitProperty> actualProperties =
            findPropertiesByName(subDocuments.first(), expectedProperty.name());
        if (expectedProperty.valueType() == QVersitProperty::VersitDocumentType) {
            QCOMPARE(subDocuments.first().subDocuments().size(), expectedProperties.size());
            QVersitDocument actualVAlarmDocument =
                    subDocuments.first().subDocuments().at(currentVAlarmSubDocumentIndex);
            QVersitDocument expectedVAlarmDocument =
                    expectedProperty.variantValue().value<QVersitDocument>();
            foreach (const QVersitProperty &valarmProp, expectedVAlarmDocument.properties()) {
                if (!actualVAlarmDocument.properties().contains(valarmProp)) {
                            qDebug() << "Actual:" << actualVAlarmDocument.properties();
                            qDebug() << "Expected to find:" << valarmProp;
                            QVERIFY(false);
                }
            }
            currentVAlarmSubDocumentIndex ++; //move to next expected valarm document
        } else if (!actualProperties.contains(expectedProperty)) {
            qDebug() << "Actual:" << actualProperties;
            qDebug() << "Expected to find:" << expectedProperty;
            QVERIFY(false);
        }
    }
}

void tst_QVersitOrganizerExporter::testExportEventDetails_data()
{
    QTest::addColumn<QList<QOrganizerItemDetail> >("details");
    QTest::addColumn<QList<QVersitProperty> >("expectedProperties");

    {
        QVersitProperty property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("jabberwocky"));
        QOrganizerItemDisplayLabel displayLabel;
        displayLabel.setLabel(QStringLiteral("jabberwocky"));
        QTest::newRow("one summary") << (QList<QOrganizerItemDetail>() << displayLabel)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;
        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("AUDIO"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 5);
        audibleReminder.setSecondsBeforeStart(90);
        QTest::newRow("audible reminder") << (QList<QOrganizerItemDetail>() << audibleReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("DISPLAY"));
        valarmProperties << property;
        property.setName(QStringLiteral("x-QTPROJECT-ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Test visual reminder"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        visualReminder.setRepetition(3, 5);
        visualReminder.setSecondsBeforeStart(90);
        visualReminder.setMessage(QStringLiteral("Test visual reminder"));
        QTest::newRow("visual reminder") << (QList<QOrganizerItemDetail>() << visualReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("DISPLAY"));
        valarmProperties << property;
        property.setName(QStringLiteral("x-QTPROJECT-ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral(""));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        visualReminder.setRepetition(3, 5);
        visualReminder.setSecondsBeforeStart(90);
        QTest::newRow("visual reminder: no message") << (QList<QOrganizerItemDetail>() << visualReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("EMAIL"));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Test email body"));
        valarmProperties << property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("Test email subject"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTENDEE"));
        property.setValue(QStringLiteral("First email recipient"));
        valarmProperties << property;
        property.setValue(QStringLiteral("Second email recipient"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemEmailReminder emailReminder;
        QVariantList attachments;
        emailReminder.setRepetition(3, 5);
        emailReminder.setSecondsBeforeStart(90);
        emailReminder.setRecipients((QStringList() << QStringLiteral("First email recipient")
                                    << QStringLiteral("Second email recipient")));
        emailReminder.setContents(QStringLiteral("Test email subject"),
                                  QStringLiteral("Test email body"),
                                  attachments);
        QTest::newRow("email reminder") << (QList<QOrganizerItemDetail>() << emailReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("EMAIL"));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral(""));
        valarmProperties << property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral(""));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTENDEE"));
        property.setValue(QStringLiteral("First email recipient"));
        valarmProperties << property;
        property.setValue(QStringLiteral("Second email recipient"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemEmailReminder emailReminder;
        QVariantList attachments;
        emailReminder.setRepetition(3, 5);
        emailReminder.setSecondsBeforeStart(90);
        emailReminder.setRecipients((QStringList() << QStringLiteral("First email recipient")
                                    << QStringLiteral("Second email recipient")));
        emailReminder.setValue(QOrganizerItemEmailReminder::FieldAttachments, attachments);
        QTest::newRow("email reminder: no subject, no body") << (QList<QOrganizerItemDetail>() << emailReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QList<QOrganizerItemDetail> detailsToExport;
        QList<QVersitProperty> expectedProperties;
        {
            QVersitProperty property;
            QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
            QList<QVersitProperty> valarmProperties;
            QMultiHash<QString, QString> parameters;
            property.setName(QStringLiteral("ACTION"));
            property.setValue(QStringLiteral("AUDIO"));
            valarmProperties << property;
            property.setName(QStringLiteral("ATTACH"));
            property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
            valarmProperties << property;
            property.setName(QStringLiteral("DURATION"));
            property.setValue(QStringLiteral("PT5S"));
            valarmProperties << property;
            property.setName(QStringLiteral("REPEAT"));
            property.setValue(3);
            valarmProperties << property;
            property.setName(QStringLiteral("TRIGGER"));
            parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
            property.setParameters(parameters);
            property.setValue(QStringLiteral("-PT90S"));
            valarmProperties << property;
            property.clear();
            valarmDocument.setComponentType("VALARM");
            valarmDocument.setProperties(valarmProperties);
            property.setValueType(QVersitProperty::VersitDocumentType);
            property.setName(QStringLiteral("VALARM"));
            property.setValue(QVariant::fromValue(valarmDocument));
            QOrganizerItemAudibleReminder audibleReminder;
            audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
            audibleReminder.setRepetition(3, 5);
            audibleReminder.setSecondsBeforeStart(90);
            detailsToExport << audibleReminder;
            expectedProperties << property;
        }

        {
            QVersitProperty property;
            QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
            QList<QVersitProperty> valarmProperties;
            QMultiHash<QString, QString> parameters;

            property.setName(QStringLiteral("ACTION"));
            property.setValue(QStringLiteral("DISPLAY"));
            valarmProperties << property;
            property.setName(QStringLiteral("x-QTPROJECT-ATTACH"));
            property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
            valarmProperties << property;
            property.setName(QStringLiteral("DURATION"));
            property.setValue(QStringLiteral("PT5S"));
            valarmProperties << property;
            property.setName(QStringLiteral("REPEAT"));
            property.setValue(3);
            valarmProperties << property;
            property.setName(QStringLiteral("TRIGGER"));
            parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
            property.setParameters(parameters);
            property.setValue(QStringLiteral("-PT90S"));
            valarmProperties << property;
            property.clear();
            property.setName(QStringLiteral("DESCRIPTION"));
            property.setValue(QStringLiteral(""));
            valarmProperties << property;

            valarmDocument.setComponentType("VALARM");
            valarmDocument.setProperties(valarmProperties);
            property.setValueType(QVersitProperty::VersitDocumentType);
            property.setName(QStringLiteral("VALARM"));
            property.setValue(QVariant::fromValue(valarmDocument));
            QOrganizerItemVisualReminder visualReminder;
            visualReminder.setDataUrl(QUrl("http://qt.nokia.com"));
            visualReminder.setRepetition(3, 5);
            visualReminder.setSecondsBeforeStart(90);
            detailsToExport << visualReminder;
            expectedProperties << property;
        }
        {
            QVersitProperty property;
            QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
            QList<QVersitProperty> valarmProperties;
            QMultiHash<QString, QString> parameters;

            property.setName(QStringLiteral("ACTION"));
            property.setValue(QStringLiteral("EMAIL"));
            valarmProperties << property;
            property.setName(QStringLiteral("DURATION"));
            property.setValue(QStringLiteral("PT5S"));
            valarmProperties << property;
            property.setName(QStringLiteral("REPEAT"));
            property.setValue(3);
            valarmProperties << property;
            property.setName(QStringLiteral("TRIGGER"));
            parameters.insert(QStringLiteral("RELATED"), QStringLiteral("START"));
            property.setParameters(parameters);
            property.setValue(QStringLiteral("-PT90S"));
            valarmProperties << property;
            property.clear();
            property.setName(QStringLiteral("DESCRIPTION"));
            property.setValue(QStringLiteral(""));
            valarmProperties << property;
            property.setName(QStringLiteral("SUMMARY"));
            property.setValue(QStringLiteral(""));
            valarmProperties << property;
            property.setName(QStringLiteral("ATTENDEE"));
            property.setValue(QStringLiteral("First email recipient"));
            valarmProperties << property;
            property.setValue(QStringLiteral("Second email recipient"));
            valarmProperties << property;

            valarmDocument.setComponentType("VALARM");
            valarmDocument.setProperties(valarmProperties);
            property.setValueType(QVersitProperty::VersitDocumentType);
            property.setName(QStringLiteral("VALARM"));
            property.setValue(QVariant::fromValue(valarmDocument));
            QOrganizerItemEmailReminder emailReminder;
            QVariantList attachments;
            emailReminder.setRepetition(3, 5);
            emailReminder.setSecondsBeforeStart(90);
            emailReminder.setRecipients((QStringList() << QStringLiteral("First email recipient")
                                        << QStringLiteral("Second email recipient")));
            emailReminder.setValue(QOrganizerItemEmailReminder::FieldAttachments, attachments);
            detailsToExport << emailReminder;
            expectedProperties << property;
        }
        QTest::newRow("multiple reminders") << detailsToExport << expectedProperties;
    }
    {
        QVersitProperty property;
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Twinkle, twinkle, little bat! How I wonder what you're at."));
        QOrganizerItemDescription description;
        description.setDescription(QStringLiteral("Twinkle, twinkle, little bat! How I wonder what you're at."));
        QTest::newRow("one description") << (QList<QOrganizerItemDetail>() << description)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property = createExtendedDetailPropertyForStringData("name", "data");
        QOrganizerItemExtendedDetail extendedDetail = createExtendedDetail("name", QStringLiteral("data"));
        QTest::newRow("extended detail") << (QList<QOrganizerItemDetail>() << extendedDetail)
                                         << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property; // Proper version.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("4711") << QStringLiteral("134f23dbb2"));
        QOrganizerItemVersion version;
        version.setVersion(4711);
        version.setExtendedVersion(QByteArray("134f23dbb2"));
        QTest::newRow("version") << (QList<QOrganizerItemDetail>() << version)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property; // Extended version empty.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("4711") << QStringLiteral(""));
        QOrganizerItemVersion version;
        version.setVersion(4711);
        version.setExtendedVersion(QByteArray(""));
        QTest::newRow("version") << (QList<QOrganizerItemDetail>() << version)
                                 << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property; // No extended version.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("4711") << QStringLiteral(""));
        QOrganizerItemVersion version;
        version.setVersion(4711);
        QTest::newRow("version") << (QList<QOrganizerItemDetail>() << version)
                                 << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property; // Empty version detail.
        QOrganizerItemVersion version;
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("0") << QStringLiteral(""));
        QTest::newRow("version") << (QList<QOrganizerItemDetail>() << version)
                                 << (QList<QVersitProperty>());
    }

    {
        QVersitProperty property1;
        property1.setName(QStringLiteral("COMMENT"));
        property1.setValue(QStringLiteral("Comment 1"));
        QVersitProperty property2;
        property2.setName(QStringLiteral("COMMENT"));
        property2.setValue(QStringLiteral("Comment 2"));
        QOrganizerItemComment comment1;
        comment1.setComment(QStringLiteral("Comment 1"));
        QOrganizerItemComment comment2;
        comment2.setComment(QStringLiteral("Comment 2"));
        QTest::newRow("two comments")
            << (QList<QOrganizerItemDetail>() << comment1 << comment2)
            << (QList<QVersitProperty>() << property1 << property2);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("UID"));
        property.setValue(QStringLiteral("1234567"));
        QOrganizerItemGuid guid;
        guid.setGuid(QStringLiteral("1234567"));
        QTest::newRow("guid") << (QList<QOrganizerItemDetail>() << guid)
            << (QList<QVersitProperty>() << property);
    }

    {
        QList<QVersitProperty> properties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        properties << dtstart;
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        dtend.setValue(QStringLiteral("20100102T030406"));
        properties << dtend;
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 6)));
        QTest::newRow("dtstart and dtend") << (QList<QOrganizerItemDetail>() << etr)
            << properties;
    }

    {
        QList<QVersitProperty> properties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102"));
        dtstart.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        properties << dtstart;
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        // Note: in iCalendar, the end date is exclusive while in Qt Organizer, it is inclusive.
        // Hence, this is an event that occurs all day on 2 January (not including 3 January)
        dtend.setValue(QStringLiteral("20100103"));
        dtend.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        properties << dtend;
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 6)));
        etr.setAllDay(true);
        QTest::newRow("all day event") << (QList<QOrganizerItemDetail>() << etr)
            << properties;
    }

    {
        QList<QVersitProperty> properties;
        QVersitProperty created;
        created.setName(QStringLiteral("CREATED"));
        created.setValue(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)).toUTC().toString(
                    QStringLiteral("yyyyMMddTHHmmssZ")));
        properties << created;
        QVersitProperty modified;
        modified.setName(QStringLiteral("LAST-MODIFIED"));
        modified.setValue(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 6)).toUTC().toString(
                    QStringLiteral("yyyyMMddTHHmmssZ")));
        properties << modified;
        QOrganizerItemTimestamp timestamp;
        timestamp.setCreated(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        timestamp.setLastModified(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 6)));
        QTest::newRow("created and last modified")
            << (QList<QOrganizerItemDetail>() << timestamp)
            << properties;
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=DAILY"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Daily);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule daily")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);

        rrule.setValue(QStringLiteral("FREQ=WEEKLY"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule weekly")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);

        rrule.setValue(QStringLiteral("FREQ=MONTHLY"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule monthly")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);

        rrule.setValue(QStringLiteral("FREQ=YEARLY"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule yearly")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);

        rrule.setValue(QStringLiteral("FREQ=MONTHLY;INTERVAL=2;BYDAY=TU"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrenceRule.setInterval(2);
        recurrenceRule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Tuesday);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule monthly")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=MONTHLY;BYMONTHDAY=-3,1"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrenceRule.setDaysOfMonth(QSet<int>() << 1 << -3);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule bymonthday")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=YEARLY;BYWEEKNO=-3,1"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setWeeksOfYear(QSet<int>() << 1 << -3);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule byweekno")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=YEARLY;BYMONTH=1,10"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                << QOrganizerRecurrenceRule::January
                << QOrganizerRecurrenceRule::October);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule bymonth")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=YEARLY;BYYEARDAY=-1,1,366"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setDaysOfYear(QSet<int>() << 1 << 366 << -1);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule byyearday")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=WEEKLY;COUNT=4;INTERVAL=2;BYDAY=TU,SU;WKST=SU"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        recurrenceRule.setInterval(2);
        recurrenceRule.setLimit(4);
        recurrenceRule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Tuesday << Qt::Sunday);
        recurrenceRule.setFirstDayOfWeek(Qt::Sunday);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule wkst")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=MONTHLY;BYDAY=MO,TU,WE,TH,FR;BYSETPOS=-1"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrenceRule.setDaysOfWeek(QSet<Qt::DayOfWeek>()
                << Qt::Monday << Qt::Tuesday << Qt::Wednesday << Qt::Thursday << Qt::Friday);
        recurrenceRule.setPositions(QSet<int>() << -1);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule bysetpos")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=DAILY;UNTIL=20000131;BYMONTH=1"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Daily);
        recurrenceRule.setLimit(QDate(2000, 1, 31));
        recurrenceRule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                << QOrganizerRecurrenceRule::January);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule until")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rrule;
        rrule.setValueType(QVersitProperty::PreformattedType);
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=DAILY;COUNT=5;BYMONTH=1"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Daily);
        recurrenceRule.setLimit(5);
        recurrenceRule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                << QOrganizerRecurrenceRule::January);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule count")
            << (QList<QOrganizerItemDetail>() << recurrence)
            << (QList<QVersitProperty>() << rrule);
    }

    {
        QVersitProperty rdate;
        rdate.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        rdate.setName(QStringLiteral("RDATE"));
        rdate.setValue(QStringLiteral("19970304"));
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(1997, 3, 4), QTime(11, 0, 0)));
        etr.setEndDateTime(QDateTime(QDate(1997, 3, 4), QTime(11, 0, 0)));
        QOrganizerItemRecurrence recurrence;
        QSet<QDate> recurrenceDates;
        recurrenceDates << QDate(1997, 3, 4);
        recurrence.setRecurrenceDates(recurrenceDates);
        QTest::newRow("rdate")
            << (QList<QOrganizerItemDetail>() << etr << recurrence)
            << (QList<QVersitProperty>() << rdate);

        rdate.setValue(QStringLiteral("19970304,19970504,19970704"));
        recurrenceDates.clear();
        recurrenceDates << QDate(1997, 3, 4)
                        << QDate(1997, 5, 4)
                        << QDate(1997, 7, 4);
        recurrence.setRecurrenceDates(recurrenceDates);
        QTest::newRow("multiple rdate")
            << (QList<QOrganizerItemDetail>() << etr << recurrence)
            << (QList<QVersitProperty>() << rdate);
    }

    {
        QVersitProperty rdate;
        rdate.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        rdate.setName(QStringLiteral("EXDATE"));
        rdate.setValue(QStringLiteral("19970304"));
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(1997, 3, 4), QTime(11, 0, 0)));
        etr.setEndDateTime(QDateTime(QDate(1997, 3, 4), QTime(11, 0, 0)));
        QOrganizerItemRecurrence recurrence;
        QSet<QDate> exceptionDates;
        exceptionDates << QDate(1997, 3, 4);
        recurrence.setExceptionDates(exceptionDates);
        QTest::newRow("exdate")
            << (QList<QOrganizerItemDetail>() << etr << recurrence)
            << (QList<QVersitProperty>() << rdate);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("CATEGORIES"));
        property.setValue(QStringLiteral("Important Event"));
        QOrganizerItemTag tag;
        tag.setTag(QStringLiteral("Important Event"));
        QTest::newRow("tag 1")
                << (QList<QOrganizerItemDetail>() << tag)
                << (QList<QVersitProperty>() << property);

        // Set another one for multiple handling test
        property.setValue(QStringLiteral("Important Event 2"));
        tag.setTag(QStringLiteral("Important Event 2"));
        QTest::newRow("tag 2")
                << (QList<QOrganizerItemDetail>() << tag)
                << (QList<QVersitProperty>() << property);

        // Empty tag
        property.clear();
        property.setName("CATEGORIES");
        tag.removeValue(QOrganizerItemTag::FieldTag);
        QTest::newRow("empty tag")
                << (QList<QOrganizerItemDetail>() << tag)
                << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("PRIORITY"));
        property.setValue(QStringLiteral("0"));

        QOrganizerItemPriority detail;
        detail.setPriority(QOrganizerItemPriority::UnknownPriority);
        QTest::newRow("priority 0")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("1"));
        detail.setPriority(QOrganizerItemPriority::HighestPriority);
        QTest::newRow("priority 1")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("2"));
        detail.setPriority(QOrganizerItemPriority::ExtremelyHighPriority);
        QTest::newRow("priority 2")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("3"));
        detail.setPriority(QOrganizerItemPriority::VeryHighPriority);
        QTest::newRow("priority 3")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("4"));
        detail.setPriority(QOrganizerItemPriority::HighPriority);
        QTest::newRow("priority 4")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("5"));
        detail.setPriority(QOrganizerItemPriority::MediumPriority);
        QTest::newRow("priority 5")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("6"));
        detail.setPriority(QOrganizerItemPriority::LowPriority);
        QTest::newRow("priority 6")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("7"));
        detail.setPriority(QOrganizerItemPriority::VeryLowPriority);
        QTest::newRow("priority 7")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("8"));
        detail.setPriority(QOrganizerItemPriority::ExtremelyLowPriority);
        QTest::newRow("priority 8")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("9"));
        detail.setPriority(QOrganizerItemPriority::LowestPriority);
        QTest::newRow("priority 9")
            << (QList<QOrganizerItemDetail>() << detail)
            << (QList<QVersitProperty>() << property);
    }
}

void tst_QVersitOrganizerExporter::testExportTodoDetails()
{
    QFETCH(QList<QOrganizerItemDetail>, details);
    QFETCH(QList<QVersitProperty>, expectedProperties);

    QVersitOrganizerExporter exporter;
    QOrganizerTodo item;
    foreach (QOrganizerItemDetail detail, details) {
        item.saveDetail(&detail);
    }
    QVERIFY(exporter.exportItems(QList<QOrganizerItem>() << item));
    QVERIFY(exporter.errorMap().isEmpty());
    QVersitDocument document = exporter.document();
    QList<QVersitDocument> subDocuments = document.subDocuments();
    QCOMPARE(subDocuments.size(), 1);
    foreach(const QVersitProperty& expectedProperty, expectedProperties) {
        QList<QVersitProperty> actualProperties =
            findPropertiesByName(subDocuments.first(), expectedProperty.name());
        if (expectedProperty.valueType() == QVersitProperty::VersitDocumentType) {
            QCOMPARE(subDocuments.first().subDocuments().size(), 1);
            QVersitDocument actualVAlarmDocument =
                    subDocuments.first().subDocuments().first();
            QVersitDocument expectedVAlarmDocument =
                    expectedProperty.variantValue().value<QVersitDocument>();
            foreach (const QVersitProperty &valarmProp, expectedVAlarmDocument.properties()) {
                if (!actualVAlarmDocument.properties().contains(valarmProp)) {
                            qDebug() << "Actual:" << actualVAlarmDocument.properties();
                            qDebug() << "Expected to find:" << valarmProp;
                            QVERIFY(false);
                }
            }
        } else if (!actualProperties.contains(expectedProperty)) {
            qDebug() << "Actual:" << actualProperties;
            qDebug() << "Expected to find:" << expectedProperty;
            QVERIFY(false);
        }
    }
}

void tst_QVersitOrganizerExporter::testExportTodoDetails_data()
{
    QTest::addColumn<QList<QOrganizerItemDetail> >("details");
    QTest::addColumn<QList<QVersitProperty> >("expectedProperties");

    {
        QList<QVersitProperty> properties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102"));
        dtstart.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        properties << dtstart;
        QVersitProperty due;
        due.setName(QStringLiteral("DUE"));
        due.setValue(QStringLiteral("20100103"));
        due.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        properties << due;
        QOrganizerTodoTime todoTime;
        todoTime.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        todoTime.setDueDateTime(QDateTime(QDate(2010, 1, 3), QTime(3, 4, 6)));
        todoTime.setAllDay(true);
        QTest::newRow("all day todo") << (QList<QOrganizerItemDetail>() << todoTime)
            << properties;
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;
        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("AUDIO"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("END"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 5);
        audibleReminder.setSecondsBeforeStart(90);
        QTest::newRow("audible reminder") << (QList<QOrganizerItemDetail>() << audibleReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("DISPLAY"));
        valarmProperties << property;
        property.setName(QStringLiteral("x-QTPROJECT-ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("END"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Test visual reminder"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        visualReminder.setRepetition(3, 5);
        visualReminder.setSecondsBeforeStart(90);
        visualReminder.setMessage(QStringLiteral("Test visual reminder"));
        QTest::newRow("visual reminder") << (QList<QOrganizerItemDetail>() << visualReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("DISPLAY"));
        valarmProperties << property;
        property.setName(QStringLiteral("x-QTPROJECT-ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("END"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral(""));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        visualReminder.setRepetition(3, 5);
        visualReminder.setSecondsBeforeStart(90);
        QTest::newRow("visual reminder: no message") << (QList<QOrganizerItemDetail>() << visualReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("EMAIL"));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("END"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Test email body"));
        valarmProperties << property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("Test email subject"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTENDEE"));
        property.setValue(QStringLiteral("First email recipient"));
        valarmProperties << property;
        property.setValue(QStringLiteral("Second email recipient"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemEmailReminder emailReminder;
        QVariantList attachments;
        emailReminder.setRepetition(3, 5);
        emailReminder.setSecondsBeforeStart(90);
        emailReminder.setRecipients((QStringList() << QStringLiteral("First email recipient")
                                    << QStringLiteral("Second email recipient")));
        emailReminder.setContents(QStringLiteral("Test email subject"),
                                  QStringLiteral("Test email body"),
                                  attachments);
        QTest::newRow("email reminder") << (QList<QOrganizerItemDetail>() << emailReminder)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QMultiHash<QString, QString> parameters;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("EMAIL"));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        parameters.insert(QStringLiteral("RELATED"), QStringLiteral("END"));
        property.setParameters(parameters);
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.clear();
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral(""));
        valarmProperties << property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral(""));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTENDEE"));
        property.setValue(QStringLiteral("First email recipient"));
        valarmProperties << property;
        property.setValue(QStringLiteral("Second email recipient"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemEmailReminder emailReminder;
        QVariantList attachments;
        emailReminder.setRepetition(3, 5);
        emailReminder.setSecondsBeforeStart(90);
        emailReminder.setRecipients((QStringList() << QStringLiteral("First email recipient")
                                    << QStringLiteral("Second email recipient")));
        emailReminder.setValue(QOrganizerItemEmailReminder::FieldAttachments, attachments);
        QTest::newRow("email reminder: no subject, no body") << (QList<QOrganizerItemDetail>() << emailReminder)
            << (QList<QVersitProperty>() << property);
    }


    {
        QVersitProperty property;
        property.setName(QStringLiteral("STATUS"));
        property.setValue(QStringLiteral("COMPLETED"));
        QOrganizerTodoProgress progress;
        progress.setStatus(QOrganizerTodoProgress::StatusComplete);
        QTest::newRow("status completed")
            << (QList<QOrganizerItemDetail>() << progress)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("NEEDS-ACTION"));
        progress.setStatus(QOrganizerTodoProgress::StatusNotStarted);
        QTest::newRow("status needs-action")
            << (QList<QOrganizerItemDetail>() << progress)
            << (QList<QVersitProperty>() << property);

        property.setValue(QStringLiteral("IN-PROCESS"));
        progress.setStatus(QOrganizerTodoProgress::StatusInProgress);
        QTest::newRow("status in-process")
            << (QList<QOrganizerItemDetail>() << progress)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("PERCENT-COMPLETE"));
        property.setValue(QStringLiteral("42"));
        QOrganizerTodoProgress progress;
        progress.setPercentageComplete(42);
        QTest::newRow("percent-complete")
            << (QList<QOrganizerItemDetail>() << progress)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("COMPLETED"));
        property.setValue(QStringLiteral("20100609T161500"));
        QOrganizerTodoProgress progress;
        progress.setFinishedDateTime(QDateTime(QDate(2010, 6, 9), QTime(16, 15, 0)));
        QTest::newRow("completed")
            << (QList<QOrganizerItemDetail>() << progress)
            << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("CATEGORIES"));
        property.setValue(QStringLiteral("Important Event"));
        QOrganizerItemTag tag;
        tag.setTag(QStringLiteral("Important Event"));
        QTest::newRow("tag 1")
                << (QList<QOrganizerItemDetail>() << tag)
                << (QList<QVersitProperty>() << property);

        // Set another one for multiple handling test
        property.setValue(QStringLiteral("Important Event 2"));
        tag.setTag(QStringLiteral("Important Event 2"));
        QTest::newRow("tag 2")
                << (QList<QOrganizerItemDetail>() << tag)
                << (QList<QVersitProperty>() << property);

        // Empty tag
        property.clear();
        property.setName("CATEGORIES");
        tag.removeValue(QOrganizerItemTag::FieldTag);
        QTest::newRow("empty tag")
                << (QList<QOrganizerItemDetail>() << tag)
                << (QList<QVersitProperty>() << property);
    }

    {
        QVersitProperty property = createExtendedDetailPropertyForStringData("name", "data");
        QOrganizerItemExtendedDetail extendedDetail = createExtendedDetail("name", QStringLiteral("data"));
        QTest::newRow("extended detail") << (QList<QOrganizerItemDetail>() << extendedDetail)
            << (QList<QVersitProperty>() << property);
    }
}

void tst_QVersitOrganizerExporter::testExtendedDetail()
{
    QFETCH(QString, extendedDetailName);
    QFETCH(QVariant, extendedDetailData);
    QFETCH(QString, extendedDetailDataInProperty);
    QFETCH(bool, extendedDetailPropertyCreated);

    QOrganizerEvent item;
    QOrganizerItemExtendedDetail extendedDetail = createExtendedDetail(extendedDetailName, extendedDetailData);
    item.saveDetail(&extendedDetail);

    QVersitProperty expectedProperty =
            createExtendedDetailProperty(extendedDetailName, extendedDetailDataInProperty);

    QVersitOrganizerExporter exporter;
    QVERIFY(exporter.exportItems(QList<QOrganizerItem>() << item));

    QVersitDocument document = exporter.document().subDocuments().first();
    QList<QVersitProperty> actualProperties =
            findPropertiesByName(document, expectedProperty.name());
    if (!extendedDetailPropertyCreated) {
        QCOMPARE(actualProperties.size(), 0);
    } else {
        QCOMPARE(actualProperties.size(), 1);
        if (static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Double
                || static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Float) {
            QCOMPARE(actualProperties.first().name(), expectedProperty.name());
            QStringList actualStringList = actualProperties.first().variantValue().toStringList();
            QCOMPARE(actualStringList.size(), 2);
            QString actualString = actualStringList.at(1);
            actualString.replace('[',"").replace(']',"").replace('\n', "");
            QStringList expectedStringList = expectedProperty.variantValue().toStringList();
            QCOMPARE(expectedStringList.size(), 2);
            QString expectedString = expectedStringList.at(1);
            expectedString.replace('[',"").replace(']',"").replace('\n', "");
            if (static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Float) {
                float actualF = actualString.toFloat();
                float expectedF = expectedString.toFloat();
                QCOMPARE(actualStringList.first(), expectedStringList.first()); // "name"
                QCOMPARE(actualF, expectedF); // "value"
            } else {
                double actualD = actualString.toDouble();
                double expectedD = expectedString.toDouble();
                QCOMPARE(actualStringList.first(), expectedStringList.first()); // "name"
                QCOMPARE(actualD, expectedD); // "value"
            }
        } else if (!actualProperties.contains(expectedProperty)) {
            qDebug() << "Actual:" << actualProperties;
            qDebug() << "Expected to find:" << expectedProperty;
            QVERIFY(false);
        }
    }
}

void tst_QVersitOrganizerExporter::testExtendedDetail_data()
{
    QTest::addColumn<QString>("extendedDetailName");
    QTest::addColumn<QVariant>("extendedDetailData");
    QTest::addColumn<QString>("extendedDetailDataInProperty");
    QTest::addColumn<bool>("extendedDetailPropertyCreated");

    QString jsonArrayWith("[\n    %1\n]\n");
    QString jsonArrayWithString = jsonArrayWith.arg("\"%1\"");
    {
        QTest::newRow("string data")
                << QString("name")
                << QVariant(QString("data"))
                << jsonArrayWithString.arg("data")
                << true;
        QTest::newRow("string data, empty")
                << QString("name")
                << QVariant(QString(""))
                << jsonArrayWithString.arg("")
                << true;
        QTest::newRow("string data, containing reserved characters")
                << QString("name")
                << QVariant(QString(",;:\\"))
                << jsonArrayWithString.arg(",;:\\\\")
                << true;
    }

    {
        QTest::newRow("double data")
                << QString("name")
                << QVariant((double)2.0)
                << jsonArrayWith.arg("2")
                << true;
        QTest::newRow("double data, negative")
                << QString("name")
                << QVariant((double)-1.0)
                << jsonArrayWith.arg("-1")
                << true;
        QTest::newRow("double data, multiple digits")
                << QString("name")
                << QVariant::fromValue<double>(10.199999999999999)
                << jsonArrayWith.arg("10.199999999999999")
                << true;
    }

    {
        QTest::newRow("boolean data")
                << QString("name")
                << QVariant(true)
                << jsonArrayWith.arg("true")
                << true;
    }

    {
        QTest::newRow("integer data, serialized as number/double")
                << QString("name")
                << QVariant((int)2)
                << jsonArrayWith.arg("2")
                << true;
        QTest::newRow("integer data, serialized as number/double")
                << QString("name")
                << QVariant((int)-10)
                << jsonArrayWith.arg("-10")
                << true;
    }
    {
        QTest::newRow("datetime data (using local time)")
                << QString("name")
                << QVariant(QDateTime::fromString("1997-07-16T19:20:30.123+01:00", Qt::ISODate))
                << jsonArrayWithString.arg("1997-07-16T19:20:30+01:00")
                << true;
        QTest::newRow("datetime data (using UTC)")
                << QString("name")
                << QVariant(QDateTime::fromString("1997-07-16T19:20:30.123+01:00", Qt::ISODate).toUTC())
                << jsonArrayWithString.arg("1997-07-16T18:20:30Z")
                << true;
        QTest::newRow("datetime data (using local time with unspecified timezone)")
                << QString("name")
                << QVariant(QDateTime::fromString("1997-07-16T19:20:30", Qt::ISODate))
                << jsonArrayWithString.arg("1997-07-16T19:20:30")
                << true;
    }

    {
        QTest::newRow("list data")
                << QString("name")
                << QVariant(QVariantList() << QString("string 1") << QString("string 2"))
                << QString("[\n    [\n        \"string 1\",\n        \"string 2\"\n    ]\n]\n")
                << true;
    }

    {
        QVariantMap map;
        map["key 1"] = QString("string 1");
        map["key 2"] = QString("string 2");
        QTest::newRow("map data")
                << QString("name")
                << QVariant(map)
                << QString("[\n    {\n        \"key 1\": \"string 1\",\n        \"key 2\": \"string 2\"\n    }\n]\n")
                << true;
    }

    {
        QVariantMap map;
        map["key"] = QVariantList() << (double)1 << (double)2;
        QTest::newRow("map data, containing a nested list")
                << QString("name")
                << QVariant(map)
                << QString("[\n    {\n        \"key\": [\n            1,\n            2\n        ]\n    }\n]\n")
                << true;
    }

    {
        QTest::newRow("empty string as name")
                << QString("")
                << QVariant(QString("data"))
                << jsonArrayWithString.arg("data")
                << true;
        QTest::newRow("name containing reserved characters")
                << QString(",;:\\")
                << QVariant(QString("data"))
                << jsonArrayWithString.arg("data")
                << true;
    }

    {
        QTest::newRow("empty variant as data")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("null")
                << true;
    }

    {
        QVariantHash hash;
        hash["key 1"] = QString("string 1");
        hash["key 2"] = QString("string 2");
        QTest::newRow("hash data")
                << QString("name")
                << QVariant(hash)
                << QString("[\n    {\n        \"key 1\": \"string 1\",\n        \"key 2\": \"string 2\"\n    }\n]\n")
                << true;
    }
    {
        QTest::newRow("data type not supported")
                << QString("name")
                << QVariant(QSize(1, 1))
                << QString()
                << false;
    }
}

void tst_QVersitOrganizerExporter::testMultipleExtendedDetails()
{
    QOrganizerEvent item;
    QOrganizerItemExtendedDetail extendedDetail1 = createExtendedDetail("detailName1", "detailData1");
    item.saveDetail(&extendedDetail1);
    QOrganizerItemExtendedDetail extendedDetail2 = createExtendedDetail("detailName2", "detailData2");
    item.saveDetail(&extendedDetail2);

    QList<QVersitProperty> expectedProperties;
    expectedProperties << createExtendedDetailPropertyForStringData(extendedDetail1.name(), "detailData1");
    expectedProperties << createExtendedDetailPropertyForStringData(extendedDetail2.name(), "detailData2");

    QVersitOrganizerExporter exporter;
    QVERIFY(exporter.exportItems(QList<QOrganizerItem>() << item));

    QVersitDocument document = exporter.document().subDocuments().first();
    QCOMPARE(findPropertiesByName(document, expectedProperties.first().name()).size(), 2);
    foreach (const QVersitProperty& expectedProperty, expectedProperties) {
        QList<QVersitProperty> actualProperties =
            findPropertiesByName(document, expectedProperty.name());
        if (!actualProperties.contains(expectedProperty)) {
            qDebug() << "Actual:" << actualProperties;
            qDebug() << "Expected to find:" << expectedProperty;
            QVERIFY(false);
        }
    }
}

QOrganizerItemExtendedDetail tst_QVersitOrganizerExporter::createExtendedDetail(
        const QString &name,
        const QVariant &data)
{
    QOrganizerItemExtendedDetail extendedDetail;
    extendedDetail.setName(name);
    extendedDetail.setData(data);
    return extendedDetail;
}

QVersitProperty tst_QVersitOrganizerExporter::createExtendedDetailProperty(
        const QString &name,
        const QVariant &data)
{
    QVersitProperty property;
    property.setName(QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL"));
    property.setValueType(QVersitProperty::CompoundType);
    property.setValue(QStringList() << name << data.toString());
    return property;
}

QVersitProperty tst_QVersitOrganizerExporter::createExtendedDetailPropertyForStringData(
        const QString &name,
        const QString &data)
{
    QString jsonArrayWith("[\n    %1\n]\n");
    QString jsonArrayWithString = jsonArrayWith.arg("\"%1\"");
    return createExtendedDetailProperty(name, jsonArrayWithString.arg(data));
}

QList<QVersitProperty> tst_QVersitOrganizerExporter::findPropertiesByName(
        const QVersitDocument &document, const QString &propertyName)
{
    QList<QVersitProperty> retval;
    foreach (const QVersitProperty& property, document.properties()) {
        if (property.name() == propertyName)
            retval << property;
    }
    return retval;
}

void tst_QVersitOrganizerExporter::testEmptyItemShouldNotBeExported()
{
    QVersitOrganizerExporter exporter;
    QList<QOrganizerItem> items;

    QOrganizerEvent ev;
    QVERIFY(ev.isEmpty());
    QOrganizerItem item1;
    QVERIFY(item1.isEmpty());
    items << static_cast<QOrganizerItem>(ev)<<item1;

    QVERIFY(!exporter.exportItems(items));
    QVERIFY(!exporter.errorMap().isEmpty());

    QVersitOrganizerExporter::Error errorCode = exporter.errorMap().value(0);
    QVERIFY2(errorCode == QVersitOrganizerExporter::EmptyOrganizerError,
             QString("exporter.errorMap().value(0) == "
                     + QString::number(errorCode)).toStdString().c_str());
    errorCode = exporter.errorMap().value(1);
    QVERIFY2(errorCode == QVersitOrganizerExporter::EmptyOrganizerError
             || errorCode == QVersitOrganizerExporter::UnknownComponentTypeError,
             QString("exporter.errorMap().value(1) == "
                     + QString::number(errorCode)).toStdString().c_str() );
    items.clear();
    exporter.errorMap().clear();
}

QTEST_MAIN(tst_QVersitOrganizerExporter)
