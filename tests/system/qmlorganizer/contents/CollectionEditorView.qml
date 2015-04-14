/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtOrganizer 5.0

Item {
    anchors.centerIn: parent
    opacity: 0
    width: calendar.width;
    height: settingsView.height - settingsView.buttonTabsRow.height;

    property Collection collection;
    property bool isNewCollection: false

    Column {
        id: editorItems
        y: settingsView.buttonTabsRow.height

        spacing: 4
        opacity: 0.8
        width: parent.width; height: parent.height;

        Text {
            text: "Collection"
            height: 30
            anchors { horizontalCenter: parent.horizontalCenter }
            color: "White"
            font { pointSize: 15; weight: Font.Bold }
        }
        FieldRow {
            id: nameRow
            label: "Name"
            value: collection ? collection.name : ""
        }
//        FieldRow {
//            id: idRow
//            label: "Id"
//            value: collection.collectionId
//        }
        FieldRow {
            id: descRow
            label: "Description"
            value: collection? collection.description : ""
        }
        FieldRow {
            id: colorRow
            label: "Color"
            value: collection? collection.color : ""
        }
        FieldRow {
            id: imageRow
            label: "Image"
            value: collection? collection.image : ""
        }
    }

    Rectangle {
        id: buttonRow
        anchors { bottom: parent.bottom; left: parent.left; right: parent.right;  }
        height: saveCollectionButton.height
        color:"#343434"

        Button { id: saveCollectionButton; text: "Save"; width: calendar.width/2; onClicked: saveCollection(); anchors.left:parent.left }
        Button { id: removeCollectionButton; text: "Delete"; width: calendar.width/2; onClicked: removeCollection(); anchors.left:saveCollectionButton.right }
    }


    function saveCollection() {
        var newCollection = Qt.createQmlObject('import QtQuick 2.0; import QtOrganizer 5.0; Collection {}',organizer);
        newCollection.name = nameRow.newValue;
        newCollection.collectionId = collection.collectionId;
        newCollection.description = descRow.newValue;
        newCollection.color = colorRow.newValue;
        newCollection.image = imageRow.newValue;
        organizer.saveCollection(newCollection);
        settingsView.state = "CollectionManagerView";
    }

    function removeCollection() {
        organizer.removeCollection(collection.collectionId);
        settingsView.state = "CollectionManagerView";
    }
}
