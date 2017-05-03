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

#ifndef QVERSITCONTACTEXPORTER_P_H
#define QVERSITCONTACTEXPORTER_P_H

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

#include <QtCore/qhash.h>
#include <QtCore/qlist.h>
#include <QtCore/qmap.h>

#include <QtVersit/qversitdocument.h>
#include <QtVersit/qversitproperty.h>

QT_BEGIN_NAMESPACE_CONTACTS
class QContact;
class QContactDetail;
QT_END_NAMESPACE_CONTACTS

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE_VERSIT

class QVersitContactHandler;

class QVersitContactExporterPrivate
{
public:
    QVersitContactExporterPrivate(const QStringList& profiles = QStringList());
    ~QVersitContactExporterPrivate();

    void exportContact(const QContact& contact, QVersitDocument& versitDocument);

protected:
    static void ensureDocumentContainsName(QVersitDocument* document);
    void encodeName(const QContactDetail& detail,
            const QVersitDocument& document,
            QList<QVersitProperty>* removedProperties,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodePhoneNumber(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeEmail(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeAddress(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeUrl(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeUid(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeRev(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeVersion(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeBirthDay(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeNote(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeGeoLocation(
            const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int>* processedFields);
    void encodeOrganization(const QContactDetail& detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeRingtone(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeAvatar(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeGender(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeNickname(const QContactDetail &detail,
            const QVersitDocument& document,
            QList<QVersitProperty>* removedProperties,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeTag(const QContactDetail &detail,
            const QVersitDocument& document,
            QList<QVersitProperty>* removedProperties,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeAnniversary(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeOnlineAccount(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeFamily(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeFavorite(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);
    void encodeDisplayLabel(
            const QContactDetail &detail,
            const QVersitDocument& document,
            QList<QVersitProperty>* removedProperties,
            QList<QVersitProperty>* generatedProperties,
            QSet<int>* processedFields);
    void encodeExtendedDetail(const QContactDetail &detail,
            QList<QVersitProperty>* generatedProperties,
            QSet<int> *processedFields);

    bool isValidRemoteUrl(const QString& resourceIdentifier);
    void encodeParameters(QVersitProperty& property,
        const QContactDetail::DetailType detailType,
        const QList<int> &contexts,
        const QList<int> &subTypes=QList<int>());
    bool encodeContentFromFile(const QString& resourcePath, QVersitProperty& property);

public: // Data
    QList<QVersitDocument> mDocuments;
    QMap<int, QVersitContactExporter::Error> mErrors;
    QVersitContactExporterDetailHandler* mDetailHandler;
    QVersitContactExporterDetailHandlerV2* mDetailHandler2;
    QList<QVersitContactHandler*> mPluginDetailHandlers;
    int mDetailHandlerVersion;
    QVersitDefaultResourceHandler* mDefaultResourceHandler;
    QVersitResourceHandler* mResourceHandler;

    // detail type -> <field type, versit property name>:
    QMap<QContactDetail::DetailType, QPair<int, QString> > mPropertyMappings;
    QMap< QPair<QContactDetail::DetailType, int>, QString> mSubTypeMappings;
    QHash< int ,QString> mContextMappings;
};

QT_END_NAMESPACE_VERSIT

#endif // QVERSITCONTACTEXPORTER_P_H
