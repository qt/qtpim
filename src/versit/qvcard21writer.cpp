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

#include "qvcard21writer_p.h"

#include <QtCore/qtextcodec.h>
#include <QtCore/qvariant.h>

#include "qversitproperty.h"

#include <algorithm>

QT_BEGIN_NAMESPACE_VERSIT

/*! Constructs a writer. */
QVCard21Writer::QVCard21Writer(QVersitDocument::VersitType type) : QVersitDocumentWriter(type)
{
}

QTextEncoder* QVCard21Writer::utf8Encoder()
{
    static QTextEncoder* encoder = 0;
    if (encoder == 0) {
        encoder = QTextCodec::codecForName("UTF-8")->makeEncoder();
        // Hack so the encoder doesn't output a byte order mark
        encoder->fromUnicode(QString());
    }
    return encoder;
}

/*! Destroys a writer. */
QVCard21Writer::~QVCard21Writer()
{
}

/*!
 * Encodes the \a property and writes it to the device.
 */
void QVCard21Writer::encodeVersitProperty(const QVersitProperty& property)
{
    encodeGroupsAndName(property);
    QMultiHash<QString,QString> parameters = property.parameters();
    QVariant variant(property.variantValue());

    QString renderedValue;
    QByteArray renderedBytes;

    /* Structured values need to have their components backslash-escaped (in vCard 2.1, semicolons
       must be escaped for compound values and commas must be escaped for list values). */
    if (variant.type() == QVariant::StringList) {
        QStringList values = property.variantValue().toStringList();
        QString separator;
        if (property.valueType() == QVersitProperty::CompoundType) {
            separator = QStringLiteral(";");
        } else {
            if (property.valueType() != QVersitProperty::ListType) {
                qWarning("Variant value is a QStringList but the property's value type is neither "
                         "CompoundType or ListType");
            }
            // Assume it's a ListType
            separator = QStringLiteral(",");
        }
        QString replacement = QLatin1Char('\\') + separator;
        QRegExp separatorRegex = QRegExp(separator);

        // Check first if any of the values need to be UTF-8 encoded (if so, all of them must be
        // UTF-8 encoded)
        bool forceUtf8 = requiresUtf8(values);

        bool first = true;
        foreach (QString value, values) {
            if (!(value.isEmpty() && property.valueType() == QVersitProperty::ListType)) {
                encodeVersitValue(parameters, value, forceUtf8);
                if (!first) {
                    renderedValue += separator;
                }
                renderedValue += value.replace(separatorRegex, replacement);
                first = false;
            }
        }
    } else if (variant.type() == QVariant::String) {
        renderedValue = variant.toString();
        encodeVersitValue(parameters, renderedValue, false);
    } else if (variant.type() == QVariant::ByteArray) {
        parameters.replace(QStringLiteral("ENCODING"), QStringLiteral("BASE64"));
        if (mCodecIsAsciiCompatible) // optimize by not converting to unicode
            renderedBytes = variant.toByteArray().toBase64();
        else
            renderedValue = QLatin1String(variant.toByteArray().toBase64().data());
    }

    // Encode parameters
    encodeParameters(parameters);

    // Encode value
    writeString(QStringLiteral(":"));
    if (variant.canConvert<QVersitDocument>()) {
        writeCrlf();
        QVersitDocument embeddedDocument = variant.value<QVersitDocument>();
        encodeVersitDocument(embeddedDocument);
    } else if (variant.type() == QVariant::String || variant.type() == QVariant::StringList) {
        // Some devices don't support vCard-style line folding if the property is
        // quoted-printable-encoded.  Therefore, we use QP soft linebreaks if the property is being
        // QP-encoded, and normal vCard folding otherwise.
        if (parameters.contains(QStringLiteral("ENCODING"), QStringLiteral("QUOTED-PRINTABLE")))
            writeStringQp(renderedValue);
        else
            writeString(renderedValue);
    } else if (variant.type() == QVariant::ByteArray) {
        // One extra folding before the value and
        // one extra line break after the value are needed in vCard 2.1
        writeCrlf();
        writeString(QStringLiteral(" "));
        if (renderedBytes.isEmpty())
            writeString(renderedValue);
        else
            writeBytes(renderedBytes);
        writeCrlf();
    }
    writeCrlf();
}

