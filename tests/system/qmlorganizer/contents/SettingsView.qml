/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtPim module of the Qt Toolkit.
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
            State {name: "CollectionEditorView"; PropertyChanges { target: collectionEditorView; opacity: 1; }}
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
        }

        CollectionManagerView {
            id: collectionManagerView;
        }
        CollectionEditorView {
            id: collectionEditorView;
        }
}
