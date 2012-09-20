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

#include <QByteArray>
#include <QUrl>
#include <QFile>

#include <qcontactmanager.h>
#include <qcontacts.h>
#include <qcontactdetails.h>
#include <qcontactdetail.h>

/*
    When these conditions are satisfied, QStringLiteral is implemented by
    gcc's statement-expression extension.  However, in this file it will
    not work, because "statement-expressions are not allowed outside functions
    nor in template-argument lists".

    Fall back to the less-performant QLatin1String in this case.
*/
#if defined(QStringLiteral) && defined(QT_UNICODE_LITERAL_II) && defined(Q_CC_GNU) && !defined(Q_COMPILER_LAMBDA)
# undef QStringLiteral
# define QStringLiteral QLatin1String
#endif

QTCONTACTS_BEGIN_NAMESPACE


/* template docs:

   XXXX::FieldYYYY:
       The field key constant for the YYYY value.
       \sa yyyy(), setYyyy()

   XXXX::Type:
        The enum constant for the type identifier of QContactXXXX details.

   XXXX::FieldSubType
        The field key constant for the field that stores the sub type of a XXXX.
        \sa subType(), setSubType()

   XXXX::SubTypeYYYY
        The predefined enum constant for a sub type value,
        indicating blah blah blah.
        \sa subTypes(), setSubTypes()
 */


/* ==================== QContactSyncTarget ======================= */

/*!
   \class QContactSyncTarget
   \brief The QContactSyncTarget class provides a sync target
   for a contact.

  \inmodule QtContacts

   \ingroup contacts-details
 */

/*!
    \variable QContactSyncTarget::Type
    The enum constant for the type identifier of QContactSyncTarget details.
*/
const QContactDetail::DetailType QContactSyncTarget::Type(QContactDetail::TypeSyncTarget);

/*!
    \enum QContactSyncTarget::SyncTargetField
   This enumeration defines the fields supported by QContactSyncTarget.
   \value FieldSyncTarget The value stored in this field is the contact to which syncronize.
   \sa syncTarget(), setSyncTarget()
 */

/*!
   \fn QContactSyncTarget::syncTarget() const

   Returns the identifier of the backend store to which the contact
   containing this detail should be synchronized.
 */

/*!
   \fn QContactSyncTarget::setSyncTarget(const QString& syncTarget)

   Sets the identifier of the backend store to which the contact
   containing this detail should be synchronized to \a syncTarget.
 */

/* ==================== QContactEmailAddress ======================= */


/*!
   \class QContactEmailAddress

   \brief The QContactEmailAddress class contains an email address of
   a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactEmailAddress::Type
   The enum constant for the type identifier of QContactEmailAddress details.
 */
const QContactDetail::DetailType QContactEmailAddress::Type(QContactDetail::TypeEmailAddress);

/*!
   \enum QContactEmailAddress::EmailAddressField

   This enumeration defines the fields supported by QContactEmailAddress.
   \value FieldEmailAddress The value stored in this field is the email address value.
   \sa emailAddress(), setEmailAddress()
 */

/*!
   \fn QContactEmailAddress::emailAddress() const
   Returns the email address of the contact which is stored in this detail.
 */

/*!
   \fn QContactEmailAddress::setEmailAddress(const QString& emailAddress)
   Sets the email address of the contact which is stored in this detail to \a emailAddress.
 */

/* ==================== QContactFamily ======================= */
/*!
   \class QContactFamily
   \brief The QContactFamily class contains names of
   family members of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactFamily::Type
   The enum constant for the type identifier of QContactFamily details.
 */
const QContactDetail::DetailType QContactFamily::Type(QContactDetail::TypeFamily);

/*!
   \enum QContactFamily::FamilyField

   This enumeration defines the fields supported by QContactFamily.
   \value FieldSpouse The value of this field contains the name of a spouse.
   \value FieldChildren The value of this field contains the names of children.
   \sa spouse(), setSpouse()
   \sa children(), setChildren()
 */

/*!
   \fn QContactFamily::spouse() const
   Returns the name of the spouse of the contact which is stored in this detail.
 */

/*!
   \fn QContactFamily::setSpouse(const QString& spouseName)
   Sets the name of the spouse of the contact which is stored in this detail to \a spouseName.
 */

/*!
   \fn QContactFamily::children() const
   Returns the names of the children of the contact which is stored in this detail.
 */

/*!
   \fn QContactFamily::setChildren(const QStringList& childrenNames)
   Sets the names of the children of the contact which is stored in this detail to \a childrenNames.
 */

/* ==================== QContactFavorite ======================= */
/*!
   \class QContactFavorite
   \brief The QContactFavorite class indicates if a contact is a favorite contact as well as the
   position it should appear in an ordered list of favorites.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactFavorite::Type
   The enum constant for the type identifier of QContactFavorite details.
 */
const QContactDetail::DetailType QContactFavorite::Type(QContactDetail::TypeFavorite);

/*!
   \enum QContactFavorite::FavoriteField

   This enumeration defines the fields supported by QContactFavorite.
   \value FieldFavorite The value of this field indicates whether a contact is a favorite.
   \value FieldIndex The value of this field contains the index of the favorite contact (which determines the order they appear).
   \sa isFavorite(), setFavorite()
   \sa index(), setIndex()
 */


/*!
   \fn bool QContactFavorite::isFavorite() const
   Returns true if the contact is a favorite, false otherwise.
 */

/*!
   \fn QContactFavorite::setFavorite(bool isFavorite)
   If \a isFavorite is true, marks the contact as a favorite.  Otherwise, marks the contact as not a favorite.
 */

/*!
   \fn int QContactFavorite::index() const
   Returns the index of the favorite contact.
 */

/*!
   \fn QContactFavorite::setIndex(int index)
   Sets the index of the favorite contact to \a index.
 */

/*!
    Returns a filter suitable for finding contacts which are marked
    as favorite contacts.
*/
QContactFilter QContactFavorite::match()
{
    QContactDetailFilter f;
    f.setDetailType(QContactFavorite::Type, QContactFavorite::FieldFavorite);
    f.setValue(true);
    f.setMatchFlags(QContactFilter::MatchExactly);

    return f;
}

/* ==================== QContactAnniversary ======================= */

/*!
   \class QContactAnniversary
   \brief The QContactAnniversary class contains an anniversary of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactAnniversary::Type
   The enum constant for the type identifier of QContactAnniversary details.
 */
const QContactDetail::DetailType QContactAnniversary::Type(QContactDetail::TypeAnniversary);

/*!
   \enum QContactAnniversary::AnniversaryField

   This enumeration defines the fields supported by QContactAnniversary.

   \value FieldCalendarId The value stored in this field is the id of the calendar event.
   \value FieldOriginalDate The value stored in this field is either a date, or a date and time.  Some managers
   may support either type, while others may convert the value here
   to a specific type (either discarding the time if only a date is
   supported, or by using midnight if a time is not supplied).
   \value FieldEvent The value stored in this field is the name of the event value.
   \value FieldSubType The value stored in this field is the sub type of a QContactAnniversary.
   \sa originalDate(), setOriginalDate(), originalDateTime(), setOriginalDateTime()
   \sa event(), setEvent()
   \sa calendarId(), setCalendarId()
 */

/*!
   \enum QContactAnniversary::SubType

   This enumeration defines the predefined enum constants for a sub type value of a QContactAnniversary.

   \value SubTypeWedding The value stored is a wedding anniversary.
   \value SubTypeEngagement The value stored is the anniversary of an engagement.
   \value SubTypeHouse The value stored is the anniversary of a new residence.
   \value SubTypeEmployment The value stored is the anniversary of a start of employment.
   \value SubTypeMemorial The value stored is the anniversary of an event of sentimental significance.
   \sa subType(), setSubType()
 */

/*!
   \fn QContactAnniversary::originalDate() const
   Returns the original date of the event stored in this detail.
   If the original event occurrence stored is a QDateTime, this returns the date portion.
 */

