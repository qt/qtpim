/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtOrganizer module of the Qt Toolkit.
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

#include <qorganizeritemdetailfilter.h>
#include <private/qorganizeritemdetailfilter_p.h>

QTORGANIZER_BEGIN_NAMESPACE

/*!
    \class QOrganizerItemDetailFilter
    \brief The QOrganizerItemDetailFilter class provides a filter based around a detail value criterion.
    \inmodule QtOrganizer
    \ingroup organizer-filters

    It may be used to select organizeritems which contain a detail of a particular type and a particular value.
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
    Sets the type of detail which will be matched to \a detailType, and the field of the detail
    which will contain the value criterion to \a field.

    \sa detailType(), detailField()
 */
void QOrganizerItemDetailFilter::setDetail(QOrganizerItemDetail::DetailType detailType, int field)
{
    Q_D(QOrganizerItemDetailFilter);
    if (detailType != QOrganizerItemDetail::TypeUndefined && field >= 0) {
        d->m_detailType = detailType;
        d->m_detailField = field;
    } else {
        d->m_detailType = QOrganizerItemDetail::TypeUndefined;
        d->m_detailField = -1;
    }
}

/*!
    Sets the value criterion of the filter to \a value.

    \sa value()
 */
void QOrganizerItemDetailFilter::setValue(const QVariant &value)
{
    Q_D(QOrganizerItemDetailFilter);
    d->m_exactValue = value;
}

/*!
    Sets the semantics of the value matching criterion to those defined in \a flags.

    \sa matchFlags()
 */
void QOrganizerItemDetailFilter::setMatchFlags(QOrganizerItemFilter::MatchFlags flags)
{
    Q_D(QOrganizerItemDetailFilter);
    d->m_flags = flags;
}

/*!
    Returns the semantics of the value matching criterion.

    \sa setMatchFlags()
 */
QOrganizerItemFilter::MatchFlags QOrganizerItemDetailFilter::matchFlags() const
{
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_flags;
}

/*!
    Returns the type of the detail which will be inspected for matching values.

    \sa setDetail()
 */
QOrganizerItemDetail::DetailType QOrganizerItemDetailFilter::detailType() const
{
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_detailType;
}

/*!
    Returns the detail field containing the value which will be matched against the value criterion.

    \sa setDetail()
 */
int QOrganizerItemDetailFilter::detailField() const
{
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_detailField;
}

/*!
    Returns the value criterion of the detail filter.

    \sa setValue()
 */
QVariant QOrganizerItemDetailFilter::value() const
{
    Q_D(const QOrganizerItemDetailFilter);
    return d->m_exactValue;
}

QTORGANIZER_END_NAMESPACE
