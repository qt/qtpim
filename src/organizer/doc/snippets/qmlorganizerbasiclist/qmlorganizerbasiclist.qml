/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt PIM Module.
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

//! [Basic list Complete Snippet]
import QtQuick 2.0
import QtOrganizer 5.0

Rectangle {
    id : organizerSample
    width: 200
    height: 400

    Rectangle {
        id:organizerEventView

        anchors.fill: parent
        color: "white"

//! [Organizer Model]
        OrganizerModel {
            id: organizer

            startPeriod: "1970-01-01"
            endPeriod:'2012-12-31'
            autoUpdate:true

            //! [Manager Choice]
            manager:"qtorganizer:memory:id=qml"
            //! [Manager Choice]

            Component.onCompleted : {
                if (managerName == "memory") {
                    organizer.importItems(Qt.resolvedUrl("content/organizer_ical_test.ics"));
                    console.log("Memory backend : import Items executed.");
                }
            }
        }
//! [Organizer Model]

//! [List View]
        ListView {
            id: calendar
            anchors.fill: parent

            clip: true

            delegate: Text { text: model.item.displayLabel }
            model: organizer
        }
//! [List View]
    }
}
//! [Basic list Complete Snippet]
