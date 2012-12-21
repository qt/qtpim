include(../../auto.pri)

QT += organizer organizer-private

qtHaveModule(jsondb): QT += jsondb

SOURCES  += tst_qorganizere2e.cpp
HEADERS += ../../jsondbprocess.h
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
