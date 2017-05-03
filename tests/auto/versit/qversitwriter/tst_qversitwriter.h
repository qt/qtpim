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

#ifndef tst_QVERSITWRITER_H
#define tst_QVERSITWRITER_H

#include <QObject>
#include <QBuffer>
#include <QtVersit/qversitwriter.h>

QT_BEGIN_NAMESPACE_VERSIT
class QVersitWriterPrivate;
QT_END_NAMESPACE_VERSIT

QTVERSIT_USE_NAMESPACE

// Poor man's QSignalSpy because I couldn't get QSignalSpy to work with the user type QVR::State.
class SignalCatcher : public QObject
{
Q_OBJECT
public slots:
    void stateChanged(QVersitWriter::State state) {
        mReceived.append(state);
    }

public:
    QList<QVersitWriter::State> mReceived;
};

class tst_QVersitWriter : public QObject
{
     Q_OBJECT

private slots: // Tests

    void init();
    void cleanup();

    void testDevice();
    void testDefaultCodec();
    void testWritingVersions();
    void testWriting21();
    void testWriting30();
    void testByteArrayOutput();
    void testWritingDocument();
    void testWritingDocument_data();

private: // Data
    QVersitWriter* mWriter;
    QBuffer* mOutputDevice;
    SignalCatcher* mSignalCatcher;
};

#endif // tst_QVERSITWRITER_H
