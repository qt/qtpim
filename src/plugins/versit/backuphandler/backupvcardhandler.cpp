/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QList>
#include <QString>
#include <QTextStream>
#include <QUrl>
#include "backupvcardhandler.h"
#include <qcontact.h>
#include <qcontactdetail.h>
#include <qversitdocument.h>
#include <qversitproperty.h>

/*
    When these conditions are satisfied, QStringLiteral is implemented by
    gcc's statement-expression extension.  However, in this file it will
    not work, because "statement-expressions are not allowed outside functions
    nor in template-argument lists".

    Fall back to the less-performant QLatin1String in this case.
*/
#if defined(QStringLiteral) && defined(QT_UNICODE_LITERAL_II) && defined(Q_CC_GNU) && !defined(Q_COMPILER_LAMBDA)
# undef QStringLiteral
# define QStringLiteral QLatin1String
#endif

QTVERSIT_BEGIN_NAMESPACE


/* See QVersitContactImporter::createBackupHandler() */
class BackupVCardHandler : public QVersitContactHandler
{
public:
    BackupVCardHandler();
    void propertyProcessed(const QVersitDocument& document,
                           const QVersitProperty& property,
                           const QContact& contact,
                           bool* alreadyProcessed,
                           QList<QContactDetail>* updatedDetails);
    void documentProcessed(const QVersitDocument& document,
                           QContact* contact);
    void detailProcessed(const QContact& contact,
                         const QContactDetail& detail,
                         const QVersitDocument& document,
                         QSet<QString>* processedFields,
                         QList<QVersitProperty>* toBeRemoved,
                         QList<QVersitProperty>* toBeAdded);
    void contactProcessed(const QContact& contact,
                          QVersitDocument* document);

private:
    static void serializeValue(QVersitProperty* property, const QVariant& value);
    DetailGroupMap mDetailGroupMap; // remembers which details came from which groups
    int mDetailNumber;
};

const QString PropertyName(QStringLiteral("X-NOKIA-QCONTACTFIELD"));
const QString DetailDefinitionParameter(QStringLiteral("DETAIL"));
const QString FieldParameter(QStringLiteral("FIELD"));
const QString DatatypeParameter(QStringLiteral("DATATYPE"));
const QString DatatypeParameterVariant(QStringLiteral("VARIANT"));
const QString DatatypeParameterDate(QStringLiteral("DATE"));
const QString DatatypeParameterDateTime(QStringLiteral("DATETIME"));
const QString DatatypeParameterTime(QStringLiteral("TIME"));
const QString DatatypeParameterBool(QStringLiteral("BOOL"));
const QString DatatypeParameterInt(QStringLiteral("INT"));
const QString DatatypeParameterUInt(QStringLiteral("UINT"));
const QString DatatypeParameterUrl(QStringLiteral("URL"));
const QString GroupPrefix(QStringLiteral("G"));

QSet<QString> BackupVCardHandlerFactory::profiles() const
{
    QSet<QString> retval;
    retval.insert(QVersitContactHandlerFactory::ProfileBackup);
    return retval;
}

QString BackupVCardHandlerFactory::name() const
{
    return QStringLiteral("com.nokia.qt.pim.versit.backuphandler");
}

int BackupVCardHandlerFactory::index() const
{
    // Prefer to run this plugin last.
    return -1;
}

QVersitContactHandler* BackupVCardHandlerFactory::createHandler() const
{
    return new BackupVCardHandler();
}

Q_EXPORT_PLUGIN2(qtversit_backuphandler, BackupVCardHandlerFactory);


BackupVCardHandler::BackupVCardHandler()
    : mDetailNumber(0)
{
}

void BackupVCardHandler::propertyProcessed(
        const QVersitDocument& document,
        const QVersitProperty& property,
        const QContact& contact,
        bool* alreadyProcessed,
        QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(document)
    Q_UNUSED(property)
    Q_UNUSED(contact)
    Q_UNUSED(alreadyProcessed)
    Q_UNUSED(updatedDetails)
}

void BackupVCardHandler::documentProcessed(
        const QVersitDocument& document,
        QContact* contact)
{
    Q_UNUSED(document)
    Q_UNUSED(contact)
}

