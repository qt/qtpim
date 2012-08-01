include(../../../auto.pri)

TARGET = tst_qorganizeritemasync

QT += organizer

contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES += tst_qorganizeritemasync.cpp
HEADERS += ../../qorganizermanagerdataholder.h ../../../jsondbprocess.h
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
