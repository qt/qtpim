/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtContacts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactdetailfilter.h"
#include "qcontactdetailfilter_p.h"

QT_BEGIN_NAMESPACE_CONTACTS

/*!
  \class QContactDetailFilter
  \brief The QContactDetailFilter class provides a filter based around a detail value criterion


  \inmodule QtContacts

  \ingroup contacts-filters

  It may be used to select contacts which contain a detail of a particular type with a particular value
 */

Q_IMPLEMENT_CONTACTFILTER_PRIVATE(QContactDetailFilter)

/*!
 * \fn QContactDetailFilter::QContactDetailFilter(const QContactFilter& other)
 * Constructs a copy of \a other if possible, otherwise constructs a new detail filter
 */

/*!
 * Constructs a new detail filter
 */
QContactDetailFilter::QContactDetailFilter()
    : QContactFilter(new QContactDetailFilterPrivate)
{
}

/*!
 * Sets the type of detail which will be matched to \a type, and the field of the detail
 * which will contain the value criterion to \a field.
 *
 * If \a type is QContactDetail::TypeUndefined, the detail filter will match no contacts. If \a field
 * is not specified, or equal to -1, the detail filter acts like a "detail exists" filter; if any
 * detail of the specified type is present in a contact, that contact will match the filter, regardless
 * of what values might be stored in that detail.
 *
 * \sa detailType(), detailField()
 */
void QContactDetailFilter::setDetailType(QContactDetail::DetailType type, int field)
{
    Q_D(QContactDetailFilter);
    d->m_type = type;
    d->m_fieldId = field;
}

/*!
 * Sets the value criterion of the filter to \a value.
 * If the field criterion (set via setDetailType())
 * of the filter is not specified or equal to -1, this value will be ignored.
 * Note that certain backends might perform backend specific sanitization of
 * \a value for those detail types that are supported by them.
 * If the provided value cannot be sanitized, the filter is considered
 * invalid.
 * \sa value(), setDetailType()
 */
void QContactDetailFilter::setValue(const QVariant& value)
{
    Q_D(QContactDetailFilter);
    d->m_exactValue = value;
}

/*!
 * Sets the semantics of the value matching criterion to those defined in \a flags
 * \sa matchFlags()
 */
void QContactDetailFilter::setMatchFlags(QContactFilter::MatchFlags flags)
{
    Q_D(QContactDetailFilter);
    d->m_flags = flags;
}

/*!
 * Returns the semantics of the value matching criterion
 * \sa setMatchFlags()
 */
QContactFilter::MatchFlags QContactDetailFilter::matchFlags() const
{
    Q_D(const QContactDetailFilter);
    return d->m_flags;
}

/*!
 * Returns the type of the details which will be inspected for matching values
 * \sa setDetailType()
 */
QContactDetail::DetailType QContactDetailFilter::detailType() const
{
    Q_D(const QContactDetailFilter);
    return d->m_type;
}

/*!
 * Returns the detail field containing the value which will be matched against the value criterion
 * \sa setDetailType()
 */
int QContactDetailFilter::detailField() const
{
    Q_D(const QContactDetailFilter);
    return d->m_fieldId;
}

/*!
 * Returns the value criterion of the detail filter
 * \sa setValue()
 */
QVariant QContactDetailFilter::value() const
{
    Q_D(const QContactDetailFilter);
    return d->m_exactValue;
}

QT_END_NAMESPACE_CONTACTS