/*! Returns true if and only if the current codec is incapable of encoding any of the \a values */
bool QVCard21Writer::requiresUtf8(const QStringList& values) {
    foreach (const QString& value, values) {
        if (!mCodec->canEncode(value)
            // if codec is ASCII and there is a character > U+007F in value, encode it as UTF-8
            || (mCodecIsAscii && containsNonAscii(value))) {
            return true;
        }
    }
    return false;
}

/*! Performs Quoted-Printable encoding and charset encoding on \a value as per vCard 2.1 spec.
    Returns true if the value will need to be encoded with UTF-8, false if mCodec is sufficient. */
void QVCard21Writer::encodeVersitValue(QMultiHash<QString,QString>& parameters, QString& value,
                                       bool forceUtf8)
{
    // Add the CHARSET parameter, if necessary and encode in UTF-8 later
    if (forceUtf8
            || !mCodec->canEncode(value)
            // if codec is ASCII and there is a character > U+007F in value, encode it as UTF-8
            || (mCodecIsAscii && containsNonAscii(value))) {
        parameters.replace(QStringLiteral("CHARSET"), QStringLiteral("UTF-8"));
        value = QString::fromLatin1(utf8Encoder()->fromUnicode(value));
    }

    // Quoted-Printable encode the value and add Quoted-Printable parameter, if necessary
    if (quotedPrintableEncode(value))
        parameters.replace(QStringLiteral("ENCODING"), QStringLiteral("QUOTED-PRINTABLE"));
}

int sortIndexOfTypeValue(const QString& type) {
    if (   type == QStringLiteral("CELL")
        || type == QStringLiteral("FAX")) {
        return 0;
    } else if (type == QStringLiteral("HOME")
            || type == QStringLiteral("WORK")) {
        return 1;
    } else {
        return 2;
    }
}

bool typeValueLessThan(const QString& a, const QString& b) {
    return sortIndexOfTypeValue(a) < sortIndexOfTypeValue(b);
}

/*! Ensure CELL and FAX are at the front because they are "more important" and some vCard
    parsers may ignore everything after the first TYPE */
void sortTypeValues(QStringList* values)
{
    std::sort(values->begin(), values->end(), typeValueLessThan);
}

/*!
 * Encodes the \a parameters and writes it to the device.
 */
void QVCard21Writer::encodeParameters(const QMultiHash<QString,QString>& parameters)
{
    // Sort the parameter names to yield deterministic ordering
    QList<QString> names = parameters.uniqueKeys();
    std::sort(names.begin(), names.end());
    foreach (const QString& name, names) {
        QStringList values = parameters.values(name);
        if (name == QStringLiteral("TYPE")) {
            // TYPE parameters should be sorted
            sortTypeValues(&values);
        }
        foreach (const QString& value, values) {
            writeString(QStringLiteral(";"));
            if (name.length() > 0 && name != QStringLiteral("TYPE")) {
                writeString(name);
                writeString(QStringLiteral("="));
            }
            writeString(value);
        }
    }
}

bool QVCard21Writer::containsNonAscii(const QString& str)
{
    for (int i = 0; i < str.length(); i++) {
        if (str[i].unicode() > 127)
            return true;
    }
    return false;
}

/*!
 * Encodes special characters in \a text
 * using Quoted-Printable encoding (RFC 1521).
 * Returns true if at least one character was encoded.
 */
bool QVCard21Writer::quotedPrintableEncode(QString& text)
{
    bool encoded = false;
    for (int i=0; i<text.length(); i++) {
        QChar current = text.at(i);
        if (shouldBeQuotedPrintableEncoded(current)) {
            QString encodedStr(QString::fromLatin1("=%1").
                               arg(current.unicode(), 2, 16, QLatin1Char('0')).toUpper());
            text.replace(i, 1, encodedStr);
            i += 2;
            encoded = true;
        }
    }
    return encoded;
}


/*!
 * Checks whether the \a chr should be Quoted-Printable encoded (RFC 1521).
 */
bool QVCard21Writer::shouldBeQuotedPrintableEncoded(QChar chr)
{
    int c = chr.unicode();
    return (c < 32 ||
            c == '!' || c == '"' || c == '#' || c == '$' ||
            c == '=' || c == '@' || c == '[' || c == '\\' ||
            c == ']' || c == '^' || c == '`' ||
            (c > 122 && c < 256));
}

QT_END_NAMESPACE_VERSIT
