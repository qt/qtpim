/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Mobility Components.
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

#ifndef QCONTACTREQUESTS_P_H
#define QCONTACTREQUESTS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qcontactabstractrequest_p.h>
#include <qcontactfilter.h>
#include <qcontactrelationshipfilter.h>
#include <qcontactsortorder.h>
#include <qcontact.h>

#include <QStringList>
#include <QList>

QTCONTACTS_BEGIN_NAMESPACE

class QContactSaveRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactSaveRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactSaveRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::ContactSaveRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactSaveRequest(";
        dbg.nospace() << "contacts=" << m_contacts << ","
                      << "definitionMask=" << m_definitionMask << ","
                      << "errorMap=" << m_errors;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    QList<QContact> m_contacts;
    QMap<int, QContactManager::Error> m_errors;
    QStringList m_definitionMask;
};

class QContactFetchRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactFetchRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactFetchRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::ContactFetchRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactFetchRequest("
                      << "filter=" << m_filter << ","
                      << "sorting=" << m_sorting << ","
                      << "fetchHint=" << m_fetchHint;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    QContactFilter m_filter;
    QList<QContactSortOrder> m_sorting;
    QContactFetchHint m_fetchHint;

    QList<QContact> m_contacts;
};

class QContactFetchByIdRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactFetchByIdRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactFetchByIdRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::ContactFetchByIdRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactFetchByIdRequest(\n"
                      << "* contactIds=" << m_contactIds << ",\n"
                      << "* contacts=" << m_contacts << ",\n"
                      << "* fetchHint=" << m_fetchHint << ",\n"
                      << "* errorMap=" << m_errors;
        dbg.nospace() << "\n)";
        return dbg.maybeSpace();
    }
#endif

    QList<QContactId> m_contactIds;
    QContactFetchHint m_fetchHint;

    QList<QContact> m_contacts;
    QMap<int, QContactManager::Error> m_errors;
};

class QContactRemoveRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactRemoveRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactRemoveRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::ContactRemoveRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactRemoveRequest("
                      << "contactIds=" << m_contactIds << ","
                      << "errorMap=" << m_errors;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    QList<QContactId> m_contactIds;
    QMap<int, QContactManager::Error> m_errors;
};

class QContactIdFetchRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactIdFetchRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactIdFetchRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::ContactIdFetchRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactIdFetchRequest("
                      << "filter=" << m_filter << ","
                      << "sorting=" << m_sorting << ","
                      << "ids=" << m_ids;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    QContactFilter m_filter;
    QList<QContactSortOrder> m_sorting;

    QList<QContactId> m_ids;
};

class QContactRelationshipFetchRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactRelationshipFetchRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactRelationshipFetchRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::RelationshipFetchRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactRelationshipFetchRequest("
                      << "first=" << m_first << ","
                      << "second=" << m_second << ","
                      << "relationshipType=" << m_relationshipType << ","
                      << "relationships=" << m_relationships;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    // selection criteria
    QContact m_first;
    QContact m_second;
    QString m_relationshipType;

    // results
    QList<QContactRelationship> m_relationships;
};

class QContactRelationshipSaveRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactRelationshipSaveRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactRelationshipSaveRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::RelationshipSaveRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactRelationshipSaveRequest("
                      << "relationships=" << m_relationships << ","
                      << "errorMap=" << m_errors;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    QList<QContactRelationship> m_relationships;
    QMap<int, QContactManager::Error> m_errors;
};

class QContactRelationshipRemoveRequestPrivate : public QContactAbstractRequestPrivate
{
public:
    QContactRelationshipRemoveRequestPrivate()
        : QContactAbstractRequestPrivate()
    {
    }

    ~QContactRelationshipRemoveRequestPrivate()
    {
    }

    QContactAbstractRequest::RequestType type() const
    {
        return QContactAbstractRequest::RelationshipRemoveRequest;
    }

#ifndef QT_NO_DEBUG_STREAM
    QDebug& debugStreamOut(QDebug& dbg) const
    {
        dbg.nospace() << "QContactRelationshipRemoveRequest("
                      << "relationships=" << m_relationships << ","
                      << "errorMap=" << m_errors;
        dbg.nospace() << ")";
        return dbg.maybeSpace();
    }
#endif

    QList<QContactRelationship> m_relationships;
    QMap<int, QContactManager::Error> m_errors;
};

QTCONTACTS_END_NAMESPACE

#endif
