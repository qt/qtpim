/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactjsondbengine.h"

#include "qcontactjsondbenginefactory.h"
#include "qcontactjsondbid.h"

QT_BEGIN_NAMESPACE_CONTACTS

QContactJsonDbEngineFactory::QContactJsonDbEngineFactory()
{
}


QContactManagerEngine* QContactJsonDbEngineFactory::engine(const QMap<QString, QString>& parameters, QContactManager::Error* error)
{
    Q_UNUSED(error);

    return new QContactJsonDbEngine(parameters); //Manager engine will take ownership of this object.
}


QContactEngineId *QContactJsonDbEngineFactory::createContactEngineId(const QMap<QString, QString> &parameters, const QString &engineIdString) const
{
    Q_UNUSED(parameters)
    return new QContactJsonDbId(engineIdString);
}

QString QContactJsonDbEngineFactory::managerName() const
{
    return QContactJsonDbStr::contactJsonDbEngineName();
}

#include "moc_qcontactjsondbenginefactory.cpp"

QT_END_NAMESPACE_CONTACTS
