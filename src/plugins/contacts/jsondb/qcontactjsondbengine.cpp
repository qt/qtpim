/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
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
#include <QVariantMap>
#include <QEventLoop>

#include <private/jsondb-strings_p.h>
#include <private/jsondb-connection_p.h>
#include <jsondb-client.h>
Q_USE_JSONDB_NAMESPACE

#include "qcontactjsondbengine.h"
#include "qcontactjsondbconverter.h"
#include "qcontactjsondbglobal.h"
#include "qcontactpersonid.h"

#include <QDebug>
#include <QEventLoop>

QTCONTACTS_BEGIN_NAMESPACE

Q_DEFINE_LATIN1_CONSTANT (QContactJsonDbEngine::EngineName, "jsondb");

/*!
  \class QContactJsonDbEngine
  \brief The QContactJsonDbEngine class provides an implementation of
  QContactManagerEngine whose functions always return an error.

  The JsonDb engine.
 */

/*! Constructs a new invalid contacts backend. */

QContactJsonDbEngine::QContactJsonDbEngine() : d(new QContactJsonDbEngineData)
{
    qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
    qRegisterMetaType<QList<QContactId> >("QList<QContactId>");
    qRegisterMetaType<QList<QContactLocalId> >("QList<QContactLocalId>");
    m_thread = new QThread();
    m_thread->start();
    connect(this, SIGNAL(requestReceived(QContactAbstractRequest*)),
            d->m_requestHandler, SLOT(handleRequest(QContactAbstractRequest*)));
    d->m_requestHandler->moveToThread(m_thread);
    QMetaObject::invokeMethod(d->m_requestHandler,"init",Qt::BlockingQueuedConnection);
    d->m_requestHandler->setEngine(this);
}





QContactJsonDbEngine::~QContactJsonDbEngine()
{

    if (m_thread) {
        m_thread->quit();
        m_thread->wait();
        m_thread->deleteLater();
    }

}





/*! \reimp */
QContactJsonDbEngine& QContactJsonDbEngine::operator=(const QContactJsonDbEngine& other)
{
    d = other.d;
    return *this;
}





bool QContactJsonDbEngine::startRequest(QContactAbstractRequest* req){
    QContactManagerEngine::updateRequestState(req, QContactAbstractRequest::ActiveState);
    connect(req, SIGNAL(destroyed(QObject*)), d->m_requestHandler, SLOT(removeDestroyed(QObject*)),Qt::QueuedConnection);
    emit requestReceived(req);
    return true;
}





/*! \reimp */
QString QContactJsonDbEngine::managerName() const
{
    return QString(EngineName.latin1());
}





bool QContactJsonDbEngine::validateContact(const QContact& contact, QContactManager::Error* error) const
{
    return QContactManagerEngine::validateContact(contact, error);
}

QContact QContactJsonDbEngine::compatibleContact(const QContact& contact, QContactManager::Error* error) const
{
    return QContactManagerEngine::compatibleContact(contact, error);
}





QContactLocalId QContactJsonDbEngine::selfContactId(QContactManager::Error* error) const
{
    // TODO: THE IDENTIFICATION FIELD DOES NOT EXIST YET IN JSON SCHEMA!
    // Just return "NotSupported" error
    *error = QContactManager::NotSupportedError;
    return QContactLocalId("");
}





QList<QContactLocalId> QContactJsonDbEngine::contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const
{
    QContactJsonDbConverter converter;
    QList<QContactLocalId> contactIds;
    QVariantMap map;
    QContactFetchRequest request;
    request.setFilter(filter);
    request.setSorting(sortOrders);
    *error = QContactManager::NoError;
    //QString query = converter.queryFromRequest(&request);
    doSyncRequest(&request, 5000);
    *error = request.error();
    if (*error != QContactManager::NoError) {
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
        return QList<QContactLocalId>();
    }
    QList<QContact> queryResults = (QList<QContact>)request.contacts();
    // found any results?
    if(queryResults.size() == 0) {
        *error = QContactManager::DoesNotExistError;
        qDebug() << "Error by function contactIds: no contacts found (DoesNotExistError)";
        return QList<QContactLocalId>();
    }
    // Convert results for needed format
    QList<QContactLocalId> results;
    QString id;

    foreach (QContact contact, queryResults) {
        results.append(contact.localId());
    }
    return results;
}





