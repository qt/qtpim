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

#include "qcontactactiondescriptor.h"
#include "qcontactactiondescriptor_p.h"

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

#include "qcontact.h"
#include "qcontactactionfactory.h"
#include "qcontactinvalidfilter.h"

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

/*!
  \class QContactActionDescriptor
  \brief The QContactActionDescriptor class provides information that
  uniquely identifies a specific implementation of an action
  \ingroup contacts-actions
  \inmodule QtContacts
*/

/*!
 * Constructs a new, invalid action descriptor
 */
QContactActionDescriptor::QContactActionDescriptor()
        : d(new QContactActionDescriptorPrivate())
{
}

/*!
 * Constructs a copy of the \a other action descriptor
 */
QContactActionDescriptor::QContactActionDescriptor(const QContactActionDescriptor& other)
        : d(other.d)
{
}

/*!
 * Assigns this action descriptor to be equal to \a other
 */
QContactActionDescriptor& QContactActionDescriptor::operator=(const QContactActionDescriptor& other)
{
    d = other.d;
    return *this;
}

/*!
 * Cleans up any memory in use by the action descriptor
 */
QContactActionDescriptor::~QContactActionDescriptor()
{
}

/*!
 * Returns the name of the action which is identified by the action descriptor
 */
QString QContactActionDescriptor::actionName() const
{
    return d.constData()->m_actionName;
}


/*!
 * Returns the name of the service of the action implementation which is identified by the action descriptor
 */
QString QContactActionDescriptor::serviceName() const
{
    return d.constData()->m_serviceName;
}

/*!
 * Returns the identifier of the action, within the service.
 */
QString QContactActionDescriptor::actionIdentifier() const
{
    return d.constData()->m_identifier;
}

/*!
  Returns the service-specified version of the action implementation which is identified by the action descriptor
 */
int QContactActionDescriptor::implementationVersion() const
{
    return d.constData()->m_implementationVersion;
}

/*!
  Returns the set of action targets which are supported by this action for the given contact \a contact
 */
QSet<QContactActionTarget> QContactActionDescriptor::supportedTargets(const QContact& contact) const
{
    if (d.constData()->m_factory) {
        return d.constData()->m_factory->supportedTargets(contact, *this);
    }

    return QSet<QContactActionTarget>();
}

/*!
  Returns a filter which will match contacts for which this action has at least one supported action target
 */
QContactFilter QContactActionDescriptor::contactFilter() const
{
    if (d.constData()->m_factory) {
        return d.constData()->m_factory->contactFilter(*this);
    }

    return QContactInvalidFilter();
}


/*!
   \variable QContactActionDescriptor::MetaDataIcon
   The meta data key which corresponds to the meta data value
   which contains the icon which should be displayed for this
   action.
   \sa metaData()
 */
const QString QContactActionDescriptor::MetaDataIcon(QStringLiteral("Icon"));

/*!
   \variable QContactActionDescriptor::MetaDataLabel
   The meta data key which corresponds to the meta data value
   which contains the display label for this action.
   \sa metaData()
 */
const QString QContactActionDescriptor::MetaDataLabel(QStringLiteral("Label"));

/*!
   \variable QContactActionDescriptor::MetaDataSecondLabel
   The meta data key which corresponds to the meta data value
   which contains the second or additional display label for this
   action.
   \sa metaData()
 */
const QString QContactActionDescriptor::MetaDataSecondLabel(QStringLiteral("SecondLabel"));

/*!
   \variable QContactActionDescriptor::MetaDataOptionalParameterKeys
   The meta data key which corresponds to the meta data value which
   contains the list of keys of parameters which the client may provide
   at invocation time which may affect the action.

   An example of an optional parameter might be an "attachment"
   parameter to a "send email" action.

   \sa metaData()
 */
const QString QContactActionDescriptor::MetaDataOptionalParameterKeys(QStringLiteral("OptionalParameterKeys"));

/*!
   \variable QContactActionDescriptor::MetaDataMandatoryParameterKeys
   The meta data key which corresponds to the meta data value which
   contains the list of keys of parameters which the client must provide
   at invocation for the action to succeed.

   An example of a mandatory parameter might be a "recipient"
   parameter to a "send email" action.

   \sa metaData()
 */
const QString QContactActionDescriptor::MetaDataMandatoryParameterKeys(QStringLiteral("MandatoryParameterKeys"));

