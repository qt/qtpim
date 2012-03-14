/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#include <qcontactdetails.h>
#include <qcontactmanager.h>
#include "qdeclarativecontact_p.h"
#include "qdeclarativecontactmodel_p.h"

#include <QtQml/qqmlengine.h>

#include <QImage>
#include <QUrl>
#include <QQmlListProperty>

/*!
    \qmlclass Contact QDeclarativeContact
    \brief The Contact element represents an addressbook contact.
    \ingroup qml-contacts-main
    \inqmlmodule QtContacts

    The Contact element is part of the \b{QtContacts} module.

    A Contact object has a collection of details (like a name, phone numbers and
    email addresses).  Each detail (which can have multiple fields) is stored
    in an appropriate subclass of ContactDetail, and the Contact allows
    retrieving these details in various ways.

    If some of the contact details are not unique details, all of this type of detail values
    can be accessed by the property with the name in plural. For example, if there are 3 phone numbers stored in
    a contact, they can be accessed by contact.phoneNumbers property, which holds a list of
    all PhoneNumber details. If a contact does not contain a detail of particular type,
    the value of the corresponding singular property (e.g. phoneNumber) in undefined and the
    plural (e.g. phoneNumbers) is empty. The list of properties which support multiple detail
    instances depends on the contact engine implementations.

    \sa QContact
*/


QTCONTACTS_BEGIN_NAMESPACE


// call-back function templates for list properties
template <typename T, QDeclarativeContactDetail::DetailType detailType>
static void list_property_append(QQmlListProperty<T> *, T *)
{
}

template <typename T, QDeclarativeContactDetail::DetailType detailType>
static int list_property_count(QQmlListProperty<T> *property)
{
    QDeclarativeContact *object = qobject_cast<QDeclarativeContact *>(property->object);
    if (object)
        return object->details(detailType).size();
    else
        return 0;
}

template <typename T, QDeclarativeContactDetail::DetailType detailType>
static T *list_property_at(QQmlListProperty<T> *property, int index)
{
    QDeclarativeContact *object = qobject_cast<QDeclarativeContact *>(property->object);
    if (object)
        return qobject_cast<T *>(qvariant_cast<QObject*>(object->details(detailType).at(index)));
    else
        return 0;
}

template <typename T, QDeclarativeContactDetail::DetailType detailType>
static void list_property_clear(QQmlListProperty<T> *)
{
}

QDeclarativeContact::QDeclarativeContact(QObject *parent)
    :QObject(parent)
    , m_modified(false)
{
    connect(this, SIGNAL(contactChanged()), SLOT(setModified()));
}

QDeclarativeContact::~QDeclarativeContact()
{
    clearDetails();
}

void QDeclarativeContact::setContact(const QContact& contact)
{
    m_id = contact.id();
    foreach (QDeclarativeContactDetail *detail, m_details)
        delete detail;
    m_details.clear();
    QList<QContactDetail> details(contact.details());
    foreach (const QContactDetail &detail, details) {
        QDeclarativeContactDetail *contactDetail = QDeclarativeContactDetailFactory::createContactDetail(static_cast<QDeclarativeContactDetail::DetailType>(detail.type()));
        contactDetail->setDetail(detail);
        connect(contactDetail, SIGNAL(detailChanged()), this, SIGNAL(contactChanged()));
        m_details.append(contactDetail);
    }

    m_modified = false;
    emit contactChanged();
}

QContact QDeclarativeContact::contact() const
{
    QContact contact;
    contact.setId(m_id);
    foreach (QDeclarativeContactDetail *detail, m_details)
        contact.saveDetail(&detail->detail());
    return contact;
}

/*!
    \qmlproperty bool Contact::modified

    This property holds the dirty flag of the Contact object.
    If the Contact has been changed, returns true, otherwise returns false.
*/
bool QDeclarativeContact::modified() const
{
     return m_modified;
}

void QDeclarativeContact::setModified()
{
    m_modified = true;
}