QList<QContact> QContactJsonDbEngine::contacts(const QContactFilter & filter, const QList<QContactSortOrder> & sortOrders, const QContactFetchHint & fetchHint, QContactManager::Error* error ) const
{
  // TODO: ERROR HANDLING  (?)
  QList<QContact> contacts;
  QContactJsonDbConverter converter;
  QContactFetchRequest fetchReq;
  fetchReq.setFilter(filter);
  fetchReq.setSorting(sortOrders);
  fetchReq.setFetchHint(fetchHint);
  *error = QContactManager::NoError;
  //QString query = converter.queryFromRequest(&fetchReq);
  doSyncRequest(&fetchReq, 5000);
  *error = fetchReq.error();
  if (*error != QContactManager::NoError) {
      if (qt_debug_jsondb_contacts())
          qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
      return QList<QContact>();
  }
  QList<QContact> queryResults = (QList<QContact>)fetchReq.contacts();
  // found any results?
  if(queryResults.size() == 0) {
      *error = QContactManager::DoesNotExistError;
      qDebug() << "Error by function contacts: no contacts found (DoesNotExistError)";
      return QList<QContact>();
  }
  /*
  else {
        converter.toQContacts(jsonDbObjectList, contacts, *this, *error);
  }
  */
  return queryResults;
}




QContact QContactJsonDbEngine::contact(const QContactLocalId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    QContact contact;
    QContactJsonDbConverter converter;
    QContactFetchRequest request;
    QList<QContactLocalId> filterIds;
    QContactLocalIdFilter idFilter;
    QString query;
    QVariantList results;

    filterIds.append(contactId);
    idFilter.setIds(filterIds);
    request.setFilter(idFilter);
    request.setFetchHint(fetchHint);
    *error = QContactManager::NoError;

    //query = converter.queryFromRequest(&request);
    doSyncRequest(&request, 5000);
    *error = request.error();
    if (*error != QContactManager::NoError) {
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
        return QContact();
    }
    QList<QContact> queryResults = (QList<QContact>)request.contacts();
    // Check if query returned a value and it can be converted
    if(queryResults.size() == 0) {
        *error = QContactManager::DoesNotExistError;
        qDebug() << "Error by function contact: no contact found (DoesNotExistError)";
        return QContact();
    }

    // Extract the desired results
    foreach (QContact curr, queryResults) {
        if (curr.localId() == contactId) contact = curr;  ;
    }
    return contact;
}




bool QContactJsonDbEngine::saveContacts(QList<QContact>* contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    QContactSaveRequest saveReq;
    *error = QContactManager::NoError;

    saveReq.setContacts(*contacts);
    doSyncRequest(&saveReq, 5000);
    *error = saveReq.error();
    if (*error != QContactManager::NoError) {
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
        return false;
    }

    for (int i = 0; i < saveReq.contacts().size(); i++)
        contacts->replace(i, saveReq.contacts()[i]);
    *errorMap = saveReq.errorMap();
    *error = saveReq.error();
    return *error == QContactManager::NoError;  // No problem detected, return NoError
}





bool QContactJsonDbEngine::removeContacts(const QList<QContactLocalId>& ids, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(errorMap);
    QContactRemoveRequest removeReq;
    *error = QContactManager::NoError;
    removeReq.setContactIds(ids);
    doSyncRequest(&removeReq, 5000);
    *error = removeReq.error();
    if (*error != QContactManager::NoError) {
        qDebug() << "Error at function removeContacts:" << *error;
        return false;
    } else {
        return true;
    }
}

bool QContactJsonDbEngine::saveContact(QContact* contact, QContactManager::Error* error)
{
   QContactSaveRequest saveReq;
   *error = QContactManager::NoError;

   saveReq.setContact(*contact);
   doSyncRequest(&saveReq, 5000);
   *error = saveReq.error();
   if (*error != QContactManager::NoError) {
       if (qt_debug_jsondb_contacts())
           qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
       return false;
   }
   *contact = saveReq.contacts().first();  // Check if this is the desired behavior !!!
   return *error == QContactManager::NoError;  // No problem detected, return NoError
}



