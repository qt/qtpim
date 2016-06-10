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

#include "tst_qversitorganizerimporter.h"
#include <QtTest/QtTest>

QTORGANIZER_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

Q_DECLARE_METATYPE(QList<QOrganizerItem>)
Q_DECLARE_METATYPE(QList<QOrganizerItemDetail>)
Q_DECLARE_METATYPE(QList<QVersitProperty>)

void tst_QVersitOrganizerImporter::testImport()
{
    QFETCH(QVersitDocument, document);
    QFETCH(QList<QOrganizerItem>, expectedItems);

    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(document));
    QVERIFY(importer.errorMap().isEmpty());
    QList<QOrganizerItem> items = importer.items();
    if (items != expectedItems) {
        qDebug() << "Actual:" << items;
        qDebug() << "Expected:" << expectedItems;
        QCOMPARE(items, expectedItems);
    }
}

void tst_QVersitOrganizerImporter::testImport_data()
{
    QTest::addColumn<QVersitDocument>("document");
    QTest::addColumn<QList<QOrganizerItem> >("expectedItems");

    {
        QVersitDocument document(QVersitDocument::ICalendar20Type);
        document.setComponentType(QStringLiteral("VCALENDAR"));
        QVersitProperty property;
        property.setName(QStringLiteral("PRODID"));
        property.setValue(QStringLiteral("-//hacksw/handcal//NONSGML v1.0//EN"));
        document.addProperty(property);
        QVersitDocument nested(QVersitDocument::ICalendar20Type);
        nested.setComponentType(QStringLiteral("VEVENT"));
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

        QTest::newRow("sample event and todo") << document << items;
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

        QTest::newRow("event occurrence exception") << document << items;
    }

    {
        QVersitDocument document(QVersitDocument::ICalendar20Type);
        document.setComponentType(QStringLiteral("VCALENDAR"));
        QVersitDocument nested(QVersitDocument::ICalendar20Type);
        nested.setComponentType(QStringLiteral("VTODO"));
        QVersitProperty property;
        property.setName(QStringLiteral("RRULE"));
        property.setValue(QStringLiteral("FREQ=WEEKLY"));
        nested.addProperty(property);
        document.addSubDocument(nested);

        QOrganizerTodo todo;
        QOrganizerRecurrenceRule rrule;
        rrule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        todo.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << rrule);

        QList<QOrganizerItem> items;
        items << static_cast<QOrganizerItem>(todo);

        QTest::newRow("todo recurrence rule") << document << items;
    }
}

void tst_QVersitOrganizerImporter::testImportEventProperties()
{
    QFETCH(QList<QVersitProperty>, properties);
    QFETCH(QList<QOrganizerItemDetail>, expectedDetails);

    QVersitDocument document(QVersitDocument::ICalendar20Type);
    document.setComponentType(QStringLiteral("VCALENDAR"));
    QVersitDocument nested(QVersitDocument::ICalendar20Type);
    nested.setComponentType(QStringLiteral("VEVENT"));
    foreach (const QVersitProperty& property, properties) {
        if (property.valueType() == QVersitProperty::VersitDocumentType)
            nested.addSubDocument(property.value<QVersitDocument>());
        else
            nested.addProperty(property);
    }
    document.addSubDocument(nested);
    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(document));
    QVERIFY(importer.errorMap().isEmpty());
    QList<QOrganizerItem> items = importer.items();
    QCOMPARE(items.size(), 1);
    foreach (const QOrganizerItemDetail& expectedDetail, expectedDetails) {

        QList<QOrganizerItemDetail> actualDetails = items.first().details(expectedDetail.type());
        if (!actualDetails.contains(expectedDetail)) {
            qDebug() << "Actual:" << actualDetails;
            qDebug() << "Expected to find:" << expectedDetail;
            QVERIFY(false);
        }
    }
}

