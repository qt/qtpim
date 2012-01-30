/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#ifndef QDECLARATIVEORGANIZERCOLLECTION_H
#define QDECLARATIVEORGANIZERCOLLECTION_H

#include <qorganizercollection.h>
#include <QtCore/qurl.h>
#include <QtQml/qqml.h>
#include <QtGui/qcolor.h>

QTORGANIZER_BEGIN_NAMESPACE

class QDeclarativeOrganizerCollection : public QObject
{
    Q_OBJECT

    Q_ENUMS(MetaDataKey)
    Q_PROPERTY(QString collectionId READ id WRITE setId NOTIFY valueChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY valueChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY valueChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY valueChanged)
    Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY valueChanged)

public:
    enum MetaDataKey {
        KeyName = QOrganizerCollection::KeyName,
        KeyDescription = QOrganizerCollection::KeyDescription,
        KeyColor = QOrganizerCollection::KeyColor,
        KeyImage = QOrganizerCollection::KeyImage,
        KeyExtended = QOrganizerCollection::KeyExtended
    };

    QDeclarativeOrganizerCollection(QObject *parent = 0);

    QString id() const;
    void setId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &description);

    QColor color() const;
    void setColor(const QColor &color);

    QUrl image() const;
    void setImage(const QUrl &url);

    Q_INVOKABLE void setMetaData(QOrganizerCollection::MetaDataKey key, const QVariant &value);
    Q_INVOKABLE QVariant metaData(QOrganizerCollection::MetaDataKey key) const;

    Q_INVOKABLE void setExtendedMetaData(const QString &key, const QVariant &value);
    Q_INVOKABLE QVariant extendedMetaData(const QString &key) const;

    // used by model
    QOrganizerCollection collection() const;
    void setCollection(const QOrganizerCollection & collection);

    // obsolete
    Q_INVOKABLE void setMetaData(const QString &key, const QVariant &value);
    Q_INVOKABLE QVariant metaData(const QString &key) const;

Q_SIGNALS:
    void valueChanged();

private:
    QOrganizerCollection d;
};

QTORGANIZER_END_NAMESPACE

QML_DECLARE_TYPE(QTORGANIZER_PREPEND_NAMESPACE(QDeclarativeOrganizerCollection))

#endif // QDECLARATIVEORGANIZERCOLLECTION_H
