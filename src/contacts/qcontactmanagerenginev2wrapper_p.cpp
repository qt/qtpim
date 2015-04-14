/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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

#include "qcontactmanagerenginev2wrapper_p.h"

#include <QtCore/qset.h>

#include "qcontact_p.h"
#include "qcontactidfilter.h"
#include "qcontactabstractrequest_p.h"
#include "qcontactfetchbyidrequest.h"

QT_BEGIN_NAMESPACE_CONTACTS

QContactManagerEngineV2Wrapper::QContactManagerEngineV2Wrapper(QContactManagerEngine *wrappee)
    : m_engine(wrappee)
{
    Q_ASSERT(wrappee);
}

QContactManagerEngineV2Wrapper::~QContactManagerEngineV2Wrapper()
{
    delete m_engine;
}

void QContactManagerEngineV2Wrapper::requestDestroyed(QContactAbstractRequest* req)
{
    RequestController* controller = m_controllerForRequest.value(req);

    if (controller) {
        // If we own it, just delete the controller (and ignore any subrequests' signals from now on)
        delete controller;
        m_controllerForRequest.insert(req, 0);
    } else {
        m_engine->requestDestroyed(req);
    }
}

bool QContactManagerEngineV2Wrapper::startRequest(QContactAbstractRequest* req)
{
    if (req && req->type() == QContactAbstractRequest::ContactSaveRequest
            && !static_cast<QContactSaveRequest*>(req)->typeMask().isEmpty()) {
        RequestController* controller;
        controller = new PartialSaveRequestController(m_engine, this);
        controller->setRequest(req);
        connect(controller, SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(requestStateChanged(QContactAbstractRequest::State)),
                Qt::QueuedConnection);
        m_controllerForRequest.insert(req, controller);
        if (controller->start()) {
            updateRequestState(req, QContactAbstractRequest::ActiveState);
            return true;
        } else {
            return false;
        }
    }

    // Otherwise, pass it on
    return m_engine->startRequest(req);
}

/* A slot connected to the stateChanged signal of a controller object.  It is signaled by the
   controller to indicate that one of the client's requests has an updated state. */
void QContactManagerEngineV2Wrapper::requestStateChanged(QContactAbstractRequest::State state)
{
    RequestController* controller = qobject_cast<RequestController*>(sender());
    Q_ASSERT(controller);
    QContactAbstractRequest* request = controller->request();

    if (state == QContactAbstractRequest::FinishedState) {
        controller->deleteLater();
        if (request) { // It's possible the request was deleted by the sender.
            // Keep the key in m_controllerForRequest but point it to null to indicate a defunct
            // controller
            m_controllerForRequest.insert(request, 0);
        }
    } else {
        updateRequestState(request, state);
    }

}

/* \reimp */
bool QContactManagerEngineV2Wrapper::cancelRequest(QContactAbstractRequest* req)
{
    if (m_controllerForRequest.contains(req)) {
        RequestController* controller = m_controllerForRequest.value(req);
        if (controller) {
            // If we own it and it hasn't already been deleted,
            // just delete the controller (and ignore any subrequests' signals from now on)
            delete controller;
            m_controllerForRequest.insert(req, 0);
        }
        return true;
    } else {
        return m_engine->cancelRequest(req);
    }
}

/* \reimp */
bool QContactManagerEngineV2Wrapper::waitForRequestFinished(QContactAbstractRequest* req, int msecs)
{
    if (m_controllerForRequest.contains(req)) {
        RequestController* controller = m_controllerForRequest.value(req);
        if (controller) {
            if (controller->waitForFinished(msecs)) {
                updateRequestState(req, QContactAbstractRequest::FinishedState);
                return true;
            } else {
                return false;
            }
        } else {
            // A request with a null controller means it has already finished
            return true;
        }
    } else {
        return m_engine->waitForRequestFinished(req, msecs);
    }

}

/* A static helper to twiddle with \a request's privates, setting its engine to \a engine. */
void QContactManagerEngineV2Wrapper::setEngineOfRequest(QContactAbstractRequest* request,
                                                        QContactManagerEngine* engine)
{
    request->d_ptr->m_engine = engine;
}


// General RequestController stuff
/* A single RequestController is associated with a single client QContactAbstractRequest.  It
   manages the sub-requests that need to be sent to the real engine and controls the asynchronous
   flow between start() and one or more handleUpdatedSubRequest() calls that might follow.
   waitForFinished() can also be called on a controller, which synchronously performs the rest of
   the necessary sub-requests.
 */

/* A slot connected the stateChanged signal of a sub request */
void RequestController::handleUpdatedSubRequest(QContactAbstractRequest::State state)
{
    QObject* caller = sender();
    QContactAbstractRequest* subRequest = qobject_cast<QContactAbstractRequest*>(caller);
    if (subRequest) {
        if (state == QContactAbstractRequest::FinishedState) {
            // It's possibly already finished if waitForFinished has previously been called
            if (!isFinished())
                handleFinishedSubRequest(subRequest);
        } else {
            // XXX maybe Canceled should be handled
        }
    }
}

