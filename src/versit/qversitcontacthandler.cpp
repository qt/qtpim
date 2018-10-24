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

#include "qversitcontacthandler.h"

QT_BEGIN_NAMESPACE_VERSIT

/*!
  \internal
 */
QStringList QVersitContactHandlerFactory::keys() const
{
    return QStringList() << name();
}

/*!
  \class QVersitContactHandler
  \brief The QVersitContactHandler class is a union of the
  QVersitContactImporterPropertyHandlerV2 and QVersitContactExporterDetailHandlerV2 interfaces.
  \ingroup versit-extension
  \inmodule QtVersit
 */

/*!
  \fn QVersitContactHandler::~QVersitContactHandler()
  This frees any memory used by the QVersitContactHandler.
 */

/*!
  \class QVersitContactHandlerFactory
  \brief The QVersitContactHandlerFactory class provides the interface for Versit plugins.
  \ingroup versit-extension
  \inmodule QtVersit

  This class provides a simple interface for the creation of QVersitContactHandler instances.
  Implement this interface to write a Versit plugin.  For more details, see
  \l{Qt Versit Plugins}.
 */

/*!
   \fn static const QString QVersitContactHandlerFactory::ProfileSync()

   The constant string signifying a plugin that is relevant to import and export in a
   synchronization context.
   \sa profiles(),
   QVersitContactImporter::QVersitContactImporter(),
   QVersitContactExporter::QVersitContactExporter()
 */

/*!
   \fn static const QString QVersitContactHandlerFactory::ProfileBackup()

   The constant string signifying a plugin that will backup a QContact to vCard, so that exporting,
   then importing a contact will not lose any data.
   \sa profiles(),
   QVersitContactImporter::QVersitContactImporter(),
   QVersitContactExporter::QVersitContactExporter()
 */

/* TODO: make this a qdoc comment in 1.2
   \variable QVersitContactHandlerFactory::ProfilePreserve

   The constant string signifying a plugin that will preserve a vCard within a QContact, so that
   importing, then exporting a vCard will not lose any data.
   \sa profiles(),
   QVersitContactImporter::QVersitContactImporter(),
   QVersitContactExporter::QVersitContactExporter()
 */

/*!
  \fn QVersitContactHandlerFactory::~QVersitContactHandlerFactory()
  This frees any memory used by the QVersitContactHandlerFactory.
 */

/*!
  \fn QSet<QString> QVersitContactHandlerFactory::profiles() const
  This function can be overridden to allow a plugin to report which profiles it is to be active
  under.  If this (as in the default case) returns the empty set, it indicates that the plugin
  should be loaded under all profiles.  If it returns a non-empty set, it will only be loaded for
  those profiles that are specified by the importer/exporter class.
 */

/*!
  \fn QString QVersitContactHandlerFactory::name() const
  This function should return a unique string that identifies the handlers provided by this factory.
  Typically, this will be of the form "org.qt-project.Qt.SampleVersitContactHandler" with the
  appropriate domain and handler name substituted.
 */

/*!
  \fn int QVersitContactHandlerFactory::index() const
  This function should return an index that helps with determining the order in which to run the
  plugins.  Plugins are run in the following order:
  \list
  \li Positively-indexed, ascending
  \li Zero-indexed
  \li Negatively-indexed, ascending
  \endlist
  For example, plugins with an index of 1 are run first and plugins of index -1 are run last.
  If more than one plugin share an index, the order of execution between them is undefined.

  By default, this returns 0, which is recommended for plugins with no special ordering
  requirements.
 */

/*!
  \fn QVersitContactHandler* QVersitContactHandlerFactory::createHandler() const
  This function is called by the Versit importer or exporter class to create an instance of the
  handler provided by this factory.
 */

QT_END_NAMESPACE_VERSIT

#include "moc_qversitcontacthandler.cpp"
