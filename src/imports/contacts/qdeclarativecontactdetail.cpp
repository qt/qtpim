/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
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

#include "qdeclarativecontactdetail_p.h"

#include "qdeclarativecontactdetails_p.h"
#include "qdeclarativecontact_p.h"

#include <QtQml/QJSValue>

QTCONTACTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE

/* ==================== QDeclarativeContactDetail ======================= */

/*!
   \qmltype ContactDetail
    \instantiates QDeclarativeContactDetail
   \brief The ContactDetail element represents a single, complete detail about a contact.
   \ingroup qml-contacts-main
   \inqmlmodule QtContacts

   \sa QContactDetail

    The ContactDetail element is part of the \b{QtContacts} module.
*/

QDeclarativeContactDetail::QDeclarativeContactDetail(QObject* parent)
    :QObject(parent)
{
    QDeclarativeContact* c = qobject_cast<QDeclarativeContact*>(parent);
    if (c)
        connect(this, SIGNAL(detailChanged()), c, SIGNAL(contactChanged()));
}

QDeclarativeContactDetail::~QDeclarativeContactDetail()
{
}

QContactDetail& QDeclarativeContactDetail::detail()
{
    return m_detail;
}

const QContactDetail& QDeclarativeContactDetail::detail() const
{
    return m_detail;
}

void QDeclarativeContactDetail::setDetail(const QContactDetail& detail)
{
    m_detail = detail;
    emit detailChanged();
}

/*!
  \qmlproperty list<int> ContactDetail::contexts
  This property holds one or more contexts that this detail is associated with.
*/
QList<int> QDeclarativeContactDetail::contexts() const
{
    return m_detail.contexts();
}
void QDeclarativeContactDetail::setContexts(const QList<int>& contexts)
{
    m_detail.setContexts(contexts);
}

/*!
  \qmlproperty bool ContactDetail::readOnly

  This property indicates whether or not this detail is writable.
  This property is read only.
*/
bool QDeclarativeContactDetail::readOnly() const
{
    return m_detail.accessConstraints().testFlag(QContactDetail::ReadOnly);
}

/*!
  \qmlproperty bool ContactDetail::removable

  This property indicates whether or not this detail is removale.
  This property is read only.
*/
bool QDeclarativeContactDetail::removable() const
{
    return !m_detail.accessConstraints().testFlag(QContactDetail::Irremovable);
}

/*!
  \qmlproperty string ContactDetail::detailUri

  This property holds the unique URI of the detail if one exists.
*/
QString QDeclarativeContactDetail::detailUri() const
{
    return m_detail.detailUri();
}
void QDeclarativeContactDetail::setDetailUri(const QString& detailUri)
{
    m_detail.setDetailUri(detailUri);
}

/*!
  \qmlproperty list<string> ContactDetail::linkedDetailUris

  This property holds a list of detail URIs to which this detail is linked.
*/
QStringList QDeclarativeContactDetail::linkedDetailUris() const
{
    return m_detail.linkedDetailUris();
}
void QDeclarativeContactDetail::setLinkedDetailUris(const QStringList& linkedDetailUris)
{
    m_detail.setLinkedDetailUris(linkedDetailUris);
}

/*!
    \qmlproperty enumeration ContactDetail::type

    This property holds the type of the detail.

    \list
    \li ContactDetail.Address
    \li ContactDetail.Anniversary
    \li ContactDetail.Avatar
    \li ContactDetail.Birthday
    \li ContactDetail.DisplayLabel
    \li ContactDetail.Email
    \li ContactDetail.ExtendedDetail
    \li ContactDetail.Family
    \li ContactDetail.Favorite
    \li ContactDetail.Gender
    \li ContactDetail.Geolocation
    \li ContactDetail.GlobalPresence
    \li ContactDetail.Guid
    \li ContactDetail.Hobby
    \li ContactDetail.Name
    \li ContactDetail.NickName
    \li ContactDetail.Note
    \li ContactDetail.OnlineAccount
    \li ContactDetail.Organization
    \li ContactDetail.PhoneNumber
    \li ContactDetail.Presence
    \li ContactDetail.Ringtone
    \li ContactDetail.SyncTarget
    \li ContactDetail.Tag
    \li ContactDetail.Timestamp
    \li ContactDetail.Url
    \li ContactDetail.Version
    \li ContactDetail.Unknown
    \endlist

    This property is read only.
*/
QDeclarativeContactDetail::DetailType QDeclarativeContactDetail::detailType() const
{
    return Unknown;
}

