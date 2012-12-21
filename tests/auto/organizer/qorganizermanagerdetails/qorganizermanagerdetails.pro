include(../../auto.pri)

QT += organizer

qtHaveModule(jsondb): QT += jsondb

SOURCES  += tst_qorganizermanagerdetails.cpp
HEADERS += ../qorganizermanagerdataholder.h ../../jsondbprocess.h
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
