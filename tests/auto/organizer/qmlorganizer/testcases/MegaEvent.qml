/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtPim Addon Module.
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

import QtOrganizer 5.0

Event {

    startDateTime: new Date("2012-01-01 12:00:00 GMT+0000")
    endDateTime: new Date("2012-01-01 13:00:00 GMT+0000")
    allDay: false

    Comment {
        comment: "Comment"
    }

    Description {
        description: "Description"
    }

    DisplayLabel {
        label: "Display label"
    }

    Location {
        latitude: 1.1
        longitude: 1.2
        label: "Location"
    }

    Priority {
        priority: Priority.Lowest
    }

    Tag {
        tag: "Tag"
    }

    Timestamp {
        created: new Date("2012-01-01 12:00:00 GMT+0000")
        lastModified: new Date("2012-01-01 12:00:00 GMT+0000")
    }

    AudibleReminder {
        repetitionCount: 1
        repetitionDelay: 1
        secondsBeforeStart: 0
        dataUrl: "ftp://Audible_reminder_data_url"
    }

    EmailReminder {
        repetitionCount: 1
        repetitionDelay: 1
        secondsBeforeStart: 0
        body: "Email reminder body"
        subject: "Email reminder subject"
        recipients: ["Recipient 1", "Recipient 2"]
        attachments: ["Attachment 1", "Attachment 2"]
    }

    VisualReminder {
        repetitionCount: 1
        repetitionDelay: 1
        secondsBeforeStart: 0
        message: "Visual reminder message"
        dataUrl: "ftp://visual_reminder_data_url"
    }

    ExtendedDetail {
        name: "extended detail: string data"
        data: "data"
    }

    ExtendedDetail {
        name: "extended detail: integer data"
        data: 1
    }

    ExtendedDetail {
        name: "extended detail: array data"
        data: ["string 1", "string 2"]
    }

    ExtendedDetail {
        name: "extended detail: object data"
        data: {"key 1": "string 1", "key 2": "string 2"}
    }

    EventAttendee {
        name: "Event attendee"
        emailAddress: "new.attendee@qt.com"
        attendeeId: "123444455555"
        participationStatus: EventAttendee.StatusAccepted
        participationRole: EventAttendee.RoleRequiredParticipant
    }

    EventRsvp {
        organizerName: "Organizer name"
        organizerEmail: "Organizer email"
        responseDate: new Date("2012-01-01 12:00:00 GMT+0000")
        responseDeadline: new Date("2012-01-01 12:00:00 GMT+0000")
        participationStatus: EventAttendee.StatusAccepted
        participationRole: EventAttendee.RoleOrganizer
        responseRequirement: EventRsvp.ResponseRequired
    }

    Classification {
        classification: Classification.AccessConfidential
    }

    Version {
        version: 1
        extendedVersion: "1234"
    }

}
