/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "backupvcardhandler.h"

#include <QtCore/qdatastream.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qvariant.h>
#include <QtCore/qurl.h>
#include <QtCore/qiodevice.h>

#include <QtContacts/qcontact.h>
#include <QtContacts/qcontactdetail.h>
#include <QtContacts/qcontactextendeddetail.h>

#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>
#include <QtVersit/private/qvcardrestorehandler_p.h>

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

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

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
                         QSet<int>* processedFields,
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
const QString DetailTypeParameter(QStringLiteral("DETAIL"));
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
    retval.insert(QVersitContactHandlerFactory::ProfileBackup());
    return retval;
}

QString BackupVCardHandlerFactory::name() const
{
    return QStringLiteral("org.qt-project.Qt.BackupVCardHandlerFactory");
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

QStringList BackupVCardHandlerFactory::keys() const
{
    return QStringList() << name();
}


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
    Q_UNUSED(document);
    Q_UNUSED(property);
    Q_UNUSED(contact);
    Q_UNUSED(alreadyProcessed);
    Q_UNUSED(updatedDetails);
}

void BackupVCardHandler::documentProcessed(
        const QVersitDocument& document,
        QContact* contact)
{
    Q_UNUSED(document);
    Q_UNUSED(contact);
}

void BackupVCardHandler::detailProcessed(
        const QContact& contact,
        const QContactDetail& detail,
        const QVersitDocument& document,
        QSet<int>* processedFields,
        QList<QVersitProperty>* toBeRemoved,
        QList<QVersitProperty>* toBeAdded)
{
    Q_UNUSED(contact);
    Q_UNUSED(document);
    Q_UNUSED(toBeRemoved);
    if (detail.accessConstraints().testFlag(QContactDetail::ReadOnly))
        return;
    QMap<int, QVariant> fields = detail.values();
    // fields from the same detail have the same group so the importer can collate them
    QString detailGroup = GroupPrefix + QString::number(mDetailNumber++);
    int toBeAddedCount = toBeAdded->count();
    bool propertiesSynthesized = false;
    for (QMap<int, QVariant>::const_iterator it = fields.constBegin(); it != fields.constEnd(); it++) {
        if (!processedFields->contains(it.key()) && !it.value().toString().isEmpty()) {
            // Generate a property for the unknown field
            QVersitProperty property;
            property.setGroups(QStringList(detailGroup));
            property.setName(PropertyName);
            property.insertParameter(DetailTypeParameter, QString::number(detail.type()));
            property.insertParameter(FieldParameter, QString::number(it.key()));

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
    if (value.metaType().id() == QMetaType::QString
        || value.metaType().id() == QMetaType::QByteArray) {
        // store QStrings and QByteArrays as-is
        property->setValue(value);
    } else if (value.metaType().id() == QMetaType::QDate) {
        // Store a QDate as a string
        QString valueString(value.toDate().toString(Qt::ISODate));
        property->insertParameter(DatatypeParameter, DatatypeParameterDate);
        property->setValue(valueString);
    } else if (value.metaType().id() == QMetaType::QTime) {
        // Store a QTime as a string
        QString valueString(value.toTime().toString(Qt::ISODate));
        property->insertParameter(DatatypeParameter, DatatypeParameterTime);
        property->setValue(valueString);
    } else if (value.metaType().id() == QMetaType::QDateTime) {
        // Store a QDateTime as a string
        QString valueString(value.toDateTime().toString(Qt::ISODate));
        property->insertParameter(DatatypeParameter, DatatypeParameterDateTime);
        property->setValue(valueString);
    } else if (value.metaType().id() == QMetaType::Bool) {
        // Store an int as a string
        QString valueString(QString::number(value.toBool() ? 1 : 0));
        property->insertParameter(DatatypeParameter, DatatypeParameterBool);
        property->setValue(valueString);
    } else if (value.metaType().id() == QMetaType::Int) {
        // Store an int as a string
        QString valueString(QString::number(value.toInt()));
        property->insertParameter(DatatypeParameter, DatatypeParameterInt);
        property->setValue(valueString);
    } else if (value.metaType().id() == QMetaType::UInt) {
        // Store a uint as a string
        QString valueString(QString::number(value.toUInt()));
        property->insertParameter(DatatypeParameter, DatatypeParameterUInt);
        property->setValue(valueString);
    } else if (value.metaType().id() == QMetaType::QUrl) {
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
    Q_UNUSED(contact);
    Q_UNUSED(document);
    mDetailNumber = 0;
}

QT_END_NAMESPACE_VERSIT

#include "moc_backupvcardhandler.cpp"
