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

#include "qcontactdetails.h"

#include <QtCore/qfile.h>

#include "qcontactmanager.h"
#include "qcontactfilter.h"
#include "qcontacts.h"
#include "qcontactdetail_p.h"

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

QT_BEGIN_NAMESPACE_CONTACTS

/* We use offsetof to determine the offset to member fields
 * in each builtin detail type private class.
 * This is technically undefined behavior according to the
 * CPP98 spec, as the classes in question are non-POD.
 * However, they do not have any virtual functions, nor any
 * private or protected non-static data, nor do they use
 * multiple inheritance or virtual inheritance otherwise.
 * As such, all modern compilers do implement the class
 * layouts as if they were POD, and the offsetof macro
 * does indeed work in our case. */
QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Winvalid-offsetof")


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

class QContactSyncTargetPrivate : public QContactDetailBuiltinPrivate<QContactSyncTargetPrivate>
{
public:
    QString m_syncTarget;

    enum { FieldCount = 1 };

    QContactSyncTargetPrivate() : QContactDetailBuiltinPrivate<QContactSyncTargetPrivate>(QContactSyncTarget::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactSyncTargetPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactSyncTargetPrivate, m_syncTarget) },
};

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
QString QContactSyncTarget::syncTarget() const
{
    return reinterpret_cast<const QContactSyncTargetPrivate*>(d.constData())->memberValue<QString>(QContactSyncTarget::FieldSyncTarget);
}

/*!
   \fn QContactSyncTarget::setSyncTarget(const QString& syncTarget)

   Sets the identifier of the backend store to which the contact
   containing this detail should be synchronized to \a syncTarget.
 */
void QContactSyncTarget::setSyncTarget(const QString &_value)
{
    reinterpret_cast<QContactSyncTargetPrivate*>(d.data())->setMemberValue<QString>(QContactSyncTarget::FieldSyncTarget, _value);
}

/* ==================== QContactEmailAddress ======================= */


/*!
   \class QContactEmailAddress

   \brief The QContactEmailAddress class contains an email address of
   a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactEmailAddressPrivate : public QContactDetailBuiltinPrivate<QContactEmailAddressPrivate>
{
public:
    QString m_emailAddress;

    enum { FieldCount = 1 };

    QContactEmailAddressPrivate() : QContactDetailBuiltinPrivate<QContactEmailAddressPrivate>(QContactEmailAddress::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactEmailAddressPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactEmailAddressPrivate, m_emailAddress) },
};

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
QString QContactEmailAddress::emailAddress() const
{
    return reinterpret_cast<const QContactEmailAddressPrivate*>(d.constData())->memberValue<QString>(QContactEmailAddress::FieldEmailAddress);
}

/*!
   \fn QContactEmailAddress::setEmailAddress(const QString& emailAddress)
   Sets the email address of the contact which is stored in this detail to \a emailAddress.
 */
void QContactEmailAddress::setEmailAddress(const QString& _value)
{
    reinterpret_cast<QContactEmailAddressPrivate*>(d.data())->setMemberValue<QString>(QContactEmailAddress::FieldEmailAddress, _value);
}

/* ==================== QContactFamily ======================= */
/*!
   \class QContactFamily
   \brief The QContactFamily class contains names of
   family members of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactFamilyPrivate : public QContactDetailBuiltinPrivate<QContactFamilyPrivate>
{
public:
    QString m_spouse;
    QStringList m_children;

    enum { FieldCount = 2 };

    QContactFamilyPrivate() : QContactDetailBuiltinPrivate<QContactFamilyPrivate>(QContactFamily::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactFamilyPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactFamilyPrivate, m_spouse) },
    { QContactDetailBuiltinPrivateBase::StringList, offsetof(QContactFamilyPrivate, m_children) },
};

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
QString QContactFamily::spouse() const
{
    return reinterpret_cast<const QContactFamilyPrivate*>(d.constData())->memberValue<QString>(QContactFamily::FieldSpouse);
}

/*!
   \fn QContactFamily::setSpouse(const QString& spouseName)
   Sets the name of the spouse of the contact which is stored in this detail to \a spouseName.
 */
void QContactFamily::setSpouse(const QString& _value)
{
    reinterpret_cast<QContactFamilyPrivate*>(d.data())->setMemberValue<QString>(QContactFamily::FieldSpouse, _value);
}

/*!
   \fn QContactFamily::children() const
   Returns the names of the children of the contact which is stored in this detail.
 */
QStringList QContactFamily::children() const
{
    return reinterpret_cast<const QContactFamilyPrivate*>(d.constData())->memberValue<QStringList>(QContactFamily::FieldChildren);
}

/*!
   \fn QContactFamily::setChildren(const QStringList& childrenNames)
   Sets the names of the children of the contact which is stored in this detail to \a childrenNames.
 */
void QContactFamily::setChildren(const QStringList &_value)
{
    reinterpret_cast<QContactFamilyPrivate*>(d.data())->setMemberValue<QStringList>(QContactFamily::FieldChildren, _value);
}

/* ==================== QContactFavorite ======================= */
/*!
   \class QContactFavorite
   \brief The QContactFavorite class indicates if a contact is a favorite contact as well as the
   position it should appear in an ordered list of favorites.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactFavoritePrivate : public QContactDetailBuiltinPrivate<QContactFavoritePrivate>
{
public:
    bool m_favorite;
    int m_index;

    enum { FieldCount = 2 };

    QContactFavoritePrivate() : QContactDetailBuiltinPrivate<QContactFavoritePrivate>(QContactFavorite::Type), m_favorite(false), m_index(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactFavoritePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::Bool, offsetof(QContactFavoritePrivate, m_favorite) },
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactFavoritePrivate, m_index) },
};

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
bool QContactFavorite::isFavorite() const
{
    return reinterpret_cast<const QContactFavoritePrivate*>(d.constData())->memberValue<bool>(QContactFavorite::FieldFavorite);
}
/*!
   \fn QContactFavorite::setFavorite(bool isFavorite)
   If \a isFavorite is true, marks the contact as a favorite.  Otherwise, marks the contact as not a favorite.
 */
void QContactFavorite::setFavorite(bool _value)
{
    reinterpret_cast<QContactFavoritePrivate*>(d.data())->setMemberValue<bool>(QContactFavorite::FieldFavorite, _value);
}

/*!
   \fn int QContactFavorite::index() const
   Returns the index of the favorite contact.
 */
int QContactFavorite::index() const
{
    return reinterpret_cast<const QContactFavoritePrivate*>(d.constData())->memberValue<int>(QContactFavorite::FieldIndex);
}

/*!
   \fn QContactFavorite::setIndex(int index)
   Sets the index of the favorite contact to \a index.
 */
void QContactFavorite::setIndex(int _value)
{
    reinterpret_cast<QContactFavoritePrivate*>(d.data())->setMemberValue<int>(QContactFavorite::FieldIndex, _value);
}

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

class QContactAnniversaryPrivate : public QContactDetailBuiltinPrivate<QContactAnniversaryPrivate>
{
public:
    QString m_calendarId;
    QDateTime m_originalDate;
    QString m_event;
    int m_subType;

    enum { FieldCount = 4 };

    QContactAnniversaryPrivate() : QContactDetailBuiltinPrivate<QContactAnniversaryPrivate>(QContactAnniversary::Type), m_subType(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactAnniversaryPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAnniversaryPrivate, m_calendarId) },
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactAnniversaryPrivate, m_originalDate) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAnniversaryPrivate, m_event) },
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactAnniversaryPrivate, m_subType) },
};

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
QDate QContactAnniversary::originalDate() const
{
    return (reinterpret_cast<const QContactAnniversaryPrivate*>(d.constData()))->memberValue<QDateTime>(QContactAnniversary::FieldOriginalDate).date();
}

/*!
   \fn QContactAnniversary::setOriginalDate(const QDate& date)
   Sets the original date of the event stored in this detail to \a date.
 */
void QContactAnniversary::setOriginalDate(const QDate& _value)
{
    reinterpret_cast<QContactAnniversaryPrivate*>(d.data())->setMemberValue<QDateTime>(QContactAnniversary::FieldOriginalDate, QDateTime(_value));
}

/*!
   \fn QContactAnniversary::originalDateTime() const
   Returns the original date and time of the event stored in this detail.
   If the original event occurrence stored is a QDate, this returns a QDateTime with the
   time set to midnight.
 */
QDateTime QContactAnniversary::originalDateTime() const
{
    return reinterpret_cast<const QContactAnniversaryPrivate*>(d.constData())->memberValue<QDateTime>(QContactAnniversary::FieldOriginalDate);
}

/*!
   \fn QContactAnniversary::setOriginalDateTime(const QDateTime& dateTime)
   Sets the original date and time of the event stored in this detail to \a dateTime.
 */
void QContactAnniversary::setOriginalDateTime(const QDateTime& _value)
{
    reinterpret_cast<QContactAnniversaryPrivate*>(d.data())->setMemberValue<QDateTime>(QContactAnniversary::FieldOriginalDate, _value);
}

/*!
   \fn QContactAnniversary::calendarId() const
 * Returns the identifier of the calendar entry associated with this anniversary.
 */
QString QContactAnniversary::calendarId() const
{
    return reinterpret_cast<const QContactAnniversaryPrivate*>(d.constData())->memberValue<QString>(QContactAnniversary::FieldCalendarId);
}

/*!
   \fn QContactAnniversary::setCalendarId(const QString& calendarId)
   Sets the identifier of the calendar entry associated with this anniversary to \a calendarId.
 */
void QContactAnniversary::setCalendarId(const QString& _value)
{
    reinterpret_cast<QContactAnniversaryPrivate*>(d.data())->setMemberValue<QString>(QContactAnniversary::FieldCalendarId, _value);
}

/*!
   \fn QContactAnniversary::event() const
   Returns the name of the event for which this detail contains information.
 */
QString QContactAnniversary::event() const
{
    return reinterpret_cast<const QContactAnniversaryPrivate*>(d.constData())->memberValue<QString>(QContactAnniversary::FieldEvent);
}

/*!
   \fn QContactAnniversary::setEvent(const QString& event)
   Sets the name of the event for which this detail contains information to \a event.
 */
