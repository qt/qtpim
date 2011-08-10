/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QVERSITDOCUMENTWRITER_H
#define QVERSITDOCUMENTWRITER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qversitproperty.h>
#include <QByteArray>
#include <QMultiHash>

QT_BEGIN_NAMESPACE
class QIODevice;
class QTextCodec;
class QTextEncoder;
QT_END_NAMESPACE

QTPIM_BEGIN_NAMESPACE

class Q_VERSIT_EXPORT QVersitDocumentWriter
{
public:
    QVersitDocumentWriter(QVersitDocument::VersitType type);
    virtual ~QVersitDocumentWriter();

    void setCodec(QTextCodec* codec);
    void setAsciiCodec();
    void setDevice(QIODevice* device);

    virtual void encodeVersitProperty(const QVersitProperty& property) = 0;
    virtual void encodeParameters(const QMultiHash<QString,QString>& parameters) = 0;
    bool encodeVersitDocument(const QVersitDocument& document, bool encodeVersion = true);
    void encodeGroupsAndName(const QVersitProperty& property);

    void writeBytes(const QByteArray& value);
    void writeString(const QString& value);
    void writeStringQp(const QString& value);
    void writeCrlf();

protected:
    QVersitDocument::VersitType mType;
    QIODevice* mDevice;
    QTextCodec* mCodec;
    bool mCodecIsAscii;
    bool mCodecIsAsciiCompatible;
    QTextEncoder* mEncoder;
    bool mSuccessful;
    int mCurrentLineLength;
};

QTPIM_END_NAMESPACE

#endif // QVERSITDOCUMENTWRITER_H