void tst_QVersitOrganizerImporter::testImportEventProperties_data()
{
    QTest::addColumn<QList<QVersitProperty> >("properties");
    QTest::addColumn<QList<QOrganizerItemDetail> >("expectedDetails");

    {
        QVersitProperty property;
        property.setName(QStringLiteral("SUMMARY"));
        property.setValue(QStringLiteral("jabberwocky"));
        QOrganizerItemDisplayLabel displayLabel;
        displayLabel.setLabel(QStringLiteral("jabberwocky"));
        QTest::newRow("one summary") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << displayLabel);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;

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
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 5);
        audibleReminder.setSecondsBeforeStart(90);
        QTest::newRow("audible reminder") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;

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
        property.setValue(QStringLiteral("-PT90S"));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DURATION"));
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
        QTest::newRow("audible reminder with trigger VALUE = DURATION") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        dtend.setValue(QStringLiteral("20100102T030415"));
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 15)));

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
        property.setValue(QStringLiteral("-PT90S"));
        property.insertParameter(QStringLiteral("RELATED"), QStringLiteral("END"));
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
        audibleReminder.setSecondsBeforeStart(80);
        QTest::newRow("audible reminder with relative (END) trigger")
                << (QList<QVersitProperty>() << dtstart << dtend << property)
            << (QList<QOrganizerItemDetail>() << etr << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        dtend.setValue(QStringLiteral("20100102T030415"));
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 15)));

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
        property.setValue(QStringLiteral("-PT90S"));
        property.insertParameter(QStringLiteral("RELATED"), QStringLiteral("START"));
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
        QTest::newRow("audible reminder with relative (START) trigger")
                << (QList<QVersitProperty>() << dtstart << dtend << property)
            << (QList<QOrganizerItemDetail>() << etr << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        dtend.setValue(QStringLiteral("20100102T030415"));
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 15)));

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
        property.setValue(QStringLiteral("20100102T030305"));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE-TIME"));
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
        audibleReminder.setSecondsBeforeStart(60);
        QTest::newRow("audible reminder with absolute trigger")
                << (QList<QVersitProperty>() << dtstart << dtend << property)
            << (QList<QOrganizerItemDetail>() << etr << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("DISPLAY"));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Test visual reminder"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setMessage(QStringLiteral("Test visual reminder"));
        visualReminder.setRepetition(3, 5);
        visualReminder.setSecondsBeforeStart(90);
        QTest::newRow("visual reminder") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << visualReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument;
        QList<QVersitProperty> valarmProperties;

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
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
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
        emailReminder.setContents(QStringLiteral("Test email subject"),
                                  QStringLiteral("Test email body"),
                                  QVariantList());
        emailReminder.setRepetition(3, 5);
        emailReminder.setSecondsBeforeStart(90);
        emailReminder.setRecipients(QStringList()
                                    << QStringLiteral("First email recipient")
                                    << QStringLiteral("Second email recipient"));
        QTest::newRow("email reminder") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << emailReminder);
    }

    {
        QList<QOrganizerItemDetail> expectedDetails;
        QList<QVersitProperty> propertiesToImport;

        {
            QVersitProperty property;
            QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
            QList<QVersitProperty> valarmProperties;

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
            property.setValue(QStringLiteral("-PT90S"));
            valarmProperties << property;

            valarmDocument.setComponentType("VALARM");
            valarmDocument.setProperties(valarmProperties);
            property.setValueType(QVersitProperty::VersitDocumentType);
            property.setName(QStringLiteral("VALARM"));
            property.setValue(QVariant::fromValue(valarmDocument));
            QOrganizerItemAudibleReminder audibleReminder;
            audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
            audibleReminder.setRepetition(3, 5);
            audibleReminder.setSecondsBeforeStart(90);
            propertiesToImport << property;
            expectedDetails << audibleReminder;
        }
        {
            QVersitProperty property;
            QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
            QList<QVersitProperty> valarmProperties;

            property.setName(QStringLiteral("ACTION"));
            property.setValue(QStringLiteral("DISPLAY"));
            valarmProperties << property;
            property.setName(QStringLiteral("DURATION"));
            property.setValue(QStringLiteral("PT5S"));
            valarmProperties << property;
            property.setName(QStringLiteral("REPEAT"));
            property.setValue(3);
            valarmProperties << property;
            property.setName(QStringLiteral("TRIGGER"));
            property.setValue(QStringLiteral("-PT90S"));
            valarmProperties << property;
            property.setName(QStringLiteral("DESCRIPTION"));
            property.setValue(QStringLiteral("Test visual reminder"));
            valarmProperties << property;

            valarmDocument.setComponentType("VALARM");
            valarmDocument.setProperties(valarmProperties);
            property.setValueType(QVersitProperty::VersitDocumentType);
            property.setName(QStringLiteral("VALARM"));
            property.setValue(QVariant::fromValue(valarmDocument));
            QOrganizerItemVisualReminder visualReminder;
            visualReminder.setMessage(QStringLiteral("Test visual reminder"));
            visualReminder.setRepetition(3, 5);
            visualReminder.setSecondsBeforeStart(90);
            propertiesToImport << property;
            expectedDetails << visualReminder;
        }
        {
            QVersitProperty property;
            QVersitDocument valarmDocument;
            QList<QVersitProperty> valarmProperties;

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
            property.setValue(QStringLiteral("-PT90S"));
            valarmProperties << property;
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
            emailReminder.setContents(QStringLiteral("Test email subject"),
                                      QStringLiteral("Test email body"),
                                      QVariantList());
            emailReminder.setRepetition(3, 5);
            emailReminder.setSecondsBeforeStart(90);
            emailReminder.setRecipients(QStringList()
                                        << QStringLiteral("First email recipient")
                                        << QStringLiteral("Second email recipient"));
            propertiesToImport << property;
            expectedDetails << emailReminder;
        }
        QTest::newRow("multiple reminders") << propertiesToImport
            << expectedDetails;
    }
    {
        QVersitProperty property;
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Twinkle, twinkle, little bat! How I wonder what you're at."));
        QOrganizerItemDescription description;
        description.setDescription(QStringLiteral("Twinkle, twinkle, little bat! How I wonder what you're at."));
        QTest::newRow("one description") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << description);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("CATEGORIES"));
        property.setValue(QStringLiteral("Important Event"));
        QOrganizerItemTag tag;
        tag.setTag(QStringLiteral("Important Event"));
        QTest::newRow("tag 1")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() << tag);

        // Set another one for multiple handling test
        property.setValue(QStringLiteral("Important Event 2"));
        tag.setTag(QStringLiteral("Important Event 2"));
        QTest::newRow("tag 2")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() << tag);

        // Empty tag
        property.clear();
        property.setName("CATEGORIES");
        tag.removeValue(QOrganizerItemTag::FieldTag);
        QTest::newRow("empty tag")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() ); // Detail side should be empty
    }

    {
        QVersitProperty property = createExtendedDetailPropertyForStringData("name", "data");
        QOrganizerItemExtendedDetail extendedDetail = createExtendedDetail("name", "data");
        QTest::newRow("extended detail")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() << extendedDetail);
    }

    {
        QVersitProperty property; // Proper property.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("4711") << QStringLiteral("134f23dbb2"));
        QOrganizerItemVersion version;
        version.setVersion(4711);
        version.setExtendedVersion(QByteArray("134f23dbb2"));
        QTest::newRow("version") << (QList<QVersitProperty>() << property)
                                 << (QList<QOrganizerItemDetail>() << version);
    }

    {
        QVersitProperty property; // Extended version empty.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("4711") << QStringLiteral(""));
        QOrganizerItemVersion version;
        version.setVersion(4711);
        QTest::newRow("version") << (QList<QVersitProperty>() << property)
                                 << (QList<QOrganizerItemDetail>() << version);
    }

    {
        QVersitProperty property; // No extended version.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("4711"));
        QOrganizerItemVersion version;
        version.setVersion(4711);
        QTest::newRow("version") << (QList<QVersitProperty>() << property)
                                 << (QList<QOrganizerItemDetail>() << version);
    }

    {
        QVersitProperty property; // Version non number.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral("2someletters"));
        QTest::newRow("version") << (QList<QVersitProperty>() << property)
                                 << (QList<QOrganizerItemDetail>());
    }

    {
        QVersitProperty property; // Version emptry string.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        property.setValue(QStringList() << QStringLiteral(""));
        QTest::newRow("version") << (QList<QVersitProperty>() << property)
                                 << (QList<QOrganizerItemDetail>());
    }

    {
        QVersitProperty property; // Empty property.
        property.setName(QStringLiteral("X-QTPROJECT-VERSION"));
        property.setValueType(QVersitProperty::CompoundType);
        QTest::newRow("version") << (QList<QVersitProperty>() << property)
                                 << (QList<QOrganizerItemDetail>());
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
            << (QList<QVersitProperty>() << property1 << property2)
            << (QList<QOrganizerItemDetail>() << comment1 << comment2);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("UID"));
        property.setValue(QStringLiteral("1234567"));
        QOrganizerItemGuid guid;
        guid.setGuid(QStringLiteral("1234567"));
        QTest::newRow("guid")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << guid);
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
        QTest::newRow("dtstart and dtend") << properties
            << (QList<QOrganizerItemDetail>() << etr);

        dtend.setValue(QStringLiteral("20100102T235959"));
        properties.prepend(dtend);
        QTest::newRow("multiple dtstart and dtend") << properties
            << (QList<QOrganizerItemDetail>() << etr); // last takes precedence
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
        // The time portion must be valid but is ignored.  This test cheats a bit because it knows
        // the implementation sets it to 00:00:00
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(0, 0, 0)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 2), QTime(0, 0, 0)));
        etr.setAllDay(true);
        QTest::newRow("all day event") << properties
            << (QList<QOrganizerItemDetail>() << etr);
    }

    {
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20091231T000000")); // equivalent to 2010, day zero

        QVersitProperty duration;
        duration.setName(QStringLiteral("DURATION"));
        duration.setValue(QStringLiteral("P15DT5H7M20S"));
        QOrganizerEventTime etr;
        etr.setStartDateTime(QDateTime(QDate(2009, 12, 31), QTime(0, 0, 0)));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 15), QTime(5, 7, 20)));
        QTest::newRow("dtstart and duration: days hours minutes seconds")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);
        QTest::newRow("duration and dtstart")
            << (QList<QVersitProperty>() << duration << dtstart)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("PT5H7M20S"));
        etr.setEndDateTime(QDateTime(QDate(2009, 12, 31), QTime(5, 7, 20)));
        QTest::newRow("dtstart and duration: hours minutes seconds")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("PT7M20S"));
        etr.setEndDateTime(QDateTime(QDate(2009, 12, 31), QTime(0, 7, 20)));
        QTest::newRow("dtstart and duration: minutes seconds")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("PT20S"));
        etr.setEndDateTime(QDateTime(QDate(2009, 12, 31), QTime(0, 0, 20)));
        QTest::newRow("dtstart and duration: seconds")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("P15DT5H7M"));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 15), QTime(5, 7, 0)));
        QTest::newRow("dtstart and duration: days hours minutes")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("P15DT5H"));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 15), QTime(5, 0, 0)));
        QTest::newRow("dtstart and duration: days hours")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("P15D"));
        etr.setEndDateTime(QDateTime(QDate(2010, 1, 15), QTime(0, 0, 0)));
        QTest::newRow("dtstart and duration: days")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        duration.setValue(QStringLiteral("P7W"));
        etr.setEndDateTime(QDateTime(QDate(2010, 2, 18), QTime(0, 0, 0)));
        QTest::newRow("dtstart and duration: weeks")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);

        dtstart.setValue(QStringLiteral("20100115T050720"));
        duration.setValue(QStringLiteral("-P15DT5H7M20S"));
        etr.setStartDateTime(QDateTime(QDate(2010, 1, 15), QTime(5, 7, 20)));
        etr.setEndDateTime(QDateTime(QDate(2009, 12, 31), QTime(0, 0, 0)));
        QTest::newRow("dtstart and duration: negative")
            << (QList<QVersitProperty>() << dtstart << duration)
            << (QList<QOrganizerItemDetail>() << etr);
    }

    {
        QList<QVersitProperty> properties;
        QVersitProperty created;
        created.setName(QStringLiteral("CREATED"));
        created.setValue(QStringLiteral("20100102T030405Z"));
        properties << created;
        QVersitProperty modified;
        modified.setName(QStringLiteral("LAST-MODIFIED"));
        modified.setValue(QStringLiteral("20100102T030406Z"));
        properties << modified;
        QOrganizerItemTimestamp timestamp;
        timestamp.setCreated(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5), Qt::UTC));
        timestamp.setLastModified(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 6), Qt::UTC));
        QTest::newRow("created and last modified") << properties
            << (QList<QOrganizerItemDetail>() << timestamp);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=DAILY"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Daily);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule daily")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=WEEKLY"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule weekly")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=MONTHLY"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule monthly")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=YEARLY"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule yearly")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=MONTHLY;INTERVAL=2;BYDAY=TU"));
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrenceRule.setInterval(2);
        recurrenceRule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Tuesday);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule monthly")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=MONTHLY;BYMONTHDAY=1,-3"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Monthly);
        recurrenceRule.setDaysOfMonth(QSet<int>() << 1 << -3);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule bymonthday")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=YEARLY;BYWEEKNO=1,-3"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setWeeksOfYear(QSet<int>() << 1 << -3);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule byweekno")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
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
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=YEARLY;BYYEARDAY=1,366,-1"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setDaysOfYear(QSet<int>() << 1 << 366 << -1);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule byyearday")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
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
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
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
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=YEARLY;BYMONTH=4;BYDAY=1SU"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Yearly);
        recurrenceRule.setMonthsOfYear(QSet<QOrganizerRecurrenceRule::Month>()
                << QOrganizerRecurrenceRule::April);
        recurrenceRule.setDaysOfWeek(QSet<Qt::DayOfWeek>() << Qt::Sunday);
        recurrenceRule.setPositions(QSet<int>() << 1);
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule byday with position")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
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
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=DAILY;UNTIL=20000131T101112;BYMONTH=1"));
        QTest::newRow("rrule until with time")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
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
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=WEEKLY;INTERVAL=0"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        recurrenceRule.setInterval(1); // default interval when invalid interval is found
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule invalid interval1")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=WEEKLY;INTERVAL=bad"));
        QTest::newRow("rrule invalid interval2")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=WEEKLY;INTERVAL="));
        QTest::newRow("rrule invalid interval3")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty rrule;
        rrule.setName(QStringLiteral("RRULE"));
        rrule.setValue(QStringLiteral("FREQ=WEEKLY;COUNT=-2"));
        QOrganizerItemRecurrence recurrence;
        QOrganizerRecurrenceRule recurrenceRule;
        recurrenceRule.setFrequency(QOrganizerRecurrenceRule::Weekly);
        recurrenceRule.setLimit(-1); // default count when invalid count is found
        recurrence.setRecurrenceRules(QSet<QOrganizerRecurrenceRule>() << recurrenceRule);
        QTest::newRow("rrule invalid count1")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=WEEKLY;COUNT=bad"));
        QTest::newRow("rrule invalid count2")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);

        rrule.setValue(QStringLiteral("FREQ=WEEKLY;COUNT="));
        QTest::newRow("rrule invalid count3")
            << (QList<QVersitProperty>() << rrule)
            << (QList<QOrganizerItemDetail>() << recurrence);
    }

    {
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("19970304T110000"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        dtend.setValue(QStringLiteral("19970304T110000"));
        QVersitProperty rdate;
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
            << (QList<QVersitProperty>() << dtstart << dtend << rdate)
            << (QList<QOrganizerItemDetail>() << etr << recurrence);

        // XXX times in RDATE are ignored
        rdate.setValue(QStringLiteral("19970304T133700"));
        recurrenceDates.clear();
        recurrenceDates << QDate(1997, 3, 4);
        recurrence.setRecurrenceDates(recurrenceDates);
        QTest::newRow("rdate with time")
            << (QList<QVersitProperty>() << dtstart << dtend << rdate)
            << (QList<QOrganizerItemDetail>() << etr << recurrence);

        rdate.setValue(QStringLiteral("19970304,19970504,19970704"));
        recurrenceDates.clear();
        recurrenceDates << QDate(1997, 3, 4)
                        << QDate(1997, 5, 4)
                        << QDate(1997, 7, 4);
        recurrence.setRecurrenceDates(recurrenceDates);
        QTest::newRow("multiple rdate")
            << (QList<QVersitProperty>() << dtstart << dtend << rdate)
            << (QList<QOrganizerItemDetail>() << etr << recurrence);
    }

    {
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("19970304T110000"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DTEND"));
        dtend.setValue(QStringLiteral("19970304T110000"));
        QVersitProperty rdate;
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
            << (QList<QVersitProperty>() << dtstart << dtend << rdate)
            << (QList<QOrganizerItemDetail>() << etr << recurrence);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("PRIORITY"));
        property.setValue(QStringLiteral("0"));

        QOrganizerItemPriority detail;
        detail.setPriority(QOrganizerItemPriority::UnknownPriority);
        QTest::newRow("priority 0")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("1"));
        detail.setPriority(QOrganizerItemPriority::HighestPriority);
        QTest::newRow("priority 1")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("2"));
        detail.setPriority(QOrganizerItemPriority::ExtremelyHighPriority);
        QTest::newRow("priority 2")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("3"));
        detail.setPriority(QOrganizerItemPriority::VeryHighPriority);
        QTest::newRow("priority 3")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("4"));
        detail.setPriority(QOrganizerItemPriority::HighPriority);
        QTest::newRow("priority 4")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("5"));
        detail.setPriority(QOrganizerItemPriority::MediumPriority);
        QTest::newRow("priority 5")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("6"));
        detail.setPriority(QOrganizerItemPriority::LowPriority);
        QTest::newRow("priority 6")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("7"));
        detail.setPriority(QOrganizerItemPriority::VeryLowPriority);
        QTest::newRow("priority 7")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("8"));
        detail.setPriority(QOrganizerItemPriority::ExtremelyLowPriority);
        QTest::newRow("priority 8")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);

        property.setValue(QStringLiteral("9"));
        detail.setPriority(QOrganizerItemPriority::LowestPriority);
        QTest::newRow("priority 9")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << detail);
    }
}