void QContactAnniversary::setEvent(const QString& _value)
{
    reinterpret_cast<QContactAnniversaryPrivate*>(d.data())->setMemberValue<QString>(QContactAnniversary::FieldEvent, _value);
}

/*!
   \fn QContactAnniversary::setSubType(const QContactAnniversary::SubType &subType)
   Sets the subtype which this detail implements to be the given \a subType.
 */
void QContactAnniversary::setSubType(QContactAnniversary::SubType _value)
{
    reinterpret_cast<QContactAnniversaryPrivate*>(d.data())->setMemberValue<int>(QContactAnniversary::FieldSubType, static_cast<int>(_value));
}

/*!
   \fn QContactAnniversary::subType() const
   Returns the subtype that this detail implements, if defined.
 */
QContactAnniversary::SubType QContactAnniversary::subType() const
{
    return static_cast<QContactAnniversary::SubType>(reinterpret_cast<const QContactAnniversaryPrivate*>(d.constData())->memberValue<int>(QContactAnniversary::FieldSubType));
}

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

class QContactAvatarPrivate : public QContactDetailBuiltinPrivate<QContactAvatarPrivate>
{
public:
    QUrl m_imageUrl;
    QUrl m_videoUrl;
    QString m_metaData;

    enum { FieldCount = 3 };

    QContactAvatarPrivate() : QContactDetailBuiltinPrivate<QContactAvatarPrivate>(QContactAvatar::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactAvatarPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactAvatarPrivate, m_imageUrl) },
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactAvatarPrivate, m_videoUrl) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAvatarPrivate, m_metaData) },
};

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
QUrl QContactAvatar::imageUrl() const
{
    return reinterpret_cast<const QContactAvatarPrivate*>(d.constData())->memberValue<QUrl>(QContactAvatar::FieldImageUrl);
}

/*!
  \fn QContactAvatar::setImageUrl(const QUrl& imageUrl)
  Sets the url of an avatar image associated with the contact to \a imageUrl
 */
void QContactAvatar::setImageUrl(const QUrl& _value)
{
    reinterpret_cast<QContactAvatarPrivate*>(d.data())->setMemberValue<QUrl>(QContactAvatar::FieldImageUrl, _value);
}

/*!
  \fn QContactAvatar::videoUrl() const
  Returns the url of an avatar video associated with the contact
 */
QUrl QContactAvatar::videoUrl() const
{
    return reinterpret_cast<const QContactAvatarPrivate*>(d.constData())->memberValue<QUrl>(QContactAvatar::FieldVideoUrl);
}

/*!
  \fn QContactAvatar::setVideoUrl(const QUrl& videoUrl)
  Sets the url of an avatar video associated with the contact to \a videoUrl
 */
void QContactAvatar::setVideoUrl(const QUrl& _value)
{
    reinterpret_cast<QContactAvatarPrivate*>(d.data())->setMemberValue<QUrl>(QContactAvatar::FieldVideoUrl, _value);
}

/*!
   \fn QContactAvatar::metaData() const
   Returns the meta data associated with the avatar url.
 */
QString QContactAvatar::metaData() const
{
    return reinterpret_cast<const QContactAvatarPrivate*>(d.constData())->memberValue<QString>(QContactAvatar::FieldMetaData);
}

/*!
   \fn QContactAvatar::setMetaData(const QString& metaData)
   Sets the meta data associated with the avatar url to \a metaData.
 */
void QContactAvatar::setMetaData(const QString& _value)
{
    reinterpret_cast<QContactAvatarPrivate*>(d.data())->setMemberValue<QString>(QContactAvatar::FieldMetaData, _value);
}

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

class QContactAddressPrivate : public QContactDetailBuiltinPrivate<QContactAddressPrivate>
{
public:
    QString m_street;
    QString m_locality;
    QString m_region;
    QString m_postcode;
    QString m_country;
    QList<int> m_subTypes;
    QString m_pobox;

    enum { FieldCount = 7 };

    QContactAddressPrivate() : QContactDetailBuiltinPrivate<QContactAddressPrivate>(QContactAddress::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactAddressPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAddressPrivate, m_street) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAddressPrivate, m_locality) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAddressPrivate, m_region) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAddressPrivate, m_postcode) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAddressPrivate, m_country) },
    { QContactDetailBuiltinPrivateBase::IntList, offsetof(QContactAddressPrivate, m_subTypes) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactAddressPrivate, m_pobox) },
};

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
QString QContactAddress::postOfficeBox() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QString>(QContactAddress::FieldPostOfficeBox);
}

/*!
   \fn QContactAddress::setPostOfficeBox(const QString& postOfficeBox)
   Sets the post office box segment of the address stored in this detail to \a postOfficeBox.
 */
void QContactAddress::setPostOfficeBox(const QString& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldPostOfficeBox, _value);
}

/*!
   \fn QContactAddress::street() const
   Returns the street segment of the address stored in this detail.
 */
QString QContactAddress::street() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QString>(QContactAddress::FieldStreet);
}

/*!
   \fn QContactAddress::setStreet(const QString& street)
   Sets the street segment of the address stored in this detail to \a street.
 */
void QContactAddress::setStreet(const QString& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldStreet, _value);
}

/*!
   \fn QContactAddress::locality() const
   Returns the locality segment of the address stored in this detail.
 */
QString QContactAddress::locality() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QString>(QContactAddress::FieldLocality);
}

/*!
   \fn QContactAddress::setLocality(const QString& locality)
   Sets the locality segment of the address stored in this detail to \a locality.
 */
void QContactAddress::setLocality(const QString& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldLocality, _value);
}

/*!
   \fn QContactAddress::region() const
   Returns the region segment of the address stored in this detail.
 */
QString QContactAddress::region() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QString>(QContactAddress::FieldRegion);
}

/*!
   \fn QContactAddress::setRegion(const QString& region)
   Sets the region segment of the address stored in this detail to \a region.
 */
void QContactAddress::setRegion(const QString& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldRegion, _value);
}

/*!
   \fn QContactAddress::postcode() const
   Returns the postcode segment of the address stored in this detail.
 */
QString QContactAddress::postcode() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QString>(QContactAddress::FieldPostcode);
}

/*!
   \fn QContactAddress::setPostcode(const QString& postcode)
   Sets the postcode segment of the address stored in this detail to \a postcode.
 */
void QContactAddress::setPostcode(const QString& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldPostcode, _value);
}

/*!
   \fn QContactAddress::country() const
   Returns the country segment of the address stored in this detail.
 */
QString QContactAddress::country() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QString>(QContactAddress::FieldCountry);
}

/*!
   \fn QContactAddress::setCountry(const QString& country)
   Sets the country segment of the address stored in this detail to \a country.
 */
void QContactAddress::setCountry(const QString& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldCountry, _value);
}

/*!
   \fn QContactAddress::setSubTypes(const QList<int> &subTypes)
   Sets the subtypes which this detail implements to be those contained in the list of given \a subTypes.
 */
void QContactAddress::setSubTypes(const QList<int>& _value)
{
    reinterpret_cast<QContactAddressPrivate*>(d.data())->setMemberValue(QContactAddress::FieldSubTypes, _value);
}

/*!
   \fn QContactAddress::subTypes() const
   Returns the list of subtypes that this detail implements.
 */
QList<int> QContactAddress::subTypes() const
{
    return reinterpret_cast<const QContactAddressPrivate*>(d.constData())->memberValue<QList<int> >(QContactAddress::FieldSubTypes);
}


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

class QContactUrlPrivate : public QContactDetailBuiltinPrivate<QContactUrlPrivate>
{
public:
    QString m_url;
    int m_subType;

    enum { FieldCount = 2 };

    QContactUrlPrivate() : QContactDetailBuiltinPrivate<QContactUrlPrivate>(QContactUrl::Type), m_subType(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactUrlPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactUrlPrivate, m_url) },
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactUrlPrivate, m_subType) },
};

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
   \fn QContactUrl::setUrl(const QUrl& url)
   Sets the url stored in this detail to the string representation
   of the given \a url.
 */
void QContactUrl::setUrl(const QUrl& _value)
{
    reinterpret_cast<QContactUrlPrivate*>(d.data())->setMemberValue<QString>(QContactUrl::FieldUrl, _value.toString());
}

/*!
   \fn QContactUrl::url() const
   Returns the url stored in this detail.
 */
QString QContactUrl::url() const
{
    return reinterpret_cast<const QContactUrlPrivate*>(d.constData())->memberValue<QString>(QContactUrl::FieldUrl);
}

/*!
   \fn QContactUrl::setUrl(const QString& url)
   Sets the url stored in this detail to \a url.
 */
void QContactUrl::setUrl(const QString& _value)
{
    reinterpret_cast<QContactUrlPrivate*>(d.data())->setMemberValue<QString>(QContactUrl::FieldUrl, _value);
}

/*!
   \fn QContactUrl::setSubType(const QContactUrl::SubType& subType)
   Sets the subtype which this detail implements to be the given \a subType.
 */
void QContactUrl::setSubType(QContactUrl::SubType _value)
{
    reinterpret_cast<QContactUrlPrivate*>(d.data())->setMemberValue<int>(QContactUrl::FieldSubType, static_cast<int>(_value));
}

/*!
   \fn QContactUrl::subType() const
   Returns the subtype that this detail implements, if defined.
 */
QContactUrl::SubType QContactUrl::subType() const
{
    return static_cast<QContactUrl::SubType>(reinterpret_cast<const QContactUrlPrivate*>(d.constData())->memberValue<int>(QContactUrl::FieldSubType));
}

/* ==================== QContactPhonenumber ======================= */

/*!
    \class QContactPhoneNumber
    \brief The QContactPhoneNumber class provides a phone number
    of a contact.
    \ingroup contacts-details
    \inmodule QtContacts
*/

class QContactPhoneNumberPrivate : public QContactDetailBuiltinPrivate<QContactPhoneNumberPrivate>
{
public:
    QString m_number;
    QString m_normalizedNumber;
    QList<int> m_subTypes;

    enum { FieldCount = 3 };

