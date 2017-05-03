/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtPIM module of the Qt Toolkit.
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

#ifndef QVERSITGLOBAL_H
#define QVERSITGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QT_NAMESPACE)
#  define QTVERSIT_PREPEND_NAMESPACE(name) ::QT_NAMESPACE::QtVersit::name
#  define QT_BEGIN_NAMESPACE_VERSIT namespace QT_NAMESPACE { namespace QtVersit {
#  define QT_END_NAMESPACE_VERSIT } }
#  define QTVERSIT_USE_NAMESPACE using namespace QT_NAMESPACE; using namespace QtVersit;
#else
#  define QTVERSIT_PREPEND_NAMESPACE(name) ::QtVersit::name
#  define QT_BEGIN_NAMESPACE_VERSIT namespace QtVersit {
#  define QT_END_NAMESPACE_VERSIT }
#  define QTVERSIT_USE_NAMESPACE using namespace QtVersit;
#endif

#ifndef QT_STATIC
#  if defined(QT_BUILD_VERSIT_LIB)
#    define Q_VERSIT_EXPORT Q_DECL_EXPORT
#  else
#    define Q_VERSIT_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_VERSIT_EXPORT
#endif

QT_BEGIN_NAMESPACE_VERSIT
QT_END_NAMESPACE_VERSIT

#endif // QVERSITGLOBAL_H
