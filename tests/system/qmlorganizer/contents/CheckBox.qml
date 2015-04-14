/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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

        Rectangle {
            id: checkBox;
            property bool checked: false
            property string checkMark: "x"

            signal clicked( bool checked )

            Component.onCompleted: {
                if (!checked)
                    text.text = ""
                else
                    text.text = checkMark
            }

            width: text.height + 5
            height: text.height + 5
            color: "lightgrey"
            opacity: 0.95
            border.width: 3
            border.color: "white"
            radius: 5
            smooth: true

            Text {
                id: text
                anchors.verticalCenter: checkBox.verticalCenter
                anchors.horizontalCenter: checkBox.horizontalCenter
                color: "black"
            }

            MouseArea {
                id: mouseArea

                anchors.fill: parent
                onClicked: {
                    checked = !checked
                    checkBox.clicked( checked )
                }
            }

            states: [
            State {
                id: stateChecked

                name: "checked"; when: checked

                PropertyChanges {
                    id: propertyChangeCheckMark

                    target: text
                    text: checkMark
                }
                PropertyChanges { target: checkBox; border.color: "white"; color: "white"}
                }
            ]
        }