/* This function handles the rest of the program flow in a synchronous way. */
bool RequestController::waitForFinished(int msecs)
{
    // If the current request is active, it must be a ContactFetchRequest.  We just need to
    // wait for it to finish, then finalize the post-processing.
    if (m_currentSubRequest.isNull()) {
        return false;
    }
    while (!isFinished()) {
        if (!m_currentSubRequest->waitForFinished(msecs))
            return false;
        handleFinishedSubRequest(m_currentSubRequest.data());
    }
    return true;
}

// FetchByIdRequestController stuff
/* A FetchByIdRequestController is a RequestController for QContactFetchByIdRequests */
bool FetchByIdRequestController::start()
{
    // Our strategy is to translate it to a ContactFetchRequest.  Later when it finishes, we can
    // fiddle with the results to get it in the right format.
    Q_ASSERT(m_request);
    QContactFetchByIdRequest* originalRequest = static_cast<QContactFetchByIdRequest*>(m_request.data());
    QContactFetchRequest* qcfr = new QContactFetchRequest;
    QContactIdFilter lif;
    lif.setIds(originalRequest->contactIds());
    qcfr->setFilter(lif);
    qcfr->setFetchHint(originalRequest->fetchHint());
    // normally, you'd set the manager, but in this case, we only have a bare engine:
    QContactManagerEngineV2Wrapper::setEngineOfRequest(qcfr, m_engine);
    m_currentSubRequest.reset(qcfr);
    connect(qcfr, SIGNAL(stateChanged(QContactAbstractRequest::State)),
            this, SLOT(handleUpdatedSubRequest(QContactAbstractRequest::State)),
            Qt::QueuedConnection);
    return qcfr->start();
}

/* One of our subrequests has finished.  Go to the next step. */
void FetchByIdRequestController::handleFinishedSubRequest(QContactAbstractRequest* subReq)
{
    // For a FetchByIdRequest, we know that the only subrequest is a QContactFetchRequest.
    // The next step is simply to take the results and reformat it.
    // Take the results:
    QContactFetchRequest* qcfr = qobject_cast<QContactFetchRequest*>(subReq);
    QList<QContact> contacts = qcfr->contacts();
    QContactManager::Error error = qcfr->error();

    // Build an index into the results
    QHash<QContactId, int> idMap; // value is index into unsorted
    if (error == QContactManager::NoError) {
        for (int i = 0; i < contacts.size(); i++) {
            idMap.insert(contacts[i].id(), i);
        }
    }

    // Find the order in which the results should be presented
    QContactFetchByIdRequest* request = static_cast<QContactFetchByIdRequest*>(m_request.data());
    QList<QContactId> ids(request->contactIds());

    // Build up the results and errors
    QList<QContact> results;
    QMap<int, QContactManager::Error> errorMap;
    for (int i = 0; i < ids.count(); i++) {
        QContactId id(ids[i]);
        if (!idMap.contains(id)) {
            errorMap.insert(i, QContactManager::DoesNotExistError);
            if (error == QContactManager::NoError)
                error = QContactManager::DoesNotExistError;
            results.append(QContact());
        } else {
            results.append(contacts[idMap[id]]);
        }
    }

    // Update the request object
    QContactManagerEngineV2Wrapper::updateContactFetchByIdRequest(
            request,
            results,
            error,
            errorMap,
            QContactAbstractRequest::FinishedState);
    finish();
}


// PartialSaveRequestController stuff
/* A PartialSaveRequestController is a RequestController for QContactSaveRequests with definition mask */
bool PartialSaveRequestController::start()
{
    // The first step is to fetch the existing contacts.
    QList<QContactId> existingContactIds;
    QList<int> newContactIndices;

    // First, remove the contacts that aren't from this manager
    QList<QContact> contacts(request()->contacts());
    // Try to figure out which of our arguments are new contacts
    for(int i = 0; i < contacts.count(); i++) {
        // See if there's a contactId that's not from this manager
        const QContact c = contacts.at(i);
        if (c.id().managerUri() == m_engine->managerUri()) {
            m_existingIdMap.insert(i, existingContactIds.count());
            existingContactIds.append(c.id());
        } else if (!c.id().isNull()) {
            // Hmm, error (wrong manager)
            m_errorMap.insert(i, QContactManager::DoesNotExistError);
        } else {
            newContactIndices.append(i);
        }
    }

    if (!existingContactIds.isEmpty()) {
        // Now do the fetch and wait for the result in handleFinishedSubRequest
        QContactFetchByIdRequest* cfbir = new QContactFetchByIdRequest;
        cfbir->setIds(existingContactIds);
        // normally, you'd set the manager, but in this case, we only have a bare engine:
        QContactManagerEngineV2Wrapper::setEngineOfRequest(cfbir, m_v2wrapper);
        m_currentSubRequest.reset(cfbir);
        connect(cfbir, SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(handleUpdatedSubRequest(QContactAbstractRequest::State)),
                Qt::QueuedConnection);
        return cfbir->start();
    } else {
        // Special case for the case where every contact is new - we don't need to bother fetching
        // any existing contacts - just prune them down to the mask and do the save request.
        QList<QContact> contactsToSave;
        QSet<QContactDetail::DetailType> mask(request()->typeMask().toSet());
        for (int i = 0; i < newContactIndices.count(); i++) {
            QContact contactToSave;
            partiallyCopyDetails(&contactToSave, contacts[newContactIndices[i]], mask);
            m_savedToOriginalMap.append(i);
            contactsToSave.append(contactToSave);
        }
        QContactSaveRequest* csr = new QContactSaveRequest;
        csr->setContacts(contactsToSave);
        // normally, you'd set the manager, but in this case, we only have a bare engine:
        QContactManagerEngineV2Wrapper::setEngineOfRequest(csr, m_engine);
        m_currentSubRequest.reset(csr);
        connect(csr, SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(handleUpdatedSubRequest(QContactAbstractRequest::State)),
                Qt::QueuedConnection);
        return csr->start();
    }
}

