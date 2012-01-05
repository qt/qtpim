/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtOrganizer 5.0

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
            property bool isEnabledInCollectionFilter: modelCollectionFilter.ids.indexOf(collectionId) != -1
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
        collectionEditorView.collection = Qt.createQmlObject('import QtQuick 2.0; import QtOrganizer 5.0; Collection {}',organizer);
        calendar.state = "CollectionEditorView";
    }

    function modifyCollection() {
        collectionEditorView.collection = organizer.collections[collectionList.currentIndex];
        calendar.state = "CollectionEditorView";
    }

    function modifyCollectionFilter(enabled, index) {
        //Get exist filter id list,
        var filterIdsList = modelCollectionFilter.ids;
        //Collection id will be added or removed from filter
        var collectionId = organizer.collections[index].collectionId;
        var filterIndex = filterIdsList.indexOf(collectionId);
        if (false == enabled) {
            if (filterIndex >= 0) //If the enable is false, remove from the list if we found inside list
                filterIdsList.splice(filterIndex, 1);
            else
                console.log("Warning: Collection id is not found in filter list" + collectionId);
        } else {//Add this id in the filter list if we do not have it in list
            if (filterIndex == -1)
                filterIdsList.push(collectionId);
            else
                console.log("Warning: Collection id exists in filter list :" + collectionId);
        }
        //Update model collection filter
        modelCollectionFilter.ids = filterIdsList;
    }
}

