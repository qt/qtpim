include(../../../auto.pri)

TARGET = tst_qcontactasync

QT += contacts
contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES += tst_qcontactasync.cpp
HEADERS += ../../qcontactmanagerdataholder.h
HEADERS += ../../../jsondbprocess.h
INCLUDEPATH += ../..
INCLUDEPATH += ../../..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