/*!
    \qmlproperty enumeration Contact::type

    This property holds type of the Contact, the value can be one of:

    \list
    \li Contact.Contact
    \li Contact.Group
    \endlist
*/
QDeclarativeContactType::ContactType QDeclarativeContact::type() const
{
    foreach (QDeclarativeContactDetail *detail, m_details) {
        if (QDeclarativeContactDetail::Type == detail->detailType())
           return static_cast<QDeclarativeContactType *>(detail)->type();
    }
    return QDeclarativeContactType::Contact;
}

/*!
    \qmlmethod Contact::removeDetail(detail)

    Removes the given contact \a detail from the contact, returns true if successful, otherwise returns false.
*/

bool QDeclarativeContact::removeDetail(QDeclarativeContactDetail* detail)
{
    if (detail) {
        if (!detail->removable())
            return false;
        int key = detail->detail().key();
        int i = 0;
        foreach (QDeclarativeContactDetail *contactDetail, m_details) {
            if (key == contactDetail->detail().key()) {
                delete contactDetail;
                m_details.removeAt(i);
                emit contactChanged();
                return true;
            }
        ++i;
        }
    }
    return false;
}

/*!
    \qmlmethod Contact::addDetail(detail)

    Adds the given contact \a detail to the contact, returns true if successful, otherwise returns false.

    Note: If the \a detail has been added into the same contact before, this operation will be ignored,
    so if you want to add a \a detail multiple times, the \a detail should be copied before calling this function.
*/
bool QDeclarativeContact::addDetail(QDeclarativeContactDetail* detail)
{
    if (!detail || m_details.contains(detail))
        return false;

    QDeclarativeContactDetail *contactDetail = QDeclarativeContactDetailFactory::createContactDetail(detail->detailType());
    contactDetail->setDetail(detail->detail());
    connect(contactDetail, SIGNAL(detailChanged()), this, SIGNAL(contactChanged()));
    m_details.append(contactDetail);

    m_modified = true;
    emit contactChanged();
    return true;
}

/*!
    \qmlproperty list<ContactDetail> Contact::contactDetails

    This property holds the list of all the details that the contact has.
*/
QQmlListProperty<QDeclarativeContactDetail> QDeclarativeContact::contactDetails()
{
    return QQmlListProperty<QDeclarativeContactDetail>(this, 0,
                                                                     &QDeclarativeContact::_q_detail_append,
                                                                     &QDeclarativeContact::_q_detail_count,
                                                                     &QDeclarativeContact::_q_detail_at,
                                                                     &QDeclarativeContact::_q_detail_clear);
}

/*!
    \qmlproperty int Contact::contactId

    This property holds the id of the Contact object.
    This property is read only.
*/
QString QDeclarativeContact::contactId() const
{
    return m_id.toString();
}

/*!
    \qmlproperty string Contact::manager

    This property holds the manager name which the Contact object comes from.
*/
QString QDeclarativeContact::manager() const
{
    return m_id.managerUri();
}

/*!
    \qmlmethod QDeclarativeContactDetail* QDeclarativeContact::detail(int type)

    Returns contactDetail object which detail name or detail type is \a name.
*/
QDeclarativeContactDetail* QDeclarativeContact::detail(int type)
{
    foreach (QDeclarativeContactDetail *detail, m_details) {
        if (type == detail->detailType()) {
            return detail;
        }
    }
    return 0;
}

/*!
    \qmlmethod QVariantList QDeclarativeContact::details(int type)

    Returns a list of ContactDetail objects which detail name or detail type is \a name.
*/
QVariantList QDeclarativeContact::details(int type)
{
    QVariantList list;
    foreach (QDeclarativeContactDetail *detail, m_details) {
        if (type == detail->detailType()) {
            list.append(QVariant::fromValue((QObject*)detail));
        }
    }
    return list;
}