    QContactPhoneNumberPrivate() : QContactDetailBuiltinPrivate<QContactPhoneNumberPrivate>(QContactPhoneNumber::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactPhoneNumberPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactPhoneNumberPrivate, m_number) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactPhoneNumberPrivate, m_normalizedNumber) },
    { QContactDetailBuiltinPrivateBase::IntList, offsetof(QContactPhoneNumberPrivate, m_subTypes) },
};


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
QString QContactPhoneNumber::number() const
{
    return reinterpret_cast<const QContactPhoneNumberPrivate*>(d.constData())->memberValue<QString>(QContactPhoneNumber::FieldNumber);
}

/*!
   \fn QContactPhoneNumber::setNumber(const QString& number)
   Sets the phone number stored in this detail to \a number.
 */
void QContactPhoneNumber::setNumber(const QString& _value)
{
    reinterpret_cast<QContactPhoneNumberPrivate*>(d.data())->setMemberValue<QString>(QContactPhoneNumber::FieldNumber, _value);
}

/*!
   \fn QContactPhoneNumber::normalizedNumber() const
   Returns the normalized version of the phone number stored in this detail.
   This value may be backend-generated, and may not be exportable.
 */
QString QContactPhoneNumber::normalizedNumber() const
{
    return reinterpret_cast<const QContactPhoneNumberPrivate*>(d.constData())->memberValue<QString>(QContactPhoneNumber::FieldNormalizedNumber);
}

/*!
   \fn QContactPhoneNumber::setNormalizedNumber(const QString& normalizedNumber)
   Sets the normalized version of the phone number stored in this detail to \a normalizedNumber.
 */
void QContactPhoneNumber::setNormalizedNumber(const QString& _value)
{
    reinterpret_cast<QContactPhoneNumberPrivate*>(d.data())->setMemberValue<QString>(QContactPhoneNumber::FieldNormalizedNumber, _value);
}

/*!
   \fn QContactPhoneNumber::setSubTypes(const QList<int>& subTypes)
   Sets the subtypes which this detail implements to be those contained in the list of given \a subTypes
 */
void QContactPhoneNumber::setSubTypes(const QList<int>& _value)
{
    reinterpret_cast<QContactPhoneNumberPrivate*>(d.data())->setMemberValue<QList<int> >(QContactPhoneNumber::FieldSubTypes, _value);
}

/*!
   \fn QContactPhoneNumber::subTypes() const
   Returns the list of subtypes that this detail implements.
 */
QList<int> QContactPhoneNumber::subTypes() const
{
    return reinterpret_cast<const QContactPhoneNumberPrivate*>(d.constData())->memberValue<QList<int> >(QContactPhoneNumber::FieldSubTypes);
}

/* ==================== QContactBirthday ======================= */

/*!
   \class QContactBirthday
   \brief The QContactBirthday class contains a birthday of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactBirthdayPrivate : public QContactDetailBuiltinPrivate<QContactBirthdayPrivate>
{
public:
    QDateTime m_birthday;
    QString m_calendarId;

    enum { FieldCount = 2 };

    QContactBirthdayPrivate() : QContactDetailBuiltinPrivate<QContactBirthdayPrivate>(QContactBirthday::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactBirthdayPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactBirthdayPrivate, m_birthday) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactBirthdayPrivate, m_calendarId) },
};

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
QDate QContactBirthday::date() const
{
    return reinterpret_cast<const QContactBirthdayPrivate*>(d.constData())->memberValue<QDateTime>(QContactBirthday::FieldBirthday).date();
}

/*!
   \fn QContactBirthday::setDate(const QDate& date)
   Sets the date of the birthday which is stored in this detail to \a date.
 */
void QContactBirthday::setDate(const QDate& _value)
{
    reinterpret_cast<QContactBirthdayPrivate*>(d.data())->setMemberValue<QDateTime>(QContactBirthday::FieldBirthday, QDateTime(_value));
}

/*!
   \fn QContactBirthday::dateTime() const
   Returns the date and time of the birthday which is stored in this detail.
   If the birthday stored is a QDate, this returns a QDateTime with the
   time set to midnight.
 */
QDateTime QContactBirthday::dateTime() const
{
    return reinterpret_cast<const QContactBirthdayPrivate*>(d.constData())->memberValue<QDateTime>(QContactBirthday::FieldBirthday);
}

/*!
   \fn QContactBirthday::setDateTime(const QDateTime& dateTime)
   Sets the date and time of the birthday which is stored in this detail to \a dateTime.
 */
void QContactBirthday::setDateTime(const QDateTime& _value)
{
    reinterpret_cast<QContactBirthdayPrivate*>(d.data())->setMemberValue<QDateTime>(QContactBirthday::FieldBirthday, _value);
}


/*!
   \fn QContactBirthday::calendarId() const
 * Returns the identifier of the calendar entry associated with this birthday.
 */
QString QContactBirthday::calendarId() const
{
    return reinterpret_cast<const QContactBirthdayPrivate*>(d.constData())->memberValue<QString>(QContactBirthday::FieldCalendarId);
}

/*!
   \fn QContactBirthday::setCalendarId(const QString& calendarId)
   Sets the identifier of the calendar entry associated with this birthday to \a calendarId.
 */
void QContactBirthday::setCalendarId(const QString& _value)
{
    reinterpret_cast<QContactBirthdayPrivate*>(d.data())->setMemberValue<QString>(QContactBirthday::FieldCalendarId, _value);
}

/* ==================== QContactDisplayLabel ======================= */

/*!
   \class QContactDisplayLabel
   \brief The QContactDisplayLabel class contains a displayLabel of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactDisplayLabelPrivate : public QContactDetailBuiltinPrivate<QContactDisplayLabelPrivate>
{
public:
    QString m_label;

    enum { FieldCount = 1 };

    QContactDisplayLabelPrivate() : QContactDetailBuiltinPrivate<QContactDisplayLabelPrivate>(QContactDisplayLabel::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactDisplayLabelPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactDisplayLabelPrivate, m_label) },
};

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
void QContactDisplayLabel::setLabel(const QString& _value)
{
    reinterpret_cast<QContactDisplayLabelPrivate*>(d.data())->setMemberValue<QString>(QContactDisplayLabel::FieldLabel, _value);
}

/*!
   \fn QContactDisplayLabel::label() const
   Returns the displayLabel of the contact which is stored in this detail.
 */
QString QContactDisplayLabel::label() const
{
    return reinterpret_cast<const QContactDisplayLabelPrivate*>(d.constData())->memberValue<QString>(QContactDisplayLabel::FieldLabel);
}


/* ==================== QContactGender ======================= */

/*!
   \class QContactGender
   \brief The QContactGender class contains the gender of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactGenderPrivate : public QContactDetailBuiltinPrivate<QContactGenderPrivate>
{
public:
    int m_gender;

    enum { FieldCount = 1 };

    QContactGenderPrivate() : QContactDetailBuiltinPrivate<QContactGenderPrivate>(QContactGender::Type), m_gender(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactGenderPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactGenderPrivate, m_gender) },
};

/*!
   \variable QContactGender::Type
   The enum constant for the type identifier of QContactGender details.
 */
const QContactDetail::DetailType QContactGender::Type(QContactDetail::TypeGender);

/*!
   \enum QContactGender::GenderField
   This enumeration defines the fields supported by QContactGender.
   \value FieldGender The value stored in this field contains the gender.
   \sa gender(), setGender()
 */

/*!
   \enum QContactGender::GenderType
   This enumeration defines built-in gender values
   \value GenderUnspecified The value that identifies this contact as being of unspecified gender.
   \value GenderMale The value that identifies this contact as being male.
   \value GenderFemale The value that identifies this contact as being female.
   \sa gender(), setGender()
 */

/*!
   \fn QContactGender::gender() const

   Returns the gender of the contact, as stored in this detail.  The
   possible values for the value stored are "Male", "Female" and
   "Unspecified".
 */
QContactGender::GenderType QContactGender::gender() const
{
    return static_cast<QContactGender::GenderType>(reinterpret_cast<const QContactGenderPrivate*>(d.constData())->memberValue<int>(QContactGender::FieldGender));
}

/*!
   \fn QContactGender::setGender(GenderType gender)

   Sets the gender of the contact (as stored in this detail) to \a
   gender, if \a gender is either GenderMale or GenderFemale, otherwise sets
   it to GenderUnspecified.
 */
void QContactGender::setGender(QContactGender::GenderType _value)
{
    reinterpret_cast<QContactGenderPrivate*>(d.data())->setMemberValue<int>(QContactGender::FieldGender, static_cast<int>(_value));
}

/* ==================== QContactGeolocation ======================= */

/*!
   \class QContactGeoLocation
   \brief The QContactGeoLocation class contains a global location
   coordinate associated with a contact.
   \ingroup contacts-details
    \inmodule QtContacts
*/

class QContactGeoLocationPrivate : public QContactDetailBuiltinPrivate<QContactGeoLocationPrivate>
{
public:
    QString m_label;
    double m_latitude;
    double m_longitude;
    double m_accuracy;
    double m_altitude;
    double m_altitudeAccuracy;
    double m_heading;
    double m_speed;
    QDateTime m_timestamp;

    enum { FieldCount = 9 };

    QContactGeoLocationPrivate() : QContactDetailBuiltinPrivate<QContactGeoLocationPrivate>(QContactGeoLocation::Type),
        m_latitude(0.0), m_longitude(0.0), m_accuracy(0.0), m_altitude(0.0), m_altitudeAccuracy(0.0), m_heading(0.0), m_speed(0.0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactGeoLocationPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactGeoLocationPrivate, m_label) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_latitude) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_longitude) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_accuracy) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_altitude) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_altitudeAccuracy) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_heading) },
    { QContactDetailBuiltinPrivateBase::Double, offsetof(QContactGeoLocationPrivate, m_speed) },
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactGeoLocationPrivate, m_timestamp) },
};

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
void QContactGeoLocation::setLabel(const QString& _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<QString>(QContactGeoLocation::FieldLabel, _value);
}

/*!
   \fn QContactGeoLocation::label() const
   Returns the label of the location stored in the detail.
 */
QString QContactGeoLocation::label() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<QString>(QContactGeoLocation::FieldLabel);
}

