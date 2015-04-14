/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#include "qvcard30writer_p.h"

#include <QtCore/qbuffer.h>
#include <QtCore/qtextcodec.h>
#include <QtCore/qvariant.h>

#include "qversitproperty.h"
#include "qversitutils_p.h"

#include <algorithm>

QT_BEGIN_NAMESPACE_VERSIT

/*! Constructs a writer. */
QVCard30Writer::QVCard30Writer(QVersitDocument::VersitType type) : QVersitDocumentWriter(type)
{
    mPropertyNameMappings.insert(
        QStringLiteral("X-NICKNAME"),QStringLiteral("NICKNAME"));
    mPropertyNameMappings.insert(
        QStringLiteral("X-IMPP"),QStringLiteral("IMPP"));
}

/*! Destroys a writer. */
QVCard30Writer::~QVCard30Writer()
{
}

/*!
 * Encodes the \a property and writes it to the device.
 */
void QVCard30Writer::encodeVersitProperty(const QVersitProperty& property)
{
    QVersitProperty modifiedProperty(property);
    QString name = mPropertyNameMappings.value(property.name(),property.name());
    modifiedProperty.setName(name);
    encodeGroupsAndName(modifiedProperty);

    QVariant variant(modifiedProperty.variantValue());
    if (variant.type() == QVariant::ByteArray) {
        modifiedProperty.insertParameter(QStringLiteral("ENCODING"), QStringLiteral("b"));
    }
    encodeParameters(modifiedProperty.parameters());
    writeString(QStringLiteral(":"));

    QString renderedValue;
    QByteArray renderedBytes;
    if (variant.canConvert<QVersitDocument>()) {
        QVersitDocument embeddedDocument = variant.value<QVersitDocument>();
        QByteArray data;
        QBuffer buffer(&data);
        buffer.open(QIODevice::WriteOnly);
        QVCard30Writer subWriter(mType);
        subWriter.setCodec(mCodec);
        subWriter.setDevice(&buffer);
        subWriter.encodeVersitDocument(embeddedDocument);
        QString documentString(mCodec->toUnicode(data));
        backSlashEscape(&documentString);
        renderedValue = documentString;
    } else if (variant.type() == QVariant::String) {
        renderedValue = variant.toString();
        if (property.valueType() != QVersitProperty::PreformattedType) {
            backSlashEscape(&renderedValue);
        }
    } else if (variant.type() == QVariant::StringList) {
        // We need to backslash escape and concatenate the values in the list
        QStringList values = property.variantValue().toStringList();
        QString separator;
        if (property.valueType() == QVersitProperty::CompoundType) {
            separator = QStringLiteral(";");
        } else {
            if (property.valueType() != QVersitProperty::ListType) {
                qWarning("Property: %s, variant value is a QStringList but the property's value type is neither "
                         "CompoundType or ListType", property.name().toLatin1().constData());
            }
            // Assume it's a ListType
            separator = QStringLiteral(",");
        }
        bool first = true;
        foreach (QString value, values) {
            if (!(value.isEmpty() && property.valueType() == QVersitProperty::ListType)) {
                if (!first) {
                    renderedValue += separator;
                }
                backSlashEscape(&value);
                renderedValue += value;
                first = false;
            }
        }
    } else if (variant.type() == QVariant::ByteArray) {
        if (mCodecIsAsciiCompatible) // optimize by not converting to unicode
            renderedBytes = variant.toByteArray().toBase64();
        else
            renderedValue = QLatin1String(variant.toByteArray().toBase64().data());
    }

    if (renderedBytes.isEmpty())
        writeString(renderedValue);
    else
        writeBytes(renderedBytes);
    writeCrlf();
}

/*!
 * Encodes the \a parameters and writes it to the device.
 */
void QVCard30Writer::encodeParameters(const QMultiHash<QString,QString>& parameters)
{
    // Sort the parameter names to yield deterministic ordering
    QList<QString> names = parameters.uniqueKeys();
    std::sort(names.begin(), names.end());
    foreach (QString nameString, names) {
        writeString(QStringLiteral(";"));
        QStringList values = parameters.values(nameString);
        backSlashEscape(&nameString);
        writeString(nameString);
        writeString(QStringLiteral("="));
        for (int i=0; i<values.size(); i++) {
            if (i > 0)
                writeString(QStringLiteral(","));
            QString value = values.at(i);

            backSlashEscape(&value);
            writeString(value);
        }
    }
}


/*!
 * Performs backslash escaping for line breaks (CRLFs), semicolons, backslashes and commas according
 * to RFC 2426.  This is called on parameter names and values and property values.
 * Colons ARE NOT escaped because the examples in RFC2426 suggest that they shouldn't be.
 */
void QVCard30Writer::backSlashEscape(QString* text)
{
    static const QString m1(QStringLiteral("([;,\\\\])"));
    static const QString r1(QStringLiteral("\\\\1"));
    static const QString m2(QStringLiteral("\r\n|\r|\n"));
    static const QString r2(QStringLiteral("\\n"));
    /* replaces ; with \;
                , with \,
                \ with \\
     */
    text->replace(QRegExp(m1), r1);
    // replaces any CRLFs with \n
    text->replace(QRegExp(m2), r2);
}

QT_END_NAMESPACE_VERSIT