/*!
  \qmlproperty list<int> ContactDetail::fields

  This property holds the list of all fields which this detail supports.

  This property is read only.
*/
QList<int> QDeclarativeContactDetail::fields() const
{
    return m_detail.values().keys();
}

QVariant QDeclarativeContactDetail::value(int field) const
{
    return m_detail.value(field);
}

bool QDeclarativeContactDetail::setValue(int field, const QVariant& v)
{
    bool changed = false;

    if (value(field) != v)
         changed = m_detail.setValue(field, v);

    if (changed)
        emit detailChanged();

    return changed;
}

/*!
    \qmlmethod bool Detail::removeValue(field)

    Removes the value stored in this detail for the given \a field. Returns true if a value was stored for
    the given field and the operation succeeded, and false otherwise.
*/
bool QDeclarativeContactDetail::removeValue(int field)
{
    bool ok = m_detail.removeValue(field);
    if (ok)
        emit detailChanged();
    return ok;
}

QDeclarativeContactDetail *QDeclarativeContactDetailFactory::createContactDetail(QDeclarativeContactDetail::DetailType type)
{
    QDeclarativeContactDetail *contactDetail;
    if (type == QDeclarativeContactDetail::Address)
        contactDetail = new QDeclarativeContactAddress;
    else if (type == QDeclarativeContactDetail::Anniversary)
        contactDetail = new QDeclarativeContactAnniversary;
    else if (type == QDeclarativeContactDetail::Avatar)
        contactDetail = new QDeclarativeContactAvatar;
    else if (type == QDeclarativeContactDetail::Birthday)
        contactDetail = new QDeclarativeContactBirthday;
    else if (type == QDeclarativeContactDetail::DisplayLabel)
        contactDetail = new QDeclarativeContactDisplayLabel;
    else if (type == QDeclarativeContactDetail::Email)
        contactDetail = new QDeclarativeContactEmailAddress;
    else if (type == QDeclarativeContactDetail::ExtendedDetail)
        contactDetail = new QDeclarativeContactExtendedDetail;
    else if (type == QDeclarativeContactDetail::Family)
        contactDetail = new QDeclarativeContactFamily;
    else if (type == QDeclarativeContactDetail::Favorite)
        contactDetail = new QDeclarativeContactFavorite;
    else if (type == QDeclarativeContactDetail::Gender)
        contactDetail = new QDeclarativeContactGender;
    else if (type == QDeclarativeContactDetail::Geolocation)
        contactDetail = new QDeclarativeContactGeoLocation;
    else if (type == QDeclarativeContactDetail::GlobalPresence)
        contactDetail = new QDeclarativeContactGlobalPresence;
    else if (type == QDeclarativeContactDetail::Guid)
        contactDetail = new QDeclarativeContactGuid;
    else if (type == QDeclarativeContactDetail::Hobby)
        contactDetail = new QDeclarativeContactHobby;
    else if (type == QDeclarativeContactDetail::Name)
        contactDetail = new QDeclarativeContactName;
    else if (type == QDeclarativeContactDetail::NickName)
        contactDetail = new QDeclarativeContactNickname;
    else if (type == QDeclarativeContactDetail::Note)
        contactDetail = new QDeclarativeContactNote;
    else if (type == QDeclarativeContactDetail::OnlineAccount)
        contactDetail = new QDeclarativeContactOnlineAccount;
    else if (type == QDeclarativeContactDetail::Organization)
        contactDetail = new QDeclarativeContactOrganization;
    else if (type == QDeclarativeContactDetail::PhoneNumber)
        contactDetail = new QDeclarativeContactPhoneNumber;
    else if (type == QDeclarativeContactDetail::Presence)
        contactDetail = new QDeclarativeContactPresence;
    else if (type == QDeclarativeContactDetail::Ringtone)
        contactDetail = new QDeclarativeContactRingtone;
    else if (type == QDeclarativeContactDetail::SyncTarget)
        contactDetail = new QDeclarativeContactSyncTarget;
    else if (type == QDeclarativeContactDetail::Tag)
        contactDetail = new QDeclarativeContactTag;
    else if (type == QDeclarativeContactDetail::Timestamp)
        contactDetail = new QDeclarativeContactTimestamp;
    else if (type == QDeclarativeContactDetail::Type)
        contactDetail = new QDeclarativeContactType;
    else if (type == QDeclarativeContactDetail::Url)
        contactDetail = new QDeclarativeContactUrl;
    else if (type == QDeclarativeContactDetail::Version)
        contactDetail = new QDeclarativeContactVersion;
    else
        contactDetail = new QDeclarativeContactDetail;
    return contactDetail;
}

