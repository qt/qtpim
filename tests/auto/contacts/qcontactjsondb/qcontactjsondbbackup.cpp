/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QList>
#include "qcontactjsondbbackup.h"
#include "qcontactjsondbconverter.h"

QTCONTACTS_USE_NAMESPACE

QContactJsonDbBackup::QContactJsonDbBackup()
{
    m_dbClient = new SynchronizedJsonDbClient();
    backupJsonDb();
    clearJsonDb();
}





QContactJsonDbBackup::~QContactJsonDbBackup()
{
    revertJsonDb();
    delete m_dbClient;
}





bool QContactJsonDbBackup::loadTestData() {
    QContactManager cm;
    QContactSaveRequest csr;

    // save new contacts
    QContact testContact;
    QContactName nameDetail = testContact.detail<QContactName>();
    nameDetail.setFirstName("Harry");
    nameDetail.setLastName("Baker");
    testContact.saveDetail(&nameDetail);
    QContactEmailAddress emailDetail;
    emailDetail.setEmailAddress("Harry.Baker@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    QContactPhoneNumber phoneNumberDetail;
    QList<int> mySubTypes;
    mySubTypes << QContactPhoneNumber::SubTypeMobile;
    phoneNumberDetail.setSubTypes(mySubTypes);
    phoneNumberDetail.setNumber("+358507654321");
    testContact.saveDetail(&phoneNumberDetail);
    QList<QContact> saveList;
    saveList << testContact;

    nameDetail.setFirstName("Paul");
    nameDetail.setLastName("Thomson");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Paul.Thomson@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(mySubTypes);
    phoneNumberDetail.setNumber("+358507654321");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    nameDetail.setFirstName("Julie");
    nameDetail.setLastName("King");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Julie.King@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(mySubTypes);
    phoneNumberDetail.setNumber("+3585012345");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    nameDetail.setFirstName("Angelina");
    nameDetail.setLastName("Row");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Angelina.Row@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(mySubTypes);
    phoneNumberDetail.setNumber("+3585012345");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    nameDetail.setFirstName("Natalie");
    nameDetail.setLastName("Watson");
    testContact.saveDetail(&nameDetail);
    emailDetail.setEmailAddress("Natalie.Watson@ovi.com");
    emailDetail.setContexts(QContactDetail::ContextHome);
    testContact.saveDetail(&emailDetail);
    phoneNumberDetail.setSubTypes(mySubTypes);
    phoneNumberDetail.setNumber("+358507654321");
    testContact.saveDetail(&phoneNumberDetail);
    saveList << testContact;

    csr.setManager(QContactManager::fromUri(cm.managerUri()));
    qRegisterMetaType<QContactSaveRequest*>("QContactSaveRequest*");
    //csr.setContact(testContact);
    csr.setContacts(saveList);
    csr.start();
    csr.waitForFinished();
    csr.isFinished();

    QList<QContact> expected = csr.contacts();
    return true;
}





bool QContactJsonDbBackup::backupJsonDb() {
    QString query = "[?_type=\"com.nokia.mt.contacts.Contact\"]";
    QList<QJsonObject> map = m_dbClient->query(query);
    m_backupData = map;
    return true;
}





bool QContactJsonDbBackup::clearJsonDb() {
    return doRequest(m_backupData, false);
}




bool QContactJsonDbBackup::revertJsonDb() {
    return doRequest(m_backupData, true);
}


bool QContactJsonDbBackup::cleanJsonDb()
{
    QString query = "[?_type=\"com.nokia.mt.contacts.Contact\"]";
    QList<QJsonObject> map = m_dbClient->query(query);
    return doRequest(map, false);
}


bool QContactJsonDbBackup::doRequest(const QList<QJsonObject>  &objects, bool isInsert) {
    QString query;
    QList<QJsonObject> map;
    for(int i = 0; i < objects.size(); ++i) {
        QJsonObject item;
        item = objects[i];
        if(isInsert) {
            map = m_dbClient->create(item);
        } else {
            map = m_dbClient->remove(item);
        }
    }
    return true;
}


int QContactJsonDbBackup::wasteSomeTime() {
    int x=0;
    int y=0;
    for (int i=0; i<30; i++)
    {
        for (int j=0; j<100000000; j++)
        {
            y=(j*j);
        }
        x=i+y;
    }
    return x;
}