void tst_QVersitOrganizerImporter::testImportTodoProperties()
{
    QFETCH(QList<QVersitProperty>, properties);
    QFETCH(QList<QOrganizerItemDetail>, expectedDetails);

    QVersitDocument document(QVersitDocument::ICalendar20Type);
    document.setComponentType(QStringLiteral("VCALENDAR"));
    QVersitDocument nested(QVersitDocument::ICalendar20Type);
    nested.setComponentType(QStringLiteral("VTODO"));
    foreach (const QVersitProperty& property, properties) {
        if (property.valueType() == QVersitProperty::VersitDocumentType)
            nested.addSubDocument(property.value<QVersitDocument>());
        else
            nested.addProperty(property);
    }
    document.addSubDocument(nested);

    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(document));
    QVERIFY(importer.errorMap().isEmpty());
    QList<QOrganizerItem> items = importer.items();
    QCOMPARE(items.size(), 1);

    foreach (const QOrganizerItemDetail& expectedDetail, expectedDetails) {
        QOrganizerItemDetail actualDetail = items.first().detail(expectedDetail.type());
        if (actualDetail != expectedDetail) {
            qDebug() << "Actual:" << actualDetail;
            qDebug() << "Expected:" << expectedDetail;
            QCOMPARE(actualDetail, expectedDetail);
        }
    }
}