/* ==================== QDeclarativeContactAddress ======================= */

/*!
   \qmltype Address
    \instantiates QDeclarativeContactAddress
   \brief The Address element contains an address of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   The fields in the Address element are based on the segments
   of the ADR property of a Versit vCard file.

   Address element contains the following field types:
   \list
   \li Address.Street
   \li Address.Locality
   \li Address.Region
   \li Address.PostCode
   \li Address.Country
   \li Address.SubTypes
   \li Address.PostOfficeBox
   \endlist

   Versit \reg is a trademark of the Internet Mail Consortium.
   This element is part of the \b{QtContacts} module.
*/
/*!
  \qmlproperty string Address::street

  This property holds the street number and street name of the address.
*/
/*!
  \qmlproperty string Address::locality

  This property holds the name of the city, town or suburb of the address.
*/
/*!
  \qmlproperty string Address::region

  This property holds the name or identifier of the state, province or region of the address.
*/
/*!
  \qmlproperty string Address::postcode

  This property holds the postal code for the address.
*/
/*!
  \qmlproperty string Address::country

  This property holds the name of the country of the address.
*/
/*!
  \qmlproperty list<variant> Address::subTypes

  This property stores the sub types of the address.

  \list
  \li Address.Parcel - An address for parcel delivery.
  \li Address.Postal - An address for postal delivery.
  \li Address.Domestic - An address for domestic mail delivery.
  \li Address.International - An address for international mail delivery.
  \endlist
*/
/*!
  \qmlproperty string Address::postOfficeBox

  This property holds the post office box identifier of the mailing address.
*/

/* ==================== QDeclarativeContactAnniversary ======================= */

/*!
   \qmltype Anniversary
    \instantiates QDeclarativeContactAnniversary
   \brief The Anniversary element contains an anniversary of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts


   Anniversary element contains the following field types:
   \list
   \li Anniversary.CalendarId
   \li Anniversary.OriginalDate
   \li Anniversary.Event
   \li Anniversary.SubType
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Anniversary::calendarId

  This property holds the id of the calendar event.
*/
/*!
  \qmlproperty date Anniversary::originalDate

  This property holds the original anniversary date value.
  This property is either a date, or a date and time.
*/
/*!
  \qmlproperty string Anniversary::event

  This property holds the name of the event value.
*/
/*!
  \qmlproperty enumeration Anniversary::subType

  This property holds the sub type of an Anniversary.

  \list
  \li Unknown - Unknown sub type (default).
  \li Wedding - A wedding anniversary.
  \li Engagement - An engagement anniversary.
  \li House - A new residence anniversary.
  \li Employment - A start of employment anniversary.
  \li Memorial - An event of sentimental significance.
  \endlist
*/

