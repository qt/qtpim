/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef tst_QVERSITREADER_H
#define tst_QVERSITREADER_H

#include <QObject>
#include <QBuffer>
#include <QtVersit/qversitreader.h>

QT_BEGIN_NAMESPACE_VERSIT
class QVersitReaderPrivate;
class LineReader;
QT_END_NAMESPACE_VERSIT

QTVERSIT_USE_NAMESPACE

// Poor man's QSignalSpy because I couldn't get QSignalSpy to work with the user type QVR::State.
class SignalCatcher : public QObject
{
Q_OBJECT
public:
    SignalCatcher() : mResultsCount(0) {}
public slots:
    void stateChanged(QVersitReader::State state) {
        mStateChanges.append(state);
    }
    void resultsAvailable() {
        mResultsCount += 1;
    }

public:
    QList<QVersitReader::State> mStateChanges;
    int mResultsCount;
};

class tst_QVersitReader : public QObject
{
     Q_OBJECT

private slots: // Tests
    void init();
    void cleanup();

    void testDevice();
    void testNullDevice();
    void testDefaultCodec();
    void testValidateUtf8();
    void testValidateUtf8_data();
    void testDetectCodec();
    void testDetectCodec_data();
    void testReading();
    void testResult();
    void testParseNextVersitProperty();
    void testParseNextVersitProperty_data();
    void testParseVersitDocument();
    void testParseVersitDocument_data();
    void testDecodeQuotedPrintable();
    void testDecodeQuotedPrintable_data();
    void testParamName();
    void testParamValue();
    void testExtractPart();
    void testExtractParts();
    void testExtractPropertyGroupsAndName();
    void testExtractVCard21PropertyParams();
    void testExtractVCard30PropertyParams();
    void testExtractParams();
    void testReadLine();
    void testReadLine_data();
    void testByteArrayInput();
    void testRemoveBackSlashEscaping();

private: // Data
    QVersitReader* mReader;
#ifdef QT_BUILD_INTERNAL
    QVersitReaderPrivate* mReaderPrivate;
#endif
    QBuffer* mInputDevice;
    QTextCodec* mAsciiCodec;
    SignalCatcher* mSignalCatcher;
};

#endif // tst_VERSITREADER_H
