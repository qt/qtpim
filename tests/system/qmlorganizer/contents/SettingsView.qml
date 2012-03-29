/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtPim module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtOrganizer 5.0

Item {
        id: view
        anchors.centerIn: parent
        opacity: 0
        width: calendar.contentArea.width;
        height: calendar.contentArea.height;
        state: "CollectionManagerView"
        property alias collectionManagerView: collectionManagerView
        property alias buttonTabsRow : buttonTabsRow

        states: [
            State {name: "CollectionManagerView"; PropertyChanges { target: collectionManagerView; opacity: 1; }},
            State {name: "CollectionEditorView"; PropertyChanges { target: collectionEditorView; opacity: 1; }},
            State {name: "StorageLocationView"; PropertyChanges { target: storageLocationView; opacity: 1; }}
        ]
        transitions: [
            Transition {
                NumberAnimation {
                    properties: "opacity"
                    easing.type: "Linear"
                    duration: 10
                }
            }
        ]

        Row {
            id: buttonTabsRow
            anchors { top: parent.top }
            Button {
                text: "Collections"
                width: calendar.width / 2
                onClicked: { view.state = "CollectionManagerView"}
            }
            Button {
                text: "Storage Locations"
                width: calendar.width / 2
                onClicked: { view.state = "StorageLocationView" }
            }
        }

        CollectionManagerView {
            id: collectionManagerView;
        }
        CollectionEditorView {
            id: collectionEditorView;
        }
        StorageLocationView {
            id: storageLocationView;
        }

        function mapStorageLocationStringToInt(storageName) {
            if (storageName == "UserDataStorage")
                return OrganizerModel.UserDataStorage;
            else if (storageName == "SystemStorage")
                return OrganizerModel.SystemStorage;
        }
}
