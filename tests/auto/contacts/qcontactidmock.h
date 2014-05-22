/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
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


#ifndef QCONTACTIDMOCK_H
#define QCONTACTIDMOCK_H

#include <QtContacts/qcontacts.h>
#include <QtContacts/qcontactengineid.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

QTCONTACTS_USE_NAMESPACE

class QContactIdMock : public QContactEngineId
{
public:
    QContactIdMock(const QString& managerUri, uint id) : m_managerUri(managerUri), m_id(id) {}

    bool isEqualTo(const QContactEngineId* other) const {
        return m_id == static_cast<const QContactIdMock*>(other)->m_id;
    }

    bool isLessThan(const QContactEngineId* other) const {
        return m_id < static_cast<const QContactIdMock*>(other)->m_id;
    }

    QString localId() const {
        return QString::number(m_id);
    }

    QString managerUri() const {
        return "qtcontacts:" + m_managerUri;
    }

    QContactEngineId* clone() const {
        QContactIdMock* cloned = new QContactIdMock(m_managerUri, m_id);
        return cloned;
    }

    QDebug& debugStreamOut(QDebug& dbg) const {
        return dbg << m_managerUri << m_id;
    }

    QString toString() const {
        return m_managerUri + QString("::") + QString::number(m_id);
    }

    uint hash() const {
        return qHash(toString());
    }

    static QContactId createId(const QString& managerUri, uint id)
    {
        return QContactId(new QContactIdMock(managerUri, id));
    }

private:
    QString m_managerUri;
    uint m_id;
};

#endif
