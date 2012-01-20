/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QDECLARATIVECONTACT_P_H
#define QDECLARATIVECONTACT_P_H

#include <qcontact.h>
#include <qcontactid.h>
#include "qdeclarativecontactdetails_p.h"
#include "qdeclarativecontacturl_p.h"
#include "qdeclarativecontacthobby_p.h"
#include "qdeclarativecontactpersonid_p.h"
#include "qdeclarativecontactdetail_p.h"
#include <QtDeclarative/qdeclarativelist.h>

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContact : public QObject
{
    Q_OBJECT

    // basic information
    Q_PROPERTY (bool modified READ modified)

    Q_PROPERTY(QDeclarativeContactType::ContactType type READ type NOTIFY contactChanged)
    Q_PROPERTY (QContactLocalId contactId READ contactId NOTIFY contactIdChanged)
    Q_PROPERTY (QString manager READ manager NOTIFY managerChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactDetail> contactDetails READ contactDetails NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactAddress* address READ address NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactAddress> addresses READ addresses NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactAnniversary* anniversary READ anniversary NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactAvatar* avatar READ avatar NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactBirthday*  birthday READ birthday NOTIFY contactChanged)
    Q_PROPERTY (QString  displayLabel READ displayLabel NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactEmailAddress*  email READ email NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactEmailAddress> emails READ emails NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactFamily*  family READ family NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactFavorite*  favorite READ favorite NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactGender*  gender READ gender NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactGeoLocation*  geolocation READ geolocation NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactGlobalPresence*  globalPresence READ globalPresence NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactGuid*  guid READ guid NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactName*  name READ name NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactNickname*  nickname READ nickname  NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactNote*  note READ note NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactOnlineAccount*  onlineAccount READ onlineAccount NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactOrganization*  organization READ organization  NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactOrganization>  organizations READ organizations  NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactPhoneNumber*  phoneNumber READ phoneNumber NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactPhoneNumber> phoneNumbers READ phoneNumbers NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactPresence*  presence READ presence NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactRingtone*  ringtone READ ringtone NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactSyncTarget*  syncTarget READ syncTarget NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactTag*  tag READ tag NOTIFY contactChanged)
    Q_PROPERTY (QUrl thumbnail READ thumbnail WRITE setThumbnail NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactUrl*  url READ url NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactUrl> urls READ urls NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactHobby*  hobby READ hobby NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactPersonId*  personid READ personid NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeContactExtendedDetail*  extendedDetail READ extendedDetail NOTIFY contactChanged)
    Q_PROPERTY (QDeclarativeListProperty<QDeclarativeContactExtendedDetail> extendedDetails READ extendedDetails NOTIFY contactChanged)
    Q_CLASSINFO("DefaultProperty", "contactDetails")

public:

    explicit QDeclarativeContact(QObject *parent = 0);
    ~QDeclarativeContact();

    void setContact(const QContact& c);
    QContact contact() const;
    bool modified() const;

    QDeclarativeContactType::ContactType type() const;

    QContactLocalId contactId() const;
    QString manager() const;
    QDeclarativeListProperty<QDeclarativeContactDetail> contactDetails();

    // use int instead of QDeclarativeContactDetail::ContactType as a work-around for QTBUG-20639
    Q_INVOKABLE QDeclarativeContactDetail* detail(int type);
    Q_INVOKABLE QVariantList details(int type);

    Q_INVOKABLE bool removeDetail(QDeclarativeContactDetail* detail);
    Q_INVOKABLE bool addDetail(QDeclarativeContactDetail* detail);

    QDeclarativeContactAddress* address();
    QDeclarativeListProperty<QDeclarativeContactAddress>  addresses();
    QDeclarativeContactAnniversary* anniversary();
    QDeclarativeContactAvatar* avatar();
    QDeclarativeContactBirthday*  birthday();
    QString  displayLabel();
    QDeclarativeContactEmailAddress*  email();
    QDeclarativeListProperty<QDeclarativeContactEmailAddress>  emails();
    QDeclarativeContactFamily*  family();
    QDeclarativeContactFavorite*  favorite();
    QDeclarativeContactGender*  gender();
    QDeclarativeContactGeoLocation*  geolocation();
    QDeclarativeContactGlobalPresence*  globalPresence();
    QDeclarativeContactGuid*  guid();
    QDeclarativeContactName*  name();
    QDeclarativeContactNickname*  nickname();
    QDeclarativeContactNote*  note();
    QDeclarativeContactOnlineAccount*  onlineAccount();
    QDeclarativeContactOrganization*  organization();
    QDeclarativeListProperty<QDeclarativeContactOrganization>  organizations();
    QDeclarativeContactPhoneNumber*  phoneNumber();
    QDeclarativeListProperty<QDeclarativeContactPhoneNumber>  phoneNumbers();
    QDeclarativeContactPresence*  presence();
    QDeclarativeContactRingtone*  ringtone();
    QDeclarativeContactSyncTarget*  syncTarget();
    QDeclarativeContactTag*  tag();
    QUrl  thumbnail() const;
    void  setThumbnail(const QUrl& url);
    QDeclarativeContactUrl*  url();
    QDeclarativeListProperty<QDeclarativeContactUrl>  urls();
    QDeclarativeContactHobby*  hobby();
    QDeclarativeContactPersonId* personid();
    QDeclarativeContactExtendedDetail*  extendedDetail();
    QDeclarativeListProperty<QDeclarativeContactExtendedDetail>  extendedDetails();

protected:
    bool m_modified;
    QContactId m_id;

    // always create a copy of the detail for QML
    // however, seems the garbage collection can't delete all of them (QTBUG-20377)
    QList<QDeclarativeContactDetail *> m_details;

public slots:
    void clearDetails();
    void save();
Q_SIGNALS:
    void contactIdChanged();
    void managerChanged();
    void contactChanged();

private slots:
    void setModified();

private:
    Q_DISABLE_COPY(QDeclarativeContact)

    template<typename T> T* getDetail(const QDeclarativeContactDetail::ContactDetailType &definitionName)
    {
        foreach (QDeclarativeContactDetail *detail, m_details) {
            if (definitionName == detail->detailType())
            {
                T* tempDetail = static_cast<T *>(detail);
                return tempDetail;
            }
        }
        T* detail = new T;
        if (detail) {
            m_details.append(detail);
            emit contactChanged();
            return detail;
        }
        return 0;
    }

    // call-back functions for list property
    static void _q_detail_append(QDeclarativeListProperty<QDeclarativeContactDetail> *property, QDeclarativeContactDetail *value);
    static QDeclarativeContactDetail *_q_detail_at(QDeclarativeListProperty<QDeclarativeContactDetail> *property, int index);
    static void _q_detail_clear(QDeclarativeListProperty<QDeclarativeContactDetail> *property);
    static int _q_detail_count(QDeclarativeListProperty<QDeclarativeContactDetail> *property);
};

QTCONTACTS_END_NAMESPACE

Q_DECLARE_METATYPE(QTCONTACTS_PREPEND_NAMESPACE(QContactLocalId))

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContact))

#endif // QDECLARATIVECONTACT_P_H
