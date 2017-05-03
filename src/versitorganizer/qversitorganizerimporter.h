/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtVersitOrganizer module of the Qt Toolkit.
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

#ifndef QVERSITORGANIZERIMPORTER_H
#define QVERSITORGANIZERIMPORTER_H

#include <QtCore/qlist.h>
#include <QtCore/qmap.h>

#include <QtOrganizer/qorganizeritem.h>
#include <QtOrganizer/qorganizeritemdetail.h>

#include <QtVersit/qversitglobal.h>

#include <QtVersitOrganizer/qversitorganizerglobal.h>

QT_BEGIN_NAMESPACE_VERSIT
class QVersitDocument;
class QVersitProperty;
QT_END_NAMESPACE_VERSIT

QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSITORGANIZER

class QVersitOrganizerImporterPrivate;

class Q_VERSIT_ORGANIZER_EXPORT QVersitOrganizerImporterPropertyHandler
{
public:
    virtual ~QVersitOrganizerImporterPropertyHandler() {}
    virtual void propertyProcessed(const QVersitDocument& document,
                                   const QVersitProperty& property,
                                   const QOrganizerItem& item,
                                   bool *alreadyProcessed,
                                   QList<QOrganizerItemDetail>* updatedDetails) = 0;
    virtual void subDocumentProcessed(const QVersitDocument& topLevel,
                                      const QVersitDocument& subDocument,
                                      QOrganizerItem* item) = 0;
};

class Q_VERSIT_ORGANIZER_EXPORT QVersitOrganizerImporter
{
public:
    enum Error {
        NoError = 0,
        InvalidDocumentError,
        EmptyDocumentError
    };

    QVersitOrganizerImporter();
    explicit QVersitOrganizerImporter(const QString& profile);
    ~QVersitOrganizerImporter();

    bool importDocument(const QVersitDocument& document);
    QList<QOrganizerItem> items() const;
    QMap<int, Error> errorMap() const;

    void setPropertyHandler(QVersitOrganizerImporterPropertyHandler* handler);

private:
    QVersitOrganizerImporterPrivate* d;
};

QT_END_NAMESPACE_VERSITORGANIZER

#endif // QVERSITORGANIZERIMPORTER_H