void tst_QVersitOrganizerImporter::testImportTodoProperties_data()
{
    QTest::addColumn<QList<QVersitProperty> >("properties");
    QTest::addColumn<QList<QOrganizerItemDetail> >("expectedDetails");

    {
        QList<QVersitProperty> properties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102"));
        dtstart.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        properties << dtstart;
        QVersitProperty dtdue;
        dtdue.setName(QStringLiteral("DUE"));
        dtdue.setValue(QStringLiteral("20100103"));
        dtdue.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE"));
        properties << dtdue;
        QOrganizerTodoTime todoTime;
        // The time portion must be valid but is ignored.  This test cheats a bit because it knows
        // the implementation sets it to 00:00:00
        todoTime.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(0, 0, 0)));
        todoTime.setDueDateTime(QDateTime(QDate(2010, 1, 3), QTime(0, 0, 0)));
        todoTime.setAllDay(true);
        QTest::newRow("all day todo") << properties
            << (QList<QOrganizerItemDetail>() << todoTime);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument;
        QList<QVersitProperty> valarmProperties;

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
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 5);
        audibleReminder.setSecondsBeforeStart(90);
        QTest::newRow("audible reminder") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DUE"));
        dtend.setValue(QStringLiteral("20100102T030415"));
        QOrganizerTodoTime todoTime;
        todoTime.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        todoTime.setDueDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 15)));

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("AUDIO"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT10S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        property.setValue(QStringLiteral("-PT90S"));
        property.insertParameter(QStringLiteral("RELATED"), QStringLiteral("END"));
        valarmProperties << property;
        property.clear();

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 10);
        audibleReminder.setSecondsBeforeStart(90);
        QTest::newRow("audible reminder with relative (END) trigger")
                << (QList<QVersitProperty>() << dtstart << dtend << property)
            << (QList<QOrganizerItemDetail>() << todoTime << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DUE"));
        dtend.setValue(QStringLiteral("20100102T030415"));
        QOrganizerTodoTime todoTime;
        todoTime.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        todoTime.setDueDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 15)));

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("AUDIO"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT10S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        property.setValue(QStringLiteral("-PT90S"));
        property.insertParameter(QStringLiteral("RELATED"), QStringLiteral("START"));
        valarmProperties << property;
        property.clear();

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 10);
        audibleReminder.setSecondsBeforeStart(100);
        QTest::newRow("audible reminder with relative (START) trigger")
                << (QList<QVersitProperty>() << dtstart << dtend << property)
            << (QList<QOrganizerItemDetail>() << todoTime << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;
        QVersitProperty dtstart;
        dtstart.setName(QStringLiteral("DTSTART"));
        dtstart.setValue(QStringLiteral("20100102T030405"));
        QVersitProperty dtend;
        dtend.setName(QStringLiteral("DUE"));
        dtend.setValue(QStringLiteral("20100102T030415"));
        QOrganizerTodoTime todoTime;
        todoTime.setStartDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5)));
        todoTime.setDueDateTime(QDateTime(QDate(2010, 1, 2), QTime(3, 4, 15)));

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("AUDIO"));
        valarmProperties << property;
        property.setName(QStringLiteral("ATTACH"));
        property.setValue(QUrl(QStringLiteral("http://qt.nokia.com")));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT10S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        property.setValue(QStringLiteral("20100102T030400"));
        property.insertParameter(QStringLiteral("VALUE"), QStringLiteral("DATE-TIME"));
        valarmProperties << property;
        property.clear();

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemAudibleReminder audibleReminder;
        audibleReminder.setDataUrl(QUrl("http://qt.nokia.com"));
        audibleReminder.setRepetition(3, 10);
        audibleReminder.setSecondsBeforeStart(15);
        QTest::newRow("audible reminder with absolute trigger")
                << (QList<QVersitProperty>() << dtstart << dtend << property)
            << (QList<QOrganizerItemDetail>() << todoTime << audibleReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument(QVersitDocument::ICalendar20Type);
        QList<QVersitProperty> valarmProperties;

        property.setName(QStringLiteral("ACTION"));
        property.setValue(QStringLiteral("DISPLAY"));
        valarmProperties << property;
        property.setName(QStringLiteral("DURATION"));
        property.setValue(QStringLiteral("PT5S"));
        valarmProperties << property;
        property.setName(QStringLiteral("REPEAT"));
        property.setValue(3);
        valarmProperties << property;
        property.setName(QStringLiteral("TRIGGER"));
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
        property.setName(QStringLiteral("DESCRIPTION"));
        property.setValue(QStringLiteral("Test visual reminder"));
        valarmProperties << property;

        valarmDocument.setComponentType("VALARM");
        valarmDocument.setProperties(valarmProperties);
        property.setValueType(QVersitProperty::VersitDocumentType);
        property.setName(QStringLiteral("VALARM"));
        property.setValue(QVariant::fromValue(valarmDocument));
        QOrganizerItemVisualReminder visualReminder;
        visualReminder.setMessage(QStringLiteral("Test visual reminder"));
        visualReminder.setRepetition(3, 5);
        visualReminder.setSecondsBeforeStart(90);
        QTest::newRow("visual reminder") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << visualReminder);
    }

    {
        QVersitProperty property;
        QVersitDocument valarmDocument;
        QList<QVersitProperty> valarmProperties;

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
        property.setValue(QStringLiteral("-PT90S"));
        valarmProperties << property;
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
        emailReminder.setContents(QStringLiteral("Test email subject"),
                                  QStringLiteral("Test email body"),
                                  QVariantList());
        emailReminder.setRepetition(3, 5);
        emailReminder.setSecondsBeforeStart(90);
        emailReminder.setRecipients(QStringList()
                                    << QStringLiteral("First email recipient")
                                    << QStringLiteral("Second email recipient"));
        QTest::newRow("email reminder") << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << emailReminder);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("STATUS"));
        property.setValue(QStringLiteral("COMPLETED"));
        QOrganizerTodoProgress progress;
        progress.setStatus(QOrganizerTodoProgress::StatusComplete);
        QTest::newRow("status completed")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << progress);

        property.setValue(QStringLiteral("NEEDS-ACTION"));
        progress.setStatus(QOrganizerTodoProgress::StatusNotStarted);
        QTest::newRow("status needs-action")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << progress);

        property.setValue(QStringLiteral("IN-PROCESS"));
        progress.setStatus(QOrganizerTodoProgress::StatusInProgress);
        QTest::newRow("status in-process")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << progress);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("PERCENT-COMPLETE"));
        property.setValue(QStringLiteral("42"));
        QOrganizerTodoProgress progress;
        progress.setPercentageComplete(42);
        QTest::newRow("percent-complete")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << progress);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("COMPLETED"));
        property.setValue(QStringLiteral("20100609T161500"));
        QOrganizerTodoProgress progress;
        progress.setFinishedDateTime(QDateTime(QDate(2010, 6, 9), QTime(16, 15, 0)));
        QTest::newRow("completed")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << progress);
    }

    {
        QVersitProperty property;
        property.setName(QStringLiteral("CATEGORIES"));
        property.setValue(QStringLiteral("Important Event"));
        QOrganizerItemTag tag;
        tag.setTag(QStringLiteral("Important Event"));
        QTest::newRow("tag 1")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() << tag);

        // Set another one for multiple handling test
        property.setValue(QStringLiteral("Important Event 2"));
        tag.setTag(QStringLiteral("Important Event 2"));
        QTest::newRow("tag 2")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() << tag);

        // Empty tag
        property.clear();
        property.setName("CATEGORIES");
        tag.removeValue(QOrganizerItemTag::FieldTag);
        QTest::newRow("empty tag")
                << (QList<QVersitProperty>() << property)
                << (QList<QOrganizerItemDetail>() ); // Detail side should be empty
    }

    {
        QVersitProperty property = createExtendedDetailPropertyForStringData("name", "data");
        QOrganizerItemExtendedDetail extendedDetail = createExtendedDetail("name", "data");
        QTest::newRow("extended detail")
            << (QList<QVersitProperty>() << property)
            << (QList<QOrganizerItemDetail>() << extendedDetail);
    }
}

