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

#ifndef DAYPAGE_H_
#define DAYPAGE_H_

#include <QWidget>
#include <QDate>
#include <QtOrganizer/qorganizeritemid.h>

QT_BEGIN_NAMESPACE_ORGANIZER
class QOrganizerManager;
class QOrganizerItem;
QT_END_NAMESPACE_ORGANIZER

QTORGANIZER_USE_NAMESPACE

QT_BEGIN_NAMESPACE
class QMainWindow;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QMenuBar;
class QMenu;
QT_END_NAMESPACE

class DayPage : public QWidget
{
    Q_OBJECT

public:
    DayPage(QWidget *parent = Q_NULLPTR);
    ~DayPage();

public Q_SLOTS:
    void refresh();
    void changeManager(QOrganizerManager *manager);
    void dayChanged(QDate date);
    void editItem();
    void removeItem();

private Q_SLOTS:
    void itemDoubleClicked(QListWidgetItem *listItem);
    void viewMonthClicked();

Q_SIGNALS:
    void showMonthPage();
    void showEditPage(const QOrganizerItem &item);
    void addNewEvent();
    void addNewTodo();
    void addNewJournal();

protected: // from QWidget
    void showEvent(QShowEvent *event);

private:
    QOrganizerManager *m_manager;
    QDate m_day;
    QLabel *m_dateLabel;
    QListWidget *m_itemList;
    QMenuBar *m_menuBar;
};

#endif // DAYPAGE_H_
