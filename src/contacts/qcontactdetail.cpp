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

#include "qcontactdetail.h"
#include "qcontactdetail_p.h"
#include "qcontactdetails.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qcontactmanager.h"

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

/* Initialise our static private data member */
QAtomicInt QContactDetailPrivate::lastDetailKey(1);

/*!
  \class QContactDetail

  \brief The QContactDetail class represents a single, complete detail about a contact.
  \inmodule QtContacts
  \ingroup contacts-main

  All of the information for a contact is stored in one or more QContactDetail objects.

  A detail is a group of logically related bits of data - for example, a street address is a single
  detail that has multiple fields (number, region, country etc).
  Different contact managers might have different fields
  for the same detail type, depending on their capabilities.  For example, for the QContactName detail
  one manager might not support the middle name field, while a different manager may add an extra field for
  specific extra information not present in the default schema.

  One field which is common to all details is the context field.  This field is intended to store one or
  more contexts that this detail is associated with.  Commonly this will be something like
  "Home" and/or "Work", although no limitations are placed on which values may be stored in this field
  in the default schema.

  There are two other, related fields which are common to all details.  The first is
  \a QContactDetail::FieldDetailUri, which stores the unique URI of the detail if one exists.
  The field is not mandatory, and backends are not required to verify that the given URI is indeed
  unique within the contact.  The second field is \a QContactDetail::LinkedDetailUris, which stores
  a list of detail URIs to which this detail is linked.  The link is one-way, and can be used to show
  how or where a detail was derived.  This is useful for things like presence information and avatars,
  which are linked to a particular online account detail of the contact.

  When a QContactDetail has been retrieved in a QContact from a QContactManager, it may have certain
  access constraints provided with it, like \l ReadOnly or \l Irremovable.  This might mean that the
  supplied detail is calculated or otherwise not modifiable by the user - presence information is a
  good example.  Also, some details may be marked \l Irremovable.  These are typically things that
  a contact has to have - like a QContactType.

  It is possible to inherit from QContactDetail to provide convenience or
  standardized access to values.  For example, \l QContactPhoneNumber provides
  a convenient API for manipulating a QContactDetail as a phone number, according
  to the schema.

  In general, QContactDetail and the built in subclasses (like \l QContactPhoneNumber) provide
  constants for the names of fields (like \l QContactPhoneNumber::FieldNumber), and for predefined
  common values like \l QContactDetail::ContextHome.  Typically the constants for field names start
  with \c Field, and the constants for predefined values of a field start with the name of that field
  (e.g. \c ContextHome is a predefined constant for \c FieldContext).

  If you wish to create your own, customized contact detail, you should use
  the \l QContactExtendedDetail detail.
  QContactDetail objects act like type checked values.  In general, you can assign them
  to and from and have reasonable behaviour, like the following example.

  \code

  QContactPhoneNumber number;
  number.setNumber("555-1212");
  // number.value(QContactPhoneNumber::FieldNumber) == "555-1212";
  // number.type() == QContactPhoneNumber::Type

  QContactDetail detail = number;
  // detail.value(QContactPhoneNumber::FieldNumber) == "555-1212";
  // detail.type() == QContactPhoneNumber::Type

  QContactPhoneNumber otherNumber = detail;
  // otherNumber.number() == "555-1212";
  // otherNumber.type() == QContactPhoneNumber::Type

  QContactAddress address = detail;
  // address is now a default constructed QContactAddress
  // address.value(QContactPhoneNumber::FieldNumber) is empty
  // address.type() == QContactAddress::Type

  QContactAddress otherAddress = number;
  // otherAddress is now a default constructed QContactAddress
  // otherAddress.value(QContactPhoneNumber::FieldNumber) is empty
  // otherAddress.type() == QContactAddress::Type
  \endcode

  \sa QContact, QContactDetail::DetailType, QContactDetailFilter, QContactDetailRangeFilter, Q_DECLARE_CUSTOM_CONTACT_DETAIL
 */