/*!
   \fn QContactAnniversary::setOriginalDate(const QDate& date)
   Sets the original date of the event stored in this detail to \a date.
 */

/*!
   \fn QContactAnniversary::originalDateTime() const
   Returns the original date and time of the event stored in this detail.
   If the original event occurrence stored is a QDate, this returns a QDateTime with the
   time set to midnight.
 */

/*!
   \fn QContactAnniversary::setOriginalDateTime(const QDateTime& dateTime)
   Sets the original date and time of the event stored in this detail to \a dateTime.
 */

/*!
   \fn QContactAnniversary::calendarId() const
 * Returns the identifier of the calendar entry associated with this anniversary.
 */

/*!
   \fn QContactAnniversary::setCalendarId(const QString& calendarId)
   Sets the identifier of the calendar entry associated with this anniversary to \a calendarId.
 */

/*!
   \fn QContactAnniversary::event() const
   Returns the name of the event for which this detail contains information.
 */

/*!
   \fn QContactAnniversary::setEvent(const QString& event)
   Sets the name of the event for which this detail contains information to \a event.
 */

/*!
   \fn QContactAnniversary::setSubType(const QContactAnniversary::SubType &subType)
   Sets the subtype which this detail implements to be the given \a subType.
 */

/*!
   \fn QContactAnniversary::subType() const
   Returns the subtype that this detail implements, if defined.
 */

/* ==================== QContactAvatar ======================= */

/*!
   \class QContactAvatar
   \ingroup contacts-details
    \inmodule QtContacts
   \brief The QContactAvatar class contains avatar URLs of a contact.

   Users can specify avatar URLs for a contact using this detail.
   Generally, a URL will specify the location of a full-sized
   image (or video) avatar.  Support for the detail is backend-specific;
   some managers will automatically load the URL and synthesize a
   (possibly scaled) thumbnail image for the contact and store it
   on a platform-specific location in the file system.

   The URLs which are contained in the detail may point to a file or
   resource whose content may dynamically change. The content of a resource
   identified by a URL specified in a QContactAvatar detail is set by whoever
   owns the resource which the URL identifies.

 */

/*!
   \variable QContactAvatar::Type
   The enum constant for the type identifier of QContactAvatar details.
 */
const QContactDetail::DetailType QContactAvatar::Type(QContactDetail::TypeAvatar);

/*!
   \enum QContactAvatar::AvatarField

   This enumeration defines the fields supported by QContactAvatar.
   \value FieldImageUrl The value stored in this field contains the URL of the avatar image.
   \value FieldVideoUrl The value stored in this field contains the URL of the video avatar.
   \sa imageUrl(), setImageUrl()
   \sa videoUrl(), setVideoUrl()
 */

/*!
  \fn QContactAvatar::imageUrl() const
  Returns the url of an avatar image associated with the contact
 */

/*!
  \fn QContactAvatar::setImageUrl(const QUrl& imageUrl)
  Sets the url of an avatar image associated with the contact to \a imageUrl
 */

/*!
  \fn QContactAvatar::videoUrl() const
  Returns the url of an avatar video associated with the contact
 */

/*!
  \fn QContactAvatar::setVideoUrl(const QUrl& videoUrl)
  Sets the url of an avatar video associated with the contact to \a videoUrl
 */

/* ==================== QContactAddress ======================= */


/*!
   \class QContactAddress
   \brief The QContactAddress class contains an address of a contact.
   \ingroup contacts-details
    \inmodule QtContacts

   The fields in the QContactAddress class are based on the segments
   of the ADR property of a Versit vCard file.
   Versit \reg is a trademark of the Internet Mail Consortium.
 */

/*!
   \variable QContactAddress::Type
   The enum constant for the type identifier of QContactAddress details.
 */
const QContactDetail::DetailType QContactAddress::Type(QContactDetail::TypeAddress);

/*!
   \enum QContactAddress::AddressField

   This enumeration defines the fields supported by QContactAddress.

   \value FieldStreet The value stored in this field contains the street number and street name of the address.
   \value FieldLocality The value stored in this field contains the name of the city, town or suburb of the address.
   \value FieldRegion The value stored in this field contains the region segment. The region segment contains the
   name or identifier of the state, province or region of the address.
   \value FieldPostcode The value stored in this field contains the postcode segment. The postcode segment contains
   the postal code for the address.
   \value FieldCountry The value stored in this field contains the country segment. The country segment contains
   the name of the country of the address.
   \value FieldPostOfficeBox The value stored in this field contains the post office box segment. The post office box
   segment contains the post office box identifier of the mailing address.
   \value FieldSubTypes The value stored in this field contains the sub types of a QContactAddress.
   \sa street(), setStreet()
   \sa locality(), setLocality()
   \sa region(), setRegion()
   \sa postcode(), setPostcode()
   \sa country(), setCountry()
   \sa postOfficeBox(), setPostOfficeBox()
 */

/*!
   \enum QContactAddress::SubType

   This enumeration defines the predefined enum constants for a sub type value of a QContactAddress.

   \value SubTypeParcel The value stored contains an address for parcel delivery.
   \value SubTypePostal The value stored contains an address for postal delivery.
   \value SubTypeDomestic The value stored contains an address for domestic mail delivery.
   \value SubTypeInternational The value stored contains an address for international mail delivery.
   \sa subTypes(), setSubTypes()
 */

/*!
   \fn QContactAddress::postOfficeBox() const
   Returns the post office box segment of the address stored in this detail.
 */

/*!
   \fn QContactAddress::setPostOfficeBox(const QString& postOfficeBox)
   Sets the post office box segment of the address stored in this detail to \a postOfficeBox.
 */

/*!
   \fn QContactAddress::street() const
   Returns the street segment of the address stored in this detail.
 */

/*!
   \fn QContactAddress::setStreet(const QString& street)
   Sets the street segment of the address stored in this detail to \a street.
 */

/*!
   \fn QContactAddress::locality() const
   Returns the locality segment of the address stored in this detail.
 */

/*!
   \fn QContactAddress::setLocality(const QString& locality)
   Sets the locality segment of the address stored in this detail to \a locality.
 */

/*!
   \fn QContactAddress::region() const
   Returns the region segment of the address stored in this detail.
 */

/*!
   \fn QContactAddress::setRegion(const QString& region)
   Sets the region segment of the address stored in this detail to \a region.
 */

/*!
   \fn QContactAddress::postcode() const
   Returns the postcode segment of the address stored in this detail.
 */

/*!
   \fn QContactAddress::setPostcode(const QString& postcode)
   Sets the postcode segment of the address stored in this detail to \a postcode.
 */

/*!
   \fn QContactAddress::country() const
   Returns the country segment of the address stored in this detail.
 */

/*!
   \fn QContactAddress::setCountry(const QString& country)
   Sets the country segment of the address stored in this detail to \a country.
 */

/*!
   \fn QContactAddress::setSubTypes(const QList<int> &subTypes)
   Sets the subtypes which this detail implements to be those contained in the list of given \a subTypes.
 */

/*!
   \fn QContactAddress::subTypes() const
   Returns the list of subtypes that this detail implements.
 */

/*!
    Returns a filter suitable for finding contacts with an address which
    contains the given \a subString in any of its fields.
*/
QContactFilter QContactAddress::match(const QString &subString)
{
    QContactDetailFilter f1;
    f1.setDetailType(QContactAddress::Type, QContactAddress::FieldStreet);
    f1.setValue(subString);
    f1.setMatchFlags(QContactFilter::MatchContains);

    QContactDetailFilter f2;
    f2.setDetailType(QContactAddress::Type, QContactAddress::FieldLocality);
    f2.setValue(subString);
    f2.setMatchFlags(QContactFilter::MatchContains);

    QContactDetailFilter f3;
    f3.setDetailType(QContactAddress::Type, QContactAddress::FieldRegion);
    f3.setValue(subString);
    f3.setMatchFlags(QContactFilter::MatchContains);

    QContactDetailFilter f4;
    f4.setDetailType(QContactAddress::Type, QContactAddress::FieldPostcode);
    f4.setValue(subString);
    f4.setMatchFlags(QContactFilter::MatchContains);

    QContactDetailFilter f5;
    f5.setDetailType(QContactAddress::Type, QContactAddress::FieldCountry);
    f5.setValue(subString);
    f5.setMatchFlags(QContactFilter::MatchContains);

    QContactDetailFilter f6;
    f6.setDetailType(QContactAddress::Type, QContactAddress::FieldPostOfficeBox);
    f6.setValue(subString);
    f6.setMatchFlags(QContactFilter::MatchContains);

    return (f1 | f2 | f3 | f4 | f5 | f6);
}

