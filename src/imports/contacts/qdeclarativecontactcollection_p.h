/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#ifndef QDECLARATIVECONTACTCOLLECTION_H
#define QDECLARATIVECONTACTCOLLECTION_H

#include <QtCore/qurl.h>

#include <QtGui/qcolor.h>

#include <QtQml/qqml.h>

#include <QtContacts/qcontactcollection.h>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactCollection : public QObject
{
    Q_OBJECT

    Q_ENUMS(MetaDataKey)
    Q_PROPERTY(QString collectionId READ id WRITE setId NOTIFY valueChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY valueChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY valueChanged)
    Q_PROPERTY(QColor secondaryColor READ secondaryColor WRITE setSecondaryColor NOTIFY valueChanged)
    Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY valueChanged)

public:
    enum MetaDataKey {
        KeyName = QContactCollection::KeyName,
        KeyDescription = QContactCollection::KeyDescription,
        KeyColor = QContactCollection::KeyColor,
        KeySecondaryColor = QContactCollection::KeySecondaryColor,
        KeyImage = QContactCollection::KeyImage,
        KeyExtended = QContactCollection::KeyExtended
    };

    QDeclarativeContactCollection(QObject *parent = nullptr);

    QString id() const;
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    QColor color() const;
    void setColor(const QColor &color);

    QColor secondaryColor() const;
    void setSecondaryColor(const QColor &secondaryColor);

    QUrl image() const;
    void setImage(const QUrl &url);

    Q_INVOKABLE void setMetaData(QContactCollection::MetaDataKey key, const QVariant &value);
    Q_INVOKABLE QVariant metaData(QContactCollection::MetaDataKey key) const;

    Q_INVOKABLE void setExtendedMetaData(const QString &key, const QVariant &value);
    Q_INVOKABLE QVariant extendedMetaData(const QString &key) const;

    // used by model
    QContactCollection collection() const;
    void setCollection(const QContactCollection & collection);

Q_SIGNALS:
    void valueChanged();

private:
    QContactCollection d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactCollection)

#endif // QDECLARATIVECONTACTCOLLECTION_H