/* ==================== QDeclarativeContactAvatar ======================= */
/*!
   \qmltype Avatar
    \instantiates QDeclarativeContactAvatar
   \brief The Avatar element contains avatar URLs of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Avatar element contains the following field types:
   \list
   \li Avatar.ImageUrl
   \li Avatar.VideoUrl
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Avatar::imageUrl

  This property holds the URL of the avatar image.
*/
/*!
  \qmlproperty string Avatar::videoUrl

  This property holds the URL of a video avatar.
*/


/* ==================== QDeclarativeContactBirthday ======================= */
/*!
   \qmltype Birthday
    \instantiates QDeclarativeContactBirthday
   \brief The Birthday element contains a birthday of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Birthday element contains the following field types:
   \list
   \li Birthday.Birthday
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty date Birthday::birthday

  This property holds the birthday date. The property value is either a date, or a date and time.
*/

/* ==================== QDeclarativeContactDisplayLabel ======================= */
/*!
   \qmltype DisplayLabel
    \instantiates QDeclarativeContactDisplayLabel
   \brief The DisplayLabel element contains a label that can be used by clients when displaying a contact, for example in a list.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   DisplayLabel element contains the following field types:
   \list
   \li DisplayLabel.Label
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string DisplayLabel::label

  This property holds the value of the display label.
*/

/* ==================== QDeclarativeContactEmailAddress ======================= */
/*!
   \qmltype EmailAddress
    \instantiates QDeclarativeContactEmailAddress
   \brief The EmailAddress element contains an email address of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   EmailAddress element contains the following field types:
   \list
   \li EmailAddress.EmailAddress
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string EmailAddress::emailAddress

  This property holds the email address value.
*/


/* ==================== QDeclarativeContactExtendedDetail ======================= */
/*!
    \qmltype ExtendedDetail
    \instantiates QDeclarativeContactExtendedDetail
    \brief The ExtendedDetail element contains an extended detail of a contact.
    \ingroup qml-contacts-details
    \inqmlmodule QtContacts

    ExtendedDetail element contains the following field types:
    \list
    \li ExtendedDetail.Name
    \li ExtendedDetail.Data
    \endlist

    This element is part of the \b{QtContacts} module.

    \sa QContactExtendedDetail
*/

/*!
    \qmlproperty string ExtendedDetail::name

    This property holds the name of the extended detail.
*/

/*!
    \qmlproperty variant ExtendedDetail::data

    This property holds the data of the extended detail.
*/

void QDeclarativeContactExtendedDetail::setData(const QVariant &newData)
{
    QVariant unboxedData(newData);
    if (newData.userType() == qMetaTypeId<QJSValue>()) {
        unboxedData = newData.value<QJSValue>().toVariant();
    }

    if (unboxedData != data() && !readOnly()) {
        detail().setValue(QContactExtendedDetail::FieldData, unboxedData);
        emit valueChanged();
    }
}

/* ==================== QDeclarativeContactFamily ======================= */
/*!
   \qmltype Family
    \instantiates QDeclarativeContactFamily
   \brief The Family element contains names of family members of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts


   Family element contains the following field types:
   \list
   \li Family.Spouse
   \li Family.Children
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Family::spouse

  This property holds the name of a spouse.
*/

/*!
  \qmlproperty list<string> Family::children

  This property holds the the names of children.
*/

/* ==================== QDeclarativeContactFavorite ======================= */
/*!
   \qmltype Favorite
    \instantiates QDeclarativeContactFavorite
   \brief The Favorite element indicates if a contact is a favorite contact as well as the
   position it should appear in an ordered list of favorites.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Favorite element contains the following field types:
   \list
   \li Favorite.Favorite
   \li Favorite.Index
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty bool Favorite::favorite

  This property holds the value that indicates whether a contact is a favorite.
*/

/*!
  \qmlproperty int Favorite::index

  This property holds the index of the favorite contact (which determines the order they appear).
*/


/* ==================== QDeclarativeContactGender ======================= */
/*!
   \qmltype Gender
    \instantiates QDeclarativeContactGender
   \brief The Gender element contains the gender of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Gender element contains the following field types:
   \list
   \li Gender.Gender
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty enumeration Gender::gender

  This property holds the value of the gender.

  \list
  \li Gender.Male
  \li Gender.Female
  \endlist
*/


