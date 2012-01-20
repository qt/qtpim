/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the documentation of the Qt Pim Module.
**
** $QT_BEGIN_LICENSE:FDL$
** GNU Free Documentation License
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms
** and conditions contained in a signed written agreement between you
** and Nokia.
**
**
**
**
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
//            manager:"qtorganizer:jsondb:id=qml"
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
