/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
import QtContacts 5.0
import com.nokia.components 1.0
import com.nokia.components.extras 1.0

Item {
    id: mainItem
    property variant contacts
    property alias visibleArea: mainList.visibleArea
    property alias count: mainList.count

    signal openContact(variant contact)

    ListView {
        id: mainList

        anchors.fill:  parent

        highlightFollowsCurrentItem: false
        focus: true
        keyNavigationWraps: true
        opacity: 0.8

        model: contacts
        section.property: "model.contact.name.firstName"
        section.criteria: ViewSection.FirstCharacter
        section.delegate: ListSectionDelegate { }

        delegate: Rectangle {
            id: wrapper
            border.width: 0
            height: 48;
            width: mainList.width;

            color: "black"

            Text {
                id: displayLabelText
                objectName: "displayLabel"
                anchors { left: parent.left; leftMargin: 7;}
                font.pixelSize: 24
                font.bold: true
                color: "white";

                /* TODO: Can we avoid clip? We could use elide, but it does not support rich text (html bolding)
                   check http://bugreports.qt.nokia.com/browse/QTBUG-16567
                */
                clip: true
                //elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: {
                    return model.contact.displayLabel;
                        // display label?
                        if (dl.length > 0) {
                            return dl;
                        }
                        return (fn + " " + ln);
                    }
            }
            Text {
                id: contactName
                objectName: "name"
                anchors { left: parent.left;
                          top: displayLabelText.bottom;
                          leftMargin: 7;
                }
                font.pixelSize: 16
                color: "grey";

                /* TODO: Can we avoid clip? We could use elide, but it does not support rich text (html bolding)
                   check http://bugreports.qt.nokia.com/browse/QTBUG-16567
                */
                clip: true
                //elide: Text.ElideRight
                wrapMode: Text.WordWrap
                text: {
                    // TBD: precalculate in a View (maybe: displayName = [..fields])
                        var fn = model.contact.name.firstName || ""
                        var ln = model.contact.name.lastName || ""

                        return (fn + " " + ln);
                    }
            }

            MouseArea {
                id: clistMouseArea
                anchors.fill: parent
                onClicked: {
                    openContact (model.contact);
                }
            }
        }
    }
}