/* ==================== QContactUrl ======================= */

/*!
   \class QContactUrl
   \brief The QContactUrl class contains a url associated with
   a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactUrl::Type
   The enum constant for the type identifier of QContactUrl details.
 */
const QContactDetail::DetailType QContactUrl::Type(QContactDetail::TypeUrl);

/*!
   \enum QContactUrl::UrlField

   This enumeration defines the fields supported by QContactUrl.

   \value FieldUrl The value stored in this field contains the URL.
   \value FieldSubType The value stored in this field contains the sub type of a QContactUrl.
   \sa url(), setUrl()
 */

/*!
   \enum QContactUrl::SubType

   This enumeration defines the predefined enum constants for a sub type value of a QContactUrl.

   \value SubTypeHomePage The value stored is a contact's home page.
   \value SubTypeFavourite The value stored is a contact's favourite URLs (or bookmarks).
   \value SubTypeBlog The value stored is one of the contact's blogs.
   \sa subType(), setSubType()
 */

/*!
   \fn QContactUrl::url() const
   Returns the url stored in this detail.
 */

/*!
   \fn QContactUrl::setUrl(const QString& url)
   Sets the url stored in this detail to \a url.
 */

/*!
   \fn QContactUrl::setUrl(const QUrl& url)
   Sets the url stored in this detail to the string representation
   of the given \a url.
 */

/*!
   \fn QContactUrl::setSubType(const QContactUrl::SubType& subType)
   Sets the subtype which this detail implements to be the given \a subType.
 */

/*!
   \fn QContactUrl::subType() const
   Returns the subtype that this detail implements, if defined.
 */

/* ==================== QContactPhonenumber ======================= */

/*!
    \class QContactPhoneNumber
    \brief The QContactPhoneNumber class provides a phone number
    of a contact.
    \ingroup contacts-details
    \inmodule QtContacts
*/


/*!
    \variable QContactPhoneNumber::Type
    The enum constant for the type identifier of QContactPhoneNumber details.
*/
const QContactDetail::DetailType QContactPhoneNumber::Type(QContactDetail::TypePhoneNumber);

/*!
   \enum QContactPhoneNumber::PhoneNumberField

   This enumeration defines the fields supported by QContactPhoneNumber.

   \value FieldNumber The value stored in this field contains the phone number.
   \value FieldSubTypes The value stored in this field contains the sub types of a QContactPhoneNumber.
   \sa number(), setNumber()
 */

/*!
   \enum QContactPhoneNumber::SubType

   This enumeration defines the predefined enum constants for a sub type value of a QContactPhoneNumber.

   \value SubTypeLandline The value stored is a landline number.
   \value SubTypeMobile The value stored is a mobile (cellular) number.
   \value SubTypeFax The value stored is a fax number.
   \value SubTypePager The value stored is a pager number.
   \value SubTypeCar The value stored is a car phone.
   \value SubTypeBulletinBoardSystem The value stored is a bulletin board system.
   \value SubTypeVoice The value stored is indicating this phone number supports voice transmission.
   \value SubTypeModem The value stored is indicating this phone number supports data transmission.
   \value SubTypeVideo The value stored is indicating this phone number supports video transmission.
   \value SubTypeMessagingCapable The value stored is indicating this phone number supports messaging services.
   \value SubTypeAssistant The value stored is indicating this phone number is the number of an assistant.
   \value SubTypeDtmfMenu The value stored is indicating this phone number supports DTMF-controlled voice menu navigation.
   \sa subTypes(), setSubTypes()
 */

/*!
   \fn QContactPhoneNumber::number() const
   Returns the phone number stored in this detail.
 */

/*!
   \fn QContactPhoneNumber::setNumber(const QString& number)
   Sets the phone number stored in this detail to \a number.
 */

/*!
   \fn QContactPhoneNumber::setSubTypes(const QList<int>& subTypes)
   Sets the subtypes which this detail implements to be those contained in the list of given \a subTypes
 */

/*!
   \fn QContactPhoneNumber::subTypes() const
   Returns the list of subtypes that this detail implements.
 */

/* ==================== QContactBirthday ======================= */

/*!
   \class QContactBirthday
   \brief The QContactBirthday class contains a birthday of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactBirthday::Type
   The enum constant for the type identifier of QContactBirthday details.
 */
const QContactDetail::DetailType QContactBirthday::Type(QContactDetail::TypeBirthday);

/*!
   \enum QContactBirthday::BirthdayField
   This enumeration defines the fields supported by QContactBirthday.
   \value FieldBirthday The value stored in this field contains the birthday date.
   This field is either a date, or a date and time.  Some managers
   may support either type, while others may convert the value here
   to a specific type (either discarding the time if only a date is
   supported, or by using midnight if a time is not supplied).
   \value FieldCalendarId The value stored in this field contains the id of the calendar event.
   \sa date(), setDate(), dateTime(), setDateTime()
   \sa calendarId(), setCalendarId()
 */

/*!
   \fn QContactBirthday::date() const
   Returns the date of the birthday which is stored in this detail.
   If the birthday stored is a QDateTime, this returns the date portion.
 */

/*!
   \fn QContactBirthday::setDate(const QDate& date)
   Sets the date of the birthday which is stored in this detail to \a date.
 */

/*!
   \fn QContactBirthday::dateTime() const
   Returns the date and time of the birthday which is stored in this detail.
   If the birthday stored is a QDate, this returns a QDateTime with the
   time set to midnight.
 */

/*!
   \fn QContactBirthday::setDateTime(const QDateTime& dateTime)
   Sets the date and time of the birthday which is stored in this detail to \a dateTime.
 */


/*!
   \fn QContactBirthday::calendarId() const
 * Returns the identifier of the calendar entry associated with this birthday.
 */

/*!
   \fn QContactBirthday::setCalendarId(const QString& calendarId)
   Sets the identifier of the calendar entry associated with this birthday to \a calendarId.
 */

/* ==================== QContactDisplayLabel ======================= */

/*!
   \class QContactDisplayLabel
   \brief The QContactDisplayLabel class contains a displayLabel of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactDisplayLabel::Type
   The enum constant for the type identifier of QContactDisplayLabel details.
*/
const QContactDetail::DetailType QContactDisplayLabel::Type(QContactType::TypeDisplayLabel);

/*!
   \enum QContactDisplayLabel::DisplayLabelField
   This enumeration defines the fields supported by QContactDisplayLabel.
   \value FieldLabel The value stored in this field contains the displaylabel.
   \sa label(), setLabel()
 */

/*!
   \fn QContactDisplayLabel::setLabel(const QString& displayLabel)
   Sets the displayLabel of the contact which is stored in this detail to \a displayLabel.
   displayLabel can be for example the first name of a contact.
 */

/*!
   \fn QContactDisplayLabel::label() const
   Returns the displayLabel of the contact which is stored in this detail.
 */


/* ==================== QContactGender ======================= */

/*!
   \class QContactGender
   \brief The QContactGender class contains the gender of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactGender::Type
   The enum constant for the type identifier of QContactGender details.
 */
const QContactDetail::DetailType QContactGender::Type(QContactDetail::TypeGender);