void tst_QVersitOrganizerImporter::testExtendedDetail()
{
    QFETCH(QString, extendedDetailName);
    QFETCH(QVariant, extendedDetailData);
    QFETCH(QString, extendedDetailDataInProperty);
    QFETCH(bool, extendedDetailCreated);

    QVersitDocument document(QVersitDocument::ICalendar20Type);
    document.setComponentType(QStringLiteral("VCALENDAR"));
    QVersitDocument nested(QVersitDocument::ICalendar20Type);
    nested.setComponentType(QStringLiteral("VEVENT"));
    nested.addProperty(createExtendedDetailProperty(extendedDetailName,
                                                    extendedDetailDataInProperty));
    document.addSubDocument(nested);

    QOrganizerItemExtendedDetail expectedDetail =
            createExtendedDetail(extendedDetailName, extendedDetailData);

    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(document));

    QList<QOrganizerItem> items = importer.items();
    QList<QOrganizerItemDetail> actualDetails = items.first().details(expectedDetail.type());
    if (!extendedDetailCreated) {
        QCOMPARE(actualDetails.size(), 0);
    } else {
        QCOMPARE(actualDetails.size(), 1);
        if (static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Double
                || static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Float) {
            QCOMPARE(actualDetails.first().values().keys(), expectedDetail.values().keys());
            QCOMPARE(actualDetails.first().values().keys().size(), 2); // name + value
            int nameKey = actualDetails.first().values().keys().at(0);
            int valueKey = actualDetails.first().values().keys().at(1);
            QCOMPARE(actualDetails.first().value(nameKey), expectedDetail.value(nameKey));
            QVariant actualValue = actualDetails.first().value(valueKey);
            QVariant expectedValue = expectedDetail.value(valueKey);
            if (static_cast<QMetaType::Type>(extendedDetailData.type()) == QMetaType::Float) {
                float actualF = actualValue.value<float>();
                float expectedF = expectedValue.value<float>();
                QCOMPARE(actualF, expectedF);
            } else {
                double actualD = actualValue.value<double>();
                double expectedD = expectedValue.value<double>();
                QCOMPARE(actualD, expectedD);
            }
        } else if (!actualDetails.contains(expectedDetail)) {
            qDebug() << "Actual:" << actualDetails;
            qDebug() << "Expected to find:" << expectedDetail;
            QVERIFY(false);
        }
    }
}

