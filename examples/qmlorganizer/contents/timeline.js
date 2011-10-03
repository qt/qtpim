/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
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