/*!
   \fn QContactGeoLocation::setLatitude(double latitude)

   Sets the latitude portion of the coordinate (in decimal degrees) of
   the location stored in the detail to \a latitude.
 */
void QContactGeoLocation::setLatitude(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldLatitude, _value);
}

/*!
   \fn QContactGeoLocation::latitude() const

   Returns the latitude portion of the coordinate (specified in
   decimal degrees) of the location stored in the detail.
 */
double QContactGeoLocation::latitude() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldLatitude);
}

/*!
   \fn QContactGeoLocation::setLongitude(double longitude)

   Sets the longitude portion of the coordinate (in decimal degrees)
   of the location stored in the detail to \a longitude.
 */
void QContactGeoLocation::setLongitude(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldLongitude, _value);
}

/*!
   \fn QContactGeoLocation::longitude() const

   Returns the longitude portion of the coordinate (specified in
   decimal degrees) of the location stored in the detail.
 */
double QContactGeoLocation::longitude() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldLongitude);
}

/*!
   \fn QContactGeoLocation::setAccuracy(double accuracy)

   Specifies that the latitude and longitude portions of the location
   stored in the detail are accurate to within \a accuracy metres.
 */
void QContactGeoLocation::setAccuracy(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldAccuracy, _value);
}

/*!
   \fn QContactGeoLocation::accuracy() const

   Returns the accuracy (in metres) of the latitude and longitude of
   the location stored in the detail.
 */
double QContactGeoLocation::accuracy() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldAccuracy);
}

/*!
   \fn QContactGeoLocation::setAltitude(double altitude)

   Sets the altitude portion of the coordinate (in metres above the
   ellipsoid) of the location stored in the detail to \a altitude.
 */
void QContactGeoLocation::setAltitude(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldAltitude, _value);
}

/*!
   \fn QContactGeoLocation::altitude() const
   Returns the altitude (in metres) of the location stored in the detail.
 */
double QContactGeoLocation::altitude() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldAltitude);
}

/*!
   \fn QContactGeoLocation::setAltitudeAccuracy(double altitudeAccuracy)

   Sets the altitude-accuracy portion of the coordinate (in metres) of
   the location stored in the detail to \a altitudeAccuracy.
 */
void QContactGeoLocation::setAltitudeAccuracy(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldAltitudeAccuracy, _value);
}

/*!
   \fn QContactGeoLocation::altitudeAccuracy() const

   Returns the accuracy of the altitude portion of the location stored
   in the detail.
 */
double QContactGeoLocation::altitudeAccuracy() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldAltitudeAccuracy);
}

/*!
   \fn QContactGeoLocation::setHeading(double heading)

   Sets the heading portion of the coordinate (in decimal degrees
   clockwise relative to true north) of the location-aware device at
   the time of measurement to \a heading.
 */
void QContactGeoLocation::setHeading(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldHeading, _value);
}

/*!
   \fn QContactGeoLocation::heading() const

   Returns the heading (at the time of measurement) of the
   location-aware device that recorded (or was provided) the
   measurement.
 */
double QContactGeoLocation::heading() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldHeading);
}

/*!
   \fn QContactGeoLocation::setSpeed(double speed)

   Sets the speed portion of the coordinate (in metres per second) of
   the location-aware device at the time of measurement to \a speed.
 */
void QContactGeoLocation::setSpeed(double _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<double>(QContactGeoLocation::FieldSpeed, _value);
}

/*!
   \fn QContactGeoLocation::speed() const

   Returns the speed (at the time of measurement) of the
   location-aware device that recorded (or was provided) the
   measurement.
 */
double QContactGeoLocation::speed() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<double>(QContactGeoLocation::FieldSpeed);
}

/*!
   \fn QContactGeoLocation::setTimestamp(const QDateTime& timestamp)

   Sets the creation (or first-valid) timestamp of the location
   information to \a timestamp.
 */
void QContactGeoLocation::setTimestamp(const QDateTime& _value)
{
    reinterpret_cast<QContactGeoLocationPrivate*>(d.data())->setMemberValue<QDateTime>(QContactGeoLocation::FieldTimestamp, _value);
}

/*!
   \fn QContactGeoLocation::timestamp() const

   Returns the timestamp associated with the location stored in the
   detail.
 */
QDateTime QContactGeoLocation::timestamp() const
{
    return reinterpret_cast<const QContactGeoLocationPrivate*>(d.constData())->memberValue<QDateTime>(QContactGeoLocation::FieldTimestamp);
}

/* ==================== QContactGuid ======================= */

/*!
   \class QContactGuid
   \brief The QContactGuid class contains a globally unique
   Id of a contact, for use in synchronization with other datastores.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactGuidPrivate : public QContactDetailBuiltinPrivate<QContactGuidPrivate>
{
public:
    QString m_guid;

    enum { FieldCount = 1 };

    QContactGuidPrivate() : QContactDetailBuiltinPrivate<QContactGuidPrivate>(QContactGuid::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactGuidPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactGuidPrivate, m_guid) },
};

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
QString QContactGuid::guid() const
{
    return reinterpret_cast<const QContactGuidPrivate*>(d.constData())->memberValue<QString>(QContactGuid::FieldGuid);
}

/*!
   \fn QContactGuid::setGuid(const QString& guid)
   Sets the globally unique identifier which is stored in this detail to \a guid.
 */
void QContactGuid::setGuid(const QString& _value)
{
    reinterpret_cast<QContactGuidPrivate*>(d.data())->setMemberValue<QString>(QContactGuid::FieldGuid, _value);
}

/* ==================== QContactHobby ======================= */

/*!
   \class QContactHobby
   \brief The QContactHobby class contains a hobby of the contact.
   \ingroup contacts-details
    \inmodule QtContacts

   A contact may have one or more hobbies.  Each QContactHobby
   detail contains information about a single hobby of the contact.
 */

class QContactHobbyPrivate : public QContactDetailBuiltinPrivate<QContactHobbyPrivate>
{
public:
    QString m_hobby;

    enum { FieldCount = 1 };

    QContactHobbyPrivate() : QContactDetailBuiltinPrivate<QContactHobbyPrivate>(QContactHobby::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactHobbyPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactHobbyPrivate, m_hobby) },
};

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
void QContactHobby::setHobby(const QString& _value)
{
    reinterpret_cast<QContactHobbyPrivate*>(d.data())->setMemberValue<QString>(QContactHobby::FieldHobby, _value);
}

/*!
   \fn QContactHobby::hobby() const
   Returns the hobby associated with a contact which is stored in this detail.
 */
QString QContactHobby::hobby() const
{
    return reinterpret_cast<const QContactHobbyPrivate*>(d.constData())->memberValue<QString>(QContactHobby::FieldHobby);
}


/* ==================== QContactName ======================= */

/*!
   \class QContactName
   \brief The QContactName class contains a name of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactNamePrivate : public QContactDetailBuiltinPrivate<QContactNamePrivate>
{
public:
    QString m_prefix;
    QString m_firstName;
    QString m_middleName;
    QString m_lastName;
    QString m_suffix;
    QString m_customLabel;

    enum { FieldCount = 6 };

    QContactNamePrivate() : QContactDetailBuiltinPrivate<QContactNamePrivate>(QContactName::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactNamePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNamePrivate, m_prefix) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNamePrivate, m_firstName) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNamePrivate, m_middleName) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNamePrivate, m_lastName) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNamePrivate, m_suffix) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNamePrivate, m_customLabel) },
};

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
QString QContactName::prefix() const
{
    return reinterpret_cast<const QContactNamePrivate*>(d.constData())->memberValue<QString>(QContactName::FieldPrefix);
}

/*!
   \fn QContactName::setPrefix(const QString& prefix)
   Sets the prefix segment of the name stored in this detail to \a prefix.
 */
void QContactName::setPrefix(const QString& _value)
{
    reinterpret_cast<QContactNamePrivate*>(d.data())->setMemberValue<QString>(QContactName::FieldPrefix, _value);
}

/*!
   \fn QContactName::firstName() const
   Returns the first (given) name segment of the name stored in this detail.
 */
QString QContactName::firstName() const
{
    return reinterpret_cast<const QContactNamePrivate*>(d.constData())->memberValue<QString>(QContactName::FieldFirstName);
}

/*!
   \fn QContactName::setFirstName(const QString& firstName)
   Sets the first name segment of the name stored in this detail to \a firstName.
 */
void QContactName::setFirstName(const QString& _value)
{
    reinterpret_cast<QContactNamePrivate*>(d.data())->setMemberValue<QString>(QContactName::FieldFirstName, _value);
}

/*!
   \fn QContactName::middleName() const

   Returns the middle (additional, or other) name segment of the name
   stored in this detail.
 */
QString QContactName::middleName() const
{
    return reinterpret_cast<const QContactNamePrivate*>(d.constData())->memberValue<QString>(QContactName::FieldMiddleName);
}

/*!
   \fn QContactName::setMiddleName(const QString& middleName)
   Sets the middle name segment of the name stored in this detail to \a middleName.
 */
void QContactName::setMiddleName(const QString& _value)
{
    reinterpret_cast<QContactNamePrivate*>(d.data())->setMemberValue<QString>(QContactName::FieldMiddleName, _value);
}

/*!
   \fn QContactName::lastName() const

   Returns the last (family, or surname) name segment of the name
   stored in this detail.
 */
QString QContactName::lastName() const
{
    return reinterpret_cast<const QContactNamePrivate*>(d.constData())->memberValue<QString>(QContactName::FieldLastName);
}

/*!
   \fn QContactName::setLastName(const QString& lastName)
   Sets the last name segment of the name stored in this detail to \a lastName.
 */
void QContactName::setLastName(const QString& _value)
{
    reinterpret_cast<QContactNamePrivate*>(d.data())->setMemberValue<QString>(QContactName::FieldLastName, _value);
}

/*!
   \fn QContactName::suffix() const
   Returns the suffix segment of the name stored in this detail.
 */
QString QContactName::suffix() const
{
    return reinterpret_cast<const QContactNamePrivate*>(d.constData())->memberValue<QString>(QContactName::FieldSuffix);
}

/*!
   \fn QContactName::setSuffix(const QString& suffix)
   Sets the suffix segment of the name stored in this detail to \a suffix.
 */
