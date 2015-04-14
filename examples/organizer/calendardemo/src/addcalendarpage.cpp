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
#include "addcalendarpage.h"

#include <QtWidgets>
#include <QtOrganizer/qorganizer.h>

QTORGANIZER_USE_NAMESPACE

AddCalendarPage::AddCalendarPage(QWidget *parent)
    :QWidget(parent),
    m_manager(0)
{
    QHBoxLayout* hbLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("Save", this);
    connect(okButton,SIGNAL(clicked()),this,SLOT(saveClicked()));
    hbLayout->addWidget(okButton);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));
    hbLayout->addWidget(cancelButton);

    QVBoxLayout *scrollAreaLayout = new QVBoxLayout();
    scrollAreaLayout->addStretch();
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

AddCalendarPage::~AddCalendarPage()
{

}

void AddCalendarPage::cancelClicked()
{
    emit showPreviousPage();
}

void AddCalendarPage::saveClicked()
{
    m_manager->saveCollection(&m_collection);
    if (m_manager->error())
        QMessageBox::warning(this, "Failed!", QString("Failed to save calendar!\n(error code %1)").arg(m_manager->error()));
    else
        emit showPreviousPage();
}

void AddCalendarPage::calendarChanged(QOrganizerManager *manager, QOrganizerCollection& calendar)
{
    m_manager = manager;
    m_collection = calendar;
    window()->setWindowTitle(!calendar.id().isNull() ? "Edit calendar" : "Add calendar");
}
