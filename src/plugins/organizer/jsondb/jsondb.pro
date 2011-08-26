QT += organizer organizer-private network jsondb jsondb-private
TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(qtorganizer_jsondb)
DESTDIR = $$QT.organizer.plugins/organizer

HEADERS += \
    qorganizerjsondbengine.h \
    qorganizerjsondbrequestthread.h \
    qorganizerjsondbenginefactory.h \
    qorganizerjsondbid.h \
    qorganizerjsondbrequestmanager.h \
    qorganizerjsondbstring_p.h

SOURCES += \
    qorganizerjsondbengine.cpp \
    qorganizerjsondbrequestthread.cpp \
    qorganizerjsondbenginefactory.cpp \
    qorganizerjsondbid.cpp \
    qorganizerjsondbrequestmanager.cpp
