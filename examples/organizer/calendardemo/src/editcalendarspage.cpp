/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "editcalendarspage.h"

#include "calendardemo.h"
#include <QtWidgets>
#include <QtOrganizer/qorganizer.h>

QTORGANIZER_USE_NAMESPACE

Q_DECLARE_METATYPE(QOrganizerCollection)

EditCalendarsPage::EditCalendarsPage(QWidget *parent)
    :QWidget(parent),
    m_manager(0)
{
    m_calendarList = new QListWidget(this);
    connect(m_calendarList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemDoubleClicked(QListWidgetItem *)));

    // Add push buttons
    QHBoxLayout* hbLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("Add new", this);
    connect(addButton,SIGNAL(clicked()), this, SIGNAL(addClicked()));
    hbLayout->addWidget(addButton);
    QPushButton *editButton = new QPushButton("Edit", this);
    connect(editButton,SIGNAL(clicked()),this,SLOT(editClicked()));
    hbLayout->addWidget(editButton);
    QPushButton *deleteButton = new QPushButton("Delete", this);
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteClicked()));
    hbLayout->addWidget(deleteButton);
    QPushButton *backButton = new QPushButton("Back", this);
    connect(backButton,SIGNAL(clicked()),this,SLOT(backClicked()));
    hbLayout->addWidget(backButton);

    QVBoxLayout *scrollAreaLayout = new QVBoxLayout();
    scrollAreaLayout->addWidget(m_calendarList);
    scrollAreaLayout->addLayout(hbLayout);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget *formContainer = new QWidget(scrollArea);
    formContainer->setLayout(scrollAreaLayout);
    scrollArea->setWidget(formContainer);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(scrollArea);
    setLayout(mainLayout);
}

EditCalendarsPage::~EditCalendarsPage()
{

}

void EditCalendarsPage::editClicked()
{
    if (m_calendarList->currentItem())
        itemDoubleClicked(m_calendarList->currentItem());
}

void EditCalendarsPage::deleteClicked()
{
    if (!m_calendarList->currentItem())
        return;

    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete this calendar?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes) {
        m_collection = m_calendarList->currentItem()->data(ORGANIZER_CALENDAR_ROLE).value<QOrganizerCollection>();
        if(!m_manager->removeCollection(m_collection.id()))
            QMessageBox::information(this, "Failed!", QString("Failed to remove calendar!\n(error code %1)").arg(m_manager->error()));
        showPage(m_manager);
    }
}

void EditCalendarsPage::backClicked()
{
    emit showPreviousPage();
}

void EditCalendarsPage::itemDoubleClicked(QListWidgetItem *listItem)
{
    if (!listItem)
        return;

    m_collection = listItem->data(ORGANIZER_CALENDAR_ROLE).value<QOrganizerCollection>();
    emit showAddCalendarPage(m_manager, &m_collection);
}

void EditCalendarsPage::showPage(QOrganizerManager *manager)
{
    m_manager = manager;
    m_calendarList->clear();
    QList<QOrganizerCollection> collections = manager->collections();
    int index = 0;
    foreach(QOrganizerCollection collection, collections) {
        QString visibleName;
        if (collection.metaData(QOrganizerCollection::KeyName).canConvert(QVariant::String))
            visibleName = collection.metaData(QOrganizerCollection::KeyName).toString();
        else
            // We currently have no way of stringifying ids
            // visibleName = "Calendar id " + QString::number(collection.id().localId());
            visibleName = "Calendar " + QString::number(index++);

        QListWidgetItem* listItem = new QListWidgetItem();
        listItem->setText(visibleName);
        QVariant data = QVariant::fromValue<QOrganizerCollection>(collection);
        listItem->setData(ORGANIZER_CALENDAR_ROLE, data);
        m_calendarList->addItem(listItem);
    }
}

void EditCalendarsPage::showEvent(QShowEvent *event)
{
    window()->setWindowTitle("Edit calendars");
    QWidget::showEvent(event);
}
