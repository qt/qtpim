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

#ifndef QDECLARATIVECONTACTAVATAR_H
#define QDECLARATIVECONTACTAVATAR_H

#include <QtContacts/qcontactavatar.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class  QDeclarativeContactAvatar : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY valueChanged)
    Q_PROPERTY(QUrl videoUrl READ videoUrl WRITE setVideoUrl NOTIFY valueChanged)
    Q_ENUMS(FieldType)
public:
    enum FieldType {
        ImageUrl = QContactAvatar::FieldImageUrl,
        VideoUrl = QContactAvatar::FieldVideoUrl
    };
    QDeclarativeContactAvatar(QObject* parent = nullptr)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactAvatar());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    void setImageUrl(const QUrl& v)
    {
        if (!readOnly() && v != imageUrl()) {
            detail().setValue(QContactAvatar::FieldImageUrl, v);
            emit valueChanged();
        }
    }
    QUrl imageUrl() const {return detail().value<QUrl>(QContactAvatar::FieldImageUrl);}

    void setVideoUrl(const QUrl& v)
    {
        if (!readOnly() && v != videoUrl()) {
            detail().setValue(QContactAvatar::FieldVideoUrl, v);
            emit valueChanged();
        }
    }
    QUrl videoUrl() const {return detail().value<QUrl>(QContactAvatar::FieldVideoUrl);}

    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Avatar;
    }

signals:
    void valueChanged();

};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactAvatar)

#endif // QDECLARATIVECONTACTAVATAR_H