void tst_QVersitOrganizerImporter::testExtendedDetail_data()
{
    QTest::addColumn<QString>("extendedDetailName");
    QTest::addColumn<QVariant>("extendedDetailData");
    QTest::addColumn<QString>("extendedDetailDataInProperty");
    QTest::addColumn<bool>("extendedDetailCreated");

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
                << QVariant((double)10.2)
                << jsonArrayWith.arg("10.2")
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
        QTest::newRow("datetime data, imported as a string")
                << QString("name")
                << QVariant(QString("1997-07-16T19:20:30+01:00"))
                << jsonArrayWithString.arg("1997-07-16T19:20:30+01:00")
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
        QTest::newRow("null denoting an empty variant")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("null")
                << true;
        QTest::newRow("compact json with extra whitespace removed")
                << QString("name")
                << QVariant(QString("data"))
                << QString("[\"data\"]")
                << true;
    }

    {
        QTest::newRow("invalid property value: empty json array as property value")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("")
                << false;
        QTest::newRow("invalid property value: invalid json value")
                << QString("name")
                << QVariant()
                << jsonArrayWith.arg("invalid")
                << false;
    }
}

void tst_QVersitOrganizerImporter::testMultipleExtendedDetails()
{
    QVersitDocument document(QVersitDocument::ICalendar20Type);
    document.setComponentType(QStringLiteral("VCALENDAR"));
    QVersitDocument nested(QVersitDocument::ICalendar20Type);
    nested.setComponentType(QStringLiteral("VEVENT"));
    nested.addProperty(createExtendedDetailPropertyForStringData("detailName1", "detailData1"));
    nested.addProperty(createExtendedDetailPropertyForStringData("detailName2", "detailData2"));
    document.addSubDocument(nested);

    QList<QOrganizerItemExtendedDetail> expectedDetails;
    expectedDetails << createExtendedDetail("detailName1", "detailData1");
    expectedDetails << createExtendedDetail("detailName2", "detailData2");

    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(document));

    QList<QOrganizerItem> items = importer.items();
    QList<QOrganizerItemDetail> actualDetails = items.first().details(QOrganizerItemDetail::TypeExtendedDetail);
    QCOMPARE(actualDetails.size(), 2);
    foreach (const QOrganizerItemDetail& expectedDetail, expectedDetails) {
        if (!actualDetails.contains(expectedDetail)) {
            qDebug() << "Actual:" << actualDetails;
            qDebug() << "Expected to find:" << expectedDetail;
            QVERIFY(false);
        }
    }
}