/* ==================== QDeclarativeContactGeoLocation ======================= */
/*!
   \qmltype GeoLocation
    \instantiates QDeclarativeContactGeoLocation
   \brief The GeoLocation element contains a global location coordinate associated with a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   GeoLocation element contains the following field types:
   \list
   \li GeoLocation.Label
   \li GeoLocation.Latitude
   \li GeoLocation.Longitude
   \li GeoLocation.Accuracy
   \li GeoLocation.Altitude
   \li GeoLocation.AltitudeAccuracy
   \li GeoLocation.Heading
   \li GeoLocation.Speed
   \li GeoLocation.Timestamp
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string GeoLocation::label

  This property holds the location label.
*/
/*!
  \qmlproperty double GeoLocation::latitude

  This property holds the value of the latitude.
*/
/*!
  \qmlproperty double GeoLocation::longitude

  This property holds the value of the longitude.
*/
/*!
  \qmlproperty double GeoLocation::accuracy

  This property holds the value of the location (latitude/longitude) accuracy.
*/
/*!
  \qmlproperty double GeoLocation::altitude

  This property holds the value of the altitude.
*/
/*!
  \qmlproperty double GeoLocation::altitudeAccuracy

  This property holds the value of the accuracy of the altitude.
*/
/*!
  \qmlproperty double GeoLocation::heading

  This property holds the value of the heading.
*/
/*!
  \qmlproperty double GeoLocation::speed

  This property holds the value of the speed.
*/
/*!
  \qmlproperty date GeoLocation::timestamp

  This property holds the value of the timestamp of the location information.
*/


/* ==================== QDeclarativeContactGlobalPresence ======================= */
/*!
   \qmltype GlobalPresence
    \instantiates QDeclarativeContactGlobalPresence
   \brief The GlobalPresence element provides aggregated presence information
   for a contact, synthesized or supplied by the backend.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   GlobalPresence element contains the following field types:
   \list
   \li GlobalPresence.Timestamp
   \li GlobalPresence.Nickname
   \li GlobalPresence.State
   \li GlobalPresence.StateText
   \li GlobalPresence.ImageUrl
   \li GlobalPresence.CustomMessage
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty date GlobalPresence::timestamp

  This property holds the timestamp value of the GlobalPresence.
*/
/*!
  \qmlproperty string GlobalPresence::nickname

  This property holds the nickname value of the GlobalPresence.
*/
/*!
  \qmlproperty enumeration GlobalPresence::state

  This property holds the presence state enumeration value.

  \list
  \li Presence.Unknown -  Signifies that the presence state of the contact is not currently known (default).
  \li Presence.Available - Signifies that the contact is available.
  \li Presence.Hidden - Signifies that the contact is hidden.
  \li Presence.Busy - Signifies that the contact is busy.
  \li Presence.Away - Signifies that the contact is away.
  \li Presence.ExtendedAway - Signifies that the contact is away for an extended period of time.
  \li Presence.Offline - Signifies that the contact is offline.
  \endlist
*/
/*!
  \qmlproperty string GlobalPresence::stateText

  This property holds the text corresponding to the current presence state.
*/
/*!
  \qmlproperty url GlobalPresence::imageUrl

  This property holds the last-known status image url of the contact for the online account
   about which this detail stores presence information.
*/
/*!
  \qmlproperty string GlobalPresence::customMessage

  This property holds the custom status message from the contact for the online account
   about which this detail stores presence information.
*/

/* ==================== QDeclarativeContactGuid ======================= */
/*!
   \qmltype Guid
    \instantiates QDeclarativeContactGuid
   \brief The Guid element contains a globally unique Id of a contact, for use in synchronization with other datastores.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Guid element contains the following field types:
   \list
   \li Guid.Guid
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Guid::guid

  This property holds the value of the GUID.
*/