void QContactName::setSuffix(const QString& _value)
{
    reinterpret_cast<QContactNamePrivate*>(d.data())->setMemberValue<QString>(QContactName::FieldSuffix, _value);
}

/*!
   \fn QContactName::customLabel() const
   Returns the custom label data for the name stored in this detail.
 */
QString QContactName::customLabel() const
{
    return reinterpret_cast<const QContactNamePrivate*>(d.constData())->memberValue<QString>(QContactName::FieldCustomLabel);
}

/*!
   \fn QContactName::setCustomLabel(const QString& customLabel)
   Sets the custom label of the name stored in this detail to \a customLabel.
 */
void QContactName::setCustomLabel(const QString& _value)
{
    reinterpret_cast<QContactNamePrivate*>(d.data())->setMemberValue<QString>(QContactName::FieldCustomLabel, _value);
}

/* ==================== QContactNickname ======================= */

/*!
   \class QContactNickname
   \brief The QContactNickname class contains a nickname of a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactNicknamePrivate : public QContactDetailBuiltinPrivate<QContactNicknamePrivate>
{
public:
    QString m_nickname;

    enum { FieldCount = 1 };

    QContactNicknamePrivate() : QContactDetailBuiltinPrivate<QContactNicknamePrivate>(QContactNickname::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactNicknamePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNicknamePrivate, m_nickname) },
};

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
void QContactNickname::setNickname(const QString& _value)
{
    reinterpret_cast<QContactNicknamePrivate*>(d.data())->setMemberValue<QString>(QContactNickname::FieldNickname, _value);
}

/*!
   \fn QContactNickname::nickname() const
   Returns the nickname of the contact which is stored in this detail.
 */
QString QContactNickname::nickname() const
{
    return reinterpret_cast<const QContactNicknamePrivate*>(d.constData())->memberValue<QString>(QContactNickname::FieldNickname);
}

/* ==================== QContactNote ======================= */

/*!
   \class QContactNote
   \brief The QContactNote class contains a note associated
   with a contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactNotePrivate : public QContactDetailBuiltinPrivate<QContactNotePrivate>
{
public:
    QString m_note;

    enum { FieldCount = 1 };

    QContactNotePrivate() : QContactDetailBuiltinPrivate<QContactNotePrivate>(QContactNote::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactNotePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactNotePrivate, m_note) },
};

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
void QContactNote::setNote(const QString& _value)
{
    reinterpret_cast<QContactNotePrivate*>(d.data())->setMemberValue<QString>(QContactNote::FieldNote, _value);
}

/*!
   \fn QContactNote::note() const
   Returns a string for a note associated with a contact.
 */
QString QContactNote::note() const
{
    return reinterpret_cast<const QContactNotePrivate*>(d.constData())->memberValue<QString>(QContactNote::FieldNote);
}

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

class QContactTagPrivate : public QContactDetailBuiltinPrivate<QContactTagPrivate>
{
public:
    QString m_tag;

    enum { FieldCount = 1 };

    QContactTagPrivate() : QContactDetailBuiltinPrivate<QContactTagPrivate>(QContactTag::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactTagPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactTagPrivate, m_tag) },
};

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
void QContactTag::setTag(const QString& _value)
{
    reinterpret_cast<QContactTagPrivate*>(d.data())->setMemberValue<QString>(QContactTag::FieldTag, _value);
}

/*!
   \fn QContactTag::tag() const
   Returns the tag associated with a contact which is stored in this detail.
 */
QString QContactTag::tag() const
{
    return reinterpret_cast<const QContactTagPrivate*>(d.constData())->memberValue<QString>(QContactTag::FieldTag);
}

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

class QContactTimestampPrivate : public QContactDetailBuiltinPrivate<QContactTimestampPrivate>
{
public:
    QDateTime m_modificationTimestamp;
    QDateTime m_creationTimestamp;
    QDateTime m_deletionTimestamp;

    enum { FieldCount = 3 };

    QContactTimestampPrivate() : QContactDetailBuiltinPrivate<QContactTimestampPrivate>(QContactTimestamp::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactTimestampPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactTimestampPrivate, m_modificationTimestamp) },
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactTimestampPrivate, m_creationTimestamp) },
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactTimestampPrivate, m_deletionTimestamp) },
};

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
   \fn QContactTimestamp::setCreated(const QDateTime& dateTime)
   Sets the creation timestamp saved in this detail to \a dateTime.
 */
void QContactTimestamp::setCreated(const QDateTime& _value)
{
    reinterpret_cast<QContactTimestampPrivate*>(d.data())->setMemberValue<QDateTime>(QContactTimestamp::FieldCreationTimestamp, _value);
}

/*!
   \fn QContactTimestamp::created() const
   Returns the creation timestamp saved in this detail.
 */
QDateTime QContactTimestamp::created() const
{
    return reinterpret_cast<const QContactTimestampPrivate*>(d.constData())->memberValue<QDateTime>(QContactTimestamp::FieldCreationTimestamp);
}

/*!
   \fn QContactTimestamp::setLastModified(const QDateTime& dateTime)
   Sets the last-modified timestamp saved in this detail to \a dateTime.
 */
void QContactTimestamp::setLastModified(const QDateTime& _value)
{
    reinterpret_cast<QContactTimestampPrivate*>(d.data())->setMemberValue<QDateTime>(QContactTimestamp::FieldModificationTimestamp, _value);
}

/*!
   \fn QContactTimestamp::lastModified() const
   Returns the last-modified timestamp saved in this detail.
 */
QDateTime QContactTimestamp::lastModified() const
{
    return reinterpret_cast<const QContactTimestampPrivate*>(d.constData())->memberValue<QDateTime>(QContactTimestamp::FieldModificationTimestamp);
}

/*!
   \fn QContactTimestamp::setDeleted(const QDateTime& dateTime)
   Sets the deletion timestamp saved in this detail to \a dateTime.
 */
void QContactTimestamp::setDeleted(const QDateTime& _value)
{
    reinterpret_cast<QContactTimestampPrivate*>(d.data())->setMemberValue<QDateTime>(QContactTimestamp::FieldDeletionTimestamp, _value);
}

/*!
   \fn QContactTimestamp::deleted() const
   Returns the deletion timestamp saved in this detail.
 */
QDateTime QContactTimestamp::deleted() const
{
    return reinterpret_cast<const QContactTimestampPrivate*>(d.constData())->memberValue<QDateTime>(QContactTimestamp::FieldDeletionTimestamp);
}


/* ==================== QContactType ======================= */

/*!
   \class QContactType
   \brief The QContactType class describes the type of the contact.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactTypePrivate : public QContactDetailBuiltinPrivate<QContactTypePrivate>
{
public:
    int m_contactType;

    enum { FieldCount = 1 };

    QContactTypePrivate() : QContactDetailBuiltinPrivate<QContactTypePrivate>(QContactType::Type), m_contactType(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactTypePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactTypePrivate, m_contactType) },
};

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

    Contacts of this type are able to be the second contact in
    relationships of the \c QContactRelationship::HasMember type, and the
    first contact in relationships of the \c QContactRelationship::Aggregates type.

   \value TypeGroup The value indicates that this contact is a group contact.

    Contacts of this type are able to be the first contact in
    relationships of the \c QContactRelationship::HasMember type.

    To enumerate the ids of members of a group, the client should
    retrieve the relationships which involve the group from the manager
    in which the group is saved.

   \value TypeFacet The value indicates that this contact is a facet of an individual contact.

    Contact facets are collections of details that form one representation
    of an individual contact. Facets can be aggregated together to create
    composite representations of a contact, represented by contacts of type
    \c TypeContact.

    Contacts of this type are able to be the second contact in
    relationships of the \c QContactRelationship::Aggregates type.

    Contact managers may automatically create composite contacts from facets, or
    require composition to be specified by clients, using QContactRelationship.

   \sa type(), setType()
 */

/*!
   \fn QContactType::type() const
   Returns the contact type value stored in this detail.
 */
QContactType::TypeValues QContactType::type() const
{
    return static_cast<QContactType::TypeValues>(reinterpret_cast<const QContactTypePrivate*>(d.constData())->memberValue<int>(QContactType::FieldType));
}

/*!
   \fn QContactType::setType(TypeValues type)
   Sets the type of the contact to be the give \a type.
 */
void QContactType::setType(QContactType::TypeValues _value)
{
    reinterpret_cast<QContactTypePrivate*>(d.data())->setMemberValue<int>(QContactType::FieldType, static_cast<int>(_value));
}

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

class QContactOnlineAccountPrivate : public QContactDetailBuiltinPrivate<QContactOnlineAccountPrivate>
{
public:
    QString m_accountUri;
    QString m_serviceProvider;
    int m_protocol;
    QStringList m_capabilities;
    QList<int> m_subTypes;

    enum { FieldCount = 5 };

    QContactOnlineAccountPrivate() : QContactDetailBuiltinPrivate<QContactOnlineAccountPrivate>(QContactOnlineAccount::Type), m_protocol(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactOnlineAccountPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOnlineAccountPrivate, m_accountUri) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOnlineAccountPrivate, m_serviceProvider) },
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactOnlineAccountPrivate, m_protocol) },
    { QContactDetailBuiltinPrivateBase::StringList, offsetof(QContactOnlineAccountPrivate, m_capabilities) },
    { QContactDetailBuiltinPrivateBase::IntList, offsetof(QContactOnlineAccountPrivate, m_subTypes) },
};

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
void QContactOnlineAccount::setAccountUri(const QString& _value)
{
    reinterpret_cast<QContactOnlineAccountPrivate*>(d.data())->setMemberValue<QString>(QContactOnlineAccount::FieldAccountUri, _value);
}

/*!
   \fn QContactOnlineAccount::accountUri() const

   Returns the universal resource identifier of the online account of
   the contact.
 */
QString QContactOnlineAccount::accountUri() const
{
    return reinterpret_cast<const QContactOnlineAccountPrivate*>(d.constData())->memberValue<QString>(QContactOnlineAccount::FieldAccountUri);
}

/*!
   \fn QContactOnlineAccount::setServiceProvider(const QString& serviceProvider)

   Sets the service provider of the contact's online account to \a
   serviceProvider.
 */