/*!
    \qmlmethod Contact::clearDetails()

    Remove all detail objects in this contact.
*/
void QDeclarativeContact::clearDetails()
{
    if (m_details.isEmpty())
        return;

    foreach (QDeclarativeContactDetail *detail, m_details)
        delete detail;
    m_details.clear();
    m_modified = true;
    emit contactChanged();
}


/*!
    \qmlmethod Contact::save()

    Saves this Contact if the contact has been modified.

    \sa  Contact::modified
*/
void QDeclarativeContact::save()
{
    if (modified()) {
        QDeclarativeContactModel* model = qobject_cast<QDeclarativeContactModel*>(parent());
        if (model) {
            model->saveContact(this);
            m_modified = false;
        }
    }
}

// convenient access to most frequently used details
/*!
    \qmlproperty Address Contact::address

    This property holds the address detail of the Contact object. In case a contact has several addresses then
    the first one is returned.
*/
QDeclarativeContactAddress* QDeclarativeContact::address()
{
    return getDetail<QDeclarativeContactAddress>(QDeclarativeContactDetail::Address);
}

/*!
    \qmlproperty list<Address> Contact::addresses

    This property holds the address details of the Contact object.
*/
QQmlListProperty<QDeclarativeContactAddress> QDeclarativeContact::addresses()
{
    return QQmlListProperty<QDeclarativeContactAddress>(
                this,
                0,
                &list_property_append<QDeclarativeContactAddress, QDeclarativeContactDetail::Address>,
                &list_property_count<QDeclarativeContactAddress, QDeclarativeContactDetail::Address>,
                &list_property_at<QDeclarativeContactAddress, QDeclarativeContactDetail::Address>,
                &list_property_clear<QDeclarativeContactAddress, QDeclarativeContactDetail::Address>);
}

/*!
    \qmlproperty Anniversary Contact::anniversary

    This property holds the anniversary detail of the Contact object.
*/
QDeclarativeContactAnniversary* QDeclarativeContact::anniversary()
{
    return getDetail<QDeclarativeContactAnniversary>(QDeclarativeContactDetail::Anniversary);
}

/*!
    \qmlproperty Avatar Contact::avatar

    This property holds the avatar detail of the Contact object.
*/
QDeclarativeContactAvatar* QDeclarativeContact::avatar()
{
    return getDetail<QDeclarativeContactAvatar>(QDeclarativeContactDetail::Avatar);
}

/*!
    \qmlproperty Birthday Contact::birthday

    This property holds the birthday detail of the Contact object.
*/
QDeclarativeContactBirthday*  QDeclarativeContact::birthday()
{
    return getDetail<QDeclarativeContactBirthday>(QDeclarativeContactDetail::Birthday);
}

/*!
    \qmlproperty string Contact::displayLabel

    This property holds the display label value of the Contact object.
*/
QString QDeclarativeContact::displayLabel()
{
    QDeclarativeContactDisplayLabel* tempDisplayLabel = getDetail<QDeclarativeContactDisplayLabel>(QDeclarativeContactDetail::DisplayLabel);
    if (tempDisplayLabel) {
        return tempDisplayLabel->label();
    }
    return QString::null;
}

/*!
    \qmlproperty EmailAddress Contact::email

    This property holds the email address detail of the Contact object. In case a contact has several email addresses then
    the first one is returned.
*/
QDeclarativeContactEmailAddress*  QDeclarativeContact::email()
{
    return getDetail<QDeclarativeContactEmailAddress>(QDeclarativeContactDetail::Email);
}

/*!
    \qmlproperty list<EmailAddress> Contact::emails

    This property holds the email address details of the Contact object.
*/
QQmlListProperty<QDeclarativeContactEmailAddress> QDeclarativeContact::emails()
{
    return QQmlListProperty<QDeclarativeContactEmailAddress>(
                this,
                0,
                &list_property_append<QDeclarativeContactEmailAddress, QDeclarativeContactDetail::Email>,
                &list_property_count<QDeclarativeContactEmailAddress, QDeclarativeContactDetail::Email>,
                &list_property_at<QDeclarativeContactEmailAddress, QDeclarativeContactDetail::Email>,
                &list_property_clear<QDeclarativeContactEmailAddress, QDeclarativeContactDetail::Email>);
}