/*!
   \enum QContactGender::GenderField
   This enumeration defines the fields supported by QContactGender.
   \value FieldGender The value stored in this field contains the gender.
   \value GenderMale The value that identifies this contact as being male.
   \value GenderFemale The value that identifies this contact as being female.
   \value GenderUnspecified The value that identifies this contact as being of unspecified gender.
   \sa gender(), setGender()
 */

/*!
   \fn QContactGender::gender() const

   Returns the gender of the contact, as stored in this detail.  The
   possible values for the value stored are "Male", "Female" and
   "Unspecified".
 */

/*!
   \fn QContactGender::setGender(const GenderField gender)

   Sets the gender of the contact (as stored in this detail) to \a
   gender, if \a gender is either GenderMale or GenderFemale, otherwise sets
   it to GenderUnspecified.
 */

/* ==================== QContactGeolocation ======================= */

/*!
   \class QContactGeoLocation
   \brief The QContactGeoLocation class contains a global location
   coordinate associated with a contact.
   \ingroup contacts-details
    \inmodule QtContacts
*/

/*!
   \variable QContactGeoLocation::Type
   The enum constant for the type identifier of QContactGeoLocation details.
 */
const QContactDetail::DetailType QContactGeoLocation::Type(QContactDetail::TypeGeoLocation);

/*!
   \enum QContactGeoLocation::GeoLocationField
   This enumeration defines the fields supported by QContactGeoLocation.
   \value FieldLabel The value stored in this field contains the location label.
   \value FieldLatitude The value stored in this field contains the latitude.
   \value FieldLongitude The value stored in this field contains the longitude.
   \value FieldAccuracy The value stored in this field contains the location (latitude/longitude) accuracy.
   \value FieldAltitude The value stored in this field contains the altitude.
   \value FieldAltitudeAccuracy The value stored in this field contains the accuracy of the altitude.
   \value FieldHeading The value stored in this field contains the heading.
   \value FieldSpeed The value stored in this field contains the speed.
   \value FieldTimestamp The value stored in this field contains the timestamp of the location information.
   \sa label(), setLabel()
   \sa latitude(), setLatitude()
   \sa longitude(), setLongitude()
   \sa accuracy(), setAccuracy()
   \sa altitude(), setAltitude()
   \sa altitudeAccuracy(), setAltitudeAccuracy()
   \sa heading(), setHeading()
   \sa speed(), setSpeed()
   \sa timestamp(), setTimestamp()
 */

/*!
   \fn QContactGeoLocation::setLabel(const QString& label)
   Sets the label of the location stored in the detail to \a label.
 */

/*!
   \fn QContactGeoLocation::label() const
   Returns the label of the location stored in the detail.
 */

/*!
   \fn QContactGeoLocation::setLatitude(double latitude)

   Sets the latitude portion of the coordinate (in decimal degrees) of
   the location stored in the detail to \a latitude.
 */

/*!
   \fn QContactGeoLocation::latitude() const

   Returns the latitude portion of the coordinate (specified in
   decimal degrees) of the location stored in the detail.
 */

/*!
   \fn QContactGeoLocation::setLongitude(double longitude)

   Sets the longitude portion of the coordinate (in decimal degrees)
   of the location stored in the detail to \a longitude.
 */

/*!
   \fn QContactGeoLocation::longitude() const

   Returns the longitude portion of the coordinate (specified in
   decimal degrees) of the location stored in the detail.
 */

/*!
   \fn QContactGeoLocation::setAccuracy(double accuracy)

   Specifies that the latitude and longitude portions of the location
   stored in the detail are accurate to within \a accuracy metres.
 */

/*!
   \fn QContactGeoLocation::accuracy() const

   Returns the accuracy (in metres) of the latitude and longitude of
   the location stored in the detail.
 */

/*!
   \fn QContactGeoLocation::setAltitude(double altitude)

   Sets the altitude portion of the coordinate (in metres above the
   ellipsoid) of the location stored in the detail to \a altitude.
 */

/*!
   \fn QContactGeoLocation::altitude() const
   Returns the altitude (in metres) of the location stored in the detail.
 */

/*!
   \fn QContactGeoLocation::setAltitudeAccuracy(double altitudeAccuracy)

   Sets the altitude-accuracy portion of the coordinate (in metres) of
   the location stored in the detail to \a altitudeAccuracy.
 */

/*!
   \fn QContactGeoLocation::altitudeAccuracy() const

   Returns the accuracy of the altitude portion of the location stored
   in the detail.
 */

/*!
   \fn QContactGeoLocation::setHeading(double heading)

   Sets the heading portion of the coordinate (in decimal degrees
   clockwise relative to true north) of the location-aware device at
   the time of measurement to \a heading.
 */

/*!
   \fn QContactGeoLocation::heading() const

   Returns the heading (at the time of measurement) of the
   location-aware device that recorded (or was provided) the
   measurement.
 */

/*!
   \fn QContactGeoLocation::setSpeed(double speed)

   Sets the speed portion of the coordinate (in metres per second) of
   the location-aware device at the time of measurement to \a speed.
 */

/*!
   \fn QContactGeoLocation::speed() const

   Returns the speed (at the time of measurement) of the
   location-aware device that recorded (or was provided) the
   measurement.
 */

/*!
   \fn QContactGeoLocation::setTimestamp(const QDateTime& timestamp)

   Sets the creation (or first-valid) timestamp of the location
   information to \a timestamp.
 */

/*!
   \fn QContactGeoLocation::timestamp() const

   Returns the timestamp associated with the location stored in the
   detail.
 */

/* ==================== QContactGuid ======================= */

/*!
   \class QContactGuid
   \brief The QContactGuid class contains a globally unique
   Id of a contact, for use in synchronization with other datastores.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactGuid::Type
   The enum constant for the type identifier of QContactGuid details.
 */
const QContactDetail::DetailType QContactGuid::Type(QContactDetail::TypeGuid);

/*!
   \enum QContactGuid::GuidField
   This enumeration defines the fields supported by QContactGuid.
   \value FieldGuid The value stored in this field contains the GUID.
   \sa guid(), setGuid()
 */

/*!
   \fn QContactGuid::guid() const

   Returns the globally unique identifier which is stored in this
   detail.
 */

/*!
   \fn QContactGuid::setGuid(const QString& guid)
   Sets the globally unique identifier which is stored in this detail to \a guid.
 */

/* ==================== QContactHobby ======================= */

/*!
   \class QContactHobby
   \brief The QContactHobby class contains a hobby of the contact.
   \ingroup contacts-details
    \inmodule QtContacts

   A contact may have one or more hobbies.  Each QContactHobby
   detail contains information about a single hobby of the contact.
 */

/*!
   \variable QContactHobby::Type
   The enum constant for the type identifier of QContactHobby details.
 */
const QContactDetail::DetailType QContactHobby::Type(QContactDetail::TypeHobby);

/*!
   \enum QContactHobby::HobbyField
   This enumeration defines the fields supported by QContactHobby.
   \value FieldHobby The value stored in this field contains the name of the hobby.
   \sa hobby(), setHobby()
 */

/*!
   \fn QContactHobby::setHobby(const QString& hobby)
   Sets the hobby associated with a contact which is stored in this detail to \a hobby.
 */

/*!
   \fn QContactHobby::hobby() const
   Returns the hobby associated with a contact which is stored in this detail.
 */


/* ==================== QContactName ======================= */

/*!
   \class QContactName
   \brief The QContactName class contains a name of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactName::Type
   The enum constant for the type identifier of QContactName details.
 */
const QContactDetail::DetailType QContactName::Type(QContactDetail::TypeName);

/*!
   \enum QContactName::NameField
   This enumeration defines the fields supported by QContactName.
   \value FieldPrefix The value stored in this field contains the prefix part of the name.
   \value FieldFirstName The value stored in this field contains the first part of the name.
   \value FieldMiddleName The value stored in this field contains the middle part of the name.
   \value FieldLastName The value stored in this field contains the last part of the name.
   \value FieldSuffix The value stored in this field contains the suffix part of the name.
   \sa prefix(), setPrefix()
   \sa firstName(), setFirstName()
   \sa middleName(), setMiddleName()
   \sa lastName(), setLastName()
   \sa suffix(), setSuffix()
 */

