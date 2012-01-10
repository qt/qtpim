/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
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
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactGeoLocation, "GeoLocation")
    const static QString FieldLabel;
    const static QString FieldLatitude;
    const static QString FieldLongitude;
    const static QString FieldAccuracy;
    const static QString FieldAltitude;
    const static QString FieldAltitudeAccuracy;
    const static QString FieldHeading;
    const static QString FieldSpeed;
    const static QString FieldTimestamp;

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

