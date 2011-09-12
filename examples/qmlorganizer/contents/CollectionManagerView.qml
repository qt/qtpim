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

import QtQuick 2.0
import QtAddOn.organizer 2.0

Item {
    anchors.centerIn: parent
    opacity: 0
    width: calendar.width;
    height: calendar.height - menuBar.height - statusBar.height - 50;

    property Collection collection;

    Column {
        spacing: 2

        width: parent.width; height: parent.height;

        Text {
            text: "Collections:"
            height: 30
            anchors { horizontalCenter: parent.horizontalCenter }
            color: "White"
            font { pointSize: 15; weight: Font.Bold }
        }

        ListView {
            id: collectionList
            opacity: 0.8
            width: parent.width; height: parent.height - instructionsText.height;
            model: organizer.collections

            clip: true
            focus: true
            delegate: listViewDelegate
        }

        Text {
            id: instructionsText
            text: "<To start filtering collections, check the squares of wanted collections, by default all collections are enabled>"
            height: 100; width: parent.width
            anchors { horizontalCenter: parent.horizontalCenter }
            color: "White"
            wrapMode: TextEdit.WordWrap
            font { italic: true }
        }
    }

    Component {
        id: listViewDelegate
        Row {
            property bool isEnabledInCollectionFilter: {
                var ret = false;
                if (organizer.filter == undefined) {
                    ret = true;
                } else {
                    var list = organizer.filter.ids;
                    if (list.indexOf(collectionId) != -1){
                        ret = true;
                    }
                }
                return ret;
            }
            anchors { horizontalCenter: parent.horizontalCenter }
            spacing: 5
            Text {
                text: name
                height: 30
                color: "White"
                font { weight: Font.Bold }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        collectionList.currentIndex = index;
                        modifyCollection();
                    }
                }
            }
            Rectangle {
                width: 20; height: 20
                border { color: "black"; width: 1; }
                color: isEnabledInCollectionFilter ? "black" : "gray"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        collectionList.currentIndex = index;
                        isEnabledInCollectionFilter = !isEnabledInCollectionFilter;
                        modifyCollectionFilter(isEnabledInCollectionFilter, index);
                    }
                }
            }
        }
    }

    function addCollection() {
        collectionEditorView.collection = Qt.createQmlObject('import QtQuick 2.0; import QtAddOn.organizer 2.0; Collection {}',organizer);
        calendar.state = "CollectionEditorView";
    }

    function modifyCollection() {
        collectionEditorView.collection = organizer.collections[collectionList.currentIndex];
        calendar.state = "CollectionEditorView";
    }

    function modifyCollectionFilter(enabled, index) {
        //Currently we shall only have collection filter
        var modelCollectionFilter = organizer.filter
        if (modelCollectionFilter == undefined) {
            //Create new filter
            modelCollectionFilter = calendar.createEmptyItem("Collectionfilter");
            var collectionList = organizer.collections;
            var collectionFilterList = [];
            if (false == enabled) {//add all ids to the list except the one is not enabled
                for (var i = 0; i < collectionList.length; i++) {
                    if (i != index)
                        collectionFilterList.push(organizer.collections[i].collectionId);
                }
            }//We shall not have the case that enable==true
            modelCollectionFilter.ids = collectionFilterList
            //Update model filter
            organizer.filter = modelCollectionFilter;

        } else { //Update filter list
            //Get exist filter id list,
            var filterIdsList = modelCollectionFilter.ids;
            var collectionId = organizer.collections[index].collectionId;
            if (false == enabled) {
                //If the enable is false, remove from the list if we found inside list
                var filterIndex = filterIdsList.indexOf(collectionId);
                if (filterIndex >= 0)
                    filterIdsList.splice(filterIndex, 1);
                else
                    console.log("Warning: Collection id is not found in filter list" + collectionId);
            } else {
                //else if the enable is true, add this id in the filter list if we do not have it in list
                if (filterIdsList.indexOf(collectionId) == -1)
                    filterIdsList.push(collectionId);
                else
                    console.log("Warning: Collection id exists in filter list :" + collectionId);
            }
            //update model filter
            modelCollectionFilter.ids = filterIdsList;
        }
    }
}
