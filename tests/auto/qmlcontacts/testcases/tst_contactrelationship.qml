/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

TestCase {

    name: "ContactRelationship"

    Contact {
        id: contact1
        Name {
            id: contact1Name
            firstName: "contact1"
        }
    }

    Contact {
        id: contact2
        Name {
            id: contact2Name
            firstName: "contact2"
        }
    }

    function setAndVerifyRelationship(first, second, relationship) {
        relationship.first = first
        relationship.second = second
        compare(relationship.first.name.firstName, first.name.firstName)
        compare(relationship.second.name.firstName, second.name.firstName)
    }

    Relationship {
        id: relationshipHasMember
        type: Relationship.HasMember
    }

    Relationship {
        id: relationshipHasMemberTypeAsString
        type: "HasMember"
    }

    function test_hasMember() {
        compare(relationshipHasMember.type, "HasMember")
        compare(relationshipHasMemberTypeAsString.type, "HasMember")

        setAndVerifyRelationship(contact1, contact2, relationshipHasMember)
        setAndVerifyRelationship(contact1, contact2, relationshipHasMemberTypeAsString)
    }

    Relationship {
        id: relationshipAggregates
        type: Relationship.Aggregates
    }

    Relationship {
        id: relationshipAggregatesTypeAsString
        type: "Aggregates"
    }

    function test_aggregates() {
        compare(relationshipAggregates.type, "Aggregates")
        compare(relationshipAggregatesTypeAsString.type, "Aggregates")

        setAndVerifyRelationship(contact1, contact2, relationshipAggregates)
    }

    Relationship {
        id: relationshipIsSameAs
        type: Relationship.IsSameAs
    }

    Relationship {
        id: relationshipIsSameAsTypeAsString
        type: "IsSameAs"
    }

    function test_isSameAs() {
        compare(relationshipIsSameAs.type, "IsSameAs")
        compare(relationshipIsSameAsTypeAsString.type, "IsSameAs")

        setAndVerifyRelationship(contact1, contact2, relationshipIsSameAs)
    }

    Relationship {
        id: relationshipHasAssistant
        type: Relationship.HasAssistant
    }

    Relationship {
        id: relationshipHasAssistantTypeAsString
        type: "HasAssistant"
    }

    function test_hasAssistant() {
        compare(relationshipHasAssistant.type, "HasAssistant")
        compare(relationshipHasAssistantTypeAsString.type, "HasAssistant")

        setAndVerifyRelationship(contact1, contact2, relationshipHasAssistant)
    }

    Relationship {
        id: relationshipHasManager
        type: Relationship.HasManager
    }

    Relationship {
        id: relationshipHasManagerTypeAsString
        type: "HasManager"
    }

    function test_hasManager() {
        compare(relationshipHasManager.type, "HasManager")
        compare(relationshipHasManagerTypeAsString.type, "HasManager")

        setAndVerifyRelationship(contact1, contact2, relationshipHasManager)
    }

    Relationship {
        id: relationshipHasSpouse
        type: Relationship.HasSpouse
    }

    Relationship {
        id: relationshipHasSpouseTypeAsString
        type: "HasSpouse"
    }

    function test_hasSpouse() {
        compare(relationshipHasSpouse.type, "HasSpouse")
        compare(relationshipHasSpouseTypeAsString.type, "HasSpouse")

        setAndVerifyRelationship(contact1, contact2, relationshipHasSpouse)
    }

}
