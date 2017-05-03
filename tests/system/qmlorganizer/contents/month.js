/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Mobility Components.
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

Date.prototype.clone = function() { return new Date(this.getTime()); }

function getDayOfMonth(startDayOfMonth, offset) {
    var day = startDayOfMonth.clone();
    day.setDate(offset);
    return day.getDate();
}

function dateOfThisDay(startDayOfMonth, offset) {
    var day = startDayOfMonth.clone();
    day.setDate(offset);
    return day;
}

function getColorOfDay(startDayOfMonth, offset) {

    var newDay = startDayOfMonth.clone();
    newDay.setDate(offset);
    if (newDay.getMonth() == startDayOfMonth.getMonth())
        return "white";
    else
        return "lightgray";

}

function isToday(startDayOfMonth, offset) {

    var newDay = startDayOfMonth.clone();
    newDay.setDate(offset);
    var today = new Date();
    return newDay.toDateString() == today.toDateString();
}