/* ==================== QDeclarativeContactHobby ======================= */
/*!
   \qmltype Hobby
    \instantiates QDeclarativeContactHobby
   \brief The Hobby element contains a hobby of the contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Hobby element contains the following field types:
   \list
   \li Hobby.Hobby
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Hobby::hobby

  This property holds the name of the hobby.
*/

/* ==================== QDeclarativeContactName ======================= */
/*!
   \qmltype Name
    \instantiates QDeclarativeContactName
   \brief The Name element contains a name of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts


   Name element contains the following field types:
   \list
   \li Name.Prefix
   \li Name.FirstName
   \li Name.MiddleName
   \li Name.LastName
   \li Name.Suffix
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Name::prefix

  This property holds the prefix name part of the name.
*/
/*!
  \qmlproperty string Name::firstName

  This property holds the first name part of the name.
*/
/*!
  \qmlproperty string Name::middleName

  This property holds the middle name part of the name.
*/
/*!
  \qmlproperty string Name::lastName

  This property holds the last name part of the name.
*/
/*!
  \qmlproperty string Name::suffix

  This property holds the suffix part of the name.
*/


/* ==================== QDeclarativeContactNickname ======================= */
/*!
   \qmltype Nickname
    \instantiates QDeclarativeContactNickname
   \brief The Nickname element contains a nickname of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts


   Nickname element contains the following field types:
   \list
   \li Nickname.Nickname
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Nickname::nickname

  This property holds the value of the nickname.
*/


/* ==================== QDeclarativeContactNote ======================= */
/*!
   \qmltype Note
    \instantiates QDeclarativeContactNote
   \brief The Note element contains a note associated with a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Note element contains the following field types:
   \list
   \li Note.Note
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Note::note

  This property holds the value of the note.
*/


/* ==================== QDeclarativeContactOnlineAccount ======================= */
/*!
   \qmltype OnlineAccount
    \instantiates QDeclarativeContactOnlineAccount
   \brief The OnlineAccount element contains a note associated with a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   OnlineAccount element contains the following field types:
   \list
   \li OnlineAccount.AccountUri - the account uri value.
   \li OnlineAccount.ServiceProvider - the account service provider name.
   \li OnlineAccount.Protocol - the account protocol value.
   \li OnlineAccount.Capabilities - the account capabilities value.
   \li OnlineAccount.SubTypes - the sub types of an online account.
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string OnlineAccount::accountUri

  This property holds the value of the account uri.
*/
/*!
  \qmlproperty string OnlineAccount::serviceProvider

  This property holds the value of the account service provider name.
*/
/*!
  \qmlproperty list<string> OnlineAccount::capabilities

  This property holds the value of the account capabilities.
*/
/*!
  \qmlproperty list<variant> OnlineAccount::subTypes

  This property holds the value of the sub types of an online account.

  \list
  \li OnlineAccount.Unknown (default)
  \li OnlineAccount.Sip - indicating this online account supports SIP.
  \li OnlineAccount.SipVoip - indicating this online account supports SIP based VOIP.
  \li OnlineAccount.Impp - indicating this online account supports IMPP.
  \li OnlineAccount.VideoShare - indicating this online account supports VideoShare.
  \endlist
*/
/*!
  \qmlproperty enumeration OnlineAccount::protocol

  This property holds the protocol enumeration value.

  \list
  \li OnlineAccount.Unknown - indicates this online account is for one unsupported protocol.
  \li OnlineAccount.Aim - indicates this online account is for the AIM protocol.
  \li OnlineAccount.Icq - indicates this online account is for the ICQ protocol.
  \li OnlineAccount.Irc - indicates this online account is for the IRC protocol.
  \li OnlineAccount.Jabber - indicates this online account is for the jabber protocol.
  \li OnlineAccount.Msn - indicates this online account is for the MSN protocol.
  \li OnlineAccount.Qq - indicates this online account is for the QQ protocol.
  \li OnlineAccount.Skype - indicates this online account is for the Skype protocol.
  \li OnlineAccount.Yahoo - indicates this online account is for the Yahoo protocol.
  \endlist
*/

