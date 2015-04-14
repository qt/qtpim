/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
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
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef REQUESTEXAMPLE_H
#define REQUESTEXAMPLE_H

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

#include <QObject>

#include <QtContacts/qcontactrequests.h>

//! [Class setup]
QTCONTACTS_USE_NAMESPACE
class AsyncRequestExample : public QObject
{
    Q_OBJECT

public:
    AsyncRequestExample();
    ~AsyncRequestExample();

public slots:
    void performRequests();

private slots:
    void contactFetchRequestStateChanged(QContactAbstractRequest::State newState);
    void contactSaveRequestStateChanged(QContactAbstractRequest::State newState);
    void contactRemoveRequestStateChanged(QContactAbstractRequest::State newState);
    void relationshipFetchRequestStateChanged(QContactAbstractRequest::State newState);
    void relationshipSaveRequestStateChanged(QContactAbstractRequest::State newState);
    void relationshipRemoveRequestStateChanged(QContactAbstractRequest::State newState);

private:
    QContactManager *m_manager;
    QContactFetchRequest m_contactFetchRequest;
    QContactSaveRequest m_contactSaveRequest;
    QContactRemoveRequest m_contactRemoveRequest;
    QContactRelationshipFetchRequest m_relationshipFetchRequest;
    QContactRelationshipSaveRequest m_relationshipSaveRequest;
    QContactRelationshipRemoveRequest m_relationshipRemoveRequest;
};
//! [Class setup]

class RequestExample : public QObject
{
    Q_OBJECT

public:
    RequestExample()
            : QObject(),
            m_previousLastIndex(-1),
            m_manager(0),
            m_fetchRequest(new QContactFetchRequest)
    {
    }

    ~RequestExample()
    {
        delete m_fetchRequest;
    }

    void setManager(QContactManager* manager)
    {
        m_manager = manager;
        m_fetchRequest->setManager(m_manager);
    }

private slots:
    void performRequest();
    void printContacts();
    void stateChanged(QContactAbstractRequest::State state);

private:
    int m_previousLastIndex;
    QContactManager *m_manager;
    QContactFetchRequest *m_fetchRequest;
};

#endif
