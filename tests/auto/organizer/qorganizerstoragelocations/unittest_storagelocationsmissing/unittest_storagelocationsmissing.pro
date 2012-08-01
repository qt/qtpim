include(../../../auto.pri)

QT += organizer

contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES += tst_qorganizerstoragelocationsmissing.cpp
HEADERS += ../../../jsondbprocess.h

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
