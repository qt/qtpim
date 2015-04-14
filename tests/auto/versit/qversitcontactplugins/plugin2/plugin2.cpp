/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QtPlugin>
#include "plugin2.h"

QTVERSIT_USE_NAMESPACE

class ContactHandler2 : public QVersitContactHandler
{
public:
    void propertyProcessed(const QVersitDocument& document,
                           const QVersitProperty& property,
                           const QContact& contact,
                           bool* alreadyProcessed,
                           QList<QContactDetail>* updatedDetails)
    {
        Q_UNUSED(document)
        Q_UNUSED(property)
        Q_UNUSED(contact)
        Q_UNUSED(alreadyProcessed)
        Q_UNUSED(updatedDetails)
    }
    void documentProcessed(const QVersitDocument& document,
                           QContact* contact)
    {
        Q_UNUSED(document)
        QContactDetail detail(QContactDetail::TypeExtendedDetail);
        detail.setValue(0, 2);
        contact->saveDetail(&detail);
    }
    void detailProcessed(const QContact& contact,
                         const QContactDetail& detail,
                         const QVersitDocument& document,
                         QSet<int>* processedFields,
                         QList<QVersitProperty>* toBeRemoved,
                         QList<QVersitProperty>* toBeAdded)
    {
        Q_UNUSED(contact)
        Q_UNUSED(detail)
        Q_UNUSED(document)
        Q_UNUSED(processedFields)
        Q_UNUSED(toBeRemoved)
        Q_UNUSED(toBeAdded)
    }
    void contactProcessed(const QContact& contact,
                          QVersitDocument* document)
    {
        Q_UNUSED(contact)
        QVersitProperty property;
        property.setName("TEST-PROPERTY");
        property.setValue("2");
        document->addProperty(property);
    }
};

QString Plugin2::name() const
{
    return QStringLiteral("org.qt-project.Qt.QVersitContactPluginsTest.Plugin2");
}

int Plugin2::index() const
{
    return 4;
}

QVersitContactHandler* Plugin2::createHandler() const
{
    return new ContactHandler2;
}

QSet<QString> Plugin2::profiles() const
{
    return QSet<QString>() << "Test";
}