/*!
  \macro Q_DECLARE_CUSTOM_CONTACT_DETAIL
  \relates QContactDetail

  Macro for simplifying declaring custom (leaf) detail classes.

  The first argument is the detail type of the class.

  If you are creating a convenience class for a type of QContactDetail,
  you should use this macro when declaring your class to ensure that
  it interoperates with other contact functionality.

  Here is an example of a custom extension class using this macro.
  Note that the class provides some predefined constants
  and some convenience methods that return values associated with schema
  fields.

  \code
  #include <QContactDetail>
  class ContactVehicle : public QContactDetail
  {
  public:
      Q_DECLARE_CUSTOM_CONTACT_DETAIL(ContactVehicle)

      enum VehicleField {
        FieldRegistration = 0,
        FieldMake,
        FieldModel,
        FieldColor
      };

      QString registration() const { return value(FieldRegistration).toString(); }
      void setRegistration(const QString& _registration) { setValue(FieldRegistration, _registration);

      QString make() const { return value(FieldMake).toString(); }
      void setMake(const QString& _make) { setValue(FieldMake, _make); }

      QString model() const { return value(FieldModel).toString(); }
      void setModel(const QString& _model) { setValue(FieldModel, _model); }

      QString color() const { return value(FieldColor).toString(); }
      void setColor(const QString &_color) { setValue(FieldColor, _color); }
  };
  \endcode
*/

/*!
   \enum QContactDetail::DetailType

   This enumeration defines the detail types provided by the contacts API.
   \value TypeUndefined Convenience value used to represent unknown / undefined detail types.
   \value TypeAddress Detail type of a QContactAddress detail.
   \value TypeAnniversary Detail type of a QContactAnniversary detail.
   \value TypeAvatar Detail type of a QContactAvatar detail.
   \value TypeBirthday Detail type of a QContactBirthday detail.
   \value TypeDisplayLabel Detail type of a QContactDisplayLabel detail.
   \value TypeEmailAddress Detail type of a QContactEmailAddress detail.
   \value TypeExtendedDetail Detail type of a QContactExtendedDetail detail.
   \value TypeFamily Detail type of a QContactFamily detail.
   \value TypeFavorite Detail type of a QContactFavorite detail.
   \value TypeGender Detail type of a QContactGender detail.
   \value TypeGeoLocation Detail type of a QContactGeoLocation detail.
   \value TypeGlobalPresence Detail type of a QContactGlobalPresence detail.
   \value TypeGuid Detail type of a QContactGuid detail.
   \value TypeHobby Detail type of a QContactHobby detail.
   \value TypeName Detail type of a QContactName detail.
   \value TypeNickname Detail type of a QContactNickname detail.
   \value TypeNote Detail type of a QContactNote detail.
   \value TypeOnlineAccount Detail type of a QContactOnlineAccount detail.
   \value TypeOrganization Detail type of a QContactOrganization detail.
   \value TypePhoneNumber Detail type of a QContactPhoneNumber detail.
   \value TypePresence Detail type of a QContactPresence detail.
   \value TypeRingtone Detail type of a QContactRingtone detail.
   \value TypeSyncTarget Detail type of a QContactSyncTarget detail.
   \value TypeTag Detail type of a QContactTag detail.
   \value TypeTimestamp Detail type of a QContactTimestamp detail.
   \value TypeType Detail type of a QContactType detail.
   \value TypeUrl Detail type of a QContactUrl detail.
   \value TypeVersion  Detail type of a QContactVersion detail.

   \sa type()
 */

/*!
  \fn QContactDetail::operator!=(const QContactDetail& other) const
  Returns true if the values or id of this detail is different to those of the \a other detail
 */

/*!
  Constructs a new, empty detail
 */
QContactDetail::QContactDetail()
    : d(new QContactDetailPrivate)
{
}

/*! Constructs a detail that is a copy of \a other
 */
QContactDetail::QContactDetail(const QContactDetail& other)
    : d(other.d)
{
}

/*!
 * Assigns this detail to \a other
 */
QContactDetail& QContactDetail::operator=(const QContactDetail& other)
{
    if (this != &other)
        d = other.d;
    return *this;
}

/*! Frees the memory used by this detail */
QContactDetail::~QContactDetail()
{
}

/*!
    Returns the (unique) enum of the type which defines the semantics and structure of this detail.
 */
QContactDetail::DetailType QContactDetail::type() const
{
    return static_cast<QContactDetail::DetailType>(d.constData()->m_type);
}

/*!
    Compares this detail to \a other.  Returns true if the type, access constraints and values of \a other are equal to those of this detail.
    The keys of each detail are not considered during the comparison, in order to allow details from different contacts to
    be compared according to their values.
 */
bool QContactDetail::operator==(const QContactDetail& other) const
{
    return (*d == *other.d);
}