void QContactOnlineAccount::setServiceProvider(const QString& _value)
{
    reinterpret_cast<QContactOnlineAccountPrivate*>(d.data())->setMemberValue<QString>(QContactOnlineAccount::FieldServiceProvider, _value);
}

/*!
   \fn QContactOnlineAccount::serviceProvider() const
   Returns the service provider of the online account of the contact.
 */
QString QContactOnlineAccount::serviceProvider() const
{
    return reinterpret_cast<const QContactOnlineAccountPrivate*>(d.constData())->memberValue<QString>(QContactOnlineAccount::FieldServiceProvider);
}

/*!
    \fn QContactOnlineAccount::protocol() const
    Returns the protocol value.
 */
QContactOnlineAccount::Protocol QContactOnlineAccount::protocol() const
{
    return static_cast<QContactOnlineAccount::Protocol>(reinterpret_cast<const QContactOnlineAccountPrivate*>(d.constData())->memberValue<int>(QContactOnlineAccount::FieldProtocol));
}

/*!
    \fn QContactOnlineAccount::setProtocol(Protocol protocol)
    Set the protocol to \a protocol.
 */
void QContactOnlineAccount::setProtocol(QContactOnlineAccount::Protocol _value)
{
    reinterpret_cast<QContactOnlineAccountPrivate*>(d.data())->setMemberValue<int>(QContactOnlineAccount::FieldProtocol, static_cast<int>(_value));
}

/*!
   \fn QContactOnlineAccount::setSubTypes(const QList<int>& subTypes)

   Sets the subtypes which this detail implements to be those
   contained in the list of given \a subTypes.
 */
void QContactOnlineAccount::setSubTypes(const QList<int>& _value)
{
    reinterpret_cast<QContactOnlineAccountPrivate*>(d.data())->setMemberValue<QList<int> >(QContactOnlineAccount::FieldSubTypes, _value);
}

/*!
   \fn QContactOnlineAccount::subTypes() const
   Returns the list of subtypes that this detail implements.
 */
QList<int> QContactOnlineAccount::subTypes() const
{
    return reinterpret_cast<const QContactOnlineAccountPrivate*>(d.constData())->memberValue<QList<int> >(QContactOnlineAccount::FieldSubTypes);
}

/*!
   \fn QContactOnlineAccount::setCapabilities(const QStringList& capabilities)

   Sets the capabilities of the online account about which this detail stores
   presence information to \a capabilities.  The \a capabilities list is a
   list of service-provider specified strings which together identify the
   types of communication which may be possible.
 */
void QContactOnlineAccount::setCapabilities(const QStringList& _value)
{
    reinterpret_cast<QContactOnlineAccountPrivate*>(d.data())->setMemberValue<QStringList>(QContactOnlineAccount::FieldCapabilities, _value);
}

/*!
   \fn QContactOnlineAccount::capabilities() const

   Returns the capabilities of the online account about which this detail stores
   presence information.
 */
QStringList QContactOnlineAccount::capabilities() const
{
    return reinterpret_cast<const QContactOnlineAccountPrivate*>(d.constData())->memberValue<QStringList>(QContactOnlineAccount::FieldCapabilities);
}

/* ==================== QContactOrganization ======================= */

/*!
   \class QContactOrganization
   \brief The QContactOrganization class provides details about an
   organization that the contact is either a part of, or stands for.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactOrganizationPrivate : public QContactDetailBuiltinPrivate<QContactOrganizationPrivate>
{
public:
    QString m_name;
    QUrl m_logoUrl;
    QStringList m_department;
    QString m_location;
    QString m_role;
    QString m_title;
    QString m_assistantName;

    enum { FieldCount = 7 };

    QContactOrganizationPrivate() : QContactDetailBuiltinPrivate<QContactOrganizationPrivate>(QContactOrganization::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactOrganizationPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOrganizationPrivate, m_name) },
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactOrganizationPrivate, m_logoUrl) },
    { QContactDetailBuiltinPrivateBase::StringList, offsetof(QContactOrganizationPrivate, m_department) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOrganizationPrivate, m_location) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOrganizationPrivate, m_role) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOrganizationPrivate, m_title) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactOrganizationPrivate, m_assistantName) },
};

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
void QContactOrganization::setName(const QString& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QString>(QContactOrganization::FieldName, _value);
}

/*!
   \fn QContactOrganization::name() const
   Returns the name of the organization stored in this detail.
 */
QString QContactOrganization::name() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QString>(QContactOrganization::FieldName);
}

/*!
   \fn QContactOrganization::setLogoUrl(const QUrl& logo)
   Sets the url of the logo of the organization stored in this detail to \a logo.
 */
void QContactOrganization::setLogoUrl(const QUrl& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QUrl>(QContactOrganization::FieldLogoUrl, _value);
}

/*!
   \fn QContactOrganization::logoUrl() const
   Returns the url of the logo of the organization stored in this detail.
 */
QUrl QContactOrganization::logoUrl() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QUrl>(QContactOrganization::FieldLogoUrl);
}


/*!
   \fn QContactOrganization::setDepartment(const QStringList& department)

   Sets the contact's department of the organization stored in this
   detail to \a department.  The department is a list of progressively
   finer-grained information.
 */
void QContactOrganization::setDepartment(const QStringList& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QStringList>(QContactOrganization::FieldDepartment, _value);
}

/*!
   \fn QContactOrganization::department() const
   Returns the contact's department stored in this detail.
 */
QStringList QContactOrganization::department() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QStringList>(QContactOrganization::FieldDepartment);
}

/*!
   \fn QContactOrganization::setLocation(const QString& location)

   Sets the location (e.g. city or suburb) of the organization stored
   in this detail to \a location.
 */
void QContactOrganization::setLocation(const QString& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QString>(QContactOrganization::FieldLocation, _value);
}

/*!
   \fn QContactOrganization::location() const
   Returns the location of the organization stored in this detail.
 */
QString QContactOrganization::location() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QString>(QContactOrganization::FieldLocation);
}


/*!
   \fn QContactOrganization::setRole(const QString& role)
   Sets the contact's role within the organization stored in this detail to \a role.
 */
void QContactOrganization::setRole(const QString& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QString>(QContactOrganization::FieldRole, _value);
}

/*!
   \fn QContactOrganization::role() const
   Returns the contact's role within the organization stored in this detail.
 */
QString QContactOrganization::role() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QString>(QContactOrganization::FieldRole);
}

/*!
   \fn QContactOrganization::setTitle(const QString& title)
   Sets the contact's title within the organization stored in this detail to \a title.
 */
void QContactOrganization::setTitle(const QString& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QString>(QContactOrganization::FieldTitle, _value);
}

/*!
   \fn QContactOrganization::title() const
   Returns the contact's title within the organization stored in this detail.
 */
QString QContactOrganization::title() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QString>(QContactOrganization::FieldTitle);
}

/*!
   \fn QContactOrganization::setAssistantName(const QString& assistantName)

   Sets the name of the default assistant of contacts belonging to
   this organization to \a assistantName.
 */
void QContactOrganization::setAssistantName(const QString& _value)
{
    reinterpret_cast<QContactOrganizationPrivate*>(d.data())->setMemberValue<QString>(QContactOrganization::FieldAssistantName, _value);
}

/*!
   \fn QContactOrganization::assistantName() const

   Returns the name of the default assistant of contacts belonging to
   this organization.
 */
QString QContactOrganization::assistantName() const
{
    return reinterpret_cast<const QContactOrganizationPrivate*>(d.constData())->memberValue<QString>(QContactOrganization::FieldAssistantName);
}


/* ==================== QContactRingtone ======================= */

/*!
   \class QContactRingtone
   \brief The QContactRingtone class provides a ringtone associated
   with a contact
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactRingtonePrivate : public QContactDetailBuiltinPrivate<QContactRingtonePrivate>
{
public:
    QUrl m_audioRingtoneUrl;
    QUrl m_videoRingtoneUrl;
    QUrl m_vibrationRingtoneUrl;

    enum { FieldCount = 3 };

    QContactRingtonePrivate() : QContactDetailBuiltinPrivate<QContactRingtonePrivate>(QContactRingtone::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactRingtonePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactRingtonePrivate, m_audioRingtoneUrl) },
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactRingtonePrivate, m_videoRingtoneUrl) },
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactRingtonePrivate, m_vibrationRingtoneUrl) },
};

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
QUrl QContactRingtone::audioRingtoneUrl() const
{
    return reinterpret_cast<const QContactRingtonePrivate*>(d.constData())->memberValue<QUrl>(QContactRingtone::FieldAudioRingtoneUrl);
}

/*!
  \fn QContactRingtone::setAudioRingtoneUrl(const QUrl& audioRingtoneUrl)

  Sets the uri of the audio ringtone stored in the ringtone detail
  to \a audioRingtoneUrl.
 */
void QContactRingtone::setAudioRingtoneUrl(const QUrl& _value)
{
    reinterpret_cast<QContactRingtonePrivate*>(d.data())->setMemberValue<QUrl>(QContactRingtone::FieldAudioRingtoneUrl, _value);
}

/*!
  \fn QContactRingtone::videoRingtoneUrl() const

  Returns the uri of the video ringtone stored in the ringtone detail.
 */
QUrl QContactRingtone::videoRingtoneUrl() const
{
    return reinterpret_cast<const QContactRingtonePrivate*>(d.constData())->memberValue<QUrl>(QContactRingtone::FieldVideoRingtoneUrl);
}

/*!
  \fn QContactRingtone::setVideoRingtoneUrl(const QUrl& videoRingtoneUrl)

  Sets the uri of the video ringtone stored in the ringtone detail
  to \a videoRingtoneUrl.
 */
void QContactRingtone::setVideoRingtoneUrl(const QUrl& _value)
{
    reinterpret_cast<QContactRingtonePrivate*>(d.data())->setMemberValue<QUrl>(QContactRingtone::FieldVideoRingtoneUrl, _value);
}

/*!
  \fn QContactRingtone::vibrationRingtoneUrl() const

  Returns the uri of the vibration ringtone stored in the ringtone detail.
 */
