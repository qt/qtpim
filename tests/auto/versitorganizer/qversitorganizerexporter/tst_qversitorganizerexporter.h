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

#ifndef tst_QVERSITORGANIZEREXPORTER_H
#define tst_QVERSITORGANIZEREXPORTER_H

#include <QObject>
#include <QtVersitOrganizer/qversitorganizerexporter.h>
#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>
#include <QtOrganizer/qorganizer.h>

QTORGANIZER_USE_NAMESPACE
QTVERSIT_USE_NAMESPACE
QTVERSITORGANIZER_USE_NAMESPACE

class tst_QVersitOrganizerExporter : public QObject
{
    Q_OBJECT

private slots:
    void testExport();
    void testExport_data();

    void testExportError();
    void testExportError_data();

    void testExportEventDetails();
    void testExportEventDetails_data();

    void testExportTodoDetails();
    void testExportTodoDetails_data();

    void testEmptyItemShouldNotBeExported();

    void testExtendedDetail();
    void testExtendedDetail_data();
    void testMultipleExtendedDetails();
    QOrganizerItemExtendedDetail createExtendedDetail(
            const QString &name,
            const QVariant &data);
    QVersitProperty createExtendedDetailProperty(
                const QString &name,
                const QVariant &data);
    QVersitProperty createExtendedDetailPropertyForStringData(
            const QString &name,
            const QString &data);

    static QList<QVersitProperty> findPropertiesByName(const QVersitDocument &document, const QString &propertyName);
};

#endif