/* ==================== QDeclarativeContactOrganization ======================= */
/*!
   \qmltype Organization
    \instantiates QDeclarativeContactOrganization
   \brief The Organization element provides details about an
   organization that the contact is either a part of, or stands for.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Organization element contains the following field types:
   \list
   \li Organization.Name
   \li Organization.LogoUrl
   \li Organization.Department
   \li Organization.Location
   \li Organization.Role
   \li Organization.Title
   \li Organization.AssistantName
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string Organization::name

  This property holds the value of the organization name.
*/
/*!
  \qmlproperty url Organization::logoUrl

  This property holds the URL of the organization logo image.
*/
/*!
  \qmlproperty list<string> Organization::department

  This property holds the value of the department name.
*/
/*!
  \qmlproperty string Organization::location

  This property holds the value of the location of the organization.
*/
/*!
  \qmlproperty string Organization::role

  This property holds the value of the contact's role in the organization.
*/
/*!
  \qmlproperty string Organization::title

  This property holds the value of the contact's title in the organization.
*/
/*!
  \qmlproperty string Organization::assistantName

  This property holds the value of the name of the contact's assistant.
*/

/* ==================== QDeclarativeContactPhoneNumber ======================= */
/*!
   \qmltype PhoneNumber
    \instantiates QDeclarativeContactPhoneNumber
   \brief The PhoneNumber element provides a phone number of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   PhoneNumber element contains the following field types:
   \list
   \li PhoneNumber.Number
   \li PhoneNumber.SubTypes
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty string PhoneNumber::number

  This property holds the value of the phone number.
*/

/*!
  \qmlproperty list<variant> PhoneNumber::subTypes

  This property holds the sub types of a PhoneNumber.

  \list
  \li PhoneNumber.Unknown - indicating this phone number type is unknown(default).
  \li PhoneNumber.Landline - indicating this phone number is a landline number.
  \li PhoneNumber.Mobile - ndicating this phone number is a mobile (cellular) number.
  \li PhoneNumber.Fax - indicating this phone number is a fax number.
  \li PhoneNumber.Pager - indicating this phone number is a pager number.
  \li PhoneNumber.Voice - indicating this phone number supports voice transmission.
  \li PhoneNumber.Modem - indicating this phone number supports data transmission.
  \li PhoneNumber.Video - indicating this phone number supports video transmission.
  \li PhoneNumber.Car - indicating this phone number is a car phone.
  \li PhoneNumber.BulletinBoardSystem - indicating this phone number is a bulletin board system.
  \li PhoneNumber.MessagingCapable - indicating this phone number supports messaging services.
  \li PhoneNumber.Assistant - indicating this phone number is the number of an assistant.
  \li PhoneNumber.DtmfMenu - indicating this phone number supports DTMF-controlled voice menu navigation.
  \endlist
*/

/* ==================== QDeclarativeContactPresence ======================= */
/*!
   \qmltype Presence
    \instantiates QDeclarativeContactPresence
   \brief The Presence element provides presence information for an online account of a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Presence element contains the following field types:
   \list
   \li Presence.Timestamp
   \li Presence.Nickname
   \li Presence.State
   \li Presence.StateText
   \li Presence.ImageUrl
   \li Presence.CustomMessage
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty date Presence::timestamp

  This property holds the timestamp value of the Presence.
*/
/*!
  \qmlproperty string Presence::nickname

  This property holds the nickname value of the Presence.
*/
/*!
  \qmlproperty enumeration Presence::state

  This property holds the presence state enumeration value.

  \list
  \li Presence.Unknown -  Signifies that the presence state of the contact is not currently known (default).
  \li Presence.Available - Signifies that the contact is available.
  \li Presence.Hidden - Signifies that the contact is hidden.
  \li Presence.Busy - Signifies that the contact is busy.
  \li Presence.Away - Signifies that the contact is away.
  \li Presence.ExtendedAway - Signifies that the contact is away for an extended period of time.
  \li Presence.Offline - Signifies that the contact is offline.
  \endlist
*/
/*!
  \qmlproperty string Presence::stateText

  This property holds the text corresponding to the current presence state.
*/
/*!
  \qmlproperty url Presence::imageUrl

  This property holds the last-known status image url of the contact for the online account
   about which this detail stores presence information.
*/
/*!
  \qmlproperty string Presence::customMessage

  This property holds the custom status message from the contact for the online account
   about which this detail stores presence information.
*/