QUrl QContactRingtone::vibrationRingtoneUrl() const
{
    return reinterpret_cast<const QContactRingtonePrivate*>(d.constData())->memberValue<QUrl>(QContactRingtone::FieldVibrationRingtoneUrl);
}

/*!
  \fn QContactRingtone::setVibrationRingtoneUrl(const QUrl& vibrationRingtoneUrl)

  Sets the uri of the vibration ringtone stored in the ringtone detail
  to \a vibrationRingtoneUrl.
 */
void QContactRingtone::setVibrationRingtoneUrl(const QUrl& _value)
{
    reinterpret_cast<QContactRingtonePrivate*>(d.data())->setMemberValue<QUrl>(QContactRingtone::FieldVibrationRingtoneUrl, _value);
}

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

class QContactPresencePrivate : public QContactDetailBuiltinPrivate<QContactPresencePrivate>
{
public:
    QDateTime m_timestamp;
    QString m_nickname;
    int m_presenceState;
    QString m_presenceStateText;
    QUrl m_presenceStateImageUrl;
    QString m_customMessage;

    enum { FieldCount = 6 };

    QContactPresencePrivate() : QContactDetailBuiltinPrivate<QContactPresencePrivate>(QContactPresence::Type), m_presenceState(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactPresencePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactPresencePrivate, m_timestamp) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactPresencePrivate, m_nickname) },
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactPresencePrivate, m_presenceState) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactPresencePrivate, m_presenceStateText) },
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactPresencePrivate, m_presenceStateImageUrl) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactPresencePrivate, m_customMessage) },
};

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
void QContactPresence::setTimestamp(const QDateTime& _value)
{
    reinterpret_cast<QContactPresencePrivate*>(d.data())->setMemberValue<QDateTime>(QContactPresence::FieldTimestamp, _value);
}

/*!
   \fn QContactPresence::timestamp() const

    Returns the timestamp at which the data in the presence detail was valid.
 */
QDateTime QContactPresence::timestamp() const
{
    return reinterpret_cast<const QContactPresencePrivate*>(d.constData())->memberValue<QDateTime>(QContactPresence::FieldTimestamp);
}

/*!
   \fn QContactPresence::setNickname(const QString& nickname)

   Sets the last-known nickname used by the contact during
   communications via the online account about which this detail
   stores presence information to \a nickname.
 */
void QContactPresence::setNickname(const QString& _value)
{
    reinterpret_cast<QContactPresencePrivate*>(d.data())->setMemberValue<QString>(QContactPresence::FieldNickname, _value);
}

/*!
   \fn QContactPresence::nickname() const

   Returns the last-known nickname used by the contact during
   communications via the online account.
 */
QString QContactPresence::nickname() const
{
    return reinterpret_cast<const QContactPresencePrivate*>(d.constData())->memberValue<QString>(QContactPresence::FieldNickname);
}

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
void QContactPresence::setPresenceState(QContactPresence::PresenceState _value)
{
    reinterpret_cast<QContactPresencePrivate*>(d.data())->setMemberValue<int>(QContactPresence::FieldPresenceState, static_cast<int>(_value));
}

/*!
   \fn QContactPresence::presenceState() const

   Returns the presence state of the online account according to the
   presence provider.
 */
QContactPresence::PresenceState QContactPresence::presenceState() const
{
    return static_cast<QContactPresence::PresenceState>(reinterpret_cast<const QContactPresencePrivate*>(d.constData())->memberValue<int>(QContactPresence::FieldPresenceState));
}

/*!
   \fn QContactPresence::setPresenceStateText(const QString& presenceStateText)

   Sets the text corresponding to the presence state to \a presenceStateText.
   This function is generally called by presence providers to allow custom
   naming of states, or to allow finer grained state reporting than is
   provided by the presence state API.
 */
void QContactPresence::setPresenceStateText(const QString& _value)
{
    reinterpret_cast<QContactPresencePrivate*>(d.data())->setMemberValue<QString>(QContactPresence::FieldPresenceStateText, _value);
}

/*!
   \fn QContactPresence::presenceStateText() const

   Returns the text corresponding to the current presence state.
 */
QString QContactPresence::presenceStateText() const
{
    return reinterpret_cast<const QContactPresencePrivate*>(d.constData())->memberValue<QString>(QContactPresence::FieldPresenceStateText);
}

/*!
  \fn QContactPresence::setCustomMessage(const QString& customMessage)

   Sets the custom status message from the contact for the online account
   about which this detail stores presence information, to \a customMessage.
   This custom message would have been set by the contact,
   and does not necessarily correspond to a particular presence state.
 */
void QContactPresence::setCustomMessage(const QString& _value)
{
    reinterpret_cast<QContactPresencePrivate*>(d.data())->setMemberValue<QString>(QContactPresence::FieldCustomMessage, _value);
}

/*!
   \fn QContactPresence::customMessage() const

   Returns the custom status message from the contact for the online account
   about which this detail stores presence information.
 */
QString QContactPresence::customMessage() const
{
    return reinterpret_cast<const QContactPresencePrivate*>(d.constData())->memberValue<QString>(QContactPresence::FieldCustomMessage);
}

/*!
   \fn QContactPresence::setPresenceStateImageUrl(const QUrl& presenceStateImageUrl)

   Sets the last-known status image url of the contact for the online account
   about which this detail stores presence information, to \a presenceStateImageUrl.
 */
void QContactPresence::setPresenceStateImageUrl(const QUrl& _value)
{
    reinterpret_cast<QContactPresencePrivate*>(d.data())->setMemberValue<QUrl>(QContactPresence::FieldPresenceStateImageUrl, _value);
}

/*!
   \fn QContactPresence::presenceStateImageUrl() const

   Returns the last-known status image url of the contact for the online account
   about which this detail stores presence information.
 */
QUrl QContactPresence::presenceStateImageUrl() const
{
    return reinterpret_cast<const QContactPresencePrivate*>(d.constData())->memberValue<QUrl>(QContactPresence::FieldPresenceStateImageUrl);
}

/* ==================== QContactGlobalPresence ======================= */

/*!
   \class QContactGlobalPresence
   \brief The QContactGlobalPresence class provides aggregated presence information
   for a contact, synthesized or supplied by the backend.
   \ingroup contacts-details
    \inmodule QtContacts
 */

class QContactGlobalPresencePrivate : public QContactDetailBuiltinPrivate<QContactGlobalPresencePrivate>
{
public:
    QDateTime m_timestamp;
    QString m_nickname;
    int m_presenceState;
    QString m_presenceStateText;
    QUrl m_presenceStateImageUrl;
    QString m_customMessage;

    enum { FieldCount = 6 };

    QContactGlobalPresencePrivate() : QContactDetailBuiltinPrivate<QContactGlobalPresencePrivate>(QContactGlobalPresence::Type), m_presenceState(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactGlobalPresencePrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::DateTime, offsetof(QContactGlobalPresencePrivate, m_timestamp) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactGlobalPresencePrivate, m_nickname) },
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactGlobalPresencePrivate, m_presenceState) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactGlobalPresencePrivate, m_presenceStateText) },
    { QContactDetailBuiltinPrivateBase::Url, offsetof(QContactGlobalPresencePrivate, m_presenceStateImageUrl) },
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactGlobalPresencePrivate, m_customMessage) },
};

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
void QContactGlobalPresence::setTimestamp(const QDateTime& _value)
{
    reinterpret_cast<QContactGlobalPresencePrivate*>(d.data())->setMemberValue<QDateTime>(QContactGlobalPresence::FieldTimestamp, _value);
}

/*!
   \fn QContactGlobalPresence::timestamp() const

    Returns the timestamp at which the data in the global presence detail was valid.
 */
QDateTime QContactGlobalPresence::timestamp() const
{
    return reinterpret_cast<const QContactGlobalPresencePrivate*>(d.constData())->memberValue<QDateTime>(QContactGlobalPresence::FieldTimestamp);
}

/*!
   \fn QContactGlobalPresence::setNickname(const QString& nickname)

   Sets the last-known nickname used by the contact during
   communications via any online account about which this detail
   aggregates presence information to \a nickname.
 */
void QContactGlobalPresence::setNickname(const QString& _value)
{
    reinterpret_cast<QContactGlobalPresencePrivate*>(d.data())->setMemberValue<QString>(QContactGlobalPresence::FieldNickname, _value);
}

/*!
   \fn QContactGlobalPresence::nickname() const

   Returns the last-known nickname used by the contact during
   communications via any online account about which this detail
   aggregates presence information.
 */
QString QContactGlobalPresence::nickname() const
{
    return reinterpret_cast<const QContactGlobalPresencePrivate*>(d.constData())->memberValue<QString>(QContactGlobalPresence::FieldNickname);
}

/*!
   \fn QContactGlobalPresence::setPresenceState(QContactPresence::PresenceState presenceState)

   Sets the presence state of this aggregate detail according to the presence
   information available from the presence providers which this detail aggregates
   to the given \a presenceState.
 */
void QContactGlobalPresence::setPresenceState(QContactPresence::PresenceState _value)
{
    reinterpret_cast<QContactGlobalPresencePrivate*>(d.data())->setMemberValue<int>(QContactGlobalPresence::FieldPresenceState, static_cast<int>(_value));
}

/*!
   \fn QContactGlobalPresence::presenceState() const

   Returns the aggregate presence state of any online accounts about which this detail
   aggregates presence information.
 */
QContactPresence::PresenceState QContactGlobalPresence::presenceState() const
{
    return static_cast<QContactPresence::PresenceState>(reinterpret_cast<const QContactGlobalPresencePrivate*>(d.constData())->memberValue<int>(QContactGlobalPresence::FieldPresenceState));
}

/*!
   \fn QContactGlobalPresence::setPresenceStateText(const QString& presenceStateText)

   Sets the text corresponding to the presence state to \a presenceStateText.
   This function is generally called by presence providers to allow custom
   naming of states, or to allow finer grained state reporting than is
   provided by the presence state API.
 */
void QContactGlobalPresence::setPresenceStateText(const QString& _value)
{
    reinterpret_cast<QContactGlobalPresencePrivate*>(d.data())->setMemberValue<QString>(QContactGlobalPresence::FieldPresenceStateText, _value);
}

