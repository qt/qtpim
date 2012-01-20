/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtPIM module of the Qt Toolkit.
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

#ifndef QVERSITORGANIZERGLOBAL_H
#define QVERSITORGANIZERGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT_NAMESPACE)
#  define QTVERSITORGANIZER_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::QtVersitOrganizer::name
#  define QTVERSITORGANIZER_BEGIN_NAMESPACE namespace QT_NAMESPACE { namespace QtVersitOrganizer {
#  define QTVERSITORGANIZER_END_NAMESPACE } }
#  define QTVERSITORGANIZER_USE_NAMESPACE using namespace QT_NAMESPACE; using namespace QtVersitOrganizer;
#else
#  define QTVERSITORGANIZER_PREPEND_NAMESPACE(name) ::QtVersitOrganizer::name
#  define QTVERSITORGANIZER_BEGIN_NAMESPACE namespace QtVersitOrganizer {
#  define QTVERSITORGANIZER_END_NAMESPACE }
#  define QTVERSITORGANIZER_USE_NAMESPACE using namespace QtVersitOrganizer;
#endif


#if defined(Q_OS_WIN)
#  if defined(QT_NODLL)
#    undef QT_MAKEDLL
#    undef QT_DLL
#  elif defined(QT_MAKEDLL)
#    if defined(QT_DLL)
#      undef QT_DLL
#    endif
#    if defined(QT_BUILD_VERSITORGANIZER_LIB)
#      define Q_VERSIT_ORGANIZER_EXPORT Q_DECL_EXPORT
#    else
#      define Q_VERSIT_ORGANIZER_EXPORT Q_DECL_IMPORT
#    endif
#  elif defined(QT_DLL)
#    define Q_VERSIT_ORGANIZER_EXPORT Q_DECL_EXPORT
#  endif
#endif

#if !defined(Q_VERSIT_ORGANIZER_EXPORT)
#  if defined(QT_SHARED)
#    define Q_VERSIT_ORGANIZER_EXPORT Q_DECL_EXPORT
#  else
#    define Q_VERSIT_ORGANIZER_EXPORT
#  endif
#endif

#endif // QVERSITORGANIZERGLOBAL_H
