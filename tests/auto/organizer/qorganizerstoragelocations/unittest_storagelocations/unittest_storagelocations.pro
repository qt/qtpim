include(../../../auto.pri)

QT += organizer

contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES += tst_qorganizerstoragelocations.cpp
SOURCES += ../../../jsondbprocess.cpp
HEADERS += ../../../jsondbprocess.h

