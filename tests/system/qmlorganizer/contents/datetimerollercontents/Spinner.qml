/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
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

Image {
    property alias model: view.model
    property alias delegate: view.delegate
    property alias currentIndex: view.currentIndex
    property real itemHeight: 15

    source: "spinner-bg.png"
    clip: true
    focus: true
    width: 30; height: 60

    PathView {
        id: view
        anchors.fill: parent
        clip: true

        pathItemCount: height/itemHeight
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        //highlight: Rectangle { color: "lightblue"; width: view.width; height: itemHeight+4; }
        highlight: Image { source: "spinner-select.png"; width: view.width; height: itemHeight+4 }
        dragMargin: view.width/2

        path: Path {
            startX: view.width/2; startY: -itemHeight/2
            PathLine { x: view.width/2; y: view.pathItemCount*itemHeight + itemHeight }
        }
    }

    Keys.onDownPressed: view.incrementCurrentIndex()
    Keys.onUpPressed: view.decrementCurrentIndex()
}
