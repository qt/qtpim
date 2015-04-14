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

#include "qcontactfetchhint.h"
#include "qcontactfetchhint_p.h"

#ifndef QT_NO_DATASTREAM
#include <QtCore/qdatastream.h>
#endif
#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/qdebug.h>
#endif

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactFetchHint

  \inmodule QtContacts

  \brief The QContactFetchHint class provides hints to the manager about which contact
  information needs to be retrieved in an asynchronous fetch request or a synchronous
  function call.

  All of the hints may be ignored at the discretion of the manager, however if a manager
  is able to optimize retrieval of contacts due to hints, it may do so.  If a manager
  ignores a hint, it must retrieve the full set of data that the hint refers to.

  The fetch hint contains:
  \list
   \li a list of detail types which the client is interested
  in (empty if interested in all detail types)
   \li a list of relationship types which the client is interested in
  (empty if interested in all relationships)
   \li some optimization flags which allow the client to tell the backend if they are
  not interested in any relationships, any action preferences, or any binary blobs (images etc).
   \li a preferred size for any images, if the backend supports multiple sizes or scaling
  \endlist

  Important note: a client should not make changes to a contact which has been retrieved
  using a fetch hint other than the default fetch hint.  Doing so can result in information
  loss when saving the contact back to the manager (as the "new" restricted contact will
  replace the previously saved contact in the backend).
 */

/*!
  \enum QContactFetchHint::OptimizationHint

  This enum defines flags which may be set to inform the backend that the client does
  not require certain information.  The backend may safely ignore the hint, but then
  must return the full set of information relating to the optimization hint.

  \value AllRequired Tells the backend that all information is required
  \value NoRelationships Tells the backend that the client does not require retrieved contacts to include a cache of relationships
  \value NoActionPreferences Tells the backend that the client does not require retrieved contacts to include a cache of action preferences
  \value NoBinaryBlobs Tells the backend that the client does not require retrieved contacts to include binary blobs
 */

/*!
  Constructs a new contact fetch hint which requests that the backend fetch all information
 */
QContactFetchHint::QContactFetchHint()
    : d(new QContactFetchHintPrivate)
{
}

/*!
  Constructs a new contact fetch hint as a copy of \a other
 */
QContactFetchHint::QContactFetchHint(const QContactFetchHint &other)
    : d(other.d)
{
}

/*!
  Frees any memory in use by the fetch hint
 */
QContactFetchHint::~QContactFetchHint()
{
}

/*!
  Assigns this fetch hint to be equal to the \a other fetch hint
 */
QContactFetchHint& QContactFetchHint::operator=(const QContactFetchHint& other)
{
    d = other.d;
    return *this;
}

/*!
  Returns the list of detail types that identify which detail type
  the manager should (at a minimum) retrieve when fetching contacts.
  This hint may be ignored by the backend, in which case it will return the full set of details for
  each contact retrieved.

  \sa setDetailTypesHint()
 */
QList<QContactDetail::DetailType> QContactFetchHint::detailTypesHint() const
{
    return d.constData()->m_typesHint;
}

/*!
  Sets the list of detail types that identify which detail type
  the manager should (at a minimum) retrieve when fetching contacts to \a types.
  This hint may be ignored by the backend, in which case it will return the full set of details for
  each contact retrieved.

  \sa detailTypesHint()
 */
void QContactFetchHint::setDetailTypesHint(const QList<QContactDetail::DetailType> &types)
{
    d->m_typesHint = types;
}

/*!
  Returns the list of relationship types that the manager should (at a minimum) retrieve
  when fetching contacts.
  This hint may be ignored by the backend, in which case it will return the full set of
  relationships for each contact retrieved.

  \sa setRelationshipTypesHint(), QContact::relationships()
 */
QStringList QContactFetchHint::relationshipTypesHint() const
{
    return d.constData()->m_relationshipsHint;
}

/*!
  Sets the list of relationship types that the manager should (at a minimum) retrieve
  when fetching contacts to \a relationshipTypes.
  This hint may be ignored by the backend, in which case it will return the full set of
  relationships for each contact retrieved.

  \sa relationshipTypesHint(), QContact::relationships()
 */
void QContactFetchHint::setRelationshipTypesHint(const QStringList& relationshipTypes)
{
    d->m_relationshipsHint = relationshipTypes;
}

/*!
  Returns the preferred pixel dimensions for any images returned
  by the manager for a given request.  This hint may be ignored
  by the manager.

  This is useful when the backend supports multiple sizes of an
  image (or the image is natively scaleable) in order to get an
  image that will look good at the indicated dimensions.

  The caller should be prepared for images of any dimensions,
  in any case.
 */
QSize QContactFetchHint::preferredImageSize() const
{
    return d.constData()->m_preferredImageSize;
}

/*!
  Sets the preferred pixel dimensions for any images returned
  by the manager for the given request to \a size.  This hint
  may be ignored by the manager.

  This is useful when the backend supports multiple sizes of an
  image (or the image is natively scaleable) in order to get an
  image that will look good at the indicated dimensions.

  The caller should be prepared for images of any dimensions,
  in any case.
 */
