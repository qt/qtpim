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

#include "qversitwriter_p.h"

#include <QtCore/qbuffer.h>
#include <QtCore/qstringlist.h>

#include <QTextCodec>

#include "qvcard21writer_p.h"
#include "qvcard30writer_p.h"
#include "qversitdocumentwriter_p.h"
#include "qversitutils_p.h"

QT_BEGIN_NAMESPACE_VERSIT

/*! Constructs a writer. */
QVersitWriterPrivate::QVersitWriterPrivate()
    : mIoDevice(0),
    mState(QVersitWriter::InactiveState),
    mError(QVersitWriter::NoError),
    mIsCanceling(false),
    mDefaultCodec(0)
{
}

/*! Destroys a writer. */
QVersitWriterPrivate::~QVersitWriterPrivate()
{
}

/*! Links the signals from this to the signals of \a writer. */
void QVersitWriterPrivate::init(QVersitWriter* writer)
{
    qRegisterMetaType<QVersitWriter::State>("QVersitWriter::State");
    connect(this, SIGNAL(stateChanged(QVersitWriter::State)),
            writer, SIGNAL(stateChanged(QVersitWriter::State)), Qt::DirectConnection);
}

/*!
 * Do the actual writing and set the error and state appropriately.
 */
void QVersitWriterPrivate::write()
{
    bool canceled = false;

    // Try to get the type from the parameter to startWriting...
    QVersitDocument::VersitType type = documentType();

    foreach (const QVersitDocument& document, mInput) {
        if (isCanceling()) {
            canceled = true;
            break;
        }

        // Get type from the document if not specified in startWriting
        if (type == QVersitDocument::InvalidType)
            type = document.type();

        QScopedPointer<QVersitDocumentWriter> writer(writerForType(type, document));
        QTextCodec* codec = mDefaultCodec;
        if (codec == NULL) {
            if (type == QVersitDocument::VCard21Type) {
                codec = QTextCodec::codecForName("ISO-8859-1");
                writer->setAsciiCodec();
            } else {
                codec = QTextCodec::codecForName("UTF-8");
            }
        }
        writer->setCodec(codec);
        writer->setDevice(mIoDevice);
        if (!writer->encodeVersitDocument(document)) {
            setError(QVersitWriter::IOError);
            break;
        }
    }
    if (canceled)
        setState(QVersitWriter::CanceledState);
    else
        setState(QVersitWriter::FinishedState);
}

/*!
 * Inherited from QThread, called by QThread when the thread has been started.
 */
void QVersitWriterPrivate::run()
{
    write();
}

void QVersitWriterPrivate::setState(QVersitWriter::State state)
{
    mMutex.lock();
    mState = state;
    mMutex.unlock();
    emit stateChanged(state);
}

QVersitWriter::State QVersitWriterPrivate::state() const
{
    QMutexLocker locker(&mMutex);
    return mState;
}

void QVersitWriterPrivate::setError(QVersitWriter::Error error)
{
    QMutexLocker locker(&mMutex);
    mError = error;
}

QVersitWriter::Error QVersitWriterPrivate::error() const
{
    QMutexLocker locker(&mMutex);
    return mError;
}

/*!
 * Returns a QVersitDocumentWriter that can encode a QVersitDocument of type \a type.
 * The caller is responsible for deleting the object.
 */
QVersitDocumentWriter* QVersitWriterPrivate::writerForType(QVersitDocument::VersitType type, const QVersitDocument& document)
{
    switch (type) {
        case QVersitDocument::InvalidType:
        {
            // Neither startWriting or the document provided the type.
            // Need to infer the type from the document's componentType
            QString componentType(document.componentType());
            if (componentType == QStringLiteral("VCARD")) {
                return new QVCard30Writer(QVersitDocument::VCard30Type);
            } else if (componentType == QStringLiteral("VCALENDAR")
                    || componentType == QStringLiteral("VEVENT")
                    || componentType == QStringLiteral("VTODO")
                    || componentType == QStringLiteral("VJOURNAL")
                    || componentType == QStringLiteral("VTIMEZONE")
                    || componentType == QStringLiteral("VALARM")) {
                return new QVCard30Writer(QVersitDocument::ICalendar20Type);
            } else {
                return new QVCard30Writer(QVersitDocument::VCard30Type);
            }
        }
        case QVersitDocument::VCard21Type:
            return new QVCard21Writer(type);
        case QVersitDocument::VCard30Type:
            return new QVCard30Writer(type);
        default:
            return new QVCard30Writer(type);
    }
}

void QVersitWriterPrivate::setCanceling(bool canceling)
{
    QMutexLocker locker(&mMutex);
    mIsCanceling = canceling;
}

bool QVersitWriterPrivate::isCanceling() const
{
    QMutexLocker locker(&mMutex);
    return mIsCanceling;
}

void QVersitWriterPrivate::setDocumentType(QVersitDocument::VersitType type)
{
    QMutexLocker locker(&mMutex);
    mType = type;
}

QVersitDocument::VersitType QVersitWriterPrivate::documentType() const
{
    QMutexLocker locker(&mMutex);
    return mType;
}

QT_END_NAMESPACE_VERSIT

#include "moc_qversitwriter_p.cpp"
