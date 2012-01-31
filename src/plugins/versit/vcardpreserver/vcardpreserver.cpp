/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "vcardpreserver.h"
#include <qversitproperty.h>

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

QTVERSIT_BEGIN_NAMESPACE

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
                         QSet<QString>* processedFields,
                         QList<QVersitProperty>* toBeRemoved,
                         QList<QVersitProperty>* toBeAdded);
    void contactProcessed(const QContact& contact,
                          QVersitDocument* document);
};

const QString DetailName(QStringLiteral("Custom"));
const QString KeyField(QStringLiteral("Key"));
const QString ValueField(QStringLiteral("Value"));


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

Q_EXPORT_PLUGIN2(qtversit_vcardpreserver, VCardPreserverFactory);

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
    QContactDetail detail(DetailName);
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
                                     QSet<QString>* processedFields,
                                     QList<QVersitProperty>* toBeRemoved,
                                     QList<QVersitProperty>* toBeAdded)
{
    Q_UNUSED(contact)
    Q_UNUSED(document)
    Q_UNUSED(toBeRemoved)
    if (detail.definitionName() == DetailName
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

QTVERSIT_END_NAMESPACE