/*! Returns the hash value for \a key.
*/
uint qHash(const QContactDetail &key)
{
    uint hash = qHash(QString().setNum(key.type()))
                + QT_PREPEND_NAMESPACE(qHash)(key.accessConstraints());
    QMap<int, QVariant> values = key.values();
    QMap<int, QVariant>::const_iterator it = values.constBegin(), end = values.constEnd();
    for ( ; it != end; ++it) {
        hash += QT_PREPEND_NAMESPACE(qHash)(it.key())
              + QT_PREPEND_NAMESPACE(qHash)(it.value().toString());
    }
    return hash;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const QContactDetail& detail)
{
    dbg.nospace() << "QContactDetail(detailType=" << detail.type() << ", key=" << detail.key();
    QMap<int, QVariant> fields = detail.values();
    QMap<int, QVariant>::const_iterator it;
    for (it = fields.constBegin(); it != fields.constEnd(); ++it) {
        dbg.nospace() << ", " << it.key() << '=' << it.value();
    }
    dbg.nospace() << ')';
    return dbg.maybeSpace();
}
#endif

#ifndef QT_NO_DATASTREAM
/*!
 * Writes \a detail to the stream \a out.
 */
QDataStream& operator<<(QDataStream& out, const QContactDetail& detail)
{
    quint8 formatVersion = 1; // Version of QDataStream format for QContactDetail
    return out << formatVersion
               << static_cast<quint32>(detail.type())
               << static_cast<quint32>(detail.accessConstraints())
               << detail.values();
}

/*!
 * Reads a contact detail from stream \a in into \a detail.
 */
QDataStream& operator>>(QDataStream& in, QContactDetail& detail)
{
    detail = QContactDetail();
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1) {
        quint32 typeInt;
        quint32 accessConstraintsInt;
        QMap<int, QVariant> values;
        in >> typeInt >> accessConstraintsInt >> values;

        detail = QContactDetail(QContactDetail::DetailType(typeInt));
        QContactDetail::AccessConstraints accessConstraints(accessConstraintsInt);
        detail.d->m_access = accessConstraints;

        QMapIterator<int, QVariant> it(values);
        while (it.hasNext()) {
            it.next();
            detail.setValue(it.key(), it.value());
        }
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}
#endif

/*!
    Returns true if no values are contained in this detail.  Note that context
    is stored as a value; hence, if a context is set, this function will return false.

    An empty value (for example, the string "") is still a value contained in this
    detail, so this function will return false.
 */
bool QContactDetail::isEmpty() const
{
    return d->isEmpty();
}

/*!
 * Returns the key of this detail.
 *
 * Be aware that if a contact is retrieved (or reloaded) from the backend, the
 * keys of any details it contains may have been changed by the backend, or other
 * threads may have modified the contact details in the backend.  Therefore,
 * clients should reload the detail that they wish to save in or remove from a contact
 * after retrieving the contact from the backend, in order to ascertain the keys of
 * any such details.
 */
int QContactDetail::key() const
{
    return d.constData()->m_detailId;
}

/*!
 *  Generates a new key (detail id) for the detail.
 *  This ensures that calling QContact::saveDetail() will result in a new detail being saved, rather than
 *  another detail being updated.
 */
void QContactDetail::resetKey()
{
    d->m_detailId = QContactDetailPrivate::lastDetailKey.fetchAndAddOrdered(1);
}

/*!
 *  Returns the value stored in this detail for the given \a field. An invalid QVariant is returned if
 *  the value of \a field is not set.
 */
QVariant QContactDetail::value(int field) const
{
    if (!hasValue(field))
        return QVariant();
    return d->value(field);
}

/*!
 *  Returns true if this detail has a field with the given \a field, or false otherwise.
 */
bool QContactDetail::hasValue(int field) const
{
    return d->hasValue(field);
}

/*!
    Inserts \a value into the detail for the given \a field if \a value is valid.  If \a value is invalid,
    removes the field with the given \a field from the detail.  Returns true if the given \a value was set
    for the \a field (if the \a value was valid), or if the given \a field was removed from detail (if the
    \a value was invalid), and returns false if the field could not be removed (and the \a value was invalid)
*/
bool QContactDetail::setValue(int field, const QVariant& _value)
{
    if (!_value.isValid())
        return removeValue(field);
    return d->setValue(field, _value);
}

void QContactDetail::setValues(const QMap<int, QVariant> &newValues)
{
    QMap<int, QVariant>::const_iterator it = newValues.constBegin(), end = newValues.constEnd();
    for ( ; it != end; ++it) {
        setValue(it.key(), it.value());
    }
}

/*!
    Removes the value stored in this detail for the given \a field.  Returns true if a value was stored
    for the given \a field and the operation succeeded, and false otherwise.
*/
bool QContactDetail::removeValue(int field)
{
    return d->removeValue(field);
}

/*!
  Returns the values stored in this detail as a field-to-value map.
 */
QMap<int, QVariant> QContactDetail::values() const
{
    return d->values();
}

