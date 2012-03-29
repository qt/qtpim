include(../../auto.pri)

QT += organizer organizer-private

contains(QT_CONFIG, jsondb): QT += jsondb

SOURCES  += tst_qorganizermanager.cpp ../../jsondbprocess.cpp
HEADERS += ../qorganizermanagerdataholder.h ../../jsondbprocess.h