bool QContactJsonDbEngine::removeContact(const QContactLocalId& contactId, QContactManager::Error* error)
{
    Q_UNUSED(contactId)
    Q_UNUSED(error)

    QContactRemoveRequest removeReq;
    *error = QContactManager::NoError;
    removeReq.setContactId(contactId);
    doSyncRequest(&removeReq, 5000);
    *error = removeReq.error();
    if (*error != QContactManager::NoError) {
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
        return false;
    }
    else return true;
}

bool QContactJsonDbEngine::hasFeature(QContactManager::ManagerFeature feature, const QString& contactType) const {
  if (!supportedContactTypes().contains(contactType)) {
        return false;
  };
  switch (feature) {
    case QContactManager::Anonymous:
    case QContactManager::ChangeLogs:
    case QContactManager::DetailOrdering:
    case QContactManager::SelfContact:
        return true;
    default:
        return false;
  };
}

bool QContactJsonDbEngine::isFilterSupported(const QContactFilter& filter) const {
  switch (filter.type()) {
    case QContactFilter::InvalidFilter:
    case QContactFilter::DefaultFilter:
    case QContactFilter::LocalIdFilter:
    case QContactFilter::ContactDetailFilter:
    case QContactFilter::ActionFilter:
    case QContactFilter::IntersectionFilter:
    case QContactFilter::UnionFilter:
      return true;
    default:
      return false;
  }
}

QList<QVariant::Type> QContactJsonDbEngine::supportedDataTypes() const {
  QList<QVariant::Type> st;
  st.append(QVariant::String);
  st.append(QVariant::Int);
  st.append(QVariant::UInt);
  st.append(QVariant::Double);
  st.append(QVariant::Date);
  st.append(QVariant::DateTime);
  st.append(QVariant::Bool);
  st.append(QVariant::Url);
  return st;
}

void QContactJsonDbEngine::requestDestroyed(QContactAbstractRequest* req){
   //We inform the handler that this request is about to be destroyed so as to
   //avoid that the worker handler thread will start handling request objects during
   //their destruction.
   QMetaObject::invokeMethod(d->m_requestHandler,"removeDestroyed",Qt::BlockingQueuedConnection,Q_ARG(QObject*, req));
   return QContactManagerEngine::requestDestroyed(req);
}

bool QContactJsonDbEngine::cancelRequest(QContactAbstractRequest* req){
    /*
        TODO

        Cancel an in progress async request.  If not possible, return false from here.
    */
    return QContactManagerEngine::cancelRequest(req);
}

bool QContactJsonDbEngine::waitForRequestProgress(QContactAbstractRequest* req, int msecs){
  Q_UNUSED(msecs);
  Q_UNUSED(req);
  //TODO: can we get progress info from jsondb??

  return true;
}

bool QContactJsonDbEngine::waitForRequestFinished(QContactAbstractRequest* req, int msecs){
    bool result = false;
    result = d->m_requestHandler->waitForRequestFinished(req, msecs);
    return result;
}

bool QContactJsonDbEngine::doSyncRequest(QContactAbstractRequest* req, int msecs) const  {
    Q_UNUSED(msecs); // TODO
    //if (req->ContactFetchRequest)
    QVariantList Idlist;
    const_cast<QContactJsonDbEngine*>(this)->startRequest(req);
    const_cast<QContactJsonDbEngine*>(this)->waitForRequestFinished(req, 0);
    //if (req->FinishedState)
    if (req->isFinished() == true)
        return true;
    else
        return false;
}


/* Internal, for debugging */
bool qt_debug_jsondb_contacts()
{
    static int debug_env = -1;
    if (debug_env == -1)
       debug_env = QT_PREPEND_NAMESPACE(qgetenv)("QT_DEBUG_JSONDB_CONTACTS").toInt();

    return debug_env != 0;
}

#include "moc_qcontactjsondbengine.cpp"

QTCONTACTS_END_NAMESPACE
