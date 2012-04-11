include(../../../auto.pri)

QT += organizer

contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES += tst_qorganizerstoragelocations.cpp
HEADERS += ../../../jsondbprocess.h

