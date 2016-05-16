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

#ifndef QDECLARATIVECONTACTRINGTONE_H
#define QDECLARATIVECONTACTRINGTONE_H

#include <QtContacts/qcontactringtone.h>

#include "qdeclarativecontactdetail_p.h"

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

class QDeclarativeContactRingtone : public QDeclarativeContactDetail
{
    Q_OBJECT

    Q_PROPERTY(QUrl audioRingtoneUrl READ audioRingtoneUrl WRITE setAudioRingtoneUrl NOTIFY valueChanged)
    Q_PROPERTY(QUrl videoRingtoneUrl READ videoRingtoneUrl WRITE setVideoRingtoneUrl NOTIFY valueChanged)
    Q_PROPERTY(QUrl vibrationRingtoneUrl READ vibrationRingtoneUrl WRITE setVibrationRingtoneUrl NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_CLASSINFO("DefaultProperty", "audioRingtoneUrl")
public:
    enum FieldType {
        AudioRingtoneUrl = QContactRingtone::FieldAudioRingtoneUrl,
        VideoRingtoneUrl = QContactRingtone::FieldVideoRingtoneUrl,
        VibrationRingtoneUrl = QContactRingtone::FieldVibrationRingtoneUrl
    };

    QDeclarativeContactRingtone(QObject* parent = Q_NULLPTR)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactRingtone());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }
    DetailType detailType() const
    {
        return QDeclarativeContactDetail::Ringtone;
    }

    void setAudioRingtoneUrl(const QUrl& v)
    {
        if (!readOnly() && v != audioRingtoneUrl()) {
            detail().setValue(QContactRingtone::FieldAudioRingtoneUrl, v);
            emit valueChanged();
        }
    }
    QUrl audioRingtoneUrl() const {return detail().value<QUrl>(QContactRingtone::FieldAudioRingtoneUrl);}

    void setVideoRingtoneUrl(const QUrl& v)
    {
        if (!readOnly() && v != videoRingtoneUrl()) {
            detail().setValue(QContactRingtone::FieldVideoRingtoneUrl, v);
            emit valueChanged();
        }
    }
    QUrl videoRingtoneUrl() const {return detail().value<QUrl>(QContactRingtone::FieldVideoRingtoneUrl);}

    void setVibrationRingtoneUrl(const QUrl& v)
    {
        if (!readOnly() && v != vibrationRingtoneUrl()) {
            detail().setValue(QContactRingtone::FieldVibrationRingtoneUrl, v);
            emit valueChanged();
        }
    }
    QUrl vibrationRingtoneUrl() const {return detail().value<QUrl>(QContactRingtone::FieldVibrationRingtoneUrl);}
signals:
    void valueChanged();

};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QDeclarativeContactRingtone)

#endif // QDECLARATIVECONTACTRINGTONE_H