void QContactFetchHint::setPreferredImageSize(const QSize& size)
{
    d->m_preferredImageSize = size;
}

/*!
  Returns the optimization hint flags specified by the client.
  These hints may be ignored by the backend, in which case it will return
  the full set of information accessible in a contact, including
  relationships, action preferences, and binary blobs.

  \sa setOptimizationHints()
 */
QContactFetchHint::OptimizationHints QContactFetchHint::optimizationHints() const
{
    return d.constData()->m_optimizationHints;
}

/*!
  Sets the optimization hint flags specified by the client to \a hints.
  These hints may be ignored by the backend, in which case it will return
  the full set of information accessible in a contact, including
  relationships, action preferences, and binary blobs.

  \sa optimizationHints()
 */
void QContactFetchHint::setOptimizationHints(OptimizationHints hints)
{
    d->m_optimizationHints = hints;
}

/*!
  Returns the number of results which the client considers to be the
  maximum number of useful results.  The client is only interested
  in this number of results, so returning any more results
  would be superfluous to the client's requirements.
  Note that this fetch hint only affects operations where the
  backend would return a list of contacts; this hint specifies
  the maximum number of contacts in the list which would be useful
  to the client.  The backend may ignore this hint, in which case it
  must return all contacts which would otherwise have been returned
  as a result of the operation.

  A negative value for count denotes that the client wishes to
  retrieve all results.  The default value is -1.
 */
int QContactFetchHint::maxCountHint() const
{
    return d.constData()->m_maxCount;
}

/*!
  Sets the maximum number of results which the client is interested in
  to \a count.  The client is only interested in this number of results,
  so returning any more results would be superfluous to the client's
  requirements. Note that this fetch hint only affects operations where
  the backend would return a list of contacts; this hint specifies
  the maximum number of contacts in the list which would be useful
  to the client.  The backend may ignore this hint, in which case it
  must return all contacts which would otherwise have been returned
  as a result of the operation.

  A negative value for count denotes that the client wishes to
  retrieve all results.  The default value is -1.
*/
void QContactFetchHint::setMaxCountHint(int count)
{
    count < 0 ? (d->m_maxCount = -1) : (d->m_maxCount = count);
}

#ifndef QT_NO_DATASTREAM
QDataStream& operator<<(QDataStream& out, const QContactFetchHint& hint)
{
    quint8 formatVersion = 2; // Version of QDataStream format for QContactFetchHint
    QList<quint32> detailTypeHintHelper;
    foreach (QContactDetail::DetailType hintType, hint.detailTypesHint())
        detailTypeHintHelper.append(static_cast<quint32>(hintType));

    return out << formatVersion
               << detailTypeHintHelper
               << hint.relationshipTypesHint()
               << static_cast<quint32>(hint.optimizationHints())
               << hint.preferredImageSize()
               << hint.maxCountHint();
}

QDataStream& operator>>(QDataStream& in, QContactFetchHint& hint)
{
    hint = QContactFetchHint();
    quint8 formatVersion;
    in >> formatVersion;
    if (formatVersion == 1 || formatVersion == 2) {
        QList<quint32> detailTypeHintsHelper;
        QList<QContactDetail::DetailType> detailTypeHints;
        QStringList relationshipTypeHints;
        quint32 optimizations;
        QSize dimensions;
        in >> detailTypeHintsHelper >> relationshipTypeHints >> optimizations >> dimensions;
        foreach (quint32 hintType, detailTypeHintsHelper)
            detailTypeHints.append(QContactDetail::DetailType(hintType));

        hint.setDetailTypesHint(detailTypeHints);
        hint.setRelationshipTypesHint(relationshipTypeHints);
        hint.setOptimizationHints(QContactFetchHint::OptimizationHints(optimizations));
        hint.setPreferredImageSize(dimensions);

        // version two also has a maximum fetch count hint.
        if (formatVersion == 2) {
            int maxCountHint = -1;
            in >> maxCountHint;
            hint.setMaxCountHint(maxCountHint);
        }
    } else {
        in.setStatus(QDataStream::ReadCorruptData);
    }
    return in;
}

#endif

#ifndef QT_NO_DEBUG_STREAM
/*!
  Outputs \a hint to the debug stream \a dbg
 */
QDebug operator<<(QDebug dbg, const QContactFetchHint& hint)
{
    dbg.nospace() << "QContactFetchHint("
                  << "detailTypeHint=" << hint.detailTypesHint() << ","
                  << "relationshipTypesHint=" << hint.relationshipTypesHint() << ","
                  << "optimizationHints=" << static_cast<quint32>(hint.optimizationHints()) << ","
                  << "preferredImageSize=" << hint.preferredImageSize()
                  << "maxCountHint=" << hint.maxCountHint()
                  << ")";
    return dbg.maybeSpace();
}
#endif

QT_END_NAMESPACE_CONTACTS