/*!
  \enum QContactDetail::AccessConstraint

  This enum defines the access constraints for a detail.  This information is typically provided by
  the manager when a contact is retrieved.

  \value NoConstraint Users can read, write, and otherwise modify this detail in any manner.
  \value ReadOnly Users cannot write or modify values in this detail.
  \value Irremovable Users cannot remove this detail from a contact.
 */


/*!
  Returns the access constraints associated with the detail.

  Some details may not be written to, while other details may
  not be removed from a contact.

  \sa QContactDetail::AccessConstraints
 */
QContactDetail::AccessConstraints QContactDetail::accessConstraints() const
{
    return d.constData()->m_access;
}


/*!
  \fn QString QContactDetail::provenance() const

  This is a convenience function to return the \c Provenance field of this detail.
  Any detail in an aggregate contact which originally comes from a \c Facet contact
  should have provenance information.

  It is equivalent to the following code:
  \code
  value(QContactDetail::FieldProvenance);
  \endcode

  \sa value()
 */

/*!
  \enum QContactDetail::DetailContext

  This enum defines the contexts for a detail.

  \value ContextHome The detail data relates to home / private detail about the contact.
  \value ContextWork The detail data relates to business / work detail about the contact.
  \value ContextOther Generic context, neither ContextHome nor ContextWork
  \sa setContexts(), contexts()
 */

/*!
  \fn void QContactDetail::setContexts(const QList<int>& contexts)

  This is a convenience function that sets the \c Context field of this detail to the given \a contexts.

  It is equivalent to the following code:
  \code
  setValue(QContactDetail::FieldContext, contexts);
  \endcode

  \sa setValue()
 */

/*!
  \fn void QContactDetail::setContexts(int context)

  This is a convenience function that sets the \c Context field of this detail to the given \a context.
  It is useful if the detail is only valid in a single context.

  It is equivalent to the following code:
  \code
  setValue(FieldContext, QList<int>() << context);
  \endcode

  \sa setValue()
 */

/*!
  \fn QStringList QContactDetail::contexts() const

  This is a convenience function to return the \c Context field of this detail.

  It is equivalent to the following code:
  \code
  value<QStringList>(QContactDetail::FieldContext);
  \endcode

  \sa value()
 */

/*!
  \enum QContactDetail::DetailField

  This enum defines the common fields supported by any detail.

  \value FieldContext The field containing the contexts of a detail.
  \value FieldDetailUri The field containing the detail URI of a detail.
  \value FieldLinkedDetailUris The field containing the URIs of other details linked to a detail.
  \value FieldProvenance The field containing the provenance information for the detail.
  \sa setContexts(), contexts()
  \sa setDetailUri(), detailUri()
  \sa setLinkedDetailUris(), linkedDetailUris()
  \sa provenance()
 */

/*!
  \fn void QContactDetail::setDetailUri(const QString& detailUri)

  This is a convenience function that sets the \c DetailUri field of this detail to the given \a detailUri.
  In order to be linked to, a detail must have a specific and (per-contact) unique detail URI set.

  It is equivalent to the following code:
  \code
  setValue(FieldDetailUri, detailUri);
  \endcode

  \sa setValue()
 */

/*!
  \fn QString QContactDetail::detailUri() const

  This is a convenience function to return the \c DetailUri field of this detail.

  It is equivalent to the following code:
  \code
  value(QContactDetail::FieldDetailUri);
  \endcode

  \sa value()
 */


/*!
  \fn void QContactDetail::setLinkedDetailUris(const QStringList& linkedDetailUris)

  This is a convenience function that sets the \c LinkedDetailUris field of this detail to the given \a linkedDetailUris.

  It is equivalent to the following code:
  \code
  setValue(QContactDetail::FieldLinkedDetailUris, linkedDetailUris);
  \endcode

  \sa setValue()
 */

/*!
  \fn void QContactDetail::setLinkedDetailUris(const QString& linkedDetailUri)

  This is a convenience function that sets the \c LinkedDetailUris field of this detail to the given \a linkedDetailUri.
  It is useful if the detail is linked to a single other detail in the contact.

  It is equivalent to the following code:
  \code
  setValue(FieldLinkedDetailUris, QStringList(linkedDetailUri));
  \endcode

  \sa setValue()
 */

/*!
  \fn QStringList QContactDetail::linkedDetailUris() const

  This is a convenience function to return the \c Context field of this detail.

  It is equivalent to the following code:
  \code
  value<QStringList>(QContactDetail::FieldLinkedDetailUris);
  \endcode

  \sa value()
 */


QT_END_NAMESPACE_CONTACTS
