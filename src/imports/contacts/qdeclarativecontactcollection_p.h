/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

    QDeclarativeContactCollection(QObject *parent = Q_NULLPTR);

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
