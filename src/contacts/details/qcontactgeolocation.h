/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QCONTACTGEOLOCATION_H
#define QCONTACTGEOLOCATION_H

#include <QString>

#include <qcontactsglobal.h>
#include <qcontactdetail.h>
#include <qcontact.h>

QTCONTACTS_BEGIN_NAMESPACE

/* Leaf class */
// replaces the below
class Q_CONTACTS_EXPORT QContactGeoLocation : public QContactDetail
{
public:
#ifndef Q_QDOC
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactGeoLocation)
#else
    static const DetailType Type;
#endif

    enum GeoLocationField {
        FieldLabel = 0,
        FieldLatitude,
        FieldLongitude,
        FieldAccuracy,
        FieldAltitude,
        FieldAltitudeAccuracy,
        FieldHeading,
        FieldSpeed,
        FieldTimestamp
    };

    void setLabel(const QString& label) {setValue(FieldLabel, label);}
    QString label() const {return value(FieldLabel).toString();}
    void setLatitude(double latitude) {setValue(FieldLatitude, latitude);}
    double latitude() const {return value(FieldLatitude).toDouble();}
    void setLongitude(double longitude) {setValue(FieldLongitude, longitude);}
    double longitude() const {return value(FieldLongitude).toDouble();}
    void setAccuracy(double accuracy) {setValue(FieldAccuracy, accuracy);}
    double accuracy() const {return value(FieldAccuracy).toDouble();}
    void setAltitude(double altitude) {setValue(FieldAltitude, altitude);}
    double altitude() const {return value(FieldAltitude).toDouble();}
    void setAltitudeAccuracy(double altitudeAccuracy) {setValue(FieldAltitudeAccuracy, altitudeAccuracy);}
    double altitudeAccuracy() const {return value(FieldAltitudeAccuracy).toDouble();}
    void setHeading(double heading) {setValue(FieldHeading, heading);}
    double heading() const {return value(FieldHeading).toDouble();}
    void setSpeed(double speed) {setValue(FieldSpeed, speed);}
    double speed() const {return value(FieldSpeed).toDouble();}
    void setTimestamp(const QDateTime& timestamp) {setValue(FieldTimestamp, timestamp);}
    QDateTime timestamp() const {return value(FieldTimestamp).toDateTime();}
};

QTCONTACTS_END_NAMESPACE

#endif

