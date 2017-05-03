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

function changeDate() {
   //TODO
}

function changeToday() {
    yearList.currentIndex = timelineView.year - yearModel.start;
    monthList.currentIndex = timelineView.month;
    dayList.positionViewAtIndex(timelineView.day, ListView.Center);
    dayList.currentIndex = timelineView.day;
}
function extendYearModel(init) {

    var start = yearModel.start;
    var end = yearModel.end;
    var now = new Date();
    var year = 1900 + now.getYear();

    if (init) {
        //initializes the year model
        if (yearModel.count == 1) {
            yearModel.set(0, {"year" : year});
            start = year;
            end = year;
        }
    }

    if (start == 0) return;

    //extends forward
    if (yearList.currentIndex == yearList.count - 1) {
        for (var i = 0; i < 10; i ++) {
            end++;
            yearModel.append({"year" : end});
        }
    }


    //extends backward
    if (yearList.currentIndex == 0) {
        for (var i = 0; i < 10; i ++) {
            start--;
            if (start == 1900)
                 break;
            yearModel.insert(1, {"year" : start});
        }
        yearModel.move(0, 10, 1);
    }
    yearModel.start = start;
    yearModel.end = end;
    if (init) {
        yearList.currentIndex = year - start;
    }
}