/*!
  Returns the meta data for the given meta data key \a key for the the given action targets \a targets with the given invocation parameters \a parameters.
 */
QVariant QContactActionDescriptor::metaData(const QString& key, const QList<QContactActionTarget>& targets, const QVariantMap& parameters) const
{
    if (d.constData()->m_factory) {
        return d.constData()->m_factory->metaData(key, targets, parameters, *this);
    }

    return QVariant();
}

/*!
  Returns the meta data for the given meta data key \a key with the given invocation parameters \a parameters.
 */
QVariant QContactActionDescriptor::metaData(const QString& key, const QVariantMap& parameters) const
{
    return metaData(key, QList<QContactActionTarget>(), parameters);
}

/*!
  Returns the meta data for the given meta data key \a key for the \a target, with the given invocation parameters \a parameters.
 */
QVariant QContactActionDescriptor::metaData(const QString& key, const QContactActionTarget& target, const QVariantMap& parameters) const
{
    return metaData(key, QList<QContactActionTarget>() << target, parameters);
}

/*!
  Returns the meta data for the given meta data key \a key for a target identified by \a contact and \a detail, with the given invocation parameters \a parameters.
 */
QVariant QContactActionDescriptor::metaData(const QString& key, const QContact& contact, const QContactDetail& detail, const QVariantMap& parameters) const
{
    return metaData(key, QList<QContactActionTarget>() << QContactActionTarget(contact, detail), parameters);
}

/*!
  Returns true if the action which this descriptor describes supports at least one action target for the given \a contact
 */
bool QContactActionDescriptor::supportsContact(const QContact& contact) const
{
    if (d.constData()->m_factory) {
        return d.constData()->m_factory->supportsContact(contact, *this);
    }

    return false;
}

/*!
 * Returns false if either the name, service and version of the descriptor are missing from the descriptor,
 * or if the descriptor is not associated with a valid action factory which can create instances of an action.
 * An empty descriptor cannot uniquely identify an action.
 */
bool QContactActionDescriptor::isValid() const
{
    if (d.constData()->m_actionName.isEmpty())
        return false;
    if (d.constData()->m_serviceName.isEmpty())
        return false;
    if (d.constData()->m_identifier.isEmpty())
        return false;
    if (d.constData()->m_implementationVersion <= 0)
        return false;
    if (d.constData()->m_factory == 0)
        return false;
    return true;
}

/*!
 * Returns true if the action identified by this descriptor is the same as the action
 * identified by the \a other descriptor.  Note that two actions with the same
 * action name, service name and implementation version may in fact be different (for example,
 * they may have different metaData), so using this function is the only way for clients
 * to tell whether or not the action descriptors identify different actions.
 */
bool QContactActionDescriptor::operator==(const QContactActionDescriptor& other) const
{
    return (d.constData()->m_factory == other.d.constData()->m_factory && d.constData()->m_identifier == other.d.constData()->m_identifier);
}

/*!
 * Returns true if the action name, service name or service-specified implementation version
 * specified by this action descriptor are different to that specified by \a other
 */
bool QContactActionDescriptor::operator!=(const QContactActionDescriptor& other) const
{
    return !(*this == other);
}

/*! Returns the hash value for \a key. */
uint qHash(const QContactActionDescriptor& key)
{
    uint ret = 0;

    ret += QT_PREPEND_NAMESPACE(qHash)(key.serviceName())
            + QT_PREPEND_NAMESPACE(qHash)(key.actionName())
            + QT_PREPEND_NAMESPACE(qHash)(key.d.constData()->m_identifier)
            + QT_PREPEND_NAMESPACE(qHash)(key.implementationVersion())
            + QT_PREPEND_NAMESPACE(qHash)(key.d.constData()->m_factory);

    return ret;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug& operator<<(QDebug dbg, const QContactActionDescriptor& descriptor)
{
    dbg.nospace() << "QContactActionDescriptor("
                  << descriptor.serviceName() << ","
                  << descriptor.actionName() << ","
                  << descriptor.d.constData()->m_identifier << ","
                  << descriptor.implementationVersion() << ","
                  << descriptor.d.constData()->m_factory
                  << ')';
    return dbg.maybeSpace();
}
#endif

QT_END_NAMESPACE_CONTACTS