/* ==================== QDeclarativeContactRingtone ======================= */
/*!
   \qmltype Ringtone
    \instantiates QDeclarativeContactRingtone
   \brief The Ringtone element provides a ringtone associated with a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Ringtone element contains the following field types:
   \list
   \li Ringtone.AudioRingtoneUrl
   \li Ringtone.VideoRingtoneUrl
   \endlist

  This element is part of the \b{QtContacts} module.
*/

/*!
  \qmlproperty url Ringtone::audioRingtoneUrl

  This property holds the value of the URL for an audio ringtone.
*/
/*!
  \qmlproperty url Ringtone::videoRingtoneUrl

  This property holds the value of the URL for a video ringtone.
*/

// Not in use (note the missing ! below)
/*
  \qmlproperty url Ringtone::vibrationRingtoneUrl

  This property holds the value of the URL for a vibration ringtone.
*/

/* ==================== QDeclarativeContactSyncTarget ======================= */
/*!
   \qmltype SyncTarget
    \instantiates QDeclarativeContactSyncTarget
   \brief The SyncTarget element provides a sync target for a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   SyncTarget element contains the following field types:
   \list
   \li SyncTarget.SyncTarget
   \endlist

   \sa QContactSyncTarget

   This element is part of the \b{QtContacts} module.
*/
/*!
  \qmlproperty string SyncTarget::syncTarget

  This property holds the sync target value.
*/

/* ==================== QDeclarativeContactTag ======================= */
/*!
   \qmltype Tag
    \instantiates QDeclarativeContactTag
   \brief The Tag element provides a contains a tag associated with a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Tag element contains the following field types:
   \list
   \li Tag.Tag
   \endlist
   \sa QContactTag

   This element is part of the \b{QtContacts} module.
*/
/*!
  \qmlproperty string Tag::tag

  This property holds the value of the tag.
*/

/* ==================== QDeclarativeContactTimestamp ======================= */
/*!
   \qmltype Timestamp
    \instantiates QDeclarativeContactTimestamp
   \brief The Timestamp element contains the creation and last-modified timestamp associated with the contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Timestamp element contains the following field types:
   \list
   \li Timestamp.LastModified
   \li Timestamp.Created
   \endlist

  This element is part of the \b{QtContacts} module.
*/
/*!
  \qmlproperty date Timestamp::lastModified

  This property holds the value of the last modified timestamp.
*/
/*!
  \qmlproperty date Timestamp::created

  This property holds the value of the timestamp a contact was created.
*/

/* ==================== QDeclarativeContactUrl ======================= */
/*!
   \qmltype Url
    \instantiates QDeclarativeContactUrl
   \brief The Url element contains a url associated with a contact.
   \ingroup qml-contacts-details
   \inqmlmodule QtContacts

   Url element contains the following field types:
   \list
   \li Url.Url
   \li Url.SubType
   \endlist

  This element is part of the \b{QtContacts} module.
*/
/*!
  \qmlproperty string Url::url

  This property holds the value of the URL.
*/
/*!
  \qmlproperty enumeration Url::subType

  This property holds the sub type of a QContactUrl.

  \list
  \li Url.Unknown - indicating this url type is unknown (default).
  \li Url.HomePage - indicating this url is a contact's home page.
  \li Url.Favourite - indicating this url is one of the contact's favourite URLs (or bookmarks).
  \endlist
*/

#include "moc_qdeclarativecontactdetail_p.cpp"

QT_END_NAMESPACE