void BackupVCardHandler::detailProcessed(
        const QContact& contact,
        const QContactDetail& detail,
        const QVersitDocument& document,
        QSet<QString>* processedFields,
        QList<QVersitProperty>* toBeRemoved,
        QList<QVersitProperty>* toBeAdded)
{
    Q_UNUSED(contact)
    Q_UNUSED(document)
    Q_UNUSED(toBeRemoved)
    if (detail.accessConstraints().testFlag(QContactDetail::ReadOnly))
        return;
    QVariantMap fields = detail.values();
    // fields from the same detail have the same group so the importer can collate them
    QString detailGroup = GroupPrefix + QString::number(mDetailNumber++);
    int toBeAddedCount = toBeAdded->count();
    bool propertiesSynthesized = false;
    for (QVariantMap::const_iterator it = fields.constBegin(); it != fields.constEnd(); it++) {
        if (!processedFields->contains(it.key()) && !it.value().toString().isEmpty()) {
            // Generate a property for the unknown field
            QVersitProperty property;
            property.setGroups(QStringList(detailGroup));
            property.setName(PropertyName);
            property.insertParameter(DetailDefinitionParameter, detail.definitionName());
            property.insertParameter(FieldParameter, it.key());

            serializeValue(&property, it.value());

            toBeAdded->append(property);
            propertiesSynthesized = true;
            processedFields->insert(it.key());
        }
    }
    if (propertiesSynthesized) {
        // We need to group the already-generated properties with the newly synthesized ones
        for (int i = 0; i < toBeAddedCount; i++) {
            QVersitProperty& property = (*toBeAdded)[i];
            property.setGroups(property.groups() << detailGroup);
        }
    }
}

void BackupVCardHandler::serializeValue(QVersitProperty* property, const QVariant& value)
{
    // serialize the value
    if (value.type() == QVariant::String
        || value.type() == QVariant::ByteArray) {
        // store QStrings and QByteArrays as-is
        property->setValue(value);
    } else if (value.type() == QVariant::Date) {
        // Store a QDate as a string
        QString valueString(value.toDate().toString(Qt::ISODate));
        property->insertParameter(DatatypeParameter, DatatypeParameterDate);
        property->setValue(valueString);
    } else if (value.type() == QVariant::Time) {
        // Store a QTime as a string
        QString valueString(value.toTime().toString(Qt::ISODate));
        property->insertParameter(DatatypeParameter, DatatypeParameterTime);
        property->setValue(valueString);
    } else if (value.type() == QVariant::DateTime) {
        // Store a QDateTime as a string
        QString valueString(value.toDateTime().toString(Qt::ISODate));
        property->insertParameter(DatatypeParameter, DatatypeParameterDateTime);
        property->setValue(valueString);
    } else if (value.type() == QVariant::Bool) {
        // Store an int as a string
        QString valueString(QString::number(value.toBool() ? 1 : 0));
        property->insertParameter(DatatypeParameter, DatatypeParameterBool);
        property->setValue(valueString);
    } else if (value.type() == QVariant::Int) {
        // Store an int as a string
        QString valueString(QString::number(value.toInt()));
        property->insertParameter(DatatypeParameter, DatatypeParameterInt);
        property->setValue(valueString);
    } else if (value.type() == QVariant::UInt) {
        // Store a uint as a string
        QString valueString(QString::number(value.toUInt()));
        property->insertParameter(DatatypeParameter, DatatypeParameterUInt);
        property->setValue(valueString);
    } else if (value.type() == QVariant::Url) {
        // Store a QUrl as a string
        QString valueString(value.toUrl().toString());
        property->insertParameter(DatatypeParameter, DatatypeParameterUrl);
        property->setValue(valueString);
    } else {
        // Store other types by serializing the QVariant in a QByteArray
        QByteArray valueBytes;
        QDataStream stream(&valueBytes, QIODevice::WriteOnly);
        stream << value;
        property->insertParameter(DatatypeParameter, DatatypeParameterVariant);
        property->setValue(valueBytes);
    }
}

void BackupVCardHandler::contactProcessed(
        const QContact& contact,
        QVersitDocument* document)
{
    Q_UNUSED(contact)
    Q_UNUSED(document)
    mDetailNumber = 0;
}

QTVERSIT_END_NAMESPACE
