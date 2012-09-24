/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qorganizeritemdetailfilter.h>
#include <private/qorganizeritemdetailfilter_p.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerItemDetailFilter
    \brief The QOrganizerItemDetailFilter class provides a filter based around a detail value criterion.
    \inmodule QtOrganizer
    \ingroup organizer-filters

    It may be used to select organizeritems which contain a detail identical to that used by
    this filter for matching.
 */

Q_IMPLEMENT_ORGANIZERITEMFILTER_PRIVATE(QOrganizerItemDetailFilter)

/*!
    \fn QOrganizerItemDetailFilter::QOrganizerItemDetailFilter(const QOrganizerItemFilter &other)

    Constructs a copy of \a other if possible, otherwise constructs a new detail filter.
 */

/*!
    Constructs a new detail filter.
 */
QOrganizerItemDetailFilter::QOrganizerItemDetailFilter()
    : QOrganizerItemFilter(new QOrganizerItemDetailFilterPrivate)
{
}

/*!
    Sets the type of detail that will be matched to \a detailType, and the field of the detail
    that will contain the value criterion to \a field.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa detailType(), detailField()
    \deprecated
 */
void QOrganizerItemDetailFilter::setDetail(QOrganizerItemDetail::DetailType detailType, int field)
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::setDetail() instead."
             , Q_FUNC_INFO);
    Q_D(QOrganizerItemDetailFilter);
    int fieldId;
    if (detailType != QOrganizerItemDetail::TypeUndefined && field >= 0) {
        d->m_detailType = detailType;
        fieldId = field;
    } else {
        d->m_detailType = QOrganizerItemDetail::TypeUndefined;
        fieldId = -1;
    }
    if (d->m_detailFields.isEmpty())
        d->m_detailFields.append(fieldId);
    else
        d->m_detailFields[0] = fieldId;
}

/*!
    Sets the detail that will be matched to \a detail.

    \sa detail()
 */
void QOrganizerItemDetailFilter::setDetail(const QOrganizerItemDetail &detail)
{
    Q_D(QOrganizerItemDetailFilter);
    d->m_detail = detail;
}

/*!
    Returns the detail which will be matched by this filter.

    \sa setDetail()
 */
QOrganizerItemDetail QOrganizerItemDetailFilter::detail() const
{
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_detail;
}

/*!
    Sets the value criterion of the filter to \a value.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa value()
    \deprecated
 */
void QOrganizerItemDetailFilter::setValue(const QVariant &value)
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::setValue() instead."
             , Q_FUNC_INFO);
    Q_D(QOrganizerItemDetailFilter);
    if (!value.isNull()) {
        if (d->m_exactValues.isEmpty())
            d->m_exactValues.append(value);
        else
            d->m_exactValues[0] = value;
    }
}

/*!
    Sets the semantics of the value matching criterion to those defined in \a flags.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa matchFlags()
 */
void QOrganizerItemDetailFilter::setMatchFlags(QOrganizerItemFilter::MatchFlags flags)
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::setMatchFlags() instead."
             , Q_FUNC_INFO);
    Q_D(QOrganizerItemDetailFilter);
    d->m_flags = flags;
}

/*!
    Returns the semantics of the value matching criterion.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa setMatchFlags()
 */
QOrganizerItemFilter::MatchFlags QOrganizerItemDetailFilter::matchFlags() const
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::matchFlags() instead."
             , Q_FUNC_INFO);
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_flags;
}

/*!
    Returns the type of the detail which will be inspected for matching values.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa setDetail()
    \deprecated
 */
QOrganizerItemDetail::DetailType QOrganizerItemDetailFilter::detailType() const
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::detailType() instead."
             , Q_FUNC_INFO);
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_detailType;
}

/*!
    Returns the detail field containing the value which will be matched against the value criterion.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa setDetail()
    \deprecated
 */
int QOrganizerItemDetailFilter::detailField() const
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::detailField() instead."
             , Q_FUNC_INFO);
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_detailFields.isEmpty() ? -1 : d->m_detailFields.first() ;
}

/*!
    Returns the value criterion of the detail filter.

    \warning This method is deprecated because same functionality
             has been migrated to QOrganizerItemDetailFieldFilter. Use the
             new QOrganizerItemDetailFieldFilter class instead.
    \sa setValue()
    \deprecated
 */
QVariant QOrganizerItemDetailFilter::value() const
{
    qWarning("%s is deprecated, use QOrganizerItemDetailFieldFilter::value() instead."
             , Q_FUNC_INFO);
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_exactValues.isEmpty() ? QVariant() : d->m_exactValues.first();
}

QTORGANIZER_END_NAMESPACE
