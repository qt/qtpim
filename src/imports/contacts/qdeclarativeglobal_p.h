/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#ifndef QDECLARATIVEGLOBAL_H
#define QDECLARATIVEGLOBAL_H

#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

QT_MODULE(Declarative)

#define DEFINE_BOOL_CONFIG_OPTION(name, var) \
    static bool name() \
    { \
        static enum { Yes, No, Unknown } status = Unknown; \
        if (status == Unknown) { \
            QByteArray v = qgetenv(#var); \
            bool value = !v.isEmpty() && v != "0" && v != "false"; \
            if (value) status = Yes; \
            else status = No; \
        } \
        return status == Yes; \
    }

struct QQmlGraphics_DerivedObject : public QObject
{
    void setParent_noEvent(QObject *parent) {
        bool sce = d_ptr->sendChildEvents;
        d_ptr->sendChildEvents = false;
        setParent(parent);
        d_ptr->sendChildEvents = sce;
    }
};

/*!
    Returns true if the case of \a fileName is equivalent to the file case of
    \a fileName on disk, and false otherwise.

    This is used to ensure that the behavior of QML on a case-insensitive file
    system is the same as on a case-sensitive file system.  This function
    performs a "best effort" attempt to determine the real case of the file.
    It may have false positives (say the case is correct when it isn't), but it
    should never have a false negative (say the case is incorrect when it is
    correct).
*/
bool QQml_isFileCaseCorrect(const QString &fileName);

/*!
    Makes the \a object a child of \a parent.  Note that when using this method,
    neither \a parent nor the object's previous parent (if it had one) will
    receive ChildRemoved or ChildAdded events.
*/
inline void QQml_setParent_noEvent(QObject *object, QObject *parent)
{
    static_cast<QQmlGraphics_DerivedObject *>(object)->setParent_noEvent(parent);
}

QT_END_NAMESPACE

#endif // QDECLARATIVEGLOBAL_H