/*!
   \fn QContactName::prefix() const
   Returns the prefix segment of the name stored in this detail.
 */

/*!
   \fn QContactName::setPrefix(const QString& prefix)
   Sets the prefix segment of the name stored in this detail to \a prefix.
 */

/*!
   \fn QContactName::firstName() const
   Returns the first (given) name segment of the name stored in this detail.
 */

/*!
   \fn QContactName::setFirstName(const QString& firstName)
   Sets the first name segment of the name stored in this detail to \a firstName.
 */

/*!
   \fn QContactName::middleName() const

   Returns the middle (additional, or other) name segment of the name
   stored in this detail.
 */

/*!
   \fn QContactName::setMiddleName(const QString& middleName)
   Sets the middle name segment of the name stored in this detail to \a middleName.
 */

/*!
   \fn QContactName::lastName() const

   Returns the last (family, or surname) name segment of the name
   stored in this detail.
 */

/*!
   \fn QContactName::setLastName(const QString& lastName)
   Sets the last name segment of the name stored in this detail to \a lastName.
 */

/*!
   \fn QContactName::suffix() const
   Returns the suffix segment of the name stored in this detail.
 */

/*!
   \fn QContactName::setSuffix(const QString& suffix)
   Sets the suffix segment of the name stored in this detail to \a suffix.
 */

/* ==================== QContactNickname ======================= */

/*!
   \class QContactNickname
   \brief The QContactNickname class contains a nickname of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactNickname::Type
   The enum constant for the type identifier of QContactNickname details.
*/
const QContactDetail::DetailType QContactNickname::Type(QContactType::TypeNickname);

/*!
   \enum QContactNickname::NicknameField
   This enumeration defines the fields supported by QContactNickname.
   \value FieldNickname The value stored in this field contains the nickname.
   \sa nickname(), setNickname()
 */

/*!
   \fn QContactNickname::setNickname(const QString& nickname)
   Sets the nickname of the contact which is stored in this detail to \a nickname.
 */

/*!
   \fn QContactNickname::nickname() const
   Returns the nickname of the contact which is stored in this detail.
 */

/* ==================== QContactNote ======================= */

/*!
   \class QContactNote
   \brief The QContactNote class contains a note associated
   with a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactNote::Type
   The enum constant for the type identifier of QContactNote details.
*/
const QContactDetail::DetailType QContactNote::Type(QContactDetail::TypeNote);

/*!
   \enum QContactNote::NoteField
   This enumeration defines the fields supported by QContactNote.
   \value FieldNote The value stored in this field contains the note.
   \sa note(), setNote()
 */

/*!
   \fn QContactNote::setNote(const QString& note)
   Sets a note associated with a contact to \a note.
 */

/*!
   \fn QContactNote::note() const
   Returns a string for a note associated with a contact.
 */

/* ==================== QContactTag ======================= */

/*!
   \class QContactTag
   \brief The QContactTag class contains a tag associated with a
   contact.
   \ingroup contacts-details
    \inmodule QtContacts

   Typically the tags associated with a contact will be distinct,
   although this is usually only enforced when the contact is saved
   in the manager.

   Here is an example of retrieving all the tags for a contact:
   \snippet qtcontactsdocsample/qtcontactsdocsample.cpp Getting all tags

   Here is an example of checking for a specific tag value:
   \snippet qtcontactsdocsample/qtcontactsdocsample.cpp Checking for a specific tag
 */

/*!
   \variable QContactTag::Type
   The enum constant for the type identifier of QContactTag details.
 */
const QContactDetail::DetailType QContactTag::Type(QContactDetail::TypeTag);

/*!
   \enum QContactTag::TagField
    This enumeration defines the fields supported by QContactTag.
    \value FieldTag The value stored in this field contains the tag.
   \sa tag(), setTag()
 */

/*!
   \fn QContactTag::setTag(const QString& tag)
   Sets the tag associated with a contact which is stored in this detail to \a tag.
 */

/*!
   \fn QContactTag::tag() const
   Returns the tag associated with a contact which is stored in this detail.
 */

/*!
    Returns a filter suitable for finding contacts which have a tag which
    contains the specified \a subString.
*/
QContactFilter QContactTag::match(const QString &subString)
{
    QContactDetailFilter f;
    f.setDetailType(QContactTag::Type, QContactTag::FieldTag);
    f.setValue(subString);
    f.setMatchFlags(QContactFilter::MatchContains);

    return f;
}

/* ==================== QContactTimestamp ======================= */

/*!
   \class QContactTimestamp
   \brief The QContactTimestamp class contains the creation and
   last-modified timestamp associated with the contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactTimestamp::Type
   The enum constant for the type identifier of QContactTimestamp details.
*/
const QContactDetail::DetailType QContactTimestamp::Type(QContactDetail::TypeTimestamp);

/*!
   \enum QContactTimestamp::TimestampField
   This enumeration defines the fields supported by QContactTimestamp.
   \value FieldModificationTimestamp The value stored in this field contains the last modified timestamp.
   \value FieldCreationTimestamp The value stored in this field contains the value of the timestamp a contact was created.
   \sa lastModified(), setLastModified()
   \sa created(), setCreated()
 */

/*!
   \fn QContactTimestamp::created() const
   Returns the creation timestamp saved in this detail.
 */

/*!
   \fn QContactTimestamp::lastModified() const
   Returns the last-modified timestamp saved in this detail.
 */

/*!
   \fn QContactTimestamp::setCreated(const QDateTime& dateTime)
   Sets the creation timestamp saved in this detail to \a dateTime.
 */

/*!
   \fn QContactTimestamp::setLastModified(const QDateTime& dateTime)
   Sets the last-modified timestamp saved in this detail to \a dateTime.
 */

/* ==================== QContactType ======================= */

/*!
   \class QContactType
   \brief The QContactType class describes the type of the contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactType::Type
   The enum constant for the type identifier of QContactType details.
*/
const QContactDetail::DetailType QContactType::Type(QContactDetail::TypeType);

/*!
   \enum QContactType::TypeField
   This enumeration defines the fields supported by QContactType.
   \value FieldType The value stored in this field contains the type value which is stored in details of
   the QContactType definition.
   \sa type(), setType()
*/

/*!
   \enum QContactType::TypeValues
   \value TypeContact The value indicates that this contact is an ordinary contact.
   \value TypeGroup The value indicates that this contact is a group contact.

    Contacts of this type are able to be the first contact in
    relationships of the \c QContactRelationship::HasMember type.

    To enumerate the ids of members of a group, the client should
    retrieve the relationships which involve the group from the manager
    in which the group is saved.

   \sa type(), setType()
 */

/*!
   \fn QContactType::type() const
   Returns the contact type value stored in this detail.
 */

/*!
   \fn QContactType::setType(const TypeValues type)
   Sets the type of the contact to be the give \a type.
 */


/* ==================== QContactOnlineAccount ======================= */

// XXX TODO explain link to QContactPresence

/*!
   \class QContactOnlineAccount
   \brief The QContactOnlineAccount class provides an online account,
   which the contact uses to communicate with friends and family.
    \inmodule QtContacts

   A QContactOnlineAccount consists of the account details required to
   communicate with the contact, including the account URI, the capabilities
   of the account, the service provider of the account, and the type of the account.

   Presence information for a particular QContactOnlineAccount detail is provided
   in a QContactPresence detail which is linked (via linkedDetailUris()) to the
   account detail.  This information is generally provided by the backend, and is
   not modifiable by clients.

   \sa QContactPresence

   \ingroup contacts-details
 */

/*!
   \variable QContactOnlineAccount::Type
   The enum constant for the type identifier of QContactOnlineAccount details.
 */