/*!
   \fn QContactGlobalPresence::presenceStateText() const

   Returns the text corresponding to the current presence state.
 */
QString QContactGlobalPresence::presenceStateText() const
{
    return reinterpret_cast<const QContactGlobalPresencePrivate*>(d.constData())->memberValue<QString>(QContactGlobalPresence::FieldPresenceStateText);
}

/*!
  \fn QContactGlobalPresence::setCustomMessage(const QString& customMessage)

   Sets the custom status message from the contact for the aggregate presence
   detail, to \a customMessage.
 */
void QContactGlobalPresence::setCustomMessage(const QString& _value)
{
    reinterpret_cast<QContactGlobalPresencePrivate*>(d.data())->setMemberValue<QString>(QContactGlobalPresence::FieldCustomMessage, _value);
}


/*!
   \fn QContactGlobalPresence::customMessage() const

   Returns the custom status message from the contact for the aggregate presence
   detail.
 */
QString QContactGlobalPresence::customMessage() const
{
    return reinterpret_cast<const QContactGlobalPresencePrivate*>(d.constData())->memberValue<QString>(QContactGlobalPresence::FieldCustomMessage);
}

/*!
   \fn QContactGlobalPresence::setPresenceStateImageUrl(const QUrl& presenceStateImageUrl)

   Sets the last-known status image url of the contact to \a presenceStateImageUrl.
 */
void QContactGlobalPresence::setPresenceStateImageUrl(const QUrl& _value)
{
    reinterpret_cast<QContactGlobalPresencePrivate*>(d.data())->setMemberValue<QUrl>(QContactGlobalPresence::FieldPresenceStateImageUrl, _value);
}

/*!
   \fn QContactGlobalPresence::presenceStateImageUrl() const

   Returns the last-known status image url of the contact.
 */
QUrl QContactGlobalPresence::presenceStateImageUrl() const
{
    return reinterpret_cast<const QContactGlobalPresencePrivate*>(d.constData())->memberValue<QUrl>(QContactGlobalPresence::FieldPresenceStateImageUrl);
}

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

class QContactExtendedDetailPrivate : public QContactDetailBuiltinPrivate<QContactExtendedDetailPrivate>
{
public:
    QString m_name;
    QVariant m_data;

    enum { FieldCount = 2 };

    QContactExtendedDetailPrivate() : QContactDetailBuiltinPrivate<QContactExtendedDetailPrivate>(QContactExtendedDetail::Type) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactExtendedDetailPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::String, offsetof(QContactExtendedDetailPrivate, m_name) },
    { QContactDetailBuiltinPrivateBase::Variant, offsetof(QContactExtendedDetailPrivate, m_data) },
};

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
void QContactExtendedDetail::setName(const QString& _value)
{
    reinterpret_cast<QContactExtendedDetailPrivate*>(d.data())->setMemberValue<QString>(QContactExtendedDetail::FieldName, _value);
}

/*!
    \fn QString QContactExtendedDetail::name() const

    Gets the name of this extended detail.
 */
QString QContactExtendedDetail::name() const
{
    return reinterpret_cast<const QContactExtendedDetailPrivate*>(d.constData())->memberValue<QString>(QContactExtendedDetail::FieldName);
}

/*!
    \fn void QContactExtendedDetail::setData(const QVariant &data)

    Sets the \a data of the extended detail.
 */
void QContactExtendedDetail::setData(const QVariant& _value)
{
    reinterpret_cast<QContactExtendedDetailPrivate*>(d.data())->setMemberValue(QContactExtendedDetail::FieldData, _value);
}

/*!
    \fn QVariant QContactExtendedDetail::data() const

    Gets the data of this extended detail.
 */
QVariant QContactExtendedDetail::data() const
{
    return reinterpret_cast<const QContactExtendedDetailPrivate*>(d.constData())->memberValue<QVariant>(QContactExtendedDetail::FieldData);
}


/* ==================== QContactVersion ======================= */
/*!
    \class QContactVersion
    \brief The QContactVersion class provides the versioning information of a QContact object.
    \inmodule QtContacts
    \ingroup contacts-details
 */

class QContactVersionPrivate : public QContactDetailBuiltinPrivate<QContactVersionPrivate>
{
public:
    int m_sequenceNumber;
    QByteArray m_extendedVersion;

    enum { FieldCount = 2 };

    QContactVersionPrivate() : QContactDetailBuiltinPrivate<QContactVersionPrivate>(QContactVersion::Type), m_sequenceNumber(0) {}
};

template<>
const QContactDetailBuiltinPrivateBase::Member QContactDetailBuiltinPrivate<QContactVersionPrivate>::s_members[] = {
    { QContactDetailBuiltinPrivateBase::Int, offsetof(QContactVersionPrivate, m_sequenceNumber) },
    { QContactDetailBuiltinPrivateBase::ByteArray, offsetof(QContactVersionPrivate, m_extendedVersion) },
};

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
void QContactVersion::setSequenceNumber(int _value)
{
    reinterpret_cast<QContactVersionPrivate*>(d.data())->setMemberValue<int>(QContactVersion::FieldSequenceNumber, _value);
}

/*!
    \fn int QContactVersion::sequenceNumber() const

    Gets the integer sequenceNumber.
 */
int QContactVersion::sequenceNumber() const
{
    return reinterpret_cast<const QContactVersionPrivate*>(d.constData())->memberValue<int>(QContactVersion::FieldSequenceNumber);
}

/*!
    \fn void QContactVersion::setExtendedVersion(const QByteArray &extendedVersion)

    Sets the \a extendedVersion.
 */
void QContactVersion::setExtendedVersion(const QByteArray& _value)
{
    reinterpret_cast<QContactVersionPrivate*>(d.data())->setMemberValue<QByteArray>(QContactVersion::FieldExtendedVersion, _value);
}

/*!
    \fn QByteArray QContactVersion::extendedVersion() const

    Gets the extendedVersion.
 */
QByteArray QContactVersion::extendedVersion() const
{
    return reinterpret_cast<const QContactVersionPrivate*>(d.constData())->memberValue<QByteArray>(QContactVersion::FieldExtendedVersion);
}


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

/*
    Adding a new builtin-detail-type requires extending this function!
*/
QContactDetailPrivate *QContactDetailPrivate::construct(QContactDetail::DetailType detailType)
{
    switch (detailType) {
        case QContactDetail::TypeAddress:       return new QContactAddressPrivate;
        case QContactDetail::TypeAnniversary:   return new QContactAnniversaryPrivate;
        case QContactDetail::TypeAvatar:        return new QContactAvatarPrivate;
        case QContactDetail::TypeBirthday:      return new QContactBirthdayPrivate;
        case QContactDetail::TypeDisplayLabel:  return new QContactDisplayLabelPrivate;
        case QContactDetail::TypeEmailAddress:  return new QContactEmailAddressPrivate;
        case QContactDetail::TypeExtendedDetail:return new QContactExtendedDetailPrivate;
        case QContactDetail::TypeFamily:        return new QContactFamilyPrivate;
        case QContactDetail::TypeFavorite:      return new QContactFavoritePrivate;
        case QContactDetail::TypeGender:        return new QContactGenderPrivate;
        case QContactDetail::TypeGeoLocation:   return new QContactGeoLocationPrivate;
        case QContactDetail::TypeGlobalPresence:return new QContactGlobalPresencePrivate;
        case QContactDetail::TypeGuid:          return new QContactGuidPrivate;
        case QContactDetail::TypeHobby:         return new QContactHobbyPrivate;
        case QContactDetail::TypeName:          return new QContactNamePrivate;
        case QContactDetail::TypeNickname:      return new QContactNicknamePrivate;
        case QContactDetail::TypeNote:          return new QContactNotePrivate;
        case QContactDetail::TypeOnlineAccount: return new QContactOnlineAccountPrivate;
        case QContactDetail::TypeOrganization:  return new QContactOrganizationPrivate;
        case QContactDetail::TypePhoneNumber:   return new QContactPhoneNumberPrivate;
        case QContactDetail::TypePresence:      return new QContactPresencePrivate;
        case QContactDetail::TypeRingtone:      return new QContactRingtonePrivate;
        case QContactDetail::TypeSyncTarget:    return new QContactSyncTargetPrivate;
        case QContactDetail::TypeTag:           return new QContactTagPrivate;
        case QContactDetail::TypeTimestamp:     return new QContactTimestampPrivate;
        case QContactDetail::TypeType:          return new QContactTypePrivate;
        case QContactDetail::TypeUrl:           return new QContactUrlPrivate;
        case QContactDetail::TypeVersion:       return new QContactVersionPrivate;
        default:                                return new QContactDetailPrivate(detailType);
    }
}


/*!
    \fn QContactDetail::QContactDetail(QContactDetail::DetailType type)
    Constructs a new, empty detail of the type identified by \a type.
 */
QContactDetail::QContactDetail(QContactDetail::DetailType type)
    : d(QContactDetailPrivate::construct(type))
{
}

/*!
    \internal
    \fn QContactDetail::QContactDetail(const QContactDetail& other, DetailType expectedType)

    Constructs a detail that is a copy of \a other if \a other is of the expected type
    identified by \a expectedType, else constructs a new, empty detail of the
    type identified by the \a expectedType
*/
QContactDetail::QContactDetail(const QContactDetail& other, DetailType expectedType)
{
    if (other.d.constData()->m_type == expectedType) {
        d = other.d;
    } else {
        d = QContactDetailPrivate::construct(expectedType);
    }
}

/*!
    \internal
    \fn QContactDetail& QContactDetail::assign(const QContactDetail& other, DetailType expectedType)

    Assigns this detail to \a other if the type of \a other is that identified
    by the given \a expectedType, else assigns this detail to be a new, empty
    detail of the type identified by the given \a expectedType
*/
QContactDetail& QContactDetail::assign(const QContactDetail& other, DetailType expectedType)
{
    if (this != &other) {
        if (other.d.constData()->m_type == expectedType) {
            d = other.d;
        } else {
            d = QContactDetailPrivate::construct(expectedType);
        }
    }
    return *this;
}

QT_WARNING_POP /* -Winvalid-offsetof */

QT_END_NAMESPACE_CONTACTS
