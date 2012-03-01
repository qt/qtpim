/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtContacts module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QVariantMap>
#include <QEventLoop>

#include "qcontactjsondbengine.h"
#include "qcontactjsondbconverter.h"
#include "qcontactjsondbglobal.h"
#include "qcontactpersonid.h"
#include "qcontactjsondbstring.h"

#include <QDebug>
#include <QEventLoop>

QTCONTACTS_BEGIN_NAMESPACE

/*
  class QContactJsonDbEngine
  \brief The QContactJsonDbEngine class provides an implementation of
  QContactManagerEngine whose functions always return an error.

  The JsonDb engine.
 */

QContactJsonDbEngine::QContactJsonDbEngine() : d(new QContactJsonDbEngineData)
{
    qRegisterMetaType<QContactAbstractRequest::State>("QContactAbstractRequest::State");
    qRegisterMetaType<QList<QContactId> >("QList<QContactId>");
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





/* \reimp */
QString QContactJsonDbEngine::managerName() const
{
    return QContactJsonDbStr::contactJsonDbEngineName();
}

QList<QContactDetail::DetailType> QContactJsonDbEngine::supportedContactDetailTypes() const
{
    QList<QContactDetail::DetailType> supportedDetails;
    supportedDetails << QContactAddress::Type
                     << QContactAvatar::Type
                     << QContactBirthday::Type
                     << QContactDisplayLabel::Type
                     << QContactEmailAddress::Type
                     << QContactExtendedDetail::Type
                     << QContactGender::Type
                     << QContactGuid::Type
                     << QContactName::Type
                     << QContactNickname::Type
                     << QContactNote::Type
                     << QContactOrganization::Type
                     << QContactPersonId::Type
                     << QContactPhoneNumber::Type
                     << QContactRingtone::Type
                     << QContactSyncTarget::Type
                     << QContactType::Type
                     << QContactUrl::Type
                     << QContactVersion::Type;
    return supportedDetails;
}



bool QContactJsonDbEngine::validateContact(const QContact& contact, QContactManager::Error* error) const
{
    QContactManagerEngine::validateContact(contact, error);
    if ((*error == QContactManager::InvalidContactTypeError) || (*error == QContactManager::DoesNotExistError))
        return false;

    QList<QContactDetail> contactDetailList = contact.details();

    for (int i=0; i<contactDetailList.count(); i++)
    {
        QContactDetail currentDetail = contactDetailList.value(i);
        if (!supportedContactDetailTypes().contains(currentDetail.type()))
        {
            *error = QContactManager::InvalidDetailError;
            return false;
        }
    }

    *error = QContactManager::NoError;
    return true;
}


QContactId QContactJsonDbEngine::selfContactId(QContactManager::Error* error) const
{
    // TODO: THE IDENTIFICATION FIELD DOES NOT EXIST YET IN JSON SCHEMA!
    // Just return "NotSupported" error
    *error = QContactManager::NotSupportedError;
    return QContactId();
}


QString QContactJsonDbEngine::synthesizedDisplayLabel(const QContact &contact, QContactManager::Error *error) const
{
    // synthesize the display name from the name of the contact, or, failing that, the organisation of the contact.
    *error = QContactManager::NoError;
    QList<QContactDetail> allNames = contact.details(QContactName::Type);
    const QLatin1String space(" ");

    // synthesize the display label from the name.
    for (int i=0; i < allNames.size(); i++) {
        const QContactName& name = allNames.at(i);

        QString result;
        if (!name.value(QContactName::FieldPrefix).toString().trimmed().isEmpty()) {
           result += name.value(QContactName::FieldPrefix).toString();
        }

        if (!name.value(QContactName::FieldFirstName).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldFirstName).toString();
        }

        if (!name.value(QContactName::FieldMiddleName).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldMiddleName).toString();
        }

        if (!name.value(QContactName::FieldLastName).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldLastName).toString();
        }

        if (!name.value(QContactName::FieldSuffix).toString().trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldSuffix).toString();
        }

        if (!result.isEmpty()) {
            return result;
        }
    }

    /* Well, we had no non empty names. if we have orgs, fall back to those */
    QList<QContactDetail> allOrgs = contact.details(QContactOrganization::Type);
    for (int i=0; i < allOrgs.size(); i++) {
        const QContactOrganization& org = allOrgs.at(i);
        if (!org.name().isEmpty()) {
            return org.name();
        }
    }

    return QString();
}



QList<QContactId> QContactJsonDbEngine::contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const
{
    QContactJsonDbConverter converter;
    QList<QContactId> contactIds;
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
        return QList<QContactId>();
    }
    QList<QContact> queryResults = (QList<QContact>)request.contacts();
    // found any results?
    if(queryResults.size() == 0) {
        *error = QContactManager::DoesNotExistError;
        qDebug() << "Error by function contactIds: no contacts found (DoesNotExistError)";
        return QList<QContactId>();
    }
    // Convert results for needed format
    QList<QContactId> results;

    foreach (const QContact &contact, queryResults)
        results.append(contact.id());

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




QContact QContactJsonDbEngine::contact(const QContactId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    QContact contact;
    QContactJsonDbConverter converter;
    QContactFetchRequest request;
    QList<QContactId> filterIds;
    QContactIdFilter idFilter;
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
        if (curr.id() == contactId) contact = curr;  ;
    }
    return contact;
}




bool QContactJsonDbEngine::saveContacts(QList<QContact>* contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    QContactSaveRequest saveReq;

    saveReq.setContacts(*contacts);
    doSyncRequest(&saveReq, 5000);
    *error = saveReq.error();
    if (*error != QContactManager::NoError) {
        if (qt_debug_jsondb_contacts())
            qDebug() << "[QContactJsonDb] Error at " << Q_FUNC_INFO << ":" << *error;
    }

    for (int i = 0; i < saveReq.contacts().size(); i++)
        contacts->replace(i, saveReq.contacts()[i]);
    *errorMap = saveReq.errorMap();
    return *error == QContactManager::NoError;  // No problem detected, return NoError
}





bool QContactJsonDbEngine::removeContacts(const QList<QContactId>& ids, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    QContactRemoveRequest removeReq;
    removeReq.setContactIds(ids);
    doSyncRequest(&removeReq, 5000);
    *error = removeReq.error();
    *errorMap = removeReq.errorMap();
    if (*error != QContactManager::NoError) {
        qWarning() << "Error at function removeContacts:" << *error;
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



bool QContactJsonDbEngine::removeContact(const QContactId& contactId, QContactManager::Error* error)
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

bool QContactJsonDbEngine::isFilterSupported(const QContactFilter& filter) const
{
    switch (filter.type()) {
    case QContactFilter::ContactDetailFilter: {
        QContactDetailFilter detailFilter = static_cast<QContactDetailFilter>(filter);
        int field = detailFilter.detailField();
        if (field < 0)
            return false;
        switch (detailFilter.detailType()) {
        case QContactDetail::TypeEmailAddress:
            if (field != QContactEmailAddress::FieldEmailAddress)
                return false;
        case QContactDetail::TypePhoneNumber:
            if (field != QContactPhoneNumber::FieldNumber)
                return false;
        case QContactDetail::TypeUrl:
            if (field != QContactUrl::FieldUrl)
                return false;
        case QContactDetail::TypeName:
            return true;
        default:
            return false;
        };
        return false;
    }
    case QContactFilter::InvalidFilter:
    case QContactFilter::DefaultFilter:
    case QContactFilter::IdFilter:
    case QContactFilter::IntersectionFilter:
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