/* One of our subrequests has finished.  Go to the next step. */
void PartialSaveRequestController::handleFinishedSubRequest(QContactAbstractRequest* subReq)
{
    if (subReq->type() == QContactAbstractRequest::ContactFetchByIdRequest) {
        QContactFetchByIdRequest* cfbir = qobject_cast<QContactFetchByIdRequest*>(subReq);
        QList<QContact> contactsToSave;
        QMap<int, QContactManager::Error> fetchErrors(cfbir->errorMap());
        QList<QContact> existingContacts(cfbir->contacts());
        QList<QContact> contacts(request()->contacts());
        QSet<QContactDetail::DetailType> mask(request()->typeMask().toSet());
        for (int i = 0; i < contacts.count(); i++) {
            // See if this is an existing contact or a new one
            const int fetchedIdx = m_existingIdMap.value(i, -1);
            QContact contactToSave;
            if (fetchedIdx >= 0) {
                // See if we had an error
                if (fetchErrors[fetchedIdx] != QContactManager::NoError) {
                    m_errorMap.insert(i, fetchErrors[fetchedIdx]);
                    continue;
                }

                // Existing contact we should have fetched
                contactToSave = existingContacts.at(fetchedIdx);

                QSharedDataPointer<QContactData>& cd = QContactData::contactData(contactToSave);
                cd->removeOnly(mask);
            } else if (m_errorMap.contains(i)) {
                // A bad argument.  Leave it out of the contactsToSave list
                continue;
            } // else new contact

            partiallyCopyDetails(&contactToSave, contacts.at(i), mask);
            m_savedToOriginalMap.append(i);
            contactsToSave.append(contactToSave);
        }

        // Now do the fetch and wait for the result
        QContactSaveRequest* csr = new QContactSaveRequest;
        csr->setContacts(contactsToSave);
        // normally, you'd set the manager, but in this case, we only have a bare engine:
        QContactManagerEngineV2Wrapper::setEngineOfRequest(csr, m_engine);
        m_currentSubRequest.reset(csr);
        connect(csr, SIGNAL(stateChanged(QContactAbstractRequest::State)),
                this, SLOT(handleUpdatedSubRequest(QContactAbstractRequest::State)),
                Qt::QueuedConnection);
        csr->start(); // TODO what if this fails?
    } else if (subReq->type() == QContactAbstractRequest::ContactSaveRequest) {
        QContactSaveRequest* csr = qobject_cast<QContactSaveRequest*>(subReq);
        QList<QContact> savedContacts(csr->contacts());
        QMap<int, QContactManager::Error> saveErrors(csr->errorMap());
        QList<QContact> contacts(request()->contacts());
        for (int i = 0; i < savedContacts.count(); i++) {
            contacts[m_savedToOriginalMap[i]].setId(savedContacts[i].id());
        }
        // Populate the m_errorMap with the m_errorMap of the attempted save
        QMap<int, QContactManager::Error>::iterator it(saveErrors.begin());
        QContactManager::Error error = QContactManager::NoError;
        while (it != saveErrors.end()) {
            error = it.value();
            m_errorMap.insert(m_savedToOriginalMap[it.key()], it.value());
            it++;
        }

        // Update the request object
        QContactManagerEngine::updateContactSaveRequest(
                request(),
                contacts,
                error,
                m_errorMap,
                QContactAbstractRequest::FinishedState);
        finish();
    } else {
        Q_ASSERT(false);
    }
}

/* Copy details specified in \a mask from contact \a from to contact \a to */
void PartialSaveRequestController::partiallyCopyDetails(QContact* to, const QContact& from,
                                                        const QSet<QContactDetail::DetailType>& mask) {
    foreach (QContactDetail::DetailType type, mask) {
        QList<QContactDetail> details = from.details(type);
        foreach(QContactDetail detail, details) {
            to->saveDetail(&detail);
        }
    }
}

#include "moc_qcontactmanagerenginev2wrapper_p.cpp"

QT_END_NAMESPACE_CONTACTS