QOrganizerItemExtendedDetail tst_QVersitOrganizerImporter::createExtendedDetail(
        const QString &name,
        const QVariant &data)
{
    QOrganizerItemExtendedDetail extendedDetail;
    extendedDetail.setName(name);
    extendedDetail.setData(data);
    return extendedDetail;
}

QVersitProperty tst_QVersitOrganizerImporter::createExtendedDetailProperty(
        const QString &name,
        const QVariant &data)
{
    QVersitProperty property;
    property.setName(QStringLiteral("X-QTPROJECT-EXTENDED-DETAIL"));
    property.setValue(QStringList() << name << data.toString());
    property.setValueType(QVersitProperty::CompoundType);
    return property;
}

QVersitProperty tst_QVersitOrganizerImporter::createExtendedDetailPropertyForStringData(
        const QString &name,
        const QString &data)
{
    QString jsonArrayWith("[\n    %1\n]\n");
    QString jsonArrayWithString = jsonArrayWith.arg("\"%1\"");
    return createExtendedDetailProperty(name, jsonArrayWithString.arg(data));
}

void tst_QVersitOrganizerImporter::testTimeZones()
{
    QFETCH(QString, tzid);
    QFETCH(QVersitDocument, timezoneSpec);
    QFETCH(QString, datetimeString);
    QFETCH(QDateTime, expected);

    QVersitDocument document(QVersitDocument::ICalendar20Type);
    document.setComponentType(QStringLiteral("VCALENDAR"));
    if (!tzid.isEmpty()) {
        document.addSubDocument(timezoneSpec);
    }
    QVersitDocument vevent(QVersitDocument::ICalendar20Type);
    vevent.setComponentType(QStringLiteral("VEVENT"));
    QVersitProperty property;
    property.setName(QStringLiteral("DTSTART"));
    property.setValue(datetimeString);
    if (!tzid.isEmpty()) {
        property.insertParameter(QStringLiteral("TZID"), tzid);
    }
    vevent.addProperty(property);
    document.addSubDocument(vevent);

    QVersitOrganizerImporter importer;
    QVERIFY(importer.importDocument(document));
    QVERIFY(importer.errorMap().isEmpty());
    QList<QOrganizerItem> items = importer.items();
    QCOMPARE(items.size(), 1);

    QOrganizerEvent event = static_cast<QOrganizerEvent>(items.first());
    QCOMPARE(event.type(), QOrganizerItemType::TypeEvent);
    QDateTime actualDatetime = event.startDateTime();
    QCOMPARE(actualDatetime, expected);
    QCOMPARE(actualDatetime.timeSpec(), expected.timeSpec());
}

