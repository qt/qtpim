/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
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

#include <QList>
#include "qcontactjsondbbackup.h"
#include "qcontactjsondbconverter.h"
#include "qcontactjsondbstring.h"

QT_BEGIN_NAMESPACE_CONTACTS

QContactJsonDbBackup::QContactJsonDbBackup()
{
    m_dbClient = new SynchronizedJsonDbClient();
    backupJsonDb();
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

    csr.setManager(&cm);
    csr.setContacts(saveList);
    csr.start();
    if (!csr.waitForFinished())
        return false;
    if (csr.error() != QContactManager::NoError)
        return false;

    return true;
}




void QContactJsonDbBackup::backupJsonDb()
{
    // Backup partitions for testing.
    QString query = "[?_type=\"com.nokia.mt.contacts.Contact\"]";
    m_backupDataSystem = m_dbClient->query(query, QContactJsonDbStr::systemPartition());
    m_backupDataUser = m_dbClient->query(query, QContactJsonDbStr::userDataPartition());
}


bool QContactJsonDbBackup::revertJsonDb()
{
    bool requestStatus = cleanJsonDb();
    requestStatus = requestStatus && addContacts(m_backupDataSystem, QContactJsonDbStr::systemPartition());
    requestStatus = requestStatus && addContacts(m_backupDataUser, QContactJsonDbStr::userDataPartition());
    m_backupDataSystem.clear();
    m_backupDataUser.clear();
    return requestStatus;
}


bool QContactJsonDbBackup::cleanJsonDb()
{
    QString query = "[?_type=\"com.nokia.mt.contacts.Contact\"]";

    QList<QJsonObject> map = m_dbClient->query(query, QContactJsonDbStr::systemPartition());
    bool requestStatus = deleteContacts(map, QContactJsonDbStr::systemPartition());
    map = m_dbClient->query(query, QContactJsonDbStr::userDataPartition());
    requestStatus = requestStatus &&  deleteContacts(map, QContactJsonDbStr::userDataPartition());
    return requestStatus;
}


bool QContactJsonDbBackup::addContacts(const QList<QJsonObject>  &objects, QString partition) {

    QList<QJsonObject> map;
    for (int i = 0; i < objects.size(); ++i) {
        QJsonObject item;
        item = objects[i];
        map = m_dbClient->create(item, partition);
    }
    return true;
}


bool QContactJsonDbBackup::deleteContacts(const QList<QJsonObject>  &objects, QString partition) {

    QList<QJsonObject> map;
    for(int i = 0; i < objects.size(); ++i) {
        QJsonObject item;
        item = objects[i];
        map = m_dbClient->remove(item, partition);
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

QT_END_NAMESPACE_CONTACTS