/*!
    \qmlproperty Family Contact::family

    This property holds the family detail of the Contact object.
*/
QDeclarativeContactFamily*  QDeclarativeContact::family()
{
    return getDetail<QDeclarativeContactFamily>(QDeclarativeContactDetail::Family);
}

/*!
    \qmlproperty Favorite Contact::favorite

    This property holds the favorite detail of the Contact object.
*/
QDeclarativeContactFavorite*  QDeclarativeContact::favorite()
{
    return getDetail<QDeclarativeContactFavorite>(QDeclarativeContactDetail::Favorite);
}

/*!
    \qmlproperty Gender Contact::gender

    This property holds the gender detail of the Contact object.
*/
QDeclarativeContactGender*  QDeclarativeContact::gender()
{
    return getDetail<QDeclarativeContactGender>(QDeclarativeContactDetail::Gender);
}

/*!
    \qmlproperty GeoLocation Contact::geolocation

    This property holds the geolocation detail of the Contact object.
*/
QDeclarativeContactGeoLocation*  QDeclarativeContact::geolocation()
{
    return getDetail<QDeclarativeContactGeoLocation>(QDeclarativeContactDetail::Geolocation);
}

/*!
    \qmlproperty GlobalPresence Contact::globalPresence

    This property holds the globalPresence detail of the Contact object.
*/
QDeclarativeContactGlobalPresence*  QDeclarativeContact::globalPresence()
{
    return getDetail<QDeclarativeContactGlobalPresence>(QDeclarativeContactDetail::GlobalPresence);
}

/*!
    \qmlproperty Guid Contact::guid

    This property holds the guid detail of the Contact object.
*/
QDeclarativeContactGuid*  QDeclarativeContact::guid()
{
    return getDetail<QDeclarativeContactGuid>(QDeclarativeContactDetail::Guid);
}

/*!
    \qmlproperty Name Contact::name

    This property holds the name detail of the Contact object.
*/
QDeclarativeContactName*  QDeclarativeContact::name()
{
   return getDetail<QDeclarativeContactName>(QDeclarativeContactDetail::Name);
}

/*!
    \qmlproperty Nickname Contact::nickname

    This property holds the nickname detail of the Contact object.
*/
QDeclarativeContactNickname*  QDeclarativeContact::nickname()
{
    return getDetail<QDeclarativeContactNickname>(QDeclarativeContactDetail::NickName);
}

/*!
    \qmlproperty Note Contact::note

    This property holds the note detail of the Contact object.
*/
QDeclarativeContactNote*  QDeclarativeContact::note()
{
    return getDetail<QDeclarativeContactNote>(QDeclarativeContactDetail::Note);
}

/*!
    \qmlproperty OnlineAccount Contact::onlineAccount

    This property holds the onlineAccount detail of the Contact object. In case a contact has several accounts then
    the first one is returned.
*/
QDeclarativeContactOnlineAccount*  QDeclarativeContact::onlineAccount()
{
    return getDetail<QDeclarativeContactOnlineAccount>(QDeclarativeContactDetail::OnlineAccount);
}

/*!
    \qmlproperty Organization Contact::organization

    This property holds the organization detail of the Contact object.
*/
QDeclarativeContactOrganization*  QDeclarativeContact::organization()
{
    return getDetail<QDeclarativeContactOrganization>(QDeclarativeContactDetail::Organization);
}

