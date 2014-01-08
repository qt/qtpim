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

#ifndef TST_MISSINGSTORAGE_H
#define TST_MISSINGSTORAGE_H

#include <QtTest/QtTest>
#include <QtGlobal>

#include <QtContacts/QContact>
#include <QtContacts/QContactAbstractRequest>
#include <QtContacts/QContactFetchRequest>
#include <QtContacts/QContactFetchByIdRequest>
#include <QtContacts/QContactIdFetchRequest>
#include <QtContacts/QContactRemoveRequest>
#include <QtContacts/QContactSaveRequest>
#include <QtContacts/qcontactengineid.h>
#include "qcontactjsondbid.h"

#include "jsondbprocess.h"

QTCONTACTS_USE_NAMESPACE

class tst_StoragesLocationsMissing : public QObject
{
    Q_OBJECT

public:
    tst_StoragesLocationsMissing();
    virtual ~tst_StoragesLocationsMissing();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();

private slots:

    void fetch();
    void fetch_data() {fetchData();}

    void idFetch();
    void idFetch_data() {fetchData();}

    void fetchById();
    void fetchById_data() {idData();}

    void fetchByIds();
    void fetchByIds_data() {idsData();}

    void removeById();
    void removeById_data() {idData();}

    void removeByIds();
    void removeByIds_data() {idsData();}

    void save();
    void save_data() {saveData();}

private:
    void fetchData();
    void idData();
    void idsData();
    void saveData();

    QContactManager::Error errorFromManager(QContactAbstractRequest &request, QContactManager *cm);
    QList<QContact> makeTestContactsForSave();
    QContactManager *m_contactManager;

    JsonDbProcess m_jsondbProcess;
};

#endif