const QContactDetail::DetailType QContactOnlineAccount::Type(QContactDetail::TypeOnlineAccount);

/*!
   \enum QContactOnlineAccount::OnlineAccountField

   This enumeration defines the fields supported by QContactOnlineAccount.

   \value FieldAccountUri The value stored in this field contains the value of the account uri.
   \value FieldServiceProvider The value stored in this field contains the value of the provider name.
   \value FieldProtocol The value stored in this field contains the value of the protocol.
   \value FieldCapabilities The value stored in this field contains the value of the account capabilities.
   \value FieldSubTypes The value stored in this field contains the value of the sub types of a QContactOnlineAccount.
   \sa accountUri(), setAccountUri()
   \sa serviceProvider(), setServiceProvider()
   \sa capabilities(), setCapabilities()
 */

/*!
   \enum QContactOnlineAccount::SubType

   This enumeration defines the predefined enum constants for a sub type value of a QContactOnlineAccount.

   \value SubTypeSip The value stored indicates if this online account supports SIP.
   \value SubTypeSipVoip The value stored indicates if this online account supports SIP based VOIP.
   \value SubTypeImpp The value stored indicates if this online account supports IMPP.
   \value SubTypeVideoShare The value stored indicates if this online account supports VideoShare.
   \sa subTypes(), setSubTypes()
 */

/*!
   \enum QContactOnlineAccount::Protocol

   This enumeration defines the predefined enum constants for a protocol value of a QContactOnlineAccount.

   \value ProtocolUnknown The value stored indicates this online account is for one unsupported protocol.
   \value ProtocolAim The value stored indicates this online account is for the AIM protocol.
   \value ProtocolIcq The value stored indicates this online account is for the ICQ protocol.
   \value ProtocolIrc The value stored indicates this online account is for the IRC protocol.
   \value ProtocolJabber The value stored indicates this online account is for the jabber protocol.
   \value ProtocolMsn The value stored indicates this online account is for the MSN protocol.
   \value ProtocolQq The value stored indicates this online account is for the QQ protocol.
   \value ProtocolSkype The value stored indicates this online account is for the Skype protocol.
   \value ProtocolYahoo The value stored indicates this online account is for the Yahoo protocol.
   \sa protocol(), setProtocol()
 */

/*!
   \fn QContactOnlineAccount::setAccountUri(const QString& accountUri)

   Sets the universal resource identifier of the contact's online
   account to \a accountUri.
 */

/*!
   \fn QContactOnlineAccount::accountUri() const

   Returns the universal resource identifier of the online account of
   the contact.
 */

/*!
   \fn QContactOnlineAccount::setServiceProvider(const QString& serviceProvider)

   Sets the service provider of the contact's online account to \a
   serviceProvider.
 */

/*!
   \fn QContactOnlineAccount::serviceProvider() const
   Returns the service provider of the online account of the contact.
 */

/*!
    \fn QContactOnlineAccount::protocol() const
    Returns the protocol value.
 */

/*!
    \fn QContactOnlineAccount::setProtocol(Protocol protocol)
    Set the protocol to \a protocol.
 */

/*!
   \fn QContactOnlineAccount::setSubTypes(const QList<int>& subTypes)

   Sets the subtypes which this detail implements to be those
   contained in the list of given \a subTypes.
 */

/*!
   \fn QContactOnlineAccount::subTypes() const
   Returns the list of subtypes that this detail implements.
 */

/*!
   \fn QContactOnlineAccount::setCapabilities(const QStringList& capabilities)

   Sets the capabilities of the online account about which this detail stores
   presence information to \a capabilities.  The \a capabilities list is a
   list of service-provider specified strings which together identify the
   types of communication which may be possible.
 */

/*!
   \fn QContactOnlineAccount::capabilities() const

   Returns the capabilities of the online account about which this detail stores
   presence information.
 */

/* ==================== QContactOrganization ======================= */

/*!
   \class QContactOrganization
   \brief The QContactOrganization class provides details about an
   organization that the contact is either a part of, or stands for.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactOrganization::Type
   The enum constant for the type identifier of QContactOrganization details.
 */
const QContactDetail::DetailType QContactOrganization::Type(QContactDetail::TypeOrganization);

/*!
   \enum QContactOrganization::OrganizationField
   This enumeration defines the fields supported by QContactOrganization.
   \value FieldName The value stored in this field contains the organization name.
   \value FieldLogoUrl The value stored in this field contains the organization logo image.
   \value FieldDepartment The value stored in this field contains the department name.
   \value FieldLocation The value stored in this field contains the location of the organization.
   \value FieldRole The value stored in this field contains the contact's role in the organization.
   \value FieldTitle The value stored in this field contains the contact's title in the organization.
   \value FieldAssistantName The value stored in this field contains the contact's assistant.
   \sa department(), setDepartment()
   \sa name(), setName()
   \sa logoUrl(), setLogoUrl()
   \sa department(), setDepartment()
   \sa location(), setLocation()
   \sa role(), setRole()
   \sa title(), setTitle()
   \sa assistantName(), setAssistantName()
 */

/*!
   \fn QContactOrganization::setName(const QString& name)
   Sets the name of the organization stored in this detail to \a name.
 */

/*!
   \fn QContactOrganization::name() const
   Returns the name of the organization stored in this detail.
 */

/*!
   \fn QContactOrganization::setLogoUrl(const QUrl& logo)
   Sets the url of the logo of the organization stored in this detail to \a logo.
 */

/*!
   \fn QContactOrganization::logoUrl() const
   Returns the url of the logo of the organization stored in this detail.
 */


/*!
   \fn QContactOrganization::setDepartment(const QStringList& department)

   Sets the contact's department of the organization stored in this
   detail to \a department.  The department is a list of progressively
   finer-grained information.
 */

/*!
   \fn QContactOrganization::department() const
   Returns the contact's department stored in this detail.
 */


/*!
   \fn QContactOrganization::setLocation(const QString& location)

   Sets the location (e.g. city or suburb) of the organization stored
   in this detail to \a location.
 */

/*!
   \fn QContactOrganization::location() const
   Returns the location of the organization stored in this detail.
 */


/*!
   \fn QContactOrganization::setRole(const QString& role)
   Sets the contact's role within the organization stored in this detail to \a role.
 */

/*!
   \fn QContactOrganization::role() const
   Returns the contact's role within the organization stored in this detail.
 */


/*!
   \fn QContactOrganization::setTitle(const QString& title)
   Sets the contact's title within the organization stored in this detail to \a title.
 */

/*!
   \fn QContactOrganization::title() const
   Returns the contact's title within the organization stored in this detail.
 */

/*!
   \fn QContactOrganization::setAssistantName(const QString& assistantName)

   Sets the name of the default assistant of contacts belonging to
   this organization to \a assistantName.
 */

/*!
   \fn QContactOrganization::assistantName() const

   Returns the name of the default assistant of contacts belonging to
   this organization.
 */

/* ==================== QContactRingtone ======================= */

/*!
   \class QContactRingtone
   \brief The QContactRingtone class provides a ringtone associated
   with a contact
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactRingtone::Type
   The enum constant for the type identifier of QContactRingtone details.
*/
const QContactDetail::DetailType QContactRingtone::Type(QContactDetail::TypeRingtone);

/*!
   \enum QContactRingtone::RingtoneField
   This enumeration defines the fields supported by QContactRingtone.
   \value FieldAudioRingtoneUrl The value stored in this field contains the URL for an audio ringtone.
   \value FieldVideoRingtoneUrl The value stored in this field contains the URL for a video ringtone.
   \value FieldVibrationRingtoneUrl The value stored in this field contains the URL for a vibration ringtone.
   \sa setAudioRingtoneUrl(), audioRingtoneUrl()
   \sa setVideoRingtoneUrl(), videoRingtoneUrl()
 */

/*!
  \fn QContactRingtone::audioRingtoneUrl() const

  Returns the uri of the audio ringtone stored in the ringtone detail.
 */