/*!
    \qmlproperty list<Organization> Contact::organizations

    This property holds the organization details of the Contact object.
*/
QQmlListProperty<QDeclarativeContactOrganization> QDeclarativeContact::organizations()
{
    return QQmlListProperty<QDeclarativeContactOrganization>(
                this,
                0,
                &list_property_append<QDeclarativeContactOrganization, QDeclarativeContactDetail::Organization>,
                &list_property_count<QDeclarativeContactOrganization, QDeclarativeContactDetail::Organization>,
                &list_property_at<QDeclarativeContactOrganization, QDeclarativeContactDetail::Organization>,
                &list_property_clear<QDeclarativeContactOrganization, QDeclarativeContactDetail::Organization>);
}

/*!
    \qmlproperty PhoneNumber Contact::phoneNumber

    This property holds the phone number detail of the Contact object. In case a contact has several numbers then
    the first one is returned.
*/
QDeclarativeContactPhoneNumber*  QDeclarativeContact::phoneNumber()
{
    return getDetail<QDeclarativeContactPhoneNumber>(QDeclarativeContactDetail::PhoneNumber);
}

/*!
    \qmlproperty list<PhoneNumber> Contact::phoneNumbers

    This property holds the phone number details of the Contact object.
*/
QQmlListProperty<QDeclarativeContactPhoneNumber> QDeclarativeContact::phoneNumbers()
{
    return QQmlListProperty<QDeclarativeContactPhoneNumber>(
                this,
                0,
                &list_property_append<QDeclarativeContactPhoneNumber, QDeclarativeContactDetail::PhoneNumber>,
                &list_property_count<QDeclarativeContactPhoneNumber, QDeclarativeContactDetail::PhoneNumber>,
                &list_property_at<QDeclarativeContactPhoneNumber, QDeclarativeContactDetail::PhoneNumber>,
                &list_property_clear<QDeclarativeContactPhoneNumber, QDeclarativeContactDetail::PhoneNumber>);
}

/*!
    \qmlproperty Presence Contact::presence

    This property holds the presence detail of the Contact object.
*/
QDeclarativeContactPresence*  QDeclarativeContact::presence()
{
    return getDetail<QDeclarativeContactPresence>(QDeclarativeContactDetail::Presence);
}

/*!
    \qmlproperty Ringtone Contact::ringtone

    This property holds the ringtone detail of the Contact object.
*/
QDeclarativeContactRingtone*  QDeclarativeContact::ringtone()
{
    return getDetail<QDeclarativeContactRingtone>(QDeclarativeContactDetail::Ringtone);
}

/*!
    \qmlproperty SyncTarget Contact::syncTarget

    This property holds the syncTarget detail of the Contact object.
*/
QDeclarativeContactSyncTarget*  QDeclarativeContact::syncTarget()
{
    return getDetail<QDeclarativeContactSyncTarget>(QDeclarativeContactDetail::SyncTarget);
}

/*!
    \qmlproperty Tag Contact::tag

    This property holds the tag detail of the Contact object.
*/
QDeclarativeContactTag*  QDeclarativeContact::tag()
{
    return getDetail<QDeclarativeContactTag>(QDeclarativeContactDetail::Tag);
}

/*!
    \qmlproperty Url Contact::url

    This property holds the url detail of the Contact object.
*/
QDeclarativeContactUrl*  QDeclarativeContact::url()
{
    return getDetail<QDeclarativeContactUrl>(QDeclarativeContactDetail::Url);
}

/*!
    \qmlproperty list<Url> Contact::urls

    This property holds the url details of the Contact object.
*/
QQmlListProperty<QDeclarativeContactUrl> QDeclarativeContact::urls()
{
    return QQmlListProperty<QDeclarativeContactUrl>(
                this,
                0,
                &list_property_append<QDeclarativeContactUrl, QDeclarativeContactDetail::Url>,
                &list_property_count<QDeclarativeContactUrl, QDeclarativeContactDetail::Url>,
                &list_property_at<QDeclarativeContactUrl, QDeclarativeContactDetail::Url>,
                &list_property_clear<QDeclarativeContactUrl, QDeclarativeContactDetail::Url>);
}

