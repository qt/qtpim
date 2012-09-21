/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Pim Module.
**
** $QT_BEGIN_LICENSE:FDL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Free Documentation License Usage
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.  Please review the following information to ensure
** the GNU Free Documentation License version 1.3 requirements
** will be met: http://www.gnu.org/copyleft/fdl.html.
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
