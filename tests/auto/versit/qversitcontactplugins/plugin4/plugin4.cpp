/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtPlugin>
#include "plugin4.h"

QTVERSIT_USE_NAMESPACE

class ContactHandler4 : public QVersitContactHandler
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
        detail.setValue(0, 4);
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
        property.setValue("4");
        document->addProperty(property);
    }
};

QString Plugin4::name() const
{
    return QStringLiteral("org.qt-project.Qt.QVersitContactPluginsTest.Plugin4");
}

int Plugin4::index() const
{
    return 2;
}

QVersitContactHandler* Plugin4::createHandler() const
{
    return new ContactHandler4;
}

QSet<QString> Plugin4::profiles() const
{
    return QSet<QString>() << "Test";
}
