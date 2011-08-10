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

#ifndef QVERSITREADER_H
#define QVERSITREADER_H

#include <qversitdocument.h>
#include "qversit_p.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class QIODevice;
class QTextCodec;
QT_END_NAMESPACE

QTPIM_BEGIN_NAMESPACE

class QVersitReaderPrivate;

// reads a QVersitDocument from i/o device
class Q_VERSIT_EXPORT QVersitReader : public QObject
{
    Q_OBJECT
public:
    enum Error {
        NoError = 0,
        UnspecifiedError,
        IOError,
        OutOfMemoryError,
        NotReadyError,
        ParseError
    };

    enum State {
        InactiveState = 0,
        ActiveState,
        CanceledState,
        FinishedState
    };

    QVersitReader();
    QVersitReader(QIODevice* inputDevice);
    QVersitReader(const QByteArray& inputData);
    ~QVersitReader();

    // input:
    void setDevice(QIODevice* inputDevice);
    QIODevice* device() const;
    void setData(const QByteArray& inputData);

    void setDefaultCodec(QTextCodec* codec);
    QTextCodec* defaultCodec() const;

    // output:
    QList<QVersitDocument> results() const;

    State state() const;
    Error error() const;

    // reading:
public Q_SLOTS:
    bool startReading();
    void cancel();
public:
    Q_INVOKABLE bool waitForFinished(int msec = -1);

Q_SIGNALS:
    void stateChanged(QVersitReader::State state);
    void resultsAvailable();

private: // data
    QVersitReaderPrivate* d;
};

QTPIM_END_NAMESPACE

Q_DECLARE_METATYPE(QTPIM_PREPEND_NAMESPACE(QVersitReader::State))

#endif // QVERSITREADER_H