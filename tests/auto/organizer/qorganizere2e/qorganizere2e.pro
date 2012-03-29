include(../../auto.pri)

QT += organizer organizer-private

contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES  += tst_qorganizere2e.cpp ../../jsondbprocess.cpp
HEADERS += ../../jsondbprocess.h
