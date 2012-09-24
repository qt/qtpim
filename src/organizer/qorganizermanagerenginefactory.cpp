/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qorganizermanagerenginefactory.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerManagerEngineFactory
    \brief The QOrganizerManagerEngineFactory class provides the interface to implement the creation
           of organizer manager engine instances.
    \inmodule QtOrganizer
    \ingroup organizer-backends

    This class should only be used by backend developers. All the functions are only called internally
    by QOrganizerManager, and should not be called by others.

    More information on writing a organizer engine plugin is available in the \l{Qt Organizer Manager Engines}
    documentation.

    \sa QOrganizerManager, QOrganizerManagerEngine
 */

/*!
    A default, empty destructor.
 */
QOrganizerManagerEngineFactory::~QOrganizerManagerEngineFactory()
{
}

/*!
    \fn QOrganizerManagerEngineFactory::engine(const QMap<QString, QString> &parameters, QOrganizerManager::Error *error)

    This function should return an instance of the engine provided by this factory.

    The \a parameters supplied can be ignored or interpreted as desired.

    If a supplied parameter results in an unfulfillable request, or some other error occurs, this
    function may return a null pointer, and the client developer will get an invalid QOrganizerManager
    in return. Any error should be saved in \a error.
 */

/*!
    \fn QOrganizerManagerEngineFactory::managerName() const

    This function should return a unique string that identifies the engines provided by this factory.

    Typically this would be of the form "org.qt-project.Qt.SampleOrganizerEngine", with the appropriate
    domain and engine name substituted.
 */

/*!
    \fn QOrganizerManagerEngineFactory::createItemEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const

    This function should return an engine-specific item ID, according to the given \a parameters and
    the \a engineIdString.
 */

/*!
    \fn QOrganizerManagerEngineFactory::createCollectionEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const


    This function should return an engine-specific collection ID, according to the given \a parameters
    and the \a engineIdString.
 */

/*!
    \internal
 */
QStringList QOrganizerManagerEngineFactory::keys() const
{
    return QStringList() << managerName();
}

#include "moc_qorganizermanagerenginefactory.cpp"

QTORGANIZER_END_NAMESPACE
