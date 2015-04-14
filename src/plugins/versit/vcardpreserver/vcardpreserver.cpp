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

#include "vcardpreserver.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qvariant.h>
#include <QtCore/qurl.h>

#include <QtContacts/qcontact.h>
#include <QtContacts/qcontactdetail.h>
#include <QtContacts/qcontactextendeddetail.h>

#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>

/*
    When these conditions are satisfied, QStringLiteral is implemented by
    gcc's statement-expression extension.  However, in this file it will
    not work, because "statement-expressions are not allowed outside functions
    nor in template-argument lists".

    Fall back to the less-performant QLatin1String in this case.
*/
#if defined(QStringLiteral) && defined(QT_UNICODE_LITERAL_II) && defined(Q_CC_GNU) && !defined(Q_COMPILER_LAMBDA)
# undef QStringLiteral
# define QStringLiteral QLatin1String
#endif

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

class VCardPreserver : public QVersitContactHandler
{
public:
    VCardPreserver();
    void propertyProcessed(const QVersitDocument& document,
                           const QVersitProperty& property,
                           const QContact& contact,
                           bool* alreadyProcessed,
                           QList<QContactDetail>* updatedDetails);
    void documentProcessed(const QVersitDocument& document,
                           QContact* contact);
    void detailProcessed(const QContact& contact,
                         const QContactDetail& detail,
                         const QVersitDocument& document,
                         QSet<int>* processedFields,
                         QList<QVersitProperty>* toBeRemoved,
                         QList<QVersitProperty>* toBeAdded);
    void contactProcessed(const QContact& contact,
                          QVersitDocument* document);
};

const QContactDetail::DetailType DetailType(QContactDetail::TypeExtendedDetail);
const QContactExtendedDetail::ExtendedDetailField KeyField(QContactExtendedDetail::FieldName);
const QContactExtendedDetail::ExtendedDetailField ValueField(QContactExtendedDetail::FieldData);


QSet<QString> VCardPreserverFactory::profiles() const
{
    QSet<QString> retval;
    // TODO: use this line in 1.2 when the constant is enabled
    //retval.insert(QVersitContactHandlerFactory::ProfilePreserve);
    retval.insert(QStringLiteral("Preserve"));
    return retval;
}

QString VCardPreserverFactory::name() const
{
    return QStringLiteral("org.qt-project.Qt.VCardPreserverFactory");
}

int VCardPreserverFactory::index() const
{
    // Prefer to run this plugin last, but before the backup handler.
    return -2;
}

QVersitContactHandler* VCardPreserverFactory::createHandler() const
{
    return new VCardPreserver();
}

QStringList VCardPreserverFactory::keys() const
{
    return QStringList() << name();
}

VCardPreserver::VCardPreserver()
{
}

void VCardPreserver::propertyProcessed(const QVersitDocument& document,
                                       const QVersitProperty& property,
                                       const QContact& contact,
                                       bool* alreadyProcessed,
                                       QList<QContactDetail>* updatedDetails)
{
    Q_UNUSED(contact)
    Q_UNUSED(document)
    if (!updatedDetails->isEmpty() || *alreadyProcessed) {
        return;
    }
    QContactDetail detail(DetailType);
    detail.setValue(KeyField, property.name());
    detail.setValue(ValueField, property.value());
    updatedDetails->append(detail);
    *alreadyProcessed = true;
}

void VCardPreserver::documentProcessed(const QVersitDocument& document,
                                       QContact* contact)
{
    Q_UNUSED(document)
    Q_UNUSED(contact)
}

void VCardPreserver::detailProcessed(const QContact& contact,
                                     const QContactDetail& detail,
                                     const QVersitDocument& document,
                                     QSet<int>* processedFields,
                                     QList<QVersitProperty>* toBeRemoved,
                                     QList<QVersitProperty>* toBeAdded)
{
    Q_UNUSED(contact)
    Q_UNUSED(document)
    Q_UNUSED(toBeRemoved)
    if (detail.type() == DetailType
            && processedFields->isEmpty()) {
        QString key(detail.value(KeyField).toString());
        QString value(detail.value(ValueField).toString());
        if (!key.isEmpty() && !value.isEmpty()) {
            QVersitProperty property;
            property.setName(key);
            property.setValue(value);
            toBeAdded->append(property);
        }
        processedFields->insert(KeyField);
        processedFields->insert(ValueField);
    }
}

void VCardPreserver::contactProcessed(const QContact& contact,
                                      QVersitDocument* document)
{
    Q_UNUSED(contact)
    Q_UNUSED(document)
}

#include "moc_vcardpreserver.cpp"

QT_END_NAMESPACE_VERSIT
