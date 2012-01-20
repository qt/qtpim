/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
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

import QtQuick 2.0
import QtOrganizer 5.0

Item {
    anchors.centerIn: parent
    opacity: 0
    width: calendar.width;
    height: calendar.height - menuBar.height - statusBar.height// - editorItems.height;

    property Collection collection;


    Column {
        id: editorItems

        spacing: 4
        opacity: 0.8

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
        calendar.state = "CollectionManagerView";
    }

    function removeCollection() {
        organizer.removeCollection(collection.collectionId);
        calendar.state = "CollectionManagerView";
    }
}
