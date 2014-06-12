include(../../../auto.pri)

TARGET = tst_qcontactasync

QT += contacts

SOURCES += tst_qcontactasync.cpp
HEADERS += ../../qcontactmanagerdataholder.h
INCLUDEPATH += ../..
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
