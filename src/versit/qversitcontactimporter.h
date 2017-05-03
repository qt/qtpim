/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersit module of the Qt Toolkit.
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

#ifndef QVERSITCONTACTIMPORTER_H
#define QVERSITCONTACTIMPORTER_H

#include <QtCore/qlist.h>
#include <QtCore/qmap.h>

#include <QtContacts/qcontact.h>
#include <QtContacts/qcontactdetail.h>

#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitresourcehandler.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

class QVersitProperty;

class QVersitContactImporterPrivate;

class Q_VERSIT_EXPORT QVersitContactImporterPropertyHandler
{
public:
    virtual ~QVersitContactImporterPropertyHandler() {}
    virtual bool preProcessProperty(const QVersitDocument& document,
                                    const QVersitProperty& property,
                                    int contactIndex,
                                    QContact* contact) = 0;
    virtual bool postProcessProperty(const QVersitDocument& document,
                                     const QVersitProperty& property,
                                     bool alreadyProcessed,
                                     int contactIndex,
                                     QContact* contact) = 0;
};

class Q_VERSIT_EXPORT QVersitContactImporterPropertyHandlerV2
{
public:
    virtual ~QVersitContactImporterPropertyHandlerV2() {}
    virtual void propertyProcessed(const QVersitDocument& document,
                                   const QVersitProperty& property,
                                   const QContact& contact,
                                   bool *alreadyProcessed,
                                   QList<QContactDetail>* updatedDetails) = 0;
    virtual void documentProcessed(const QVersitDocument& document,
                                   QContact* contact) = 0;
};

class Q_VERSIT_EXPORT QVersitContactImporter
{
public:
    enum Error {
        NoError = 0,
        InvalidDocumentError,
        EmptyDocumentError
    };

    QVersitContactImporter();
    explicit QVersitContactImporter(const QString& profile);
    explicit QVersitContactImporter(const QStringList& profiles);
    ~QVersitContactImporter();

    bool importDocuments(const QList<QVersitDocument>& documents);
    QList<QContact> contacts() const;
    QMap<int, Error> errorMap() const;

    void setPropertyHandler(QVersitContactImporterPropertyHandlerV2* handler);

    void setResourceHandler(QVersitResourceHandler* handler);
    QVersitResourceHandler* resourceHandler() const;

    /* deprecated */
    QMap<int, Error> errors() const;
    void setPropertyHandler(QVersitContactImporterPropertyHandler* handler);
    QVersitContactImporterPropertyHandler* propertyHandler() const;

private:
    QVersitContactImporterPrivate* d;
};

QT_END_NAMESPACE_VERSIT

#endif // QVERSITCONTACTIMPORTER_H

