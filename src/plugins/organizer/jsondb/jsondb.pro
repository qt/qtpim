QT += organizer network jsondb

TARGET = qtorganizer_jsondb

PLUGIN_TYPE = organizer
load(qt_plugin)

HEADERS += \
    qorganizerjsondbengine.h \
    qorganizerjsondbrequestthread.h \
    qorganizerjsondbenginefactory.h \
    qorganizerjsondbid.h \
    qorganizerjsondbrequestmanager.h \
    qorganizerjsondbstring.h \
    qorganizerjsondbconverter.h \
    qorganizerjsondbdatastorage.h

SOURCES += \
    qorganizerjsondbengine.cpp \
    qorganizerjsondbrequestthread.cpp \
    qorganizerjsondbenginefactory.cpp \
    qorganizerjsondbid.cpp \
    qorganizerjsondbrequestmanager.cpp \
    qorganizerjsondbstring.cpp \
    qorganizerjsondbconverter.cpp \
    qorganizerjsondbdatastorage.cpp

OTHER_FILES += jsondb.json