/*!
    \qmlproperty Version Contact::version

    This property holds the version detail of the Contact object.
*/
QDeclarativeContactVersion*  QDeclarativeContact::version()
{
    return getDetail<QDeclarativeContactVersion>(QDeclarativeContactDetail::Version);
}

/*!
    \qmlproperty Hobby Contact::hobby

    This property holds the hobby detail of the Contact object.
*/
QDeclarativeContactHobby*  QDeclarativeContact::hobby()
{
    return getDetail<QDeclarativeContactHobby>(QDeclarativeContactDetail::Hobby);
}

/*!
    \qmlproperty PersonID Contact::personid

    This property holds the personid detail of the Contact object.
*/
QDeclarativeContactPersonId*  QDeclarativeContact::personid()
{
    return getDetail<QDeclarativeContactPersonId>(QDeclarativeContactDetail::PersonId);
}

/*!
    \qmlproperty ExtendedDetail Contact::extendedDetail

    This property holds the extended detail of the Contact object. In case a contact has several extended
    details then the first one is returned.
*/
QDeclarativeContactExtendedDetail*  QDeclarativeContact::extendedDetail()
{
    return getDetail<QDeclarativeContactExtendedDetail>(QDeclarativeContactDetail::ExtendedDetail);
}

/*!
    \qmlproperty list<ExtendedDetail> Contact::extendedDetails

    This property holds the extended details of the Contact object.
*/
QQmlListProperty<QDeclarativeContactExtendedDetail> QDeclarativeContact::extendedDetails()
{
    return QQmlListProperty<QDeclarativeContactExtendedDetail>(
                this,
                0,
                &list_property_append<QDeclarativeContactExtendedDetail, QDeclarativeContactDetail::ExtendedDetail>,
                &list_property_count<QDeclarativeContactExtendedDetail, QDeclarativeContactDetail::ExtendedDetail>,
                &list_property_at<QDeclarativeContactExtendedDetail, QDeclarativeContactDetail::ExtendedDetail>,
                &list_property_clear<QDeclarativeContactExtendedDetail, QDeclarativeContactDetail::ExtendedDetail>);
}

// call-back functions for list property

/*!
    \internal
 */
void QDeclarativeContact::_q_detail_append(QQmlListProperty<QDeclarativeContactDetail> *property, QDeclarativeContactDetail *value)
{
    QDeclarativeContact *object = qobject_cast<QDeclarativeContact *>(property->object);
    if (object)
    {
        object->m_details.append(value);
        value->connect(value, SIGNAL(valueChanged()), SIGNAL(detailChanged()), Qt::UniqueConnection);
        value->connect(value, SIGNAL(detailChanged()), object, SIGNAL(contactChanged()), Qt::UniqueConnection);
    }
}

/*!
    \internal
 */
QDeclarativeContactDetail *QDeclarativeContact::_q_detail_at(QQmlListProperty<QDeclarativeContactDetail> *property, int index)
{
    QDeclarativeContact *object = qobject_cast<QDeclarativeContact *>(property->object);
    if (object)
        return object->m_details.at(index);
    else
        return 0;
}

/*!
    \internal
 */
void QDeclarativeContact::_q_detail_clear(QQmlListProperty<QDeclarativeContactDetail> *property)
{
    QDeclarativeContact *object = qobject_cast<QDeclarativeContact *>(property->object);
    if (object) {
        foreach (QDeclarativeContactDetail *obj, object->m_details)
            delete obj;
        object->m_details.clear();
    }
}

/*!
    \internal
 */
int QDeclarativeContact::_q_detail_count(QQmlListProperty<QDeclarativeContactDetail> *property)
{
    QDeclarativeContact *object = qobject_cast<QDeclarativeContact *>(property->object);
    if (object)
        return object->m_details.size();
    else
        return 0;
}

#include "moc_qdeclarativecontact_p.cpp"

QTCONTACTS_END_NAMESPACE
