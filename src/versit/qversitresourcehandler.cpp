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

#include "qversitresourcehandler.h"

#include <QtCore/qbytearray.h>
#include <QtCore/qfile.h>
#include <QtCore/qstring.h>

#include "qversitdefs_p.h"
#include "qversitproperty.h"

QT_BEGIN_NAMESPACE_VERSIT

/*!
  \class QVersitResourceHandler
  \brief The QVersitResourceHandler class is an interface for clients wishing to implement custom
  behaviour for loading and saving files to disk when exporting and importing.
  \ingroup versit-extension
  \inmodule QtVersit

  \sa QVersitContactImporter
  \sa QVersitContactExporter
  \sa QVersitDefaultResourceHandler
 */

/*!
  \fn virtual QVersitResourceHandler::~QVersitResourceHandler()
  Frees any memory used by the handler.
 */

/*!
  \fn virtual bool QVersitResourceHandler::saveResource(const QByteArray& contents, const QVersitProperty& property, QString* location) = 0;
  Saves the binary data \a contents to a file on a persistent storage medium.

  \a property holds the QVersitProperty which is the context in which the binary is coming from.
  The QVersitResourceHandler can use this, for example, to determine file extension it should choose.
  *\a location is filled with the contents of the file.
  Returns true on success, false on failure.
 */

/*!
 \fn virtual bool QVersitResourceHandler::loadResource(const QString& location, QByteArray* contents, QString* mimeType) = 0
 Loads a file from \a location.
 *\a contents is filled with the contents of the file and *\a mimeType is set to the MIME
 type that it is determined to be.
 Returns true on success, false on failure.
*/

/*!
  \class QVersitDefaultResourceHandler

  \brief The QVersitDefaultResourceHandler class provides a default implementation of a Versit
  resource handler.
  \ingroup versit-extension

  An example resource handler implementation:
  \snippet qtversitdocsample/qtversitdocsample.cpp Resource handler

  \sa QVersitContactImporter, QVersitContactExporter, QVersitResourceHandler
 */

class QVersitDefaultResourceHandlerPrivate {
public:
    QMap<QString,QString> mFileExtensionMapping;
};

/*!
  Constructs a QVersitDefaultResourceHandler.
 */
QVersitDefaultResourceHandler::QVersitDefaultResourceHandler()
    : d(new QVersitDefaultResourceHandlerPrivate)
{
    // File extension mappings
    int fileExtensionCount = sizeof(versitFileExtensionMappings)/sizeof(VersitFileExtensionMapping);
    for (int i = 0; i < fileExtensionCount; i++) {
        d->mFileExtensionMapping.insert(
            QLatin1String(versitFileExtensionMappings[i].extension),
            QLatin1String(versitFileExtensionMappings[i].mimeType));
    }
}

/*!
  Frees any memory used by the resource handler.
 */
QVersitDefaultResourceHandler::~QVersitDefaultResourceHandler()
{
    delete d;
}

/*!
  Default resource loader.
  Loads file from given \a location into \a contents and returns true if successful.
  Sets the \a mimeType based on the file extension.
 */
bool QVersitDefaultResourceHandler::loadResource(const QString& location,
                                                 QByteArray* contents,
                                                 QString* mimeType)
{
    QString extension = location.split(QLatin1Char('.')).last().toLower();
    *mimeType = d->mFileExtensionMapping.value(extension);
    if (location.isEmpty())
        return false;
    QFile file(location);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!file.isReadable())
        return false;
    *contents = file.readAll();
    return contents->size() > 0;
}

/*!
  Default resource saver.
  Does nothing and returns false, ignoring \a contents, \a property and \a location.  By default,
  resources aren't persisted because we don't know when it is safe to remove them.
 */
bool QVersitDefaultResourceHandler::saveResource(const QByteArray& contents,
                                                 const QVersitProperty& property,
                                                 QString* location)
{
    Q_UNUSED(contents)
    Q_UNUSED(property)
    Q_UNUSED(location)
    return false;
}

QT_END_NAMESPACE_VERSIT