/*!
  \fn QContactRingtone::setAudioRingtoneUrl(const QUrl& audioRingtoneUrl)

  Sets the uri of the audio ringtone stored in the ringtone detail
  to \a audioRingtoneUrl.
 */

/*!
  \fn QContactRingtone::videoRingtoneUrl() const

  Returns the uri of the video ringtone stored in the ringtone detail.
 */

/*!
  \fn QContactRingtone::setVideoRingtoneUrl(const QUrl& videoRingtoneUrl)

  Sets the uri of the video ringtone stored in the ringtone detail
  to \a videoRingtoneUrl.
 */

/*!
  \fn QContactRingtone::vibrationRingtoneUrl() const
  \internal

  Returns the uri of the vibration ringtone stored in the ringtone detail.
 */

/*!
  \fn QContactRingtone::setVibrationRingtoneUrl(const QUrl& vibrationRingtoneUrl)
  \internal

  Sets the uri of the vibration ringtone stored in the ringtone detail
  to \a vibrationRingtoneUrl.
 */

/* ==================== QContactPresence ======================= */

// XXX TODO add more stuff here
/*!
   \class QContactPresence
   \brief The QContactPresence class provides presence information
   for an online account of a contact.
    \inmodule QtContacts

   Presence information for a particular QContactOnlineAccount detail is provided
   in a QContactPresence detail which is linked (via linkedDetailUris()) to the
   account detail.  This information is generally provided by the backend, and is
   not modifiable by clients.

   Presence information can include update timestamp, screen name,
   and the status icon, status value, and status text provided by
   the service provider, as well as user defined status message.

   \sa QContactOnlineAccount

   \ingroup contacts-details
 */

/*!
   \variable QContactPresence::Type
   The enum constant for the type identifier of QContactPresence details.
 */
const QContactDetail::DetailType QContactPresence::Type(QContactDetail::TypePresence);

/*!
   \enum QContactPresence::PresenceField
   This enumeration defines the fields supported by QContactPresence.
   \value FieldTimestamp The value stored in this field contains the timestamp value.
   \value FieldNickname The value stored in this field contains the nickname value.
   \value FieldPresenceState The value stored in this field contains the presence state enumeration value.
   \value FieldPresenceStateText The value stored in this field contains the presence state description value.
   \value FieldPresenceStateImageUrl The value stored in this field contains the presence state image URL.
   \value FieldCustomMessage The value stored in this field contains the user-entered custom presence message.
   \sa setTimestamp(), timestamp()
   \sa setNickname(), nickname()
   \sa setPresenceState(), presenceState()
   \sa setPresenceStateText(), presenceStateText()
   \sa setPresenceStateImageUrl(), presenceStateImageUrl()
   \sa setCustomMessage(), customMessage()
 */

/*!
   \fn QContactPresence::setTimestamp(const QDateTime& updateTimestamp)

   Sets the timestamp for the last update of the presence detail to be
   \a updateTimestamp.
 */

/*!
   \fn QContactPresence::timestamp() const

    Returns the timestamp at which the data in the presence detail was valid.
 */

/*!
   \fn QContactPresence::setNickname(const QString& nickname)

   Sets the last-known nickname used by the contact during
   communications via the online account about which this detail
   stores presence information to \a nickname.
 */

/*!
   \fn QContactPresence::nickname() const

   Returns the last-known nickname used by the contact during
   communications via the online account.
 */

/*!
  \enum QContactPresence::PresenceState

  This enum defines the possible presence states supported by the default schema.
  Not all presence providers support all of these states.

  \value PresenceUnknown Signifies that the presence state of the contact is not currently known
  \value PresenceAvailable Signifies that the contact is available
  \value PresenceHidden Signifies that the contact is hidden
  \value PresenceBusy Signifies that the contact is busy
  \value PresenceAway Signifies that the contact is away
  \value PresenceExtendedAway Signifies that the contact is away for an extended period of time
  \value PresenceOffline Signifies that the contact is offline
 */

/*!
   \fn QContactPresence::setPresenceState(QContactPresence::PresenceState presenceState)

   Sets the presence state of the online account according to the presence
   information provider to the given \a presenceState.
 */

/*!
   \fn QContactPresence::presenceState() const

   Returns the presence state of the online account according to the
   presence provider.
 */

/*!
   \fn QContactPresence::setPresenceStateText(const QString& presenceStateText)

   Sets the text corresponding to the presence state to \a presenceStateText.
   This function is generally called by presence providers to allow custom
   naming of states, or to allow finer grained state reporting than is
   provided by the presence state API.
 */

/*!
   \fn QContactPresence::presenceStateText() const

   Returns the text corresponding to the current presence state.
 */

/*!
  \fn QContactPresence::setCustomMessage(const QString& customMessage)

   Sets the custom status message from the contact for the online account
   about which this detail stores presence information, to \a customMessage.
   This custom message would have been set by the contact,
   and does not necessarily correspond to a particular presence state.
 */

/*!
   \fn QContactPresence::customMessage() const

   Returns the custom status message from the contact for the online account
   about which this detail stores presence information.
 */

/*!
   \fn QContactPresence::setPresenceStateImageUrl(const QUrl& presenceStateImageUrl)

   Sets the last-known status image url of the contact for the online account
   about which this detail stores presence information, to \a presenceStateImageUrl.
 */

/*!
   \fn QContactPresence::presenceStateImageUrl() const

   Returns the last-known status image url of the contact for the online account
   about which this detail stores presence information.
 */

/* ==================== QContactGlobalPresence ======================= */

/*!
   \class QContactGlobalPresence
   \brief The QContactGlobalPresence class provides aggregated presence information
   for a contact, synthesized or supplied by the backend.
   \ingroup contacts-details
    \inmodule QtContacts
 */

/*!
   \variable QContactGlobalPresence::Type
   The enum constant for the type identifier of QContactGlobalPresence details.
 */
const QContactDetail::DetailType QContactGlobalPresence::Type(QContactDetail::TypeGlobalPresence);

/*!
   \enum QContactGlobalPresence::GlobalPresenceField
   This enumeration defines the fields supported by QContactGlobalPresence.
   \value FieldTimestamp The value stored in this field contains the timestamp value.
   \value FieldNickname The value stored in this field contains the nickname value.
   \value FieldPresenceState The value stored in this field contains the presence state enumeration value.
   \value FieldPresenceStateText The value stored in this field contains the presence state description value.
   \value FieldPresenceStateImageUrl The value stored in this field contains the presence state image URL.
   \value FieldCustomMessage The value stored in this field contains the user-entered custom presence message.
   \sa setTimestamp(), timestamp()
   \sa setNickname(), nickname()
   \sa setPresenceState(), presenceState()
   \sa setPresenceStateText(), presenceStateText()
   \sa setPresenceStateImageUrl(), presenceStateImageUrl()
   \sa setCustomMessage(), customMessage()
 */

/*!
   \fn QContactGlobalPresence::setTimestamp(const QDateTime& updateTimestamp)

   Sets the update timestamp of the global presence detail to be
   \a updateTimestamp.
 */

/*!
   \fn QContactGlobalPresence::timestamp() const

    Returns the timestamp at which the data in the global presence detail was valid.
 */

/*!
   \fn QContactGlobalPresence::setNickname(const QString& nickname)

   Sets the last-known nickname used by the contact during
   communications via any online account about which this detail
   aggregates presence information to \a nickname.
 */

/*!
   \fn QContactGlobalPresence::nickname() const

   Returns the last-known nickname used by the contact during
   communications via any online account about which this detail
   aggregates presence information.
 */

/*!
   \fn QContactGlobalPresence::setPresenceState(QContactPresence::PresenceState presenceState)

   Sets the presence state of this aggregate detail according to the presence
   information available from the presence providers which this detail aggregates
   to the given \a presenceState.
 */

/*!
   \fn QContactGlobalPresence::presenceState() const

   Returns the aggregate presence state of any online accounts about which this detail
   aggregates presence information.
 */