void tst_QVersitOrganizerImporter::testTimeZones_data()
{
    QTest::addColumn<QString>("tzid"); // set this to empty if you don't want to associate a tzid
    QTest::addColumn<QVersitDocument>("timezoneSpec");
    QTest::addColumn<QString>("datetimeString");
    QTest::addColumn<QDateTime>("expected");

    QVersitDocument vtimezone(QVersitDocument::ICalendar20Type);
    vtimezone.setComponentType(QStringLiteral("VTIMEZONE"));
    QTest::newRow("utc") << QString() << QVersitDocument(QVersitDocument::ICalendar20Type)
        << QString::fromLatin1("20100102T030405Z")
        << QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5), Qt::UTC);

    QTest::newRow("floating") << QString() << QVersitDocument(QVersitDocument::ICalendar20Type)
        << QString::fromLatin1("20100102T030405")
        << QDateTime(QDate(2010, 1, 2), QTime(3, 4, 5), Qt::LocalTime);

    {
        QVersitDocument vtimezone(QVersitDocument::ICalendar20Type);
        vtimezone.setComponentType(QStringLiteral("VTIMEZONE"));

        QVersitProperty property;
        property.setName(QStringLiteral("TZID"));
        property.setValue(QStringLiteral("Asia/Singapore"));
        vtimezone.addProperty(property);
        property.setName(QStringLiteral("X-LIC-LOCATION"));
        property.setValue(QStringLiteral("Asia/Singapore"));
        vtimezone.addProperty(property);

        QVersitDocument standard(QVersitDocument::ICalendar20Type);
        standard.setComponentType(QStringLiteral("STANDARD"));
        property.setName(QStringLiteral("TZOFFSETFROM"));
        property.setValue(QStringLiteral("+0800"));
        standard.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETTO"));
        property.setValue(QStringLiteral("+0800"));
        standard.addProperty(property);
        property.setName(QStringLiteral("TZNAME"));
        property.setValue(QStringLiteral("EST"));
        standard.addProperty(property);
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("19700101T000000"));
        standard.addProperty(property);
        vtimezone.addSubDocument(standard);

        QTest::newRow("no dst") << QString::fromLatin1("Asia/Singapore")
            << vtimezone << QString::fromLatin1("20100102T100405")
            << QDateTime(QDate(2010, 1, 2), QTime(2, 4, 5), Qt::UTC);
    }

    {
        QVersitDocument vtimezone(QVersitDocument::ICalendar20Type);
        vtimezone.setComponentType(QStringLiteral("VTIMEZONE"));

        QVersitProperty property;
        property.setName(QStringLiteral("TZID"));
        property.setValue(QStringLiteral("Australia/Sydney"));
        vtimezone.addProperty(property);
        property.setName(QStringLiteral("X-LIC-LOCATION"));
        property.setValue(QStringLiteral("Australia/Sydney"));
        vtimezone.addProperty(property);

        QVersitDocument standard(QVersitDocument::ICalendar20Type);
        standard.setComponentType(QStringLiteral("STANDARD"));
        property.setName(QStringLiteral("TZOFFSETFROM"));
        property.setValue(QStringLiteral("+1100"));
        standard.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETTO"));
        property.setValue(QStringLiteral("+1000"));
        standard.addProperty(property);
        property.setName(QStringLiteral("TZNAME"));
        property.setValue(QStringLiteral("EST"));
        standard.addProperty(property);
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("19700405T030000"));
        standard.addProperty(property);
        property.setName(QStringLiteral("RRULE"));
        property.setValue(QStringLiteral("FREQ=YEARLY;BYMONTH=4;BYDAY=1SU"));
        standard.addProperty(property);
        vtimezone.addSubDocument(standard);

        QVersitDocument daylight(QVersitDocument::ICalendar20Type);
        daylight.setComponentType(QStringLiteral("DAYLIGHT"));
        property.setName(QStringLiteral("TZOFFSETFROM"));
        property.setValue(QStringLiteral("+1000"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETTO"));
        property.setValue(QStringLiteral("+1100"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("TZNAME"));
        property.setValue(QStringLiteral("EST"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("19701004T020000"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("RRULE"));
        property.setValue(QStringLiteral("FREQ=YEARLY;BYMONTH=10;BYDAY=1SU"));
        daylight.addProperty(property);
        vtimezone.addSubDocument(daylight);

        QTest::newRow("dst area in standard time") << QString::fromLatin1("Australia/Sydney")
            << vtimezone << QString::fromLatin1("20100502T100405")
            << QDateTime(QDate(2010, 5, 2), QTime(0, 4, 5), Qt::UTC);

        QTest::newRow("dst") << QString::fromLatin1("Australia/Sydney")
            << vtimezone << QString::fromLatin1("20100102T100405")
            << QDateTime(QDate(2010, 1, 1), QTime(23, 4, 5), Qt::UTC);
    }

    {
        QVersitDocument vtimezone(QVersitDocument::ICalendar20Type);
        vtimezone.setComponentType(QStringLiteral("VTIMEZONE"));

        QVersitProperty property;
        property.setName(QStringLiteral("TZID"));
        property.setValue(QStringLiteral("US-Eastern"));
        vtimezone.addProperty(property);

        QVersitDocument standard(QVersitDocument::ICalendar20Type);
        standard.setComponentType(QStringLiteral("STANDARD"));
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("19961026T020000"));
        standard.addProperty(property);
        property.setName(QStringLiteral("RDATE"));
        property.setValue(QStringLiteral("19971026T020000"));
        standard.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETFROM"));
        property.setValue(QStringLiteral("-0400"));
        standard.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETTO"));
        property.setValue(QStringLiteral("-0500"));
        standard.addProperty(property);
        vtimezone.addSubDocument(standard);

        QVersitDocument daylight(QVersitDocument::ICalendar20Type);
        daylight.setComponentType(QStringLiteral("DAYLIGHT"));
        property.setName(QStringLiteral("DTSTART"));
        property.setValue(QStringLiteral("19960406T020000"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("RDATE"));
        property.setValue(QStringLiteral("19970406T020000"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETFROM"));
        property.setValue(QStringLiteral("-0500"));
        daylight.addProperty(property);
        property.setName(QStringLiteral("TZOFFSETTO"));
        property.setValue(QStringLiteral("-0400"));
        daylight.addProperty(property);
        vtimezone.addSubDocument(daylight);

        QTest::newRow("dst specified with rdate - daylight") << QString::fromLatin1("US-Eastern")
            << vtimezone << QString::fromLatin1("19970615T100000")
            << QDateTime(QDate(1997, 6, 15), QTime(14, 0, 0), Qt::UTC);

        QTest::newRow("dst specified with rdate - standard") << QString::fromLatin1("US-Eastern")
            << vtimezone << QString::fromLatin1("19971215T100000")
            << QDateTime(QDate(1997, 12, 15), QTime(15, 0, 0), Qt::UTC);
    }
}

QTEST_MAIN(tst_QVersitOrganizerImporter)
