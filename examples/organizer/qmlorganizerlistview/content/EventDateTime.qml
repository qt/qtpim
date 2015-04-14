/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Pim Module.
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

//![Event Date Time Component]
//![Create Event Date Time Component]
import QtQuick 2.0

Rectangle {
    property alias date: eventDate.text
    property alias month: eventMonth.text
    property alias year: eventYear.text

    radius: 5
    border {color: "black"; width: 2}
    height: eventDate.height + eventMonth.height + eventYear.height * 1.3
    width: parent.width

//![Create Event Date Time Component]

//![Grid to edit Date Month Year]
    Grid {
        columns: 2
        spacing: 3
        //anchors.centerIn: parent

        Text {
            text: "Date:"
        }

        TextInput {
            id: eventDate
            validator: IntValidator {
                bottom: 1
                top: 31
            }
        }

        Text {
            text: "Month:"
        }
        TextInput {
            id: eventMonth
            validator: IntValidator {
                bottom: 1
                top: 12
            }
        }

        Text {
            text: "Year:"
        }
        TextInput {
            id: eventYear
            validator: IntValidator {
                bottom: 1970
                top: 2020
            }
        }
    }
//![Grid to edit Date Month Year]
}
//![Event Date Time Component]