/*!
   \fn QContactGlobalPresence::setPresenceStateText(const QString& presenceStateText)

   Sets the text corresponding to the presence state to \a presenceStateText.
   This function is generally called by presence providers to allow custom
   naming of states, or to allow finer grained state reporting than is
   provided by the presence state API.
 */

/*!
   \fn QContactGlobalPresence::presenceStateText() const

   Returns the text corresponding to the current presence state.
 */

/*!
  \fn QContactGlobalPresence::setCustomMessage(const QString& customMessage)

   Sets the custom status message from the contact for the aggregate presence
   detail, to \a customMessage.
 */

/*!
   \fn QContactGlobalPresence::customMessage() const

   Returns the custom status message from the contact for the aggregate presence
   detail.
 */

/*!
   \fn QContactGlobalPresence::setPresenceStateImageUrl(const QUrl& presenceStateImageUrl)

   Sets the last-known status image url of the contact to \a presenceStateImageUrl.
 */

/*!
   \fn QContactGlobalPresence::presenceStateImageUrl() const

   Returns the last-known status image url of the contact.
 */

/*!
  Returns a filter which matches any contact whose global presence state
  is listed as \a state.
 */
QContactFilter QContactGlobalPresence::match(QContactPresence::PresenceState state)
{
    QContactDetailFilter f;
    f.setDetailType(QContactGlobalPresence::Type, QContactGlobalPresence::FieldPresenceState);
    f.setValue(state);
    f.setMatchFlags(QContactFilter::MatchExactly);

    return f;
}


/* ==================== QContactExtendedDetail ======================= */
/*!
    \class QContactExtendedDetail
    \brief The QContactExtendedDetail class provides the possibility to save extended details to QContact objects.
    \inmodule QtContacts
    \ingroup contacts-details

    Different back-end engines may or may not support extended details for different contact types. Even
    if supported, they may accept different QVariant types as the data. The back-end engine may convert
    the data to another type that the engine supports.
 */

/*!
    \variable QContactExtendedDetail::Type

    The constant enum which identifies the type of details which are extended details.
 */
const QContactDetail::DetailType QContactExtendedDetail::Type(QContactDetail::TypeExtendedDetail);

/*!
    \enum QContactExtendedDetail::ExtendedDetailField
    This enumeration defines the fields supported by QContactExtendedDetail.
    \value FieldName The value stored in this field contains the name of the extended detail.
    \value FieldData The value stored in this field contains the data of this extended detail.
 */

/*!
    \fn void QContactExtendedDetail::setName(const QString &name)

    Sets the \a name of this extended detail.
 */

/*!
    \fn QString QContactExtendedDetail::name() const

    Gets the name of this extended detail.
 */

/*!
    \fn void QContactExtendedDetail::setData(const QVariant &data)

    Sets the \a data of the extended detail.
 */

/*!
    \fn QVariant QContactExtendedDetail::data() const

    Gets the data of this extended detail.
 */

/* ==================== QContactVersion ======================= */
/*!
    \class QContactVersion
    \brief The QContactVersion class provides the versioning information of a QContact object.
    \inmodule QtContacts
    \ingroup contacts-details
 */

/*!
    \variable QContactVersion::Type

    The constant string which identifies the definition of details which are extended details.
 */
const QContactDetail::DetailType QContactVersion::Type(QContactDetail::TypeVersion);

/*!
    \enum QContactVersion::VersionField
    This enumeration defines the fields supported by QContactVersion.
    \value FieldSequenceNumber Contains the integer sequence number of a QContact object.
    \value FieldExtendedVersion Contains the extended version of a QContact object. It can be used to represent the version stored
    in the back-end in cases when the back-end specific version cannot be represented only by a sequence number.
 */

/*!
    \fn void QContactVersion::setSequenceNumber(int sequenceNumber)

    Sets the integer \a sequenceNumber.
 */

/*!
    \fn int QContactVersion::sequenceNumber() const

    Gets the integer sequenceNumber.
 */

/*!
    \fn void QContactVersion::setExtendedVersion(const QByteArray &extendedVersion)

    Sets the \a extendedVersion.
 */

/*!
    \fn QByteArray QContactVersion::extendedVersion() const

    Gets the extendedVersion.
 */

/* ==================== Convenience Filters ======================= */

/*!
    Returns a filter suitable for finding contacts with a display label containing the specified
    \a label.
*/
QContactFilter QContactDisplayLabel::match(const QString &label)
{
    QContactDetailFilter f;
    f.setDetailType(QContactDisplayLabel::Type, QContactDisplayLabel::FieldLabel);
    f.setValue(label);
    f.setMatchFlags(QContactFilter::MatchContains);

    return f;
}

/*!
    Returns a filter suitable for finding contacts with a name with a first name containing the
    specified \a firstName and a last name containing the specified \a lastName.  If either
    parameter is empty, any value will match that component.
*/
QContactFilter QContactName::match(const QString &firstName, const QString &lastName)
{
    if (firstName.isEmpty()) {
        if (lastName.isEmpty()) {
            // Matches contacts that have a name
            QContactDetailFilter f;
            f.setDetailType(QContactName::Type);
            return f;
        } else {
            // Contact with matching lastname
            QContactDetailFilter f;
            f.setDetailType(QContactName::Type, QContactName::FieldLastName);
            f.setValue(lastName);
            f.setMatchFlags(QContactFilter::MatchContains);
            return f;
        }
    } else {
        if (lastName.isEmpty()) {
            // Contact with matching firstName
            QContactDetailFilter f;
            f.setDetailType(QContactName::Type, QContactName::FieldFirstName);
            f.setValue(firstName);
            f.setMatchFlags(QContactFilter::MatchContains);
            return f;
        } else {
            // Match a contact with the specified first and last names
            // XXX This needs multi detail filter!

            // Best we can currently do is "and" and assume there's only one name per contact
            QContactDetailFilter f;
            f.setDetailType(QContactName::Type, QContactName::FieldFirstName);
            f.setValue(firstName);
            f.setMatchFlags(QContactFilter::MatchContains);
            QContactDetailFilter l;
            l.setDetailType(QContactName::Type, QContactName::FieldLastName);
            l.setValue(lastName);
            l.setMatchFlags(QContactFilter::MatchContains);

            return f & l;
        }
    }
}

/*!
    Returns a filter suitable for finding contacts with any name field (e.g. first, last) that
    contains the supplied \a name.
*/
QContactFilter QContactName::match(const QString &name)
{
    QContactUnionFilter nameFilter;
    QList<int> nameFields;
    nameFields << QContactName::FieldFirstName
            << QContactName::FieldLastName
            << QContactName::FieldMiddleName
            << QContactName::FieldPrefix
            << QContactName::FieldSuffix;
    foreach (int fieldName, nameFields) {
        QContactDetailFilter subFilter;
        subFilter.setDetailType(QContactName::Type, fieldName);
        subFilter.setValue(name);
        subFilter.setMatchFlags(QContactFilter::MatchContains);
        nameFilter.append(subFilter);
    }
    return nameFilter;
}

/*!
    Returns a filter suitable for finding contacts with an email address containing the specified
    \a emailAddress.
*/
QContactFilter QContactEmailAddress::match(const QString &emailAddress)
{
    QContactDetailFilter l;
    l.setDetailType(QContactEmailAddress::Type, QContactEmailAddress::FieldEmailAddress);
    l.setValue(emailAddress);
    l.setMatchFlags(QContactFilter::MatchContains);
    return l;
}

/*!
    Returns a filter suitable for finding contacts with a phone number containing the specified
    \a number.
*/
QContactFilter QContactPhoneNumber::match(const QString &number)
{
    QContactDetailFilter l;
    l.setDetailType(QContactPhoneNumber::Type, QContactPhoneNumber::FieldNumber);
    l.setValue(number);
    l.setMatchFlags(QContactFilter::MatchPhoneNumber);
    return l;
}


QTCONTACTS_END_NAMESPACE
