include(../../auto.pri)

QT += organizer organizer-private

qtHaveModule(jsondb): QT += jsondb

SOURCES  += tst_qorganizermanager.cpp
HEADERS += ../qorganizermanagerdataholder.h ../../jsondbprocess.h
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
